
#ifndef _IOLOOP_H_
#define _IOLOOP_H_

#include <sys/epoll.h>

/* tackle io, while , single */
class IOLoop{

private:
    int epfd;
    int pollSize;
    int maxEvents;
    struct epoll_event *events;
    struct epoll_event ev;

public:
    IOLoop();
    ~IOLoop();

    /* set the maxEvents of the events array */
    void SetMaxEvents(int maxEvents);
    /* set the pollSize of the epfd */
    void SetPollSize(int pollSize);


    /* create the epoll instance in the kernel */
    int Init();

    /* register the fd to read event */
    int RegReadEventFd( int fd );

    /* delete the fd */
    int DelWriteEventFd( int fd );

    /* delete read fd */
    int DelReadEventFd( int fd );

    /* delete by index of events[]*/
    int DelEventFd( int index, int fd );

    /* register the fd to read event */
    int RegReadEventFd( int readfd, void * data );

    /* modify the fd to write event */
    int ModToWriteEventFd( int writefd );

    /* register the fd to write event */
    int RegWriteEventFd( int writefd );

    /* modify the fd to read event */
    int ModToReadEventFd( int readfd, void * data );

    /* modify the fd to write event */
    int ModToWriteEventFd( int fd, void * data );

    /* wait unitl any event happpend and to tackle, or blocked */
    int Loopfd();

};




#endif
