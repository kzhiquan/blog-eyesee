

#include "Comment.h"
#include "../server/ESLog.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/* set id */
void Comment::SetID(int id){
    this->id = id;
}

/* get id */
int Comment::GetID(){
    return this->id;
}

/* set Post_id */
void Comment::SetPostID( int postID ){
    this->post_id = postID;
}

/* get post id */
int Comment::GetPostID(){
    return this->post_id;
}

/* set Post_guid */
void Comment::SetPostGUID( const unsigned char * postGUID ){
    this->post_guid.append(postGUID);
}

/* get post guid */
const unsigned char *Comment::GetPostGUID(){
    return this->post_guid.c_str();
}

/* set the comm_date */
void Comment::SetCommDate(const unsigned char * strCommDate ){
    this->comm_date.append(strCommDate);
}

/* get the comm_date */
const unsigned char * Comment::GetCommDate(){
    return this->comm_date.c_str();
}

/* set the comm_author */
void Comment::SetCommAuthor( const unsigned char * strCommAuthor ){
    this->comm_author.append(strCommAuthor);
}

/* get the comm_author */
const unsigned char * Comment::GetCommAuthor(){
    return this->comm_author.c_str();
}


/* set the comm_author_email */
void Comment::SetCommAuthorEmail( const unsigned char *strCommAuthorEmail ){
    this->comm_author_email.append(strCommAuthorEmail);
    
}

/* get the comm_author email */
const unsigned char * Comment::GetCommAuthorEmail(){
    return this->comm_author_email.c_str();
}

/* Set the Comm_author_url */
void Comment::SetCommAuthorUrl( const unsigned char *strCommAuthorUrl ){
    this->comm_author_url.append(strCommAuthorUrl);
}

/* get the comm_author url */
const unsigned char * Comment::GetCommAuthorUrl(){
    return this->comm_author_url.c_str();
}


/* Set the Com_content */
void Comment::SetCommContent( const unsigned char *strCommContent ){
    this->comm_content.append(strCommContent);
}

/* get the comm_content */
const unsigned char * Comment::GetCommContent(){
    return this->comm_content.c_str();
}


/* set object form dbConnectin */
void Comment::GetCommentFromDB(DbConn* pDbConn ){
    
    /* comment_id */
    this->SetID( pDbConn->GetColumnInt(0) );

    /* post _id */
    this->SetPostID( pDbConn->GetColumnInt(1) );

    /* author */
    this->SetCommAuthor( pDbConn->GetColumnText(2) );

    /* author_email */
    this->SetCommAuthorEmail( pDbConn->GetColumnText(3) );

    /* author_url */
    this->SetCommAuthorUrl( pDbConn->GetColumnText(4) );

    /* comm_date */
    this->SetCommDate( pDbConn->GetColumnText(6) );

    /* comm_content */
    this->SetCommContent( pDbConn->GetColumnText(8) );

    return;
}



/* Out L-Object format comment */
string Comment::OutLObjectFormat(){

    /* LObject format of post */
    string LObject;
    
    char IDBuf[1024];
    memset( IDBuf, 0, sizeof(IDBuf) );
    
    
    LObject.append("LObjectName = comment&\n");

    sprintf(IDBuf, "%d", this->id);
    LObject.append("id = ");
    LObject.append(IDBuf);
    LObject.append("&\n");

    sprintf(IDBuf, "%d", this->post_id);
    LObject.append("post_id = ");
    LObject.append(IDBuf);
    LObject.append("&\n");

    sprintf(IDBuf, "%s&\n", this->comm_date.c_str());
    LObject.append("comm_date = ");
    LObject.append(IDBuf);

    sprintf(IDBuf, "%s&\n", this->comm_author.c_str());
    LObject.append("comm_author = ");
    LObject.append(IDBuf);

    sprintf(IDBuf, "%s&\n", this->comm_author_email.c_str());
    LObject.append("comm_author_email = ");
    LObject.append(IDBuf);

    sprintf(IDBuf, "%s&\n", this->comm_author_url.c_str());
    LObject.append("comm_author_url = ");
    LObject.append(IDBuf);

    sprintf(IDBuf, "%s&\n", this->comm_content.c_str());
    LObject.append("comm_content = ");
    LObject.append(IDBuf);


    return LObject;
}


