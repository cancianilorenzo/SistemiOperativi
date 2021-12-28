lista=("$1")
for ((i=2; i<"$#"+1; i++))
do
    lista+=("$i")
    #shift
done

for ((i="$#"-1; i>-1; i--))
do
    echo ${lista[i]}
done