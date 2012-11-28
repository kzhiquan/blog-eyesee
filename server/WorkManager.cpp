

#include "WorkManager.h"
#include "Server.h"
#include "WebStream.h"
#include <stdio.h>


/* constructor */
WorkManager::WorkManager(){

    this->tid = 0;
    this->hasWorkToDo = false;
    this->taskPrimitive = NULL;

    /* dynamic initilize condition */
    pthread_cond_init( &this->qready, NULL );

    /* dynamic initilize the mutex */
    pthread_mutex_init( &this->qlock, NULL );

}

/* Set the has work to do */
void WorkManager::SetHasWorkToDo(bool flag){
    pthread_mutex_lock(&this->qlock);
    this->hasWorkToDo = flag;
    pthread_mutex_unlock(&this->qlock);
}

/* Get the has work to do flag */
bool WorkManager::GetHasWorkToDo(){
    return this->hasWorkToDo;
}




/* set the mananger thread id */
void WorkManager::SetThreadID( pthread_t tid ){
    this->tid = tid;
}


/* set the primitive fo the task */
int WorkManager::InitTaskPrimitive( Task *task ){
    this->taskPrimitive = task;
}

/* WorkManager thread triger to work */
int  WorkManager::WorkConditionSignal(){
    pthread_cond_signal(&this->qready); 
}

/* close the task */
Task * WorkManager::CloneTask(){

    Task *task = this->taskPrimitive->Clone();

    return task;
}


/* get an idle thread for the task */
WorkThread* WorkManager::GetIdleThread(){

    /* result */
    WorkThread *workThread = NULL;

    /* loop the threadpool, and see the status of the thread */
    vector<WorkThread*>::iterator ite;
    for( ite = this->threadPool.begin(); ite != this->threadPool.end(); ++ite ) {
        if( (*ite)->IsStatusIdle() ) {
            workThread = *ite;
            ESLog::ISLog("An idle Task Thread Get:%u!\n",__FILE__, __LINE__, (unsigned int)workThread->GetThreadID() );
            break;
        }
    }

    if( ite == this->threadPool.end() ) {
        ESLog::ISLog("An new Task Thread Get!\n", __FILE__, __LINE__);
        workThread = new WorkThread();
        workThread->SetStatus(WTHREAD_UNSTART);
        this->threadPool.push_back(workThread);
    }

    return workThread;
}



/* start up the taskThread */
int WorkManager::StartupTaskThread(WorkThread *workThread){

    int result = 0;
    pthread_t tid;


    /* is the workThread is unstart */
    if( workThread->GetStatus() == WTHREAD_UNSTART ) {
        /* set status */
        workThread->SetStatus(WTHREAD_BUSY);

        /* crerate the work thread */
        result = pthread_create( &tid, NULL, WorkThread::HandleTask, workThread );
        if( result < 0 ) {
            return result;
        }

        /* set the thread id */
        workThread->SetThreadID(tid);
    }
    else{
        /* set status */
        workThread->SetStatus(WTHREAD_BUSY);

        /* it is already startup, just trip the task Thread, condition */
        workThread->TaskConditionSignal();

    }




    return result;
}


/* static method */

void * WorkManager::WorkManagerThread( void *arg ){

    ESLog::ISLog("WORKMANAGER THREAD START\n",__FILE__, __LINE__);

    WorkManager *workManager = (WorkManager *)arg;

    /* thread body */
    while(1) {
        ESLog::ISLog("WORKMANAGER LOOP CYCLE START\n", __FILE__, __LINE__);
        /* just decide to sleep or work */
        pthread_mutex_lock(&(workManager->qlock));
        while( workManager->hasWorkToDo == false ) {
            pthread_cond_wait(&(workManager->qready), &(workManager->qlock));
        }
        pthread_mutex_unlock(&(workManager->qlock));
        //sleep(100);

        /* parser the http head, control the task Stream  */
        Server::Instance()->GetStreamManager().HttpParseHeader();

        /* tacke the taskStream */
        while(1) {
            ESLog::ISLog("WORKMANAGER HANDLE TASKSTREAM LOOP CYCLE START\n", __FILE__, __LINE__);

            /* get taskStream */
            WebStream *taskStream = Server::Instance()->GetStreamManager().GetNextUndispatherTaskStream();

            //if( taskStream == NULL && Server::Instance()->GetStreamManager().GetReadWebStreamSize() == 0 ) {
            if( taskStream == NULL ){
                break;
            }
      
            /* copy an taskPrimitive */
            Task *newTask = workManager->CloneTask();
            ESLog::ISLog("Application ptr:%u\n", __FILE__, __LINE__, newTask);

            /* set the application local vary http ptr */
            newTask->SetParameter( (void *)(taskStream->GetHttpPtr()) );

            /* Get an idle workThread */
            WorkThread * workThread = workManager->GetIdleThread();

            /* give the task to the thread */
            workThread->SetTask(newTask);

            /* set the status of the taskStream to dispathed */
            taskStream->SetStatus(WSTREAM_DISPATHED);

            /* start up the taskThread */
            workManager->StartupTaskThread(workThread);

            ESLog::ISLog("Work Manager: thread size:%d\n", __FILE__, __LINE__, workManager->threadPool.size());

            ESLog::ISLog("WORKMANAGER HANDLE TASKSTREAM LOOP CYCLE END\n", __FILE__, __LINE__);
        }

        /* whetcher to loop or not in workManager ? unparse http stream  */
        //if( ! Server::Instance()->GetStreamManager().IsExistReadStatusStream() ) {
        ESLog::ISLog("There is no task to handle in workManager cycle, just rest\n", __FILE__, __LINE__);
        workManager->SetHasWorkToDo(false);
        //}

        ESLog::ISLog("WORKMANAGER LOOP CYCLE END\n", __FILE__, __LINE__);
    }
    
    ESLog::ISLog("WORKMANAGER THREAD END\n", __FILE__, __LINE__);
}



