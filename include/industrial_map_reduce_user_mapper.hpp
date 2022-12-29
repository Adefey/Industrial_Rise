// Copyright 2022 howardano

#include <industrial_map_reduce.hpp>

#include <algorithm>
#include <condition_variable>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <mutex>
#include <set>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_set>
#include <utility>
#include <vector>

struct UserMapper : public IndustrialRise::IMapper {
  std::string bad_pattern = "0-9.!?/@#()*+-";

  std::string removeSubstrs(std::string s, std::string &p) {
    std::string::size_type n = p.length();
    for (std::string::size_type i = s.find(p); i != std::string::npos;
         i = s.find(p))
      s.erase(i, n);
    return s;
  }

  std::vector<std::pair<std::string, std::string>>
  operator()(const std::string &str) override {
    // string str = "red tuna, blue tuna, black tuna, one tuna";
    // std::string pattern = "^0-9A-Za-zА-Яа-яёЁ.!?/@#()*+-";

    std::string formatted_string = removeSubstrs(str, bad_pattern);
    // cout << str << endl;
    std::transform(formatted_string.begin(), formatted_string.end(),
                   formatted_string.begin(),
                   [](unsigned char c) { return std::tolower(c); });

    std::vector<std::string> all_words;
    std::istringstream iss(formatted_string);
    std::string word;
    while (std::getline(iss, word, ' ')) {
      all_words.push_back(word);
    }

    std::set<std::string> unique_words;

    for (const auto &it : all_words) {
      unique_words.insert(it);
    }

    std::vector<std::pair<std::string, std::string>> result;

    int z = 0;
    for (const auto &unique_word : unique_words) {
      result.push_back(std::make_pair(unique_word, "0"));
      for (const auto &word : all_words) {
        if (unique_word == word) {
          result[z].second = std::to_string(std::stoi(result[z].second) + 1);
        }
      }
      z++;
    }

    return result;
  }
};
