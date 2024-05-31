import e2studio

class DebuggerBreakpoint(object):

    def Set(self, breakCondition):
        if ((breakCondition.BreakType != BreakType.Hardware)
                and (breakCondition.BreakType != BreakType.Software)):
            # Break types other than Hardware and Software need Eventpoints support
            # so temporary raise exception for now
            raise Exception("Break type '{}' is not supported".format(breakCondition.BreakType))

        address = debugger.Address(breakCondition.Address)
        return e2studio.add_address_breakpoint(address, breakCondition.BreakType)

    def Delete(self, breakNumber=""):
        try:
            if (breakNumber == ""):
                e2studio.remove_all_breakpoints()
                return True
            else:
                breakpoints = self.Information()
                for breakpoint in breakpoints:
                    if (breakpoint.Number == breakNumber):
                        e2studio.remove_address_breakpoint(debugger.Address(breakpoint.Address1))
                        return True
                return False
        except:
            return False

    def Information(self):
        e2s_breakpoints = e2studio.get_all_breakpoints()
        csp_breakpoints = []
        for e2s_breakpoint in e2s_breakpoints:
            e2s_breakpoint_info = e2s_breakpoint.split(",")
            csp_breakpoint = BreakpointInfo()
            csp_breakpoint.Number = int(e2s_breakpoint_info[0])
            csp_breakpoint.Enable = (e2s_breakpoint_info[1] == "true")
            csp_breakpoint.BreakType = None
            if (e2s_breakpoint_info[2] == "hardware"):
                csp_breakpoint.BreakType = BreakType.Hardware
            elif (e2s_breakpoint_info[2] == "software"):
                csp_breakpoint.BreakType = BreakType.Software
            csp_breakpoint.Address1 = e2s_breakpoint_info[4]
            csp_breakpoints.append(csp_breakpoint)
        return csp_breakpoints
