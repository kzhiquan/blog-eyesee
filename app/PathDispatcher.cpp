

#include "PathDispatcher.h"
#include "Post.h"
#include "Comment.h"
#include "../server/ESLog.h"
#include "../server/RollBuff.h"
#include "../server/CFile.h"
#include "../server/Server.h"

#include "../db/BgDao.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

/* dispath the path, and return the content */
int PathDispatcher::Dispatch( HttpRequest &request, HttpResponse &response, RollBuff *rollBuf, string & content ){
    
    int err = 0;
    
    ESLog::ISLog("PATH DISPATCHER DISPATCH START\n", __FILE__, __LINE__);

    /* pathfile */
    string & path = request.GetUri().GetFileName();
    ESLog::ISLog("path: %s\n", __FILE__, __LINE__, path.c_str());

    /* cmd type */
    //char *strcmd = request.GetCmdType();

    /* home page path */
    if( strcmp( path.c_str(), "/homepage") == 0 ) {
        
        err = this->HomePagePath( request, response, content);
        if( err ) {
            ESLog::ISLog("Post path error: %d\n", __FILE__, __LINE__, err );
        } 
    }

    /* post page path */
    else if( strcmp( path.c_str(), "/postpage") == 0 ) {
        
        err = this->PostPagePath( request, response, content);
        if( err ) {
            ESLog::ISLog("Post path error: %d\n", __FILE__, __LINE__, err );
        } 
    }

    /* newpostpage*/
    else if( strcmp( path.c_str(), "/newpostpage") == 0 ) {
        err = this->NewPostPagePath( request, response, rollBuf,content);
        if( err ) {
            ESLog::ISLog("Post path error: %d\n", __FILE__, __LINE__, err );
        }
    }

    /* modifypostpage*/
    else if( strcmp( path.c_str(), "/modifypostpage") == 0 ) {
        err = this->ModifyPostPagePath( request, response, rollBuf,content);
        if( err ) {
            ESLog::ISLog("Post path error: %d\n", __FILE__, __LINE__, err );
        }
    }

    /* newcommentpage*/
    else if( strcmp( path.c_str(), "/newcommentpage") == 0 ) {
        err = this->NewCommentPagePath( request, response, rollBuf,content);
        if( err ) {
            ESLog::ISLog("comment path error: %d\n", __FILE__, __LINE__, err );
        }
    }

    /* upload */
    else if( strcmp( path.c_str(), "/upload") == 0 ) {
        err = this->UploadPagePath( request, response, rollBuf, content);
        if( err ) {
            ESLog::ISLog("Upload path err:%d\n", __FILE__, __LINE__, err );
        }
    }

    /* mediabrowser */
    else if( strcmp( path.c_str(), "/mediabrowser") == 0 ) {
        err = this->MediaBrowserPath( request, response, content );
        if( err ) {
            ESLog::ISLog("Media browser page path err:%d\n", __FILE__, __LINE__, err );
        }
    }
    
    /* get all posts */
    else if( strcmp( path.c_str(), "/posts") == 0 ) {
        err = this->PostsPath( request, response, content );
        if( err ) {
            ESLog::ISLog("Media browser page path err:%d\n", __FILE__, __LINE__, err );
        }
    }

    /* post path */
    else if( strcmp( path.c_str(), "/post") == 0 ) {
        
        err = this->PostPath( request, response, content);
        if( err ) {
            ESLog::ISLog("Post path error: %d\n", __FILE__, __LINE__, err );
        } 
    }

    /* post category path */
    else if( strcmp( path.c_str(), "/postcategory") == 0 ) {
        
        err = this->PostCategoryPath( request, response, content);
        if( err ) {
            ESLog::ISLog("Post path error: %d\n", __FILE__, __LINE__, err );
        } 
     }

     /* categories path */
     else if( strcmp( path.c_str(), "/categories") == 0 ) {
         err= this->CategoriesPath( request, response, content );
         if( err ) {
             ESLog::ISLog("Category path error:%d\n",__FILE__, __LINE__,  err );
         }
     }


     /* comments path */
     else if( strcmp( path.c_str(), "/comments") == 0 ) {
         err= this->CommentsPath( request, response, content );
         if( err ) {
             ESLog::ISLog("Comments path error:%d\n", __FILE__, __LINE__, err );
         }
     }
     else{
         ESLog::ISLog(" path can not found!\n", __FILE__, __LINE__);
         err = -1;
     }

     ESLog::ISLog("PATH DISPATCHER DISPATCH END\n", __FILE__, __LINE__);

     return err;
} 



