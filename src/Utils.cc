//
// Created by micha on 10.05.16.
//

#include "Utils.hh"
#include <iostream>
#include <sstream>

using namespace std;

std::vector<std::string> split(const std::string & str, const std::string & delim, bool dotrim) {

  std::string s(str);
  std::vector<std::string> result;
  if (str == "") return result;
  size_t i;
  while ((i = s.find_first_of(delim)) != std::string::npos) {
    result.push_back(dotrim ? trim(s.substr(0, i)) : s.substr(0, i));
    s = s.substr(i + 1);
  }
  result.push_back(s);
  return result;
}

std::string trim(const std::string & s, std::string trim_characters) {

  size_t b = s.find_first_not_of(trim_characters);
  size_t e = s.find_last_not_of(trim_characters);
  if (b == std::string::npos || e == std::string::npos) {
    return "";
  }
  return std::string(s, b, e - b + 1);
}

void print_banner(std::string message, const char seperator, uint16_t max_length) {
  
  uint16_t mes_length = 0;
  vector<string> split_msg = split(message, "\n");;
  stringstream ss;
  for (vector<string>::iterator it = split_msg.begin(); it != split_msg.end(); it++)
  if (it->size() > mes_length) mes_length = uint16_t(it->size());
  if (mes_length > max_length) mes_length = max_length;
  for (vector<string>::iterator it = split_msg.begin(); it != split_msg.end(); it++){
    it->resize(mes_length);
    ss << it->c_str() << "\n";
  }
  string send_msg = trim(ss.str(), "\n");
  string banner = string(mes_length, seperator);
  cout << "\n" << banner << "\n" << send_msg << "\n" << banner << "\n" << endl;
}
