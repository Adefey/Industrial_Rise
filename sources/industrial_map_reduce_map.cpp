// Copyright 2022 howardano

#include <industrial_map_reduce.hpp>

namespace IndustrialRise {

void IndustrialMapReduce::SetMapper(IMapper &mapper_) { mapper = &mapper_; }

void IndustrialMapReduce::InitializePostMapper(size_t num_mappers, size_t num_reducers) {
  post_mapper.reserve(num_mappers);
  for (int i = 0; i < num_mappers; ++i) {
    post_mapper[i].reserve(num_reducers);
  }
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
      (*mapper)(res);
  //std::lock_guard<std::mutex> lg(m);
  /*post_mapper.push_back(post_mapper_element);
  std::cout << "Result set to global storage; Now its size: "
            << post_mapper.size() << std::endl;*/

  std::sort(post_mapper_element.begin(), post_mapper_element.end(), [](auto &left, auto &right) {
    return left.compare(right.compare);
  });

  int z = 0;
  int k = 0;

  for (size_t i = 0; i < post_mapper_element.size(); ++i) {
    std::vector<std::pair<std::string, std::string>> buf;
    for (int j = k; j < k + num_reducers; ++j) {
      buf.push_back(post_mapper_element[j]);
    }
    k += num_reducers;
    post_mapper[file_num][z] = buf;
    z++;
    if (z == num_reducers) {
      z = 0;
    }
  }
}

} // namespace IndustrialRise
