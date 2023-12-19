#!/bin/bash

echo "Running test 1..."
./shell_art < test1_input.txt > test1_output.txt
if diff -q test1_expected_output.txt test1_output.txt
then
    echo "Test 1 passed"
else
    echo "Test 1 failed"
fi

echo "Running test 2..."
./shell_art < test2_input.txt > test2_output.txt
if diff -q test2_expected_output.txt test2_output.txt
then
    echo "Test 2 passed"
else
    echo "Test 2 failed"
fi

echo "Running test 3..."
./shell_art < test3_input.txt > test3_output.txt
if diff -q test3_expected_output.txt test3_output.txt
then
    echo "Test 3 passed"
else
    echo "Test 3 failed"
fi

echo "Running test 4..."
./shell_art < test4_input.txt > test4_output.txt
if diff -q test4_expected_output.txt test4_output.txt
then
    echo "Test 4 passed"
else
    echo "Test 4 failed"
fi

echo "Running test 5..."
./shell_art < test5_input.txt > test5_output.txt
if diff -q test5_expected_output.txt test5_output.txt
then
    echo "Test 5 passed"
else
    echo "Test 5 failed"
fi
