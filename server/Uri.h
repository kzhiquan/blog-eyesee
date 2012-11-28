#ifndef _URI_H_
#define _URI_H_

#include <string>
#include <map>
using namespace std;




class Uri{

private:
    string                  uri;
    string                  file;
    map<string, string>     query;

public:
    Uri();
    Uri(string uri);
    
    /* reInit */
    void ReInit();

    /* is the uri valid */
    bool IsValid();

    /* has query info */
    bool IsQueryExist();

    bool IsValid(char *strBuf, int length );

    /* set the uri char */
    void SetContent( char *buf, int length );

    /* parse uri */
    int ParseUri();

    /* Get uri */
    string &GetUriStr();

    /* Get file name */
    string & GetFileName();

    /* Get query value by name */
    string & GetQueryValuebyName(string name);

    /* parse query name value */
    int ParseQueryParameters( const char *strBuf, int startIndex, int endIndex );
};

#endif
