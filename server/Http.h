
#ifndef _HTTP_H_
#define _HTTP_H_


#include "HttpRequest.h"
#include "HttpResponse.h"
#include "WebStream.h"


class WebStream;

enum HttpStatus{
    HTTP_INIT,
    HTTP_REQHEAD_PARSED,
    HTTP_REQBODY_PARSED,
    HTTP_RESSEND_OVER
};

class Http{
private:
    int                 status; // 0:init; 1:reqheadparsed; 2:reqBodyParsed;
    pthread_mutex_t     status_mutex;

    bool                firstLineParsed;
    bool                resheader;
    int                 parsedCharNum;
    int                 fromWritefd;
    int                 synasyflag; // 0:init, 1:sys, 2:asy
    int                 headParseLostCnt;

    WebStream       *webStream;
    HttpRequest     request;
    HttpResponse    response;

    /* static data */
    static char* headNames[];
    /* static data */
    static char* cmdType[];
    /* static data */
    static char* httpVersion[];

    /* status description */
    static char* httpstatusdesc[];

public:
    Http();

    /* reset */
    void ReInit();

    /* Get the status of Http */
    int GetStatus();

    /* Get the descritpion of http status */
    char * GetStatusDesc();

    /* get headParseLostCnt */
    int GetLostCnt();

    /* incremt lost cnt */
    void IncLostCnt();

    /* Get the status of Http */
    void SetStatus( int status );

    /* Get request uri object */
    Uri & GetRequestUri();

    /* set the request out file fd */
    void SetWriteFd(int writeFd );

    /* get the request out file fd */
    int GetWriteFd();

    /* set the synasy flag */
    void SetSynAsyFlag(int flag);

    /* Get the synasy flag */
    int GetSynAsyFlag();

    /* init webStream to write */
    WebStream* WebStreamInitToWrite();

    /* Set the WebStream Status */
    WebStream* GetWebStreamPtr();

    /* set the WebStream Status */
    void SetWebStreamPtr(WebStream *stream);

    /* parser the http header */
    int ParseHeader( char *strBuf, int length );

    /* parse a line of http header */
    int ParseHeaderLine( char *strBuf, int length );

    /* parse requestline */
    int ParseHeaderRequestLine( char *strBuf, int length );

    /* parse request headers */
    int ParseHeaderRequestHeaders( char *strBuf, int length );

    /* is the header name right */
    bool IsHttpRequestHeaderNameRight(char *strbuf, int length );

    /* is the request type right */
    bool IsHeaderRequestTypeRight( char **strBuf, int length);

    /* parse the header request type */
    int ParseHeaderRequestType( char *strBuf, int length );

    /* uri format right */
    bool IsHeaderRequestUriRight(char *buf, int length);

    /* parse the header request uri */
    int ParseHeaderRequestUri( char *charBuf, int length );

    /* parse the header request http version */
    int ParseHeaderRequestHttpVersion( char *charBuf, int length );

    /* is http version right */
    bool IsHeaderRequestHttpVersionRight( char **strBuf, int length );

    /* Is response header */
    bool IsResponseHeaderSend();

    /* set the response header flag */
    void SetResponseHeaderSendFlag(bool flag);

    /* Get the HttpRequest */
    HttpRequest & GetHttpRequest();

    /* Get the HttpResponse */
    HttpResponse & GetHttpResponse();


};






#endif
