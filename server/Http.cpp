
#include "Http.h"
#include "ESLog.h"
#include <stdio.h>

#ifdef _cplusplus
extern "C" {
#endif

/* static data */
char* Http::headNames[] = { "Host", "Accept", "From", "User-Agent", "Content-Length", 
  "Content-Type","Accept-Language", "Accept-Encoding", "Connection", "Content-Encoding", 
  "Cache-Control", "Accept-Charset", "Referer",  "Pragma", "Cookie", "Origin", "If-Modified-Since", 
  "If-None-Match", NULL };

/* static data */
char* Http::cmdType[] = {"GET", "POST", NULL};

/* static data */
char* Http::httpVersion[] = {"HTTP/1.0", "HTTP/1.1", NULL};


/* status description */
char* Http::httpstatusdesc[] = { "HTTP_INIT", "HTTP_REQHEAD_PARSED", "HTTP_REQBODY_PARSED", "HTTP_RESSEND_OVER"};

#ifdef _cplusplus
}
#endif



/* constructor */
Http::Http(){
    webStream = NULL;
    firstLineParsed = false;
    parsedCharNum = 0;
    resheader = false;
    fromWritefd = -1;
    synasyflag = 0;
    headParseLostCnt = 0;

    status = HTTP_INIT;
    pthread_mutex_init(&status_mutex, NULL);
}


/* reset */
void Http::ReInit(){
    
    firstLineParsed = false;
    resheader = false;
    fromWritefd = -1;
    status = 0;

    synasyflag = 0;

    headParseLostCnt=0;
    
    request.ReInit();
    response.ReInit();
}

/* get headParseLostCnt */
int Http::GetLostCnt(){
    return this->headParseLostCnt;
}

/* increment lost cnt */
void Http::IncLostCnt(){
    this->headParseLostCnt++;
}

/* Get the status of Http */
int Http::GetStatus(){
    return this->status;
}

/* Get the status of description */
char * Http::GetStatusDesc(){
    return Http::httpstatusdesc[this->status];
}

/* Get the status of Http */
void Http::SetStatus( int status ){
    pthread_mutex_lock(&status_mutex);
    this->status = status;
    pthread_mutex_unlock(&status_mutex);
}

/* set the request out file fd */
void Http::SetWriteFd(int writeFd ){
    this->fromWritefd = writeFd;
}

/* set the synasy flag */
void Http::SetSynAsyFlag(int flag){
    this->synasyflag = flag;
}

/* get the request out file fd */
int Http::GetWriteFd(){
    return this->fromWritefd;
}


/* Get the synasy flag */
int Http::GetSynAsyFlag(){
    return this->synasyflag;
}

/* Set the WebStream Status */
WebStream* Http::GetWebStreamPtr(){
    return this->webStream;
}

/* set the WebStream Status */
void Http::SetWebStreamPtr( WebStream *stream ){
    this->webStream = stream;
}

/* init webStream to write */
WebStream* Http::WebStreamInitToWrite(){
    this->webStream->ReInit();
    return this->webStream;
}

/* Get request uri object */
Uri & Http::GetRequestUri(){
    return this->request.GetUri();
}

/* Get the HttpRequest */
HttpRequest & Http::GetHttpRequest(){
    return this->request;
}

HttpResponse & Http::GetHttpResponse(){
    return this->response;
}

/* Is response header */
bool Http::IsResponseHeaderSend(){
    return this->resheader;
}

/* set the response header flag */
void Http::SetResponseHeaderSendFlag(bool flag){
    this->resheader = true;
}