/* tackle /homepage path */
int PathDispatcher::HomePagePath( HttpRequest &request, HttpResponse &response, string & content ){
    int err = 0;

    ESLog::ISLog("PATH DISPATCHER HOMEPAGE START\n", __FILE__, __LINE__);

    /* posts */
    vector<Post *> posts;

    /* category */
    vector<Category *> categories;

    /* out Json */
    char *outJson;

    /* data access object */
    BgDao bgDao;

    /* init */
    err = bgDao.Init();
    if( err ) {
        ESLog::ISLog("bg Dao init error!\n", __FILE__, __LINE__);
        return err;
    }

    /* Get the Object posts */
    err = bgDao.GetPostsTopNum(10, posts);
    if( err ) {
        return err;
    }

    /* Get the Object posts */
    err = bgDao.GetAllCategories(categories);
    if( err ) {
        return err;
    }

    /* json output */
    err = this->OutJsonFormat(&posts, NULL, &categories,content);
    if( err ) {
        ESLog::ISLog("out put Json errors !\n", __FILE__, __LINE__);
        return err;
    }



    ESLog::ISLog("end:%s end\n", __FILE__, __LINE__, content.c_str() );

    ESLog::ISLog("PATH DISPATCHER HOMEPAGE END\n", __FILE__, __LINE__);
    
    return err;
}


/* tackle /postpage path */
int PathDispatcher::PostPagePath( HttpRequest &request, HttpResponse &response, string & content ){
    int err = 0;

    ESLog::ISLog("PATH DISPATCHER POSTPAGE START\n", __FILE__, __LINE__);
    /* post */
    Post *post = new Post();

    vector<Post *> posts;

    /* category */
    vector<Comment *> comments;

    /* data access object */
    BgDao bgDao;

    /* init */
    err = bgDao.Init();
    if( err ) {
        ESLog::ISLog("bg Dao init error!\n", __FILE__, __LINE__);
        return err;
    }

    /* get parameter guid of post */
    string & postguid = request.GetUri().GetQueryValuebyName("guid");
    ESLog::ISLog("guid=%s\n", __FILE__, __LINE__, postguid.c_str());

    /* Get the Object posts */
    err = bgDao.GetPostByGUID(postguid.c_str(), (*post) );
    if( err ) {
        return err;
    }
    posts.push_back(post);

    /* Get the Object posts */
    err = bgDao.GetCommentsByPostID(comments, post->GetID() );
    if( err ) {
        return err;
    }
    
    /* json output */
    err = this->OutJsonFormat(&posts, &comments, NULL,content);
    if( err ) {
        ESLog::ISLog("out put Json errors !\n", __FILE__, __LINE__);
        return err;
    }

    ESLog::ISLog("end:%s end\n", __FILE__, __LINE__, content.c_str());

    ESLog::ISLog("PATH DISPATCHER POSTPAGE END\n", __FILE__, __LINE__);
    
    return err;

}

