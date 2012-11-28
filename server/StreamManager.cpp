

#include "StreamManager.h"
#include "WebStream.h"
#include "ESLog.h"
#include "Server.h"
#include <stdio.h>

/* constructor */
StreamManager::StreamManager(){
}

/* get an idle webstream from idleStream, if no idleStream
   new a WebStream */
WebStream * StreamManager::GetIdleWebStream(){
    WebStream *idleStream = NULL;
    
    /* find an idle stream in the vector */
    if( this->idleStream.empty() ) {
        ESLog::ISLog("IO Server: An new IdleStream Create \n", __FILE__, __LINE__);
        idleStream  = new WebStream(); // produce a new idleStream from the heap memory.
    }
    else{ // get an element from the end;
       ESLog::ISLog("IO Server: An exist idleStream Get \n", __FILE__, __LINE__);
       idleStream = this->idleStream.back();
       this->idleStream.pop_back();
    }

    return idleStream;
}

/* add an idle Stream into idle pool */
int StreamManager::AddIdleStream(WebStream *idleStream ){

    int result = 0;

    this->idleStream.push_back(idleStream);

    return result;

}

/* Get idle Stream size */
int StreamManager::GetIdleStreamSize(){
    return this->idleStream.size();
}



/* get a Task WebStream from the front */
WebStream * StreamManager::GetReadWebStream( int fd ){


}

/* add an Task Web Stream in the vector, we may use the condition */
int  StreamManager::AddReadWebStream(WebStream *readStream ){
    int result = 0;

    this->readStream.push_back( readStream );

    return result;
}

/* delete an webWrite Stream in the pool */
int StreamManager::DeleteReadWebStream( WebStream *readWebStream ){

    int result = 0;

    list<WebStream *>::iterator ite;
    for( ite = this->readStream.begin(); ite != this->readStream.end(); ++ite ) {
        if( *ite == readWebStream ) {
            this->readStream.erase(ite);
            result = 1;
            break;
        }
    }

    return result;
}

/* is the taskWebStream in the taskStream */
bool StreamManager::IsReadWebStreamExist( WebStream *readWebStream ){
    
    bool result = false;

    list<WebStream *>::iterator ite;
    for( ite = this->readStream.begin(); ite != this->readStream.end(); ++ite ) {
        if( *ite == readWebStream ) {
            result = true;
        }
    }

    return result;
}


/* is the readStream set has uninitlize stream */
bool StreamManager::IsExistUninitilizeReadStream(){

    ESLog::ISLog("IS EXIST UNINITILIZE READ STREAM START\n", __FILE__, __LINE__);

    bool result = false;

    list<WebStream *>::iterator ite;
    for( ite = this->readStream.begin(); ite != this->readStream.end(); ++ite ) {
        
        WebStream *webStream = (*ite);
        ESLog::ISLog("read webStream->http ptr:%u\n", __FILE__, __LINE__, webStream->GetHttpPtr());
        ESLog::ISLog("read webStream->fd:%d\n", __FILE__, __LINE__, webStream->GetTcpConnFd());
        ESLog::ISLog("read webStream->Status:%s\n", __FILE__, __LINE__, webStream->GetStatusDesc());

        if( (*ite)->GetStatus() == WSTREAM_UNINITILIZED || (*ite)->GetStatus() == WSTREAM_READ ) {
            result = true;
            break;
        }
    }

    ESLog::ISLog("IS EXIST UNINITILIZE READ STREAM END\n", __FILE__, __LINE__);

    return result;

}


/* is the readStream set has reading stream */
bool StreamManager::IsExistReadStatusStream(){

    ESLog::ISLog("IS EXIST READ STREAM START\n", __FILE__, __LINE__);

    bool result = false;

    list<WebStream *>::iterator ite;
    for( ite = this->readStream.begin(); ite != this->readStream.end(); ++ite ) {
        
        WebStream *webStream = (*ite);

        ESLog::ISLog("read webStream->http ptr:%u\n", __FILE__, __LINE__, webStream->GetHttpPtr());
        ESLog::ISLog("read webStream->fd:%u\n", __FILE__, __LINE__, webStream->GetTcpConnFd());
        ESLog::ISLog("read webStream->Status:%s\n", __FILE__, __LINE__, webStream->GetStatusDesc());

        if( (*ite)->GetStatus() == WSTREAM_READ ) {
            result = true;
            break;
        }
    }

    ESLog::ISLog("IS EXIST READ STREAM END\n", __FILE__, __LINE__);

    return result;

}

/* Get the readWebStream size */
int StreamManager::GetReadWebStreamSize(){
    //ESLog::ISLog("readStreamSize:%d\n",__FILE__, __LINE__,  this->readStream.size() );
    return this->readStream.size();
}

