all: counter.o
	@echo "The binary $^ was built"
%.out: %.c
	gcc $^ -o $@ -pthread
clean:
	rm -f *.out
.PHONY: clean