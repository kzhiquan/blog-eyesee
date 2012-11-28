


#include "BgDao.h"

#include "../server/Server.h"

#include <stdio.h>
#include <string.h>


/* init */
int BgDao::Init( DbConn *pDbConn ){

    int err = 0;

    /* Get an dbConnection */
    if( pDbConn == NULL ) {
        this->pDbConn = Server::Instance()->GetDbConnMg().GetIdleDbConn();
        if( this->pDbConn == NULL ) {
            ESLog::ISLog(" it can not get an idle dbConnection!\n", __FILE__, __LINE__);
            return -1;
        }
    }
    else{
        this->pDbConn = pDbConn;
    }

    return err;
}


/* Get post object by guid */
int BgDao::GetPostByGUID( const char *guid, Post & post ){
    int err = 0;
   
    /* Get the sql select */
    string sql = BgSql::SelectPostByGUID(guid);
    ESLog::ISLog("sql:%s\n", __FILE__, __LINE__, sql.c_str());

    /* compile the sql */
    err = this->pDbConn->CompileSql(sql.c_str(), -1);
    if( err ) {
        return -1;
    }

    /* excute one by one */
    int stepflag = 0;
    stepflag = this->pDbConn->SqlStep();
    if( stepflag != SQLITE_ROW ) {

        /* release the stmt */
        this->pDbConn->ReleaseStmt();

        /* set the status of the DbConn */
        this->pDbConn->SetStatus(1);

        return  -1;
    }

    while( stepflag == SQLITE_ROW ) {
        /* set post object  */
        post.GetPostFromDB(this->pDbConn);

        stepflag = this->pDbConn->SqlStep();
    }

    /* release the stmt */
    this->pDbConn->ReleaseStmt();

    /* set the status of the DbConn */
    this->pDbConn->SetStatus(1);


    return err;
}


/* update post object by guid */
int BgDao::UpdatePostByGUID( const char *guid, Post &post ){
    int err = 0;
   
    ESLog::ISLog("UPDATE POST START!\n", __FILE__, __LINE__);

    /* Get the update sql select */
    string sql= BgSql::UpdatePostByGUID(post);
    ESLog::ISLog("sql:%s\n", __FILE__, __LINE__, sql.c_str());

    /* compile the sql */
    err = this->pDbConn->CompileSql(sql.c_str(), -1);
    if( err ) {
        return -1;
    }

    /* excute one by one */
    int stepflag = 0;
    stepflag = this->pDbConn->SqlStep();
    if( stepflag != SQLITE_DONE ) {

        /* release the stmt */
        this->pDbConn->ReleaseStmt();

        /* set the status of the DbConn */
        this->pDbConn->SetStatus(1);

        return  -1;
    }

    /* release the stmt */
    this->pDbConn->ReleaseStmt();

    /* set the status of the DbConn */
    this->pDbConn->SetStatus(1);

    ESLog::ISLog("UPDATE POST END!\n", __FILE__, __LINE__);


    return err;

}

/* insert post into db */
int BgDao::InsertPost( Post & post ){
    int err = 0;
   
    ESLog::ISLog("INSERT POST START!\n", __FILE__, __LINE__);

    /* Get the insert sql select */
    string sql= BgSql::InsertIntoPost(post);
    ESLog::ISLog("sql:%s\n", __FILE__, __LINE__, sql.c_str());

    /* compile the sql */
    err = this->pDbConn->CompileSql(sql.c_str(), -1);
    if( err ) {
        return -1;
    }

    /* excute one by one */
    int stepflag = 0;
    stepflag = this->pDbConn->SqlStep();
    if( stepflag != SQLITE_DONE ) {

        /* release the stmt */
        this->pDbConn->ReleaseStmt();

        /* set the status of the DbConn */
        this->pDbConn->SetStatus(1);

        return  -1;
    }

    /* release the stmt */
    this->pDbConn->ReleaseStmt();

    /* set the status of the DbConn */
    this->pDbConn->SetStatus(1);

    ESLog::ISLog("INSERT POST END!\n", __FILE__, __LINE__);



    return err;
}

