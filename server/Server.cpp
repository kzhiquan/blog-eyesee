


#include "Server.h"
#include <stdio.h>

/* static data */
Server* Server::server = 0;
const char* Server::configureFile = "WConfigure.cg";

/* static method: single object, return the point to the Server */
Server* Server::Instance(){
    if(Server::server == 0 ) {
        Server::server = new Server();
    }

    return Server::server;
}

/* constructor */
Server::Server(){
}

/* destructor */
Server::~Server(){
}

/* get the site of the server */
Site & Server::GetSite(){
    return this->site;
}

/* get the ioloop of the server */
IOLoop & Server::GetIOLoop(){
    return this->ioLoop;
}

/* get the stream manager of the server */
StreamManager & Server::GetStreamManager(){
    return this->streamManager;
}


/* Get WorkManager object */
WorkManager & Server::GetWorkManager(){
    return this->workManager;
}

/* Get DbConnMg object */
DbConnMg   & Server::GetDbConnMg(){
    return this->dbConnMg;
}


/* Get Configure object */
Configure  & Server::GetConfigure(){
    return this->configure;
}


/* Get ESLog object */
ESLog   & Server::GetESLog(){
    return this->log;
}

/* load server parameters, if error, return < 0 */
int Server::LoadConfigure(){

    int result = 0;
    Server *server = Server::Instance();

    result = server->configure.LoadFromFile(server->configureFile);
    if( result < 0 ) {
        printf("LoadFromFile Error!\n");
    }

    return result;
}


/* load Application Task */
int Server::LoadApplicationTask( Task *task ){
    this->workManager.InitTaskPrimitive(task);
}

/* init the server after load the configure  */
int Server::Init(){

    int result = 0;

    /* log init */
    result  = this->log.Init();
    if( result < 0 ) {
        return result;
    }

    /* site init */
    result = this->site.Init();
    if( result < 0 ) {
        return result;
    }

    return result;
}

/* start up the work manager thread */
int Server::StartupWorkManager(){

    int result = 0;
    pthread_t tid;

    /* crerate the work thread */
    result = pthread_create( &tid, NULL, WorkManager::WorkManagerThread, (void *)(&(this->workManager)) );
    if( result < 0 ) {
        return result;
    }

    this->workManager.SetThreadID(tid);

    return result;
}


/* start up the io loop, for noblock io */
int Server::StartupIOLoop(){

    int result = 0;

    /* according to init parameter, create fd, create events array store
       the events happend from the epfd */
    result = this->ioLoop.Init();
    if( result < 0 ) {
        return result;
    }

    /* according to init parameter, start up site listen the netword */
    result = this->site.StartLisnNet();
    if( result < 0 ) {
        return result;
    }

    /* register the listen fd in the epoll */
    result = ioLoop.RegReadEventFd(this->site.GetSiteLisnFd());
    if( result < 0 ) {
        return result;
    }
    
    /* listen the io, and loop, until events happend, or blocked */
    result = this->ioLoop.Loopfd();
    if( result<0 ) {
        return result;
    }

}

