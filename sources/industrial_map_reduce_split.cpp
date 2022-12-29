// Copyright 2022 Adefe

#include <industrial_map_reduce.hpp>

namespace IndustrialRise {

void IndustrialMapReduce::FilesToBuf() {
  /* Соединяет все входные файлы в один большой */
  std::ofstream big_file(buf_big_file_name);

  for (auto input_file_name : input_files) {
    std::ifstream input_file(input_file_name);
    while (input_file.is_open()) {
      std::string buf = "";
      input_file >> buf;
      big_file << buf;
    }
    input_file.close();
  }
  big_file.close();
}

void IndustrialMapReduce::SplitFiles() {
  /* Разбивает большой файл по маленьким примерно равного размера, разбиение
   * идет только по \n */
  std::ifstream big_file(buf_big_file_name);
  size_t file_length = big_file.tellg();
  split_count = 0;
  for (size_t cur_byte_count = 0; cur_byte_count < file_length;
       cur_byte_count += split_size) {
    std::stringstream ss;
    while (big_file.is_open() && (ss.tellg() < split_size)) {
      std::string line = "";

      big_file >> line; /* Читает по строкам, что гарантирует разбиение по \n */
      ss << line;
    }
    std::ofstream splitted_file(tmp_dir + split_prefix +
                                std::to_string(split_count++));
    splitted_file << ss.str();
    splitted_file.close();
  }
}

void IndustrialMapReduce::ClearFiles() {
  /* Удаляет весь tmp_dir*/
  std::filesystem::remove_all(tmp_dir);
}

} // namespace IndustrialRise
