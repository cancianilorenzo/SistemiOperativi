#!/bin/bash
path=$1
numero=$2

make; 
[ -e $path ] && rm $path
./program $path $numero
sleep 1
cat $path

