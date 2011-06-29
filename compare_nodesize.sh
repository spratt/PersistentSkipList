#!/bin/bash
for (( i=2; i<=$4; i++ ))
do
    echo +-----------------------------------------------------------------------------+
    echo \| Building a persistent list...
    echo \| ...with $1 nodes of size $i
    echo \| ...querying $2 times
    echo \| ...enumerating $3 times
    echo +-----------------------------------------------------------------------------+
    test_list $1 $2 $3 $i quiet
done