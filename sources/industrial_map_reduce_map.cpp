// Copyright 2022 howardano

#include <industrial_map_reduce.hpp>
#include <cmath>

namespace IndustrialRise {

void IndustrialMapReduce::SetMapper(IMapper &mapper_) { mapper = &mapper_; }

void IndustrialMapReduce::InitializePostMapper() {
  post_mapper.resize(split_count);
  for (int i = 0; i < split_count; ++i) {
    post_mapper[i].resize(num_reducers);
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
  // post_mapper.push_back(post_mapper_element);

  std::sort(post_mapper_element.begin(), post_mapper_element.end(), [](auto &left, auto &right) {
    return left.first.compare(right.first) < 0;
  });

  std::vector<std::vector<char>> Chars;
  std::cout << "reducers: " << num_reducers << "\n";
  std::vector<int> numOfChars(num_reducers, alphabet.size() / num_reducers);

  for (int i = 0; i < alphabet.size() % num_reducers; ++i) {
    numOfChars[i] += 1;
  }
  
  int l = 0;

  for (int i = 0; i < numOfChars.size(); ++i) {
    std::vector<char> buf;
    for (int j = 0; j < numOfChars[i]; ++j) {
      buf.push_back(alphabet[l]);
      l++;
    }
    Chars.push_back(buf);
  }

  int k = 0;
  int reducer = 0;
 
  for (int i = 0; i < Chars.size(); ++i) {
    while (k < post_mapper_element.size() && std::count(alphabet.begin(), alphabet.end(), post_mapper_element[k].first[0]) == 0) {
      k++;
    }
    std::vector<std::pair<std::string, std::string>> buf;
    
    while(k < post_mapper_element.size() && std::count(Chars[i].begin(), Chars[i].end(), post_mapper_element[k].first[0]) != 0) {
      buf.push_back(post_mapper_element[k]);
      k++;
    }
    post_mapper[file_num][reducer].insert(post_mapper[file_num][reducer].end(), buf.begin(), buf.end());
    reducer++;
  }
  std::cout << "Done" << std::endl;
}


} // namespace IndustrialRise