Polymer({is: "ti-rov-graph",
    behaviors: [
      rovBehaviors.viewBehaviors,
      rovBehaviors.notifyLogBehaviors,
      rovBehaviors.viewStatusBehaviors
    ],

    properties: {

        rows: {        /* our copy of the values being plotted */
            type:  Array,
            value: function() { return [];}
        },

        viewName: {
            type:  String,
            value: ''
        },

        viewRefresh: { /* recommended view state indicator */
            type:  Boolean,
            value: false
        },

        sample: {       /* sample number */
            type:  Number,
            value: 0
        },

        /* threshold for removing rows from the chart */
        MAX_ROWS: {     /* max number of rows to display */
            type:  Number,
            value: 2048
        },

        plJsTraces : {       /*  plotly line traces */
            type: Array,
            value: null
        },

        layout : {      /*  plotly layout */
            type: Object,
            value: null
        },

        VISIBLE_ROWS: { /* Visible x axis */
            type:  Number,
            value: 101
        },

        viewList : {
            type: Object,
            value: {}
        },

        maxValue : {    /* track y axis max to update range accordingly */
            type:  Number,
            value: 1000
        },

        minValue : {    /* track y axis min to update range accordingly */
            type:  Number,
            value: 0
        },

        graphName: {
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

        requestErrors: {
            type: Array,
            value: []
        },

        rovTraces : {
            type: Array,
            value: []
        },

        removeButtonHidden: {
            type: Boolean,
            value: false
        },

        selectedTrace: {
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

        xAxisLabel: {
            type: String,
            value: 'Sample Number'
        },

        yAxisLabel: {
            type: String,
            value: ''
        },

        graphWidth: {
            type:  Number,
            value: 390
        },
        graphHeight: {
            type:  Number,
            value: 264
        },

        primaryKeySectionHidden: {
            type: Boolean,
            value: false
        },

        persistProperties: {
            type: Array,
            value: ['graphName', 'rovTraces', 'xAxisLabel', 'yAxisLabel',
                    'graphWidth', 'graphHeight', 'VISIBLE_ROWS',
                    'logMax', 'logWrap', 'modeBarVisible']
        },

        evalWorker : {
            type: Object,
            value: null
        },

        exprCount : {
          type : Number,
          value : 0
        },

        openLogViewerHidden: {
            type: Boolean,
            value: false
        },
        plotConfigModeBar : {
            type: Object,
            value : {modeBarButtonsToRemove: ['hoverCompareCartesian', 'hoverClosest3d', 'toggleSpikelines', 'hoverClosestCartesian'],
                     scrollZoom: true, displaylogo: false}
        },
        plotConfigNoModeBar : {
            type: Object,
            value: {displayModeBar: false, scrollZoom: true}
        },
        modeBarVisible : {
            type: Boolean,
            value: false
        }
    },

/*
    rovTrace = {module,
                view,
                args,
                viewArgs,
                argsId,
                viewType,
                firstArg,
                selectedColumnName,
                primaryKeyColumnName,
                primaryKeyColumnValue,
                primaryKeyColumns,
                assignedTraceName,
                viewData,
                columns

    }
*/


    addTrace : function (newTrace) {
        if (this.$.chartOptionsMenuDialog.opened) {
            this.$.chartOptionsMenuDialog.close();
        }
        if (this.sameModuleView) {
            this.sameModuleView = this.isSameModuleView(this.rovTraces[0], newTrace);
        }
        this.rows.push({x:[], y:[]});

        var trace = {};
        if (newTrace.isCustom) {
            trace.isCustom = true;
            trace.viewName = newTrace.viewName;
            trace.viewType = newTrace.viewType;
            trace.customTable = true;
            trace.displayColumnName = newTrace.displayColumnName;
            trace.expressionType = newTrace.expressionType,
            trace.expression = newTrace.expression;
            trace.actions = newTrace.actions;
            trace.tableCells = newTrace.tableCells;
        }
        else {
            trace.args = newTrace.args;
            trace.module = newTrace.module;
            trace.view = newTrace.view;
            trace.viewType = newTrace.viewType ? newTrace.viewType : this.getViewType(trace.module, trace.view);
            trace.customTable = newTrace.customTable;
            if (trace.customTable) {
                trace.viewName = newTrace.viewName;
                trace.displayColumnName = newTrace.displayColumnName;
                trace.tableCells = newTrace.tableCells;
                if (newTrace.expression) {
                    trace.expression = newTrace.expression;
                    trace.expressionType = newTrace.expressionType;
                    trace.actions = newTrace.actions;
                }
            }
            trace.viewArgs = rovUtils.shallowCopy(newTrace.viewArgs);
            trace.argsId = newTrace.argsId;
            trace.selectedColumnName = newTrace.selectedColumnName;
            trace.primaryKeyColumnName = newTrace.primaryKeyColumnName;
            trace.primaryKeyColumnValue = newTrace.primaryKeyColumnValue;
            if (this.sameModuleView && this.rovTraces[0].viewData) {
                trace.viewData = this.rovTraces[0].viewData;
            }
            if (trace.args) {
                trace.firstArg = trace.args.substr(1, trace.args.indexOf(',') - 1);
            }
            else if (trace.viewType != 'MODULE') {
                trace.columns = this.getViewColumns(trace.module, trace.view);
                trace.primaryKeyColumns = [];
                for (var i = 0; i < trace.columns.length; i++) {
                    if (trace.columns[i] != trace.selectedColumnName) {
                        trace.primaryKeyColumns.push(trace.columns[i]);
                    }
                }
            }
        }
        this.rovTraces.push(trace);
        this.set('removeButtonHidden', this.rovTraces.length == 1);
        var name = this.traceName(trace);
        this.plJsTraces.push(this.newTrace(name));
        Plotly.newPlot(this.$.graph, this.plJsTraces, this.layout,
                       this.modeBarVisible ? this.plotConfigModeBar : this.plotConfigNoModeBar);
        this.openLogViewerHidden = !this.hasLogging();
    },

    attached: function () {
        if (this.plJsTraces != null) {
            return;
        }
        this.yesterday = Date.now() - 900000;
        this.rovTraces = [];
        this.viewList = this.rovPanel.getViewList();
        this.viewName = this.graphName;
        for (var i = 0; i < this.plotTraces.length; i++) {
            var trace = {};
            if (this.plotTraces[i].isCustom) {
                trace.isCustom = true;
                trace.customTable = true;
                trace.viewName = this.plotTraces[i].viewName;
                trace.viewType = this.plotTraces[i].viewType;
                trace.displayColumnName = this.plotTraces[i].displayColumnName;
                trace.assignedTraceName = this.plotTraces[i].assignedTraceName;
                trace.expressionType = this.plotTraces[i].expressionType;
                trace.expression = this.plotTraces[i].expression;
                trace.actions = this.plotTraces[i].actions;
                trace.tableCells = this.plotTraces[i].tableCells;
            }
            else {
                trace.args = this.plotTraces[i].args;
                trace.module = this.plotTraces[i].module;
                trace.view = this.plotTraces[i].view;
                trace.viewType = this.plotTraces[i].viewType ? this.plotTraces[i].viewType : this.getViewType(trace.module, trace.view);
                trace.customTable = this.plotTraces[i].customTable;
                if (trace.customTable) {
                    trace.viewName = this.plotTraces[i].viewName;
                    trace.displayColumnName = this.plotTraces[i].displayColumnName;
                    trace.tableCells = this.plotTraces[i].tableCells;
                    if (this.plotTraces[i].expression) {
                        trace.expression = this.plotTraces[i].expression;
                        trace.expressionType = this.plotTraces[i].expressionType;
                        trace.actions = this.plotTraces[i].actions;
                    }
                }
                trace.viewArgs = rovUtils.shallowCopy(this.plotTraces[i].viewArgs);
                trace.argsId = this.plotTraces[i].argsId;
                trace.selectedColumnName = this.plotTraces[i].selectedColumnName;
                trace.primaryKeyColumnName = this.plotTraces[i].primaryKeyColumnName;
                trace.primaryKeyColumnValue = this.plotTraces[i].primaryKeyColumnValue;
                trace.assignedTraceName = this.plotTraces[i].assignedTraceName;
                if (trace.args) {
                    trace.firstArg = trace.args.substr(1, trace.args.indexOf(',') - 1);
                }
                else if (trace.viewType != 'MODULE') {
                    trace.columns = this.getViewColumns(trace.module, trace.view);
                    trace.primaryKeyColumns = [];
                    for (var j = 0; j < trace.columns.length; j++) {
                        if (trace.columns[j] != trace.selectedColumnName) {
                            trace.primaryKeyColumns.push(trace.columns[j]);
                        }
                    }
                }
            }
            this.rovTraces.push(trace);
        }
        this.set('removeButtonHidden', this.rovTraces.length == 1);
        var graph = this.$.graph;

        graph.style.width = this.graphWidth + 'px';
        graph.style.height = this.graphHeight + 'px';

        /* initialize all our state */
        this.sameModuleView = true;
        for (var i = 0; i < this.rovTraces.length - 1; i++) {
            for (j = i + 1; j < this.rovTraces.length; j++) {
                if (!this.isSameModuleView(this.rovTraces[i], this.rovTraces[j])) {
                    this.sameModuleView = false;
                    break;
                }
            }
            if (!this.sameModuleView) {
                break;
            }
        }
        this.set('rows', []);
        this.set('plJsTraces', []);
        var name;
        for (var i = 0; i < this.rovTraces.length; i++) {
            this.rows.push({x:[], y:[]});
            name = this.traceName(this.rovTraces[i]);
            this.plJsTraces.push(this.newTrace(name));
        }
        this.layout = {
          xaxis: {
            title: this.xAxisLabel,
            showgrid: true,
            type: 'linear',
            range: [0, this.VISIBLE_ROWS - 1],
            tickmode: 'auto',
            nticks: 11,
            tickfont: {size: 11},
            titlefont: {size: 11},
            showline: true,
            autorange: false
          },
          yaxis: {
            title: this.yAxisLabel,
            type: 'linear',
            range: [0, this.maxValue],
            tickmode: 'auto',
            nticks: 6,
            tickfont: {size: 11},
            titlefont: {size: 11},
            showline: true,
            zeroline: false,
            autorange: true
          },
          width: this.graphWidth,
          height: this.graphHeight,
          autosize: true,
          margin: {
            l: 60,
            r: 20,
            b: 50,
            t: 10
          },
          showlegend: true,
          legend: {
            x: 0,
            y: 1,
            'orientation': 'h',
            'traceorder': 'normal',
            'yanchor': 'bottom',
            'xanchor': 'left',
            font: {size: 11}
          }
        };

        Plotly.newPlot(graph, this.plJsTraces, this.layout,
                       this.modeBarVisible ? this.plotConfigModeBar : this.plotConfigNoModeBar);
        var pre = this.rovPanel.ccs7 ? './' : '../../';
        this.evalWorker = new Worker(pre + 'client/evalWorker.js');
        this.evalWorker.onmessage = this.evalWorkerMessage;
        this.evalWorker.msgObj = this;
        this.openLogViewerHidden = !this.hasLogging();
        this.onRefresh();

        var tiRovGraph = this;
        graph.on('plotly_relayout', function(eData) {
            tiRovGraph.relayoutEvent(eData);
        });

        this.$.showModeBarCheckbox.checked = this.modeBarVisible;
    },

    axesLabelsClicked : function (e) {
        var dialog = this.$.axesLabelsDialog;
        dialog.resetFit();
        var left = e.x;
        if ((left + 225) > document.documentElement.clientWidth) {
            left = document.documentElement.clientWidth - 225;
        }
        dialog.style.left = left + 'px';
        dialog.style.top = e.y + 'px';
        dialog.open();
        dialog.style.zIndex = String(this.rovPanel.getMaxZindex() + 1);
        if (this.$.chartOptionsMenu.selected != undefined) {
            this.$.chartOptionsMenu.selected = -1;
        }
    },

    axesLabelsDialogOkClicked : function (e) {
        var update = false;
        if (this.$.xAxisInput.value != this.xAxisLabel) {
            this.xAxisLabel = this.$.xAxisInput.value;
            this.layout.xaxis.title = this.xAxisLabel;
            update = true;
        }
        if (this.$.yAxisInput.value != this.yAxisLabel) {
            this.yAxisLabel = this.$.yAxisInput.value;
            this.layout.yaxis.title = this.yAxisLabel;
            update = true;
        }
        if (update) {
            Plotly.update(this.$.graph, this.plJsTraces, this.layout);
        }
        this.$.axesLabelsDialog.close();
    },

    chartOptionsMenuItemClicked : function (e) {
        var selected = e.currentTarget.id;
        this.$.chartOptionsMenu.selected = -1;
        if (selected && selected.indexOf('Clicked') > 0) {
            this.clearChartSettingsMenuState();
            this[selected](e); /* id of the item is the function name */
        }
    },

    chartSettingsClicked : function (e) {
        /* Init trace props submenu */
        var traceNamesMenuItems = [];
        var fullData = this.$.graph._fullData;
        for (var i = 0; i < fullData.length; i++) {
            var item = {};
            item.id = fullData[i].name;
            var color = fullData[i].line.color;
            item.style = 'background-color:' + color + ';border-color:' + color + ';cursor:pointer;';
            item.label = fullData[i].name.replace(/<br>/, ' ');
            item.traceId = 'trace_' + i;
            var trace = this.rovTraces[i];
            item.title = this.traceTooltip(trace);
            traceNamesMenuItems.push(item);
        }
        this.set('traceNamesMenuItems', traceNamesMenuItems);

        var dialog = this.$.chartOptionsMenuDialog;
        dialog.resetFit();
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
        this.clearChartSettingsMenuState();
        dialog.open();
        dialog.style.zIndex = String(this.rovPanel.getMaxZindex() + 1);
    },

    clearChartSettingsMenuState : function () {
        this.$.chartOptionsMenu.selected = -1;
        this.$.tracePropertiesSubMenu.selected = -1;
        this.$.traceNamesMenu.selected = -1;
        this.$.traceNamesMenu.hidden = true;
    },

    closeChartOptionsClicked : function (e) {
        this.$.chartOptionsMenuDialog.close();
    },

    customViewOptionsMenuItemSelected : function (e) {
        var selected = e.currentTarget.selectedItem.id;
        this.$.customViewOptionsMenu.selected = -1;
        if (selected != 'dividerItem') {
            this.$.customViewOptionsMenuDialog.close();
        }
        if (selected != 'dividerItem') {
            this[selected](e); /* id of the item is the function name */
        }
    },

    deleteTrace : function (e) {
        var i = Number(e.currentTarget.id.substr(e.currentTarget.id.indexOf('_') + 1));
        var traceNamesMenuItems = [];
        for (var j = 0; j < this.traceNamesMenuItems.length; j++) {
            if (j != i) {
                var k = traceNamesMenuItems.length;
                traceNamesMenuItems.push(this.traceNamesMenuItems[j]);
                traceNamesMenuItems[k].traceId = 'trace_' + k;
            }
        }
        this.set('traceNamesMenuItems', traceNamesMenuItems);
        this.rovPanel.removeTraceFromPlotViewData(this, this.rovTraces[i]);
        Plotly.deleteTraces(this.$.graph, i);
        this.rows.splice(i, 1);
        this.rovTraces.splice(i, 1);
        if (this.rovTraces.length == 1) {
            this.set('removeButtonHidden', true);
        }
        this.openLogViewerHidden = !this.hasLogging();
    },

    detached : function () {
        if (this.evalWorker != null) {
            this.evalWorker.terminate();
            this.evalWorker = null;
        }
    },

    displayTableView : function (e) {
        var i = Number(e.currentTarget.id.substr(e.currentTarget.id.indexOf('_') + 1));
        var trace = this.rovTraces[i];
        if (trace.customTable) {
            this.rovPanel.customViewFromGraph(trace);
            return;
        }
        else {
            var rowKeys = {primaryKeyColumnName: trace.primaryKeyColumnName,
                           primaryKeyColumnValue: trace.primaryKeyColumnValue,
                           selectedColumnName: trace.selectedColumnName};
            this.rovPanel.viewFromGraphOrCustom(trace.module, trace.view,
                                                trace.viewArgs, rowKeys);
        }
    },

    evalExpression : function (item, itemIdx) {
        var decls = [];
        for (var i = 0; i < item.declCells.length; i++) {
            if (item.declCells[i].val !== undefined && item.displayColumnName != item.declCells[i].displayColumnName) {
                var decl = 'var ' + item.declCells[i].displayColumnName + '=' + item.declCells[i].val;
                decls.push(decl);
            }
        }
        decls = decls.join(';') + ';';
        if (item.declCells) {
            item.declCells = null;
        }
        this.evalWorker.postMessage([decls + item.expression.trim() + ';', itemIdx]);
    },

    evalResult : function (data) {
        var value = {val: data[0], status: data[1]};
        var item = this.allExpr[data[2]];
        item.val = String(value.val);
        item.status = value.status;

        /* as long as this.exprCount > 0, no new refreshes are allowed.
           hold onto it as long as possible
         */
        var count = this.exprCount - 1;
        if (count == 0) {
            /* Retrieve expression values from table cells */
            for (var i = 0; i < this.rovTraces.length; i++) {
                if (this.rovTraces[i].expression) {
                    var tableCells = this.rovTraces[i].tableCells;
                    for (var j = 0; j < tableCells.length; j++) {
                        if (this.rovTraces[i].displayColumnName == tableCells[j].displayColumnName) {
                            this.rovTraces[i].val = tableCells[j].val;
                            this.rovTraces[i].status = tableCells[j].status;
                            break;
                        }
                    }
                }
            }
            var vals = [];
            var status = [];
            var log = [];
            var saveData = false;
            var saveAllData = false;
            var stopUpdates = false;
            var stopAllUpdates = false;
            for (var i = 0; i < this.rovTraces.length; i++) {
                if (this.rovTraces[i].expression && this.rovTraces[i].expressionType == 'boolean') {
                    var val = this.rovTraces[i].origVal;
                    if (this.rovTraces[i].val == 'true') {
                        if (this.rovTraces[i].actions.notify) {
                            status.push(this.rovTraces[i].expression);
                        }
                        if (this.rovTraces[i].actions.logNotifications) {
                            var lessYesteday = Date.now() - this.yesterday;
                            var logStr = lessYesteday + ': ' + this.rovTraces[i].expression;
                            if (this.rovTraces[i].actions.notifyLogComment) {
                                logStr += '; '  + this.rovTraces[i].actions.notifyLogComment;
                            }
                            log.push(logStr);
                            if (!this.$.logViewerDialog.hidden && this.$.exportLogButton.disabled) {
                                this.$.exportLogButton.disabled = false;
                                this.$.clearLogButton.disabled = false;
                            }
                        }
                        if (this.rovTraces[i].actions.stopUpdates && this.rovPanel.inRepeatRefresh) {
                            stopUpdates = true;
                            this.stopUpdates = true;
                        }
                        if (this.rovTraces[i].actions.stopAllUpdates && this.rovPanel.inRepeatRefresh) {
                            stopAllUpdates = true;
                            this.rovPanel.stopRepeatRefresh(true);
                        }
                        if (this.rovTraces[i].actions.saveView) {
                            saveData = true;
                        }
                        if (this.rovTraces[i].actions.saveAllViews) {
                            saveAllData = true;
                        }
                    }
                }
                else {
                    val = this.rovTraces[i].val;
                }
                vals.push(Number(val));
            }
            this.plotNewData(vals);
            this.exprCount = 0;
            if (log.length > 0) {
                this.notifyLog = this.notifyLog.concat(log);
                if (this.notifyLog.length > 2048) {
                    while (this.notifyLog.length > 2048) {
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
                status.push('Graph data saved');
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
        }
        else {
            var itemIdx = this.allExpr.length - (--this.exprCount);
            this.evalExpression(this.allExpr[itemIdx], itemIdx);
        }
    },

    evaluateTraceExpressions : function (exprMap) {
        var allExpr = [];

        /* Do just table cell expressions, get trace value when done */
        for (var i = 0; i < exprMap.length; i++) {
            var trace = this.rovTraces[exprMap[i]];
            var tableCells = trace.tableCells;
            for (var j = 0; j < tableCells.length; j++) {
                if (tableCells[j].expression) {
                    tableCells[j].declCells = trace.tableCells;
                    allExpr.push(tableCells[j]);
                }
            }
        }
        this.allExpr = allExpr;
        this.exprCount = allExpr.length;
        this.evalExpression(allExpr[0], 0);
    },

    evalWorkerMessage : function (e) {
        this.msgObj.evalResult(e.data);
    },

    getColumnValue : function (viewType, primaryKeyColumnName, primaryKeyColumnValue, columnName, viewData) {
        var val;
        if (viewType == 'INSTANCE' || viewType == 'CUSTOM') {
            if (rovUtils.isArray(viewData)) {
                for (var i = 0; i < viewData.length; i++) {
                    if (viewData[i][primaryKeyColumnName] == primaryKeyColumnValue) {
                        val = viewData[i][columnName];
                        break;
                    }
                }
            }
            else if (viewData[primaryKeyColumnName] == primaryKeyColumnValue) {
                val = viewData[columnName];
            }
        }
        else if (viewType == 'MODULE') {
            val = viewData[columnName];
        }
        else if (viewType == 'INSTANCE_DATA' || viewType == 'MODULE_DATA') {
            if (rovUtils.isArray(viewData)) {
            }
            else {
                var elemArr = viewData.elements;
                for (var i = 0; i < elemArr.length; i++) {
                    if (elemArr[i][primaryKeyColumnName] == primaryKeyColumnValue) {
                        val = elemArr[i][columnName];
                        break;
                    }
                }
            }
        }
        return (val);
    },

    getSelectedKeyProperties : function (sel) {
        var sepIdx = sel.indexOf(':');
        var props = {key: sel.substr(0, sepIdx), value: sel.substr(sepIdx + 3)};
        props.legendTitle = props.value + ' ' + this.selectedTrace.selectedColumnName;
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
    getViewCallback: function (error, viewData, module, view, handle) {
        /* show the refresh button and hide the refresh spinner */
        if (--this.requests == 0 && this.viewRefresh) {
            this.viewRefresh = false;
        }

        /* handle data acquisition error (if any) */
        if (error == null) {
            for (var i = 0; i < this.rovTraces.length; i++) {
                if (!this.rovTraces[i].isCustom) {
                    var traceView = this.rovTraces[i].args ? this.rovTraces[i].view + this.rovTraces[i].args : this.rovTraces[i].view;
                    if (this.rovTraces[i].module == module && traceView == view) {
                        this.rovTraces[i].viewData = viewData;
                    }
                }
                if (this.rovTraces[i].expression && this.rovTraces[i].tableCells.length > 0) {
                    for (var j = 0; j < this.rovTraces[i].tableCells.length; j++) {
                        var cell = this.rovTraces[i].tableCells[j];
                        var cellView = cell.args ? cell.view + cell.args : cell.view;
                        if (cell.module == module && cellView == view) {
                            cell.viewData = viewData;
                        }
                    }
                }
            }
        }
        else {
            for (var i = 0; i < this.rovTraces.length; i++) {
                if (!this.rovTraces[i].isCustom) {
                    var traceView = this.rovTraces[i].args ? this.rovTraces[i].view + this.rovTraces[i].args : this.rovTraces[i].view;
                    if (this.rovTraces[i].module == module && traceView == view) {
                        var cleanName = this.plJsTraces[i].name.replace(/<br>/, ' ');
                        this.requestErrors.push(cleanName + ': ' + error);
                    }
                }
            }
        }
        if (this.requests == 0) {
            if (this.requestErrors.length == 0) {
                this.exprCount = 0;
                this.subExprCount = 0;
                var exprMap = [];
                var vals = [];
                for (var i = 0; i < this.rovTraces.length; i++) {
                    if (!this.rovTraces[i].isCustom) {
                        this.rovTraces[i].val = this.getColumnValue(this.rovTraces[i].viewType,
                                                this.rovTraces[i].primaryKeyColumnName,
                                                this.rovTraces[i].primaryKeyColumnValue,
                                                this.rovTraces[i].selectedColumnName,
                                                this.rovTraces[i].viewData);
                        this.rovTraces[i].origVal = this.rovTraces[i].val;
                    }
                    if (this.rovTraces[i].expression) {
                        if (this.rovTraces[i].tableCells.length > 0) {
                            this.traceTableCellsVals(this.rovTraces[i]);
                        }
                        exprMap.push(i);
                    }
                }
                if (exprMap.length > 0) {
                    this.evaluateTraceExpressions(exprMap);
                }
                else {
                    for (var i = 0; i < this.rovTraces.length; i++) {
                        vals.push(Number(this.rovTraces[i].val));
                    }
                    this.plotNewData(vals);
                }
            }
            else {
                for (var i = 0; i < this.requestErrors.length; i++) {
                    if (this.isReportableError(this.requestErrors[i])) {
                        this.rovViewStatus(this.requestErrors[i], 'error');
                    }
                }
            }
        }
    },

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

    graphNameClicked : function (e) {
        var dialog = this.$.graphNameDialog;
        dialog.resetFit();
        var left = e.x;
        if ((left + 225) > document.documentElement.clientWidth) {
            left = document.documentElement.clientWidth - 225;
        }
        dialog.style.left = left + 'px';
        dialog.style.top = e.y + 'px';
        dialog.open();
        dialog.style.zIndex = String(this.rovPanel.getMaxZindex() + 1);
        this.$.graphNameInput.value = this.graphName;
    },

    graphNameDialogOkClicked : function (e) {
        this.graphName = this.$.graphNameInput.value;
        this.viewName = this.graphName;
        this.$.graphNameDialog.close();
    },

    hasLogging : function () {
        var has = false;
        for (var i = 0; i < this.rovTraces.length; i++) {
            if (this.rovTraces[i].expression && this.rovTraces[i].expressionType == 'boolean' &&
                this.rovTraces[i].actions.logNotifications) {
                has = true;
                break;
            }
        }
        return (has);
    },

    isSameModuleView : function (trace1, trace2) {
       return(trace1.module == trace2.module && trace1.view == trace2.view &&
              trace1.args == trace2.args);
    },

    newTrace : function (traceName) {
        var trace = {
            x: [0],
            y: [0],
            type: 'scatter',
            mode: 'lines',
        };
        if (traceName) {
            trace.name = traceName;
        }
        return (trace);
    },

    onPrimaryKeySelected : function (e) {
        if (e.currentTarget.selectedItemLabel) {
            var prevProps = this.getSelectedKeyProperties(this.selectedTrace.dlgPrimaryKey);
            var curProps = this.getSelectedKeyProperties(e.currentTarget.selectedItemLabel);
            if (!this.selectedTrace.dlgAssignedTraceName) {
                if (this.$.assignedTraceNameInput.value != prevProps.legendTitle) {
                    /* user modified legend title */
                    this.selectedTrace.dlgAssignedTraceName = this.$.assignedTraceNameInput.value;
                }
                else {
                    this.$.assignedTraceNameInput.value = curProps.legendTitle;
                }
            }
            this.selectedTrace.dlgPrimaryKey = e.currentTarget.selectedItemLabel;
            this.selectedTrace.dlgLegendTitle = this.$.assignedTraceNameInput.value;
            if (this.$.chartOptionsMenu.selected != undefined) {
                this.$.chartOptionsMenu.selected = -1;
            }
        }
    },

    /*
     *  ======== onRefresh ========
     *  Called when user clicks the 'Refresh' button on this view,
     *  or 'Refresh all' or 'Repeat refresh' buttons on the ROV toolbar
     */
    onRefresh: function () {
        /*
         *  Request CPU load from an ROV view.
         *
         *  This is an asynchronous call; getViewCallback() will be called
         *  with the result in its viewData parameter.
         */
        if (this.exprCount > 0) {
            return;  /* not done with previous */
        }
        this.requestErrors = [];
        var requests = [];
        for (var i = 0; i < this.rovTraces.length; i++) {
            if (!this.rovTraces[i].isCustom) {
                var view = this.rovTraces[i].args ? this.rovTraces[i].view + this.rovTraces[i].args : this.rovTraces[i].view;
                if (!requests[this.rovTraces[i].module + '.' + view]) {
                    requests[this.rovTraces[i].module + '.' + view] = true;
                    if (this.rovData.getView(this.rovTraces[i].module, view, this)) {
                        ++this.requests;
                    }
                }
            }
            if (this.rovTraces[i].expression && this.rovTraces[i].tableCells.length > 0) {

                for (var j = 0; j < this.rovTraces[i].tableCells.length; j++) {
                    var cell = this.rovTraces[i].tableCells[j];
                    if (!cell.isCustom) {
                        var cellView = cell.args ? cell.view + cell.args : cell.view;
                        if (!requests[cell.module + '.' + cellView]) {
                            requests[cell.module + '.' + cellView] = true;
                            if (this.rovData.getView(cell.module, cellView, this)) {
                                ++this.requests;
                            }
                        }
                    }
                }
            }
        }
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

    onTracePropertiesDialogClosed : function (e) {
        if (e.target.id == 'tracePropertiesDialog') {
            this.set('primaryKeyMenuItems', []);
            this.set('primaryKeyMenuLabel', '');
            this.$.primaryKeyMenu._setSelectedItem(null);
        }
    },

    /*
     *  ======== plotNewData ========
     *  Add a new value to the line graph
     */
    plotNewData: function (vals) {
        /* add row to both our data set and the graph's */
        for (var i = 0; i <  vals.length; i++) {
            this.rows[i].x.push(this.sample);
            this.rows[i].y.push(vals[i]);
        }
        ++this.sample;

        /* limit total number of rows */
         for (i = 0; i < this.rows.length; i++) {
            if (this.rows[i].x.length > this.MAX_ROWS) {
                /* remove "oldest" row from our array and the data table */
                this.rows[i].x.splice(0, 1);
                this.rows[i].y.splice(0, 1);
            }
        }

        for (i = 0; i < this.rows.length; i++) {
            this.plJsTraces[i].x = this.rows[i].x;
            this.plJsTraces[i].y = this.rows[i].y;
        }

        if (this.sample > this.VISIBLE_ROWS) {
            /* always show latest data */
            this.layout.xaxis.range = [this.sample - this.VISIBLE_ROWS, this.sample - 1];
        }

        /* update graph's x-axis (if necessary) and redraw it */
        Plotly.update(this.$.graph, this.plJsTraces, this.layout);
    },

    plotOptionsDialog : function (event) {
        if (!this.selectedTrace.args && this.selectedTrace.viewType != 'MODULE' &&
            !this.selectedTrace.customTable) {

            this.primaryKeySectionHidden = false;
            var primaryKeyMenuItems = [];
            var primaryKeyColumns = this.selectedTrace.primaryKeyColumns;
            var maxStr = '';
            var pKeyIndex = 0;
            for (var i = 0; i < primaryKeyColumns.length; i++) {
                var val = this.getColumnValue(this.selectedTrace.viewType,
                                              this.selectedTrace.primaryKeyColumnName, this.selectedTrace.primaryKeyColumnValue,
                                              primaryKeyColumns[i], this.selectedTrace.viewData);
                primaryKeyMenuItems.push(primaryKeyColumns[i] + ':  ' + val);
                if (primaryKeyColumns[i] == this.selectedTrace.primaryKeyColumnName) {
                    pKeyIndex = i;
                }
                if (primaryKeyMenuItems[i].length > maxStr.length) {
                    maxStr = primaryKeyMenuItems[i];
                }
            }

            this.set('primaryKeyMenuItems', primaryKeyMenuItems);
            this.set('primaryKeyMenuLabel', this.selectedTrace.primaryKeyColumnName + ':  ' +  this.selectedTrace.primaryKeyColumnValue);
        }
        else {
            this.primaryKeySectionHidden = true;
        }
        this.$.assignedTraceNameInput.value = event.currentTarget.innerText.trim();

        /* stuff to maintain state of tracePropertiesDialog */
        if (!this.selectedTrace.args && this.selectedTrace.viewType != 'MODULE' &&
            !this.selectedTrace.customTable) {

            this.selectedTrace.dlgPrimaryKey = this.primaryKeyMenuLabel;
        }
        this.selectedTrace.dlgAssignedTraceName = this.selectedTrace.assignedTraceName;

        /* for margins, checkbox and droplists */
        var width = rovUtils.getStringWidth(maxStr);
        this.$.primaryKeyMenu.$.menuButton.style.width = width + 'px';

        var left = event.x;
        if ((left + width + 100) > document.documentElement.clientWidth) {
            left -= (width + 100);
        }
        var dialog = this.$.tracePropertiesDialog;
        dialog.resetFit();
        dialog.style.left = left + 'px';
        dialog.style.top = event.y + 'px';
        dialog.open();
        dialog.style.zIndex = String(this.rovPanel.getMaxZindex() + 1);
        if (this.$.chartOptionsMenu.selected != undefined) {
            this.$.chartOptionsMenu.selected = -1;
        }
    },

    /*
     *  ======== relayoutEvent ========
     *  Recalc visiblet rows on x axis resize to
     *  enable scrolling latest data into view
     */
    relayoutEvent : function (eData) {
        var x0 = eData['xaxis.range[0]'];
        var x1 = eData['xaxis.range[1]'];
        if (x0 != undefined && x1 != undefined) {
            this.VISIBLE_ROWS = x1 > x0 ? x1 - x0 : x0 - x1;
        }
    },

    /*
     *  ======== resized ========
     *  Callback initially passed to rovUtils.initRovResize() from
     *  resizerClicked(). Called after the view has been resized,
     *  enabling us to resize the graph accordingly
     */
    resized: function () {
        /* get viewPaperCard defined via the element in ti-rov-graph.html */
        var viewPaperCard = this.$.viewPaperCard;
        var graph = this.$.graph;

        /* get the new width and height from viewPaperCard's style */
        var width = Number(viewPaperCard.style.width.slice(0, -2));
        var height = Number(viewPaperCard.style.height.slice(0, -2));

        /* get viewPaperCard minWidth and minHeight */
        var minWidth = Number(viewPaperCard.style.minWidth.slice(0, -2));
        var minHeight = Number(viewPaperCard.style.minHeight.slice(0, -2));

        /* Don't let new size go below the defined minimums */
        this.graphWidth = Math.max(width, minWidth) * .95;
        this.graphHeight = Math.max(height, minHeight) * .85;

        /* Scale the graph accordingly */
        graph.style.width = this.graphWidth + 'px';
        graph.style.height = this.graphHeight + 'px';

        /* update the graph and redisply it */
        this.layout.width = this.graphWidth;
        this.layout.height = this.graphHeight;
        this.VISIBLE_ROWS = Math.floor((this.graphWidth - 80) / 3.1) + 1;
        if (this.VISIBLE_ROWS > this.MAX_ROWS) {
            this.VISIBLE_ROWS = this.MAX_ROWS;
        }
        if (this.sample > this.VISIBLE_ROWS) {
            /* always show latest data */
            this.layout.xaxis.range = [this.sample - this.VISIBLE_ROWS, this.sample - 1];
        }
        else {
            this.layout.xaxis.range = [0, this.VISIBLE_ROWS - 1];
        }
        Plotly.update(this.$.graph, this.plJsTraces, this.layout);
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
        rovUtils.initRovResize(e, this.$.viewPaperCard, {elem: this, fxn: this.resized});
    },

    rovGraphDialogKeyPress : function (e) {
        var keyCode = e.keyCode || e.which;
        if (keyCode === 13) {
            var dlgId = e.currentTarget.id;
            this[dlgId + 'OkClicked'](e);
        }
    },

    setAxisLabels : function (labelsObj) {
        if (labelsObj.xAxisLabel) {
            this.xAxisLabel = labelsObj.xAxisLabel;
            this.layout.xaxis.title = this.xAxisLabel;
        }
        if (labelsObj.yAxisLabel) {
            this.yAxisLabel = labelsObj.yAxisLabel;
            this.layout.yaxis.title = this.yAxisLabel;
        }
        Plotly.update(this.$.graph, this.plJsTraces, this.layout);
    },

    showModeBarChanged : function (e) {
        this.modeBarVisible = e.target.checked;
        Plotly.react(this.$.graph, this.plJsTraces, this.layout,
                     this.modeBarVisible ? this.plotConfigModeBar : this.plotConfigNoModeBar);
        if (this.$.chartOptionsMenuDialog.opened) {
            this.$.chartOptionsMenuDialog.close();
        }
    },

    traceName : function (trace) {
        var name;
        if (trace.assignedTraceName) {
            name = trace.assignedTraceName;
        }
        else if (trace.customTable) {
            name = trace.viewName + '<br>' + trace.displayColumnName;
        }
        else if (trace.args) {
            name = (trace.view == 'Variable') ? trace.firstArg : trace.selectedColumnName;
        }
        else if (trace.viewType == 'MODULE') {
            name = trace.selectedColumnName;
        }
        else {
            name = trace.primaryKeyColumnValue + '<br>' + trace.selectedColumnName;
        }
        return (name);
    },

    traceNameMenuItemSelected : function (e) {
        this.$.traceNamesMenu.selected = -1;
        /* delete trace button */
        if (e.srcElement.id != 'icon' && !this.$.tracePropertiesDialog.opened) {
            var idElem = e.currentTarget.lastElementChild;
            var i = Number(idElem.id.substr(idElem.id.indexOf('_') + 1));
            this.set('selectedTrace', this.rovTraces[i]);
            this.selectedTrace.index = i;
            this.plotOptionsDialog(e);
        }
    },

    tracePropertiesClicked : function (e) {
        if (e.srcElement.className.indexOf('chartOptionsMenuI') >= 0) {
            this.$.traceNamesMenu.hidden = !this.$.traceNamesMenu.hidden;
            this.$.chartOptionsMenu.selected = -1;
            this.$.tracePropertiesSubMenu.selected = -1;
            this.$.traceNamesMenu.selected = -1;
        }
    },

    tracePropertiesDialogOkClicked : function (e) {
        var keyChanged = false;
        var legendChanged = false;
        var update = false;
        if (!this.selectedTrace.args && this.selectedTrace.viewType != 'MODULE') {
            var label = this.$.primaryKeyMenu.selectedItemLabel ?
                        this.$.primaryKeyMenu.selectedItemLabel :
                        this.$.primaryKeyMenu.label;
            var props = this.getSelectedKeyProperties(label);
            if (this.selectedTrace.primaryKeyColumnName != props.key) {
                this.selectedTrace.primaryKeyColumnName = props.key;
                this.selectedTrace.primaryKeyColumnValue = props.value;
                keyChanged = true;
            }
            if (this.$.assignedTraceNameInput.value != props.legendTitle) {
                if (this.selectedTrace.assignedTraceName != this.$.assignedTraceNameInput.value) {
                    this.selectedTrace.assignedTraceName = this.$.assignedTraceNameInput.value;
                    legendChanged = true;
                }
            }
            else if (this.selectedTrace.assignedTraceName) {
                this.selectedTrace.assignedTraceName = null;
                legendChanged = true;
            }
        }
        else {
            var defaultName;
            if (this.selectedTrace.customTable) {
                defaultName = this.selectedTrace.viewName + ' ' + this.selectedTrace.displayColumnName;
            }
            else if (this.selectedTrace.args) {
                defaultName = (this.selectedTrace.view == 'Variable') ? this.selectedTrace.firstArg : this.selectedTrace.selectedColumnName;
            }
            else {
                defaultName = this.selectedTrace.selectedColumnName;
            }
            if (this.$.assignedTraceNameInput.value != defaultName) {
                if (this.selectedTrace.assignedTraceName != this.$.assignedTraceNameInput.value) {
                    this.selectedTrace.assignedTraceName = this.$.assignedTraceNameInput.value;
                    legendChanged = true;
                }
            }
            else if (this.selectedTrace.assignedTraceName) {
                this.selectedTrace.assignedTraceName = null;
                legendChanged = true;
            }
        }
        if (keyChanged && !legendChanged && !this.selectedTrace.assignedTraceName) {
            this.plJsTraces[this.selectedTrace.index].name = this.selectedTrace.primaryKeyColumnValue + '<br>' + this.selectedTrace.selectedColumnName;
            update = true;
        }
        else if (legendChanged) {
            this.plJsTraces[this.selectedTrace.index].name = this.traceName(this.selectedTrace);
            update = true;
        }
        if (update) {
            this.set('traceNamesMenuItems.' + this.selectedTrace.index + '.id',
                     this.plJsTraces[this.selectedTrace.index].name);
            this.set('traceNamesMenuItems.' + this.selectedTrace.index + '.label',
                     this.plJsTraces[this.selectedTrace.index].name.replace(/<br>/, ' '));
            this.$.traceNamesMenuTemplate.render();
            Plotly.update(this.$.graph, this.plJsTraces, this.layout);
        }

        /* error check legend title ? */
        this.$.tracePropertiesDialog.close();
    },

    traceTableCellsVals : function(trace) {
        var tableCells = trace.tableCells;
        for (var i = 0; i < tableCells.length; i++) {
            if (!tableCells[i].isCustom) {
                var value = this.getColumnValue(tableCells[i].viewType,
                            tableCells[i].primaryKeyColumnName, tableCells[i].primaryKeyColumnValue,
                            tableCells[i].selectedColumnName, tableCells[i].viewData);
                tableCells[i].origVal = value;
                tableCells[i].val = value;
            }
        }
    },

    /*
    *  ======== traceTooltip ========
    */
    traceTooltip : function (trace) {
        if (trace.isCustom) {
            var tooltip = '\nView: ' + trace.viewName + '\nColumn: '  + trace.displayColumnName;
            tooltip += '\nExpression: ' + trace.expression;
        }
        else {
            if (trace.customTable) {
                var tooltip = '\nView: ' + trace.viewName +
                              '\nColumn: '  + trace.displayColumnName +
                              '\n  Module: ' + trace.module;
                var pre = '\n  ';
            }
            else {
                var tooltip = 'Module: ' + trace.module;
                var pre = '\n';
            }
            if (trace.args) {
                if (trace.view == 'Variable') {
                    tooltip += pre + 'Variable: ' + trace.firstArg;
                }
                else {
                    tooltip += pre + 'View: ' + trace.view;
                    var args = trace.viewArgs[trace.argsId].args;
                    for(var j = 0; j < args.length; j++) {
                        tooltip += pre + args[j].name + ': ' + args[j].value;
                    }
                }
            }
            else {
                tooltip += pre + 'View: ' + trace.view;
            }
            tooltip += pre + 'Column: ' + trace.selectedColumnName;
            tooltip += pre + trace.primaryKeyColumnName + ': ' + trace.primaryKeyColumnValue;
            if (trace.expression) {
                tooltip += '\nExpression: ' + trace.expression;
                if (trace.expressionType == 'boolean') {
                    var actionsStr = '';
                    if (trace.actions.notify) {
                        actionsStr += '\n  Display notification';
                    }
                    if (trace.actions.logNotifications) {
                        actionsStr += '\n  Log notifications';
                    }
                    if (trace.actions.stopUpdates) {
                        actionsStr += '\n  Stop updating this view';
                    }
                    if (trace.actions.stopAllUpdates) {
                        actionsStr += '\n  Stop updating all views';
                    }
                    if (trace.actions.saveView) {
                        actionsStr += "\n  Save this view's data";
                    }
                    if (trace.actions.saveAllViews) {
                        actionsStr += "\n  Save all views' data";
                    }
                    if (actionsStr) {
                        tooltip += '\nActions:' + actionsStr;
                    }
                }
            }
        }
        return (tooltip);
    }
});
