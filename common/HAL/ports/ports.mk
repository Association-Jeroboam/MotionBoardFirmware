ifeq ($(PLATFORM),STM32G4)
  include $(COMMONDIR)/HAL/ports/STM32G4/port.mk
else ifeq ($(PLATFORM),Simulator)
  include $(COMMONDIR)/HAL/ports/Simulator/port.mk
  else
  $(error Invalid platform: "$(PLATFORM)")
endif
