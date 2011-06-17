#!/bin/bash
echo +------------------------------------------------------------------------------+
echo \| Building a priority search tree with $1 nodes and querying $2 times
echo +------------------------------------------------------------------------------+
test_pst $1 $2 quiet
echo +------------------------------------------------------------------------------+
echo \| Building a persistent list with $1 nodes and querying $2 times
echo +------------------------------------------------------------------------------+
test_list $1 3 $2 quiet