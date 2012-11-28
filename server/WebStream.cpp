
#include "WebStream.h"
#include "Server.h"
#include "TcpConnection.h"
#include <errno.h>
#include <unistd.h>
#include <sys/sendfile.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <netinet/in.h>



/* stats desciption */
char *WebStream::wstreamstatusdesc[] = { 
    "WSTREAM_DISPATHED",
    "WSTREAM_UNINITILIZED",
    "WSTREAM_UNDISPATHED",
    "WSTREAM_HANDLED",
    "WSTREAM_SIDLE",
    "WSTREAM_READ",
    "WSTREAM_WRITE",
    "WSTREAM_ERROR"
};


/* constructor */
WebStream::WebStream(){
    
    this->tcpConn = NULL;
    this->http = NULL;
    this->totalCnt = 0;
    this->fdwritecnt = 0;
    this->curCnt = 0;
    this->soreadcnt = 0;
    this->status = WSTREAM_UNINITILIZED;
    this->manager = &(Server::Instance()->GetStreamManager());
    pthread_mutex_init(&status_mutex, NULL);
}

/* Get the Status */
WebStreamStatus WebStream::GetStatus(){
    //printf("webStreamstatus:%d\n", this->status);
    //pthread_mutex_lock(&status_mutex);
    return this->status;
    //pthread_mutex_unlock(&status_mutex);
}

/* Get Status desciption */
char *WebStream::GetStatusDesc(){
    return WebStream::wstreamstatusdesc[this->status];
}


/* get soreadcnt */
int WebStream::GetSoReadCnt(){
    return this->soreadcnt;
}


/* Set the WebStream Status */
void WebStream::SetStatus(WebStreamStatus status){
    //printf("webStreamstatus:%d\n", this->status);
    pthread_mutex_lock(&status_mutex);
    this->status = status;
    pthread_mutex_unlock(&status_mutex);
}

/* Get the http object embeded in the WebStream */
Http *WebStream::GetHttpPtr(){
    return this->http;
}

/* Get TcpConnection */
TcpConnection *WebStream::GetTcpConn(){
    return this->tcpConn;
}


/* is WebStream can be idle */
bool WebStream::IsStatusIdle(){
    bool result = false;

    /* there is no tcpconn bind to webStream */
    if( this->tcpConn == NULL ) {
        result = true;
    }

    return result;
}

/* set the tcpConn of the webStream */
void WebStream::SetTcpConn( TcpConnection * tcpConn ){
    this->tcpConn = tcpConn;
}

/* set the status of the tcpConnection */
int WebStream::SetTcpConnStatus( TcpConnStatus status ){
    int result = 0;
    this->tcpConn->SetStatus( status );
    return result;
}

/* set the the parter tcpConnection close */
int WebStream::SetTcpConnStatusCliClose(){
    int result = 0;

    
    this->tcpConn->SetStatus(TCP_STATUS_FINWAIT2);

    return result;
}

/* is the parter tcpConnection close */
bool WebStream::IsTcpConnStatusCliClose(){

    bool result = false;

    if( this->tcpConn->GetStatus() ==  TCP_STATUS_FINWAIT2 )  {
        result = true;
    }

    return result;
}

/* is the webStream has the parser */
bool WebStream::IsHttpParserExist(){
    
    bool result = false;

    if( this->http != NULL ) {
        result = true;
    }

    return result;
}

/* set the webStream parser */
void WebStream::SetHttpParser( Http *http ){
    this->http = http;
}

/* Set the total Ccount*/
int WebStream::SetTotalCnt(int cnt ){
    this->totalCnt = cnt;
}

/* update the total Count */
int WebStream::IncrementTotalCnt(int cnt ){
    this->totalCnt += cnt;
}

/* parse the http header */
int WebStream::ParseHttpHeader(){
    int     result = 0;
    char    *strBuf = NULL;
    int     bufLength = 0;

    ESLog::ISLog("PARSE AN HTTP HEAD START\n", __FILE__, __LINE__);

    /* Get the buf to write */
    strBuf = this->rollBuff.GetPerfectWriteBufPtr(&bufLength);
    if( strBuf == NULL ) {  // there is no data to read
        ESLog::ISLog("there is no data to get in rollbuf when parse http header\n", __FILE__, __LINE__);
        this->GetHttpPtr()->IncLostCnt();
        if( this->GetHttpPtr()->GetLostCnt() > 5 ) {
            ESLog::ISLog("free resource when lost http parse get data lost\n", __FILE__, __LINE__);
            return -1;
        }
        return result;
    }

    //for(int i = 0; i < bufLength; i++ ) {
    //    printf("%c", strBuf[i]);
    //}
    //printf("\n");
    /* parser, return the char num parse */
    result = this->http->ParseHeader(strBuf, bufLength);
    if( result < 0 ) {
        ESLog::ISLog("parse http head failed\n", __FILE__, __LINE__);
        return result; // happend an error
    }

    /* update the char write from rollBuff */
    this->rollBuff.UpdateFirstIndex( result );

    ESLog::ISLog("PARSE AN HTTP HEAD END\n", __FILE__, __LINE__);


    return result;
}