/* parser the http header */
int Http::ParseHeader( char *strBuf, int length ){

    int iCharNum = 0; // record the char num parse   
    int i = 0;
    
    ESLog::ISLog("PARSE HEAD CONTENT START\n", __FILE__, __LINE__);
  
    // parse the each line
    while( i < length ) {
        int iLineCharNum = 0;

        iLineCharNum = this->ParseHeaderLine(strBuf+iCharNum, length-iCharNum);
        
        ESLog::ISLog("iLineCharNum:%d\n", __FILE__, __LINE__, iLineCharNum);
        /* read the last line of the header \r\n */
        if( iLineCharNum == 2 ) {
            int contentLength = 0;
            /* update \r\n */
            iCharNum += iLineCharNum;

            ESLog::ISLog("there is an Http head parsed\n",__FILE__, __LINE__);
            
            ESLog::ISLog("cmdType:%s\n", __FILE__, __LINE__, this->request.GetCmdType());
            ESLog::ISLog("GetFile:%s\n", __FILE__, __LINE__, this->request.GetUri().GetUriStr().c_str());
            ESLog::ISLog("HttpVersion:%s\n", __FILE__, __LINE__, this->request.GetHttpVersion());

            //this->request.Ge
            /* set the total count hava to read */
            contentLength = this->request.GetHttpRequestContentLenght();
            
            /* set the total */
            this->webStream->SetTotalCnt(iCharNum+contentLength);

            /* set the status of the http */
            ESLog::ISLog("status:%s\n", __FILE__, __LINE__, this->GetStatusDesc() );
            ESLog::ISLog("http from writefd:%d\n",__FILE__, __LINE__, this->fromWritefd );

            this->SetStatus(HTTP_REQHEAD_PARSED);
            this->webStream->SetStatus(WSTREAM_UNDISPATHED);
    
            break;
        }
        else if( iLineCharNum <= 0 ) {
            this->parsedCharNum += iCharNum;
            return iLineCharNum; // error format, or no data parsed 
        }

        iCharNum += iLineCharNum;
        i += iLineCharNum;
    }

    
    this->parsedCharNum += iCharNum;

    ESLog::ISLog("PARSE HEAD CONTENT END\n", __FILE__, __LINE__);

    return iCharNum;
}

/* parse a line of the header */
int Http::ParseHeaderLine( char *strBuf, int length ){

    int iCharNum = 0;

    ESLog::ISLog("PARSE HTTP HEAD LINE START\n", __FILE__, __LINE__);

    if( this->firstLineParsed == false ) {
        /* if requestline parse over, set the firsLineParsed to true */
        iCharNum = this->ParseHeaderRequestLine(strBuf, length);
        if( iCharNum <= 0 ) {
            return iCharNum; // an error happed.
        }

        this->firstLineParsed = true;
    }
    else{
        /* parse the line */
        iCharNum = this->ParseHeaderRequestHeaders(strBuf, length);
    }

    ESLog::ISLog("PARSE HTTP HEAD LINE START\n", __FILE__, __LINE__);

    return iCharNum;
}


/* parse the header request line */
int Http::ParseHeaderRequestLine( char *strBuf, int length){

    int iCharNum = 0;

    ESLog::ISLog("PARSE HTTP REQUEST LINE START\n", __FILE__, __LINE__);

    /* parse the request type; if return < 0 , error happend;
       if >0, the char num in strBuf skip, parse right;
       if =0, not parse any thing, not end */
    iCharNum = this->ParseHeaderRequestType(strBuf, length );
    if( iCharNum <= 0 ) {
        ESLog::ISLog("request type parse error!\n", __FILE__, __LINE__);
        return iCharNum;
    }
    
    /* parse the request uri */
    iCharNum +=this->ParseHeaderRequestUri(strBuf+iCharNum, length-iCharNum );
    if( iCharNum <= 0 ) {
        ESLog::ISLog("request uri parse error!\n", __FILE__, __LINE__);
        return iCharNum;
    }

    /* parse the request http version */
    iCharNum += this->ParseHeaderRequestHttpVersion(strBuf+iCharNum, length-iCharNum );
    if( iCharNum <= 0 ) {
        ESLog::ISLog("request httpversion parse error!\n", __FILE__, __LINE__);
        return iCharNum;
    }

    ESLog::ISLog("PARSE HTTP REQUEST LINE END\n", __FILE__, __LINE__);

    return iCharNum;
}

