#list C source files

CSRC	+=																		\
	$(ASF_THIRDPARTY_QTOUCH_QDEBUG_DIR)/QDebugTransport.c

ifneq (, $(filter $(PLATFORM),mega xmega))
CSRC	+=																		\
	$(ASF_THIRDPARTY_QTOUCH_QDEBUG_DIR)/eeprom_access.c							\
	$(ASF_THIRDPARTY_QTOUCH_QDEBUG_DIR)/eeprom_access_mega.c					\
	$(ASF_THIRDPARTY_QTOUCH_QDEBUG_DIR)/QDebug.c
endif

ifneq (, $(filter $(PLATFORM),uc3))
CSRC	+=																		\
	$(ASF_THIRDPARTY_QTOUCH_QDEBUG_DIR)/QDebug_at32uc3l.c
endif

ifneq (, $(filter $(ARMTYPE),SAM4L))
CSRC	+=																		\
	$(ASF_THIRDPARTY_QTOUCH_QDEBUG_DIR)/QDebug_sam4l.c
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
	$(ASF_THIRDPARTY_QTOUCH_QDEBUG_DIR)