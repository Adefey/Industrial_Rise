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
  std::string bad_pattern = "^0-9.!$?/@#()*+-";

  std::string removeSubstrs(std::string str, std::string &charsToRemove) {
    for (size_t i = 0; i < charsToRemove.size(); ++i) {
      str.erase(std::remove(str.begin(), str.end(), charsToRemove[i]),
                str.end());
    }
    return str;
  }

  std::vector<std::pair<std::string, std::string>>
  operator()(const std::string &str) override {
    std::string formatted_string = removeSubstrs(str, bad_pattern);
    std::transform(formatted_string.begin(), formatted_string.end(),
                   formatted_string.begin(),
                   [](unsigned char c) { return std::tolower(c); });

    std::vector<std::string> all_words;
    std::istringstream iss(formatted_string);
    std::string word;
    while (std::getline(iss, word, ' ')) {
      all_words.push_back(word);
    }

    std::set<std::string> unique_words = {};

    for (const auto &it : all_words) {
      unique_words.insert(it);
    }

    std::vector<std::pair<std::string, std::string>> result = {};

    int z = 0;
    for (const auto &unique_word : unique_words) {
      result.push_back(std::make_pair(unique_word, "0"));
      for (const auto &word_ : all_words) {
        if (unique_word == word_) {
          result[z].second = std::to_string(std::stoi(result[z].second) + 1);
        }
      }
      z++;
    }

    return result;
  }
};
