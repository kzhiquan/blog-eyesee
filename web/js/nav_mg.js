
/* this js control the nav load different page html
 * into main manager page.
 */


/* load different page to the main manager page iframe
 * according to the obj parameters 
 */
function loadNavPage(obj){
    console.log("load page start");
    
    var textNode = obj.firstChild;
    console.log(textNode.nodeValue);
    
    var parentTextNode = (((obj.parentElement).parentElement).parentElement).children[0];
    console.log(parentTextNode.firstChild.nodeValue);
    
    var iframe = document.getElementById("content").children[0];
    
    var boardpath = document.getElementById("boardpath");
    boardpath.textContent = parentTextNode.firstChild.nodeValue + "->" + textNode.nodeValue;
    
    /*post nav menu */
    if( parentTextNode.firstChild.nodeValue == "post"){
        
        if( textNode.nodeValue == "all"){
            console.log("post->all");
            console.log(iframe.getAttribute("src"));
            iframe.setAttribute("src", "allpost_mg.html");
        }
        else if( textNode.nodeValue == "new"){
            console.log("post->new");
            console.log(iframe.getAttribute("src"));
            iframe.setAttribute("src", "newpost_mg.html");
        }
        else if( textNode.nodeValue == "categories"){
            console.log("post->categories");
            console.log(iframe.getAttribute("src"));
            iframe.setAttribute("src", "categorypost_mg.html");
        }
        else if( textNode.nodeValue == "tag"){
            console.log("post->tag");
            console.log(iframe.getAttribute("src"));
            iframe.setAttribute("src", "tagpost_mg.html");
        }
        else{
            console.log("the nav is error!");
        }
    }
    else if( parentTextNode.firstChild.nodeValue == "media"){
        if( textNode.nodeValue == "all"){
            console.log("media->all");
            console.log(iframe.getAttribute("src"));
            iframe.setAttribute("src", "allmedia_mg.html");
        }
        else if( textNode.nodeValue == "new"){
            console.log("media->new");
            console.log(iframe.getAttribute("src"));
            iframe.setAttribute("src", "newmedia_mg.html");
        }
        else{
            console.log("the nav is error!");
        }
    }
   
    console.log("load page end");
}
