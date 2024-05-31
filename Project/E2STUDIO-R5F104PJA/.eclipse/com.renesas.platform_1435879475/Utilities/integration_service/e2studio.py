"""This file contains APIs that allow external applications
to 'drive' e2 studio using Python script.

A set of functions are provided that allow:
- Invoking and terminating e2 studio.
- Building projects.
- Starting and terminating debugging sessions.
- Getting target status.
- Getting and setting variables and memory from the target.
- Executing GDB commands and scripts.
"""

from isserver import e2studio_server as e2studio

def invoke(e2studio_folder, workspace_folder):
    e2studio.invoke(e2studio_folder, workspace_folder)

def connect():
    e2studio.connect()

def disconnect():
    e2studio.disconnect()

def terminate():
    e2studio.terminate()

def add_event_listener(listener):
    e2studio.add_event_listener(listener)

def remove_event_listener(listener):
    e2studio.remove_event_listener(listener)

def create_project(project_name, device_name, toolchain_type, project_type, settings={}):
    return e2studio.create_project(project_name, device_name, toolchain_type, project_type, settings)

def is_project_exists(project_name):
    return e2studio.is_project_exists(project_name)

def is_project_open(project_name):
    return e2studio.is_project_open(project_name)

def open_project(project_name):
    return e2studio.open_project(project_name)

def remove_project(project_name, delete_content=False):
    e2studio.remove_project(project_name, delete_content)

def import_project(project_path):
    e2studio.import_project(project_path)

def get_project_name(project_path):
    return e2studio.get_project_name(project_path)

def get_device_group(project_name):
    return e2studio.get_device_group(project_name)

def get_device_group_list(micro_type):
    return e2studio.get_device_group_list(micro_type)

def get_device_group_pin_list(micro_type, device_group):
    return e2studio.get_device_group_pin_list(micro_type, device_group)

def get_device_name_list(micro_type, device_group_pin = "All"):
    return e2studio.get_device_name_list(micro_type, device_group_pin)

def build_file(project_name, file_path, rebuild=False, wait_build=False):
    e2studio.build_file(project_name, file_path, rebuild, wait_build)

def create_virtual_folder(project_name, folder_name):
    e2studio.create_virtual_folder(project_name, folder_name)

def create_linked_source_file(project_name, folder_name, file_path):
    e2studio.create_linked_source_file(project_name, folder_name, file_path)

def include_source_folder(project_name, folder_name, build_config_name=""):
    e2studio.include_source_folder(project_name, folder_name, build_config_name)

def include_file(project_name, file_name, configuration_name=""):
    return e2studio.include_file(project_name, file_name, configuration_name)

def get_active_build_config_name(project_name):
    return e2studio.get_active_build_config_name(project_name)

def set_active_build_config(project_name, build_config_name):
    e2studio.set_active_build_config(project_name, build_config_name)

def create_new_build_config(project_name, base_config_name, new_config_name):
    e2studio.create_new_build_config(project_name, base_config_name, new_config_name)

def get_toolchain(project_name, build_config_name):
    return e2studio.get_toolchain(project_name, build_config_name)

def get_toolchain_id(project_name, build_config_name):
    return e2studio.get_toolchain_id(project_name, build_config_name)

def get_build_config_option(project_name, build_config_name, tool, option):
    return e2studio.get_build_config_option(project_name, build_config_name, tool, option)

def set_build_config_option(project_name, build_config_name, tool, option, value):
    e2studio.set_build_config_option(project_name, build_config_name, tool, option, value)

def build_project(project_name, build_config_name, rebuild=False, wait_build=False):
    e2studio.build_project(project_name, build_config_name, rebuild, wait_build)

def is_file_exist(project_name, file_path):
    return e2studio.is_file_exist(project_name, file_path)

def is_linked_file(project_name, file_path):
    return e2studio.is_linked_file(project_name, file_path)

def remove_linked_file(project_name, file_path):
    e2studio.remove_linked_file(project_name, file_path)

def exclude_from_build(project_name, source_file, build_config_name=""):
    e2studio.exclude_from_build(project_name, source_file, build_config_name)

def get_all_files(project_name):
    return e2studio.get_all_files(project_name)

def get_debug_config_option(debug_config_name, option_id):
    return e2studio.get_debug_config_option(debug_config_name, option_id)

def set_debug_config_option(debug_config_name, option_id, option_value):
    e2studio.set_debug_config_option(debug_config_name, option_id, option_value)

