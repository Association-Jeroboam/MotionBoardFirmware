COMMONCPPSRC += $(SOURCEDIR)/main.cpp
COMMONCPPSRC += $(SOURCEDIR)/Motor.cpp
COMMONCPPSRC += $(SOURCEDIR)/MotorControl.cpp
COMMONCPPSRC += $(SOURCEDIR)/Shell.cpp
COMMONCPPSRC += $(SOURCEDIR)/ControlThread.cpp
COMMONCPPSRC += $(SOURCEDIR)/StrategyThread.cpp
COMMONCPPSRC += $(SOURCEDIR)/Memory.cpp
COMMONCPPSRC += $(SOURCEDIR)/LidarThread.cpp
COMMONCPPSRC += $(SOURCEDIR)/Point.cpp

include $(SOURCEDIR)/$(TARGET_TYPE)/target.mk
