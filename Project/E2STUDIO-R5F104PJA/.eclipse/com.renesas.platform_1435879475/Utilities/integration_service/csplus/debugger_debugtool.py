import e2studio

class DebuggerDebugTool(object):

    def GetType(self):
        debug_tool = self.__get_debug_tool()
        if "simulator" in debug_tool:
            return DebugTool.Simulator
        elif "iecube" in debug_tool:
            return DebugTool.Iecube
        elif "ez" in debug_tool:
            return DebugTool.EZ_Emulator
        elif "e1" in debug_tool:
            if "rl78" in debug_tool:
                return DebugTool.E1Serial
            elif "rh850" in debug_tool:
                return DebugTool.E1Lpd
            elif "rx" in debug_tool:
                connection_type = e2studio.get_debug_config_option(
                    debugger.DebugConfiguration,
                    "com.renesas.hardwaredebug.e1.jtag.or.fine")
                if connection_type == "0":
                    return DebugTool.E1Jtag
                elif connection_type == "1":
                    return DebugTool.E1Serial
        elif "e20" in debug_tool:
            if "rl78" in debug_tool:
                return DebugTool.E20Serial
            elif "rh850" in debug_tool:
                return DebugTool.E20Lpd
            elif "rx" in debug_tool:
                connection_type = e2studio.get_debug_config_option(
                    debugger.DebugConfiguration,
                    "com.renesas.hardwaredebug.e20.jtag.or.fine")
                if connection_type == "0":
                    return DebugTool.E20Jtag
                elif connection_type == "1":
                    return DebugTool.E20Serial
        elif "e2.e2" in debug_tool or debug_tool.endswith("arm.e2"):
            # ID of RX/RL78/RH850 + E2 is
            # com.renesas.hardwaredebug.[rx|rl78|rh850].e2.e2[rx|rl78|rh850]
            # while ARM is com.renesas.hardwaredebug.arm.e2
            return DebugTool.E2
        elif debug_tool.endswith("e2") or debug_tool.endswith("arm.e2lite"):
            # ID of RX/RL78/RH850 + E2Lite is
            # com.renesas.hardwaredebug.[rx|rl78|rh850].e2
            # while ARM is com.renesas.hardwaredebug.arm.e2lite
            return DebugTool.E2Lite
        elif "rl78.serial" in debug_tool:
            # ID of RL78 ComPort is
            # com.renesas.hardwaredebug.rl78.serial
            return DebugTool.ComPort
        return None

    def Change(self, debugTool):
        try:
            current_debug_tool = self.__get_debug_tool()
            if debugTool == DebugTool.Simulator:
                if "rx" in current_debug_tool:
                    self.__change_debug_tool("com.renesas.hardwaredebug.simulator.rx")
                elif "rl78" in current_debug_tool:
                    self.__change_debug_tool("com.renesas.hardwaredebug.simulator.rl78")
                else:
                    raise Exception("The specified debug tool is not supported.")
            elif debugTool == DebugTool.Iecube:
                if "rl78" in current_debug_tool:
                    self.__change_debug_tool("com.renesas.hardwaredebug.rl78.iecube")
                else:
                    raise Exception("The specified debug tool is not supported.")
            elif debugTool == DebugTool.EZ_Emulator:
                if "rx" in current_debug_tool:
                    self.__change_debug_tool("com.renesas.hardwaredebug.rx.ez")
                elif "rl78" in current_debug_tool:
                    self.__change_debug_tool("com.renesas.hardwaredebug.rl78.ez")
                else:
                    raise Exception("The specified debug tool is not supported.")
            elif debugTool == DebugTool.E1Jtag:
                if "rx" in current_debug_tool:
                    self.__change_debug_tool("com.renesas.hardwaredebug.rx.e1")
                    e2studio.set_debug_config_option(
                        debugger.DebugConfiguration,
                        "com.renesas.hardwaredebug.e1.jtag.or.fine",
                        "0")
                else:
                    raise Exception("The specified debug tool is not supported.")
            elif debugTool == DebugTool.E1Serial:
                if "rx" in current_debug_tool:
                    self.__change_debug_tool("com.renesas.hardwaredebug.rx.e1")
                    e2studio.set_debug_config_option(
                        debugger.DebugConfiguration,
                        "com.renesas.hardwaredebug.e1.jtag.or.fine",
                        "1")
                elif "rl78" in current_debug_tool:
                    self.__change_debug_tool("com.renesas.hardwaredebug.rl78.e1")
                else:
                    raise Exception("The specified debug tool is not supported.")
            elif debugTool == DebugTool.E1Lpd:
                if "rh850" in current_debug_tool:
                    self.__change_debug_tool("com.renesas.hardwaredebug.rh850.e1")
                else:
                    raise Exception("The specified debug tool is not supported.")
            elif debugTool == DebugTool.E20Jtag:
                if "rx" in current_debug_tool:
                    self.__change_debug_tool("com.renesas.hardwaredebug.rx.e20")
                    e2studio.set_debug_config_option(
                        debugger.DebugConfiguration,
                        "com.renesas.hardwaredebug.e20.jtag.or.fine",
                        "0")
                else:
                    raise Exception("The specified debug tool is not supported.")
            elif debugTool == DebugTool.E20Serial:
                if "rx" in current_debug_tool:
                    self.__change_debug_tool("com.renesas.hardwaredebug.rx.e20")
                    e2studio.set_debug_config_option(
                        debugger.DebugConfiguration,
                        "com.renesas.hardwaredebug.e20.jtag.or.fine",
                        "1")
                elif "rl78" in current_debug_tool:
                    self.__change_debug_tool("com.renesas.hardwaredebug.rl78.e1")
                else:
                    raise Exception("The specified debug tool is not supported.")
            elif debugTool == DebugTool.E2:
                if "rx" in current_debug_tool:
                    self.__change_debug_tool("com.renesas.hardwaredebug.rx.e2.e2rx")
                elif "rl78" in current_debug_tool:
                    self.__change_debug_tool("com.renesas.hardwaredebug.rl78.e2.e2rl78")
                elif "rh850" in current_debug_tool:
                    self.__change_debug_tool("com.renesas.hardwaredebug.rh850.e2.e2rh850")
                elif "arm" in current_debug_tool or current_debug_tool.endswith("rz.jlink"):
                    # RA/RE + JLink uses com.renesas.hardwaredebug.rz.jlink as debug tool ID
                    # so allow it to change to E2.
                    # For RZ, as it only support JLink, there should be no case
                    # that user want to change to E2.
                    self.__change_debug_tool("com.renesas.hardwaredebug.arm.e2")
                else:
                    raise Exception("The specified debug tool is not supported.")
            elif debugTool == DebugTool.E2Lite:
                if "rx" in current_debug_tool:
                    self.__change_debug_tool("com.renesas.hardwaredebug.rx.e2")
                elif "rl78" in current_debug_tool:
                    self.__change_debug_tool("com.renesas.hardwaredebug.rl78.e2")
                elif "arm" in current_debug_tool or current_debug_tool.endswith("rz.jlink"):
                    # RA/RE + JLink uses com.renesas.hardwaredebug.rz.jlink as debug tool ID
                    # so allow it to change to E2Lite.
                    # For RZ, as it only support JLink, there should be no case
                    # that user want to change to E2Lite.
                    self.__change_debug_tool("com.renesas.hardwaredebug.arm.e2lite")
                else:
                    raise Exception("The specified debug tool is not supported.")
            elif debugTool == DebugTool.ComPort:
                if "rl78" in current_debug_tool:
                    self.__change_debug_tool("com.renesas.hardwaredebug.rl78.serial")
                else:
                    raise Exception("The specified debug tool is not supported.")
            else:
                raise Exception("The specified debug tool is not supported.")
            common._reset_hook()
            return True
        except Exception as e:
            print(e)
            return False

    def __get_debug_tool(self):
        return e2studio.get_debug_config_option(
            debugger.DebugConfiguration,
            "com.renesas.cdt.core.jtagDeviceId")

    def __change_debug_tool(self, new_debug_tool_id):
        e2studio.set_debug_config_option(
            debugger.DebugConfiguration,
            "com.renesas.cdt.core.jtagDeviceId",
            new_debug_tool_id)