/* is the http header paser ok */
bool WebStream::IsHttpParseHeaderOver(){
       
    bool result = false;

    if( this->http->GetStatus() == 1 ) {
        result = true;
    }

    return result;
}

/* get the tcpConnection fd */
int WebStream::GetTcpConnFd(){
    if( this->tcpConn ) {
        return this->tcpConn->GetTcpConnectionFd();
    }
    
    return 0;
}



/* Close TcpConnection and free the space*/
int WebStream::FreeTcpConn(){

    int result = 0;

    result = this->tcpConn->Closefd();
    if( result < 0 ) {
        return result;
    }

    if( this->tcpConn ) {
        delete this->tcpConn;
        this->tcpConn = NULL;
    }
    
    if( this->http ) {
        delete this->http;
        this->http = NULL;
    }

    this->totalCnt = 0;
    this->curCnt = 0;
    this->fdwritecnt = 0;
    this->soreadcnt = 0;

    return result;
}

/* free Http */
int WebStream::FreeHttp(){
    if( this->http) {
        delete this->http;
        this->http = NULL;
    }
}

/* Close TcpConnection and free the space*/
int WebStream::ReInit(){

    int result = 0;

    this->totalCnt = 0;
    this->curCnt = 0;
    this->fdwritecnt = 0;
    this->soreadcnt = 0;
    //this->status = WSTREAM_UNINITILIZED,

    this->rollBuff.ReInit();

    return result;
}

/* reset http  */
int WebStream::ReSetHttp(){
}


/* is WebStream can be read */
bool WebStream::IsStatusRead(){

}


/* is WebStream can write */
bool WebStream::IsStatusWrite(){

}

/* Get RollBuf Ptr */
RollBuff * WebStream::GetRollBufPtr(){
    return &(this->rollBuff);
}

/* stream start read, if return >0, we still hava data to read
   if return 0, there is no data to read, it's end */
int WebStream::StreamRead(){

 /* read unitl end, this is a nonblock fd, if there is not data, just return */
    ESLog::ISLog("WEBSTREAM READ FD START\n", __FILE__, __LINE__);
    this->soreadcnt = 0;
    while( 1 ){
        int n = 0; /* record the bytes read from the kernel onetime */
        char *strTmpBuf = NULL;
        int  rollBackPos = -1;

        ESLog::ISLog("start loog read rollbuff first:%d, last:%d, curtotal:%d\n\n", __FILE__, __LINE__, this->rollBuff.GetFirstIndex(), this->rollBuff.GetLastIndex(), this->curCnt);
        /* Get the perfect block buff from the rollbuffer */
        strTmpBuf = this->rollBuff.GetPerfectReadBufPtr(1024, &rollBackPos );
        if( strTmpBuf == NULL ) {
            ESLog::ISLog("there is no space in rollbuf\n", __FILE__, __LINE__);
            return -1;
        }

        /* read from the kernel, max bytes is readBlock */
        n = read( this->GetTcpConnFd(), (void *)strTmpBuf, 1024 );
        if( n < 0 ) {
            ESLog::ISLog("Stream read:%s\n", __FILE__, __LINE__, strerror(errno));
            if( errno == EAGAIN ) { // now this read event, we have tackle complete, but the data is not end;
                return -1; //error happend
            }
            return -2;
        }
        else if( n == 0 ) {
            ESLog::ISLog("remote close!\n", __FILE__, __LINE__);
            return 0;  //normal terminate, client close socket 
        }
        
        string tmp;
        tmp.append(strTmpBuf, n);
        ESLog::ISLog("read fd %d read data to rollbuf:%s\n",__FILE__, __LINE__, this->GetTcpConnFd(), tmp.c_str());




        /* update the have read position */
        if( rollBackPos >= 0 ) {
            this->rollBuff.ReadTriggerRollBack();
        }
        this->rollBuff.UpdateLastIndex( n );
        this->curCnt += n;
        this->soreadcnt += n;

        ESLog::ISLog("end loog read rollbuff first:%d, last:%d, curtotal:%d\n\n", __FILE__, __LINE__, this->rollBuff.GetFirstIndex(), this->rollBuff.GetLastIndex(), this->curCnt);
    }

    ESLog::ISLog("current read event read count:%d\n", __FILE__, __LINE__, this->soreadcnt);
    ESLog::ISLog("WEBSTREAM READ FD END\n", __FILE__, __LINE__);

}


/* read data from the buf ptr
   return the number of char to read this time */
