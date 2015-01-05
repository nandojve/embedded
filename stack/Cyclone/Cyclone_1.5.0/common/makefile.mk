# Cyclone Common Directory Makefile
###############################################################################

# Path variables
## Path to main project directory

#list C source files
CSRC +=																			\
	$(CYCLONE_STACK_COMMON_DIR)/date_time.c										\
	$(CYCLONE_STACK_COMMON_DIR)/debug.c											\
	$(CYCLONE_STACK_COMMON_DIR)/endian.c										\
	$(CYCLONE_STACK_COMMON_DIR)/path.c											\
	$(CYCLONE_STACK_COMMON_DIR)/resource_manager.c								\
	$(CYCLONE_STACK_COMMON_DIR)/str.c

ifneq (, $(filter $(CYCLONE_TCP_FATFS),yes YES 1))
	CSRC +=																		\
		$(CYCLONE_STACK_COMMON_DIR)/fs_port_fatfs.c
endif
ifneq (, $(filter $(CYCLONE_RTOS),chibios cmsis_rtos freertos rtx ucos3 windows))
	CSRC +=																		\
		$(CYCLONE_STACK_COMMON_DIR)/os_port_$(CYCLONE_RTOS).c
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
	$(CYCLONE_STACK_COMMON_DIR)

# Include Makefiles