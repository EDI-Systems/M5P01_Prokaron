import e2studio
from csplus.utility import Util

class BuildLink(object):

    @property
    def SectionStartAddress(self):
        if (project.project_name != ""):
            build_config_name = e2studio.get_active_build_config_name(project.project_name)
            toolchainId = e2studio.get_toolchain_id(project.project_name, build_config_name)
            if (toolchainId == "Renesas_CCRL"):
                toolId = "com.renesas.cdt.managedbuild.renesas.ccrl.base.linker"
                optionId = "com.renesas.cdt.managedbuild.renesas.ccrl.linker.option.section"
            elif (toolchainId == "Renesas_RXC"):
                toolId = "com.renesas.cdt.managedbuild.renesas.ccrx.base.linker"
                optionId = "com.renesas.cdt.managedbuild.renesas.ccrx.linker.option.linkerSection"
            else:
                raise Exception("Invalid toolchainId")
            return e2studio.get_build_config_option(project.project_name, build_config_name, toolId, optionId)
        else:
            raise Exception("Invalid project name")

    @SectionStartAddress.setter
    def SectionStartAddress(self, value):
        if (project.project_name != ""):
            if (isinstance(value, str)):
                build_config_name = e2studio.get_active_build_config_name(project.project_name)
                toolchainId = e2studio.get_toolchain_id(project.project_name, build_config_name)
                if (toolchainId == "Renesas_CCRL"):
                    toolId = "com.renesas.cdt.managedbuild.renesas.ccrl.base.linker"
                    optionId = "com.renesas.cdt.managedbuild.renesas.ccrl.linker.option.section"
                elif (toolchainId == "Renesas_RXC"):
                    toolId = "com.renesas.cdt.managedbuild.renesas.ccrx.base.linker"
                    optionId = "com.renesas.cdt.managedbuild.renesas.ccrx.linker.option.linkerSection"
                else:
                    raise Exception("Invalid toolchainId")
                e2studio.set_build_config_option(project.project_name, build_config_name, toolId, optionId, value)
            else:
                raise Exception("Value must be string")
        else:
            raise Exception("Invalid project name")

    @property
    def LibraryFile(self):
        if (project.project_name != ""):
            build_config_name = e2studio.get_active_build_config_name(project.project_name)
            toolchainId = e2studio.get_toolchain_id(project.project_name, build_config_name)
            if (toolchainId == "Renesas_CCRL"):
                toolId = "com.renesas.cdt.managedbuild.renesas.ccrl.base.linker"
                optionId = "com.renesas.cdt.managedbuild.renesas.ccrl.linker.option.inputFile"
            elif (toolchainId == "Renesas_RXC"):
                toolId = "com.renesas.cdt.managedbuild.renesas.ccrx.base.linker"
                optionId = "com.renesas.cdt.managedbuild.renesas.ccrx.linker.option.inputFile"
            else:
                raise Exception("Invalid toolchainId")
            return e2studio.get_build_config_option(project.project_name, build_config_name, toolId, optionId)
        else:
            raise Exception("Invalid project name")

    @LibraryFile.setter
    def LibraryFile(self, value):
        if (project.project_name != ""):
            if (isinstance(value, list)):
                build_config_name = e2studio.get_active_build_config_name(project.project_name)
                toolchainId = e2studio.get_toolchain_id(project.project_name, build_config_name)
                if (toolchainId == "Renesas_CCRL"):
                    toolId = "com.renesas.cdt.managedbuild.renesas.ccrl.base.linker"
                    optionId = "com.renesas.cdt.managedbuild.renesas.ccrl.linker.option.inputFile"
                elif (toolchainId == "Renesas_RXC"):
                    toolId = "com.renesas.cdt.managedbuild.renesas.ccrx.base.linker"
                    optionId = "com.renesas.cdt.managedbuild.renesas.ccrx.linker.option.inputFile"
                else:
                    raise Exception("Invalid toolchainId")
                e2studio.set_build_config_option(project.project_name, build_config_name, toolId, optionId, value)
            else:
                raise Exception("Value must be list")
        else:
            raise Exception("Invalid project name")

    @property
    def AdditionalOptions(self):
        if (project.project_name != ""):
            build_config_name = e2studio.get_active_build_config_name(project.project_name)
            toolchainId = e2studio.get_toolchain_id(project.project_name, build_config_name)
            if (toolchainId == "Renesas_CCRL"):
                toolId = "com.renesas.cdt.managedbuild.renesas.ccrl.base.linker"
                optionId = "com.renesas.cdt.managedbuild.renesas.ccrl.linker.option.userAfter"
            elif (toolchainId == "Renesas_RXC"):
                toolId = "com.renesas.cdt.managedbuild.renesas.ccrx.base.linker"
                optionId = "com.renesas.cdt.managedbuild.renesas.ccrx.linker.option.userAfter"
            else:
                raise Exception("Invalid toolchainId")
            value = e2studio.get_build_config_option(project.project_name, build_config_name, toolId, optionId)
            return " ".join(value)
        else:
            raise Exception("Invalid project name")

    @AdditionalOptions.setter
    def AdditionalOptions(self, value):
        if (project.project_name != ""):
            if (Util.is_str_instance(value)):
                build_config_name = e2studio.get_active_build_config_name(project.project_name)
                toolchainId = e2studio.get_toolchain_id(project.project_name, build_config_name)
                if (toolchainId == "Renesas_CCRL"):
                    toolId = "com.renesas.cdt.managedbuild.renesas.ccrl.base.linker"
                    optionId = "com.renesas.cdt.managedbuild.renesas.ccrl.linker.option.userAfter"
                elif (toolchainId == "Renesas_RXC"):
                    toolId = "com.renesas.cdt.managedbuild.renesas.ccrx.base.linker"
                    optionId = "com.renesas.cdt.managedbuild.renesas.ccrx.linker.option.userAfter"
                else:
                    raise Exception("Invalid toolchainId")
                e2studio.set_build_config_option(project.project_name, build_config_name, toolId, optionId, str(value))
            else:
                raise Exception("Value must be string")
        else:
            raise Exception("Invalid project name")
