
#include "Task.h"
#include "ESLog.h"





/* Set the parameter */
void Task::SetParameter( void *arg ){

}

/* Set the */
void Task::Work(){
    ESLog::ISLog("Task Work CALL\n", __FILE__, __LINE__);
}

/* Set the */
Task* Task::Clone(){

    ESLog::ISLog("Task close here\n", __FILE__, __LINE__);

    Task *task = new Task(*this);


    return task;
}
