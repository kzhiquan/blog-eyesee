
#include "Server.h"
#include "IOLoop.h"
#include "WebStream.h"
#include <string.h>
#include <errno.h>
#include <stdio.h>


/*constructor */
IOLoop::IOLoop(){
    this->epfd = 0;
    this->maxEvents = 0;
    this->events = 0;
}

/* destructor */
IOLoop::~IOLoop(){

    if( this->events != 0 ) {
        delete [] events;
    }
}

/* set the maxEvents of the events array */
void IOLoop::SetMaxEvents(int maxEvents){
    this->maxEvents = maxEvents;
}

/* set the pollSize of the epfd */
void IOLoop::SetPollSize(int pollSize){
    this->pollSize = pollSize;
}

/* init, create epoll fd */
int IOLoop::Init(){

    /* epoll fd */
    this->epfd = epoll_create( this->pollSize );
    if( this->epfd < 0 ) {
        return -1;
    }

    /* events array that store the happend events */
    if( this->events == 0 ) {
        this->events = new epoll_event [this->maxEvents];
        memset( this->events, 0, sizeof(epoll_event)*(this->maxEvents) );
    }
}




/* add fd to epoll listen the read event */
int IOLoop::RegReadEventFd( int fd){
    
    int result = 0;

    /* set the event */
    memset( &(this->ev), 0, sizeof(this->ev));
    this->ev.data.fd = fd;
    //this->ev.events = EPOLLIN|EPOLLET;
    this->ev.events = EPOLLIN;
    //ESLog::ISLog("events:%d\n", __FILE__, __LINE__, this->ev.events);
    ESLog::ISLog("reg read Event %d\n", __FILE__, __LINE__,  fd );
    result  = epoll_ctl( this->epfd, EPOLL_CTL_ADD, fd, &(this->ev) );
    if( result < 0 ) {
        ESLog::ISLog("add fd to epoll error :%d\n", __FILE__, __LINE__, fd);
        return result;
    }

    return result;
}

/* delete the fd */
int IOLoop::DelWriteEventFd( int fd ){

    int result = 0;

    /* set the event */
    memset( &(this->ev), 0, sizeof(this->ev));
    this->ev.data.fd = fd;
    //this->ev.events = EPOLLOUT|EPOLLET;
    this->ev.events = EPOLLOUT;
    //ESLog::ISLog("events:%d\n", __FILE__, __LINE__, this->ev.events);
    //ESLog::ISLog("reg read Event %d\n", __FILE__, __LINE__,  fd );
    //result  = epoll_ctl( this->epfd, EPOLL_CTL_ADD, fd, &(this->ev) );
    result = epoll_ctl( this->epfd, EPOLL_CTL_DEL, fd, &(this->ev));
    if( result < 0 ) {
        ESLog::ISLog("EPOLL delete error\n", __FILE__, __LINE__);
        return result;
    }

    return result;
}

/* delete the fd */
int IOLoop::DelEventFd( int index, int fd ){

    int result = 0;
    
    //this->ev.
    /* set the event */
    memset( &(this->ev), 0, sizeof(this->ev));
    //this->ev.data.fd = this->events[index].data.fd;
    this->ev.data.fd = fd;
    //this->ev.events = EPOLLOUT|EPOLLET;
    this->ev.events = this->events[index].events;
    //ESLog::ISLog("events:%d\n", __FILE__, __LINE__, this->ev.events);
    //ESLog::ISLog("reg read Event %d\n", __FILE__, __LINE__,  fd );
    //result  = epoll_ctl( this->epfd, EPOLL_CTL_ADD, fd, &(this->ev) );
 
    ESLog::ISLog("delete error epoll fd:%d\n", __FILE__, __LINE__, this->ev.data.fd);

    result = epoll_ctl( this->epfd, EPOLL_CTL_DEL, this->ev.data.fd, &(this->ev));
    if( result < 0 ) {
        ESLog::ISLog("EPOLL delete error %s\n", __FILE__, __LINE__, strerror(errno));
        return result;
    }

    result = close(this->ev.data.fd);
    if( result < 0 ) {
        ESLog::ISLog("close fd error %s\n", __FILE__, __LINE__, strerror(errno));
        return result;
    }

    return result;
}


