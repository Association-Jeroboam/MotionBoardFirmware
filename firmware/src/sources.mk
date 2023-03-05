COMMONCPPSRC += $(SOURCEDIR)/main.cpp
COMMONCPPSRC += $(SOURCEDIR)/Motor.cpp
COMMONCPPSRC += $(SOURCEDIR)/MotorControl.cpp
COMMONCPPSRC += $(SOURCEDIR)/MotionBoardShell.cpp
COMMONCPPSRC += $(SOURCEDIR)/ControlThread.cpp
COMMONCPPSRC += $(SOURCEDIR)/Memory.cpp
COMMONCPPSRC += $(SOURCEDIR)/EmergencyWatcher.cpp

include $(SOURCEDIR)/$(TARGET_TYPE)/target.mk