/* Out XmlObject format comment */
string Comment::OutXmlObjectFormat(){
    /* LObject format of post */
    string XmlObject;
    
    char IDBuf[1024];
    memset( IDBuf, 0, sizeof(IDBuf) );
    
    
    XmlObject.append("<comment>");

    sprintf(IDBuf, "%d", this->id);
    XmlObject.append("<id>");
    XmlObject.append(IDBuf);
    XmlObject.append("</id>");

    sprintf(IDBuf, "%d", this->post_id);
    XmlObject.append("<postid>");
    XmlObject.append(IDBuf);
    XmlObject.append("</postid>");

    sprintf(IDBuf, "%s", this->comm_date.c_str());
    XmlObject.append("<commdate>");
    XmlObject.append(IDBuf);
    XmlObject.append("</commdate>");

    sprintf(IDBuf, "%s", this->comm_author.c_str());
    XmlObject.append("<commauthor>");
    XmlObject.append(IDBuf);
    XmlObject.append("</commauthor>");

    sprintf(IDBuf, "%s", this->comm_author_email.c_str());
    XmlObject.append("<commauthoremail>");
    XmlObject.append(IDBuf);
    XmlObject.append("</commauthoremail>");

    sprintf(IDBuf, "%s", this->comm_author_url.c_str());
    XmlObject.append("<commauthorurl>");
    XmlObject.append(IDBuf);
    XmlObject.append("</commauthorurl>");

    sprintf(IDBuf, "%s", this->comm_content.c_str());
    XmlObject.append("<commcontent>");
    XmlObject.append(IDBuf);
    XmlObject.append("</commcontent>");


    XmlObject.append("</comment>");


    return XmlObject;

}



/* out cJson formt comment */
cJSON *Comment::OutJsonObjectFormat(){

    cJSON *comment;
    char *out;

    char IDBuf[1024];
    memset(IDBuf, 0, sizeof(IDBuf) );

    /* create cJSON object */
    comment = cJSON_CreateObject();

    /* id */
    cJSON_AddNumberToObject( comment, "id", this->id );

    /* commcount */
    cJSON_AddNumberToObject( comment, "postid", this->post_id );

    /* commdate */
    sprintf(IDBuf, "%s", this->comm_date.c_str());
    cJSON_AddStringToObject( comment, "commdate", IDBuf );

    /* comm_author  */
    sprintf(IDBuf, "%s", this->comm_author.c_str());
    cJSON_AddStringToObject( comment, "commauthor", IDBuf );

    /* commauthorurl*/
    sprintf(IDBuf, "%s", this->comm_author_email.c_str());
    cJSON_AddStringToObject( comment, "commauthoremail", IDBuf );

    /* commauthorurl */
    sprintf(IDBuf, "%s", this->comm_author_url.c_str());
    cJSON_AddStringToObject( comment, "commauthorurl", IDBuf );

    /* commcontent */
    sprintf(IDBuf, "%s", this->comm_content.c_str());
    cJSON_AddStringToObject( comment, "commcontent", IDBuf );

    /* print */
    out = cJSON_Print( comment );
    
    ESLog::ISLog("cJson Comment: %s\n", __FILE__, __LINE__, out );

    free(out);


    return comment;
}

/* in json-object format from str */
int Comment::InJsonObjectFormat( const char *strjson ){
    int err = 0;

    /* parse jsonstr to post */
    cJSON *comment = cJSON_Parse(strjson);

    /* postguid */
    char *postguid = cJSON_GetObjectItem( comment, "postguid")->valuestring;
    this->SetPostGUID((unsigned char *)postguid);

    /* author */
    char *commauthor = cJSON_GetObjectItem( comment, "commname")->valuestring;
    this->SetCommAuthor((unsigned char *)commauthor);

    /* email */
    char *commemail = cJSON_GetObjectItem( comment, "commemail")->valuestring;
    this->SetCommAuthorEmail((unsigned char*)commemail);

    /* url */
    char *commurl = cJSON_GetObjectItem( comment, "commsite")->valuestring;
    this->SetCommAuthorUrl((unsigned char*)commurl);

    /* content */
    char *commcontent = cJSON_GetObjectItem( comment, "commcontent")->valuestring;
    this->SetCommContent((unsigned char*)commcontent);

    return err;
}
