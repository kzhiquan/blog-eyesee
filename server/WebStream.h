
#ifndef _WEBSTREAM_H_
#define _WEBSTREAM_H_


#include <vector>
using namespace std;


#include "TcpConnection.h"
#include "RollBuff.h"
#include "Http.h"



class StreamManager;

enum WebStreamStatus{
    WSTREAM_DISPATHED,
    WSTREAM_UNINITILIZED,
    WSTREAM_UNDISPATHED,
    WSTREAM_HANDLED,
    WSTREAM_SIDLE,
    WSTREAM_READ,
    WSTREAM_WRITE,
    WSTREAM_ERROR
};

class Http;

/* web stream */
class WebStream
{
private:
    /* common info fields */
    StreamManager       *manager;
    TcpConnection       *tcpConn;
    Http                *http;
    WebStreamStatus     status;
    pthread_mutex_t     status_mutex;


    //buffer
    RollBuff            rollBuff;
    int                 curCnt;
    int                 totalCnt;
    off_t               fdwritecnt;
    int                 soreadcnt;

    /* stats desciption */
    static  char *wstreamstatusdesc[];
        
public:
    /* constructor*/
    WebStream();

    /* reset */
    int ReInit();

    /* reset http  */
    int ReSetHttp();

    /* get soreadcnt */
    int GetSoReadCnt();

    /* Set the WebStream Status */
    void SetStatus(WebStreamStatus status);

    /* Get the WebStream Status */
    WebStreamStatus GetStatus();

    /* Get Status desciption */
    char *GetStatusDesc();

    /* Get the http object embeded in the WebStream */
    Http *GetHttpPtr();

    /* Get RollBuf Ptr */
    RollBuff * GetRollBufPtr();

    /* Get TcpConnection */
    TcpConnection *GetTcpConn();

    /* Set the total Ccount*/
    int SetTotalCnt(int cnt );

    /* update the total Count */
    int IncrementTotalCnt(int cnt );

    /* set the TcpConnection of the webStream */
    void SetTcpConn( TcpConnection *tcpConn );

    /* set the status of the tcpConnection */
    int SetTcpConnStatus( TcpConnStatus status );

    /* set the the parter tcpConnection close */
    int SetTcpConnStatusCliClose();

    /* is the parter tcpConnection close */
    bool IsTcpConnStatusCliClose();

    /* is the webStream has the parser */
    bool IsHttpParserExist();

    /* is the http header paser ok */
    bool IsHttpParseHeaderOver();

    /* set the webStream parser */
    void SetHttpParser( Http *http );
    
    /* get the fd of the tcpConn */
    int GetTcpConnFd();

    /* parse the http header */
    int ParseHttpHeader();

    /* Close TcpConnection and free the space*/
    int FreeTcpConn();

    /* Free Http */
    int FreeHttp();

    /* according to the tcpConn, see the status */
    bool IsStatusIdle();

    /* according to the tcpConn, see the status */
    bool IsStatusRead();
    
    /* according to the tcpConn, see the status */
    bool IsStatusWrite();

    /* read data from the tcpConn into buf */
    int  StreamRead();

    /* write data from the writefd into tcpConn */
    int  StreamWrite();

    /* read data from the buf ptr */
    int StreamRead( const char *strBuf );

    /* write data from rollbuf to buf */
    char *StreamWrite( int *buflen );

    /* is parse over */
    bool IsHeaderReadOver();



};

#endif
