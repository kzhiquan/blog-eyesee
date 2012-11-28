
/* contructor */
function Browser(){
    
}


/* prototype for Browser */
Browser.prototype = {
    
    /* constructor */
    constructor : Browser,
    
    /* load browser.hmtl page data */
    loadBrowserPage : function(){
        //console.log("load browser page start");
            
        /* create server object */
        var eyeBase = new EyeBase();
        var xhr = eyeBase.createXHR();
        
        /* set the hook function */
        eyeBase.setXHRParseRData( xhr, Browser.prototype.parseRData);
        
        
        /* create an connection */
        var url = "/homepage";
        //console.log(url);
        xhr.open("get", url);
        
        /* send request */
        xhr.send(null);
    
        //console.log("load browser page end!\n");
    },
    
    /* parse server return data */
    parseRData : function( serverData ){
        //console.log("browser page parseRData start");
        
        //alert(serverData);
        
        /* homepage object */
        var homepage = JSON.parse(serverData);
        var posts = homepage.posts;
        var categories = homepage.categories;
        
        //var content = document.getElementById("content");
        
        var posthtml = "";
        /* posts parse */
        for( var i = 0; i < posts.length; i++ ){
            
            /* post */
            var post = "<div class='post'><div class='post_title'>";
            post += "<a href='html/post_bw.html#" + posts[i].guid +"'>";
            post += "<span class='post_title_text'>" + posts[i].title + "</span></a>";
            post += "<div class='post_title_detail'>";
            post += "<span class='date'>   发表： " + posts[i].postdate + "/</span>";
            post += "<span class='post_title_cnt'>    评论:  " + posts[i].commcount + "/</span>";
            post += "<span class='post_title_category'>    分类：  " + posts[i].category + "</span>";
            post += "</div></div>";
            
            /* excerpt */
            if( posts[i].content.length > 200 )
            {
                posts[i].content = posts[i].content.substring(0, 200);
                posts[i].content += "...";
            }
            post += "<div class='post_excerpt'>" + posts[i].content + "</div><p></p>";
            post += "<a  id='postcontinue' href='html/post_bw.html#" + posts[i].guid + "'>阅文...</a></div>";
            
            posthtml += post;   
        }
        
        document.getElementById("content").innerHTML = posthtml;
        
        //Browser.prototype.loadPostBiref();
        
        //console.log("browser page parseRData end");
    },
    
    loadPostBiref : function(){
        //console.log("load post biref start");
            
        /* create server object */
        var eyeBase = new EyeBase();
        var xhr = eyeBase.createXHR();
        
        /* set the hook function */
        eyeBase.setXHRParseRData( xhr, Browser.prototype.postBirefRData);
        
        
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
            
            postbrief += "<li><a href='html/post_bw.html#" + postsbrief[i].guid +"'>";
            postbrief += postsbrief[i].postdate.substring(8, 10) + " " + postsbrief[i].postdate.substring(11, 13);
            postbrief += "-";
            
            postbrief += postsbrief[i].title + "</a>";
            
            
            postbriefhtml += postbrief;
        }
        
        postbriefhtml += "</ul>";
        
        document.getElementById("postsbrief").innerHTML = postbriefhtml;
        
        
        //console.log("browser page post biref parseRData end");  
    }
    
}

var eyeBase = new EyeBase();
eyeBase.addEventHandler( window, "load", Browser.prototype.loadBrowserPage);
eyeBase.addEventHandler( window, "load", Browser.prototype.loadPostBiref);
