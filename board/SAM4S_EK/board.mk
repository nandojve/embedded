Board_DIR			= $(ROOT_DIR)/board/$(BOARD_TYPE)

#list C source files
CSRC +=																			\
	$(Board_DIR)/src/board_init.c

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

EXTRAINCDIRS +=																	\
	$(Board_DIR)/inc															\
	$(Board_DIR)/config

ifeq ($(strip $(USE_WIRELESS)),1)
	ifeq ($(strip $(STACK_TYPE)),LwMesh)
		include $(Board_DIR)/LwMesh/board.mk
	endif
endif