
#include "CFile.h"
#include "ESLog.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>

/* is a file */
bool CFile::IsFile(char *file ){
    bool result = false;
    int  err = 0;
    struct stat fileState;

    err = stat( file, &fileState );
    if( err < 0 ) {
        return result;
    }

    result = S_ISREG(fileState.st_mode);


    return result;
}

/* is a directory */
bool CFile::IsDirectory( char *directoryPath ){
    bool result = false;
    int  err;
    struct stat fileState;

    err = stat( directoryPath, &fileState );
    if( err < 0 ) {
        return result;
    }

    result = S_ISDIR(fileState.st_mode);


    return result;
}


/* Get the file Size */
int CFile::GetFileSize(char *fileFullName ){
    int fileSize = 0;
    int err = 0;
    struct stat fileState;

    err = stat( fileFullName, &fileState );
    if( err < 0 ) {
        ESLog::ISLog("ERROR! fileFullName:%s\n", __FILE__, __LINE__, fileFullName );
        return err;
    }

    fileSize = fileState.st_size;

    return fileSize;
}



/* is the file in the directory local */
bool CFile::IsFileIn( const char *file ){
    bool    result = false;
    int     n = 0;
    char    name[256];

    memset(name, 0, 256 );
    strcpy(name, this->rootDirectory.c_str());
    n = strlen(this->rootDirectory.c_str());
    strcpy( name+n, file);
    
    vector<string*>::iterator ite;
    for( ite = this->files.begin(); ite != this->files.end(); ++ite ) {
        //ESLog::ISLog("%s\n", __FILE__, __LINE__, (*ite)->c_str());
        //ESLog::ISLog("%s\n", __FILE__, __LINE__, name);
        if( strcmp( (*ite)->c_str(), name ) == 0 ) {
            result = true; break;
        }
    }

    return result;
}

/* open the relative file base on the rootDirectory */
int CFile::GetFileFd( const char *file ){

    int     fd;
    int     n = 0;
    char    name[256];

    memset(name, 0, 256 );
    strcpy(name, this->rootDirectory.c_str());
    n = strlen(this->rootDirectory.c_str());
    strcpy( name+n, file);

    /* open the file */
    fd = open( name, O_RDONLY );
    if( fd < 0 ) {
        return  -1;
    }

    return fd;
}

/* is the file in the directory */
bool CFile::IsFileInDirectory( char *file, char *directory ){


}



/* Get the files under the directory */
int CFile::GetFiles(const char *directoryPath, vector<string>& files ){
    ESLog::ISLog("CFILE GET FILES START\n", __FILE__, __LINE__);

    ESLog::ISLog("directory path:%s\n", __FILE__, __LINE__, directoryPath );
    
    int   err = 0;
    DIR   *pDir = NULL;
    struct dirent   *pDirent = NULL;

    char  parentdircbuf[256];
    memset(parentdircbuf, 0, sizeof(parentdircbuf));
    sprintf(parentdircbuf, "%s/%s", this->rootDirectory.c_str(), directoryPath);
    
    ESLog::ISLog("parentdircbuf:%s\n", __FILE__, __LINE__, parentdircbuf);

    pDir = opendir( parentdircbuf );
    if( pDir == NULL ) {
        ESLog::ISLog("error: %s\n", __FILE__, __LINE__, strerror(errno));
        return -1;
    }

    /* parse the directory */
    while( (pDirent = readdir(pDir)) != NULL ) {
        char name[256];
        int  n = 0;

        ESLog::ISLog("%s\n", __FILE__, __LINE__, pDirent->d_name);
        /* current directory or parent directory skip */
        if( strcmp( pDirent->d_name, ".") == 0 || strcmp( pDirent->d_name, "..") == 0 ) {
            continue;
        }

        /* get the fullfilePath name */
        memset(name, 0, sizeof(name));
        sprintf(name, "%s/%s", parentdircbuf, pDirent->d_name);

        ESLog::ISLog("fullfilepath name:%s\n", __FILE__, __LINE__, name);
        /* if it is a file */
        if( this->IsFile(name) ) {
            string file;
            file.append(pDirent->d_name);
            files.push_back(file);
        }
    }

    closedir(pDir);

  
    ESLog::ISLog("CFILE GET FILES END\n", __FILE__, __LINE__);

    return err;
}


