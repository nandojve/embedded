ASF_THIRDPARTY_DIR						= $(ASF_ROOT_DIR)/thirdparty

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

ifneq (, $(filter $(USE_CMSIS),1))
	include $(ASF_THIRDPARTY_DIR)/CMSIS/makefile.mk
endif
ifneq (, $(filter $(USE_FATFS),1))
ASF_THIRDPARTY_FATFS_DIR				= $(ASF_THIRDPARTY_DIR)/fatfs

	include $(ASF_THIRDPARTY_FATFS_DIR)/makefile.mk
endif
ifneq (, $(filter $(USE_FREERTOS),1))
ASF_THIRDPARTY_FREERTOS_DIR				= $(ASF_THIRDPARTY_DIR)/freertos

	include $(ASF_THIRDPARTY_FREERTOS_DIR)/makefile.mk
endif
ifneq (, $(filter $(USE_LWIP),1))
ASF_THIRDPARTY_LWIP_DIR					= $(ASF_THIRDPARTY_DIR)/lwip

	include $(ASF_THIRDPARTY_LWIP_DIR)/makefile.mk
endif
ifneq (, $(filter $(USE_QTOUCH),1))
ASF_THIRDPARTY_QTOUCH_DIR				= $(ASF_THIRDPARTY_DIR)/qtouch

	include $(ASF_THIRDPARTY_QTOUCH_DIR)/makefile.mk
endif
ifneq (, $(filter $(USE_WIRELESS),1))
ifneq ($(strip $(USE_WIRELESS_OLD)),1)
ASF_THIRDPARTY_WIRELESS_DIR				= $(ASF_THIRDPARTY_DIR)/wireless

	include $(ASF_THIRDPARTY_WIRELESS_DIR)/makefile.mk
endif
endif