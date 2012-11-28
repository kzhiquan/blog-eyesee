
#ifndef _TASK_H_
#define _TASK_H_


/* just a interface */
class Task
{
public:

    /* work, tackle the task */
    virtual void Work();

    /* close self */
    virtual Task* Clone();

    /* set the task parameter of the task */
    virtual void SetParameter( void * parameter );
    
};



#endif
