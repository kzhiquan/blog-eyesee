
#ifndef _COMMENT_H_
#define _COMMENT_H_

#include <string>
using namespace std;

#include "../db/DbConn.h"
#include "../json/cJSON.h"

class Comment{
private:
    int                             id;
    int                             post_id;
    basic_string<unsigned char>     post_guid;
    basic_string<unsigned char>     comm_date;
    basic_string<unsigned char>     comm_author;
    basic_string<unsigned char>     comm_author_email;
    basic_string<unsigned char>     comm_author_url;
    basic_string<unsigned char>     comm_content;

public:

    /* set id */
    void SetID(int id);

    /* get id */
    int GetID();

    /* set Post_id */
    void SetPostID( int postID );

    /* get post id */
    int GetPostID();

    /* set Post_guid */
    void SetPostGUID( const unsigned char * postGUID );

    /* get post guid */
    const unsigned char *GetPostGUID();

    /* set the comm_date */
    void SetCommDate(const unsigned char * strCommDate );

    /* get the comm_date */
    const unsigned char * GetCommDate();

    /* set the comm_author */
    void SetCommAuthor( const unsigned char * strCommAuthor );

    /* get the comm_author */
    const unsigned char * GetCommAuthor();

    /* set the comm_author_email */
    void SetCommAuthorEmail( const unsigned char *strCommAuthorEmail );

    /* get the comm_author email */
    const unsigned char * GetCommAuthorEmail();

    /* Set the Comm_author_url */
    void SetCommAuthorUrl( const unsigned char *strCommAuthorUrl );

    /* get the comm_author url */
    const unsigned char * GetCommAuthorUrl();

    /* Set the Com_content */
    void SetCommContent( const unsigned char *strCommContent );

    /* get the comm_content */
    const unsigned char * GetCommContent();

    /* set object form dbConnectin */
    void GetCommentFromDB(DbConn* pDbConn );


    /* Out L-Object format comment */
    string OutLObjectFormat();

    /* Out XmlObject format comment */
    string OutXmlObjectFormat();

    /* out cJson formt comment */
    cJSON *OutJsonObjectFormat();

    /* in json-object format from str */
    int InJsonObjectFormat( const char *strjson );
};



#endif
