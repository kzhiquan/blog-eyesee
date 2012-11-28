
#ifndef _DB_CONN_MG_H_
#define _DB_CONN_MG_H_

#include "DbConn.h"

#include <string>
#include <vector>
using namespace std;

/* Db Connection manager */
class DbConnMg{
private:
    string  dbname;

    vector<DbConn *> dbConnPool;
    
public:

    /* constructor */
    DbConnMg();

    /* destructor */
    ~DbConnMg();

    /* set the DbName */
    void SetDbName(char *fullName );

    /* get an idle DbConn */
    DbConn * GetIdleDbConn();

};

#endif