/* handle /newpostpage path */
int PathDispatcher::NewPostPagePath( HttpRequest &request, HttpResponse &response, RollBuff *rollBuf, string & content ){
    int err = 0;

    ESLog::ISLog("PATH DISPATCHER NEWPOSTPAGE START\n", __FILE__, __LINE__);

    char *strcmd = request.GetCmdType();
    ESLog::ISLog("cmd:%s\n", __FILE__, __LINE__, strcmd);

    /* post cmd */
    if( strcmp( strcmd, "POST") == 0 ) {
        /* get parameter guid of post */
        string  action = request.GetUri().GetQueryValuebyName("action");
        ESLog::ISLog("action=%s\n", __FILE__, __LINE__, action.c_str());

        /* publish */
        if( strcmp(action.c_str(), "publish") == 0 ) {
            
            /* strpost */
            string strpost;

            /* content_length */
            int content_length = request.GetHttpRequestContentLenght();

            ESLog::ISLog("content_lenght:%d\n", __FILE__, __LINE__, content_length);

            /* get content buffer */
            while( content_length ) {
                int sleepcnt  = 0;
                char *buf = NULL;

                int spansize = 0;

                ESLog::ISLog("start rollbuf first:%d, last:%d\n", __FILE__, __LINE__, rollBuf->GetFirstIndex(), rollBuf->GetLastIndex());
                buf = rollBuf->GetPerfectWriteBufPtr(&spansize);
                if( buf == NULL ) {
                    sleepcnt++;
                    if( sleepcnt > 5 ) {
                        ESLog::ISLog("can not get ennough data from client!\n", __FILE__, __LINE__);
                        err = -1;
                        return err;
                    }
                }
                
                /* update first index */
                rollBuf->UpdateFirstIndex(spansize);

                ESLog::ISLog("start rollbuf first:%d, last:%d\n", __FILE__, __LINE__, rollBuf->GetFirstIndex(), rollBuf->GetLastIndex());

                /* append into post */
                strpost.append(buf, spansize);
                content_length -= spansize;
            }
            ESLog::ISLog("read data from rollbuf:%s\n", __FILE__, __LINE__, strpost.c_str());

            /* post object */
            Post post;

            /* jsonc post */
            post.InJsonObjectFormat(strpost.c_str());


            /* data access object */
            BgDao bgDao;

            /* init */
            err = bgDao.Init();
            if( err ) {
                ESLog::ISLog("bg Dao init error!\n", __FILE__, __LINE__);
                return err;
            }

            /* post sequence */
            int postsequence = 0;

            /* get curent post id */
            err = bgDao.InsertPostSequence();
            if( err ) {
                ESLog::ISLog("bg dao insert postid sequence err!\n", __FILE__, __LINE__);
                return err;
            }

            /* get curent post id */
            err = bgDao.GetLastInsertPostSequence(&postsequence);
            if( err ) {
                ESLog::ISLog("bg dao get last insert postid sequence err!\n", __FILE__, __LINE__);
                return err;
            }

            ESLog::ISLog("post sequence :%d\n", __FILE__, __LINE__, postsequence );

            /* set post id */
            post.SetID(postsequence);

            /* set post comm_count */
            post.SetCommCount(0);

            /* set guid */
            string guid;
            guid.append("blog/post");

            char seqbuf[10];
            memset(seqbuf, 0, sizeof(seqbuf));
            sprintf( seqbuf, "%d", postsequence );
            guid.append(seqbuf);

            post.SetGUID((unsigned char *)guid.c_str());

            /* insert into dbase */
            err = bgDao.InsertPost(post);
            if( err ) {
                ESLog::ISLog("bg dao insert into post err!\n", __FILE__, __LINE__);
                return err;
            }

            /* get taxonomy id */
            int taxonomyid = 0;
            err = bgDao.GetPostTaxonomyID(post.GetCategory(), &taxonomyid );
            if( err ) {
                ESLog::ISLog("db dao get post taxonomy id err!\n", __FILE__, __LINE__);
                return err;
            }

            ESLog::ISLog("taxonomyid :%d\n", __FILE__, __LINE__, taxonomyid );

            /* insert into term_relationships */
            err = bgDao.InsertTermRelationship(postsequence, taxonomyid);
            if( err ) {
                ESLog::ISLog("db dao insert term relationship err!\n", __FILE__, __LINE__);
                return err;
            }

        }

    }
    else if( strcmp( strcmd, "GET") == 0 ) {

    }
    else{
        ESLog::ISLog("cmd type error!\n", __FILE__, __LINE__);
        err = -1;
    }

    ESLog::ISLog("PATH DISPATCHER NEWPOSTPAGE END\n", __FILE__, __LINE__);
    
    return err;

}


