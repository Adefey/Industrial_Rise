SHELL=/bin/bash #-e pipefail

BUILD_DIR=./build/
INCLUDE_DIR=./include/
SRC_DIR=./sources/
TEST_DIR=./test/unit/
CODE_FILES=$(INCLUDE_DIR)*.hpp $(SRC_DIR)*.cpp $(TEST_DIR)*.cpp

all: format check build_app build_test run_test make_report integrational_test

format:
	clang-format -i $(CODE_FILES)

check:
	cppcheck --enable=all --suppress=missingIncludeSystem --suppress=unusedFunction $(CODE_FILES)
	cpplint --extensions=cpp,hpp --filter=-whitespace/indent,-whitespace/comments,-build/c++11,-readability/multiline_comment,-build/header_guard,-runtime/references $(CODE_FILES)

build_app:
	cd build && \
	cmake CMakeLists.txt && \
	cmake --build .

build_test:
	cd test/unit/build/ && \
	cmake CMakeLists.txt && \
	cmake --build .

run_test:
	valgrind --leak-check=full ./test/unit/build/industrial_test

make_report:
	lcov -t ./test/unit/build/ -o ./test/unit/coverage_report.info -c -d ./test/unit
	genhtml -o ./test/unit/coverage_report ./test/unit/coverage_report.info

integrational_test:
	python3 ./test/integration/test.py

