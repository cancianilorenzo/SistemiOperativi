( [[ -f $DATA ]] && echo file ) || ( [[ -d $DATA ]] && echo cartella ) || echo "?"





#stampa il risultato dell'operazione contenuta nel file indicato dalla variabile DATA oppure ? se file non esistente
echo $(( cat $DATA )) || echo "?"




[[ -f $DATA ]] && echo $(( cat $DATA )) || echo "?"

[[ -f $DATA ]] && echo $(( $(cat $DATA) )) || echo "?"
