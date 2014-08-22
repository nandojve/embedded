# Full TAL makefile
###############################################################################
#list C source files
CSRC +=																			\
	$(PATH_TAL)/$(TAL_TYPE)/Src/tal.c											\
	$(PATH_TAL)/$(TAL_TYPE)/Src/tal_rx.c										\
	$(PATH_TAL)/$(TAL_TYPE)/Src/tal_tx.c										\
	$(PATH_TAL)/$(TAL_TYPE)/Src/tal_ed.c										\
	$(PATH_TAL)/$(TAL_TYPE)/Src/tal_slotted_csma.c								\
	$(PATH_TAL)/$(TAL_TYPE)/Src/tal_pib.c										\
	$(PATH_TAL)/$(TAL_TYPE)/Src/tal_init.c										\
	$(PATH_TAL)/$(TAL_TYPE)/Src/tal_irq_handler.c								\
	$(PATH_TAL)/$(TAL_TYPE)/Src/tal_pwr_mgmt.c									\
	$(PATH_TAL)/$(TAL_TYPE)/Src/tal_rx_enable.c

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
## Include directories for TAL
EXTRAINCDIRS +=																	\
	$(PATH_TAL)/Inc/															\
	$(PATH_TAL)/$(TAL_TYPE)/Inc/
