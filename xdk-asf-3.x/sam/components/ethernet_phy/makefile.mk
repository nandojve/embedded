ifneq (, $(filter $(ASF_DRIVER_COMPONENT_ETHERNET_PHY),1))

ASF_DRIVER_COMPONENT_ETHERNET_PHY_DIR	= $(ASF_COMPONENTS_DIR)/ethernet_phy/$(ETHERNET_PHY_MODEL)

ifneq (, $(filter $(ETHERNET_PHY_MODEL),ksz8851snl))
	CSRC	+=																	\
		$(ASF_DRIVER_COMPONENT_ETHERNET_PHY_DIR)/ksz8851snl.c
else
	CSRC	+=																	\
		$(ASF_DRIVER_COMPONENT_ETHERNET_PHY_DIR)/ethernet_phy.c
endif


ASF_COMPONENTS_DIR					= $(ASF_PLATFORM_DIR)/components

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
EXTRAINCDIRS += $(ASF_DRIVER_COMPONENT_ETHERNET_PHY_DIR)

endif