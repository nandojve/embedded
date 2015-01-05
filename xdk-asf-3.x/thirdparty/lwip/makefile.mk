ASF_THIRDPARTY_LWIP_SOURCE_DIR			= $(ASF_THIRDPARTY_LWIP_DIR)/lwip-$(LWIP_VERSION)/src
ASF_THIRDPARTY_LWIP_PORTABLE_DIR		= $(ASF_THIRDPARTY_LWIP_DIR)/lwip-port-$(LWIP_VERSION)

#list C source files
###############################################################################

CSRC	+=																		\
	$(ASF_THIRDPARTY_LWIP_SOURCE_DIR)/api/api_lib.c								\
	$(ASF_THIRDPARTY_LWIP_SOURCE_DIR)/api/api_msg.c								\
	$(ASF_THIRDPARTY_LWIP_SOURCE_DIR)/api/err.c									\
	$(ASF_THIRDPARTY_LWIP_SOURCE_DIR)/api/netbuf.c								\
	$(ASF_THIRDPARTY_LWIP_SOURCE_DIR)/api/netdb.c								\
	$(ASF_THIRDPARTY_LWIP_SOURCE_DIR)/api/netifapi.c							\
	$(ASF_THIRDPARTY_LWIP_SOURCE_DIR)/api/sockets.c								\
	$(ASF_THIRDPARTY_LWIP_SOURCE_DIR)/api/tcpip.c

ifneq (, $(filter $(LWIP_VERSION),1.3.2))
	CSRC	+=																	\
		$(ASF_THIRDPARTY_LWIP_SOURCE_DIR)/core/init.c
endif
ifneq (, $(filter $(LWIP_VERSION),1.4.0))
	CSRC	+=																	\
		$(ASF_THIRDPARTY_LWIP_SOURCE_DIR)/core/init.c							\
		$(ASF_THIRDPARTY_LWIP_SOURCE_DIR)/core/lwip_timers_140.c
endif
ifneq (, $(filter $(LWIP_VERSION),1.4.1))
	CSRC	+=																	\
		$(ASF_THIRDPARTY_LWIP_SOURCE_DIR)/core/lwip_init.c						\
		$(ASF_THIRDPARTY_LWIP_SOURCE_DIR)/core/lwip_timers_141.c
endif
ifneq (, $(filter $(LWIP_VERSION),1.4.0 1.4.1))
	CSRC	+=																	\
		$(ASF_THIRDPARTY_LWIP_SOURCE_DIR)/core/def.c
endif

CSRC	+=																		\
	$(ASF_THIRDPARTY_LWIP_SOURCE_DIR)/core/dhcp.c								\
	$(ASF_THIRDPARTY_LWIP_SOURCE_DIR)/core/dns.c								\
	$(ASF_THIRDPARTY_LWIP_SOURCE_DIR)/core/mem.c								\
	$(ASF_THIRDPARTY_LWIP_SOURCE_DIR)/core/memp.c								\
	$(ASF_THIRDPARTY_LWIP_SOURCE_DIR)/core/netif.c								\
	$(ASF_THIRDPARTY_LWIP_SOURCE_DIR)/core/pbuf.c								\
	$(ASF_THIRDPARTY_LWIP_SOURCE_DIR)/core/raw.c								\
	$(ASF_THIRDPARTY_LWIP_SOURCE_DIR)/core/stats.c								\
	$(ASF_THIRDPARTY_LWIP_SOURCE_DIR)/core/sys.c								\
	$(ASF_THIRDPARTY_LWIP_SOURCE_DIR)/core/tcp.c								\
	$(ASF_THIRDPARTY_LWIP_SOURCE_DIR)/core/tcp_in.c								\
	$(ASF_THIRDPARTY_LWIP_SOURCE_DIR)/core/tcp_out.c							\
	$(ASF_THIRDPARTY_LWIP_SOURCE_DIR)/core/udp.c

CSRC	+=																		\
	$(ASF_THIRDPARTY_LWIP_SOURCE_DIR)/core/ipv4/autoip.c						\
	$(ASF_THIRDPARTY_LWIP_SOURCE_DIR)/core/ipv4/icmp.c							\
	$(ASF_THIRDPARTY_LWIP_SOURCE_DIR)/core/ipv4/igmp.c							\
	$(ASF_THIRDPARTY_LWIP_SOURCE_DIR)/core/ipv4/inet.c							\
	$(ASF_THIRDPARTY_LWIP_SOURCE_DIR)/core/ipv4/inet_chksum.c					\
	$(ASF_THIRDPARTY_LWIP_SOURCE_DIR)/core/ipv4/ip.c							\
	$(ASF_THIRDPARTY_LWIP_SOURCE_DIR)/core/ipv4/ip_addr.c						\
	$(ASF_THIRDPARTY_LWIP_SOURCE_DIR)/core/ipv4/ip_frag.c

#CSRC	+=																		\
	$(ASF_THIRDPARTY_LWIP_SOURCE_DIR)/core/ipv6/icmp6.c							\
	$(ASF_THIRDPARTY_LWIP_SOURCE_DIR)/core/ipv6/inet6.c							\
	$(ASF_THIRDPARTY_LWIP_SOURCE_DIR)/core/ipv6/ip6.c							\
	$(ASF_THIRDPARTY_LWIP_SOURCE_DIR)/core/ipv6/ip6_addr.c

