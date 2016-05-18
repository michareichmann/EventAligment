//
// Created by micha on 04.05.16.
//

#include "EventAlignment.hh"
#include <TROOT.h>
#include "Utils.hh"
#include "TTree.h"
#include "TFile.h"
#include "TMacro.h"
#include <iostream>
#include "TSystem.h"
#include <algorithm>
#include <sstream>
#include <iomanip>

using namespace std;

EventAlignment::EventAlignment(string file_name, uint32_t max_event_num) : fFilePath(file_name), fMaxEventNumber(max_event_num), fNAveragedEvents(100), fNLastEvents(5),
                                                                           fNPulserEvents(0), fOffset(0), fFoundOffset(false), fThreshold(float(.5 * fNAveragedEvents)),
                                                                           fNOffsets(0) {

  StartTime = clock();
  gSystem->Load("libTree.so");
  open_file();
  read_tree();
  Macro = (TMacro*)InFile->Get("region_information");
  if (!fMaxEventNumber) fMaxEventNumber = fNEntries;

  NewFile = new TFile("test", "RECREATE");
  NewTree = InTree->CloneTree(0);

  vTelescopeBranches = {"plane", "col", "row", "adc", "charge"};

  resize_vectors();

  fLastPulserEvents.resize(6, 0);
  fHitRatios.resize(11, 0);


}

EventAlignment::~EventAlignment() {

  delete InFile;
  delete NewFile;
}

void EventAlignment::open_file() {

  InFile = new TFile(fFilePath.c_str());
  string dir = "";
  for (auto iline:split(fFilePath, "/"))
    if (iline.find(".root") == string::npos)
      dir += iline + "/";
    else
      fFileName = iline;
  fTreeDir = dir;
  if (!InFile->IsOpen()) {
    cerr << "ERROR: cannot open input file: " << fFilePath << endl;
    throw;
  }
  else cout << "Successfully read file " << fFileName << " from " << fTreeDir << endl;
}

void EventAlignment::read_tree() {

  InTree = (TTree*)InFile->Get("tree");
  fAtEntry = 0;
  fNEntries = uint32_t(InTree->GetEntries());
  if (!fNEntries) {
    cerr << "ERROR: tree is empty!" << endl;
    throw;
  }
  InTree->SetBranchAddress("plane", &vPlane);
  InTree->SetBranchAddress("pulser", &fPulser);
  InTree->SetBranchAddress("col", &vCol);
  InTree->SetBranchAddress("row", &vRow);
  InTree->SetBranchAddress("adc", &vAdc);
  InTree->SetBranchAddress("charge", &vCharge);
  cout << "The tree has " << InTree->GetNbranches() << " branches and " << fNEntries << " entries!" << endl;
}

bool EventAlignment::get_next_event() {

  if (fAtEntry == fNEntries) return false;
  InTree->GetEntry(fAtEntry);
  fAtEntry++;
  return true;
}

void EventAlignment::resize_vectors() {

  fLastHits.resize(fNLastEvents, 0);

  fLastPlanes.resize(fNLastEvents);
  fLastCols.resize(fNLastEvents);
  fLastRows.resize(fNLastEvents);
  fLastAdcs.resize(fNLastEvents);
  fLastCharges.resize(fNLastEvents);

  fHitsBeforePulser.resize(fNLastEvents, 0);
}

// **********************************************
// EVENT LOOP
void EventAlignment::event_loop() {

  while (get_next_event()){

    // stop loop at given event
    if (fAtEntry == fMaxEventNumber){
      cout << endl;
      break;
    }

    print_progress();

    fill_temp_vectors();

    fill_offset_data();
    check_offset();

    if(fOffset < 0)
      correct_offset();

    fill_tree();
  }
}

void EventAlignment::fill_temp_vectors(){

  add_entry(fLastHits, vPlane->size());

  add_entry(fLastPlanes, vPlane);
  add_entry(fLastCols, vCol);
  add_entry(fLastRows, vRow);
  add_entry(fLastAdcs, vAdc);
  add_entry(fLastCharges, vCharge);
}

template <typename T, typename Q>
  void EventAlignment::add_entry(std::vector<T> & vec, Q val) {

    vec.push_back(val);
    vec.erase(vec.begin());
  }

