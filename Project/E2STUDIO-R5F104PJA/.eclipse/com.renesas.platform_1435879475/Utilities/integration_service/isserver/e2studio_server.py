import subprocess
import re
from isserver import control_status_socket, e2studio_socket

# Default port number of server socket inside e2 studio
DEFAULT_E2STUDIO_PORT_NO = 61246
# Default port number of server socket inside ISServer
DEFAULT_CONTROL_STATUS_PORT_NO = 61244
# Default IP address of ISServer
DEFAULT_IP_ADDR = "localhost"
# Format of hexadecimal value to be used in APIs.
# Use this to avoid the additional "L" character
# when using hex() with <long> value in Python 2.
HEX_FORMAT = "{:x}"

# Port number of server socket inside e2 studio
e2studio_port_no = 0
# Port number of server socket inside ISServer
control_status_port_no = 0
# Flag indicating the connection to e2 studio has been created
connected_to_e2studio = False

event_listeners = []

def invoke(e2studio_folder, workspace_folder):
    global control_status_port_no

    # Setup the server socket to get the OS allocated port number
    control_status_port_no = control_status_socket.setup_server_socket(0)

    # Prepare command line arguments
    cmd_line = [
        e2studio_folder + "/eclipse/e2studio.exe",
        "-data",
        workspace_folder,
        "-eisIPAddress",
        DEFAULT_IP_ADDR,
        "-eisPortNo",
        str(get_control_status_port_no())
    ]
    # Invoke e2 studio
    subprocess.Popen(cmd_line)

def connect():
    global e2studio_port_no
    global connected_to_e2studio
    global connection_to_e2studio_failed

    if (connected_to_e2studio):
        return

    # Create the connection for control/status message sending
    e2studio_port_no = control_status_socket.connect(get_control_status_port_no(), 0)
    # Create the connection for actual command sending
    e2studio_socket.connect(get_e2studio_port_no())
    connected_to_e2studio = True
    # Listen control status events
    control_status_socket.set_event_listener(listen_events)

def get_control_status_port_no():
    global control_status_port_no

    if (control_status_port_no == 0):
        control_status_port_no = DEFAULT_CONTROL_STATUS_PORT_NO
    return control_status_port_no

def get_e2studio_port_no():
    global e2studio_port_no

    if (e2studio_port_no == 0):
        e2studio_port_no =  DEFAULT_E2STUDIO_PORT_NO
    return e2studio_port_no

def send_message(message, good_result=""):
    if (not connected_to_e2studio):
        raise Exception("Not connected to e2 studio")

    response = e2studio_socket.send_message(message)

    if ("Failed" in response):
        raise Exception("'{}' command failed with response '{}'".format(message, response))
    elif (good_result != ""):
        # Check the expected result if any
        if (good_result not in response):
            raise Exception("'{}' command failed with response '{}'".format(message, response))
    return response

def create_message(leader, *params):
    message = leader
    for param in params:
        message = message + "," + param
    return message

def add_event_listener(listener):
    event_listeners.append(listener)

def remove_event_listener(listener):
    if listener in event_listeners:
        event_listeners.remove(listener)

def listen_events(event):
    for listener in event_listeners:
        try:
            listener(event)
        except:
            pass

def disconnect():
    global connected_to_e2studio

    if (connected_to_e2studio):
        send_message("Disconnect", "OK")
        connected_to_e2studio = False

def terminate():
    control_status_socket.terminate()
    send_message("Terminatee2Studio", "OK")

def create_project(project_name, device_name, toolchain_type, project_type, settings):
    if (not is_str_instance(project_name) or project_name == "" or not is_str_instance(device_name) or device_name == "" or not is_str_instance(toolchain_type) or toolchain_type == ""
            or not is_str_instance(project_type) or project_type == ""):
        return False
    if("debugger_HardwareDebug" in settings):
        hardwareDebugToolId = get_debug_tool_id(
            toolchain_type, settings.get('debugger_HardwareDebug'))
        if(hardwareDebugToolId == None):
            settings['debugger_HardwareDebug'] = ""
        else:
            settings['debugger_HardwareDebug'] = hardwareDebugToolId
    if("debugger_Debug" in settings):
        debugToolId = get_debug_tool_id(
            toolchain_type, settings.get('debugger_Debug'))
        if(debugToolId == None):
            settings['debugger_Debug'] = ""
        else:
            settings['debugger_Debug'] = debugToolId
    if("debugger" in settings):
        debugToolId = get_debug_tool_id(
            toolchain_type, settings.get('debugger'))
        if(debugToolId != None):
            settings['debugger'] = debugToolId
    parameters = {"projectName": str(project_name), "device": str(
        device_name), "toolchainType": str(toolchain_type), "projectType": str(project_type)}
    parameters.update(settings)
    message = create_message("CreateProject", str(parameters))
    return (send_message(message) == "true")

