//
// Created by micha on 04.05.16.
//

#include "EventAlignment.hh"
#include <TROOT.h>

using namespace std;

EventAlignment::EventAlignment(string file_name) : fFileName(file_name) {
  InFile = new TFile(file_name.c_str());
  InTree = (TTree*)InFile->Get("tree");
}

EventAlignment::~EventAlignment() {
  delete InTree;
  delete InFile;
}

void EventAlignment::print_branches() {
  InTree->GetListOfBranches()->Print();
}

bool EventAlignment::open_file() {

  cout << fFileName << endl;
  InFile = new TFile(fFileName.c_str());

  if (!InFile->IsOpen()) return false;

  InTree = (TTree*)InFile->Get("tree");
  return true;
}