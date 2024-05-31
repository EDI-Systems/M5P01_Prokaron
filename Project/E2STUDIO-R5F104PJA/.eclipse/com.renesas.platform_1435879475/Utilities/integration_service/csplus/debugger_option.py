import e2studio

class DebuggerOption(object):

    @property
    def MainClockFrequency(self):
        option_id = self.__get_option_id_main_clock_freq()
        if (option_id is not None):
            # value get from debug_config is MHz
            return_value = float(e2studio.get_debug_config_option(
                debugger.DebugConfiguration, option_id))
            # convert to kHz before return
            return int(return_value * 1000)
        return None

    @MainClockFrequency.setter
    def MainClockFrequency(self, input_value):
        option_id = self.__get_option_id_main_clock_freq()
        if (option_id is not None):
            # input_value is kHz
            # convert to MHz before setting to debug_config
            option_value = float(input_value) / 1000
            e2studio.set_debug_config_option(
                debugger.DebugConfiguration, option_id, str(option_value))

    def __get_option_id_main_clock_freq(self):
        debug_tool = self.__get_debug_tool()
        if "rl78" in debug_tool:
            return self.__get_option_id_main_clock_freq_RL78(debug_tool)
        elif debug_tool.endswith("arm.e2") or debug_tool.endswith("arm.e2lite"):
            return self.__get_option_id_main_clock_freq_ARM(debug_tool)
        elif "rh850" in debug_tool:
            return self.__get_option_id_main_clock_freq_RH850()
        elif "rx" in debug_tool:
            return self.__get_option_id_main_clock_freq_RX(debug_tool)
        return None

    def __get_option_id_main_clock_freq_RL78(self, debug_tool):
        if "simulator" in debug_tool or "e1" in debug_tool or "e2" in debug_tool or "ez" in debug_tool:
            return "com.renesas.hardwaredebug.e1rl78.main.clock.freq"
        elif "iecube" in debug_tool:
            return "com.renesas.hardwaredebug.iecube.main.clock.freq"
        elif "serial" in debug_tool:
            return "com.renesas.hardwaredebug.rl78.serial.main.clock.freq"
        return None

    def __get_option_id_main_clock_freq_ARM(self, debug_tool):
        return debug_tool + ".inputclock"

    def __get_option_id_main_clock_freq_RH850(self):
        return "com.renesas.hardwaredebug.e1rh850.mainFreq"

    def __get_option_id_main_clock_freq_RX(self, debug_tool):
        if "e1" in debug_tool:
            return "com.renesas.hardwaredebug.e1.inputclock"
        elif "e2.e2rx" in debug_tool:
            return "com.renesas.hardwaredebug.rx.e2.inputclock"
        elif "e20" in debug_tool:
            return "com.renesas.hardwaredebug.e20.inputclock"
        elif "e2" in debug_tool:
            return "com.renesas.hardwaredebug.rx.e2lite.inputclock"
        elif "ez" in debug_tool:
            return "com.renesas.hardwaredebug.ez.inputclock"
        elif "jlink" in debug_tool:
            return "com.renesas.hardwaredebug.rx.jlink.inputclock"
        return None

    def __get_debug_tool(self):
        return e2studio.get_debug_config_option(
            debugger.DebugConfiguration,
            "com.renesas.cdt.core.jtagDeviceId")

    @property
    def ExecuteAfterReset(self):
        try:
            option_id = self.__get_option_id_execute_after_reset()
            # value get from debug_config is Boolean
            return e2studio.get_debug_config_option(
                debugger.DebugConfiguration, option_id)
        except:
            return False

    @ExecuteAfterReset.setter
    def ExecuteAfterReset(self, input_value):
        option_id = self.__get_option_id_execute_after_reset()
        e2studio.set_debug_config_option(
                debugger.DebugConfiguration, option_id, input_value)

    def __get_option_id_execute_after_reset(self):
        return "org.eclipse.cdt.debug.gdbjtag.core.setResume"

    @property
    def Trace(self):
        if debugger.debug_session is "":
            # No debug session, get from launch configuration
            debug_tool = self.__get_debug_tool()
            if "arm.e2" in debug_tool:
                # Both ARM+E2 (device ID ends with "arm.e2") and ARM+E2Lite (device ID ends with "arm.e2lite")
                # use the same option ID for MTB trace (ends with "arm.e2.traceMTB")
                return e2studio.get_debug_config_option(
                    debugger.DebugConfiguration,
                    "com.renesas.hardwaredebug.arm.e2.traceMTB")
        else:
            e2studio.select_trace_provider("com.renesas.cdt.debug.trace.ui.scripting.standardtrace")
            if e2studio.is_trace_supported(debugger.debug_session):
                setting = e2studio.get_trace_settings(debugger.debug_session, ["TraceEnable"])
                if "TraceEnable" in setting.keys():
                    return setting["TraceEnable"] == "true"
        return None

    @Trace.setter
    def Trace(self, enable):
        if debugger.debug_session is "":
            # No debug session, set to launch configuration
            debug_tool = self.__get_debug_tool()
            if "arm.e2" in debug_tool:
                # Both ARM+E2 (device ID ends with "arm.e2") and ARM+E2Lite (device ID ends with "arm.e2lite")
                # use the same option ID for MTB trace (ends with "arm.e2.traceMTB")
                e2studio.set_debug_config_option(
                    debugger.DebugConfiguration,
                    "com.renesas.hardwaredebug.arm.e2.traceMTB",
                    enable)
        else:
            e2studio.select_trace_provider("com.renesas.cdt.debug.trace.ui.scripting.standardtrace")
            if e2studio.is_trace_supported(debugger.debug_session):
                e2studio.set_trace_settings(debugger.debug_session, {"TraceEnable":str(enable)})

    @property
    def AfterTraceMemoryFull(self):
        if debugger.debug_session is "":
            raise Exception("Debug session is not available")
        else:
            e2studio.select_trace_provider("com.renesas.cdt.debug.trace.ui.scripting.standardtrace")
            if e2studio.is_trace_supported(debugger.debug_session):
                setting = e2studio.get_trace_settings(debugger.debug_session, ["TraceMode"])
                if "TraceMode" in setting.keys():
                    return self.__get_trace_mode_enum(setting["TraceMode"])
        return None

    @AfterTraceMemoryFull.setter
    def AfterTraceMemoryFull(self, mode):
        if debugger.debug_session is "":
            raise Exception("Debug session is not available")
        else:
            e2studio.select_trace_provider("com.renesas.cdt.debug.trace.ui.scripting.standardtrace")
            if e2studio.is_trace_supported(debugger.debug_session):
                trace_mode = self.__get_trace_mode(mode)
                if trace_mode is None:
                    raise Exception("Invalid parameters")
                else:
                    e2studio.set_trace_settings(debugger.debug_session, {"TraceMode":trace_mode})

    def __get_trace_mode_enum(self, mode):
        # AfterTraceMemoryFull.NoneStop       Fill until stop, overwrite trace memory and continue execution
        # AfterTraceMemoryFull.StopTrace      Fill until full, stop tracing
        # AfterTraceMemoryFull.Stop           Fill until full then break, stop execution (stop the program)
        if (mode == "FillUntilStop"):
            return AfterTraceMemoryFull.NoneStop # Fill until stop
        elif (mode == "FillUntilFull"):
            return AfterTraceMemoryFull.StopTrace # Fill until full
        elif (mode == "FillUntilFullBreak"):
            return AfterTraceMemoryFull.Stop # Stop program when full
        else:
            return None

    def __get_trace_mode(self, mode):
        # AfterTraceMemoryFull.NoneStop       Fill until stop, overwrite trace memory and continue execution
        # AfterTraceMemoryFull.StopTrace      Fill until full, stop tracing
        # AfterTraceMemoryFull.Stop           Fill until full then break, stop execution (stop the program)
        if (mode == AfterTraceMemoryFull.NoneStop):
            return "FillUntilStop" # Fill until stop
        elif (mode == AfterTraceMemoryFull.StopTrace):
            return "FillUntilFull" # Fill until full
        elif (mode == AfterTraceMemoryFull.Stop):
            return "FillUntilFullBreak" # Stop program when full
        else:
            return None

    @property
    def TracePriority(self):
        if debugger.debug_session is "":
            raise Exception("Debug session is not available")
        else:
            e2studio.select_trace_provider("com.renesas.cdt.debug.trace.ui.scripting.standardtrace")
            if e2studio.is_trace_supported(debugger.debug_session):
                setting = e2studio.get_trace_settings(debugger.debug_session, ["TraceOutput"])
                if "TraceOutput" in setting.keys():
                    return self.__get_trace_priority(setting["TraceOutput"])
        return None

    @TracePriority.setter
    def TracePriority(self, priority):
        if debugger.debug_session is "":
            raise Exception("Debug session is not available")
        else:
            e2studio.select_trace_provider("com.renesas.cdt.debug.trace.ui.scripting.standardtrace")
            if e2studio.is_trace_supported(debugger.debug_session):
                trace_output = self.__get_trace_output(priority)
                if trace_output is None:
                    raise Exception("Invalid parameters")
                else:
                    e2studio.set_trace_settings(debugger.debug_session, {"TraceOutput":trace_output})

    def __get_trace_priority(self, output):
        if (output == "TraceOutput"):
            return TracePriority.DataPriority
        elif (output == "CPUExecution"):
            return TracePriority.SpeedPriority
        else:
            return None

    def __get_trace_output(self, priority):
        if (priority == TracePriority.DataPriority):
            return "TraceOutput"
        elif (priority == TracePriority.SpeedPriority):
            return "CPUExecution"
        else:
            return None

    @property
    def AccumulateTraceTime(self):
        if debugger.debug_session is "":
            raise Exception("Debug session is not available")
        else:
            e2studio.select_trace_provider("com.renesas.cdt.debug.trace.ui.scripting.standardtrace")
            if e2studio.is_trace_supported(debugger.debug_session):
                setting = e2studio.get_trace_settings(debugger.debug_session, ["TraceTimestampDisplayMode"])
                if "TraceTimestampDisplayMode" in setting.keys():
                    return setting["TraceTimestampDisplayMode"] == "Accumulate"
        return None

    @AccumulateTraceTime.setter
    def AccumulateTraceTime(self, isAccumulate):
        if debugger.debug_session is "":
            raise Exception("Debug session is not available")
        else:
            e2studio.select_trace_provider("com.renesas.cdt.debug.trace.ui.scripting.standardtrace")
            if e2studio.is_trace_supported(debugger.debug_session):
                if isAccumulate is True:
                    e2studio.set_trace_settings(debugger.debug_session, {"TraceTimestampDisplayMode":"Accumulate"})
                elif isAccumulate is False:
                    e2studio.set_trace_settings(debugger.debug_session, {"TraceTimestampDisplayMode":"Differential"})
                else:
                    raise Exception("Invalid parameters")

    @property
    def TraceBranchPC(self):
        if debugger.debug_session is "":
            raise Exception("Debug session is not available")
        else:
            e2studio.select_trace_provider("com.renesas.cdt.debug.trace.ui.scripting.standardtrace")
            if e2studio.is_trace_supported(debugger.debug_session):
                setting = e2studio.get_trace_settings(debugger.debug_session, ["TraceType"])
                if "TraceType" in setting.keys():
                    return setting["TraceType"] == "Branch" or setting["TraceType"] == "BranchDataAccess"
        return None

    @TraceBranchPC.setter
    def TraceBranchPC(self, isTraceBranchPC):
        if debugger.debug_session is "":
            raise Exception("Debug session is not available")
        else:
            isTraceDataAccess = self.TraceDataAccess
            if isTraceDataAccess is not None:
                if isTraceBranchPC is True and isTraceDataAccess is True:
                    e2studio.set_trace_settings(debugger.debug_session, {"TraceType":"BranchDataAccess"})
                elif isTraceBranchPC is True and isTraceDataAccess is False:
                    e2studio.set_trace_settings(debugger.debug_session, {"TraceType":"Branch"})
                elif isTraceBranchPC is False and isTraceDataAccess is True:
                    e2studio.set_trace_settings(debugger.debug_session, {"TraceType":"DataAccess"})
                elif isTraceBranchPC is False and isTraceDataAccess is False:
                    raise Exception("Invalid Setting")
                else:
                    raise Exception("Invalid parameters")

    @property
    def TraceDataAccess(self):
        if debugger.debug_session is "":
            raise Exception("Debug session is not available")
        else:
            e2studio.select_trace_provider("com.renesas.cdt.debug.trace.ui.scripting.standardtrace")
            if e2studio.is_trace_supported(debugger.debug_session):
                setting = e2studio.get_trace_settings(debugger.debug_session, ["TraceType"])
                if "TraceType" in setting.keys():
                    return setting["TraceType"] == "DataAccess" or setting["TraceType"] == "BranchDataAccess"
        return None

    @TraceDataAccess.setter
    def TraceDataAccess(self, isTraceDataAccess):
        if debugger.debug_session is "":
            raise Exception("Debug session is not available")
        else:
            isTraceBranchPC = self.TraceBranchPC
            if isTraceBranchPC is not None:
                if isTraceDataAccess is True and isTraceBranchPC is True:
                    e2studio.set_trace_settings(debugger.debug_session, {"TraceType":"BranchDataAccess"})
                elif isTraceDataAccess is True and isTraceBranchPC is False:
                    e2studio.set_trace_settings(debugger.debug_session, {"TraceType":"DataAccess"})
                elif isTraceDataAccess is False and isTraceBranchPC is True:
                    e2studio.set_trace_settings(debugger.debug_session, {"TraceType":"Branch"})
                elif isTraceDataAccess is False and isTraceBranchPC is False:
                    raise Exception("Invalid Setting")
                else:
                    raise Exception("Invalid parameters")
