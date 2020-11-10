#!/bin/bash

# executable name
exec=./a.out

for value in {1..27}
do
    echo __________________
    echo file: test/testcase$value.txt
    echo __________________
    ./$exec test/testcase$value.txt
    echo
done