def is_project_exists(project_name):
    if (project_name != ""):
        message = create_message("IsProjectExists", project_name)
        return send_message(message) == "true"
    else:
        raise Exception("Invalid project name")

def is_project_open(project_name):
    message = create_message("IsProjectOpen", project_name, "python")
    return send_message(message) == "true"

def open_project(project_name):
    message = create_message("OpenProject", project_name)
    send_message(message)

def remove_project(project_name, delete_contents):
    if (project_name != ""):
        message = create_message("RemoveProject", project_name, str(delete_contents))
        send_message(message, "OK")
    else:
        raise Exception("Invalid project name")

def create_virtual_folder(project_name, folder_name):
    if (project_name != ""):
        message = create_message('CreateVirtualFolder', project_name, folder_name)
        send_message(message, "OK")
    else:
        raise Exception("Invalid project name")

def create_linked_source_file(project_name, folder_name, file_path):
    if (project_name != ""):
        message = create_message('CreateLinkedSourceFile', project_name, folder_name, file_path)
        send_message(message, "OK")
    else:
        raise Exception("Invalid project name")

def include_source_folder(project_name, folder_name, build_config_name):
    if (project_name != ""):
        message = create_message('IncludeSourceFolder', project_name, build_config_name, folder_name)
        send_message(message, "OK")
    else:
        raise Exception("Invalid project name")

def include_file(project_name, file_name, configuration_name):
    if (project_name != "" and file_name != ""):
        message = ""
        if (configuration_name == ""):
            message = create_message("IncludeFileAll", project_name, file_name)
        else:
            message = create_message("IncludeFile", project_name, configuration_name, file_name)
        send_message(message, "OK")
    else:
        raise Exception("Project name or file name cannot be empty")

def build_file(project_name, file_path, rebuild, wait_build):
    if(project_name == '' or file_path == '' or not isinstance(rebuild, bool) or not isinstance(wait_build, bool)):
        raise Exception("Invalid input value")
    message = create_message("BuildFile", project_name, file_path, str(rebuild), str(wait_build))
    send_message(message, "OK")

def import_project(project_path):
    message = create_message("ImportProject", project_path)
    send_message(message)

def get_project_name(project_path):
    message = create_message("GetProjectName", project_path)
    return send_message(message)

def get_device_group(project_name):
    if (project_name != ""):
        message = create_message("GetDeviceGroup", project_name)
        return send_message(message)
    else:
        raise Exception("Invalid project name")

def get_device_group_list(micro_type):
    if (micro_type != ""):
        message = create_message("GetDeviceGroupList", micro_type)
        return send_message(message).split("|")
    else:
        raise Exception("Invalid microcontroller name")

def get_device_group_pin_list(micro_type, device_group):
    if (micro_type != ""):
        if (device_group != ""):
            message = create_message("GetDeviceGroupPinList", micro_type, device_group)
            return send_message(message).split("|")
        else:
            raise Exception("Invalid device group name")
    else:
        raise Exception("Invalid microcontroller name")

def get_device_name_list(micro_type, device_group_pin = "All"):
    device_list = []
    if (micro_type != ""):
        if (micro_type == "RA"):
            message = create_message("GetDeviceNameList", micro_type, "All")
            return send_message(message).split("|")
        if (device_group_pin != "All"):
            message = create_message("GetDeviceNameList", micro_type, device_group_pin)
            return send_message(message).split("|")
        if (device_group_pin == "All"):
            device_group_list = get_device_group_list(micro_type)
            for device_group_name in device_group_list:
                device_group_pin_list = get_device_group_pin_list(micro_type, device_group_name)
                for device_group_pin_name in device_group_pin_list:
                    device_name_list = get_device_name_list(micro_type, device_group_pin_name)
                    for device_name in device_name_list:
                        device_list.append(device_name)
            return device_list
    else:
        raise Exception("Invalid microcontroller name")

