/**
 * @author kzhiquan
 */

/* constructor */
function Comment(){
    this.postguid = null;
    this.commname = null;
    this.commemail = null;
    this.commsite = null;
    this.commcontent = null;
}

/* prototype for Post */
Comment.prototype = {
    /* constructor */
    constructor : Comment,
    
    /* init */
    init : function( xmlPost ){
    },
    
    /* set postguid */
    setPostguid: function( guid ){
        this.postguid = guid;
    },
    
    /* set commname */
    setName: function( name ){
        this.commname = name;
    },
    
    /* set commemail */
    setEmail : function( email ){
        this.commemail = email;
    },
    
    /* set commsite */
    setSite : function( site ){
        this.commsite = site;
    },
    
    /* set content */
    setContent : function( content ){
        this.commcontent = content;
    }
}