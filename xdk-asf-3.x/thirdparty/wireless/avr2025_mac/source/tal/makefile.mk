ASF_THIRDPARTY_WIRELESS_STACK_TAL_DIR	= $(ASF_THIRDPARTY_WIRELESS_STACK_DIR)/source/tal

#list C source files
CSRC	+=																		\
	$(ASF_THIRDPARTY_WIRELESS_STACK_TAL_DIR)/$(TAL_TYPE)/src/tal.c				\
	$(ASF_THIRDPARTY_WIRELESS_STACK_TAL_DIR)/$(TAL_TYPE)/src/tal_ed.c			\
	$(ASF_THIRDPARTY_WIRELESS_STACK_TAL_DIR)/$(TAL_TYPE)/src/tal_init.c			\
	$(ASF_THIRDPARTY_WIRELESS_STACK_TAL_DIR)/$(TAL_TYPE)/src/tal_irq_handler.c	\
	$(ASF_THIRDPARTY_WIRELESS_STACK_TAL_DIR)/$(TAL_TYPE)/src/tal_pib.c			\
	$(ASF_THIRDPARTY_WIRELESS_STACK_TAL_DIR)/$(TAL_TYPE)/src/tal_pwr_mgmt.c		\
	$(ASF_THIRDPARTY_WIRELESS_STACK_TAL_DIR)/$(TAL_TYPE)/src/tal_rx.c			\
	$(ASF_THIRDPARTY_WIRELESS_STACK_TAL_DIR)/$(TAL_TYPE)/src/tal_rx_enable.c	\
	$(ASF_THIRDPARTY_WIRELESS_STACK_TAL_DIR)/$(TAL_TYPE)/src/tal_slotted_csma.c	\
	$(ASF_THIRDPARTY_WIRELESS_STACK_TAL_DIR)/$(TAL_TYPE)/src/tal_tx.c

CSRC	+=																		\
	$(ASF_THIRDPARTY_WIRELESS_STACK_TAL_DIR)/src/tal_ed_end_cb.c				\
	$(ASF_THIRDPARTY_WIRELESS_STACK_TAL_DIR)/src/tal_helper.c					\
	$(ASF_THIRDPARTY_WIRELESS_STACK_TAL_DIR)/src/tal_rx_frame_cb.c				\
	$(ASF_THIRDPARTY_WIRELESS_STACK_TAL_DIR)/src/tal_tx_frame_done_cb.c

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
	$(ASF_THIRDPARTY_WIRELESS_STACK_TAL_DIR)/inc								\
	$(ASF_THIRDPARTY_WIRELESS_STACK_TAL_DIR)/$(TAL_TYPE)/inc