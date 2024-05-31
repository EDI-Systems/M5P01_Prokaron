class ProjectKind(object):

    @property
    def Auto(self):
        return "auto"

    @property
    def Application(self):
        return "executable"

    @property
    def Library(self):
        return "library"