/* delete an webWrite Stream in the pool */
int StreamManager::DeleteWriteWebStream( WebStream *writeWebStream ){

    int result = 0;

    list<WebStream *>::iterator ite;
    for( ite = this->writeStream.begin(); ite != this->writeStream.end(); ++ite ) {
        if( *ite == writeWebStream ) {
            this->writeStream.erase(ite);
            result = 1;
            break;
        }
    }

    return result;
}

/* add an webStream into taskStream */
int StreamManager::AddTaskWebStream(WebStream *taskStream ){

    int result = 0;

    this->taskStream.push_back(taskStream);

    return result;

}

/* Get the next undispatcher TaskStream */
WebStream * StreamManager::GetNextUndispatherTaskStream(){

    WebStream *webStream = NULL;
    WebStream *stream = NULL;
    list<WebStream *>::iterator ite;

    ESLog::ISLog("GET NEXT UNDISPATCHER TASKSTREAM START\n", __FILE__, __LINE__);
    ESLog::ISLog("taskStreamSize:%d\n", __FILE__, __LINE__, this->taskStream.size());
    /* loop */
    for( ite = this->taskStream.begin(); ite != this->taskStream.end(); ++ite ) {
        webStream = *ite;
       
        if( (*ite)->GetHttpPtr() == NULL /*&& (*ite)->GetTcpConn() == NULL */) {
            ESLog::ISLog("delete a webStream from taskStream\n", __FILE__, __LINE__);
            if( this->taskStream.size() == 1 ) {
                ESLog::ISLog("There is only one in taskStream be careful \n", __FILE__, __LINE__);
                this->taskStream.erase(ite);
                break;
            }
            continue;
        }
        

        ESLog::ISLog("task webStream->http:%u\n", __FILE__, __LINE__, webStream->GetHttpPtr());
        ESLog::ISLog("task webStream->fd:%d\n", __FILE__, __LINE__, webStream->GetTcpConnFd());

        if( webStream->GetHttpPtr() != NULL ) {
            ESLog::ISLog("task WebStream->HttpStatus:%s\n", __FILE__, __LINE__, webStream->GetHttpPtr()->GetStatusDesc());
        }
        ESLog::ISLog("task WebStream->Status:%s\n", __FILE__, __LINE__, webStream->GetStatusDesc());

        /* is the readStream parser header */
        if( (*ite)->GetStatus() == WSTREAM_UNDISPATHED || (*ite)->GetHttpPtr()->GetStatus() == HTTP_REQHEAD_PARSED ) {
            ESLog::ISLog("there is taskStream to tackle\n", __FILE__, __LINE__);
            stream = *ite;
            this->taskStream.erase(ite);
            break;
        }


    }
    ESLog::ISLog("GET NEXT UNDISPATCHER TASKSTREAM END\n", __FILE__, __LINE__);

    return stream;
}

/* Get the size of the taskWebStream */
int StreamManager::GetTaskWebStreamSize(){
    return this->taskStream.size();
}

/* Loop the readStream list, and return the already stream */
int StreamManager::HttpParseHeader(){

    WebStream *webStream = NULL;
    list<WebStream *>::iterator ite;
    
    ESLog::ISLog("HTTP HEADER SETS PARSE START\n", __FILE__, __LINE__);

    ESLog::ISLog("readStreamSize:%d\n", __FILE__, __LINE__, this->readStream.size());
    /* loop */
    for( ite = this->readStream.begin(); ite != this->readStream.end(); ++ite ) {

        int result = -1;
        webStream = *ite;

        
        ESLog::ISLog("read webStream->http ptr:%u\n", __FILE__, __LINE__, webStream->GetHttpPtr());
        ESLog::ISLog("read webStream->fd:%d\n", __FILE__, __LINE__, webStream->GetTcpConnFd());
        ESLog::ISLog("read webStream->Status:%s\n", __FILE__, __LINE__, webStream->GetStatusDesc());

        /* if the readStream has writed */
        if( webStream->GetStatus() == WSTREAM_WRITE ) {
            continue;
        }

        /* is http protocol parser exist */
        if( !webStream->IsHttpParserExist() ) {
            ESLog::ISLog("An new Http Object Create \n", __FILE__, __LINE__ );
            Http *httpParser = new Http();
            httpParser->SetWebStreamPtr(webStream);
            webStream->SetHttpParser(httpParser);
            ESLog::ISLog("New Http read webStream->http ptr:%u\n", __FILE__, __LINE__, webStream->GetHttpPtr());
        }

        ESLog::ISLog("read WebStream->HttpStatus:%s\n", __FILE__, __LINE__, webStream->GetHttpPtr()->GetStatusDesc());

        /* if the header has parserd*/
        if( webStream->GetHttpPtr()->GetStatus() == HTTP_REQHEAD_PARSED  ) {
            continue;
        }

        /* if the latest http parsed over, again init http object */
        if( webStream->GetHttpPtr()->GetStatus() == HTTP_RESSEND_OVER  ) {
            
            webStream->GetHttpPtr()->ReInit();
        }

        /* parse the http header, if the http header parsed, then we give the webStream to the
           session/application, it will send to a idle thread to tackle the session
           if return 0, the header parser over; if return > 0 , it still to parser;
           if return < 0 , there is an error happend */
        result = webStream->ParseHttpHeader();  // <--i) set the status of the webStream , http Status
        if( result < 0 ) {
            
            ESLog::ISLog("ParseHttpHeader Erorr START\n", __FILE__, __LINE__ );
            webStream->SetStatus(WSTREAM_ERROR);
            //webStream->GetHttpPtr()->SetStatus(HTTP_RESSEND_OVER);
            //Server::Instance()->GetIOLoop().ModToReadEventFd(webStream->GetTcpConnFd(), (void *)webStream);
            //webStream->ReInit();
            //this->DeleteReadWebStream(webStream);

            Server::Instance()->GetIOLoop().DelReadEventFd(webStream->GetTcpConnFd());

            //webStream->FreeTcpConn();
            //webStream->FreeHttp();
            //webStream->ReInit();
            //webStream->SetStatus(WSTREAM_UNINITILIZED);
            //this->AddIdleStream(webStream);

            ESLog::ISLog("ParseHttpHeader Erorr END\n", __FILE__, __LINE__ );
            continue;
            //break;
            //return  result;  // an error happend
        }

        /* the header has passed  */
        if( webStream->GetHttpPtr()->GetStatus() == HTTP_REQHEAD_PARSED ) {
            //webStream->GetHttpPtr()->SetStatus(2);
            ESLog::ISLog("there is http head parsed\n", __FILE__, __LINE__);
            webStream->SetStatus(WSTREAM_UNDISPATHED);
            this->AddTaskWebStream(webStream);
        }
        
    }

    ESLog::ISLog("HTTP HEADER SETS PARSE END\n", __FILE__, __LINE__);
}


