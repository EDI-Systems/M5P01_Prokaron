Polymer({is: "ti-rov-customview",
    behaviors: [
      rovBehaviors.viewBehaviors,
      rovBehaviors.notifyLogBehaviors,
      rovBehaviors.viewStatusBehaviors
    ],

    properties: {

        moduleName: {
          type: String,
          value: ''
        },

        viewName: {
            type:  String,
            value: ''
        },

        viewRefresh: { /* recommended view state indicator */
            type:  Boolean,
            value: false
        },

        viewList : {
            type: Object,
            value: {}
        },

        tableName: {
            type: String,
            value: ''
        },

        sameModuleView : {
            type: Boolean,
            value: true
        },

        requests: {        /* outstanding requests */
            type: Number,
            value: 0
        },

        rovCells : {
            type: Array,
            value: []
        },

        removeButtonHidden: {
            type: Boolean,
            value: false
        },

        selectedCell: {
            type: Object,
            value: function() { return {};}
        },

        primaryKeyMenuItems: {
            type: Array,
            value: []
        },

        primaryKeyMenuLabel: {
            type: String,
            value: ''
        },

        primaryKeySectionHidden: {
            type: Boolean,
            value: false
        },

        titleSectionHidden: {
            type: Boolean,
            value: false
        },

        expressionSectionHidden: {
            type: Boolean,
            value: false
        },

        persistProperties: {
            type: Array,
            value: ['tableName', 'logMax', 'logWrap']
        },

        dataRefresh: {
            type:  Boolean,
            value: false
        },

        viewsData : {
          type: Object,
          value: null
        },

        actionDisabled: {
            type: Boolean,
            value: true
        },

        actionLabelValue: {
            type: String,
            value: 'Action: Apply to cell'
        },

        cellExpressionErrorDesc: {
            type: String,
            value: ''
        },

        cellNameErrorDesc: {
            type: String,
            value: ''
        },

        tableEvalWorker : {
            type: Object,
            value: null
        },

        dialogEvalWorker : {
            type: Object,
            value: null
        },

        exprCount : {
            type : Number,
            value : 0
        }
    },

    listeners: {
      'iron-resize': 'onIronResize'
    },

/*
    rovCell = {module,
                view,
                args,
                viewArgs,
                argsId,
                viewType,
                firstArg,
                selectedColumnName,
                displayColumnName,
                primaryKeyColumnName,
                primaryKeyColumnValue,
                primaryKeyColumns,
                assignedName,
                viewData,
                columns,
                format,
                expression,
                expressionType,
                origVal,
                val,
                status,
                viewData,
                isCustom,
                actions
                  saveView
                  saveAll
                  stopUpdates
                  notify
                  saveDashboard
                  saveDashboardName
    }
*/

/*
    customCell
      isCustom
      displayColumnName
      expression
      expressionType
      val
      origVal - not used for custom
*/


   /*
    *  ======== actionSelectCheckboxClicked ========
    */
    actionSelectCheckboxClicked : function (e) {
        var action = e.currentTarget.id.replace(/ActionBox/, '');
        if (action == 'logNotifications') {
            this.$.notifyLogCommentButton.disabled = !e.currentTarget.checked;
        }
    },

   /*
    *  ======== addCell ========
    */
    addCell : function (newCell) {
/*
        if (this.$.customViewOptionsMenuDialog.opened) {
            this.$.customViewOptionsMenuDialog.close();
        }
*/
        if (this.sameModuleView) {
            this.sameModuleView = this.isSameModuleView(this.rovCells[0], newCell);
        }

        var cell = {};
        cell.args = newCell.args;
        cell.module = newCell.module;
        cell.view = newCell.view;
        cell.viewType = newCell.viewType ? newCell.viewType : this.getViewType(cell.module, cell.view);
        cell.viewArgs = rovUtils.shallowCopy(newCell.viewArgs);
        cell.argsId = newCell.argsId;
        cell.selectedColumnName = newCell.selectedColumnName;
        cell.primaryKeyColumnName = newCell.primaryKeyColumnName;
        cell.primaryKeyColumnValue = newCell.primaryKeyColumnValue;
        cell.val = newCell.selectedColumnValue;
        cell.origVal = cell.val;
        if (this.sameModuleView && this.rovCells[0].viewData) {
            cell.viewData = this.rovCells[0].viewData;
        }
        else {
            cell.viewData = newCell.viewData;
        }
        if (cell.args) {
            cell.firstArg = cell.args.substr(1, cell.args.indexOf(',') - 1);
        }
        else if (cell.viewType != 'MODULE') {
            cell.columns = this.getViewColumns(cell.module, cell.view);
            cell.primaryKeyColumns = [];
            for (var i = 0; i < cell.columns.length; i++) {
                if (cell.columns[i] != cell.selectedColumnName) {
                    cell.primaryKeyColumns.push(cell.columns[i]);
                }
            }
        }
        cell.displayColumnName = this.checkDisplayColumnName(this.nameCell(cell), cell);
        this.rovCells.push(cell);
        this.set('removeButtonHidden', this.rovCells.length == 1);
        var tableData = this.formatTableData();
        this.$.tiRovTable.displayTable(tableData.data, this.viewName, 'CUSTOM', tableData.columns, true);
    },

    /*
    *  ======== addCustomCell ========
    */
    addCustomCell : function (e) {
        var cell = {};
        cell.isCustom = true;
        cell.displayColumnName = this.$.assignedCellNameInput.value;
        var val = '';
        if (this.$.cellExpressionInput.value != '') {
            var expr = this.makeCellExpression(this.$.cellExpressionInput.value,
                                               cell.displayColumnName, true);
            if (expr.status) {
                this.$.cellExpressionInvalidIcon.hidden = false;
                this.cellExpressionErrorDesc = expr.status;
                return;
            }
            expr.expression = '"use strict";' + expr.expression;
            this.$.cellPropertiesDialog.expr = expr;
            this.$.cellPropertiesDialog.cell = cell;
            this.dialogEvalWorker.postMessage([expr.expression, true]);
            return;
        }
        this.newCustomCellCloseCellPropertiesDialog(cell, val);
    },

   /*
    *  ======== attached ========
    */
    attached : function () {
        this.yesterday = Date.now() - 900000;
        this.rovCells = [];
        this.viewList = this.rovPanel.getViewList();
        this.viewName = this.tableName;
        for (var i = 0; i < this.viewCells.length; i++) {
            var cell = {};
            if (this.viewCells[i].isCustom) {
                cell.isCustom = true;
                cell.displayColumnName = this.viewCells[i].displayColumnName;
                cell.expressionType = this.viewCells[i].expressionType;
                cell.expression = this.viewCells[i].expression;
                cell.actions = this.viewCells[i].actions;
            }
            else {
                cell.args = this.viewCells[i].args;
                cell.module = this.viewCells[i].module;
                cell.view = this.viewCells[i].view;
                cell.viewType = this.viewCells[i].viewType ? this.viewCells[i].viewType : this.getViewType(cell.module, cell.view);
                cell.viewArgs = rovUtils.shallowCopy(this.viewCells[i].viewArgs);
                cell.argsId = this.viewCells[i].argsId;
                cell.selectedColumnName = this.viewCells[i].selectedColumnName;
                cell.primaryKeyColumnName = this.viewCells[i].primaryKeyColumnName;
                cell.primaryKeyColumnValue = this.viewCells[i].primaryKeyColumnValue;
                if (this.viewCells[i].selectedColumnValue) {
                    cell.format = rovUtils.getFormat(this.viewCells[i].selectedColumnValue);
                }
                cell.expression = this.viewCells[i].expression ? this.viewCells[i].expression : '';
                cell.expressionType = this.viewCells[i].expressionType;
                cell.actions = this.viewCells[i].actions;
                if (cell.args) {
                    cell.firstArg = cell.args.substr(1, cell.args.indexOf(',') - 1);
                }
                else if (cell.viewType != 'MODULE') {
                    cell.columns = this.getViewColumns(cell.module, cell.view);
                    cell.primaryKeyColumns = [];
                    for (var j = 0; j < cell.columns.length; j++) {
                        if (cell.columns[j] != cell.selectedColumnName) {
                            cell.primaryKeyColumns.push(cell.columns[j]);
                        }
                    }
                }
                cell.displayColumnName = this.viewCells[i].displayColumnName ?
                                         this.viewCells[i].displayColumnName : this.nameCell(cell);
            }
            this.rovCells.push(cell);
        }

        this.set('removeButtonHidden', this.rovCells.length == 1);

        /* initialize all our state */
        this.sameModuleView = true;
        for (var i = 0; i < this.rovCells.length - 1; i++) {
            for (j = i + 1; j < this.rovCells.length; j++) {
                if (!this.isSameModuleView(this.rovCells[i], this.rovCells[j])) {
                    this.sameModuleView = false;
                    break;
                }
            }
            if (!this.sameModuleView) {
                break;
            }
        }
        if (this.viewsData != null) {
            this.$.tiRovTable.setViewsData(this.viewsData);
        }
        var pre = this.rovPanel.ccs7 ? './' : '../../';
        this.tableEvalWorker = new Worker(pre + 'client/evalWorker.js');
        this.tableEvalWorker.onmessage = this.tableEvalWorkerMessage;
        this.tableEvalWorker.msgObj = this;
        this.viewStatusPosition = 'bottom';
        this.onRefresh(true);
    },

   /*
    *  ======== cellExpressionChanged ========
    */
    cellExpressionChanged : function (e) {
        if (this.$.cellPropertiesDialog.opened) {
            if (!this.$.cellExpressionInvalidIcon.hidden) {
                this.$.cellExpressionInvalidIcon.hidden = true;
            }
            if (this.$.cellExpressionInput.value != '') {
                var error = false;
                var expr = this.makeCellExpression(this.$.cellExpressionInput.value,
                                                   this.selectedCell.displayColumnName,
                                                   this.selectedCell.isCustom);
                if (!expr.status) {
                    try {
                        var val = eval('"use strict";' + expr.expression + ';');
                    }
                    catch (ev) {
                        error = true;
                    }
                }
                else {
                    error = true;
                }
                this.enableCellActions(error == false && typeof(val) == 'boolean');
            }
            else {
                this.enableCellActions(false);
            }
        }
    },

   /*
    *  ======== cellExpressionDecls ========
    */
    cellExpressionDecls : function (cellName) {
        var decls = [];
        for (var i = 0; i < this.rovCells.length; i++) {
            var cell = this.rovCells[i];
            if (cell.name != cellName) {
                var decl = 'var ' + cell.displayColumnName + '=';
                decl += cell.expressionTpe == 'boolean' ? cell.origVal : cell.val;
                decls.push(decl);
            }
        }
        return (decls.length > 0 ? decls.join(';') + ';': null);
    },

   /*
    *  ======== cellNameChanged ========
    */
    cellNameChanged : function (e) {
        if (this.$.cellPropertiesDialog.opened) {
            if (!this.$.cellNameInvalidIcon.hidden) {
                this.$.cellNameInvalidIcon.hidden = true;
            }
        }
    },

   /*
    *  ======== cellNameMenuItemSelected ========
    */
    cellNameMenuItemSelected : function (e) {
        this.$.cellNamesMenu.selected = -1;
            /* delete trace button */
        if (e.srcElement.id != 'icon' && !this.$.cellPropertiesDialog.opened) {
            var idElem = e.currentTarget.lastElementChild;
            var i = Number(idElem.id.substr(idElem.id.indexOf('_') + 1));
            var selectedCell = this.rovCells[i];
            if (selectedCell.isCustom ||
                (!selectedCell.isCustom && !selectedCell.args && selectedCell.viewType != 'MODULE')) {
                this.set('selectedCell', selectedCell);
                this.selectedCell.index = i;
                this.cellPropertiesDialog(e);
            }
        }
    },

   /*
    *  ======== cellPropertiesClicked ========
    */
    cellPropertiesClicked : function (e) {
        if (e.srcElement.className.indexOf('tableSettingsMenuI') >= 0) {
            this.$.cellNamesMenu.hidden = !this.$.cellNamesMenu.hidden;
            this.$.tableSettingsMenu.selected = -1;
            this.$.cellPropertiesSubMenu.selected = -1;
            this.$.cellNamesMenu.selected = -1;
        }
    },

   /*
    *  ======== cellPropertiesDialog ========
    */
    cellPropertiesDialog : function (event) {
        this.$.cellExpressionInvalidIcon.hidden = true;
        this.$.cellNameInvalidIcon.hidden = true;
        if (this.selectedCell.isCustom) {
            this.cellPropertiesDlgTitle = this.selectedCell.newCell ? 'New cell properties' : 'Cell properties';
        }
        else {
            this.cellPropertiesDlgTitle = this.selectedCell.displayColumnName;
        }
        if (!this.selectedCell.args && this.selectedCell.viewType != 'MODULE' && !this.selectedCell.isCustom) {
            this.primaryKeySectionHidden = false;
            var primaryKeyMenuItems = [];
            var primaryKeyColumns = this.selectedCell.primaryKeyColumns;
            var maxStr = '';
            var pKeyIndex = 0;
            for (var i = 0; i < primaryKeyColumns.length; i++) {
                var value = this.getCellValue(this.selectedCell.viewType,
                                              this.selectedCell.primaryKeyColumnName, this.selectedCell.primaryKeyColumnValue,
                                              primaryKeyColumns[i], this.selectedCell.viewData);
                primaryKeyMenuItems.push(primaryKeyColumns[i] + ':  ' + value.val);
                if (primaryKeyColumns[i] == this.selectedCell.primaryKeyColumnName) {
                    pKeyIndex = i;
                }
                if (primaryKeyMenuItems[i].length > maxStr.length) {
                    maxStr = primaryKeyMenuItems[i];
                }
            }

            this.set('primaryKeyMenuItems', primaryKeyMenuItems);
            this.set('primaryKeyMenuLabel', this.selectedCell.primaryKeyColumnName + ':  ' +  this.selectedCell.primaryKeyColumnValue);
        }
        else {
            this.primaryKeySectionHidden = true;
        }
        this.expressionSectionHidden = !this.selectedCell.isCustom;
        this.titleSectionHidden = !this.selectedCell.isCustom;
        this.$.assignedCellNameInput.value = this.selectedCell.displayColumnName;
        this.$.cellExpressionInput.value = this.selectedCell.expression;
        this.enableCellActions(this.selectedCell.expressionType == 'boolean');
        this.setCellActions(this.selectedCell);

        /* for margins, checkbox and droplists */
        var width = rovUtils.getStringWidth(maxStr);
        this.$.primaryKeyMenu.$.menuButton.style.width = width + 'px';

        if (this.selectedCell.newCell) {
            var left = this.rovPanel.isNarrow() ?
                       this.offsetLeft + this.offsetWidth :
                       this.offsetLeft + this.offsetWidth + rovUtils.leftPaneWidth;
        }
        else {
            var left = event.x;
        }
        if ((left + width + 100) > document.documentElement.clientWidth) {
            left -= (width + 100);
        }
        var dialog = this.$.cellPropertiesDialog;
        dialog.resetFit();
        dialog.style.left = left + 'px';
        dialog.style.top = event.y + 'px';
        dialog.open();
        dialog.style.zIndex = String(this.rovPanel.getMaxZindex() + 1);
        if (this.$.tableSettingsMenu.selected != undefined) {
            this.$.tableSettingsMenu.selected = -1;
        }
        var pre = this.rovPanel.ccs7 ? './' : '../../';
        this.dialogEvalWorker = new Worker(pre + 'client/evalWorker.js');
        this.dialogEvalWorker.onmessage = this.dialogEvalWorkerMessage;
        this.dialogEvalWorker.msgObj = this;

    },

   /*
    *  ======== cellPropertiesDialogOkClicked ========
    */
    cellPropertiesDialogOkClicked : function (e) {
        if (this.selectedCell.newCell) {
            this.addCustomCell(e);
            return;
        }
        var keyChanged = false;
        if (!this.selectedCell.args && this.selectedCell.viewType != 'MODULE' && !this.selectedCell.isCustom) {
            var label = this.$.primaryKeyMenu.selectedItemLabel ?
                        this.$.primaryKeyMenu.selectedItemLabel :
                        this.$.primaryKeyMenu.label;
            var props = this.getSelectedKeyProperties(label);
            if (this.selectedCell.primaryKeyColumnName != props.key) {
                this.selectedCell.primaryKeyColumnName = props.key;
                this.selectedCell.primaryKeyColumnValue = props.value;
                keyChanged = true;
            }
        }
        var titleChanged = this.selectedCell.displayColumnName != this.$.assignedCellNameInput.value;
        if (titleChanged) {
            for (var i = 0; i < this.rovCells.length; i++) {
                var match = this.selectedCell == this.rovCells[i];
                if (this.selectedCell != this.rovCells[i] &&
                    this.rovCells[i].displayColumnName == this.$.assignedCellNameInput.value) {

                    this.$.cellNameInvalidIcon.hidden = false;
                    this.cellNameErrorDesc = 'Column title must be unique, ' +
                                             this.$.assignedCellNameInput.value + ' is already being used';
                    return;
                }
            }
        }
        var expressionChanged = this.selectedCell.expression != this.$.cellExpressionInput.value;
        if (keyChanged) {
            this.set('cellNamesMenuItems.' + this.selectedCell.index + '.id', props.value + ',' + props.key);
        }
        if (titleChanged) {
            var prevTitle = this.selectedCell.displayColumnName;
            this.set('cellNamesMenuItems.' + this.selectedCell.index + '.label',  this.$.assignedCellNameInput.value);
        }
        if (keyChanged || titleChanged) {
            this.$.cellNamesMenuTemplate.render();
        }
        this.$.cellPropertiesDialog.titleChanged = titleChanged;
        this.$.cellPropertiesDialog.expressionChanged = expressionChanged;
        this.$.cellPropertiesDialog.prevTitle = prevTitle;
        if (this.$.cellExpressionInput.value != '') {
            var expr = this.makeCellExpression(this.$.cellExpressionInput.value,
                                               this.selectedCell.displayColumnName,
                                               this.selectedCell.isCustom);
            if (expr.status) {
                this.$.cellExpressionInvalidIcon.hidden = false;
                this.cellExpressionErrorDesc = expr.status;
                return;
            }
            expr.expression = '"use strict";' + expr.expression;
            this.$.cellPropertiesDialog.expr = expr;
            this.dialogEvalWorker.postMessage([expr.expression, false]);
            return;
        }
        else {
            delete this.selectedCell.expressionType;
            if (!this.selectedCell.isCustom) {
                this.selectedCell.val = this.selectedCell.origVal;
            }
        }
        this.closeCellPropertiesDialog();
    },


   /*
    *  ======== cellTooltip ========
    */
    cellTooltip : function (cell) {
        if (cell.isCustom) {
            var tooltip = cell.expression;
        }
        else {
            var tooltip = 'Module: ' + cell.module;
            if (cell.args) {
                if (cell.view == 'Variable') {
                    tooltip += '\nVariable: ' + cell.firstArg;
                }
                else {
                    tooltip += '\nView: ' + cell.view;
                    var args = cell.viewArgs[cell.argsId].args;
                    for(var j = 0; j < args.length; j++) {
                        tooltip += '\n' + args[j].name + ': ' + args[j].value;
                    }
                }
            }
            else {
                tooltip += '\nView: ' + cell.view;
            }
            tooltip += '\nColumn: ' + cell.selectedColumnName;
            tooltip += '\n' + cell.primaryKeyColumnName + ': ' + cell.primaryKeyColumnValue;
        }
        return (tooltip);
    },

   /*
    *  ======== checkDisplayColumnName ========
    */
    checkDisplayColumnName : function (displayColumnName, cell) {
        var found = false;
        for (var i = 0; i < this.rovCells.length; i++) {
            if (this.rovCells[i].displayColumnName == displayColumnName) {
                found = true;
                break;
            }
        }
        if (found) {
            displayColumnName = cell.primaryKeyColumnValue + '_' + displayColumnName;
        }
        return (displayColumnName);
    },

   /*
    *  ======== clearTableSettingsMenuState ========
    */
    clearTableSettingsMenuState : function () {
        this.$.tableSettingsMenu.selected = -1;
        this.$.cellPropertiesSubMenu.selected = -1;
        this.$.cellNamesMenu.selected = -1;
        this.$.cellNamesMenu.hidden = true;
    },

   /*
    *  ======== closeCellPropertiesDialog ========
    */
    closeCellPropertiesDialog : function () {
        var dialog = this.$.cellPropertiesDialog;
        this.selectedCell.displayColumnName = this.$.assignedCellNameInput.value;
        this.selectedCell.expression = this.$.cellExpressionInput.value;
        if (dialog.titleChanged || dialog.expressionChanged) {
            if (dialog.titleChanged) {
                this.$.tiRovTable.customViewColumnNameChanged(dialog.prevTitle, this.selectedCell.displayColumnName);
            }
            var tableData = this.formatTableData();
            this.$.tiRovTable.displayTable(tableData.data, this.viewName, 'CUSTOM', tableData.columns, true);
        }
        dialog.close();
    },

   /*
    *  ======== closeTableSettingsClicked ========
    */
    closeTableSettingsClicked : function (e) {
        this.$.tableSettingsMenuDialog.close();
    },

   /*
    *  ======== columnsTapped ========
    */
    columnsTapped : function(e) {
        this.$.tiRovTable.columnsContextMenu(e, this.rovPanel.viewContainer, this.rovPanel.isNarrow() ? 0: rovUtils.leftPaneWidth);
    },

   /*
    *  ======== customViewOptionsMenuItemSelected ========
    */
    customViewOptionsMenuItemSelected : function (e) {
        var selected = e.currentTarget.selectedItem.id;
        this.$.customViewOptionsMenu.selected = -1;
        if (selected != 'dividerItem') {
            this.$.customViewOptionsMenuDialog.close();
        }
        if (selected != 'fixedFontItem' && selected != 'dividerItem') {
            this[selected](e); /* id of the item is the function name */
        }
    },

   /*
    *  ======== deleteCell ========
    */
    deleteCell : function (e) {
        var i = Number(e.currentTarget.id.substr(e.currentTarget.id.indexOf('_') + 1));
        var cellNamesMenuItems = [];
        for (var j = 0; j < this.cellNamesMenuItems.length; j++) {
            if (j != i) {
                var k = cellNamesMenuItems.length;
                cellNamesMenuItems.push(this.cellNamesMenuItems[j]);
                cellNamesMenuItems[k].cellId = 'cell_' + k;
            }
        }
        this.set('cellNamesMenuItems', cellNamesMenuItems);
        var selectedName = this.rovCells[i].selectedColumnName;
        if (!this.rovCells[i].isCustom) {
            this.rovPanel.removeCellFromCustomViewData(this, this.rovCells[i]);
        }
        delete this.rovCells[i].val; /* for references */
        this.rovCells.splice(i, 1);
        if (this.rovCells.length == 1) {
            this.set('removeButtonHidden', true);
        }
        var tableData = this.formatTableData();
        this.$.tiRovTable.displayTable(tableData.data, this.viewName, 'CUSTOM', tableData.columns, false);
    },

   /*
    *  ======== detached ========
    */
    detached : function () {
        if (this.tableEvalWorker != null) {
            this.tableEvalWorker.terminate();
            this.tableEvalWorker = null;
        }
    },

   /*
    *  ======== dialogEvalResult ========
    */
    dialogEvalResult : function (data) {
        var val = data[0];
        var status = data[1];
        var newCustomCell = data[2];
        if (status) {
            this.$.cellExpressionInvalidIcon.hidden = false;
            this.cellExpressionErrorDesc = status;
            return;
        }
        if (newCustomCell) {
            var cell = this.$.cellPropertiesDialog.cell;
            var expr = this.$.cellPropertiesDialog.expr;
            cell.expressionType = typeof(val);
            this.newCustomCellCloseCellPropertiesDialog(cell, val);
        }
        else {
            this.selectedCell.expressionType = typeof(val);
            if (this.selectedCell.expressionType == 'boolean') {
                this.getCellActions(this.selectedCell);
            }
            if (this.selectedCell.isCustom) {
                this.selectedCell.val = val;
            }
            else {
                this.selectedCell.val = (this.selectedCell.expressionType != 'boolean') ? val.toFixed(1) : this.selectedCell.origVal;
            }
            this.selectedCell.status = null;
            this.closeCellPropertiesDialog();
        }
    },

   /*
    *  ======== dialogEvalWorkerMessage ========
    */
    dialogEvalWorkerMessage : function (e) {
        this.msgObj.dialogEvalResult(e.data);
    },

   /*
    *  ======== displayTableView ========
    */
    displayTableView : function (e) {
        var i = Number(e.currentTarget.id.substr(e.currentTarget.id.indexOf('_') + 1));
        var cell = this.rovCells[i];
        var rowKeys = {primaryKeyColumnName: cell.primaryKeyColumnName,
                       primaryKeyColumnValue: cell.primaryKeyColumnValue,
                       selectedColumnName: cell.selectedColumnName};
        this.rovPanel.viewFromGraphOrCustom(cell.module, cell.view, cell.viewArgs, rowKeys);
    },

   /*
    *  ======== enableCellActions ========
    */
    enableCellActions : function (enable) {
        this.actionDisabled = !enable;
        this.actionLabelValue = enable ? '' : 'Apply to cell';
    },

   /*
    *  ======== evalCellExpression ========
    */
    evalCellExpression : function (cellIndex) {
        var cell = this.rovCells[cellIndex];
        var value = {val: 0, status: null};
        var expr = cell.expression.trim();

        var decls = this.cellExpressionDecls(cell.displayColumnName);
        var expression = (decls ? decls + expr : expr) + ';';
        this.tableEvalWorker.postMessage([expression, cellIndex]);
    },

   /*
    *  ======== evaluateTableExpressions ========
    */
    evaluateTableExpressions : function (exprMap) {
        this.exprMap = exprMap;
        this.evalCellExpression(exprMap[0]);
    },

   /*
    *  ======== fixedFontChanged ========
    */
    fixedFontChanged : function (e) {
        this.$.tiRovTable.fontChanged(e);
    },

   /*
    *  ======== formatMutiRowTableData ========
    *  just in case...
    */
    formatMutiRowTableData : function () {
        /* columns are always in order of rovCells.selectedColumnName */
        var columns = [];
        for (var i = 0; i < this.rovCells.length; i++) {
            if (columns.indexOf(this.rovCells[i].selectedColumnName) == -1) {
                columns.push(this.rovCells[i].selectedColumnName);
            }
        }

        /* map cells by column name to calc num rows */
        var map = {};
        for (var i = 0; i < columns.length; i++) {
            map[columns[i]] = [];
            for (var j = 0; j < this.rovCells.length; j++) {
                if (this.rovCells[j].selectedColumnName == columns[i]) {
                    map[columns[i]].push(j);
                }
            }
        }

        /* number of rows */
        var numRows = 1;
        for (var i = 0; i < columns.length; i++) {
            if (map[columns[i]].length > numRows) {
                numRows = map[columns[i]].length;
            }
        }

        /* init data */
        var data = [];
        for (var i = 0; i < numRows; i++) {
            data.push({});
        }

        /* populate data */
        for (var i = 0; i < columns.length; i++) {
            for (var j = 0; j < map[columns[i]].length; j++) {
                var rowIndex = j;
                var cellIndex = map[columns[i]][j];
                var cell = this.rovCells[cellIndex];
                data[rowIndex][cell.selectedColumnName] ={};
                data[rowIndex][cell.selectedColumnName].val = cell.val;
                if (cell.status) {
                    data[rowIndex][cell.selectedColumnName].status = cell.status;
                }
                data[rowIndex][cell.selectedColumnName].cellIndex = cellIndex;
                data[rowIndex][cell.selectedColumnName].primaryKeyColumnValue = cell.primaryKeyColumnValue;
                data[rowIndex][cell.selectedColumnName].selectedColumnName = cell.selectedColumnName;
            }
        }

        /* fill in empty cells */
        for (var i = 0; i < data.length; i++) {
            for (var j = 0; j < columns.length; j++) {
                if (data[i][columns[j]] == undefined) {
                    data[i][columns[j]] = {};
                    data[i][columns[j]].val = '';
                    data[i][columns[j]].cellIndex = -1;
                }
            }
        }
        return ({data: data, columns: columns});
    },

   /*
    *  ======== formatTableData ========
    */
    formatTableData : function () {
        var columns = [];
        var data = {};
        for (var i = 0; i < this.rovCells.length; i++) {
            var cell = this.rovCells[i];
            columns.push(cell.displayColumnName);
            data[cell.displayColumnName] = {};
            data[cell.displayColumnName].val = cell.val;
            data[cell.displayColumnName].cellIndex = i;
            data[cell.displayColumnName].displayColumnName = cell.displayColumnName;
            data[cell.displayColumnName].tooltip = this.cellTooltip(cell);
            data[cell.displayColumnName].isCustom = cell.isCustom;
            data[cell.displayColumnName].status = cell.status;
            if (!cell.isCustom) {
                data[cell.displayColumnName].primaryKeyColumnValue = cell.primaryKeyColumnValue;
                data[cell.displayColumnName].selectedColumnName = cell.selectedColumnName;
            }

        }
        return ({data: [data], columns: columns});
    },

   /*
    *  ======== formatVal ========
    */
    formatVal : function (val, format) {
        var valFormat = rovUtils.getFormat(val);
        if (valFormat == format) {
            return (val);
        }
        if (format == 'Decimal') {
            if (valFormat == 'Hex') {
                return (parseInt(val, 16));
            }
            if (valFormat == 'Scientific') {
                return (Number(val));
            }
        }
        else if (format == 'Hex') {
            var retVal = Number(val).toString(16);
            if (retVal == 'NaN') {
                return (val);
            }
            if (retVal.search(/^0x/i) == -1) {
                retVal = '0x' + retVal;
            }
            return (retVal);
        }
        else if (format == 'Scientific') {
            return (Number(val).toExponential());
        }
        return (val);
    },

   /*
    *  ======== getCellActions ========
    */
    getCellActions : function (cell) {
        cell.actions = {
            notify: this.$.notifyActionBox.checked,
            logNotifications: this.$.logNotificationsActionBox.checked,
            stopUpdates: this.$.stopUpdatesActionBox.checked,
            stopAllUpdates: this.$.stopAllUpdatesActionBox.checked,
            saveView: this.$.saveViewActionBox.checked,
            saveAllViews: this.$.saveAllViewsActionBox.checked,
            notifyLogComment:this.$.notifyLogCommentDialog.notifyLogComment
        };
    },

   /*
    *  ======== getCellValue ========
    */
    getCellValue : function (viewType, primaryKeyColumnName, primaryKeyColumnValue, columnName, viewData) {
        var data = null;
        var value = {};
        if (viewType == 'INSTANCE') {
            if (rovUtils.isArray(viewData)) {
                for (var i = 0; i < viewData.length; i++) {
                    if (viewData[i][primaryKeyColumnName] == primaryKeyColumnValue) {
                        data = viewData[i];
                        break;
                    }
                }
            }
            else if (viewData[primaryKeyColumnName] == primaryKeyColumnValue) {
                data = viewData;
            }
        }
        else if (viewType == 'MODULE') {
            data = viewData;
        }
        else if (viewType == 'INSTANCE_DATA' || viewType == 'MODULE_DATA') {
            if (rovUtils.isArray(viewData)) {
            }
            else {
                var elemArr = viewData.elements;
                for (var i = 0; i < elemArr.length; i++) {
                    if (elemArr[i][primaryKeyColumnName] == primaryKeyColumnValue) {
                        data = elemArr[i];
                        break;
                    }
                }
            }
        }
        if (data) {
            value.val = data[columnName];
            if (data[columnName + '$status']) {
                value.status = data[columnName + '$status'];
            }
        }
        else {
            value.val = '';
            value.status = columnName + ' not found for ' + primaryKeyColumnName + ' ' + primaryKeyColumnValue;
        }
        return (value);
    },

   /*
    *  ======== getProperties ========
    */
    getProperties : function () {
        var props = this.behaviors[0].getProperties(this);  /* call super, pass scope */
        props.viewsData = {};
        props.viewsData[this.moduleName + '.' + this.viewName] =
            this.getViewsData()[this.moduleName + '.' + this.viewName];
        props.rovCells = [];
        for (var i = 0; i < this.rovCells.length ; i++) {
            var cell = rovUtils.shallowCopy(this.rovCells[i]);
            props.rovCells.push(cell);
        }
        return (props);
    },

   /*
    *  ======== getSelectedKeyProperties ========
    */
    getSelectedKeyProperties : function (sel) {
        var sepIdx = sel.indexOf(':');
        var props = {key: sel.substr(0, sepIdx), value: sel.substr(sepIdx + 3)};
        return (props);
    },

    /*
     *  ======== getViewCallback ========
     *  Callback from rovData.getView()
     *
     *  Params
     *    error      - null when there is no error; otherwise an error
     *                 message string
     *    viewData   - the ROV view data requested as a JSON object
     *    module     - module name string for the returned view data
     *    view       - view name string for the returned view data
     */
    getViewCallback : function (error, viewData, module, view, handle) {
        /* show the refresh button and hide the refresh spinner */
        if (--this.requests == 0 && this.viewRefresh) {
            this.viewRefresh = false;
        }

        /* handle data acquisition error (if any) */
        if (error != null) {
            this.rovViewStatus(error, 'error');
            return;
        }

        for (var i = 0; i < this.rovCells.length; i++) {
            var cellView = this.rovCells[i].args ? this.rovCells[i].view + this.rovCells[i].args : this.rovCells[i].view;
            if (this.rovCells[i].module == module && cellView == view) {
                this.rovCells[i].viewData = viewData;
            }
        }
        if (this.requests == 0) {
            this.exprCount = 0;
            var exprMap = [];
            for (var i = 0; i < this.rovCells.length; i++) {
                var value = {val: 0, status: null};
                if (!this.rovCells[i].isCustom) {
                    value = this.getCellValue(this.rovCells[i].viewType,
                                 this.rovCells[i].primaryKeyColumnName, this.rovCells[i].primaryKeyColumnValue,
                                 this.rovCells[i].selectedColumnName, this.rovCells[i].viewData);
                    this.rovCells[i].origVal = value.val;
                    this.rovCells[i].val = value.val;
                }
                else if (this.rovCells[i].val == '') {
                    this.rovCells[i].val = 0;
                }
                this.rovCells[i].status = value.status;
                if (this.rovCells[i].expression) {
                    ++this.exprCount;
                    exprMap.push(i);
                }
            }
            if (exprMap.length > 0) {
                this.evaluateTableExpressions(exprMap);
            }
            else {
                this.renderFinalData();
            }
        }
    },

   /*
    *  ======== getViewColumns ========
    */
    getViewColumns : function(module, view) {
        var viewObjs = this.viewList[module].viewTabs ? this.viewList[module].viewTabs : this.viewList[module];
        var viewColumns = null;
        for (var i = 0; i < viewObjs.length; i++) {
            if (viewObjs[i].name == view) {
                viewColumns = viewObjs[i].columns;
                break;
            }
        }
        return (viewColumns);
    },

   /*
    *  ======== getViews ========
    */
    getViews : function(module) {
        var views = [];
        var viewObjs = this.viewList[module].viewTabs ? this.viewList[module].viewTabs : this.viewList[module];
        if (viewObjs) {
            for (var i = 0; i < viewObjs.length; i++) {
                views.push(viewObjs[i].name);
            }
        }
        return (views);
    },

   /*
    *  ======== getViewsData ========
    */
    getViewsData : function () {
        return(this.$.tiRovTable.getViewsData());
    },

   /*
    *  ======== getViewType ========
    */
    getViewType : function(module, view) {
        var viewObjs = this.viewList[module].viewTabs ? this.viewList[module].viewTabs : this.viewList[module];
        var viewType = null;
        for (var i = 0; i < viewObjs.length; i++) {
            if (viewObjs[i].name == view) {
                viewType = viewObjs[i].type;
                break;
            }
        }
        return (viewType);
    },

   /*
    *  ======== isSameModuleView ========
    */
    isSameModuleView : function (cell1, cell2) {
       return(cell1.module == cell2.module && cell1.view == cell2.view &&
              cell1.args == cell2.args);
    },

   /*
    *  ======== makeCellExpression ========
    */
    makeCellExpression : function (rawExp, cellName, isCustom) {
        var expression;
        var status = null;
        var expr = rawExp.trim();
        var pattern = RegExp(cellName);
        if (expr.match(pattern) && isCustom) {
            status = 'Self reference';
        }
        else {
            var decls = this.cellExpressionDecls(cellName);
            expression = decls ? decls + expr + ';' : expr;
        }
        return ({expression: expression, status: status});
    },

   /*
    *  ======== nameCell ========
    */
    nameCell : function (cell) {
        var name;
        if (cell.assignedCellName) {
            name = cell.assignedCellName;
        }
        else if (cell.args) {
            name = (cell.view == 'Variable') ? cell.firstArg : cell.selectedColumnName;
        }
        else {
            name = cell.selectedColumnName;
        }
        return (name);
    },

   /*
    *  ======== newCustomCell ========
    */
    newCustomCell : function (e) {
        this.set('selectedCell', {newCell: true, isCustom: true, displayColumnName: this.newCustomCellName(), expression: ''});
        this.cellPropertiesDialog(e);
    },

   /*
    *  ======== newCustomCellCloseCellPropertiesDialog ========
    */
    newCustomCellCloseCellPropertiesDialog : function (cell, val) {
        cell.val = (cell.expressionType == 'boolean') ? String(val) : val.toFixed(1);
        cell.origVal = '';
        cell.expression = this.$.cellExpressionInput.value;
        this.rovCells.push(cell);
        if (cell.expressionType == 'boolean') {
            this.getCellActions(cell);
        }
        this.set('removeButtonHidden', this.rovCells.length == 1);
        var tableData = this.formatTableData();
        this.$.tiRovTable.displayTable(tableData.data, this.viewName, 'CUSTOM', tableData.columns, true);
        this.$.cellPropertiesDialog.close();
    },

   /*
    *  ======== newCustomCellName ========
    */
    newCustomCellName : function () {
        return ('column_' + (this.rovCells.length + 1));
    },

   /*
    *  ======== notifyLogCommentClicked ========
    */
    notifyLogCommentClicked : function (e) {
        var dialog = this.$.notifyLogCommentDialog;
        this.$.notifyLogCommentText.value = dialog.notifyLogComment;
        dialog.resetFit();
        var top = Number(this.$.cellPropertiesDialog.style.top.replace(/px/, '')) + 16;
        dialog.style.top = top + 'px';
        dialog.style.left = (e.x - 16) + 'px';
        dialog.open();
    },

   /*
    *  ======== notifyLogCommentDialogCloseClicked ========
    */
    notifyLogCommentDialogCloseClicked : function (e) {
        this.$.notifyLogCommentDialog.close();
    },


   /*
    *  ======== notifyLogCommentKeyPress ========
    */
    notifyLogCommentKeyPress : function (e) {
        var keyCode = e.keyCode || e.which;
        if (keyCode === 13) {
            this.notifyLogCommentOkClicked(e);
        }
    },

   /*
    *  ======== notifyLogCommentOkClicked ========
    */
    notifyLogCommentOkClicked : function (e) {
        this.$.notifyLogCommentDialog.notifyLogComment = this.$.notifyLogCommentText.value;
        this.$.notifyLogCommentDialog.close();
    },

   /*
    *  ======== onCellPropertiesDialogClosed ========
    */
    onCellPropertiesDialogClosed : function (e) {
        if (e.target.id == 'cellPropertiesDialog') {
            this.set('primaryKeyMenuItems', []);
            this.set('primaryKeyMenuLabel', '');
            this.$.primaryKeyMenu._setSelectedItem(null);
            if (this.dialogEvalWorker != null) {
                this.dialogEvalWorker.terminate();
                this.dialogEvalWorker = null;
            }
        }
    },

   /*
    *  ======== onPrimaryKeySelected ========
    */
    onPrimaryKeySelected : function (e) {
        if (e.currentTarget.selectedItemLabel) {
            if (this.$.tableSettingsMenu.selected != undefined) {
                this.$.tableSettingsMenu.selected = -1;
            }
        }
    },

    /*
     *  ======== onRefresh ========
     *  Called when user clicks the 'Refresh' button on this view,
     *  or 'Refresh all' or 'Repeat refresh' buttons on the ROV toolbar
     */
    onRefresh : function (refreshArg) {
        if (this.exprCount > 0) {
            return;  /* not done with previous */
        }
        /*
         *  Request CPU load from an ROV view.
         *
         *  This is an asynchronous call; getViewCallback() will be called
         *  with the result in its viewData parameter.
         */
        var requests = [];
        for (var i = 0; i < this.rovCells.length; i++) {
            if (!this.rovCells[i].isCustom) {
                var view = this.rovCells[i].args ? this.rovCells[i].view + this.rovCells[i].args : this.rovCells[i].view;
                if (!requests[this.rovCells[i].module + '.' + view]) {
                    requests[this.rovCells[i].module + '.' + view] = true;
                    if (this.rovData.getView(this.rovCells[i].module, view, this)) {
                        ++this.requests;
                    }
                }
            }
        }
        this.dataRefresh = refreshArg != true;
        this.viewRefresh = this.requests > 0;
    },

   /*
    *  ======== onSaveView ========
    *  Called when user clicks the 'Download' button on this view,
    *  or the 'Download all' button on the ROV toolbar
    */
    onSaveView: function () {
        /* return the graph's current data set */
        return (this.rows);
    },

   /*
    *  ======== ready ========
    */
    ready : function () {
        this.$.tiRovTable.setIsGlobal(false);
        this.$.tiRovTable.setNotifyObj(this);
    },

   /*
    *  ======== renderFinalData ========
    */
    renderFinalData : function () {
        var status = [];
        var log = [];
        var saveData = false;
        var saveAllData = false;
        var stopUpdates = false;
        var stopAllUpdates = false;
        for (var i = 0; i < this.rovCells.length; i++) {
            var value = {val: this.rovCells[i].val, status: this.rovCells[i].status};
            if (this.rovCells[i].expression) {
                if (this.rovCells[i].expressionType == 'boolean') {
                    if (value.val == 'true') {
                        if (this.rovCells[i].actions.notify) {
                            status.push(this.rovCells[i].expression);
                        }
                        if (this.rovCells[i].actions.logNotifications) {
                            var lessYesteday = Date.now() - this.yesterday;
                            var logStr = lessYesteday + ': ' + this.rovCells[i].expression;
                            if (this.rovCells[i].actions.notifyLogComment) {
                                logStr += '; '  + this.rovCells[i].actions.notifyLogComment;
                            }
                            log.push(logStr);
                            if (!this.$.logViewerDialog.hidden && this.$.exportLogButton.disabled) {
                                this.$.exportLogButton.disabled = false;
                                this.$.clearLogButton.disabled = false;
                            }
                        }
                        if (this.rovCells[i].actions.stopUpdates && this.rovPanel.inRepeatRefresh) {
                            stopUpdates = true;
                            this.repeatRefreshEnabled = false;
                            this.repeatCounter = 0;
                        }
                        if (this.rovCells[i].actions.stopAllUpdates && this.rovPanel.inRepeatRefresh) {
                            stopAllUpdates = true;
                            this.rovPanel.stopRepeatRefresh(true);
                        }
                        if (this.rovCells[i].actions.saveView) {
                            saveData = true;
                        }
                        if (this.rovCells[i].actions.saveAllViews) {
                            saveAllData = true;
                        }
                    }
                    if (!this.rovCells[i].isCustom) {
                        value.val = this.rovCells[i].origVal;
                    }
                }
            }
            if (this.rovCells[i].format) {
                this.rovCells[i].val = this.formatVal(value.val, this.rovCells[i].format);
                this.rovCells[i].format = null; /* only used for initial refresh, so table can get it */
            }
            else {
                this.rovCells[i].val = value.val;
            }
            this.rovCells[i].status = value.status;
        }
        if (log.length > 0) {
            this.notifyLog = this.notifyLog.concat(log);
            if (this.notifyLog.length > this.logMax) {
                while (this.notifyLog.length > this.logMax) {
                    this.notifyLog.pop();
                }
            }
            if (this.$.logViewerDialog.hidden == false) {
                this.$.logViewerText.value = this.notifyLog.join('\n');
                this.$.logViewerText.scrollTop = this.$.logViewerText.scrollHeight;
            }
        }
        if (stopAllUpdates) {
            status.push('All view updates stopped');
        }
        else if (stopUpdates) {
            status.push('View updates stopped');
        }
        if (saveData) {
            this.behaviors[0].onSave(null, this);  /* call super, pass scope */
            status.push('View data saved');
        }
        if (saveAllData) {
            this.rovPanel.saveAllViews();
            status.push('All views data saved');
        }
        if (status.length > 0) {
            for (var i = 0; i < status.length; i++) {
                this.rovViewStatus(status[i], 'info');
            }
        }
        var tableData = this.formatTableData();
        this.viewIsShowing = this.$.tiRovTable.displayTable(tableData.data, this.viewName, 'CUSTOM', tableData.columns, this.dataRefresh);
        if (this.viewIsShowing && this.highlightCell) {
            this.$.tiRovTable.highlightCell(this.highlightCell);
            this.highlightCell = null;
        }
    },

    /*
     *  ======== resizerClicked ========
     *  Called on a bottom corner on-mousedown event
     *
     *  See ti-rov-graph.html resizer div.
     */
    resizerClicked: function (e) {
        /*
         *  call polymerUI/src/rovUtils.js initRovResize(), passing the
         *  viewPaperCard element that is being resized. Also pass callback
         *  object in order to be notified via this.resized() when the element
         *  is done being is resized, so we can make any required size related
         *  changes to the graph itself
         */
        rovUtils.initRovResize(e, this.$.viewPaperCard);
    },

   /*
    *  ======== rovCustomViewDialogKeyPress ========
    */
    rovCustomViewDialogKeyPress : function (e) {
        var keyCode = e.keyCode || e.which;
        if (keyCode === 13) {
            var dlgId = e.currentTarget.id;
            this[dlgId + 'OkClicked'](e);
        }
    },

   /*
    *  ======== setCellActions ========
    */
    setCellActions : function (cell) {
        var actions = cell.actions;
        if (actions && Object.keys(actions).length > 0) {
            this.$.notifyActionBox.checked = actions.notify;
            this.$.logNotificationsActionBox.checked = actions.logNotifications;
            this.$.stopUpdatesActionBox.checked = actions.stopUpdates;
            this.$.stopAllUpdatesActionBox.checked = actions.stopAllUpdates;
            this.$.saveViewActionBox.checked = actions.saveView;
            this.$.saveAllViewsActionBox.checked = actions.saveAllViews;
            this.$.notifyLogCommentButton.disabled = !this.$.logNotificationsActionBox.checked;
            this.$.notifyLogCommentDialog.notifyLogComment = actions.notifyLogComment ? actions.notifyLogComment : '';
        }
        else {
            this.$.notifyActionBox.checked = true;
            this.$.logNotificationsActionBox.checked = false;
            this.$.stopUpdatesActionBox.checked = false;
            this.$.stopAllUpdatesActionBox.checked = false;
            this.$.saveViewActionBox.checked = false;
            this.$.saveAllViewsActionBox.checked = false;
            this.$.notifyLogCommentButton.disabled = true;
            this.$.notifyLogCommentDialog.notifyLogComment = '';
        }
    },

   /*
    *  ======== tableCellContextMenu ========
    */
    tableCellContextMenu : function (eData) {
        var cell = this.rovCells[eData.cellIndex];
        var tableCells = [];
        for (var i = 0; i < this.rovCells.length; i++) {
            tableCells.push(rovUtils.shallowCopy(this.rovCells[i]));
        }
        if (cell.isCustom) {
            var dataProps = {isCustom: true,
                             event: eData.event,
                             viewName: this.viewName,
                             displayColumnName: cell.displayColumnName,
                             val: cell.val,
                             origVal: cell.origVal,
                             viewType: cell.viewType,
                             expression: cell.expression,
                             expressionType: cell.expressionType,
                             actions: cell.actions,
                             customTable: true,
                             tableCells: tableCells
            };
        }
        else {
            var dataProps = {module: cell.module,
                             view: cell.view,
                             viewName: this.viewName,
                             displayColumnName: cell.displayColumnName,
                             selectedColumnName: cell.selectedColumnName,
                             selectedColumnValue: cell.selectedColumnValue,
                             primaryKeyColumnName: cell.primaryKeyColumnName,
                             primaryKeyColumnValue: cell.primaryKeyColumnValue,
                             event: eData.event,
                             args: cell.args,
                             viewArgs: cell.viewArgs,
                             argsId: cell.argsId,
                             viewType: cell.viewType,
                             expression: cell.expression,
                             expressionType: cell.expressionType,
                             actions: cell.actions,
                             customTable: true,
                             tableCells: tableCells
            };
        }
        this.rovPanel.plotViewDataContextMenu(dataProps);
    },

   /*
    *  ======== tableEvalResult ========
    */
    tableEvalResult : function (data) {
        var value = {val: data[0], status: data[1]};
        var cell = this.rovCells[data[2]];
        if (!value.status) {
            cell.val = (cell.expressionType == 'boolean') ? String(value.val) : (value.val).toFixed(1);
        }
        cell.status = value.status;

        /* as long as this.exprCount > 0, no new refreshes are allowed.
           hold onto it as long as possible
         */
        var count = this.exprCount - 1;
        if (count == 0) {
            this.renderFinalData();
            this.exprCount = 0;
        }
        else {
            var exprIdx = this.exprMap.length - (--this.exprCount);
            this.evalCellExpression(this.exprMap[exprIdx]);
        }
    },

   /*
    *  ======== tableEvalWorkerMessage ========
    */
    tableEvalWorkerMessage : function (e) {
        this.msgObj.tableEvalResult(e.data);
    },

   /*
    *  ======== tableNameClicked ========
    */
    tableNameClicked : function (e) {
        var dialog = this.$.tableNameDialog;
        dialog.resetFit();
        var left = e.x;
        if ((left + 225) > document.documentElement.clientWidth) {
            left = document.documentElement.clientWidth - 225;
        }
        dialog.style.left = left + 'px';
        dialog.style.top = e.y + 'px';
        dialog.open();
        dialog.style.zIndex = String(this.rovPanel.getMaxZindex() + 1);
        this.$.tableNameInput.value = this.tableName;
    },

   /*
    *  ======== tableNameDialogOkClicked ========
    */
    tableNameDialogOkClicked : function (e) {
        if (this.$.tableNameInput.value != this.tableName) {
            var prevName = this.tableName;
            this.tableName = this.$.tableNameInput.value;
            this.viewName = this.tableName;
            this.$.tiRovTable.viewNameChanged(this.viewName);
            this.rovPanel.customViewNameChanged(prevName, this.viewName);
        }
        this.$.tableNameDialog.close();
    },

   /*
    *  ======== tableSettingsClicked ========
    */
    tableSettingsClicked : function (e) {
        /* Init cell props submenu */
        var cellNamesMenuItems = [];
        for (var i = 0; i < this.rovCells.length; i++) {
            var item = {};
            item.id = this.rovCells[i].primaryKeyColumnValue + ',' + this.rovCells[i].selectedColumnName;
            item.label = this.rovCells[i].displayColumnName;
            item.cellId = 'cell_' + i;
            item.isCustom = this.rovCells[i].isCustom == true;
            if (item.isCustom || (!item.isCustom && !this.rovCells[i].args &&
                this.rovCells[i].viewType != 'MODULE')) {
                item.cursor = 'pointer';
            }
            else {
                item.cursor = 'default';
            }
            if (!item.isCustom) {
                var cell = this.rovCells[i];
                var titleStr = 'Module: ' + cell.module;
                if (cell.args) {
                    if (cell.view == 'Variable') {
                        titleStr += '\nVariable: ' + cell.firstArg;
                    }
                    else {
                        titleStr += '\nView: ' + cell.view;
                        var args = cell.viewArgs[cell.argsId].args;
                        for(var j = 0; j < args.length; j++) {
                            titleStr += '\n' + args[j].name + ': ' + args[j].value;
                        }
                    }
                }
                else {
                    titleStr += '\nView: ' + cell.view;
                }
                item.title = titleStr;
            }
            cellNamesMenuItems.push(item);
        }
        this.set('cellNamesMenuItems', cellNamesMenuItems);

        var dialog = this.$.tableSettingsMenuDialog;
        dialog.resetFit();
        this.$.tableSettingsMenu.selected = -1;
        var left = e.x;
        var width = 180; /* ballpark from styling in html */
        if ((left + 200) > document.documentElement.clientWidth) {
            left = document.documentElement.clientWidth - 200;
        }
        dialog.style.left = left + 'px';
        var height = 30 + 10;
        var top = e.y - this.$.closeButton.offsetHeight;
        if (top + height > this.rovPanel.viewContainer.clientHeight) {
          top = top - height;
          if (top < 0) {
              top = 0;
          }
        }
        dialog.style.top = top + 'px';
        dialog.style.minWidth = width + 'px';
        this.clearTableSettingsMenuState();
        dialog.open();
        dialog.style.zIndex = String(this.rovPanel.getMaxZindex() + 1);
    },

   /*
    *  ======== tableSettingsMenuItemClicked ========
    */
    tableSettingsMenuItemClicked : function (e) {
        var selected = e.currentTarget.id;
        this.$.tableSettingsMenu.selected = -1;
        if (selected && selected.indexOf('Clicked') > 0) {
            this.clearTableSettingsMenuState();
            this[selected](e); /* id of the item is the function name */
        }
    }

});