/* handle /modifypostpage path */
int PathDispatcher::ModifyPostPagePath( HttpRequest &request, HttpResponse &response, RollBuff *rollBuf, string & content ){
    int err = 0;

    ESLog::ISLog("PATH DISPATCHER MODIFYPOSTPAGE START\n", __FILE__, __LINE__);

    char *strcmd = request.GetCmdType();
    ESLog::ISLog("cmd:%s\n", __FILE__, __LINE__, strcmd);

    /* post cmd */
    if( strcmp( strcmd, "POST") == 0 ) {
        /* get parameter guid of post */
        string  action = request.GetUri().GetQueryValuebyName("action");
        ESLog::ISLog("action=%s\n", __FILE__, __LINE__, action.c_str());

        /* publish */
        if( strcmp(action.c_str(), "modify") == 0 ) {
            
            /* strpost */
            string strpost;

            /* content_length */
            int content_length = request.GetHttpRequestContentLenght();

            ESLog::ISLog("content_lenght:%d\n", __FILE__, __LINE__, content_length);

            /* get content buffer */
            while( content_length ) {
                int sleepcnt  = 0;
                char *buf = NULL;

                int spansize = 0;

                ESLog::ISLog("start rollbuf first:%d, last:%d\n", __FILE__, __LINE__, rollBuf->GetFirstIndex(), rollBuf->GetLastIndex());
                buf = rollBuf->GetPerfectWriteBufPtr(&spansize);
                if( buf == NULL ) {
                    sleepcnt++;
                    if( sleepcnt > 5 ) {
                        ESLog::ISLog("can not get ennough data from client!\n", __FILE__, __LINE__);
                        err = -1;
                        return err;
                    }
                }
                
                /* update first index */
                rollBuf->UpdateFirstIndex(spansize);

                ESLog::ISLog("start rollbuf first:%d, last:%d\n", __FILE__, __LINE__, rollBuf->GetFirstIndex(), rollBuf->GetLastIndex());

                /* append into post */
                strpost.append(buf, spansize);
                content_length -= spansize;
            }
            ESLog::ISLog("read data from rollbuf:%s\n", __FILE__, __LINE__, strpost.c_str());

            /* post object */
            Post post;

            /* jsonc post */
            post.InJsonObjectFormat(strpost.c_str());


            /* data access object */
            BgDao bgDao;

            /* init */
            err = bgDao.Init();
            if( err ) {
                ESLog::ISLog("bg Dao init error!\n", __FILE__, __LINE__);
                return err;
            }


            char guidbuf[256];
            memset( guidbuf, 0, sizeof(guidbuf));
            sprintf( guidbuf, "%s", post.GetGUID());
            /* modify post */
            err = bgDao.UpdatePostByGUID(guidbuf,post);
            if( err ) {
                ESLog::ISLog("bg dao modify post error!\n", __FILE__, __LINE__);
                return err;
            }


        }

    }
    else if( strcmp( strcmd, "GET") == 0 ) {

    }
    else{
        ESLog::ISLog("cmd type error!\n", __FILE__, __LINE__);
        err = -1;
    }

    ESLog::ISLog("PATH DISPATCHER MODIFYPOSTPAGE END\n", __FILE__, __LINE__);
    
    return err;

}

/* handle /newcommentpage path */
int PathDispatcher::NewCommentPagePath( HttpRequest &request, HttpResponse &response, RollBuff *rollBuf, string & content ){
    int err = 0;

    ESLog::ISLog("PATH DISPATCHER NEWCOMMENTPAGE START\n", __FILE__, __LINE__);

    char *strcmd = request.GetCmdType();
    ESLog::ISLog("cmd:%s\n", __FILE__, __LINE__, strcmd);

    /* post cmd */
    if( strcmp( strcmd, "POST") == 0 ) {
        /* get parameter guid of post */
        string  action = request.GetUri().GetQueryValuebyName("action");
        ESLog::ISLog("action=%s\n", __FILE__, __LINE__, action.c_str());

        /* publish */
        if( strcmp(action.c_str(), "comment") == 0 ) {
            
            /* strcomment */
            string strcomment;

            /* content_length */
            int content_length = request.GetHttpRequestContentLenght();

            ESLog::ISLog("content_lenght:%d\n", __FILE__, __LINE__, content_length);

            /* get content buffer */
            while( content_length ) {
                int sleepcnt  = 0;
                char *buf = NULL;

                int spansize = 0;

                ESLog::ISLog("start rollbuf first:%d, last:%d\n", __FILE__, __LINE__, rollBuf->GetFirstIndex(), rollBuf->GetLastIndex());
                buf = rollBuf->GetPerfectWriteBufPtr(&spansize);
                if( buf == NULL ) {
                    sleepcnt++;
                    if( sleepcnt > 5 ) {
                        ESLog::ISLog("can not get ennough data from client!\n", __FILE__, __LINE__);
                        err = -1;
                        return err;
                    }
                }
                
                /* update first index */
                rollBuf->UpdateFirstIndex(spansize);

                ESLog::ISLog("start rollbuf first:%d, last:%d\n", __FILE__, __LINE__, rollBuf->GetFirstIndex(), rollBuf->GetLastIndex());

                /* append into post */
                strcomment.append(buf, spansize);
                content_length -= spansize;
            }
            ESLog::ISLog("read data from rollbuf:%s\n", __FILE__, __LINE__, strcomment.c_str());

            /* comment object */
            Comment comment;

            /* jsonc post */
            comment.InJsonObjectFormat(strcomment.c_str());

            /* data access object */
            BgDao bgDao;

            /* init */
            err = bgDao.Init();
            if( err ) {
                ESLog::ISLog("bg Dao init error!\n", __FILE__, __LINE__);
                return err;
            }

            /* post sequence */
            int commsequence = 0;

            /* get curent post id */
            err = bgDao.InsertCommSequence();
            if( err ) {
                ESLog::ISLog("bg dao insert commid sequence err!\n", __FILE__, __LINE__);
                return err;
            }

            /* get curent post id */
            err = bgDao.GetLastInsertCommSequence(&commsequence);
            if( err ) {
                ESLog::ISLog("bg dao get last insert commid sequence err!\n", __FILE__, __LINE__);
                return err;
            }

            ESLog::ISLog("comm sequence :%d\n", __FILE__, __LINE__, commsequence );
            /* set comm id */
            comment.SetID(commsequence);

            /* update post */
            Post post;

            char postguidbuf[256];
            memset( postguidbuf, 0, sizeof(postguidbuf));
            sprintf( postguidbuf, "%s", comment.GetPostGUID());

            /* get post by guid */
            err = bgDao.GetPostByGUID( postguidbuf, post );
            if( err ) {
                ESLog::ISLog("bg dao get post by guid err!\n", __FILE__, __LINE__);
                return err;
            }

            /* set post comm count */
            post.SetCommCount(post.GetCommCount()+1);

            /* update post by guid */
            err = bgDao.UpdatePostByGUID( postguidbuf, post );
            if( err ) {
                ESLog::ISLog("bg dao update post by guid err!\n", __FILE__, __LINE__);
                return err;
            }

            /* get comment id */
            comment.SetPostID(post.GetID());

            /* insert into comment dbase */
            err = bgDao.InsertComment(comment);
            if( err ) {
                ESLog::ISLog("bg dao insert into comment err!\n", __FILE__, __LINE__);
                return err;
            }


        }

    }
    else if( strcmp( strcmd, "GET") == 0 ) {

    }
    else{
        ESLog::ISLog("cmd type error!\n", __FILE__, __LINE__);
        err = -1;
    }

    ESLog::ISLog("PATH DISPATCHER NEWCOMMENTPAGE END\n", __FILE__, __LINE__);
    
    return err;

}

