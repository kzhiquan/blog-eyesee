#ifndef _CFILE_H_
#define _CFILE_H_


#include <string>
#include <vector>
using namespace std;


class CFile{
private:
    string               rootDirectory;
    vector<string *>     files;
public:

    /* is a file */
    bool IsFile(char *file );

    /* is a directory */
    bool IsDirectory( char *directoryPath );

    /* is the file in the directory local */
    bool IsFileIn( const char *file );

    /* Get the file Size */
    int GetFileSize(char *fileFullName );

    /* open the relative file base on the rootDirectory */
    int GetFileFd( const char *file );

    /* is the file in the directory */
    bool IsFileInDirectory( char *file, char *directory );

    /* Get the full file of name */
    char *GetFullFileName( const char *fileName, int size );

    /* Get the files under the directory */
    int GetFiles(const char *directoryPath, vector<string>& files );

    /* Get the directory under the directory */
    int GetDirectories(const char *directoryPath, vector<string>& directories );

    /* Set the root directory */
    void SetRootDirectory( char *rootDirectory );

    /* Get the root directory */
    string & GetRootDirectory();

    /* init the files */
    int Init( char *rootDirectory=NULL );

    /* Get files recursive in the direcotory */
    int ReGetFile(const char* directory );
};






#endif
