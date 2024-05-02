var rovLocalStorage = window.localStorageExt || window.localStorage;
window._rovResizeObj = null;
window._rovViewDragObj = null;
var thePanel = null;

window.addEventListener('keydown', function(e) {
    if (thePanel != null) {
        thePanel.onKeyDown(e);
    }
});

var rovUtils = {
    coreName : '',

    setCoreName : function (coreName) {
        this.coreName = coreName;
    },
    getCoreName : function () {
        return (this.coreName);
    },

    setPanel : function (panel) {
        thePanel = panel;
        window.onbeforeunload = function () {
            thePanel.onBeforeUnload(thePanel);
        };
    },

    leftPaneWidth : 184,

    getGlobalSettings : function () {
        var globalSettings = null;
        if (thePanel != null) {
            globalSettings = thePanel.rovSettings;
        }
        return (globalSettings);
    },

    getArgsString : function (viewArgs) {
        var args = '';
        for (var i = 0; i < viewArgs.args.length; i++) {
            args += viewArgs.args[i].value;
            if (i < viewArgs.args.length - 1) {
                args += ',';
            }
        }
        return (args);
    },

    destroyClickedElement : function(event) {
	    document.body.removeChild(event.target);
    },

    downloadText : function(textToWrite, fileNameToSaveAs) {
        /* create a link that can be clicked to download the saved data */
        var downloadLink = document.createElement("a");
        downloadLink.download = fileNameToSaveAs;
        downloadLink.innerHTML = "Download File";

        var textFileAsBlob = new Blob([textToWrite], {type:'text/plain'});
        if (window.webkitURL != null) {
            /* Chrome allows clicking links that are not in the DOM */
            downloadLink.href = window.webkitURL.createObjectURL(textFileAsBlob);
        }
        else {
            /* Firefox requires clickable links to be in the DOM */
            downloadLink.href = window.URL.createObjectURL(textFileAsBlob);
            downloadLink.onclick = this.destroyClickedElement;
            downloadLink.style.display = "none";
            document.body.appendChild(downloadLink);
        }
        downloadLink.click();
    },
    enableDragPointerEvents : function (panel, elem, enable) {
        panel.$.rovHeaderPanel.style.pointerEvents = enable;
        elem.style.pointerEvents = enable;
    },
    initRovViewDrag : function (e, elem, panel) {
        var rect = elem.getBoundingClientRect();
        _rovViewDragObj = {};
        _rovViewDragObj.xOffset = e.clientX - rect.left;
        _rovViewDragObj.yOffset = e.clientY - rect.top;
        _rovViewDragObj.elem = elem;
        _rovViewDragObj.rovUtils = this;
        _rovViewDragObj.panel = panel;
        _rovViewDragObj.dragged = false;
        _rovViewDragObj.tBound = 48; /* toolbar height */
        _rovViewDragObj.lBound = panel.isNarrow() ? 0 : this.leftPaneWidth;
        _rovViewDragObj.rBound = panel.isNarrow() ? _rovViewDragObj.panel.viewContainer.clientWidth : _rovViewDragObj.panel.viewContainer.clientWidth + this.leftPaneWidth;
        _rovViewDragObj.bBound = _rovViewDragObj.panel.viewContainer.clientHeight;
        panel.style.cursor = !panel.ccs7 ? '-webkit-grabbing' : 'move';
        this.enableDragPointerEvents(panel, elem, 'none');
        window.addEventListener('mousemove', this.rovViewDrag, false);
        window.addEventListener('mouseup', this.stopRovViewDrag, false);
    },

    rovViewDrag : function (e) {
        if (_rovViewDragObj) {
            if (_rovViewDragObj.elem.id.indexOf('Dialog') > 0) {
                var xPos = e.clientX - _rovViewDragObj.xOffset;
                if (xPos < _rovViewDragObj.lBound) {
                    xPos = _rovViewDragObj.lBound;
                }
                var yPos = e.clientY - _rovViewDragObj.yOffset;
                if (yPos < 48) {
                    yPos = 48;
                }
            }
            else {
                if (e.clientX >= _rovViewDragObj.rBound) {
                    var xPos = (_rovViewDragObj.rBound - _rovViewDragObj.xOffset) - _rovViewDragObj.lBound;
                }
                else {
                    var xPos = (e.clientX - _rovViewDragObj.xOffset) < _rovViewDragObj.lBound ?
                               0 : (e.clientX - _rovViewDragObj.xOffset) - _rovViewDragObj.lBound;
                }
                if ((e.clientY  - _rovViewDragObj.tBound) >= _rovViewDragObj.bBound) {
                    var yPos = (_rovViewDragObj.bBound - _rovViewDragObj.yOffset);
                    if (_rovViewDragObj.panel.viewContainer.scrollWidth > _rovViewDragObj.panel.viewContainer.clientWidth) {
                        yPos -= 16;  /* scroll bar will hide top of view w/o this */
                    }
                }
                else {                                            /* paper-card margin-top is 8 */
                    var yPos = (e.clientY - _rovViewDragObj.yOffset) < 40 ? -8 : (e.clientY - _rovViewDragObj.yOffset) - _rovViewDragObj.tBound;
                }
            }
            _rovViewDragObj.left = xPos + _rovViewDragObj.panel.viewContainer.scrollLeft;
            _rovViewDragObj.top = yPos + _rovViewDragObj.panel.viewContainer.scrollTop;
            _rovViewDragObj.elem.style.left = _rovViewDragObj.left + 'px';
            _rovViewDragObj.elem.style.top = _rovViewDragObj.top + 'px';
            _rovViewDragObj.dragged = true;
        }
    },

    stopRovViewDrag : function (e) {
        if (_rovViewDragObj) {
             if (_rovViewDragObj.dragged) {
                 var dlgElem = _rovViewDragObj.elem.id.indexOf('Dialog') > 0 ? _rovViewDragObj.elem : null;
                 _rovViewDragObj.panel.viewDropDisplay.call(_rovViewDragObj.panel, e, _rovViewDragObj.elem.id, 0, 8, dlgElem);
             }
            _rovViewDragObj.panel.style.cursor = 'inherit';
            _rovViewDragObj.rovUtils.enableDragPointerEvents(_rovViewDragObj.panel, _rovViewDragObj.elem, '');
            window.removeEventListener('mousemove', _rovViewDragObj.rovUtils.rovViewDrag, false);
            window.removeEventListener('mouseup', _rovViewDragObj.rovUtils.stopRovViewDrag, false);
            _rovViewDragObj = null;
        }
    },

    getLocalStorageItem : function (key) {
        return (rovLocalStorage.getItem(key));
    },

    getLocalStorageObject : function (key) {
        var item = rovLocalStorage.getItem(key);
        return (item ? JSON.parse(item) : {});
    },

    rovLocalStorageLength : function () {
        return (rovLocalStorage.length);
    },

    removeLocalStorageItem : function (key) {
        rovLocalStorage.removeItem(key);
    },

    setLocalStorageItem : function (key, value) {
        if (value === null || value === undefined) {
            rovLocalStorage.setItem(key, null);
            rovLocalStorage.removeItem(key);
        }
        else {
            rovLocalStorage.setItem(key, value);
        }
    },

    setLocalStorageObject : function (key, value) {
        if (value === null || value === undefined) {
            rovLocalStorage.setItem(key, null);
            rovLocalStorage.removeItem(key);
        }
        else {
            rovLocalStorage.setItem(key, JSON.stringify(value));
        }
    },

    rovResize : function (e) {
        if (_rovResizeObj) {
            e.preventDefault();
            if (_rovResizeObj.viewContentDiv) {
                _rovResizeObj.viewContentDiv.style.position = 'absolute';
                _rovResizeObj.viewContentDiv = null;
            }
            var rect = _rovResizeObj.elem.getBoundingClientRect();
            var minWidth = window.getComputedStyle(_rovResizeObj.elem).minWidth.replace(/px/, '');
            var minHeight = window.getComputedStyle(_rovResizeObj.elem).minHeight.replace(/px/, '');
            if (_rovResizeObj.leftResize) {
                _rovResizeObj.left = (e.clientX - _rovResizeObj.xOffset) < _rovResizeObj.lBound ?
                                     0 : (e.clientX - _rovResizeObj.xOffset) - _rovResizeObj.lBound;
                if (_rovResizeObj.left > 0) {
                    _rovResizeObj.width = rect.width + (rect.left - (e.clientX - _rovResizeObj.xOffset));
                }
                if (_rovResizeObj.width >= minWidth) {
                    _rovResizeObj.positionElem.style.left = _rovResizeObj.left + 'px';
                }
                if (_rovResizeObj.topResize) {
                    var newHeight = rect.bottom  - (e.clientY - _rovResizeObj.yOffset);
                    if (_rovResizeObj.height === undefined || newHeight > _rovResizeObj.height || _rovResizeObj.height >= minHeight) {
                        _rovResizeObj.top = (e.clientY - _rovResizeObj.yOffset) < 40 ? -8 : (e.clientY - _rovResizeObj.yOffset) - 48;
                        _rovResizeObj.height = newHeight;
                    }
                    if (e.clientY >= 48) {
                        _rovResizeObj.elem.style.height = rect.bottom  - (e.clientY - _rovResizeObj.yOffset) + 'px';
                    }
                    _rovResizeObj.positionElem.style.top = (_rovResizeObj.top - 8) + 'px';  /* positionElem */
                }
                else {
                    var clientY = e.clientY;
                    if (clientY > _rovResizeObj.bBound) {
                        clientY = _rovResizeObj.bBound;
                    }
                    /* bottom right */
                    _rovResizeObj.height = clientY - rect.top + 12;
                    _rovResizeObj.elem.style.height = _rovResizeObj.height + 'px';                }
            }
            else {
                if (_rovResizeObj.topResize) {
                    var newHeight = rect.bottom  - (e.clientY - _rovResizeObj.yOffset);
                    if (_rovResizeObj.height === undefined || newHeight > _rovResizeObj.height || _rovResizeObj.height >= minHeight) {
                        _rovResizeObj.top = (e.clientY - _rovResizeObj.yOffset) < 40 ? -8 : (e.clientY - _rovResizeObj.yOffset) - 48;
                        _rovResizeObj.height = newHeight;
                    }
                    if (e.clientY >= 48) {
                        _rovResizeObj.elem.style.height =  _rovResizeObj.height+ 'px';
                    }
                    _rovResizeObj.positionElem.style.top = (_rovResizeObj.top - 8) + 'px';  /* positionElem */
                }
                else {
                    var clientY = e.clientY;
                    if (clientY > _rovResizeObj.bBound) {
                        clientY = _rovResizeObj.bBound;
                    }
                    /* bottom right */
                    _rovResizeObj.height = clientY - rect.top + 12;
                    _rovResizeObj.elem.style.height = _rovResizeObj.height + 'px';
                }
                var clientX = e.clientX;
                if (clientX >= _rovResizeObj.rBound) {
                    clientX = _rovResizeObj.rBound;
                }
                _rovResizeObj.width = clientX - rect.left + 10;
            }
        }
        if (_rovResizeObj.width < minWidth) {
            _rovResizeObj.width = minWidth;
        }
        _rovResizeObj.elem.style.width = _rovResizeObj.width + 'px';
        _rovResizeObj.positionElem.style.width = _rovResizeObj.width + 'px';  /* positionElem */
    },

    stopRovResize : function (e) {
        if (_rovResizeObj) {
            _rovResizeObj.elem.style.cursor = 'inherit';
            if (e.shiftKey) {
                _rovResizeObj.panel.viewResized.call(_rovResizeObj.panel, e, _rovResizeObj);
            }
            if (_rovResizeObj.notifyObj) {
                _rovResizeObj.notifyObj.fxn.call(_rovResizeObj.notifyObj.elem);
            }
            window.removeEventListener('mousemove', _rovResizeObj.rovUtils.rovResize, false);
            window.removeEventListener('mouseup', _rovResizeObj.rovUtils.stopRovResize, false);
            _rovResizeObj = null;
        }
    },

    initRovResize : function (e, elem, notifyObj, viewContentDiv) {
        e.preventDefault();

        var rect = elem.getBoundingClientRect();
        _rovResizeObj = {};
        _rovResizeObj.positionElem = e.currentTarget.parentElement.parentElement;
        _rovResizeObj.panel = document.querySelector('ti-rov-panel');
        _rovResizeObj.positionElem.zIndex = String(_rovResizeObj.panel.getMaxZindex() + 2);
        _rovResizeObj.elem = elem;
        _rovResizeObj.rovUtils = this;
        _rovResizeObj.viewContentDiv = viewContentDiv;
        _rovResizeObj.leftResize = e.currentTarget.className.indexOf('Left') > 0;
        _rovResizeObj.topResize = e.currentTarget.className.indexOf('Top') > 0;
        _rovResizeObj.yOffset = e.clientY - rect.top;
        _rovResizeObj.xOffset = e.clientX - rect.left;
        _rovResizeObj.lBound = _rovResizeObj.panel.isNarrow() ? 0 : this.leftPaneWidth;
        _rovResizeObj.rBound = _rovResizeObj.panel.isNarrow() ?
                               _rovResizeObj.panel.viewContainer.clientWidth :
                               _rovResizeObj.panel.viewContainer.clientWidth + this.leftPaneWidth;
         _rovResizeObj.rBound -= 16;  /* padding for visuals */

        /* 34 is top toolbar size minus a margin to keep bottom of view visisble */
        _rovResizeObj.bBound = _rovResizeObj.panel.viewContainer.clientHeight + 34;

        if (notifyObj) {
            _rovResizeObj.notifyObj = notifyObj;
        }
        if (_rovResizeObj.leftResize) {
            elem.style.cursor = _rovResizeObj.topResize ? 'nw-resize' : 'sw-resize';
        }
        else {
            elem.style.cursor = _rovResizeObj.topResize ? 'ne-resize' : 'se-resize';
        }
        window.addEventListener('mousemove', this.rovResize, false);
        window.addEventListener('mouseup', this.stopRovResize, false);
    },

    isObjectVal : function (val) {
        return (typeof(val)  == 'object' && !val.length);
    },

    getObjectVal : function (val) {
        var keys = Object.keys(val);
        return (val[keys[0]]);
    },

    isObject : function (obj) {
       return (obj && (typeof(obj)  === "object"));
    },

    isArray : function (obj) {
      return (this.isObject(obj) && (obj instanceof Array));
    },

    deepCopy : function (srcObj) {
        return(JSON.parse(JSON.stringify(srcObj)));
    },

    shallowCopy : function (srcObj) {
        var newObj = null;
        if (this.isArray(srcObj)) {
            newObj = [];
            for (var i = 0; i < srcObj.length; i++) {
                newObj.push(srcObj[i]);
            }
        }
        else if (this.isObject(srcObj)) {
            newObj = {};
            var keys = Object.keys(srcObj);
            for (var i = 0; i < keys.length; i++) {
                newObj[keys[i]] = srcObj[keys[i]];
            }
        }
        return (newObj);
    },

    getPackagesInfo : function (modules) {
        var pkgNames = [];
        var pkgs = [];

        for (var i = 0; i < modules.length; i++) {
            var pkgName = modules[i].substr(0, modules[i].lastIndexOf('.'));
            if (pkgNames.indexOf(pkgName) == -1) {
                pkgNames.push(pkgName);
            }
        }
        for (i = 0; i < pkgNames.length; i++) {
            var pkg = {};
            pkg.name = pkgNames[i];
            pkg.units = this.getPkgUnits(pkgNames[i], modules);
            pkgs[pkgNames[i]] = pkg;
        }
        return ({pkgNames : pkgNames, pkgs : pkgs});
    },

    getPkgUnits : function (pkgName, modules) {
        var units = [];
        for (var i = 0; i < modules.length; i++) {
            var name = modules[i].substr(0, modules[i].lastIndexOf('.'));
            if (name == pkgName) {
                var unit = {};
                unit.name = modules[i].substr(modules[i].lastIndexOf('.') + 1);
                units.push(unit);
            }
        }
        return (units);
    },

    objNameCompare : function (obj1, obj2) {
        var name1 = obj1.name.toUpperCase();
        var name2 = obj2.name.toUpperCase();
        if (name1 < name2) {
            return (-1);
        }
        if (name1 > name2) {
            return (1);
        }
        else {
            /* if equal, put upper case first */
            var ch1 = obj1.name.substr(0, 1);
            var ch2 = obj2.name.substr(0, 1);
            if (ch1.toUpperCase() == ch2) {
                return (1);
            }
            if (ch2.toUpperCase() == ch1) {
                return (-1);
            }
        }
        return (0);
    },

    objQualCompare : function (obj1, obj2) {
        var qual1 = obj1.qual.toUpperCase();
        var qual2 = obj2.qual.toUpperCase();
        if (qual1 < qual2) {
            return (-1);
        }
        if (qual1 > qual2) {
            return (1);
        }
        else {
            /* if equal, put upper case first */
            var ch1 = obj1.qual.substr(0, 1);
            var ch2 = obj2.qual.substr(0, 1);
            if (ch1.toUpperCase() == ch2) {
                return (1);
            }
            if (ch2.toUpperCase() == ch1) {
                return (-1);
            }
        }
        return (0);
    },

    strArrCompare : function (arr1, arr2) {
        if (arr1.length != arr2.length) {
            return (false);
        }
        for (var i = 0; i < arr1.length; i++) {
            if (arr1[i] != arr2[i]) {
                return (false);
            }
        }
        return (true);
    },

    getFormat : function (val) {
        if (!val || typeof(val) != 'string' || isNaN(val)) {
            return (null);
        }
        if (val.match(/^0x/i)) {
            return ('Hex');
        }
        if (val.indexOf('E') > 0 || val.indexOf('e') > 0) {
            return ('Scientific');
        }
        return ('Decimal');
    },

    getStringWidth : function (str, font) {
         var canvas = document.createElement("canvas");
         var context = canvas.getContext("2d");
         context.font = font ? font : '14pt arial';
         var metrics = context.measureText(str);
         return (metrics.width);

    },

    validDragElement : function (targetElem) {
        return (typeof(targetElem.className) == 'string' &&
                targetElem.className.indexOf('rovTh') == -1);
    },

    isAnROVNumber : function (val) {
        if ((val && typeof(val) == 'string' && !isNaN(val)) || typeof(val) == 'number') {
            return (true);
        }
        return (false);
    },

    getViewTabs : function (viewList, moduleName) {
        return (this.isArray(viewList[moduleName]) ?
                viewList[moduleName] : viewList[moduleName].viewTabs);
    },

    getStatusCountWidth : function (statusItems) {
        var max = 0;
        var width = 0;
        for (var i = 0; i < statusItems.length; i++) {
            if (statusItems[i].count != '' && Number(statusItems[i].count) > max) {
                max = Number(statusItems[i].count);
            }
        }
        if (max > 99) {
            width = 21;
        }
        else if (max > 9) {
            width = 16;
        }
        else if (max > 1) {
            width = 11;
        }
        for (var i = 0; i < statusItems.length; i++) {
            statusItems[i].countStyle = 'width:' + width + 'px';
        }
        return(width);
    },
    compressStatusItems : function (statusItems) {
        var items = rovUtils.shallowCopy(statusItems);
        for (var i = 0; i < items.length - 1; i++) {
            items[i].count = 1;
            for (var j = i + 1; j < items.length; j++) {
                if (!items[j].dup &&
                    items[i].message == items[j].message &&
                    items[i].type == items[j].type) {
                        ++items[i].count;
                        items[j].dup = true;
                }
            }
        }
        var retItems = [];
        for (var i = 0; i < items.length; i++) {
            if (!items[i].dup) {
                items[i].count = (items[i].count > 1) ? String(items[i].count) : '';
                retItems.push(items[i]);
            }
        }
        return(retItems);
    },
    statusItem : function (message, type) {
        var statusItem = {};
        if (type == 'warning') {
            statusItem.icon = 'warning';
            statusItem.iconClass = 'statusWarningIcon';
        }
        else if (type == 'error') {
            statusItem.icon = 'error';
            statusItem.iconClass = 'statusErrorIcon';
        }
        else {
            statusItem.icon = 'info-outline';
            statusItem.iconClass = 'statusInfoIcon';
        }
        statusItem.message = message;
        return(statusItem);
    },

    isValidLink : function(hRef) {
        var valid = true;
        if (hRef.indexOf('file://') >= 0) {
            valid = false;
        }
        else if (hRef.indexOf('rov://') >= 0) {
            var search = hRef.substr(hRef.indexOf('?'));
            var params = new URLSearchParams(search);
            if (hRef.indexOf('/addon') == -1) {
                var viewList = thePanel.getViewList();
                var module = params.get('module');
                var view = params.get('view');
                var row = params.get('row');
                if (row) {
                    row = row.split(':');
                }
                var col = params.get('col');

                var moduleFound = false;
                var viewFound = false;
                var columns = null;
                var rowFound = false;
                var colFound = false;
                if (viewList[module]) {
                    moduleFound = true;
                    if (view.indexOf(':') > 0) {
                        view = view.substr(0, view.indexOf(':'));
                    }
                    for (var i = 0; i < viewList[module].viewTabs.length; i++) {
                        if (viewList[module].viewTabs[i].name == view) {
                            columns = viewList[module].viewTabs[i].columns;
                            viewFound = true;
                            break;
                        }
                    }
                    if (viewFound) {
                        for (var i = 0; row != null && i < columns.length; i++) {
                            if (columns[i] == row[0]) {
                                rowFound = true;
                                break;
                            }
                        }
                        if (rowFound) {
                            if (col) {
                                for (var i = 0; i < columns.length; i++) {
                                    if (columns[i] == col) {
                                        colFound = true;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
                if (moduleFound && viewFound) {
                    valid = (rowFound && colFound) ||
                            (row == null && col == null) ||
                            (rowFound && col == null);

                }
                else {
                    valid = false;
                }
            }
            else if (params.get('markdown') != null) {
                var params = params.get('params');
                valid = params.match(/.*:.*/) != null;
            }
        }
        return (valid);
    }
}
