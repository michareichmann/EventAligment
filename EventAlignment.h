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
  void print_branches();

private:
  TFile * InFile;
  TTree * InTree;

};

#endif //EVENTALIGNMENT_EVENTALIGNMENT_H
