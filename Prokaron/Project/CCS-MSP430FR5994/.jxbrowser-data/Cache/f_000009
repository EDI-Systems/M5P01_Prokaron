/*
 *  ======== rovapi.js ========
 *  Browser-side ROVServer API
 */

var rov;            /* TICloudAgent ROVServer sub module */

var packagePath;    /* package path array from the environment, CCS, or ... */

var wssPort = 8888; /* standalone nodeserver's WebSocket server port */
var sock;           /* standalone WebSocket server socket */

/* ccs7 is true when we are running in the context of the CCS cloud */
var ccs7 = false;

var ds;             /* DebugServer module: agent.getSubModule("DS") */
var dsFlag = false; /* true if we're told to connect via DSLite */
var dsActiveCoreName = null;
var dsActiveCoreId = 0;

var deferedSymbols = null;
var exeSet = null;  /* exeSet != null => ROV connected to the exe exeSet */

/*
 *  ======== debuggerStatus ========
 *  Current debugger state
 *
 *      null           - unknown or disconnected
 *      "Disconnected" -
 *      "TargetHalt"   -
 *      "Running"      -
 *      "Connected"    -
 *
 * TODO: connected/disconnected should be orthogonal to
 *       running/halted so what's up with the states above?!?!
 */
var debuggerStatus = null;

/*
 *  ======== init ========
 */
function init()
{
    ccs7 = (typeof TICloudAgent) == "undefined" ? false : true;

    /* parse URL for setup parameters */
    var tokens = location.href.split(/[?&]/);
    tokens.shift();
    for (var i = 0; i < tokens.length; i++) {
        var param = tokens.shift();
        var pair = param.split('=');
        switch (pair[0]) {
            case "wsPort": {
                if (pair[1]) {
                    wssPort = pair[1];
                    console.log("set wssPort to: " + wssPort);
                }
                break;
            }
        }
    }
}

/*
 * ======== setExecutable ========
 * Specify the executable to decode
 *
 * This function initializes ROV and must be called before any other calls.
 *
 * During the operation, progress() is called with a progress string that
 * may be useful to an impatient user.
 *
 * On completion, callback() is called with a response of the form:
 *     {resStr: "...", exeName: "...", connType: "...", connArgs: ["...", ...]}
 *  where, 
 *      exeName  - full path of the executable
 *      connType - connection type: "DSLite", "Serial", "Net", ...
 *      connArgs - array of String values needed by the connection
 *                  or, in the event that none are supplied, []
 *      coreName - name of core to which we're connected (if unknown, null)
 *      resStr   - final status result string from this operation:
 *                     "OK"             - success
 *                     any other string - failure and the string is a printable
 *                                        message explaining the cause
 *
 * Events coming from the target after a connection is estabilished
 * but possibly _before_ and after the executable has been set are
 * returned by calling targetEvents().
 *
 * path is a string of the form:
 *     <exe_path>?comm=<comm_type>[:<comm_arg>]*[&<name=value>]*
 * where,
 *     <exe_path>   is the full path to the executable
 *     <comm_type>  is "DSLite", "Serial", "Net", ...
 *     <comm_arg>   is a communication-specific value
 *     <name=value> is an arbitrary name-value pair that is
 *                  specified by the client
 */
