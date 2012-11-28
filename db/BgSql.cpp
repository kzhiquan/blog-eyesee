
#include "BgSql.h"
#include "../app/Post.h"
#include <stdio.h>
#include <string.h>


char* BgSql::select_post_by_guid = 
    "select a.*, d.name \
    from bg_posts a, bg_term_relationships b, \
    bg_term_taxonomy c, bg_terms d \
    where a.post_id = b.object_id \
    and b.term_taxonomy_id = c.term_taxonomy_id \
    and c.term_id = d.term_id \
    and c.taxonomy='post' \
    and a.guid = '%s'; ";

char* BgSql::select_post_by_category = 
    "select a.*, d.name \
    from bg_posts a, bg_term_relationships b, \
    bg_term_taxonomy c, bg_terms d \
    where a.post_id = b.object_id \
    and b.term_taxonomy_id = c.term_taxonomy_id \
    and c.term_id = d.term_id \
    and c.taxonomy='post' \
    and d.name = '%s'; ";

char* BgSql::select_post_top_num = 
    "select a.*, d.name \
    from bg_posts a, bg_term_relationships b, \
    bg_term_taxonomy c, bg_terms d \
    where a.post_id = b.object_id \
    and b.term_taxonomy_id = c.term_taxonomy_id \
    and c.term_id = d.term_id \
    and c.taxonomy='post' \
    order by a.post_date desc \
    limit 0, %d; ";

char* BgSql::select_all_post = "select a.*, d.name \
    from bg_posts a, bg_term_relationships b, \
    bg_term_taxonomy c, bg_terms d \
    where a.post_id = b.object_id \
    and b.term_taxonomy_id = c.term_taxonomy_id \
    and c.term_id = d.term_id \
    and c.taxonomy='post' \
    order by a.post_date desc;";


char* BgSql::update_post_by_guid = 
    "update bg_posts\
     set comm_counts = %d,\
         post_title = '%s',\
         post_content = '%s'\
     where guid = '%s'; ";

char* BgSql::insert_into_post = 
    "insert into bg_posts(post_id,post_content,post_title, guid, comm_counts)\
     values( %d, '%s', '%s', '%s', %d );";

char* BgSql::insert_post_sequence = 
    "insert into bg_post_sequence(post_id) values(NULL);";

char* BgSql::insert_comm_sequence = 
    "insert into bg_comm_sequence(comm_id) values(NULL);";

char* BgSql::insert_into_comment = 
    "insert into bg_comments(comm_id, comm_post_id, comm_author, comm_author_email, comm_author_url, comm_content)\
     values( %d, %d, '%s', '%s', '%s', '%s' );";


char* BgSql::select_last_insert_post_sequence = 
     "select max(post_id) from bg_post_sequence;";

char* BgSql::select_last_insert_comm_sequence = 
     "select max(comm_id) from bg_comm_sequence;";

char* BgSql::select_all_category = "select a.term_taxonomy_id, a.parent, b.name \
                                    from bg_term_taxonomy a, bg_terms b \
                                    where a.term_id = b.term_id \
                                         and a.taxonomy = 'post';";

char* BgSql::select_comments_by_postid = "select * from bg_comments \
                                          where comm_post_id = %d;";


char* BgSql::select_termtaxonomy_by_taxonomy_and_termname = 
        "select a.term_taxonomy_id \
         from bg_term_taxonomy a, bg_terms b \
         where a.term_id = b.term_id \
         and a.taxonomy = '%s' \
         and b.name = '%s';";

char* BgSql::insert_into_term_relationship = 
    "insert into bg_term_relationships(object_id, term_taxonomy_id)\
     values( %d, %d );";
                                        
/* Set the select_post_by_guid  the guid */
string BgSql::SelectPostByGUID(const char *guid ){

    string select;
    char selectBuf[512];
    memset( selectBuf, 0, sizeof(selectBuf) );

    sprintf(selectBuf, BgSql::select_post_by_guid, guid);
    select.append(selectBuf);

    return select;
}


/* Set the select_post_by_guid  the guid */
string BgSql::SelectAllPosts(){

    string select;
    char selectBuf[512];
    memset( selectBuf, 0, sizeof(selectBuf) );

    sprintf(selectBuf, BgSql::select_all_post);
    select.append(selectBuf);

    return select;
}

