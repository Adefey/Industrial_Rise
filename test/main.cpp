// Copyright 2022 Adefe

#include <gtest/gtest.h>

#include <industrial_map_reduce.hpp>

TEST(IndustrialMapReduceTest, TestMapReduce) {
  IndustrialRise::IndustrialMapReduce map_reducer;
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
