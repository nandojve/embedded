ASF_THIRDPARTY_QTOUCH_DEVSPECIFIC_DIR	= $(ASF_THIRDPARTY_QTOUCH_DIR)/devspecific
ASF_THIRDPARTY_QTOUCH_GENERIC_DIR		= $(ASF_THIRDPARTY_QTOUCH_DIR)/generic
ASF_THIRDPARTY_QTOUCH_QDEBUG_DIR		= $(ASF_THIRDPARTY_QTOUCH_DIR)/qdebug

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

ifneq (, $(filter $(PLATFORM),sam uc3))
	include $(ASF_THIRDPARTY_QTOUCH_DEVSPECIFIC_DIR)/makefile.mk
endif

include $(ASF_THIRDPARTY_QTOUCH_GENERIC_DIR)/makefile.mk
include $(ASF_THIRDPARTY_QTOUCH_QDEBUG_DIR)/makefile.mk
