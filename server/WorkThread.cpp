

#include "WorkThread.h"
#include "ESLog.h"

#include <stdio.h>



/* constructor */
WorkThread::WorkThread(){
    this->task = NULL;
    this->tid = 0;
    this->status = WTHREAD_UNSTART;

    /* dynamic initilize condition */
    pthread_cond_init( &this->qready, NULL );

    /* dynamic initilize the mutex */
    pthread_mutex_init( &this->qlock, NULL );
}

/* is the workTread idle */
bool WorkThread::IsStatusIdle(){
    bool result = false;

    if( this->status == WTHREAD_IDLE ) {
        result = true;
    }

    return result;
}

/* set the workThread idle */
void WorkThread::SetStatusIdle(){
    this->status = WTHREAD_IDLE;
}





/* set the status of the WorkThread */
void WorkThread::SetStatus( WorkThreadStatus status ){

    pthread_mutex_lock(&this->qlock);
    this->status = status;
    pthread_mutex_unlock(&this->qlock);
    
}

/* Get the status of the workThread */
WorkThreadStatus WorkThread::GetStatus(){
    return this->status;
}


/* set the workThread task */
void WorkThread::SetTask( Task *task ){

    pthread_mutex_lock(&this->qlock);
    this->task = task;
    pthread_mutex_unlock(&this->qlock);
}

/* set the thread id */
void WorkThread::SetThreadID( pthread_t tid ){
    this->tid = tid;
}

/* get thread id */
pthread_t  WorkThread::GetThreadID(){
    return this->tid;
}

/* condtion signal */
int WorkThread::TaskConditionSignal(){
    pthread_cond_signal(&this->qready); 
}

/* static method, the thread run method */
void *WorkThread::HandleTask( void *arg ){

    /* start */
    ESLog::ISLog("WORKTHREAD START\n", __FILE__, __LINE__);

    WorkThread *workThread = (WorkThread* )arg;

    while(1) {
        /* blocked or not, wait for the event */
        /* first lock */
        pthread_mutex_lock(&(workThread->qlock));

        ESLog::ISLog("WorkThread task ptr %u\n", __FILE__, __LINE__, workThread->task);

        while( workThread->task == NULL ) {//just wait 
            pthread_cond_wait( &(workThread->qready), &(workThread->qlock) );
        }
        /* set the status of the taskThread */
        //workThread->status = WTHREAD_BUSY;
        pthread_mutex_unlock(&(workThread->qlock));

        //ESLog::ISLog("WorkThread for ", workThread->)

        /* handle work */
        workThread->task->Work();


        pthread_mutex_lock(&(workThread->qlock));
        /* set the status of the taskThread */
        workThread->status = WTHREAD_IDLE;
        delete workThread->task;
        workThread->task = NULL;
        pthread_mutex_unlock(&(workThread->qlock));
    }
   


    ESLog::ISLog("WORKTHREAD END\n", __FILE__, __LINE__);
}
