
#include <stdio.h>
#include <stdlib.h>
#include "Session.h"
#include "Site.h"
#include "Server.h"
#include <string.h>
#include <errno.h>


/* constructor */
Session::Session(){
    this->http = NULL;
}

/* set the session Asy */
void Session::SetAsyStyle(){
    this->http->SetSynAsyFlag(2);
}

/* is the session is Asy */
bool Session::IsAsySession(){
    bool result = false;

    if( this->http->GetSynAsyFlag() == 2 ) {
        result = true;
    }

    return result;
}

/* Get the http */
Http *Session::GetHttpPtr(){
    return this->http;
}


/* work  Task method */
void Session::Work(){

    ESLog::ISLog("SESSION START!\n", __FILE__, __LINE__);

    ESLog::ISLog("Session for ip:%s, port:%d, fd:%d\n", __FILE__, __LINE__, this->http->GetWebStreamPtr()->GetTcpConn()->GetIP().c_str(),
                        this->http->GetWebStreamPtr()->GetTcpConn()->GetPort(), this->http->GetWebStreamPtr()->GetTcpConnFd());
  
    int err = 0;

    /* Accept Http body, if no body,just skip;
     if has body, and the body have to wait receive all, just receive completely;
     if not have to wait, also just skip,
     on common, this decide by application, so it retain to application */
    //this->AcceptHttp();

     /* just parse the uri, and handle default, for example, just file request */
     err = this->ProcessHttpDefault();
     if( err == 0 ) { // get the file
         ESLog::ISLog("Session:: Get An file Work end! Success\n", __FILE__, __LINE__);
         return;
     }

     /* this function just override by the application */
     err = ProcessHttp(this->http->GetHttpRequest(), this->http->GetHttpResponse());
     if(err == 0) {
         ESLog::ISLog("Session: get query data end! Success \n", __FILE__, __LINE__);
         return;
     }

     /* according to the handle result, file, or memory data, read into the buffer
     just anwser async io */
     //this->AnwserDefaultHttp();

     /* process error */
     /*this->http->GetHttpResponse().SetResHeaderValueByName("Content-Length", "0");
     this->http->GetHttpResponse().SetResCode("404");
     this->http->GetHttpResponse().SetResPhrase("FAIL");
     this->StreamOut(NULL);*/
     this->http->GetWebStreamPtr()->SetStatus(WSTREAM_ERROR);
     Server::Instance()->GetIOLoop().DelReadEventFd(this->http->GetWebStreamPtr()->GetTcpConnFd());

    /* this->http->GetHttpResponse().SetResHeaderValueByName("Content-Length", "0");
     this->http->GetHttpResponse().SetResCode("404");
     this->http->GetHttpResponse().SetResPhrase("FAIL");
     this->StreamOut(NULL); */


     ESLog::ISLog("SESSION END\n", __FILE__, __LINE__);
}


