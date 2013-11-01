CFLAGS=-g
BIN=while

$(BIN): $(BIN).o main.o tree.o 
	cc $(CFLAGS) $(BIN).o main.o  tree.o -o $@

$(BIN).o: $(BIN).y type.h $(BIN).l
	yacc -vd $(BIN).y
	lex $(BIN).l
	mv y.tab.c $(BIN).c
	cc -c $(CFLAGS)  $(BIN).c

tree.o : tree.c
	cc -c ${CFLAGS} tree.c

gencode.o : gencode.c
	cc -c ${CFLAGS} gencode.c

main.o : main.c
	cc -c ${CFLAGS} main.c

mainl.o : main.c
	cc -c ${CFLAGS} -DLEX main.c
	mv main.o mainl.o

clean:
	rm -f *.o analy.* y.* *.out y.* lex.yy.* $(BIN).c analy $(BIN)

