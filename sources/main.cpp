// Copyright 2022 Adefe

#include <industrial_map_reduce.hpp>
#include <industrial_map_reduce_user_mapper.hpp>
#include <industrial_map_reduce_user_reducer.hpp>

int main(int argc, char **argv) {
  IndustrialRise::IndustrialMapReduce map_reducer(56123);
  map_reducer.SetInputFiles({"./example_data/text.txt"});
  map_reducer.SetMaxSimultaneouslyWorkers(10);
  map_reducer.SetNumReducers(10);
  map_reducer.SetTmpDir("./tmp_dir/");
  map_reducer.SetOutputDir("./result_dir/");
  UserMapper mapper;
  map_reducer.SetMapper(mapper);
  UserReducer reducer;
  map_reducer.SetReducer(reducer);
  map_reducer.Start();
  return 0;
}