// process get file request
int Session::ProcessGetFileReq(){

    int uriFd = 0;
    int err = 0;

    ESLog::ISLog("PROCESS GET FILE REQ START!\n", __FILE__, __LINE__);
    ESLog::ISLog("Http ptr %u\n", __FILE__, __LINE__, this->http);
    /* parse the uri */
    Uri  uri = this->http->GetRequestUri();

    /* Get root Directory of the site */
    Site  site = Server::Instance()->GetSite();

    /* is uriFile exit  */
    string reqfile = uri.GetFileName();
    if( strcmp(reqfile.c_str(), "/") == 0 ) {
        reqfile.append(site.GetDefaultPage().c_str());
    }
    ESLog::ISLog("defaultPage:%s\n", __FILE__, __LINE__, site.GetDefaultPage().c_str());
    ESLog::ISLog("reqfie:%s\n", __FILE__, __LINE__, reqfile.c_str());

    /* */
    if( !reqfile.empty() && !uri.IsQueryExist() ) {
        if( site.IsFileExist( reqfile.c_str() ) ) {
            int content_length = 0;

            /* get The file fd */
            uriFd = site.OpenUriFile(reqfile.c_str());

            /* Get File Size */
            char Buf[10];
            content_length = site.GetFileSize( reqfile.c_str() );
            if( content_length < 0 ) {
                return content_length;
            }
            sprintf(Buf, "%d", content_length );
            ESLog::ISLog("Session handle file request response body size:%d\n", __FILE__, __LINE__, content_length);
            this->http->GetHttpResponse().SetResHeaderValueByName("Content-Length", Buf );

            /* set the session style */
            //this->SetAsyStyle();
        }
        else{
            ESLog::ISLog("the file %s not found\n", __FILE__, __LINE__, reqfile.c_str());
            return -1;
            //this->http->GetHttpResponse().SetResCode("404");
            //this->http->GetHttpResponse().SetResPhrase("fail");
        }
    }
    else{
        err = -1;
        ESLog::ISLog("there is not file to find to tacke !\n", __FILE__, __LINE__);
    }

    /* set the uriFd */
    this->http->SetWriteFd(uriFd);
    ESLog::ISLog("http write fd:%d\n", __FILE__, __LINE__, uriFd );

    ESLog::ISLog("PROCESS GET FILE REQ END!\n", __FILE__, __LINE__);

    return err;
}


// Anwser get file req
int Session::AnwserGetFileReq(){

    ESLog::ISLog("ANWSER GET FILE REQ START\n", __FILE__, __LINE__);

    if(this->http->GetWriteFd() > 0 ) {
        int err = 0;
        char *strBuf = NULL;

        /* set the taskStream status */
        WebStream *webStream = this->http->GetWebStreamPtr();

        /* change the taskStream into writeStream */
        this->http->WebStreamInitToWrite();

        /* Set the WebStream Status to write */
        webStream->SetStatus(WSTREAM_WRITE);
        //this->AddWriteWebStream(webStream);

        /* write into the buf of webStream */
        strBuf = this->http->GetHttpResponse().StreamHeaderOut();

        /* len */
        int headerbuflen = strlen(strBuf);

        /* read from the buf */
        int sum = 0;
        while(1) {   
            int n = 0;     
            n = webStream->StreamRead(strBuf+sum);
            if( n < -1 ) {
                //sleep(100);
                continue;
            }
            if( n == 0) {
                break;
            }

            sum += n;

            ESLog::ISLog("session file header: n:%d, sum:%d, headerbuflen:%d\n", __FILE__, __LINE__,  n, sum, headerbuflen);
            /* have write */
            if( headerbuflen == sum ) {
                break;
            }
        }

        /* update the total*/
        string strconlen = this->http->GetHttpResponse().GetResHeaderValueByName("Content-Length");
        ESLog::ISLog("strcontent-len:%s\n", __FILE__, __LINE__, strconlen.c_str() );
        int content_length = atoi(strconlen.c_str());// error??
        ESLog::ISLog("Anwser request file resposne http header size:%d, body size:%d\n", __FILE__, __LINE__, sum, content_length);
        webStream->IncrementTotalCnt(content_length+sum);

        ESLog::ISLog("Task Modify read event fd %d to write event\n", __FILE__, __LINE__, webStream->GetTcpConnFd() );
        /* add into poll write, and listen the write event */
        err = Server::Instance()->GetIOLoop().ModToWriteEventFd(webStream->GetTcpConnFd(), (void *)webStream );
        if(err<0) {
            ESLog::ISLog("ModToWriteEventEroor:%s", __FILE__, __LINE__, strerror(errno));
            return err;
        }
    }

    ESLog::ISLog("ANWSER GET FILE REQ END\n", __FILE__, __LINE__);
}

/* set the paramter of the task */
void Session::SetParameter(void *parameter){
    this->http = ( Http *)parameter;
}

