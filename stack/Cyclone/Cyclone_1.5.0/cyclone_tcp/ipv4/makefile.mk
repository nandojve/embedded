# Cyclone TCP IPv4 Directory Makefile
###############################################################################

#ifneq (, $(filter $(CYCLONE_SERVICES),ipv4))

# Path variables
## Path to main project directory

#list C source files
CSRC +=																			\
	$(CYCLONE_STACK_TCP_IPV4_DIR)/arp.c											\
	$(CYCLONE_STACK_TCP_IPV4_DIR)/icmp.c										\
	$(CYCLONE_STACK_TCP_IPV4_DIR)/igmp.c										\
	$(CYCLONE_STACK_TCP_IPV4_DIR)/ipv4.c										\
	$(CYCLONE_STACK_TCP_IPV4_DIR)/ipv4_frag.c

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
	$(CYCLONE_STACK_TCP_IPV4_DIR)

# Include Makefiles

#endif