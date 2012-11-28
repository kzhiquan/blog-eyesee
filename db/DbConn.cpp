
#include "DbConn.h"
#include "../server/ESLog.h"
#include "../sqlite/sqlite3.h"

#include <stdio.h>


/*Contructor */
DbConn::DbConn(){
    this->db = 0;
    this->stmt = 0;
    this->status = 0;
}

/* destructor */
DbConn::~DbConn(){
    if(this->db) {
        sqlite3_close(this->db);
    }

    sqlite3_finalize(this->stmt);

}

/* set status */
void  DbConn::SetStatus(int status ){
    this->status = status;

}

/* Get status */
int DbConn::GetStatus(){
    return this->status;
}


/* open db connnection */
int DbConn::OpenDBConn(const char *dbname )
{
    int err = 0;

    /* open */
    //err = sqlite3_open16( dbname, &(this->db) );
    err = sqlite3_open(dbname, &(this->db));
    if( err ) {
        ESLog::ISLog("open db error!\n",__FILE__, __LINE__);
        sqlite3_close(this->db);
    }

    return err;
}


/* close db connection */
int DbConn::CloseDBConn(){
    int err = 0;

    if(this->db) {
        sqlite3_close(this->db);
    }

    sqlite3_finalize(this->stmt);


    return err;
}

/* comiple sql */
int DbConn::CompileSql(const char *sql, int sqllen ){
    int err = 0;

    ESLog::ISLog("compile sql:%s\n", __FILE__, __LINE__, sql);
    /* compile sql */
    err = sqlite3_prepare(this->db, sql, sqllen, &(this->stmt), &(this->zTail));
    if(err) {
        ESLog::ISLog("%s\n", __FILE__, __LINE__, sqlite3_errmsg(this->db));
        ESLog::ISLog("compile sql failed!\n", __FILE__, __LINE__);
    }
    
    return err;
}


/* sql step by step */
int DbConn::SqlStep(){
    int result = 0;

    result = sqlite3_step(this->stmt);

    return result;
}

/* release sql compile resource stmt */
int DbConn::ReleaseStmt(){

    int err = 0;

    err = sqlite3_finalize(this->stmt);

    return err;
}

/* get column value integer */
int DbConn::GetColumnInt(int columnIndex ){
    return sqlite3_column_int(this->stmt, columnIndex );
}

/* get column value text */
const unsigned char * DbConn::GetColumnText(int columnIndex ){
    return sqlite3_column_text(this->stmt, columnIndex );
}

/* get column value of sqlite3_int64 */
sqlite3_int64 DbConn::GetColumnInt64(int columnIndex ){
    return sqlite3_column_int64(this->stmt, columnIndex );

}
