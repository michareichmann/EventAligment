//
// Created by micha on 04.05.16.
//

#ifndef EVENTALIGNMENT_EVENTALIGNMENT_H
#define EVENTALIGNMENT_EVENTALIGNMENT_H

#include <string>
#include <vector>
#include "stdint.h"

#define VERBOSE 0

class TTree;
class TFile;
class TMacro;

class EventAlignment{

public:
  EventAlignment(std::string file_name, uint32_t max_event_num);
  ~EventAlignment();
  void open_file();
  void read_tree();
  bool get_next_event();
  void event_loop();

  void print_progress();
  void print_results();
  float StartTime;
  float get_time(const float);

  void resize_vectors();
  template <typename T, typename Q>
  void add_entry(std::vector<T>&, Q);
  template <typename T, typename Q>
  void add_entry(std::vector<std::vector<T> >&, std::vector<Q>*);
  void check_offset();
  void fill_offset_data();
  int16_t find_offset();
  void clear_offset_data();
  void correct_offset();
  void fill_temp_vectors();

  void save_tree();
  void fill_tree();

private:
  TFile * InFile;
  TFile * NewFile;
  TTree * InTree;
  TTree * NewTree;
  TMacro * Macro;

  std::string fFilePath;
  std::string fTreeDir;
  std::string fFileName;
  std::vector<std::string> vTelescopeBranches;

  // tree stuff
  uint32_t fNEntries;
  uint32_t fAtEntry;
  uint32_t fMaxEventNumber;

  // branches
  std::vector<uint16_t> * vPlane;
  std::vector<uint16_t> * vCol;
  int fPulser;
  std::vector<uint16_t> * vRow;
  std::vector<int16_t> * vAdc;
  std::vector<uint32_t> * vCharge;

  // check offset
  uint16_t fNAveragedEvents;
  uint16_t fNLastEvents;

  uint32_t fNPulserEvents;
  int16_t fOffset;
  bool fFoundOffset;
  float fThreshold;
  uint16_t fNOffsets;

  std::vector<uint16_t> fLastHits;
  std::vector<uint16_t> fHitsBeforePulser;
  std::vector<uint32_t> fLastPulserEvents;
  std::vector<float> fHitRatios;

  // tmp vectors
  std::vector<std::vector<uint16_t> > fLastPlanes;
  std::vector<std::vector<uint16_t> > fLastCols;
  std::vector<std::vector<uint16_t> > fLastRows;
  std::vector<std::vector<int16_t> > fLastAdcs;
  std::vector<std::vector<uint32_t> > fLastCharges;


};

#endif //EVENTALIGNMENT_EVENTALIGNMENT_H
