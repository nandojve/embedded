#list C source files
CSRC	+=

# List C++ source files here. (C dependencies are automatically generated.)
CPPSRC += 

# List Assembler source files here.
# Make them always end in a capital .S.  Files ending in a lowercase .s
# will not be considered source files but generated files (assembler
# output from the compiler), and will be deleted upon "make clean"!
# Even though the DOS/Win* filesystem matches both .s and .S the same,
# it will preserve the spelling of the filenames, and gcc itself does
# care about how the name is spelled on its command-line.
ASRC +=

# List any extra directories to look for include files here.
#     Each directory must be seperated by a space.
EXTRAINCDIRS +=

ifneq (, $(filter $(STACK_TYPE),ATMAC-802.15.4))
ASF_THIRDPARTY_WIRELESS_STACK_DIR		= $(ASF_THIRDPARTY_WIRELESS_DIR)/avr2025_mac
else
	ifneq (, $(filter $(STACK_TYPE),LwMesh))
		ASF_THIRDPARTY_WIRELESS_STACK_DIR = $(ASF_THIRDPARTY_WIRELESS_DIR)/avr2130_lwmesh
	else
		ifneq (, $(filter $(STACK_TYPE),RF4CE))
			ASF_THIRDPARTY_WIRELESS_STACK_DIR = $(ASF_THIRDPARTY_WIRELESS_DIR)/avr2102_rf4control
		endif
	endif
endif

ifdef ASF_THIRDPARTY_WIRELESS_STACK_DIR
	include $(ASF_THIRDPARTY_WIRELESS_STACK_DIR)/makefile.mk
endif

ASF_THIRDPARTY_WIRELESS_SERVICES_DIR	= $(ASF_THIRDPARTY_WIRELESS_DIR)/services
ASF_THIRDPARTY_WIRELESS_ADDONS_DIR		= $(ASF_THIRDPARTY_WIRELESS_DIR)/addons

include $(ASF_THIRDPARTY_WIRELESS_SERVICES_DIR)/makefile.mk
include $(ASF_THIRDPARTY_WIRELESS_ADDONS_DIR)/makefile.mk