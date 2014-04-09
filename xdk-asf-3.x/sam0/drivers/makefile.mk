ASF_DRIVERS_DIR					= $(ASF_PLATFORM_DIR)/drivers

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

include																			\
		$(ASF_DRIVERS_DIR)/ac/makefile.mk										\
		$(ASF_DRIVERS_DIR)/adc/makefile.mk										\
		$(ASF_DRIVERS_DIR)/bod/makefile.mk										\
		$(ASF_DRIVERS_DIR)/dac/makefile.mk										\
		$(ASF_DRIVERS_DIR)/events/makefile.mk									\
		$(ASF_DRIVERS_DIR)/extint/makefile.mk									\
		$(ASF_DRIVERS_DIR)/nvm/makefile.mk										\
		$(ASF_DRIVERS_DIR)/pac/makefile.mk										\
		$(ASF_DRIVERS_DIR)/port/makefile.mk										\
		$(ASF_DRIVERS_DIR)/rtc/makefile.mk										\
		$(ASF_DRIVERS_DIR)/sercom/makefile.mk									\
		$(ASF_DRIVERS_DIR)/system/makefile.mk									\
		$(ASF_DRIVERS_DIR)/tc/makefile.mk										\
		$(ASF_DRIVERS_DIR)/wdt/makefile.mk