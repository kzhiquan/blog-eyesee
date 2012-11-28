#server makefile
#the object

LIB := libServer.a

OBJECT=Server.o Configure.o Site.o IOLoop.o WorkManager.o StreamManager.o WorkThread.o \
	TcpConnection.o WebStream.o HttpResponse.o HttpRequest.o Http.o Session.o \
	RollBuff.o Uri.o Task.o CFile.o ESLog.o

#interface all
all:$(LIB)
$(LIB):$(OBJECT)
	ar rs $(LIB) $(OBJECT)
	mv $(LIB) ../

#Task
Task.o:Task.cpp
	g++ -c -g Task.cpp
#server
Server.o:Server.cpp
	g++ -c -g Server.cpp

#configure
Configure.o:Configure.cpp
	g++ -c -g Configure.cpp

#uri
Uri.o:Uri.cpp
	g++ -c -g Uri.cpp
#Site
Site.o:Site.cpp
	g++ -c -g Site.cpp

#IOLoop
IOLoop.o:IOLoop.cpp
	g++ -c -g IOLoop.cpp

#WorkThread
WorkThread.o:WorkThread.cpp
	g++ -c -g WorkThread.cpp

#TcpConnection
TcpConnection.o:TcpConnection.cpp
	g++ -c -g TcpConnection.cpp

#RollBuff
RollBuff.o:RollBuff.cpp
	g++ -c -g RollBuff.cpp
#WeStream
WebStream.o:WebStream.cpp
	g++ -c -g WebStream.cpp

#WorkManager
WorkManager.o:WorkManager.cpp
	g++ -c -g WorkManager.cpp

#StreamManager
StreamManager.o:StreamManager.cpp
	g++ -c -g StreamManager.cpp

#HttpResponse
HttpResponse.o:HttpResponse.cpp
	g++ -c -g HttpResponse.cpp

#HttpRequest
HttpRequest.o:HttpRequest.cpp
	g++ -c -g HttpRequest.cpp

#Http
Http.o:Http.cpp
	g++ -c -g Http.cpp

#session
Session.o:Session.cpp
	g++ -c -g Session.cpp

#CFile
CFile.o:CFile.cpp
	g++ -c -g CFile.cpp

#ESLog
ESLog.o:ESLog.cpp
	g++ -c -g ESLog.cpp

clean:
	rm -f *.o