/* delete the fd */
int IOLoop::DelReadEventFd( int fd ){

    int result = 0;

    /* set the event */
    memset( &(this->ev), 0, sizeof(this->ev));
    this->ev.data.fd = fd;
    //this->ev.events = EPOLLIN|EPOLLET;
    this->ev.events = EPOLLIN;
    //ESLog::ISLog("events:%d\n", __FILE__, __LINE__, this->ev.events);
    //ESLog::ISLog("reg read Event %d\n", __FILE__, __LINE__,  fd );
    //result  = epoll_ctl( this->epfd, EPOLL_CTL_ADD, fd, &(this->ev) );
    result = epoll_ctl( this->epfd, EPOLL_CTL_DEL, fd, &(this->ev));
    if( result < 0 ) {
        ESLog::ISLog("EPOLL delete error\n", __FILE__, __LINE__);
        return result;
    }

    return result;
}


/* add fd to epoll listen the read event */
int IOLoop::RegReadEventFd( int fd, void *data ){
    
    int result = 0;

    /* set the event */
    memset( &(this->ev), 0, sizeof(this->ev));
    this->ev.data.ptr = data;
    //this->ev.events = EPOLLIN|EPOLLET;
    this->ev.events = EPOLLIN;
    //ESLog::ISLog("events:%d\n", __FILE__, __LINE__, this->ev.events);
    ESLog::ISLog("reg as read event:%d, data:%d\n", __FILE__, __LINE__, fd, data );
    result  = epoll_ctl( this->epfd, EPOLL_CTL_ADD, fd, &(this->ev) );
    if( result < 0 ) {
        ESLog::ISLog("add fd to epoll error :%d :%s\n", __FILE__, __LINE__, fd, strerror(errno));
        return result;
    }

    return result;
}

/* modify listen event of fd in the epoll */
int IOLoop::ModToReadEventFd( int fd, void * data ){
    int result = 0;

    /* set the event */
    memset( &(this->ev), 0, sizeof(this->ev));
    this->ev.data.ptr = data;
    //this->ev.events = EPOLLIN|EPOLLET;
    this->ev.events = EPOLLIN;
    //ESLog::ISLog("events:%d\n", __FILE__, __LINE__, this->ev.events);

    /* modify to the  write */
    result  = epoll_ctl( this->epfd, EPOLL_CTL_MOD, fd, &(this->ev) );
    if( result < 0 ) {
        ESLog::ISLog("ModToReadEventFd fd %d ERR:%s\n", __FILE__, __LINE__, fd, strerror(errno) );
    }

    return result;
}

/* add fd to epoll listen to write event */
int IOLoop::RegWriteEventFd( int fd ){
    int result = 0;

    /* set the event */
    memset( &(this->ev), 0, sizeof(this->ev));
    this->ev.data.fd = fd;
    //this->ev.events = EPOLLOUT|EPOLLET;
    this->ev.events = EPOLLOUT;
    //ESLog::ISLog("events:%d\n", __FILE__, __LINE__, this->ev.events);

    result  = epoll_ctl( this->epfd, EPOLL_CTL_ADD, fd, &(this->ev) );
    if( result < 0 ) {
        return result;
    }

    return result;
}

/* modify listen event of fd in the epoll */
int IOLoop::ModToWriteEventFd( int fd ){
    int result = 0;
    

    /* set the event */
    memset( &(this->ev), 0, sizeof(this->ev));
    this->ev.data.fd = fd;
    //this->ev.events = EPOLLOUT|EPOLLET;
    this->ev.events = EPOLLOUT;
    //ESLog::ISLog("events:%d\n", __FILE__, __LINE__, this->ev.events);

    /* modify to the  write */
    result  = epoll_ctl( this->epfd, EPOLL_CTL_MOD, fd, &(this->ev) );

    return result;
}

/* modify listen event of fd in the epoll */
int IOLoop::ModToWriteEventFd( int fd, void * data ){
    int result = 0;

    /* set the event */
    memset( &(this->ev), 0, sizeof(this->ev));

    this->ev.data.ptr = data;
    //this->ev.events = EPOLLOUT|EPOLLET;
    this->ev.events = EPOLLOUT;

    //ESLog::ISLog("modify to write event:%d\n", __FILE__, __LINE__, fd );
    //ESLog::ISLog("events:%d\n", __FILE__, __LINE__, this->ev.events);

    /* modify to the  write */
    result  = epoll_ctl( this->epfd, EPOLL_CTL_MOD, fd, &(this->ev) );


    return result;
}

