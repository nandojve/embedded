# Cyclone TCP Core Directory Makefile
###############################################################################

# Path variables
## Path to main project directory

#list C source files
CSRC +=																			\
	$(CYCLONE_STACK_TCP_CORE_DIR)/bsd_socket.c									\
	$(CYCLONE_STACK_TCP_CORE_DIR)/ethernet.c									\
	$(CYCLONE_STACK_TCP_CORE_DIR)/ip.c											\
	$(CYCLONE_STACK_TCP_CORE_DIR)/nic.c											\
	$(CYCLONE_STACK_TCP_CORE_DIR)/ping.c										\
	$(CYCLONE_STACK_TCP_CORE_DIR)/raw_socket.c									\
	$(CYCLONE_STACK_TCP_CORE_DIR)/socket.c										\
	$(CYCLONE_STACK_TCP_CORE_DIR)/tcp.c											\
	$(CYCLONE_STACK_TCP_CORE_DIR)/tcp_fsm.c										\
	$(CYCLONE_STACK_TCP_CORE_DIR)/tcp_ip_stack.c								\
	$(CYCLONE_STACK_TCP_CORE_DIR)/tcp_ip_stack_mem.c							\
	$(CYCLONE_STACK_TCP_CORE_DIR)/tcp_misc.c									\
	$(CYCLONE_STACK_TCP_CORE_DIR)/tcp_timer.c									\
	$(CYCLONE_STACK_TCP_CORE_DIR)/udp.c

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
	$(CYCLONE_STACK_TCP_CORE_DIR)

# Include Makefiles