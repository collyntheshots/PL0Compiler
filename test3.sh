#!/bin/bash

# executable name
exec=./a.out

for value in {1..21}
do
    ./$exec test/sample$value.txt
    echo
    echo
done
