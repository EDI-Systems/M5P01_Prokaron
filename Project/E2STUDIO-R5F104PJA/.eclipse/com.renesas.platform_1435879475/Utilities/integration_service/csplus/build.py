import e2studio
from csplus.build_compile import BuildCompile
from csplus.build_link import BuildLink

class Build(object):
    Compile = BuildCompile()
    Link = BuildLink()

    def All(self, rebuild=False, waitBuild=True):
        try:
            build_config_name = e2studio.get_active_build_config_name(project.project_name)
            e2studio.build_project(project.project_name, build_config_name, rebuild, waitBuild)
            return True
        except:
            return False

    def File(self, file_name, rebuild=False, wait_build=True):
        try:
           e2studio.build_file(project.project_name, file_name, rebuild, wait_build)
           return True
        except:
           return False

    def ChangeBuildMode(self, config_name):
        if project.project_name != "":
            if config_name != "":
                try:
                    e2studio.set_active_build_config(project.project_name, config_name)
                    return True
                except:
                    try:
                        base_config_name = e2studio.get_active_build_config_name(project.project_name)
                        e2studio.create_new_build_config(project.project_name, base_config_name, config_name)
                        e2studio.set_active_build_config(project.project_name, config_name)
                        return True
                    except:
                        return False
            else:
                return False
        else:
            raise Exception("Invalid project name")