/* clone self */
Task * Session::Clone(){
    ESLog::ISLog("close session here\n", __FILE__, __LINE__);

    Task *task = new Session(*this);

    return task;
}


/* accept the content from the parter */
int Session::AcceptHttp(){

    printf("Session::AcceptHttp");
}



/* process Http default*/
int Session::ProcessHttpDefault(){

    int err = 0;

    ESLog::ISLog("PROCESS HTTP DEFAULT START\n", __FILE__, __LINE__);

    /* anwset the http request by default */
    err = this->AnwserDefaultHttp();
    if( err < 0 ) {
        ESLog::ISLog("anwser default error!\n", __FILE__, __LINE__);
        return err;
    }


    /* process get file request */
    err = this->ProcessGetFileReq();
    if( err == 0 ) { // now file found
        err = this->AnwserGetFileReq();
        if( err < 0 ) {
            ESLog::ISLog("Anwser get file request failed\n",__FILE__, __LINE__);
            return err;
        }
    }

    ESLog::ISLog("PROCESS HTTP DEFAULT END\n", __FILE__, __LINE__);

    return err;
}

/* reply the parter by the request type and url */
int Session::AnwserDefaultHttp(){

    int err = 0;

    ESLog::ISLog("ANWSER DEFAULT HTTP START\n", __FILE__, __LINE__);

    ESLog::ISLog("Http ptr %u\n", __FILE__, __LINE__, this->http );

    /* set the default respose content */
    this->http->GetHttpResponse().SetResCode("200");
    this->http->GetHttpResponse().SetHttpVersion("HTTP/1.1");
    this->http->GetHttpResponse().SetResPhrase("OK");
    
    time_t temp;
    char *pTime = NULL;
    temp = time(NULL);
    pTime = ctime(&temp);
    pTime[strlen(pTime)-1] = '\0';
    /* res content */
    this->http->GetHttpResponse().SetResHeaderValueByName("Date", pTime );
    this->http->GetHttpResponse().SetResHeaderValueByName("Server", "EyeSee/0.1" );

    /* parse the uri */
    this->http->GetRequestUri().ParseUri();

    /* */
    ESLog::ISLog("ANWSER DEFAULT HTTP END\n", __FILE__, __LINE__);

    return err;
}


/* process Http */
int Session::ProcessHttp(HttpRequest &request, HttpResponse &response ){

    ESLog::ISLog("SESSION PROCESS HTTP START\n", __FILE__, __LINE__);

    Uri &uri = request.GetUri(); 
    string & uristr = uri.GetUriStr();
    printf("%s", uristr.c_str());

    char *strBuf = NULL;
    /* change the taskStream into writeStream */
    WebStream *writeStream = this->http->WebStreamInitToWrite();
    /* Set the WebStream Status to write */
    writeStream->SetStatus(WSTREAM_WRITE);
    /* write into the buf of webStream */
    strBuf = this->http->GetHttpResponse().StreamHeaderOut();
    /* add into poll write, and listen the write event */
    Server::Instance()->GetIOLoop().ModToWriteEventFd(writeStream->GetTcpConnFd());

    /* out header */
    this->StreamOut(strBuf);

    /* the data about get the business from database or other */
    char dataone[10244];
    memset( dataone, 0, sizeof(dataone));

    /* out stream */
    StreamOut(dataone);

    /* the data about get the business from database or other */
    char datatwo[10244];
    memset( datatwo, 0, sizeof(datatwo));

    /* out stream */
    StreamOut(datatwo);

    ESLog::ISLog("SESSION PROCESS HTTP END\n", __FILE__, __LINE__);
    
}


