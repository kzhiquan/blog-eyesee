
#ifndef _WORKMANAGER_H_
#define _WORKMANAGER_H_


#include "Task.h"
#include "WorkThread.h"

#include <vector>
using namespace std;

#include <pthread.h>

/* control the thread, the manager */
class WorkManager{
private:
    pthread_t               tid;
    Task                    *taskPrimitive;
    vector<WorkThread *>    threadPool;

    bool                    hasWorkToDo;
    pthread_mutex_t         qlock;
    pthread_cond_t          qready;
public:
    WorkManager();

    /* set the has work to do flag */
    void SetHasWorkToDo(bool flag);

    /* get the work to do flag */
    bool GetHasWorkToDo();

    /* is working or not */
    //bool IsWorking();

    /* WorkManager thread triger to work */
    int  WorkConditionSignal();

    /* set threadid */
    void SetThreadID( pthread_t tid );

    /* set the primitive fo the task */
    int InitTaskPrimitive( Task *task );

    /* get an idle thread for the task */
    WorkThread* GetIdleThread();

    /* start up the taskThread */
    int StartupTaskThread(WorkThread *workThread);

    /* get an task in the waiting list */
    int GetNewTask();

    /* close the task */
    Task *CloneTask();

    /* static method, the thread run method */
    static void *WorkManagerThread( void *arg );                                                                                                                                                                                               


};

#endif