/* get post sequence */
int BgDao::InsertPostSequence(){
    int err = 0;
   
    /* Get the insert sql select */
    string sql= BgSql::InsertPostSequence();
    ESLog::ISLog("sql:%s\n", __FILE__, __LINE__, sql.c_str());

    /* compile the sql */
    err = this->pDbConn->CompileSql(sql.c_str(), -1);
    if( err ) {
        return -1;
    }

    /* excute one by one */
    int stepflag = 0;
    stepflag = this->pDbConn->SqlStep();
    ESLog::ISLog("sqlstep result: %d\n", __FILE__, __LINE__, stepflag);
    if( stepflag != SQLITE_DONE ) {

        /* release the stmt */
        this->pDbConn->ReleaseStmt();

        /* set the status of the DbConn */
        this->pDbConn->SetStatus(1);

        return  -1;
    }


    /* release the stmt */
    this->pDbConn->ReleaseStmt();

    /* set the status of the DbConn */
    this->pDbConn->SetStatus(1);


    return err;
}

    /* get last insert post sequence */
int BgDao::GetLastInsertPostSequence( int *sequence ){
    int err = 0;
    *sequence = 0;
   
    /* Get the insert sql select */
    string sql= BgSql::SelectLastInsertPostSequence();
    ESLog::ISLog("sql:%s\n", __FILE__, __LINE__, sql.c_str());

    /* compile the sql */
    err = this->pDbConn->CompileSql(sql.c_str(), -1);
    if( err ) {
        return -1;
    }

    /* excute one by one */
    int stepflag = 0;
    stepflag = this->pDbConn->SqlStep();
    ESLog::ISLog("sqlstep result: %d\n", __FILE__, __LINE__, stepflag);
    if( stepflag != SQLITE_ROW ) {

        /* release the stmt */
        this->pDbConn->ReleaseStmt();

        /* set the status of the DbConn */
        this->pDbConn->SetStatus(1);

        return  -1;
    }

    /* set post object  */
    *sequence = this->pDbConn->GetColumnInt(0);
    //*sequence = this->pDbConn->GetColumnInt64(0);

    ESLog::ISLog("sequecen:%d\n", __FILE__, __LINE__, *sequence );


    /* release the stmt */
    this->pDbConn->ReleaseStmt();

    /* set the status of the DbConn */
    this->pDbConn->SetStatus(1);


    return err;
}


/* get comm sequence */
int BgDao::InsertCommSequence(){
    int err = 0;
   
    /* Get the insert sql select */
    string sql= BgSql::InsertCommSequence();
    ESLog::ISLog("sql:%s\n", __FILE__, __LINE__, sql.c_str());

    /* compile the sql */
    err = this->pDbConn->CompileSql(sql.c_str(), -1);
    if( err ) {
        return -1;
    }

    /* excute one by one */
    int stepflag = 0;
    stepflag = this->pDbConn->SqlStep();
    ESLog::ISLog("sqlstep result: %d\n", __FILE__, __LINE__, stepflag);
    if( stepflag != SQLITE_DONE ) {

        /* release the stmt */
        this->pDbConn->ReleaseStmt();

        /* set the status of the DbConn */
        this->pDbConn->SetStatus(1);

        return  -1;
    }

    /* release the stmt */
    this->pDbConn->ReleaseStmt();

    /* set the status of the DbConn */
    this->pDbConn->SetStatus(1);


    return err;
}

    /* get last insert comm sequence */
int BgDao::GetLastInsertCommSequence( int *sequence ){
    int err = 0;
    *sequence = 0;
   
    /* Get the insert sql select */
    string sql= BgSql::SelectLastInsertCommSequence();
    ESLog::ISLog("sql:%s\n", __FILE__, __LINE__, sql.c_str());

    /* compile the sql */
    err = this->pDbConn->CompileSql(sql.c_str(), -1);
    if( err ) {
        return -1;
    }

    /* excute one by one */
    int stepflag = 0;
    stepflag = this->pDbConn->SqlStep();
    ESLog::ISLog("sqlstep result: %d\n", __FILE__, __LINE__, stepflag);
    if( stepflag != SQLITE_ROW ) {

        /* release the stmt */
        this->pDbConn->ReleaseStmt();

        /* set the status of the DbConn */
        this->pDbConn->SetStatus(1);

        return  -1;
    }

    /* set post object  */
    *sequence = this->pDbConn->GetColumnInt(0);
    //*sequence = this->pDbConn->GetColumnInt64(0);

    ESLog::ISLog("sequecen:%d\n", __FILE__, __LINE__, *sequence );


    /* release the stmt */
    this->pDbConn->ReleaseStmt();

    /* set the status of the DbConn */
    this->pDbConn->SetStatus(1);


    return err;
}