int WebStream::StreamRead( const char *strBuf ){

    int total = strlen(strBuf); /* record the bytes read from the kernel onetime */
    int n = 0;

    ESLog::ISLog("WEBSTREAM READ BUF START\n", __FILE__, __LINE__);

    /* read unitl end, this is a nonblock fd, if there is not data, just return */
    while( 1 ){
        ESLog::ISLog("before read total:%d, curCnt:%d\n", __FILE__, __LINE__, total, this->curCnt);
        ESLog::ISLog("start Rollbuf first index:%d, last index:%d\n", __FILE__, __LINE__, this->rollBuff.GetFirstIndex(), this->rollBuff.GetLastIndex());

        char *strTmpBuf = NULL;
        int  rollBackPos = -1;
        int  m = 0; //this loop bytes read

        /* Get the perfect block buff from the rollbuffer */
        strTmpBuf = this->rollBuff.GetPerfectReadBufPtr(1024, &rollBackPos );
        if( strTmpBuf == NULL ) {
            ESLog::ISLog("rollbuf have no space to read\n", __FILE__, __LINE__);
            return n; //return the n number byte read 
        }

        //ESLog::ISLog("copy to data to rollBuf start\n", __FILE__, __LINE__);
        /* updata position */
        if( total > 1024 ) {
            memcpy( strTmpBuf, strBuf+n, 1024);
            m = 1024;
            n += m;
            total -= m;
            
        }
        else{
            memcpy( strTmpBuf, strBuf+n, total);
            m = total;
            n += m;
            total -= m;
        }

        //ESLog::ISLog("copy to data to rollBuf end\n", __FILE__, __LINE__);

        //strTmpBuf[m] = '\0';

        ESLog::ISLog("m:%d, n:%d, total:%d\n", __FILE__, __LINE__, m, n, total );
        //ESLog::ISLog("SRC BUF:%s END\n", __FILE__, __LINE__, strBuf);
        string temp;
        temp.append(strTmpBuf, m);
        ESLog::ISLog("read data to rollbuffer: %s\n", __FILE__, __LINE__, temp.c_str());

        /* update the have read position */
        if( rollBackPos >= 0 ) {
            this->rollBuff.ReadTriggerRollBack();
        }

        this->rollBuff.UpdateLastIndex( m );
        ESLog::ISLog("rollbuff lastindex:%d\n", __FILE__, __LINE__, this->rollBuff.GetLastIndex());

        this->curCnt += m;

        ESLog::ISLog("after read total:%d, curCnt:%d\n", __FILE__, __LINE__, total, this->curCnt);
        ESLog::ISLog("after Rollbuf first index:%d, last index:%d\n", __FILE__, __LINE__, this->rollBuff.GetFirstIndex(), this->rollBuff.GetLastIndex());

        if(  total <= 0 ) {
            //this->curCnt = 0; //just for sendfile fd record.
            break;
        }
    }

    //this->totalCnt += this->curCnt;

    //this->curCnt = 0;

    ESLog::ISLog("WEBSTREAM READ BUF END\n", __FILE__, __LINE__);

    return n;

}

/* write data from rollbuf to buf */
char *WebStream::StreamWrite( int *buflen ){
    char *buf = NULL;
    *buflen = 0;

    buf = this->rollBuff.GetPerfectWriteBufPtr(buflen);

    return buf;
}


/* is parse over */
bool WebStream::IsHeaderReadOver(){
    int result = false;

    if( this->http->GetStatus() == 1 ) {
        result = true;
    }

    return result;
}

/* stream start write, if return >0, we still have data to write,
   if return = 0, we have no data to write */
