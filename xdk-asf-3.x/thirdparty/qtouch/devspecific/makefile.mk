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

ifneq (, $(filter $(ARMTYPE),SAM4L))
ASF_THIRDPARTY_QTOUCH_DEVSPECIFIC_SELECTED_BASE_DIR = $(ASF_THIRDPARTY_QTOUCH_DEVSPECIFIC_DIR)/sam4
	include $(ASF_THIRDPARTY_QTOUCH_DEVSPECIFIC_SELECTED_BASE_DIR)/makefile.mk
endif