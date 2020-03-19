#!/usr/bin/env bash
find ./include -type f \( -iname \*.cpp -o -iname \*.h \) | xargs clang-format -style=llvm -i
find ./test -type f \( -iname \*.cpp -o -iname \*.h \)| xargs clang-format -style=llvm -i
