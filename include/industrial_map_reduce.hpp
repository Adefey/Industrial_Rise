// Copyright 2022 Adefe

#ifndef INDUSTRIAL_MAP_REDUCE_HPP_
#define INDUSTRIAL_MAP_REDUCE_HPP_

#include <algorithm>
#include <chrono>
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
  operator()(const std::string &str) = 0;
};

struct IReducer {
  virtual std::vector<std::pair<std::string, std::string>>
  operator()(std::vector<std::pair<std::string, std::vector<std::string>>>) = 0;
};

enum STATUSES {
  NOT_WORKING = 0,
  WORKING = 1,
  EXITED = 2,
};

class IndustrialMapReduce {
private:
  const std::vector<char> alphabet = {
      'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
      'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

  const std::string buf_big_file_name;
  const std::string split_prefix;
  const std::string reduced_prefix;
  const size_t split_size;
  size_t split_count;

  size_t num_mappers;
  size_t num_reducers;

  std::vector<std::string> input_files;
  std::vector<int> mapper_status;

  std::string tmp_dir;

  std::string output_dir;

  std::vector<std::vector<std::vector<std::pair<std::string, std::string>>>>
      post_mapper;

  std::mutex m;

  IMapper *mapper;
  IReducer *reducer;

  size_t find_ready_slot();
  void FilesToBuf();
  void SplitFiles();
  void ClearFiles();
  void InitializePostMapper();

  void Map(const size_t file_num, size_t mapper_num);
  void Reduce(const size_t reducer_num);

  class PairCompare {
  public:
    int
    operator()(const std::pair<std::pair<std::string, std::string>, int> &p1,
               const std::pair<std::pair<std::string, std::string>, int> &p2) {
      return p1.first.first.compare(p2.first.first) > 0;
    }
  };

public:
  IndustrialMapReduce();
  explicit IndustrialMapReduce(size_t split_size_bytes);

  void SetInputFiles(const std::vector<std::string> &files);

  void SetMaxSimultaneouslyWorkers(size_t max_simultaneous_workers);
  void SetNumReducers(size_t max_simultaneous_reducers);

  void SetTmpDir(const std::string &dir);
  void SetOutputDir(const std::string &dir);
  void SetMapper(IMapper &mapper_);
  void SetReducer(IReducer &reducer_);
  void Start();
};

} // namespace IndustrialRise

#endif // INDUSTRIAL_MAP_REDUCE_HPP_
