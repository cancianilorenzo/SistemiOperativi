#!/usr/bin/env bash 
first=$1;
second=$2;
#rimuoviamo first se esisteva già
[[ -e $first ]] && rm -rf $first; 

gcc main.c -o program; ./program "${first}" $second 
sleep 2
[[ -f "${first}" ]] && cat "${first}";