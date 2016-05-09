//
// Created by micha on 04.05.16.
//

#ifndef EVENTALIGNMENT_EVENTALIGNMENT_H
#define EVENTALIGNMENT_EVENTALIGNMENT_H

#include <iostream>

#include <string>
#include "TTree.h"
#include "TFile.h"
#include <vector>

class EventAlignment{

public:
  EventAlignment(std::string file_name);
  EventAlignment();
  ~EventAlignment();
  void print_branches();
  bool open_file();

private:
  TFile * InFile;
  TTree * InTree;
  std::string fFileName;

};

#endif //EVENTALIGNMENT_EVENTALIGNMENT_H
