CC= gcc
CFLAGS= -Werror
EXE= eval
SOURCES= \
	src/eval.c \
	src/stack.c \
	src/main.c

$(EXE): clean 
	$(CC) $(SOURCES) $(CFLAGS) -o $@

memory: $(EXE)
	leaks --atExit -- ./$(EXE)

clean: 
	rm -f *.o $(EXE)

