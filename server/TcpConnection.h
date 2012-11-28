
#ifndef _TCPCONNECTION_H_
#define _TCPCONNECTION_H_

#include <string>
using namespace std;


/* TcpConnStatus */
enum TcpConnStatus{
    TCP_STATUS_NONE, //none
    TCP_STATUS_CLOSED,//closed,
    TCP_STATUS_LISTENING, //listening,
    TCP_STATUS_ESTABLISHED, //established,
    TCP_STATUS_FINWAIT2 //fin_wait_2
};

/* TcpConnection */
class TcpConnection{
private:
    int             fd;             /* the sockfd */
    int             port;           /* the connection establish on the port */
    string          ip;             /* the connection establis on the ip */
    TcpConnStatus   status;         /* the connection status */

    static char * tcpstatusdesc[];  /* status desc */
public:
    TcpConnection();

    /* set the ip */
    void SetIP( char *strIP );
    /* get the ip address */
    string & GetIP();

    /* set the port */
    void SetPort( int port );
    /* get the port */
    int GetPort();

    /* set the TcpConnection status */
    void SetStatus( TcpConnStatus status );

    /* Get TCP status desc */
    char *GetStatusDesc();

    /* Get the TcpConnection Status */
    TcpConnStatus GetStatus();

    /* get the sockfd */
    int GetTcpConnectionFd();
    /* set ths sockfd */
    void SetTcpConnection( int fd );

    /* set not blocking fd */
    int SetNonBlock();

    /* close the TcpConnection of fd */
    int Closefd();

    /* init the TcpConnectin as a listen socket fd, and liesten the network, be ready
       to accept connection from the network */
    int InitLisnSocket(int domain, int type, int protocol, int lisnBackLog );

    /* listen fd accept a tcpConnection from the network, and return the TcpConnection established */
    TcpConnection * LisnFdAcceptConn();

};



#endif
