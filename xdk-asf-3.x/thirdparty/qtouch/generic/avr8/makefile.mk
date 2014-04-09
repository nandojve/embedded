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
ifneq (, $(filter $(PLATFORM),mega))
ASRC +=																			\
	$(ASF_THIRDPARTY_QTOUCH_GENERIC_PLATFORM_DIR)/qmatrix/common/qm_asm_tiny_mega.S
endif
ifneq (, $(filter $(PLATFORM),xmega))
ASRC +=																			\
	$(ASF_THIRDPARTY_QTOUCH_GENERIC_PLATFORM_DIR)/qtouch/common/qt_asm_xmega.S	\
	$(ASF_THIRDPARTY_QTOUCH_GENERIC_PLATFORM_DIR)/qmatrix/common/qm_asm_xmega.S
endif

# List any extra directories to look for include files here.
#     Each directory must be seperated by a space.
EXTRAINCDIRS +=																	\
	$(ASF_THIRDPARTY_QTOUCH_GENERIC_DIR)										\
	$(ASF_THIRDPARTY_QTOUCH_GENERIC_DIR)/include								\
	$(ASF_THIRDPARTY_QTOUCH_GENERIC_PLATFORM_DIR)/qtouch/common					\
	$(ASF_THIRDPARTY_QTOUCH_GENERIC_PLATFORM_DIR)/qmatrix/common

# List any extra directories to look for libraries here.
#     Each directory must be seperated by a space.
#     Use forward slashes for directory separators.
#     For a directory that has spaces, enclose it in quotes.
EXTRALIBDIRS +=																	\	
	$(ASF_THIRDPARTY_QTOUCH_GENERIC_PLATFORM_DIR)/qtouch/common					\
	$(ASF_THIRDPARTY_QTOUCH_GENERIC_PLATFORM_DIR)/qmatrix/common
