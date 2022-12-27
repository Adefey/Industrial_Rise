// Copyright 2022 Adefe

#include <industrial_map_reduce.hpp>
namespace IndustrialRise {

IndustrialMapReduce::IndustrialMapReduce()
    : buf_big_file_name("big_buf"), split_prefix("splitted_"),
      reduced_prefix("reduced_"), split_size(33554432) {}

void IndustrialMapReduce::Start() {
  FilesToBuf();
  SplitFiles();
  std::vector<std::thread> threads = {};
  for (size_t i = 0; i < split_count; ++i) {
    threads.emplace_back(std::thread(&IndustrialMapReduce::Map, this, i));
  }
  for (size_t i = 0; i < split_count; ++i) {
    threads[i].join();
  }
  Shuffle();
  threads.clear();
  for (size_t i = 0; i < num_reducers; ++i) {
    threads.emplace_back(std::thread(&IndustrialMapReduce::Reduce, this, i));
  }
  for (size_t i = 0; i < num_reducers; ++i) {
    threads[i].join();
  }
  ClearFiles();
}
} // namespace IndustrialRise
