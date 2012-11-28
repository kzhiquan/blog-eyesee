
/* Global object */

/* constructor */
function EyeBase(){
    
}



/* prototype for Global */
EyeBase.prototype = {
   
   /* constructor */
   constructor: EyeBase,
    
    /* create XHR */
   createXHR : function(){
       
       if( typeof XMLHttpRequest != "undefined" ){
           return new XMLHttpRequest();
       }
       else if( typeof ActiveXObject != "undefined" ){
           
           if( typeof arguments.callee.activeXString != "string" ){
               var versions = ["MSXML2.XMLHttp.6.0", "MSXML2.XMLHttp.3.0",
                                "MSXML2.XMLHttp"];
                                
               for( var i=0, len=versions.length; i < len; i++ ){
                   try{
                       var xhr = new ActiveXObject(versions[i]);
                       arguments.callee.activeXString  = versions[i];
                       return xhr;
                   }catch(ex){}
               }
           }
           
           return new ActiveXObject(arguments.callee.activeXString);
       }
       else{
           throw new Error("No XHR object available");
       }
       
   },
   
   /* set xhr state event function */
   setXHRParseRData : function( xhr, hookfun ){
       xhr.parseRData = hookfun;
       xhr.onreadystatechange = function(){
           if( xhr.readyState == 4 ){
               if( (xhr.status >= 200 && xhr.status < 300) || xhr.status == 304 ){
                   //console.log(xhr.responseText);
                   xhr.parseRData(xhr.responseText);
               }
               else{
                   alert("Request was unsuccessful:" + xhr.status );
               }
           }
       }
   },
   
   /* add url parameter of xhr */
   addURLParam : function(url, name, value){
       url += (url.indexOf("?") == -1 ? "?" : "&");
       //url += window.encodeURIComponent(name) + "=" + window.encodeURIComponent(value);
       url += name + "=" + value;
       return url;
   },
   
   /* add event handler */
   addEventHandler : function( element, type, handler ){
       if( element.addEventListener ){// DOM2
           element.addEventListener( type, handler, false );
       }
       else if( element.atatchEvent ){//IE
           element.atatchEvent( "on" + type, handler );
       }
       else{
           element["on" + type ] = handler;
       } 
   },
   
   /* remove event handler */
   removeEventHandler: function( element, type, handler ){
       if( element.removeEventListener ){// DOM2
           element.removeEventListener( type, handler, false );
       }
       else if( element.detachEvent ){//IE
           element.detachEvent( "on" + type, handler );
       }
       else{
           element["on" + type ] = null;
       } 
   }
}