/* Set the select_post_by_guid  the guid */
string BgSql::SelectPostByCategory(const char *category ){

    string select;
    char selectBuf[512];
    memset( selectBuf, 0, sizeof(selectBuf) );

    sprintf(selectBuf, BgSql::select_post_by_category, category );
    select.append(selectBuf);

    return select;
}

/* Set the select_post_top num the guid */
string BgSql::SelectPostTopNum(int topnum ){
    string select;
    char selectBuf[512];
    memset( selectBuf, 0, sizeof(selectBuf) );

    sprintf(selectBuf, BgSql::select_post_top_num, topnum );
    select.append(selectBuf);

    return select;
}

/* insert int post sql */
string BgSql::InsertIntoPost( Post &post ){
    string insert;
    char insertBuf[8192];
    memset( insertBuf, 0, sizeof(insertBuf) );

    sprintf(insertBuf, BgSql::insert_into_post, post.GetID(), post.GetContent(), post.GetTitle(), post.GetGUID(), post.GetCommCount());
    insert.append(insertBuf);

    return insert;
}

/* get the update_post_by_guid  the guid */
string BgSql::UpdatePostByGUID( Post &post ){

    string update;
    char updateBuf[8192];
    memset( updateBuf, 0, sizeof(updateBuf) );

    sprintf(updateBuf, BgSql::update_post_by_guid, post.GetCommCount(), post.GetTitle(), post.GetContent(), post.GetGUID());
    update.append(updateBuf);

    return update;
}

/* get post sequence */
string BgSql::InsertPostSequence(){
    string insert;
    char insertBuf[512];
    memset( insertBuf, 0, sizeof(insertBuf) );

    sprintf(insertBuf, BgSql::insert_post_sequence);
    insert.append(insertBuf);

    return insert;
}

/* insert int post sql */
string BgSql::InsertIntoComment( Comment &comment ){
    string insert;
    char insertBuf[8192];
    memset( insertBuf, 0, sizeof(insertBuf) );

    sprintf(insertBuf, BgSql::insert_into_comment, comment.GetID(), comment.GetPostID(), comment.GetCommAuthor(), \
                comment.GetCommAuthorEmail(), comment.GetCommAuthorUrl(), comment.GetCommContent());
    insert.append(insertBuf);

    return insert;
}

/* get post sequence */
string BgSql::InsertCommSequence(){
    string insert;
    char insertBuf[512];
    memset( insertBuf, 0, sizeof(insertBuf) );

    sprintf(insertBuf, BgSql::insert_comm_sequence);
    insert.append(insertBuf);

    return insert;
}

/* get post sequence */
string BgSql::SelectLastInsertPostSequence(){
    string select;
    char selectBuf[512];
    memset( selectBuf, 0, sizeof(selectBuf) );

    sprintf(selectBuf, BgSql::select_last_insert_post_sequence);
    select.append(selectBuf);

    return select;
}


/* get comm sequence */
string BgSql::SelectLastInsertCommSequence(){
    string select;
    char selectBuf[512];
    memset( selectBuf, 0, sizeof(selectBuf) );

    sprintf(selectBuf, BgSql::select_last_insert_comm_sequence);
    select.append(selectBuf);

    return select;
}

/* get all category */
string BgSql::SelectAllCategories(){

    string select;

    select.append(BgSql::select_all_category);

    return select;
}

/* get all category */
string BgSql::SelectCommentsByPostID(int postid ){
    string select;
    char selectBuf[512];
    memset( selectBuf, 0, sizeof(selectBuf) );

    sprintf(selectBuf, BgSql::select_comments_by_postid, postid);
    select.append(selectBuf);

    return select;
}

/* get taxonomyid */
string BgSql::SelectTaxonomyIDByNameAndTaxonomy( const unsigned char* termname, char *taxonomy ){
    string select;
    char selectBuf[512];
    memset( selectBuf, 0, sizeof(selectBuf) );

    sprintf(selectBuf, BgSql::select_termtaxonomy_by_taxonomy_and_termname, taxonomy, termname );
    select.append(selectBuf);

    return select;

}

/* insert into term taxonomy relationship */
string BgSql::InsertIntoTermRelationship(int objectid, int taxonomyid){
    string insert;
    char insertBuf[512];
    memset( insertBuf, 0, sizeof(insertBuf) );

    sprintf(insertBuf, BgSql::insert_into_term_relationship, objectid, taxonomyid);
    insert.append(insertBuf);

    return insert;
}