function setExecutable(path, reqObj, callback, progress, targetEvents)
{
    init();

    /* standalone (for now we need TICloudAgent to use DSLite, blech!) */
    if (!ccs7 && path.indexOf("DSLite") == -1) {
        var old = sock;
        sock = new WebSocket("ws://localhost:" + wssPort);
        sock.onopen = function() {
            sock.send(JSON.stringify("exec:" + path + "\r\n"));
            if (old != null) {
                old.close();
            }
        };

        sock.onmessage = function (ret) {
            console.log("setExec response: " + ret.data);
            if (ret.data != null && ret.data.indexOf("Working:") == 0) {
                if (progress != null) progress(ret.data, reqObj);
            }
            else {
                var result = _parseExePath(path);

                /* add resStr to result to form callback value */
                result.resStr = ret.data;
                result.exeName = "";      /* safe, but is this needed??? */
                result.coreName = "";
                callback(result, reqObj);
            }
        };
    }
    /* TI Cloud */
    else {
        var dev = "";
        var k = path.indexOf("DSLite");
        if (k != -1) {
            dsFlag = true;

            /* check path for a core name (default is "") */
            var tmp = path.substr(k + 7).split(':')[0];
            if (tmp != null && tmp != "" && tmp != "undefined") {
                dev = tmp;
            }
        }

        /* connect to the ROV module and DS (if requested) */
        _connect().then(
            /* connect to active core's module */
            function(obj) {
                /* save ROVServer module for subsequent use */
                rov = obj;

                /* use ds to get debug core */
                if (ds == null || dsFlag == false) {
                    var promContainer = Q.defer();
                    var prom = promContainer.promise;
                    promContainer.resolve();
                    return (prom); /* return undefined */
                }

                /* if ds != null, get all core names */
                return ds.listCores().then(
                    function(cores) {
                        /* compute active core id */
                        var id = 0;
                        if (dsActiveCoreName != null) {
                            for (var i = 0; i < cores.cores.length; i++) {
                                if (cores.cores[i] == dsActiveCoreName) {
                                    id = i;
                                    break;
                                }
                            }
                        }
                        dsActiveCoreId = id;
                        return _setupCore(cores, dev, targetEvents, reqObj);
                    }
                ).then(
                    function(core0) {
                        console.log("dstatus: " + debuggerStatus);
                        if (debuggerStatus == null
                            || debuggerStatus == "Disconnected") {
                            console.log("debugger is not connected");
                            /* don't fail if connection is, say, Serial.
                             * (does this make sense?)
                             */
                            if (dsFlag) {
                                throw new Error("The debugger is not connected");
                            }
                        }
                        return (core0); /* return core's module */
                    }
                );
            }
        )
        .then(
            /* use debug core to get exe name */
            function(core0) {
                if (core0 != null) {
                    /* ask debugger for files loaded on specified core */
                    return _getSymbols(core0);
                }
                console.log("no debugger => no symbol file list");
                return ({symbolFiles: []});
            }
        )
        .then(
            /* get package path for the specified exe */
            function(files) {
                console.log("core files: "
                            + JSON.stringify(files, null, 3));
                var exe = _validateExe(path, files).split('?')[0];
                return _getProject(exe).then(
                    function (proj) {
                        console.log("exe project: "
                            + JSON.stringify(proj, null, 3));
                        if (proj.location && rov_setFileDefaults) {
                            rov_setFileDefaults(proj.location, proj.location);
                        }
                        return (_getPackagePath(proj.name));
                    }
                )
                .then(
                    function (ppath) {
                        packagePath = ppath;
                        console.log("exe package path: " + ppath);
                        return (files);
                    }
                )
                .fail(
                    function(msg) {
                        throw new Error(msg);
                    }
                );
            }
        )
        .then(
            /* start ROV model with an exe specified by path or debugger */
            function(files) {
                console.log("core files: "
                            + JSON.stringify(files, null, 3));
                var newPath = _validateExe(path, files);
                var obj = _parsePackagePath(packagePath);

                console.log("rov.setExec(" + newPath + ", " + obj.xdcroot
                    + ", " + obj.xdcpath + ")");
                return rov.setExec(newPath, obj.xdcroot, obj.xdcpath).then(
                    function detectProgress(res) {
                        console.log("setExec response: " + res);
                        if (res != null && res.indexOf("Working:") == 0) {
                            rov.getProgress().then(detectProgress);
                            if (progress != null) {
                                progress(res, reqObj);
                            }
                        }
                        else {
                            /* parse newPath for the UI */
                            var result = _parseExePath(newPath);
                            result.coreName = _getCoreName(dsActiveCoreName);

                            /* add resStr to result to form callback value */
                            result.resStr = res;

                            /* save just the exe name */
                            exeSet = result.exeName;

                            callback(result, reqObj);
                        }
                    }
                );
            }
        )
        .fail(
            function(err) {
                var msg = err.message ? err.message : err;
                console.log("Error: setExecutable() failed: " + msg);
                var result = _parseExePath(path);
                result.resStr = String(msg);
                result.exeName = "";
                result.coreName = "";
                callback(result, reqObj);
            }
        );
    }

    return (true);
}

