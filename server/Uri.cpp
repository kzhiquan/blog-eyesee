

#include "Uri.h"
#include "ESLog.h"
#include <stdio.h>


/* constructor */
Uri::Uri(){

}

Uri::Uri( string uri ){
    this->uri = uri;
}

/* reInit */
void Uri::ReInit(){

    this->file.clear();
    this->uri.clear();
    this->query.clear();
}


/* is valid */
bool Uri::IsValid(){

    bool result = true;

    if( this->uri.length() == 0 || this->uri[0] != '/') {
        result = false;
    }

    return result;
}

/* is query exist */
bool Uri::IsQueryExist(){
    bool result = false;
    
    map<string, string>::iterator ite;
    for( ite = this->query.begin(); ite != this->query.end(); ++ite ) {

        printf("name:%s\n", (ite->first).c_str());
        printf("value:%s\n", (ite->second).c_str() );

    }

    if(!this->query.empty()){
        result = true;
    }

    return result;

}

/* is valid */
bool Uri::IsValid(char *strBuf, int length ){

    bool result = true;



    if( strBuf[0] != '/') {
        result = false;
    }

    return result;
}

/* set the uri char */
void Uri::SetContent( char *buf, int length ){

    this->uri.append(buf, length);

}

/* parse uri */
int Uri::ParseUri(){

    int len = this->uri.length();

    ESLog::ISLog("URI PARSE START\n", __FILE__, __LINE__);

    ESLog::ISLog("len:%d\n", __FILE__, __LINE__, len);

    int i = 0;
    //parse the file
    ESLog::ISLog("Uri:%s\n", __FILE__, __LINE__, this->uri.c_str());
    for( ; i < len; i++ ) {
        //ESLog::ISLog("%c\n", __FILE__, __LINE__, this->uri[i]);
        if( uri[i] == '?') {
            break;
        }
    }
    this->file.append(this->uri, 0, i );

    ESLog::ISLog("file:%s\n", __FILE__, __LINE__, this->file.c_str());

    //parse the query name, value.
    int j = i+1;
    while( j < len ) {
     
        /* find the name value */
        int k = j;
        while( this->uri[j] != '&' && j < len ){
            j++;
        }
        
        /* parse the query name value */
        this->ParseQueryParameters( this->uri.c_str(), k, j-1);
        j++;
    }

    ESLog::ISLog("URI PARSE END\n", __FILE__, __LINE__);
    return len;
}

/* parse query name value */
int Uri::ParseQueryParameters( const char *strBuf, int startIndex, int endIndex )
{
    
    int         pos = 0;
    string      name;
    string      value;

    /* find the seporate */
    for( pos = startIndex; pos < endIndex; pos++ ) {
        if( strBuf[pos] == '=') {
            break;
        }
    }

    ESLog::ISLog("startindex:%d\n", __FILE__, __LINE__, startIndex);
    ESLog::ISLog("endIndex:%d\n",__FILE__, __LINE__, endIndex);
    ESLog::ISLog("pos:%d\n", __FILE__, __LINE__, pos);
 
    name.append(strBuf+startIndex, pos-startIndex);
    value.append(strBuf+pos+1, endIndex-pos);

    this->query[name] = value;

    return pos;

}

/* Get file name */
string & Uri::GetFileName(){
    return this->file;
}

/* Get uri */
string & Uri::GetUriStr(){
    return this->uri;
}

/* Get query value by name */
string & Uri::GetQueryValuebyName(string name){
    return this->query[name];
}
