// Copyright 2022 Adefe

#include <gtest/gtest.h>

#include <industrial_map_reduce.hpp>
#include <industrial_map_reduce_user_mapper.hpp>
#include <industrial_map_reduce_user_reducer.hpp>

TEST(IndustrialMapReduceTest, TestMapReduce) {
  IndustrialRise::IndustrialMapReduce map_reducer(100000);
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

  for (size_t i = 0; i < 8; ++i) {
    EXPECT_TRUE(
        std::filesystem::exists("./result_dir/reduced_" + std::to_string(i)));
  }

  bool penis_found = false;
  for (size_t i = 0; i < 10; ++i) {
    std::ifstream file("./result_dir/reduced_" + std::to_string(i));
    std::string buf = "";
    while (std::getline(file, buf)) {
      if (buf.find("penis") != std::string::npos) {
        penis_found = true;
        break;
      }
    }
    file.close();
  }
  EXPECT_TRUE(penis_found);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
