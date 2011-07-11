#!/bin/bash

BOOST_INCLUDE=../../boost_1_46_1

g++ -I$BOOST_INCLUDE -o test ./main.cpp
./test qwe
cat qwe
