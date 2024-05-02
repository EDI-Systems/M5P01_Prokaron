Polymer({ is : "ti-rov-table",

    behaviors: [rovBehaviors.localStorageBehaviors],

    properties: {
        columnStates: {
          type: Array,
          value: function() { return []; }
        },
        viewsData: {
          type: Object,
          value: {}
        },
        dataMessage: {
          type: String,
          value: ""
        },
        expandAll: {
          type: Boolean,
          value: false
        },
        instanceDataTable: {
          type: Object,
          value: function() { return {}; }
        },
        instanceTable: {
          type: Object,
          value: function() { return {}; }
        },
        isInstanceDataView: {
          type: Boolean,
          value: false
        },
        isInstanceView: {
          type: Boolean,
          value: false
        },
        isRawView: {
          type: Boolean,
          value: false
        },
        isTreeView: {
          type: Boolean,
          value: false
        },
        isTreeTableView: {
          type: Boolean,
          value: false
        },
        moduleName: {
          type: String,
          value: ""
        },
        rawTable: {
          type: Array,
          value: function() { return []; }
        },
        showDataMessage: {
          type: Boolean,
          value: false
        },
        treeTable: {
          type: Array,
          value: function() { return []; }
        },
        treeTableTable: {
          type: Object,
          value: function() { return []; }
        },
        viewName: {
          type: String,
          value: ""
        },
        isGlobal: {
          type: Boolean,
          value: true
        },
        viewData: {
          type: Object,
          value: {}
        },
        viewType: {
          type: String,
          value: ""
        },
        hasFormats: {
          type: Boolean,
          value: false
        },
        rovTdStyle: {
          type: String,
          value: 'display:table-cell;text-align:left;padding-left:4px;padding-right:5px;white-space:nowrap;'
        },
        colsDraggable : {
          type : Boolean,
          value : true
        },
        notifyObj : {
          type: Object,
          value: null
        },
        colFormatsDirty : {
          type: Boolean,
          value: false
        },
        preSelected : {
          type: Boolean,
          value: false
        },
        isMac : {
          type: Boolean,
          value: false
        },
        customTable: {
          type: Object,
          value: function() { return {}; }
        },
        isCustomView: {
          type: Boolean,
          value: false
        },
        highlighted: {
          type: Object,
          value: null
        },
        ccs7 : {
          type: Boolean,
          value: false
        }
    },

    ready : function () {
        this.ccs7 = window.ccs7_browser;
        this.viewsData = this.getViewsDataStorage();
        this.colsDraggable = navigator.appVersion.indexOf('Win') != -1;
        this.isMac = navigator.appVersion.indexOf('Mac') != -1;
    },
    atMaxDepth : function(dataObj) {
        return (dataObj.properties.length > 0 &&
                typeof(dataObj.properties[0]) == 'string' &&
                dataObj.properties[0].search(/Max depth/) >= 0);
    },
    cellArray : function(arrObj, colFormats, colName, fontFamily) {
        if (arrObj.val.length <= 1) {
            arrObj.val = arrObj.val.length == 0 ? '' : arrObj.val[0];
            arrObj.val = this.formatVal(arrObj.val, colFormats, colName);
        }
        else {
            arrObj.arrVals = [];
            var vals = rovUtils.shallowCopy(arrObj.val);
            for (var i = 0; i < vals.length; i++) {
                arrObj.arrVals.push({});
                var linkDesc = this.hasLink(vals[i]);
                if (linkDesc != null) {
                    arrObj.arrVals[i].isLink = true;
                    arrObj.arrVals[i].val = linkDesc[1];
                    arrObj.arrVals[i].href = linkDesc[2];
                    arrObj.arrVals[i].title = linkDesc[3];
                }
                else {
                    arrObj.arrVals[i].isLink = false;
                    arrObj.arrVals[i].val = this.formatVal(vals[i], colFormats, colName);
                }
            }
            arrObj.val = arrObj.arrVals[0];
            arrObj.isArray = true;
            arrObj.style = this.rovTdStyle + 'cursor:default;' + fontFamily;
        }
    },
    cellArrayCompare : function(prevArr, newArr) {
        var changed = false;
        if (newArr.length != prevArr.length) {
            changed = true;
        }
        else {
            for (var i = 0; i < prevArr.length; i++) {
                if (!newArr[i].isLink  && !prevArr[i].isLink &&
                    newArr[i].val != prevArr[i].val) {
                    changed = true;
                    break;
                }
            }
        }
        return (changed);
    },
    cellContextMenu : function (e) {
        if (this.isMac) {
            if (this.preSelected) {
                this.preSelected = false;
                return;
            }
        }
        else if (window.getSelection().toString().length > 0) {
            return;
        }
        var id = e.currentTarget.id;
        if (!id || id.length == 0) {
            return;
        }
        var columnName = this.columnNameFromId(e.currentTarget.id);
        if (!this.isNumericColumn(columnName) && !this.isCustomView) {
            return;
        }
        if (!this.isInstanceView && !this.isInstanceDataView && !this.isCustomView) {
            return;
        }

        if (this.isInstanceView) {
            e.preventDefault();
            var primaryKeyColumnName = this.instanceTable.allCols[0];
            if (rovUtils.isArray(this.viewData)) {
                var row = Number(id.substr(id.lastIndexOf('_') + 1));
                var primaryKeyColumnValue = this.viewData[row][primaryKeyColumnName];
            }
            else {
                var primaryKeyColumnValue = this.viewData[primaryKeyColumnName];
            }
            var cellObj = {event: e,
                           selectedColumnName: columnName,
                           selectedColumnValue: e.target.textContent.trim(),
                           primaryKeyColumnName: primaryKeyColumnName,
                           primaryKeyColumnValue: primaryKeyColumnValue,
                           viewData: this.viewData
            };
            if (this.notifyObj) {
                this.notifyObj.tableCellContextMenu(cellObj);
            }
        }
        else if (this.isInstanceDataView) {
            if (!rovUtils.isArray(this.viewData)) {
                e.preventDefault();
                var primaryKeyColumnName = this.instanceDataTable.allCols[0];
                var row = Number(id.substr(id.lastIndexOf('_') + 1));
                var elemArr = this.viewData.elements;
                var primaryKeyColumnValue = elemArr[row][primaryKeyColumnName];
                var cellObj = {event: e,
                               selectedColumnName: columnName,
                               selectedColumnValue: e.target.textContent.trim(),
                               primaryKeyColumnName: primaryKeyColumnName,
                               primaryKeyColumnValue: primaryKeyColumnValue,
                               viewData: this.viewData
                };
                if (this.notifyObj) {
                    this.notifyObj.tableCellContextMenu(cellObj);
                }
            }
        }
        else if (this.isCustomView) {
            e.preventDefault();
            var cellIndex = Number(id.substr(id.lastIndexOf('_') + 1));
            var cellObj = {event: e, isCustomView: true, cellIndex : cellIndex};
            if (this.notifyObj) {
                this.notifyObj.tableCellContextMenu(cellObj);
            }
        }
    },
    cellDoubleClick : function (e) {
        if (rovUtils.getFormat(e.currentTarget.textContent.trim())) {
            var sel = window.getSelection();
            if (sel.toString().length > 0) {
                sel.selectAllChildren(e.currentTarget);
            }
        }
    },
    cellDropMouseOver : function (e) {
        if (e.srcElement.className.indexOf('rovTdDropdownCell') == -1) {
            return;
        }
        var listElem = e.currentTarget.lastElementChild;
        var vc = thePanel.viewContainer;
        /*
            48        - top toolbar
            offsetTop - the view top
            offsetTop - row top
            8         - paper card margin
            36        - drag div
         */
        var top = 48 + this.notifyObj.offsetTop + e.srcElement.offsetTop + 8 + 36;
        listElem.style.top = (top - vc.scrollTop) + 'px';

                                             /* + 5 table margin, +1 paper card margin */
        var left = this.notifyObj.offsetLeft + 5 + 1 + e.currentTarget.offsetLeft;
        if (!thePanel.isNarrow()) {
            left += rovUtils.leftPaneWidth;
        }
        listElem.style.left = (left - vc.scrollLeft) + 'px';
    },
    cellMouseDown : function (e) {
       if (this.isMac && e.button == 2) {
           this.preSelected = false;
           if (window.getSelection().toString().length > 0) {
               this.preSelected = e.currentTarget.id == window.getSelection().focusNode.id;
           }
       }
    },
    chRawNodeTapped : function(event) {
        var indexArr = event.currentTarget.id.split('_');
        var parentIndex = Number(indexArr[0]);  /* temporary! */
        var chIndex = Number(indexArr[1]);
        var goldChildRows = this.rawTable[parentIndex].goldChildRows;
        goldChildRows[chIndex].expanded = !goldChildRows[chIndex].expanded;
        var newRows = [];
        var i = 0;
        while (i < goldChildRows.length) {
            if (goldChildRows[i].id == event.currentTarget.id) {
                if (goldChildRows[i].expanded) {
                   goldChildRows[i].icon = 'remove';
                   var newRow = rovUtils.shallowCopy(goldChildRows[i++]);
                   newRows.push(newRow);
                }
                else {
                   goldChildRows[i].icon = 'add';
                   var newRow = rovUtils.shallowCopy(goldChildRows[i]);
                   newRows.push(newRow);

                   /* Skip everything at a deeper level */
                   var level = goldChildRows[i++].level;
                   while (i < goldChildRows.length && goldChildRows[i].level > level) {
                       ++i;
                   }
                }
                if (i >= goldChildRows.length) {
                    break;
                }
            }
            if (goldChildRows[i].hasIcon && !goldChildRows[i].expanded) {
                newRows.push(goldChildRows[i]);
                /* skip all deeper levels */
                var level = goldChildRows[i++].level;
                while (i < goldChildRows.length && goldChildRows[i].level > level) {
                    ++i;
                }
            }
            else {
                newRows.push(goldChildRows[i++]);
            }
        }
        this.set('rawTable.' + indexArr[0] + '.childRows', newRows);
    },
    chttNodeTapped : function(event) {
        var indexArr = event.currentTarget.id.split('_');
        var parentIndex = Number(indexArr[0]);  /* temporary! */
        var chIndex = Number(indexArr[1]);
        var goldChildRows = this.treeTableTable[parentIndex].goldChildRows;
        goldChildRows[chIndex].expanded = !goldChildRows[chIndex].expanded;
        var newRows = [];
        var i = 0;
        while (i < goldChildRows.length) {
            if (goldChildRows[i].id == event.currentTarget.id) {
                if (goldChildRows[i].expanded) {
                   goldChildRows[i].icon = 'remove';
                   newRows.push(rovUtils.shallowCopy(goldChildRows[i++]));
                }
                else {
                   goldChildRows[i].icon = 'add';
                   newRows.push(rovUtils.shallowCopy(goldChildRows[i]));

                   /* Skip everything at a deeper level */
                   var level = goldChildRows[i++].level;
                   while (i < goldChildRows.length && goldChildRows[i].level > level) {
                       ++i;
                   }
                }
                if (i >= goldChildRows.length) {
                    break;
                }
            }
            if (goldChildRows[i].hasIcon && !goldChildRows[i].expanded) {
                newRows.push(goldChildRows[i]);
                /* skip all deeper levels */
                var level = goldChildRows[i++].level;
                while (i < goldChildRows.length && goldChildRows[i].level > level) {
                    ++i;
                }
            }
            else {
                newRows.push(goldChildRows[i++]);
            }
        }
        this.treeTableTable.childRowsWithAllCols[parentIndex] = newRows;
        var selData = this.getSelectedData('TREE_TABLE', [newRows]);
        this.set('treeTableTable.' + parentIndex + '.childRows', selData.rows[0]);
    },
    clearHighlight : function () {
        if (this.highlighted != null) {
            /* if a cell or row is already highlighted, unhighlight it */
            if (this.isAttached && this.clientWidth > 0) {
                if (this.highlighted.elem) {
                    this.highlighted.elem.style.backgroundColor = this.highlighted.prevBackgroundColor;
                }
                else if (this.highlighted.rowKey) {
                    var row = this.findRow(this.highlighted.rowKey);
                    if (row >= 0) {
                        var id;
                        var elem;
                        var columns = this.highlighted.columns;
                        for (var i = 0; i < columns.length; i++) {
                            id = columns[i].name + '_' + row;
                            elem = this.$$('#' + id);
                            if (elem) {
                                elem.style.backgroundColor = columns[i].backgroundColor;
                            }
                        }
                    }
                }
            }
            this.highlighted = null;
        }
    },
    closeColumnsSelector : function (e) {
        var dialog = this.$.columnsSelector;
        if (dialog) {
            dialog.close();
        }
    },
    colFormatChanged : function(e) {
        var newFormat = e.target.selectedItem.textContent.trim();
        var colName = e.currentTarget.parentElement.previousElementSibling.firstElementChild.id;
        var columnStates = this.viewsData[this.moduleName + '.' + this.viewName].columnStates;
        for (var i = 0; i < columnStates.length; i++) {
            if (columnStates[i].name == colName) {
                columnStates[i].format = newFormat;
                break;
            }
        }
        if (this.isGlobal) {
            this.saveViewData();
        }

        /* Apply format */
        var allCols;
        if (this.viewType == 'INSTANCE' || this.viewType == 'MODULE') {
            allCols = this.instanceTable.allCols;
        }
        else if (this.viewType == 'INSTANCE_DATA' || this.viewType == 'MODULE_DATA') {
            allCols = this.instanceDataTable.allCols;
        }
        else if (this.viewType == 'TREE_TABLE') {
            allCols = this.treeTableTable.allCols;
        }
        else if (this.viewType == 'CUSTOM') {
            allCols = this.customTable.allCols;
        }
        this.displayTable(this.viewData, this.viewName, this.viewType, allCols, false, true);
    },
    colsChanged : function(e) {
        if (!this.$.columnsSelector.opened) {
            return;
        }
        var formatSelect = e.currentTarget.parentElement.nextElementSibling.firstElementChild;
        if (formatSelect) {
            formatSelect.disabled = !e.currentTarget.checked;
        }
        var colName = e.currentTarget.id;
        var columnStates = this.viewsData[this.moduleName + '.' + this.viewName].columnStates;
        for (var i = 0; i < columnStates.length; i++) {
            if (columnStates[i].name == colName) {
                columnStates[i].checked = e.currentTarget.checked;
                break;
            }
        }
        if (this.notifyObj && this.notifyObj.notifyOnContentResize) {
            this.notifyObj.contentResizing();
        }
        this.columnsChanged();
        if (this.notifyObj && this.notifyObj.notifyOnContentResize) {
            this.notifyObj.contentResized();
        }
        if (this.isGlobal) {
            this.saveViewData();
        }
    },
    columnDragOver : function (event) {
        event.preventDefault();
    },
    columnDragStart : function (event) {
        event.dataTransfer.setData("text", 'cell: ' + this.getColumnIndex(event.target.textContent));
    },
    columnDrop : function (event) {
        var data = event.dataTransfer.getData("text");
        if (data.indexOf('cell: ') == -1) {
           return;
        }
        event.preventDefault();
        var srcCol = Number(data.substr(6));
        var destCol = this.getColumnIndex(event.target.textContent);
        if (srcCol == destCol) {
            return;
        }
        var viewData = this.viewsData[this.moduleName + '.' + this.viewName];
        if (this.isInstanceView) {
            var instTable = {};
            /* preserve old */
            instTable.allCols = this.instanceTable.allCols;
            instTable.fullRows = this.instanceTable.fullRows;
            instTable.columns = this.instanceTable.columns;

            var newCols = rovUtils.shallowCopy(this.instanceTable.columns);
            newCols.splice(destCol, 0, newCols.splice(srcCol, 1)[0]);
            this.updateColumnsData(instTable, newCols);

            var newDisplayCols = rovUtils.shallowCopy(this.instanceTable.displayColumns);
            newDisplayCols.splice(destCol, 0, newDisplayCols.splice(srcCol, 1)[0]);
            if (viewData.sortCol) {
                var sortCol = newCols.indexOf(viewData.sortCol);
                if (sortCol == -1) {
                    viewData.sortCol = null;
                }
            }
            if (this.isGlobal) {
                this.saveViewData();
            }

            var newRows = [];
            var row;
            var rows = this.instanceTable.rows;
            for (var i = 0; i < rows.length; i++) {
                row = rovUtils.shallowCopy(rows[i]);
                row.compareKey = rows[i].compareKey;
                row.splice(destCol, 0, row.splice(srcCol, 1)[0]);
                newRows.push(row);
            }

            /* refresh view */
            instTable.rows = newRows;
            instTable.columns = newCols;
            instTable.displayColumns = newDisplayCols;
            this.instanceTable = instTable;
        }
        else if (this.isInstanceDataView) {
            var instDataTable = {};
            this.copyInstanceDataTableProps(instDataTable);

            var newCols = rovUtils.shallowCopy(this.instanceDataTable.columns);
            newCols.splice(destCol, 0, newCols.splice(srcCol, 1)[0]);
            this.updateColumnsData(instDataTable, newCols);

            var newDisplayCols = rovUtils.shallowCopy(this.instanceDataTable.displayColumns);
            newDisplayCols.splice(destCol, 0, newDisplayCols.splice(srcCol, 1)[0]);
            if (viewData.sortCol) {
                var sortCol = newCols.indexOf(viewData.sortCol);
                if (sortCol == -1) {
                    viewData.sortCol = null;
                }
            }
            if (this.isGlobal) {
                this.saveViewData();
            }

            var newRows = [];
            var row;
            var rows = this.instanceDataTable.rows;
            for (var i = 0; i < rows.length; i++) {
                newRows.push({});
                newRows[i].expanded = rows[i].expanded;
                if (instDataTable.showInstSelect) {
                    newRows[i].label = rows[i].label;
                }
                newRows[i].rowVals = [];
                for (var j = 0; j < rows[i].rowVals.length; j++) {
                    row = rovUtils.shallowCopy(rows[i].rowVals[j]);
                    row.compareKey = rows[i].rowVals[j].compareKey;
                    row.splice(destCol, 0, row.splice(srcCol, 1)[0]);
                    newRows[i].rowVals.push(row);
                }
            }

            /* refresh view */
            instDataTable.rows = newRows;
            instDataTable.columns = newCols;
            instDataTable.displayColumns = newDisplayCols;
            this.instanceDataTable = instDataTable;
        }
        else if (this.isTreeTableView) {
            /* preserve old */
            var trTableTable = this.copyTreeTable(this.treeTableTable);
            var newCols = rovUtils.shallowCopy(this.treeTableTable.columns);
            newCols.splice(destCol, 0, newCols.splice(srcCol, 1)[0]);
            this.updateColumnsData(trTableTable, newCols);
            if (this.isGlobal) {
                this.saveViewData();
            }
            for (var i = 0; i < trTableTable.length; i++) {
                for (var j = 0; j < trTableTable[i].childRows.length; j++) {
                    var curChildRow = trTableTable[i].childRows[j];
                    if (!curChildRow.hasIcon && !curChildRow.atMax) {
                        var curRowCells = rovUtils.shallowCopy(curChildRow.cells);
                        curRowCells.splice(destCol, 0, curRowCells.splice(srcCol, 1)[0]);
                        trTableTable[i].childRows[j].cells = curRowCells;
                        trTableTable[i].childRows[j].compareKey = curChildRow.compareKey;
                    }
                }
            }
            /* refresh view */
            trTableTable.columns = newCols;
            this.set('treeTableTable', trTableTable);
        }
        else if (this.isCustomView) {
            var custTable = {};
            /* preserve old */
            custTable.allCols = this.customTable.allCols;
            custTable.fullRows = this.customTable.fullRows;
            custTable.columns = this.customTable.columns;

            var newCols = rovUtils.shallowCopy(this.customTable.columns);
            newCols.splice(destCol, 0, newCols.splice(srcCol, 1)[0]);
            this.updateColumnsData(custTable, newCols);

            var newDisplayCols = rovUtils.shallowCopy(this.customTable.displayColumns);
            newDisplayCols.splice(destCol, 0, newDisplayCols.splice(srcCol, 1)[0]);
            if (viewData.sortCol) {
                var sortCol = newCols.indexOf(viewData.sortCol);
                if (sortCol == -1) {
                    viewData.sortCol = null;
                }
            }

            var newRows = [];
            var row;
            var rows = this.customTable.rows;
            for (var i = 0; i < rows.length; i++) {
                row = rovUtils.shallowCopy(rows[i]);
                row.splice(destCol, 0, row.splice(srcCol, 1)[0]);
                newRows.push(row);
            }

            /* refresh view */
            custTable.rows = newRows;
            custTable.columns = newCols;
            custTable.displayColumns = newDisplayCols;
            this.customTable = custTable;
        }
    },
    columnNameChanged : function (prevName, newName) {
        var key = this.moduleName + '.' + this.viewName;
        if (this.viewsData[key] && this.viewsData[key].columnStates) {
            var columnStates = this.viewsData[key].columnStates;
            for (var i = 0; i < columnStates.length; i++) {
                if (columnStates[i].name == prevName) {
                    columnStates[i].name = newName;
                    break;
                }
            }
        }
    },
    columnNameFromId : function (id) {
        var columnName = id;
        if (this.isInstanceView) {
            columnName = columnName.substr(0, columnName.lastIndexOf('_'));
        }
        else if (this.isInstanceDataView) {
            if (rovUtils.isArray(this.viewData)) {
                columnName = columnName.substr(0, columnName.lastIndexOf('_'));
            }
            columnName = columnName.substr(0, columnName.lastIndexOf('_'));
        }
        else if (this.isCustomView) {
            columnName = columnName.substr(0, columnName.lastIndexOf('_'));
        }
        return (columnName);
    },
    columnIsVisible : function(columnName) {
        var visible = false;
        var columnStates = this.viewsData[this.moduleName + '.' + this.viewName].columnStates;
        for (var i = 0; i < columnStates.length; i++) {
            if (columnStates[i].name == columnName) {
                visible = columnStates[i].checked;
                break;
            }
        }
        return (visible);
    },
    columnsChanged : function() {
        if (this.isInstanceView) {
            var instTable = {};
            /* preserve old */
            instTable.allCols = this.instanceTable.allCols;
            instTable.fullRows = this.instanceTable.fullRows;
            var viewData = this.viewsData[this.moduleName + '.' + this.viewName];

            /* refresh view */
            var selData = this.getSelectedData('INSTANCE', this.instanceTable.fullRows);
            if (viewData.orderedColumns) {
                this.updateColumnsData(instTable, selData.cols);
            }
            var sortCol = -1;
            if (viewData.sortCol) {
                sortCol = selData.cols.indexOf(viewData.sortCol);
                if (sortCol == -1) {
                    viewData.sortCol = null;
                }
            }
            instTable.columns = selData.cols;
            instTable.rows = selData.rows;

            instTable.displayColumns = [];
            for (var i = 0; i < instTable.columns.length; i++) {
                var colObj = {};
                colObj.name = instTable.columns[i];
                if (viewData.sortCol && colObj.name == viewData.sortCol) {
                    colObj.isSortCol = true;
                    colObj.icon = (viewData.sortDir == 'up') ? 'arrow-drop-up' : 'arrow-drop-down';
                }
                else {
                    colObj.isSortCol = false;
                }
                instTable.displayColumns.push(colObj);
            }
            this.instanceTable = instTable;
            if (sortCol >= 0) {
                this.columnSort(sortCol);
            }
            this.$.rovInstanceTemplate.render();
        }
        else if (this.isInstanceDataView) {
            var instDataTable = {};
            this.copyInstanceDataTableProps(instDataTable);
            var viewData = this.viewsData[this.moduleName + '.' + this.viewName];

            /* refresh view */
            var selData = this.getSelectedData('INSTANCE_DATA', this.instanceDataTable.fullRows);
            if (this.viewsData[this.moduleName + '.' + this.viewName].orderedColumns) {
                this.updateColumnsData(instDataTable, selData.cols);
            }
            var sortCol = -1;
            if (viewData.sortCol) {
                sortCol = selData.cols.indexOf(viewData.sortCol);
                if (sortCol == -1) {
                    viewData.sortCol = null;
                }
            }
            instDataTable.columns = selData.cols;
            instDataTable.rows = selData.rows;

            instDataTable.displayColumns = [];
            for (var i = 0; i < instDataTable.columns.length; i++) {
                var colObj = {};
                colObj.name = instDataTable.columns[i];
                if (viewData.sortCol && colObj.name == viewData.sortCol) {
                    colObj.isSortCol = true;
                    colObj.icon = (viewData.sortDir == 'up') ? 'arrow-drop-up' : 'arrow-drop-down';
                }
                else {
                    colObj.isSortCol = false;
                }
                instDataTable.displayColumns.push(colObj);
            }
            this.instanceDataTable = instDataTable;
            if (sortCol >= 0) {
                this.columnSort(sortCol);
            }
            this.$.rovInstanceTemplate.render();
        }
        else if (this.isTreeTableView) {
            /* preserve old */
            var trTableTable = [];
            trTableTable.allCols = this.treeTableTable.allCols;
            trTableTable.childRowsWithAllCols = this.treeTableTable.childRowsWithAllCols;
            for (var i = 0; i < this.treeTableTable.length; i++) {
                trTableTable.push({});
                trTableTable[i].parentNode = this.treeTableTable[i].parentNode;
                trTableTable[i].goldChildRows = this.treeTableTable[i].goldChildRows;
                trTableTable[i].expanded = this.treeTableTable[i].expanded;
            }
            /* refresh view */
            var selData = this.getSelectedData('TREE_TABLE', this.treeTableTable.childRowsWithAllCols);
            if (this.viewsData[this.moduleName + '.' + this.viewName].orderedColumns) {
                this.updateColumnsData(trTableTable, selData.cols);
            }
            trTableTable.columns = selData.cols;
            for (var i = 0; i < selData.rows.length; i++) {
                trTableTable[i].childRows = selData.rows[i];
            }
            this.treeTableTable = trTableTable;
            this.$.rovTreeTableTemplate.render();
        }
    },
    columnsContextMenu : function(e, container, containerLeft) {
        var dialog = this.$.columnsSelector;
        dialog.resetFit();
        this.set('columnStates', this.viewsData[this.moduleName + '.' + this.viewName].columnStates);
        var maxStr = '';
        var numHasFormats = 0;
        for (var i = 0; i < this.columnStates.length; i++) {
            if (this.columnStates[i].name.length > maxStr.length) {
                maxStr = this.columnStates[i].name;
            }
            if (this.columnStates[i].hasFormat) {
                ++numHasFormats;
            }
        }
        /* for margins, checkbox and droplists */
        var width = rovUtils.getStringWidth(maxStr) + rovUtils.getStringWidth('Scientific') + 65;
        dialog.style.width = width + 'px';
        var left = e.pageX;
        if (left + width > document.documentElement.clientWidth) {
            left -= width;
            var leftMin = containerLeft;
            if (left < leftMin) {
                left = leftMin;
            }
        }
        dialog.style.left = left + 'px';                       /* + 43 for close button & column titles*/
        var height = ((this.columnStates.length + 1) * 25) + (numHasFormats * 7) + 43;
        if (height > container.clientHeight) {
            height = container.clientHeight;
        }
        var top = e.y + (36 - e.offsetY) + 23; /* make table column headings visible */
        if (top + height > document.documentElement.clientHeight) {
          top = e.y - height;
          if (top < 0) {
              top = 0;
          }
        }
        dialog.style.top = top + 'px';
        dialog.style.height = height + 'px';
        dialog.style.maxHeight = (height * 1.25) + 'px';
        dialog.open();
    },
    columnSort : function(colIndex, toggle) {
        function numCompare(a, b) {
            return (Number(a) - Number(b));
        }
        function reverseNumCompare(a, b) {
            return (Number(b) - Number(a));
        }
        var viewData = this.viewsData[this.moduleName + '.' + this.viewName];
        if (this.isInstanceView) {
            var newTable = {};
            var prevTable = this.instanceTable;
            /* preserve old */
            newTable.allCols = prevTable.allCols;
            newTable.fullRows = prevTable.fullRows;
            newTable.columns = prevTable.columns;
            var rows = prevTable.rows;
            var sortColName = newTable.columns[colIndex];
            var colVals = [];
            for (var i = 0; i < rows.length; i++) {
                colVals.push(rows[i][colIndex].val);
            }
            var origVals = colVals.join(',');

            var sortDir;
            var nan = isNaN(colVals[0]) || colVals[0] == '';
            if (toggle ) {
                if (viewData.sortCol == sortColName) {
                    sortDir = (viewData.sortDir == 'down') ? 'up' : 'down';
                    if (sortDir == 'down') {
                        nan ? colVals.sort() : colVals.sort(numCompare);
                    }
                    else {
                        nan ? colVals.reverse() : colVals.sort(reverseNumCompare);
                    }
                }
                else {                /* new sort col */
                    nan ? colVals.sort() : colVals.sort(numCompare);
                    sortDir = 'down';
                    if (origVals == colVals.join(',')) {
                        nan ? colVals.reverse() : colVals.sort(reverseNumCompare);
                        sortDir = 'up';
                    }
                }
            }
            else if (viewData.sortCol) {  /* it's a restore/refresh, use saved direction */
                sortDir = viewData.sortDir;
                nan ? colVals.sort() : colVals.sort(numCompare);
                if (sortDir == 'up') {
                    nan ? colVals.reverse() : colVals.sort(reverseNumCompare);
                }
            }
            else {    /* defensive */
                nan ? colVals.sort() : colVals.sort(numCompare);
                sortDir = 'down';
                if (origVals == colVals.join(',')) {
                    nan ? colVals.reverse() : colVals.sort(reverseNumCompare);
                    sortDir = 'up';
                }
            }

            var sortRows = [];
            var pushed = [];
            for (i = 0; i < colVals.length; i++) {
                for (var j = 0; j < rows.length; j++) {
                    if (rows[j][colIndex].val == colVals[i] && pushed.indexOf(j) == -1) {
                        sortRows.push(rows[j]);
                        pushed.push(j);
                        break;
                    }
                }
            }
            /* Update persistant storage */
            viewData.sortCol = newTable.columns[colIndex];
            viewData.sortDir =  sortDir;
            if (this.isGlobal) {
                this.saveViewData();
            }

            /* refresh view */
            newTable.rows = sortRows;
            newTable.displayColumns = [];
            for (var i = 0; i < newTable.columns.length; i++) {
                var colObj = {};
                colObj.name = newTable.columns[i];
                if (colObj.name == viewData.sortCol) {
                    colObj.isSortCol = true;
                    colObj.icon = (sortDir == 'down') ? 'arrow-drop-down' : 'arrow-drop-up';
                }
                else {
                    colObj.isSortCol = false;
                }
                newTable.displayColumns.push(colObj);
            }
            this.instanceTable = newTable;
        }
        else if (this.isInstanceDataView) {
            var instDataTable = {};
            this.copyInstanceDataTableProps(instDataTable);

            var sortColName = instDataTable.columns[colIndex];
            var rows = this.instanceDataTable.rows;
            var sortRows = [];
            var sortDir;
            for (var i = 0; i < rows.length; i++) {
                sortRows.push({});
                sortRows[i].expanded = rows[i].expanded;
                if (instDataTable.showInstSelect) {
                    sortRows[i].label = rows[i].label;
                }
                var colVals = [];
                for (var j = 0; j < rows[i].rowVals.length; j++) {
                    colVals.push(rows[i].rowVals[j][colIndex].val);
                }
                var nan = isNaN(colVals[0]) || colVals[0] == '';
                if (i == 0) {
                    var origVals = colVals.join(',');
                    if (toggle ) {
                        if (viewData.sortCol == sortColName) {
                            sortDir = (viewData.sortDir == 'down') ? 'up' : 'down';
                            if (sortDir == 'down') {
                                nan ? colVals.sort() : colVals.sort(numCompare);
                            }
                            else {
                                nan ? colVals.reverse() : colVals.sort(reverseNumCompare);
                            }
                        }
                        else {                /* new sort col */
                            nan ? colVals.sort() : colVals.sort(numCompare);
                            sortDir = 'down';
                            if (origVals == colVals.join(',')) {
                                nan ? colVals.reverse() : colVals.sort(reverseNumCompare);
                                sortDir = 'up';
                            }
                        }
                    }
                    else if (viewData.sortCol) {  /* it's a restore/refresh, use saved direction */
                        sortDir = viewData.sortDir;
                        nan ? colVals.sort() : colVals.sort(numCompare);
                        if (sortDir == 'up') {
                            nan ? colVals.reverse() : colVals.sort(reverseNumCompare);
                        }
                    }
                    else {    /* defensive */
                        nan ? colVals.sort() : colVals.sort(numCompare);
                        sortDir = 'down';
                        if (origVals == colVals.join(',')) {
                            nan ? colVals.reverse() : colVals.sort(reverseNumCompare);
                            sortDir = 'up';
                        }
                    }
                }
                else {
                    nan ? colVals.sort() : colVals.sort(numCompare);
                    if (sortDir == 'up') {
                        nan ? colVals.reverse() : colVals.sort(reverseNumCompare);
                    }
                }
                var sortRowVals = [];
                var pushed = [];
                for (j = 0; j < colVals.length; j++) {
                    for (var k = 0; k < rows[i].rowVals.length; k++) {
                        if (rows[i].rowVals[k][colIndex].val == colVals[j] && pushed.indexOf(k) == -1) {
                            sortRowVals.push(rows[i].rowVals[k]);
                            pushed.push(k);
                            break;
                        }
                    }
                }
                sortRows[i].rowVals = sortRowVals;
            }
            /* Update persistant storage */
            viewData.sortCol = instDataTable.columns[colIndex];
            viewData.sortDir =  sortDir;
            if (this.isGlobal) {
                this.saveViewData();
            }

            /* refresh view */
            instDataTable.rows = sortRows;
            instDataTable.displayColumns = [];
            for (var i = 0; i < instDataTable.columns.length; i++) {
                var colObj = {};
                colObj.name = instDataTable.columns[i];
                if (colObj.name == viewData.sortCol) {
                    colObj.isSortCol = true;
                    colObj.icon = (sortDir == 'down') ? 'arrow-drop-down' : 'arrow-drop-up';
                }
                else {
                    colObj.isSortCol = false;
                }
                instDataTable.displayColumns.push(colObj);
            }
            this.instanceDataTable = instDataTable;
        }
    },
    columnsSanityCheck : function(allCols, noWarning) {
        var key = this.moduleName + '.' + this.viewName;
        var columnStates = this.viewsData[key].columnStates;
        var storedNames = [];

        /* Check for columns that  exist in storage, but not in exe */
        var missingExeCols = [];
        for (var i = 0; i < columnStates.length; i++) {
            if (allCols.indexOf(columnStates[i].name) == -1) {
                missingExeCols.push(columnStates[i].name);
            }
            storedNames.push(columnStates[i].name); /* save names for next check */
        }

        /* Check for columns exist in exe, but not in storage */
        var missingStorageCols = [];
        for (var i = 0; i < allCols.length; i++) {
            if (storedNames.indexOf(allCols[i]) == -1) {
                missingStorageCols.push(allCols[i]);
            }
        }
        if (missingExeCols.length > 0) {
            if (!noWarning) {
                for (var i = 0; i < missingExeCols.length; i++) {
                    console.warn('Warning: Column "' + missingExeCols[i] +
                                 '" is not present in ' + key);
                }
            }
            /* Remove them from storage */
            var newColumnStates = [];
            var prevColumnStates = this.viewsData[key].columnStates;
            for (var i = 0; i < prevColumnStates.length; i++) {
                if (missingExeCols.indexOf(prevColumnStates[i].name) == -1) {
                   newColumnStates.push(rovUtils.shallowCopy(prevColumnStates[i]));
                }
            }
            this.viewsData[key].columnStates = newColumnStates;

        }
        if (missingStorageCols.length > 0) {
            if (!noWarning) {
                for (var i = 0; i < missingStorageCols.length; i++) {
                    console.warn('Warning: Column "' + missingStorageCols[i] +
                                 '" is not present in ' + key + ' saved state');
                }
            }
            /* Add them to storage */
            var newColumnStates = [];
            var prevColumnStates = this.viewsData[key].columnStates;
            for (var i = 0; i < allCols.length; i++) {
                if (missingStorageCols.indexOf(allCols[i]) >= 0) {
                    newColumnStates.push({name:allCols[i], checked:true});
                }
                else {
                    for (var j = 0; j < prevColumnStates.length; j++) {
                        if (allCols[i] == prevColumnStates[j].name) {
                            newColumnStates.push(rovUtils.shallowCopy(prevColumnStates[j]));
                            break;
                        }
                    }
                }
            }
            this.viewsData[key].columnStates = newColumnStates;
        }
        if ((missingExeCols.length > 0 || missingStorageCols.length > 0) && this.isGlobal) {
            this.saveViewData();
        }
        return (missingStorageCols);
    },
    columnsSelectorClosed : function (e) {
        if (e.target.id == 'columnsSelector') {
            this.set('columnStates', []);
        }
    },
    columnsSelectorKeyPress : function (e) {
        var keyCode = e.keyCode || e.which;
        if (keyCode === 13) {
            if (this.$.columnsSelector.opened) {
                this.$.columnsSelector.close();
            }
        }
    },
    columnsSelectorOpened : function (e) {
        this.$.columnsSelector.scrollTop = 0;
    },
    copyInstanceDataTableProps : function (instDataTable) {
        instDataTable.allCols = this.instanceDataTable.allCols;
        instDataTable.fullRows = this.instanceDataTable.fullRows;
        instDataTable.columns = this.instanceDataTable.columns;
        instDataTable.showInstSelect = this.instanceDataTable.showInstSelect;
        instDataTable.colsShowing = this.instanceDataTable.colsShowing;
        instDataTable.currentInst = this.instanceDataTable.currentInst;
        instDataTable.currentInstIdx = this.instanceDataTable.currentInstIdx;
        instDataTable.instNames = rovUtils.shallowCopy(this.instanceDataTable.instNames);
    },
    /*
        TREE_TABLE data structure:

        treeTableTable = [
              { childRows = [
                    { hasIcon:
                          true
                              expanded: true
                              icon: "remove"
                              id: "0_0"
                          false
                              atMax: Boolean
                              cells: Array (only defined if atMax is false)
                      level: Integer (0)
                      name: String ("drivers")
                      style: String ("padding-left: 30px; cursor:pointer;")
                    }
                ]
                expanded:      Boolean
                goldChildRows: Array
                parentNode:    String
              }
            ]
            allCols:              Array
            childRowsWithAllCols: Array    * the current expanded state of the table *
            columns:              Array
     */
    copyTreeTable : function(srcTable) {
        function copyChildRows(srcRows) {
            var childRows = [];
            for (var i = 0; i < srcRows.length; i++) {
                childRows.push({});
                childRows[i].level = srcRows[i].level;
                childRows[i].name = srcRows[i].name;
                childRows[i].style = srcRows[i].style;
                childRows[i].hasIcon = srcRows[i].hasIcon;
                if (childRows[i].hasIcon) {
                    childRows[i].expanded = srcRows[i].expanded;
                    childRows[i].icon = srcRows[i].icon;
                    childRows[i].id = srcRows[i].id;
                    childRows[i].compareKey = srcRows[i].compareKey;
                }
                else {
                    childRows[i].atMax = srcRows[i].atMax;
                    if (!childRows[i].atMax) {
                        childRows[i].compareKey = srcRows[i].compareKey;
                        childRows[i].cells = rovUtils.shallowCopy(srcRows[i].cells);
                    }
                }
            }
            return (childRows);
        }

        var destTable = [];
        destTable.allCols = rovUtils.shallowCopy(srcTable.allCols);
        destTable.columns = rovUtils.shallowCopy(srcTable.columns);

        for (var i = 0; i < srcTable.length; i++) {
            destTable.push({});
            destTable[i].parentNode = srcTable[i].parentNode;
            destTable[i].expanded = srcTable[i].expanded;
            destTable[i].childRows = copyChildRows(srcTable[i].childRows);
            destTable[i].goldChildRows = copyChildRows(srcTable[i].goldChildRows);
        }
        destTable.childRowsWithAllCols = [];
        for (i = 0; i < srcTable.childRowsWithAllCols.length; i++) {
            destTable.childRowsWithAllCols.push(copyChildRows(srcTable.childRowsWithAllCols[i]));
        }
        return (destTable);
    },
    customViewColumnNameChanged : function (name, newName) {
        var allCols = this.customTable.allCols;
        for (var i = 0; i < allCols.length; i++) {
            if (allCols[i] == name) {
                allCols[i] = newName;
                break;
            }
        }
        var columns = this.customTable.columns;
        for (var i = 0; i < columns.length; i++) {
            if (columns[i] == name) {
                columns[i] = newName;
                break;
            }
        }
        var displayColumns = this.customTable.displayColumns;
        for (var i = 0; i < displayColumns.length; i++) {
            if (displayColumns[i].name == name) {
                displayColumns[i].name = newName;
                break;
            }
        }
        var key = this.moduleName + '.' + this.viewName;
        if (this.viewsData[key]) {
            if (this.viewsData[key].columnStates) {
                var columnStates = this.viewsData[key].columnStates;
                for (var i = 0; i < columnStates.length; i++) {
                    if (columnStates[i].name == name) {
                        columnStates[i].name = newName;
                        break;
                    }
                }
            }
            if (this.viewsData[key].orderedColumns) {
                var colsOrder = this.viewsData[key].orderedColumns;
                for (var i = 0; i < colsOrder.length; i++) {
                    if (colsOrder[i] == name) {
                        colsOrder[i] = newName;
                        break;
                    }
                }
            }
        }
    },
    displayTable : function(viewData, viewName, viewType, viewColumns, refresh, keepExpanded, showColumn) {
        this.viewName = viewName;
        this.isInstanceView = false;
        this.isTreeView = false;
        this.isTreeTableView = false;
        this.isInstanceDataView = false;
        this.isRawView = false;
        this.showDataMessage = false;
        var viewIsShowing = false;
        this.colFormatsDirty = false;
        this.highlighted = null;
        if (viewData && typeof(viewData) == 'object') {
            if ((rovUtils.isArray(viewData) && viewData.length == 0) ||
                (rovUtils.isObject(viewData) && Object.keys(viewData).length == 0) ||
                (!rovUtils.isArray(viewData) && viewData.elements && viewData.elements.length == 0)) {

                this.dataMessage = 'No data to display';
                this.showDataMessage = true;
            }
            else if (viewType == 'INSTANCE' || viewType == 'MODULE') {
                var fullRows = [];
                var allCols = rovUtils.shallowCopy(viewColumns);
                var key = this.moduleName + '.' + this.viewName;
                this.viewsData[key] = this.initViewsDataItem(key);
                var fontInfo = this.getFontInfo(this.viewsData, key);
                var fontFamily = fontInfo.fontFamily;
                var colFormats = this.getColFormats(allCols);
                if (rovUtils.isArray(viewData)) {
                    for (var i = 0; i < viewData.length; i++) {
                        fullRows.push([]);
                        for (var j= 0; j < allCols.length; j++) {
                            var propName = allCols[j];
                            var cellObj = {};
                            cellObj.val = viewData[i][propName];
                            if (rovUtils.isArray(cellObj.val) && cellObj.val.length == 1) {
                                cellObj.val = cellObj.val[0];
                            }
                            cellObj.id = propName + '_' + i;
                            cellObj.isLink = false;
                            var linkDesc = this.hasLink(cellObj.val);
                            if (linkDesc != null) {
                                cellObj.isLink = true;
                                cellObj.val = linkDesc[1];
                                cellObj.href = linkDesc[2];
                                cellObj.title = linkDesc[3];
                            }
                            else if (rovUtils.isArray(cellObj.val)) {
                                this.cellArray(cellObj, colFormats, propName, fontFamily);
                            }
                            else {
                                cellObj.val = this.formatVal(cellObj.val, colFormats, propName);
                            }
                            if (viewData[i][propName + '$status']) {
                                cellObj.style = cellObj.isArray ? (cellObj.style + 'background-color:#ff4d4d;') :
                                                this.rovTdStyle + 'background-color:#ff4d4d;' + fontFamily;
                                if (!cellObj.isLink) {
                                    cellObj.title = viewData[i][propName + '$status'];
                                }
                                else {
                                    cellObj.title = viewData[i][propName + '$status'] + '\n' + cellObj.title;
                                }
                                if (cellObj.val == '') {
                                    cellObj.val = 'error';
                                }
                            }
                            else {
                                if (!cellObj.isArray) {
                                    cellObj.style = this.rovTdStyle + fontFamily;
                                }
                                if (!cellObj.isLink) {
                                    cellObj.title = '';
                                }
                            }
                            fullRows[i].push(cellObj);
                            if (j == 0) {
                                fullRows[i].compareKey = cellObj.val;
                            }
                        }
                    }
                }
                else {
                    fullRows.push([]);
                    for (var j = 0; j < allCols.length; j++) {
                        var propName = allCols[j];
                        var cellObj = {};
                        cellObj.val = viewData[propName];
                        if (rovUtils.isArray(cellObj.val) && cellObj.val.length == 1) {
                            cellObj.val = cellObj.val[0];
                        }
                        cellObj.id = propName + '_0';
                        cellObj.isLink = false;
                        var linkDesc = this.hasLink(cellObj.val);
                        if (linkDesc != null) {
                            cellObj.isLink = true;
                            cellObj.val = linkDesc[1];
                            cellObj.href = linkDesc[2];
                            cellObj.title = linkDesc[3];
                        }
                        else if (rovUtils.isArray(cellObj.val)) {
                            this.cellArray(cellObj, colFormats, propName, fontFamily);
                        }
                        else {
                            cellObj.val = this.formatVal(cellObj.val, colFormats, propName);
                        }
                        if (viewData[propName + '$status']) {
                            cellObj.style = cellObj.isArray ? (cellObj.style + 'background-color:#ff4d4d;') :
                                            this.rovTdStyle + 'background-color:#ff4d4d;' + fontFamily;
                            if (!cellObj.isLink) {
                                cellObj.title = viewData[propName + '$status'];
                            }
                            else {
                                cellObj.title = viewData[propName + '$status'] + '\n' + cellObj.title;
                            }
                            if (cellObj.val == '') {
                                cellObj.val = 'error';
                            }
                        }
                        else {
                            if (!cellObj.isArray) {
                                cellObj.style = this.rovTdStyle + fontFamily;
                            }
                            if (!cellObj.isLink) {
                                cellObj.title = '';
                            }
                        }
                        fullRows[0].push(cellObj);
                        if (j == 0) {
                            fullRows[0].compareKey = cellObj.val;
                        }
                    }
                }
                var instTable = {};
                var missingFormats = [];
                if (!this.viewsData[key]) {
                    this.viewsData[key] = {};
                    this.viewsData[key].columnStates = [];
                    for (var i = 0; i < allCols.length; i++) {
                        this.viewsData[key].columnStates.push({name: allCols[i], checked:true});
                    }
                    if (this.isGlobal) {
                        this.saveViewData();
                    }
                    instTable.columns = allCols;
                    instTable.rows = fullRows;
                }
                else {
                    if (!refresh) {
                        missingFormats = this.columnsSanityCheck(allCols);
                    }
                    var selData = this.getSelectedData(viewType, fullRows, showColumn);
                    instTable.columns = selData.cols;
                    instTable.rows = selData.rows;
                }
                if (this.viewsData[key].fixedFont === undefined) {
                    this.viewsData[key].fixedFont = fontInfo.fixedFont;
                }
                if (this.viewsData[key].hasFormats == undefined || missingFormats.length > 0 ||
                    this.colFormatsDirty) {
                    this.initColumnFormats(allCols, fullRows[0]);
                    if (this.colFormatsDirty) {
                        this.colFormatsDirty = false;
                    }
                }

                var sortCol = this.viewsData[key].sortCol ?
                              instTable.columns.indexOf(this.viewsData[key].sortCol) :
                              -1;
                if (refresh) {
                    var prevRows = rovUtils.shallowCopy(this.instanceTable.rows);
                }
                instTable.displayColumns = [];
                for (var i = 0; i < instTable.columns.length; i++) {
                    var colObj = {};
                    colObj.name = instTable.columns[i];
                    colObj.sortCol = false;
                    instTable.displayColumns.push(colObj);
                }
                this.columnStates = this.viewsData[key].columnStates;
                this.instanceTable = instTable;
                this.isInstanceView = true;
                this.instanceTable.allCols = allCols;
                this.instanceTable.fullRows  = fullRows;
                if (sortCol >= 0) {
                    this.columnSort(sortCol);
                }
                if (refresh) {
                    this.rowsCompare(prevRows, this.instanceTable.rows, viewType, fontFamily);
                }
                this.hasFormats = this.viewsData[key].hasFormats;
                this.viewData = viewData;
                this.viewType = viewType;
                viewIsShowing = true;
                this.$.rovInstanceTemplate.render();
            }
            else if (viewType == 'INSTANCE_DATA' || viewType == 'MODULE_DATA') {
                var fullRows = [];
                var instDataTable = {};
                var allCols = rovUtils.shallowCopy(viewColumns);
                var key = this.moduleName + '.' + this.viewName;
                this.viewsData[key] = this.initViewsDataItem(key);
                var fontInfo = this.getFontInfo(this.viewsData, key);
                var fontFamily = fontInfo.fontFamily;
                var colFormats = this.getColFormats(allCols);
                if (rovUtils.isArray(viewData)) {
                    instDataTable.instNames = [];
                    for (var i = 0; i < viewData.length; i++) {
                        var rowVals = [];
                        var elemArr = viewData[i].elements;
                        for (var j = 0; j < elemArr.length; j++) {
                            rowVals.push([]);
                            for (var k = 0; k < allCols.length; k++) {
                                var propName = allCols[k];
                                var rowValObj = {};
                                rowValObj.val = elemArr[j][propName];
                                if (rovUtils.isArray(rowValObj.val) && rowValObj.val.length == 1) {
                                    rowValObj.val = rowValObj.val[0];
                                }
                                rowValObj.id = propName + '_' + i + '_' + j;
                                var linkDesc = this.hasLink(rowValObj.val);
                                if (linkDesc != null) {
                                    rowValObj.isLink = true;
                                    rowValObj.val = linkDesc[1];
                                    rowValObj.href = linkDesc[2];
                                    rowValObj.title = linkDesc[3];
                                }
                                else if (rovUtils.isArray(rowValObj.val)) {
                                    this.cellArray(rowValObj, colFormats, propName, fontFamily);
                                }
                                else {
                                    rowValObj.val = this.formatVal(rowValObj.val, colFormats, propName);
                                }
                                if (elemArr[j][propName + '$status']) {
                                    rowValObj.style = rowValObj.isArray ? (rowValObj.style + 'background-color:#ff4d4d;') :
                                                      this.rovTdStyle + 'background-color:#ff4d4d;' + fontFamily;
                                    if (!rowValObj.isLink) {
                                        rowValObj.title = elemArr[j][propName + '$status'];
                                    }
                                    else {
                                        rowValObj.title = elemArr[j][propName + '$status'] + '\n' + rowValObj.title;
                                    }
                                    if (rowValObj.val == '') {
                                        rowValObj.val = 'error';
                                    }
                                }
                                else {
                                    if (!rowValObj.isArray) {
                                        rowValObj.style = this.rovTdStyle + fontFamily;
                                    }
                                    if (!rowValObj.isLink) {
                                        rowValObj.title = '';
                                    }
                                }
                                if (k == 0) {
                                    rowVals[j].compareKey = rowValObj.val;
                                }
                                rowVals[j].push(rowValObj);
                            }
                        }
                        fullRows.push({});
                        fullRows[i].expanded = (i == 0);
                        fullRows[i].label = viewData[i].label;
                        fullRows[i].rowVals = rowVals;
                        instDataTable.instNames.push(fullRows[i].label);
                    }
                    instDataTable.currentInst = fullRows[0].label;
                    instDataTable.showInstSelect = true;
                }
                else {
                    var rowVals = [];
                    var elemArr = viewData.elements;
                    for (var i = 0; i < elemArr.length; i++) {
                        rowVals.push([]);
                        for (var j = 0; j < allCols.length; j++) {
                            var propName = allCols[j];
                            var rowValObj = {};
                            rowValObj.val = elemArr[i][propName];
                            if (rovUtils.isArray(rowValObj.val) && rowValObj.val.length == 1) {
                                rowValObj.val = rowValObj.val[0];
                            }
                            rowValObj.id = propName + '_' + i;
                            var linkDesc = this.hasLink(rowValObj.val);
                            if (linkDesc != null) {
                                rowValObj.isLink = true;
                                rowValObj.val = linkDesc[1];
                                rowValObj.href = linkDesc[2];
                                rowValObj.title = linkDesc[3];
                            }
                            else if (rovUtils.isArray(rowValObj.val)) {
                                this.cellArray(rowValObj, colFormats, propName, fontFamily);
                            }
                            else {
                                rowValObj.val = this.formatVal(rowValObj.val, colFormats, propName);
                            }
                            if (elemArr[i][propName + '$status']) {
                                var styleStr = 'background-color:#ff4d4d';
                                rowValObj.style = rowValObj.isArray ? (rowValObj.style + 'background-color:#ff4d4d;') :
                                                  this.rovTdStyle + 'display:table-cell;background-color:#ff4d4d;' + fontFamily;
                                if (!rowValObj.isLink) {
                                    rowValObj.title = elemArr[i][propName + '$status'];
                                }
                                else {
                                    rowValObj.title = elemArr[i][propName + '$status'] + '\n' + rowValObj.title;
                                }
                                if (rowValObj.val == '') {
                                    rowValObj.val = 'error';
                                }
                            }
                            else {
                                if (!rowValObj.isArray) {
                                    rowValObj.style = this.rovTdStyle + fontFamily;
                                }
                                if (!rowValObj.isLink) {
                                    rowValObj.title = '';
                                }
                            }
                            if (j == 0) {
                                rowVals[i].compareKey = rowValObj.val;
                            }
                            rowVals[i].push(rowValObj);
                        }
                    }
                    fullRows.push({});
                    fullRows[0].rowVals = rowVals;
                    fullRows[0].expanded = true;
                    instDataTable.showInstSelect = false;
                }
                var missingFormats = [];
                if (!this.viewsData[key]) {
                    this.viewsData[key] = {};
                    this.viewsData[key].columnStates = [];
                    for (var i = 0; i < allCols.length; i++) {
                        this.viewsData[key].columnStates.push({name: allCols[i], checked:true});
                    }
                    if (this.isGlobal) {
                        this.saveViewData();
                    }
                    instDataTable.columns = allCols;
                    instDataTable.rows = fullRows;
                    instDataTable.colsShowing = allCols.length > 1 ? allCols.length - 1 : 0;
                }
                else {
                    if (!refresh) {
                        missingFormats = this.columnsSanityCheck(allCols);
                    }
                    if (instDataTable.showInstSelect && this.viewsData[key].currentInst) {
                         /* Expand the instance this.viewsData[key].currentInst only if exists in current rows */
                        var found = false;
                        for (var i = 0; i < fullRows.length; i++) {
                            if (fullRows[i].label == this.viewsData[key].currentInst) {
                                found = true;
                                break;
                            }
                        }
                        /* If instance is an address, label may not match, use index instead */
                        if (!found && this.viewsData[key].currentInstIdx >= 0 &&
                            this.viewsData[key].currentInstIdx < fullRows.length) {

                            this.viewsData[key].currentInst = fullRows[this.viewsData[key].currentInstIdx].label;
                            found = true;
                        }
                        if (found) {
                            for (var i = 0; i < fullRows.length; i++) {
                                fullRows[i].expanded = (fullRows[i].label == this.viewsData[key].currentInst);
                            }
                            instDataTable.currentInst = this.viewsData[key].currentInst;
                        }
                    }
                    var selData = this.getSelectedData(viewType, fullRows, showColumn);
                    instDataTable.columns = selData.cols;
                    instDataTable.rows = selData.rows;
                    instDataTable.colsShowing = instDataTable.columns.length > 1 ? instDataTable.columns.length - 1 : 0;
                }
                if (this.viewsData[key].fixedFont === undefined) {
                    this.viewsData[key].fixedFont = fontInfo.fixedFont;
                }
                if (this.viewsData[key].hasFormats == undefined || missingFormats.length > 0 ||
                    this.colFormatsDirty) {
                    for (var i = 0; i < fullRows.length; i++) {
                        if (fullRows[i].rowVals.length > 0) {
                            this.initColumnFormats(allCols, fullRows[i].rowVals[0]);
                            if (this.colFormatsDirty) {
                                this.colFormatsDirty = false;
                            }
                            break;
                        }
                    }
                }

                var sortCol = this.viewsData[key].sortCol ?
                              instDataTable.columns.indexOf(this.viewsData[key].sortCol) :
                              -1;
                if (refresh) {
                    var prevRows = rovUtils.shallowCopy(this.instanceDataTable.rows);
                }
                this.columnStates = this.viewsData[key].columnStates;
                instDataTable.displayColumns = [];
                for (var i = 0; i < instDataTable.columns.length; i++) {
                    var colObj = {};
                    colObj.name = instDataTable.columns[i];
                    colObj.sortCol = false;
                    instDataTable.displayColumns.push(colObj);
                }
                this.instanceDataTable = instDataTable;
                this.isInstanceDataView = true;
                this.instanceDataTable.allCols = allCols;
                this.instanceDataTable.fullRows  = fullRows;
                if (sortCol >= 0) {
                    this.columnSort(sortCol);
                }
                if (refresh) {
                    this.rowsCompare(prevRows, this.instanceDataTable.rows, viewType, fontFamily);
                }
                this.hasFormats = this.viewsData[key].hasFormats;
                this.viewData = viewData;
                this.viewType = viewType;
                viewIsShowing = true;
                this.$.rovInstanceDataTemplate.render();
            }
            else if (viewType == 'TREE') {
                var key = this.moduleName + '.' + this.viewName;
                this.viewsData[key] = this.initViewsDataItem(key);
                var fontInfo = this.getFontInfo(this.viewsData, key);
                var fontFamily = fontInfo.fontFamily;
                if (!this.viewsData[key]) {
                    this.viewsData[key] = {};
                }
                if (this.viewsData[key].fixedFont === undefined) {
                    this.viewsData[key].fixedFont = fontInfo.fixedFont;
                }
                var parentNodes = Object.keys(viewData);
                var trTable = [];
                for (var i = 0; i < parentNodes.length; i++) {
                    var childRows = [];
                    if (rovUtils.isArray(viewData[parentNodes[i]])) {
                        for (var j = 0; j < viewData[parentNodes[i]].length; j+=2) {
                            var childRow = [];
                            var rowCell_0 = {style:fontFamily};
                            rowCell_0.val = viewData[parentNodes[i]][j];
                            childRow.push(rowCell_0);
                            var rowCell_1 = {style:fontFamily};
                            if (rovUtils.isObjectVal(viewData[parentNodes[i]][j + 1])) {
                                rowCell_1.val = rovUtils.getObjectVal(viewData[parentNodes[i]][j + 1]);
                            }
                            else {
                                rowCell_1.val = viewData[parentNodes[i]][j + 1];
                            }
                            childRow.push(rowCell_1);
                            childRows.push(childRow);
                        }
                    }
                    else {                                               /* Object */
                        for (var prop in viewData[parentNodes[i]]) {
                            var childRow = [];
                            var rowCell_0 = {style:fontFamily};
                            rowCell_0.val = prop;
                            childRow.push(rowCell_0);
                            var rowCell_1 = {style:fontFamily};
                            if (rovUtils.isObjectVal(viewData[parentNodes[i]][prop])) {
                                rowCell_1.val = rovUtils.getObjectVal(viewData[parentNodes[i]][prop]);
                            }
                            else {
                                rowCell_1.val = viewData[parentNodes[i]][prop];
                            }
                            childRow.push(rowCell_1);
                            childRows.push(childRow);
                        }
                    }
                    trTable.push({});
                    trTable[i].parentNode = parentNodes[i];
                    trTable[i].childRows = childRows;
                    trTable[i].style = fontFamily;
                    if (this.treeTable.length <= i) {  /* 1st time making table */
                        trTable[i].expanded = true;
                    }
                    else {
                        trTable[i].expanded = this.treeTable[i].expanded;   /* save previous state, this is garbage if number of nodes changes */
                    }
                }
                trTable.columns = ['Property', 'Value'];
                this.treeTable = trTable;
                this.isTreeView = true;
                this.viewData = viewData;
                this.viewType = viewType;
                viewIsShowing = true;
                this.$.rovTreeTemplate.render();
            }
            else if (viewType == 'TREE_TABLE') {
                function createTreeTableRows(srcRows, destRows) {
                    var i = 0;
                    while (i < srcRows.length) {
                        if (srcRows[i].hasIcon && !srcRows[i].expanded) {
                            destRows.push(srcRows[i]);
                            /* skip all deeper levels */
                            var level = srcRows[i++].level;
                            while (i < srcRows.length && srcRows[i].level > level) {
                                ++i;
                            }
                        }
                        else {
                            destRows.push(srcRows[i++]);
                        }
                    }
                }
                var trTableTable = [];
                var treeTableIndex = 0;
                var childRowsWithAllCols = [];
                var allCols = rovUtils.shallowCopy(viewColumns);
                var key = this.moduleName + '.' + this.viewName;
                this.viewsData[key] = this.initViewsDataItem(key);
                var fontInfo = this.getFontInfo(this.viewsData, key);
                var fontFamily = fontInfo.fontFamily;
                var colFormats = this.getColFormats(allCols);
                for (var i = 0; i < viewData.length; i++) {
                    var goldChildRows = [];
                    this.makeTreeTableRows(viewData[i], null, goldChildRows, i, 0, allCols, colFormats, fontFamily);
                    trTableTable.push({});
                    trTableTable[i].parentNode = viewData[i].label;
                    trTableTable[i].expanded = true;
                    if (this.expandAll) {
                        trTableTable[i].childRows = goldChildRows;
                    }
                    else {
                        var childRows = [];
                        if (refresh || keepExpanded) { /* retain expanded state on refresh */
                            var tableElem = this.getParentNodeElem(this.treeTableTable, trTableTable[i].parentNode);
                            if (tableElem) {
                                trTableTable[i].expanded = tableElem.expanded;
                                for (var j = 0; j < goldChildRows.length; j++) {
                                    if (goldChildRows[j].hasIcon) {
                                        var prevNode = this.getPreviousTreeTableNode(goldChildRows[j].compareKey, tableElem);
                                        if (prevNode) {
                                            goldChildRows[j].expanded = prevNode.expanded;
                                            goldChildRows[j].icon = prevNode.icon;
                                        }
                                    }
                                }
                            }
                        }
                        createTreeTableRows(goldChildRows, childRows);
                        trTableTable[i].childRows = childRows;
                    }
                    childRowsWithAllCols.push(trTableTable[i].childRows);
                    trTableTable[i].goldChildRows = goldChildRows;
                }
                var missingFormats = [];
                if (!this.viewsData[key]) {
                    this.viewsData[key] = {};
                    this.viewsData[key].columnStates = [];
                    for (var i = 0; i < allCols.length; i++) {
                        this.viewsData[key].columnStates.push({name: allCols[i], checked:true});
                    }
                    if (this.isGlobal) {
                        this.saveViewData();
                    }
                    trTableTable.columns = allCols;
                }
                else {
                    if (!refresh) {
                        missingFormats = this.columnsSanityCheck(allCols);
                    }
                    var selData = this.getSelectedData(viewType, childRowsWithAllCols);
                    trTableTable.columns = selData.cols;
                    for (var i = 0; i < selData.rows.length; i++) {
                        trTableTable[i].childRows = selData.rows[i];
                    }
                }
                if (this.viewsData[key].fixedFont === undefined) {
                    this.viewsData[key].fixedFont = fontInfo.fixedFont;
                }
                if (this.viewsData[key].hasFormats == undefined || missingFormats.length > 0 ||
                    this.colFormatsDirty) {
                    /* get first available row with values */
                    var cells = null;
                    for (var i = 0; i < trTableTable.length; i++) {
                        var childRows = childRowsWithAllCols[i];
                        for (var j = 0; j < childRows.length; j++) {
                            if (!childRows[j].hasIcon && !childRows[j].atMax) {
                                cells = childRows[j].cells;
                                break;
                            }
                        }
                        if (cells != null) {
                            break;
                        }
                    }
                    if (cells) {
                        this.initColumnFormats(allCols, cells);
                        if (this.colFormatsDirty) {
                            this.colFormatsDirty = false;
                        }
                    }
                }
                if (refresh) {
                    this.rowsCompare(this.treeTableTable, trTableTable, viewType, fontFamily);
                }
                this.columnStates = this.viewsData[key].columnStates;
                this.treeTableTable = trTableTable;
                this.treeTableTable.allCols = allCols;
                this.treeTableTable.childRowsWithAllCols = childRowsWithAllCols;
                this.isTreeTableView = true;
                this.hasFormats = this.viewsData[key].hasFormats;
                this.viewData = viewData;
                this.viewType = viewType;
                viewIsShowing = true;
                this.$.rovTreeTableTemplate.render();
            }
            else if (viewType == 'RAW') {
                function createRawRows(srcRows, destRows) {
                    var i = 0;
                    while (i < srcRows.length) {
                        if (srcRows[i].hasIcon && !srcRows[i].expanded) {
                            destRows.push(srcRows[i]);
                            /* skip all deeper levels */
                            var level = srcRows[i++].level;
                            while (i < srcRows.length && srcRows[i].level > level) {
                                ++i;
                            }
                        }
                        else {
                            destRows.push(srcRows[i++]);
                        }
                    }
                }
                var key = this.moduleName + '.' + this.viewName;
                this.viewsData[key] = this.initViewsDataItem(key);
                var fontInfo = this.getFontInfo(this.viewsData, key);
                var fontFamily = fontInfo.fontFamily;
                if (!this.viewsData[key]) {
                    this.viewsData[key] = {};
                    if (this.isGlobal) {
                        this.saveViewData();
                    }
                }
                if (this.viewsData[key].fixedFont === undefined) {
                    this.viewsData[key].fixedFont = fontInfo.fixedFont;
                }
                var parentNodes = ['Module State', 'Instance States' , 'Module Configuration Parameters'];
                var rawTable = [];
                var rawIndex = 0;

                /* module state */
                var modState = viewData[parentNodes[0]];
                if (modState && Object.keys(modState).length > 0) {
                    var goldChildRows = [];
                    for (var prop in modState) {
                        this.makeRawRow(modState, prop, 'modState.' + prop, goldChildRows, rawIndex, 0, fontFamily);
                    }
                    rawTable.push({});
                    rawTable[rawIndex].parentNode = parentNodes[0];
                    rawTable[rawIndex].style = fontFamily;
                    rawTable[rawIndex].expanded = true;
                    if (this.expandAll) {
                        rawTable[rawIndex].childRows = goldChildRows;
                    }
                    else {
                        var childRows = [];
                        if (refresh || keepExpanded) { /* retain expanded state on refresh */
                            var tableElem = this.getParentNodeElem(this.rawTable, parentNodes[0]);
                            if (tableElem) {
                                rawTable[rawIndex].expanded = tableElem.expanded;
                                this.refreshRawExpandedState(tableElem.goldChildRows, goldChildRows);
                            }
                        }
                        createRawRows(goldChildRows, childRows);
                        rawTable[rawIndex].childRows = childRows;
                    }
                    rawTable[rawIndex].goldChildRows = goldChildRows;
                    ++rawIndex;
                }

                /* instance states */
                var instStates = viewData[parentNodes[1]];
                if (instStates && instStates.length > 0) {
                    var goldChildRows = [];
                    for (var i = 0; i < instStates.length; i++) {
                        var childRow = {};
                        childRow.property = instStates[i].$label.substr(instStates[i].$label.indexOf(':') + 1);
                        childRow.level = 0;
                        childRow.hasIcon = true;
                        childRow.style = this.rawRowStyle(0, true) + fontFamily;
                        childRow.expanded = true;
                        childRow.icon = 'remove';
                        childRow.id = rawIndex + '_' + goldChildRows.length;
                        childRow.value = '';
                        goldChildRows.push(childRow);
                        for (var prop in instStates[i]) {
                            var qual = 'instState.' + childRow.property + '.' + prop;
                            this.makeRawRow(instStates[i], prop, qual, goldChildRows, rawIndex, 1, fontFamily);
                        }
                    }
                    rawTable.push({});
                    rawTable[rawIndex].parentNode = parentNodes[1];
                    rawTable[rawIndex].style = fontFamily;
                    rawTable[rawIndex].expanded = true;
                    if (this.expandAll) {
                        rawTable[rawIndex].childRows = goldChildRows;
                    }
                    else {
                        var childRows = [];
                        if (refresh || keepExpanded) { /* retain expanded state on refresh */
                            var tableElem = this.getParentNodeElem(this.rawTable, parentNodes[1]);
                            if (tableElem) {
                                rawTable[rawIndex].expanded = tableElem.expanded;
                                this.refreshRawExpandedState(tableElem.goldChildRows, goldChildRows);
                            }
                        }
                        createRawRows(goldChildRows, childRows);
                        rawTable[rawIndex].childRows = childRows;
                    }
                    rawTable[rawIndex].goldChildRows = goldChildRows;
                    ++rawIndex;
                }

                /* module config */
                var modCfg = viewData[parentNodes[2]];
                if (modCfg && Object.keys(modCfg).length > 0) {
                    var goldChildRows = [];
                    for (var prop in modCfg) {
                        this.makeRawRow(modCfg, prop, 'modCfg.' + prop, goldChildRows, rawIndex, 0, fontFamily);
                    }
                    rawTable.push({});
                    rawTable[rawIndex].parentNode = parentNodes[2];
                    rawTable[rawIndex].style = fontFamily;
                    rawTable[rawIndex].expanded = true;
                    if (this.expandAll) {
                        rawTable[rawIndex].childRows = goldChildRows;
                    }
                    else {
                        var childRows = [];
                        if (refresh || keepExpanded) { /* retain expanded state on refresh */
                            var tableElem = this.getParentNodeElem(this.rawTable, parentNodes[2]);
                            if (tableElem) {
                                rawTable[rawIndex].expanded = tableElem.expanded;
                                this.refreshRawExpandedState(tableElem.goldChildRows, goldChildRows);
                            }
                        }
                        createRawRows(goldChildRows, childRows);
                        rawTable[rawIndex].childRows = childRows;
                    }
                    rawTable[rawIndex].goldChildRows = goldChildRows;
                }
                rawTable.columns = ['Property', 'Value'];
                if (refresh) {
                    this.rowsCompare(this.rawTable, rawTable, viewType, fontFamily);
                }
                this.rawTable = rawTable;
                this.isRawView = true;
                this.viewData = viewData;
                this.viewType = viewType;
                viewIsShowing = true;
                this.$.rovRawTemplate.render();
            }
            else if (viewType == 'CUSTOM') {
                var sortColIndex = -1;
                var fullRows = [];
                var allCols = rovUtils.shallowCopy(viewColumns);
                var key = this.moduleName + '.' + this.viewName;
                this.viewsData[key] = this.initViewsDataItem(key);
                var fontInfo = this.getFontInfo(this.viewsData, key);
                var fontFamily = fontInfo.fontFamily;
                var colFormats = this.getColFormats(allCols);

                for (var i = 0; i < viewData.length; i++) {
                    fullRows.push([]);
                    for (var j = 0; j < allCols.length; j++) {
                        var propName = allCols[j];
                        var cellObj = {};
                        cellObj.val = viewData[i][propName].val;
                        cellObj.isCustom = viewData[i][propName].isCustom;
                        cellObj.id = propName + '_' + viewData[i][propName].cellIndex;
                        cellObj.val = this.formatVal(cellObj.val, colFormats, propName);
                        if (cellObj.isCustom) {
                            cellObj.displayColumnName = propName;
                        }
                        else {
                            cellObj.primaryKeyColumnValue = viewData[i][propName].primaryKeyColumnValue;
                            cellObj.selectedColumnName = viewData[i][propName].selectedColumnName;
                        }
                        if (viewData[i][propName].status) {
                            cellObj.style = this.rovTdStyle + 'background-color:#ff4d4d;' + fontFamily;
                            cellObj.title = viewData[i][propName].status;
                            if (cellObj.val == '') {
                                cellObj.val = 'error';
                            }
                        }
                        else {
                            cellObj.style = this.rovTdStyle + fontFamily;
                            cellObj.title = '';
                        }
                        fullRows[i].push(cellObj);
                    }
                }
                var custTable = {};
                var missingFormats = [];
                if (!this.viewsData[key]) {
                    this.viewsData[key] = {};
                    this.viewsData[key].columnStates = [];
                    for (var i = 0; i < allCols.length; i++) {
                        this.viewsData[key].columnStates.push({name: allCols[i], checked:true});
                    }
                    custTable.columns = allCols;
                    custTable.rows = fullRows;
                }
                else {
                    missingFormats = this.columnsSanityCheck(allCols, true);
                    var selData = this.getSelectedData(viewType, fullRows);
                    custTable.columns = selData.cols;
                    custTable.rows = selData.rows;
                }
                if (this.viewsData[key].fixedFont === undefined) {
                    this.viewsData[key].fixedFont = fontInfo.fixedFont;
                }
                if (this.viewsData[key].hasFormats == undefined) {
                    this.initColumnFormats(allCols, fullRows[0]);
                }
                else if (missingFormats.length > 0) {
                    this.initMissingFormats(missingFormats, allCols, fullRows[0]);
                }
                if (this.viewsData[key].sortCol && custTable.rows.length > 1
                    && custTable.columns.indexOf(this.viewsData[key].sortCol) >= 0) {
                    sortColIndex = custTable.columns.indexOf(this.viewsData[key].sortCol);
                }
                if (refresh) {
                    var prevRows = rovUtils.shallowCopy(this.customTable.rows);
                }
                custTable.displayColumns = [];
                for (var i = 0; i < custTable.columns.length; i++) {
                    var colObj = {};
                    colObj.name = custTable.columns[i];
                    colObj.sortCol = false;
                    colObj.title = viewData[0][colObj.name].tooltip;
                    custTable.displayColumns.push(colObj);
                }
                this.columnStates = this.viewsData[key].columnStates;
                this.customTable = custTable;
                this.isCustomView = true;
                this.customTable.allCols = allCols;
                this.customTable.fullRows  = fullRows;
                if (sortColIndex >= 0) {
                    this.columnSort(sortColIndex);
                }
                else {
                    this.viewsData[key].sortCol = undefined;
                }
                if (refresh) {
                    this.rowsCompare(prevRows, this.customTable.rows, viewType, fontFamily);
                }
                this.hasFormats = this.viewsData[key].hasFormats;
                this.viewData = viewData;
                this.viewType = viewType;
                viewIsShowing = true;
                this.$.rovCustomTemplate.render();
            }
        }
        else if (viewData && typeof(viewData) == 'string') {
            this.dataMessage = viewData;
            this.showDataMessage = true;
        }
        return (viewIsShowing);
    },
    findRow : function (rowKey) {
        var row = -1;
        if (this.isInstanceView) {
            if (rovUtils.isArray(this.viewData)) {
                var colIndex = -1
                var columns = this.instanceTable.columns;
                for (var i = 0; i < columns.length; i++) {
                    if (columns[i] == rowKey.columnName) {
                        colIndex = i;
                        break;
                    }
                }
                var rows = this.instanceTable.fullRows;
                for (var i = 0; i < rows.length; i++) {
                    if (rows[i][colIndex].val == rowKey.columnValue) {
                        row = i;
                        break;
                    }
                }
            }
            else {
                row = 0;
            }
        }
        else if (this.isInstanceDataView) {
            var colIndex = -1
            var columns = this.instanceDataTable.columns;
            for (var i = 0; i < columns.length; i++) {
                if (columns[i] == rowKey.columnName) {
                    colIndex = i;
                    break;
                }
            }
            var rowVals = this.instanceDataTable.fullRows[0].rowVals;
            for (var i = 0; i < rowVals.length; i++) {
                if (rowVals[i][colIndex].val == rowKey.columnValue) {
                    row = i;
                    break;
                }
            }
        }
        return (row);
    },
    fontChanged :  function (e) {
        this.viewsData[this.moduleName + '.' + this.viewName].fixedFont = e.target.checked;
        if (this.isGlobal) {
            this.saveViewData();
        }
        var allCols;
        if (this.viewType == 'INSTANCE' || this.viewType == 'MODULE') {
            allCols = this.instanceTable.allCols;
        }
        else if (this.viewType == 'INSTANCE_DATA' || this.viewType == 'MODULE_DATA') {
            allCols = this.instanceDataTable.allCols;
        }
        else if (this.viewType == 'TREE_TABLE') {
            allCols = this.treeTableTable.allCols;
        }
        else if (this.viewType == 'CUSTOM') {
            allCols = this.customTable.allCols;
        }
        this.displayTable(this.viewData, this.viewName, this.viewType, allCols, false, true);
    },
    formatVal : function (val, colFormats, colName) {
        if (val == null) {
            return ('');
        }
        if (!colFormats[colName]) {
            if (rovUtils.isAnROVNumber(val)) {
                this.colFormatsDirty = true;
            }
            return (val);
        }
        var valFormat = rovUtils.getFormat(val);
        if (!valFormat) {
            this.colFormatsDirty = true;
        }
        else if(valFormat != colFormats[colName]) {
            switch (colFormats[colName]) {
                case 'Decimal':
                    if (valFormat == 'Hex') {
                        return (parseInt(val, 16));
                    }
                    if (valFormat == 'Scientific') {
                        return (Number(val));
                    }
                case 'Hex':
                    var retVal = Number(val).toString(16);
                    if (retVal == 'NaN') {
                        this.colFormatsDirty = true;
                        break;
                    }
                    if (retVal.search(/^0x/i) == -1 && retVal.search(/^-0x/i) == -1) {
                        retVal = (Number(val) < 0) ? '-0x' + retVal.slice(1) : '0x' + retVal;
                    }
                    return (retVal);
                case 'Scientific':
                    return (Number(val).toExponential());
            }
        }
        return (val);
    },
    getColFormats : function (allCols) {
        var formats = [];
        var key = this.moduleName + '.' + this.viewName;
        if (this.viewsData[key] && this.viewsData[key].columnStates) {
            var columnStates = this.viewsData[key].columnStates;
            if (this.viewType == 'CUSTOM') {
                for (var i = 0; i < allCols.length; i++) {
                    for (var j = 0; j < columnStates.length; j++) {
                        if (columnStates[j].hasFormat && columnStates[j].name == allCols[i]) {
                            formats[columnStates[j].name] = columnStates[j].format;
                        }
                    }
                }
            }
            else {
                for (var i = 0; i < columnStates.length; i++) {
                    if (columnStates[i].hasFormat) {
                        formats[columnStates[i].name] = columnStates[i].format;
                    }
                }
            }
        }
        return (formats);
    },
    getColumnIndex : function(colName) {
        colName = colName.trim();
        if (this.isInstanceView) {
            return (this.instanceTable.columns.indexOf(colName));
        }
        else if (this.isInstanceDataView) {
            return (this.instanceDataTable.columns.indexOf(colName));
        }
        else if (this.isTreeTableView) {
            return (this.treeTableTable.columns.indexOf(colName));
        }
        else if (this.isCustomView) {
            return (this.customTable.columns.indexOf(colName));
        }
        return (0);
    },
    getFontInfo : function (viewsData, key) {
        if (viewsData[key] && viewsData[key].fixedFont != undefined) {
            var fixedFont = viewsData[key].fixedFont;
        }
        else {
            var globalSettings = rovUtils.getGlobalSettings();
            var fixedFont = globalSettings.globalFixedFont != undefined ? globalSettings.globalFixedFont : false;
        }
        var fontFamily = 'font-family:';
        fontFamily += fixedFont ? tiRovStyles.rovTableFixedFontFamily + ';'
                                : tiRovStyles.rovTableFontFamily + ';';
        return ({fontFamily: fontFamily, fixedFont: fixedFont});
    },
    getModuleName : function () {
        return (this.moduleName);
    },
    getParentNodeElem : function(table, parentNode) {
        if (table) {
            for (var i = 0; i < table.length; i++) {
                if (table[i].parentNode == parentNode) {
                    return (table[i]);
                }
            }
        }
        return (null);
    },
    getPreviousCompareCell : function (cell, prevRows) {
        /* each row is an array of columns */
        for (var i = 0; i < prevRows.length; i++) {
            for (var j = 0; j < prevRows[i].length; j++) {
                var prevCell = prevRows[i][j];
                if (prevCell.id.indexOf('_-1') > 0) {  /* padding cell */
                    continue;
                }
                if (cell.isCustom) {
                    if (prevCell.displayColumnName == cell.displayColumnName) {
                        return (prevCell);
                    }
                }
                else if (prevCell.selectedColumnName == cell.selectedColumnName &&
                    prevCell.primaryKeyColumnValue == cell.primaryKeyColumnValue) {

                    return (prevCell);
                }
            }
        }
        return (null);
    },
    getPreviousCompareRow : function(prevRows, compareKey, viewType) {
        if (prevRows) {
            if (viewType == 'INSTANCE' || viewType == 'MODULE') {
                for (var i = 0; i < prevRows.length; i++) {
                    if (prevRows[i].compareKey == compareKey) {
                        return (prevRows[i]);
                    }
                }
            }
            else if (viewType == 'INSTANCE_DATA' || viewType == 'MODULE_DATA') {
                for (var i = 0; i < prevRows.length; i++) {
                    for (var j = 0; j < prevRows[i].rowVals.length; j++) {
                        if (prevRows[i].rowVals[j].compareKey == compareKey) {
                            return (prevRows[i].rowVals[j]);
                        }
                    }
                }
            }
        }
        return (null);
    },
    getPreviousInstCompareRow : function(prevInstRows, compareKey) {
        if (prevInstRows) {
            /* viewType is INSTANCE_DATA or MODULE_DATA */
            for (var i = 0; i < prevInstRows.rowVals.length; i++) {
                if (prevInstRows.rowVals[i].compareKey == compareKey) {
                    return (prevInstRows.rowVals[i]);
                }
            }
        }
        return (null);
    },
    getPreviousInstRows : function(prevRows, instLabel) {
        if (prevRows) {
            /* viewType is INSTANCE_DATA or MODULE_DATA */
            for (var i = 0; i < prevRows.length; i++) {
                if (prevRows[i].label == instLabel) {
                    return (prevRows[i]);
                }
            }
        }
        return (null);
    },
    getPreviousRawCompareRow : function (compareKey, prevTableElem) {
       var row = null;
       if (prevTableElem) {
           for (var i = 0; i < prevTableElem.goldChildRows.length; i++) {
               var prevRow = prevTableElem.goldChildRows[i];
               if (!prevRow.hasIcon && prevRow.compareKey == compareKey) {
                   row = prevRow;
                   break;
               }
           }
       }
       return (row);
    },
    getPreviousRawNode : function (compareKey, prevGoldChildRows) {
       var row = null;
       for (var i = 0; i < prevGoldChildRows.length; i++) {
           var prevRow = prevGoldChildRows[i];
           if (prevRow.hasIcon && prevRow.compareKey == compareKey) {
               row = prevRow;
               break;
           }
       }
       return (row);
    },
    getPreviousTreeTableCompareRow : function (compareKey, prevTableElem) {
       var row = null;
       if (prevTableElem) {
           for (var i = 0; i < prevTableElem.goldChildRows.length; i++) {
               var prevRow = prevTableElem.goldChildRows[i];
               if (!prevRow.hasIcon && !prevRow.atMax && prevRow.compareKey == compareKey) {
                   row = prevRow;
                   break;
               }
           }
       }
       return (row);
    },
    getPreviousTreeTableNode : function (compareKey, prevTableElem) {
       var row = null;
       for (var i = 0; i < prevTableElem.goldChildRows.length; i++) {
           var prevRow = prevTableElem.goldChildRows[i];
           if (prevRow.hasIcon && !prevRow.atMax && prevRow.compareKey == compareKey) {
               row = prevRow;
               break;
           }
       }
       return (row);
    },
    /*
     *    Remaps each row item based on selected columns to display and
     *    order of columns from user drag and drop
     */
    getSelectedData : function(viewType, fullRows, checkColumn) {
        var rows = [];
        var cols = [];
        var colsMap = [];
        var columnStates = this.viewsData[this.moduleName + '.' + this.viewName].columnStates;
        var colsOrder = this.viewsData[this.moduleName + '.' + this.viewName].orderedColumns;

        if (checkColumn) {
            for (var i = 0; i < columnStates.length; i++) {
                if (columnStates[i].name == checkColumn) {
                    columnStates[i].checked = true;
                    break;
                }
            }
        }
        if (colsOrder) {
            for (var i = 0; i < colsOrder.length; i++) {
                for (var j = 0; j < columnStates.length; j++) {
                    if (colsOrder[i] == columnStates[j].name && columnStates[j].checked) {
                        cols.push(colsOrder[i]);
                        colsMap.push(j);
                        break;
                    }
                }
            }

            /* added colums to end */
            for (var i = 0; i < columnStates.length; i++) {
                if (columnStates[i].checked && colsOrder.indexOf(columnStates[i].name) == -1) {
                    cols.push(columnStates[i].name);
                    colsMap.push(i);
                }
            }
        }
        else {
            for (var i = 0; i < columnStates.length; i++) {
                if (columnStates[i].checked) {
                    cols.push(columnStates[i].name);
                    colsMap.push(i);
                }
            }
        }
        if (viewType == 'INSTANCE' || viewType == 'MODULE') {
            for (var i = 0; i < fullRows.length; i++) {
                rows.push([]);
                rows[i].compareKey = fullRows[i].compareKey;
                for (var j = 0; j < colsMap.length; j++) {
                    rows[i].push(fullRows[i][colsMap[j]]);
                }
            }
        }
        else if (viewType == 'INSTANCE_DATA' || viewType == 'MODULE_DATA') {
            for (var i = 0; i < fullRows.length; i++) {
                var rowVals = [];
                for (var j = 0; j < fullRows[i].rowVals.length; j++) {
                    rowVals.push([]);
                    rowVals[j].compareKey = fullRows[i].rowVals[j].compareKey;
                    for (var k = 0; k <  colsMap.length; k++) {
                        rowVals[j].push(fullRows[i].rowVals[j][colsMap[k]]);
                    }
                }
                rows.push({});
                rows[i].label = fullRows[i].label;
                rows[i].expanded = fullRows[i].expanded;
                rows[i].rowVals = rowVals;
            }
        }
        else if (viewType == "TREE_TABLE") {
            for (var i = 0; i < fullRows.length; i++) {
                rows.push([]);
                for (var j = 0; j < fullRows[i].length; j++) {
                    var fullChildRow = fullRows[i][j];
                    var childRow = {};
                    childRow.name = fullChildRow.name;
                    childRow.level = fullChildRow.level;
                    childRow.hasIcon = fullChildRow.hasIcon;
                    childRow.style = fullChildRow.style;
                    if (fullChildRow.hasIcon) {
                        childRow.expanded = fullChildRow.expanded;
                        childRow.icon = fullChildRow.icon;
                        childRow.id = fullChildRow.id;
                        childRow.compareKey = fullChildRow.compareKey;
                    }
                    else {
                        childRow.atMax = fullChildRow.atMax;
                        if (!fullChildRow.atMax) {
                            childRow.cells = [];
                            for (var k = 0; k <  colsMap.length; k++) {
                                childRow.cells.push(fullChildRow.cells[colsMap[k]]);
                            }
                            childRow.compareKey = fullChildRow.compareKey;
                        }
                    }
                    rows[i].push(childRow);
                }
            }
        }
        else if (viewType == 'CUSTOM') {
            for (var i = 0; i < fullRows.length; i++) {
                rows.push([]);
                for (var j = 0; j < colsMap.length; j++) {
                    rows[i].push(fullRows[i][colsMap[j]]);
                }
            }
        }
        return ({cols: cols, rows: rows});
    },
    getViewsData : function () {
        return (this.viewsData);
    },
    hasLink : function (val) {
        var match = null;
        if (val && typeof(val) == 'string') {
            match = val.match(/^\[(.+)\]\((\S+)( .*)?\)$/);
            if (match != null && match.length > 2) {
                match[2] = decodeURI(match[2]);
                if (rovUtils.isValidLink(match[2])) {
                    if (match.length == 4) {
                        match[3] = match[3] ? match[3].trim() : '';
                    }
                }
                else {
                    if (match.length == 3) {
                        match.push('');
                    }
                    match[2] = 'rov://~invalid';
                    match[3] = match[3] ? match[3].trim() + '\n' : '';
                    match[3] += 'Invalid link';
                }
            }
        }
        return (match);
    },
    hasSelectableColumns : function(e) {
        return (this.isInstanceView ||
                this.isInstanceDataView ||
                this.isTreeTableView);
    },
    highlightCell : function (rowKeys) {
        if (this.highlighted != null) {
            this.clearHighlight();
        }
        if (this.isInstanceView || this.isInstanceDataView) {
            var row = this.findRow({columnName: rowKeys.primaryKeyColumnName, columnValue: rowKeys.primaryKeyColumnValue});
            var id = rowKeys.selectedColumnName + '_' + row;
        }
        else if (this.isCustomView) {
            var colIndex = -1
            var columns = this.customTable.columns;
            for (var i = 0; i < columns.length; i++) {
                if (columns[i] == rowKeys.displayColumnName) {
                    colIndex = i;
                    break;
                }
            }
            var id = rowKeys.displayColumnName + '_' + colIndex;
        }
        var elem = this.$$('#' + id);
        if (elem) {
            this.highlighted = {elem: elem, prevBackgroundColor: elem.style.backgroundColor};
            elem.style.backgroundColor = 'greenyellow';
        }
        else {
            var text = this.isCustomView ? rowKeys.displayColumnName + ' not found' :
                       rowKeys.selectedColumnName + ' for ' + rowKeys.primaryKeyColumnName + ' = ' + rowKeys.primaryKeyColumnValue + ' not found';
            this.notifyObj.rovViewStatus(text, 'info');
        }
    },
    highlightRow : function (rowKey) {
        if (this.highlighted != null) {
            this.clearHighlight();
        }
        var row = this.findRow(rowKey);
        if (row >= 0) {
            var id;
            var elem;
            var columns = this.isInstanceView ? this.instanceTable.columns : this.instanceDataTable.columns;
            var prevColumns = [];
            for (var i = 0; i < columns.length; i++) {
                id = columns[i] + '_' + row;
                elem = this.$$('#' + id);
                if (elem) {
                    prevColumns.push({name: columns[i], backgroundColor: elem.style.backgroundColor});
                    elem.style.backgroundColor = 'greenyellow';
                }
            }
            this.highlighted = {rowKey: rowKey, columns: prevColumns};
        }
        else {
            this.notifyObj.rovViewStatus(rowKey.columnName + ': ' + rowKey.columnValue + ' not found', 'info');
        }
    },
    initColumnFormats : function (cols, rows) {
        var key = this.moduleName + '.' + this.viewName;
        var columnStates = this.viewsData[key].columnStates;
        this.viewsData[key].hasFormats = false;
        for (var i = 0; i < cols.length; i++) {
            var colFormat = rovUtils.getFormat(rows[i].val);
            for (var j = 0; j < columnStates.length; j++) {
                if (columnStates[j].name == cols[i]) {
                    columnStates[j].hasFormat = (colFormat != null);
                    columnStates[j].format = colFormat;
                    if (!this.viewsData[key].hasFormats && colFormat != null) {
                        this.viewsData[key].hasFormats = true;
                    }
                    break;
                }
            }
        }
        if (this.isGlobal) {
            this.saveViewData();
        }
    },
    initMissingFormats : function (missingCols, cols, rows) {
        var newCols = [];
        var key = this.moduleName + '.' + this.viewName;
        var columnStates = this.viewsData[key].columnStates;
        for (var i = 0; i < missingCols.length; i++) {
            for (var j = 0; j < cols.length; j++) {
                if (cols[j] == missingCols[i]) {
                    for (k = 0; k < columnStates.length; k++) {
                        if (columnStates[k].name == missingCols[i]) {
                            columnStates[k].hasFormat = true;
                            columnStates[k].format = rovUtils.getFormat(rows[j].val);
                            break;
                        }
                    }
                    break;
                }
            }
        }
    },
    initViewsDataItem : function (key) {
        /*  If we are in a dashboard (isGlobal == false) and there
         *  is no saved data for this module.view, use the global
         *  localStorage settings
         */
        var dataItem = this.viewsData[key];
        if (!dataItem && !this.isGlobal) {
            var viewsData = this.getViewsDataStorage();
            dataItem = viewsData[key];
        }
        return (dataItem);
    },
    instanceSelected : function (sel) {
        if (this.instanceDataTable.currentInst != sel.label) {
            this.set('instanceDataTable.currentInst', sel.label);
            /* Contract current expanded instance */
            for (var i = 0; i < this.instanceDataTable.rows.length; i++) {
                if (this.instanceDataTable.rows[i].expanded) {
                    this.set('instanceDataTable.rows.' + i + '.expanded', false);
                    this.instanceDataTable.fullRows[i].expanded = false;
                    break;
                }
            }
            /* Display selected instance */
            for (var i = 0; i < this.instanceDataTable.rows.length; i++) {
                if (this.instanceDataTable.rows[i].label == sel.label) {
                    this.set('instanceDataTable.rows.' + i + '.expanded', true);
                    this.instanceDataTable.fullRows[i].expanded = true;
                    break;
                }
            }
            this.viewsData[this.moduleName + '.' + this.viewName].currentInst = sel.label;
            this.viewsData[this.moduleName + '.' + this.viewName].currentInstIdx = sel.index;
            if (this.isGlobal) {
                this.saveViewData();
            }
        }
    },
    isNumericColumn : function (columnName) {
        var hasFormat = false;
        var columnStates = this.viewsData[this.moduleName + '.' + this.viewName].columnStates;
        for (var i = 1; i < columnStates.length; i++) {
            if (columnStates[i].name == columnName) {
                hasFormat = columnStates[i].hasFormat;
            }
        }
        return (hasFormat);
    },
    linkClicked : function (e) {
        e.preventDefault();
        if (e.currentTarget.href && e.currentTarget.href.indexOf('rov://~invalid') == -1) {
            if (e.currentTarget.href.indexOf('rov://') >= 0) {
                var params = new URLSearchParams(e.currentTarget.search);
                if (e.currentTarget.href.indexOf('/addon') == -1) {
                    var module = params.get('module');
                    var view = params.get('view');
                    var row = params.get('row');
                    if (row) {
                        row = row.split(':');
                    }
                    var col = params.get('col');
                    var rowKeys = row ? {primaryKeyColumnName: row[0],
                                         primaryKeyColumnValue: row[1],
                                         selectedColumnName: col}
                                      : null;
                    try {
                        thePanel.viewFromGraphOrCustom(module, view, null, rowKeys);
                    }
                    catch (e) {
                        console.error(e);
                        thePanel.showStatus("Error opening Module: '" + module + "', View: '" + view + "' - " + e.message, "error");
                    }
                }
                else if (params.get('markdown') != null) {
                    params = params.get('params');
                    try {
                        var args = null;
                        var viewParams = params.match(/(.*):([^,]*),(.*)/) || params.match(/(.*):([^,]*)/);
                        if (viewParams.length > 3) {
                            args = viewParams[3];
                        }
                        thePanel.newMarkdownView(viewParams[1], viewParams[2], args);
                    }
                    catch (e) {
                        console.error(e);
                        thePanel.showStatus("Error opening manpage add on: '" + params + "' - " + e.message, "error");
                    }
                }
            }
            else {
                try {
                    var href = e.currentTarget.href;
                    var match = e.currentTarget.outerHTML.match(/href="(.*)"/);
                    if (this.ccs7 && match && match[1].indexOf('http') == -1 && match[1].indexOf(':/') == -1 && match[1][0] != '/') {
                        href = '~' + thePanel.getSessionId() + '/' + match[1];
                    }
                    window.open(href ,'_blank');
                }
                catch (e) {
                    thePanel.showStatus(e.message, "error");
                }
            }
        }
    },
    makeRawRow : function(obj, prop, qualName, allRows, parentIndex, depth, fontFamily) {
        var childRow = {};
        childRow.property = prop;
        childRow.level = depth;
        childRow.hasIcon = false;
        childRow.style = this.rawRowStyle(depth, false) + fontFamily;
        if (rovUtils.isArray(obj[prop])) {
            if (obj[prop].length > 0) {
                childRow.hasIcon = true;
                childRow.style = this.rawRowStyle(depth, true) + fontFamily;
                childRow.expanded = this.expandAll ? true : false;
                childRow.icon = this.expandAll ? 'remove' : 'add';
                childRow.id = parentIndex + '_' + allRows.length;
                childRow.value = '';
                childRow.compareKey = qualName;
                allRows.push(childRow);
                var arr = obj[prop];
                if (rovUtils.isObject(arr[0])) { /* Array of Objects */
                    ++depth;
                    for (var i = 0; i < arr.length; i++) {
                        var objChildRow = {};
                        objChildRow.property = arr[i].$name ? arr[i].$name : i;
                        objChildRow.level = depth;
                        objChildRow.hasIcon = true;
                        objChildRow.style = this.rawRowStyle(depth, true) + fontFamily;
                        objChildRow.expanded = this.expandAll ? true : false;
                        objChildRow.icon = this.expandAll ? 'remove' : 'add';
                        objChildRow.id = parentIndex + '_' + allRows.length;
                        objChildRow.value = '';
                        objChildRow.compareKey = qualName + '.' + objChildRow.property;
                        allRows.push(objChildRow);
                        for (var objProp in arr[i]) {
                            var newQual = qualName + '.' + objChildRow.property + '.' + objProp;
                            this.makeRawRow(arr[i], objProp, newQual, allRows, parentIndex, depth + 1, fontFamily);
                        }
                    }
                }
                else {
                    for (var i = 0; i < arr.length; i++) {
                        this.makeRawRow(arr, i,  qualName + '.' + i, allRows, parentIndex, depth + 1, fontFamily);
                    }
                }
            }
            else {
                childRow.compareKey = qualName;
                childRow.value = '';
                childRow.valStyle = 'display:table-cell;';
                childRow.valTitle = '';
                allRows.push(childRow);
            }
        }
        else if (rovUtils.isObject(obj[prop])) {
            if (Object.keys(obj[prop]).length > 0) { /* Has keys? */
                childRow.hasIcon = true;
                childRow.style = this.rawRowStyle(depth, true) + fontFamily;
                childRow.expanded = this.expandAll ? true : false;
                childRow.icon = this.expandAll ? 'remove' : 'add';
                childRow.id = parentIndex + '_' + allRows.length;
                childRow.value = '';
                childRow.compareKey = qualName;
                allRows.push(childRow);
                for (var chProp in obj[prop]) {
                    this.makeRawRow(obj[prop], chProp,  qualName + '.' + chProp, allRows, parentIndex, depth + 1, fontFamily);
                }
            }
            else {
                childRow.compareKey = qualName;
                childRow.value = '';
                childRow.valStyle = '';
                childRow.valTitle = '';
                allRows.push(childRow);
            }
        }
        else {
            childRow.compareKey = qualName;
            childRow.value = obj[prop];
            childRow.valStyle = fontFamily;
            childRow.valTitle = '';
            allRows.push(childRow);
        }
    },
    makeTreeTableRows : function(dataObj, qualName, allRows, parentIndex, depth, allCols, colFormats, fontFamily) {
        var curQual = qualName ? qualName + '.' + dataObj.label : dataObj.label;
        if (depth > 0) {
            if (!this.atMaxDepth(dataObj)) {
                var childRow = {name: dataObj.label, level: depth, hasIcon: true, cells: []};
                childRow.style = this.treeTableRowStyle(depth, true) + fontFamily;
                childRow.expanded = this.expandAll ? true : false;
                childRow.icon = this.expandAll ? 'remove' : 'add';
                childRow.id = parentIndex + '_' + allRows.length;
                childRow.compareKey = curQual;
                allRows.push(childRow);
            }
        }
        for (var i = 0; i < dataObj.properties.length; i++) {
            var rowObj = dataObj.properties[i];
            if (typeof(rowObj) == 'string' && rowObj.search(/Max depth/) >= 0) {
                var childRow = {name: dataObj.label, level: depth, hasIcon: false, atMax: true};
                childRow.style = this.treeTableRowStyle(depth, false) + fontFamily;
                allRows.push(childRow);
                break;
            }
            var childRow = {level: depth + 1, hasIcon: false, atMax: false, cells: []};
            for (var j = 0; j < allCols.length; j++) {
                var propName = allCols[j];
                var cellObj = {};
                cellObj.val = rowObj[propName];
                if (rovUtils.isArray(cellObj.val) && cellObj.val.length == 1) {
                    cellObj.val = cellObj.val[0];
                }
                if (j == 0) {
                    childRow.compareKey = curQual + '.' + cellObj.val;
                }
                cellObj.isLink = false;
                var linkDesc = this.hasLink(cellObj.val);
                if (linkDesc != null) {
                    cellObj.isLink = true;
                    cellObj.val = linkDesc[1];
                    cellObj.href = linkDesc[2];
                    cellObj.title = linkDesc[3];
                }
                else if (rovUtils.isArray(cellObj.val)) {
                    this.cellArray(cellObj, colFormats, propName, fontFamily);
                }
                else {
                    cellObj.val = this.formatVal(cellObj.val, colFormats, propName);
                }
                if (rowObj[propName + '$status']) {
                    cellObj.style = cellObj.isArray ? (cellObj.style + 'background-color:#ff4d4d;') :
                                    this.rovTdStyle + 'background-color:#ff4d4d;' + fontFamily;
                    if (!cellObj.isLink) {
                        cellObj.title = rowObj[propName + '$status'];
                    }
                    else {
                        cellObj.title = rowObj[propName + '$status'] + '\n' + cellObj.title;
                    }
                    if (cellObj.val == '') {
                        cellObj.val = 'error';
                    }
                }
                else {
                    if (!cellObj.isArray) {
                        cellObj.style = this.rovTdStyle + fontFamily;
                    }
                    if (!cellObj.isLink) {
                        cellObj.title = '';
                    }
                }
                childRow.cells.push(cellObj);
            }
            allRows.push(childRow);
        }
        for (var i = 0; i < dataObj.children.length; i++) {
            this.makeTreeTableRows(dataObj.children[i], curQual, allRows, parentIndex, depth + 1, allCols, colFormats, fontFamily);
        }
    },
    noTable : function () {
        this.isInstanceView = false;
        this.isTreeView = false;
        this.isInstanceDataView = false;
        this.isTreeTableView = false;
        this.isRawView = false;
        this.showDataMessage = false;
    },
    onColumnClick : function (event) {
        var colIndex = this.getColumnIndex(event.currentTarget.textContent);
        if (colIndex >= 0) {
            this.columnSort(colIndex, true);
        }
    },
    rawNodeTapped : function(event) {
        var index;
        var node = event.currentTarget.id;
        for (var i = 0; i < this.rawTable.length; i++) {
            if (this.rawTable[i].parentNode == node) {
                index = i;
                break;
            }
        }
        if (index >= 0) {
            var str = 'rawTable.' + index + '.expanded';
            this.set(str, !this.rawTable[index].expanded);
        }
    },
    rawRowStyle : function(depth, hasIcon) {
        var pad = 35 + (depth * 25);
        if (hasIcon) {
            pad -= 10;
        }
        var style = 'display:table-cell;padding-left:' + pad + 'px;white-space:nowrap;';
        if (hasIcon) {
            style += 'cursor:pointer;';
        }
        return (style);
    },
    refreshRawExpandedState : function(srcGoldChildRows, destGoldChildRows) {
        for (var i = 0; i < destGoldChildRows.length; i++) {
            var curRow = destGoldChildRows[i];
            if (curRow.hasIcon) {
                var prevNode = this.getPreviousRawNode(curRow.compareKey, srcGoldChildRows);
                if (prevNode) {
                    curRow.expanded = prevNode.expanded;
                    curRow.icon = prevNode.icon;
                }
            }
        }
    },
    rowsCompare : function(prevRows, newRows, viewType, fontFamily) {
        if (!newRows) {
            return;
        }
        if (viewType == 'INSTANCE' || viewType == 'MODULE') {
            for (var i = 0; i < newRows.length; i++) {
                var prevRow = this.getPreviousCompareRow(prevRows, newRows[i].compareKey, viewType);
                for (var j = 0; j < newRows[i].length; j++) {
                    var changed = false;
                    if (prevRow == null) {
                        changed = true
                    }
                    else {
                        for (var k = 0; k < prevRow.length; k++) {
                            if (newRows[i][j].id == prevRow[k].id) {
                                if (newRows[i][j].isArray || prevRow[k].isArray) {
                                    if (!newRows[i][j].isArray || !prevRow[k].isArray) {
                                        changed = true;
                                    }
                                    else {
                                        changed = this.cellArrayCompare(prevRow[k].arrVals, newRows[i][j].arrVals);
                                    }
                                    if (changed) {
                                        break;
                                    }
                                }
                                else if (!newRows[i][j].isLink && !prevRow[k].isLink &&
                                         newRows[i][j].val != prevRow[k].val) {
                                    changed = true;
                                    break;
                                }
                            }
                        }
                    }
                    if (changed && newRows[i][j].style.indexOf('#ff4d4d') == -1) {
                        newRows[i][j].style = this.rovTdStyle + 'background-color:gold;' + fontFamily;
                        if (prevRow != null && prevRow[k].isArray) {
                            var prevArr = prevRow[k].arrVals;
                            newRows[i][j].title = 'Previous values: ';
                            for (var l = 0; l < prevArr.length; l++) {
                                newRows[i][j].title += '\n' + prevArr[l].val;
                            }
                        }
                        else {
                            newRows[i][j].title = (prevRow == null) ? 'New row' : 'Previous value: ' + prevRow[k].val;
                        }
                    }
                }
            }
        }
        else if (viewType == 'INSTANCE_DATA' || viewType == 'MODULE_DATA') {
            if (this.instanceDataTable.showInstSelect) {
                for (var i = 0; i < newRows.length; i++) {
                    var prevInstRows = this.getPreviousInstRows(prevRows, newRows[i].label);
                    if (!prevInstRows) {
                        /* mark all rows in the instance as modified(new) */
                        for (var j = 0; j < newRows[i].rowVals.length; j++) {
                            for (var k = 0; k < newRows[i].rowVals[j].length; k++) {
                                if (!newRows[i].rowVals[j][k].isLink && newRows[i].rowVals[j][k].style.indexOf('#ff4d4d') == -1) {
                                    newRows[i].rowVals[j][k].style = this.rovTdStyle + 'background-color:gold;' + fontFamily;
                                    newRows[i].rowVals[j][k].title = 'New row';
                                }
                            }
                        }
                    }
                    else {
                        for (var j = 0; j < newRows[i].rowVals.length; j++) {
                            var prevRowVals = this.getPreviousInstCompareRow(prevInstRows, newRows[i].rowVals[j].compareKey);
                            for (var k = 0; k < newRows[i].rowVals[j].length; k++) {
                                var changed = false;
                                if (prevRowVals == null) {
                                    changed = true;
                                }
                                else {
                                    if (newRows[i].rowVals[j][k].isArray || prevRowVals[k].isArray) {
                                        if (!newRows[i].rowVals[j][k].isArray || !prevRowVals[k].isArray) {
                                            changed = true;
                                        }
                                        else {
                                            changed = this.cellArrayCompare(prevRowVals[k].arrVals, newRows[i].rowVals[j][k].arrVals);
                                        }
                                    }
                                    else if (!newRows[i].rowVals[j][k].isLink &&
                                             newRows[i].rowVals[j][k].val != prevRowVals[k].val) {
                                        changed = true;
                                    }
                                }
                                if (changed && newRows[i].rowVals[j][k].style.indexOf('#ff4d4d') == -1) {
                                    newRows[i].rowVals[j][k].style = this.rovTdStyle + 'background-color:gold;' + fontFamily;
                                    if (prevRowVals != null && prevRowVals[k].isArray) {
                                        var prevArr = prevRowVals[k].arrVals;
                                        newRows[i].rowVals[j][k].title = 'Previous values: ';
                                        for (var l = 0; l < prevArr.length; l++) {
                                            newRows[i].rowVals[j][k].title += '\n' + prevArr[l].val;
                                        }
                                    }
                                    else {
                                        newRows[i].rowVals[j][k].title = (prevRowVals == null) ? 'New row' : 'Previous value: ' + prevRowVals[k].val;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            else {
              for (var i = 0; i < newRows.length; i++) {
                for (var j = 0; j < newRows[i].rowVals.length; j++) {
                    var prevRowVals = this.getPreviousCompareRow(prevRows, newRows[i].rowVals[j].compareKey, viewType);
                    for (var k = 0; k < newRows[i].rowVals[j].length; k++) {
                        var changed = false;
                        if (prevRowVals == null) {
                            changed = true;
                        }
                        else {
                            if (newRows[i].rowVals[j][k].isArray || prevRowVals[k].isArray) {
                                if (!newRows[i].rowVals[j][k].isArray || !prevRowVals[k].isArray) {
                                    changed = true;
                                }
                                else {
                                    changed = this.cellArrayCompare(prevRowVals[k].arrVals, newRows[i].rowVals[j][k].arrVals);
                                }
                            }
                            else if (!newRows[i].rowVals[j][k].isLink &&
                                     newRows[i].rowVals[j][k].val != prevRowVals[k].val) {
                                changed = true;
                            }
                        }
                        if (changed && newRows[i].rowVals[j][k].style.indexOf('#ff4d4d') == -1) {
                            newRows[i].rowVals[j][k].style = this.rovTdStyle + 'background-color:gold;' + fontFamily;
                            if (prevRowVals != null && prevRowVals[k].isArray) {
                                var prevArr = prevRowVals[k].arrVals;
                                newRows[i].rowVals[j][k].title = 'Previous values: ';
                                for (var l = 0; l < prevArr.length; l++) {
                                    newRows[i].rowVals[j][k].title += '\n' + prevArr[l].val;
                                }
                            }
                            else {
                                newRows[i].rowVals[j][k].title = (prevRowVals == null) ? 'New row' : 'Previous value: ' + prevRowVals[k].val;
                            }
                        }
                    }
                }
              }
            }
        }
        else if (viewType == 'TREE_TABLE') {
            /* For TREE_TABLE, we're passing in whole tables, not just rows; cast accordingly */
            var prevTable = prevRows;
            var curTable = newRows;
            for (var i = 0; i < curTable.length; i++) {
                var prevElem = this.getParentNodeElem(prevTable, curTable[i].parentNode);
                if (!prevElem) {
                    /* mark all rows in the instance as modified(new) */
                    for (var j = 0; j < curTable[i].goldChildRows.length; j++) {
                        var curRow = curTable[i].goldChildRows[j];
                        if (!curRow.hasIcon && !curRow.atMax) {
                            for (var k = 0; k < curRow.cells.length; k++) {
                                if (!curRow.cells[k].isLink && curRow.cells[k].style.indexOf('#ff4d4d') == -1) {
                                    curRow.cells[k].style = this.rovTdStyle + 'background-color:gold;' + fontFamily;
                                    curRow.cells[k].title = 'New row';
                                }
                            }
                        }
                    }
                }
                else {
                    for (var j = 0; j < curTable[i].goldChildRows.length; j++) {
                        var curRow = curTable[i].goldChildRows[j];
                        if (!curRow.hasIcon && !curRow.atMax) {
                            var prevRow = this.getPreviousTreeTableCompareRow(curRow.compareKey, prevElem);
                            for (var k = 0; k < curRow.cells.length; k++) {
                                var changed = false;
                                if (prevRow == null) {
                                    changed = true
                                }
                                else {
                                    if (curRow.cells[k].isArray || prevRow.cells[k].isArray) {
                                        if (!curRow.cells[k].isArray || !prevRow.cells[k].isArray) {
                                            changed = true;
                                        }
                                        else {
                                            changed = this.cellArrayCompare(prevRow.cells[k].arrVals, curRow.cells[k].arrVals);
                                        }
                                    }
                                    else if (!curRow.cells[k].isLink &&
                                             curRow.cells[k].val != prevRow.cells[k].val) {
                                        changed = true;
                                    }
                                }
                                if (changed && curRow.cells[k].style.indexOf('#ff4d4d') == -1) {
                                    curRow.cells[k].style = this.rovTdStyle + 'background-color:gold; + fontFamily';
                                    if (prevRow != null && prevRow.cells[k].isArray) {
                                        var prevArr = prevRow.cells[k].arrVals;
                                        curRow.cells[k].title = 'Previous values: ';
                                        for (var l = 0; l < prevArr.length; l++) {
                                            curRow.cells[k].title += '\n' + prevArr[l].val;
                                        }
                                    }
                                    else {
                                        curRow.cells[k].title = (prevRow == null) ? 'New row' : 'Previous value: ' + prevRow.cells[k].val;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        else if (viewType == 'RAW') {
            /* For RAW, we're passing in whole tables, not just rows; cast accordingly */
            var prevTable = prevRows;
            var curTable = newRows;
            for (var i = 0; i < curTable.length; i++) {
                var prevElem = this.getParentNodeElem(prevTable, curTable[i].parentNode);
                for (var j = 0; j < curTable[i].goldChildRows.length; j++) {
                    var curRow = curTable[i].goldChildRows[j];
                    if (!curRow.hasIcon) {
                        var prevRow = this.getPreviousRawCompareRow(curRow.compareKey, prevElem);
                        if (prevRow == null || curRow.value != prevRow.value) {
                            curRow.valStyle = 'background-color:gold;' + fontFamily;
                            curRow.valTitle = (prevRow == null) ? 'New row' : 'Previous value: ' + prevRow.value;
                        }
                    }
                }
            }
        }
        else if (viewType == 'CUSTOM') {
            if (prevRows) {
                /* each row is an array of columns */
                for (var i = 0; i < newRows.length; i++) {
                    for (var j = 0; j < newRows[i].length; j++) {
                        var cell = newRows[i][j];
                        if (cell.id.indexOf('_-1') > 0) {  /* padding cell */
                            continue;
                        }
                        var prevCell = this.getPreviousCompareCell(cell, prevRows);
                        if (prevCell != null && prevCell.val != cell.val) {
                            if (cell.style.indexOf('#ff4d4d') == -1) {
                                cell.style = this.rovTdStyle + 'background-color:gold;' + fontFamily;
                                cell.title = 'Previous value: ' + prevCell.val;
                            }
                        }
                    }
                }
            }
        }
    },
    saveViewData : function () {
        var viewsData = this.getViewsDataStorage();
        var key = this.moduleName + '.' + this.viewName;
        viewsData[key] = this.viewsData[key];
        this.setViewsDataStorage(viewsData);
    },
    setIsGlobal : function (isGlobal) {
        this.isGlobal = isGlobal;
    },
    setModuleName : function (name) {
        if (name != this.moduleName) {
            this.moduleName = name;
            this.noTable();
        }
    },
    setNotifyObj : function (obj) {
        this.notifyObj = obj;
    },
    setViewsData : function (viewsData) {
        this.viewsData = viewsData;
    },
    treeNodeTapped : function(event) {
        var index;
        var node = event.currentTarget.id;
        for (var i = 0; i < this.treeTable.length; i++) {
            if (this.treeTable[i].parentNode == node) {
                index = i;
                break;
            }
        }
        if (index >= 0) {
            var str = 'treeTable.' + index + '.expanded';
            this.set(str, !this.treeTable[index].expanded);
        }
    },
    treeTableRowStyle : function(depth, hasIcon) {
        var pad = (depth * 25);
        var style = 'display:table-cell;padding-left:' + pad + 'px;white-space:nowrap;';
        if (hasIcon) {
            style += 'cursor:pointer;';
        }
        return (style);
    },
    ttNodeTapped : function(event) {
        var index;
        var node = event.currentTarget.id;
        for (var i = 0; i < this.treeTableTable.length; i++) {
            if (this.treeTableTable[i].parentNode == node) {
                index = i;
                break;
            }
        }
        if (index >= 0) {
            var str = 'treeTableTable.' + index + '.expanded';
            this.set(str, !this.treeTableTable[index].expanded);
        }
    },
    updateColumnsData : function(table, cols) {
        if (table.allCols.join(' ') != cols.join(' ')) {
            this.viewsData[this.moduleName + '.' + this.viewName].orderedColumns = rovUtils.shallowCopy(cols);
        }
        else {
            /* If matches original data, don't save order */
            this.viewsData[this.moduleName + '.' + this.viewName].orderedColumns = null;
        }
    },

    viewNameChanged : function (newName) {
        var key = this.moduleName + '.' + this.viewName;
        if (this.viewsData[key]) {
            var data = rovUtils.shallowCopy(this.viewsData[key]);
            delete this.viewsData[key];
            this.viewsData[this.moduleName + '.' + newName] = data;
        }
        this.viewName = newName;
    }
});