/* clip the readStream has taskStream */
int StreamManager::ClipReadToTaskStream(){

    int result = 0;

    WebStream *webStream = NULL;
    list<WebStream *>::iterator ite;

    /* loop */
    for( ite = this->readStream.begin(); ite != this->readStream.end(); ++ite ) {

        webStream = *ite;
        /* is the readStream parser header */
        if( webStream->GetHttpPtr() != NULL && webStream->IsHttpParseHeaderOver() ) {

            this->DeleteReadWebStream(webStream);
            webStream->SetStatus(WSTREAM_UNDISPATHED);
            this->AddTaskWebStream(webStream);
 
        }

    }

    return 0;

}

/* clip the readStream to idle stream */
int StreamManager::ClipReadToIdleStream(){

    int result = 0;

    WebStream *webStream = NULL;
    list<WebStream *>::iterator ite;

    ESLog::ISLog("CLIP READ TO IDLE START\n", __FILE__, __LINE__);

    /* loop */
    for( ite = this->readStream.begin(); ite != this->readStream.end(); ++ite ) {

        webStream = *ite;
        
        ESLog::ISLog("readStream status:%s\n", __FILE__, __LINE__, webStream->GetStatusDesc());
        /* is the readStream parser header */
        if( webStream->GetStatus() == WSTREAM_ERROR ) {
           
            //this->DeleteReadWebStream(webStream);
            *ite = NULL;
            webStream->FreeTcpConn();
            webStream->ReInit();
            webStream->SetStatus(WSTREAM_UNDISPATHED);
            this->AddIdleStream(webStream);      
            ESLog::ISLog("An readStream clip to idle stream\n", __FILE__, __LINE__);
 
        }
    }

    this->readStream.remove(NULL);

    ESLog::ISLog("CLIP READ TO IDLE END\n", __FILE__, __LINE__);

    return 0;
}

/* clip the taskStream has writeStream */
int StreamManager::ClipTaskToWriteStream(){

    int result = 0;

    WebStream *webStream = NULL;
    list<WebStream *>::iterator ite;

    /* loop */
    for( ite = this->taskStream.begin(); ite != this->taskStream.end(); ++ite ) {

        webStream = *ite;
        /* is the readStream parser header */
        if( webStream->GetStatus() == WSTREAM_WRITE ) {

            this->AddWriteWebStream(webStream);
            this->DeleteTaskWebStream(webStream);
        }

    }

    return 0;

}


/* add an writeStream */
int StreamManager::AddWriteWebStream(WebStream *webStream){
    this->writeStream.push_back(webStream);
}

/* delete from webStream from taskWebStream  */
int StreamManager::DeleteTaskWebStream(WebStream *webStream ){

    int result = 0;

    list<WebStream *>::iterator ite;
    for( ite = this->taskStream.begin(); ite != this->taskStream.end(); ++ite ) {
        if( *ite == webStream ) {
            this->taskStream.erase(ite);
            result = 1;
            break;
        }
    }

    return result;
}
