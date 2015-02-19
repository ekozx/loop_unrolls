#! /bin/bash

echo "looping the integer unrolling"

for i in `seq 1 100`; do 
  echo $i
  ./a.out
done