ASF_COMPONENT_CRIPTO_SHA204_DIR			= $(ASF_COMPONENT_CRIPTO_DIR)/sha204

#list C source files
ifeq ($(strip $(ASF_COMPONENT_CRIPTO_SHA204)),1)

CSRC +=																			\
	$(ASF_COMPONENT_CRIPTO_SHA204_DIR)/sha204_comm.c							\
	$(ASF_COMPONENT_CRIPTO_SHA204_DIR)/sha204_command_marshaling.c				\
	$(ASF_COMPONENT_CRIPTO_SHA204_DIR)/sha204_i2c.c

CSRC +=																			\
	$(ASF_COMPONENT_CRIPTO_SHA204_DIR)/xmega_sha204/sha204_timer.c

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
	$(ASF_COMPONENT_CRIPTO_SHA204_DIR)											\
	$(ASF_COMPONENT_CRIPTO_SHA204_DIR)/xmega_sha204

endif