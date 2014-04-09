ASF_THIRDPARTY_WIRELESS_STACK_PAL_DIR	= $(ASF_THIRDPARTY_WIRELESS_STACK_DIR)/source/pal

LDFLAGS += -Wl,--start-group -L$(ASF_THIRDPARTY_WIRELESS_STACK_PAL_DIR)/common_hw_timer/$(PLATFORM)/lib

ifneq (, $(filter $(PLATFORM),mega))
ifneq (, $(filter $(TAL_TYPE),atmegarfr2 ATMEGARFR2))
LDFLAGS += -lmegarfr2_lib_hw_timer
else
LDFLAGS += -lmegarfa1_lib_hw_timer
endif
endif
ifneq (, $(filter $(PLATFORM),xmega))
LDFLAGS += -lxmegaa3_hw_timer_lib
endif
ifneq (, $(filter $(PLATFORM),sam))
LDFLAGS += -lsam4l_lib_hw_timer
endif
ifneq (, $(filter $(PLATFORM),uc3))
LDFLAGS += -luc3_lib_hw_timer
endif

LDFLAGS += -Wl,--end-group

#list C source files
CSRC	+=																		\
	$(ASF_THIRDPARTY_WIRELESS_STACK_PAL_DIR)/pal.c								\
	$(ASF_THIRDPARTY_WIRELESS_STACK_PAL_DIR)/common_sw_timer/common_sw_timer.c	\
	$(ASF_THIRDPARTY_WIRELESS_STACK_PAL_DIR)/common_hw_timer/$(PLATFORM)/hw_timer.c

ifneq (, $(filter $(PLATFORM),xmega sam uc3))
CSRC	+=																		\
	$(ASF_THIRDPARTY_WIRELESS_STACK_PAL_DIR)/pal_ext_trx.c
endif

ifneq (, $(filter $(PLATFORM),mega sam))
CSRC	+=																		\
	$(ASF_THIRDPARTY_WIRELESS_STACK_PAL_DIR)/common/drivers/nvm/$(PLATFORM)/$(PLATFORM)_nvm.c
endif

ifneq (, $(filter $(PLATFORM),mega))
CSRC	+=																		\
	$(ASF_THIRDPARTY_WIRELESS_STACK_PAL_DIR)/mega/drivers/flash/flash.c			\
	$(ASF_THIRDPARTY_WIRELESS_STACK_PAL_DIR)/mega/drivers/tc/tc_megarf.c
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
EXTRAINCDIRS +=																	\
	$(ASF_THIRDPARTY_WIRELESS_STACK_PAL_DIR)									\
	$(ASF_THIRDPARTY_WIRELESS_STACK_PAL_DIR)/common_sw_timer					\
	$(ASF_THIRDPARTY_WIRELESS_STACK_PAL_DIR)/common_hw_timer					\
	$(ASF_THIRDPARTY_WIRELESS_STACK_PAL_DIR)/common_hw_timer/$(PLATFORM)		\
	$(ASF_THIRDPARTY_WIRELESS_STACK_PAL_DIR)/common/drivers/nvm

ifneq (, $(filter $(PLATFORM),mega sam))
EXTRAINCDIRS +=																	\
	$(ASF_THIRDPARTY_WIRELESS_STACK_PAL_DIR)/common/drivers/nvm/$(PLATFORM)
endif

ifneq (, $(filter $(PLATFORM),mega))
EXTRAINCDIRS +=																	\
	$(ASF_THIRDPARTY_WIRELESS_STACK_PAL_DIR)/mega/drivers/flash					\
	$(ASF_THIRDPARTY_WIRELESS_STACK_PAL_DIR)/mega/drivers/tc
endif