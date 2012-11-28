

#include "Configure.h"
#include "Server.h"
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

/* constructor */
Configure::Configure(){

}

/*load from the file */
int Configure::LoadFromFile(const char *file ){
    printf("Configure: %s\n", file);
    
    int fd;

    // open the file according the filename 
    fd = open( file, O_RDONLY,  00664);
    if( fd > 0) {
        int n = 0;
        int iLineEndFlag = 0; /* if 0 the line is end, or line is not end */
        int iLineFirst = 0;
        int iLineEnd = 0;
        char strLineBuf[1024];
        char strBuf[1024];

        /* read the file until the end of the file , give the condition , the max lenght of a line is less than 1024 */
        while( (n = read(fd, strBuf, sizeof(strBuf))) > 0 ) {
            printf("%d->%s", n, strBuf);
            int i = 0;
            while( i <= n-1 ) {
                //skip the space and the \n of the line before.
                while( strBuf[i] == '\n' || strBuf[i] == ' ' ) {
                    i++;
                }
                if( iLineEndFlag == 0 ) {// the line is end
                    /* the unuseful line*/
                    if( strBuf[i] == '#') {
                        while( strBuf[i] != '\n' && i <= n-1 ) {
                            i++;
                        }

                        if( i >= n ) {
                            iLineEndFlag = 1; // not end, unuseful line
                            break;
                        }
                        i++;
                        continue;
                    }


                    /* the useful line */
                    iLineFirst = i;
                    while( strBuf[i] != '\n' && i <= n-1) {
                        i++;
                    }

                    if( i >=n ) {
                        iLineEndFlag = 2; // not end; useful line
                        memcpy( strLineBuf, strBuf+iLineFirst, n-iLineFirst+1);
                        iLineEnd = n;
                        break;
                    }

                    iLineEnd = i-1;
                    i++;
            
                    // parse the line
                    this->ParseLine( strBuf, iLineFirst, iLineEnd );
                }
                else if( iLineEndFlag == 1) { // the line is not end, unsuful
                    while( strBuf[i] != '\n' && i <= n-1 ) {
                        i++;
                    }

                    if( i >= n ) {
                        iLineEndFlag = 1; // not end, unuseful line
                        break;
                    }
                    
                    i++;
                    iLineEndFlag = 0;
                    continue;
                }
                else{ // the line is not end , useful
                    while( strBuf[i] != '\n' && i <= n-1 ) {
                        i++;
                    }

                    if( i >= n ) {
                        iLineEndFlag = 2; // not end, useful line
                        //...copy
                        break;
                    }

                    memcpy( strLineBuf+iLineEnd-iLineFirst+1, strBuf, i-1);
                    this->ParseLine(strLineBuf, 0, iLineEnd+i );
                    i++;
                    iLineEndFlag = 0;
                    continue;
                }   
            }
        }
    }
    else{
        printf("open file error:%s\n", strerror(errno));
    }

    return 0;
}


/* parse a line */
int Configure::ParseLine(char *strBuf, int iLineStartIndex, int iLineEndIndex ){
    printf("%d-%d", iLineStartIndex, iLineEndIndex);
    for( int i = iLineStartIndex; i <= iLineEndIndex; i++ ) {
        printf("%c", strBuf[i]);
    }
    printf("\n");

    int result = 0;
    
    int  iSep = 0;
    int  iNameFirstIndex = 0;
    int  iNameEndIndex = 0;
    int  iValuesFirstIndex = 0;
    int  iValuesEndIndex = 0;
    char strName[16];
    char strValues[256];
    
    /* find the first begin index for name */
    int i = iLineStartIndex;
    while( i <= iLineEndIndex && strBuf[i] == ' ') {
        i++;
    }
    iNameFirstIndex = i;
    /* find the seperator '=' */
    while( i <= iLineEndIndex && strBuf[i] != '='){
        i++;
    }
    iSep = i;
    /* find the end index for name  */
    int j = iSep-1;
    while( j>= iLineStartIndex && strBuf[j] == ' ' ){
        j--;
    }
    iNameEndIndex = j;
    /* copy the name string */
    memcpy( strName, strBuf+iNameFirstIndex, iNameEndIndex-iNameFirstIndex+1 );
    strName[iNameEndIndex-iNameFirstIndex+1] = '\0';

    /* find the first begin index for values */
    i++;
    while( i<= iLineEndIndex && strBuf[i] == ' ') {
        i++;
    }
    iValuesFirstIndex = i;
    /* find the end index for values */
    j = iLineEndIndex;
    while( j>= iValuesFirstIndex && strBuf[j] == ' ') {
        j--;
    }
    iValuesEndIndex = j;
    /* copy the values string */
    memcpy( strValues, strBuf+iValuesFirstIndex, iValuesEndIndex-iValuesFirstIndex+1);
    strValues[iValuesEndIndex-iValuesFirstIndex+1] = '\0';

    printf("Name:%s\n", strName);
    printf("Values:%s\n", strValues);

    result = this->SetValuesByName(strName,strValues);

    return result;
    
}

/* set the server parameters by name */
int Configure::SetValuesByName(char *strName, char *strValues){

    int result = 0;
    


    /* the site ip parameters */
    if( strcmp( strName, "ip") == 0 ) {
        Server::Instance()->GetSite().SetIp(strValues);
    }

    /* the site port parameters */
    else if( strcmp( strName, "port") == 0 ) {
        Server::Instance()->GetSite().SetPort(strValues);

    }

    /* the site listen back log  parameters */
    else if( strcmp( strName, "lisnBackLog") == 0 ) {
        Server::Instance()->GetSite().SetLisnBackLog(strValues);
    }

    /* the site rootDirectory parameters */
    else if( strcmp( strName, "rootDirectory") == 0 ) {
        Server::Instance()->GetSite().SetRootDir(strValues);

    }

    /* the site defaultPage parameters */
    else if( strcmp( strName, "defaultPage") == 0 ) {
        Server::Instance()->GetSite().SetDefaultPage(strValues);
    }

    /* the pollsize for the epfd */
    else if( strcmp( strName, "pollSize") == 0 ){
        int pollSize = atoi(strValues);
        Server::Instance()->GetIOLoop().SetPollSize(pollSize);
    }

    /* the maxEvents of the events array size */
    else if( strcmp( strName, "maxEvents") == 0) {
        int maxEvents = atoi(strValues);
        Server::Instance()->GetIOLoop().SetMaxEvents(maxEvents);
    }

    /* the data base name  */
    else if( strcmp( strName, "dbname") == 0) {
        Server::Instance()->GetDbConnMg().SetDbName(strValues);
    }

    /* the log file name  */
    else if( strcmp( strName, "logfile") == 0) {
        Server::Instance()->GetESLog().LoadLogFileName(strValues);
    }


    else{
        //result = -1;
        printf("strName:%s->strValues:%s, unknows parameters\n", strName, strValues );
    }

    return result;
}
