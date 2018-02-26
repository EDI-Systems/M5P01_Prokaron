Polymer({ is : "ti-rov-view",

    behaviors: [rovBehaviors.viewBehaviors],

    properties: {
        moduleName: {
          type: String,
          notify: true,
          observer: 'moduleNameChanged'
        },
        shortModuleName: {
          type: String,
          value: ''
        },
        viewData : {
          type: Object,
          value: function() { return {};}
        },
        viewIsShowing: {
          type: Boolean,
          value: false
        },
        viewSelected: {
          type: Boolean,
          value: false
        },
        viewList : {
          type: Object,
          value: function() { return {};}
        },
        viewName: {
          type: String,
          value: ''
        },
        views: {
          type: Array,
          value: function() { return []; }
        },
        hasSelectableColumns : {
          type: Boolean,
          value: false
        },
        dashboardView : {
          type: Boolean,
          value: false
        },
        viewsData : {
          type: Object,
          value: null
        },
        viewRefresh : {
          type: Boolean,
          value: false
        },
        timeoutHandle : {
          type : String,
          value : null
        },
        requestPending : {
          type: Boolean,
          value: false
        },
        pendingViewName : {
          type : String,
          value : ''
        },
        argsId : {
          type: String,
          value: null
        },
        viewMenuPlaceholder : {
          type : String,
          value: 'Select view'
        },
        viewArgsRefresh : {
            type : Boolean,
            value : false
        },
        viewArgsVisible : {
            type : Boolean,
            value : false
        },
        selectableColumnsVisible : {
            type : Boolean,
            value : false
        },
        refreshVisible : {
            type : Boolean,
            value : false
        },
        cloneViewLabel : {
          type : String,
          value: ''
        },
        viewChanged : {
          type : Boolean,
          value : false
        },
        scrollToEnd : {
          type : Boolean,
          value : false
        },
        viewArgsDescription : {
          type : String,
          value: ''
        },
        gettingView : {
          type : Boolean,
          value : false
        },
        viewArgs : {
          type : Object,
          value: null
        },
        viewMenuVisible : {
          type : Boolean,
          value : false
        }
    },
    ready : function () {
        this.$.tiRovTable.setNotifyObj(this);
    },
    cloneView : function (e) {
        var panel = document.querySelector('ti-rov-panel');
        var elem = panel.newModuleView(this.moduleName);
        elem.style.zIndex = String(panel.getMaxZindex() + 1);
        var top = 40;
        if (this.style.top) {
            top +=  Number(this.style.top.replace(/px/, ''));
        }
        var left = this.$.viewMenu.offsetLeft + this.$.viewMenu.offsetWidth;
        if (this.style.left) {
            left +=  Number(this.style.left.replace(/px/, ''));
        }
        elem.style.left = left + 'px';
        elem.style.top = top + 'px';
    },
    columnsTapped : function(e) {
        var panel = document.querySelector('ti-rov-panel');
        this.$.tiRovTable.columnsContextMenu(e, panel.viewContainer, panel.isNarrow() ? 0: rovUtils.leftPaneWidth);
    },
    contentResized : function () {
        this.lockSize();
    },
    contentResizing : function () {
        this.enableAutoResize();
    },
    enableAutoResize : function () {
        this.$.viewContentDiv.style.position = 'static';
        this.$.viewPaperCard.style.width = '';
        this.$.viewPaperCard.style.height = '';
        this.viewChanged = false;
    },
    fixedFontChanged : function (e) {
        this.$.tiRovTable.fontChanged(e);
    },
    getModuleName : function () {
        return (this.moduleName);
    },
    getSavedViewArgs : function (id) {
        var viewArgs = null;
        var viewsData = rovUtils.getLocalStorageObject('viewsData-storage');
        if (viewsData[this.moduleName] && viewsData[this.moduleName].viewArgs &&
            viewsData[this.moduleName].viewArgs[id]) {

            return (viewsData[this.moduleName].viewArgs[id]);
        }
        return (viewArgs);
    },
    getViewArgs : function (id) {
        if (this.viewList[this.moduleName].viewArgs && this.viewList[this.moduleName].viewArgs[id]) {
            return (this.viewList[this.moduleName].viewArgs[id]);
        }
        return (null);
    },
    getViewArgsDescription : function (argsId) {
        var argsDesc = '';
        if (this.argsId != null) {
            viewArgs = this.getViewArgs(argsId);
            if (viewArgs) {
                argsDesc = viewArgs.description;
            }
        }
        return (argsDesc);
    },
    getViewArgsId : function (viewName) {
        var argsId = null;
        var viewTabs = rovUtils.getViewTabs(this.viewList, this.moduleName);
        for (var i = 0; i < viewTabs.length; i++) {
            if (viewTabs[i].name == viewName) {
                if (viewTabs[i].argsId) {
                    argsId = viewTabs[i].argsId;
                }
                break;
            }
        }
        return (argsId);
    },
    getViewCallback : function (error, viewData, module, view) {
        this.requestPending = false;
        if (error == null) {
            try {
                if (view.indexOf(':') > 0) {
                    view = view.substr(0, view.indexOf(':'));
                }
                if (module == this.moduleName && view == this.viewName) {
                    var viewType = this.getViewType(this.moduleName, this.viewName);
                    var viewColumns = this.getViewColumns(this.moduleName, this.viewName);
                    if (this.viewChanged) {
                        var key = this.moduleName + '.' + this.viewName;
                        var viewsData = rovUtils.getLocalStorageObject('viewsData-storage');
                        if (viewsData[key] && viewsData[key].width) {
                            this.$.viewPaperCard.style.width = viewsData[key].width;
                            this.$.viewPaperCard.style.height = viewsData[key].height;
                            this.$.viewContentDiv.style.position = 'absolute';
                        }
                        else {
                            this.enableAutoResize();
                        }
                    }
                    var scrollTop = this.$.viewContentDiv.scrollTop;
                    this.viewIsShowing = this.$.tiRovTable.displayTable(viewData, this.viewName, viewType, viewColumns, this.viewRefresh);
                    if (this.viewIsShowing) {
                        this.viewData = viewData;
                        this.hasSelectableColumns = this.$.tiRovTable.hasSelectableColumns();
                        if (!this.viewRefresh && this.$.viewContentDiv.style.position != 'absolute') {
                            this.lockSize();
                        }
                        if (this.scrollToEnd) {
                            this.$.viewContentDiv.scrollTop = this.$.viewContentDiv.scrollHeight;
                        }
                        else {
                            this.$.viewContentDiv.scrollTop = scrollTop;
                        }
                    }
                    else {
                        this.hasSelectableColumns = false;
                        /* Temporarily disable showing args dialog again if no data
                           It appears as if the dialog is not being closed, ie, not letting
                           the user out of it
                        if (this.argsId) {
                            var pos = {x: this.offsetLeft + 10, y: this.offsetTop + 100};
                            if (!document.querySelector('ti-rov-panel').isNarrow()) {
                                pos.x += rovUtils.leftPaneWidth;
                            }
                            this.viewArgsDialog(pos);
                        }
                        */
                    }
                }
                else {
                    var err = 'Error: module/view mismatch';
                    console.error(err);
                    this.viewIsShowing = this.$.tiRovTable.displayTable(err, this.viewName, viewType, viewColumns, this.viewRefresh);
                }
            }
            catch (e) {
                console.error(e);
                document.querySelector('ti-rov-panel').showStatus('Internal error: ' + e.message, 'error');
            }
        }
        else {
            if (error.indexOf('timeout') == -1 && error.indexOf('cancel') == -1) {
                this.viewIsShowing = this.$.tiRovTable.displayTable(error, this.viewName, viewType, viewColumns, this.viewRefresh);
            }
            else {
                var panel = document.querySelector('ti-rov-panel');
                if (panel.inRepeatRefresh) {
                    panel.stopRepeatRefresh(false);
                }
                if (!this.viewRefresh) {
                    this.viewIsShowing = this.$.tiRovTable.displayTable(error, this.viewName, viewType, viewColumns, this.viewRefresh);
                }
            }
        }
        this.viewSelected = true;
        this.gettingView = false;
        this.viewToolbarState(this);
        document.querySelector('ti-rov-panel').toolbarButtonsState();
        this.viewRefresh = false;
    },
    getViewColumns : function(moduleName, viewName) {
        var viewColumns = null;
        var viewTabs = rovUtils.getViewTabs(this.viewList, moduleName);
        for (var i = 0; i < viewTabs.length; i++) {
            if (viewTabs[i].name == viewName) {
                viewColumns = viewTabs[i].columns;
                break;
            }
        }
        return (viewColumns);
    },
    getViews : function(moduleName) {
        var views = [];
        var viewTabs = rovUtils.getViewTabs(this.viewList, moduleName);
        if (!viewTabs) {
            console.error('No views for module ' + moduleName);
        }
        for (var i = 0; i < viewTabs.length; i++) {
            views.push(viewTabs[i].name);
        }
        return (views);
    },
    getViewsData : function () {
        return(this.$.tiRovTable.getViewsData());
    },
    getViewType : function(moduleName, viewName) {
        var viewTabs = rovUtils.getViewTabs(this.viewList, moduleName);
        var viewType = null;
        for (var i = 0; i < viewTabs.length; i++) {
            if (viewTabs[i].name == viewName) {
                viewType = viewTabs[i].type;
                break;
            }
        }
        return (viewType);
    },
    lockSize : function () {
        var rect = this.$.viewPaperCard.getBoundingClientRect();
        this.$.viewPaperCard.style.width = rect.width +'px';
        this.$.viewPaperCard.style.height = rect.height + 'px';
        this.$.viewContentDiv.style.position = 'absolute';
    },
    menuSelectView : function (viewName) {
        /* Programmatic way to imitate a physical menu select */
        var index = this.views.indexOf(viewName);
        this.$.viewMenu.selectedItem = this.$.viewListbox.items[index];
        this.$.viewListbox.selected = index;
    },
    moduleNameChanged : function (e) {
        if (!e || e.length == 0) {
            return;
        }
        this.shortModuleName = this.moduleName.substr(this.moduleName.lastIndexOf('.') + 1);
        this.cloneViewLabel = 'New ' + this.shortModuleName + ' view';
        var rovPanel = document.querySelector('ti-rov-panel');
        this.viewList = rovPanel.getViewList();
        this.views = this.getViews(this.moduleName);

        var viewMenuFont = tiRovStyles.rovViewMenuFontSize + 'pt Open Sans';
        var viewMenuWidth = rovUtils.getStringWidth('Select view', viewMenuFont) + 50;
        this.$.viewMenu.style.width = viewMenuWidth  + 'px';
        this.setMinWidth('Select view', viewMenuFont);
        this.viewIsShowing = false;
        this.argsId = null;
        this.viewSelected = false;
        this.hasSelectableColumns = false;

        this.viewArgsVisible = false;
        this.selectableColumnsVisible = false;
        this.refreshVisible = false;

        if (this.dashboardView) {
            this.$.tiRovTable.setIsGlobal(false);
            if (this.viewsData != null) {
                this.$.tiRovTable.setViewsData(this.viewsData);
            }
        }
        else {
            this.$.tiRovTable.setIsGlobal(true);
        }
        this.$.tiRovTable.setModuleName(this.moduleName);
    },
    onSaveView : function() {
        return(this.viewData);
    },
    onViewSelected : function(e) {
        var viewName = this.$.viewListbox.selectedItem.textContent.trim();
        var refresh = this.viewIsShowing && viewName == this.viewName;
        if (viewName != this.viewName) {
            this.viewChanged = true;
            this.scrollToEnd = false;
        }
        this.showView(viewName, refresh, e);
    },
    refreshTapped : function() {
        this.showView(this.viewName, true, false);
    },
    resizerClicked : function (e) {
        rovUtils.initRovResize(e, this.$.viewPaperCard,
                               {elem: this, fxn: this.viewResized},
                               this.$.viewContentDiv);
    },
    saveLastViewSelected : function (viewName, viewArgs) {
        /* Always save last view selected in global storage */
        var viewsData = rovUtils.getLocalStorageObject('viewsData-storage');
        if (!viewsData[this.moduleName]) {
            viewsData[this.moduleName] = {};
        }
        viewsData[this.moduleName].defaultViewName = viewName;
        if (viewArgs) {
            if (!viewsData[this.moduleName].viewArgs) {
                viewsData[this.moduleName].viewArgs = {};
            }
            viewsData[this.moduleName].viewArgs[this.argsId] = {description: viewArgs.description,
                                                                args: viewArgs.args};
        }
        rovUtils.setLocalStorageObject('viewsData-storage', viewsData);
    },
    setDashboardView : function (isDashboardView) {
        this.dashboardView = isDashboardView;
        this.$.tiRovTable.setIsGlobal(!isDashboardView);
    },
    setMinWidth : function (viewName, viewMenuFont) {
        var viewMenuMaxWidth = rovUtils.getStringWidth(viewName + '000',  viewMenuFont) + 50;
        this.$.viewPaperCard.style.minWidth = (rovUtils.getStringWidth(this.shortModuleName, '14pt Open Sans') +
                                               viewMenuMaxWidth + 20 + 72) + 'px'; /* margin:20, 2 buttons:2x36 */
    },
    setViewList : function (list) {
        this.viewList = list;
    },
    showNoTable : function (msg) {
        this.refreshVisible = false;
        this.selectableColumnsVisible = false;
        this.viewIsShowing = false;
        this.enableAutoResize();
        this.$.tiRovTable.displayTable(msg, this.viewName, null, null, false);
        this.viewSelected = false;
        document.querySelector('ti-rov-panel').toolbarButtonsState();
    },
    showView : function(viewName, refresh, selectedEvent, newView) {
        if (this.requestPending) {
            if (viewName != this.viewName &&
                document.querySelector('ti-rov-panel').inRepeatRefresh) {
                this.pendingViewName = viewName;
            }
            return;
        }
        if (this.pendingViewName != '') {
            if (viewName != this.pendingViewName) {
                viewName = this.pendingViewName;
                refresh = false;
            }
            this.pendingViewName = '';
        }
        this.argsId = this.getViewArgsId(viewName);
        this.viewRefresh = refresh;
        this.gettingView = true;
        this.refreshVisible = true;
        this.viewToolbarState();

        /*  Being shown for the first time from a query or a dashboard. Select
         *  the view in the dropdown menu without causing the select event to fire
         */
        if (!refresh && !selectedEvent) {
            var index = this.views.indexOf(viewName);
            this.$.viewMenu._onIronActivate({detail : {item : this.$.viewListbox.items[index]}});
        }
        if (selectedEvent) {
            this.saveLastViewSelected(viewName);
        }
        this.viewName = viewName;
        var viewMenuFont = tiRovStyles.rovViewMenuFontSize + 'pt Open Sans';
        var viewMenuWidth = rovUtils.getStringWidth(viewName, viewMenuFont) + 50;
        this.$.viewMenu.style.width = viewMenuWidth  + 'px';
        this.setMinWidth(viewName, viewMenuFont);
        this.viewArgsDescription = this.getViewArgsDescription(this.argsId);
        if (this.argsId) {
            var viewArgs = (this.viewArgs && this.viewArgs[this.argsId]) ?
                           this.viewArgs[this.argsId] :
                           this.getSavedViewArgs(this.argsId);
            if (viewArgs) {
                var args = ':';
                for (var i = 0; i < viewArgs.args.length; i++) {
                    args += viewArgs.args[i].value;
                    if (i < viewArgs.args.length - 1) {
                        args += ',';
                    }
                }
                viewName += args;
            }
            else if (selectedEvent || newView) {
                this.viewArgsRefresh = refresh;
                var pos = {x: this.offsetLeft + 10, y: this.offsetTop + 100};
                if (!document.querySelector('ti-rov-panel').isNarrow()) {
                    pos.x += rovUtils.leftPaneWidth;
                }
                this.viewArgsDialog(pos);
                return;
            }
        }
        this.requestPending = true;
        if (refresh) {
            var maxScroll = this.$.viewContentDiv.scrollHeight - this.$.viewContentDiv.clientHeight;
            if (maxScroll > 0) {
                if (!this.scrollToEnd) {
                    this.scrollToEnd = (maxScroll > 0 && this.$.viewContentDiv.scrollTop >= maxScroll);
                }
                else if (maxScroll > 0 && this.$.viewContentDiv.scrollTop < maxScroll) {
                    this.scrollToEnd = false;
                }
            }
        }
        this.rovData.getView(this.moduleName, viewName, this);
    },
    viewArgsDialog : function (e) {
        var viewArgs = (this.viewArgs && this.viewArgs[this.argsId]) ?
                       this.viewArgs[this.argsId] :
                       this.getSavedViewArgs(this.argsId);
        if (!viewArgs) {
            viewArgs = this.getViewArgs(this.argsId);
        }
        if (e && e.currentTarget &&
            (e.currentTarget.id == 'viewArgsButton' || e.currentTarget.id == 'viewOptionsMenu')) {
            this.viewArgsRefresh = this.viewIsShowing;
        }
        this.$.rovViewOptions.viewArgsDialog(e, viewArgs, {elem : this, fxn : 'viewArgsDialogCallback'});
    },
    viewArgsDialogCallback : function (res, viewArgs) {
        var refresh = this.viewArgsRefresh;
        this.viewArgsRefresh = false;
        if (res == 'cancelled') {
            if (!refresh) {
                var msg = 'Please specify ' + this.shortModuleName + '.' + this.viewName +
                          ' arguments by clicking the ' + this.viewArgsDescription + ' button above';
                this.showNoTable(msg);
            }
            return;
        }
        this.saveLastViewSelected(this.viewName, viewArgs);
        if (!this.viewArgs) {
            this.viewArgs = {};
        }
        this.viewArgs[this.argsId] = viewArgs;
        this.showView(this.viewName, refresh, false);
    },
    viewOptionsMenuClicked : function (e) {
        var dialog = this.$.viewOptionsMenuDialog;
        if (dialog) {

            var left = e.x - 34;
            var width = 235; /* ballpark from styling in html */
            if ((left + width) > document.documentElement.clientWidth) {
                left = document.documentElement.clientWidth - width;
            }
            dialog.style.left = left + 'px';
            var height = 30 + 33 + 10; /* onSave button, fixed font checkbox, padding */
            if (this.$.viewArgsDialog.hidden == false) {
                height += 30;
            }
            if (this.$.columnsTapped.hidden == false) {
                height += 30;
            }
            if (this.$.refreshTapped.hidden == false) {
                height += 30;
            }
            var top = e.y - this.$.closeButton.offsetHeight;
            if (top + height > document.querySelector('ti-rov-panel').viewContainer.clientHeight) {
              top = top - height;
              if (top < 0) {
                  top = 0;
              }
            }
            dialog.style.top = top + 'px';
            dialog.open();
            var viewsData = this.getViewsData();
            this.$.fixedFontCheckbox.checked = viewsData[this.moduleName + '.' + this.viewName].fixedFont;
            dialog.style.zIndex = String(document.querySelector('ti-rov-panel').getMaxZindex() + 1);
        }
    },
    viewOptionsMenuItemSelected : function (e) {
        var selected = e.currentTarget.selectedItem.id;
        this.$.viewOptionsMenu.selected = -1;
        this.$.viewOptionsMenuDialog.close();
        if (selected != 'fixedFontItem') {
            this[selected](e); /* id of the item is the function name */
        }
    },
    viewResized : function () {
        this.$.tiRovTable.setNotifyObj(null);
        this.viewToolbarState();
        if (!this.dashboardView && this.viewName) {
            var rect = this.$.viewPaperCard.getBoundingClientRect();
            var key = this.moduleName + '.' + this.viewName;
            var viewsData = rovUtils.getLocalStorageObject('viewsData-storage');
            if (viewsData[key]) {
                viewsData[key].width = rect.width +'px';
                viewsData[key].height = rect.height +'px';
                rovUtils.setLocalStorageObject('viewsData-storage', viewsData);
            }
        }
    },
    viewToolbarState : function () {
        var toolbarLeft = this.$.viewMenu.offsetLeft + this.$.viewMenu.offsetWidth + 8;
        var viewWidth = this.offsetWidth;
        var numButtons = 1; /* close button */

        /* initial visible state of dropdown items */
        this.$.columnsTapped.hidden = true;
        this.$.viewArgsDialog.hidden = true;
        this.$.refreshTapped.hidden = true;

        var stateArr = [];
        if (this.hasSelectableColumns) {  /* columns button */
            ++numButtons;
            stateArr.push({btnVisible : 'selectableColumnsVisible', item : 'columnsTapped'});
        }
        if (this.argsId) {  /* args dialog button */
            ++numButtons;
            stateArr.push({btnVisible : 'viewArgsVisible', item : 'viewArgsDialog'});
        }
        if (this.viewSelected) {   /* refresh button */
            /*  when user inputs bogus args, we don't get an error back,
             *  just no data, so enabling the read memory button is the 'refresh'
             *  we want them to use, not the refresh button
             */
            if (!(this.argsId && !this.viewIsShowing)) {
                ++numButtons;
                stateArr.push({btnVisible : 'refreshVisible', item : 'refreshTapped'});
            }
        }
        if (this.viewIsShowing) {
            ++numButtons; /* for viewOptionsMenu */
        }
        var buttonsVisible = Math.round(((viewWidth - toolbarLeft) / 36) - .9);
        if (buttonsVisible > numButtons) {
            buttonsVisible = numButtons;
        }
        var diffCount = (buttonsVisible < numButtons) ? numButtons - buttonsVisible : 0;
        for (var i = 0; i < stateArr.length; i++) {
            if (i < diffCount) {
                this[stateArr[i].btnVisible] = false;
                this.$[stateArr[i].item].hidden = false;
            }
            else {
                this[stateArr[i].btnVisible] = true;
                this.$[stateArr[i].item].hidden = true;
            }
        }
        if (buttonsVisible <= 2 && this.gettingView) {
            this.viewMenuVisible = false;
            this.refreshVisible = true;
            if (this.argsId) {
                this.viewArgsVisible = false;
            }
            this.$.refreshVisibleTemplate.render();
        }
        else {
            this.viewMenuVisible = this.viewIsShowing;
        }
    }
});