template <typename T, typename Q>
  void EventAlignment::add_entry(vector<vector<T> > & vec, vector<Q> * val) {

    vec.push_back(*val);
    vec.erase(vec.begin());
  }

void EventAlignment::clear_offset_data() {

  fNPulserEvents = 0;
  fHitsBeforePulser.clear();
  fHitsBeforePulser.resize(fNLastEvents, 0);
}

void EventAlignment::fill_offset_data() {

  // clear the data if the number of averaging events is reached
  if (fNPulserEvents == fNAveragedEvents)
    clear_offset_data();

  if (fPulser){
    uint8_t i = 0;
    for (auto ihit:fLastHits){
      fHitsBeforePulser.at(i) += bool(ihit);
      i++;
    }
    fNPulserEvents++;
  }
}

void EventAlignment::check_offset() {

  if (fNPulserEvents == fNAveragedEvents){
    if (fHitsBeforePulser.back() > fThreshold){
      int16_t old_offset = fOffset;
      fOffset = find_offset();
      if (fOffset != old_offset and fOffset){
        fNOffsets++;
        cout << "Found event misalignment with offset: " << fOffset << endl;
      }
    }
  }
}

int16_t EventAlignment::find_offset() {

  uint32_t min_ind = find_min_index(fHitsBeforePulser);
  int16_t offset = int16_t(min_ind - fNLastEvents + 1);
  fFoundOffset = bool(offset);
  if (fFoundOffset and VERBOSE){
    for (auto i:fHitsBeforePulser) cout << float(i) / fNAveragedEvents << " ";
    cout << endl;
  }
  return offset;
}

void EventAlignment::correct_offset() {

  vPlane = &fLastPlanes.at(uint16_t(fNLastEvents - 1 + fOffset));
  vCol = &fLastCols.at(uint16_t(fNLastEvents - 1 + fOffset));
  vRow = &fLastRows.at(uint16_t(fNLastEvents - 1 + fOffset));
  vAdc = &fLastAdcs.at(uint16_t(fNLastEvents - 1 + fOffset));
  vCharge = &fLastCharges.at(uint16_t(fNLastEvents - 1 + fOffset));
}

void EventAlignment::fill_tree(){
  NewTree->Fill();
}

void EventAlignment::save_tree(){
  NewFile->cd();
  NewTree->Write();
  if (Macro) Macro->Write();
  NewFile->Write();
  NewFile->Close();
  delete NewFile;
}

void EventAlignment::print_results() {

  stringstream ss;
  ss << "FINISHED CONVERSION:\n";
  ss << "Time: " << setprecision(2) << fixed << get_time(StartTime) << " seconds\n";
  ss << "Successfully corrected for " << fNOffsets << " Offsets!";
  print_banner(ss.str());
}

float EventAlignment::get_time(const float time){

  return (clock() - time) / CLOCKS_PER_SEC;
}

void EventAlignment::print_progress(){

  stringstream ss;
  float prog = float(fAtEntry) / fMaxEventNumber * 100;
  if (fAtEntry % 100 == 1){
    if (fAtEntry > 1) cout << "\r";
    ss << "Progress: "  << setprecision(2) << setw(5) << setfill('0') << fixed << prog << "% ";
    ss << "|" << string(uint32_t(round(prog)) / 2, '=') << ">";
    ss << string(50 - uint32_t(round(prog)) / 2, ' ') << "| 100%    ";
    cout << ss.str() << flush;
    if (fAtEntry == fMaxEventNumber) cout << endl;
//    if (fMaxEventNumber - fAtEntry < 10) cout << "|" << string(50 , '=') << ">";
//    else cout << "|" <<string(uint32_t(float(fAtEntry) / fMaxEventNumber * 100) / 2, '=') << ">";
//    if (fMaxEventNumber - fAtEntry < 10) cout << "| 100%    ";
//    else cout << string(50 - uint32_t(float(fAtEntry) / fMaxEventNumber * 100) / 2, ' ') << "| 100%    ";
//    cout.flush();
//    if (fMaxEventNumber - fAtEntry < 10) cout << endl;
  }
}