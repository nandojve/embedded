#CFLAGS += -DDISABLE_TSTAMP_IRQ=1
#CFLAGS += -DENABLE_TSTAMP
#CFLAGS += -DFFD
#CFLAGS += -DENABLE_TFA
#CFLAGS += -DHIGH_DATA_RATE_SUPPORT
#CFLAGS += -DHIGHEST_STACK_LAYER=$(HIGHEST_STACK_LAYER)
#CFLAGS += -DPAL_GENERIC_TYPE=$(strip $(PLATFORM))
#CFLAGS += -DPAL_TYPE=$(shell echo $(MCU) | tr [:upper:] [:lower:])
#CFLAGS += -DTAL_TYPE=$(TAL_TYPE)
#CFLAGS += -DSAL_TYPE=$(SAL_TYPE)
CFLAGS += -DPHY_$(TAL_TYPE)

ifneq (, $(filter $(PLATFORM),xmega sam uc3))
CFLAGS += -DPAL_USE_SPI_TRX=1
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
	$(ASF_THIRDPARTY_WIRELESS_STACK_DIR)/include

include																			\
		$(ASF_THIRDPARTY_WIRELESS_STACK_DIR)/source/nwk/makefile.mk				\
		$(ASF_THIRDPARTY_WIRELESS_STACK_DIR)/source/phy/makefile.mk				\
		$(ASF_THIRDPARTY_WIRELESS_STACK_DIR)/source/sys/makefile.mk