/* insert comment into db */
int BgDao::InsertComment( Comment &comment ){
    int err = 0;
   
    ESLog::ISLog("INSERT COMMENT START!\n", __FILE__, __LINE__);

    /* Get the insert sql select */
    string sql= BgSql::InsertIntoComment(comment);
    ESLog::ISLog("sql:%s\n", __FILE__, __LINE__, sql.c_str());

    /* compile the sql */
    err = this->pDbConn->CompileSql(sql.c_str(), -1);
    if( err ) {
        return -1;
    }

    /* excute one by one */
    int stepflag = 0;
    stepflag = this->pDbConn->SqlStep();
    if( stepflag != SQLITE_DONE ) {

        /* release the stmt */
        this->pDbConn->ReleaseStmt();

        /* set the status of the DbConn */
        this->pDbConn->SetStatus(1);

        return  -1;
    }

    /* release the stmt */
    this->pDbConn->ReleaseStmt();

    /* set the status of the DbConn */
    this->pDbConn->SetStatus(1);

    ESLog::ISLog("INSERT COMMENT END!\n", __FILE__, __LINE__);


    return err;
}


/* Get post taxonomy id */
int BgDao::GetPostTaxonomyID( const unsigned char *strcategory, int *taxonomyid ){
    int err = 0;
    *taxonomyid = 0;
   
    /* Get the insert sql select */
    string sql= BgSql::SelectTaxonomyIDByNameAndTaxonomy(strcategory, "post");
    ESLog::ISLog("sql:%s\n", __FILE__, __LINE__, sql.c_str());

    /* compile the sql */
    err = this->pDbConn->CompileSql(sql.c_str(), -1);
    if( err ) {
        return -1;
    }

    /* excute one by one */
    int stepflag = 0;
    stepflag = this->pDbConn->SqlStep();
    ESLog::ISLog("sqlstep result: %d\n", __FILE__, __LINE__, stepflag);
    if( stepflag != SQLITE_ROW ) {

        /* release the stmt */
        this->pDbConn->ReleaseStmt();

        /* set the status of the DbConn */
        this->pDbConn->SetStatus(1);

        return  -1;
    }

    while( stepflag == SQLITE_ROW ) {
        /* set post object  */
        *taxonomyid = this->pDbConn->GetColumnInt(0);

        stepflag = this->pDbConn->SqlStep();
    }

    /* release the stmt */
    this->pDbConn->ReleaseStmt();

    /* set the status of the DbConn */
    this->pDbConn->SetStatus(1);


    return err;
}


/* Get Posts by category */
int BgDao::GetPostsByCategory( const char *category, vector<Post *> &posts ){

    int err = 0;

        
    /* Get the sql select */
    string sql = BgSql::SelectPostByCategory(category);
    ESLog::ISLog("sql:%s\n", __FILE__, __LINE__, sql.c_str());

    /* compile the sql */
    err = this->pDbConn->CompileSql(sql.c_str(), -1);
    if( err ) {
        return -1;
    }

    /* excute one by one */
    int stepflag = 0;
    stepflag = this->pDbConn->SqlStep();
    while( stepflag == SQLITE_ROW ) {
        
        /* new an object */
        Post *pPost = new Post();

        /* set post object  */
        pPost->GetPostFromDB(this->pDbConn);

        /* add in */
        posts.push_back(pPost);

        stepflag = this->pDbConn->SqlStep();
    }

    /* release the stmt */
    this->pDbConn->ReleaseStmt();

    /* set the status of the DbConn */
    this->pDbConn->SetStatus(1);


    return err;
}

/* Get Posts top Num */
int BgDao::GetPostsTopNum( int topnum, vector<Post *> & posts ){
    int err = 0;

    ESLog::ISLog("GET POSTS TOP NUM START\n", __FILE__, __LINE__);
        
    /* Get the sql select */
    string sql = BgSql::SelectPostTopNum(topnum);
    ESLog::ISLog("sql:%s\n", __FILE__, __LINE__, sql.c_str());

    /* compile the sql */
    err = this->pDbConn->CompileSql(sql.c_str(), -1);
    if( err ) {
        return -1;
    }
    
    ESLog::ISLog("compile sucess!\n", __FILE__, __LINE__);

    /* excute one by one */
    int stepflag = 0;
    stepflag = this->pDbConn->SqlStep();
    while( stepflag == SQLITE_ROW ) {
        
        /* new an object */
        Post *pPost = new Post();

        /* set post object  */
        pPost->GetPostFromDB(this->pDbConn);

        /* add in */
        posts.push_back(pPost);

        stepflag = this->pDbConn->SqlStep();

        ESLog::ISLog("sqlstep:%d\n", __FILE__, __LINE__, stepflag);
    }

    /* release the stmt */
    this->pDbConn->ReleaseStmt();

    /* set the status of the DbConn */
    this->pDbConn->SetStatus(1);


    ESLog::ISLog("GET POSTS TOP NUM END\n", __FILE__, __LINE__);


    return err;
}


