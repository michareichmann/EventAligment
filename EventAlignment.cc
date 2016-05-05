//
// Created by micha on 04.05.16.
//

#include "EventAlignment.h"

using namespace std;

EventAlignment::EventAlignment(string file_name) {
  InFile = new TFile(file_name.c_str(), "READ");
  InTree = (TTree*)InFile->Get("tree");
}

void EventAlignment::print_branches() {
  InTree->GetListOfBranches()->Print();
}