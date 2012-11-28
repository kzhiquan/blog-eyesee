
#ifndef _BG_SQL_H_
#define _BG_SQL_H_


#include <string>
using namespace std;

#include "../app/Post.h"
#include "../app/Comment.h"

/* BgSql object */
class BgSql{
private:
    static char *select_post_by_guid;
    static char *update_post_by_guid;
    static char *select_post_by_category;
    static char *select_post_top_num;
    static char *select_all_post;
    static char *insert_into_post;

    static char *insert_post_sequence;

    static char *insert_comm_sequence;

    static char *insert_into_comment;

    static char* select_last_insert_post_sequence;

    static char* select_last_insert_comm_sequence;

    static char *select_all_category;

    static char *select_comments_by_postid;

    static char* select_termtaxonomy_by_taxonomy_and_termname;

    static char* insert_into_term_relationship;
public:

    /* get post sql */
    static string SelectPostByGUID( const char *guid );

    /* select all posts  */
    static string SelectAllPosts();

    /* get the update_post_by_guid  the guid */
    static string UpdatePostByGUID( Post &post );

    /* get post sequence */
    static string InsertPostSequence();

    /* get post sequence */
    static string InsertCommSequence();

    /* get post sequence */
    static string SelectLastInsertPostSequence();

    /* get comm sequence */
    static string SelectLastInsertCommSequence();

    /* insert int post sql */
    static string InsertIntoPost( Post &post );

    /* insert int comment sql */
    static string InsertIntoComment( Comment &comment );

    /* insert into term taxonomy relationship */
    static string InsertIntoTermRelationship(int objectid, int taxonomyid);

    /* Set the select_post_by_guid  the guid */
    static string SelectPostByCategory(const char *categroy );

    /* Set the select_post_top num the guid */
    static string SelectPostTopNum(int topnum );

    /* get all category */
    static string SelectAllCategories();

    /* get comments by post id  */
    static string SelectCommentsByPostID(int postid );

    /* get taxonomyid */
    static string SelectTaxonomyIDByNameAndTaxonomy( const unsigned char* termname, char *taxonomy );

};


#endif
