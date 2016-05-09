#include "EventAlignment.hh"

using namespace std;

int main(int argc, char* argv[]) {

  if (argc < 2) {
    cout << "You have to give a tree to handle!" << endl;
    return -1;
  }
  string bla = "Hallo";
  cout << bla << endl;
  string file_name = argv[1];
  cout << "TEST2" << endl;
  EventAlignment * EA;
  EA = new EventAlignment(file_name);
  EA->print_branches();
  return 0;
}