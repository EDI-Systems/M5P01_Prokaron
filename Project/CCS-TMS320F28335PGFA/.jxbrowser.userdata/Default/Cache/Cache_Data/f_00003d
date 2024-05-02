Polymer({ is : "ti-rov-view",

    behaviors: [
        rovBehaviors.viewBehaviors,
        rovBehaviors.localStorageBehaviors,
        rovBehaviors.viewStatusBehaviors
    ],

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
        notifyOnContentResize : {
          type : Boolean,
          value : true
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
        },
        instNames : {
          type : Array,
          value: []
        },
        currentInst : {
          type : String,
          value: ''
        },
        ccs7 : {
          type: Boolean,
          value: false
        },
        _ccs7 : {
          type: Boolean,
          value: false
        }
    },
    attached : function () {
        this.viewStatusPosition = 'bottom';
    },
    ready : function () {
        this.ccs7 = window.ccs7_browser;
        this.$.tiRovTable.setNotifyObj(this);
    },
    checkInitialWidth : function () {
        /* Make sure all toolbar buttons are visible */
        var toolbarLeft = this.$.viewMenu.offsetLeft + this.$.viewMenu.offsetWidth + 8;
        var allButtonsWidth = toolbarLeft + (36 * 4) + 20;
        if (this.offsetWidth < allButtonsWidth) {
            this.$.viewPaperCard.style.width = allButtonsWidth + 'px';
        }
    },
    cloneView : function (e) {
        var elem = this.rovPanel.newModuleView(this.moduleName);
        elem.style.zIndex = String(this.rovPanel.getMaxZindex() + 1);
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
        if (this.rovPanel.inFullScreenMode) {
            this.rovPanel.maximizeNewView(elem, {left: left, top: top});
        }
    },
    columnsTapped : function(e) {
        this.$.tiRovTable.columnsContextMenu(e, this.rovPanel.viewContainer,
                this.rovPanel.isNarrow() ? 0: rovUtils.leftPaneWidth);
    },
    contentResized : function () {
        this.lockSize();
    },
    contentResizing : function () {
        this.enableAutoResize();
    },
    enableAutoResize : function () {
        if (!this.rovPanel.inFullScreenMode) {
            this.$.viewContentDiv.style.position = 'static';
            this.$.viewPaperCard.style.width = '';
            this.$.viewPaperCard.style.height = '';
        }
    },
    fixedFontChanged : function (e) {
        if (this.defaultSize) {
            this.enableAutoResize();
        }
        this.$.tiRovTable.fontChanged(e);
        if (this.defaultSize) {
            this.lockSize();
        }
    },
    getModuleName : function () {
        return (this.moduleName);
    },
    getProperties : function () {
        var props = this.behaviors[0].getProperties(this);  /* call super, pass scope */
        props.moduleName = this.moduleName;
        props.viewsData = {};
        if (this.viewName) {
            props.viewsData[this.moduleName + '.' + this.viewName] =
                this.getViewsData()[this.moduleName + '.' + this.viewName];
        }
        if (this.viewArgs) {
            props.viewArgs = this.viewArgs;
        }
        return (props);
    },
    getSavedViewArgs : function (id) {
        var viewArgs = null;
        var viewsData = this.getViewsDataStorage();
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
            var viewArgs = this.getViewArgs(argsId);
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
                this.$.instSelectDiv.hidden = true;
                if (module == this.moduleName && view == this.viewName) {
                    var viewType = this.getViewType(this.moduleName, this.viewName);
                    var viewColumns = this.getViewColumns(this.moduleName, this.viewName);
                    if (this.viewChanged && !this.rovPanel.inFullScreenMode) {
                        var key = this.moduleName + '.' + this.viewName;
                        var viewsData = this.getViewsDataStorage();
                        if (viewsData[key] && viewsData[key].width) {
                            this.$.viewPaperCard.style.width = viewsData[key].width;
                            this.$.viewPaperCard.style.height = viewsData[key].height;
                            this.$.viewContentDiv.style.position = 'absolute';
                        }
                        else {
                            this.enableAutoResize();
                        }
                        this.viewChanged = false;
                    }
                    var scrollTop = this.$.viewContentDiv.scrollTop;
                    var showColumn = (this.highlightCell && this.highlightCell.showColumn) ?
                                     this.highlightCell.selectedColumnName : null;
                    this.viewIsShowing = this.$.tiRovTable.displayTable(viewData, this.viewName, viewType,
                                                                        viewColumns, this.viewRefresh, false,
                                                                        showColumn);
                    if (this.viewIsShowing) {
                        if (viewType == 'INSTANCE_DATA' && this.$.tiRovTable.instanceDataTable.showInstSelect) {
                            this.instNames = this.$.tiRovTable.instanceDataTable.instNames;
                            this.$.instSelectDiv.hidden = false;
                            this.currentInst = this.$.tiRovTable.instanceDataTable.currentInst;
                            this.$.viewContentDiv.style.top = '64px';
                        }
                        else {
                            this.$.viewContentDiv.style.top = '36px';
                        }
                        this.viewData = viewData;
                        this.hasSelectableColumns = this.$.tiRovTable.hasSelectableColumns();
                        if (!this.viewRefresh && this.$.viewContentDiv.style.position != 'absolute') {
                            this.checkInitialWidth();
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
                if (this.viewIsShowing && this.highlightCell) {
                    if (this.highlightCell.selectedColumnName) {
                        this.$.tiRovTable.highlightCell(this.highlightCell);
                    }
                    else {
                        this.$.tiRovTable.highlightRow({
                            columnName: this.highlightCell.primaryKeyColumnName,
                            columnValue: this.highlightCell.primaryKeyColumnValue
                        });
                    }
                    this.highlightCell = null;
                }
            }
            catch (e) {
                console.error(e);
                this.rovPanel.showStatus('Internal error: ' + e.message, 'error');
            }
        }
        else {
            if (this.isReportableError(error)) {
                this.viewIsShowing = this.$.tiRovTable.displayTable(error, this.viewName, viewType, viewColumns, this.viewRefresh);
            }
            else {
                if (this.rovPanel.inRepeatRefresh) {
                    this.rovPanel.stopRepeatRefresh(false);
                }
                if (!this.viewRefresh) {
                    this.viewIsShowing = this.$.tiRovTable.displayTable(error, this.viewName, viewType, viewColumns, this.viewRefresh);
                }
            }
        }
        this.viewSelected = true;
        this.gettingView = false;
        this.viewToolbarState(this);
        this.rovPanel.toolbarButtonsState();
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
    instanceSelected : function (e) {
        var sel = {label: e.currentTarget.selectedItem.textContent.trim(), index: e.currentTarget.selected};
        this.$.selectInstanceMenu.selected = -1;
        this.$.selectInstanceDialog.close();
        this.currentInst = sel.label;
        this.$.tiRovTable.instanceSelected(sel);
    },
    lockSize : function () {
        if (!this.rovPanel.inFullScreenMode) {
            var rect = this.$.viewPaperCard.getBoundingClientRect();
            this.$.viewPaperCard.style.width = rect.width +'px';

            /* don't let it go outside bottom bound */
            var top = (this.style.top == '') ? 0 :  Number(this.style.top.replace(/px/, ''));
            var bBound = this.rovPanel.viewContainer.clientHeight - 12;
            if ((top + rect.height) > bBound) {
                this.$.viewPaperCard.style.height = (bBound - top) + 'px';
            }
            else {
                this.$.viewPaperCard.style.height = rect.height + 'px';
            }
            this.$.viewContentDiv.style.position = 'absolute';
        }
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
        this.viewList = this.rovPanel.getViewList();
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

        this.$.tiRovTable.setIsGlobal(!this.dashboardView);
        if (this.viewsData != null) {
            this.$.tiRovTable.setViewsData(this.viewsData);
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
    rovViewOptionsMenuItemSelected : function (e) {
        var selected = e.currentTarget.selectedItem.id;
        this.$.rovViewOptionsMenu.selected = -1;
        if (selected != 'dividerItem') {
            this.$.rovViewOptionsMenuDialog.close();
        }
        if (selected != 'fixedFontItem' && selected != 'dividerItem') {
            this[selected](e); /* id of the item is the function name */
        }
    },
    saveLastViewSelected : function (viewName, viewArgs) {
        /* Always save last view selected in global storage */
        var viewsData = this.getViewsDataStorage();
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
        this.setViewsDataStorage(viewsData);
    },
    selectInstanceDialog : function (e) {
        var dialog = this.$.selectInstanceDialog;
        dialog.resetFit();
        dialog.style.left = (e.x - 42) + 'px';
        dialog.style.top = (e.y) + 'px';
        dialog.open();
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
        this.$.instSelectDiv.hidden = true;
        this.enableAutoResize();
        this.$.tiRovTable.displayTable(msg, this.viewName, null, null, false);
        this.viewSelected = false;
        this.rovPanel.toolbarButtonsState();
    },
    showView : function(viewName, refresh, selectedEvent, newView) {
        if (this.requestPending) {
            if (viewName != this.viewName && this.rovPanel.inRepeatRefresh) {
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
        /* args stuff */
        this.argsId = this.getViewArgsId(viewName);
        var viewArgs = (this.viewArgs && this.viewArgs[this.argsId]) ?
                       this.viewArgs[this.argsId] :
                       this.getSavedViewArgs(this.argsId);
        var showArgsDialog = this.argsId && !viewArgs && (selectedEvent || newView);

        this.viewRefresh = refresh;
        this.gettingView = !showArgsDialog;
        this.refreshVisible = !showArgsDialog;
        this.viewArgsVisible = this.argsId != null;
        this.viewToolbarState();

        /*  Being shown for the first time from a query or a dashboard. Select
         *  the view in the dropdown menu without causing the select event to fire
         */
        if (!refresh && !selectedEvent) {
            var index = this.views.indexOf(viewName);
            this.$.viewMenu._onIronSelect({detail : {item : this.$.viewListbox.items[index]}});
        }
        if (selectedEvent) {
            this.saveLastViewSelected(viewName);
        }
        this.viewName = viewName;
        var viewMenuFont = tiRovStyles.rovViewMenuFontSize + 'pt Open Sans';
        var viewMenuWidth = rovUtils.getStringWidth(viewName, viewMenuFont) + 50;
        this.$.viewMenu.style.width = viewMenuWidth  + 'px';
        this.setMinWidth(viewName, viewMenuFont);

        var key = this.moduleName + '.' + this.viewName;
        var viewsData = this.getViewsDataStorage();
        if (viewsData[key] && viewsData[key].defaultSize == false) {
            this.defaultSize = false;
        }
        this.viewArgsDescription = this.getViewArgsDescription(this.argsId);
        if (this.argsId) {
            if (viewArgs) {
                var args = ':' + rovUtils.getArgsString(viewArgs);
                viewName += args;
            }
            else if (showArgsDialog) {
                this.viewArgsRefresh = refresh;
                var pos = {x: this.offsetLeft + 10, y: this.offsetTop + 100};
                if (!this.rovPanel.isNarrow()) {
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
    tableCellContextMenu : function (eData) {
        var args;
        var curViewArgs;
        if (this.argsId) {
            var viewArgs = (this.viewArgs && this.viewArgs[this.argsId]) ?
                           this.viewArgs[this.argsId] :
                           this.getSavedViewArgs(this.argsId);
            if (viewArgs) {
                args = ':' + rovUtils.getArgsString(viewArgs);
                curViewArgs = {};
                curViewArgs[this.argsId] = viewArgs;
            }
        }
        var dataProps = {module: this.moduleName,
                         view: this.viewName,
                         selectedColumnName: eData.selectedColumnName,
                         selectedColumnValue: eData.selectedColumnValue,
                         primaryKeyColumnName: eData.primaryKeyColumnName,
                         primaryKeyColumnValue: eData.primaryKeyColumnValue,
                         event: eData.event,
                         args: args,
                         viewArgs: curViewArgs,
                         argsId: this.argsId,
                         viewData: eData.viewData
        };
        this.rovPanel.plotViewDataContextMenu(dataProps);
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
    viewResized : function () {
        this.notifyOnContentResize = false;
        this.viewToolbarState();
        if (this.defaultSize) {
            this.defaultSize = false;
        }
        if (!this.dashboardView && this.viewName) {
            var viewsData = this.getViewsDataStorage();
            var key = this.moduleName + '.' + this.viewName;
            if (viewsData[key]) {
                var rect = this.$.viewPaperCard.getBoundingClientRect();
                viewsData[key].width = rect.width +'px';
                viewsData[key].height = rect.height +'px';
                viewsData[key].defaultSize = this.defaultSize;
                this.setViewsDataStorage(viewsData);
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
            if (this.viewArgsVisible) {
                this.viewArgsVisible = false;
            }
            this.$.refreshVisibleTemplate.render();
        }
        else {
            this.viewMenuVisible = this.viewIsShowing;
        }
    }
});
