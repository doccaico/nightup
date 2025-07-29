#!/bin/bash
set -eu
cd "$(dirname "$0")"

exe_name=nightup

# ./build.bash          -> debug mode (implicit)
# ./build.bash debug    -> debug mode
# ./build.bash asan     -> debug mode with asan
# ./build.bash release  -> release mode

if [ "$#" -eq 0 ]; then
  debug=
  echo "[debug mode]"
else
    for arg in "$@"; do declare $arg=; done
    if [ -v debug ]; then
        echo "[debug mode]"
    elif [ -v asan ]; then
        debug=
        echo "[debug mode with asan]"
    elif [ -v release ]; then
        echo "[release mode]"
    fi
fi

compiler=gcc

auto_compile_flags=
if [ -v asan ]; then auto_compile_flags="$auto_compile_flags -fsanitize=address"; fi

gcc_common="-I../vendor/ini/ -I../vendor/mlib/ -std=gnu99 -Wall -Wextra -Wpedantic -Wconversion -Wdouble-promotion -Wno-unused-parameter -Wno-unused-function -Wno-sign-conversion -Wno-sign-compare -Wno-unused-result"
gcc_debug="$compiler -g -O0 -DDEBUG ${gcc_common} ${auto_compile_flags}"
gcc_release="$compiler -Os -s -DNDEBUG ${gcc_common} ${auto_compile_flags}"
gcc_link=""
gcc_out="-o"

if [ -v debug ]; then compile="$gcc_debug"; fi
if [ -v release ]; then compile="$gcc_release"; fi
compile_link="$gcc_link"
out="$gcc_out"

mkdir -p build

# Build
cd build
$compile ../src/main.c $compile_link $out $exe_name
cd ..
