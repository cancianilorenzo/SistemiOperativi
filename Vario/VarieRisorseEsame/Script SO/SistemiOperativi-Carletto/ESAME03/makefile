BASE=program
rule1: 
	( [ "a${NAME}" = "a" ] && gcc main.c -o ${BASE} ) || gcc main.c -o ${NAME}
	[[ -e $NAME ]] && echo "T" || echo "F"
clean:
	rm ${NAME}
.PHONY: clean