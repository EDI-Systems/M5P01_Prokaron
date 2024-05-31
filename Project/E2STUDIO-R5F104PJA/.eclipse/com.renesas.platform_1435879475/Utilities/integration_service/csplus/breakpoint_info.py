class BreakpointInfo(object):

    def __init__(self):
        self.__number = 0
        self.__name = None
        self.__enable = True
        self.__break_type = BreakType.Hardware
        self.__address_1 = None
        self.__address_2 = None
        self.__address_3 = None
        self.__address_4 = None

    @property
    def Number(self):
        return self.__number

    @Number.setter
    def Number(self, number):
        self.__number = number

    @property
    def Name(self):
        return self.__name

    @Name.setter
    def Name(self, name):
        self.__name = name

    @property
    def Enable(self):
        return self.__enable

    @Enable.setter
    def Enable(self, enable):
        self.__enable = enable

    @property
    def BreakType(self):
        return self.__break_type

    @BreakType.setter
    def BreakType(self, break_type):
        self.__break_type = break_type

    @property
    def Address1(self):
        return self.__address_1

    @Address1.setter
    def Address1(self, address):
        self.__address_1 = address

    @property
    def Address2(self):
        return self.__address_2

    @Address2.setter
    def Address2(self, address):
        self.__address_2 = address

    @property
    def Address3(self):
        return self.__address_3

    @Address3.setter
    def Address3(self, address):
        self.__address_3 = address

    @property
    def Address4(self):
        return self.__address_4

    @Address4.setter
    def Address4(self, address):
        self.__address_4 = address
