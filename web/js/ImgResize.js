
/* img resize object */
var xcResizer = {};

xcResizer.resize = function(id, left, top, dragPointColor, dragPointSize, opacity, isRepos, containerId,minWidth,minHeight) {
    /*参数分别为： 需要包装为可缩放元素的元素id、元素的左位置、元素的右位置、拖动点宽/高、拖动点颜色、缩放时的透明度(0~1)、缩放后是否自动回到原来位置*/
    var dragElem;
    var dragPointArr = [];
    var elem = document.getElementById(id);
    var container = document.getElementById(containerId);
    var type = elem.tagName;
    var style = elem.style;
    var leftX;
    var eleStyle;
    var isDrag = false;
    var marginX;
    var dragDistant;
    style.position = 'absolute';
    style.left = left + 'px';
    style.top = top + 'px';
    style.width = elem.clientWidth + 'px';
    style.height = elem.clientHeight + 'px';

    var leftPoint;
    var rightPoint;
    var topPoint;
    var bottomPoint;
    var rightbottomPoint;
    var leftbottomPoint;
    var righttopPoint;
    var lefttopPoint;

    function getAllPoints() {
        leftPoint = document.getElementById('left');
        rightPoint = document.getElementById('right');
        topPoint = document.getElementById('top');
        bottomPoint = document.getElementById('bottom');
        rightbottomPoint = document.getElementById('rightbottom');
        leftbottomPoint = document.getElementById('leftbottom');
        righttopPoint = document.getElementById('righttop');
        lefttopPoint = document.getElementById('lefttop');

    }

    function setDragPoint(pos) {
        var ele;
        if (!document.getElementById(pos))
            ele = document.createElement('div');
        else
            ele = document.getElementById(pos);
        dragPointArr.push(ele);
        eleStyle = ele.style;
        eleStyle.backgroundColor = dragPointColor;
        eleStyle.position = 'absolute';
        eleStyle.width = eleStyle.height = dragPointSize + 'px';

        switch (pos) {
            case 'left':
                {
                    eleStyle.cursor = 'w-resize';
                    eleStyle.left = parseInt(left) - parseInt(eleStyle.width) / 2 + container.offsetLeft + 'px';
                    eleStyle.top = parseInt(top) + (parseInt(style.height) - parseInt(eleStyle.height)) / 2 + container.offsetTop + 'px';
                    ele.id = pos;
                    setDragEvent(ele, pos);
                    break;
                }
            case 'right':
                {
                    eleStyle.cursor = 'e-resize';
                    eleStyle.left = parseInt(left) + parseInt(style.width) - parseInt(eleStyle.width) / 2 + container.offsetLeft + 'px';
                    eleStyle.top = parseInt(top) + (parseInt(style.height) - parseInt(eleStyle.height)) / 2 + container.offsetTop + 'px';
                    ele.id = pos;
                    setDragEvent(ele, pos);
                    break;
                }
            case 'top':
                {

                    eleStyle.cursor = 'n-resize';
                    eleStyle.left = parseInt(left) + (parseInt(style.width) - parseInt(eleStyle.width)) / 2 + container.offsetLeft + 'px';
                    eleStyle.top = parseInt(top) - parseInt(eleStyle.height) / 2 + container.offsetTop + 'px';
                    ele.id = pos;
                    setDragEvent(ele, pos);
                    break;
                }
            case 'bottom':
                {
                    eleStyle.cursor = 's-resize';
                    eleStyle.left = parseInt(left) + (parseInt(style.width) - parseInt(eleStyle.width)) / 2 + container.offsetLeft + 'px';
                    eleStyle.top = parseInt(top) + parseInt(style.height) - parseInt(eleStyle.height) / 2 + container.offsetTop + 'px';
                    ele.id = pos;
                    setDragEvent(ele, pos);
                    break;
                }
            case 'lefttop':
                {
                    eleStyle.cursor = 'nw-resize';
                    eleStyle.left = parseInt(left) - parseInt(eleStyle.width) / 2 + container.offsetLeft + 'px';
                    eleStyle.top = parseInt(top) - parseInt(eleStyle.height) / 2 + container.offsetTop + 'px';
                    ele.id = pos;
                    setDragEvent(ele, pos);
                    break;
                }

            case 'rightbottom':
                {
                    eleStyle.cursor = 'se-resize';
                    eleStyle.left = parseInt(left) + parseInt(style.width) - parseInt(eleStyle.width) / 2 + container.offsetLeft + 'px';
                    eleStyle.top = parseInt(top) + parseInt(style.height) - parseInt(eleStyle.height) / 2 + container.offsetTop + 'px';
                    ele.id = pos;
                    setDragEvent(ele, pos);
                    break;
                }
            case 'righttop':
                {
                    eleStyle.cursor = 'ne-resize';
                    eleStyle.left = parseInt(left) + parseInt(style.width) - parseInt(eleStyle.width) / 2 + container.offsetLeft + 'px';
                    eleStyle.top = parseInt(top) - parseInt(eleStyle.height) / 2 + container.offsetTop + 'px';
                    ele.id = pos;
                    setDragEvent(ele, pos);
                    break;

                }
            case 'leftbottom':
                {
                    eleStyle.cursor = 'sw-resize';
                    eleStyle.left = parseInt(left) - parseInt(eleStyle.width) / 2 + container.offsetLeft + 'px';

                    eleStyle.top = parseInt(top) + parseInt(style.height) - parseInt(eleStyle.height) / 2 + container.offsetTop + 'px';
                    ele.id = pos;
                    setDragEvent(ele, pos);
                    break;
                }

        }
        document.body.appendChild(ele);
    }

    function setDragEvent(elem, pos) {
        document.onclick = function(eve) {
            eve = eve || window.event;
            var tar = eve.target || eve.srcElement;

            if (tar.tagName == 'HTML'||tar==container) {

                window.event ? style.filter = "alpha(opacity=100)" : style.opacity = 1;
                removeAllDragPoints();
            }
        }
        document.onmousedown = function(eve) {

            eve = eve || window.event;
            var tar = eve.target || eve.srcElement;
            pos = tar.id;
            for (var i = 0, len = dragPointArr.length; i < len; i++) {
                if (dragPointArr[i] == tar) {
                    dragElem = tar;

                    break;
                }
            }
            if (dragElem) {


                window.event ? style.filter = "alpha(opacity=" + (opacity * 100) + ")" : style.opacity = opacity; //设置拉动时的透明度
                isDrag = true;
                leftX = parseInt(style.left);
                topY = parseInt(style.top);
                oriWidth = parseInt(style.width);
                oriHeight = parseInt(style.height);
                bottomY = topY + oriHeight;
                rightX = leftX + oriWidth;
                marginX = parseInt(eve.clientX) - parseInt(dragElem.style.left);
                marginY = parseInt(eve.clientY) - parseInt(dragElem.style.top);
            }


        }


        document.onmousemove = function(eve) {

            eve = eve || window.event;


            if (isDrag) {


                switch (pos) {
                    case 'left': /*计算左拉动*/
                        {
                            dragLeft(eve);
                            break;
                        }
                    case 'right': /*计算右拉动*/
                        {

                            dragRight(eve);
                            break;
                        }
                    case 'top': /*计算上拉动*/
                        {
                            dragTop(eve);
                            break;
                        }
                    case 'bottom': /*计算下拉动*/
                        {
                            dragBottom(eve);
                            break;
                        }
                    case 'lefttop': /*计算左上拉动*/
                        {

                            dragLeft(eve);
                            dragTop(eve);

                            break;
                        }

                    case 'rightbottom': /*计算右下拉动*/
                        {
                            dragRight(eve);
                            dragBottom(eve);
                            break;
                        }
                    case 'righttop': /*计算右上拉动*/
                        {
                            dragRight(eve);
                            dragTop(eve);
                            break;
                        }
                    case 'leftbottom': /*计算左下拉动*/
                        {
                            dragLeft(eve);
                            dragBottom(eve);
                            break;
                        }


                }
            }

        }
        document.body.onmouseup = function() {
            dragElem = null;
            isDrag = false;
            window.event ? style.filter = "alpha(opacity=100)" : style.opacity = 1;
            if (isRepos) {
                var ele = document.getElementById(id);
                ele.style.left = left + 'px';
                ele.style.top = top + 'px';
                setAllDragPoints();
            }
        }

    }

    function dragLeft(eve) {
        dragElem.style.left = Math.min((rightX - parseInt(dragElem.style.width) / 2 - minWidth) + container.offsetLeft, (parseInt(eve.clientX) - marginX)) + 'px'; 
        style.left = Math.min(rightX, parseInt(dragElem.style.left) + parseInt(dragElem.style.width) / 2 - container.offsetLeft) + 'px';
        style.width = Math.max(rightX - parseInt(style.left), 0) + 'px';

        getAllPoints();

        lefttopPoint.style.left = leftbottomPoint.style.left = leftPoint.style.left = dragElem.style.left;
        topPoint.style.left = (parseInt(style.width) - parseInt(topPoint.style.width)) / 2 + parseInt(style.left) + container.offsetLeft + 'px';
        bottomPoint.style.left = (parseInt(style.width) - parseInt(bottomPoint.style.width)) / 2 + parseInt(style.left) + container.offsetLeft + 'px';

    }
    
    function dragRight(eve) {
        dragElem.style.left = Math.max(parseInt(eve.clientX) - marginX, leftX + minWidth + container.offsetLeft) + 'px';
        style.width = Math.max(oriWidth + parseInt(eve.clientX) - rightX - container.offsetLeft, minWidth + parseInt(dragElem.style.width) / 2) + 'px';

        getAllPoints();
        righttopPoint.style.left = rightbottomPoint.style.left = rightPoint.style.left = dragElem.style.left;

        topPoint.style.left = (parseInt(style.width) - parseInt(topPoint.style.width)) / 2 + parseInt(style.left) + container.offsetLeft + 'px';
        bottomPoint.style.left = (parseInt(style.width) - parseInt(bottomPoint.style.width)) / 2 + parseInt(style.left) + container.offsetLeft + 'px';
    }
    
    function dragTop(eve) {
        dragElem.style.top = Math.min(parseInt(eve.clientY) - marginY, bottomY - parseInt(dragElem.style.height) / 2 - minHeight + container.offsetTop) + 'px';
        style.top = parseInt(dragElem.style.top) + parseInt(dragElem.style.height) / 2 - container.offsetTop + 'px';
        style.height = Math.max(bottomY - parseInt(style.top), minHeight) + 'px';
        getAllPoints();


        righttopPoint.style.top = lefttopPoint.style.top = topPoint.style.top = dragElem.style.top;

        leftPoint.style.top = (parseInt(style.height) - parseInt(leftPoint.style.height)) / 2 + parseInt(style.top) + container.offsetTop + 'px';
        rightPoint.style.top = (parseInt(style.height) - parseInt(rightPoint.style.height)) / 2 + parseInt(style.top) + container.offsetTop + 'px';
    }

    function dragBottom(eve) {
        dragElem.style.top = Math.max(parseInt(eve.clientY) - marginY, topY + parseInt(dragElem.style.height) / 2 + minHeight + container.offsetTop) + 'px';
        style.height = Math.max(parseInt(dragElem.style.top) + parseInt(dragElem.style.height) / 2 - topY - container.offsetTop, minHeight) + 'px';
        getAllPoints();

        leftbottomPoint.style.top = rightbottomPoint.style.top = bottomPoint.style.top = dragElem.style.top;
        leftPoint.style.top = (parseInt(style.height) - parseInt(leftPoint.style.height)) / 2 + parseInt(style.top) + container.offsetTop + 'px';
        rightPoint.style.top = (parseInt(style.height) - parseInt(rightPoint.style.height)) / 2 + parseInt(style.top) + container.offsetTop + 'px';
    }

    function setAllDragPoints() {
        setDragPoint('left');
        setDragPoint('right');
        setDragPoint('top');
        setDragPoint('bottom');
        setDragPoint('lefttop');
        setDragPoint('rightbottom');
        setDragPoint('righttop');
        setDragPoint('leftbottom');
    }
    
    function removeAllDragPoints() {
        elem.style.border = 'none';
        var arr = ['top', 'left', 'right', 'bottom', 'lefttop', 'righttop', 'rightbottom', 'leftbottom'];
        for (var n = 0; n < arr.length; n++) {
            if (document.getElementById(arr[n])) {
                document.body.removeChild(document.getElementById(arr[n]));
            }
        }
    }
    
    function removeBorders() {
        var arr = elem.parentNode.getElementsByTagName(type);
        for (var i = 0; i < arr.length; i++) {
            arr[i].style.border = 'none';
        }
    }
    
    elem.onclick = function() {/*点击元素时显示拖动点*/
        removeBorders();
        this.style.border = 'solid 1px ' + dragPointColor;
        setAllDragPoints();
    }
}

/*
xcResizer.resize('dragObj1', 20, 10, 'green', 5, 0.3, true,'container',20,30);
xcResizer.resize('dragObj2', 180, 80, 'yellow', 5, 0.3, true, 'container',20,30);
xcResizer.resize('dragObj3', 350, 20, 'red', 5, 0.3, true, 'container',20,30);*/