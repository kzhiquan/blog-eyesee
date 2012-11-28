
#include "HttpRequest.h"
#include "ESLog.h"
#include <stdlib.h>
#include <stdio.h>


/* constructor */
HttpRequest::HttpRequest(){
    this->httpVersion = NULL;
    this->reqCmd = NULL;
}

/* reInit*/
void HttpRequest::ReInit(){
    this->httpVersion = NULL;
    this->reqCmd = NULL;
    this->uri.ReInit();
    this->reqHeaders.clear();
}

/* set the request cmd type */
void HttpRequest::SetCmdType( char *strBuf ){
    this->reqCmd = strBuf;
}

/* Get the cmd type */
char *HttpRequest::GetCmdType(){
    return this->reqCmd;
}


/* set the request uri */
void HttpRequest::SetUri(char *strBuf, int length ){

    this->uri.SetContent(strBuf, length );

}

/* Get Uri Object */
Uri & HttpRequest::GetUri(){
    return this->uri;
}

/* is the uri format right */
bool HttpRequest::IsUriFormatRight( char *strBuf, int length ){
    return this->uri.IsValid(strBuf, length);
}

/* set http version */
void HttpRequest::SetHttpVersion( char *strBuf ){
    this->httpVersion = strBuf;
}

/* Get http version */
char * HttpRequest::GetHttpVersion(){
    return this->httpVersion;
}

/* set reqHeaders */
void HttpRequest::SetHeaderValueByName(char *strName, int nameLen, char *strValue, int valueLen ){
    
    string name;
    name.append(strName, nameLen);

    ESLog::ISLog("name:%s\n", __FILE__, __LINE__, name.c_str());

    string value;
    value.append(strValue, valueLen);
    ESLog::ISLog("value:%s\n", __FILE__, __LINE__, value.c_str());


    this->reqHeaders[name] = value;
}


/* Get http Request content-lenght */
int HttpRequest::GetHttpRequestContentLenght(){
    int cnt = 0;
    
    map<string, string>::iterator ite;
    ite = this->reqHeaders.find(string("Content-Length"));
    if( ite != this->reqHeaders.end() ) {
        cnt = atoi( (ite->second).c_str() );
    }
    ESLog::ISLog("cnt:%d\n", __FILE__, __LINE__, cnt);
    return cnt;
}
