#!/bin/bash
clear;
ordinata=( $($1) ); inversa=()
for i in ${!ordinata[@]}; do
    inversa=( "${ordinata[$i]}" "${inversa[$@]}" )
done

echo "ORIGINAL: "; ls; echo; echo "REVERSED: "; echo "${inversa[@]}"; echo
