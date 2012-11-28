

/* constructor */
function PostBW(){

}

/* prototype for PostBW */
PostBW.prototype = {
    
    /* constuctor */
    constructor : PostBW,
    
    /* load post page */
    loadPostPage : function(){
        /*console.log("load page start");*/
    
        /* parser parameter of page page */
        var url = document.URL;
        /*console.log(url);*/
        var urlvalues = url.split("#");
        var urlvalue = urlvalues[1];
        
        /* create server object */
        var eyeBase = new EyeBase();
        var xhr = eyeBase.createXHR();
        
        /* set the hook function */
        eyeBase.setXHRParseRData( xhr, PostBW.prototype.parseRData);
        
        
        /* create an connection */
        var url = "/postpage";
        url = eyeBase.addURLParam(url, "guid", urlvalue);
        //console.log(url);
        xhr.open("get", url);
        
        /* send request */
        xhr.send(null);
        
        /*console.log("load page end!\n");*/
    },
    
    /* parse the postpage server return data */
    parseRData : function( serverData ){
        //console.log("parse Response data start\n");
        
        //alert("serverdata:"+serverData);
        
        /* postpage object */
        var postpage = JSON.parse(serverData);
        var posts = postpage.posts;
        var comments = postpage.comments;
           
        posthtml = "";
        /* posts parse */
        for( var i = 0; i < posts.length; i++ ){
            /* post */
            var post = "<div class='post'><div class='post_title'>";
            post += "<a href='post_bw.html#" + posts[i].guid +"'>";
            post += "<span class='post_title_text'>" + posts[i].title + "</span></a>";
            post += "<div class='post_title_detail'>";
            post += "<span class='date'>   发表： " + posts[i].postdate + "/</span>";
            post += "<span class='post_title_cnt'>    评论:  " + posts[i].commcount + "/</span>";
            post += "<span class='post_title_category'>    分类：  " + posts[i].category + "</span>";
            post += "</div></div>";
            
            post += "<div class='post_content'>" + posts[i].content + "</div></div>";
            
            posthtml += post;   
        }
        document.getElementById("content").innerHTML = posthtml;
        
        commenthtml = "";
        /* comments parse */
        for( var i = 0; i < comments.length; i++ ){
            /* comment */
            var comment = "<div class='post_comment'><div class='post_comment_author'>";
            comment += "<div class='post_comment_name'><a href='" + comments[i].commauthorurl;
            comment += "'>作者：  " + comments[i].commauthor + "</a></div>";
            comment += "<div class='post_comment_date'>评论：  " + comments[i].commdate + "</div></div>";
            comment += "<div class='post_comment_content'>" + comments[i].commcontent + "</div></div>";
            
            commenthtml += comment;
        }
        document.getElementById("comment_his").innerHTML = commenthtml;
        
        
        //PostBW.prototype.loadPostBiref();
        
        
        //console.log("parse Response data end\n"); 
    },

    loadPostBiref : function(){
        //console.log("load post biref start");
            
        /* create server object */
        var eyeBase = new EyeBase();
        var xhr = eyeBase.createXHR();
        
        /* set the hook function */
        eyeBase.setXHRParseRData( xhr, PostBW.prototype.postBirefRData);
        
        
        /* create an connection */
        var url = "/posts";
        //console.log(url);
        xhr.open("get", url);
        
        /* send request */
        xhr.send(null);
    
        //console.log("load post brief end!\n");     
    },
    
    postBirefRData : function(serverData){
        //console.log("browser page post biref parseRData start");
        
        //alert(serverData);
        
        var postbriefpage = JSON.parse(serverData);
        
        var postsbrief = postbriefpage.posts;
        
        var postbriefhtml = "<div class='sidebar_content_title'>文章列表</div><ul>";
        
        var lastpostdate = "";
        
        for( var i = 0; i < postsbrief.length; i++ ){
            
            var postbrief = "";
            if( postsbrief[i].postdate.substring(0, 7) != lastpostdate ){
                lastpostdate = postsbrief[i].postdate.substring(0, 7);
                
                if( i != 0 ){
                    postbriefhtml += "</ul></li>";
                }
                
                postbrief += "<li><span>"  + lastpostdate + "</span><ul>";
            }
            
            postbrief += "<li><a href='post_bw.html#" + postsbrief[i].guid +"'>";
            postbrief += postsbrief[i].postdate.substring(8, 10) + " " + postsbrief[i].postdate.substring(11, 13);
            postbrief += "-";
            
            postbrief += postsbrief[i].title + "</a>";
            
            
            postbriefhtml += postbrief;
        }
        
        postbriefhtml += "</ul>";
        
        document.getElementById("postsbrief").innerHTML = postbriefhtml;
        
        
        //console.log("browser page post biref parseRData end");  
    },
        
    /* submit comment */
    submitComm : function(){
        //console.log("submit comment start\n");
        
        /* comment name */
        var commname = document.getElementById("comment_cur_name");   
        /* comment email */
        var commemail = document.getElementById("comment_cur_email");
        /* comment site */
        var commsite = document.getElementById("comment_cur_site");
        /* comment content */
        var commcontent = document.getElementById("comment_content");
        /* parser parameter of page page */
        var url = document.URL;
        var urlvalues = url.split("#");
        var urlvalue = urlvalues[1];
        
        /* comment object */
        var comm = new Comment();
        comm.setPostguid(urlvalue);
        comm.setName(commname.value);
        commname.value = "";
        comm.setEmail(commemail.value);
        commemail.value = "";
        comm.setSite(commsite.value);
        commsite.value = "";
        comm.setContent(commcontent.value);
        commcontent.value = "";
        var commjson = JSON.stringify(comm);
        
        /* create server object */
        var eyeBase = new EyeBase();
        var xhr = eyeBase.createXHR();
        eyeBase.setXHRParseRData(xhr, PostBW.prototype.addComment);
        
        /* create an connection */
        var url = "/newcommentpage";
        url = eyeBase.addURLParam(url, "action", "comment");
        //console.log(url);
        xhr.open("post", url);
      
        /* send request */
        xhr.send(commjson);
        //console.log("submit comment end\n");
    },
    
    /* submit comment return */
    addComment : function(serverData){
        //alert("addComment:" + serverData);
        PostBW.prototype.loadPostPage();
    }
}


/* windows envirentment */
/* register load event */
var eyeBase = new EyeBase();
var postBW = new PostBW();
eyeBase.addEventHandler( window, "hashchange", postBW.loadPostPage);
eyeBase.addEventHandler( window, "hashchange", postBW.loadPostBiref);


eyeBase.addEventHandler( window, "load", postBW.loadPostPage);
eyeBase.addEventHandler( window, "load", postBW.loadPostBiref);


/* comment submit */
var commsubmitbtn = document.getElementById("comment_content_submit");
eyeBase.addEventHandler( commsubmitbtn, "click", postBW.submitComm);
