#!/usr/bin/env bash

set -e pipefail

CODE_FILES="./include/*.hpp ./sources/*.cpp ./test/unit/*.cpp"

clang-format -i $CODE_FILES
cppcheck --enable=all --suppress=missingIncludeSystem --suppress=unusedFunction $CODE_FILES
cpplint --extensions=cpp,hpp --filter=-whitespace/indent,-whitespace/comments,-build/c++11,-readability/multiline_comment,-build/header_guard,-runtime/references $CODE_FILES
