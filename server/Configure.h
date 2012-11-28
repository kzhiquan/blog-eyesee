
#ifndef _CONFIGURE_H_
#define _CONFIGURE_H_


class Configure{

public:
    /* constructor */
    Configure();

    /* parse  the configure file */
    int LoadFromFile(const char * file );

    /* parse a line into name and value */
    int ParseLine( char *strBuffer, int iLineStartIndex, int iLineEndIndex );

    /* set value by name */
    int SetValuesByName( char *strName, char *strValues );

};



#endif
