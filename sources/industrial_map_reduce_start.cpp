// Copyright 2022 Adefe

#include <industrial_map_reduce.hpp>
namespace IndustrialRise {

IndustrialMapReduce::IndustrialMapReduce()
    : buf_big_file_name("big_buf"), split_prefix("splitted_"),
      reduced_prefix("reduced_"), split_size(33554432) {
} /* Default constructor with 32MB split size*/

IndustrialMapReduce::IndustrialMapReduce(size_t split_size_bytes)
    : buf_big_file_name("big_buf"), split_prefix("splitted_"),
      reduced_prefix("reduced_"), split_size(split_size_bytes) {
} /* Constructor with parameter wich specifies split size in bytes*/

void IndustrialMapReduce::SetInputFiles(const std::vector<std::string> &files) {
  input_files = files;
}

void IndustrialMapReduce::SetMaxSimultaneouslyWorkers(
    size_t max_simultaneous_workers) {
  num_mappers = max_simultaneous_workers;
}
void IndustrialMapReduce::SetNumReducers(size_t max_simultaneous_reducers) {
  num_reducers = max_simultaneous_reducers;
}

void IndustrialMapReduce::SetTmpDir(const std::string &dir) { tmp_dir = dir; }
void IndustrialMapReduce::SetOutputDir(const std::string &dir) {
  output_dir = dir;
}

size_t IndustrialMapReduce::find_ready_slot() {
  while (true) {
    for (size_t i = 0; i < mapper_status.size(); ++i) {
      if (mapper_status[i] == NOT_WORKING || mapper_status[i] == EXITED) {
        return i;
      }
    }
  }
}

void IndustrialMapReduce::Start() {
  std::filesystem::create_directory(tmp_dir);
  FilesToBuf();
  SplitFiles();
  std::vector<std::thread> threads(num_mappers);

  InitializePostMapper();

  mapper_status = std::vector<int>(num_mappers, NOT_WORKING);

  for (size_t i = 0; i < split_count; ++i) {
    size_t ind_emplace = find_ready_slot();
    std::cout << "slot: " << ind_emplace << std::endl;
    if (mapper_status[ind_emplace] == EXITED) {
      threads[ind_emplace].join();
    }

    mapper_status[ind_emplace] = WORKING;
    threads[ind_emplace] =
        std::thread(&IndustrialMapReduce::Map, this, i, ind_emplace);
  }

  for (size_t i = 0; i < num_mappers; ++i) {
    if (mapper_status[i] == EXITED || mapper_status[i] == WORKING) {
      threads[i].join();
    }
  }

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