/* parse the headers */
int Http::ParseHeaderRequestHeaders(char *buf, int length){
    int     i = 0;
    int     iCharNum=0;
    bool    isFormatRight = false;
    char    *strBuf = buf;

    ESLog::ISLog("HTTP HEADER REQUEST HEADER NAME PARSE START!\n", __FILE__, __LINE__);
   
    /* Get the content */
    while( i < length ) {
        if( strBuf[i] != '\r' && strBuf[i] != '\n') {
            i++;
            iCharNum++;
            if( iCharNum > 256) { // error 
                ESLog::ISLog("errors http request header parse\n", __FILE__, __LINE__);
                return -1;
            }
            continue;
        }
        isFormatRight = true;
        break;
    }

    /* the end */
    if( iCharNum == 0 && isFormatRight ) {
        return 2;
    }

    /* check the content */
    if( isFormatRight ) {

        int j = 0;
        char *strValue = NULL;
        isFormatRight = false;

        /* find the name */
        while( j < i ) {

            if( strBuf[j] !=':' && strBuf[j+1] != ' ' ) {
                j++;
                continue;
            }
            isFormatRight = true;
            break;
        }
        if( !isFormatRight ) {
            ESLog::ISLog("request header format error!\n", __FILE__, __LINE__);
            return -1;
        }

        /* is the header name right */
        if(!this->IsHttpRequestHeaderNameRight( strBuf, j-1 )){
            string tmpheadname;
            tmpheadname.append(strBuf, j-1);
            ESLog::ISLog("Error headName:%s", __FILE__, __LINE__, strBuf);
            ESLog::ISLog("request header name error!\n", __FILE__, __LINE__);
            return -1;
        }

        strValue = &(strBuf[j+2]);
        
        this->request.SetHeaderValueByName( strBuf, j, strValue, iCharNum-j-2 );

        iCharNum = iCharNum+2;
    }

    ESLog::ISLog("HTTP HEADER REQUEST HEADER NAME PARSE END!\n", __FILE__, __LINE__);

    return iCharNum;
}


/* is the header name right */
bool Http::IsHttpRequestHeaderNameRight(char *strBuf, int length ){

    int         i = 0;
    bool        result = false;

   

    /* loop the request type */
    while( this->headNames[i] != NULL) {

        int j = 0;
        const char *pChar = headNames[i];
        while( *pChar != '\0' ) {
            if( strBuf[j] != *pChar ) {
                break;
            }
            pChar++;
            j++;
        }

        /* right */
        if( *pChar == '\0' && j-1 == length ) {
            result = true;
            break;
        }

        i++;
    }

    return result;
}

/* parse the header request line type */
int Http::ParseHeaderRequestType( char *buf, int length ){

    ESLog::ISLog("HTTP HEADER REQUEST TYPE PARSE START!\n", __FILE__, __LINE__);

    int     i = 0;
    int     iCharNum=0;
    bool    isFormatRight = false;
    char    *strBuf = buf;

    //ESLog::ISLog("request type:\n", __FILE__, __LINE__);
    //for( int j = 0; j < length; j++ ) {
    //    ESLog::ISLog("%c\n", __FILE__, __LINE__, buf[j]);
    //}
    string requesttype;
    requesttype.append(buf, length);
    //ESLog::ISLog("http request:%s\n", __FILE__, __LINE__, requesttype.c_str());
    //ESLog::ISLog("http request len :%d\n", __FILE__, __LINE__, length);

    /* Get the content */
    while( i < length ) {
        if( strBuf[i] != ' ') {
            i++;
            iCharNum++;
            if( iCharNum > 4) { // error 
                ESLog::ISLog("request type error, there is more chars in request type!\n", __FILE__, __LINE__);
                return -1;
            }
            continue;
        }
        isFormatRight = true;
        break;
    }

    /* check the content */
    if( isFormatRight ) {
        isFormatRight = this->IsHeaderRequestTypeRight(&strBuf, i-1);
        if( !isFormatRight ) {
            ESLog::ISLog("request type error, there is no this type!\n", __FILE__, __LINE__);
            return -1;
        }

        /* Set the request type of httpRequest object */
        this->request.SetCmdType(strBuf);

        iCharNum = iCharNum+1;
    }

    ESLog::ISLog("request type:%s\n", __FILE__, __LINE__,  strBuf);
    ESLog::ISLog("iCharNum:%d\n", __FILE__, __LINE__, iCharNum);

    ESLog::ISLog("HTTP HEADER REQUEST TYPE PARSE  END!\n", __FILE__, __LINE__);

    return iCharNum;
}

