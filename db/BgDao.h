
#ifndef _BG_DAO_H_
#define _BG_DAO_H_

#include "../app/Post.h"
#include "../app/Category.h"
#include "../app/Comment.h"
#include "BgSql.h"


#include <vector>
using namespace std;


/* blog data access object , communication with database,
   and get the object */
class BgDao{
private:
    
    DbConn * pDbConn; // dbconnection 
    BgSql    sql;     // current sql 


public:
    /* init */
    int Init( DbConn *pDbConn = NULL );

    /* Get post object by guid */
    int GetPostByGUID( const char *guid, Post & post );

    /* get all posts */
    int GetAllPosts( vector< Post *> & posts );

    /* update post object by guid */
    int UpdatePostByGUID( const char *guid, Post &post );

    /* insert post into db */
    int InsertPost( Post & post );

    /* insert term relationship */
    int InsertTermRelationship(int objectid, int taxonomyid );

    /* get post sequence */
    int InsertPostSequence();

    /* insert comment into db */
    int InsertComment( Comment &comment );

    /* get comm sequence */
    int InsertCommSequence();

    /* get last insert post sequence */
    int GetLastInsertPostSequence( int *sequence );

    /* get last insert comm sequence */
    int GetLastInsertCommSequence( int *sequence );

    /* Get post taxonomy id */
    int GetPostTaxonomyID( const unsigned char *strcategory, int *taxonomyid );

    /* Get Posts by category */
    int GetPostsByCategory( const char *category, vector<Post *> & posts );

    /* Get Posts top Num */
    int GetPostsTopNum( int topnum, vector<Post *> & posts );

    /* Get all Categories */
    int GetAllCategories( vector< Category *> & categories );

    /* Get Comments by postid */
    int GetCommentsByPostID( vector< Comment *> & comments, int postid );

};



#endif