def get_active_build_config_name(project_name):
    message = create_message("GetActiveBuildConfigurationName", project_name)
    return send_message(message)

def set_active_build_config(project_name, build_config_name):
    message = create_message("SetActiveBuildConfiguration", project_name, build_config_name)
    send_message(message, "OK")

def create_new_build_config(project_name, base_config_name, new_config_name):
    message = create_message("CreateNewBuildConfiguration", project_name, base_config_name, new_config_name)
    send_message(message, "OK")

def get_toolchain(project_name, build_config_name):
    message = create_message("GetToolchain", project_name, build_config_name)
    return send_message(message)

def get_toolchain_id(project_name, build_config_name):
    message = create_message("GetToolchainId", project_name, build_config_name)
    return send_message(message)

def get_tool_option_id(project_name, build_config_name, toolType):
    message = create_message("GetToolOptionId", project_name, build_config_name,toolType)
    return send_message(message)

def get_build_config_option(project_name, build_config_name, tool, option):
    message = create_message("GetToolOptionValue", project_name, build_config_name, tool, option)
    result = send_message(message)
    formatted_result = re.findall("^\[(Boolean|StringList|Enumerated|String)\]\[(.*)\]$", result)
    if len(formatted_result) == 1:
        option_type, option_value = formatted_result[0]
    else:
        return result[1:len(result) - 1]

    if option_type == "Boolean":
        return option_value == "true"
    elif option_type == "StringList":
        if option_value == "":
            return []
        else:
            return option_value.split("|")
    else:
        return option_value

def set_build_config_option(project_name, build_config_name, tool, option, value):
    if (value != None):
        if (isinstance(value, list)):
            handled_value = "|".join(map(str, value))
        else:
            handled_value = str(value)
    else:
        raise Exception("Invalid input value type")
    message = create_message("SetToolOption", project_name, build_config_name, tool, option, handled_value)
    send_message(message, "OK")

def build_project(project_name, build_config_name, rebuild, wait_build):
    if (isinstance(rebuild, bool) and isinstance(wait_build, bool) and is_str_instance(project_name) and project_name != ""):
        message = create_message("BuildProject", str(project_name), build_config_name, str(rebuild), str(wait_build))
        send_message(message, "OK")
    else:
        raise Exception("Invalid input value")

def is_file_exist(project_name, file_path):
    if (project_name != ""):
        message = create_message("IsFileExistsInProject", project_name, file_path)
        return (send_message(message) == "true")
    else:
        raise Exception("Invalid project name")

def is_linked_file(project_name, file_path):
    if (project_name != ""):
        message = create_message("IsLinkedFile", project_name, file_path)
        return (send_message(message) == "true")
    else:
        raise Exception("Invalid project name")

def remove_linked_file(project_name, file_path):
    if (project_name != ""):
        message = create_message("RemoveLinkedFile", project_name, file_path)
        send_message(message,"OK")
    else:
        raise Exception("Invalid project name")

def exclude_from_build(project_name, source_file, build_config_name):
    message = ""
    if (build_config_name == ""):
        message = create_message("ExcludeFileAll", project_name, source_file)
    else:
        message = create_message("ExcludeFile", project_name, build_config_name, source_file)
    send_message(message, "OK")

def get_all_files(project_name):
    if (project_name != ""):
        message = create_message("GetSourceFilesPath", project_name)
        list_value = list(send_message(message).split(","))
        for i in range(len(list_value)):
            list_value[i] = str(list_value[i])
        return list_value
    else:
        raise Exception("Invalid project name")

def escape(value):
    # Escape the value before formatting it to send to e2 studio
    # "&" need to be escaped first
    return value.replace("&", "&amp;").replace('"', "&quot;").replace("\r", "&#13;").replace("\n", "&#10;")

def unescape(value):
    # Unescape the value after extracted it from the string received from e2 studio
    # Unescape the "&" after unescaped the others
    return value.replace("&quot;", '"').replace("&#13;", "\r").replace("&#10;", "\n").replace("&amp;", "&")

def extract_map_value(value):
    # Extract map entries follow its format "key1":"value1""key2":"value2"
    map_value = re.findall('"([^"]*)":"([^"]*)"', value)
    # Each item of map_value is an entry of the map as a tuple
    for i in range(len(map_value)):
        # Convert the tuple to list to unescape its key and value
        map_entry = list(map_value[i])
        # Unescape the key
        map_entry[0] = unescape(map_entry[0])
        # Unescape the value
        map_entry[1] = unescape(map_entry[1])
        # Convert the entry back to tuple
        map_value[i] = tuple(map_entry)
    return dict(map_value)

