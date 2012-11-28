

#include "HttpResponse.h"
#include <string.h>
#include <stdio.h>


/* constructor */
HttpResponse::HttpResponse(){

}

/* ReInit */
void HttpResponse::ReInit(){
    this->httpVersion.clear();
    this->resCode.clear();
    this->resPhrase.clear();
    this->resHeaders.clear();
}

/* set the respose code */
void HttpResponse::SetResCode( char *resCode ){
    this->resCode.clear();
    this->resCode.append(resCode);

}

/* set the httpVersion */
void HttpResponse::SetHttpVersion( char *httpVersion ){
    this->httpVersion.clear();
    this->httpVersion.append(httpVersion);
    //printf("%s\n", this->httpVersion.c_str());
}

/* set the resPhrase */
void HttpResponse::SetResPhrase( char *resPhrase ){
    this->resPhrase.clear();
    this->resPhrase.append(resPhrase);
}

/* set the resHeaders by name */
void HttpResponse::SetResHeaderValueByName( char* strname, char *strvalue ){

    string name;
    string value;
    
    name.append(strname);
    value.append(strvalue, strlen(strvalue) );

    this->resHeaders[name] = value;
}

/* Get the resHeaders by Name */
string HttpResponse::GetResHeaderValueByName( char *name ){
       
    map<string, string>::iterator ite;

    ite = this->resHeaders.find(string(name));

    if( ite != this->resHeaders.end() ) {
        return ite->second;
        //printf("cnt:%s\n",strBuf);
    }

    return "-1";
}

/* stream content out */
char *HttpResponse::StreamHeaderOut(){
    int            n = 0;
    int            m = 0;
    static char    buf[512];
    memset( buf, 0, sizeof(buf) );

    /* copy the httpversion */
    m = this->httpVersion.length();
    strcpy( buf, this->httpVersion.c_str() );
    buf[m] = ' ';

    /* copy the resCode */
    n = m;
    m = n + 1 + this->resCode.length();
    strcpy( buf+n+1, this->resCode.c_str());
    buf[m] = ' ';

    /* copy the resPhrase */
    n = m;
    m = n + 1 + this->resPhrase.length();
    strcpy( buf+n+1, this->resPhrase.c_str());
    buf[m] = '\r';
    buf[m+1] = '\n';

    
    /* copy the headers name: value\r\n*/
    map<string, string>::iterator ite;
    for( ite = this->resHeaders.begin(); ite != this->resHeaders.end(); ++ite ) {
        
        /* name */
        n = m+1;
        m = n + 1 + strlen((ite->first).c_str());
        strcpy(buf+n+1, (ite->first).c_str());
        buf[m] = ':';
        buf[m+1] = ' ';

        /* value */
        n = m+1;
        m = n + 1 + strlen((ite->second).c_str());
        strcpy(buf+n+1, (ite->second).c_str());
        buf[m] = '\r';
        buf[m+1] = '\n';
    }

    m = m +2;
    buf[m] = '\r';
    buf[m+1] = '\n';
    buf[m+2] = '\0';

    return buf;
}
