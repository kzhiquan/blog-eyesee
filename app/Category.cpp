
#include "Category.h"
#include "../server/ESLog.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* set id */
void Category::SetID( int id ){
    this->id = id;
}

/* set parent id */
void Category::SetParentID( int parentid ){
    this->parentid = parentid;
}

/* set the name */
void Category::SetName( const unsigned char *strName ){
    this->name.append(strName);
}


/* set the Category from db */
void Category::GetCategoryFromDB( DbConn *pDbConn ){

    /* id */
    this->SetID(pDbConn->GetColumnInt(0));

    /* parent id */
    this->SetParentID(pDbConn->GetColumnInt(1));

    /* set name */
    this->SetName(pDbConn->GetColumnText(2));


    return ;
}

/* xml Object out */
string Category::OutXmlObjectFormat(){
        /* XmlObject format of post */
    string XmlObject;
    char IDBuf[1024];
    memset(IDBuf, 0, sizeof(IDBuf) );
   
    XmlObject.append("<category>");

    sprintf(IDBuf, "%d", this->id);
    XmlObject.append("<id>");
    XmlObject.append(IDBuf);
    XmlObject.append("</id>");


    sprintf(IDBuf, "%d", this->parentid);
    XmlObject.append("<parentid>");
    XmlObject.append(IDBuf);
    XmlObject.append("</parentid>");

    
    sprintf(IDBuf, "%s", this->name.c_str());
    XmlObject.append("<name>");
    XmlObject.append(IDBuf);
    XmlObject.append("</name>");
    
  

    XmlObject.append("</category>");


    printf("category:%s\n", XmlObject.c_str());


    return XmlObject;
}


/* cJson object out */
cJSON *Category::OutJsonObjectFormat(){

    cJSON *category;
    char *out;

    char IDBuf[1024];
    memset(IDBuf, 0, sizeof(IDBuf) );

    /* create cJSON object */
    category = cJSON_CreateObject();

    /* id */
    cJSON_AddNumberToObject( category, "id", this->id );


    /* parent id */
    cJSON_AddNumberToObject( category, "parentid", this->parentid );


    /* name */
    sprintf(IDBuf, "%s", this->name.c_str());
    cJSON_AddStringToObject( category, "name", IDBuf );

    /* print */
    out = cJSON_Print( category );
    
    ESLog::ISLog("cJson Category: %s\n",__FILE__, __LINE__,  out );

    free(out);

    return category;
}