/*
 *  ======== discoverAddons ========
 */
function discoverAddons(reqObj, callback)
{
    /* standalone */
    if (sock != null) {
        sock.onmessage = function(ret) {
            var parsed = null;
            var err = null;
            try {
                parsed = JSON.parse(ret.data);
            }
            catch (e) {
                err = 'Error: JSON.parse exception: ' + e.message;
            }
            callback(err, parsed, reqObj);
        };
        sock.send(JSON.stringify("addons\r\n"));
    }
    /* TI Cloud */
    else {
        rov.discoverAddons().then(
            function(list) {
                console.log("addons: " + list);
                var parsed = null;
                var err = null;
                try {
                    parsed = JSON.parse(list);
                }
                catch (e) {
                    err = 'Error: JSON.parse exception: ' + e.message;
                }
                callback(err, parsed, reqObj);
            }
        ).fail(
            function (err) {
                var msg = err.message ? err.message : err;
                console.log(
                    "Error: Unexpected Exception from discoverAddons callback:" 
                    + msg);
            }
        );
    }

    return (true);
}

/*
 * ======== getViewList ========
 * Get list of all modules and their supported views
 *
 * The view list is a map of arrays, where keys are module names and values
 * are arrays of the objects with 'name' and 'type' properties.
 * [{name:"view1", type:"view1_type"}, ... ,{name:"viewN", type:"viewN_type"}]
 *
 * Returns the view list or an error by calling callback(err, list, reqObj),
 * where,
 *     err    - error string (or null for no error)
 *     list   - the view list (or null if err is non-null
 *     reqObj - the passed in context object
 */
function getViewList(reqObj, callback)
{
    /* standalone */
    if (sock != null) {
        sock.onmessage = function(ret) {
            var parsed = null;
            var err = null;
            try {
                parsed = JSON.parse(ret.data);
            }
            catch (e) {
                err = 'Error: JSON.parse exception: ' + e.message;
            }
            callback(err, parsed, reqObj);
        };
        sock.send(JSON.stringify("list\r\n"));
    }
    /* TI Cloud */
    else {
        rov.getViews().then(
            function(list) {
                console.log("list: " + list);
                var parsed = null;
                var err = null;
                try {
                    parsed = JSON.parse(list);
                }
                catch (e) {
                    err = 'Error: JSON.parse exception: ' + e.message;
                }
                callback(err, parsed, reqObj);
            }
        ).fail(
            function (err) {
                var msg = err.message ? err.message : err;
                console.log(
                    "Error: Unexpected Exception from getViewList callback:" 
                    + msg);
            }
        );
    }

    return (true);
}

/*
 *  ======== getView ========
 *  Get specified view from a module
 *
 *  Returns the view or an error by calling
 *  callback(err, viewObj, module, viewName, reqObj),
 *  where,
 *      err      - error string (or null forno error)
 *      viewObj  - the view JSON object (or null if err is non-null)
 *      module   - the passed in module name
 *      viewName - the passed in view name
 *      reqObj   - the passed in context object
 */
var queue = [];
function getView(module, view, reqObj, callback)
{
    /* standalone */
    if (sock != null) {
        queue.push({module: module, view: view, reqObj: reqObj});
        sock.onmessage = function(ret) {
            var parsed = null;
            var err = null;

            if (ret.data.indexOf("Error:") == 0) {
                err = ret.data;
            }
            else {
                try {
                    parsed = JSON.parse(ret.data);
                }
                catch (e) {
                    err = 'Error: JSON.parse exception: '
                        + e.message + ": " + ret.data;
                }
            }
            var args = queue.shift();
            callback(err, parsed, args.module, args.view, args.reqObj);
        };

        sock.send(JSON.stringify("view:" + module + "," + view + "\r\n"));
    }
    /* TI Cloud */
    else {
        //if (debuggerStatus == "Running") {
        //    callback("Target is running!", null, module, view, reqObj);
        //    return;
        //}
        if (dsFlag && (debuggerStatus == null
                       || debuggerStatus == "Disconnected")) {
            callback("Target is disconnected!", null, module, view, reqObj);
            return (false);
        }
        rov.getView(module, view).then(
            function(response) {
                if (response.indexOf("Error:") == 0) {
                    callback(response, null, module, view, reqObj);
                }
                else {
                    var parsedRes;
                    var parseErr = null;
                    try {
                        parsedRes = JSON.parse(response);
                    }
                    catch (e) {
                        parseErr = 'Error: JSON.parse exception: ' + e.message;
                        parsedRes = null;
                    }
                    callback(parseErr, parsedRes, module, view, reqObj);
                }
                console.log(module + ":" + view + ": " + response);
            }
        ).fail(
            function (err) {
                var msg = err.message ? err.message : err;
                console.log("Error: Unexpected exception from getView("
                            + module + ", " + view + "): " + msg);
            }
        );
    }

    return (true);
}

