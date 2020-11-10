#!/bin/bash

# executable name
exec=./a.out

for value in {1..19}
do
    echo __________________
    echo file: test/parserTest$value.txt
    echo __________________
    ./$exec test/parserTest$value.txt -l -a -v
    echo
done
