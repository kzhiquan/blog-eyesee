
#ifndef _HTTPRESPONSE_H_
#define _HTTPRESPONSE_H_


#include <string>
#include <map>
using namespace std;

class HttpResponse{
private:
    string                  httpVersion;
    string                  resCode;
    string                  resPhrase;
    map<string, string>     resHeaders;
    
public:
    HttpResponse();

    /* ReInit */
    void ReInit();

    /* set the respose code */
    void SetResCode( char *resCode );

    /* set the httpVersion */
    void SetHttpVersion(char *httpVersion );

    /* set the resPhrase */
    void SetResPhrase( char *resPhrase );

    /* set the resHeaders by name */
    void SetResHeaderValueByName( char* name, char *value );

    /* Get the resHeaders by Name */
    string GetResHeaderValueByName( char *name );

    /* stream content out */
    char *StreamHeaderOut();

};





#endif
