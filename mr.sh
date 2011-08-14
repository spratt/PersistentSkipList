#!/bin/bash
make clean &&
make &&
rm -f psl.log &&
./test_timestamped_array && ./test_persistent_skiplist 2> psl.log
