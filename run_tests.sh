#!/bin/bash

success=0
fail=0

for test in "$@"; do
    echo "----------> $test <----------"
    if "$test"; then
        success=$(( success + 1 ))
    else
        fail=$(( fail + 1 ))
    fi
done

echo "==========> Results <=========="
echo "$success tests successful"
echo "$fail tests failed"
