

/* constructor */
function NewPost(){
    
}

/* prototype for the NewPost */
NewPost.prototype = {
    /* constructor */
    constructor : NewPost,
    
    /* post publish */
    PostPublish : function(){
        console.log("Post Publish Start!\n");
        
        /* post object */
        var post = new Post();
        
        /* post title */
        var posttitle = document.getElementById("content_title");
        post.setTitle(posttitle.value);
       
        /* visible */
        var postvisibleselect = document.getElementById("postvisibility");
        var postvisible = postvisibleselect.options[postvisibleselect.selectedIndex];
        post.setVisible(postvisible.value);
       
        /* category */
        var postcategoryselect = document.getElementById("postcategory");
        var postcategory = postcategoryselect.options[postcategoryselect.selectedIndex];
        post.setCategory(postcategory.value);
       
        /* post content */
        //var postcontent = document.getElementById("content_utitlty");
        post.setContent(frames["richedit"].document.body.innerHTML);
        
        /* create server object */
        var eyeBase = new EyeBase();
        var xhr = eyeBase.createXHR();
        eyeBase.setXHRParseRData(xhr, function(servData){alert(servData);});
        
        /* create an connection */
        var url = "/newpostpage";
        url = eyeBase.addURLParam(url, "action", "publish");
        console.log(url);
        xhr.open("post", url);
      
        var postjsonstr = JSON.stringify(post);
        /* send request */
        xhr.send(postjsonstr);
        
        console.log("Post Publish end!\n");
    },
    
    /* post publish */
    PostModify : function(){
        console.log("Modify Publish Start!\n");
        
        /* post object */
        var post = new Post();
        
        /* post title */
        var posttitle = document.getElementById("content_title");
        post.setTitle(posttitle.value);
       
        /* visible */
        var postvisibleselect = document.getElementById("postvisibility");
        var postvisible = postvisibleselect.options[postvisibleselect.selectedIndex];
        post.setVisible(postvisible.value);
       
        /* category */
        var postcategoryselect = document.getElementById("postcategory");
        var postcategory = postcategoryselect.options[postcategoryselect.selectedIndex];
        post.setCategory(postcategory.value);
       
        /* post content */
        //var postcontent = document.getElementById("content_utitlty");
        post.setContent(frames["richedit"].document.body.innerHTML);
        
        /* post guid */
        var postguid = document.getElementById("postguid");
        post.setGuid(postguid.textContent);
        
        /* create server object */
        var eyeBase = new EyeBase();
        var xhr = eyeBase.createXHR();
        eyeBase.setXHRParseRData(xhr, function(servData){alert(servData);});
        
        /* create an connection */
        var url = "/modifypostpage";
        url = eyeBase.addURLParam(url, "action", "modify");
        console.log(url);
        xhr.open("post", url);
      
        var postjsonstr = JSON.stringify(post);
        /* send request */
        xhr.send(postjsonstr);
        
        console.log("Post Publish end!\n");
    },
    
    /* load modify post */
    loadModifyPost : function(){
        console.log("load modify page start");
    
        /* parser parameter of page page */
        var url = document.URL;
        console.log(url);
        var urlvalues = url.split("#");
        var urlvalue = urlvalues[1];
        if( urlvalue == ""){
            return;
        }
        
        /* post guid */
        var postguid = document.getElementById("postguid");
        postguid.textContent = urlvalue;
        
        /* create server object */
        var eyeBase = new EyeBase();
        var xhr = eyeBase.createXHR();
        
        /* set the hook function */
        eyeBase.setXHRParseRData( xhr, NewPost.prototype.modifyPostInit);
        
        
        /* create an connection */
        var url = "/postpage";
        url = eyeBase.addURLParam(url, "guid", urlvalue);
        console.log(url);
        xhr.open("get", url);
        
        /* send request */
        xhr.send(null);
    
        console.log("load modify page end!\n");
        
    },
    
    /* init modify post */
    modifyPostInit : function(serverData){
        console.log("modify post init start!\n");
        
        alert(serverData);
        
        var postpage = JSON.parse(serverData);
        
        /* title */
        var title = document.getElementById("content_title");
        title.value = postpage.posts[0].title;
        
        /* content */
        frames["richedit"].document.body.innerHTML = postpage.posts[0].content;
        
        var categoryselect = document.getElementById("postcategory");
        /* category */
       
        /*postpage.posts[0].category*/
        for( var i = 0, len = categoryselect.options.length; i < len; i++){
            option = categoryselect.options[i];
            if( option.value == postpage.posts[0].category ){
                option.selected = true;
                break;
            }
        }
        
        
        
        console.log("modify post init end\n");
    },
    
    
    /* image upload */
    InsertImg : function(){
        var insertimglight = document.getElementById("insertimglight");
        insertimglight.style.display = 'block';
        
        var insertimgfade = document.getElementById("insertimgfade");
        insertimgfade.style.display = 'block';
        
        /* base object */
        var eyeBase = new EyeBase();
        var xhr = eyeBase.createXHR();
        
        /* create an connection */
        var url = "/mediabrowser";
        url = eyeBase.addURLParam(url, "path", "upload");
        console.log(url);
        xhr.open("get", url);
        
        /* send request */
        xhr.send(null);
        
        eyeBase.setXHRParseRData(xhr, NewPost.prototype.parseRData);
        
        var insertimgok = document.getElementById("insertimgok");
        eyeBase.addEventHandler(insertimgok, "click", NewPost.prototype.insertimgokfunc);

        var insertimgcancel = document.getElementById("insertimgcancel");
        eyeBase.addEventHandler(insertimgcancel, "click", NewPost.prototype.insertimgcancelfunc);
           
    },
    
    /* event ok */
    insertimgokfunc : function(){
        document.getElementById("insertimglight").style.display = 'none';
        document.getElementById("insertimgfade").style.display = 'none';
            
        /* post rich textarea */
        var postiframe = frames["richedit"];
            
        /* the select img */
        var imgselected = document.getElementById("imgminibrowser");
        postiframe.document.execCommand("insertimage", false, imgselected.getAttribute("src"));
              
     },
     
     /* event cancel */
    insertimgcancelfunc : function(){
        document.getElementById("insertimglight").style.display = 'none';
        document.getElementById("insertimgfade").style.display = 'none';
    },
    
    /* parse the postpage server return data */
    parseRData : function( serverData ){
        console.log("path browser start\n");
        //alert(serverData);
        var imgfilebrowser = document.getElementById("imgfilebrowser");
            
        /* get the object */
        var docinfo = JSON.parse(serverData);
            
        var curpath = docinfo.curdirectory;
        var directories = docinfo.directories;
        var files = docinfo.files;
            
        /* curpath */
        var imgfilebrowsertext = "<span id='imgfilebrowsercurnode'>"+curpath+"</span>";
        imgfilebrowsertext += "<ul class='imgfilebrowsertree'>";
            
        /* directories */
        if(directories != undefined ){
            for( var i = 0; i < directories.length; i++ ){
                imgfilebrowsertext +="<li><a href='#' onclick='NewPost.prototype.directoryChange(this)'>"
                    +directories[i].name +"</a></li>";

            }
        }
  
        /* files */
        if( files != undefined ){
            for( var i = 0; i < files.length; i++ ){
                imgfilebrowsertext +="<li><a href='#' onclick='NewPost.prototype.imgBrowser(this)'>"
                    +files[i].name +"</a></li>";

            }
        }
 
        imgfilebrowsertext += "</div>";
        imgfilebrowser.innerHTML = imgfilebrowsertext;
        console.log("path browser end\n"); 
    },
    
    /* change directories */
    directoryChange: function( obj ){
        console.log("directoryChange start\n");
            
        var curnode = document.getElementById("imgfilebrowsercurnode");
    
        var path = curnode.textContent;
    
        if( obj.textContent != ".."){
           path = path + "/" + obj.textContent; 
        }
        else{
           //path.lastIndexOf("/", 0);
           console.log(path.lastIndexOf("/"));
           if(path.lastIndexOf("/") == -1 ){
               path = "..";
               return;
           }
           else{
               path = path.slice(0, path.lastIndexOf("/"));
           }
           
        }
       
        console.log(path);
    
        /* base object */
        var eyeBase = new EyeBase();
        var xhr = eyeBase.createXHR();
        
        /* create an connection */
        var url = "/mediabrowser";
        url = eyeBase.addURLParam(url, "path", path);
        console.log(url);
        xhr.open("get", url);
        
        /* servdata parse in the imgbrowser */
        eyeBase.setXHRParseRData(xhr, NewPost.prototype.parseRData);
        
        /* send request */
        xhr.send(null);
    
        console.log("directoryChange End\n"); 
    },
    
    /* img browser */
    imgBrowser : function( obj ){
        console.log("img browser start\n");
        var img = document.getElementById("imgminibrowser");
    
        var curnode = document.getElementById("imgfilebrowsercurnode");
        var imgpath = "../" + curnode.textContent + "/" + obj.textContent;
        console.log(imgpath);
   
        img.setAttribute("src", imgpath);
        console.log("img browser end");
    },
    
    /* insert link  */
    InsertLink : function(){
        var insertlinklight = document.getElementById("insertlinklight");
        insertlinklight.style.display = 'block';
        
        var insertlinkfade = document.getElementById("insertlinkfade");
        insertlinkfade.style.display = 'block';
        
   
        var insertlinkok = document.getElementById("insertlinkok");

        eyeBase.addEventHandler(insertlinkok, "click", NewPost.prototype.insertlinkokfunc);
        
        var insertlinkcancel = document.getElementById("insertlinkcancel");

        eyeBase.addEventHandler(insertlinkcancel, "click", NewPost.prototype.insertlinkcancelfunc);
    },
    
    /* event ok */
    insertlinkokfunc : function (){
        document.getElementById("insertlinklight").style.display = 'none';
        document.getElementById("insertlinkfade").style.display = 'none';
            
        /* post rich textarea */
        var postiframe = frames["richedit"];
            
        /* insert link href */
        var linkhref = document.getElementById("insertlinkhref");
            
        postiframe.document.execCommand("createlink", false, linkhref.value);
    },
    
    /* event cancel */
    insertlinkcancelfunc : function (){
        document.getElementById("insertlinklight").style.display = 'none';
        document.getElementById("insertlinkfade").style.display = 'none';
    }
    
}


/* windows envirentment */
/* register load event */
var eyeBase = new EyeBase();
var newPost = new NewPost();

/* publish button click event */
var publishbtn = document.getElementById("postpublish");
eyeBase.addEventHandler( publishbtn, "click", newPost.PostPublish);


/* publish button click event */
var modifybtn = document.getElementById("postmodify");
eyeBase.addEventHandler( modifybtn, "click", NewPost.prototype.PostModify);


/* add img button click event */
var imgbtn = document.getElementById("edit_tool_img");
eyeBase.addEventHandler( imgbtn, "click", newPost.InsertImg);

/* add img button click event */
var linkbtn = document.getElementById("edit_tool_link");
eyeBase.addEventHandler( linkbtn, "click", newPost.InsertLink);

/* load modify post */
eyeBase.addEventHandler( window, "load", NewPost.prototype.loadModifyPost);




    
