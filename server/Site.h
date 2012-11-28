
#ifndef _SITE_H_
#define _SITE_H_

#include <string>
using namespace std;


#include "TcpConnection.h"
#include "CFile.h"

class Site
{
private:
    CFile           sitefiles;    // manager the files of the site
    string          defaultPage;  // the default page according to "/"
    int             lisnBackLog;  // the max waitting list of the listen socketfd
    TcpConnection   lisnTcpConn;  // the tcpConnection as the site to listen

public:
    Site();

    /* init */
    int Init();

    /* init the site, just listen */
    int StartLisnNet();

    /* get the listen fd */
    int GetSiteLisnFd();

    /* Get SiteFileManager */
    CFile & GetFileManager();

    /* AcceptTcpRequest from the network */
    TcpConnection *AcceptTcpConn();

    /* set the port */
    void SetPort( char *strPort );
    /* get the port */
    int  GetPort();

    /* set the ip of the site */
    void SetIp( char *strIP);
    /* get the ip address of the site */
    string& GetIp();

    /* set the listen back log */
    void SetLisnBackLog( char *strlisnBackLog );

    /* set the root dir */
    void SetRootDir( char *strRootDir);
    /* get the root directory */
    string& GetRootDir();

    /* set the default page */
    void SetDefaultPage( char *strDefaultPage );
    /* get the default page */
    string& GetDefaultPage();

    /* is file exist */
    bool IsFileExist(const char *file );

    /* Get file size */
    int GetFileSize(const char *file );

    /* open urifile */
    int OpenUriFile(const char *UriFile );
};



#endif
