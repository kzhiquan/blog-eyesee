

#ifndef _WORKTHREAD_H_
#define _WORKTHREAD_H_

#include <pthread.h>
#include "Task.h"

enum WorkThreadStatus{
    WTHREAD_IDLE,
    WTHREAD_BUSY,
    WTHREAD_UNSTART
};

class WorkThread{
private:
    pthread_t           tid;
    pthread_cond_t      qready;    //condition
    pthread_mutex_t     qlock;    //lock
    WorkThreadStatus    status;
    Task                *task;
public:
    WorkThread();

    /* is the workTread idle */
    bool IsStatusIdle();

    /* set the workThread idle */
    void SetStatusIdle();

    /* set the status of the WorkThread */
    void SetStatus(WorkThreadStatus status);

    /* Get the status of the workThread */
    WorkThreadStatus GetStatus();

    /* Set the status of the WorkThread */
    //void SetStatus( WorkThreadStatus status );

    /* set the workThread task */
    void SetTask( Task *task );

    /* set the thread id */
    void SetThreadID( pthread_t tid );

    /* get thread id */
    pthread_t  GetThreadID();

    /* static method, the thread run method */
    static void *HandleTask( void *arg );  
    
    /* condtion signal */
    int TaskConditionSignal(); 

};





#endif
