
/* contructor */
function AllPost(){
    
}


/* prototype for Browser */
AllPost.prototype = {
    
    /* constructor */
    constructor : AllPost,
    
    /* load browser.hmtl page data */
    loadAllPostPage : function(){
        console.log("load browser page start");
            
        /* create server object */
        var eyeBase = new EyeBase();
        var xhr = eyeBase.createXHR();
        
        /* set the hook function */
        eyeBase.setXHRParseRData( xhr, AllPost.prototype.parseRData);
        
        
        /* create an connection */
        var url = "/posts";
        console.log(url);
        xhr.open("get", url);
        
        /* send request */
        xhr.send(null);
    
        console.log("load browser page end!\n");
    },
    
    /* parse server return data */
    parseRData : function( serverData ){
        console.log("browser page parseRData start");
        
        alert(serverData);
        
        var postpage = JSON.parse(serverData);
        var posts = postpage.posts;
        
        postshtml = "";
        /* posts parse */
        for( var i = 0; i < posts.length; i++ ){
            
            /* post */
            var post = "<tr><td><a href='newpost_mg.html#" + posts[i].guid + "'>" + posts[i].title + "</a></td>";
            post += "<td>kzhiquan</td>";
            post += "<td>" + posts[i].category + "</td>";
            post += "<td>undefine</td>";
            post += "<td>"+ posts[i].commcount + "</td>";
            post += "<td>" + posts[i].postdate + "</td>";
            
            postshtml += post;   
        }
        
        document.getElementById("allposts").innerHTML = postshtml;
 
        
        console.log("browser page parseRData end");
    }
    
}

var eyeBase = new EyeBase();
eyeBase.addEventHandler( window, "load", AllPost.prototype.loadAllPostPage);