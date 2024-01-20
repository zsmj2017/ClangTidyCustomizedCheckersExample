#!/bin/bash

set -eu

SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" &>/dev/null && pwd)
BUILD_DIR="$SCRIPT_DIR/build"

rm -rf "${BUILD_DIR}"
mkdir "${BUILD_DIR}"
cd "${BUILD_DIR}"

cmake "$SCRIPT_DIR"
make

cd "$SCRIPT_DIR"

LIB_FILE="$BUILD_DIR/libWCharToBoolConversionCheck.dylib"
DEMO_FILE="$SCRIPT_DIR/demo.cpp"


echo "clang-tidy demo.cpp use std::string"
clang-tidy --checks='-*,wchar-to-bool-conversion-check' -load "$LIB_FILE" "$DEMO_FILE" -- 

echo "clang-tidy demo.cpp use std::wstring"
clang-tidy --checks='-*,wchar-to-bool-conversion-check' -load "$LIB_FILE" "$DEMO_FILE" -- -DUSE_WIDE_STRINGS

