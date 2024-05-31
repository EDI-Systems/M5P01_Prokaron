import e2studio
from csplus.utility import Util

class ProjectFile(object):

    def Exists(self, fileName):
        try:
            return e2studio.is_file_exist(project.project_name, fileName)
        except:
            return False

    def Add(self, fileName, category=''):
        new_folder_path = ''
        if (category == ''):
            new_folder_path = 'src'
            try:
                e2studio.create_virtual_folder(project.project_name, new_folder_path)
                e2studio.include_source_folder(project.project_name, new_folder_path)
            except:
                pass

            return self.__handle_file_name(new_folder_path, fileName)
        else:
            # category has one value. e.g. 'src', 'virtual_folder', etc
            if Util.is_str_instance(category):
                new_folder_path = str(category)
                try:
                    e2studio.create_virtual_folder(project.project_name, new_folder_path)
                    e2studio.include_source_folder(project.project_name, new_folder_path)
                except:
                    pass

                return self.__handle_file_name(new_folder_path, fileName)
            # category is a list. e.g. ['src', 'virtual_folder']
            # new_folder_path will be: 'src/virtual_folder'
            elif isinstance(category, list):
                i = 0
                for folder in category:
                    if i == 0:
                        new_folder_path = folder
                    else:
                        new_folder_path = new_folder_path + '/' + folder
                    try:
                        e2studio.create_virtual_folder(project.project_name, new_folder_path)
                        e2studio.include_source_folder(project.project_name, new_folder_path)
                    except:
                        pass

                    i = i + 1

                return self.__handle_file_name(new_folder_path, fileName)
            else:
                return False

    def __handle_file_name(self, folder_path, file_name):
        if isinstance(file_name, str):
            return self.__add(folder_path, file_name)
        elif isinstance(file_name, list):
            for file in file_name:
                # if a file cannot add, will stop processing
                if (self.__add(folder_path, file) == False):
                    return False
            return True
        else:
            return False

    def __add(self, folder_path, file_name):
        try:
            # if cannot find file_name to include, it means file_name does not exist in the project
            # or there are some errors when include file_name, so will try to linked file_name in this case.
            e2studio.include_file(project.project_name, file_name)
            return True
        except:
            try:
                e2studio.create_linked_source_file(project.project_name, folder_path, file_name)
                return True
            except:
                return False

    def Remove(self, fileName):
        try:
            if (isinstance(fileName, str)):
                if (e2studio.is_file_exist(project.project_name, fileName)):
                    if (e2studio.is_linked_file(project.project_name, fileName)):
                        e2studio.remove_linked_file(project.project_name, fileName)
                        return True
                    else:
                        e2studio.exclude_from_build(project.project_name, fileName)
                        return True
                else:
                    return False
            elif (isinstance(fileName, list)):
                for file in fileName:
                    if (e2studio.is_file_exist(project.project_name, file)):
                        if (e2studio.is_linked_file(project.project_name, file)):
                            e2studio.remove_linked_file(project.project_name, file)
                        else:
                            e2studio.exclude_from_build(project.project_name, file)
                    else:
                        return False
                return True
            else:
                return False
        except:
            return False

    def Information(self):
        return e2studio.get_all_files(project.project_name)