/* Get the files under the directory */
int CFile::GetDirectories(const char *directoryPath, vector<string>& directories ){
    ESLog::ISLog("CFILE GET DIRECTORIES START\n", __FILE__, __LINE__);

    ESLog::ISLog("directory path:%s\n", __FILE__, __LINE__, directoryPath );
    
    int   err = 0;
    DIR   *pDir = NULL;
    struct dirent   *pDirent = NULL;

    char  parentdircbuf[256];
    memset(parentdircbuf, 0, sizeof(parentdircbuf));
    sprintf(parentdircbuf, "%s/%s", this->rootDirectory.c_str(), directoryPath);
    
    ESLog::ISLog("parentdircbuf:%s\n", __FILE__, __LINE__, parentdircbuf);

    pDir = opendir( parentdircbuf );
    if( pDir == NULL ) {
        ESLog::ISLog("error: %s\n", __FILE__, __LINE__, strerror(errno));
        return -1;
    }

    /* parse the directory */
    while( (pDirent = readdir(pDir)) != NULL ) {
        char name[256];
        int  n = 0;

        ESLog::ISLog("%s\n", __FILE__, __LINE__, pDirent->d_name);
        /* current directory or parent directory skip */
        if( strcmp( pDirent->d_name, ".") == 0 /*|| strcmp( pDirent->d_name, "..") == 0 */) {
            continue;
        }

        /* get the fullfilePath name */
        memset(name, 0, sizeof(name));
        sprintf(name, "%s/%s", parentdircbuf, pDirent->d_name);

        ESLog::ISLog("fullfilepath name:%s\n", __FILE__, __LINE__, name);
        /* if it is a file */
        if( this->IsDirectory(name) ) {
            string directory;
            directory.append(pDirent->d_name);
            directories.push_back(directory);
        }
    }

    closedir(pDir);

  
    ESLog::ISLog("CFILE GET DIRECTORIES END\n", __FILE__, __LINE__);

    return err;
}



/* Set the root directory */
void CFile::SetRootDirectory( char *rootDirectory ){

    this->rootDirectory.append(rootDirectory);

}

/* Get the root directory */
string & CFile::GetRootDirectory(){
    return this->rootDirectory;
}



/* Get the full file of name */
char *CFile::GetFullFileName( const char *fileName, int size ){
    int n = 0;
    char  fullFile[256];

    memset( fullFile, 0, sizeof(fullFile) );
    n = this->rootDirectory.size();
    memcpy( fullFile, this->rootDirectory.c_str(), n );

    fullFile[n]='/';

    memcpy( fullFile+n, fileName, size );

    fullFile[n+size+1] = '\0';

    return fullFile;
}

/* Get files recursive in the direcotory */
int CFile::ReGetFile( const char* directory ){

    int             err = 0;
    DIR             *pDir = NULL;
    struct dirent   *pDirent = NULL;

    ESLog::ISLog("directory:%s\n", __FILE__, __LINE__, directory);
    /* open the rootDirectory */
    pDir = opendir( directory );
    if( pDir == NULL ) {
        ESLog::ISLog("error: %s\n", __FILE__, __LINE__, strerror(errno));
        return -1;
    }

    /* parse the directory */
    while( (pDirent = readdir(pDir)) != NULL ) {
        char name[256];
        int  n = 0;

        ESLog::ISLog("%s\n", __FILE__, __LINE__, pDirent->d_name);
        /* current directory or parent directory skip */
        if( strcmp( pDirent->d_name, ".") == 0 || strcmp( pDirent->d_name, "..") == 0 ) {
            continue;
        }

        /* get the fullfilePath name */
        memset(name, 0, sizeof(name));
        n = strlen(directory);
        strcpy( name, directory );
        name[n]='/';
        strcpy( name+n+1, pDirent->d_name );

        ESLog::ISLog("fullfilepath name:%s\n", __FILE__, __LINE__, name);
        /* if it is a file */
        if( this->IsFile(name) ) {
            string *file = new string();
            file->append(name);
            this->files.push_back(file);
        }

        /* if it is a directory */
        else if( this->IsDirectory(name)) {
            this->ReGetFile(name);
        }
    }

    closedir(pDir);

    return err;
}

/* init the files */
int CFile::Init( char *rootDirectory ){
    
    ESLog::ISLog("SITE FILES INIT START\n", __FILE__, __LINE__);
    int  result = 0;

    /* set the root Directory */
    if( this->rootDirectory.length() == 0 ) {
          this->rootDirectory.append(rootDirectory);
    }

    this->ReGetFile((this->rootDirectory).c_str());

    ESLog::ISLog("SITE FILES INIT END\n", __FILE__, __LINE__);

    return result;
  
}
