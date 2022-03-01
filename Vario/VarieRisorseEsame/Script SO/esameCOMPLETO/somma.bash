alias feedback='(echo "Come ti chiami?"; read nome; echo "Ciao $nome")'



alias somma='(var="."; dati="0"; while [[ ! -z "$var" ]]; do echo -n "Numero? "; read var; dati="$dati + $var"; done; dati="$dati 0"; echo "La somma è $(( $dati ))" )'