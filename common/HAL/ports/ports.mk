ifeq ($(PLATFORM),STM32G4)
  include $(COMMONDIR)/HAL/ports/STM32G4/platform.mk
else ifeq ($(PLATFORM),STM32F303)
	include $(COMMONDIR)/HAL/ports/STM32F303/platform.mk
else ifeq ($(PLATFORM),Simulator)
  include $(COMMONDIR)/HAL/ports/Simulator/platform.mk
else
  $(error Invalid platform: "$(PLATFORM)")
endif
