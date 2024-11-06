#!/bin/bash

cd `dirname $0`
source ./test.bash

test_command 'echo -n "Hello." | simple-pcypher -k 0x123 | simple-pcypher -d -s 6 -k 0x123' 'Hello.'
test_command 'echo -n "Hello." | simple-pcypher -e -k 0x123 | simple-pcypher -d -s 6 -k 0x123' 'Hello.'
test_command 'echo -n "Hello." | simple-pcypher -e -k 0x123 | simple-pcypher -d -p 0 -s 3 -k 0x123' 'Hel'
test_command 'echo -n "Hello." | simple-pcypher -e -k 0x123 | simple-pcypher -d -p 3 -s 3 -k 0x123' 'lo.'