/*
 *  ======== _configureDebugger ========
 *  Configure debugger using the supplied target configuration
 *
 *  devName is the basename of a .ccxml file (minus the .ccxml extension)
 *  that defines how to setup the debugger
 *
 *  rov is assumed to be non-null (i.e., _connect() returns a promise to get
 *  the ROVServer module _and_ that promise resolved to rov).
 *
 *  ds is also assumed to be non-null, which is implied by rov != null
 *  because, ds is initialized by _connect() before creating the promise to
 *  get rov.
 *
 *  Returns a promise
 *      resolved to dbg, where dbg.cores == array of cores
 *  or
 *      rejected because devName.ccxml is not found or
 *                       configuration with this file failed
 *
 *  (Internal function)
 */
function _configureDebugger(devName)
{
    console.log("_configureDebugger(" + devName + ")");
    return _getProject().then(
        /* get the package path from active project (or workspace if none) */
        function(proj) {
            console.log("activeProjectContext: " + JSON.stringify(proj, null, 3));
            return _getPackagePath(proj.name);
        }
    ).then(
        /* lookup .ccxml file along package path */
        function (packagePath) {
            return rov.find(devName + ".ccxml", packagePath);
        }
    ).then(
        /* configure the debugger */
        function(ccxmlFile) {
            console.log("configuring with: " + ccxmlFile);
            return ds.configure(ccxmlFile).fail(
                function(err) {
                    var msg = "configuration for '" + devName
                        + "' failed: " + (err.message ? err.message : err);
                    console.log(msg);
                    throw new Error(msg);
                }
            );
        }
    ).fail(
        function(err) {
            debuggerStatus = null;

            var msg = "can't find " + devName + ".ccxml";
            console.log(msg);
            throw new Error(msg);
        }
    );
}

/*
 *  ======== _connect ========
 *  Connect to the ROV module in the TICloudAgent WebSocket server
 *
 *  Called directly by setExecutable() to establish connection with
 *  the ROVServer module (rov_server.js).
 *
 *  Returns a promise
 *      resolved to rov = ROVServer submodule (agent.getSubModule("ROVServer"))
 *      rejected because we can't start agent or the connection is
 *                       DSLite and we failed to get the DS submodule
 *
 *  Throws an exception if TICloudAgent.Init() throws (which it shouldn't)
 *
 *  Side Effects: sets ds to the DebugServer/DSLite module
 *
 *  (Internal function)
 */
