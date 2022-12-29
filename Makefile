SHELL=/bin/bash #-e pipefail

BUILD_DIR=./build/
INCLUDE_DIR=./include/
SRC_DIR=./sources/
TEST_DIR=./test/
CODE_FILES=$(INCLUDE_DIR)*.hpp $(SRC_DIR)*.cpp $(TEST_DIR)*.cpp

all: format check build_app build_test run_test make_report

format:
	clang-format -i $(CODE_FILES)

check:
	cppcheck --enable=all --suppress=missingIncludeSystem --suppress=unusedFunction $(CODE_FILES)
	cpplint --extensions=cpp,hpp --filter=-whitespace/indent,-whitespace/comments,-build/c++11,-readability/multiline_comment,-build/header_guard $(CODE_FILES)

build_app:
	cd build && \
	cmake CMakeLists.txt && \
	cmake --build .

build_test:
	cd test/build && \
	cmake CMakeLists.txt && \
	cmake --build .

run_test:
	valgrind --leak-check=full ./test/build/industrial_test

make_report:
	lcov -t ./test/build/ -o ./test/coverage_report.info -c -d ./test
	genhtml -o ./test/coverage_report ./test/coverage_report.info

