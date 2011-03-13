#!/bin/sh

# list of unittest binaries
targets=`ls $1/bin/*_unittest`

# run all unittest
for target in $targets 
do 
  $target
done 

