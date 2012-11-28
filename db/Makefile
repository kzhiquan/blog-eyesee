#Db makefile



#lib
LIB := libDb.a

#object
OBJECT := DbConn.o DbConnMg.o BgSql.o BgDao.o


#DbConn
DbConn.o:DbConn.cpp
	g++ -c -g DbConn.cpp

#DbConnMg
DbConnMg.o:DbConnMg.cpp
	g++ -c -g DbConnMg.cpp

#BgSql
BgSql.o:BgSql.cpp
	g++ -c -g BgSql.cpp

#BgDao
BgDao.o:BgDao.cpp
	g++ -c -g BgDao.cpp

#all target
all:$(LIB)

$(LIB):$(OBJECT)
	ar rs $(LIB) $(OBJECT)
	mv $(LIB) ../

#clean
clean:
	rm -f *.o





