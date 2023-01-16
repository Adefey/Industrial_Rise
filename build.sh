#!/usr/bin/env bash

set -e pipefail

mkdir -p ./build/
cmake -S . -B ./build/
cmake --build ./build/
