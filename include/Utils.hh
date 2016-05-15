//
// Created by micha on 10.05.16.
//

#ifndef EVENTALIGNMENT_UTILS_HH
#define EVENTALIGNMENT_UTILS_HH

#include <vector>
#include <string>

std::string trim(const std::string & s, std::string trim_characters="\t\n\r\v ");
std::vector<std::string> split(const std::string & str, const std::string & delim = "\t", bool dotrim=true);
void print_banner(std::string message, const char seperator = '=', uint16_t max_length = 100);

template <typename T>
uint32_t find_min_index(std::vector<T> vec){

  uint32_t min_ind = uint32_t(vec.size() - 1);
  uint16_t min = vec.front();
  for (uint32_t i = 1; i < vec.size(); i++){
    if (vec.at(i) < min) min_ind = i;
    min = vec.at(i);
  }
  return min_ind;
}

#endif //EVENTALIGNMENT_UTILS_HH
