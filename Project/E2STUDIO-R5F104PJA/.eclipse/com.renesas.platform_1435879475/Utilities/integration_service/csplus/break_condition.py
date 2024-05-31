class BreakCondition(object):

    def __init__(self):
        self.__address = ""
        self.__data = None
        self.__access_size = None
        self.__break_type = BreakType.Hardware

    @property
    def Address(self):
        return self.__address

    @Address.setter
    def Address(self, address):
        self.__address = address

    @property
    def Data(self):
        return self.__data

    @Data.setter
    def Data(self, data):
        self.__data = data

    @property
    def AccessSize(self):
        return self.__access_size

    @AccessSize.setter
    def AccessSize(self, access_size):
        self.__access_size = access_size

    @property
    def BreakType(self):
        return self.__break_type

    @BreakType.setter
    def BreakType(self, break_type):
        self.__break_type = break_type
