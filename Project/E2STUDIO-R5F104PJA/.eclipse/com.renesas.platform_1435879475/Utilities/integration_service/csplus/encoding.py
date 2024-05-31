import locale

class Encoding(object):

    @property
    def ASCII(self):
        return "US-ASCII"

    @property
    def BigEndianUnicode(self):
        return "UTF-16BE"

    @property
    def Unicode(self):
        return "UTF-16"

    @property
    def UTF32(self):
        return "UTF-32"

    @property
    def UTF7(self):
        return "UTF-7"

    @property
    def UTF8(self):
        return "UTF-8"

    @property
    def Default(self):
        return locale.getpreferredencoding()