/* loop */
int IOLoop::Loopfd(){

    /* loop ,until block */
    while(1) {
        ESLog::ISLog("LOOP FD CYCLE START\n", __FILE__, __LINE__);

        int result = 0;
        int nfds = 0;
        /* wait the events */
        nfds = epoll_wait( this->epfd, this->events, this->maxEvents,-1 );
        
        ESLog::ISLog("Loofd event come, there is %d number total events\n", __FILE__, __LINE__, nfds );

        /* tackle the events happend */
        for( int i = 0; i < nfds; i++ ) {
            ESLog::ISLog("event data(fd) %d\n",__FILE__, __LINE__, this->events[i].data.fd );
            ESLog::ISLog("event events %d\n",  __FILE__, __LINE__,this->events[i].events );

            /* if the listen fd event happend, than is to say a new tcp connection establish */
            if( this->events[i].data.fd == Server::Instance()->GetSite().GetSiteLisnFd() ) {
                
                ESLog::ISLog("AN NEW SESSION START\n", __FILE__, __LINE__);

                /* accept a new tcp connection */
                TcpConnection *clientConn = Server::Instance()->GetSite().AcceptTcpConn();
                if( clientConn == NULL ) {
                    continue;
                }

                /* set the connection is nonblocking */
                result = clientConn->SetNonBlock();
                if( result < 0 ) {
                    continue;
                }

                /* get an idle webStream */
                WebStream *idleStream = ((Server::Instance())->GetStreamManager()).GetIdleWebStream();

                ESLog::ISLog("now idleStream size:%d\n", __FILE__, __LINE__,Server::Instance()->GetStreamManager().GetIdleStreamSize());

                /* set the tcpConnection of the idleStream */
                idleStream->SetTcpConn(clientConn);

                /* add the idleWebStream into the readWebStram
                   and it will become an readable webStream */
                //Server::Instance()->GetStreamManager().AddReadWebStream( idleStream );

                /* listen clientconnection fd for read event in epoll */
                result = this->RegReadEventFd(clientConn->GetTcpConnectionFd(), (void *)idleStream );
                if( result < 0 ) {
                    continue;
                }

                idleStream->SetStatus(WSTREAM_UNINITILIZED);

                /* add into the readStream */
                Server::Instance()->GetStreamManager().AddReadWebStream( idleStream ); 

                ESLog::ISLog("remote bounded idlestream tcpconn fd: %d \n", __FILE__, __LINE__, idleStream->GetTcpConnFd());
                ESLog::ISLog("remote ip:%s, port:%d\n", __FILE__, __LINE__, idleStream->GetTcpConn()->GetIP().c_str(), idleStream->GetTcpConn()->GetPort());

                ESLog::ISLog("AN NEW SESSION END\n", __FILE__, __LINE__);
                //continue;

            }
            /* if the events happend, fd can be read */
            else if( this->events[i].events & EPOLLIN ) {

                ESLog::ISLog("AN READ EVENT START\n",__FILE__, __LINE__);

                int result = 0;

                 /* get the parameter from the kernel , that we pass in the former step */
                WebStream *readStream = (WebStream * )events[i].data.ptr;

                readStream->SetStatus(WSTREAM_READ);

                ESLog::ISLog("read event from fd: %d \n",__FILE__, __LINE__, readStream->GetTcpConnFd());
                ESLog::ISLog("read stream bound ip:%s, port:%d\n", __FILE__, __LINE__, readStream->GetTcpConn()->GetIP().c_str(), readStream->GetTcpConn()->GetPort());

                /* we tackle the read event, read the data from the kernel into webStream buffer */
                result = readStream->StreamRead();
                if( result == -1 ) {
                    ESLog::ISLog("there is no data to read\n", __FILE__, __LINE__);
                    ESLog::ISLog("AN READ EVENT END, THIS TIME HAS NO DATA TO READ\n",__FILE__, __LINE__);
                    continue; // we have to tackle , there is data in the remote, there is no data in the kernel
                              // or in the user space, we have no space to read.
                }
                else if( result == 0 || result == -2) {// read compelete, the client close the tcpConnection, after write, the server close too.

                    ESLog::ISLog("read complete, will close the session\n", __FILE__, __LINE__);

                    ESLog::ISLog("tcpConnection:%d fd, set fin_wait_2 status !\n", __FILE__, __LINE__, readStream->GetTcpConnFd());

                    /* change the state of the tcpConnection of webStream */
                    readStream->SetTcpConnStatusCliClose();

                    ESLog::ISLog("http ptr:%u\n", __FILE__, __LINE__, readStream->GetHttpPtr());
                    ESLog::ISLog("TcpConnection status:%s\n", __FILE__, __LINE__, readStream->GetTcpConn()->GetStatusDesc());
                    ESLog::ISLog("readStream this time read count:%d\n", __FILE__, __LINE__, readStream->GetSoReadCnt());

                    if( readStream->GetHttpPtr() != NULL ) {
                        ESLog::ISLog("http status:%s\n", __FILE__, __LINE__, readStream->GetHttpPtr()->GetStatusDesc());
                    }

                    //ESLog::ISLog("http Status:%s\n", __FILE__, __LINE__, readStream->GetHttpPtr()->GetStatusDesc());
                    //if( readStream->GetHttpPtr() != NULL && readStream->GetHttpPtr()->GetStatus() == HTTP_RESSEND_OVER ) {
                    if( /*(readStream->GetHttpPtr() == NULL && readStream->GetSoReadCnt() == 0 ) || */
                                (readStream->GetHttpPtr() != NULL && readStream->GetHttpPtr()->GetStatus() == HTTP_RESSEND_OVER) ) {

                        /* delete from the read pool */
                        Server::Instance()->GetStreamManager().DeleteReadWebStream(readStream);

                        ESLog::ISLog("http have send over, free resouce start\n", __FILE__, __LINE__);

                        //this->DelReadEventFd(readStream->GetTcpConnFd());

                        /* free the tcpConnection in the writeStream */
                        readStream->FreeTcpConn();

                        /* free http */
                        //readStream->FreeHttp();

                        /* roll reinit*/
                        readStream->ReInit();

                        //status
                        readStream->SetStatus(WSTREAM_UNINITILIZED);

                        /* add into the idle pool */
                        Server::Instance()->GetStreamManager().AddIdleStream(readStream);

                        ESLog::ISLog("http have send over, free resouce end\n", __FILE__, __LINE__);
                    }


                    ESLog::ISLog("AN READ EVENT END, THR REMOTE CLOSED\n",__FILE__, __LINE__);

                    //continue;

                }


                ESLog::ISLog("AN READ EVENT END\n",__FILE__, __LINE__);

            }

            /* if the events happend, fd can be write */
            else if( this->events[i].events & EPOLLOUT ) {

                ESLog::ISLog("AN WRITE EVENT START\n", __FILE__, __LINE__);

                int result = 0;

                /* get the parameter from the kernel, we pass from the former step */
                WebStream *writeStream = (WebStream *)events[i].data.ptr;

                ESLog::ISLog("write event from fd: %d \n", __FILE__, __LINE__, writeStream->GetTcpConnFd());
                ESLog::ISLog("write stream bound ip:%s, port:%d\n", __FILE__, __LINE__, writeStream->GetTcpConn()->GetIP().c_str(), writeStream->GetTcpConn()->GetPort());
                /* write until return */
                result = writeStream->StreamWrite();
                ESLog::ISLog("write result:%d\n", __FILE__,__LINE__, result);
                if( result == -1 ) {
                    ESLog::ISLog("There is still some data to write\n", __FILE__, __LINE__);
                    continue;  // we still have data to write
                }
                else if( result == 0 || result == -2 ) {
                    /* write complete, we would read in the future, but according to http, the server must close the connection
                    that represent the http is over this time */
                    // the partner on the remote doesn't close, just to still read in the server side.
                    // change the webStream status write->read
                    ESLog::ISLog("now the data hava write complete this time\n",__FILE__, __LINE__);
                    ESLog::ISLog("tcp status:%s\n", __FILE__, __LINE__, writeStream->GetTcpConn()->GetStatusDesc());
                    /* close */
                    if( (writeStream->GetTcpConn()->GetStatus() == TCP_STATUS_FINWAIT2) || result == -2 ) {  //just close write over

                        ESLog::ISLog("remote have close tcpconnenction, we write over and close too ,start\n", __FILE__, __LINE__);

                        /* delete from the read pool */
                        Server::Instance()->GetStreamManager().DeleteReadWebStream(writeStream);

                        //this->DelWriteEventFd(writeStream->GetTcpConnFd());

                        /* delete */
                        //this->DelWriteEventFd(writeStream->GetTcpConnFd());

                        /* free the tcpConnection in the writeStream */
                        writeStream->FreeTcpConn();

                        /* free http */
                        //writeStream->FreeHttp();

                        /* roll reinit*/
                        writeStream->ReInit();

                        //status
                        writeStream->SetStatus(WSTREAM_UNINITILIZED);

                        /* add into the idle pool */
                        Server::Instance()->GetStreamManager().AddIdleStream(writeStream);

                        

                        ESLog::ISLog("remote have close tcpconnenction, we write over and close too ,end\n", __FILE__, __LINE__);
                     }
                     else{
                        ESLog::ISLog("Modify write event fd :%d to read event\n",__FILE__, __LINE__, writeStream->GetTcpConnFd());

                        writeStream->GetHttpPtr()->SetStatus(HTTP_RESSEND_OVER);
                        this->ModToReadEventFd( writeStream->GetTcpConnFd(), (void *)writeStream );
                        writeStream->ReInit();
                     }
                                           
                }

                ESLog::ISLog("AN WRITE EVENT END\n", __FILE__, __LINE__);
                    
            } 

            /* */
            else if( this->events[i].events & EPOLLERR ) {

                ESLog::ISLog("EPOLL Error hapends start \n", __FILE__, __LINE__);
                
                int result = 0;
                
                /* get the parameter from the kernel, we pass from the former step */
                WebStream *errStream = (WebStream *)events[i].data.ptr;

                ESLog::ISLog("error event from fd: %d \n", __FILE__, __LINE__, errStream->GetTcpConnFd());
                ESLog::ISLog("error stream bound ip:%s, port:%d\n", __FILE__, __LINE__, errStream->GetTcpConn()->GetIP().c_str(), errStream->GetTcpConn()->GetPort());

                /* delete fd */
                result = this->DelEventFd(i, errStream->GetTcpConnFd());
                if( result < 0 ) {
                    ESLog::ISLog("delete fd from epoll error!\n", __FILE__, __LINE__);
                    continue;
                }

                /* free resource */
                /* delete from the read pool */
                Server::Instance()->GetStreamManager().DeleteReadWebStream(errStream);

                /* free the tcpConnection in the writeStream */
                errStream->FreeTcpConn();

                /* roll reinit*/
                errStream->ReInit();

                //status
                errStream->SetStatus(WSTREAM_UNINITILIZED);

                /* add into the idle pool */
                Server::Instance()->GetStreamManager().AddIdleStream(errStream);

                ESLog::ISLog("EPOLL Error hapends end \n", __FILE__, __LINE__);
                
            }
           

        }

        /* add the parser http head in the taskStream */
        //Server::Instance()->GetStreamManager().ClipReadToTaskStream();

        /* add taskStream to the writeStream */
        //Server::Instance()->GetStreamManager().ClipTaskToWriteStream();

        Server::Instance()->GetStreamManager().ClipReadToIdleStream();

        /* triger the event condition, one loop, if the  readStream or taskStream > 0 */
        if( Server::Instance()->GetStreamManager().IsExistReadStatusStream() && Server::Instance()->GetWorkManager().GetHasWorkToDo() == false
            /*|| Server::Instance()->GetStreamManager().GetTaskWebStreamSize() > 0*/ ) {

            ESLog::ISLog("TRIGER WORKMANAGER START\n", __FILE__, __LINE__);

            //ESLog::ISLog("the read Stream size %d\n",__FILE__, __LINE__, Server::Instance()->GetStreamManager().GetReadWebStreamSize());

            /* set the WorkManager has work to do flag */
            Server::Instance()->GetWorkManager().SetHasWorkToDo(true);

            /* triger the sigal */
            Server::Instance()->GetWorkManager().WorkConditionSignal();

            ESLog::ISLog("TRIGER WORKMANAGER END\n", __FILE__, __LINE__);
        }

        ESLog::ISLog("LOOP FD CYCLE END\n", __FILE__, __LINE__);

    }


    return 0;
}
