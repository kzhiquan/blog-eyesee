
#ifndef _CATEGORY_H_
#define _CATEGORY_H_

#include <string>
using namespace std;


#include "../db/DbConn.h"
#include "../json/cJSON.h"

/* Category object */
class Category{
private:
    int                          id;
    int                          parentid;
    basic_string<unsigned char>  name;
public:

    /* set id */
    void SetID( int id );

    /* set parent id */
    void SetParentID( int parentid );

    /* set the name */
    void SetName( const unsigned char *strName );

    /* set the Category from db */
    void GetCategoryFromDB( DbConn *pDbConn );

    /* xml Object out */
    string OutXmlObjectFormat();

    /* cJson object out */
    cJSON *OutJsonObjectFormat();

};












#endif
