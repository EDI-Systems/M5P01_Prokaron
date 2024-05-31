import sys
if (sys.version_info[0] > 2):
    import builtins as builtin
else:
    import __builtin__ as builtin

# Globalize enums
from csplus.debug_tool import DebugTool
builtin.DebugTool = DebugTool()
from csplus.download_option import DownloadOption
builtin.DownloadOption = DownloadOption()
from csplus.vendor_type import VendorType
builtin.VendorType = VendorType()
from csplus.go_option import GoOption
builtin.GoOption = GoOption()
from csplus.break_type import BreakType
builtin.BreakType = BreakType()
from csplus.memory_option import MemoryOption
builtin.MemoryOption = MemoryOption()
from csplus.step_option import StepOption
builtin.StepOption = StepOption()
from csplus.watch_option import WatchOption
builtin.WatchOption = WatchOption()
from csplus.encoding import Encoding
builtin.Encoding = Encoding()
from csplus.project_kind import ProjectKind
builtin.ProjectKind = ProjectKind()
from csplus.compiler import Compiler
builtin.Compiler = Compiler()
from csplus.micom_type import MicomType
builtin.MicomType = MicomType()
from csplus.after_trace_memory_full import AfterTraceMemoryFull
builtin.AfterTraceMemoryFull = AfterTraceMemoryFull()
from csplus.trace_priority import TracePriority
builtin.TracePriority = TracePriority()

# Globalize data classes
from csplus.break_condition import BreakCondition
builtin.BreakCondition = BreakCondition
from csplus.breakpoint_info import BreakpointInfo
builtin.BreakpointInfo = BreakpointInfo
from csplus.xtime_info import XTimeInfo
builtin.XTimeInfo = XTimeInfo

# Globalize modules
from csplus.project import Project
builtin.project = Project()
from csplus.build import Build
builtin.build = Build()
from csplus.debugger import Debugger
builtin.debugger = Debugger()
from csplus.common import Common
builtin.common = Common()

# Globalize functions without module
builtin.CubeSuiteExit = builtin.common.CubeSuiteExit
builtin.Hook = builtin.common.Hook
