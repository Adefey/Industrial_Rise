// Copyright 2022 howardano

#include <industrial_map_reduce.hpp>

namespace IndustrialRise {

void IndustrialMapReduce::SetMapper(const IMapper &mapper_) {
  mapper = mapper_;
}

void IndustrialMapReduce::Map(const size_t file_num) {
  std::ifstream in(tmp_dir + split_prefix + std::to_string(file_num));

  std::string res = "";

  std::string line = "";

  while (std::getline(in, line)) {
    std::istringstream iss(line);
    res += line + " ";
  }

  std::vector<std::pair<std::string, std::string>> post_mapper_element =
      mapper(res);
  post_mapper[file_num] = post_mapper_element;
}
} // namespace IndustrialRise
