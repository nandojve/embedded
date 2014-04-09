ASF_THIRDPARTY_FATFS_PORT_DIR			= $(ASF_THIRDPARTY_FATFS_DIR)/fatfs-port-r0.09
ASF_THIRDPARTY_FATFS_SYS_DIR			= $(ASF_THIRDPARTY_FATFS_DIR)/fatfs-r0.09

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
EXTRAINCDIRS +=

ifneq (, $(filter $(PLATFORM),sam sam0 xmega))
include $(ASF_THIRDPARTY_FATFS_PORT_DIR)/makefile.mk
include $(ASF_THIRDPARTY_FATFS_SYS_DIR)/makefile.mk
else
#error "FATFS port is avaliable only for platform sam, sam0 and xmega" 
endif