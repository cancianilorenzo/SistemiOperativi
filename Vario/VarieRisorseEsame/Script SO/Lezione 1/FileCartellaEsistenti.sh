#verifica se la variabile DATA ri riferisce a un file o una cartella ESISTENTI

    [[ -e $DATA ]] && echo T || echo  F

    