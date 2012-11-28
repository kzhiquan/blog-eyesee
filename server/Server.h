
#ifndef _SERVER_H_
#define _SERVER_H_

#include "Site.h"
#include "IOLoop.h"
#include "Configure.h"
#include "Task.h"
#include "StreamManager.h"
#include "WorkManager.h"
#include "ESLog.h"
#include "../db/DbConnMg.h"

class Server{

private:
    static Server*      server;
    static const  char* configureFile;

    Site                site;
    IOLoop              ioLoop;
    Configure           configure;
    WorkManager         workManager;
    
    StreamManager       streamManager;

    DbConnMg            dbConnMg;

    /* log object */
    ESLog               log;
      
    Server();

public:
    /* destructor function */
    ~Server();

    /* single instance */
    static Server* Instance();

    /* get Site of the server */
    Site & GetSite();

    /* get ioLoop of the server */
    IOLoop & GetIOLoop();

    /* Get StreamManager object */
    StreamManager & GetStreamManager();

    /* Get WorkManager object */
    WorkManager & GetWorkManager();

    /* Get DbConnMg object */
    DbConnMg   & GetDbConnMg();

    /* Get Log Object */
    ESLog   & GetESLog();

    /* Get Configure object */
    Configure   & GetConfigure();

    /* load server parameter from the configure file */
    int LoadConfigure();

    /* load application task */
    int LoadApplicationTask( Task *task );

    /* init the server after load the configure  */
    int Init();

    /* startup the WorkManager Thread */
    int StartupWorkManager();

    /* startup the IOLoop */
    int StartupIOLoop();

};


#endif