function _connect()
{
    var agent;
    console.log("_connect: connecting ...");

    if (rov != null) {
        var prom = Q.defer();
        prom.resolve(rov);
        console.log("_connect: using cached rov = " + rov);
        return (prom.promise);
    }

    return TICloudAgent.Init().then(
        /* init/configure agent and get EventBroker module */
        function(_agent) {
            agent = _agent;
            agent.addConfigProperty("cloudAgentInstallerServerURL",
                                    location.origin + "/ticloudagent/");
            return agent.getSubModule("EventBroker");
        }
    ).then(
        /* add EventBroker listeners and get active project */
        function(broker) {
            broker.addListener("ccs.debug.activeContextChangedEvent",
                function(data) {
                    console.log("ccs.debug.activeContextChanged: "
                                + JSON.stringify(data, null, 3));
                });

            return broker.fetchData("ccs.debug.activeContext");
        }
    ).then(
        /* get DS module */
        function(obj) {
            console.log("activeDebugContext: " + JSON.stringify(obj, null, 3));
            if (obj.data && obj.data.length > 0) {
                dsActiveCoreName = obj.data[0].core;
            }

            /* get DS sub-model from agent */
            return agent.getSubModule("DS");
        }
    ).then(
        /* add DS listeners */
        function(_ds) {
            ds = _ds;
            console.log("acquired DS sub-module: " + ds);
            ds.addListener("close",
                function() {
                    console.log("DS closing");
                    debuggerStatus = null;
                }
            );
            ds.addListener("configChanged",
                function(data) {
                    console.log("DS config changed: "
                        + JSON.stringify(data, null, 3));
                    if (data.cores[0] == undefined) {
                        debuggerStatus = null;
                        console.log("DS config changed: debugger terminated");
                        if (exeSet != null) {
                            location.reload(true);
                            exeSet = null;
                        }
                    }
                }
            );
         }
    ).fail(
        /* throw exception if we can't start agent or get DS submodule */
        function(err) {
            var emsg = err.message ? err.message : err;
            if (agent == null) {
                var msg = "ROV _connect failed to start agent: " + emsg;
                console.log(msg);
                throw new Error(msg);
            }
            else if (ds == null) {
                var msg = "ROV _connect failed to acquire DS: " + emsg;
                console.log(msg);
                if (String(emsg).indexOf("ccs.debug.activeContext") != -1) {
                    msg = "Unable to obtain package path from CCS.  Try "
                        + "restarting ROV after starting the debugger.  "
                        + emsg;
                }
                if (dsFlag) {
                    throw new Error(msg);
                }
            }
        }
     ).then(
         /* return ROVServer submodule */
         function() {
             console.log("_connect: requesting server ROV Module from agent");
             return agent.getSubModule("ROVServer");
         }
     );
}

/*
 *  ======== _getCoreName ========
 */
function _getCoreName(dsName)
{
    var coreName = "";
    if (dsName != null) {
        var k = dsName.lastIndexOf('/');
        coreName = dsName.substr(k + 1);
    }

    return (coreName);
}

/*
 *  ======== _getProject ========
 *  Return the "current project" or the project containing exePath
 *
 *  Returns a promise that resolves to an object containing the name of the
 *  current project and its location.
 * 
 *  XMLHttpRequest returns:
 *      400 and an error message when a required argument is not present 
 *          in the request.
 *      500 and log the error message when there is an unexpected exception
 *          while servicing a request
 *      200 and an empty body when there is nothing to return.  Eg, if the
 *          findContextProject finds nothing
 *      200 and a JSON object in body (with contentType header set to
 *          "application/json"), when there is data to return
 *      404, 403, etc. in the cases where server is down for some reason, 
 *          or when request is sent to an unknown route
 */
function _getProject(exePath)
{
    var defer = Q.defer();

    if (!ccs7) {
        defer.resolve(null);
        return (defer.promise);
    }

    /* get project/workspace information */
    function transferFailed(req) {
        /* the request for info failed */
        return function (evt) {
            var msg = "An error occurred while getting "
                + req + " info: " + evt;
            console.log(msg);
            defer.reject(msg);
        };
    }

    function reqListener(req) {
        /* the request for info succeeded (maybe: 404 errors return HTML!) */
        return function() {
            console.log(req + " info: " + this.responseText);
            var result = {name: null, location: null};
            try {
                /* TODO: throw if req.status != 200? */
                if (this.responseText != "") {
                    result = JSON.parse(this.responseText);
                }
            }
            catch (e) {
                /* TODO: use statusText rather than responseText? */
                defer.reject("An error occurred while getting "
                    + req + " info: IDE server failed: " + this.responseText);
            }
            defer.resolve(result);
        };
    }

    /* request current project from the CCS "ide server" */
    var req = new XMLHttpRequest();
    console.log("getting project ...");
    req.addEventListener("load", reqListener("project"));
    req.addEventListener("error", transferFailed("project"));

    var suffix = "/ide/findContextProject";
    if (exePath != null) {
        suffix = "/ide/findOwnerProject?resourcePath=" + exePath;
    }

    req.open("GET", encodeURI(location.origin + suffix));
    req.send();

    return (defer.promise);
}

