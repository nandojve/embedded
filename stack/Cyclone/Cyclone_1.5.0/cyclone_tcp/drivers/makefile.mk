# Cyclone TCP Drivers Directory Makefile
###############################################################################

# Path variables
## Path to main project directory

#list C source files
ifneq (, $(filter $(CYCLONE_TCP_MAC),sam4e_eth))
	CSRC +=																		\
		$(CYCLONE_STACK_TCP_DRIVERS_DIR)/$(CYCLONE_TCP_MAC).c
else
	ifneq (, $(filter $(CYCLONE_TCP_MAC),none))
	else
		$(error The Cyclone TCP/IP Stack not support MAC $(CYCLONE_TCP_MAC))
	endif
endif
ifneq (, $(filter $(CYCLONE_TCP_PHY),ksz8051 ksz8081))
	CSRC +=																		\
		$(CYCLONE_STACK_TCP_DRIVERS_DIR)/$(CYCLONE_TCP_PHY).c
else
	ifneq (, $(filter $(CYCLONE_TCP_PHY),none))
	else
		$(error The Cyclone TCP/IP Stack not support PHY $(CYCLONE_TCP_PHY))
	endif
endif

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
## Include directories for general includes
EXTRAINCDIRS +=																	\
	$(CYCLONE_STACK_TCP_DRIVERS_DIR)

# Include Makefiles