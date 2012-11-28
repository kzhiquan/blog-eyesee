
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>

#include "Server.h"
#include "Application.h"


/* signal segv handler */
void SEGVlogbufflush(int){
    
    ESLog::ISLog("SIGSEGV signal catch\n", __FILE__, __LINE__);
    /* fush */
    ESLog::flush();

    //signal(SIGSEGV, SIG_DFL);
}

/* signal bus handler */
void BUSlogbufflush(int){
    
    ESLog::ISLog("SIGBUS signal catch\n", __FILE__, __LINE__);
    /* fush */
    ESLog::flush();

    //signal(SIGBUS, SIG_DFL);
}


/* the main function  open the linstiong */
int main(int argc, char **argv){

    printf("hello world!\n");

    /* server */
    Server *server = Server::Instance();

    /* application */
    Application application;

    /* load the parameters of the server */
    server->LoadConfigure();

    /* load the application primitive for the server*/
    server->LoadApplicationTask( &application );

    /* init */
    server->Init();

    /* ignore the signal sig pipe , which is event by
       sending date to unconnected socket */
    signal(SIGPIPE, SIG_IGN);

    /* SIGSEGV handle */
    signal(SIGSEGV, SEGVlogbufflush);

    /* SIGBUS handle */
    signal(SIGBUS, BUSlogbufflush);

    /* startup the task Manager thread */
    server->StartupWorkManager();

    /* start up the io loop, for the noblock */
    server->StartupIOLoop();


    printf("hello world! The end!\n");

    return 1;
}


