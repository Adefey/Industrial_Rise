// Copyright 2022 nokrolikno

#include <industrial_map_reduce.hpp>

namespace IndustrialRise {

void IndustrialMapReduce::SetReducer(IReducer &reducer_) {
  reducer = &reducer_;
}

void IndustrialMapReduce::Shuffle() {
  std::vector<std::unordered_set<std::string>> keys;
  for (size_t i = 0; i < num_reducers; ++i) {
    std::unordered_set<std::string> k;
    keys.push_back(k);
    std::vector<std::pair<std::string, std::vector<std::string>>> new_vec = {};
    pred_reducer.push_back(new_vec);
  }
  size_t cur_new_key = 0;
  for (size_t mapper_i = 0; mapper_i < split_count; ++mapper_i) {
    std::cout << "process mapper " << mapper_i << std::endl;
    for (size_t mapper_j = 0; mapper_j < post_mapper[mapper_i].size();
         ++mapper_j) {
      bool elem_in_reducers = false;
      for (size_t reducer_i = 0; reducer_i < num_reducers; ++reducer_i) {
        if (keys[reducer_i].contains(post_mapper[mapper_i][mapper_j].first)) {
          for (size_t reducer_j = 0; reducer_j < pred_reducer[reducer_i].size();
               ++reducer_j) {
            if (pred_reducer[reducer_i][reducer_j].first ==
                post_mapper[mapper_i][mapper_j].first) {
              pred_reducer[reducer_i][reducer_j].second.push_back(
                  post_mapper[mapper_i][mapper_j].second);
              break;
            }
          }
          elem_in_reducers = true;
        }
      }
      if (!elem_in_reducers) {
        std::pair<std::string, std::vector<std::string>> new_pair;
        new_pair.first = post_mapper[mapper_i][mapper_j].first;
        std::vector<std::string> new_vector;
        new_pair.second = new_vector;
        new_vector.push_back(post_mapper[mapper_i][mapper_j].second);
        if (pred_reducer[cur_new_key].size() == 0) {
          pred_reducer[cur_new_key].push_back(new_pair);
        } else {
          auto it = pred_reducer[cur_new_key].begin();
          while (it != pred_reducer[cur_new_key].end() &&
                 post_mapper[mapper_i][mapper_j].first.compare((*it).first) >
                     0) {
            it++;
          }
          pred_reducer[cur_new_key].insert(it, new_pair);
        }
        keys[cur_new_key].insert(post_mapper[mapper_i][mapper_j].first);
        cur_new_key = (cur_new_key + 1) % num_reducers;
      }
    }
    std::cout << "Done" << std::endl;
  }
}

void IndustrialMapReduce::Reduce(const size_t reducer_num) {
  auto result = (*reducer)(pred_reducer[reducer_num]);
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
