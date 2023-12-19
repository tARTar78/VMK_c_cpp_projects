#!/bin/bash

echo "Running test 1..."
./shell_art < /tmp1/test0_input.txt > /tmp1/test0_output.txt
if diff -q /tmp1/test0_expected_output.txt /tmp1/test0_output.txt
then
    echo "Test 1 passed"
else
    echo "Test 1 failed"
fi

echo "Running test 2..."
./shell_art < /tmp/test1_input.txt > /tmp/test1_output.txt
if diff -q /tmp/test1_expected_output.txt /tmp/test1_output.txt
then
    echo "Test 2 passed"
else
    echo "Test 2 failed"
fi

echo "Running test 3..."
./shell_art < /tmp/test2_input.txt > /tmp/test2_output.txt
if diff -q /tmp/test2_expected_output.txt /tmp/test2_output.txt
then
    echo "Test 3 passed"
else
    echo "Test 3 failed"
fi

echo "Running test 4..."
./shell_art < /tmp/test3_input.txt > /tmp/test3_output.txt
if diff -q /tmp/test3_expected_output.txt /tmp/test3_output.txt
then
    echo "Test 4 passed"
else
    echo "Test 4 failed"
fi

echo "Running test 5..."
./shell_art < /tmp/test4_input.txt > /tmp/test4_output.txt
if diff -q /tmp/test4_expected_output.txt /tmp/test4_output.txt
then
    echo "Test 5 passed"
else
    echo "Test 5 failed"
fi
