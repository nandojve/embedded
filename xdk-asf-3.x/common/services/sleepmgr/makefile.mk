ASF_COMMON_SERVICES_SLEEPMGR_DIR	= $(ASF_COMMON_SERVICES_DIR)/sleepmgr

#list C source files
ifneq (, $(filter $(ARMTYPE),SAM4L))
CSRC	+=																		\
	$(ASF_COMMON_SERVICES_SLEEPMGR_DIR)/sam4l/sleepmgr.c
endif
ifneq (, $(filter $(ARMTYPE),SAMD11 SAMD20 SAMD21 SAMR21))
CSRC	+=																		\
	$(ASF_COMMON_SERVICES_SLEEPMGR_DIR)/samd/sleepmgr.c
else
CSRC	+=																		\
	$(ASF_COMMON_SERVICES_SLEEPMGR_DIR)/$(PLATFORM)/sleepmgr.c
endif

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
EXTRAINCDIRS += 																\
	$(ASF_COMMON_SERVICES_SLEEPMGR_DIR)

ifneq (, $(filter $(ARMTYPE),SAM4L))
EXTRAINCDIRS += 																\
	$(ASF_COMMON_SERVICES_SLEEPMGR_DIR)/sam4l
endif
ifneq (, $(filter $(ARMTYPE),SAMD11 SAMD20 SAMD21 SAMR21))
EXTRAINCDIRS += 																\
	$(ASF_COMMON_SERVICES_SLEEPMGR_DIR)/samd
else
EXTRAINCDIRS += 																\
	$(ASF_COMMON_SERVICES_SLEEPMGR_DIR)/$(PLATFORM)
endif