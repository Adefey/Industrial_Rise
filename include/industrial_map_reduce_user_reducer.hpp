// Copyright 2022 nokrolikno

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

struct UserReducer : public IndustrialRise::IReducer {
  std::vector<std::pair<std::string, std::string>> operator()(
      std::vector<std::pair<std::string, std::vector<std::string>>> input)
      override {
    std::vector<std::pair<std::string, std::string>> result;
    for (size_t i = 0; i < input.size(); ++i) {
      std::pair<std::string, std::string> new_pair;
      new_pair.first = input[i].first;
      int sum = 0;
      for (size_t j = 0; j < input[i].second.size(); ++j) {
        sum += std::stoi(input[i].second[j]);
      }
      new_pair.second = std::to_string(sum);
      result.push_back(new_pair);
    }
    return result;
  }
};
