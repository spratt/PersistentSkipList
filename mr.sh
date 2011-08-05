#!/bin/bash
make clean &&
make &&
rm psl.log &&
./test_timestamped_array && ./test_persistent_skiplist 2> psl.log