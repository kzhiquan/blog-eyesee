#top makefile

#target
TARGET := eyesee

#subdir
SUBDIR := server app sqlite db json
 

#object in current dir
OBJECT = main.o


#main object
main.o:main.c
	g++ -c -g -Iserver -Iapp main.c


#all interface
all:$(TARGET)
$(TARGET):$(OBJECT)
	for dir in $(SUBDIR);\
	do make -C $$dir all || exit 1;\
	done
	g++ -o $(TARGET) $(OBJECT) -g -L. -lApp -lServer -lDb  -lJsonc -lSqlite3 -lpthread -ldl 


#clean
clean:
	for dir in $(SUBDIR);\
	do make -C $$dir clean || exit 1;\
	done
	rm -rf *.o *.a  deploy $(TARGET)
#deploy
deploy:
	mkdir deploy
	cp eyesee ./deploy
	cp WConfigure.cg ./deploy
	cp eyesee.db ./deploy
	cp eyeseedb.sql ./deploy
	cp Configure	./deploy
	cp -r web	./deploy

