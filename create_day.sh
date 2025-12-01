#!/bin/bash

if [ -z $1 ]; then
  echo "enter year and day"
  exit 1
elif [ -z $2 ]; then
  echo "enter day"
  exit 1
fi

if [ ! -d "Advent_$1/" ]; then
  mkdir Advent_$1
elif [ -d "Advent_$1/Day_$2" ]; then
  echo "Directory exists ... so go there you dummy"
  exit 1
fi

cd Advent_$1
if [ "$3" == "rust" ]; then
  cargo new Day_$2
  cd Day_$2
else
  mkdir Day_$2
  cd Day_$2
  cp ../../Utils/template.cpp main.cpp
fi

touch test.txt
python ../../Utils/get_inputs.py $1 $2
