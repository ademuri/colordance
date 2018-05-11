#!/bin/bash

# Formats all of the source files using clang-format.
clang-format -i -style=Google $(find src -name *.[ch]pp)
clang-format -i -style=Google $(find arduino/src -name *.[ch]pp)
