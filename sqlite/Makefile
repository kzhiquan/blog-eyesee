#sqlite makefile



#lib
LIB := libSqlite3.a

#object
OBJECT := sqlite3.o

#sqlite3
sqlite3.o:sqlite3.c
	gcc -c -g sqlite3.c

#all target
all:$(LIB)

$(LIB):$(OBJECT)
	ar rs $(LIB) $(OBJECT)
	mv $(LIB) ../

#clean
clean:
	rm -f *.o





