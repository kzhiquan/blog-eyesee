

#include "DbConnMg.h"
#include "DbConn.h"
#include "../server/ESLog.h"
#include <stdio.h>


/* constructor */
DbConnMg::DbConnMg(){

}

/* destructor */
DbConnMg::~DbConnMg(){

}

/* set dbName */
void DbConnMg::SetDbName(char *fullname ){

    this->dbname.append(fullname);

}

/* Get an idle DbConn */
DbConn * DbConnMg::GetIdleDbConn(){
    int err = 0;
    DbConn *pDbConn = 0;

    vector<DbConn*>::iterator ite;
    for( ite = this->dbConnPool.begin(); ite != this->dbConnPool.end(); ++ite ) {
        DbConn * pDbConn = (*ite);
        if( pDbConn->GetStatus() == 1 ) { /* it is an idle DbConnection */
            pDbConn->SetStatus(2);
            ESLog::ISLog("Get idle DbConnection!\n", __FILE__, __LINE__);
            return pDbConn;
        }
    }

    /* new DbConnection obj */
    pDbConn = new DbConn();

    ESLog::ISLog("dbname:%s\n", __FILE__, __LINE__, this->dbname.c_str());
    /* open the connection */
    err = pDbConn->OpenDBConn(this->dbname.c_str());
    if( err ) {
        delete pDbConn;
        pDbConn = 0;
        return pDbConn;
    }
    
    /* set the status */
    pDbConn->SetStatus(2);

    ESLog::ISLog(" An new DbConnection open!\n", __FILE__, __LINE__);

    /* insert into pool */
    this->dbConnPool.push_back(pDbConn);

    return pDbConn;
}
