#include "EventAlignment.h"

using namespace std;

int main(int argc, char* argv[]) {

  string file_name = argv[1];
  EventAlignment * EA;
  EA = new EventAlignment(file_name);
  EA->print_branches();
  return 0;
}