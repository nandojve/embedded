ASF_PLATFORM_DIR					= $(ASF_ROOT_DIR)/$(PLATFORM)

CFLAGS += -DBOARD=USER_BOARD
CFLAGS += -DVENDOR_BOARDTYPES=1
CFLAGS += -DBOARD_TYPE=$(BOARD_TYPE)
CFLAGS += -DPLATFORM=$(PLATFORM)
CFLAGS += -Wpointer-arith
CFLAGS += -fdata-sections

ifneq (, $(filter $(ARMTYPE),SAM4E))
CFLAGS += -mfloat-abi=softfp -mfpu=fpv4-sp-d16
CPPFLAGS += -mfloat-abi=softfp -mfpu=fpv4-sp-d16
ASFLAGS += -mfloat-abi=softfp -mfpu=fpv4-sp-d16
endif

ifneq (, $(filter $(PLATFORM),xmega))
CFLAGS += -DIOPORT_XMEGA_COMPAT
endif

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
EXTRAINCDIRS +=																	\
	$(ASF_ROOT_DIR)

include $(ASF_PLATFORM_DIR)/drivers/makefile.mk									\
		$(ASF_ROOT_DIR)/common/makefile.mk

ifneq (, $(filter $(PLATFORM),xmega sam uc3))
	include $(ASF_PLATFORM_DIR)/services/makefile.mk
endif

ifneq (, $(filter $(PLATFORM),sam))
	include $(ASF_PLATFORM_DIR)/components/makefile.mk
endif

include $(ASF_PLATFORM_DIR)/utils/makefile.mk
