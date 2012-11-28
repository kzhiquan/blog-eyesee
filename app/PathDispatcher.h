
#ifndef _PATH_DISPATCHER_H_
#define _PATH_DISPATCHER_H_


#include "../server/HttpRequest.h"
#include "../server/HttpResponse.h"
#include "../server/RollBuff.h"

#include "../app/Post.h"
#include "../app/Comment.h"
#include "../app/Category.h"


#include <string>
#include <vector>
using namespace std;


/* path dispatcher */
class PathDispatcher{
public:

    /* dispath the path, and return the content */
    int Dispatch( HttpRequest &request, HttpResponse &response, RollBuff *rollBuf, string & content );

    /* tackle /homepage path */
    int HomePagePath( HttpRequest &request, HttpResponse &response, string & content );

    /* get all posts */
    int PostsPath( HttpRequest &request, HttpResponse &response, string & content );

    /* tackle /postpage path */
    int PostPagePath( HttpRequest &request, HttpResponse &response, string & content );

    /* handle /newpostpage path */
    int NewPostPagePath( HttpRequest &request, HttpResponse &response, RollBuff *rollBuf, string & content );

    /* modify post path */
    int ModifyPostPagePath( HttpRequest &request, HttpResponse &response, RollBuff *rollBuf, string & content );

    /* handle /newcommentpage path */
    int NewCommentPagePath( HttpRequest &request, HttpResponse &response, RollBuff *rollBuf, string & content );

    /* upload file */
    int UploadPagePath( HttpRequest &request, HttpResponse &response, RollBuff *rollBuf, string & content );

    /* media browser */
    int MediaBrowserPath( HttpRequest &request, HttpResponse &response, string & content );

    /* tackle /post path */
    int PostPath( HttpRequest &request, HttpResponse &response, string & content );

    /* tackle /comments path */
    int PostCategoryPath( HttpRequest &request, HttpResponse &response, string & content );

    /* tackle /categories path */
    int CategoriesPath( HttpRequest &request, HttpResponse &response, string & content );

    /* tackle /comments path */
    int CommentsPath( HttpRequest &request, HttpResponse &response, string & content );

    /* out Json string formart */
    int OutJsonFormat( vector<Post *> * posts, vector<Comment *> * comments, vector<Category *> *categories, string &content );

    /* out Json string formart */
    //int OutJsonFormat( vector<Post *> * posts, vector<Comment *> * comments, vector<Category *> *categories, string &content );

};

#endif