def format_map_value(value):
    map_value = []
    for map_item in value.items():
        # Key and value of an item of the dictionary will be surrounded by " and separated by :
        # E.g. "key":"value"
        map_value.append('"{}":"{}"'.format(escape(map_item[0]), escape(map_item[1])))
    # Each formatted item of the dictionary will be appended together
    # E.g. "key1":"value1""key2":"value2"
    return "".join(["{}"]*len(map_value)).format(*map_value)

def extract_list_value(value):
    # Extract list items follow its format "item1""item2"
    list_value = re.findall('"([^"]*)"', value)
    for i in range(len(list_value)):
        list_value[i] = unescape(list_value[i])
    return list_value

def format_list_value(value):
    list_value = []
    for list_item in value:
        list_value.append(escape(list_item))
    # Each item of the list will be surrounded by " and appended together
    # E.g. "item1""item2"
    return "".join(['"{}"']*len(list_value)).format(*list_value)

def get_debug_config_option(debug_config_name, option_id):
    message = create_message("GetDebugConfigurationOption", debug_config_name, option_id)
    result = send_message(message)

    # The empty case in value type part means eclipse support new value type
    # but IS does not support it
    formatted_result = re.findall("^\[(Boolean|Integer|List|Set|Map|String|)\]\[(.*)\]$", result)
    if len(formatted_result) == 1:
        option_type, option_value = formatted_result[0]
    else:
        # This case means an older ISServer is working with a newer e2 studio
        # which support new value type and ISServer cannot recognize that value type
        # Return the result as is which include both type and value information
        return result

    if option_type == "Boolean":
        return option_value == "true"
    elif option_type == "Integer":
        return int(option_value)
    elif option_type == "List":
        return extract_list_value(option_value)
    elif option_type == "Set":
        return set(extract_list_value(option_value))
    elif option_type == "Map":
        return extract_map_value(option_value)
    elif option_type == "String":
        return unescape(option_value)
    else:
        # For empty type, return the value as is
        return option_value

def set_debug_config_option(debug_config_name, option_id, option_value):
    if isinstance(option_value, bool):
        option_type = "Boolean"
        formatted_option_value = str(option_value)
    elif isinstance(option_value, int):
        option_type = "Integer"
        formatted_option_value = str(option_value)
    elif isinstance(option_value, list):
        option_type = "List"
        formatted_option_value = format_list_value(option_value)
    elif isinstance(option_value, set):
        option_type = "Set"
        set_value = []
        for set_item in option_value:
            set_value.append(escape(set_item))
        # Each item of the set will be surrounded by " and appended together
        # E.g. "item1""item2"
        formatted_option_value = "".join(['"{}"']*len(set_value)).format(*set_value)
    elif isinstance(option_value, dict):
        option_type = "Map"
        formatted_option_value = format_map_value(option_value)
    elif isinstance(option_value, str):
        option_type = "String"
        formatted_option_value = escape(option_value)
    else:
        raise Exception("Invalid option value type")

    message = create_message(
        "SetDebugConfigurationOption",
        debug_config_name,
        option_id,
        # Type and formatted value of option will be sent to e2 studio in the format [type][formatted value]
        "[{}][{}]".format(option_type, formatted_option_value))
    send_message(message, "OK")

def launch_debug_session(debug_config_name, block_download):
    message = create_message("LaunchDebugSession", debug_config_name, str(block_download))
    return send_message(message)

def terminate_debug_session(session_id):
    message = create_message("TerminateDebugSession", session_id)
    send_message(message, "OK")

def download_loadmodule(session_id, file_path, offset, load_image, load_symbols, clear_old_symbols, core_name):
    message = "DownloadLoadmodule"
    if (file_path == ""):
        message = create_message(
            "DownloadLoadmodule",
            session_id)
    else:
        message = create_message(
            "DownloadLoadmodule",
            session_id,
            file_path,
            HEX_FORMAT.format(offset),
            str(load_image),
            str(load_symbols),
            str(clear_old_symbols),
            core_name)
    send_message(message, "OK")

def resume_debug_session(session_id, wait_break):
    message = create_message("ResumeDebugSession", session_id, str(wait_break))
    send_message(message, "OK")