/* handle /newpostpage path */
int PathDispatcher::UploadPagePath( HttpRequest &request, HttpResponse &response, RollBuff *rollBuf, string & content ){
    int err = 0;

    ESLog::ISLog("PATH DISPATCHER UPLOADPAGE START\n", __FILE__, __LINE__);

    char *strcmd = request.GetCmdType();
    ESLog::ISLog("cmd:%s\n", __FILE__, __LINE__, strcmd);

    /* post cmd */
    if( strcmp( strcmd, "POST") == 0 ) {
        /* get parameter guid of post */
        string  filename = request.GetUri().GetQueryValuebyName("filename");
        ESLog::ISLog("filename=%s\n", __FILE__, __LINE__, filename.c_str());

        string  uploadpath = request.GetUri().GetQueryValuebyName("uploadpath");
        ESLog::ISLog("uploadpath=%s\n", __FILE__, __LINE__, uploadpath.c_str());
            
        /*string strpost;*/

        /* create the directory */
        char directoryfullname[256];
        memset(directoryfullname,  0, sizeof(directoryfullname));
        sprintf( directoryfullname, "web/upload/%s", uploadpath.c_str());
        if( !Server::Instance()->GetSite().IsFileExist(directoryfullname) ) {
            err = mkdir( directoryfullname, S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IWGRP|S_IXGRP|S_IROTH|S_IWOTH|S_IXOTH);
            if( err < 0 ) {
                ESLog::ISLog("mkdir error:%s\n", __FILE__, __LINE__, strerror(errno));
            }
            err = 0;
        }

        /* create the file */
        char filefullname[256];
        memset(filefullname,  0, sizeof(filefullname));
        sprintf( filefullname, "web/upload/%s/%s", uploadpath.c_str(), filename.c_str());
        int fd = open( filefullname, O_WRONLY|O_CREAT|O_TRUNC|O_SYNC, S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IWGRP|S_IXGRP|S_IROTH|S_IWOTH|S_IXOTH);
        if( fd < 0 ) {
            ESLog::ISLog("open file error:%s\n", __FILE__, __LINE__, strerror(errno));
            return -1;
        }

        /* content_length */
        int content_length = request.GetHttpRequestContentLenght();

        ESLog::ISLog("content_lenght:%d\n", __FILE__, __LINE__, content_length);
        
        int uploadcnt  = 0;
        /* get content buffer */
        while( content_length ) {
            char *buf = NULL;

            int spansize = 0;

            ESLog::ISLog("app start read rollbuf first:%d, last:%d, content-length:%d\n", __FILE__, __LINE__, rollBuf->GetFirstIndex(), rollBuf->GetLastIndex(), content_length);
            buf = rollBuf->GetPerfectWriteBufPtr(&spansize);
            if( buf == NULL ) {
                ESLog::ISLog("app there is no data in rollbuf\n", __FILE__, __LINE__);
                continue;
                /*sleepcnt++;
                if( sleepcnt > 5 ) {
                    ESLog::ISLog("can not get ennough data from client!\n", __FILE__, __LINE__);
                    err = -1;
                    return err;
                }*/
                //sleep(500);
            }
                
            /* update first index */
            rollBuf->UpdateFirstIndex(spansize);

            /* append into post */
            //strpost.append(buf, spansize);
            content_length -= spansize;
            uploadcnt += spansize;

            int writenum = 0;
            int writecur = 0;
            /* write the buf in file */
            while( spansize ) {
                writenum = write(fd, (void *)(buf+writecur), spansize );
                if( writenum < 0 ) {
                    ESLog::ISLog("Write error:%s\n", __FILE__, __LINE__, strerror(errno));
                    if(  errno  ==  EAGAIN ) {
                        continue;
                    }
                    
                    err = -1;
                    break;
                }
                writecur += writenum;
                spansize -= writenum;
            }

            //string tmp;
            //tmp.append(buf, spansize);
            ESLog::ISLog("app read rollbuf data lenght:%d, writenum:%d\n", __FILE__, __LINE__, spansize, writenum );
            //ESLog::ISLog("app read rollbuf data:%s\n", __FILE__, __LINE__, tmp.c_str());
            ESLog::ISLog("app end read rollbuf first:%d, last:%d, content-length:%d\n", __FILE__, __LINE__, rollBuf->GetFirstIndex(), rollBuf->GetLastIndex(), content_length);

            if( err < 0 ) {
                break;
            }

      }

      close(fd);

      /* update site files */
      Server::Instance()->GetSite().Init();

      //ESLog::ISLog("read data from rollbuf:%s\n", __FILE__, __LINE__, strpost.c_str());
      ESLog::ISLog("read data from rollbuf:%d\n", __FILE__, __LINE__, uploadcnt);

    }
    else if( strcmp( strcmd, "GET") == 0 ) {

    }
    else{
        ESLog::ISLog("cmd type error!\n", __FILE__, __LINE__);
        err = -1;
    }

    ESLog::ISLog("PATH DISPATCHER UPLOAD END\n", __FILE__, __LINE__);
    
    return err;

}

