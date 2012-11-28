
#include "TcpConnection.h"
#include "ESLog.h"

#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>

/* status description */
char * TcpConnection::tcpstatusdesc[] = { "TCP_STATUS_NONE", "TCP_STATUS_CLOSED", "TCP_STATUS_LISTENING", 
                                           "TCP_STATUS_ESTABLISHED", "TCP_STATUS_FINWAIT2"};

/* constructor */
TcpConnection::TcpConnection(){

}

/* set the TcpConnection ip address */
void TcpConnection::SetIP( char *strIP ){
    this->ip.append(strIP);
}

/* get the tcpConnection ip address */
string & TcpConnection::GetIP(){
    return this->ip;
}

/* set the TcpConnection port */
void TcpConnection::SetPort( int port ){
    this->port = port;
}

/* get the tcpConnection port */
int TcpConnection::GetPort(){
    return this->port;
}


/* set the TcpConnection Status */
void TcpConnection::SetStatus( TcpConnStatus status ){
    this->status = status;
}

/* Get the TcpConnection Status */
TcpConnStatus TcpConnection::GetStatus(){
    return this->status;
}

/* Get TCP status desc */
char *TcpConnection::GetStatusDesc(){
    return TcpConnection::tcpstatusdesc[this->status];
}

/* Close the TcpConnection fd */
int TcpConnection::Closefd(){
    return close(this->fd);
}


/*  get the sockfd  */
int TcpConnection::GetTcpConnectionFd(){
    return this->fd;
}

/* set the sockfd */
void TcpConnection::SetTcpConnection( int fd ){
    this->fd = fd;
}


/* init the TcpConnectin as a listen socket fd, and liesten the network, be ready
       to accept connection from the network */
int TcpConnection::InitLisnSocket(int domain, int type, int protocol, int lisnBackLog ){

    int result = 0;

    struct sockaddr_in serveraddr;
    memset( &serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = domain;

    
    /* translate the ip format(127.0.0.1) into net */  
    ESLog::ISLog("site ip:%s\n",__FILE__, __LINE__, this->ip.c_str());  
    if( strcmp(this->ip.c_str(), "0" ) == 0 ) {
        serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    }
    else{
        result = inet_aton( this->ip.c_str(), &(serveraddr.sin_addr) );
        if( result < 0 ) {
            return -1;
        }
    }

    /* translate the port into net */
    ESLog::ISLog("site port:%d\n", __FILE__, __LINE__, this->port);
    serveraddr.sin_port = htons(this->port);

    /* get a socket */
    this->fd = socket( domain, type, protocol );

    /* set reuse ip port */
    int optvalue = 1;// open
    socklen_t socklen = sizeof(optvalue);
    result = setsockopt(this->fd, SOL_SOCKET, SO_REUSEADDR, &optvalue, socklen );
    if( result ) {
        return result;
    }

    /* bind the socket fd */
    result = bind( this->fd, (sockaddr *)&serveraddr, sizeof(serveraddr) );
    if( result < 0 ) {
        ESLog::ISLog("%s\n", __FILE__, __LINE__, strerror(errno));
        return -1;
    }

    /* listen the network */
    result  = listen( this->fd, lisnBackLog );
    if( result < 0 ) {
        return result;
    }

    result;
}
/* set non blocking fd */
int TcpConnection::SetNonBlock( ){
    
    int opts;

    opts = fcntl( this->fd, F_GETFL );
    if( opts < 0) {
        return -1;
    }

    opts = opts | O_NONBLOCK;

    if( fcntl(this->fd, F_SETFL, opts ) < 0 ) {
        return -1;
    }
}

/* listen fd accept a tcpConnection from the network,
   and return the TcpConnection established */
TcpConnection * TcpConnection::LisnFdAcceptConn(){

    int clientFd = 0;
    socklen_t sockaddrlen;

    struct sockaddr_in clientaddr;
    memset( &clientaddr, 0, sizeof(clientaddr));
    sockaddrlen = sizeof( clientaddr );

    /* accent */
    clientFd = accept( this->fd, (sockaddr *)&clientaddr, &sockaddrlen );
    if( clientFd < 0 ) {
        return NULL;
    }

    /* create an object */
    TcpConnection *clientConn = new TcpConnection(); 

    clientConn->SetIP( inet_ntoa(clientaddr.sin_addr) );
    clientConn->SetPort( ntohs(clientaddr.sin_port));
    clientConn->fd = clientFd;

    clientConn->SetStatus(TCP_STATUS_ESTABLISHED);
    
    return clientConn;
}
