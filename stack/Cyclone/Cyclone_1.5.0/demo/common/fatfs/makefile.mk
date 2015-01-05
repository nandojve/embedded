ifneq (, $(filter $(CYCLONE_TCP_FATFS),yes YES 1))

CYCLONE_STACK_DEMO_COMMON_FATFS_OPTION_DIR = $(CYCLONE_STACK_DEMO_COMMON_FATFS_DIR)/option

#list C source files
###############################################################################

CSRC	+=																		\
	$(CYCLONE_STACK_DEMO_COMMON_FATFS_DIR)/diskio.c								\
	$(CYCLONE_STACK_DEMO_COMMON_FATFS_DIR)/ff.c

CSRC	+=																		\
	$(CYCLONE_STACK_DEMO_COMMON_FATFS_OPTION_DIR)/cc932.c						\
	$(CYCLONE_STACK_DEMO_COMMON_FATFS_OPTION_DIR)/cc936.c						\
	$(CYCLONE_STACK_DEMO_COMMON_FATFS_OPTION_DIR)/cc949.c						\
	$(CYCLONE_STACK_DEMO_COMMON_FATFS_OPTION_DIR)/cc950.c						\
	$(CYCLONE_STACK_DEMO_COMMON_FATFS_OPTION_DIR)/ccsbcs.c						\
	$(CYCLONE_STACK_DEMO_COMMON_FATFS_OPTION_DIR)/syscall.c						\
	$(CYCLONE_STACK_DEMO_COMMON_FATFS_OPTION_DIR)/unicode.c

#list C portable source files
###############################################################################


# List C++ source files here. (C dependencies are automatically generated.)
###############################################################################
CPPSRC += 

###############################################################################
# List Assembler source files here.
# Make them always end in a capital .S.  Files ending in a lowercase .s
# will not be considered source files but generated files (assembler
# output from the compiler), and will be deleted upon "make clean"!
# Even though the DOS/Win* filesystem matches both .s and .S the same,
# it will preserve the spelling of the filenames, and gcc itself does
# care about how the name is spelled on its command-line.
ASRC +=

###############################################################################
# List any extra directories to look for include files here.
#     Each directory must be seperated by a space.
EXTRAINCDIRS +=																	\
	$(CYCLONE_STACK_DEMO_COMMON_FATFS_DIR)

endif