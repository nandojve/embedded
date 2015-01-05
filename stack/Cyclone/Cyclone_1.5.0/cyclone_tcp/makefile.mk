# Cyclone TCP Directory Makefile
###############################################################################

# Path variables
## Path to main project directory
CYCLONE_STACK_TCP_CORE_DIR				= $(CYCLONE_STACK_TCP_DIR)/core
CYCLONE_STACK_TCP_DHCP_DIR				= $(CYCLONE_STACK_TCP_DIR)/dhcp
CYCLONE_STACK_TCP_DHCPV6_DIR			= $(CYCLONE_STACK_TCP_DIR)/dhcpv6
CYCLONE_STACK_TCP_DNS_DIR				= $(CYCLONE_STACK_TCP_DIR)/dns
CYCLONE_STACK_TCP_DRIVERS_DIR			= $(CYCLONE_STACK_TCP_DIR)/drivers
CYCLONE_STACK_TCP_FTP_DIR				= $(CYCLONE_STACK_TCP_DIR)/ftp
CYCLONE_STACK_TCP_HTTP_DIR				= $(CYCLONE_STACK_TCP_DIR)/http
CYCLONE_STACK_TCP_ICECAST_DIR			= $(CYCLONE_STACK_TCP_DIR)/icecast
CYCLONE_STACK_TCP_IPV4_DIR				= $(CYCLONE_STACK_TCP_DIR)/ipv4
CYCLONE_STACK_TCP_IPV6_DIR				= $(CYCLONE_STACK_TCP_DIR)/ipv6
CYCLONE_STACK_TCP_MDNS_DIR				= $(CYCLONE_STACK_TCP_DIR)/mdns
CYCLONE_STACK_TCP_NETBIOS_DIR			= $(CYCLONE_STACK_TCP_DIR)/netbios
CYCLONE_STACK_TCP_SMTP_DIR				= $(CYCLONE_STACK_TCP_DIR)/smtp
CYCLONE_STACK_TCP_SNMP_DIR				= $(CYCLONE_STACK_TCP_DIR)/snmp
CYCLONE_STACK_TCP_SNTP_DIR				= $(CYCLONE_STACK_TCP_DIR)/sntp
CYCLONE_STACK_TCP_STD_SERVICES_DIR		= $(CYCLONE_STACK_TCP_DIR)/std_services

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
	$(CYCLONE_STACK_TCP_DIR)

# Include Makefiles
include																			\
	$(CYCLONE_STACK_TCP_CORE_DIR)/makefile.mk									\
	$(CYCLONE_STACK_TCP_DHCP_DIR)/makefile.mk									\
	$(CYCLONE_STACK_TCP_DHCPV6_DIR)/makefile.mk									\
	$(CYCLONE_STACK_TCP_DNS_DIR)/makefile.mk									\
	$(CYCLONE_STACK_TCP_DRIVERS_DIR)/makefile.mk								\
	$(CYCLONE_STACK_TCP_FTP_DIR)/makefile.mk									\
	$(CYCLONE_STACK_TCP_HTTP_DIR)/makefile.mk									\
	$(CYCLONE_STACK_TCP_ICECAST_DIR)/makefile.mk								\
	$(CYCLONE_STACK_TCP_IPV4_DIR)/makefile.mk									\
	$(CYCLONE_STACK_TCP_IPV6_DIR)/makefile.mk									\
	$(CYCLONE_STACK_TCP_MDNS_DIR)/makefile.mk									\
	$(CYCLONE_STACK_TCP_NETBIOS_DIR)/makefile.mk								\
	$(CYCLONE_STACK_TCP_SMTP_DIR)/makefile.mk									\
	$(CYCLONE_STACK_TCP_SNMP_DIR)/makefile.mk									\
	$(CYCLONE_STACK_TCP_SNTP_DIR)/makefile.mk									\
	$(CYCLONE_STACK_TCP_STD_SERVICES_DIR)/makefile.mk