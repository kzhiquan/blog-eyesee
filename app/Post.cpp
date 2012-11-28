
#include "Post.h"
#include "../server/ESLog.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* set post id */
void Post::SetID(int id){
    this->id = id;
}

/* GetID */
int Post::GetID(){
    return this->id;
}

/* set CommCount */
void Post::SetCommCount( int comm_count ){
    this->comm_count = comm_count;
}

/* Get get commcount */
int Post::GetCommCount(){
    return this->comm_count;
}

/* set the guid */
void Post::SetGUID( const unsigned char *strguid ){
    this->guid.clear();
    this->guid.append(strguid);
}

/* Get the guid */
const unsigned char * Post::GetGUID(){
    return this->guid.c_str();
}

/* set the title */
void Post::SetTitle( const unsigned char *strtitle ){
    this->title.append(strtitle);
}

/* get the title */
const unsigned char * Post::GetTitle(){
    return this->title.c_str();
}

/* set the excerpt */
void Post::SetExcerpt( const  unsigned char *strExcerpt ){
    this->excerpt.append(strExcerpt);
}

/* get the excerpt */
const unsigned char *Post::GetExcerpt(){
    return this->excerpt.c_str();
}

/* set the content */
void Post::SetContent( const  unsigned char *strContent ){
    this->content.append(strContent);
}

/* get the content */
const unsigned char *Post::GetContent(){
    return this->content.c_str();
}

/* set the post_date */
void Post::SetPostDate( const  unsigned char *strPostDate ){
    this->post_date.append(strPostDate);
}

/* get the post date */
const unsigned char *Post::GetPostDate(){
    return this->post_date.c_str();
}

/* set the post_date */
void Post::SetCategory( const  unsigned char *strCategory ){
    this->category.append(strCategory);
}

/* get the post category */
const unsigned char * Post::GetCategory(){
    return this->category.c_str();
}

/* set the post_date */
void Post::SetVisible( const  unsigned char *strVisible ){
    this->visible.append(strVisible);
}

/* get the visible */
const unsigned char *Post::GetVisible(){
    return this->visible.c_str();
}

/* set the post form dbconnection */
void Post::GetPostFromDB( DbConn *pDbConn ){

    ESLog::ISLog("GET POST FROM DB START\n", __FILE__, __LINE__);

    /* post_id */
    this->SetID(pDbConn->GetColumnInt(0));

    /* post_date */
    this->SetPostDate(pDbConn->GetColumnText(2));

    /* post_content */
    this->SetContent(pDbConn->GetColumnText(3));

    /* post_title */
    this->SetTitle(pDbConn->GetColumnText(4));

    /* post_excerpt */
    //this->SetExcerpt(pDbConn->GetColumnText(5));

    /* post_guid */
    this->SetGUID(pDbConn->GetColumnText(15));

    /* comm_counts */
    this->SetCommCount(pDbConn->GetColumnInt(19));

    /* category */
    this->SetCategory(pDbConn->GetColumnText(20));

    ESLog::ISLog("GET POST FROM DB END\n", __FILE__, __LINE__);
    
    return;
}




/* out L-Object format */
string Post::OutLObjectFormat(){

    /* LObject format of post */
    string LObject;
    char IDBuf[1024];
    memset(IDBuf, 0, sizeof(IDBuf) );
   
    LObject.append("LObjectName = post&\n");

    sprintf(IDBuf, "%d&\n", this->id);
    LObject.append("id = ");
    LObject.append(IDBuf);

    sprintf(IDBuf, "%d&\n", this->comm_count);
    LObject.append("comm_count = ");
    LObject.append(IDBuf);
    
    sprintf(IDBuf, "%s&\n", this->guid.c_str());
    LObject.append("guid = ");
    LObject.append(IDBuf);
    
    sprintf(IDBuf, "%s&\n", this->post_date.c_str());
    LObject.append("post_date = ");
    LObject.append(IDBuf);

    sprintf(IDBuf, "%s&\n", this->title.c_str());
    LObject.append("title = ");
    LObject.append(IDBuf);


    sprintf(IDBuf, "%s&\n", this->excerpt.c_str());
    LObject.append("excerpt = ");
    LObject.append(IDBuf);

    sprintf(IDBuf, "%s&\n", this->content.c_str());
    LObject.append("content = ");
    LObject.append(IDBuf);

    return LObject;
}

