#!/bin/bash

# Formats all of the source files using clang-format.
clang-format -i $(find . -name *.[ch]pp)
