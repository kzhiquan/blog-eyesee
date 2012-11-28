
#ifndef _HTTPREQUEST_H_
#define _HTTPREQUEST_H_


#include <map>
using namespace std;


#include "Uri.h"



/* httpRequest */
class HttpRequest{
private:
    char *              reqCmd;
    char *              httpVersion;
    Uri                 uri;
    map<string, string> reqHeaders;
    
public:
    /* constructor */
    HttpRequest();

    /* reInit*/
    void ReInit();

    /* set the request cmd type */
    void SetCmdType( char *strBuf );

    /* Get cmd type */
    char *GetCmdType();

    /* set the request uri */
    void SetUri(char *strBuf, int lenght );

    /* Get Uri Object */
    Uri & GetUri();

    /* is the uri format right */
    bool IsUriFormatRight( char *strBuf, int length );

    /* set http version */
    void SetHttpVersion( char *strBuf );

    /* Get http version */
    char * GetHttpVersion();

    /* set reqHeaders */
    void SetHeaderValueByName(char *strName, int nameLen, char *strValue, int valueLen );

    /* Get http Request content-lenght */
    int GetHttpRequestContentLenght();


};






#endif