def suspend_debug_session(session_id):
    message = create_message("SuspendDebugSession", session_id)
    send_message(message, "OK")

def reset_debug_session(session_id):
    message = create_message("ResetDebugSession", session_id)
    send_message(message, "OK")

def is_debug_session_running(session_id):
    message = create_message("IsDebugSessionRunning", session_id)
    return (send_message(message) == "running")

def get_symbol_address(session_id, symbol_name):
    message = create_message("SymbolToAddress", session_id, symbol_name)
    return int(send_message(message), 16)

def get_source_line_address(session_id, source_file, line_number):
    message = "GetSourceLineAddress"
    if (line_number is None):
        # Line number is not specified,
        # get it from source_file based on the format <source file>:<line number>
        params = source_file.rsplit(":", 1)
        if ((len(params) == 2) and params[1].isnumeric()):
            message = create_message("GetSourceLineAddress", session_id, params[0], params[1])
        else:
            raise Exception("Invalid input format")
    else:
        message = create_message("GetSourceLineAddress", session_id, source_file, str(line_number))

    address = send_message(message)
    if (address == ""):
        # No address found
        return None
    else:
        return int(address, 16)

def add_address_breakpoint(address, breakpoint_type, temporary):
    message = create_message(
        "AddAddressBreakpoint",
        HEX_FORMAT.format(address),
        breakpoint_type,
        str(temporary))
    return int(send_message(message))

def remove_address_breakpoint(address):
    message = create_message("RemoveAddressBreakpoint", HEX_FORMAT.format(address))
    send_message(message, "OK")

def get_all_breakpoints():
    breakpoints = send_message("GetAllBreakpoints")
    if (breakpoints != ""):
        return breakpoints.split("|")
    else:
        return []

def skip_all_breakpoints(skip):
    message = create_message("SkipAllBreakpoints", str(skip))
    send_message(message, "OK")

def remove_all_breakpoints():
    send_message("RemoveAllBreakpoints", "OK")

def read_memory(session_id, address, length):
    message = create_message(
        "ReadMemory",
        session_id,
        HEX_FORMAT.format(address),
        HEX_FORMAT.format(length))
    return send_message(message)

def write_memory(session_id, address, length, data):
    message = create_message(
        "WriteMemory",
        session_id,
        HEX_FORMAT.format(address),
        HEX_FORMAT.format(length),
        data)
    send_message(message, "OK")

def is_big_endian_target(session_id):
    message = create_message("IsBigEndianTarget", session_id)
    return (send_message(message) == "true")

def get_register(session_id, register_name):
    message = create_message("GetRegister", session_id, register_name)
    return int(send_message(message), 16)

def step_in(session_id, instruction_step):
    message = create_message("StepIn", session_id, "0" if instruction_step else "1")
    send_message(message, "OK")

def get_variable_size(session_id, variable):
    message = create_message("GetVariableSize", session_id, variable)
    return int(send_message(message))

def get_variable_type(session_id, variable):
    message = create_message("GetVariableType", session_id, variable)
    return send_message(message)

def get_integer_expression(session_id, expression):
    message = create_message("GetExpressionI", session_id, expression)
    return int(send_message(message), 16)

def set_integer_expression(session_id, expression, value):
    message = create_message(
        "SetExpressionI",
        session_id,
        expression,
        HEX_FORMAT.format(value))
    send_message(message, "OK")

def get_float_expression(session_id, expression):
    message = create_message("GetExpressionF", session_id, expression)
    return float(send_message(message))

def set_float_expression(session_id, expression, value):
    message = create_message("SetExpressionF", session_id, expression, str(value))
    send_message(message, "OK")

def get_expression_info(session_id, expression):
    message = create_message("GetExpressionInfo", session_id, expression)
    return send_message(message)

def is_multicore_device(debug_config_name):
    message = create_message("IsMulticoreDevice", debug_config_name)
    return (send_message(message) == "true")

def does_debug_session_exist(session_id):
    message = create_message("DoesDebugSessionExist", session_id)
    return (send_message(message) == "true")

def get_trace_providers():
    return extract_map_value(send_message("GetTraceProviders"))

def select_trace_provider(provider_id):
    message = create_message("SelectTraceProvider", provider_id)
    send_message(message, "OK")

def is_trace_supported(session_id):
    message = create_message("IsTraceSupported", session_id)
    return (send_message(message) == "true")

