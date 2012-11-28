
#ifndef _DB_CONN_H_
#define _DB_CONN_H_

#include "../sqlite/sqlite3.h"

/* DBConnection object */
class DbConn{

private:
    sqlite3         *db;    /* datebase */
    sqlite3_stmt    *stmt;  /* sql prepare */
    const char      *zTail; /* the sql prepared postion */
    int             status; /* 0: unopen, 1: idle, 2: busy; set the DbConn Status */

public:

    /* constructor */
    DbConn();

    /* destructor */
    ~DbConn();

    /* set status */
    void  SetStatus(int status );

    /* Get status */
    int GetStatus();

    /* open the connection */
    int OpenDBConn(const char *dbname);

    /* close the connection */
    int CloseDBConn();

    /* compile sql */
    int CompileSql(const char *sql, int sqllen );

    /* sql step by step */
    int SqlStep();

    /* get column value integer */
    int GetColumnInt(int columnIndex );

    /* get column value of sqlite3_int64 */
    sqlite3_int64 GetColumnInt64(int columnIndex );

    /* get column value text */
    const unsigned char *GetColumnText(int columnIndex );

    /* release sql compile resource stmt */
    int ReleaseStmt();
};




#endif
