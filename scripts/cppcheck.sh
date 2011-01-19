#!/bin/sh

echo "running cppcheck..."
cppcheck --enable=all -f --inline-suppr --verbose . 2> /tmp/cppcheck.txt
echo "DONE (/tmp/cppcheck.txt)"
