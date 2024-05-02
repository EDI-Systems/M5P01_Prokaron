Polymer({ is : "ti-rov-panel",
    behaviors: [
        Polymer.IronResizableBehavior,
        rovBehaviors.localStorageBehaviors
    ],

    properties: {
        dashboardVersion : {
          type : String,
          value : '1.0'
        },
        allModules : {
          type: Array,
          value: function() { return [];}
        },
        viewableModules : {
          type: Array,
          value: []
        },
        favoriteModules : {
          type: Array,
          value: []
        },
        favoriteTocItems : {
          type: Array,
          value: function() { return [];}
        },
        curTocModules : {
          type: Array,
          value: function() { return [];}
        },
        tocItems : {
          type: Array,
          value: function() { return [];}
        },
        allModulesVisible : {
          type: Boolean,
          value: false
        },
        dashboards : {
          type: Object
        },
        rovData : {
          type: Object,
          value: function() { return {};}
        },
        matchedModules : {
          type: Array,
          value: function() { return [];}
        },
        matchedModuleNames : {
          type: Array,
          value: function() { return [];}
        },
        filterValue : {
          type: String,
          value: ''
        },
        showFilterButtonLeft : {
          type: Boolean,
          value: false
        },
        showFilterButtonRight : {
          type: Boolean,
          value: false
        },
        viewList : {
          type: Object,
          value: function() { return {};}
        },
        toggleIcon : {
          type: String,
          value: 'chevron-left'
        },
        toggleTooltip : {
          type: String,
          value: 'Hide Modules'
        },
        rovModuleViews : {
          type: Array,
          value: function() { return [];}
        },
        dashboardNames : {
          type: Array,
          value: function() { return [];}
        },
        refreshInterval : {
          type : Number,
          value : 1000
        },
        dataTimeoutPeriod : {
          type : Number,
          value : 10000
        },
        connectTimeoutPeriod : {
          type : Number,
          value : 20000
        },
        repeatHandle : {
          type : Number,
          value : 0
        },
        confirmTitle : {
            type : String,
            value : ''
        },
        confirmLabel : {
            type : String,
            value : ''
        },
        confirmProps : {
          type : Object,
          value : {}
        },
        rovSettings : {
          type: Object,
          value: {}
        },
        inRepeatRefresh : {
          type: Boolean,
          value: false
        },
        favoriteMenuItem : {
          type: String,
          value: ''
        },
        statusItems : {
          type: Array,
          value: []
        },
        compressedStatusItems: {
          type: Array,
          value: []
        },
        rovInit : {
          type: Object,
          value: {}
        },
        connectMessage : {
          type: String,
          value: ''
        },
        currentDashboard : {
          type: String,
          value: ''
        },
        exeName : {
          type: String,
          value: ''
        },
        viewToTopList : {
            type: Array,
            value: []
        },
        curModsLabel : {
          type: String,
          value: 'Viewable Modules'
        },
        otherModsLabels : {
          type: Array,
          value: []
        },
        rovViews : {
          type: Array,
          value: function() { return [];}
        },
        enableOtherViews : {
          type: Boolean,
          value: false
        },
        otherViews : {
          type: Array,
          value: []
        },
        addOnMrus : {
          type: Array,
          value: []
        },
        commLinkMrus : {
          type: Array,
          value: []
        },
        exeMrus : {
          type: Array,
          value: []
        },
        viewsStorageItems : {
          type: Array,
          value: []
        },
        dashboardConflicts : {
          type: Array,
          value: []
        },
        overwriteDashboardsItems : {
          type: Array,
          value: []
        },
        addOnsHash : {
          type: Array,
          value: []
        },
        ccs7 : {
          type: Boolean,
          value: false
        },
        _ccs7 : {
          type: Boolean,
          value: false
        },

        /*  addOn views tooltips were being cached for this.otherViews
         *  to workaround, use this array to display this.otherViews in dialogs,
         *  setting it to this.otherViews before showing the dialog and
         *  then setting it to empty when the dialog is closed
         */
        dlgOtherViews : {
          type: Array,
          value: []
        },
        panelWidth : {
          type: Number,
          value: -1
        },
        viewContainer : {
          type : Object,
          value : {}
        },
        useCurrentProgram : {
          type : String,
          value : '<Use the program currently being debugged>'
        },
        inited : {
          type: Boolean,
          value: false
        },
        dragData : {
          type: Array,
          value: []
        },
        discoveredAddOns : {
          type: Array,
          value: []
        },
        dlgDiscoveredAddOns : {
          type: Array,
          value: []
        },
        autoConnectMessage : {
          type : String,
          value : ''
        },
        introMessage : {
          type: String,
          value: ''
        },
        introModulesMessage : {
          type: String,
          value: ''
        },
        plotViews : {
          type: Array,
          value: []
        },
        plotViewDataSubItemsHidden : {
          type: Boolean,
          value: true
        },
        plotViewDataSubMenuItems : {
          type: Array,
          value: function() { return [];}
        },
        canRestoreViews : {
          type: Boolean,
          value: false
        },
        exeInputFocused : {
          type: Boolean,
          value: false
        },
        customViews : {
          type: Array,
          value: []
        },
        customViewDataSubMenuItems : {
          type: Array,
          value: function() { return [];}
        },
        customViewDataSubItemsHidden : {
          type: Boolean,
          value: true
        },
        exportLocalStorageMrus : {
          type: Array,
          value: []
        },
        rovDashboardAccept : {
          type: String,
          value: ''
        },
        rovStorageAccept : {
          type: String,
          value: ''
        },
        inFullScreenMode : {
          type: Boolean,
          value: false
        },
        setExecutableReqObj : {
          type: Object,
          value: null
        },
        coreName : {
          type: String,
          value: ''
        },
        unknownCore : {
          type: Boolean,
          value: false
        },
        sessionId : {
          type: Number,
          value: -1
        },
        markdownAddon : {
          type: Object,
          value: null
        }
    },

    listeners: {
      'iron-resize': 'onIronResize'
    },
    attached : function () {
        this.set('dashboards', rovUtils.getLocalStorageObject('dashboards-storage'));
        var favoritesStorage = rovUtils.getLocalStorageObject('favorites-storage');
        if (Object.keys(favoritesStorage).length > 0) {
            this.set('favoriteModules', favoritesStorage);
        }
        if (this._ccs7) {
//            this.coreName = this.rovData.getCoreName();
            this.coreName = rovUtils.getCoreName();
            this.unknownCore = this.coreName == '';
        }
        this.set('rovInit', this.getRovInitStorage());
        if (this.canRestoreViews && this.rovInit.restoreViews === undefined) {
            this.rovInit.restoreViews = true;
            this.setRovInitStorage(this.rovInit);
        }
		
      // auto connect if setting undefined
		if (this.rovInit.autoConnect === undefined) {
			this.rovInit.autoConnect = true;
			
			// we want all initializations done in these functions without actual dialog pop up
			this.showConnectDialog(undefined,true);
			this.connectClicked();

			this.setRovInitStorage(this.rovInit);
		}
		
        this.setRovSettings(this.getRovSettingsStorage());
        
		if (this.rovInit.autoConnect) {
            this.autoConnect();
        }
        else {
            this.showConnectDialog();
        }
        this.$.rovDrawerPanel.disableEdgeSwipe = true;
        this.$.rovDrawerPanel.disableSwipe = true;
        this.viewContainer = this.$.rovHeaderPanel.$.mainContainer;
        this.$.openDashboardFileClicked.hidden = !this.ccs7;
        /* initial toolbar state */
        this.$.rovTitle.hidden = false;
        this.$.titleSpan.hidden = true;
        rovUtils.setPanel(this);
        if (this.rovInit.restoreViews && this.rovInit.showIntro == false &&
            this.rovInit.isNarrow) {
            this.toggleDrawerClicked();
        }
    },
    ready : function () {
        this.rovData = this.$.rovData;
        this.ccs7 = window.ccs7_browser;
        var ccsVers = window.CCS_VERSION ? Number(window.CCS_VERSION.split('.')[0]) : -1;
        this.canRestoreViews = !this.ccs7 || ccsVers >= 8;
    },
    addCellToCustomView : function (cellProps, viewElem) {
        viewElem.addCell(cellProps);
        for (var i = 0; i < this.customViews.length; i++) {
            if (this.customViews[i].elem.id == viewElem.id) {
                this.customViews[i].cells.push(cellProps);
                break;
            }
        }
    },
    addDiscoveredAddOnClicked : function (e) {
        var selected = [];
        var listItems = this.$.discoveredAddOnsList.items;
        for (var i = 0; i < listItems.length; i++) {
            if (listItems[i].lastElementChild.checked) {
                selected.push({path: this.dlgDiscoveredAddOns[i].path,
                               name: this.dlgDiscoveredAddOns[i].name,
                               elem: listItems[i].firstElementChild,
                               index: i});
            }
        }
        for (var i = 0; i < selected.length; i++) {
            var path = this.parseAddOnPath(selected[i].path);
            this.addOnsHash[path] = selected[i];
            this.importHref(path,
                function(e){
                    var key = this.getAddOnHashKey(e.target.outerHTML);
                    var item = this.addOnsHash[key];
                    if (this.createAddOn(e, item.path)) {
                        item.elem.checked = false;
                    }
                    this.syncDiscoveredAddOnsList();
                    this.sanityCheckSelectAllBox('selectAllCurrentAddOnsCheckbox', 'curAddOnsList');
                },
                function(e){
                    var key = this.getAddOnHashKey(e.target.outerHTML);
                    this.addOnsHash[key].href = 'error';
                    this.showStatus('Error importing ' + e.target.href, 'error');
                }
            );
        }
    },
    addFavorite : function (mod) {
        var tempModules = rovUtils.shallowCopy(this.favoriteModules);
        tempModules.push(mod);
        this.set('favoriteModules', tempModules);
        this.set('favoriteTocItems', this.makeModulesToc(this.favoriteModules));
        rovUtils.setLocalStorageObject('favorites-storage', this.favoriteModules);
    },
    addOnDoneClicked : function (e) {
        var dialog = this.$.manageAddOnsDialog;
        /* clean up */
        this.$.curAddOnsList.selected = -1;
        this.$.discoveredAddOnsList.selected = -1;
        this.$.removeAddOnButton.disabled = true;
        this.$.selectAllCurrentAddOnsCheckbox.checked = false;
        this.$.selectAllDiscoveredAddOnsCheckbox.checked = false;
        this.$.addDiscoveredAddOnButton.disabled = true;
        this.$.loadAddOnButton.disabled = true;
        this.showAddOnInput(false);
        if (this.$.addOnErrorToast.opened) {
            this.$.addOnErrorToast.text = '';
            this.$.addOnErrorToast.close();
        }
        this.$.addOnInput.value = '';
        dialog.close();
        this.set('dlgOtherViews', []);
        this.set('dlgDiscoveredAddOns', []);
    },
    addOnErrorCloseClicked : function(e) {
        this.$.addOnErrorToast.close();
        this.$.addOnErrorToast.text = '';
    },
    addOnKeyPress : function (e) {
        var keyCode = e.keyCode || e.which;
        if (keyCode === 13 && this.$.addOnInput.value != '') {
            this.loadAddOnClicked(e);
        }
    },
    addOnOnInput : function (e) {
        if (this.$.addOnErrorToast.opened) {
            this.$.addOnErrorToast.text = '';
            this.$.addOnErrorToast.close();
        }
        this.$.loadAddOnButton.disabled = e.currentTarget.value == '';
    },
    addOnMruSelected : function (e) {
        this.mruSelected(e);
        if (this.$.addOnErrorToast.opened) {
            this.$.addOnErrorToast.text = '';
            this.$.addOnErrorToast.close();
        }
        this.$.loadAddOnButton.disabled = false;
    },
    addOnViewsClicked : function (e) {
        var dialog = this.$.manageAddOnsDialog;
        dialog.resetFit();
        this.set('dlgOtherViews', this.otherViews);
        this.set('dlgDiscoveredAddOns', this.discoveredAddOns);
        this.$.selectAllCurrentAddOnsCheckbox.disabled = (this.dlgOtherViews.length == 0);
        this.populateDiscoveredAddOns();
        this.$.selectAllDiscoveredAddOnsCheckbox.disabled = (this.dlgDiscoveredAddOns.length == 0);
        dialog.open();
        dialog.style.zIndex = String(this.getMaxZindex() + 1);
        this.$.addOnInput.value = '';
        if (this.$.addOnErrorToast.opened) {
            this.$.addOnErrorToast.text = '';
            this.$.addOnErrorToast.close();
        }
    },
    addTraceToRovGraph : function (traceProps, graphElem) {
        graphElem.addTrace(traceProps);
        for (var i = 0; i < this.plotViews.length; i++) {
            if (this.plotViews[i].elem.id == graphElem.id) {
                this.plotViews[i].traces.push(traceProps);
                break;
            }
        }
    },
    autoConnect : function () {
        var exe = (this.ccs7 && this.rovInit.noExe) ? '' : this.rovInit.exe;
        var dialog = this.$.autoConnectProgressDialog;
        this.$.autoConnectProgressCancelDiv.hidden = true;
        if (dialog) {
            dialog.open();
            dialog.style.zIndex = String(this.getMaxZindex() + 1);
            if (!this.rovInit.progressInfo) {
                this.rovInit.progressInfo = {};
                this.rovInit.progressInfo.min = 0;
                this.rovInit.progressInfo.max = 10;
                this.rovInit.progressInfo.step = 1;
            }
            this.$.autoConnectProgress.min = this.rovInit.progressInfo.min;
            this.$.autoConnectProgress.max = this.rovInit.progressInfo.max;
            this.$.autoConnectProgress.step = this.rovInit.progressInfo.step;
            this.rovInit.progressInfo.notifyCount = 0;
            this.$.autoConnectProgress.value = this.rovInit.progressInfo.step;
            var exeName = exe;
            if (exeName.length > 0) {
                exeName = exeName.replace(/\\/g, '/');
                exeName = exeName.substr(exeName.lastIndexOf('/') + 1);
                this.exeName = exeName;
                exeName += ', ';
            }
            this.autoConnectMessage = 'Connecting to ' + exeName + this.rovInit.commLink;
            this.connectMessage = 'Working...';
        }
        this.initRov(exe, this.rovInit.commLink);
    },
    averageIntroProgess : function () {
        if (!this.rovInit.progressInfo.average) {
            this.rovInit.progressInfo.average = this.rovInit.progressInfo.notifyCount;
            this.rovInit.progressInfo.numLoads = 1;

        }
        else {
            /* cumulative moving average */
            this.rovInit.progressInfo.average = (this.rovInit.progressInfo.notifyCount +
                                                (this.rovInit.progressInfo.numLoads * this.rovInit.progressInfo.average)) /
                                                ++this.rovInit.progressInfo.numLoads;

        }
        this.rovInit.progressInfo.max = this.rovInit.progressInfo.average + this.rovInit.progressInfo.step;
    },
    checkForMissingDashboardAddOns : function (dashboard) {
        for (var i = 0; i < dashboard.length; i++) {
            if (dashboard[i].elemName) {   /* is otherView */
                if (!this.otherViewsHaveElem(dashboard[i].elemName)) {
                    this.loadMissingSavedAddOn(dashboard[i], false);
                }
            }
        }
    },
    clearAllLocalStorage: function () {
        this.set('dashboards', {});
        rovUtils.setLocalStorageObject('dashboards-storage', null);
        this.set('favoriteModules', []);
        rovUtils.setLocalStorageObject('favorites-storage', null);
        this.set('rovInit', {});
        this.setRovInitStorage(null);
        this.set('rovSettings', {});
        this.setRovSettingsStorage(null);
        this.setViewsDataStorage(null);
        this.setCurrentViewsStorage(null);
        if (this._ccs7) {
            this.clearAllCoresStorage();
            rovUtils.setLocalStorageObject('coreNames-storage', null);
        }
    },
    clearAllStorageClicked : function (e) {
        this.confirmDialog('Clear all ROV storage', 'Are you sure?', 'clearAll');
    },
    clearAViewChecked : function (e) {
        if (!this.$.deleteModuleStorageDialog.opened) {
            return;
        }
        this.listItemWithSelectAllOptionClicked(e, 'selectAllViewsStorageCheckbox',
                                                'viewsStorageList');
        this.enableChecklistDialogActionButton(e, 'viewsStorageList',
                                               'clearViewStorageButton');
    },
    clearDashboardOptionsMenuDialogState : function () {
        this.$.dashboardOptionsMenu.selected = -1;
        this.$.openDashboardSubMenu.selected = -1;
        this.$.deleteDashboardSubMenu.selected = -1;
        this.$.openDashboardMenu.selected = -1;
        this.$.deleteDashboardMenu.selected = -1;
        this.$.openDashboardMenu.hidden = true;
        this.$.deleteDashboardMenu.hidden = true;
    },
    clearFavorite : function(idx) {
        /* modifying 'favoriteModules' directly didn't work, only first item was displayed */
        var tempModules = rovUtils.shallowCopy(this.favoriteModules);
        tempModules.splice(idx, 1);
        this.set('favoriteModules', tempModules);
        this.set('favoriteTocItems', this.makeModulesToc(this.favoriteModules));
        rovUtils.setLocalStorageObject('favorites-storage', this.favoriteModules);
    },
    clearFavoriteConfirm : function (mod) {
        var idx = this.favoriteModules.indexOf(mod);
        var title = 'Remove Favorite';
        var label = 'Remove ' + this.favoriteModules[idx] + ' ?';
        this.confirmDialog(title, label, 'favorite', idx);
    },
    clearRovOptionsMenuDialogState : function () {
        this.$.rovOptionsMenu.selected = -1;
        this.$.storageOptionsSubMenu.selected = -1;
        this.$.startupOptionsSubMenu.selected = -1;
        this.$.storageOptionsMenu.selected = -1;
        this.$.startupOptionsMenu.selected = -1;
        this.$.storageOptionsMenu.hidden = true;
        this.$.startupOptionsMenu.hidden = true;
    },
    clearViewStorageClicked : function (e) {
        var viewsStorage = this.getViewsDataStorage();
        var listItems = this.$.viewsStorageList.items;
        for (var i = 0; i < listItems.length; i++) {
            if (listItems[i].lastElementChild.checked) {
                viewsStorage[listItems[i].innerText.trim()] = null;
                listItems[i].lastElementChild.checked = false;
            }
        }
        this.setViewsDataStorage(viewsStorage);
        /* repopulate list */
        var keys = Object.keys(viewsStorage);
        var nonNullKeys = [];
        for (var i = 0; i < keys.length; i++) {
            if (viewsStorage[keys[i]]) {
                nonNullKeys.push(keys[i]);
            }
        }
        if (nonNullKeys.length > 0) {
            nonNullKeys.sort();
            this.set('viewsStorageItems', nonNullKeys);
            this.$.clearViewStorageButton.disabled = true;
        }
        else {
            this.$.deleteModuleStorageDialog.close();
        }
        this.$.selectAllViewsStorageCheckbox.checked = false;
        this.$.selectAllViewsStorageCheckbox.disabled = nonNullKeys.length == 0;
    },
    closeAllClicked : function () {
       this.closeAllViews();
    },
    closeAllViews : function() {
        var ids = [];
        for (var i = 0; i < this.rovModuleViews.length; i++) {
            ids.push(this.rovModuleViews[i].id);
        }
        for (var i = 0; i < this.rovViews.length; i++) {
            ids.push(this.rovViews[i].id);
        }
        for (var i = 0; i < ids.length; i++) {
            this.closeView(ids[i]);
        }
    },
    closeView : function(viewId) {
        var rovViews = [];
        var found = false;
        for (var i = 0; i < this.rovModuleViews.length; i++) {
            if (this.rovModuleViews[i].id != viewId) {
                rovViews.push(rovUtils.shallowCopy(this.rovModuleViews[i]));
            }
            else {
                found = true;
            }
        }
        if (found) {
            this.rovModuleViews = rovViews;
        }
        else {
            rovViews = [];
            var foundView = null;
            for (var i = 0; i < this.rovViews.length; i++) {
                if (this.rovViews[i].id != viewId) {
                    rovViews.push(rovUtils.shallowCopy(this.rovViews[i]));
                }
                else {
                    foundView = this.rovViews[i];
                }
            }
            if (foundView) {
                var elem = this.getViewById(foundView.id);
                this.rovViews = rovViews;
                if (elem.name == 'ti-rov-graph') {
                    this.viewDataGraphClosed(elem.id);
                }
                else if (elem.name == 'ti-rov-customview') {
                    this.customViewClosed(elem.id);
                }
            }
        }
        var chNodes = Polymer.dom(this.$.contentDiv).childNodes;
        for (var i = 0; i < chNodes.length; i++) {
            if (chNodes[i].id == viewId) {
                contentDiv.removeChild(chNodes[i]);
                Polymer.dom.flush();
                break;
            }
        }
        this.toolbarButtonsState();
        this.dashboardButtonsState();
    },
    columnIsVisibleFromStorage : function (moduleName, viewName, columnName) {
        var visible = true;
        var viewsStorage = this.getViewsDataStorage();
        if (viewsStorage[moduleName + '.' + viewName]) {
            var columnStates = viewsStorage[moduleName + '.' + viewName].columnStates;
            for (var i = 0; i < columnStates.length; i++) {
                if (columnStates[i].name == columnName) {
                    visible = columnStates[i].checked;
                    break;
                }
            }
        }
        return (visible);
    },
    confirmDialog : function(title, label, objName, sel) {
        var dialog = this.$.confirmDialog;
        this.confirmProps = {};
        this.confirmProps.objName = objName;
        this.confirmProps.sel = sel;
        this.confirmTitle = title;
        this.confirmLabel = label;
        dialog.open();
        dialog.style.zIndex = String(this.getMaxZindex() + 2);
    },
    confirmOkClicked : function (e) {
        this.$.confirmDialog.close();
        if (this.confirmProps.objName == 'dashboard') {
            this.deleteDashboard(this.confirmProps.sel);
        }
        else if (this.confirmProps.objName == 'favorite') {
            this.clearFavorite(this.confirmProps.sel);
        }
        else if (this.confirmProps.objName == 'dashboardConflict') {
            this.saveDroppedDashboard(this.confirmProps.sel.name,
                                      this.confirmProps.sel.dashboard,
                                      this.confirmProps.sel.show);
        }
        else if (this.confirmProps.objName == 'clearAll') {
            this.clearAllLocalStorage();
        }
    },
    connectClicked : function (e) {
        var rovInit = this.getRovInitStorage();
        this.rovInit.autoConnect = rovInit.autoConnect = this.$.autoConnectCheckbox.checked;
        if (this.rovInit.autoConnect) {
            this.rovInit.exe = rovInit.exe = this.$.exeInput.value;
            this.rovInit.commLink = rovInit.commLink = this.$.commLinkInput.value;
            this.rovInit.noExe = rovInit.noExe = this.$.exeInput.value == this.useCurrentProgram;
        }
        this.setRovInitStorage(rovInit);
        this.$.connectMessageDiv.hidden = true;
        var useProgram = this.$.exeInput.value == this.useCurrentProgram;
        var exeInvalid = !useProgram && this.$.exeInput.value.length == 0;
        var commLinkInvalid = this.$.commLinkInput.value.length == 0;
        this.$.exeInputInvalid.hidden = !exeInvalid;
        this.$.commLinkInputInvalid.hidden = !commLinkInvalid;
        if (exeInvalid || commLinkInvalid) {
            return;
        }
        if (!useProgram) {
            var exe = this.$.exeInput.value.replace(/\\/g, '/');
            if (exe.indexOf('/') >= 0) {
                exe = exe.substr(exe.lastIndexOf('/') + 1);
            }
            this.exeName = exe;
            this.connectMessage = 'Connecting to ' + this.exeName + ', ' + this.$.commLinkInput.value;
        }
        else {
            this.connectMessage = 'Connecting to ' + this.$.commLinkInput.value;
        }
        this.$.connectErrorIcon.hidden = true;
        this.$.connectMessageDiv.hidden = false;
        this.$.connectProgress.hidden = false;
        this.$.connectButton.disabled = true;
        this.initRov(useProgram ? '' : this.$.exeInput.value, this.$.commLinkInput.value);
        this.rovInit.progressInfo.notifyCount = 0;
        this.$.connectProgress.value = this.rovInit.progressInfo.step;
        this.postConnectMenuState();
    },
    connectDialogInputOnFocus : function (e) {
        if (e.currentTarget.id == 'exeInput') {
            if (!this.exeInputFocused) {
                this.exeInputFocused = true;
            }
            else {
                e.currentTarget.$.input.select();
            }
        }
        else {
            e.currentTarget.$.input.select();
        }
    },
    connectKeyPress : function (e) {
        var keyCode = e.keyCode || e.which;
        if (keyCode === 13 && this.$.connectButton.disabled == false) {
            this.connectClicked(e);
        }
    },
    connectProgressCancelClicked : function (e) {
        if (this.setExecutableReqObj != null) {
            this.rovData.cancelConnectWait(this.setExecutableReqObj);
        }
    },
    createAddOn : function (e, path) {
        var addOn = {};
        addOn.path = path;
        addOn.element = this.getImportedAddOnId(e.target.import.activeElement);
        var elem = document.createElement(addOn.element);
        if (elem.viewName == undefined || elem.textContent.length == 0) {
            this.$.addOnErrorToast.text = 'Error creating element <' + addOn.element + '>';
            this.$.addOnErrorToast.fitInto = this.$.manageAddOnsDialog;
            this.$.addOnErrorToast.open();
            Polymer.dom.flush();
            return (false);
        }
        addOn.name = elem.viewName;
        addOn.href = e.target.href;
        if (path.indexOf('markdown') == -1) {
            var other = (this.otherViews.length > 0) ? rovUtils.shallowCopy(this.otherViews) : [];
            other.unshift(addOn);
            this.set('otherViews', other);
            this.set('dlgOtherViews', other);
            this.$.curAddOnsTemplate.render();
            this.$.selectAllCurrentAddOnsCheckbox.disabled = (this.dlgOtherViews.length == 0);
            this.$.curAddOnsList.selected = -1;
            this.$.removeAddOnButton.disabled = true;
            if (!this.enableOtherViews) {
                this.enableOtherViews = true;
                if (this.$$('#otherViewsButton')) {
                    this.$$('#otherViewsButton').disabled = true;
                }
                if (this.$$('#overflowOtherViewsButton')) {
                    this.$$('#overflowOtherViewsButton').disabled = true;
                }
            }
            this.saveAddOnMru(addOn.path);
            var rovSettings = this.getRovSettingsStorage();
            rovSettings.otherViews = this.otherViews;
            this.setRovSettingsStorage(rovSettings);
        }
        else {
            this.markdownAddon = addOn;
        }
        return (true);
    },
    curAddOnClicked : function (e) {
        if (!this.$.manageAddOnsDialog.opened) {
            return;
        }
        this.listItemWithSelectAllOptionClicked(e, 'selectAllCurrentAddOnsCheckbox',
                                                'curAddOnsList');
        this.enableChecklistDialogActionButton(e, 'curAddOnsList',
                                               'removeAddOnButton');
    },
    customViewClosed : function (id) {
        for (var i = 0; i < this.customViews.length; i++) {
            if (this.customViews[i].elem.id == id) {
                this.customViews.splice(i, 1);
                break;
            }
        }
    },
    customViewDataSubMenuItemClicked : function (e) {
        var elem = this.getCustomViewByName(e.currentTarget.textContent.trim());
        this.addCellToCustomView(this.$.plotViewDataContextDialog.dataProps, elem);
        elem.style.zIndex = String(this.getMaxZindex() + 1);
        this.$.plotViewDataContextDialog.close();
    },
    customViewFromGraph : function(trace) {
        var foundElem = null;
        for (var i = 0; i < this.rovViews.length; i++) {
            var elem = this.getViewById(this.rovViews[i].id);
            if (elem.name == 'ti-rov-customview' && elem.viewName == trace.viewName) {
                for (var j = 0; j < elem.rovCells.length; j++) {
                    if (elem.rovCells[j].displayColumnName == trace.displayColumnName) {
                        foundElem = elem;
                        break;
                    }
                }
                if (foundElem) {
                    break;
                }
            }
        }
        if (foundElem) {
            this.viewToForeground(foundElem);
            foundElem.$.tiRovTable.highlightCell({displayColumnName: trace.displayColumnName});
        }
        else {
            /* open a view */
            var offset = this.getNewClickedViewPosition();
            var newElem = this.newRovCustomView({tableName: trace.viewName}, trace.tableCells);
            newElem.highlightCell = {displayColumnName: trace.displayColumnName};
            if (this.inFullScreenMode) {
                this.maximizeNewView(newElem, offset);
            }
            else {
                if (offset.left > 0) {
                    newElem.style.left = offset.left + 'px';
                }
                if (offset.top > 0) {
                    newElem.style.top = offset.top + 'px';
                }
            }
            this.viewToForeground(newElem);
        }
    },
    customViewNameChanged : function (prevName, newName) {
        for (var i = 0; i < this.plotViews.length; i++) {
            for (var j = 0; j < this.plotViews[i].traces.length; j++) {
                var trace = this.plotViews[i].traces[j];
                if (trace.viewName == prevName) {
                    trace.viewName = newName;
                    var rovTraces = this.plotViews[i].elem.rovTraces;
                    for (var k = 0; k < rovTraces.length; k++) {
                        if (rovTraces[k].viewName == prevName) {
                            rovTraces[k].viewName = newName;
                            break;
                        }
                    }
                }
            }
        }
    },
    dashboardButtonsState : function() {
        var hasSavedDashboards = this.hasSavedDashboards();
        this.$.openDashboardSubMenu.disabled = !hasSavedDashboards;
        this.$.openDashboardButton.disabled = !hasSavedDashboards;
        this.$.deleteDashboardSubMenu.disabled = !hasSavedDashboards;
        this.$.deleteDashboardButton.disabled = !hasSavedDashboards;
        this.$.exportDashboardsClicked.disabled = !hasSavedDashboards;
        var allViewsLength = this.rovModuleViews.length + this.rovViews.length;
        this.$.saveDashboardClicked.disabled = allViewsLength == 0;
        if (allViewsLength == 0) {
            this.setCurrentDashboard('');
        }
    },
    dashboardConflictsDialog : function () {
        var dialog = this.$.overwriteDashboardsDialog;
        var names = [];
        for (var i = 0; i < this.dashboardConflicts.length; i++) {
            names.push(this.dashboardConflicts[i].name);
        }
        names.sort();
        this.set('overwriteDashboardsItems', names);
        dialog.open();
        dialog.style.zIndex = String(this.getMaxZindex() + 10);

        /* Init checkboxes and clear selection button */
        var listItems = this.$.overwriteDashboardsList.items;
        for (var i = 0; i < listItems.length; i++) {
            listItems[i].lastElementChild.checked = true;
        }
        this.$.overwriteDashboardsList.selected = -1;
        this.$.overwriteDashboardsButton.disabled = false;
    },
    dashboardDrag : function (e, dragOp) {
        if (e.preventDefault) {
            e.preventDefault();
        }
        if (this.ccs7) {
            if (dragOp == 'dragover') {
                e.dataTransfer.dropEffect = 'none';
            }
            return;
        }
        if (this.isOverOpenButton(e)) {
            e.dataTransfer.dropEffect = 'copy';
            this.$.dashboardOptionsButton.style.color =  'dodgerblue';
        }
        else {
            if (dragOp == 'dragover') {
                e.dataTransfer.dropEffect = 'none';
            }
            this.$.dashboardOptionsButton.style.color =  '';
        }
    },
    dashboardDragEnter : function (e) {
        this.dashboardDrag(e);
    },
    dashboardDragOver : function (e) {
        this.dashboardDrag(e, 'dragover');
    },
    dashboardFileLoaded : function (file, e) {
        var valid = false;
        var error = null;
        try {
            var dashboard = JSON.parse(e.target.result);
            if (rovUtils.isArray(dashboard)) {
                if (dashboard[0].moduleName || dashboard[0].elemName) {
                    valid = true;
                    if (this.rovModuleViews.length + this.rovViews.length > 0) {
                        this.$.openingDashboardAlertDialog.dashboard = dashboard;
                        this.$.openingDashboardAlertDialog.rovSel = null;
                        this.$.openingDashboardAlertDialog.open();
                        this.$.openingDashboardAlertDialog.style.zIndex = String(this.getMaxZindex() + 1);
                    }
                    else {
                        var name = dashboard.pop();
                        var savedDashboards = Object.keys(this.dashboards);
                        if (savedDashboards.length > 0 && savedDashboards.indexOf(name) >= 0) {
                            this.confirmDialog('Overwrite dashboard',
                                               'Overwrite ' + name + ' ?',
                                               'dashboardConflict',
                                               {name: name, dashboard: dashboard,
                                                show: true, file: file});
                        }
                        else {
                            this.saveDroppedDashboard(name, dashboard, true);
                        }
                    }
                }
                else if (dashboard[0].fileName && dashboard[0].numDashboards) {
                    valid = true;
                    dashboard.shift();
                    this.dashboardConflicts = [];
                    this.dashboardsArrayLoaded(dashboard, true);
                }
            }
        }
        catch (e) {
            error = e;
        }
        if (!valid) {
            this.showStatus(file.name + ' is not a valid dashboard file', 'error');
        }
        if (error) {
            this.showStatus(error, 'error');
        }
    },
    dashboardFileReader : function (file) {
        var reader = new FileReader();
        reader.onload = this.dashboardFileLoaded.bind(this, file);
        reader.readAsText(file);
    },
    dashboardOptionsClicked : function (e) {
        this.initDashboardsDialogList();
        var dialog = this.$.dashboardOptionsMenuDialog;
        this.clearDashboardOptionsMenuDialogState();
        dialog.open();
        this.dashboardButtonsState();
    },
    dashboardOptionsItemSelected : function (e) {
        if (e.detail.item &&
            e.detail.item.id.indexOf('Clicked') > 0) {
            var selected = e.detail.item.id;
            this.clearDashboardOptionsMenuDialogState();
            this.$.dashboardOptionsMenuDialog.close();
            this[selected](e); /* id of the item is the function name */
        }
        else if (e.srcElement.id.indexOf('SubMenu') > 0) {
            this.$.dashboardOptionsMenu.selected = -1;
            this.$.openDashboardSubMenu.selected = -1;
            this.$.deleteDashboardSubMenu.selected = -1;
            var menuId = e.srcElement.id.replace(/SubMenu/, 'Menu');
            var hidden = !this.$[menuId].hidden;
            this.$[menuId].hidden = hidden;
            if (!hidden) {
                if (menuId.indexOf('open') >= 0) {
                    if (!this.$.deleteDashboardMenu.hidden) {
                        this.$.deleteDashboardMenu.hidden = true;
                    }
                }
                else if (!this.$.openDashboardMenu.hidden) {
                    this.$.openDashboardMenu.hidden = true;
                }
            }
        }
    },
    dashboardOptionsMenuDialogOpened : function (e) {
        var dialog = e.currentTarget;
        if (dialog.style.width == '') {
            dialog.style.maxWidth = String(e.currentTarget.clientWidth + 16) + 'px';
            dialog.style.width = dialog.style.maxWidth;
        }
    },
    dashboardsArrayLoaded : function (dashboardsArr, done) {
        var savedDashboards = Object.keys(this.dashboards);
        for (var i = 0; i < dashboardsArr.length; i++) {
            var name = dashboardsArr[i].pop();
            if (savedDashboards.length > 0 && savedDashboards.indexOf(name) >= 0) {
                this.dashboardConflicts.push({name: name, dashboard: dashboardsArr[i], file: null});
            }
            else {
                this.dashboards[name] = dashboardsArr[i];
                rovUtils.setLocalStorageObject('dashboards-storage', this.dashboards);
                this.showStatus('Saved dashboard ' + name, 'info');
                this.dashboardButtonsState();
            }
        }
        if (this.dashboardConflicts.length > 0 && done) {
            this.resolveSaveDashboardsConflicts();
        }
    },
    /*
     * dashboard storage {
     *    id         : '',
     *    moduleName : '',
     *    viewName   : '',
     *    left       : '',
     *    top        : '',
     *    width      : '',
     *    height     : '',
     *    zIndex     : ''
     * }
     *
     */
    dashboardSelected : function(e) {
        var dialog = e.currentTarget.offsetParent;
        if (dialog) {
            if (dialog.id.indexOf('list') == 0 || dialog.id.indexOf('intro') == 0) {
                var sel = e.currentTarget.selectedItem.textContent.trim();
                if (dialog.name == 'Save') {
                    dialog.close();
                    this.$.saveDashboardInput.value = sel;
                    this.$.saveDashboardComment.value = this.dashboards[sel][0].comment ?
                                                        this.dashboards[sel][0].comment : '';
                }
                else {
                    this.$.introDialog.close();
                    dialog.close();
                    this.showSelectedDashboard(sel, true);
                }
            }
            else if (dialog.id == 'dashboardOptionsMenuDialog') {
                if (e.currentTarget.id == "openDashboardMenu") {
                    var sel = e.detail.item.innerText.trim();
                    if ((this.rovModuleViews.length + this.rovViews.length) > 0) {
                        this.$.openingDashboardAlertDialog.dashboard = null;
                        this.$.openingDashboardAlertDialog.rovSel = sel;
                        this.$.openingDashboardAlertDialog.open();
                        this.$.openingDashboardAlertDialog.style.zIndex = String(this.getMaxZindex() + 1);
                    }
                    else {
                        this.showSelectedDashboard(sel, true);
                    }
                }
                else if (e.currentTarget.id == "deleteDashboardMenu") {
                    var sel = e.detail.item.innerText.trim();
                    this.confirmDialog('Delete dashboard',
                                       'Delete ' + sel + ' ?',
                                       'dashboard', sel);
                }
            }
        }
    },
    dataOptionsClicked : function (e) {
        var dialog = this.$.dataOptionsDialog;
        dialog.resetFit();
        dialog.addEventListener('iron-overlay-closed', function() {
            var reason = this.closingReason;
            return (false);
        });
        dialog.open();
        dialog.style.left = this.isNarrow() ? '0px': rovUtils.leftPaneWidth + 'px';
        if (this.$.connectDialog.hidden == true) {
            dialog.style.zIndex = String(this.getMaxZindex() + 10);
        }
        else {
            var elemStyle = window.getComputedStyle(this.$.connectDialog);
            dialog.style.zIndex = String(Number(elemStyle.zIndex) + 10);
            var top = Number(elemStyle.top.replace(/px/, '')) + 10;
            dialog.style.top = top + 'px';
        }
        this.$.repeatRefreshInput.value = String(this.refreshInterval);
        this.$.repeatRefreshInputInvalid.hidden = true;
        this.$.dataTimeoutInput.value = String(this.dataTimeoutPeriod);
        this.$.dataTimeoutInputInvalid.hidden = true;
        this.$.connectTimeoutInput.value = String(this.connectTimeoutPeriod);
        this.$.connectTimeoutInputInvalid.hidden = true;
    },
    dataOptionsKeyPress : function (e) {
        var keyCode = e.keyCode || e.which;
        if (keyCode === 13 && this.$.repeatRefreshInput.value != '' && this.$.dataTimeoutInput.value != '') {
            this.saveDataOptionsOkClicked(e);
        }
    },
    dataViewsShowing : function () {
        var count = this.moduleViewsShowing();
        for (var i = 0; i < this.rovViews.length; i++) {
            var elem = this.getViewById(this.rovViews[i].id);
            if (elem) {
                if (elem.hasViewableContent && elem.hasViewableContent()) {
                    ++count;
                }
            }
            else {
                console.error('Undefined view, id: ' + this.rovViews[i].id);
            }
        }
        return (count);
    },
    debuggerEvent : function (event) {
        if (event == 'TargetHalt') {
            if (this.inRepeatRefresh) {
                this.stopRepeatRefresh(true);
            }
            this.refreshAllClicked();
        }
    },
    deleteDashboard : function(sel) {
        var dashboards = {};
        for (var name in this.dashboards) {
            if (name != sel) {
                dashboards[name] = this.dashboards[name];
            }
        }
        if (sel == this.currentDashboard) {
            this.setCurrentDashboard('');
        }
        this.dashboards = dashboards;
        rovUtils.setLocalStorageObject('dashboards-storage', this.dashboards);

        /* refesh dashboard options menu */
        this.initDashboardsDialogList();
        this.dashboardButtonsState();
        if (this.dashboardNames.length == 0) {
            this.$.dashboardOptionsMenuDialog.close();
        }
    },
    deleteDashboardClicked : function(e) {
        this.dashboardSelected(e);
        this.$.deleteDashboardMenu.selected = -1;
    },
    deleteModuleStorageClicked : function (e) {
        var dialog = this.$.deleteModuleStorageDialog;
        dialog.resetFit();
        var viewsStorage = this.getViewsDataStorage();
        var keys = Object.keys(viewsStorage);
        var nonNullKeys = [];
        var maxWidth = 0;
        for (var i = 0; i < keys.length; i++) {
            if (viewsStorage[keys[i]]) {
                nonNullKeys.push(keys[i]);
                var width = rovUtils.getStringWidth(keys[i]);
                if (width > maxWidth) {
                    maxWidth = width;
                }
            }
        }
        nonNullKeys.sort();
        this.set('viewsStorageItems', nonNullKeys);
        dialog.open();
        this.$.viewsStorageList.style.width = maxWidth + 'px';
        dialog.style.left = this.isNarrow() ? '0px': rovUtils.leftPaneWidth + 'px';
        dialog.style.zIndex = String(this.getMaxZindex() + 10);

        /* Init checkboxes and clear selection button */
        var listItems = this.$.viewsStorageList.items;
        for (var i = 0; i < listItems.length; i++) {
            listItems[i].lastElementChild.checked = false;
        }
        this.$.viewsStorageList.selected = -1;
        this.$.clearViewStorageButton.disabled = true;
        this.$.selectAllViewsStorageCheckbox.checked = false;
        this.$.selectAllViewsStorageCheckbox.disabled = nonNullKeys.length == 0;
    },
    deleteModuleStorageKeyPress : function (e) {
        var keyCode = e.keyCode || e.which;
        if (keyCode === 13) {
            if (!this.$.clearViewStorageButton.disabled) {
                this.clearViewStorageClicked();
            }
            if (this.$.deleteModuleStorageDialog.opened) {
                this.$.deleteModuleStorageDialog.close();
            }
        }
    },
    deleteMruClicked : function (e) {
        var mruItem = e.currentTarget.parentElement;
        this.$[mruItem.parentElement.id].selected = -1;
        var pre = mruItem.parentElement.id.replace(/MruMenu/, '');
        var dialog = this.$[pre + 'MruDialog'];
        dialog.style.minWidth = '';
        var mrusProp = pre + 'Mrus';
        var mruList = rovUtils.shallowCopy(this[mrusProp]);
        var sel = mruItem.textContent.trim();
        var pos = mruList.indexOf(sel);
        mruList.splice(pos, 1);
        this.set(mrusProp, mruList);
        this.$[pre + 'MrusTemplate'].render();
        dialog.style.minWidth = dialog.clientWidth + 'px';
        this.rovSettings[mrusProp] = mruList;
        var rovSettings = this.getRovSettingsStorage();
        rovSettings[mrusProp] = this.rovSettings[mrusProp];
        this.setRovSettingsStorage(rovSettings);
    },
    destroyClickedElement : function(event) {
        document.body.removeChild(event.target);
    },
    dialogCloseClicked : function (e) {
        var dlgId = e.target.id.replace(/Close/, '');
        this.$[dlgId].cancel(e);
    },
    dialogClosed : function (e) {
        if (this.$.connectDialog.hidden == true) {
            /* re-enable toolbar buttons */
            this.$.rovOptionsButton.disabled = false;
            this.$.dashboardOptionsButton.disabled = false;
            if (this.$$('#otherViewsButton')) {
                this.$$('#otherViewsButton').disabled = false;
            }
            if (this.$$('#overflowOtherViewsButton')) {
                this.$$('#overflowOtherViewsButton').disabled = false;
            }
            this.toolbarButtonsState();
            this.dashboardButtonsState();
        }
        else {
            this.$.rovOptionsButton.disabled = false;
            this.$.dashboardOptionsButton.disabled = false;
            this.$.connectButton.disabled = false;
        }
    },
    dialogOpened : function () {
        if (this.$.connectDialog.hidden == true) {
            this.disableAllButtons();
        }
        else {
            this.$.rovOptionsButton.disabled = true;
            this.$.dashboardOptionsButton.disabled = true;
            this.$.connectButton.disabled = true;
        }
    },
    disableAllButtons : function () {
        this.$.viewToTopButton.disabled = true;
        this.$.saveAllButton.disabled = true;
        this.$.repeatRefreshButton.disabled = true;
        this.$.overflowRepeatRefreshButton.disabled = true;
        this.$.refreshAllButton.disabled = true;
        this.$.overflowRefreshAllButton.disabled = true;
        this.$.closeAllButton.disabled = true;
        this.$.overflowCloseAllButton.disabled = true;
        this.$.titleOverflowDialogButton.disabled = true;
        if (this.$$('#otherViewsButton')) {
            this.$$('#otherViewsButton').disabled = true;
        }
        if (this.$$('#overflowOtherViewsButton')) {
            this.$$('#overflowOtherViewsButton').disabled = true;
        }
        this.$.fullScreenButton.disabled =  true;
        this.$.overflowFullScreenButton.disabled = true;
        this.$.exitFullScreenButton.disabled = true;
        this.$.overflowExitFullScreenButton.disabled = true;
        this.$.dashboardOptionsButton.disabled = true;
        this.$.rovOptionsButton.disabled = true;
    },
    discoverAddOns : function () {
        this.$.rediscoverAddOnButton.disabled = true;
        this.rovData.discoverAddOns(this);
    },
    discoverAddOnsCallback : function (res, addOns) {
        if (res == null) {
            var discoveredAddOns = [];
            for (var i = 0; i < addOns.length; i++) {
                if (addOns[i].name == 'Markdown View' && !this.$.manageAddOnsDialog.opened) {
                    this.markdownAddon = rovUtils.shallowCopy(addOns[i]);
                    this.importHref(this.markdownAddon.path + '/markdown.html',
                        function(e){
                            this.createAddOn(e, this.markdownAddon.path);
                        },
                        function(e){
                            this.showStatus('Error importing Markdown addon', 'error');
                            this.markdownAddon = null;
                        }
                    );
                }
                else {
                    discoveredAddOns.push({path : addOns[i].path,
                                           name : addOns[i].name});
                }
            }
            this.set('discoveredAddOns',  discoveredAddOns);
            if (this.$.manageAddOnsDialog.opened) {
                this.populateDiscoveredAddOns();
                var listItems = this.$.discoveredAddOnsList.items;
                for (var j = 0; j < listItems.length; j++) {
                    listItems[j].lastElementChild.checked = false;
                }
                this.$.addDiscoveredAddOnButton.disabled = true;
                this.$.selectAllDiscoveredAddOnsCheckbox.checked = false;
                this.$.selectAllDiscoveredAddOnsCheckbox.disabled = (discoveredAddOns.length == 0);
            }
        }
        else {
            if (this.$.manageAddOnsDialog.opened) {
                this.$.addOnErrorToast.text = res;
                this.$.addOnErrorToast.fitInto = this.$.manageAddOnsDialog;
                this.$.addOnErrorToast.open();
                return (false);
            }
            else {
                this.showStatus(res, 'error');
                return (false);
            }
        }
        this.$.rediscoverAddOnButton.disabled = false;
        if (this.rovInit.restoreViews && !this.$.manageAddOnsDialog.opened) {
            this.restoreViewsState();
        }
        return (true);
    },
    discoveredAddOnClicked : function (e) {
        if (!this.$.manageAddOnsDialog.opened) {
            return;
        }
        this.listItemWithSelectAllOptionClicked(e, 'selectAllDiscoveredAddOnsCheckbox',
                                                'discoveredAddOnsList');
        this.enableChecklistDialogActionButton(e, 'discoveredAddOnsList',
                                               'addDiscoveredAddOnButton');
    },
    dlgClicked : function(e) {
        if (this.$.connectDialog.hidden == true) {
            e.currentTarget.style.zIndex = String(this.getMaxZindex() + 1);
        }
    },
    drawerDragEnter : function (event) {
        event.preventDefault();
    },
    drawerDragOver : function (event) {
        event.preventDefault();
        event.dataTransfer.dropEffect = 'none';
    },
    drawerDragStart : function (event) {
        event.preventDefault();
    },
    drawerDrop : function (event) {
        event.preventDefault();
    },
    enableChecklistDialogActionButton : function (e, checklistId, buttonId) {
        var btnDisabled = true;
        if (e && e.currentTarget.checked) {
            btnDisabled = false;
        }
        else {
            var listItems = this.$[checklistId].items;
            for (var i = 0; i < listItems.length; i++) {
                if (listItems[i].lastElementChild.checked) {
                    btnDisabled = false;
                    break;
                }
            }
        }
        this.$[buttonId].disabled = btnDisabled;
    },
    evalPrevViewState : function (elem) {
        var cardWidth = null;
        var cardHeight;
        var viewContentDivPosition = null;
        if (elem.moduleName) {
            var viewsData = this.getViewsDataStorage();
            var key = elem.moduleName + '.' + elem.viewName;
            if (viewsData[key] && viewsData[key].width) {
                cardWidth = viewsData[key].width;
                cardHeight = viewsData[key].height;
                viewContentDivPosition = 'absolute';
            }
            if (elem.viewChanged) {
                elem.viewChanged = false;
            }
        }
        if (elem.prevState.openedMaximized) {
            if (cardWidth == null) {
                cardWidth = '';
                cardHeight = '';
                if (elem.moduleName) {
                    viewContentDivPosition = 'static';
                }
            }
            elem.prevState.cardWidth = cardWidth;
            elem.prevState.cardHeight = cardHeight;
            if (viewContentDivPosition) {
                elem.prevState.viewContentDivPosition = viewContentDivPosition;
            }
            elem.prevState.viewHeight = '';
            elem.prevState.viewWidth = '';
        }
    },
    exportADashboardChecked : function (e) {
        if (!this.$.exportDashboardsDialog.opened) {
            return;
        }
        this.listItemWithSelectAllOptionClicked(e, 'selectAllExportDashboardsCheckbox',
                                                'exportDashboardsList');
        if (this.$.exportDashboardsInput.value == '') {
            this.$.exportDashboardsButton.disabled = true;
        }
        else {
            this.enableChecklistDialogActionButton(e, 'exportDashboardsList',
                                                   'exportDashboardsButton');
        }
    },
    exportDashboard : function(dashName, dashArr) {
        dashArr.push(dashName);
        var textToWrite = JSON.stringify(dashArr, null, '\t');
        dashArr.pop();
        var fileNameToSaveAs = (dashName + '.rov.json');
        rovUtils.downloadText(textToWrite, fileNameToSaveAs);
    },
    exportDashboards : function(fileName, dashboardsArr) {
        var fileNameToSaveAs = (fileName + '.rov.json');
        dashboardsArr.unshift({fileName: fileNameToSaveAs, numDashboards: dashboardsArr.length});
        var textToWrite = JSON.stringify(dashboardsArr, null, '\t');
        rovUtils.downloadText(textToWrite, fileNameToSaveAs);
    },
    exportDashboardsButtonClicked : function (e) {
        var fileName = this.$.exportDashboardsInput.value;
        var checkedDashboards = [];
        var listItems = this.$.exportDashboardsList.items;
        for (var i = 0; i < listItems.length; i++) {
            if (listItems[i].lastElementChild.checked) {
                checkedDashboards.push(listItems[i].innerText.trim());
            }
        }
        var dashArr = [];
        for (var i = 0; i < checkedDashboards.length; i++) {
            var dashboard = this.dashboards[checkedDashboards[i]];
            dashboard.push(checkedDashboards[i]);
            dashArr.push(rovUtils.shallowCopy(dashboard));
            dashboard.pop();
        }
        this.$.exportDashboardsDialog.close();
        this.$.exportDashboardsList.selected = -1;
        this.$.exportDashboardsInput.value = '';
        this.$.exportDashboardsButton.disabled = true;
        this.exportDashboards(fileName, dashArr);
    },
    exportDashboardsClicked : function (e) {
        this.initDashboardsDialogList();
        var dialog = this.$.exportDashboardsDialog;
        dialog.resetFit();
        dialog.open();
        dialog.style.left = this.isNarrow() ? '0px': rovUtils.leftPaneWidth + 'px';
        if (this.$.connectDialog.hidden == true) {
            dialog.style.zIndex = String(this.getMaxZindex() + 10);
        }
        else {
            var elemStyle = window.getComputedStyle(this.$.connectDialog);
            dialog.style.zIndex = String(Number(elemStyle.zIndex) + 10);
            var top = Number(elemStyle.top.replace(/px/, '')) + 10;
            dialog.style.top = top + 'px';
        }

        /* Init checkboxes and clear selection button */
        var listItems = this.$.exportDashboardsList.items;
        for (var i = 0; i < listItems.length; i++) {
            listItems[i].lastElementChild.checked = true;
        }
        this.$.exportDashboardsList.selected = -1;
        if (this.ccs7) {
            this.$.exportDashboardsInput.hidden = true;
            this.$.exportDashboardsInputDiv.style.height = '0px';
            this.$.exportDashboardsDialog.style.height = '240px';
            this.$.exportDashboardsInput.value = 'dashboards';
            this.$.exportDashboardsButton.disabled = false;
        }
        else {
            this.$.exportDashboardsInput.value = '';
            this.$.exportDashboardsButton.disabled = true;
        }
    },
    exportDashboardsKeyPress : function (e) {
        var keyCode = e.keyCode || e.which;
        if (keyCode === 13 && this.$.exportDashboardsButton.disabled == false) {
            this.exportDashboardsButtonClicked(e);
        }
    },
    exportDashboardsOnInput : function (e) {
        if (e.currentTarget.value == '') {
            this.$.exportDashboardsButton.disabled = true;
        }
        else {
            this.enableChecklistDialogActionButton(null, 'exportDashboardsList',
                                                   'exportDashboardsButton');
        }
    },
    exportLocalStorageClicked : function (e) {
        if (!this.ccs7) {
            var dialog = this.$.exportLocalStorageDialog;
            dialog.resetFit();
            dialog.addEventListener('iron-overlay-closed', function() {
                var reason = this.closingReason;
                return (false);
            });
            dialog.open();
            dialog.style.left = e.x + 'px';
            dialog.style.top = (e.y - 60) + 'px';
            if (this.$.connectDialog.hidden == true) {
                dialog.style.left = e.x + 'px';
                dialog.style.top = (e.y - 60) + 'px';
                dialog.style.zIndex = String(this.getMaxZindex() + 1);
            }
            else {
                var elemStyle = window.getComputedStyle(this.$.connectDialog);
                var left = Number(elemStyle.width.replace(/px/, ''));
                var dlgStyle = window.getComputedStyle(dialog);
                var max = Number(dlgStyle.maxWidth.replace(/px/, ''));
                if ((left + max) > document.documentElement.clientWidth) {
                    left = document.documentElement.clientWidth - max;
                }
                dialog.style.left = left + 'px';
                var top = Number(elemStyle.top.replace(/px/, ''));
                var height = Number(elemStyle.minHeight.replace(/px/, ''));
                dialog.style.top = top + (height / 4) + 'px';
                dialog.style.zIndex = String(Number(elemStyle.zIndex) + 10);
            }
        }
        else {
            this.exportLocalStorageOKClicked('localStorage');
        }
    },
    exportLocalStorageKeyPress : function (e) {
        var keyCode = e.keyCode || e.which;
        if (keyCode === 13 && this.$.exportLocalStorageInput.value != '') {
            this.exportLocalStorageOKClicked(e);
        }
    },
    exportLocalStorageOKClicked : function (arg) {
        var exportName = (typeof(arg) == 'string') ? arg : this.$.exportLocalStorageInput.value;
        if (exportName.length) {
            this.$.exportLocalStorageInput.value = '';
            this.$.exportLocalStorageDialog.close();
            if (this.rovInit.restoreViews) {
                this.saveViewsState();
            }
            var allStorage = [];
            allStorage.push(rovUtils.getLocalStorageObject('rovSettings-storage'));
            allStorage.push(rovUtils.getLocalStorageObject('dashboards-storage'));
            allStorage.push(rovUtils.getLocalStorageObject('favorites-storage'));
            allStorage.push(rovUtils.getLocalStorageObject('rovInit-storage'));
            allStorage.push(rovUtils.getLocalStorageObject('viewsData-storage'));
            allStorage.push(rovUtils.getLocalStorageObject('currentViews-storage'));

            if (this._ccs7) {
                allStorage.push(rovUtils.getLocalStorageObject('coreNames-storage'));
                allStorage.push(this.getCoresStorage());
            }

            var textToWrite = JSON.stringify(allStorage, null, '\t');
            var fileNameToSaveAs = (exportName + '.ros.json');
            rovUtils.downloadText(textToWrite, fileNameToSaveAs);
            this.saveExportLocalStorageMru(exportName);
        }
    },
    filterLeftClicked : function(e) {
        this.set('showFilterButtonRight', true);
        this.set('showFilterButtonLeft', false);
    },
    filterModules : function(filterStr) {
        var exp = RegExp('^' + filterStr, 'i');
        var filterArr = (this.curModsLabel == 'All Modules') ? this.allModules : this.viewableModules;
        var foundModules = [];
        for (var i = 0; i < filterArr.length; i++) {
            if (String(filterArr[i]).search(exp) == 0) {
                foundModules.push(filterArr[i]);
            }
        }
        if (foundModules.length == 0 && filterStr.length > 2) {
            exp = RegExp(filterStr + '$', 'i');
            var exp2 = RegExp(filterStr, 'i');
            for (var i = 0; i < filterArr.length; i++) {
                if (String(filterArr[i]).search(exp) > 0) {
                    foundModules.push(filterArr[i]);
                }
                else if (String(filterArr[i]).search(exp2) > 0) {
                    foundModules.push(filterArr[i]);
                }
            }
        }
        var equal = rovUtils.strArrCompare(this.curTocModules, foundModules);
        if (!equal) {
            this.set('curTocModules', foundModules);
            this.set('tocItems', this.makeModulesToc(foundModules));
        }
        return (!equal);
    },
    filterModulesChanged : function(e) {
        this.filterValue = e.currentTarget.value;
        this.filterModules(this.filterValue);
    },
    filterRightClicked : function(e) {
        if (this.allModules.length > 0) {
            this.set('showFilterButtonRight', false);
            this.set('showFilterButtonLeft', true);
        }
    },
    fullScreenToggleClicked : function (e) {
        this.toggleFullScreen(e.currentTarget.id == 'fullScreenButton');
    },
    getAddOnHashKey : function (href) {
        var key = href;
        key = key.substr(key.indexOf('href=') + 6);
        return (key.substr(0, key.indexOf('"')));
    },
    getAddOnInfo : function (elemName, path) {
        var info = null;
        for (var i = 0; i < this.otherViews.length; i++) {
            if (this.otherViews[i].element == elemName && this.otherViews[i].path == path) {
                info = this.otherViews[i];
                break;
            }
        }
        return (info);
    },
    getAllModules : function () {
        return (this.allModules);
    },
    getCustomViewByName : function (tableName) {
        for (var i = 0; i < this.rovViews.length; i++) {
            var elem = this.getViewById(this.rovViews[i].id);
            if (elem.name == 'ti-rov-customview' && elem.tableName == tableName) {
                break;
            }
        }
        return (elem);
    },
    getImportedAddOnId : function (activeElement) {
        for (var i = 0; i < activeElement.children.length; i++) {
            if (activeElement.children[i].localName == 'dom-module') {
                return (activeElement.children[i].id);
            }
        }
        return ('');
    },
    getMaxZindex : function () {
        var max = 0;
        for (var i = 0; i < this.rovModuleViews.length; i++) {
            var elem = this.getViewById(this.rovModuleViews[i].id);
            if (elem.style.zIndex != '') {
                if (Number(elem.style.zIndex) > max) {
                    max = Number(elem.style.zIndex);
                }
            }
        }
        for (var i = 0; i < this.rovViews.length; i++) {
            var elem = this.getViewById(this.rovViews[i].id);
            if (elem.style.zIndex != '') {
                if (Number(elem.style.zIndex) > max) {
                    max = Number(elem.style.zIndex);
                }
            }
        }
        return (max);
    },
    getNewClickedViewPosition : function () {
        var offset = (this.rovModuleViews.length + this.rovViews.length - 1) * 28;
        var left = offset;
        var top = offset;
        if (this.viewContainer.scrollLeft > 0) {
            left += this.viewContainer.scrollLeft;
        }
        if (this.viewContainer.scrollTop > 0) {
            top += this.viewContainer.scrollTop;
        }
        return ({left : left, top : top});
    },
    getNewModuleViewId : function () {
        var maxId = -1;
        for (var i = 0; i < this.rovModuleViews.length; i++) {
            var arr = this.rovModuleViews[i].id.split('_');
            if (Number(arr[1]) > maxId) {
                maxId = Number(arr[1]);
            }
        }
        return ('rovModuleView_' + (maxId + 1));
    },
    getNewViewId : function () {
        var maxId = -1;
        for (var i = 0; i < this.rovViews.length; i++) {
            var arr = this.rovViews[i].id.split('_');
            if (Number(arr[1]) > maxId) {
                maxId = Number(arr[1]);
            }
        }
        return ('rovView_' + (maxId + 1));
    },
    getNextZindexView : function (previous, moduleName) {
        var nextElem;
        var nextIndex;
        var max = 0;
        var maxIndex = -1;
        var viewElems = [];
        for (var i = 0; i < this.rovModuleViews.length; i++) {
            var elem = this.getViewById(this.rovModuleViews[i].id);
            viewElems.push(elem);
        }
        for (var i = 0; i < this.rovViews.length; i++) {
            var elem = this.getViewById(this.rovViews[i].id);
            viewElems.push(elem);
        }
        for (var i = 0; i < viewElems.length; i++) {
            if (viewElems[i].style.zIndex != '') {
                if (Number(viewElems[i].style.zIndex) > max) {
                    max = Number(viewElems[i].style.zIndex);
                    maxIndex = i;
                }
            }
        }
        if (moduleName) { /* Get next in list with matching moduleName */
            nextIndex = (maxIndex == viewElems.length - 1) ? 0 : maxIndex + 1;
            nextElem = viewElems[nextIndex];
            for (var i = 0; i < viewElems.length; i++) {
                if (viewElems[nextIndex].localName == 'ti-rov-view' &&
                    viewElems[nextIndex].moduleName == moduleName) {

                    nextElem = viewElems[nextIndex];
                    break;
                }
                if (++nextIndex >= viewElems.length) {
                    nextIndex = 0;
                }
            }
        }
        else {
            if (previous) {
                nextElem = (maxIndex == 0) ? viewElems[viewElems.length - 1] : viewElems[maxIndex - 1];
            }
            else {
                nextElem = (maxIndex == viewElems.length - 1) ? viewElems[0] : viewElems[maxIndex + 1];
            }
        }
        return ({maxZ: max, nextElem : nextElem});
    },
    getParsedAddOnRootDir : function (addOn) {
        var parsed = addOn.replace(/\\/g,'/').replace(/\.html$/, '');
        return (parsed.substr(0, parsed.lastIndexOf('/')));
    },
    getSessionId : function () {
        return (this.sessionId);
    },
    getRovData : function () {
        return (this.rovData);
    },
    getTocIndex : function (id, tocItems) {
        var index = -1;
        for (var i = 0; i < tocItems.length; i++) {
            if (tocItems[i].id == id) {
                index = i;
                break;
            }
        }
        return (index);
    },

    getTocItemStr : function (e) {
        if (this.allModulesVisible) {
            var items = this.tocItems;
            var itemStr = 'tocItems.';
        }
        else {
            var items = this.favoriteTocItems;
            var itemStr = 'favoriteTocItems.';
        }
        if (e.currentTarget.className.indexOf('subcontent') >= 0) {
            var index = this.getTocSubItemIndex(e.currentTarget.id, items);
            itemStr += index.tocIndex + '.dups.' + index.dupIndex;
        }
        else {
            var index = this.getTocIndex(e.currentTarget.id, items);
            itemStr += String(index);
        }
        return (itemStr);
    },

    getTocSubItemIndex : function (id, tocItems) {
        var index = {tocIndex: -1, dupIndex: -1};
        for (var i = 0; i < tocItems.length; i++) {
            if (tocItems[i].hasDups) {
                for (j = 0; j < tocItems[i].dups.length; j++) {
                    var dup = tocItems[i].dups[j];
                    if (dup.id == id) {
                        index.tocIndex = i;
                        index.dupIndex = j;
                        break;
                    }
                }
            }
            if (index.tocIndex != -1) {
                break;
            }
        }
        return (index);
    },
    getViewableModules : function () {
        return (this.viewableModules);
    },
    getViewById : function(viewId) {
        var view;
        var chNodes = Polymer.dom(this.$.contentDiv).childNodes;
        for (var i = 0; i < chNodes.length; i++) {
            if (chNodes[i].id == viewId) {
                view = chNodes[i];
                break;
            }
        }
        if (view == undefined) {
            console.error('getViewById, undefined view, id: ' + viewId);
            for (var i = 0; i < chNodes.length; i++) {
                console.log('  child node[' + i + '] '+ chNodes[i]);
            }
        }
        return (view);
    },
    getViewByRovGraphName : function (graphName) {
        for (var i = 0; i < this.rovViews.length; i++) {
            var elem = this.getViewById(this.rovViews[i].id);
            if (elem.name == 'ti-rov-graph' && elem.graphName == graphName) {
                break;
            }
        }
        return (elem);
    },
    getViewList : function () {
        return (this.viewList);
    },
    getViewListCallback : function (error, viewList) {
        if (error == null) {
            this.viewList = viewList;
            var keys = Object.keys(this.viewList);
            var viewTabs;
            for (var i = 0; i < keys.length; i++) {
                this.allModules.push(keys[i]);
                viewTabs = rovUtils.getViewTabs(this.viewList, keys[i]);
                if (viewTabs.length > 1 ||
                   (viewTabs.length == 1 && viewTabs[0].type != 'RAW')) {

                   this.viewableModules.push(keys[i]);
                }
                var argsKeys = Object.keys(this.viewList[keys[i]].viewArgs);
                for (var j = 0; j < argsKeys.length; j++) {
                    var viewArg = this.viewList[keys[i]].viewArgs[argsKeys[j]];
                    for (var k = 0; k < viewArg.args.length; k++) {
                        viewArg.args[k].defaultValue = viewArg.args[k].value;
                    }
                }
            }
            this.setCurModsList(this.curModsLabel);
            this.discoverAddOns();
            if (this.rovInit.showIntro != false) {
                this.introDialog();
            }
        }
        else {
            this.showStatus(error, 'error');
            this.disableAllButtons();
        }
    },
    getViewSaveData : function (elem) {
        var viewData = elem.onSaveView();
        var info = {};
        if (elem.moduleName) {
            info.module = elem.moduleName;
        }
        info.view = elem.viewName;
        if (elem.argsId) {
            var viewArgs = (elem.viewArgs && elem.viewArgs[elem.argsId]) ?
                           elem.viewArgs[elem.argsId] :
                           elem.getViewArgs(elem.argsId);
            if (viewArgs) {
                info.args = {};
                for (var i = 0; i < viewArgs.args.length; i++) {
                    info.args[viewArgs.args[i].name] = viewArgs.args[i].value;
                }
            }
        }
        return (JSON.stringify([info, viewData], null, '\t'));
    },
    globalFixedFontChecked : function (e) {
        this.clearRovOptionsMenuDialogState();
        this.rovSettings.globalFixedFont = e.currentTarget.checked;
        var rovSettings = this.getRovSettingsStorage();
        rovSettings.globalFixedFont = this.rovSettings.globalFixedFont;
        this.setRovSettingsStorage(rovSettings);
    },
    hasSavedDashboards : function (e) {
        return (Object.keys(this.dashboards).length > 0);
    },
    hasView : function(moduleName, viewName) {
        var viewTabs = rovUtils.getViewTabs(this.viewList, moduleName);
        var found = false;
        for (var i = 0; i < viewTabs.length; i++) {
            if (viewName == viewTabs[i].name) {
                found = true;
                break;
            }
        }
        return (found);
    },
    hasViewsStorage : function () {
        var viewsStorage = this.getViewsDataStorage();
        var keys = Object.keys(viewsStorage);
        for (var i = 0; i < keys.length; i++) {
            if (viewsStorage[keys[i]]) {
                return (true);
            }
        }
        return (false);
    },
    hideStatus : function () {
        this.$.statusConsole.hidden = true;
        this.statusItems = [];
    },
    importLocalStorageClicked : function (e) {
        var isMac = navigator.appVersion.indexOf('Mac') != -1;
        this.rovStorageAccept = isMac ? '.ros,.json' : '.ros.json';
        if (window.rov_setFileFilter) {
            window.rov_setFileFilter(isMac ? 'ROV Local storage|*.ros;*.json' : 'ROV Local storage|*.ros.json');
        }
        this.$.importLocalStorageFileInput.value = '';
        this.$.importLocalStorageFileInput.click();
    },
    importLocalStorageFileSelected : function (e) {
        var files = e.currentTarget.files;
        if (files.length == 1) {
            var extMatch = files[0].name.match(/\.ros\.json$/) != null;
            if (!extMatch && navigator.appVersion.indexOf('Mac') != -1) {
                extMatch = files[0].name.match(/\.ros$/) != null;
            }
            if (extMatch) {
                this.localStorageFileReader(files[0]);
            }
            else {
                this.showStatus(files[0].name + ' is not a valid ROV storage file', 'error');
            }
            this.$.importLocalStorageFileInput.value = '';
        }
    },
    initDashboardsDialogList : function () {
        var namesSort = [];
        for (var name in this.dashboards) {
            namesSort.push(name);
        }
        namesSort.sort();
        var namesArr = [];
        for (var i = 0; i < namesSort.length; i++) {
            namesArr.push({name : namesSort[i], comment : this.dashboards[namesSort[i]][0].comment});
        }
        this.dashboardNames = namesArr;
    },
    initPlotViewDataContextMenu : function (dataProps) {
        var numLines = 0;
        if (this.plotViews.length == 0) {
            this.$.plotViewData.hidden = false;
            this.$.addToGraph.hidden = true;
            this.$.newGraph.hidden = true;
            ++numLines;
        }
        else {
            var plots = this.matchPlotViews(dataProps);
            if (plots != null) {
                this.$.plotViewData.hidden = true;
                this.$.newGraph.hidden = false;
                ++numLines;
                if (plots.different.length > 0) {
                    this.$.addToGraph.hidden = false;
                    ++numLines;
                    var plotViewDataSubMenuItems = [];
                    for (var i = 0; i < plots.different.length; i++) {
                        plotViewDataSubMenuItems.push(plots.different[i].elem.graphName);
                    }
                    this.set('plotViewDataSubMenuItems', plotViewDataSubMenuItems);
                }
                else {
                    this.$.addToGraph.hidden = true;
                }
            }
            else {
                this.$.plotViewData.hidden = false;
                this.$.addToGraph.hidden = true;
                this.$.newGraph.hidden = true;
                ++numLines;
            }
        }
        if (dataProps.customTable) {
            this.$.customViewData.hidden = true;
            this.$.addToCustomView.hidden = true;
            this.$.newCustomViewData.hidden = true;
        }
        else if (this.customViews.length == 0) {
            this.$.customViewData.hidden = false;
            this.$.addToCustomView.hidden = true;
            this.$.newCustomViewData.hidden = true;
            ++numLines;
        }
        else {
            var views = this.matchCustomViews(dataProps);
            if (views != null) {
                this.$.customViewData.hidden = true;
                this.$.newCustomViewData.hidden = false;
                ++numLines;
                if (views.different.length > 0) {
                    this.$.addToCustomView.hidden = false;
                    ++numLines;
                    var customViewDataSubMenuItems = [];
                    for (var i = 0; i < views.different.length; i++) {
                        customViewDataSubMenuItems.push(views.different[i].elem.tableName);
                    }
                    this.set('customViewDataSubMenuItems', customViewDataSubMenuItems);
                }
                else {
                    this.$.addToCustomView.hidden = true;
                }
            }
            else {
                this.$.customViewData.hidden = false;
                this.$.addToCustomView.hidden = true;
                this.$.newCustomViewData.hidden = true;
                ++numLines;
            }
        }
        return (numLines);
    },
    initRov : function (exe, commLink) {
        var fullPath = exe + '?' + 'comm=' + (commLink == 'Debugger' ? 'DSLite' : commLink);
        if ((commLink == 'DSLite' || commLink == 'Debugger') && window.ti != undefined) {
            fullPath = fullPath + ":" + window.ti.debug.cloudagent.dsPort;
        }
        this.setExecutableReqObj = null;
        this.rovData.setExecutable(fullPath, this);
    },
    introDashboardsClicked : function(event) {
        if (this.hasSavedDashboards()) {
            this.initDashboardsDialogList();
            var dialog = this.$.introListDashboardsDialog;
            dialog.resetFit();
            dialog.name = 'Intro';
            dialog.style.left = rovUtils.leftPaneWidth + event.currentTarget.offsetLeft + 4 + 'px';
            dialog.style.top = 43 + event.currentTarget.offsetTop + event.currentTarget.clientHeight - 5 + 'px';
            dialog.style.width = event.currentTarget.clientWidth + 'px';
            dialog.open();
            var introDialog = document.getElementById('introDialog');
            dialog.style.zIndex = String(Number(introDialog.style.zIndex) + 1);
            this.$.introDashboardNamesMenu.selected = -1;
        }
        else {
            this.openDashboardFileClicked();
            this.$.introDialog.close();
        }
    },
    introDialog : function () {
        var dialog = this.$.introDialog;
        dialog.name = 'Intro';
        dialog.open();
        dialog.style.position = 'fixed';
        dialog.style.left = rovUtils.leftPaneWidth + 'px !important';
        dialog.style.top = '48px !important';
        this.introMessage = 'Welcome to Runtime Object View. To begin';
        this.introModulesMessage = 'click on a module';
        if (this.hasSavedDashboards() || this.ccs7) {
            this.introMessage += ', either';
        }
        else {
            this.introModulesMessage += ' in this list';
            this.$.introOr.hidden = true;
            this.$.introDashboardsButton.hidden = true;
        }
    },
    introDialogClosed : function (e) {
        this.dialogClosed();
        var rovInit = this.getRovInitStorage();
        this.rovInit.showIntro = rovInit.showIntro = !this.$.noIntroCheckbox.checked;
        this.setRovInitStorage(rovInit);
    },
    isADragDropFile : function (e) {
        if (e.dataTransfer.types) {
            return (e.dataTransfer.types[0] == 'Files' ||
                    e.dataTransfer.types[0] == 'public.file-url');
        }
        else if (e.dataTransfer.files) {
            return (e.dataTransfer.files.length == 1);
        }
        return (false);
    },
    isDiscoveredAddOn : function (path) {
        var lowerPath = this.parseAddOnPath(path).toLowerCase();
        for (var i = 0; i < this.discoveredAddOns.length; i++) {
            var lowerOther = this.parseAddOnPath(this.discoveredAddOns[i].path).toLowerCase();
            if (lowerOther == lowerPath) {
                return (true);
            }
        }
        return (false);
    },
    isDupAddOn : function (path) {
        var lowerPath = path.toLowerCase();
        for (var i = 0; i < this.otherViews.length; i++) {
            if (lowerPath == this.otherViews[i].path.toLowerCase()) {
                return (true);
            }
        }
        return (false);
    },
    isNarrow : function () {
        return (this.$.rovDrawerPanel.narrow);
    },
    isOverOpenButton : function (e) {
        return ((e.target.id == 'dashboardOptionsButton' ||
                 (e.target.id == 'icon' && e.target.icon == 'dashboard')) &&
                 this.isADragDropFile(e));
    },
    listDashboardsDialog : function(dlgName, event) {
        this.initDashboardsDialogList();
        var id = (dlgName == 'Save') ? 'listSavedDashboardsDialog' : 'listDashboardsDialog';
        var dialog = this.$[id];
        dialog.resetFit();
        dialog.name = dlgName;
        if (dlgName == 'Save') {
            dialog.style.left = event.x + 'px';
            dialog.style.top = (event.y + 14) + 'px';
        }
        dialog.open();
        if (dlgName == 'Save') {
            var saveDialog = this.$.saveDashboardDialog;
            dialog.style.zIndex = String(Number(saveDialog.style.zIndex) + 1);
            this.$.listSavedDashboardsMenu.selected = -1;
        }
        else {
            dialog.style.zIndex = String(this.getMaxZindex() + 2);
            this.$.dashboardNamesMenu.selected = -1;
        }
    },
    listItemWithSelectAllOptionClicked : function (e, selectAllCheckboxId, checkListId) {
        if (!e.currentTarget.checked) {
            if (this.$[selectAllCheckboxId].checked) {
                this.$[selectAllCheckboxId].checked = false;
            }
        }
        else {
            this.sanityCheckSelectAllBox(selectAllCheckboxId, checkListId);
        }
        this.$[checkListId].selected = -1;
    },
    loadAddOnClicked : function (e) {
        if (this.$.addOnErrorToast.opened) {
            this.$.addOnErrorToast.text = '';
            this.$.addOnErrorToast.close();
        }
        var path = this.parseAddOnPath(this.$.addOnInput.value);
        if (this.isDupAddOn(this.getParsedAddOnRootDir(path))) {
            this.$.addOnErrorToast.text = path + ' has already been added';
            this.$.addOnErrorToast.fitInto = this.$.manageAddOnsDialog;
            this.$.addOnErrorToast.open();
            return;
        }
        this.addOnInputPath = this.$.addOnInput.value;
        this.importHref(path,
            function(e) {
                if (this.createAddOn(e, this.addOnInputPath)) {
                    this.$.addOnInput.value = '';
                    this.syncDiscoveredAddOnsList();
                    this.sanityCheckSelectAllBox('selectAllCurrentAddOnsCheckbox',
                                                 'curAddOnsList');
                }
            },
            function(e){
                this.$.addOnErrorToast.text = 'Error importing ' + e.target.href;
                this.$.addOnErrorToast.fitInto = this.$.manageAddOnsDialog;
                this.$.addOnErrorToast.open();
            }
          );
    },
    loadMissingSavedAddOn : function(savedAddOnView, showIt) {
        var path = this.parseAddOnPath(savedAddOnView.elemPath);
        this.addOnsHash[path] = savedAddOnView;
        this.importHref(path,
            function(e){
                var key = this.getAddOnHashKey(e.target.outerHTML);
                var savedItem = this.addOnsHash[key];
                var elem = document.createElement(savedItem.elemName);
                if (elem.viewName == undefined || elem.textContent.length == 0) {
                    this.showStatus('Error creating ' + savedItem.viewName + ' element', 'error');
                    Polymer.dom.flush();
                }
                else {
                    var addOn = {};
                    addOn.element = savedItem.elemName;
                    addOn.path = savedItem.elemPath;
                    addOn.name = elem.viewName;
                    addOn.href = e.target.href;
                    if (showIt) {
                        this.newSavedView(savedItem, 'addOn');
                    }
                    var other = (this.otherViews.length > 0) ? rovUtils.shallowCopy(this.otherViews) : [];
                    other.push(addOn);
                    this.set('otherViews', other);
                    this.enableOtherViews = true;
                    var rovSettings = this.getRovSettingsStorage();
                    rovSettings.otherViews = this.otherViews;
                    this.setRovSettingsStorage(rovSettings);
                }
            },
            function(e){
                var savedItem = this.addOnsHash[this.getAddOnHashKey(e.target.outerHTML)];
                this.showStatus('Error importing ' + savedItem.viewName + ' at ' + e.target.href, 'error');
            }
        );
    },
    localStorageFileLoaded : function (file, e) {
        var valid = false;
        var error = null;
        try {
            var storage = JSON.parse(e.target.result);
            if (rovUtils.isArray(storage)) {
                if (storage[3].autoConnect !== undefined) {
                    valid = true;
                    rovUtils.setLocalStorageObject('rovSettings-storage', storage[0]);
                    this.setRovSettings(storage[0]);
                    rovUtils.setLocalStorageObject('dashboards-storage', storage[1]);
                    this.set('dashboards', storage[1]);
                    rovUtils.setLocalStorageObject('favorites-storage', storage[2]);
                    var favoritesStorage = storage[2];
                    if (Object.keys(favoritesStorage).length > 0) {
                        this.set('favoriteModules', favoritesStorage);
                    }
                    rovUtils.setLocalStorageObject('rovInit-storage', storage[3]);
                    this.set('rovInit',storage[3]);
                    rovUtils.setLocalStorageObject('viewsData-storage', storage[4]);
                    rovUtils.setLocalStorageObject('currentViews-storage', storage[5]);

                    if (this._ccs7) {
                        rovUtils.setLocalStorageObject('coreNames-storage', storage[6]);
                        var storageArr;
                        var coresStorage = storage[7];
                        var cores = Object.keys(coresStorage);
                        for (var i = 0; i < cores.length; i++) {
                            storageArr = coresStorage[cores[i]];
                            rovUtils.setLocalStorageObject(cores[i] + '-viewsData-storage', storageArr[0]);
                            rovUtils.setLocalStorageObject(cores[i] + '-currentViews-storage', storageArr[1]);
                            rovUtils.setLocalStorageObject(cores[i] + 'rovSettings-storage', storageArr[2]);
                            rovUtils.setLocalStorageObject(cores[i] + '-rovInit-storage', storageArr[3]);
                        }
                        this.set('rovInit', this.getRovInitStorage());
                        this.setRovSettings(this.getRovSettingsStorage());
                    }
                }
            }
        }
        catch (e) {
            error = e;
        }
        if (!valid) {
            this.showStatus(file.name + ' is not a valid ROV storage file', 'error');
        }
        if (error) {
            this.showStatus(error, 'error');
        }
        if (valid && !error) {
            if (this.$.connectDialog.hidden == false) {
                if (this.rovInit.autoConnect) {
                    this.$.connectDialog.hidden = true;
                    this.autoConnect();
                }
                else {
                    this.showConnectDialog();
                }
                if (this.rovInit.restoreViews && this.rovInit.showIntro == false &&
                    this.rovInit.isNarrow) {
                    this.toggleDrawerClicked();
                }
            }
            else if (this.rovInit.restoreViews) {
                this.closeAllViews();
                this.restoreViewsState();
            }
        }
    },
    localStorageFileReader : function (file) {
        var reader = new FileReader();
        reader.onload = this.localStorageFileLoaded.bind(this, file);
        reader.readAsText(file);
    },
    matchCustomViews : function (cellProps) {
        /* cellProps.primaryKeyColumnValues && cellProps.selectedColumnNames is always 1 item array in this function */
        var cells = {same: [], different: []};
        for (var i = 0; i < this.customViews.length; i++) {
            var found = false;
            for (var j = 0; j < this.customViews[i].cells.length; j++) {
                var cell = this.customViews[i].cells[j];
                if (cellProps.view == cell.view &&
                    cellProps.primaryKeyColumnName == cell.primaryKeyColumnName &&
                    cellProps.primaryKeyColumnValue == cell.primaryKeyColumnValue &&
                    cellProps.selectedColumnName == cell.selectedColumnName) {

                    found = true;
                    break;
                }
            }
            if (found) {
                cells.same.push(this.customViews[i]);
            }
            else {
                cells.different.push(this.customViews[i]);
            }

        }
        return ((cells.same.length == 0 && cells.different.length == 0) ? null : cells);
    },
    matchPlotViews : function (dataProps) {
        function foundMatch (srcTrace, tstTrace) {
            if (srcTrace.isCustom) {
                return (srcTrace.viewName == tstTrace.viewName &&
                        srcTrace.displayColumnName == tstTrace.displayColumnName);
            }
            else {
                if (srcTrace.viewArgs || tstTrace.viewArgs) {
                    return (srcTrace.module == tstTrace.module &&
                            srcTrace.view == tstTrace.view &&
                            srcTrace.selectedColumnName == tstTrace.selectedColumnName &&
                            srcTrace.args == tstTrace.args);
                }
                else {
                    return (srcTrace.module == tstTrace.module &&
                            srcTrace.view == tstTrace.view &&
                            srcTrace.primaryKeyColumnName == tstTrace.primaryKeyColumnName &&
                            srcTrace.primaryKeyColumnValue == tstTrace.primaryKeyColumnValue &&
                            srcTrace.selectedColumnName == tstTrace.selectedColumnName);
                }
            }
        }

        /* dataProps.primaryKeyColumnValues && dataProps.selectedColumnNames is always 1 item array in this function */
        var plots = {same: [], different: []};
        for (var i = 0; i < this.plotViews.length; i++) {
            var found = false;
            for (var j = 0; j < this.plotViews[i].traces.length; j++) {
                if (foundMatch(dataProps, this.plotViews[i].traces[j])) {
                    found = true;
                    break;
                }
            }
            if (found) {
                plots.same.push(this.plotViews[i]);
            }
            else {
                plots.different.push(this.plotViews[i]);
            }

        }
        return ((plots.same.length == 0 && plots.different.length == 0) ? null : plots);
    },
    makeModulesToc : function(modules) {
        var info = rovUtils.getPackagesInfo(modules);
        var modulesToc = [];
        /* Get all units and sort by name */
        var units = [];
        for (var i = 0; i < info.pkgNames.length; i++) {
            var pkg = info.pkgs[info.pkgNames[i]];
            for (var j = 0; j < pkg.units.length; j++) {
                pkg.units[j].qual = info.pkgNames[i];
                units.push(pkg.units[j]);
            }
        }
        units.sort(rovUtils.objNameCompare);

        var i = 0;
        while (i < units.length - 1) {
            if (units[i + 1].name == units[i].name) {
                var j = i + 1;
                var uSame = [units[i], units[j]];
                while (j < units.length - 1) {
                    if (units[j + 1].name == units[j].name) {
                        uSame.push(units[++j]);
                    }
                    else {
                        ++j;
                        break;
                    }
                }
                uSame.sort(rovUtils.objQualCompare);

                var tocItem = {};
                tocItem.name = units[i].name;
                if (uSame.length) {
                    tocItem.id = units[i].name;
                    tocItem.hasDups = true;
                    tocItem.expanded = false;
                    tocItem.dups = [];
                    for (var k = 0; k < uSame.length; k++) {
                        var dup = {};
                        dup.name = uSame[k].qual;
                        dup.id = uSame[k].qual + '.' + uSame[k].name;
                        dup.iconTitle = 'New ' + uSame[k].name + ' view';
                        dup.iconHidden = true;
                        tocItem.dups.push(dup);
                    }
                }
                else {
                    tocItem.id = units[i].qual + '.' + units[i].name;
                }
                modulesToc.push(tocItem);
                i = j;
                continue;
            }
            else {
                var tocItem = {};
                tocItem.name = units[i].name;
                tocItem.id = units[i].qual + '.' + units[i].name;
                tocItem.iconTitle = 'New ' + units[i].name + ' view';
                tocItem.iconHidden = true;
                modulesToc.push(tocItem);
            }
            ++i;
        }
        if (i == units.length - 1 &&
            (i == 0 || (i > 0 && units[i].name != units[i - 1].name))) {
            var tocItem = {};
            tocItem.name = units[i].name;
            tocItem.id = units[i].qual + '.' + units[i].name;
            tocItem.iconTitle = 'New ' + units[i].name + ' view';
            tocItem.iconHidden = true;
            modulesToc.push(tocItem);
        }
        return (modulesToc);
    },
    maximizeNewView : function (elem, offset) {
        var elemStyle = window.getComputedStyle(elem);
        var cardStyle = window.getComputedStyle(elem.$.viewPaperCard);
        var contentDivStyle = window.getComputedStyle(elem.$.viewContentDiv);
        var prevState = {};
        prevState.openedMaximized = true;
        prevState.viewWidth = elemStyle.width;
        prevState.viewHeight = elemStyle.height;
        prevState.left = offset.left + 'px';
        prevState.top = offset.top + 'px';
        prevState.cardWidth = cardStyle.width;
        prevState.cardHeight = cardStyle.height;
        prevState.viewContentDivPosition = contentDivStyle.position;
        elem.prevState = prevState;
        this.maximizeView(elem);
    },
    maximizeView : function (elem) {
        elem.style.width = '100%';
        elem.style.height = '100%';
        elem.style.left = '0px';
        elem.style.top = '0px';
        elem.$.viewPaperCard.style.width = '100%';
        elem.$.viewPaperCard.style.height = '100%';
        elem.$.viewPaperCard.style.marginTop = '0px';
        elem.resizeViewsDisabled = true;
    },
    modClicked : function(e) {
        if (this.$.introDialog.opened) {
            this.$.introDialog.close();
        }
        var moduleName = e.currentTarget.id;
        if (this.moduleIsShowing(moduleName) && e.srcElement.localName != 'iron-icon') {
            var zInfo = this.getNextZindexView(null, moduleName);
            zInfo.nextElem.style.zIndex = String(zInfo.maxZ + 1);
            return;
        }
        var viewsData = this.getViewsDataStorage();
        var viewName = null;
        var viewArgs = null;
        if (viewsData[moduleName]) {
            if (this.hasView(moduleName, viewsData[moduleName].defaultViewName)) {
                viewName = viewsData[moduleName].defaultViewName;
                viewArgs = viewsData[moduleName].viewArgs;
            }
            else {
                delete viewsData[moduleName];
                this.setViewsDataStorage(viewsData);
                if (this.viewList[moduleName]) {
                    var viewTabs = rovUtils.getViewTabs(this.viewList, moduleName);
                    viewName = viewTabs[0].name;
                }
            }
        }
        else if (this.viewList[moduleName]) {
            var viewTabs = rovUtils.getViewTabs(this.viewList, moduleName);
            viewName = viewTabs[0].name;
        }
        var elem = this.newModuleView(moduleName, viewName, viewArgs);
        elem.style.zIndex = String(this.getMaxZindex() + 1);
        var offset = this.getNewClickedViewPosition();
        if (this.inFullScreenMode) {
            this.maximizeNewView(elem, offset);
        }
        else {
            if (offset.left > 0) {
                elem.style.left = offset.left + 'px';
            }
            if (offset.top > 0) {
                elem.style.top = offset.top + 'px';
            }
            var key = moduleName + '.' + viewName;
            if (viewsData[key] && viewsData[key].width) {
                elem.$.viewPaperCard.style.width = viewsData[key].width;
                elem.$.viewPaperCard.style.height = viewsData[key].height;
                elem.$.viewContentDiv.style.position = 'absolute';
            }
        }
        if (e.srcElement.localName != 'iron-icon') {
            var itemStr = this.getTocItemStr(e) + '.iconHidden';
            this.set(itemStr, false);
        }
    },
    modContextMenu : function (e) {
        e.preventDefault();
        if (this.allModulesVisible) {
            var idx = this.favoriteModules.indexOf(e.currentTarget.id);
            this.favoriteMenuItem = idx == -1 ? 'Add to' : 'Remove from';
        }
        else {
            this.favoriteMenuItem = 'Remove from';
        }
        this.favoriteMenuItem += ' Favorite Modules';
        var dialog = this.$.modContextDialog;
        dialog.resetFit();
        dialog.style.left = e.x + 'px';
        dialog.style.top = e.y + 'px';
        dialog.selectedMod = e.currentTarget.id;
        dialog.open();
    },
    modContextMenuItemClicked : function (e) {
        var selectedMod = this.$.modContextDialog.selectedMod;
        var menuItem = e.currentTarget.textContent;
        this.$.modContextDialog.close();
        if (menuItem.indexOf('Add') == 0) {
            this.addFavorite(selectedMod);
        }
        else {
            this.clearFavoriteConfirm(selectedMod);
        }
    },
    modulesViewItemSelected : function (e) {
        var sel = e.currentTarget.selectedItem.textContent.trim();
        this.$.selectModulesViewMenu.selected = -1;
        var dialog = this.$.selectModulesViewDialog;
        if (dialog) {
            dialog.close();
        }
        this.setCurModsList(sel);
        var rovSettings = this.getRovSettingsStorage();
        rovSettings.curModsLabel = sel;
        this.setRovSettingsStorage(rovSettings);
    },
    moduleIsShowing : function (moduleName) {
        var found = false;
        for (var i = 0; i < this.rovModuleViews.length; i++) {
            var elem = this.getViewById(this.rovModuleViews[i].id);
            if (elem.moduleName == moduleName) {
                found = true;
                break;
            }
        }
        return (found);
    },
    moduleViewsShowing : function () {
        var count = 0;
        for (var i = 0; i < this.rovModuleViews.length; i++) {
            var elem = this.getViewById(this.rovModuleViews[i].id);
            if (elem) {
                if (elem.viewIsShowing || elem.viewSelected) {
                    ++count;
                }
            }
            else {
                console.error('Undefined view, id: ' + this.rovModuleViews[i].id);
            }
        }
        return (count);
    },
    mruClicked : function (e) {
        var dlgId = e.currentTarget.id.replace(/Button/, 'Dialog');
        var dialog = this.$[dlgId];
        dialog.resetFit();
        dialog.open();
        dialog.style.left = e.x + 'px';
        dialog.style.top = (e.y + 14) + 'px';
    },
    mruSelected : function (e) {
        if (e.srcElement.localName == 'iron-icon') {
            return;
        }
        var menu = e.currentTarget.parentElement;
        var pre = menu.id.replace(/MruMenu/, '');
        var sel = e.currentTarget.textContent.trim();
        this.$[menu.id].selected = -1;
        this.$[pre + 'MruDialog'].close();
        this.$[pre + 'MruDialog'].style.minWidth = '';
        this.$[pre + 'Input'].value = sel;
    },
    mruOpened : function (e) {
        var dialog = e.currentTarget;                                          /* +30 = margins & padding */
        var width = dialog.clientWidth < dialog.scrollWidth ? dialog.scrollWidth + 30 : dialog.clientWidth;
        dialog.style.minWidth = width + 'px';
    },
    multiDashboardFileLoaded : function (file, done, e) {
        var valid = false;
        var error = null;
        try {
            var dashboard = JSON.parse(e.target.result);
            if (rovUtils.isArray(dashboard)) {
                if (dashboard[0].moduleName || dashboard[0].elemName) {
                    valid = true;
                    var name = dashboard.pop();
                    var savedDashboards = Object.keys(this.dashboards);
                    if (savedDashboards.length > 0 && savedDashboards.indexOf(name) >= 0) {
                        this.dashboardConflicts.push({name: name, dashboard: dashboard, file: file});
                    }
                    else {
                        this.dashboards[name] = dashboard;
                        rovUtils.setLocalStorageObject('dashboards-storage', this.dashboards);
                        this.showStatus('Saved dashboard ' + name, 'info');
                    }
                }
                else if (dashboard[0].fileName && dashboard[0].numDashboards) {
                    valid = true;
                    dashboard.shift();
                    this.dashboardsArrayLoaded(dashboard, false);
                }
            }
        }
        catch (e) {
            error = e;
        }
        if (!valid) {
            this.showStatus(file.name + ' is not a valid dashboard file', 'error');
        }
        if (error) {
            this.showStatus(error, 'error');
        }
        if (done) {
            if (this.dashboardConflicts.length > 0) {
                this.resolveSaveDashboardsConflicts();
            }
            else {
                this.dashboardButtonsState();
            }
        }
    },
    newMarkdownView : function (module, view, args) {
        var elemProps = {name: this.markdownAddon.element,
                         path: this.markdownAddon.path,
                         module: module,
                         view: view,
                         args: args};
        var elem = this.newRovView(elemProps, true);

    },
    newModuleView : function(moduleName, viewName, viewArgs, viewsData, dashboard) {
        var rovViews = [];
        if (this.rovModuleViews.length > 0) {
            for (var i = 0; i < this.rovModuleViews.length; i++) {
                rovViews.push(rovUtils.shallowCopy(this.rovModuleViews[i]));
            }
        }
        var newView = {};
        newView.id = this.getNewModuleViewId();
        rovViews.push(newView);
        this.rovModuleViews = rovViews;

        var view = document.createElement('ti-rov-view');
        view.setAttribute('id', newView.id);
        if (viewsData) {
            view.viewsData = viewsData;
        }
        if (viewArgs) {
            view.viewArgs = viewArgs;
        }
        if (dashboard) {
            view.dashboardView = dashboard;
        }
        /* this triggers moduleNameChanged() in ti-rov-view.js */
        view.moduleName = moduleName;

        Polymer.dom(this.$.contentDiv).appendChild(view);
        Polymer.dom.flush();
        if (viewName) {
            view.showView(viewName, false, false, true);
        }
        this.toolbarButtonsState();
        this.dashboardButtonsState();
        return (view);
    },
    newRovCustomView : function (viewProps, rovCells, viewsData) {
        var elemProps = {name: 'ti-rov-customview'};
        if (viewProps.fromClick) {
            elemProps.viewCells = [{}];
            var keys = Object.keys(viewProps);
            for (var i = 0; i < keys.length; i++) {
                elemProps.viewCells[0][keys[i]] = viewProps[keys[i]];
            }
            if (!viewProps.tableName) {
                elemProps.tableName = this.newRovCustomViewName();
            }
            var elem = this.newRovView(elemProps, true);

            var tableProps = {elem: elem, cells: [{}]};
            var keys = Object.keys(viewProps);
            for (var i = 0; i < keys.length; i++) {
                tableProps.cells[0][keys[i]] = viewProps[keys[i]];
            }
            this.customViews.push(tableProps);
            if (this.inFullScreenMode) {
                this.maximizeNewView(elem, {left: elem.offsetLeft, top: elem.offsetTop});
            }
        }
        else {
            var keys = Object.keys(viewProps);
            for (var i = 0; i < keys.length; i++) {
                if (keys[i] != 'rovCells') {
                    elemProps[keys[i]] = viewProps[keys[i]];
                }
            }
            if (rovCells) {
                elemProps.viewCells = rovCells;
            }
            if (viewsData) {
                elemProps.viewsData = viewsData;
            }
            var elem = this.newRovView(elemProps, true);
            var tableProps = {elem: elem, cells : rovCells};
            this.customViews.push(tableProps);
        }
        return (elem);
    },
    newRovCustomViewName : function () {
        /* get all suffix numbers ,
           sort them and add one to the last one
        */
        var defName = 'ROV Table ';
        var nums = [0];
        for (var i = 0; i < this.rovViews.length; i++) {
            var elem = this.getViewById(this.rovViews[i].id);
            if (elem.name == 'ti-rov-customview') {
                if (elem.tableName.match(/^ROV Table /)) {
                    nums.push(Number(elem.tableName.substr(9)));
                }
            }
        }
        nums.sort();
        var name = defName + (nums[nums.length - 1] + 1);
        return (defName + (nums[nums.length - 1] + 1));
    },
    newRovGraph : function (graphProps) {
        var elemProps = {name: 'ti-rov-graph'};
        if (graphProps.fromClick) {
            elemProps.plotTraces = [{}];
            var keys = Object.keys(graphProps);
            for (var i = 0; i < keys.length; i++) {
                elemProps.plotTraces[0][keys[i]] = graphProps[keys[i]];
            }
            if (!graphProps.graphName) {
                elemProps.graphName = this.newRovGraphName();
            }
            var elem = this.newRovView(elemProps, true);
            var plotProps = {elem: elem, traces : [{}]};
            var keys = Object.keys(graphProps);
            for (var i = 0; i < keys.length; i++) {
                plotProps.traces[0][keys[i]] = graphProps[keys[i]];
            }
            this.plotViews.push(plotProps);
            if (this.inFullScreenMode) {
                this.maximizeNewView(elem, {left: elem.offsetLeft, top: elem.offsetTop});
            }
        }
        else {
            var keys = Object.keys(graphProps);
            for (var i = 0; i < keys.length; i++) {
                if (keys[i] != 'rovTraces') {
                    elemProps[keys[i]] = graphProps[keys[i]];
                }
                else {
                    elemProps.plotTraces = graphProps.rovTraces;
                }
            }
            var elem = this.newRovView(elemProps, true);
            var plotProps = {elem: elem, traces : graphProps.rovTraces};
            this.plotViews.push(plotProps);
        }
        return (elem);
    },
    newRovGraphName : function () {
        /* get all suffix numbers ,
           sort them and add one to the last one
        */
        var defName = 'ROV Graph ';
        var nums = [0];
        for (var i = 0; i < this.rovViews.length; i++) {
            var elem = this.getViewById(this.rovViews[i].id);
            if (elem.name == 'ti-rov-graph') {
                if (elem.graphName.match(/^ROV Graph /)) {
                    nums.push(Number(elem.graphName.substr(9)));
                }
            }
        }
        nums.sort();
        var name = defName + (nums[nums.length - 1] + 1);
        return (defName + (nums[nums.length - 1] + 1));
    },
    newRovView : function(elemProps, toTop) {
        var rovViews = [];
        if (this.rovViews.length > 0) {
            for (var i = 0; i < this.rovViews.length; i++) {
                rovViews.push(rovUtils.shallowCopy(this.rovViews[i]));
            }
        }
        var newView = {};
        newView.id = this.getNewViewId();
        rovViews.push(newView);
        this.rovViews = rovViews;

        var elem = document.createElement(elemProps.name);
        elem.setAttribute('id', newView.id);
        var keys = Object.keys(elemProps);
        for (var i = 0; i < keys.length; i++) {
            elem[keys[i]] = elemProps[keys[i]];
        }

        Polymer.dom(this.$.contentDiv).appendChild(elem);
        Polymer.dom.flush();
        this.toolbarButtonsState();
        this.dashboardButtonsState();
        if (toTop) {
            elem.style.zIndex = String(this.getMaxZindex() + 1);
        }
        return (elem);
    },
    newSavedView : function (viewProps, viewType) {
        if (viewType == 'module') {
            var dashboard = viewProps.dashboardVersion !== undefined;
            if (!this.hasView(viewProps.moduleName, viewProps.viewName)) {
                var viewTabs = rovUtils.getViewTabs(this.viewList, viewProps.moduleName);
                viewProps.viewName = viewTabs[0].name;
                viewProps.viewArgs = null;
                viewProps.viewsData = null;
            }
            var view = this.newModuleView(viewProps.moduleName, viewProps.viewName,
                                          viewProps.viewArgs, viewProps.viewsData,
                                          dashboard);
        }
        else if (viewType == 'graph') {
            var view = this.newRovGraph(viewProps.persistProperties);
        }
        else if (viewType == 'custom') {
            var view = this.newRovCustomView(viewProps.persistProperties,
                                             viewProps.rovCells,
                                             viewProps.viewsData);
        }
        else {
            var props = {name: viewProps.elemName, path: viewProps.elemPath};
            if (viewProps.persistProperties) {
                var keys = Object.keys(viewProps.persistProperties);
                for (var i = 0; i < keys.length; i++) {
                    props[keys[i]] = viewProps.persistProperties[keys[i]];
                }
            }
            var view = this.newRovView(props);
        }
        if (viewProps.prevState) {
            view.prevState = viewProps.prevState;
            this.maximizeView(view);
        }
        else {
            if (String(viewProps.left).indexOf('px') == -1) {
                viewProps.left += 'px';
                viewProps.top += 'px';

            }
            if (String(viewProps.width).indexOf('px') == -1) {
                viewProps.width += 'px';
                viewProps.height += 'px';
            }
            view.style.left = viewProps.left;
            view.style.top = viewProps.top;
            if (viewProps.position == 'absolute') {
                view.$.viewContentDiv.style.position = 'absolute';
            }
            view.$.viewPaperCard.style.width = viewProps.width;
            view.$.viewPaperCard.style.height = viewProps.height;
            view.style.zIndex = viewProps.zIndex;
        }
        if (viewProps.repeatDivider !== undefined) {
            view.repeatDivider = viewProps.repeatDivider;
        }
        if (viewProps.repeatRefreshEnabled !== undefined) {
            view.repeatRefreshEnabled = viewProps.repeatRefreshEnabled;
        }
        viewProps.defaultSize = viewProps.defaultSize == true ? true : false;
        return (view);
    },
    onBeforeUnload : function () {
        if (this.rovInit.restoreViews && this.inited) {
            this.saveViewsState();
        }
    },
    onIronResize: function(e) {      /* height only resize */
        if (!this.inited || this.panelWidth == this.$.rovDrawerPanel.offsetWidth) {
            return;
        }
        if (!this.$.rovDrawerPanel.narrow) {
            if (this.$.rovDrawerPanel.offsetWidth < 875 &&
                (this.$.rovDrawerPanel.offsetWidth < this.panelWidth || this.panelWidth == -1)) {

                this.toggleDrawerClicked();
            }
            else {
                this.toolbarResponseToResize(false);
            }
        }
        else {
            this.toolbarResponseToResize(true);
        }
        this.panelWidth = this.$.rovDrawerPanel.offsetWidth;
    },
    onKeyDown : function (e) {
        var accKey = navigator.appVersion.indexOf('Mac') == -1 ? e.ctrlKey : e.metaKey;
        if ((e.key == 'ArrowLeft' || e.key == 'ArrowRight') && accKey == true) {
            e.preventDefault();
            var zInfo = this.getNextZindexView(e.key == 'ArrowLeft');
            this.viewToForeground(zInfo.nextElem, zInfo.maxZ);
        }
    },
    onPlotViewDataContextMenuClosed : function () {
        this.$.plotViewData.hidden = true;
        this.$.addToGraph.hidden = true;
        this.plotViewDataSubItemsHidden = true;
        this.set('plotViewDataSubMenuItems', []);
        this.$.newGraph.hidden = true;

        this.$.customViewData.hidden = true;
        this.$.addToCustomView.hidden = true;
        this.customViewDataSubItemsHidden = true;
        this.set('customViewDataSubMenuItems', []);
        this.$.newCustomViewData.hidden = true;
    },
    openDashboardClicked : function(e) {
        this.dashboardSelected(e);
        this.clearDashboardOptionsMenuDialogState();
        this.$.dashboardOptionsMenuDialog.close();
    },
    openDashboardDrop : function (e) {
        if (this.ccs7) {
            e.preventDefault();
            return;
        }
        this.dashboardButtonsState();
        this.$.dashboardOptionsButton.style.color =  '';
        if (this.isADragDropFile(e)) {
            e.preventDefault();
            var files = e.dataTransfer.files;
            if (files.length == 1) {
                this.dashboardFileReader(files[0]);
            }
            else {
                this.dashboardConflicts = [];
                for (var i = 0; i < files.length; i++) {
                    var reader = new FileReader();
                    reader.onload = this.multiDashboardFileLoaded.bind(this, files[i], i == (files.length - 1));
                    reader.readAsText(files[i]);
                }
            }
        }
        else {
            alert('Not a valid dashboard file');
        }
    },
    openDashboardFileClicked : function (e) {
        var isMac = navigator.appVersion.indexOf('Mac') != -1;
        this.rovDashboardAccept = isMac ? '.rov,.json' : '.rov.json';
        if (window.rov_setFileFilter) {
            window.rov_setFileFilter(isMac ? 'ROV Dashboards|*.rov;*.json' : 'ROV Dashboards|*.rov.json');
        }
        this.$.openDashboardFileInput.value = '';
        this.$.openDashboardFileInput.click();
    },
    openDashboardFileSelected : function (e) {
        var files = e.currentTarget.files;
        if (files.length == 1) {
            var extMatch = files[0].name.match(/\.rov\.json$/) != null;
            if (!extMatch && navigator.appVersion.indexOf('Mac') != -1) {
                extMatch = files[0].name.match(/\.rov$/) != null;
            }
            if (extMatch) {
                this.dashboardFileReader(files[0]);
            }
            else {
                this.showStatus(files[0].name + ' is not a valid dashboard file', 'error');
            }
            this.$.openDashboardFileInput.value = '';
        }
    },
    openingDashboardAlertDialogButtonClicked : function (e) {
        var closeAllViews = (e.currentTarget.id == 'openingDashboardAlertDialogOKButton');
        this.$.openingDashboardAlertDialog.close();
        if (this.$.openingDashboardAlertDialog.dashboard == null &&
            this.$.openingDashboardAlertDialog.rovSel != null) {

            this.showSelectedDashboard(this.$.openingDashboardAlertDialog.rovSel, closeAllViews);
        }
        else if (this.$.openingDashboardAlertDialog.dashboard != null &&
            this.$.openingDashboardAlertDialog.rovSel == null) {

            var dashboard = this.$.openingDashboardAlertDialog.dashboard;
            var name = dashboard.pop();
            this.dashboards[name] = dashboard;
            this.showDashboard(dashboard, name, closeAllViews);
        }
    },
    otherViewsDialog : function(event) {
        var dialog = this.$.otherViewsDialog;
        dialog.resetFit();
        this.set('dlgOtherViews', this.otherViews);
                           /* 24 is the padding + 10 */
        dialog.style.left = (event.x - 34) + 'px';
        dialog.open();
        dialog.style.zIndex = String(this.getMaxZindex() + 1);
        if (this.$.otherViewsMenu.selected != undefined) {
            this.$.otherViewsMenu.selected = -1;
        }
    },
    otherViewsDialogClosed : function(e) {
        this.set('dlgOtherViews', []);
    },
    otherViewSelected : function(e) {
        var elemInfo = this.getAddOnInfo(e.detail.item.id, e.detail.item.title);
        var elemProps = {name: e.detail.item.id, path: elemInfo.path};
        var elem = this.newRovView(elemProps, true);
        this.$.otherViewsMenu.selected = -1;
        this.$.otherViewsDialog.close();
        var offset = this.getNewClickedViewPosition();
        if (this.inFullScreenMode) {
            this.maximizeNewView(elem, offset);
        }
        else {
            if (offset.left > 0) {
                elem.style.left = offset.left + 'px';
            }
            if (offset.top > 0) {
                elem.style.top = offset.top + 'px';
            }
        }
    },
    otherViewsHaveElem : function (elemName) {
        for (var i = 0; i < this.otherViews.length; i++) {
            if (this.otherViews[i].element == elemName) {
                return (true);
            }
        }
        return (false);
    },
    overflowToolbarButtonClicked : function (e) {
        var dialogId = e.currentTarget.id.replace(/Button/, '');
        var dialog = this.$[dialogId];
        if (dialog.opened) {
            dialog.close();
        }
        else {
            dialog.open();
            dialog.style.zIndex = String(this.getMaxZindex() + 1);
        }
    },
    overwriteADashboardChecked : function (e) {
        if (!this.$.overwriteDashboardsDialog.opened) {
            return;
        }
        this.enableChecklistDialogActionButton(e, 'overwriteDashboardsList',
                                               'overwriteDashboardsButton');
    },
    overwriteDashboardsClicked : function (e) {
        var listItems = this.$.overwriteDashboardsList.items;
        /* make list of checked and unchecked items */
        var checked = [];
        var unchecked = [];
        for (var i = 0; i < listItems.length; i++) {
            if (listItems[i].lastElementChild.checked) {
                checked.push(listItems[i].innerText.trim());
                listItems[i].lastElementChild.checked = false;
            }
            else {
                unchecked.push(listItems[i].innerText.trim());
            }
        }
        /* save all checked dashboards */
        for (var i = 0; i < this.dashboardConflicts.length; i++) {
            if (checked.indexOf(this.dashboardConflicts[i].name) >= 0) {
                this.saveDroppedDashboard(this.dashboardConflicts[i].name,
                                          this.dashboardConflicts[i].dashboard,
                                          false);
            }
        }
        if (unchecked.length > 0) {   /* redisplay unchecked dashboards */
            unchecked.sort();
            this.set('overwriteDashboardsItems', unchecked);
            this.$.overwriteDashboardsButton.disabled = true;
        }
        else {
            this.$.overwriteDashboardsDialog.close();
        }
    },
    overwriteDashboardsKeyPress : function (e) {
        var keyCode = e.keyCode || e.which;
        if (keyCode === 13) {
            if (!this.$.overwriteDashboardsButton.disabled) {
                this.overwriteDashboardsClicked();
            }
            if (this.$.overwriteDashboardsDialog.opened) {
                this.$.overwriteDashboardsDialog.close();
            }
        }
    },
    panelDragEnd : function (event) {
        if (!rovUtils.validDragElement(event.target)) {
            return;
        }
        var elem = this.getViewById(event.target.parentElement.dataHost.id);
        if (elem.$.viewContentDiv) {  /* re-enable mouse events in content */
            elem.$.viewContentDiv.style.pointerEvents = '';
        }
    },
    panelDragEnter : function (event) {
        if (!rovUtils.validDragElement(event.target)) {
            return;
        }
        event.preventDefault();
    },
    panelDragOver : function (event) {
        if (event.dataTransfer.types.length > 0 &&
            event.dataTransfer.types.indexOf('Files') >= 0) {

            event.preventDefault();
            if (this.isOverOpenButton(event)) {
                event.dataTransfer.dropEffect = 'copy';
            }
            else {
                event.dataTransfer.dropEffect = 'none';
            }
            return;
        }
        if (!rovUtils.validDragElement(event.target)) {
            return;
        }
        if (event.target.id == 'contentDiv' || event.target.id == 'mainContainer' ||
            event.target.id.indexOf('rovModuleView_') >= 0 ||
            event.target.id.indexOf('rovView_') >= 0 ||
            event.target.className.indexOf('ti-rov-table') >= 0 ||
            event.target.className.indexOf('ti-rov-view') >= 0) {

            event.dataTransfer.dropEffect = 'none';
            event.preventDefault();
        }
    },
    panelDragStart : function (event) {
        if (event.target.id != 'dragDiv') {
            if (rovUtils.validDragElement(event.target)) {
                event.preventDefault();
            }
            return;
        }
        var id = event.target.parentElement.dataHost.id;
        var elem = this.getViewById(id);
        /* disable mouse events in content when dragging over our own view */
        if (elem.$.viewContentDiv) {
            elem.$.viewContentDiv.style.pointerEvents = 'none';
        }
    },
    panelDrop : function (event) {
        event.preventDefault();
    },
    parseAddOnPath : function (path) {
        /*  If path does not end in '.html', assume it is either the
         *  add on files directory, or the add on .html file with the
         *  '.html' missing.
         */
        if (path) {
            if (path.search(/\.html$/) == -1) {
                path = path.replace(/\\/g,'/');
                if (path.search(/\/$/) > 0) {
                    path = path.slice(0, -1);
                }
                /* Get the add on files directory from the path */
                if (path.indexOf('/') != -1) {
                    var dirName = path.substr(path.lastIndexOf('/'));
                    /* Also check if full name was entered w/o '.html' at the end */
                    var checkStr = path.slice(0, dirName.length * -1);
                    var rgex = new RegExp(dirName + '$');
                    if (checkStr.search(rgex) == -1) {
                       /* Is just the directory, append it before adding '.html' */
                        path += dirName;
                    }
                }
                else {
                    path = path + '/' + path;
                }
                path += '.html';
            }
            path = path.replace(/([^:])\/\/+/g, '$1/'); /* clean any dup '/' */
        }
        return (path);
    },
    plotViewDataContextMenu : function (dataProps) {
        var dialog = this.$.plotViewDataContextDialog;
        var numLines = this.initPlotViewDataContextMenu(dataProps);
        dialog.resetFit();
        var width = 250;  /* ballpark from styling in html */
        var left = dataProps.event.x;
        if ((left + width) > document.documentElement.clientWidth) {
            left = document.documentElement.clientWidth - width;
        }
        var top = dataProps.event.y;
        var height = numLines * 25;
        if (top + height > document.documentElement.clientHeight) {
          top = top - height;
          if (top < 0) {
              top = 0;
          }
        }
        dialog.style.left = left + 'px';
        dialog.style.top = top + 'px';
        dataProps.fromClick = true;
        dialog.dataProps = dataProps;
        dialog.open();
        dialog.style.zIndex = String(this.getMaxZindex() + 1);
    },
    plotViewDataMenuItemClicked : function (e) {
        e.stopPropagation();
        if (e.currentTarget.id == 'plotViewData' || e.currentTarget.id == 'newGraph') {
            this.newRovGraph(this.$.plotViewDataContextDialog.dataProps);
            this.$.plotViewDataContextDialog.close();
        }
        else if(e.currentTarget.id == 'addToGraph') {
            this.plotViewDataSubItemsHidden = !this.plotViewDataSubItemsHidden;
        }
        else if(e.currentTarget.id == 'customViewData' || e.currentTarget.id == 'newCustomViewData') {
            this.newRovCustomView(this.$.plotViewDataContextDialog.dataProps);
            this.$.plotViewDataContextDialog.close();
        }
        else if(e.currentTarget.id == 'addToCustomView') {
            this.customViewDataSubItemsHidden = !this.customViewDataSubItemsHidden;
        }
    },
    plotViewDataSubMenuItemClicked : function (e) {
        var graphElem = this.getViewByRovGraphName(e.currentTarget.textContent.trim());
        this.addTraceToRovGraph(this.$.plotViewDataContextDialog.dataProps, graphElem);
        graphElem.style.zIndex = String(this.getMaxZindex() + 1);
        this.$.plotViewDataContextDialog.close();
    },
    populateDiscoveredAddOns : function () {
        var addOns = [];
        for (var i = 0; i < this.discoveredAddOns.length; i++) {
            if (!this.isDupAddOn(this.discoveredAddOns[i].path)) {
                addOns.push(this.discoveredAddOns[i]);
            }
        }
        this.set('dlgDiscoveredAddOns', addOns);
        this.$.discoveredAddOnsTemplate.render();
    },
    postConnectMenuState : function () {
        this.$.rovOptionsButton.disabled = true;
        this.$.dashboardOptionsButton.disabled = true;
        this.$.openDashboardButton.disabled = false;
        this.$.saveDashboardClicked.disabled = false;
        this.$.deleteDashboardButton.disabled = false;

        this.$.addOnViewsClicked.disabled = false;
        this.$.deleteModuleStorageClicked.disabled = false;
        this.$.clearAllStorageClicked.disabled = false;
        this.$.globalFixedFontCheckbox.disabled = false;
    },
    preConnectMenuState : function () {
        this.disableAllButtons();
        this.$.rovOptionsButton.disabled = false;
        this.$.dashboardOptionsButton.disabled = false;
        this.$.openDashboardButton.disabled = true;
        this.$.saveDashboardClicked.disabled = true;
        this.$.deleteDashboardButton.disabled = true;
        var hasSavedDashboards = this.hasSavedDashboards();
        this.$.exportDashboardsClicked.disabled = !hasSavedDashboards;

        this.$.addOnViewsClicked.disabled = true;
        this.$.deleteModuleStorageClicked.disabled = true;
        this.$.clearAllStorageClicked.disabled = true;
        this.$.globalFixedFontCheckbox.disabled = true;
    },
    queryViewSearchModules : function(module, searchArr) {
        /*  First, search for exact match */
        var foundArr = [];
        if (searchArr.indexOf(module) >= 0) {
            foundArr.push({module : module});
        }
        else {
            /* If not found, try case insensitive and suffixes */
            var exp = RegExp(module + '$', 'i');
            var exp2 = RegExp(module, 'i');
            for (var i = 0; i < searchArr.length; i++) {
                if (searchArr[i].search(exp) >= 0) {
                    foundArr.push({module : searchArr[i]});
                }
                else if (searchArr[i].search(exp2) >= 0) {
                    foundArr.push({module : searchArr[i]});
                }
            }
        }
        return (foundArr);
    },
    rediscoverAddOnsClicked : function (e) {
        this.discoverAddOns();
    },
    refreshAllClicked : function() {
        for (var i = 0; i < this.rovModuleViews.length; i++) {
            var elem = this.getViewById(this.rovModuleViews[i].id);
            if (elem.viewName) {
                if (this.inRepeatRefresh) {
                    if (!elem.repeatRefreshEnabled) {
                        continue;
                    }
                    if (elem.repeatDivider > 1) {
                        if (elem.repeatCounter++ > 0) {
                            if (elem.repeatCounter >= elem.repeatDivider) {
                                elem.repeatCounter = 0;
                            }
                            continue;
                        }
                    }
                }
                elem.showView(elem.viewName, true, false);
            }
        }
        /* Notify any other views */
        for (var i = 0; i < this.rovViews.length; i++) {
            var view = this.getViewById(this.rovViews[i].id);
            if (view && view.onRefresh) {
                if (this.inRepeatRefresh) {
                    if (!view.repeatRefreshEnabled) {
                        continue;
                    }
                    if (view.repeatDivider > 1) {
                        if (view.repeatCounter++ > 0) {
                            if (view.repeatCounter >= view.repeatDivider) {
                                view.repeatCounter = 0;
                            }
                            continue;
                        }
                    }
                }
                view.onRefresh();
            }
        }
    },
    removeAddOnClicked : function (e) {
        var otherViews = [];
        var listItems = this.$.curAddOnsList.items;
        for (var i = 0; i < listItems.length; i++) {
            if (!listItems[i].lastElementChild.checked) {
                for (var j = 0; j < this.otherViews.length; j++) {
                    if (this.otherViews[j].path == listItems[i].firstElementChild.title) {
                        otherViews.push(this.otherViews[j]);
                        break;
                    }
                }
            }
            else {
                listItems[i].lastElementChild.checked = false;
            }
        }
        this.set('otherViews', otherViews);
        this.set('dlgOtherViews', otherViews);
        this.syncDiscoveredAddOnsList();
        this.$.removeAddOnButton.disabled = true;
        this.$.selectAllCurrentAddOnsCheckbox.checked = false;
        this.$.selectAllCurrentAddOnsCheckbox.disabled = (this.dlgOtherViews.length == 0);
        this.$.curAddOnsList.selected = -1;
        this.enableOtherViews = this.otherViews.length > 0;
        var rovSettings = this.getRovSettingsStorage();
        rovSettings.otherViews = this.otherViews;
        this.setRovSettingsStorage(rovSettings);
    },
    removeCellFromCustomViewData : function (viewElem, cellProps) {
        var found = false;
        for (var i = 0; i < this.customViews.length; i++) {
            if (this.customViews[i].elem.id == viewElem.id) {
                for (var j = 0; j < this.customViews[i].cells.length; j++) {
                    var cell = this.customViews[i].cells[j];
                    if (cellProps.view == cell.view &&
                        cellProps.primaryKeyColumnName == cell.primaryKeyColumnName &&
                        cellProps.primaryKeyColumnValue == cell.primaryKeyColumnValue &&
                        cellProps.selectedColumnName == cell.selectedColumnName) {

                        this.customViews[i].cells.splice(j, 1);
                        found = true;
                        break;
                    }
                }
            }
            if (found) {
                break;
            }
        }
    },
    removeTraceFromPlotViewData : function (graphElem, traceProps) {
        var found = false;
        for (var i = 0; i < this.plotViews.length; i++) {
            if (this.plotViews[i].elem.id == graphElem.id) {
                for (var j = 0; j < this.plotViews[i].traces.length; j++) {
                    var trace = this.plotViews[i].traces[j];
                    if (traceProps.isCustom) {
                        if (traceProps.viewName == trace.viewName &&
                            traceProps.displayColumnName == trace.displayColumnName) {

                            this.plotViews[i].traces.splice(j, 1);
                            found = true;
                            break;
                        }
                    }
                    else if (traceProps.module == trace.module &&
                             traceProps.view == trace.view &&
                             traceProps.primaryKeyColumnName == trace.primaryKeyColumnName &&
                             traceProps.primaryKeyColumnValue == trace.primaryKeyColumnValue &&
                             traceProps.selectedColumnName == trace.selectedColumnName) {

                             this.plotViews[i].traces.splice(j, 1);
                             found = true;
                             break;
                    }
                }
            }
            if (found) {
                break;
            }
        }
    },
    repeatRefreshAll: function () {
        if (this.repeatHandle != 0) {
            if (this.dataViewsShowing() > 0) {
                this.refreshAllClicked();
                this.repeatHandle = this.async(this.repeatRefreshAll, this.refreshInterval);
            }
            else {
                this.repeatHandle = 0;
                this.stopRepeatRefresh(true);
                this.toolbarButtonsState();
            }
        }
    },
    repeatRefreshClicked : function () {
        this.inRepeatRefresh = true;
        this.$.refreshAllButton.disabled = true;
        this.$.overflowRefreshAllButton.disabled = true;
        if (this.repeatHandle == 0) {
            this.refreshAllClicked();
            this.repeatHandle = this.async(this.repeatRefreshAll, this.refreshInterval);
        }
    },
    resolveSaveDashboardsConflicts : function() {
        if (this.dashboardConflicts.length == 1) {
            this.confirmDialog('Overwrite dashboard',
                               'Overwrite ' + this.dashboardConflicts[0].name + ' ?',
                               'dashboardConflict',
                               {name: this.dashboardConflicts[0].name,
                                dashboard: this.dashboardConflicts[0].dashboard,
                                show: false});
        }
        else {
            this.dashboardConflictsDialog();
        }
    },
    restorePrevViewState : function (elem) {
        if (elem.prevState.openedMaximized || elem.moduleName) {
            this.evalPrevViewState(elem);
        }
        elem.style.width = elem.prevState.viewWidth;
        elem.style.height = elem.prevState.viewHeight;
        elem.style.left = elem.prevState.left;
        elem.style.top = elem.prevState.top;
        elem.$.viewPaperCard.style.width = elem.prevState.cardWidth;
        elem.$.viewPaperCard.style.height = elem.prevState.cardHeight;
        if (elem.moduleName) {
            elem.$.viewContentDiv.style.position = elem.prevState.viewContentDivPosition;
        }
        elem.$.viewPaperCard.style.marginTop = '8px';
        elem.resizeViewsDisabled = false;
        if (elem.moduleName && elem.prevState.viewContentDivPosition != 'absolute') {
            elem.lockSize();
        }
        elem.prevState = null;
    },
    restoreViewsChecked : function (e) {
        this.$.startupOptionsMenu.selected = -1;
        var rovInit = this.getRovInitStorage();
        this.rovInit.restoreViews = rovInit.restoreViews = e.currentTarget.checked;
        this.setRovInitStorage(rovInit);
        if (!this.$.connectDialog.hidden && e.currentTarget.checked) {
            this.setCurrentViewsStorage(null);
        }
    },
    restoreViewsState : function () {
        var stateArr = this.getCurrentViewsStorage();
        for (var i = 0; i < stateArr.length; i++) {
            if (stateArr[i].moduleName) {
                if (this.allModules.indexOf(stateArr[i].moduleName) >= 0) {
                    this.newSavedView(stateArr[i], 'module');
                }
                else {
                    this.showStatus('Module ' + stateArr[i].moduleName + ' is not present in ' + this.exeName + '\'s configuration', 'warning');
                }
            }
            else if (stateArr[i].elemName) {   /* is otherView */
                if (stateArr[i].elemName == 'ti-rov-graph') {
                    this.newSavedView(stateArr[i], 'graph');
                }
                else if (stateArr[i].elemName == 'ti-rov-customview') {
                    this.newSavedView(stateArr[i], 'custom');
                }
                else if (!this.otherViewsHaveElem(stateArr[i].elemName)) {
                    this.loadMissingSavedAddOn(stateArr[i], true);
                }
                else {
                    this.newSavedView(stateArr[i], 'addOn');
                }
            }
        }
        if (this.rovInit.inFullScreenMode) {
            this.inFullScreenMode = true;
        }
    },
    rovHelpClicked : function (e) {
        this.clearRovOptionsMenuDialogState();
        this.$.rovOptionsMenuDialog.close();
        //window.open('http://processors.wiki.ti.com/index.php/Runtime_Object_View_(ROV)' ,'_blank');
        window.open('docs/index.html' ,'_blank');
    },
    rovOptionsClicked : function(e) {
        var dialog = this.$.rovOptionsMenuDialog;
        if (dialog) {
            dialog.open();

            /* show/disable connect dialog at startup */
            this.$.showConnectDialogCheckbox.checked = !this.rovInit.autoConnect;
            this.$.restoreViewsCheckbox.checked = this.rovInit.restoreViews == true;
            this.$.globalFixedFontCheckbox.checked = this.rovSettings.globalFixedFont == true;
            this.clearRovOptionsMenuDialogState();
        }
    },
    rovOptionsItemSelected : function (e) {
        if (e.detail.item &&
            e.detail.item.id.indexOf('Clicked') > 0) {
            var selected = e.detail.item.id;
            this.clearRovOptionsMenuDialogState();
            this.$.rovOptionsMenuDialog.close();
            this[selected](e); /* id of the item is the function name */
        }
        else if (e.srcElement.id.indexOf('SubMenu') > 0) {
            this.$.rovOptionsMenu.selected = -1;
            this.$.storageOptionsSubMenu.selected = -1;
            this.$.startupOptionsSubMenu.selected = -1;
            var menuId = e.srcElement.id.replace(/SubMenu/, 'Menu');
            var hidden = !this.$[menuId].hidden;
            this.$[menuId].hidden = hidden;
            if (!hidden) {
                if (menuId.indexOf('storage') >= 0) {
                    if (!this.$.startupOptionsMenu.hidden) {
                        this.$.startupOptionsMenu.hidden = true;
                    }
                }
                else if (!this.$.storageOptionsMenu.hidden) {
                    this.$.storageOptionsMenu.hidden = true;
                }
            }
        }
    },
    sanityCheckAddOns : function () {
        if (this.otherViews.length) {
            var others = [];
            var errFound = false;
            for (var i = 0; i < this.otherViews.length; i++) {
                if (this.otherViews[i].href != 'error') {
                    others.push(this.otherViews[i]);
                }
                else if (!errFound) {
                    errFound = true;
                }
            }
            if (errFound) {
                this.otherViews = rovUtils.shallowCopy(others);
                var rovSettings = this.getRovSettingsStorage();
                rovSettings.otherViews = this.otherViews;
                this.setRovSettingsStorage(rovSettings);
            }
        }
    },
    sanityCheckSelectAllBox : function (selectAllCheckboxId, checkListId) {
        var allChecked = true;
        var listItems = this.$[checkListId].items;
        for (var i = 0; i < listItems.length; i++) {
            if (!listItems[i].lastElementChild.checked) {
                allChecked = false;
                break;
            }
        }
        this.$[selectAllCheckboxId].checked = allChecked && listItems.length > 0;
        this.$[selectAllCheckboxId].disabled = (listItems.length == 0);
    },
    saveAddOnMru : function (addOnPath) {
        if (!this.isDiscoveredAddOn(addOnPath)) {
            this.saveMru(addOnPath, this.addOnMrus.indexOf(addOnPath), 'addOnMrus',
                         rovUtils.shallowCopy(this.addOnMrus));
        }
    },
    saveAllViews : function (e) {
        var textToWrite = '';
        for (var i = 0; i < this.rovModuleViews.length; i++) {
            var elem = this.getViewById(this.rovModuleViews[i].id);
            if (elem.viewName) {
                textToWrite += this.getViewSaveData(elem) + '\n\n';
            }
        }
        /* Notify any other views */
        for (var i = 0; i < this.rovViews.length; i++) {
            var elem = this.getViewById(this.rovViews[i].id);
            if (elem && elem.onSaveView) {
                textToWrite += this.getViewSaveData(elem) + '\n\n';
            }
        }
        var fileNameToSaveAs = this.currentDashboard != '' ? this.currentDashboard : 'currentViews';
        fileNameToSaveAs = fileNameToSaveAs.replace(/ /g,'_') + '.rod.json';
        rovUtils.downloadText(textToWrite, fileNameToSaveAs);
    },
    saveCommLinkMru : function (commLink) {
        var lwrCommLink = commLink.toLowerCase();
        var index = -1;
        for (var i = 0; i < this.commLinkMrus.length; i++) {
            if (this.commLinkMrus[i].toLowerCase() == lwrCommLink) {
                index = i;
                break;
            }
        }
        this.saveMru(commLink, index, 'commLinkMrus', rovUtils.shallowCopy(this.commLinkMrus));
    },
    saveDashboardClicked : function(e) {
        var dialog = this.$.saveDashboardDialog;
        dialog.addEventListener('iron-overlay-closed', function() {
            var reason = this.closingReason;
            return (false);
        });
        dialog.open();
        dialog.style.zIndex = String(this.getMaxZindex() + 1);
        this.$.saveDashboardInput.value = this.currentDashboard;
        if (this.currentDashboard != '' && this.dashboards[this.currentDashboard][0].comment) {
            this.$.saveDashboardComment.value = this.dashboards[this.currentDashboard][0].comment;
        }
        else {
            this.$.saveDashboardComment.value = '';
        }
        this.$.showSavedDashboardsButton.disabled = !this.hasSavedDashboards();
        this.$.exportDashboardCheckbox.checked = false;
    },
    saveDashboardKeyPress : function (e) {
        var keyCode = e.keyCode || e.which;
        if (keyCode === 13 && this.$.saveDashboardInput.value != '') {
            this.saveDashboardOkClicked(e);
        }
    },
    saveDashboardOkClicked : function (e) {
        var dashName = this.$.saveDashboardInput.value;
        if (dashName.length) {
            this.$.saveDashboardInput.value = '';
            var dashComment = this.$.saveDashboardComment.value;
            this.$.saveDashboardComment.value = '';
            var exportChecked = this.$.exportDashboardCheckbox.checked;
            this.$.saveDashboardDialog.close();
            var dashArr = [];
            for (var i = 0; i < this.rovModuleViews.length; i++) {
                var elem = this.getViewById(this.rovModuleViews[i].id);
                elem.setDashboardView(true);
                var elemObj = elem.getProperties();
                elemObj.dashboardVersion = this.dashboardVersion;
                elemObj.inFullScreenMode = this.inFullScreenMode;
                dashArr.push(elemObj);
                /*  If any changes are made to the fields here, make sure to change */
                /*  the dashboard version property!!!!!!!!!  */
            }
            for (var i = 0; i < this.rovViews.length; i++) {
                var elem = this.getViewById(this.rovViews[i].id);
                var elemObj = elem.getProperties();
                if (elem.persistProperties) {
                    elemObj.persistProperties = {};
                    for (var j = 0; j < elem.persistProperties.length; j++) {
                        elemObj.persistProperties[elem.persistProperties[j]] = elem[elem.persistProperties[j]];
                    }
                }
                elemObj.dashboardVersion = this.dashboardVersion;
                elemObj.inFullScreenMode = this.inFullScreenMode;
                dashArr.push(elemObj);
                /*  If any changes are made to the fields here, make sure to change */
                /*  the dashboard version property!!!!!!!!!  */
            }
            dashArr[0].comment = dashComment;
            this.dashboards[dashName] = dashArr;
            this.setCurrentDashboard(dashName);
            rovUtils.setLocalStorageObject('dashboards-storage', this.dashboards);
            if (exportChecked) {
                this.exportDashboard(dashName, dashArr);
            }
            this.dashboardButtonsState();
        }
    },
    saveDataOptionsOkClicked: function(e) {
        this.$.repeatRefreshInputInvalid.hidden = true;
        this.$.dataTimeoutInputInvalid.hidden = true;
        var invalidInterval = false;
        var invalidDataTimeout = false;
        var invalidConnectTimeout = false;
        var interval = this.$.repeatRefreshInput.value;
        var dataTimeout = this.$.dataTimeoutInput.value;
        var connectTimeout = this.$.connectTimeoutInput.value;
        if (!interval.match(/^[1-9][0-9]*$/)) {
            invalidInterval = true;
            this.$.repeatRefreshInputInvalid.hidden = false;
        }
        if (!dataTimeout.match(/^[1-9][0-9]*$/)) {
            invalidDataTimeout = true;
            this.$.dataTimeoutInputInvalid.hidden = false;
        }
        if (!connectTimeout.match(/^[1-9][0-9]*$/)) {
            invalidConnectTimeout = true;
            this.$.connectTimeoutInputInvalid.hidden = false;
        }
        if (!invalidInterval && !invalidDataTimeout && !invalidConnectTimeout) {
            this.refreshInterval = Number(interval);
            this.dataTimeoutPeriod = Number(dataTimeout);
            this.connectTimeoutPeriod = Number(connectTimeout);
            this.rovData.setRequestTimeout(dataTimeout);
            this.rovData.setConnectTimeout(connectTimeout);
            var rovSettings = this.getRovSettingsStorage();
            rovSettings.refreshInterval = this.refreshInterval;
            rovSettings.dataTimeoutPeriod = this.dataTimeoutPeriod;
            rovSettings.connectTimeoutPeriod = this.connectTimeoutPeriod;
            this.setRovSettingsStorage(rovSettings);
            this.$.dataOptionsDialog.close();
        }
    },
    saveDroppedDashboard : function (name, dashboard, show) {
        this.dashboards[name] = dashboard;
        rovUtils.setLocalStorageObject('dashboards-storage', this.dashboards);
        if (show) {
            this.showDashboard(dashboard, name, true);
        }
        else {
            this.dashboardButtonsState();
        }
        this.showStatus('Saved dashboard: ' + name, 'info');
    },
    saveExeMru : function (exe) {
        exe = exe.replace(/\\/g, '/');
        var lwrExe = exe.toLowerCase();
        var index = -1;
        for (var i = 0; i < this.exeMrus.length; i++) {
            if (this.exeMrus[i].toLowerCase() == lwrExe) {
                index = i;
                break;
            }
        }
        this.saveMru(exe, index, 'exeMrus', rovUtils.shallowCopy(this.exeMrus));
    },
    saveExportLocalStorageMru : function (exportName) {
        var lwrExportName = exportName.toLowerCase();
        var index = -1;
        for (var i = 0; i < this.exportLocalStorageMrus.length; i++) {
            if (this.exportLocalStorageMrus[i].toLowerCase() == lwrExportName) {
                index = i;
                break;
            }
        }
        this.saveMru(exportName, index, 'exportLocalStorageMrus', rovUtils.shallowCopy(this.exportLocalStorageMrus));
    },
    saveMru : function (mru, mruIndex, mrusProp, mrusArr) {
        if (mruIndex > 0) {
            mrusArr.splice(mruIndex, 1);
            mrusArr.unshift(mru);
        }
        else if (mruIndex < 0) {
            mrusArr.unshift(mru);
        }
        if (mrusArr.length > 8) {
            while (mrusArr.length > 8) {
                mrusArr.pop();
            }
        }
        this.set(mrusProp, mrusArr);
        var rovSettings = this.getRovSettingsStorage();
        rovSettings[mrusProp] = this[mrusProp];
        this.setRovSettingsStorage(rovSettings);
    },
    saveThisCoreInfo : function () {
        var cores = rovUtils.getLocalStorageObject('coreNames-storage');
        if (!cores[this.coreName]) {
            cores[this.coreName] = this.coreName;
        }
        if ('' in cores) {
            delete cores[''];
        }
        cores.lastConnectedCore = this.coreName;
        rovUtils.setLocalStorageObject('coreNames-storage', cores);
    },
    saveViewsState : function (e) {
        var rovInit = this.getRovInitStorage();
        this.rovInit.isNarrow = rovInit.isNarrow = this.$.rovDrawerPanel.narrow;
        this.rovInit.inFullScreenMode = rovInit.inFullScreenMode = this.inFullScreenMode;
        this.setRovInitStorage(rovInit);
        var stateArr = [];
        for (var i = 0; i < this.rovModuleViews.length; i++) {
            var elem = this.getViewById(this.rovModuleViews[i].id);
            stateArr.push(elem.getProperties());
        }
        for (var i = 0; i < this.rovViews.length; i++) {
            var elem = this.getViewById(this.rovViews[i].id);
            var elemObj = elem.getProperties();
            if (elem.persistProperties) {
                elemObj.persistProperties = {};
                for (var j = 0; j < elem.persistProperties.length; j++) {
                    elemObj.persistProperties[elem.persistProperties[j]] = elem[elem.persistProperties[j]];
                }
            }
            stateArr.push(elemObj);
        }
        this.setCurrentViewsStorage(stateArr);
    },
    selectAllClicked : function (e, checklistId, actionButtonId) {
        var listItems = this.$[checklistId].items;
        for (var i = 0; i < listItems.length; i++) {
            if (!listItems[i].lastElementChild.disabled) {
                listItems[i].lastElementChild.checked = e.currentTarget.checked;
            }
        }
        if (actionButtonId) {
            this.$[actionButtonId].disabled = !e.currentTarget.checked;
        }
    },
    selectAllCurrentAddOnsClicked : function (e) {
        if (!this.$.manageAddOnsDialog.opened) {
            return;
        }
        this.selectAllClicked(e, 'curAddOnsList', 'removeAddOnButton');
    },
    selectAllDiscoveredAddOnsClicked : function (e) {
        if (!this.$.manageAddOnsDialog.opened) {
            return;
        }
        this.selectAllClicked(e, 'discoveredAddOnsList', 'addDiscoveredAddOnButton');
    },
    selectAllExportDashboardsClicked : function (e) {
        if (!this.$.exportDashboardsDialog.opened) {
            return;
        }
        this.selectAllClicked(e, 'exportDashboardsList');
        this.$.exportDashboardsButton.disabled = !e.currentTarget.checked ||
                                                 this.$.exportDashboardsInput.value == '';
    },
    selectAllViewStorageClicked : function(e) {
        if (!this.$.deleteModuleStorageDialog.opened) {
            return;
        }
        this.selectAllClicked(e, 'viewsStorageList', 'clearViewStorageButton');
    },
    selectModulesViewDialogClicked : function (e) {
        if (this.allModules.length > 0) {
            var dialog = this.$.selectModulesViewDialog;
            if (dialog) {
                dialog.style.left = (e.x) + 'px';
                dialog.open();
                this.$.selectModulesViewMenu.selected = -1;
            }
        }
    },
    setCurModsList : function (label) {
        if (label != 'Favorite Modules') {
            this.set('showFilterButtonRight', true);
            this.set('showFilterButtonLeft', false);
            this.set('allModulesVisible', true);
            this.set('curModsLabel', label);
            if (label == 'Viewable Modules') {
                this.set('otherModsLabels', ['All Modules', 'Favorite Modules']);
                var curModules = this.viewableModules;
            }
            else {
                this.set('otherModsLabels', ['Favorite Modules', 'Viewable Modules']);
                var curModules = this.allModules;
            }
            this.set('curTocModules', curModules);
            this.set('tocItems', this.makeModulesToc(curModules));
        }
        else {
            this.set('allModulesVisible', false);
            if (this.favoriteModules.length > 0 && this.favoriteTocItems.length == 0) {
                this.set('favoriteTocItems', this.makeModulesToc(this.favoriteModules));
            }
            this.set('curModsLabel', label);
            this.set('otherModsLabels', ['All Modules', 'Viewable Modules']);
        }
        if (this.allModulesVisible && this.filterValue!= '') {
            this.filterModules(this.filterValue);
        }
    },
    setCurrentDashboard : function (name) {
        this.currentDashboard = name;
        if (name == '') {
            this.$.currentDashboardDiv.title = '';
        }
        else {
            this.$.currentDashboardDiv.title = this.dashboards[name][0].comment ?
                                               this.dashboards[name][0].comment :
                                               'Current dashboard';
        }
    },
    setExecutableCallback : function (resObj) {
        var res = resObj.resStr;
        if (res != 'OK') {
            if (res == 'Connect cancelled') {
                if (this.$.connectDialog.hidden == false) {
                    this.$.connectButton.disabled = false;
                    this.connectMessage = '';
                    this.$.connectErrorIcon.hidden = true;
                    this.$.connectMessageDiv.hidden = true;
                    this.$.connectProgress.hidden = true;
                    this.$.connectProgressCancelDiv.hidden = true;
                }
                else if (this.$.autoConnectProgressDialog.opened) {
                    this.$.autoConnectProgressCancelDiv.hidden = true;
                    this.$.autoConnectProgressDialog.close();
                    this.showConnectDialog();
                }
            }
            else {
                if (this.$.connectDialog.hidden == false) {
                    this.$.connectButton.disabled = false;
                    this.connectMessage = res;
                    this.$.connectErrorIcon.hidden = false;
                    this.$.connectMessageDiv.hidden = false;
                    this.$.connectProgress.hidden = true;
                }
                else if (this.$.autoConnectProgressDialog.opened) {
                    this.$.autoConnectProgressDialog.close();
                    this.showConnectDialog(res);
                }
                else {
                    this.showStatus(res, 'error');
                }
            }
            return;
        }
        if (this._ccs7) {
            if (this.unknownCore) {
                this.coreName = resObj.coreName;
                rovUtils.setCoreName(this.coreName);
                if (this.hasRovSettingsStorage(this.coreName)) {
                    this.setRovSettings(this.getRovSettingsStorage());
                }
                this.unknownCore = false;
            }
        }
        if (this.$.connectDialog.hidden == false) {
            var rovInit = this.getRovInitStorage();
            this.rovInit.exe = rovInit.exe = this.$.exeInput.value;
            this.rovInit.commLink = rovInit.commLink = this.$.commLinkInput.value;
            this.rovInit.noExe = rovInit.noExe = this.$.exeInput.value == this.useCurrentProgram;
            this.averageIntroProgess();
            rovInit.progressInfo = this.rovInit.progressInfo;
            this.setRovInitStorage(rovInit);
            this.$.connectDialog.hidden = true;
            this.$.connectDialog.style.display = 'none';
        }
        else if (this.$.autoConnectProgressDialog.opened) {
            this.averageIntroProgess();
            var rovInit = this.getRovInitStorage();
            rovInit.progressInfo = this.rovInit.progressInfo;
            this.setRovInitStorage(rovInit);
            this.$.autoConnectProgressDialog.close();
        }
        if (this.exeName == '' && resObj.exeName.length > 0) {
            var exe = resObj.exeName.replace(/\\/g, '/');
            this.$.rovTitleToolbar.title = exe;
            this.exeName  = exe.substr(exe.lastIndexOf('/') + 1);
        }
        else {
            this.$.rovTitleToolbar.title = this.rovInit.exe;
        }
        document.title = "ROV: " + this.exeName;
        if (resObj.coreName && resObj.coreName.length > 0) {
            document.title += ' - ' + resObj.coreName;
            this.$.rovTitleToolbar.title += ' - ' + resObj.coreName;
        }
        if (window.rov_setTitle) {
            window.rov_setTitle(document.title);
        }
        if (window.rov_getSessionId) {
            this.sessionId = window.rov_getSessionId();
        }

        /* Enable toolbars */
        this.$.toggleDrawerBtn.disabled = false;
        this.$.rovOptionsButton.disabled = false;
        this.$.dashboardOptionsButton.disabled = false;
        this.panelWidth = this.$.rovDrawerPanel.offsetWidth;
        this.toolbarResponseToResize(this.$.rovDrawerPanel.narrow);
        this.dashboardButtonsState();
        this.toolbarButtonsState();
        this.sanityCheckAddOns();
        if (!this.ccs7 || !this.rovInit.noExe) {
            this.saveExeMru(this.rovInit.exe);
        }
        else if (exe) {
            this.saveExeMru(exe);
        }
        if (this.rovInit.commLink != 'Debugger') {
            this.saveCommLinkMru(this.rovInit.commLink);
        }
        if (this._ccs7) {
            this.saveThisCoreInfo();
        }
        this.rovData.getViewList(this);
        this.inited = true;
    },
    setExecutableProgressCallback : function (res, reqObj) {
        var waitingRes = res.match(/^waiting/i) != null;
        if (this.$.connectDialog.hidden == false && this.$.connectProgress.hidden == false) {
            this.$.connectProgress.value += this.rovInit.progressInfo.step;
            ++this.rovInit.progressInfo.notifyCount;
            this.$.connectProgressCancelDiv.hidden = !waitingRes;
        }
        else if (this.$.connectDialog.hidden == true && this.rovInit.autoConnect) {
            this.$.autoConnectProgress.value += this.rovInit.progressInfo.step;
            ++this.rovInit.progressInfo.notifyCount;
            this.$.autoConnectProgressCancelDiv.hidden = !waitingRes;
        }
        this.connectMessage = res;
        this.setExecutableReqObj = reqObj;
    },
    setRovSettings : function(rovSettings) {
        this.set('rovSettings', rovSettings);
        if (this.rovSettings.refreshInterval) {
            this.refreshInterval = this.rovSettings.refreshInterval;
        }
        if (this.rovSettings.dataTimeoutPeriod) {
            this.dataTimeoutPeriod = this.rovSettings.dataTimeoutPeriod;
            this.rovData.setRequestTimeout(this.dataTimeoutPeriod);
        }
        if (this.rovSettings.connectTimeoutPeriod) {
            this.connectTimeoutPeriod = this.rovSettings.connectTimeoutPeriod;
            this.rovData.setConnectTimeout(this.connectTimeoutPeriod);
        }
        if (this.rovSettings.curModsLabel) {
            this.curModsLabel = this.rovSettings.curModsLabel;
            if (this.curModsLabel == 'All Modules' || this.curModsLabel == 'Viewable Modules') {
                this.set('allModulesVisible', true);
                this.set('showFilterButtonRight', true);
                this.set('showFilterButtonLeft', false);
            }
        }
        if (this.rovSettings.otherViews) {
            this.otherViews = rovUtils.shallowCopy(this.rovSettings.otherViews);
            for (var i = 0; i < this.otherViews.length; i++) {
                var path = this.parseAddOnPath(this.otherViews[i].path);
                this.addOnsHash[path] = this.otherViews[i];
                this.importHref(path,
                    function(e){
                        var key = this.getAddOnHashKey(e.target.outerHTML);
                        var importedElem = this.getImportedAddOnId(e.target.import.activeElement);
                        if (this.addOnsHash[key].element == importedElem) {
                            this.addOnsHash[key].href = e.target.href;
                        }
                        else {
                            this.addOnsHash[key].href = 'error';
                            this.showStatus('Error importing <' + this.addOnsHash[key].element + '> at ' + e.target.href, 'error');
                            this.showStatus('Element mismatch <' + this.addOnsHash[key].element + '> vs <' + importedElem + '>');
                        }
                    },
                    function(e){
                        var key = this.getAddOnHashKey(e.target.outerHTML);
                        this.addOnsHash[key].href = 'error';
                        this.showStatus('Error importing ' + e.target.href, 'error');
                    }
                );
            }
        }
        if (this.rovSettings.addOnMrus) {
            this.addOnMrus = rovUtils.shallowCopy(this.rovSettings.addOnMrus);
        }
        if (this.rovSettings.exeMrus) {
            this.exeMrus = rovUtils.shallowCopy(this.rovSettings.exeMrus);
        }
        if (this.rovSettings.commLinkMrus) {
            this.commLinkMrus = rovUtils.shallowCopy(this.rovSettings.commLinkMrus);
        }
        if (this.rovSettings.exportLocalStorageMrus) {
            this.exportLocalStorageMrus = rovUtils.shallowCopy(this.rovSettings.exportLocalStorageMrus);
        }
    },
    showAddOnInput : function (show) {
        this.$.manageAddOnsDialog.style.height = show ? '270px' : '240px';
        this.$.showAddOnInput.disabled = show;
        this.$.hideAddOnInput.hidden =
        this.$.addOnMruButton.hidden =
        this.$.addOnInput.hidden =
        this.$.loadAddOnButton.hidden =  !show;
        this.$.closeManageAddOnsDialogButton.style.marginTop = show ? '10px' : '0px';
        if (show) {
            this.$.addOnInput.focus();
        }
    },
    showAddOnInputClicked : function (e) {
        var show = e.currentTarget.id.indexOf('show') != -1;
        this.showAddOnInput(show);
    },
    showConnectDialog : function (autoConnectError,autoconnectInit = false) {
        this.$.connectProgressCancelDiv.hidden = true;
        this.$.connectMessageDiv.hidden = true;
        this.$.commLinkInputInvalid.hidden = true;
        this.$.exeInputInvalid.hidden = true;
        if (this.rovInit.exe) {
            this.$.exeInput.value = this.rovInit.exe;
        }
        else if (this.ccs7) {
            this.$.exeInput.value = this.useCurrentProgram;
        }
        if (this.rovInit.commLink) {
            this.$.commLinkInput.value = this.rovInit.commLink;
        }
        else if (this.ccs7) {
            this.$.commLinkInput.value = 'Debugger';
        }
        if (this.ccs7) {
            var commLinkMrus = [];
            for (var i = 0; i < this.commLinkMrus.length; i++) {
                if (this.commLinkMrus[i] != 'Debugger') {
                    commLinkMrus.push(this.commLinkMrus[i]);
                }
            }
            this.set('commLinkMrus', commLinkMrus);
        }
        this.$.autoConnectCheckbox.checked = (this.rovInit.autoConnect) == true ? true : false;
        if (!this.rovInit.progressInfo) {
            this.rovInit.progressInfo = {};
            this.rovInit.progressInfo.min = 0;
            this.rovInit.progressInfo.max = 10;
            this.rovInit.progressInfo.step = 1;
        }
        this.$.connectProgress.min = this.rovInit.progressInfo.min;
        this.$.connectProgress.max = this.rovInit.progressInfo.max;
        this.$.connectProgress.step = this.rovInit.progressInfo.step;
        this.$.connectProgress.value = 0;
        this.$.connectButton.disabled = false;
        if (autoConnectError) {
            this.connectMessage = autoConnectError;
            this.$.connectErrorIcon.hidden = false;
            this.$.connectMessageDiv.hidden = false;
            this.$.connectProgress.hidden = true;
        }
		if (!autoconnectInit){
			this.$.connectDialog.hidden = false;
			this.$.connectDialog.style.display = 'inline-block';
		}
		this.preConnectMenuState();
    },
     showConnectDialogChecked : function (e) {
        this.$.startupOptionsMenu.selected = -1;
        var rovInit = this.getRovInitStorage();
        this.rovInit.autoConnect = rovInit.autoConnect = !e.currentTarget.checked;
        this.setRovInitStorage(rovInit);
    },
    showDashboard : function (dashboard, dashName, closeAllViews) {
        if (dashboard.length > 0 &&
            (!dashboard[0].dashboardVersion ||
              dashboard[0].dashboardVersion != this.dashboardVersion)) {
            this.showStatus('Incompatible dashboard', 'warning');
        }
        if (closeAllViews) {
            this.closeAllViews();
            this.setCurrentDashboard(dashName);
        }
        else {
            this.setCurrentDashboard('');
        }
        for (var i = 0; i < dashboard.length; i++) {
            if (dashboard[i].moduleName) {
                if (this.allModules.indexOf(dashboard[i].moduleName) >= 0) {
                    dashboard[i].position = 'absolute';
                    var view = this.newSavedView(dashboard[i], 'module');
                    if (!closeAllViews) {
                        view.style.zIndex = String(Number(dashboard[i].zIndex) + this.getMaxZindex() + i + 1);
                    }
                }
                else {
                    this.showStatus('Module ' + dashboard[i].moduleName + ' is not present in ' + this.exeName + '\'s configuration', 'warning');
                }
            }
            else if (dashboard[i].elemName) {   /* is otherView */
                if (dashboard[i].elemName == 'ti-rov-graph') {
                    dashboard[i].position = 'absolute';
                    var view = this.newSavedView(dashboard[i], 'graph');
                    if (!closeAllViews) {
                        view.style.zIndex = String(Number(dashboard[i].zIndex) + this.getMaxZindex() + i + 1);
                    }
                }
                else if (dashboard[i].elemName == 'ti-rov-customview') {
                    dashboard[i].position = 'absolute';
                    var view = this.newSavedView(dashboard[i], 'custom');
                    if (!closeAllViews) {
                        view.style.zIndex = String(Number(dashboard[i].zIndex) + this.getMaxZindex() + i + 1);
                    }
                }
                else if (!this.otherViewsHaveElem(dashboard[i].elemName)) {
                    this.loadMissingSavedAddOn(dashboard[i], true);
                }
                else {
                    this.newSavedView(dashboard[i], 'addOn');
                }
            }
        }
        this.inFullScreenMode = dashboard[0].inFullScreenMode == true;
    },
    showSavedDashboardsClicked : function (event) {
        this.listDashboardsDialog('Save', event);
    },
    showSelectedDashboard : function (sel, closeAllViews) {
        /* Get the original dashboard in case one in memory has been changed */
        var dashboards = rovUtils.getLocalStorageObject('dashboards-storage');
        var dashboard = dashboards[sel] ? dashboards[sel] : this.dashboards[sel]; /* defensive */
        this.showDashboard(dashboard, sel, closeAllViews);
    },
    showStatus : function (message, type) {
        if (this.statusItems.length > 0) {
            if (this.statusItems.length > 255) {
                while (this.statusItems.length > 255) {
                    this.statusItems.pop();
                }
            }
        }
        this.statusItems.push(rovUtils.statusItem(message, type));
        var statusItems = rovUtils.deepCopy(this.statusItems);
        statusItems = rovUtils.compressStatusItems(statusItems);

        var fontInfo = tiRovStyles.rovTitleFontSize + 'pt ' + 'Open Sans';
        var consoleWidth = 0;
        for (var i = 0; i < statusItems.length; i++) {
            var width = rovUtils.getStringWidth(statusItems[i].message, fontInfo);
            if (width > consoleWidth) {
                consoleWidth = width;
            }
        }
        var countWidth = rovUtils.getStatusCountWidth(statusItems);
        consoleWidth += 47 + countWidth; /* +23 icon, +24 padding, + count */
        if (statusItems.length > 10) {
            if (this.$.statusConsole.style.overflowY != 'auto') {
                this.$.statusConsole.style.overflowY = 'auto';
            }
            this.$.statusConsole.style.height = this.$.statusConsole.clientHeight + 'px';
            consoleWidth += 14;  /* scroll bar */
        }
        else if (this.$.statusConsole.style.overflowY == 'auto') {
            this.$.statusConsole.style.overflowY = '';
            this.$.statusConsole.style.height = '';
        }
        var maxWidth = this.isNarrow() ? document.documentElement.clientWidth : (document.documentElement.clientWidth - rovUtils.leftPaneWidth);
        if (consoleWidth >= maxWidth) {
            consoleWidth = maxWidth - 2;
        }
        this.$.statusConsole.style.width = String(consoleWidth) + 'px';
        this.set('compressedStatusItems', statusItems);
        this.$.statusConsole.hidden = false;
    },
    snapCorner : function (event, id, elem, top, left, right, bottom, allViews) {
        var cornerFound = false;
        var newTop;
        var newLeft;
        var shiftKey = event.shiftKey;
        for (var i = 0; i < allViews.length; i++) {
            if (allViews[i].id != id) {
                var siblingElem = this.getViewById(allViews[i].id);
                if (siblingElem.style.top == '') {
                    var siblingTop = siblingElem.clientTop;
                    var siblingLeft = siblingElem.clientLeft;
                }
                else {
                    var siblingTop = Number(siblingElem.style.top.replace(/px/, ''));
                    var siblingLeft = Number(siblingElem.style.left.replace(/px/, ''));
                }
                var siblingRight = siblingLeft + siblingElem.clientWidth;
                var siblingBottom = siblingTop + siblingElem.clientHeight;
                if (shiftKey) {
                    if (bottom > siblingTop - 90 && bottom < siblingTop) {  /* elem bottom edge -> sibling top edge */
                        if (left > siblingLeft && left < siblingLeft + 90) { /* elem bottom left -> sibling top left */
                            newTop = siblingTop - elem.clientHeight + 7;
                            newLeft = siblingLeft;
                            cornerFound = true;
                        }
                        else if (right > siblingRight - 90 && right < siblingRight) { /* elem bottom right -> sibling top right */
                            newTop = siblingTop - elem.clientHeight + 7;
                            newLeft = siblingRight - elem.clientWidth - 1;
                            cornerFound = true;
                        }
                    }
                    else if (bottom > siblingBottom - 90 && bottom < siblingBottom) {  /* elem bottom edge -> sibling bottom edge */
                        if (left > siblingRight && left < siblingRight + 30) {  /* elem bottom left -> sibling bottom right */
                            newTop = siblingBottom - elem.clientHeight;
                            newLeft = siblingLeft + siblingElem.clientWidth + 1;
                            cornerFound = true;
                        }
                        else if (right > siblingLeft - 30 && right < siblingLeft) {   /* elem bottom right -> sibling bottom left */
                            newTop = siblingBottom - elem.clientHeight;
                            newLeft = siblingLeft - elem.clientWidth - 1;
                            cornerFound = true;
                        }
                    }
                    else if (top > siblingBottom && top < siblingBottom + 90) {  /* elem top edge -> sibling bottom edge */
                        if (left > siblingLeft && left < siblingLeft + 90) { /* elem top left -> sibling bottom left */
                            newTop = siblingBottom - 8;
                            newLeft = siblingLeft;
                            cornerFound = true;
                        }
                        else if (right > siblingRight - 90 && right < siblingRight) { /* elem top right -> sibling bottom right */
                            newTop = siblingBottom - 8;
                            newLeft = siblingRight - elem.clientWidth - 1;
                            cornerFound = true;
                        }
                    }
                    else if (top > siblingTop && top < siblingTop + 90) {   /* elem top edge -> sibling top edge */
                        if (left > siblingRight && left < siblingRight + 30) {/* elem top left -> sibling top right */
                            newTop = siblingTop;
                            newLeft = siblingLeft + siblingElem.clientWidth + 1;
                            cornerFound = true;
                        }
                        else if (right > siblingLeft - 30 && right < siblingLeft) {/* elem top right -> sibling top left */
                            newTop = siblingTop;
                            newLeft = siblingLeft - elem.clientWidth - 1;
                            cornerFound = true;
                        }
                    }
                }
                else if (left > siblingRight - 20 && left < siblingRight + 20) {  /* elem left edge -> sibling right edge */
                    if (top > siblingTop - 20 && top < siblingTop + 20) {    /* elem top left -> sibling top right */
                        newTop = siblingTop;
                        newLeft = siblingLeft + siblingElem.clientWidth + 1;
                        cornerFound = true;
                    }
                    else if (top > siblingBottom - 20 && top < siblingBottom + 20) { /* elem top left -> sibling bottom right */
                        newTop = siblingBottom - 8;
                        newLeft = siblingLeft + siblingElem.clientWidth + 1;
                        cornerFound = true;
                    }
                    else if (bottom > siblingTop - 20 && bottom < siblingTop + 20) { /* elem bottom left -> sibling top right */
                        newTop = siblingTop - elem.clientHeight + 7;
                        newLeft = siblingLeft + siblingElem.clientWidth + 1;
                        cornerFound = true;
                    }
                    else  if (bottom > siblingBottom - 20 && bottom < siblingBottom + 20) { /* elem bottom left -> sibling bottom right */
                        newTop = siblingBottom - elem.clientHeight;
                        newLeft = siblingLeft + siblingElem.clientWidth + 1;
                        cornerFound = true;
                    }
                }
                else if (right > siblingLeft - 20 && right < siblingLeft + 20) {  /* elem right edge -> sibling left edge */
                    if (top > siblingTop - 20 && top < siblingTop + 20) {    /* elem top right -> sibling top left */
                        newTop = siblingTop;
                        newLeft = siblingLeft - elem.clientWidth - 1;
                        cornerFound = true;
                    }
                    else if (top > siblingBottom - 20 && top < siblingBottom + 20) { /* elem top right -> sibling bottom left */
                        newTop = siblingBottom - 8;
                        newLeft = siblingLeft - elem.clientWidth - 1;
                        cornerFound = true;
                    }
                    else if (bottom > siblingBottom - 20 && bottom < siblingBottom + 20) { /* elem bottom right -> sibling bottom left */
                        newTop = siblingBottom - elem.clientHeight;
                        newLeft = siblingLeft - elem.clientWidth - 1;
                        cornerFound = true;
                    }
                    else if (bottom > siblingTop - 20 && bottom < siblingTop + 20) { /* elem bottom right -> sibling top left */
                        newTop = siblingTop - elem.clientHeight + 7;
                        newLeft = siblingLeft - elem.clientWidth - 1;
                        cornerFound = true;
                    }
                }
                if (!cornerFound) {
                    if (top > siblingBottom - 30 && top < siblingBottom + 20) {  /* elem top edge -> sibling bottom edge */
                        if (left > siblingLeft - 20 && left < siblingLeft + 20) { /* elem top left -> sibling bottom left */
                            newTop = siblingBottom - 8;
                            newLeft = siblingLeft;
                            cornerFound = true;
                        }
                        else if (right > siblingRight - 20 && right < siblingRight + 20) { /* elem top right -> sibling bottom right */
                            newTop = siblingBottom - 8;
                            newLeft = siblingRight - elem.clientWidth - 1;
                            cornerFound = true;
                        }
                    }
                    else if (bottom > siblingTop - 20 && bottom < siblingTop + 20) {  /* elem bottom edge -> sibling top edge */
                        if (left > siblingLeft - 20 && left < siblingLeft + 20) { /* elem bottom left -> sibling top left */
                            newTop = siblingTop - elem.clientHeight + 7;
                            newLeft = siblingLeft;
                            cornerFound = true;
                        }
                        else if (right > siblingRight - 20 && right < siblingRight + 20) { /* elem bottom right -> sibling top right */
                            newTop = siblingTop - elem.clientHeight + 7;
                            newLeft = siblingRight - elem.clientWidth - 1;
                            cornerFound = true;
                        }
                    }
                }
                if (cornerFound) {
                    if (newTop < 0) {
                        cornerFound = false;
                    }
                    else {
                        top = newTop;
                        left = newLeft;
                        break;
                    }
                }
            }
        }
        return ({top          : top,
                 left         : left,
                 cornerFound  : cornerFound,
                 leftAssigned : false,
                 topAssigned  : false});
    },
    snapEdge : function (event, id, elem, top, left, right, bottom, allViews) {
        var topAssigned = false;
        var leftAssigned = false;
        var newLeft;
        var newTop;
        var shiftKey = event.shiftKey;
        for (var i = 0; i < allViews.length; i++) {
            if (allViews[i].id != id) {
                var siblingElem = this.getViewById(allViews[i].id);
                if (siblingElem.style.top == '') {
                    var siblingTop = siblingElem.clientTop;
                    var siblingLeft = siblingElem.clientLeft;
                }
                else {
                    var siblingTop = Number(siblingElem.style.top.replace(/px/, ''));
                    var siblingLeft = Number(siblingElem.style.left.replace(/px/, ''));
                }
                var siblingRight = siblingLeft + siblingElem.clientWidth;
                var siblingBottom = siblingTop + siblingElem.clientHeight;
                if (shiftKey && bottom > siblingTop - 90 && bottom < siblingTop) {  /* elem bottom edge -> sibling top edge */
                    newTop = siblingTop - elem.clientHeight + 7;
                    topAssigned = true;
                }
                else if (shiftKey && top > siblingBottom && top < siblingBottom + 90) {  /* elem top edge -> sibling bottom edge */
                    newTop = siblingBottom - 8;
                    topAssigned = true;
                }
                else if (left > siblingRight - 20 && left < siblingRight + 20) {       /* elem left edge -> sibling right edge */
                    newLeft = siblingLeft + siblingElem.clientWidth + 1;
                    leftAssigned = true;
                }
                else if (top > siblingBottom - 30 && top < siblingBottom + 20) {  /* elem top edge -> sibling bottom edge */
                    newTop = siblingBottom - 8;
                    topAssigned = true;
                }
                else if (right > siblingLeft - 20 && right < siblingLeft + 20) {  /* elem right edge -> sibling left edge */
                    newLeft = siblingLeft - elem.clientWidth - 1;
                    leftAssigned = true;
                }
                else if (bottom > siblingTop - 20 && bottom < siblingTop + 20) {  /* elem bottom edge -> sibling top edge */
                    newTop = siblingTop - elem.clientHeight + 7;
                    topAssigned = true;
                }
            }
            if (leftAssigned || topAssigned) {
                if (topAssigned) {
                    if (newTop < 0) {
                        topAssigned = false;
                    }
                    else {
                        topAssigned = (left >= siblingLeft && left <= siblingRight) ||
                                      (right >= siblingLeft && right <= siblingRight) ||
                                      (siblingLeft >= left && siblingLeft <= right) ||
                                      (siblingRight >= left && siblingRight <= right);
                    }
                }
                if (leftAssigned) {
                    leftAssigned = (top >= siblingTop && top <= siblingBottom) ||
                                   (bottom >= siblingTop && bottom <= siblingBottom) ||
                                   (siblingTop >= top && siblingTop <= bottom) ||
                                   (siblingBottom >= top && siblingBottom <= bottom);
                }
                if (leftAssigned || topAssigned) {
                    break;
                }
            }
        }
        return ({top          : (topAssigned ? newTop : top),
                 left         : (leftAssigned ? newLeft : left),
                 cornerFound  : false,
                 leftAssigned : leftAssigned,
                 topAssigned  : topAssigned});
    },
    snapSize : function (event, id, elem, top, left, right, bottom, leftResize, topResize, allViews) {
        var cornerFound = false;
        var topEdgeFound = false;
        var bottomEdgeFound = false;
        var rightEdgeFound = false;
        var leftEdgeFound = false;
        var newRight = -1;
        var newLeft = -1;
        var newBottom = -1;
        var newTop = -1;
        var sibTop = -1;
        var sibBottom = -1;
        for (var i = 0; i < allViews.length; i++) {
            if (allViews[i].id != id) {
                var siblingElem = this.getViewById(allViews[i].id);
                if (siblingElem.style.top == '') {
                    var siblingTop = siblingElem.clientTop;
                    var siblingLeft = siblingElem.clientLeft;
                }
                else {
                    var siblingTop = Number(siblingElem.style.top.replace(/px/, ''));
                    var siblingLeft = Number(siblingElem.style.left.replace(/px/, ''));
                }
                var siblingRight = siblingLeft + siblingElem.clientWidth;
                var siblingBottom = siblingTop + siblingElem.clientHeight;
                if (leftResize && left > siblingRight && left < siblingRight + 90) {  /* elem left edge -> sibling right edge */
                    if (!topResize && bottom > siblingBottom - 90 && bottom < siblingBottom) { /* elem bottom left -> sibling bottom right */
                        newBottom = siblingBottom;
                        newLeft = siblingLeft + siblingElem.clientWidth + 1;
                        cornerFound = true;
                    }
                    else if (topResize && top > siblingTop - 15 && top < siblingTop + 90) {    /* elem top left -> sibling top right */
                        newTop = siblingTop;
                        newLeft = siblingLeft + siblingElem.clientWidth + 1;
                        newBottom = bottom;
                        cornerFound = true;
                    }
                }
                else if (!leftResize && right > siblingLeft - 90 && right < siblingLeft) {  /* elem right edge -> sibling left edge */
                    if (!topResize && bottom > siblingBottom - 90 && bottom < siblingBottom) { /* elem bottom right -> sibling bottom left */
                        newBottom = siblingBottom;
                        newRight = siblingLeft;
                        cornerFound = true;
                    }
                    else if (topResize && top > siblingTop - 15 && top < siblingTop + 90) {    /* elem top right -> sibling top left */
                        newTop = siblingTop;
                        newRight = siblingLeft;
                        newBottom = bottom;
                        cornerFound = true;
                    }

                }
                if (!cornerFound) {
                    if (!topResize && bottom > siblingTop - 90 && bottom < siblingTop) {  /* elem bottom edge -> sibling top edge */
                        if (leftResize && left > siblingLeft && left < siblingLeft + 90) { /* elem bottom left -> sibling top left */
                            sibTop = siblingTop;
                            newLeft = siblingLeft;
                            cornerFound = true;
                        }
                        else if (!leftResize && right > siblingRight - 90 && right < siblingRight) { /* elem bottom right -> sibling top right */
                            sibTop = siblingTop;
                            newRight = siblingLeft + siblingElem.clientWidth + 1;
                            cornerFound = true;
                        }
                    }
                    else if (topResize && top > siblingBottom - 15 && top < siblingBottom + 90) {  /* elem top edge -> sibling bottom edge */
                        if (leftResize && left > siblingLeft && left < siblingLeft + 90) { /* elem top left -> sibling bottom left */
                            newTop = siblingBottom - 8;
                            newLeft = siblingLeft;
                            newBottom = bottom;
                            cornerFound = true;
                        }
                        else if (!leftResize && right > siblingRight - 90 && right < siblingRight) { /* elem top right -> sibling bottom right */
                            newTop = siblingBottom - 8;
                            newRight = siblingRight;
                            newBottom = bottom;
                            cornerFound = true;
                        }
                    }
                }
                if (cornerFound) {
                    break;
                }
            }
        }
        if (!cornerFound) {
            for (var i = 0; i < allViews.length; i++) {
                if (allViews[i].id != id) {
                    var siblingElem = this.getViewById(allViews[i].id);
                    var siblingTop = siblingElem.style.top == '' ? siblingElem.clientTop : Number(siblingElem.style.top.replace(/px/, ''));
                    var siblingLeft = siblingElem.style.left == '' ? siblingElem.clientLeft : Number(siblingElem.style.left.replace(/px/, ''));
                    var siblingBottom = siblingTop + siblingElem.clientHeight;
                    var siblingRight = siblingLeft + siblingElem.clientWidth;
                    if (!topResize && bottom > siblingTop - 90 && bottom < siblingTop) {   /* elem bottom edge -> sibling top edge */
                        sibTop = siblingTop;
                        topEdgeFound = true;
                        break;
                    }
                    else if (topResize && top > siblingBottom - 15 && top < siblingBottom + 90) {  /* elem top edge -> sibling bottom edge */
                        sibBottom = siblingBottom - 8;
                        bottomEdgeFound = true;
                        break;
                    }
                    else if (leftResize && left < siblingRight + 90 && left > siblingRight + 10) {       /* elem left edge -> sibling right edge */
                        newLeft = siblingLeft + siblingElem.clientWidth + 1;
                        rightEdgeFound = true;
                        break;
                    }
                    else if (!leftResize && right > siblingLeft - 90 && right < siblingLeft + 10) {  /* elem right edge -> sibling left edge */
                        newRight = siblingLeft;
                        leftEdgeFound = true;
                        break;
                    }
                }
            }
        }
        return ({bottom       : newBottom,
                 left         : newLeft,
                 right        : newRight,
                 top          : newTop,
                 siblingTop   : sibTop,
                 siblingBottom: sibBottom,
                 cornerFound  : cornerFound,
                 topEdgeFound : topEdgeFound,
                 bottomEdgeFound : bottomEdgeFound,
                 rightEdgeFound : rightEdgeFound,
                 leftEdgeFound  : leftEdgeFound});
    },
    stopRepeatRefresh : function (cancelAll) {
        this.inRepeatRefresh = false;
        this.$.repeatRefreshButton.disabled =
            this.$.refreshAllButton.disabled = (this.dataViewsShowing() < 1);
        this.$.overflowRepeatRefreshButton.disabled = this.$.repeatRefreshButton.disabled;
        this.$.overflowRefreshAllButton.disabled = this.$.refreshAllButton.disabled;
        if (this.repeatHandle != 0) {
            var handle = this.repeatHandle;
            this.repeatHandle = 0;
            this.cancelAsync(handle);
        }
        if (cancelAll) {
            this.rovData.cancelAllPendingRequests();
        }
    },
    stopRepeatRefreshClicked : function () {
        this.stopRepeatRefresh(true);
    },
    storableRovViewsShowing : function() {
        var count = 0;
        for (var i = 0; i < this.rovViews.length; i++) {
            var elem = this.getViewById(this.rovViews[i].id);
            if (elem) {
                if (elem.hasStorableContent && elem.hasStorableContent()) {
                    ++count;
                }
            }
        }
        return (count);
    },
    storageOptionSelected : function (e) {
        var selected = e.detail.item.id;
        this.clearRovOptionsMenuDialogState();
        this[selected](e);
    },
    subMenuClicked : function (event) {
        if (this.allModulesVisible) {
            var index = this.getTocIndex(event.currentTarget.id, this.tocItems);
            if (index >= 0) {
                var str = 'tocItems.' + index + '.expanded';
                this.set(str, !this.tocItems[index].expanded);
            }
        }
        else {
            var index = this.getTocIndex(event.currentTarget.id, this.favoriteTocItems);
            if (index >= 0) {
                var str = 'favoriteTocItems.' + index + '.expanded';
                this.set(str, !this.favoriteTocItems[index].expanded);
            }
        }
    },
    syncDiscoveredAddOnsList : function () {
        var hash = [];
        var listItems = this.$.discoveredAddOnsList.items;
        for (var i = 0; i < listItems.length; i++) {
            hash[listItems[i].lastElementChild.title] = {};
            hash[listItems[i].lastElementChild.title].checked = listItems[i].lastElementChild.checked;
        }
        var addOns = [];
        for (var i = 0; i < this.discoveredAddOns.length; i++) {
            if (!this.isDupAddOn(this.discoveredAddOns[i].path)) {
                addOns.push(this.discoveredAddOns[i]);
                if (!hash[this.discoveredAddOns[i].path]) {
                    hash[this.discoveredAddOns[i].path] = {};
                    hash[this.discoveredAddOns[i].path].checked = false;
                }
            }
        }
        this.set('dlgDiscoveredAddOns', addOns);
        this.$.discoveredAddOnsTemplate.render();
        listItems = this.$.discoveredAddOnsList.items;
        var foundUnchecked = false;
        var foundChecked = false;
        for (var i = 0; i < listItems.length; i++) {
            listItems[i].lastElementChild.checked = hash[listItems[i].lastElementChild.title].checked;
            if (!foundUnchecked && !listItems[i].lastElementChild.checked) {
                foundUnchecked = true;
            }
            if (!foundChecked && listItems[i].lastElementChild.checked) {
                foundChecked = true;
            }
        }
        this.$.discoveredAddOnsTemplate.render();
        this.$.selectAllDiscoveredAddOnsCheckbox.checked = !foundUnchecked && this.dlgDiscoveredAddOns.length > 0;
        this.$.selectAllDiscoveredAddOnsCheckbox.disabled = (this.dlgDiscoveredAddOns.length == 0);
        this.$.addDiscoveredAddOnButton.disabled = !foundChecked;
    },
    tocMouseEvent : function (e) {
        if (this.moduleIsShowing(e.currentTarget.id)) {
            if (this.allModulesVisible) {
                var items = this.tocItems;
                var setStr = 'tocItems.';
            }
            else {
                var items = this.favoriteTocItems;
                var setStr = 'favoriteTocItems.';
            }
            if (e.currentTarget.className.indexOf('subcontent') >= 0) {
                var index = this.getTocSubItemIndex(e.currentTarget.id, items);
                var subItem = items[index.tocIndex].dups[index.dupIndex];
                var item = rovUtils.shallowCopy(subItem);
                setStr += index.tocIndex + '.dups.' + index.dupIndex;
            }
            else {
                var index = this.getTocIndex(e.currentTarget.id, items);
                var item = rovUtils.shallowCopy(items[index]);
                setStr += String(index);
            }
            item.iconHidden = e.type == 'mouseleave';
            this.set(setStr, item);
        }
    },
    toggleDrawerClicked : function (e) {
        var narrow = !this.$.rovDrawerPanel.narrow;
        this.$.rovDrawerPanel._responsiveChange(narrow);
        this.set('toggleIcon', narrow ? 'chevron-right' : 'chevron-left');
        this.set('toggleTooltip', narrow ? 'Show modules list' : 'Hide modules list');
        this.$.rovTitle.hidden = narrow;
        this.$.titleSpan.style.left = narrow ? '30px' : '210px';
        this.toolbarResponseToResize(narrow);
    },
    toggleFullScreen : function (fullMode) {
        function savePrevState(elem) {
            var elemStyle = window.getComputedStyle(elem);
            var cardStyle = window.getComputedStyle(elem.$.viewPaperCard);
            var contentDivStyle = window.getComputedStyle(elem.$.viewContentDiv);
            var prevState = {};
            prevState.viewWidth = elemStyle.width;
            prevState.viewHeight = elemStyle.height;
            prevState.left = elemStyle.left;
            prevState.top = elemStyle.top;
            prevState.cardWidth = cardStyle.width;
            prevState.cardHeight = cardStyle.height;
            prevState.viewContentDivPosition = contentDivStyle.position;
            elem.prevState = prevState;
        }

        this.inFullScreenMode = fullMode;
        if (fullMode) {
            for (var i = 0; i < this.rovModuleViews.length; i++) {
                var elem = this.getViewById(this.rovModuleViews[i].id);
                savePrevState(elem);
                this.maximizeView(elem);
            }
            for (var i = 0; i < this.rovViews.length; i++) {
                var elem = this.getViewById(this.rovViews[i].id);
                savePrevState(elem);
                this.maximizeView(elem);
            }
        }
        else {
            for (var i = 0; i < this.rovModuleViews.length; i++) {
                var elem = this.getViewById(this.rovModuleViews[i].id);
                this.restorePrevViewState(elem);
            }
            for (var i = 0; i < this.rovViews.length; i++) {
                var elem = this.getViewById(this.rovViews[i].id);
                this.restorePrevViewState(elem);
            }
        }
    },
    toolbarButtonsState : function() {
        var allViewsLength = this.rovModuleViews.length + this.rovViews.length;
        this.$.viewToTopButton.disabled = allViewsLength == 0;
        this.$.saveAllButton.disabled = (this.moduleViewsShowing() + this.storableRovViewsShowing()) < 1;
        this.$.repeatRefreshButton.disabled = this.$.refreshAllButton.disabled =
                (this.dataViewsShowing() < 1 || this.repeatHandle != 0);
        this.$.overflowRepeatRefreshButton.disabled = this.$.repeatRefreshButton.disabled;
        this.$.overflowRefreshAllButton.disabled = this.$.refreshAllButton.disabled;
        this.$.closeAllButton.disabled = allViewsLength == 0;
        this.$.overflowCloseAllButton.disabled = this.$.closeAllButton.disabled;
        this.$.titleOverflowDialogButton.disabled = this.$.overflowRefreshAllButton.disabled && this.$.overflowCloseAllButton.disabled;
        if (this.$.titleOverflowDialogButton.disabled &&
            this.$.titleOverflowDialog.opened) {
              this.$.titleOverflowDialog.close();
        }
        if (allViewsLength == 0) {
            this.setCurrentDashboard('');
        }
        this.enableOtherViews = this.otherViews.length > 0;
        if (this.$$('#otherViewsButton')) {
            this.$$('#otherViewsButton').disabled = !this.enableOtherViews;
        }
        if (this.$$('#overflowOtherViewsButton')) {
            this.$$('#overflowOtherViewsButton').disabled = !this.enableOtherViews;
        }
        this.$.fullScreenButton.disabled =  allViewsLength == 0;
        this.$.exitFullScreenButton.disabled =  allViewsLength == 0;
        this.$.overflowFullScreenButton.disabled = this.$.fullScreenButton.disabled;
        this.$.overflowExitFullScreenButton.disabled = this.$.exitFullScreenButton.disabled;
    },
    toolbarDragEnter : function (e) {
        this.dashboardDrag(e);
    },
    toolbarDragOver : function (e) {
        this.dashboardDrag(e, 'dragover');
    },
    toolbarResponseToResize : function (narrow) {
        var panelWidth = this.$.rovDrawerPanel.offsetWidth;
        var addOnsButtonWidth = this.enableOtherViews ? 36 : 0;
        if (!narrow) {
            if (panelWidth < 810) {
                this.$.rovTitle.hidden = true;
                this.$.titleSpan.style.left = '30px';
            }
            else {
                this.$.rovTitle.hidden = false;
                this.$.titleSpan.style.left = '210px';
            }
            this.$.titleSpan.hidden = panelWidth < (490 + addOnsButtonWidth);
            if (panelWidth >= 250 && panelWidth < 335) {
                this.$.titleOverflowDialogButton.style.left = '14px';
            }
            if (panelWidth < 250) {
                this.$.rovOptionsButton.hidden = true;
                this.$.dashboardDiv.hidden = true;
                this.$.titleOverflowDialogButton.hidden = true;
            }
            else if (panelWidth < 285) {
                this.$.rovOptionsButton.hidden = true;
                this.$.dashboardDiv.hidden = true;
                this.$.titleOverflowDialogButton.hidden = false;
            }
            else if (panelWidth < 310) {
                this.$.rovOptionsButton.hidden = true;
                this.$.dashboardDiv.hidden = true;
                this.$.titleOverflowDialogButton.hidden = false;
            }
            else {
                this.$.titleOverflowDialogButton.hidden = !this.$.titleSpan.hidden;
                if (panelWidth < 300) {
                    this.$.rovOptionsButton.hidden = true;
                    this.$.dashboardDiv.hidden = true;
                }
                else {
                    this.$.rovOptionsButton.hidden = false;
                    this.$.dashboardDiv.hidden = false;
                    if (!this.$.titleOverflowDialogButton.hidden) {
                        this.$.titleOverflowDialogButton.style.left = '24px';
                    }
                    if (panelWidth < 480 && this.$.titleOverflowDialogButton.hidden) {
                        this.$.rovOptionsButton.style.right = '5px';
                    }
                    else {
                        this.$.rovOptionsButton.style.right = '12px';
                    }
                }
            }
        }
        else {
            this.$.rovOptionsButton.hidden = false;
            this.$.dashboardDiv.hidden = false;
            this.$.titleSpan.hidden = panelWidth < (300 + addOnsButtonWidth);
            this.$.titleOverflowDialogButton.hidden = !this.$.titleSpan.hidden;
            if (panelWidth < 335) {
                if (!this.$.titleOverflowDialogButton.hidden) {
                    this.$.titleOverflowDialogButton.style.left = '24px';
                }
                else {
                    this.$.titleSpan.style.left = '18px';
                }
            }
            else {
                this.$.titleSpan.style.left = '30px';
            }
        }
        if (this.$.titleOverflowDialogButton.hidden &&
            this.$.titleOverflowDialog.opened) {
              this.$.titleOverflowDialog.close();
        }
    },
    viewDataGraphClosed : function (id) {
        for (var i = 0; i < this.plotViews.length; i++) {
            if (this.plotViews[i].elem.id == id) {
                this.plotViews.splice(i, 1);
                break;
            }
        }
    },
    viewDropDisplay : function (event, id, xOffset, yOffset, dlgElem) {
        var elem = dlgElem ? dlgElem : this.getViewById(id);
        var top;
        var left;
        if (elem.style.top == '') {
            top = yOffset;
            left = xOffset;
        }
        else {
            top = Number(elem.style.top.replace(/px/, ''));
            left = Number(elem.style.left.replace(/px/, ''));
            top = top + yOffset;
            left = left + xOffset;
        }
        if (!dlgElem) {
            var right = left + elem.clientWidth;
            var bottom = top + elem.clientHeight - 10;
            var allViews = [];
            for (var i = 0; i < this.rovModuleViews.length; i++) {
                allViews.push(this.rovModuleViews[i]);
            }
            for (var i = 0; i < this.rovViews.length; i++) {
                allViews.push(this.rovViews[i]);
            }
            var snapObj = this.snapCorner(event, id, elem, top, left, right, bottom, allViews);
            if (!snapObj.cornerFound) {
                snapObj = this.snapEdge(event, id, elem, top, left, right, bottom, allViews);
            }
            if (!snapObj.cornerFound && !snapObj.topAssigned) {
                snapObj.top -= 8;   /* -8 for cursor arrow */
            }
        }
        else {
            var snapObj = {top: top, left: left};
        }
        elem.style.top = snapObj.top + 'px';
        elem.style.left = snapObj.left + 'px';
        elem.style.zIndex = String(this.getMaxZindex() + 1);
    },
    viewFromGraphOrCustom : function(moduleName, viewName, viewArgs, rowKeys) {
        function viewArgsCompare(srcArgs, view) {
            var viewArgs = (view.viewArgs && view.viewArgs[view.argsId]) ?
                           view.viewArgs[view.argsId] :
                           view.getSavedViewArgs(view.argsId);
            return (rovUtils.getArgsString(srcArgs) == rovUtils.getArgsString(viewArgs));
        }

        var found = false;
        var foundElems = [];
        for (var i = 0; i < this.rovModuleViews.length; i++) {
            var elem = this.getViewById(this.rovModuleViews[i].id);
            if (elem.moduleName == moduleName && elem.viewName == viewName) {
                if (viewArgs) {
                    if (elem.argsId && viewArgs[elem.argsId] &&
                        viewArgsCompare(viewArgs[elem.argsId], elem)) {

                        foundElems.push(elem);
                    }
                }
                else {
                    foundElems.push(elem);
                }
            }
        }
        if (foundElems.length > 0 && rowKeys != null) {
            for (var i = 0; i < foundElems.length; i++) {  /* check view for visible column */
                var elem = foundElems[i];
                if (rowKeys.selectedColumnName) {
                    if (elem.$.tiRovTable.columnIsVisible(rowKeys.selectedColumnName)) {
                        this.viewToForeground(elem);
                        elem.$.tiRovTable.highlightCell(rowKeys);
                        found = true;
                        break;
                    }
                }
                else if (elem.$.tiRovTable.columnIsVisible(rowKeys.primaryKeyColumnName)) {
                    this.viewToForeground(elem);
                    elem.$.tiRovTable.highlightRow({columnName: rowKeys.primaryKeyColumnName, columnValue: rowKeys.primaryKeyColumnValue});
                    found = true;
                    break;
                }
            }
        }
        if (!found) {
            /* open a view */
            var offset = this.getNewClickedViewPosition();
            var columnVisible = null;
            if (rowKeys != null) {
                columnVisible = this.columnIsVisibleFromStorage(moduleName, viewName, rowKeys.selectedColumnName);
                rowKeys.showColumn = !columnVisible;
            }
            var newElem = this.newModuleView(moduleName, viewName, viewArgs);
            newElem.highlightCell = rowKeys;
            if (this.inFullScreenMode) {
                this.maximizeNewView(newElem, offset);
            }
            else {
                if (offset.left > 0) {
                    newElem.style.left = offset.left + 'px';
                }
                if (offset.top > 0) {
                    newElem.style.top = offset.top + 'px';
                }
                if (columnVisible) {
                    var viewsData = this.getViewsDataStorage();
                    var key = moduleName + '.' + viewName;
                    if (viewsData[key] && viewsData[key].width) {
                        newElem.$.viewPaperCard.style.width = viewsData[key].width;
                        newElem.$.viewPaperCard.style.height = viewsData[key].height;
                        newElem.$.viewContentDiv.style.position = 'absolute';
                    }
                }
            }
            this.viewToForeground(newElem);
        }
    },
    viewResized : function (event, resizeObj) {
        var elem = resizeObj.elem;
        var positionElem = resizeObj.positionElem;
        var top = positionElem.style.top == '' ? 0 : Number(positionElem.style.top.replace(/px/, ''));
        var left = positionElem.style.left == '' ? 0 : Number(positionElem.style.left.replace(/px/, ''));
        var right = left + elem.clientWidth;
        var bottom = top + elem.clientHeight;
        var allViews = [];
        for (var i = 0; i < this.rovModuleViews.length; i++) {
            allViews.push(this.rovModuleViews[i]);
        }
        for (var i = 0; i < this.rovViews.length; i++) {
            allViews.push(this.rovViews[i]);
        }
        var snapObj = this.snapSize(event, positionElem.id, elem, top, left, right, bottom, resizeObj.leftResize, resizeObj.topResize, allViews);
        if (snapObj.cornerFound || snapObj.topEdgeFound || snapObj.rightEdgeFound || snapObj.leftEdgeFound) {
            if (snapObj.left > 0) {
                var newWidth = resizeObj.width + (resizeObj.left - snapObj.left);
                positionElem.style.left = snapObj.left + 'px';
                elem.style.width = newWidth + 'px';
                positionElem.style.width = newWidth + 'px';
            }
            else if (snapObj.right > 0) {
                var newWidth = snapObj.right - left;
                elem.style.width = newWidth + 'px';
                positionElem.style.width = newWidth + 'px';
            }
            if (snapObj.top > 0) {
                positionElem.style.top = snapObj.top + 'px';
            }
            if (snapObj.bottom > 0) {
                elem.style.height = (snapObj.top > 0) ? (snapObj.bottom - snapObj.top + 2) + 'px' :
                                    (snapObj.bottom - top - 8) + 'px';
            }
            else {
                elem.style.height = (snapObj.siblingTop - top) + 'px';
            }
        }
        else if (snapObj.bottomEdgeFound) {
            positionElem.style.top = snapObj.siblingBottom + 'px';
            elem.style.height = (bottom - snapObj.siblingBottom) + 'px';
        }
    },
    viewToForeground : function (elem, maxZ) {
        elem.style.zIndex = String(maxZ ? maxZ + 1 : this.getMaxZindex() + 1);
        if (this.viewContainer.scrollHeight != this.viewContainer.clientHeight) {
            this.viewContainer.scrollTop = elem.offsetTop;
        }
        if (this.viewContainer.scrollWidth != this.viewContainer.clientWidth) {
            this.viewContainer.scrollLeft = elem.offsetLeft;
        }
    },
    viewToTopDialog : function(event) {
        function indexInModuleViews(moduleViews, index) {
            var found = false;
            for (var i = 0; i < moduleViews.length; i++) {
                if (moduleViews[i].indexOf(index) != -1) {
                    found = true;
                    break;
                }
            }
            return (found);
        }

        this.viewToTopList = [];
        var viewList = [];
        var elemList = [];
        var moduleViews = [];

        for (var i = 0; i < this.rovModuleViews.length; i++) {
            var elem = this.getViewById(this.rovModuleViews[i].id);
            elemList.push(elem);
        }

        for (var i = 0; i < elemList.length; i++) {
            if (indexInModuleViews(moduleViews, i)) {
                continue;
            }
            var viewIndexes = [i];
            for (j = i + 1; j < elemList.length; j++) {
                if (elemList[i].shortModuleName == elemList[j].shortModuleName &&
                    elemList[i].viewName == elemList[j].viewName &&
                    elemList[i].moduleName != elemList[j].moduleName) {

                    if (!indexInModuleViews(moduleViews, j)) {
                        viewIndexes.push(j);
                    }
                }
            }
            if (viewIndexes.length > 0) {
                moduleViews.push(viewIndexes);
            }
        }
        for (var i = 0; i < moduleViews.length; i++) {
            var listItem = {};
            if (moduleViews[i].length > 1) {
                var elem = elemList[moduleViews[i][0]];
                listItem.name = elem.shortModuleName + ' ' + elem.viewName;
                listItem.hasDups = true;
                listItem.dupList = [];
                for (var j = 0; j < moduleViews[i].length; j++) {
                    var dupItem = {};
                    elem = elemList[moduleViews[i][j]];
                    dupItem.fullName = elem.moduleName + ' ' + elem.viewName;
                    dupItem.qualName = elem.moduleName.substr(0, elem.moduleName.lastIndexOf('.'));
                    dupItem.id = elem.id;
                    listItem.dupList.push(dupItem);
                }
            }
            else {
                var elem = elemList[moduleViews[i][0]];
                listItem.name = elem.shortModuleName + ' ' + elem.viewName;
                listItem.fullName = elem.moduleName + ' ' + elem.viewName;
                listItem.id = elem.id;
            }
            viewList.push(listItem);
        }
        for (var i = 0; i < this.rovViews.length; i++) {
            var listItem = {};
            var elem = this.getViewById(this.rovViews[i].id);
            listItem.name = elem.viewName;
            listItem.fullName = elem.viewName;
            listItem.id = elem.id;
            viewList.push(listItem);
        }
        this.set('viewToTopList', viewList);
        var dialog = this.$.listViewsDialog;
        dialog.resetFit();
                           /* 24 is the padding + 10 */
        dialog.style.left = (event.x - 34) + 'px';
        dialog.open();
        dialog.style.zIndex = String(this.getMaxZindex() + 1);
        if (this.$.viewToTopMenu.selected != undefined) {
            this.$.viewToTopMenu.selected = -1;
        }
    },
    viewToTopMenuItemSelected : function (e) {
        this.$.viewToTopMenu.selected = -1;
        if (e.detail.item.id) {
            var elem = this.getViewById(e.detail.item.id);
            if (elem) {
                this.viewToForeground(elem);
            }
            if (e.target.className.indexOf('SubMenu') >= 0) {
                e.target.selected = -1;
            }
            this.$.listViewsDialog.close();
        }
    }
});