/* get all posts */
int BgDao::GetAllPosts( vector< Post *> & posts ){
    int err = 0;

    ESLog::ISLog("GET ALL POSTS START\n", __FILE__, __LINE__);
        
    /* Get the sql select */
    string sql = BgSql::SelectAllPosts();
    ESLog::ISLog("sql:%s\n", __FILE__, __LINE__, sql.c_str());

    /* compile the sql */
    err = this->pDbConn->CompileSql(sql.c_str(), -1);
    if( err ) {
        return -1;
    }
    
    ESLog::ISLog("compile sucess!\n", __FILE__, __LINE__);

    /* excute one by one */
    int stepflag = 0;
    stepflag = this->pDbConn->SqlStep();
    while( stepflag == SQLITE_ROW ) {
        
        /* new an object */
        Post *pPost = new Post();

        /* set post object  */
        pPost->GetPostFromDB(this->pDbConn);

        /* add in */
        posts.push_back(pPost);

        stepflag = this->pDbConn->SqlStep();

        ESLog::ISLog("sqlstep:%d\n", __FILE__, __LINE__, stepflag);
    }

    /* release the stmt */
    this->pDbConn->ReleaseStmt();

    /* set the status of the DbConn */
    this->pDbConn->SetStatus(1);


    ESLog::ISLog("GET ALL POSTS END\n", __FILE__, __LINE__);


    return err;
}


/* Get all Categories */
int BgDao::GetAllCategories( vector< Category *> & categories ){
    int err = 0;

    /* Get the sql select */
    string sql = BgSql::SelectAllCategories();
    ESLog::ISLog("sql:%s\n", __FILE__, __LINE__, sql.c_str());

    /* compile the sql */
    err = this->pDbConn->CompileSql(sql.c_str(), -1);
    if( err ) {
        return -1;
    }

    /* excute one by one */
    int stepflag = 0;
    stepflag = this->pDbConn->SqlStep();
    while( stepflag == SQLITE_ROW ) {

        /* new a category */
        Category *pCategory = new Category();
        
        /* Get Category from db */
        pCategory->GetCategoryFromDB(this->pDbConn);
        
        /* add in the set */
        categories.push_back(pCategory);

        stepflag = this->pDbConn->SqlStep();
    }

    /* release the stmt */
    this->pDbConn->ReleaseStmt();

    /* set the status of the DbConn */
    this->pDbConn->SetStatus(1);


    return err;

}


/* Get Comments by postid */
int BgDao::GetCommentsByPostID( vector< Comment *> & comments, int postid ){

    int err = 0;

    /* Get the sql select */
    string sql = BgSql::SelectCommentsByPostID(postid);
    ESLog::ISLog("sql:%s\n", __FILE__, __LINE__, sql.c_str());

    /* compile the sql */
    err = this->pDbConn->CompileSql(sql.c_str(), -1);
    if( err ) {
        return -1;
    }

    /* excute one by one */
    int stepflag = 0;
    stepflag = this->pDbConn->SqlStep();
    while( stepflag == SQLITE_ROW ) {

        /* new a comment */
        Comment *pComment = new Comment();
        
        /* Get Comment from db */
        pComment->GetCommentFromDB(this->pDbConn);
        
        /* add in the set */
        comments.push_back(pComment);

        stepflag = this->pDbConn->SqlStep();
    }

    /* release the stmt */
    this->pDbConn->ReleaseStmt();

    /* set the status of the DbConn */
    this->pDbConn->SetStatus(1);


    return err;
}


/* insert term relationship */
int BgDao::InsertTermRelationship(int objectid, int taxonomyid ){
    int err = 0;
    /* Get the insert sql select */
    string sql= BgSql::InsertIntoTermRelationship(objectid,taxonomyid);
    ESLog::ISLog("sql:%s\n", __FILE__, __LINE__, sql.c_str());

    /* compile the sql */
    err = this->pDbConn->CompileSql(sql.c_str(), -1);
    if( err ) {
        return -1;
    }

    /* excute one by one */
    int stepflag = 0;
    stepflag = this->pDbConn->SqlStep();
    ESLog::ISLog("sqlstep result: %d\n", __FILE__, __LINE__, stepflag);
    if( stepflag != SQLITE_DONE ) {

        /* release the stmt */
        this->pDbConn->ReleaseStmt();

        /* set the status of the DbConn */
        this->pDbConn->SetStatus(1);

        return  -1;
    }

    /* release the stmt */
    this->pDbConn->ReleaseStmt();

    /* set the status of the DbConn */
    this->pDbConn->SetStatus(1);


    return err;
}
