

#include "Application.h"
#include "PathDispatcher.h"
#include "Post.h"
#include "../server/Server.h"
#include "../db/DbConn.h"
#include "../db/BgSql.h"
#include <stdio.h>
#include <string.h>

/* constructor*/
Application::Application(){

}

/* clone */
Task* Application::Clone(){

    ESLog::ISLog("APPLICATION CLONE HERE\n", __FILE__, __LINE__);

    Task *task = new Application(*this);

    return task;
}


/* override the session */
int Application::ProcessHttp( HttpRequest &request, HttpResponse &response ){

    ESLog::ISLog("APPLICATION PROCESS HTTP START\n", __FILE__, __LINE__);

    int err = 0;

    /* path dispatcher */
    PathDispatcher pathDispatcher;

    /* resposne string */
    string resbody;

    /* request body */
    RollBuff *reqbody = this->GetHttpPtr()->GetWebStreamPtr()->GetRollBufPtr();

    /* dispather */
    err = pathDispatcher.Dispatch( request, response, reqbody, resbody );
    if( err < 0 ) {
        ESLog::ISLog("path dispatcher error!\n", __FILE__, __LINE__);
        ESLog::ISLog("APPLICATION PROCESS HTTP ERR END\n", __FILE__, __LINE__);
        return err;
    }

    /* set send buf lenght */
    char lenbuf[10];
    memset(lenbuf, 0, sizeof(lenbuf));
    sprintf(lenbuf, "%d", resbody.length());

    ESLog::ISLog("Application ressponse http body size:%d\n", __FILE__, __LINE__, resbody.length());

    response.SetResHeaderValueByName("Content-Length", lenbuf);

    this->StreamOut(resbody.c_str());


    ESLog::ISLog("APPLICATION PROCESS HTTP END\n", __FILE__, __LINE__);

    return err;
}
