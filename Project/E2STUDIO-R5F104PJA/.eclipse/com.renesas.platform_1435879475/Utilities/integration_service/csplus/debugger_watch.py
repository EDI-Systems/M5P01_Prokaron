import sys
import e2studio

class DebuggerWatch(object):

    def __init__(self):
        self.__all_watch_options = [
            WatchOption.Decimal,
            WatchOption.Hexdecimal,
            WatchOption.SignedDecimal,
            WatchOption.UnsignedDecimal,
            WatchOption.Octal,
            WatchOption.Binary,
            WatchOption.Float,
            WatchOption.Double,
            WatchOption.Auto,
            WatchOption.String,
            WatchOption.Sizeof
            ]
        self.__unsigned_watch_options = [
            WatchOption.Decimal,
            WatchOption.Hexdecimal,
            WatchOption.UnsignedDecimal,
            WatchOption.Octal,
            WatchOption.Binary
            ]

    def GetValue(self, variableName, encode=Encoding.Default, watchOption=WatchOption.Auto):
        # Script may skip encode and specify watchOption only
        # which will make watchOption value stay in encode
        if (encode in self.__all_watch_options):
            # Move watchOption value back from encode
            watchOption = encode
            # Set default for encode
            encode = Encoding.Default

        # Convert variable name from CS+ format to e2 studio format
        expression = self.__convert_expression(variableName)

        if (watchOption == WatchOption.Sizeof):
            return e2studio.get_variable_size(debugger.debug_session, expression)

        # Get size, type, value of expression as array
        expression_information = e2studio.get_expression_info(debugger.debug_session, expression).split(";")

        variable_size = int(expression_information[0])
        variable_type = expression_information[1]
        variable_value = None
        if (variable_type == "FLOAT"):
            variable_value = float(expression_information[2])
        else:
            variable_value = int(expression_information[2], 16)

        # Convert variable value according to the selected WatchOption
        if (watchOption in self.__unsigned_watch_options):
            return self.__convert_to_unsigned(variable_value, variable_type, variable_size)
        elif (watchOption == WatchOption.SignedDecimal):
            return self.__convert_to_signed(variable_value, variable_type, variable_size)
        elif (watchOption == WatchOption.Float):
            return self.__convert_to_float(variable_value, variable_type)
        elif (watchOption == WatchOption.Double):
            return self.__convert_to_double(variable_value, variable_type)
        elif (watchOption == WatchOption.String):
            return self.__convert_to_string(variable_value, variable_type, variable_size, encode)
        else:
            return self.__convert_auto(variable_value, variable_type, variable_size, encode)

    def SetValue(self, variableName, value):
        # Convert variable name from CS+ format to e2 studio format
        expression = self.__convert_expression(variableName)

        try:
            # <int> type exist in both Python 2 and Python 3, <long> type is removed from Python 3
            if (isinstance(value, int) or ((sys.version_info[0] < 3) and isinstance(value, long))):
                e2studio.set_integer_expression(debugger.debug_session, expression, value)
            elif (isinstance(value, float)):
                e2studio.set_float_expression(debugger.debug_session, expression, value)
            else:
                # Cannot set expression if other type of value is used
                return False
            # No exception means success to set expression
            return True
        except:
            # Exception means failed to set expression
            return False

    def __convert_expression(self, expression):
        gdb_expression = expression
        if ("$" in gdb_expression):
            # Strip the loadmodule part out of expression
            # e.g.
            # "loadmodule.abs$function#variable" -> "function#variable"
            gdb_expression = gdb_expression.rsplit("$", 1)[1]
        if ("#" in gdb_expression):
            # Convert the expression to gdb's symbol expression format
            # e.g. "function#variable" -> "function::variable"
            gdb_expression = gdb_expression.replace("#", "::")
            # In some cases the CS+ source file part is covered by double quotes,
            # then convert it to single quotes as gdb format
            # e.g. "\"D:/path/file.c\"::variable" -> "'D:/path/file.c'::variable"
            gdb_expression = gdb_expression.replace("\"", "'")
        return gdb_expression

    def __convert_to_unsigned(self, variable_value, variable_type, variable_size):
        # Raise same exception as CS+ if value is not an integer
        # (value of string which is pointer of char is an integer)
        if (not (("INT" in variable_type) or (variable_type == "STRING"))):
            raise Exception("Input string was not in a correct format.")

        max_value = 2**(variable_size*8)
        if (variable_value < 0):
            return variable_value + max_value
        else:
            return variable_value

    def __convert_to_signed(self, variable_value, variable_type, variable_size):
        # Raise same exception as CS+ if value is not an integer
        # (value of string which is pointer of char is an integer)
        if (not (("INT" in variable_type) or (variable_type == "STRING"))):
            raise Exception("Input string was not in a correct format.")

        max_value = 2**(variable_size*8)
        if (variable_value >= (max_value/2)):
            return variable_value - max_value
        else:
            return variable_value

    def __convert_to_float(self, variable_value, variable_type):
        if (variable_type == "FLOAT"):
            return variable_value
        else:
            # Raise exception same as CS+ if it's not float value
            raise Exception("Input string was not in a correct format.")

    def __convert_to_double(self, variable_value, variable_type):
        # Return string for double value, which is FLOAT type in e2 studio
        if (variable_type == "FLOAT"):
            return str(variable_value)
        else:
            return "?"

    def __convert_to_string(self, variable_value, variable_type, variable_size, encode):
        # Try to get the unsigned value first
        unsigned_value = variable_value
        try:
            unsigned_value = self.__convert_to_unsigned(variable_value, variable_type, variable_size)
        except:
            # This means FLOAT value
            return "?"

        # The array of bytes to be decoded
        byte_value = []
        # The format of the result string
        value_format = u"{}"
        if (variable_type == "STRING"):
            # Update the result format of string (surround by ")
            value_format = u'"{}"'
            # The value of char pointer is the address in memory to read string data from
            address = unsigned_value
            # UTF-32 is a fixed-length encoding so need to handle differently
            if (encode == Encoding.UTF32):
                # Read memory from the value of pointer
                # until null word (4 bytes of "00") is read or maximum 250 bytes are read
                word = e2studio.read_memory(debugger.debug_session, address, 4)
                while ((word != "00000000") and (len(byte_value) < 250)):
                    # Push each byte of the word into the byte array
                    while (word != ""):
                        byte_value.append(int(word[:2], 16))
                        word = word[2:]
                    address += 4
                    # Read the next word
                    word = e2studio.read_memory(debugger.debug_session, address, 4)
            else:
                # Read memory from the value of pointer
                # until null byte "00" is read or maximum 250 bytes are read
                byte = e2studio.read_memory(debugger.debug_session, address, 1)
                while ((byte != "00") and (len(byte_value) < 250)):
                    byte_value.append(int(byte, 16))
                    address += 1
                    # Read 1 byte each time is enough for normal use case
                    byte = e2studio.read_memory(debugger.debug_session, address, 1)
        elif (unsigned_value <= 0xff):
            # Update the result format of character (surround by ')
            value_format = u"'{}'"
            byte_value.append(unsigned_value)
        else:
            # Result of a number bigger than char
            return "'?'"
        # Decode the read byte array with the specified encode,
        # replace unknown character by question mark ?,
        # and put the result in the dedicated format
        result = value_format.format(bytearray(byte_value).decode(encode, "replace"))
        if (isinstance(result, str)):
            # In Python 3, result is <str> type so just return it
            return result
        else:
            # In Python 2, result is <unicode> type, so convert it to <str> type
            # This requires user script files to be written in UTF-8 encoding
            return result.encode(Encoding.UTF8, "replace")

    def __convert_auto(self, variable_value, variable_type, variable_size, encode):
        # char and string will be converted to string,
        # other type will be returned as it is
        if (variable_type.endswith("INT8") or (variable_type == "STRING")):
            return self.__convert_to_string(variable_value, variable_type, variable_size, encode)
        else:
            return variable_value
