// Copyright 2022 howardano

#include <industrial_map_reduce.hpp>
#include <cmath>

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
    return left.first.compare(right.first) < 0;
  });

  float alphSize = alphabet.size();
  float nr = num_reducers;
  int adder = std::ceil(alphSize / nr);
  int l = 0;
  std::vector<std::vector<char>> Chars;

  for (int i = 0; i < alphabet.size(); ++i) {
    std::vector<char> bufCh;
    for (int j = 0; j < adder; ++j) {
      if (i >= alphabet.size()) {
        break;
      }
      bufCh.push_back(alphabet[i]);
      i++;
    }
    Chars.push_back(bufCh);
  }

  int k = 0;
  int reducer = 0;

  for (int i = 0; i < Chars.size(); ++i) {
    std::vector<std::pair<std::string, std::string>> buf;
    while(std::find(Chars[i].begin(), Chars[i].end(), post_mapper_element[k].first[0]) != Chars[i].end()) {
      buf.push_back(post_mapper_element[k]);
      k++;
    }
    post_mapper[file_num][reducer].insert(post_mapper[file_num][reducer].end(), buf.begin(), buf.end());
    reducer++;
  }

}

} // namespace IndustrialRise