bool Http::IsHeaderRequestTypeRight( char **strBuf, int length ){

    int         i = 0;
    bool        result = false;


    /* loop the request type */
    while( this->cmdType[i] != NULL) {

        int j = 0;
        const char *pChar = this->cmdType[i];
        while( *pChar != '\0' ) {
            if( (*strBuf)[j] != *pChar ) {
                break;
            }
            pChar++;
            j++;
        }

        /* right */
        if( *pChar == '\0' && j-1 == length ) {
            result = true;
            *strBuf = this->cmdType[i];
            break;
        }

        i++;
    }

    return result;
}

/* parset the request line uri */
int Http::ParseHeaderRequestUri( char *buf, int length ){
    int     i = 0;
    int     iCharNum=0;
    bool    isFormatRight = false;
    char    *strBuf = buf;

    ESLog::ISLog("HTTP HEADER REQUEST URI PARSE START!\n", __FILE__, __LINE__);

    /* Get the content */
    while( i < length ) {
        if( strBuf[i] != ' ') {
            i++;
            iCharNum++;
            if( iCharNum > 256) { // error 
                return -1;
            }
            continue;
        }
        isFormatRight = true;
        break;
    }

    /* check the content */
    if( isFormatRight ) {
        isFormatRight = this->IsHeaderRequestUriRight(strBuf, i-1);
        if( !isFormatRight ) {
            return -1;
        }

        /* Set the request type of httpRequest object */
        this->request.SetUri(strBuf, i);

        iCharNum = iCharNum+1;
    }
    
    string tmpuri;
    tmpuri.append(strBuf, i);
    ESLog::ISLog("request uri:%s\n", __FILE__, __LINE__, tmpuri.c_str());

    ESLog::ISLog("iCharNum:%d\n", __FILE__, __LINE__, iCharNum);

    ESLog::ISLog("HTTP HEADER REQUEST URI PARSE END!\n", __FILE__, __LINE__);

    return iCharNum;

}

/* uri format right */
bool Http::IsHeaderRequestUriRight(char *buf, int length){

    bool result = false;

    result = this->request.IsUriFormatRight(buf, length );

    return result;
}



/* parse the request line http version */
int Http::ParseHeaderRequestHttpVersion(char *buf, int length ){

    int     i = 0;
    int     iCharNum=0;
    bool    isFormatRight = false;
    char    *strBuf = buf;

    ESLog::ISLog("HTTP HEADER REQUEST HTTPVERSION PARSE START!\n", __FILE__, __LINE__);

    /* Get the content */
    while( i < length-1 ) {
        if( strBuf[i] != '\r' && strBuf[i+1] != '\n' ) {
            i++;
            iCharNum++;
            if( iCharNum > 8) { // error 
                return -1;
            }
            continue;
        }
        isFormatRight = true;
        break;
    }

    /* check the content */
    if( isFormatRight ) {
        isFormatRight = this->IsHeaderRequestHttpVersionRight(&strBuf, i-1);
        if( !isFormatRight ) {
            return -1;
        }

        /* Set the request type of httpRequest object */
        this->request.SetHttpVersion(strBuf);

        iCharNum = iCharNum+2;
    }

    ESLog::ISLog("request httpversion:%s\n", __FILE__, __LINE__, strBuf);
    ESLog::ISLog("iCharNum:%d\n", __FILE__, __LINE__, iCharNum);

    ESLog::ISLog("HTTP HEADER REQUEST HTTPVERSION PARSE END!\n", __FILE__, __LINE__);


    return iCharNum;

}

bool Http::IsHeaderRequestHttpVersionRight( char **strBuf, int length ){

    int         i = 0;
    bool        result = false;



    /* loop the request type */
    while( this->httpVersion[i] != NULL) {

        int j = 0;
        const char *pChar = this->httpVersion[i];
        while( *pChar != '\0' ) {
            if( (*strBuf)[j] != *pChar ) {
                break;
            }
            pChar++;
            j++;
        }

        /* right */
        if( *pChar == '\0' && j-1 == length ) {
            result = true;
            *strBuf = this->httpVersion[i];
            break;
        }

        i++;
    }

    return result;
}

