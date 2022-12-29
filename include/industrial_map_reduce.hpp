// Copyright 2022 Adefe

#ifndef INDUSTRIAL_MAP_REDUCE_HPP_
#define INDUSTRIAL_MAP_REDUCE_HPP_

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

namespace IndustrialRise {

struct IMapper {
  virtual std::vector<std::pair<std::string, std::string>>
  operator()(const std::string &str) {
    return {};
  }
};

struct IReducer {
  virtual std::vector<std::pair<std::string, std::string>>
  operator()(std::vector<std::pair<std::string, std::vector<std::string>>>) {
    return {};
  }
};

class IndustrialMapReduce {
private:
  const std::string buf_big_file_name;
  const std::string split_prefix;
  const std::string reduced_prefix;
  const size_t split_size;
  size_t split_count;

  size_t num_mappers;
  size_t num_reducers;

  std::vector<std::string> input_files;

  std::string tmp_dir;

  std::string output_dir;

  std::vector<std::vector<std::pair<std::string, std::string>>> post_mapper;

  std::vector<std::vector<std::pair<std::string, std::vector<std::string>>>>
      pred_reducer;

  std::mutex m;

  IMapper mapper;
  IReducer reducer;

  void FilesToBuf();
  void SplitFiles();
  void ClearFiles();

  void Shuffle();
  void Map(const size_t file_num);
  void Reduce(const size_t reducer_num);

public:
  IndustrialMapReduce();
  explicit IndustrialMapReduce(size_t split_size_bytes);

  void SetInputFiles(const std::vector<std::string> &files);

  void SetMaxSimultaneouslyWorkers(size_t max_simultaneous_workers);
  void SetNumReducers(size_t max_simultaneous_reducers);

  void SetTmpDir(const std::string &dir);
  void SetOutputDir(const std::string &dir);
  void SetMapper(const IMapper &mapper_);
  void SetReducer(const IReducer &reducer_);
  void Start();
};

} // namespace IndustrialRise

#endif // INDUSTRIAL_MAP_REDUCE_HPP_
