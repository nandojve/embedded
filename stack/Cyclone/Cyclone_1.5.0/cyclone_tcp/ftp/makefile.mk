# Cyclone TCP FTP Directory Makefile
###############################################################################

#ifneq (, $(filter $(CYCLONE_SERVICES),ipv4))
#ifneq (, $(filter $(CYCLONE_SERVICES),ftp))

# Path variables
## Path to main project directory

#list C source files
CSRC +=																			\
	$(CYCLONE_STACK_TCP_FTP_DIR)/ftp_client.c									\
	$(CYCLONE_STACK_TCP_FTP_DIR)/ftp_server.c									\
	$(CYCLONE_STACK_TCP_FTP_DIR)/ftp_server_commands.c							\
	$(CYCLONE_STACK_TCP_FTP_DIR)/ftp_server_events.c							\
	$(CYCLONE_STACK_TCP_FTP_DIR)/ftp_server_misc.c

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
	$(CYCLONE_STACK_TCP_FTP_DIR)

# Include Makefiles

#endif
#endif