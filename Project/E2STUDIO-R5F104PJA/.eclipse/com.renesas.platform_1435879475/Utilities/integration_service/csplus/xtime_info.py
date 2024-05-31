class XTimeInfo(object):

    def __init__(self):
        self.__value = 0
        self.__is_cpu_clock = False
        self.__is_over_flow = False

    @property
    def Value(self):
        return self.__value

    @Value.setter
    def Value(self, value):
        self.__value = value

    @property
    def IsCpuClock(self):
        return self.__is_cpu_clock

    @IsCpuClock.setter
    def IsCpuClock(self, value):
        self.__is_cpu_clock = value

    @property
    def IsOverFlow(self):
        return self.__is_over_flow

    @IsOverFlow.setter
    def IsOverFlow(self, value):
        self.__is_over_flow = value
