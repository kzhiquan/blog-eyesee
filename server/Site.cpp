
#include "Site.h"
#include "ESLog.h"
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <stdio.h>

/* constructor */
Site::Site(){

}



/* init */
int Site::Init(){

    int result = 0;

    result = this->sitefiles.Init();

    return result;
}

/* Get SiteFileManager */
CFile & Site::GetFileManager(){

    return this->sitefiles;

}


/* start up the listening  */
int Site::StartLisnNet(){
    
   int result = 0;

   ESLog::ISLog("SITE LISN NET START\n", __FILE__, __LINE__);

   /* start to listen the network */
   result = this->lisnTcpConn.InitLisnSocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, this->lisnBackLog);
   if( result < 0 ) {
       return result;
   }

   /* set non block */
   result  = this->lisnTcpConn.SetNonBlock();
   if( result < 0 ) {
       return result;
   }


   ESLog::ISLog("SITE LISN NET END\n", __FILE__, __LINE__);


   return result ;
}


/* get the site listen fd */
int Site::GetSiteLisnFd(){
    return this->lisnTcpConn.GetTcpConnectionFd();
}


/* Accpet TcpConnection */
TcpConnection * Site::AcceptTcpConn(){
    return this->lisnTcpConn.LisnFdAcceptConn();
}


/* set ip address */
void Site::SetIp( char *strIP ){
    this->lisnTcpConn.SetIP(strIP);
}

/* return the ip address */
string& Site::GetIp(){
    return this->lisnTcpConn.GetIP();
}

/* set the port */
void Site::SetPort( char* strPort ){
    this->lisnTcpConn.SetPort(atoi(strPort));
}

/* return the port */
int Site::GetPort(){
    this->lisnTcpConn.GetPort();
}

/* set the listen fd waiting list size */
void Site::SetLisnBackLog( char * strLisnBackLog ){
    this->lisnBackLog =  atoi(strLisnBackLog);
}

/* set the root directory */
void Site::SetRootDir(char* strRootDir ){
    this->sitefiles.SetRootDirectory(strRootDir);
}

/* return the root directory */
string& Site::GetRootDir(){
    return this->sitefiles.GetRootDirectory();
}

/* set the defaultPage */
void Site::SetDefaultPage(char* strDefaultPage){
    this->defaultPage.append(strDefaultPage);
}
/* return the defaultPage */
string& Site::GetDefaultPage(){
    return this->defaultPage;
}

/* is file exist */
bool Site::IsFileExist(const char *file ){
    bool result = false;

    ESLog::ISLog("file %s\n", __FILE__, __LINE__, file);

    /* find the default page */
    result = this->sitefiles.IsFileIn(file);

    return result;
}


/* Get file size */
int Site::GetFileSize(const char *file ){
     int size;

    char *fileFullName = NULL;
    fileFullName = this->sitefiles.GetFullFileName(file, strlen(file));

     /* find the default page */
    if( strcmp(file, "/") == 0 ) {
        strcat(fileFullName, this->defaultPage.c_str());
    }

    size = this->sitefiles.GetFileSize(fileFullName);
    if(  size < 0 ) {
        return size;
    }

    return size;

}

/* open urifile */
int Site::OpenUriFile(const char *UriFile ){
    return this->sitefiles.GetFileFd(UriFile);
}
