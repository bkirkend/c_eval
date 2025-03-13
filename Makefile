CC= gcc
CFLAGS= -Werror
EXE= eval
BIN_EXE= bval
SOURCES= \
	src/eval.c \
	src/stack.c \
	src/main.c

MY_DIR= ~/Documents/C/c_eval
BIN_DIR= ~/Documents/C/bin

run: $(EXE)
	./$(EXE)

$(EXE): clean 
	$(CC) $(SOURCES) $(CFLAGS) -o $@

bin: $(EXE)
	rm -f $(BIN_DIR)/$(BIN_EXE)
	ln -s $(MY_DIR)/$(EXE) $(BIN_DIR)/$(BIN_EXE)

memory: $(EXE)
	leaks --atExit -- ./$(EXE)

clean: 
	rm -f *.o $(EXE)