/* tackle /mediabrowser path */
int PathDispatcher::MediaBrowserPath( HttpRequest &request, HttpResponse &response, string & content ){
    int err = 0;

    ESLog::ISLog("MEDIA BROWSER PATH START\n", __FILE__, __LINE__);

    /* get parameter browser path name */
    string & pathname = request.GetUri().GetQueryValuebyName("path");
    ESLog::ISLog("pathname=%s\n", __FILE__, __LINE__, pathname.c_str());

    /* file */
    vector<string> files;
    vector<string> directories;

    CFile siteFileManager = Server::Instance()->GetSite().GetFileManager();

    /* Get file */
    err = siteFileManager.GetFiles(pathname.c_str(),files);
    if( err ) {
        ESLog::ISLog("Get File Errors:%d\n", __FILE__, __LINE__, err);
        return err;
    }

    /* Get directory */
    err = siteFileManager.GetDirectories(pathname.c_str(), directories );
    if( err ) {
        ESLog::ISLog("Get directory Errors:%d\n", __FILE__, __LINE__, err);
        return err;
    }


    /* out put cJson */
    cJSON *mediadirectory;
    mediadirectory = cJSON_CreateObject();

    /* current directory */
    cJSON_AddStringToObject( mediadirectory, "curdirectory", pathname.c_str() );

    /* files in the current directory */
    if( !files.empty()) {
        cJSON *Json_files;
        Json_files = cJSON_CreateArray();

        vector<string>::iterator ite;
        for( ite = files.begin(); ite != files.end(); ++ite ) {
            cJSON * Json_file = cJSON_CreateObject();
            cJSON_AddStringToObject(Json_file, "name", (*ite).c_str());
            cJSON_AddItemToArray( Json_files, Json_file );
        }

        cJSON_AddItemToObject( mediadirectory, "files", Json_files );
    }

    /* directories in the current directory */
    if( !directories.empty()) {
        cJSON *Json_directories;
        Json_directories = cJSON_CreateArray();

        vector<string>::iterator ite;
        for( ite = directories.begin(); ite != directories.end(); ++ite ) {
            cJSON * Json_directory = cJSON_CreateObject();
            cJSON_AddStringToObject(Json_directory, "name", (*ite).c_str());
            cJSON_AddItemToArray( Json_directories, Json_directory );
        }

        cJSON_AddItemToObject( mediadirectory, "directories", Json_directories );
    }

    /* print out */
    char *outJson;
    outJson = cJSON_Print( mediadirectory );
    content.append(outJson);
    free(outJson);
    cJSON_Delete(mediadirectory);


    ESLog::ISLog("MEDIA BROWSER PATH END\n", __FILE__, __LINE__);

    return err;
}


