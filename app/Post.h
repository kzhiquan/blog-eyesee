
#ifndef _POST_H_
#define _POST_H_

#include <string>
using namespace std;

#include "../db/DbConn.h"

#include "../json/cJSON.h"

/* post object */
class Post{
private:
    int                             id;
    int                             comm_count;
    basic_string<unsigned char>     visible;
    basic_string<unsigned char>     guid;
    basic_string<unsigned char>     title;
    basic_string<unsigned char>     category;
    basic_string<unsigned char>     excerpt;
    basic_string<unsigned char>     content;
    basic_string<unsigned char>     post_date;

public:
    /* set post id */
    void SetID( int id );

    /* GetID */
    int GetID();

    /* set CommCount */
    void SetCommCount( int comm_count );

    /* get commCount */
    int GetCommCount();

    /* set the guid */
    void SetGUID( const unsigned char *strguid );

    /* get the guid */
    const unsigned char *GetGUID();

    /* set the title */
    void SetTitle( const  unsigned char *strtitle );

    /* Get the title */
    const unsigned char *GetTitle();

    /* set the excerpt */
    void SetExcerpt( const  unsigned char *strExcerpt );

    /* Get the excerpt */
    const unsigned char * GetExcerpt();

    /* set the content */
    void SetContent( const  unsigned char *strContent );

    /* Get the content */
    const unsigned char *GetContent();

    /* set the post_date */
    void SetPostDate( const unsigned char *strPostDate );

    /* Get the post_date */
    const unsigned char * GetPostDate();

    /* set the post_date */
    void SetCategory( const unsigned char *strCategory );

    /* Get the post category */
    const unsigned char *GetCategory();

    /* set the post visible */
    void SetVisible( const  unsigned char *strVisible );

    /* get the post visible */
    const unsigned char * GetVisible();

    /* set the post form dbconnection */
    void GetPostFromDB( DbConn *pDbConn );

    /* out L-Object format */
    string OutLObjectFormat();

    /* out xml-object format */
    string OutXmlObjectFormat();

    /* out json-object format */
    cJSON * OutJsonObjectFormat();

    /* in json-object format from str */
    int InJsonObjectFormat( const char *strjson );

};







#endif
