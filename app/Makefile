#app makefile:q


#lib
LIB := libApp.a

#interface
all:$(LIB)

#object to ar static libary
OBJECT=Application.o Post.o Comment.o Category.o PathDispatcher.o

#target
$(LIB):$(OBJECT)
	ar rs $(LIB) $(OBJECT)
	mv $(LIB) ../

#Task
Application.o:Application.cpp
	g++ -c -g  Application.cpp

#PathDispatcher
PathDispatcher.o:PathDispatcher.cpp
	g++ -c -g PathDispatcher.cpp

#Post
Post.o:Post.cpp
	g++ -c -g Post.cpp


#Comment
Comment.o:Comment.cpp
	g++ -c -g Comment.cpp


#Category
Category.o:Category.cpp
	g++ -c -g Category.cpp

clean:
	rm -f *.o
