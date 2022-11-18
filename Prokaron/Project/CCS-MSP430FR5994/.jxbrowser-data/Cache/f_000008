/*global chrome*/
/*global console*/
/*global InstallTrigger*/
/*global process*/
/*global require*/
/*global Q*/
// use jsdoc agent.js to generate documentation
var __extends = (this && this.__extends) || function (d, b) {
    for (var p in b) if (b.hasOwnProperty(p)) d[p] = b[p];
    function __() { this.constructor = d; }
    d.prototype = b === null ? Object.create(b) : (__.prototype = b.prototype, new __());
};
var TICloudAgent;
(function (TICloudAgent) {
    // base error
    var Error = (function () {
        function Error(name, msg) {
            if (name === void 0) { name = "Error"; }
            if (msg === void 0) { msg = "ERROR"; }
            this.name = name;
            this.msg = msg;
        }
        return Error;
    }());
    TICloudAgent.Error = Error;
    var AlreadyInitialized = (function (_super) {
        __extends(AlreadyInitialized, _super);
        function AlreadyInitialized() {
            return _super.call(this, "AlreadyInitialized", "AGENT_ALREADY_INITIALIZED") || this;
        }
        return AlreadyInitialized;
    }(Error));
    TICloudAgent.AlreadyInitialized = AlreadyInitialized;
    // Base for installation related errors.
    var InstallError = (function (_super) {
        __extends(InstallError, _super);
        function InstallError() {
            return _super.apply(this, arguments) || this;
        }
        return InstallError;
    }(Error));
    TICloudAgent.InstallError = InstallError;
    // Base error type that may require installation
    var InvalidAgentVersion = (function (_super) {
        __extends(InvalidAgentVersion, _super);
        function InvalidAgentVersion() {
            return _super.call(this, "InvalidAgentVersion", "Installed version is out of date") || this;
        }
        return InvalidAgentVersion;
    }(Error));
    TICloudAgent.InvalidAgentVersion = InvalidAgentVersion;
    var InvalidExtensionVersion = (function (_super) {
        __extends(InvalidExtensionVersion, _super);
        function InvalidExtensionVersion() {
            return _super.call(this, "InvalidExtensionVersion", "Installed browser extension version is out of date") || this;
        }
        return InvalidExtensionVersion;
    }(Error));
    TICloudAgent.InvalidExtensionVersion = InvalidExtensionVersion;
    var MissingExtension = (function (_super) {
        __extends(MissingExtension, _super);
        function MissingExtension() {
            return _super.call(this, "MissingExtension", "MISSING_EXTENTSION") || this;
        }
        return MissingExtension;
    }(Error));
    TICloudAgent.MissingExtension = MissingExtension;
    var AgentNotStarted = (function (_super) {
        __extends(AgentNotStarted, _super);
        function AgentNotStarted(msg) {
            if (msg === void 0) { msg = "AGENT_NOT_STARTED"; }
            return _super.call(this, "AgentNotStarted", msg) || this;
        }
        return AgentNotStarted;
    }(Error));
    TICloudAgent.AgentNotStarted = AgentNotStarted;
    // OS Enum
    TICloudAgent.OS = {
        WIN: "win",
        LINUX: "linux",
        OSX: "osx",
    };
    // Browser Enum
    TICloudAgent.BROWSER = {
        CHROME: "chrome",
        SAFARI: "safari",
        FIREFOX: "firefox",
        IE: "ie",
    };
    function dynamicLink(url, newWindow) {
        if (newWindow === void 0) { newWindow = false; }
        var browser = getBrowser();
        var appendChild = browser === TICloudAgent.BROWSER.FIREFOX || browser === TICloudAgent.BROWSER.IE;
        var a = document.createElement("a");
        a.href = url;
        if (newWindow) {
            a.target = "_blank";
        }
        if (appendChild) {
            document.body.appendChild(a);
        }
        a.click();
    }
    // utility function to figure out the browser
    function getBrowser() {
        var browser = TICloudAgent.BROWSER.CHROME;
        // chrome claims to be safari and chrome.. so special care is needed
        if (navigator.userAgent.indexOf("Safari") !== -1 && navigator.userAgent.indexOf("Chrome") === -1) {
            browser = TICloudAgent.BROWSER.SAFARI;
        }
        else if (navigator.userAgent.indexOf("Firefox") !== -1) {
            browser = TICloudAgent.BROWSER.FIREFOX;
        }
        else if (!!navigator.userAgent.match(/Trident.*rv\:11\./)) {
            browser = TICloudAgent.BROWSER.IE;
        }
        return browser;
    }
    TICloudAgent.getBrowser = getBrowser;
    // utility function to figure out OS
    function getOS() {
        // default to linux because it is not always possible to tell it from
        // the
        // appVersion
        var os = TICloudAgent.OS.LINUX;
        if (navigator.appVersion.indexOf("Mac") !== -1) {
            os = TICloudAgent.OS.OSX;
        }
        if (navigator.appVersion.indexOf("Win") !== -1) {
            os = TICloudAgent.OS.WIN;
        }
        return os;
    }
    TICloudAgent.getOS = getOS;
    ;
    function assertNever(x) {
        throw new Error("Unexpected object: " + x);
    }
    function getOSFolderName() {
        var os = getOS();
        switch (os) {
            case "win": return "win32";
            case "osx": return "mac";
            case "linux": return "linux";
            default: return assertNever(os);
        }
    }
    var hostInfo;
    (function (hostInfo) {
        // Host and port of the server
        var protocol = window.location.protocol;
        var host = window.location.hostname;
        var port = window.location.port ? ":" + window.location.port : "";
        // override - may be useful for testing
        // explicitly check for undefined, empty strings are valid overrides
        var hackOfWindowForTesting = window;
        if (typeof hackOfWindowForTesting.TI_CLOUD_AGENT_HOST !== "undefined") {
            host = hackOfWindowForTesting.TI_CLOUD_AGENT_HOST;
        }
        if (typeof hackOfWindowForTesting.TI_CLOUD_AGENT_PORT !== "undefined") {
            port = hackOfWindowForTesting.TI_CLOUD_AGENT_PORT;
        }
        function tiCloudAgentServer() {
            return protocol + "//" + host + port + "/ticloudagent";
        }
        hostInfo.tiCloudAgentServer = tiCloudAgentServer;
        function isProduction() {
            return (host === "dev.ti.com");
        }
        hostInfo.isProduction = isProduction;
    })(hostInfo || (hostInfo = {}));
    function isOfflineConfig(agentConfig) {
        return undefined !== agentConfig.offline;
    }
    function isDesktopConfig(agentConfig) {
        return undefined !== agentConfig.agentPort;
    }
    var loadAgentConfig;
    if (typeof process === "undefined") {
        loadAgentConfig = function () {
            var deferred = Q.defer();
            var xmlhttp = new XMLHttpRequest();
            var url = hostInfo.tiCloudAgentServer() + "/agent_config.json";
            xmlhttp.onreadystatechange = function () {
                if (xmlhttp.readyState === 4) {
                    if (xmlhttp.status === 200) {
                        deferred.resolve(JSON.parse(xmlhttp.responseText));
                    }
                    else {
                        deferred.reject(xmlhttp.statusText);
                    }
                }
            };
            xmlhttp.open("GET", url); // sync
            xmlhttp.send();
            return deferred.promise;
        };
    }
    else {
        loadAgentConfig = function () {
            var deferred = Q.defer();
            try {
                // In the node case, agent_config.json contains the path to 
                // ticloudagent, and not version information
                var path = require("path");
                var agentConfigData = require(path.resolve("../ticloudagent/server/public/agent_config.json"));
                deferred.resolve(agentConfigData);
            }
            catch (e) {
                console.warn("No agent_config.json found, switching to default config");
                deferred.resolve();
            }
            return deferred.promise;
        };
    }
    var Extension = (function () {
        function Extension() {
        }
        Extension.prototype.isInstalled = function () {
            var extDomItem = document.getElementById("com.ti.TICloudAgent.Bridge");
            return extDomItem ? true : false;
        };
        Extension.prototype.getVersion = function () {
            var extDomItem = document.getElementById("com.ti.TICloudAgent.Bridge");
            return extDomItem.title;
        };
        return Extension;
    }());
    // chrome ext specific
    var ChromeExt = (function (_super) {
        __extends(ChromeExt, _super);
        function ChromeExt() {
            var _this = _super.apply(this, arguments) || this;
            _this.ID = "pfillhniocmjcapelhjcianojmoidjdk";
            _this.port = null;
            _this.lastRegLisenter = null;
            return _this;
        }
        ChromeExt.prototype.init = function () {
            var _this = this;
            // connect to app if possible
            if (chrome.runtime && chrome.runtime.connect) {
                this.port = chrome.runtime.connect(this.ID);
                this.port.onDisconnect.addListener(function () {
                    _this.port = null;
                });
            }
        };
        ChromeExt.prototype.postMessage = function (msgObj) {
            this.port.postMessage(msgObj);
        };
        ChromeExt.prototype.regOnMessage = function (listener) {
            this.lastRegLisenter = listener;
            this.port.onMessage.addListener(this.lastRegLisenter);
        };
        ChromeExt.prototype.removeLastOnMessage = function () {
            this.port.onMessage.removeListener(this.lastRegLisenter);
        };
        ChromeExt.prototype.install = function () {
            var failure = function (msg) {
                console.error("Extension Installation Failed: " + msg);
            };
            var url = "https://chrome.google.com/webstore/detail/" + this.ID;
            // scheme is very delicate make sure to test all the use cases if you make changes
            if (hostInfo.isProduction()) {
                try {
                    // inline- install only works for production
                    chrome.webstore.install(url, function () { }, failure);
                }
                catch (e) {
                    dynamicLink(url, true); // do non inline install instead
                }
            }
            else {
                dynamicLink(url, true); // do non inline install instead
            }
        };
        return ChromeExt;
    }(Extension));
    var DOMEventExt = (function (_super) {
        __extends(DOMEventExt, _super);
        function DOMEventExt() {
            var _this = _super.apply(this, arguments) || this;
            _this.TO_APP_MSG_STR = "ti-cloud-agent-msg-app";
            _this.TO_EXT_MSG_STR = "ti-cloud-agent-msg-ext";
            _this.lastRegListener = null;
            return _this;
        }
        DOMEventExt.prototype.postMessage = function (msgObj) {
            var event = window.document.createEvent("CustomEvent");
            event.initCustomEvent(this.TO_EXT_MSG_STR, true, true, msgObj);
            window.dispatchEvent(event);
        };
        DOMEventExt.prototype.regOnMessage = function (listener) {
            this.lastRegListener = function (event) {
                listener(event.detail);
            };
            window.addEventListener(this.TO_APP_MSG_STR, this.lastRegListener, false);
        };
        DOMEventExt.prototype.removeLastOnMessage = function () {
            window.removeEventListener(this.TO_APP_MSG_STR, this.lastRegListener, false);
        };
        return DOMEventExt;
    }(Extension));
    var FirefoxExt = (function (_super) {
        __extends(FirefoxExt, _super);
        function FirefoxExt() {
            return _super.apply(this, arguments) || this;
        }
        FirefoxExt.prototype.init = function (newconfig) {
            this.config = newconfig;
        };
        FirefoxExt.prototype.install = function () {
            var extUrl = hostInfo.tiCloudAgentServer() + "/getFile/" + getOS() + "/ticloudagent/extensions/";
            extUrl += "firefox/ticloudagentbridge.xpi/LATEST";
            var params = {
                TICloudAgentXPI: {
                    URL: extUrl,
                    Hash: this.config.hash,
                    toString: function () {
                        return extUrl;
                    },
                },
            };
            InstallTrigger.install(params);
        };
        return FirefoxExt;
    }(DOMEventExt));
    var SafariExt = (function (_super) {
        __extends(SafariExt, _super);
        function SafariExt() {
            return _super.apply(this, arguments) || this;
        }
        SafariExt.prototype.init = function () {
        };
        SafariExt.prototype.install = function () {
            var extUrl = hostInfo.tiCloudAgentServer() + "/getFile/" + getOS() + "/ticloudagent/extensions/";
            extUrl += "safari/ticloudagentbridge.safariextz/LATEST";
            dynamicLink(extUrl, false);
        };
        return SafariExt;
    }(DOMEventExt));
    function createExtension(browser) {
        switch (browser) {
            case "chrome": return new ChromeExt();
            case "firefox": return new FirefoxExt();
            case "safari": return new SafariExt();
            default: return null;
        }
    }
    // for firefox and safari use the firefoxExt object.
    var browserExt = createExtension(getBrowser());
    // The socket abstraction for communicating with the host agent
    // tslint:disable-next-line variable-name - it's a class
    var Socket = WebSocket;
    function launchHostApp(agentConfig) {
        // abstraction for interfacting with the host app
        function initExtensionBased(extAgentConfig, browser) {
            // message types for the init TI cloud agent
            var EXT_MSG_TYPES = {
                INIT: "INIT_EXTENTION",
                INIT_COMPLETE: "INIT_EXTENTION_COMPLETE",
            };
            var POST_MESSAGE_TYPES = {
                CREATE: "CREATE",
                CLOSE: "CLOSE",
                SEND: "SEND",
            };
            var ON_MESSAGE_TYPES = {
                ON_ERROR: "ON_ERROR",
                ON_CLOSE: "ON_CLOSE",
                ON_MESSAGE: "ON_MESSAGE",
                ON_OPEN: "ON_OPEN",
            };
            var typeToFunc = {
                ON_ERROR: "onerror",
                ON_CLOSE: "onclose",
                ON_MESSAGE: "onmessage",
                ON_OPEN: "onopen",
            };
            var SocketToExtension = (function () {
                function SocketToExtension(url, subProtocol) {
                    this.url = url;
                    this.subProtocol = subProtocol;
                    this.key = SocketToExtension.id++;
                    SocketToExtension.socketCache[this.key] = this;
                    browserExt.postMessage({
                        key: this.key,
                        url: this.url,
                        subProtocol: this.subProtocol,
                        type: POST_MESSAGE_TYPES.CREATE,
                    });
                }
                SocketToExtension.dispatchSocketEvent = function (msgObj) {
                    var funcName = typeToFunc[msgObj.type];
                    var key = msgObj.key;
                    var socket = SocketToExtension.socketCache[key];
                    if (socket && socket[funcName]) {
                        var func = socket[funcName];
                        func.apply(socket, [msgObj.msgEvt]);
                    }
                    if (msgObj.type === ON_MESSAGE_TYPES.ON_CLOSE) {
                        if (SocketToExtension.socketCache[key]) {
                            delete SocketToExtension.socketCache[key];
                        }
                    }
                };
                SocketToExtension.prototype.close = function () {
                    // Do not rely on the extension sending us a close event.  Instead,
                    // ignore all future events by removing ourself from the cache, and 
                    // then send our own close event.
                    // See TICLD-1664 
                    delete SocketToExtension.socketCache[this.key];
                    browserExt.postMessage({
                        key: this.key,
                        type: POST_MESSAGE_TYPES.CLOSE,
                    });
                    if (this.onclose) {
                        this.onclose();
                    }
                };
                SocketToExtension.prototype.send = function (data) {
                    browserExt.postMessage({
                        key: this.key,
                        data: data,
                        type: POST_MESSAGE_TYPES.SEND,
                    });
                };
                return SocketToExtension;
            }());
            SocketToExtension.id = 0;
            SocketToExtension.socketCache = {};
            var deferred = Q.defer();
            try {
                Socket = SocketToExtension;
                var errors_1 = []; // list of issues while starting up
                browserExt.init(extAgentConfig[browser]);
                // browser ext is not installed return error
                // we can't do anything else
                if (!browserExt.isInstalled()) {
                    errors_1.push(new MissingExtension());
                    deferred.reject(errors_1);
                    return deferred.promise;
                }
                var EXT_VERSION = extAgentConfig[browser].version;
                // if the browser ext verision does not match, we can still try to start the agent
                if (browserExt.getVersion() !== EXT_VERSION) {
                    errors_1.push(new InvalidExtensionVersion());
                }
                function initEventListener(msgObj) {
                    if (msgObj.type === EXT_MSG_TYPES.INIT_COMPLETE) {
                        var cleanup = null;
                        browserExt.removeLastOnMessage(); // stop listening to the init event
                        if (msgObj.data.error) {
                            // there was an issues starting the agent
                            errors_1.push(new AgentNotStarted(msgObj.data.error));
                        }
                        else if (msgObj.data.version !== extAgentConfig.installer.version) {
                            // we started fine, but the version was invalid
                            errors_1.push(new InvalidAgentVersion());
                            // The agent will remain running until something
                            // connects to it.  To prevent that, we'll connect
                            // but immediately close the agent
                            browserExt.regOnMessage(SocketToExtension.dispatchSocketEvent);
                            cleanup = createClientModule(msgObj.data.port)
                                .then(function (agent) { return agent.close(); });
                        }
                        if (errors_1.length === 0) {
                            // expect all other messages to be for the
                            // socket interface
                            browserExt.regOnMessage(SocketToExtension.dispatchSocketEvent);
                            deferred.resolve(msgObj.data);
                        }
                        else {
                            if (cleanup) {
                                cleanup.finally(function () { return deferred.reject(errors_1); });
                            }
                            else {
                                deferred.reject(errors_1);
                            }
                        }
                    }
                    else {
                        console.error("UNEXPECTED MESSAGE TYPE: " + msgObj.type);
                    }
                }
                // set up listeners
                browserExt.regOnMessage(initEventListener);
                // send message to init
                browserExt.postMessage({ type: EXT_MSG_TYPES.INIT });
            }
            catch (e) {
                deferred.reject(e);
            }
            return deferred.promise;
        }
        function initIE() {
            var deferred = Q.defer();
            var plugin = window.TICloudAgent_HACK.plugin;
            var errors = [];
            if (plugin && plugin.start) {
                var initParamsRaw = plugin.start();
                var initParams = JSON.parse(initParamsRaw);
                if (initParams.error) {
                    errors.push(new AgentNotStarted(initParams.error));
                    deferred.reject(errors);
                }
                else {
                    deferred.resolve(initParams);
                }
            }
            else {
                errors.push(new MissingExtension());
                deferred.reject(errors);
            }
            return deferred.promise;
        }
        function initGUIComposer() {
            var osFolderName = getOSFolderName();
            var isWin = /^win/.test(process.platform);
            var ext = isWin ? ".bat" : ".sh";
            var spawn = require("child_process").spawn;
            var fileToExec = "ticloudagent" + ext;
            var pathObj = require("path");
            var fs = require("fs");
            var workingDir = pathObj.dirname(process.cwd());
            // folder structure is <install directory>/.../<app name>
            // need <install directory>/win32/TICloudAgent
            // start by going up to parent folder
            var subdirectoryLimit = 3;
            workingDir = pathObj.join(workingDir, osFolderName);
            do {
                workingDir = pathObj.join(workingDir, "../..");
                var dirName = "" + workingDir.toLowerCase();
                // if current folder name doesn't already end in the OS name, add it
                if ((dirName.indexOf(osFolderName, dirName.length - osFolderName.length) === -1)) {
                    workingDir = pathObj.join(workingDir, osFolderName);
                }
            } while (!fs.existsSync(workingDir) && subdirectoryLimit-- > 0);
            workingDir = pathObj.join(workingDir, "TICloudAgent");
            fileToExec = pathObj.join(workingDir, fileToExec);
            var initParams = null;
            var errors = [];
            var deferred = Q.defer();
            var lp = spawn(fileToExec, ["not_chrome"], { cwd: workingDir });
            lp.stdout.on("data", function (data) {
                if (initParams) {
                    throw new Error("AREADY INITIALIZED");
                }
                initParams = JSON.parse(data.toString());
                deferred.resolve(initParams);
            });
            lp.stderr.on("data", function (data) {
                errors.push(new AgentNotStarted(data.toString()));
                deferred.reject(errors);
            });
            return deferred.promise;
        }
        function initGenericNW(configFile) {
            var path = require("path");
            var hostAgentStart = require(path.resolve(configFile.offline.hostAgentPath)).start;
            return hostAgentStart();
        }
        if (!agentConfig) {
            return initGUIComposer();
        }
        else if (isOfflineConfig(agentConfig)) {
            return initGenericNW(agentConfig);
        }
        else {
            // Running in a browser of some kind
            if (isDesktopConfig(agentConfig)) {
                // Agent already launched, check for error, then return port
                if (agentConfig.error) {
                    return Q.reject(agentConfig.error);
                }
                return Q({ port: agentConfig.agentPort });
            }
            else {
                var browser = getBrowser();
                switch (browser) {
                    case "ie": return initIE();
                    case "chrome":
                    case "firefox":
                    case "safari": return initExtensionBased(agentConfig, browser);
                    default: return assertNever(browser);
                }
            }
        }
    }
    var InstallWizard = (function () {
        function InstallWizard(errors, connectionID) {
            this.title = "TI Cloud Agent Setup";
            this.detailsLink = {
                text: "What's this?",
                url: "http://processors.wiki.ti.com/index.php/TI_Cloud_Agent#What_is_it.3F",
            };
            this.helpLink = {
                text: "Help. I already did this",
                url: "http://processors.wiki.ti.com/index.php/TI_Cloud_Agent#Troubleshooting",
            };
            this.finishStep = {
                description: "Refresh the current browser page",
                action: {
                    text: "$Refresh$ Page",
                    handler: function () {
                        window.location.reload();
                    },
                },
            };
            this.initialMessage = {
                description: "Install TI Cloud Agent to enable flashing.",
                action: {
                    text: "Install TI Cloud Agent to enable flashing.",
                },
            };
            this.description = InstallWizard.getDescriptionText(errors);
            this.steps = InstallWizard.getSteps(errors, connectionID);
        }
        InstallWizard.getDescriptionText = function (errors) {
            var wizardDesc = "Obsolete TI Cloud Agent installation found. An update is required.";
            for (var _i = 0, errors_2 = errors; _i < errors_2.length; _i++) {
                var error = errors_2[_i];
                if (error instanceof MissingExtension) {
                    wizardDesc = "Hardware interaction requires additional one time set up.";
                    break;
                }
                if (error instanceof AgentNotStarted) {
                    wizardDesc = "Could not launch TI Cloud Agent : " + error.msg + " .";
                }
            }
            wizardDesc += " Please perform the actions listed below and try your operation again.";
            return wizardDesc;
        };
        InstallWizard.createInstallAgentStep = function (connectionID, error) {
            var actionText = "$Download$ and install the TI Cloud Agent Application";
            if (error instanceof InvalidAgentVersion) {
                actionText = "$Download$ and install a new version of the TI Cloud Agent Application";
            }
            var step = {
                description: "Download and install the TI Cloud Agent host application.",
                action: {
                    text: actionText,
                    handler: function () {
                        var url = hostInfo.tiCloudAgentServer() + "/getInstaller" + "?os=" + getOS();
                        if (undefined !== connectionID) {
                            url += "&connectionID=" + connectionID;
                        }
                        dynamicLink(url);
                    },
                },
            };
            return step;
        };
        InstallWizard.getSteps = function (errors, connectionID) {
            var stepInstallExt = {
                description: "Install the TI Cloud Agent browser extension.",
                action: {
                    text: "$Install$ browser extension",
                    handler: function () { browserExt.install(); },
                },
            };
            var steps = [];
            var browser = getBrowser();
            for (var _i = 0, errors_3 = errors; _i < errors_3.length; _i++) {
                var error = errors_3[_i];
                // on I.E there is no inline extension install.. its all done by the host app installer
                if (browser === TICloudAgent.BROWSER.IE && error instanceof MissingExtension) {
                    steps.push(InstallWizard.createInstallAgentStep(connectionID));
                }
                else if (error instanceof MissingExtension) {
                    steps.push(stepInstallExt);
                    steps.push(InstallWizard.createInstallAgentStep(connectionID));
                }
                else if (error instanceof AgentNotStarted || error instanceof InvalidAgentVersion) {
                    steps.push(InstallWizard.createInstallAgentStep(connectionID, error));
                }
                else if (error instanceof InvalidExtensionVersion) {
                    steps.push(stepInstallExt);
                }
            }
            // safari. we also need to restart the browser
            if (browser === TICloudAgent.BROWSER.SAFARI) {
                steps.push({
                    description: "Restart the browser",
                    action: {
                        text: "Restart the browser",
                    },
                });
            }
            return steps;
        };
        return InstallWizard;
    }());
    // make it async, to be consistent with all other API's			
    // We have to keep this poor name for now due to it's use throughout
    // tslint:disable-next-line variable-name
    TICloudAgent.Install = {
        getInstallWizard: function (params) {
            var deferred = Q.defer();
            deferred.resolve(new InstallWizard(params.errors, params.connectionID));
            return deferred.promise;
        },
    };
    // create the client side module
    function createClientModule(port, subProtocol) {
        // add events related functions
        var eventListeners = {};
        var moduleObj = {
            addListener: function (type, listener) {
                if (!eventListeners[type]) {
                    eventListeners[type] = [];
                }
                eventListeners[type].push(listener);
            },
            removeListener: function (type, listener) {
                if (eventListeners[type]) {
                    var typeListeners = eventListeners[type];
                    // tslint:disable-next-line prefer-for-of - index is needed
                    for (var i = 0; i < typeListeners.length; i++) {
                        if (typeListeners[i] === listener) {
                            // don't remove it, just null it out
                            // if we remove it and the remove was called from a dispatch
                            // it could impact the dispatch because the length of eventsListeners will change
                            typeListeners[i] = null;
                            break;
                        }
                    }
                }
            },
            getSubModule: null,
            close: null,
        };
        var internalModuleObj = {
            createSubModule: null,
            listCommands: null,
        };
        // command dispatch module
        function createCommandDispatch() {
            var commandID = 1; // start from 1 ( 0 could be mistaken for false, in certain places)
            var pendingCommands = {};
            var rejectMsg = { message: "Module Closed" };
            var commandDispatchObj = {
                exec: function (ws, commandName, data) {
                    var defCommand = Q.defer();
                    var obj = {
                        command: commandName,
                        id: commandID++,
                        data: data,
                    };
                    pendingCommands[obj.id] = defCommand;
                    var message = JSON.stringify(obj);
                    try {
                        ws.send(message);
                    }
                    catch (e) {
                        defCommand.reject(rejectMsg);
                    }
                    return defCommand.promise;
                },
                ret: function (retObj) {
                    var response = retObj.response;
                    var error = retObj.error;
                    var data = retObj.data;
                    // it's should only be one of these
                    var id = response || error;
                    var defCommand = pendingCommands[id];
                    if (defCommand) {
                        if (response) {
                            defCommand.resolve(data);
                        }
                        else {
                            defCommand.reject(data);
                        }
                        // delete it instaed of nulling so the map doesn't grow too large over time
                        delete pendingCommands[id];
                    }
                    else {
                        console.error("commandDispatch : ret , Error, no promise found corresponding to id : " + id);
                    }
                },
            };
            function cleanUp() {
                // reject all outstanding requests
                for (var key in pendingCommands) {
                    if (pendingCommands.hasOwnProperty(key)) {
                        var commandPromise = pendingCommands[key];
                        commandPromise.reject(rejectMsg);
                    }
                }
                pendingCommands = {};
                // replace exec and ret to reject and ignore incoming requests
                commandDispatchObj.exec = function () {
                    var defCommand = Q.defer();
                    defCommand.reject(rejectMsg);
                    return defCommand.promise;
                };
                commandDispatchObj.ret = function () {
                    // do nothing
                    return;
                };
            }
            moduleObj.addListener("close", cleanUp);
            return commandDispatchObj;
        }
        var commandDispatch = createCommandDispatch();
        var eventsDispatch = {
            dispatch: function (listeners, retObj) {
                var typeListeners = listeners[retObj.event];
                if (typeListeners) {
                    for (var _i = 0, typeListeners_1 = typeListeners; _i < typeListeners_1.length; _i++) {
                        var listener = typeListeners_1[_i];
                        if (listener) {
                            listener(retObj.data);
                        }
                    }
                }
            },
        };
        function socketUrl() {
            return "ws://127.0.0.1:" + port;
        }
        var subModulePromises = {};
        moduleObj.getSubModule = function (subModuleName) {
            var subModulePromise = subModulePromises[subModuleName];
            if (!subModulePromise) {
                subModulePromise = internalModuleObj.createSubModule(subModuleName)
                    .then(function (data) {
                    return createClientModule(data.port, data.subProtocol);
                })
                    .then(function (subModule) {
                    // lets register for an onclose and onerror events to clean ourselves up
                    function cleanUp() {
                        subModulePromises[subModuleName] = null;
                    }
                    subModule.addListener("close", cleanUp);
                    return subModule; // pass the module down the chain
                })
                    .catch(function (err) {
                    subModulePromises[subModuleName] = null;
                    throw err;
                });
                subModulePromises[subModuleName] = subModulePromise;
            }
            return subModulePromise;
        };
        function createCommand(ws, fullCommandName) {
            // add namespace
            var commandNameParts = fullCommandName.split(".");
            // everything up to the last part is part of the namespace
            var parentObj = moduleObj;
            // keep track of nested namespaces
            var parentNamespace = "";
            function createAddListener(eventTypePrefix) {
                return function (type, listener) {
                    // add name spaces
                    type = eventTypePrefix + type;
                    moduleObj.addListener(type, listener);
                };
            }
            function createRemoveListener(eventTypePrefix) {
                return function (type, listener) {
                    // add name spaces
                    type = eventTypePrefix + type;
                    moduleObj.removeListener(type, listener);
                };
            }
            for (var i = 0; i < commandNameParts.length - 1; i++) {
                var currentNamespacePart = commandNameParts[i];
                parentNamespace += commandNameParts[i];
                var newObj = parentObj[currentNamespacePart];
                if (!newObj) {
                    // lets create it
                    var eventTypePrefix = parentNamespace + ".";
                    newObj = {
                        addListener: createAddListener(eventTypePrefix),
                        removeListener: createRemoveListener(eventTypePrefix),
                    };
                }
                // it becomes the new parent
                parentObj[currentNamespacePart] = newObj;
                parentObj = newObj;
                parentNamespace = parentNamespace + ".";
            }
            var commandName = commandNameParts[commandNameParts.length - 1];
            if ("createSubModule" === fullCommandName || "listCommands" === fullCommandName) {
                parentObj = internalModuleObj;
            }
            function theCommand() {
                var data = Array.prototype.slice.call(arguments);
                return commandDispatch.exec(ws, fullCommandName, data);
            }
            parentObj[commandName] = theCommand;
        }
        var setUpDef = Q.defer();
        var ws = subProtocol ? new Socket(socketUrl(), subProtocol) : new Socket(socketUrl());
        function pageUnloadHandler() {
            ws.close();
        }
        ws.onclose = function () {
            setUpDef.reject("socket closed");
            eventsDispatch.dispatch(eventListeners, {
                event: "close",
                data: {
                    message: "socket closed",
                },
            });
            // remove the listener
            window.removeEventListener("unload", pageUnloadHandler);
        };
        ws.onerror = function () {
            setUpDef.reject("socket error");
            eventsDispatch.dispatch(eventListeners, {
                event: "error",
                data: {
                    message: "socket error",
                },
            });
        };
        ws.onopen = function () {
            try {
                // close the socket before unloading to clean up agent
                window.addEventListener("unload", pageUnloadHandler);
                // set up command and event return messages
                ws.onmessage = function (msgEvt) {
                    var retObj = JSON.parse(msgEvt.data);
                    if (retObj.event) {
                        eventsDispatch.dispatch(eventListeners, retObj);
                    }
                    else {
                        commandDispatch.ret(retObj);
                    }
                };
                moduleObj.close = function () {
                    ws.close();
                    return Q.when();
                };
                createCommand(ws, "listCommands"); // everymodule should have a listCommands command
                internalModuleObj.listCommands()
                    .then(function (dataObj) {
                    // create commands
                    for (var _i = 0, _a = dataObj.commands; _i < _a.length; _i++) {
                        var command = _a[_i];
                        if (command !== "listCommands") {
                            createCommand(ws, command);
                        }
                    }
                    setUpDef.resolve(moduleObj);
                })
                    .catch(function (err) { return setUpDef.reject(err); });
            }
            catch (err) {
                setUpDef.reject(err);
            }
        };
        return setUpDef.promise;
    }
    var cachedInit;
    function Init() {
        if (!cachedInit) {
            cachedInit = loadAgentConfig()
                .then(function (agentConfig) {
                return launchHostApp(agentConfig);
            })
                .then(function (initParams) {
                return createClientModule(initParams.port);
            })
                .then(function (agent) {
                // Post creation configuration
                return agent.addConfigProperty("cloudAgentInstallerServerURL", hostInfo.tiCloudAgentServer())
                    .then(function () {
                    return agent;
                });
            });
        }
        return cachedInit;
    }
    TICloudAgent.Init = Init;
    // hack API to figure out weather a target only supports flashing.
    // it is the only sync api we have
    function isFlashOnly(targetName) {
        return targetName.match(".*MSP430.*G2.*") && getOS() !== TICloudAgent.OS.WIN;
    }
    TICloudAgent.isFlashOnly = isFlashOnly;
    var Util;
    (function (Util) {
        function encodeAsBase64(data) {
            var def = Q.defer();
            if (data instanceof Blob) {
                // encode data as base 64string
                var reader_1 = new FileReader();
                reader_1.readAsDataURL(data);
                reader_1.onloadend = function () {
                    def.resolve(reader_1.result.split(",")[1]);
                };
            }
            else {
                def.resolve(btoa(data));
            }
            return def.promise;
        }
        Util.encodeAsBase64 = encodeAsBase64;
        var keywords = ["Texas Instruments", "Texas_Instruments", "MSP", "FTDI"];
        // iterate over all ports and find the ones with matching key words
        function findInterestingPorts(ports) {
            var foundPorts = [];
            for (var _i = 0, ports_1 = ports; _i < ports_1.length; _i++) {
                var port = ports_1[_i];
                for (var _a = 0, keywords_1 = keywords; _a < keywords_1.length; _a++) {
                    var keyword = keywords_1[_a];
                    if (port.displayName.indexOf(keyword) !== -1) {
                        foundPorts.push(port);
                        break;
                    }
                }
            }
            return foundPorts;
        }
        ;
        // select the port with match pnpId suffix
        function selectWithMatchingPnpSuffixWinAndLinux(ports, suffix) {
            var foundPorts = findInterestingPorts(ports);
            var found = false;
            // no interesting ports found
            if (foundPorts.length === 0 && ports.length > 0) {
                ports[0].selected = true;
            }
            else {
                // try to find one with the matching suffix
                for (var _i = 0, foundPorts_1 = foundPorts; _i < foundPorts_1.length; _i++) {
                    var port = foundPorts_1[_i];
                    if (port.pnpId.indexOf(suffix, port.pnpId.length - suffix.length) !== -1) {
                        port.selected = true;
                        found = true;
                        break;
                    }
                }
                // we still haven't found one, default to the first found port
                if (!found) {
                    foundPorts[0].selected = true;
                    found = true;
                }
            }
            return found;
        }
        function selectDefaultWinAndLinux(ports) {
            return selectWithMatchingPnpSuffixWinAndLinux(ports, "02");
        }
        function selectMSP432WinAndLinux(ports) {
            return selectWithMatchingPnpSuffixWinAndLinux(ports, "00");
        }
        function selectMSP432OSX(ports) {
            var foundPorts = findInterestingPorts(ports);
            var found = false;
            if (foundPorts.length > 0) {
                foundPorts[0].selected = true;
                found = true;
            }
            return found;
        }
        function selectDefaultOSX(ports) {
            var found = false;
            for (var _i = 0, ports_2 = ports; _i < ports_2.length; _i++) {
                var port = ports_2[_i];
                if (port.comName.indexOf("/dev/cu.usb") !== -1 && port.manufacturer === "") {
                    found = true;
                    port.selected = found;
                }
                if (found) {
                    break;
                }
            }
            if (!found && ports.length > 0) {
                ports[0].selected = true;
            }
            return found;
        }
        function selectDefaultPort(params) {
            var ports = params.ports;
            var targetName = params.targetName ? params.targetName : "";
            var selectFunc = (getOS() === TICloudAgent.OS.OSX) ? selectDefaultOSX : selectDefaultWinAndLinux;
            // 432 and f128 follow the same rules
            if (targetName.indexOf("432") !== -1 || targetName.indexOf("cc2650f128") !== -1) {
                selectFunc = (getOS() === TICloudAgent.OS.OSX) ? selectMSP432OSX : selectMSP432WinAndLinux;
            }
            if (targetName.indexOf("MSP430G2") !== -1 && getOS() === TICloudAgent.OS.OSX) {
                selectFunc = selectMSP432OSX;
            }
            var found = selectFunc(ports);
            return Q(found);
        }
        Util.selectDefaultPort = selectDefaultPort;
        var baudRates = [{
                rate: "50",
                selected: false,
            }, {
                rate: "75",
                selected: false,
            }, {
                rate: "110",
                selected: false,
            }, {
                rate: "134",
                selected: false,
            }, {
                rate: "150",
                selected: false,
            }, {
                rate: "300",
                selected: false,
            }, {
                rate: "600",
                selected: false,
            }, {
                rate: "1200",
                selected: false,
            }, {
                rate: "1800",
                selected: false,
            }, {
                rate: "2400",
                selected: false,
            }, {
                rate: "4800",
                selected: false,
            }, {
                rate: "7200",
                selected: false,
            }, {
                rate: "9600",
                selected: true,
            }, {
                rate: "14400",
                selected: false,
            }, {
                rate: "19200",
                selected: false,
            }, {
                rate: "28800",
                selected: false,
            }, {
                rate: "38400",
                selected: false,
            }, {
                rate: "56000",
                selected: false,
            }, {
                rate: "57600",
                selected: false,
            }, {
                rate: "115200",
                selected: false,
            }, {
                rate: "128000",
                selected: false,
            }, {
                rate: "153600",
                selected: false,
            }, {
                rate: "230400",
                selected: false,
            }, {
                rate: "256000",
                selected: false,
            }, {
                rate: "460800",
                selected: false,
            }, {
                rate: "921600",
                selected: false,
            }];
        function getBaudRates() {
            return Q(baudRates);
        }
        Util.getBaudRates = getBaudRates;
    })(Util = TICloudAgent.Util || (TICloudAgent.Util = {}));
})(TICloudAgent || (TICloudAgent = {}));