/* tackle /posts path */
int PathDispatcher::PostsPath( HttpRequest &request, HttpResponse &response, string & content ){
    int err = 0;

    /* post object */
    vector<Post *> posts;

    /* data access object */
    BgDao bgDao;

    /* init */
    err = bgDao.Init();
    if( err ) {
        ESLog::ISLog("bg Dao init error!\n", __FILE__, __LINE__);
        return err;
    }

    /* get parameter guid of post */
    //string & postguid = request.GetUri().GetQueryValuebyName("guid");
    //ESLog::ISLog("guid=%s\n", __FILE__, __LINE__, postguid.c_str());

    /* Get the Object post */
    //err = bgDao.GetPostByGUID( postguid.c_str(), (*post) );
    err = bgDao.GetAllPosts( posts );
    if( err ) {
        return err;
    }
        
    /* json output */
    err = this->OutJsonFormat(&posts, NULL, NULL,content);
    if( err ) {
        ESLog::ISLog("out put Json errors !\n", __FILE__, __LINE__);
        return err;
    }

    ESLog::ISLog("end:%s end\n",__FILE__, __LINE__,  content.c_str());
    
    return err;
}


/* tackle /post path */
int PathDispatcher::PostPath( HttpRequest &request, HttpResponse &response, string & content ){
    int err = 0;

    /* post object */
    Post *post = new Post();
    vector<Post *> posts;

    /* data access object */
    BgDao bgDao;

    /* init */
    err = bgDao.Init();
    if( err ) {
        ESLog::ISLog("bg Dao init error!\n", __FILE__, __LINE__);
        return err;
    }

    /* get parameter guid of post */
    string & postguid = request.GetUri().GetQueryValuebyName("guid");
    ESLog::ISLog("guid=%s\n", __FILE__, __LINE__, postguid.c_str());

    /* Get the Object post */
    err = bgDao.GetPostByGUID( postguid.c_str(), (*post) );
    if( err ) {
        delete post;
        return err;
    }
    posts.push_back(post);
        
    /* json output */
    err = this->OutJsonFormat(&posts, NULL, NULL,content);
    if( err ) {
        ESLog::ISLog("out put Json errors !\n", __FILE__, __LINE__);
        return err;
    }

    ESLog::ISLog("end:%s end\n",__FILE__, __LINE__,  content.c_str());
    
    return err;
}

/* tackle /comments path */
int PathDispatcher::PostCategoryPath( HttpRequest &request, HttpResponse &response, string & content ){

    int err = 0;

    /* post object */
    vector<Post *> posts;

    /* data access object */
    BgDao bgDao;

    /* init */
    err = bgDao.Init();
    if( err ) {
        ESLog::ISLog("bg Dao init error!\n", __FILE__, __LINE__);
        return err;
    }

    /* get parameter guid of post */
    string & category = request.GetUri().GetQueryValuebyName("categroy");
    ESLog::ISLog("categroy=%s\n", __FILE__, __LINE__, category.c_str());

    /* Get the Object post */
    err = bgDao.GetPostsByCategory( category.c_str(), posts );
    if( err ) {
        return err;
    }

    /* json output */
    err = this->OutJsonFormat(&posts, NULL, NULL,content);
    if( err ) {
        ESLog::ISLog("out put Json errors !\n", __FILE__, __LINE__);
        return err;
    }


    ESLog::ISLog("end:%s end\n", __FILE__, __LINE__, content.c_str());
    
    return err;
}


/* tackle /post path */
int PathDispatcher::CategoriesPath( HttpRequest &request, HttpResponse &response, string & content ){
    int err = 0;

    /* category object */
    vector<Category *> categories;

    /* data access object */
    BgDao bgDao;

    /* init */
    err = bgDao.Init();
    if( err ) {
        ESLog::ISLog("bg Dao init error!\n", __FILE__, __LINE__);
        return err;
    }

    /* Get the Object post */
    err = bgDao.GetAllCategories(categories);
    if( err ) {
        return err;
    }
     
    /* json output */
    err = this->OutJsonFormat(NULL, NULL, &categories,content);
    if( err ) {
        ESLog::ISLog("out put Json errors !\n", __FILE__, __LINE__);
        return err;
    }


    ESLog::ISLog("end:%s end\n",__FILE__, __LINE__,  content.c_str());
    
    return err;

}

