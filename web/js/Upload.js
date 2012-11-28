/**
 * @author kzhiquan
 */



/* constructor for upload resource */
function Upload(){
  
}


/* prototype for upload */
Upload.prototype = {
    
    /* constuctor */
    constructor : Upload,
    
    /* upload media */
    uploadMedia : function(){
        console.log("upload media start!\n");
        
        /* get the file */
        var inputfile = document.getElementById("inputfile");
        //var file = files[0];
        
        var file = inputfile.files[0];
        console.log(file);
        console.log(file.size);
        console.log(file.name);
        
        /* try sending */
        var reader = new FileReader();
        
        /* set load start event */
        reader.onloadstart = function(){
            console.log("load start start\n");
            document.getElementById("bytesTotal").textContent = file.size;
            console.log("load start end\n");
        }
        
        /* reader progress event */
        reader.onprogress = function(p){
            console.log("progress start!\n");
            document.getElementById("bytesRead").textContent = p.loaded;
            console.log("progress end!\n");
        }
        
        /* reader sucess event */
        reader.onload = function(){
            console.log("load complete start!\n");
            console.log("load complete end\n");
        }
        
        /* reader finished event , sucess or failed */
        reader.onloadend = function(){
            if( reader.error ){
                console.log(reader.error);
            }
            else{
                
                /* base object */
                var eyeBase = new EyeBase();
                
                /* communication */
                var xhr = eyeBase.createXHR();
                
                var uploadpath = document.getElementById("uploadpath");
                
                /* create an connection */
                var url = "/upload";
                url = eyeBase.addURLParam(url, "filename", file.name );
                url = eyeBase.addURLParam(url, "uploadpath", uploadpath.value );
                console.log(url);
                xhr.open("post", url);
                
                /* type */
                //xhr.overrideMimeType("application/octet-stream");
                xhr.sendAsBinary(reader.result);
                //xhr.send(reader.result);
                
                /* send over handle */
                eyeBase.setXHRParseRData(xhr, Upload.prototype.parseRData);
                
            }
            
        }
        
        /* read data from file */
        reader.readAsBinaryString(file); 
        //reader.readAsText(file);
        //reader.read       
        console.log("upload media end\n");
    },
    
    /* parse server return data */
    parseRData : function( serverData ){
        console.log("browser page parseRData start");
        
        alert(serverData);
        
        console.log("browser page parseRData end");
    }
    
}


if (window.File && window.FileReader && window.FileList && window.Blob) {
} else {
  alert('这个浏览器没有完全支持此API.');
}


/* event */
var eyeBase = new EyeBase();
var upload = new Upload();
var mediabtn = document.getElementById("fileupload");

eyeBase.addEventHandler(mediabtn, "click", upload.uploadMedia );
