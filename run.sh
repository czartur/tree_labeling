#!/usr/bin/bash

echo "number of tests ? (less than 10 please)"
read N

echo "compiling..."
g++ -o executable hang.cpp

export LC_NUMERIC="en_US.UTF-8"
TIMEFORMAT=%R

DIR="time.csv"
echo "" >$DIR

for i in input/*.in
do
  i=$(basename $i)
  i=${i%".in"}
  echo "testing $i..."
  echo -n "$i" >> $DIR
  for run in $(seq 1 $N)
  do
    echo -n "," >> $DIR
    duration=$( { time ./executable < input/$i".in" > output/$i".out" ;} 2>&1)
    echo -n "$duration" >> $DIR
    echo "$duration"
  done
  printf "\n" >> $DIR
done

rm executable
