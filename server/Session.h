

#ifndef _SESSION_H_
#define _SESSION_H_

#include "Task.h"
#include "Http.h"
#include "WebStream.h"
#include "HttpRequest.h"
#include "HttpResponse.h"

/* just to a IP */
class Session: public Task{

private:
    Http     *http;

public:
    Session();
    virtual void Work();
    virtual Task* Clone();
    virtual void SetParameter(void *parameter);

    /* Set the http */
    void SetHttp();

    /* Get the http */
    Http *GetHttpPtr();

    /* set the session Asy */
    void SetAsyStyle();

    /* is the session is Asy */
    bool IsAsySession();

    // parse http
    int AcceptHttp();

    // process get file request
    int ProcessGetFileReq();

    // Anwser get file req
    int AnwserGetFileReq();

    // process the necessary step of the protocol
    int ProcessHttpDefault();

    /* out stream to webStream */
    int StreamOut(const char *buf );

    /* in stream from webStream */
    char *StreamIn( int *buflen );

    // process the http business, it is override by child.
    virtual int ProcessHttp( HttpRequest &request, HttpResponse &response ); 

    // reply
    int AnwserDefaultHttp();  
};




#endif
