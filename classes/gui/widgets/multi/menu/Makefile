########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../../../../..

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-options

# Enter object files here:
OBJECTS	  = menu.o menubar.o menubarentry.o menuentry.o micromenu.o popupmenu.o popupmenuentry.o popupmenuentrycheck.o popupmenuentryoption.o

ifeq ($(BUILD_OSX),True)
	OBJECTS += menubar_cocoa.o
endif

## Do not change anything below this line. ##

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-commands
