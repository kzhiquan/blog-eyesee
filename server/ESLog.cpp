

#include "ESLog.h"
#include "Server.h"


#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <sys/stat.h>

#include <sys/types.h>
#include <linux/unistd.h>
#define gettid() syscall(__NR_gettid)




/* buff */
char  ESLog::buff[LOG_BUF_SIZE];

/* Load file Name */
void ESLog::LoadLogFileName( const char *fileName ){
    printf("%s\n", fileName);
    this->file.append(fileName);
}

pthread_mutex_t * ESLog::GetESLogMutex(){
    return &(this->qlock);

}

/* init , just open the file */
int ESLog::Init(){
    int err = 0;

    this->first = 0;
    this->last = 0;
    this->fd = -1;
    memset( ESLog::buff, 0, sizeof( ESLog::buff) );

    /* dynamic initilize the mutex */
    pthread_mutex_init( &this->qlock, NULL );


    printf("filename:%s\n", this->file.c_str());
    /* open the file to write */
    this->fd = open( this->file.c_str(), O_APPEND|O_WRONLY|O_CREAT, 00664);
    if( this->fd < 0 ) {
        printf("open file failure:%s\n", strerror(errno));
        return this->fd;
    }

    /* write log start */
    write( this->fd, "\n\n#################### START #######################\n", sizeof("\n\n#################### START #######################\n"));
    //printf("\n\n#################### START #######################\n");

    return err;
}


/* Get Buf Size */
int ESLog::GetBufSize(){

    /* init */
    if( this->first == this->last ) {
        return ESLog::LOG_BUF_SIZE-1;
    }
    else if( this->first < this->last ) {
        return ESLog::LOG_BUF_SIZE - this->last + this->first;
    }
    else{
        return this->first-this->last;
    }
}

/* static method flush the buffer to fd */
void ESLog::flush(){

    ESLog & log = Server::Instance()->GetESLog();
    /* lock*/
    pthread_mutex_lock(log.GetESLogMutex());

    /* write buf to file */
    log.WriteBuf2File();

    /* unlock */
    pthread_mutex_unlock(log.GetESLogMutex());
}



/* static method , log */
void ESLog::ISLog(char *format, ...){

    string  logcontent;
    
    /* object log */
    ESLog & log = Server::Instance()->GetESLog();

    /* now system time */
    tm      *ptm;
    time_t  logtime;
    char    timebuf[50];
    
    memset(timebuf, 0, sizeof(timebuf) );

    //pthread_mutex_lock(&log.GetESLogMutex()); ? it is an error, just get an copy ?
    logtime = time(NULL);
    ptm = localtime(&logtime);
    strftime( timebuf, sizeof(timebuf), "%F %X", ptm);
    logcontent.append(timebuf);
    //pthread_mutex_unlock(&log.GetESLogMutex());

    /* process id, thread id */
    char buf[256];
    memset( buf, 0, sizeof(buf) );
    //sprintf(buf, ",%u %u", (unsigned int)getpid(), (unsigned int)pthread_self());
    sprintf(buf, ",%u %u", (unsigned int)getpid(), (unsigned int)gettid());
    logcontent.append(buf);


    /* content */
    char addformat[128];
    memset( addformat, 0, sizeof(addformat) );
    //printf("format:%s\n", format);
    sprintf(addformat, ",\%\%s \%\%d==>>%s", format );
    //printf("addformat:%s\n", addformat);

    char logBuffer[ONE_TIME_SIZE];
    memset(logBuffer, 0, sizeof(logBuffer));

    //pthread_mutex_lock(&log.GetESLogMutex());
    va_list ap;
    va_start(ap, addformat);
    vsprintf(logBuffer, addformat, ap);
    va_end(ap);
    //pthread_mutex_unlock(&log.GetESLogMutex());

    logcontent.append(logBuffer);
    //logcontent.append("\n");

    /* out put to the stdout */
    //printf("%s", logcontent.c_str());

    /* Log */
    Server::Instance()->GetESLog().Log(logcontent.c_str());

}