int WebStream::StreamWrite(){

    ESLog::ISLog("WEBSTREAM WRITE START\n", __FILE__, __LINE__);

    ESLog::ISLog("webStream start point totalCnt:%d, curCnt:%d\n", __FILE__, __LINE__, this->totalCnt, this->curCnt);

    ESLog::ISLog("rollBuf filled size:%d\n", __FILE__, __LINE__, this->rollBuff.GetFilledSize());
    ESLog::ISLog("rollBuf first:%d last:%d\n", __FILE__, __LINE__, this->rollBuff.GetFirstIndex(), this->rollBuff.GetLastIndex()); //error
    
    /* we have the data in the buffer */
    if( this->rollBuff.GetFilledSize() > 0 ){
        while(1) {
            int n = 0;
            char * strTmpBuf = NULL;
            int    writeSpan = 0;

            ESLog::ISLog("rollBuf first:%d last:%d\n", __FILE__, __LINE__, this->rollBuff.GetFirstIndex(), this->rollBuff.GetLastIndex()); //error
            /* get the write buffer ptr */
            strTmpBuf = this->rollBuff.GetPerfectWriteBufPtr( &writeSpan );
            if( strTmpBuf == NULL ) {
                //this->curCnt = 0;
                ESLog::ISLog("Now there is no date in buffer to write \n",__FILE__, __LINE__);
                ESLog::ISLog("totalCnt:%d\n",__FILE__, __LINE__, this->totalCnt);
                break;
            }
            
            /* forbiden the nagle algorithm of tcp */
            if( this->GetTcpConnFd() > 0 ) {
                int err = 0;
                int optvalue = 1; //start
                socklen_t optsize = sizeof(optvalue);
                err = setsockopt( this->GetTcpConnFd(), IPPROTO_TCP, TCP_CORK, &(optvalue), optsize);
                if( err ) {
                    ESLog::ISLog("set socket opt errors:%s\n", __FILE__, __LINE__, strerror(errno));
                    return -1;
                }
            }

            ESLog::ISLog("Write fd:%d\n", __FILE__, __LINE__, this->GetTcpConnFd());

            /* write into fd from buf */
            n = write( this->GetTcpConnFd(), strTmpBuf, writeSpan );
            if( n < 0  ) {
                ESLog::ISLog("write error:%s\n", __FILE__, __LINE__, strerror(errno));
                if( errno == EAGAIN ) { // now this read event, we have tackle complete, but the data is not end;
                    return -1; //error happend
                }
                return -2;
            }
           
            ESLog::ISLog("writespan %d, n:%d\n", __FILE__, __LINE__, writeSpan, n);
            string tmp;
            tmp.append(strTmpBuf, writeSpan);
            ESLog::ISLog("write fd data:%s\n", __FILE__, __LINE__, tmp.c_str());

            /* update the write info */
            this->rollBuff.UpdateFirstIndex( n );
            ESLog::ISLog("rollBuf first:%d last:%d\n", __FILE__, __LINE__, this->rollBuff.GetFirstIndex(), this->rollBuff.GetLastIndex()); //error
            //this->curCnt += n;
            this->totalCnt -= n;

            ESLog::ISLog("totalcnt:%d, curcnt:%d\n", __FILE__, __LINE__, this->totalCnt, this->curCnt);

            if( this->totalCnt <= 0 ) {
                break;
                //return this->totalCnt;
            }
        }

        //return this->totalCnt;
    }

    ESLog::ISLog("webStream Http ptr:%u\n", __FILE__, __LINE__, this->GetHttpPtr());
    /* if we have a new write fd */
    if( this->GetHttpPtr() != NULL && this->GetHttpPtr()->GetWriteFd() > 0 ) {
        while(1) {
            int n = 0;
            
            ESLog::ISLog("send tcp fd:%d, from fd:%d\n", __FILE__, __LINE__, this->GetTcpConnFd(), this->http->GetWriteFd());
            ESLog::ISLog("write before fdwritecnt %d, totalCnt:%d\n", __FILE__, __LINE__, this->fdwritecnt, this->totalCnt);
            /* sendfile between fds */
            n = sendfile( this->GetTcpConnFd(), this->http->GetWriteFd(), (off_t *)&(this->fdwritecnt),  this->totalCnt );
            if(  n < 0  ) {
                ESLog::ISLog("SendFile ERROR %s, %d\n", __FILE__, __LINE__, strerror(errno), n);
                if( errno == EAGAIN ) { // now this read event, we have tackle complete, but the data is not end;
                    return -1; //error happend
                }
                return -2;
            }
            
            ESLog::ISLog("write aflte fdwritecnt %d, totalCnt:%d\n", __FILE__, __LINE__, this->fdwritecnt, this->totalCnt);
        
            /* update the write info */
            this->totalCnt -= n; 
            //filesendbytes += n;
            //this->curCnt += n;

            ESLog::ISLog("Sendfile %d bytes:%d, totalCnt:%d\n", __FILE__, __LINE__, this->GetTcpConnFd(), n, this->totalCnt);

            if( this->totalCnt <= 0 ) {
                int err = 0;
                int optvalue = 0; //close
                socklen_t optsize = sizeof(optvalue);
                
                close(this->http->GetWriteFd());
                this->http->SetWriteFd(-1);
                this->fdwritecnt = 0;

                err = setsockopt( this->GetTcpConnFd(), IPPROTO_TCP, TCP_CORK, &optvalue, optsize );
                if( err ) {
                    ESLog::ISLog("set socket opt errors:%s\n", __FILE__, __LINE__, strerror(errno));
                    return -1;
                }

                break;
            }
        }
    }

    ESLog::ISLog("webStream end point totalCnt:%d, curCnt:%d\n", __FILE__, __LINE__, this->totalCnt, this->curCnt);
    ESLog::ISLog("WEBSTREAM WRITE END\n", __FILE__, __LINE__);

    return this->totalCnt;
   
}