def get_trace_settings(session_id, settings):
    message = create_message("GetTraceSettings", session_id, format_list_value(settings))
    return extract_map_value(send_message(message))

def set_trace_settings(session_id, settings):
    message = create_message("SetTraceSettings", session_id, format_map_value(settings))
    send_message(message, "OK")

def get_timer_providers():
    return extract_map_value(send_message("GetTimerProviders"))

def select_timer_provider(provider_id):
    message = create_message("SelectTimerProvider", provider_id)
    send_message(message, "OK")

def is_timer_supported(session_id):
    message = create_message("IsTimerSupported", session_id)
    return (send_message(message) == "true")

def get_timers(session_id):
    message = create_message("GetTimers", session_id)
    return extract_list_value(send_message(message))

def get_time_measurement_result(session_id, timer_name):
    message = create_message("GetTimeMeasurementResult", session_id, timer_name)
    return extract_list_value(send_message(message))

def get_time_measurement_result_XTime_wrapper(session_id):
    message = create_message("GetTimeMeasurementResult", session_id, "XTime_wrapper")
    return extract_list_value(send_message(message))

def get_debug_tool_id(toolchain_type, debug_tool_name):
    if('CC-RX' == toolchain_type):
        if('E1' == debug_tool_name):
            return "com.renesas.hardwaredebug.rx.e1"
        elif ('E2' == debug_tool_name):
            return "com.renesas.hardwaredebug.rx.e2.e2rx"
        elif ('E2Lite' == debug_tool_name):
            return "com.renesas.hardwaredebug.rx.e2"
        elif ('E20' == debug_tool_name):
            return "com.renesas.hardwaredebug.rx.e20"
        elif ('EZ' == debug_tool_name):
            return "com.renesas.hardwaredebug.rx.ez"
        elif ('SeggerJLink' == debug_tool_name):
            return "com.renesas.hardwaredebug.rx.jlink"
        elif ('Serial' == debug_tool_name):
            return "com.renesas.hardwaredebug.rx.serial"
        elif ('Simulator' == debug_tool_name):
            return "com.renesas.hardwaredebug.simulator.rx"
    elif ('CC-RL' ==  toolchain_type):
        if ('E1' == debug_tool_name or 'E20' == debug_tool_name):
            return "com.renesas.hardwaredebug.rl78.e1"
        elif ('E2' == debug_tool_name):
            return "com.renesas.hardwaredebug.rl78.e2.e2rl78"
        elif ('E2Lite' == debug_tool_name):
            return "com.renesas.hardwaredebug.rl78.e2"
        elif ('EZ' == debug_tool_name):
            return "com.renesas.hardwaredebug.rl78.ez"
        elif ('Iecube' == debug_tool_name):
            return "com.renesas.hardwaredebug.rl78.iecube"
        elif ('ComPort' == debug_tool_name):
            return "com.renesas.hardwaredebug.rl78.serial"
        elif ('Simulator' == debug_tool_name):
            return "com.renesas.hardwaredebug.simulator.rl78"
    elif (toolchain_type in {'Renesas_FSP_GCC', 'Renesas_FSP_IAR', 'Renesas_FSP_AC', 'Renesas_FSP_GCC_AARCH'}):
        if ('E2' == debug_tool_name):
            return "com.renesas.hardwaredebug.arm.e2"
        elif ('E2Lite' == debug_tool_name):
            return "com.renesas.hardwaredebug.arm.e2lite"
        elif ('JLink' == debug_tool_name):
            return "com.renesas.hardwaredebug.rz.jlink"
    return None


def is_str_instance(val):
    try:
        return isinstance(val, str) or isinstance(val, unicode)
    except:
        return False

def execute_debug_command(session_id, command):
    message = create_message("ExecuteDebugCommand", session_id, command, "python")
    return unescape(send_message(message))

def get_trace_record_count(session_id):
    message = create_message("GetTraceRecordCount", session_id)
    result = send_message(message)
    return int(result)

def get_trace_data(session_id, start_index, number_of_records, settings):
    if (start_index < 0):
        raise Exception("Start index must be greater than or equal to 0")
    if (number_of_records < 1):
       raise Exception("Number of records must be greater than or equal to 1")
    message = create_message("GetTraceData", session_id, str(start_index), str(number_of_records), format_map_value(settings))
    return extract_list_value(send_message(message))
