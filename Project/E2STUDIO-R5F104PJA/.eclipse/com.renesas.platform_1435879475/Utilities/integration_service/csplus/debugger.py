import e2studio
from csplus.debugger_option import DebuggerOption
from csplus.debugger_debugtool import DebuggerDebugTool
from csplus.debugger_download import DebuggerDownload
from csplus.debugger_breakpoint import DebuggerBreakpoint
from csplus.debugger_memory import DebuggerMemory
from csplus.debugger_watch import DebuggerWatch

class Debugger(object):

    Option = DebuggerOption()
    DebugTool = DebuggerDebugTool()
    Download = DebuggerDownload()
    Breakpoint = DebuggerBreakpoint()
    Memory = DebuggerMemory()
    Watch = DebuggerWatch()

    def __init__(self):
        self.debug_config = ""
        self.debug_session = ""

    @property
    def DebugConfiguration(self):
        return self.debug_config

    @DebugConfiguration.setter
    def DebugConfiguration(self, debug_config):
        self.debug_config = debug_config

    @property
    def IsMulticore(self):
        return e2studio.is_multicore_device(self.debug_config)

    def Connect(self):
        try:
            self.debug_session = e2studio.launch_debug_session(self.debug_config, True)
            return True
        except:
            return False

    def Disconnect(self):
        try:
            e2studio.terminate_debug_session(self.debug_session)
            self.debug_session = ""
            return True
        except:
            return False

    def Go(self, goOption=GoOption.Normal):
        if (not e2studio.is_debug_session_running(self.debug_session)):
            e2studio.skip_all_breakpoints(goOption == GoOption.IgnoreBreak)
            e2studio.resume_debug_session(self.debug_session, goOption == GoOption.WaitBreak)
        else:
            # Raise exception if debug session is running same as CS+
            raise Exception("Failed to execute the go command. User program is running.")

    def Stop(self):
        e2studio.suspend_debug_session(self.debug_session)

    def Reset(self):
        e2studio.reset_debug_session(self.debug_session)

    def IsRunning(self):
        try:
            return e2studio.is_debug_session_running(self.debug_session)
        except:
            return False

    def Address(self, expression):
        # Try to cover some simple number formats first
        try:
            return int(expression, 0)
        except:
            pass

        # Not a simple number, try to resolve as an expression
        gdb_expression = expression
        if ("$" in gdb_expression):
            # Strip the loadmodule part out of expression
            # e.g.
            # "loadmodule.abs$file.c#15" -> "file.c#15"
            # "loadmodule.abs$function#variable" -> "function#variable"
            gdb_expression = gdb_expression.rsplit("$", 1)[1]
        if ("#" in gdb_expression):
            # Check whether the remain string represent a source line expression or symbol expression
            if (gdb_expression.rsplit("#", 1)[1].isnumeric()):
                # The last part after '#' is a line number,
                # then convert the expression to gdb's source line expression format
                # e.g. "file.c#15" -> "file.c:15"
                address = e2studio.get_source_line_address(self.debug_session, gdb_expression.replace("#", ":"))
                if (address is None):
                    # No address found with the specified expression, throw exception same as CS+
                    raise Exception("Illegal line number")
                else:
                    return address
            else:
                # The last part after '#' is not a line number,
                # then convert the expression to gdb's symbol expression format
                # e.g. "function#variable" -> "function::variable"
                gdb_expression = gdb_expression.replace("#", "::")
                # In some cases the CS+ source file part is covered by double quotes,
                # then convert it to single quotes as gdb format
                # e.g. "\"D:/path/file.c\"::function" -> "'D:/path/file.c'::function"
                gdb_expression = gdb_expression.replace("\"", "'")
        return e2studio.get_symbol_address(self.debug_session, gdb_expression)

    def GetPC(self):
        return e2studio.get_register(self.debug_session, "pc")

    def Step(self, stepOption=StepOption.Source):
        e2studio.step_in(self.debug_session, stepOption == StepOption.Instruction)

    def IsConnected(self):
        return e2studio.does_debug_session_exist(self.debug_session)

    def XTime(self):
        measurement_result = e2studio.get_time_measurement_result_XTime_wrapper(debugger.debug_session)
        if len(measurement_result) > 0:
            measurement_result_info = measurement_result[0].split("|")
            result = XTimeInfo()
            result.Value = int(measurement_result_info[0])
            result.IsCpuClock = (measurement_result_info[2] == "Debug/CPU clock")
            result.IsOverFlow = (measurement_result_info[3] == "true")
            return result
        return None