def launch_debug_session(debug_config_name, block_download=False):
    return e2studio.launch_debug_session(debug_config_name, block_download)

def terminate_debug_session(session_id):
    e2studio.terminate_debug_session(session_id)

def download_loadmodule(
        session_id, file_path="", offset=0,
        load_image=True, load_symbols=True,
        clear_old_symbols=False, core_name=""):
    e2studio.download_loadmodule(
        session_id, file_path, offset,
        load_image, load_symbols,
        clear_old_symbols, core_name)

def resume_debug_session(session_id, wait_break=False):
    e2studio.resume_debug_session(session_id, wait_break)

def suspend_debug_session(session_id):
    e2studio.suspend_debug_session(session_id)

def reset_debug_session(session_id):
    e2studio.reset_debug_session(session_id)

def is_debug_session_running(session_id):
    return e2studio.is_debug_session_running(session_id)

def get_symbol_address(session_id, symbol_name):
    return e2studio.get_symbol_address(session_id, symbol_name)

def get_source_line_address(session_id, source_file, line_number=None):
    return e2studio.get_source_line_address(session_id, source_file, line_number)

def add_address_breakpoint(address, breakpoint_type="", temporary=False):
    return e2studio.add_address_breakpoint(address, breakpoint_type, temporary)

def remove_address_breakpoint(address):
    e2studio.remove_address_breakpoint(address)

def get_all_breakpoints():
    return e2studio.get_all_breakpoints()

def skip_all_breakpoints(skip=True):
    e2studio.skip_all_breakpoints(skip)

def remove_all_breakpoints():
    e2studio.remove_all_breakpoints()

def read_memory(session_id, address, length):
    return e2studio.read_memory(session_id, address, length)

def write_memory(session_id, address, length, data):
    e2studio.write_memory(session_id, address, length, data)

def is_big_endian_target(session_id):
    return e2studio.is_big_endian_target(session_id)

def get_register(session_id, register_name):
    return e2studio.get_register(session_id, register_name)

def step_in(session_id, instruction_step=False):
    e2studio.step_in(session_id, instruction_step)

def get_variable_size(session_id, variable):
    return e2studio.get_variable_size(session_id, variable)

def get_variable_type(session_id, variable):
    return e2studio.get_variable_type(session_id, variable)

def get_integer_expression(session_id, expression):
    return e2studio.get_integer_expression(session_id, expression)

def set_integer_expression(session_id, expression, value):
    e2studio.set_integer_expression(session_id, expression, value)

def get_float_expression(session_id, expression):
    return e2studio.get_float_expression(session_id, expression)

def set_float_expression(session_id, expression, value):
    e2studio.set_float_expression(session_id, expression, value)

def get_expression_info(session_id, expression):
    return e2studio.get_expression_info(session_id, expression)

def is_multicore_device(debug_config_name):
    return e2studio.is_multicore_device(debug_config_name)

def does_debug_session_exist(session_id):
    return e2studio.does_debug_session_exist(session_id)

def get_trace_providers():
    return e2studio.get_trace_providers()

def select_trace_provider(provider_id):
    e2studio.select_trace_provider(provider_id)

def is_trace_supported(session_id):
    return e2studio.is_trace_supported(session_id)

def get_trace_settings(session_id, settings=[]):
    return e2studio.get_trace_settings(session_id, settings)

def set_trace_settings(session_id, settings):
    e2studio.set_trace_settings(session_id, settings)

def get_timer_providers():
    return e2studio.get_timer_providers()

def select_timer_provider(provider_id):
    e2studio.select_timer_provider(provider_id)

def is_timer_supported(session_id):
    return e2studio.is_timer_supported(session_id)

def get_timers(session_id):
    return e2studio.get_timers(session_id)

def get_time_measurement_result(session_id, timer_name):
    return e2studio.get_time_measurement_result(session_id, timer_name)

def get_time_measurement_result_XTime_wrapper(session_id):
    return e2studio.get_time_measurement_result_XTime_wrapper(session_id)

def execute_debug_command(session_id, command):
    return e2studio.execute_debug_command(session_id, command)

def get_trace_record_count(session_id):
    return e2studio.get_trace_record_count(session_id)

def get_trace_data(session_id, start_index, number_of_records, settings={}):
    return e2studio.get_trace_data(session_id, start_index, number_of_records, settings)

def get_tool_option_id(project_name, build_config_name, toolType):
    return e2studio.get_tool_option_id(project_name, build_config_name, toolType)