/* stream out to webStream */
int Session::StreamOut(const char *buf ){

    int err = 0;
    int total = 0;
    int headersum = 0;
    int bodysum = 0;

    ESLog::ISLog("SESSION STREAM OUT START\n", __FILE__, __LINE__);

    /* if the reqheader has not stream out
       we first write it out in the stream */
    if( !this->http->IsResponseHeaderSend()) {
        char *strBuf = NULL;
        
        ESLog::ISLog("SESSION SEND RESPONSE HEADER START \n", __FILE__, __LINE__);
        
        /* set the taskStream status */
        WebStream *webStream = this->http->GetWebStreamPtr();

        /* change the taskStream into writeStream */
        this->http->WebStreamInitToWrite();

        /* Set the WebStream Status to write */
        webStream->SetStatus(WSTREAM_WRITE);
        //this->AddWriteWebStream(webStream);

        /* write into the buf of webStream */
        strBuf = this->http->GetHttpResponse().StreamHeaderOut();
        
        /* headertotal */
        int headertotal = strlen(strBuf);

        /* read from the buf */
        while(1) {   
            int n = 0;  
            ESLog::ISLog("session start:n:%d, headertotal:%d, headersum:%d\n", __FILE__, __LINE__, n, headertotal, headersum);   
            n = webStream->StreamRead(strBuf+headersum);
            if( n < -1 ) {
                return n;
            }

            headersum += n;
            total +=n;

            ESLog::ISLog("session end:n:%d, headertotal:%d, headersum:%d\n", __FILE__, __LINE__, n, headertotal, headersum);
            if(headersum == headertotal) {
                break;
            }
        }

        /* update the total*/
        const char *lenBuf = this->http->GetHttpResponse().GetResHeaderValueByName("Content-Length").c_str();
        if( lenBuf ) {
            ESLog::ISLog("strcontent-lenght:%s\n", __FILE__, __LINE__, lenBuf );

            int content_length = atoi(lenBuf);
            ESLog::ISLog("http response header size:%d, response body size:%d\n", __FILE__, __LINE__, headersum, content_length);
            content_length +=headersum;
            if( content_length !=0  ) {
                ESLog::ISLog("Increment total cnt of webstream :%d\n", __FILE__, __LINE__, content_length );
                webStream->IncrementTotalCnt(content_length);
            }
        }

        ESLog::ISLog("Task Modify read event fd %d to write evetn\n", __FILE__, __LINE__, webStream->GetTcpConnFd() );
        /* add into poll write, and listen the write event */
        err = Server::Instance()->GetIOLoop().ModToWriteEventFd(webStream->GetTcpConnFd(), (void *)webStream );
        if(err<0) {
            ESLog::ISLog("ModToWriteEventEroor:%s", __FILE__, __LINE__, strerror(errno));
            return err;
        }

        this->http->SetResponseHeaderSendFlag(true);

        ESLog::ISLog("SESSION SEND RESPONSE HEADER END \n", __FILE__, __LINE__);
    }

    /* send the body */
    if( this->http->IsResponseHeaderSend() && buf !=NULL ) {

        ESLog::ISLog("SESSION SEND RESPONSE BODY START \n", __FILE__, __LINE__);
        
        WebStream *writeStream = this->http->GetWebStreamPtr();
        while(1) {
            int n = 0;
            n = writeStream->StreamRead(buf+bodysum);
            if( n < -1 ) {
                return -1;
            }

            bodysum += n;
            total += bodysum;

            if( bodysum == strlen(buf)) {
                break;
            }
        }

        this->http->SetResponseHeaderSendFlag(false);

        ESLog::ISLog("SESSION SEND RESPONSE BODY END \n", __FILE__, __LINE__);
    }

    ESLog::ISLog("SESSION STREAM OUT END\n", __FILE__, __LINE__);

    return total-headersum;
   
}

/* in stream from webStream */
char *Session::StreamIn( int *buflen ){

    char * buf = NULL;
    *buflen = 0;

    /* webStream of this session */
    WebStream *webStream = this->http->GetWebStreamPtr();
    if( webStream == NULL ) {
        return buf;
    }

    /* */
    buf = webStream->StreamWrite(buflen);


    return buf;
}
