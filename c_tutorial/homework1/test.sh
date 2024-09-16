#!/bin/bash

# Test learn executable
echo "Testing 'learn' program..."
./learn > learn_output.txt
expected_learn_output="25 MPG is 10.63 kilometers per liter
25 MPG is 9.41 liters per 100 kilometers
9.41 liters per 100 kilometers is 25.00 MPG
Testing with a bad argument (negative MPG): 0.00"
learn_output=$(cat learn_output.txt)

if [ "$learn_output" == "$expected_learn_output" ]; then
    echo "'learn' program output is correct."
else
    echo "'learn' program output is incorrect!"
    echo "Expected:"
    echo "$expected_learn_output"
    echo "Got:"
    echo "$learn_output"
fi

# Test arger executable with different arguments
echo "Testing 'arger' program..."

# Test case 1: Uppercase
output=$(./arger -u help is on the way)
expected_output="HELP IS ON THE WAY"
if [ "$output" == "$expected_output" ]; then
    echo "Test 1 passed: -u help is on the way"
else
    echo "Test 1 failed: Expected '$expected_output', but got '$output'"
fi

# Test case 2: Lowercase
output=$(./arger -l HELP IS ON THE WAY)
expected_output="help is on the way"
if [ "$output" == "$expected_output" ]; then
    echo "Test 2 passed: -l HELP IS ON THE WAY"
else
    echo "Test 2 failed: Expected '$expected_output', but got '$output'"
fi

# Test case 3: Capitalize
output=$(./arger -cap help IS on the WAY)
expected_output="Help Is On The Way"
if [ "$output" == "$expected_output" ]; then
    echo "Test 3 passed: -cap help IS on the WAY"
else
    echo "Test 3 failed: Expected '$expected_output', but got '$output'"
fi

# Test case 4: Invalid option
output=$(./arger -x help is on the way)
expected_output="Error: Unknown option '-x'."
if [ "$output" == "$expected_output" ]; then
    echo "Test 4 passed: Invalid option"
else
    echo "Test 4 failed: Expected '$expected_output', but got '$output'"
fi

# Test case 5: Not enough arguments
output=$(./arger -u)
expected_output="Error: Not enough arguments provided."
if [ "$output" == "$expected_output" ]; then
    echo "Test 5 passed: Not enough arguments"
else
    echo "Test 5 failed: Expected '$expected_output', but got '$output'"
fi
