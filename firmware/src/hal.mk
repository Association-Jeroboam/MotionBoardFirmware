COMMONCPPSRC += $(COMMONDIR)/HAL/Motor.cpp

COMMONINC += $(COMMONDIR)/HAL
COMMONINC += $(CHIBIOS)/os/various/cpp_wrappers

include $(COMMONDIR)/HAL/ports/ports.mk
