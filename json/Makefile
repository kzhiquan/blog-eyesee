#Db makefile



#lib
LIB := libJsonc.a

#object
OBJECT := cJSON.o


#cJSON.c
cJSON.o:cJSON.c
	gcc -c -g cJSON.c

#all target
all:$(LIB)

$(LIB):$(OBJECT)
	ar rs $(LIB) $(OBJECT)
	mv $(LIB) ../

#clean
clean:
	rm -f *.o