/* out xml-object format */
string  Post::OutXmlObjectFormat(){

    /* XmlObject format of post */
    string XmlObject;
    char IDBuf[1024];
    memset(IDBuf, 0, sizeof(IDBuf) );
   
    XmlObject.append("<post>");

    sprintf(IDBuf, "%d", this->id);
    XmlObject.append("<id>");
    XmlObject.append(IDBuf);
    XmlObject.append("</id>");


    sprintf(IDBuf, "%d", this->comm_count);
    XmlObject.append("<commcount>");
    XmlObject.append(IDBuf);
    XmlObject.append("</commcount>");

    
    sprintf(IDBuf, "%s", this->guid.c_str());
    XmlObject.append("<guid>");
    XmlObject.append(IDBuf);
    XmlObject.append("</guid>");
    
    sprintf(IDBuf, "%s", this->post_date.c_str());
    XmlObject.append("<postdate>");
    XmlObject.append(IDBuf);
    XmlObject.append("</postdate>");

    sprintf(IDBuf, "%s", this->title.c_str());
    XmlObject.append("<title>");
    XmlObject.append(IDBuf);
    XmlObject.append("</title>");

    sprintf(IDBuf, "%s", this->category.c_str());
    XmlObject.append("<category>");
    XmlObject.append(IDBuf);
    XmlObject.append("</category>");

    sprintf(IDBuf, "%s", this->excerpt.c_str());
    XmlObject.append("<excerpt>");
    XmlObject.append(IDBuf);
    XmlObject.append("</excerpt>");

    sprintf(IDBuf, "%s", this->content.c_str());
    XmlObject.append("<content>");
    XmlObject.append(IDBuf);
    XmlObject.append("</content>");


    XmlObject.append("</post>");


    printf("post:%s\n", XmlObject.c_str());



    return XmlObject;

}

/* out json-object format */
cJSON * Post::OutJsonObjectFormat(){
    
    ESLog::ISLog(" POST OUT JSON FORMAT START\n", __FILE__, __LINE__);

    //char IDBuf[8192];
    //memset(IDBuf, 0, sizeof(IDBuf) );

    /* create cJSON object */
    cJSON *post;
    post = cJSON_CreateObject();

    /* id */
    cJSON_AddNumberToObject( post, "id", this->id );

    /* commcount */
    cJSON_AddNumberToObject( post, "commcount", this->comm_count );

    /* guid */
    //sprintf(IDBuf, "%s", this->guid.c_str());
    //cJSON_AddStringToObject( post, "guid", IDBuf );
    cJSON_AddStringToObject( post, "guid", (char *)this->guid.c_str() );



    /* postdate */
    //sprintf(IDBuf, "%s", this->post_date.c_str());
    //cJSON_AddStringToObject( post, "postdate", IDBuf );
    cJSON_AddStringToObject( post, "postdate", (char *)this->post_date.c_str() );

    /* title */
    //sprintf(IDBuf, "%s", this->title.c_str());
    //cJSON_AddStringToObject( post, "title", IDBuf );
    cJSON_AddStringToObject( post, "title", (char *)this->title.c_str() );

    /* category */
    //sprintf(IDBuf, "%s", this->category.c_str());
    //cJSON_AddStringToObject( post, "category", IDBuf );
    cJSON_AddStringToObject( post, "category", (char *)this->category.c_str() );

    /* excerpt */
    //sprintf(IDBuf, "%s", this->excerpt.c_str());
    //cJSON_AddStringToObject( post, "excerpt", IDBuf );

    /* content */
    //sprintf(IDBuf, "%s", this->content.c_str());
    //cJSON_AddStringToObject( post, "content", IDBuf );
    cJSON_AddStringToObject( post, "content", (char *)this->content.c_str() );


    /* print */
    char *outjson = NULL;
    outjson = cJSON_Print( post );

    ESLog::ISLog("cJson Post: %s\n", __FILE__, __LINE__, outjson );
    ESLog::ISLog(" POST OUT JSON FORMAT END0\n", __FILE__, __LINE__);

    free(outjson);

    ESLog::ISLog(" POST OUT JSON FORMAT END\n", __FILE__, __LINE__);


    return post;
}


/* in json-object format from str */
int Post::InJsonObjectFormat( const char *strjson ){
    int err = 0;

    ESLog::ISLog("IN JSON OBJECT START!\n", __FILE__, __LINE__);

    /* parse jsonstr to post */
    cJSON *post = cJSON_Parse(strjson);

    /* title */
    char *posttitle = cJSON_GetObjectItem( post, "title")->valuestring;
    this->SetTitle((unsigned char *)posttitle);

    /* guid */
    char *postguid = cJSON_GetObjectItem( post, "guid")->valuestring;
    if( postguid != NULL) {
        this->SetGUID((unsigned char *)postguid);
    }
    

    /* content */
    char *postcontent = cJSON_GetObjectItem( post, "content")->valuestring;
    this->SetContent((unsigned char*)postcontent);

    /* category */
    char *postcategory = cJSON_GetObjectItem( post, "category")->valuestring;
    this->SetCategory((unsigned char*)postcategory);

    /* visible */
    char *postvisible = cJSON_GetObjectItem( post, "visible")->valuestring;
    this->SetVisible((unsigned char*)postvisible);

    ESLog::ISLog("IN JSON OBJECT END!\n", __FILE__, __LINE__);
    
    return err;
}


