import e2studio

class BuildCompile(object):

    @property
    def Macro(self):
        if (project.project_name != ""):
            build_config_name = e2studio.get_active_build_config_name(project.project_name)
            try:
               toolchainId = e2studio.get_toolchain_id(project.project_name, build_config_name)
            except:
               toolchainId = ""
            if (toolchainId == "Renesas_CCRL"):
                toolId = "com.renesas.cdt.managedbuild.renesas.ccrl.base.compiler"
                optionId = "com.renesas.cdt.managedbuild.renesas.ccrl.compiler.option.D"
            elif (toolchainId == "Renesas_RXC"):
                toolId = "com.renesas.cdt.managedbuild.renesas.ccrx.base.compiler"
                optionId = "com.renesas.cdt.managedbuild.renesas.ccrx.compiler.option.define"
            else:
                toolId = e2studio.get_tool_option_id(project.project_name, build_config_name, ".c.compiler")
                if (toolId == "ilg.gnuarmeclipse.managedbuild.cross.tool.c.compiler"):
                   optionId = "ilg.gnuarmeclipse.managedbuild.cross.option.c.compiler.defs"
                else:
                   raise Exception("Invalid tool option")
            return e2studio.get_build_config_option(project.project_name, build_config_name, toolId, optionId)
        else:
            raise Exception("Invalid project name")

    @Macro.setter
    def Macro(self, value):
        if (project.project_name != ""):
            if (isinstance(value, list)):
                build_config_name = e2studio.get_active_build_config_name(project.project_name)
                try:
                   toolchainId = e2studio.get_toolchain_id(project.project_name, build_config_name)
                except:
                   toolchainId = ""
                if (toolchainId == "Renesas_CCRL"):
                    toolId = "com.renesas.cdt.managedbuild.renesas.ccrl.base.compiler"
                    optionId = "com.renesas.cdt.managedbuild.renesas.ccrl.compiler.option.D"
                elif (toolchainId == "Renesas_RXC"):
                    toolId = "com.renesas.cdt.managedbuild.renesas.ccrx.base.compiler"
                    optionId = "com.renesas.cdt.managedbuild.renesas.ccrx.compiler.option.define"
                else:
                    toolId = e2studio.get_tool_option_id(project.project_name, build_config_name, ".c.compiler")
                    if (toolId == "ilg.gnuarmeclipse.managedbuild.cross.tool.c.compiler"):
                       optionId = "ilg.gnuarmeclipse.managedbuild.cross.option.c.compiler.defs"
                    else:
                       raise Exception("Invalid tool option")
                e2studio.set_build_config_option(project.project_name, build_config_name, toolId, optionId, value)
            else:
                raise Exception("Value must be list")
        else:
            raise Exception("Invalid project name")

    @property
    def IncludePath(self):
        if (project.project_name != ""):
            build_config_name = e2studio.get_active_build_config_name(project.project_name)
            try:
               toolchainId = e2studio.get_toolchain_id(project.project_name, build_config_name)
            except:
               toolchainId = ""
            if (toolchainId == "Renesas_CCRL"):
                toolId = "com.renesas.cdt.managedbuild.renesas.ccrl.base.compiler"
                optionId = "com.renesas.cdt.managedbuild.renesas.ccrl.compiler.option.I"
            elif (toolchainId == "Renesas_RXC"):
                toolId = "com.renesas.cdt.managedbuild.renesas.ccrx.base.compiler"
                optionId = "com.renesas.cdt.managedbuild.renesas.ccrx.compiler.option.include"
            else:
                toolId = e2studio.get_tool_option_id(project.project_name, build_config_name, ".c.compiler")
                if (toolId == "ilg.gnuarmeclipse.managedbuild.cross.tool.c.compiler"):
                   optionId = "ilg.gnuarmeclipse.managedbuild.cross.option.c.compiler.include.paths"
                else:
                   raise Exception("Invalid tool option")
            return e2studio.get_build_config_option(project.project_name, build_config_name, toolId, optionId)
        else:
            raise Exception("Invalid project name")

    @IncludePath.setter
    def IncludePath(self, value):
        if (project.project_name != ""):
            if (isinstance(value, list)):
                build_config_name = e2studio.get_active_build_config_name(project.project_name)
                try:
                   toolchainId = e2studio.get_toolchain_id(project.project_name, build_config_name)
                except:
                   toolchainId = ""
                if (toolchainId == "Renesas_CCRL"):
                    toolId = "com.renesas.cdt.managedbuild.renesas.ccrl.base.compiler"
                    optionId = "com.renesas.cdt.managedbuild.renesas.ccrl.compiler.option.I"
                elif (toolchainId == "Renesas_RXC"):
                    toolId = "com.renesas.cdt.managedbuild.renesas.ccrx.base.compiler"
                    optionId = "com.renesas.cdt.managedbuild.renesas.ccrx.compiler.option.include"
                else:
                    toolId = e2studio.get_tool_option_id(project.project_name, build_config_name, ".c.compiler")
                    if (toolId == "ilg.gnuarmeclipse.managedbuild.cross.tool.c.compiler"):
                       optionId = "ilg.gnuarmeclipse.managedbuild.cross.option.c.compiler.include.paths"
                    else:
                       raise Exception("Invalid tool option")
                e2studio.set_build_config_option(project.project_name, build_config_name, toolId, optionId, value)
            else:
                raise Exception("Value must be list")
        else:
            raise Exception("Invalid project name")

