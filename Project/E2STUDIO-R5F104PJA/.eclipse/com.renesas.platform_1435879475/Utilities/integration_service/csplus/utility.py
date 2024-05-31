class Util(object):

    @staticmethod
    def is_str_instance(val):
        try:
            return isinstance(val, str) or isinstance(val, unicode)
        except:
            return False