/*
 *  ======== _getPackagePath ========
 *  Return the package path from the current context (if provided)
 *
 *  Returns an array consisting of the package path components, the last
 *  being the "./packages" sub-directory of XDCtools.
 *
 *  If running in the context of CCS7, ask CCS, otherwise returns null.
 */
function _getPackagePath(projectName)
{
    var defer = Q.defer();

    if (!ccs7) {
        defer.resolve(null);
        return (defer.promise);
    }

    /* get project/workspace information */
    function transferFailed(req) {
        /* the request for info failed */
        return function (evt) {
            var msg = "An error occurred while getting "
                + req + " info: " + evt;
            console.log(msg);
            defer.reject(msg);
        };
    }

    function reqListener(req) {
        /* the request for info succeeded (maybe: 404 errors return HTML!) */
        return function() {
            console.log(req + " info: " + this.responseText);
            var result = null;
            try {
                if (this.responseText != "") {
                    var obj = JSON.parse(this.responseText);
                    result = obj.packagePath;
                }
            }
            catch (e) {
                defer.reject("An error occurred while getting "
                    + req + " info: IDE server failed: " + this.responseText);
            }
            defer.resolve(result);
        };
    }

    /* request package path from the CCS "ide server" */
    var req = new XMLHttpRequest();
    if (projectName != null && projectName != "") {
        /* get path from project */
        console.log("getting package path from project '" + projectName + "'.");
        req.addEventListener("load", reqListener("project"));
        req.addEventListener("error", transferFailed("project"));
        req.open("GET", location.origin
            + "/ide/getProjectProperties?projectName=" + projectName);
    }
    else {
        /* get path from workspace */
        console.log("getting package path from workspace");
        req.addEventListener("load", reqListener("workspace"));
        req.addEventListener("error", transferFailed("workspace"));
        req.open("GET", location.origin + "/ide/getBuildPreferences");
    }
    req.send();

    return (defer.promise);
}

/*
 *  ======== _getSymbols ========
 *  Return a non-empty set of symbol files for the specified core
 *
 *  If the current set of symbol files is [], wait up to 5s for that to
 *  change.  If it doesn't, fail by rejecting the returned promise.
 */
function _getSymbols(core)
{
    console.log("_getSymbols: request core's symbol files");
    var defered = Q.defer();
    deferedSymbols = defered; /* request symbol files from symbols listener */
    return (core.symbols.getSymbolFiles().then(
        function (syms) {
            if (syms.symbolFiles.length != 0) {
                console.log("_getSymbols: canceled core.symbols request");
                deferedSymbols = null; /* cancel symbols listener request */
                defered.resolve(syms);
            }
            return (defered.promise.timeout(5000,
                        "Can't connect: Program symbols must be loaded to "
                        + "core '" + dsActiveCoreName + "'"));
        }
    ));
}

/*
 *  ======== _parsePackagePath ========
 */
function  _parsePackagePath(packagePath)
{
    if (packagePath != null && packagePath.length > 0) {
        var xdcroot = packagePath[packagePath.length - 1].replace(/packages$/, "");
        var xdcpath = packagePath.slice(0, packagePath.length - 1).join(';');
        return ({
              xdcroot: xdcroot,
              xdcpath: xdcpath
        });
    }
    return ({xdcpath: null, xdcroot:null});
}

/*
 *  ======== _parseExePath ========
 *  Return JSON object with path components split out into distinct items
 *     {
 *       exeName:  non-null String
 *       connType: non-null String
 *       connArgs: Array of 0 or more Strings
 *     }
 *  where, 
 *       exeName  - full path of the executable
 *       connType - connection type: "DSLite", "Serial", "Net", ...
 *       connArgs - array of String values needed by the connection
 *                  or, in the event that none are supplied, []
 * 
 */
function _parseExePath(path)
{
    var result = {};

    /* separate exe from path */
    var tokens = path.split('?');
    result.exeName = tokens[0];

    /* separate connection from name=value pairs */
    var connection = tokens[1].split('&')[0]; 

    /* split connection into type and args */
    tokens = connection.split(':');
    result.connType = tokens.shift();
    result.connArgs = tokens;

    return (result);
}

