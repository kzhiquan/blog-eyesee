
#ifndef _ES_LOG_H_
#define _ES_LOG_H_


#include <string>
using namespace std;



/*  log class object */
class ESLog{
private:
    string                  file;
    int                     fd;
    int                     first;
    int                     last;
    pthread_mutex_t         qlock;    //lock
    static  const int LOG_BUF_SIZE = 16384;
    static  const int ONE_TIME_SIZE = 8192;
    static  char    buff[LOG_BUF_SIZE];

public:

    /* load log file */
    void LoadLogFileName( const char *fileName );

    /* init */
    int Init();

    /* log interface  */
    static void ISLog( char *format, ...);

    /* fush the buffer to fd */
    static void flush();

    /* log internal */
    void Log( const char * buf );

    /* write to file */
    void WriteBuf2File();

    /* write to buf */
    void Write2Buf(const char *logcontent, int loglen );

    /* update last index */
    int UpdateLastIndex( int index );

    /* update first index */
    int UpdateFirstIndex( int index );

    pthread_mutex_t * GetESLogMutex();

    /* Get log buf size */
    int GetBufSize();

};







#endif
