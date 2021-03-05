CXX_SOURCES = $(wildcard src/*.cpp) $(wildcard ../common/*.cpp)
CFLAGS = -std=c++11
INCLUDE = -I"../common" -I"./src"

### Do not modify: this includes Webots global Makefile.include
null :=
space := $(null) $(null)
WEBOTS_HOME_PATH=$(subst $(space),\ ,$(strip $(subst \,/,$(WEBOTS_HOME))))
include $(WEBOTS_HOME_PATH)/resources/Makefile.include