/*
 *  ======== _setupCore ========
 *  If the debugger is not already configured, we configure it here for the
 *  specified device
 *
 *  This function is only called when we are connecting via DSLite.
 *
 *  dbg     is the a debugger object that has a list of (possibly already)
 *          configured cores.
 *
 *  devName is either "" or a basename (minus extension) of a .ccxml file
 *          that should be used to configure the device; if devName is ""
 *          the debugger should already have been configured.
 *
 *  targetEvents and obj are used to send target events generated by
 *          dbg.cores[0] up to the caller
 *
 *  Returns a promise
 *      resolved to core 0 module with debugger connected and target running
 *  or
 *      rejected because we can't start agent or the connection is
 *                       DSLite and we failed to get the DS submodule
 *
 *  (Internal function)
 */
function _setupCore(dbg, devName, targetEvents, obj)
{
    var prom;
    console.log("configuring one debugger core of " + dbg.cores.length
                + " cores");
    if (dbg.cores.length == 0 && devName != "") {
        /* if the number of cores is 0, core 0 has not already been configured
         * by the user via the debugger or a second instance of ROV; so, we
         * try to do it now using the .ccxml file specified by devName.
         */
        prom = _configureDebugger(devName);
    }
    else {
        var promContainer = Q.defer();
        prom = promContainer.promise;

        if (dbg.cores.length == 0) { /* and devName == "" */
            promContainer.reject(new Error(
                "the debugger has not been configured for any device.  "
                + "Try reconnecting after connecting to the target with "
                + "the debugger."));
        }
        else if (devName != "") {   /* and dbg.cores.length != 0 */
           /* deconfigure closes websockets connected to all cores; so, to
            * properly deconfigure we must restart monserver. For now we
            * skip the deconfigure step until we figure out how to manage
            * the lifecycle of DSLite cores and monserver instances
            */
           promContainer.reject(new Error(
               "the debugger has already been configured for the device.  "
               + "To connect with the existing configuration, don't "
               + "specify a device configuration; use just 'Debugger'."));
        }
        else { /* devName == "" and dbg.cores.length != 0 */
            /* Assume that the debugger has already started for the
             * right configuration.
             */
            console.log("debugger is up for core " 
                        + dbg.cores[dsActiveCoreId]);
            promContainer.resolve(dbg);
        }
    }

    /* at this point prom is either resolved as dbg, or rejected */
    return prom.then(
        function(dbg) {
            /* select the currently active debug core */
            var cname = dbg.cores[dsActiveCoreId];
            console.log("adding listeners to core[" + dsActiveCoreId 
                        + "] = '" + cname + "'");

            return ds.getSubModule(cname).then(
                /* add listeners to active core */
                function(debugCore) {
                    console.log("adding target state halt listener");
                    debugCore.targetState.waitForHalt(
                        function(halted) {
                            debuggerStatus = "TargetHalt";
                            console.log("targetState halted");
                        }
                    );

                    debugCore.addListener("refresh",
                        function(status) {
                            console.log("core 'refresh' status: "
                                + JSON.stringify(status, null, 3));
                            debuggerStatus = status.reason;
                            if (targetEvents) {
                                targetEvents(debuggerStatus, obj);
                            }
                        }
                    );

                    console.log("adding target state change listener");
                    debugCore.targetState.addListener("changed",
                        function(status) {
                            console.log("targetState status: "
                                + JSON.stringify(status, null, 3));
                            if (status.description == "Running") {
                                debuggerStatus = "Running";
                            }
                            else if (status.description.match(/^Disconnected/))
                            {
                                debuggerStatus = "Disconnected";
                            }
                        }
                    );

                    console.log("adding target symbols change listener");
                    debugCore.symbols.addListener("changed",
                        function(status) {
                            console.log("core.symbols status: "
                                + JSON.stringify(status, null, 3));

                            /* resolve non-empty symbol file requests */
                            if (deferedSymbols
                                && status.symbolFiles
                                && status.symbolFiles.length != 0) {

                                deferedSymbols.resolve(status);
                                deferedSymbols = null;
                                console.log("core.symbols resolved request");
                            }

                            /* trigger ROV reloads (if necessary) */
                            if (exeSet != null
                                && status.symbolFiles
                                && status.symbolFiles.length == 0) {

                                exeSet = null;
                                location.reload(true);
                            }
                        }
                    );

                    return (debugCore);
                }
            ).then(
               /* connect to and start "active" debug core */
                function(debugCore) {
                    var alreadyConnected = false;

                    console.log("connecting to active core");
                    return debugCore.targetState.connect().then(
                        function() {
                            debuggerStatus = "Connected";
                        }
                     ).fail(
                         function(err) {
                             if (err.message != undefined
                                 && err.message ==
                                     "Target is already connected") {
                                 /* If we made it here with this error message,
                                  * it means the user already started debugger
                                  * so we are not going to call run().  We
                                  * let the user control ROV via the debugger.
                                  */
                                 debuggerStatus = "Connected";
                                 alreadyConnected = true;
                             }
                             else {
                                 console.log("connect failed: "
                                     + (err.message ? err.message : err));
                                 throw (err);
                             }
                         }
                     ).then(
                         function() {
                            if (alreadyConnected == false) {
                                /* try starting the core, but ignore fails */
                                console.log("starting: " + debugCore);
                                debugCore.targetState.run().fail(
                                    function (err) {
                                        var msg = 
                                            err.message ? err.message : err;
                                        console.log(debugCore
                                            + " could not be started: " + msg);
                                    }
                                );
                            }

                            return (debugCore);
                         }
                     );
                }
            );
        }
    ).
    fail(
        /* handle the case where original prom was rejected */
        function(err) {
            var msg = "Debugger setup failed: "
                + (err.message ? err.message : err);
            console.log(msg);
            throw new Error(msg);
        }
    );
}

