// Copyright 2022 Adefe

#include <industrial_map_reduce.hpp>

namespace IndustrialRise {

void IndustrialMapReduce::FilesToBuf() {
  /* Соединяет все входные файлы в один большой */
  std::ofstream big_file(tmp_dir + buf_big_file_name);

  for (auto input_file_name : input_files) {
    std::ifstream input_file(input_file_name);
    std::cout << "Creating file" << std::endl;
    std::string line;
    while (std::getline(input_file, line)) {
      big_file << line << '\n';
    }
    input_file.close();
    std::cout << "Input file closed" << std::endl;
  }
  big_file.close();
  std::cout << "Result file closed" << std::endl;
}

void IndustrialMapReduce::SplitFiles() {
  /* Разбивает большой файл по маленьким примерно равного размера, разбиение
   * идет только по \n */
  std::ifstream big_file(tmp_dir + buf_big_file_name);
  size_t file_length = std::filesystem::file_size(tmp_dir + buf_big_file_name);
  std::cout << "Reading big file with size of " << file_length << std::endl;
  split_count = 0;
  std::cout << "Start file split" << std::endl;
  for (size_t cur_byte_count = 0; cur_byte_count < file_length;
       cur_byte_count += split_size) {
    std::cout << "Making file #" << split_count << std::endl;
    std::stringstream ss;
    std::string line;
    while (std::getline(big_file, line) && (ss.str().size() <= split_size)) {
      ss << line;
    }
    ss << line;
    std::ofstream splitted_file(tmp_dir + split_prefix +
                                std::to_string(split_count++));
    std::string data = ss.str();
    data.erase(0, 1);
    splitted_file << data;
    splitted_file.close();
    std::cout << "Split file #" << split_count << " saved" << std::endl;
  }
  big_file.close();
}

void IndustrialMapReduce::ClearFiles() {
  /* Удаляет весь tmp_dir*/
  std::filesystem::remove_all(tmp_dir);
}

} // namespace IndustrialRise
