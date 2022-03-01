NAME=CommunicationControl

compila: ${NAME}.o
	@gcc ${NAME}.o -o ${NAME}.out
	@echo "File eseguibile"
	@echo "Compilato!"

${NAME}.o: ${NAME}.s
	@echo "File oggetto"
	@gcc -c ${NAME}.s -o ${NAME}.o

${NAME}.s: ${NAME}.i
	@echo "File assembly"
	@gcc -S ${NAME}.i -o ${NAME}.s

${NAME}.i: ${NAME}.c
	@echo "File da preprocessore"
	@gcc -E ${NAME}.c -o ${NAME}.i

${NAME}.c:
	@echo "Non posso compilare se non ho il file sorgente!!!"

.SECONDARY: ${NAME}.i ${NAME}.s ${NAME}.o