/* log */
void ESLog::Log( const char *buf ){

    int loglen = strlen(buf);
    const char *log = buf;
    int writesum = 0;

    //printf("logsize:%d\n", loglen );

    while( loglen > 0 )  {

        pthread_mutex_lock(&this->qlock);

        /* bufsize */
        int bufsize = this->GetBufSize(); // the buffer size may be changed when the
                                          // lock in the side, so we put it outside.

        //printf("bufsize:%d\n", bufsize);

        // there is no buf space 
        if( bufsize < loglen ) {
            
            /* write to file */
            this->WriteBuf2File();


            /* loglen is larger number */
            while( loglen / ESLog::LOG_BUF_SIZE != 0 ) {
                
                /* write to buf */
                this->Write2Buf( log+writesum, ESLog::LOG_BUF_SIZE );//errors
                loglen -= ESLog::LOG_BUF_SIZE;
                writesum += ESLog::LOG_BUF_SIZE;

                /* write to file */
                this->WriteBuf2File();
            }


            /* write to buf */
            this->Write2Buf( log+writesum, loglen );//errors
            writesum += loglen;
            loglen -= loglen;
        }
        else{
            this->Write2Buf(log+writesum, loglen );
            writesum += loglen;
            loglen = 0;
        }

        pthread_mutex_unlock(&this->qlock);
        
    }

}

/* update last index */
int ESLog::UpdateLastIndex( int index ){

    //pthread_mutex_lock(&this->qlock);
    this->last = index;
    if( this->last == ESLog::LOG_BUF_SIZE-1 ) {
        this->last = 0;
    }
    //pthread_mutex_unlock(&this->qlock);

    return 0;
}

/* update first index */
int ESLog::UpdateFirstIndex( int index ){

    //pthread_mutex_lock(&this->qlock);

    this->first = index;
    if( this->first == ESLog::LOG_BUF_SIZE-1 ) {
        this->first = 0;
    }

    //pthread_mutex_unlock(&this->qlock);

    return 0;

}

/* write to buf */
void ESLog::Write2Buf( const char *logcontent, int loglen ){
    char *pbuf = ESLog::buff;

    //printf("WRITE BUF TO STATIC BUF START\n");
    
    //printf("Write before:first:%d, last:%d\n", this->first, this->last );

    /* init */
    /*if( this->first == this->last ) {
        int tailbufsize = ESLog::LOG_BUF_SIZE - this->last;

        if( tailbufsize < loglen ) {
            memcpy( pbuf+this->last, logcontent, tailbufsize );

            memcpy( pbuf, logcontent+tailbufsize, loglen-tailbufsize );

            this->UpdateLastIndex(loglen-tailbufsize-1);
        }
        else{
            memcpy( pbuf+this->last , logcontent, loglen );
            this->UpdateLastIndex(this->last+loglen);
        }
    }*/
    if( this->first <= this->last ) {
        
        int tailbufsize = ESLog::LOG_BUF_SIZE - this->last;
        
        if(  tailbufsize >= loglen ) {
            memcpy( pbuf+this->last, logcontent, loglen );
            this->UpdateLastIndex(this->last+loglen);

        }
        else{
            memcpy( pbuf+this->last, logcontent, tailbufsize );

            memcpy( pbuf, logcontent+tailbufsize, loglen-tailbufsize);

            this->UpdateLastIndex(loglen-tailbufsize);
        }

    }
    else{
        memcpy( pbuf+this->last, logcontent, loglen );
        this->UpdateLastIndex(this->last+loglen);
    }


    //printf("Write after:first:%d, last:%d\n", this->first, this->last );

    //printf("WRITE BUF TO STATIC BUF END\n");
}

/* static method, wirte to buff */
void ESLog::WriteBuf2File(){

    //printf("WRITE BUF TO FILE START\n");

    char *pbuf = ESLog::buff;

    /* file size */
    struct stat  filestat;
    if( !fstat( this->fd, &filestat) ) {
    
        if( filestat.st_size >= 10485760 ) {

            close(this->fd);

            static int filecnt = 1;
            char filecntbuf[10];
            memset( filecntbuf, 0, sizeof(filecntbuf));
            sprintf( filecntbuf, "%d", filecnt);
            string curfilename;
            curfilename.append(this->file.c_str(), this->file.length()-1);
            curfilename.append(filecntbuf);

            filecnt++;
            if( filecnt == 3 ) {
                filecnt = 0;
            }

            /* open the file */
            this->fd = open( curfilename.c_str() , O_TRUNC|O_WRONLY|O_CREAT, 00664);
            if( this->fd < 0 ) {
                printf("open file failure:%s\n", strerror(errno));
            }
        }

    }

    if( this->first < this->last ) {
        //pthread_mutex_lock(&this->qlock);

        write( this->fd, pbuf+this->first, this->last - this->first);
        
        this->UpdateFirstIndex(this->last);

        //pthread_mutex_unlock(&this->qlock);
    }
    else{
        //pthread_mutex_lock(&this->qlock);

        write( this->fd, pbuf+this->first, ESLog::LOG_BUF_SIZE-this->first);
        write( this->fd, pbuf, this->last);
        this->UpdateFirstIndex(this->last);

        //pthread_mutex_unlock(&this->qlock);
    }

    //printf("WRITE BUF TO FILE END\n");

}
