// Copyright 2022 nokrolikno

#include <industrial_map_reduce.hpp>
#include <queue>

namespace IndustrialRise {

void IndustrialMapReduce::SetReducer(IReducer &reducer_) {
  reducer = &reducer_;
}

void IndustrialMapReduce::Reduce(const size_t reducer_num) {
  std::priority_queue<std::pair<std::pair<std::string, std::string>, int>, std::vector<std::pair<std::pair<std::string, std::string>, int>>, PairCompare> min_heap;
  std::vector<std::pair<std::string, std::vector<std::string>>> pred_reducer;
  std::string last_key = "";
  std::vector<size_t> pointers(num_mappers, 0);
  for (size_t mapper = 0; mapper < num_mappers; ++mapper) {
    std::pair<std::pair<std::string, std::string>, int> new_pair;
    new_pair.first = post_mapper[mapper][reducer_num][0];
    new_pair.second = mapper;
    min_heap.push(new_pair);
  }
  while (!min_heap.empty()) {
    tmp_pair = min_heap.top();
    min_heap.pop();

    pointers[tmp_pair.second] += 1;
    if (pointers[tmp_pair.second] < post_mapper[tmp_pair][reducer_num].size()) {
      std::pair<std::pair<std::string, std::string>, int> new_pair;
      new_pair.first = post_mapper[mapper][reducer_num][pointers[tmp_pair.second]];
      new_pair.second = tmp_pair.second;
      min_heap.push(new_pair);
    }

    if (tmp_pair.first.first == last_key) {
      pred_reducer[pred_reducer.size() - 1].second.push_back(tmp_pair.first.second);
    } else {
      std::vector<std::string> new_vec;
      new_vec.push_back(tmp_pair.first.second);
      std::pair<std::string, std::vector<std::string>> new_pair;
      new_pair.first = tmp_pair.first.first;
      new_pair.second = new_vec;
      pred_reducer.push_back(new_pair);
      last_key = tmp_pair.first.first;
    }
  }

  auto result = (*reducer)(pred_reducer);
  std::ofstream out;
  out.open(output_dir + reduced_prefix + std::to_string(reducer_num));
  if (out.is_open()) {
    for (auto const &elem : result) {
      out << elem.first << ": " << elem.second << std::endl;
    }
    out.close();
    std::cout << "Reduced file #" << reducer_num << std::endl;
  } // else throw
}

} // namespace IndustrialRise
