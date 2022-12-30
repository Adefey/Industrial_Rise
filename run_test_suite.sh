#!/usr/bin/env bash

set -e pipefail

valgrind --leak-check=full ./build/test/unit/industrial_test
lcov -t ./build/test/unit/ -o ./build/test/unit/coverage_report.info -c -d ./build/test/unit
genhtml -o ./build/test/unit/coverage_report ./build/test/unit/coverage_report.info
python3 ./test/integration/test.py