/* tackle /comments path */
int PathDispatcher::CommentsPath( HttpRequest &request, HttpResponse &response, string & content ){
    int err = 0;

    /* category object */
    vector<Comment *> comments;

    /* data access object */
    BgDao bgDao;

    /* init */
    err = bgDao.Init();
    if( err ) {
        ESLog::ISLog("bg Dao init error!\n", __FILE__, __LINE__);
        return err;
    }

    /* get parameter guid of post */
    string & postid = request.GetUri().GetQueryValuebyName("postid");
    ESLog::ISLog("postid=%s\n", __FILE__, __LINE__, postid.c_str());


    /* Get the Object post */
    err = bgDao.GetCommentsByPostID(comments, atoi(postid.c_str()) );
    if( err ) {
        return err;
    }
     
    /* json output */
    err = this->OutJsonFormat(NULL, &comments, NULL,content);
    if( err ) {
        ESLog::ISLog("out put Json errors !\n", __FILE__, __LINE__);
        return err;
    }

    ESLog::ISLog("end:%s end\n", __FILE__, __LINE__, content.c_str());
    
    return err;
}

/* out Json string formart */
int PathDispatcher::OutJsonFormat( vector<Post *> * posts, vector<Comment *> * comments, vector<Category *> *categories, string &content ){

    ESLog::ISLog(" OUT JSON FORMAT START \n", __FILE__, __LINE__);

    /* err */
    int err = 0;

    /* char */
    char *outJson;

    /* out put cJson */
    cJSON *page;
    page = cJSON_CreateObject();

    ESLog::ISLog(" Posts Json:\n", __FILE__, __LINE__);

    /* posts */
    if( posts != NULL ) {
        cJSON *Json_posts;
        Json_posts = cJSON_CreateArray();

        vector<Post *>::iterator ite;
        for( ite = posts->begin(); ite != posts->end(); ++ite ) {
            cJSON * Json_post = (*ite)->OutJsonObjectFormat();
            ESLog::ISLog("Add to array start\n", __FILE__, __LINE__);

            cJSON_AddItemToArray( Json_posts, Json_post );

            ESLog::ISLog("Add to array end\n", __FILE__, __LINE__);
            //cJSON_Delete( Json_post ); /* free */
            delete (*ite); /* free object */
        }

        /* add into page */
        ESLog::ISLog("add\n", __FILE__, __LINE__);
        cJSON_AddItemToObject( page, "posts", Json_posts );
        //printf("delete\n");
        //cJSON_Delete(Json_posts);
    }

    ESLog::ISLog(" Comments Json:\n", __FILE__, __LINE__);
    /* comments */
    if( comments != NULL ) {
        /* comments */
        cJSON *Json_comments;
        Json_comments = cJSON_CreateArray();
        vector<Comment *>::iterator ite;
        for( ite = comments->begin(); ite != comments->end(); ++ite ) {
            cJSON * Json_comment = (*ite)->OutJsonObjectFormat();
            cJSON_AddItemToArray( Json_comments, Json_comment );
            //cJSON_Delete( Json_comment ); /* free */
            delete (*ite); /* free object */
        }

        cJSON_AddItemToObject( page, "comments", Json_comments );
        //cJSON_Delete(Json_comments);
    }

    ESLog::ISLog(" Categoies Json:\n", __FILE__, __LINE__);
    /* categories */
    if( categories != NULL ) {
        /* categories */
        cJSON *Json_categories;
        Json_categories = cJSON_CreateArray();
        vector<Category *>::iterator ite;
        for( ite = categories->begin(); ite != categories->end(); ++ite ) {
            cJSON * Json_category = (*ite)->OutJsonObjectFormat();
            cJSON_AddItemToArray( Json_categories, Json_category );
            //cJSON_Delete( Json_category ); /* free */
            delete (*ite); /* free object */
        }

        cJSON_AddItemToObject( page, "categories", Json_categories);
        //cJSON_Delete(Json_categories);
    }

    /* print out */
    outJson = cJSON_Print( page );
    content.append(outJson);
    free(outJson);

    cJSON_Delete(page);

    ESLog::ISLog(" OUT JSON FORMAT END\n", __FILE__, __LINE__);
    
    return err;
}
