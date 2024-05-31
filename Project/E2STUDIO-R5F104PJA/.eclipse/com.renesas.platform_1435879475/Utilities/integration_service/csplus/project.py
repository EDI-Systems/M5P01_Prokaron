from csplus.project_file import ProjectFile
from csplus.utility import Util
import e2studio

class Project(object):

    File = ProjectFile()

    def __init__(self):
        self.project_name = ""

    def Create(self, projectName, micomType, deviceName, projectKind=ProjectKind.Auto, compiler=Compiler.Auto, subProject=False, settings={}):
        # Validate microcontroller type
        if (not Util.is_str_instance(micomType) or micomType == ""):
            return False
        micomType = str(micomType)
        # Get toolchain type
        if(Compiler.Auto == compiler):
            if(MicomType.RX == micomType):
                compiler = Compiler.CC_RX
            elif(MicomType.RL78 == micomType):
                compiler = Compiler.CC_RL
            elif(MicomType.RA == micomType):
                compiler = Compiler.Renesas_FSP_GCC
        # Get project type (executable or library)
        if(ProjectKind.Auto == projectKind):
            if(MicomType.RX == micomType or MicomType.RL78 == micomType or MicomType.RA == micomType):
                projectKind = ProjectKind.Application
        # Execute project creation
        if(e2studio.create_project(projectName, deviceName, compiler, projectKind, settings) == True):
            self.project_name = projectName
            common._reset_hook()
            return True
        else:
            return False

    def Open(self, fileName, save = False):
        e2studio.import_project(fileName)
        self.project_name = e2studio.get_project_name(fileName)
        common._reset_hook()

    def Change(self, projectName):
        if (projectName != ""):
            try:
                if (e2studio.is_project_open(projectName) == False):
                    e2studio.open_project(projectName)
            except:
                return False
            self.project_name = projectName
            common._reset_hook()
            return True
        else:
            return False

    def Remove(self, projectName, deleteContents = False):
            e2studio.remove_project(projectName, deleteContents)

    def Exists(self, projectName):
        return e2studio.is_project_exists(projectName)

    @property
    def Nickname(self):
        return e2studio.get_device_group(self.project_name)

    def GetDeviceNameList(self, micomType, nickName=""):
        device_list = []
        if (micomType != ""):
            if (nickName == ""):
                device_group_list = e2studio.get_device_group_list(micomType)
                for device_group_name in device_group_list:
                    device_group_pin_list = e2studio.get_device_group_pin_list(micomType, device_group_name)
                    for device_group_pin_name in device_group_pin_list:
                        device_name_list = e2studio.get_device_name_list(micomType, device_group_pin_name)
                        for device_name in device_name_list:
                            device_list.append(device_name)
                return device_list
            else:
                device_name_list = e2studio.get_device_name_list(micomType, nickName)
                for device_name in device_name_list:
                    device_list.append(device_name)
                return device_list
        else:
            raise Exception("Invalid microcontroller")
