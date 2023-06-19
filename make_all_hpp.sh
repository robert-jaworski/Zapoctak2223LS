#!/bin/bash

for test in "$@"; do
    echo "#include \"$test\""
done
