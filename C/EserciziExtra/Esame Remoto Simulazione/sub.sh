if [ $1 -gt 10 ]
then echo "troppi parametri!"
fi

for ((i=0; i<$1; i++))
do
    ( echo $BASHPID )
done