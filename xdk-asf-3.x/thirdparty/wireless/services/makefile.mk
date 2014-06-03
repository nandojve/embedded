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

ASF_THIRDPARTY_WIRELESS_SERVICES_COMMON_HW_TIMMER_DIR = $(ASF_THIRDPARTY_WIRELESS_SERVICES_DIR)/common_hw_timer
include $(ASF_THIRDPARTY_WIRELESS_SERVICES_COMMON_HW_TIMMER_DIR)/makefile.mk

ifneq (, $(filter $(PLATFORM),mega))
ASF_THIRDPARTY_WIRELESS_SERVICES_MEGA_DIR = $(ASF_THIRDPARTY_WIRELESS_SERVICES_DIR)/mega
	include $(ASF_THIRDPARTY_WIRELESS_SERVICES_MEGA_DIR)/makefile.mk
endif

ASF_THIRDPARTY_WIRELESS_SERVICES_SAL_DIR = $(ASF_THIRDPARTY_WIRELESS_SERVICES_DIR)/sal
include $(ASF_THIRDPARTY_WIRELESS_SERVICES_SAL_DIR)/makefile.mk

ASF_THIRDPARTY_WIRELESS_SERVICES_SLEEP_DIR = $(ASF_THIRDPARTY_WIRELESS_SERVICES_DIR)/sleep_mgr
include $(ASF_THIRDPARTY_WIRELESS_SERVICES_SLEEP_DIR)/makefile.mk

ifneq (, $(filter $(TAL_TYPE),AT86RF212 AT86RF212B AT86RF231 AT86RF232 AT86RF233))
ASF_THIRDPARTY_WIRELESS_SERVICES_TRX_ACCESS_DIR = $(ASF_THIRDPARTY_WIRELESS_SERVICES_DIR)/trx_access
	include $(ASF_THIRDPARTY_WIRELESS_SERVICES_TRX_ACCESS_DIR)/makefile.mk
endif