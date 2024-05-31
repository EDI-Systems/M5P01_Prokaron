import sys
import os
import importlib
import e2studio

class Common(object):

    def __init__(self):
        self.__hook_script = None

    def CubeSuiteExit(self):
        e2studio.terminate()

    def Hook(self, scriptFile):
        folder, file_name = os.path.split(scriptFile)
        if folder not in sys.path:
            sys.path.append(folder)
        if file_name.endswith(".py"):
            file_name = file_name[:-3]
        if self.__hook_script is None:
            e2studio.add_event_listener(self.__handle_e2studio_event)
        self.__hook_script = importlib.import_module(file_name)

    def __handle_e2studio_event(self, event):
        if self.__hook_script is not None:
            event_info = event.split(";", 1)
            if len(event_info) == 2 and event_info[0] == "debugger state":
                self.__handle_debugger_state_event(event_info[1])

    def __handle_debugger_state_event(self, event):
        event_info = event.split(";")
        if len(event_info) == 2:
            hook_functions = dir(self.__hook_script)
            if event_info[1] == "downloaded":
                if "AfterDownload" in hook_functions:
                    self.__hook_script.AfterDownload()
            elif event_info[1] == "reset":
                if "AfterCpuReset" in hook_functions:
                    self.__hook_script.AfterCpuReset()
                if "pythonConsoleCallback" in hook_functions:
                    self.__hook_script.pythonConsoleCallback(14)
            elif event_info[1] == "resuming":
                if "BeforeCpuRun" in hook_functions:
                    self.__hook_script.BeforeCpuRun()
                if "pythonConsoleCallback" in hook_functions:
                    self.__hook_script.pythonConsoleCallback(12)
            elif event_info[1] == "suspended":
                if "AfterCpuStop" in hook_functions:
                    self.__hook_script.AfterCpuStop()
                if "pythonConsoleCallback" in hook_functions:
                    self.__hook_script.pythonConsoleCallback(13)

    def _reset_hook(self):
        if self.__hook_script is not None:
            e2studio.remove_event_listener(self.__handle_e2studio_event)
            self.__hook_script = None