CSRC	+=																		\
	$(ASF_THIRDPARTY_LWIP_SOURCE_DIR)/core/snmp/asn1_dec.c						\
	$(ASF_THIRDPARTY_LWIP_SOURCE_DIR)/core/snmp/asn1_enc.c						\
	$(ASF_THIRDPARTY_LWIP_SOURCE_DIR)/core/snmp/mib_structs.c					\
	$(ASF_THIRDPARTY_LWIP_SOURCE_DIR)/core/snmp/mib2.c							\
	$(ASF_THIRDPARTY_LWIP_SOURCE_DIR)/core/snmp/msg_in.c						\
	$(ASF_THIRDPARTY_LWIP_SOURCE_DIR)/core/snmp/msg_out.c

CSRC	+=																		\
	$(ASF_THIRDPARTY_LWIP_SOURCE_DIR)/netif/etharp.c							\
	$(ASF_THIRDPARTY_LWIP_SOURCE_DIR)/netif/ethernetif.c						\
	$(ASF_THIRDPARTY_LWIP_SOURCE_DIR)/netif/slipif.c							\
	$(ASF_THIRDPARTY_LWIP_SOURCE_DIR)/netif/ppp/auth.c							\
	$(ASF_THIRDPARTY_LWIP_SOURCE_DIR)/netif/ppp/chap.c							\
	$(ASF_THIRDPARTY_LWIP_SOURCE_DIR)/netif/ppp/chpms.c							\
	$(ASF_THIRDPARTY_LWIP_SOURCE_DIR)/netif/ppp/fsm.c							\
	$(ASF_THIRDPARTY_LWIP_SOURCE_DIR)/netif/ppp/ipcp.c							\
	$(ASF_THIRDPARTY_LWIP_SOURCE_DIR)/netif/ppp/lcp.c							\
	$(ASF_THIRDPARTY_LWIP_SOURCE_DIR)/netif/ppp/magic.c							\
	$(ASF_THIRDPARTY_LWIP_SOURCE_DIR)/netif/ppp/md5.c							\
	$(ASF_THIRDPARTY_LWIP_SOURCE_DIR)/netif/ppp/pap.c							\
	$(ASF_THIRDPARTY_LWIP_SOURCE_DIR)/netif/ppp/ppp.c							\
	$(ASF_THIRDPARTY_LWIP_SOURCE_DIR)/netif/ppp/ppp_oe.c						\
	$(ASF_THIRDPARTY_LWIP_SOURCE_DIR)/netif/ppp/randm.c							\
	$(ASF_THIRDPARTY_LWIP_SOURCE_DIR)/netif/ppp/vj.c

#list C portable source files
###############################################################################

ifneq (, $(filter $(PLATFORM),uc3))
	ASF_THIRDPARTY_LWIP_PORTABLE_DEV_DIR = $(ASF_THIRDPARTY_LWIP_PORTABLE_DIR)/at32uc3
endif

ifneq (, $(filter $(PLATFORM),sam sam0))
	ASF_THIRDPARTY_LWIP_PORTABLE_DEV_DIR = $(ASF_THIRDPARTY_LWIP_PORTABLE_DIR)/sam
endif

ifdef ASF_THIRDPARTY_LWIP_PORTABLE_DEV_DIR
	CSRC	+=																	\
		$(ASF_THIRDPARTY_LWIP_PORTABLE_DEV_DIR)/sys_arch.c

	ifneq (, $(filter $(ARMTYPE),SAM4E))
		ifneq (, $(filter $(LWIP_VERSION),1.4.1))
			CSRC	+=															\
				$(ASF_THIRDPARTY_LWIP_PORTABLE_DEV_DIR)/netif/sam4e_gmac.c
		else
			CSRC	+=															\
				$(ASF_THIRDPARTY_LWIP_PORTABLE_DEV_DIR)/netif/ethernetif_sam4.c
		endif
	else
		CSRC	+=																\
			$(ASF_THIRDPARTY_LWIP_PORTABLE_DEV_DIR)/netif/ethernetif.c
	endif
else
	ifdef ARMTYPE
		$(error Platform $(PLATFORM) with ARM TYPE $(ARMTYPE) is not ported to LWIP version $(LWIP_VERSION))
	else
		$(error Platform $(PLATFORM) is not ported to LWIP version $(LWIP_VERSION))
	endif
endif

# List C++ source files here. (C dependencies are automatically generated.)
###############################################################################
CPPSRC += 

###############################################################################
# List Assembler source files here.
# Make them always end in a capital .S.  Files ending in a lowercase .s
# will not be considered source files but generated files (assembler
# output from the compiler), and will be deleted upon "make clean"!
# Even though the DOS/Win* filesystem matches both .s and .S the same,
# it will preserve the spelling of the filenames, and gcc itself does
# care about how the name is spelled on its command-line.
ASRC +=

###############################################################################
# List any extra directories to look for include files here.
#     Each directory must be seperated by a space.
EXTRAINCDIRS +=																	\
	$(ASF_THIRDPARTY_LWIP_SOURCE_DIR)/include									\
	$(ASF_THIRDPARTY_LWIP_SOURCE_DIR)/include/ipv4								\
	$(ASF_THIRDPARTY_LWIP_SOURCE_DIR)/include/netif								\
	$(ASF_THIRDPARTY_LWIP_SOURCE_DIR)/include/posix								\
	$(ASF_THIRDPARTY_LWIP_SOURCE_DIR)/netif/ppp									\
	$(ASF_THIRDPARTY_LWIP_PORTABLE_DEV_DIR)/include


#$(ASF_THIRDPARTY_LWIP_SOURCE_DIR)/include/ipv6