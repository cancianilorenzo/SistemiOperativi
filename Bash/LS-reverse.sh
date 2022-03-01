#!/bin/bash
clear
dati=( $(ls) ); lista=()
for i in ${!dati[@]}; do
    lista=( "${dati[$i]}" "${lista[@]}" )
done
echo "REVERSE: "; echo ${lista[@]};