/*
 *  ======== _validateConnection ========
 */
function _validateConnection(conn)
{
    console.log("_validateConnection(" + conn + ")");

    if (conn.indexOf("comm=DSLite") == 0) {
        var prefix = conn;
        var suffix = "";

        /* strip off any extra name-value pairs from conn */
        var k = conn.indexOf('&');
        if (k >= 0) {
            prefix = conn.substring(0, k);
            suffix = conn.substring(k);
        }

        /* update the DSLite connection's first argument */
        var tokens = prefix.split(':');
        if (tokens[1] == "undefined") {
            tokens[1] = dsActiveCoreId;
        }
        prefix = tokens.join(':');

        /* reconstruct the connection string */
        conn = prefix + suffix;
    }

    return conn;
}

/*
 *  ======== _validateExe ========
 *  cleanup Exe path before sending to server
 */
function _validateExe(path, files)
{
    console.log("_validateExe(" + path + ", [" + files.symbolFiles + "])");

    var tokens = path.split('?');  /* split exe path from the connection */
    var fpath = tokens[0].replace(/\\/g, '/');

    /* update the connection arguments for the server */
    var connection = _validateConnection(tokens[1]);
    var suffix = '?' + connection;

    /* update the exe path part */
    var dpath = "";
    if (files.symbolFiles.length > 0) {
        dpath = files.symbolFiles[0].replace(/\\/g, '/');
    }

    /* check for miracles */
    if (dpath == fpath)  {
        return (dpath + suffix);
    }

    /* take debugger's path if it's set and nothing else if specified */
    if (dpath != "" && (fpath == "undefined" || fpath == "")) {
        return (dpath + suffix);
    }

    /* take specified path if it's set and the debugger is clueless */
    if (dpath == "" && (fpath != "undefined" && fpath != "")) {
        return (fpath + suffix);
    }

    /* else fail */
    var msg = "can't determine the executable path from input '"
                  + fpath + "' and debugger's '" + dpath + "'";

    if (dpath != "" && fpath != "undefined" && fpath != "") {
        msg = "debugger loaded " + dpath
                  + "' but '" + fpath + "' was specified.";
    }
    else if (dpath == "" && (fpath == "undefined" || fpath == "")) {
        msg = "no executable path was specifed.  Either specify the path to the executable or, if connected via CCS, load program symbols.";
    }

    throw (new Error(msg));
}
