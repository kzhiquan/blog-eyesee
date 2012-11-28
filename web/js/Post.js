/**
 * @author kzhiquan
 * 
 * Post object 
 */

/* constructor */
function Post(){
    this.postid = null;
    this.guid = null;
    this.title = null;
    this.post_date = null;
    this.comm_count = null;
    this.category = null;
    this.excerpt = null;
    this.content = null;
    this.visible = null;
}

/* prototype for Post */
Post.prototype = {
    /* constructor */
    constructor : Post,
    
    /* init */
    init : function( xmlPost ){
    },
    
    /* set posttitle */
    setTitle: function( title ){
        this.title = title;
    },
    
    /* set posttitle */
    setGuid: function( guid ){
        this.guid = guid;
    },
    
    /* set postcategory */
    setCategory : function( category ){
        this.category = category;
    },
    
    /* set visible */
    setVisible : function( visible ){
        this.visible = visible;
    },
    
    /* set content */
    setContent : function( content ){
        this.content = content;
    }
}


