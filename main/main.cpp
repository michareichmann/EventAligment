#include "EventAlignment.hh"
#include <iostream>
#include "Utils.hh"

using namespace std;

int main(int argc, char* argv[]) {

  if (argc < 2) {
    cout << "You have to give a tree to handle!" << endl;
    return -1;
  }
  print_banner("STARTING EVENT ALIGNMENT");
  string file_name = argv[1];
  uint32_t max_event_num = 0;
  if (argc > 3) max_event_num = uint32_t(atoi(argv[2]));

  EventAlignment * EA;
  EA = new EventAlignment(file_name, max_event_num);
  EA->event_loop();
  EA->save_tree();
  EA->print_results();

  return 0;
}