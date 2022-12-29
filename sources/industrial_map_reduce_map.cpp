// Copyright 2022 howardano

#include <industrial_map_reduce.hpp>

namespace IndustrialRise {

void IndustrialMapReduce::SetMapper(const IMapper &mapper_) {
  mapper = mapper_;
}

void IndustrialMapReduce::Map(const size_t file_num) {
  std::ifstream in(tmp_dir + split_prefix + std::to_string(file_num));
  std::cout << "Read file" << std::endl;
  std::string res = "";

  std::string line = "";

  while (std::getline(in, line)) {
    std::istringstream iss(line);
    res += line + " ";
  }

  std::vector<std::pair<std::string, std::string>> post_mapper_element =
      mapper(res);
  std::lock_guard<std::mutex> lg(m);
  post_mapper.push_back(post_mapper_element);
  std::cout << "Result set to global storage; Now its size: "
            << post_mapper.size() << std::endl;
}

} // namespace IndustrialRise
