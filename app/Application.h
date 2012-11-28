
#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include "../server/Session.h"


/* application */
class Application : public Session
{
private:

public:
    Application();
    virtual Task* Clone();
    virtual int ProcessHttp( HttpRequest &request, HttpResponse &response ); 

};









#endif
