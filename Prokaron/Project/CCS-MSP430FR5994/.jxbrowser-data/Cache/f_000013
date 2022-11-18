Polymer({ is : "ti-rov-panel",
    behaviors: [
      Polymer.IronResizableBehavior
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
        this.set('rovInit', rovUtils.getLocalStorageObject('rovInit-storage'));
        this.setRovSettings(rovUtils.getLocalStorageObject('rovSettings-storage'));
        if (this.rovInit.autoConnect) {
            this.autoConnect();
        }
        else {
            this.showConnectDialog();
        }
        this.$.rovDrawerPanel.disableEdgeSwipe = true;
        this.$.rovDrawerPanel.disableSwipe = true;
        this.viewContainer = this.$.rovHeaderPanel.$.mainContainer;
        this.$.openDashboardFileButton.hidden = !this.ccs7;
        this.$.overflowOpenDashboardFileButton.hidden = !this.ccs7;
        /* initial toolbar state */
        this.$.rovTitle.hidden = false;
        this.$.titleSpan.hidden = true;
        this.$.dashboardDiv.hidden = true;
    },
    ready : function () {
        this.ccs7 = window.ccs7_browser;
        this.rovData = document.querySelector('#rovData');
    },
    addDiscoveredAddOnClicked : function (e) {
        var selected = [];
        var listItems = this.$.discoveredAddOnsList.items;
        for (var i = 0; i < listItems.length; i++) {
            if (listItems[i].lastElementChild.checked) {
                selected.push({path: this.dlgDiscoveredAddOns[i].path,
                               root: this.dlgDiscoveredAddOns[i].root,
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
                    if (this.createAddOn(e, item.path, item.root)) {
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
        var dialog = document.getElementById('manageAddOnsDialog');
        if (dialog) {
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
        }
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
        var dialog = document.getElementById('manageAddOnsDialog');
        if (dialog) {
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
        }
    },
    autoConnect : function () {
        var exe = (this.ccs7 && this.rovInit.noExe) ? '' : this.rovInit.exe;
        var dialog = this.$.autoConnectProgressDialog;
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
                    this.loadMissingDashboardAddOn(dashboard[i], false);
                }
            }
        }
    },
    clearAllLocalStorage: function () {
        rovUtils.setLocalStorageObject('dashboards-storage', null);
        rovUtils.setLocalStorageObject('favorites-storage', null);
        rovUtils.setLocalStorageObject('rovInit-storage', null);
        rovUtils.setLocalStorageObject('rovSettings-storage', null);
        rovUtils.setLocalStorageObject('viewsData-storage', null);
        if (this.$.manageStorageDialog.opened) {
            this.$.manageStorageDialog.close();
        }
    },
    clearAllStorageClicked : function (e) {
        this.confirmDialog('Clear all ROV storage', 'Are you sure?', 'clearAll');
    },
    clearAViewChecked : function (e) {
        if (!this.$.manageStorageDialog.opened) {
            return;
        }
        this.listItemWithSelectAllOptionClicked(e, 'selectAllViewsStorageCheckbox',
                                                'viewsStorageList');
        this.enableChecklistDialogActionButton(e, 'viewsStorageList',
                                               'clearViewStorageButton');
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
    clearViewStorageClicked : function (e) {
        var viewsStorage = rovUtils.getLocalStorageObject('viewsData-storage');
        var listItems = this.$.viewsStorageList.items;
        for (var i = 0; i < listItems.length; i++) {
            if (listItems[i].lastElementChild.checked) {
                viewsStorage[listItems[i].innerText.trim()] = null;
                listItems[i].lastElementChild.checked = false;
            }
        }
        rovUtils.setLocalStorageObject('viewsData-storage', viewsStorage);
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
            this.$.manageStorageDialog.close();
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
            found = false;
            for (var i = 0; i < this.rovViews.length; i++) {
                if (this.rovViews[i].id != viewId) {
                    rovViews.push(rovUtils.shallowCopy(this.rovViews[i]));
                }
                else {
                    found = true;
                }
            }
            if (found) {
                this.rovViews = rovViews;
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
    confirmDialog : function(title, label, objName, sel) {
        var dialog = document.getElementById('confirmDialog');
        if (dialog) {
            this.confirmProps = {};
            this.confirmProps.objName = objName;
            this.confirmProps.sel = sel;
            this.confirmTitle = title;
            this.confirmLabel = label;
            dialog.open();
            dialog.style.zIndex = String(this.getMaxZindex() + 2);
        }
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
        this.rovInit.autoConnectChecked = this.$.autoConnectCheckbox.checked;
        rovUtils.setLocalStorageObject('rovInit-storage', this.rovInit);
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
    },
    connectKeyPress : function (e) {
        var keyCode = e.keyCode || e.which;
        if (keyCode === 13 && this.$.connectButton.disabled == false) {
            this.connectClicked(e);
        }
    },
    createAddOn : function (e, path, root) {
        var addOn = {};
        addOn.path = path;
        addOn.root = root;
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
        }
        this.saveAddOnMru(addOn.path);
        var rovSettings = rovUtils.getLocalStorageObject('rovSettings-storage');
        rovSettings.otherViews = this.otherViews;
        rovUtils.setLocalStorageObject('rovSettings-storage', rovSettings);
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
    dashboardButtonsState : function() {
        var hasSavedDashboards = this.hasSavedDashboards();
        this.enableOpenDashboardButton(hasSavedDashboards);
        this.$.deleteDashboardButton.disabled = !hasSavedDashboards;
        this.$.overflowDeleteDashboardButton.disabled = this.$.deleteDashboardButton.disabled;
        this.$.exportDashboardsClicked.hidden = !hasSavedDashboards;
        var allViewsLength = this.rovModuleViews.length + this.rovViews.length;
        this.$.openDashboardFileButton.disabled = false;
        this.$.overflowOpenDashboardFileButton.disabled = this.$.openDashboardFileButton.disabled;
        this.$.saveDashboardButton.disabled = allViewsLength == 0;
        this.$.overflowSaveDashboardButton.disabled = this.$.saveDashboardButton.disabled;
        if (allViewsLength == 0) {
            this.setCurrentDashboard('');
        }
    },
    dashboardConflictsDialog : function () {
        var dialog = document.getElementById('overwriteDashboardsDialog');
        if (dialog) {
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
        }
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
            this.enableOpenDashboardButton(true, 'dodgerblue');
        }
        else {
            if (dragOp == 'dragover') {
                e.dataTransfer.dropEffect = 'none';
            }
            this.enableOpenDashboardButton(this.hasSavedDashboards());
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
                else if (dashboard[0].fileName && dashboard[0].numDashboards) {
                    valid = true;
                    dashboard.shift();
                    this.dashboardsArrayLoaded(dashboard);
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
    dashboardsArrayLoaded : function (dashboardsArr) {
        var savedDashboards = Object.keys(this.dashboards);
        this.dashboardConflicts = [];
        for (var i = 0; i < dashboardsArr.length; i++) {
            var name = dashboardsArr[i].pop();
            if (savedDashboards.length > 0 && savedDashboards.indexOf(name) >= 0) {
                this.dashboardConflicts.push({name: name, dashboard: dashboardsArr[i], file: null});
            }
            else {
                this.dashboards[name] = dashboardsArr[i];
                rovUtils.setLocalStorageObject('dashboards-storage', this.dashboards);
                if (savedDashboards.length == 0) {
                    this.enableOpenDashboardButton(true);
                    this.$.deleteDashboardButton.disabled = false;
                }
                this.showStatus('Saved dashboard ' + name, 'info');
            }
        }
        if (this.dashboardConflicts.length > 0) {
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
        var dialog = e.target.offsetParent;
        if (dialog) {
            var sel = e.currentTarget.selectedItem.textContent.trim();
            this.$.dashboardNamesMenu.selected = -1;
            if (dialog.name == 'Open' || dialog.name == 'Intro') {
                if (dialog.name == 'Intro') {
                    this.$.introDialog.close();
                }
                dialog.close();
                if (dialog.name == 'Open' &&
                    (this.rovModuleViews.length + this.rovViews.length) > 0) {

                    this.$.openingDashboardAlertDialog.rovFile = null;
                    this.$.openingDashboardAlertDialog.rovSel = sel;
                    this.$.openingDashboardAlertDialog.open();
                }
                else {
                    this.showSelectedDashboard(sel);
                }
            }
            else if (dialog.name == 'Delete') {
                this.confirmDialog('Delete dashboard',
                                   'Delete ' + sel + ' ?',
                                   'dashboard', sel);
            }
            else if (dialog.name == 'Save') {
                dialog.close();
                this.$.saveDashboardInput.value = sel;
                this.$.saveDashboardComment.value = this.dashboards[sel][0].comment ?
                                                    this.dashboards[sel][0].comment : '';
            }
        }
    },
    dataOptionsClicked : function (e) {
        var dialog = document.getElementById('dataOptionsDialog');
        if (dialog) {
            dialog.addEventListener('iron-overlay-closed', function() {
                var reason = this.closingReason;
                return (false);
            });
            dialog.open();
            dialog.style.left = this.isNarrow() ? '0px': rovUtils.leftPaneWidth + 'px';
            dialog.style.zIndex = String(this.getMaxZindex() + 1);
            this.$.repeatRefreshInput.value = String(this.refreshInterval);
            this.$.repeatRefreshInputInvalid.hidden = true;
            this.$.dataTimeoutInput.value = String(this.dataTimeoutPeriod);
            this.$.dataTimeoutInputInvalid.hidden = true;
            this.$.connectTimeoutInput.value = String(this.connectTimeoutPeriod);
            this.$.connectTimeoutInputInvalid.hidden = true;
        }
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
        this.$.listDashboardsDialog.close();
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
        this.dashboardButtonsState();
    },
    deleteDashboardClicked : function(e) {
        this.listDashboardsDialog('Delete', e);
    },
    destroyClickedElement : function(event) {
        document.body.removeChild(event.target);
    },
    dialogCloseClicked : function (e) {
        var dlgId = e.target.id.replace(/Close/, '');
        this.$[dlgId].cancel(e);
    },
    dialogClosed : function (e) {
        /* re-enable toolbar buttons */
        this.$.rovOptionsButton.disabled = false;
        this.$.dashboardOverflowDialogButton.disabled = false;
        if (this.$$('#otherViewsButton')) {
            this.$$('#otherViewsButton').disabled = false;
        }
        this.toolbarButtonsState();
        this.dashboardButtonsState();
    },
    dialogOpened : function () {
        /* disable toolbar buttons */
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

        this.$.rovOptionsButton.disabled = true;
        this.$.dashboardOverflowDialogButton.disabled = true;
        this.enableOpenDashboardButton(false);
        this.$.deleteDashboardButton.disabled = true;
        this.$.saveDashboardButton.disabled = true;
        this.$.openDashboardFileButton.disabled = true;
    },
    discoverAddOns : function () {
        this.$.rediscoverAddOnButton.disabled = true;
        this.rovData.discoverAddOns(this);
    },
    discoverAddOnsCallback : function (res, addOns) {
        if (res == null) {
            var discoveredAddOns = [];
            for (var i = 0; i < addOns.length; i++) {
                discoveredAddOns.push({path : addOns[i].path,
                                       root : addOns[i].root,
                                       name : addOns[i].name});
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
        e.currentTarget.style.zIndex = String(this.getMaxZindex() + 1);
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
    enableOpenDashboardButton : function (enable, color) {
        if (enable) {
            this.$.openDashboardButton.style.color =  color ? color : tiRovStyles.titleToolbarColor;
            this.$.openDashboardButton.style.cursor = 'pointer';
            this.$.openDashboardButton.title = 'Open a dashboard';
        }
        else {
            this.$.openDashboardButton.style.color =  tiRovStyles.titleToolbarDisabledColor;
            this.$.openDashboardButton.style.cursor = 'default';
            this.$.openDashboardButton.title = '';
        }
        this.$.overflowOpenDashboardButton.style.color =  this.$.openDashboardButton.style.color;
        this.$.overflowOpenDashboardButton.style.cursor = this.$.openDashboardButton.style.cursor;
        this.$.overflowOpenDashboardButton.title = this.$.openDashboardButton.title;
    },
    exeInputClicked : function (e) {
        this.$.exeInput.$.input.select();
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
        if (dialog) {
            dialog.open();
            dialog.style.left = this.isNarrow() ? '0px': rovUtils.leftPaneWidth + 'px';
            dialog.style.zIndex = String(this.getMaxZindex() + 10);

            /* Init checkboxes and clear selection button */
            var listItems = this.$.exportDashboardsList.items;
            for (var i = 0; i < listItems.length; i++) {
                listItems[i].lastElementChild.checked = true;
            }
            this.$.exportDashboardsList.selected = -1;
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
                if (viewTabs.length > 1) {
                    this.viewableModules.push(keys[i]);
                }
            }
            this.setCurModsList(this.curModsLabel);
        }
        else {
            alert(error);
        }
        this.discoverAddOns();
        if (this.rovInit.showIntro != false) {
            this.introDialog();
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
        var viewsStorage = rovUtils.getLocalStorageObject('viewsData-storage');
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
    initRov : function (exe, commLink) {
        var fullPath = exe + '?' + 'comm=' + (commLink == 'Debugger' ? 'DSLite' : commLink);
        if ((commLink == 'DSLite' || commLink == 'Debugger') && window.ti != undefined) {
            fullPath = fullPath + ":" + window.ti.debug.cloudagent.dsPort;
        }
        this.rovData.setExecutable(fullPath, this);
    },
    introDashboardsClicked : function(event) {
        if (this.hasSavedDashboards()) {
            this.initDashboardsDialogList();
            var dialog = document.getElementById('introListDashboardsDialog');
            if (dialog) {
                dialog.name = 'Intro';
                dialog.style.left = rovUtils.leftPaneWidth + event.currentTarget.offsetLeft + 4 + 'px';
                dialog.style.top = 43 + event.currentTarget.offsetTop + event.currentTarget.clientHeight - 5 + 'px';
                dialog.style.width = event.currentTarget.clientWidth + 'px';
                dialog.open();
                var introDialog = document.getElementById('introDialog');
                dialog.style.zIndex = String(Number(introDialog.style.zIndex) + 1);
                this.$.introDashboardNamesMenu.selected = -1;
            }
        }
        else {
            this.openDashboardFileClicked();
            this.$.introDialog.close();
        }
    },
    introDialog : function () {
        var dialog = this.$.introDialog;
        if (dialog) {
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
        }
    },
    introDialogClosed : function (e) {
        this.dialogClosed();
        this.rovInit.showIntro = !this.$.noIntroCheckbox.checked;
        rovUtils.setLocalStorageObject('rovInit-storage', this.rovInit);
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
        return ((e.target.id == 'openDashboardButton' ||
                 e.target.id == 'overflowOpenDashboardButton' ||
                 (e.target.id == 'icon' && e.target.icon == 'dashboard')) &&
                 this.isADragDropFile(e));
    },
    listDashboardsDialog : function(dlgName, event) {
        this.initDashboardsDialogList();
        var id = (dlgName == 'Save') ? 'listSavedDashboardsDialog' : 'listDashboardsDialog';
        var dialog = document.getElementById(id);
        if (dialog) {
            dialog.name = dlgName;
            if (dlgName == 'Save') {
                dialog.style.left = event.x + 'px';
                dialog.style.top = (event.y + 14) + 'px';
            }
            else if (dlgName == 'Open' || dlgName == 'Delete') {
                dialog.style.marginRight = '0px';
                dialog.style.marginLeft = '0px';
                if (event.currentTarget.id.indexOf('overflow') == 0) {
                    dialog.style.left = '';
                    dialog.style.right = '4px';
                    dialog.style.top = (event.currentTarget.id.indexOf('overflow') == 0) ? (event.y - 4) + 'px' : '24px';
                }
                else {
                    dialog.style.right = '';
                    dialog.style.left = (event.x - 34) + 'px';
                    dialog.style.top = '24px';
                }
            }
            dialog.open();
            if (dlgName == 'Save') {
                var saveDialog = document.getElementById('saveDashboardDialog');
                dialog.style.zIndex = String(Number(saveDialog.style.zIndex) + 1);
                this.$.listSavedDashboardsMenu.selected = -1;
            }
            else {
                dialog.style.zIndex = String(this.getMaxZindex() + 2);
                this.$.dashboardNamesMenu.selected = -1;
            }
        }
    },
    listDashboardsDialogClosed : function () {
        if (this.$.dashboardOverflowDialog.opened) {
            this.$.dashboardOverflowDialog.close();
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
        if (this.isDupAddOn(path)) {
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
    loadMissingDashboardAddOn : function(dashboardAddOnView, showIt) {
        var path = this.parseAddOnPath(dashboardAddOnView.elemPath);
        this.addOnsHash[path] = dashboardAddOnView;
        this.importHref(path,
            function(e){
                var key = this.getAddOnHashKey(e.target.outerHTML);
                var dashboardItem = this.addOnsHash[key];
                var elem = document.createElement(dashboardItem.elemName);
                if (elem.viewName == undefined || elem.textContent.length == 0) {
                    this.showStatus('Error creating ' + dashboardItem.viewName + ' element', 'error');
                    Polymer.dom.flush();
                }
                else {
                    var addOn = {};
                    addOn.element = dashboardItem.elemName;
                    addOn.path = dashboardItem.elemPath;
                    addOn.root = dashboardItem.elemRoot;
                    addOn.name = elem.viewName;
                    addOn.href = e.target.href;
                    if (showIt) {
                        this.newDashboardRovView(dashboardItem);
                    }
                    var other = (this.otherViews.length > 0) ? rovUtils.shallowCopy(this.otherViews) : [];
                    other.push(addOn);
                    this.set('otherViews', other);
                    this.enableOtherViews = true;
                    var rovSettings = rovUtils.getLocalStorageObject('rovSettings-storage');
                    rovSettings.otherViews = this.otherViews;
                    rovUtils.setLocalStorageObject('rovSettings-storage', rovSettings);
                }
            },
            function(e){
                var dashboardItem = this.addOnsHash[this.getAddOnHashKey(e.target.outerHTML)];
                this.showStatus('Error importing ' + dashboardItem.viewName + ' at ' + e.target.href, 'error');
            }
        );
    },
    manageStorageKeyPress : function (e) {
        var keyCode = e.keyCode || e.which;
        if (keyCode === 13) {
            if (!this.$.clearViewStorageButton.disabled) {
                this.clearViewStorageClicked();
            }
            if (this.$.manageStorageDialog.opened) {
                this.$.manageStorageDialog.close();
            }
        }
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
                    for (k = 0; k < uSame.length; k++) {
                        var dup = {};
                        dup.name = uSame[k].qual;
                        dup.id = uSame[k].qual + '.' + uSame[k].name;
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
                modulesToc.push(tocItem);
            }
            ++i;
        }
        if (i == units.length - 1 &&
            (i == 0 || (i > 0 && units[i].name != units[i - 1].name))) {
            var tocItem = {};
            tocItem.name = units[i].name;
            tocItem.id = units[i].qual + '.' + units[i].name;
            modulesToc.push(tocItem);
        }
        return (modulesToc);
    },
    modClicked : function(e) {
        if (this.$.introDialog.opened) {
            this.$.introDialog.close();
        }
        var viewsData = rovUtils.getLocalStorageObject('viewsData-storage');
        var moduleName = e.currentTarget.id;
        var viewName = null;
        var viewArgs = null;
        if (viewsData[moduleName]) {
            viewName = viewsData[moduleName].defaultViewName;
            viewArgs = viewsData[moduleName].viewArgs;
        }
        else if (this.viewList[moduleName]) {
            var viewTabs = rovUtils.getViewTabs(this.viewList, moduleName);
            viewName = viewTabs[0].name;
        }
        var elem = this.newModuleView(moduleName, viewName, false, viewArgs);
        elem.style.zIndex = String(this.getMaxZindex() + 1);
        var offset = this.getNewClickedViewPosition();
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
          dialog.style.left = event.x + 'px';
          dialog.style.top = event.y + 'px';
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
        var rovSettings = rovUtils.getLocalStorageObject('rovSettings-storage');
        rovSettings.curModsLabel = sel;
        rovUtils.setLocalStorageObject('rovSettings-storage', rovSettings);
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
        var dialog = document.getElementById(dlgId);
        if (dialog) {
            dialog.open();
            dialog.style.left = (event.x + 4) + 'px';
            dialog.style.top = (event.y + 14) + 'px';
        }
    },
    mruSelected : function (e) {
        var pre = e.currentTarget.id.replace(/MruMenu/, '');
        var sel = e.currentTarget.selectedItem.textContent.trim();
        this.$[e.currentTarget.id].selected = -1;
        this.$[pre + 'MruDialog'].close();
        this.$[pre + 'Input'].value = sel;
    },
    multiDashboardFileLoaded : function (file, done, e) {
        var valid = false;
        var error = null;
        try {
            var dashboard = JSON.parse(e.target.result);
            if (rovUtils.isArray(dashboard) &&
                (dashboard[0].moduleName || dashboard[0].elemName)) {
                valid = true;
                var name = dashboard.pop();
                var savedDashboards = Object.keys(this.dashboards);
                if (savedDashboards.length > 0 && savedDashboards.indexOf(name) >= 0) {
                    this.dashboardConflicts.push({name: name, dashboard: dashboard, file: file});
                }
                else {
                    this.dashboards[name] = dashboard;
                    rovUtils.setLocalStorageObject('dashboards-storage', this.dashboards);
                    if (savedDashboards.length == 0) {
                        this.enableOpenDashboardButton(true);
                        this.$.deleteDashboardButton.disabled = false;
                    }
                    this.showStatus('Saved dashboard ' + name, 'info');
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
        if (done && this.dashboardConflicts.length > 0) {
            this.resolveSaveDashboardsConflicts();
        }
    },
    newDashboardRovView : function (dashboardItem) {
        var view = this.newRovView({name: dashboardItem.elemName, path: dashboardItem.elemPath, root: dashboardItem.elemRoot});
        view.style.left = dashboardItem.left + 'px';
        view.style.top = dashboardItem.top + 'px';
        view.$.viewPaperCard.style.width = dashboardItem.width;
        view.$.viewPaperCard.style.height = dashboardItem.height;
        view.style.zIndex = dashboardItem.zIndex;
    },
    newModuleView : function(moduleName, viewName, dashboardViewsData, viewArgs) {
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
        if (dashboardViewsData) {
            view.dashboardView = true;
            view.viewsData = dashboardViewsData;
        }
        if (viewArgs) {
            view.viewArgs = viewArgs;
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
    openDashboardClicked : function(e) {
        if (this.hasSavedDashboards()) {
            this.listDashboardsDialog('Open', e);
        }
    },
    openDashboardDrop : function (e) {
        if (this.ccs7) {
            e.preventDefault();
            return;
        }
        this.dashboardButtonsState();
        if (this.isADragDropFile(e)) {
            e.preventDefault();
            var files = e.dataTransfer.files;
            if (files.length == 1) {
                if (this.rovModuleViews.length + this.rovViews.length > 0) {
                    this.$.openingDashboardAlertDialog.rovFile = files[0];
                    this.$.openingDashboardAlertDialog.rovSel = null;
                    this.$.openingDashboardAlertDialog.open();
                }
                else {
                    this.dashboardFileReader(files[0]);
                }
            }
            else {
                this.dashboardConflicts = [];
                for (var i = 0; i < files.length; i++) {
                    var reader = new FileReader();
                    reader.onload = this.multiDashboardFileLoaded.bind(this, files[i], i == (files.length - 1));
                    reader.readAsText(files[i]);
                }
            }
            if (this.$.dashboardOverflowDialog.opened) {
                this.$.dashboardOverflowDialog.close();
            }
        }
        else {
            alert('Not a valid dashboard file');
        }
    },
    openDashboardFileClicked : function (e) {
        this.$.openDashboardFileInput.value = '';
        this.$.openDashboardFileInput.click();
    },
    openDashboardFileSelected : function (e) {
        var files = e.currentTarget.files;
        if (files.length == 1) {
            if (files[0].name.match(/\.rov\.json$/) != null) {
                if (this.rovModuleViews.length + this.rovViews.length > 0) {
                    this.$.openingDashboardAlertDialog.rovFile = files[0];
                    this.$.openingDashboardAlertDialog.rovSel = null;
                    this.$.openingDashboardAlertDialog.open();
                }
                else {
                    this.dashboardFileReader(files[0]);
                }
            }
            else {
                this.showStatus(files[0].name + ' is not a valid dashboard file', 'error');
            }
            this.$.openDashboardFileInput.value = '';
        }
    },
    openingDashboardAlertDialogClosed : function (e) {
        if (e.detail.confirmed) {
            if (this.$.openingDashboardAlertDialog.rovFile == null &&
                this.$.openingDashboardAlertDialog.rovSel != null) {

                this.showSelectedDashboard(this.$.openingDashboardAlertDialog.rovSel);
            }
            else if (this.$.openingDashboardAlertDialog.rovFile != null &&
                this.$.openingDashboardAlertDialog.rovSel == null) {

                this.dashboardFileReader(this.$.openingDashboardAlertDialog.rovFile);
            }
        }
    },
    otherViewsDialog : function(event) {
        var dialog = document.getElementById('otherViewsDialog');
        if (dialog) {
            this.set('dlgOtherViews', this.otherViews);
                               /* 24 is the padding + 10 */
            dialog.style.left = (event.x - 34) + 'px';
            dialog.open();
            dialog.style.zIndex = String(this.getMaxZindex() + 1);
            if (this.$.otherViewsMenu.selected != undefined) {
                this.$.otherViewsMenu.selected = -1;
            }
        }
    },
    otherViewsDialogClosed : function(e) {
        this.set('dlgOtherViews', []);
    },
    otherViewSelected : function(e) {
        var elemInfo = this.getAddOnInfo(e.detail.item.id, e.detail.item.title);
        var elemProps = {name: e.detail.item.id, path: elemInfo.path, root: elemInfo.root};
        var elem = this.newRovView(elemProps, true);
        this.$.otherViewsMenu.selected = -1;
        var dialog = document.getElementById('otherViewsDialog');
        if (dialog) {
            dialog.close();
        }
        var offset = this.getNewClickedViewPosition();
        if (offset.left > 0) {
            elem.style.left = offset.left + 'px';
        }
        if (offset.top > 0) {
            elem.style.top = offset.top + 'px';
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
    parseDiscoveredAddOn : function (addOn) {
        var parsed = addOn.replace(/\\/g,'/').replace(/\.html$/, '');
        return (parsed.substr(parsed.lastIndexOf('/') + 1));
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
                elem.showView(elem.viewName, true, false);
            }
        }
        /* Notify any other views */
        for (var i = 0; i < this.rovViews.length; i++) {
            var view = this.getViewById(this.rovViews[i].id);
            if (view && view.onRefresh) {
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
        var rovSettings = rovUtils.getLocalStorageObject('rovSettings-storage');
        rovSettings.otherViews = this.otherViews;
        rovUtils.setLocalStorageObject('rovSettings-storage', rovSettings);
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
    rovOptionsClicked : function(e) {
        var dialog = this.$.rovOptionsMenuDialog;
        if (dialog) {
            dialog.open();
            dialog.style.zIndex = String(this.getMaxZindex() + 1);

            /* set disabled state of storage options button */
//            this.$.storageOptionsDialog.disabled = !this.hasViewsStorage();

            /* show/disable connect dialog at startup */
            this.$.showConnectDialogCheckbox.checked = !this.rovInit.autoConnect;
        }
    },
    rovOptionsItemSelected : function (e) {
        var selected = e.currentTarget.selectedItem.id;
        this.$.rovOptionsMenu.selected = -1;
        this.$.rovOptionsMenuDialog.close();
        if (selected != 'showConnectDialogItem') {
            this[selected](e); /* id of the item is the function name */
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
                var rovSettings = rovUtils.getLocalStorageObject('rovSettings-storage');
                rovSettings.otherViews = this.otherViews;
                rovUtils.setLocalStorageObject('rovSettings-storage', rovSettings);
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
        if (this.ccs7) {
            this.commLinkMrus.shift(); /* remove 'Debugger' from top */
        }
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
        if (this.$.dashboardOverflowDialog.opened) {
            this.$.dashboardOverflowDialog.close();
        }
        var dialog = document.getElementById('saveDashboardDialog');
        if (dialog) {
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
        }
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
                var elemObj = {};
                elemObj.id = elem.id;
                elemObj.moduleName = elem.moduleName;
                elemObj.viewName = elem.viewName;
                elemObj.viewsData = {};
                if (elem.viewName) {
                    elemObj.viewsData[elem.moduleName + '.' + elem.viewName] =
                        elem.getViewsData()[elem.moduleName + '.' + elem.viewName];
                }
                if (elem.viewArgs) {
                    elemObj.viewArgs = elem.viewArgs;
                }
                elemObj.left = elem.offsetLeft;
                elemObj.top  = elem.offsetTop;
                elemObj.width = elem.$.viewPaperCard.offsetWidth;
                elemObj.height = elem.$.viewPaperCard.offsetHeight;
                elemObj.zIndex = elem.style.zIndex;
                elemObj.dashboardVersion = this.dashboardVersion;
                dashArr.push(elemObj);
                /*  If any changes are made to the fields here, make sure to change */
                /*  the dashboard version property!!!!!!!!!  */
            }
            for (var i = 0; i < this.rovViews.length; i++) {
                var elem = this.getViewById(this.rovViews[i].id);
                var elemObj = {};
                elemObj.elemName = elem.name;
                elemObj.elemPath = elem.path;
                elemObj.elemRoot = elem.root;
                elemObj.id = elem.id;
                elemObj.viewName = elem.viewName;
                elemObj.left = elem.offsetLeft;
                elemObj.top  = elem.offsetTop;
                elemObj.width = elem.$.viewPaperCard.style.width;
                elemObj.height = elem.$.viewPaperCard.style.height;
                elemObj.zIndex = elem.style.zIndex;
                elemObj.dashboardVersion = this.dashboardVersion;
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
            var rovSettings = rovUtils.getLocalStorageObject('rovSettings-storage');
            rovSettings.refreshInterval = this.refreshInterval;
            rovSettings.dataTimeoutPeriod = this.dataTimeoutPeriod;
            rovSettings.connectTimeoutPeriod = this.connectTimeoutPeriod;
            rovUtils.setLocalStorageObject('rovSettings-storage', rovSettings);
            this.$.dataOptionsDialog.close();
        }
    },
    saveDroppedDashboard : function (name, dashboard, show) {
        this.dashboards[name] = dashboard;
        rovUtils.setLocalStorageObject('dashboards-storage', this.dashboards);
        if (show) {
            this.showDashboard(dashboard, name);
        }
        else {
            this.dashboardButtonsState();
        }
        this.showStatus('Saved dashboard: ' + name, 'info');
    },
    saveExeMru : function (exe) {
        if (this.ccs7) {
            this.exeMrus.shift(); /* remove 'use current program' from top */
        }
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
        var rovSettings = rovUtils.getLocalStorageObject('rovSettings-storage');
        rovSettings[mrusProp] = this[mrusProp];
        rovUtils.setLocalStorageObject('rovSettings-storage', rovSettings);
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
        if (!this.$.manageStorageDialog.opened) {
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
            if (this.$.connectDialog.hidden == false) {
                this.$.connectButton.disabled = false;
                this.connectMessage = res;
                this.$.connectErrorIcon.hidden = false;
                this.$.connectMessageDiv.hidden = false;
                this.$.connectProgress.hidden = true;
            }
            else if (this.$.autoConnectProgressDialog.opened) {
                this.$.autoConnectProgressDialog.close();
                if (this.rovInit.autoConnect) {
                    this.rovInit.autoConnect = false;
                    rovUtils.setLocalStorageObject('rovInit-storage', this.rovInit);
                }
                this.showConnectDialog(res);
            }
            else {
                this.showStatus(res, 'error');
            }
            return;
        }
        if (this.$.connectDialog.hidden == false) {
            this.rovInit.exe = this.$.exeInput.value;
            this.rovInit.commLink = this.$.commLinkInput.value;
            this.rovInit.autoConnect = this.$.autoConnectCheckbox.checked;
            this.rovInit.noExe = this.$.exeInput.value == this.useCurrentProgram;
            this.averageIntroProgess();
            rovUtils.setLocalStorageObject('rovInit-storage', this.rovInit);
            this.$.connectDialog.hidden = true;
            this.$.connectDialog.style.display = 'none';
        }
        else if (this.$.autoConnectProgressDialog.opened) {
            this.averageIntroProgess();
            rovUtils.setLocalStorageObject('rovInit-storage', this.rovInit);
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

        /* Enable toolbars */
        this.$.toggleDrawerBtn.disabled = false;
        this.$.rovOptionsButton.disabled = false;
        this.$.dashboardOverflowDialogButton.disabled = false;
        this.$.openDashboardButton.disabled = false;
        this.$.overflowOpenDashboardButton.disabled = false;
        this.panelWidth = this.$.rovDrawerPanel.offsetWidth;
        this.toolbarResponseToResize(false);
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
        this.rovData.getViewList(this);
        this.inited = true;
    },
    setExecutableProgressCallback : function (res) {
        if (this.$.connectDialog.hidden == false && this.$.connectProgress.hidden == false) {
            this.$.connectProgress.value += this.rovInit.progressInfo.step;
            ++this.rovInit.progressInfo.notifyCount;
        }
        else if (this.$.connectDialog.hidden == true && this.rovInit.autoConnect) {
            this.$.autoConnectProgress.value += this.rovInit.progressInfo.step;
            ++this.rovInit.progressInfo.notifyCount;
        }
        this.connectMessage = res;
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
    showConnectDialog : function (autoConnectError) {
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
            var exeMrus = [this.useCurrentProgram];
            for (var i = 0; i < this.exeMrus.length; i++) {
                exeMrus.push(this.exeMrus[i]);
            }
            this.set('exeMrus', exeMrus);

            var commLinkMrus = ['Debugger'];
            for (i = 0; i < this.commLinkMrus.length; i++) {
                if (this.commLinkMrus[i] != 'Debugger') {
                    commLinkMrus.push(this.commLinkMrus[i]);
                }
            }
            this.set('commLinkMrus', commLinkMrus);
        }
        this.$.autoConnectCheckbox.checked = (this.rovInit.autoConnectChecked) == true ? true : false;
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
        this.$.connectDialog.hidden = false;
        this.$.connectDialog.style.display = 'inline-block';
    },
    showConnectDialogChecked : function (e) {
        this.$.rovOptionsMenu.selected = -1;
        this.rovInit.autoConnect = !e.currentTarget.checked;
        rovUtils.setLocalStorageObject('rovInit-storage', this.rovInit);
    },
    showDashboard : function (dashboard, dashName) {
        if (dashboard.length > 0 &&
            (!dashboard[0].dashboardVersion ||
              dashboard[0].dashboardVersion != this.dashboardVersion)) {
            this.showStatus('Incompatible dashboard', 'warning');
        }
        this.closeAllViews();
        this.setCurrentDashboard(dashName);
        for (var i = 0; i < dashboard.length; i++) {
            if (dashboard[i].moduleName) {
                if (this.allModules.indexOf(dashboard[i].moduleName) >= 0) {
                    var view = this.newModuleView(dashboard[i].moduleName, dashboard[i].viewName,
                                                  dashboard[i].viewsData, dashboard[i].viewArgs);
                    view.style.left = dashboard[i].left + 'px';
                    view.style.top = dashboard[i].top + 'px';
                    view.$.viewContentDiv.style.position = 'absolute';
                    view.$.viewPaperCard.style.width = dashboard[i].width + 'px';
                    view.$.viewPaperCard.style.height = dashboard[i].height + 'px';
                    view.style.zIndex = dashboard[i].zIndex;
                }
                else {
                    this.showStatus('Module ' + dashboard[i].moduleName + ' is not present in ' + document.title, 'warning');
                }
            }
            else if (dashboard[i].elemName) {   /* is otherView */
                if (!this.otherViewsHaveElem(dashboard[i].elemName)) {
                    this.loadMissingDashboardAddOn(dashboard[i], true);
                }
                else {
                    this.newDashboardRovView(dashboard[i]);                }
            }
        }
    },
    showSavedDashboardsClicked : function (event) {
        this.listDashboardsDialog('Save', event);
    },
    showSelectedDashboard : function (sel) {
        /* Get the original dashboard in case one in memory has been changed */
        var dashboards = rovUtils.getLocalStorageObject('dashboards-storage');
        var dashboard = dashboards[sel] ? dashboards[sel] : this.dashboards[sel]; /* defensive */
        this.showDashboard(dashboard, sel);
    },
    showStatus : function (message, type) {
        var statusItems = [];
        if (this.statusItems.length > 0) {
            statusItems = rovUtils.shallowCopy(this.statusItems);
        }
        var statusItem = {};
        if (!type) {
            statusItem.iconHidden = true;
            statusItem.style = 'margin-left:33px';
        }
        else {
            statusItem.style = '';
            statusItem.iconHidden = false;
            if (type == 'warning') {
                statusItem.icon = 'warning';
                statusItem.iconStyle = 'color:goldenrod;--iron-icon-stroke-color:black';
                statusItem.iconClass = 'consoleWarningIcon';
            }
            else if (type == 'error') {
                statusItem.icon = 'error';
                statusItem.iconStyle = 'color:red';
                statusItem.iconClass = 'consoleErrorIcon';
            }
            else if (type == 'info') {
                statusItem.icon = 'info-outline';
                statusItem.iconStyle = 'color:blue';
                statusItem.iconClass = 'consoleStatusIcon';
            }
        }
        statusItem.message = message;
        statusItems.push(statusItem);
        var fontInfo = tiRovStyles.rovContentFontSize + 'pt ' + 'Open Sans';
        var consoleWidth = 0;
        for (var i = 0; i < statusItems.length; i++) {
            var width = rovUtils.getStringWidth(statusItems[i].message, fontInfo);
            if (width > consoleWidth) {
                consoleWidth = width;
            }
        }
        consoleWidth += 24;
        var maxWidth = this.isNarrow() ? document.documentElement.clientWidth : (document.documentElement.clientWidth - rovUtils.leftPaneWidth);
        if (consoleWidth >= maxWidth) {
            consoleWidth = maxWidth - 2;
        }
        this.$.statusConsole.style.width = String(consoleWidth) + 'px';
        if (statusItems.length > 10) {
            if (this.$.statusConsole.style.overflowY != 'auto') {
                this.$.statusConsole.style.overflowY = 'auto';
            }
            this.$.statusConsole.style.height = String(11 * 24) + 'px';
        }
        this.set('statusItems', statusItems);
        this.$.statusConsole.hidden = false;
    },
    snapCorner : function (event, id, xOffset, yOffset, elem, top, left, right, bottom, allViews) {
        var cornerFound = false;
        var newTop;
        var newLeft;
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
                if (left > siblingRight - 20 && left < siblingRight + 20) {  /* elem left edge at sibling right edge */
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
                else if (right > siblingLeft - 20 && right < siblingLeft + 20) {  /* elem right edge at sibling left edge */
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
                    if (top > siblingBottom - 30 && top < siblingBottom + 20) {  /* elem top edge at sibling bottom edge */
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
                    else if (bottom > siblingTop - 20 && bottom < siblingTop + 20) {  /* elem bottom edge at sibling top edge */
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
    snapEdge : function (event, id, xOffset, yOffset, elem, top, left, right, bottom, allViews) {
        var topAssigned = false;
        var leftAssigned = false;
        var newLeft;
        var newTop;
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
                if (left > siblingRight - 20 && left < siblingRight + 20) {       /* elem left edge -> sibling right edge */
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
    storageOptionsDialog : function (e) {
        var dialog = document.getElementById('manageStorageDialog');
        if (dialog) {
            var viewsStorage = rovUtils.getLocalStorageObject('viewsData-storage');
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
        }
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
    toggleDrawerClicked : function (e) {
        var narrow = !this.$.rovDrawerPanel.narrow;
        this.$.rovDrawerPanel._responsiveChange(narrow);
        this.set('toggleIcon', narrow ? 'chevron-right' : 'chevron-left');
        this.set('toggleTooltip', narrow ? 'Show modules list' : 'Hide modules list');
        this.$.rovTitle.hidden = narrow;
        this.$.titleSpan.style.left = narrow ? '30px' : '210px';
        this.toolbarResponseToResize(narrow);
    },
    toolbarButtonsState : function() {
        var allViewsLength = this.rovModuleViews.length + this.rovViews.length;
        this.$.viewToTopButton.disabled = allViewsLength <= 1;
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
    },
    toolbarDragEnter : function (e) {
        this.dashboardDrag(e);
    },
    toolbarDragOver : function (e) {
        this.dashboardDrag(e, 'dragover');
    },
    toolbarResponseToResize : function (narrow) {
        var panelWidth = this.$.rovDrawerPanel.offsetWidth;
        if (!narrow) {
            if (panelWidth < 775) {
                this.$.rovTitle.hidden = true;
                this.$.titleSpan.style.left = '30px';
            }
            else {
                this.$.rovTitle.hidden = false;
                this.$.titleSpan.style.left = '210px';
            }
            this.$.titleSpan.hidden = panelWidth < 455;
            this.$.dashboardDiv.hidden = panelWidth < 585;
            if (panelWidth >= 215 && panelWidth < 300) {
                this.$.titleOverflowDialogButton.style.left = '14px';
                this.$.titleOverflowDialog.style.left = '194px';
            }
            if (panelWidth < 215) {
                this.$.rovOptionsButton.hidden = true;
                this.$.titleOverflowDialogButton.hidden = true;
                this.$.dashboardOverflowDialogButton.hidden = true;
            }
            else if (panelWidth < 250) {
                this.$.rovOptionsButton.hidden = true;
                this.$.titleOverflowDialogButton.hidden = false;
                this.$.dashboardOverflowDialogButton.hidden = true;
            }
            else if (panelWidth < 275) {
                this.$.rovOptionsButton.hidden = true;
                this.$.titleOverflowDialogButton.hidden = false;
                this.$.dashboardOverflowDialogButton.hidden = false;
                this.$.dashboardOverflowDialogButton.style.right = '0px';
                this.$.dashboardOverflowDialog.style.right = '0px';
            }
            else {
                this.$.titleOverflowDialogButton.hidden = !this.$.titleSpan.hidden;
                this.$.dashboardOverflowDialogButton.hidden = !this.$.dashboardDiv.hidden;
                if (panelWidth < 300) {
                    this.$.rovOptionsButton.hidden = true;
                    this.$.dashboardOverflowDialogButton.style.right = '16px';
                    this.$.dashboardOverflowDialog.style.right = '12px';
                }
                else {
                    this.$.rovOptionsButton.hidden = false;
                    if (!this.$.titleOverflowDialogButton.hidden) {
                        this.$.titleOverflowDialogButton.style.left = '24px';
                        this.$.titleOverflowDialog.style.left = '206px';
                    }
                    if (panelWidth < 480 && this.$.titleOverflowDialogButton.hidden) {
                        this.$.rovOptionsButton.style.right = '5px';
                        this.$.dashboardOverflowDialogButton.style.right = '25px';
                        this.$.dashboardOverflowDialog.style.right = '21px';
                    }
                    else {
                        this.$.rovOptionsButton.style.right = '12px';
                        if (!this.$.dashboardOverflowDialogButton.hidden) {
                            this.$.dashboardOverflowDialogButton.style.right = '40px';
                            this.$.dashboardOverflowDialog.style.right = '36px';
                        }
                    }
                }
            }
        }
        else {
            this.$.rovOptionsButton.hidden = false;
            this.$.dashboardDiv.hidden = panelWidth < 400;
            this.$.dashboardOverflowDialogButton.hidden = !this.$.dashboardDiv.hidden;
            this.$.titleSpan.hidden = panelWidth < 265;
            this.$.titleOverflowDialogButton.hidden = !this.$.titleSpan.hidden;
            if (panelWidth < 300) {
                if (!this.$.titleOverflowDialogButton.hidden) {
                    this.$.titleOverflowDialogButton.style.left = '24px';
                    this.$.titleOverflowDialog.style.left = '206px';
                }
                else {
                    this.$.titleSpan.style.left = '18px';
                }
                this.$.rovOptionsButton.style.right = '2px';
                this.$.dashboardOverflowDialogButton.style.right = '28px';
                this.$.dashboardOverflowDialog.style.right = '24px';
            }
            else {
                this.$.titleSpan.style.left = '30px';
                this.$.rovOptionsButton.style.right = '12px';
                this.$.dashboardOverflowDialogButton.style.right = '40px';
                this.$.dashboardOverflowDialog.style.right = '36px';
            }
        }
        if (this.$.listDashboardsDialog.opened &&
            (this.$.listDashboardsDialog.name == 'Open' || this.$.listDashboardsDialog.name == 'Delete')) {
              this.$.listDashboardsDialog.close();
        }
        if (this.$.dashboardOverflowDialogButton.hidden &&
            this.$.dashboardOverflowDialog.opened) {
            this.$.dashboardOverflowDialog.close();
        }
        if (this.$.titleOverflowDialogButton.hidden &&
            this.$.titleOverflowDialog.opened) {
              this.$.titleOverflowDialog.close();
        }
    },
    viewDropDisplay : function (event, id, xOffset, yOffset) {
        var elem = this.getViewById(id);
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
        var right = left + elem.clientWidth;
        var bottom = top + elem.clientHeight - 10;
        var allViews = [];
        for (var i = 0; i < this.rovModuleViews.length; i++) {
            allViews.push(this.rovModuleViews[i]);
        }
        for (var i = 0; i < this.rovViews.length; i++) {
            allViews.push(this.rovViews[i]);
        }
        var snapObj = this.snapCorner(event, id, xOffset, yOffset, elem, top, left, right, bottom, allViews);
        if (!snapObj.cornerFound) {
            snapObj = this.snapEdge(event, id, xOffset, yOffset, elem, top, left, right, bottom, allViews);
        }
        if (!snapObj.cornerFound && !snapObj.topAssigned) {
            snapObj.top -= 8;   /* -8 for cursor arrow */
        }
        elem.style.top = snapObj.top + 'px';
        elem.style.left = snapObj.left + 'px';
        elem.style.zIndex = String(this.getMaxZindex() + 1);
    },
    viewToTopDialog : function(event) {
        this.viewToTopList = [];
        var viewList = [];
        for (var i = 0; i < this.rovModuleViews.length; i++) {
            var listItem = {};
            var elem = this.getViewById(this.rovModuleViews[i].id);
            listItem.name = elem.shortModuleName;
            listItem.fullName = elem.moduleName + ' ' + elem.viewName;
            listItem.id = elem.id;
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
        var dialog = document.getElementById('listViewsDialog');
        if (dialog) {
                               /* 24 is the padding + 10 */
            dialog.style.left = (event.x - 34) + 'px';
            dialog.open();
            dialog.style.zIndex = String(this.getMaxZindex() + 1);
            if (this.$.viewToTopMenu.selected != undefined) {
                this.$.viewToTopMenu.selected = -1;
            }
        }
    },
    viewToTopMenuItemSelected : function (e) {
        var elem = this.getViewById(e.detail.item.id);
        if (elem) {
            elem.style.zIndex = String(this.getMaxZindex() + 1);
        }
        this.$.viewToTopMenu.selected = -1;
        var dialog = document.getElementById('listViewsDialog');
        if (dialog) {
            dialog.close();
        }
    }
});
