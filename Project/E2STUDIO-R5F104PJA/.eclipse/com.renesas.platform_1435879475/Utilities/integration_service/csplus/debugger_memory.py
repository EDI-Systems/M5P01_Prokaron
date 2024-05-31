import e2studio

class DebuggerMemory(object):

    def Read(self, address, memoryOption=MemoryOption.Byte):
        unit_length = self.__get_memory_unit_length(memoryOption)
        data = e2studio.read_memory(debugger.debug_session, address, unit_length)
        big_endian = e2studio.is_big_endian_target(debugger.debug_session)
        return self.__swap_endian(int(data, 16), unit_length, big_endian)

    def ReadRange(self, address, count, memoryOption=MemoryOption.Byte):
        result = []
        unit_length = self.__get_memory_unit_length(memoryOption)
        big_endian = e2studio.is_big_endian_target(debugger.debug_session)
        data = e2studio.read_memory(debugger.debug_session, address, count * unit_length)
        for i in range(count):
            try:
                start = unit_length * 2 * i
                end = start + unit_length * 2
                result.append(self.__swap_endian(int(data[start:end], 16), unit_length, big_endian))
            except:
                result.append(None)
        return result

    def Write(self, address, value, memoryOption=MemoryOption.Byte):
        try:
            unit_length = self.__get_memory_unit_length(memoryOption)
            big_endian = e2studio.is_big_endian_target(debugger.debug_session)
            data = self.__swap_endian(value, unit_length, big_endian)
            e2studio.write_memory(
                debugger.debug_session, address, unit_length,
                "{0:0{1}x}".format(data, unit_length * 2))
            return True
        except:
            return False

    def WriteRange(self, address, valueList, memoryOption=MemoryOption.Byte):
        all_data = ""
        try:
            unit_length = self.__get_memory_unit_length(memoryOption)
            big_endian = e2studio.is_big_endian_target(debugger.debug_session)
            for i in range(len(valueList)):
                data = self.__swap_endian(valueList[i], unit_length, big_endian)
                all_data += str("{0:0{1}x}".format(data, unit_length * 2))
            e2studio.write_memory(debugger.debug_session, address, len(valueList) * unit_length, all_data)
            return True
        except:
            return False

    @property
    def NoVerify(self):
        option_id = self.__get_option_id_verify_on_writing_to_memory()
        if (option_id is not None):
            return e2studio.get_debug_config_option(
                debugger.DebugConfiguration, option_id)
        return None

    @NoVerify.setter
    def NoVerify(self, input_value):
        option_id = self.__get_option_id_verify_on_writing_to_memory()
        if (option_id is not None):
            e2studio.set_debug_config_option(
                debugger.DebugConfiguration, option_id, input_value)

    def __get_option_id_verify_on_writing_to_memory(self):
        debug_tool = self.__get_debug_tool()
        if "rl78" in debug_tool:
            return self.__get_option_id_verify_on_writing_to_memory_RL78(debug_tool)
        elif "rh850" in debug_tool:
            return self.__get_option_id_verify_on_writing_to_memory_RH850(debug_tool)
        elif "rx" in debug_tool:
            return self.__get_option_id_verify_on_writing_to_memory_RX(debug_tool)
        return None

    def __get_option_id_verify_on_writing_to_memory_RL78(self, debug_tool):
        if "e1" in debug_tool or "e2" in debug_tool or "ez" in debug_tool:
            return "com.renesas.hardwaredebug.e1rl78.verify.on.writing.to.memory"
        elif "iecube" in debug_tool:
            return "com.renesas.hardwaredebug.iecube.verify.on.writing.to.memory"
        elif "serial" in debug_tool:
            return "com.renesas.hardwaredebug.rl78.serial.verify.on.writing.to.memory"
        return None

    def __get_option_id_verify_on_writing_to_memory_RH850(self, debug_tool):
        if "e1" in debug_tool:
            return "com.renesas.hardwaredebug.e1rh850.verify_on_writing_to_memory"
        elif "e2" in debug_tool:
            return "com.renesas.hardwaredebug.rh850.e2.verify_on_writing_to_memory"
        return None

    def __get_option_id_verify_on_writing_to_memory_RX(self, debug_tool):
        if "e1" in debug_tool:
            return "com.renesas.hardwaredebug.e1.verify.on.writing.to.memory"
        elif "e2.e2rx" in debug_tool:
            return "com.renesas.hardwaredebug.rx.e2.verify.on.writing.to.memory"
        elif "e20" in debug_tool:
            return "com.renesas.hardwaredebug.e20.verify.on.writing.to.memory"
        elif "e2" in debug_tool:
            return "com.renesas.hardwaredebug.rx.e2lite.verify.on.writing.to.memory"
        elif "ez" in debug_tool:
            return "com.renesas.hardwaredebug.ez.verify.on.writing.to.memory"
        elif "jlink" in debug_tool:
            return "com.renesas.hardwaredebug.rx.jlink.verify.on.writing.to.memory"
        return None

    def __get_memory_unit_length(self, unit):
        if (unit == MemoryOption.Word):
            return 4
        elif (unit == MemoryOption.HalfWord):
            return 2
        else:
            return 1

    def __swap_endian(self, data, size, big_endian):
        if (big_endian):
            return data
        else:
            if (size == 4):
                return (((data << 24) & 0xFF000000) |
                        ((data <<  8) & 0x00FF0000) |
                        ((data >>  8) & 0x0000FF00) |
                        ((data >> 24) & 0x000000FF))
            elif (size == 2):
                return (((data << 8) & 0xFF00) |
                        ((data >> 8) & 0x00FF))
            else:
                return data

    def __get_debug_tool(self):
        return e2studio.get_debug_config_option(
            debugger.DebugConfiguration,
            "com.renesas.cdt.core.jtagDeviceId")
