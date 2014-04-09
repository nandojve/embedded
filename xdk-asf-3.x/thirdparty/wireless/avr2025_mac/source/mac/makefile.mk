ASF_THIRDPARTY_WIRELESS_STACK_MAC_DIR	= $(ASF_THIRDPARTY_WIRELESS_STACK_DIR)/source/mac

#list C source files
CSRC	+=																		\
	$(ASF_THIRDPARTY_WIRELESS_STACK_MAC_DIR)/src/mac.c							\
	$(ASF_THIRDPARTY_WIRELESS_STACK_MAC_DIR)/src/mac_api.c						\
	$(ASF_THIRDPARTY_WIRELESS_STACK_MAC_DIR)/src/mac_associate.c				\
	$(ASF_THIRDPARTY_WIRELESS_STACK_MAC_DIR)/src/mac_beacon.c					\
	$(ASF_THIRDPARTY_WIRELESS_STACK_MAC_DIR)/src/mac_callback_wrapper.c			\
	$(ASF_THIRDPARTY_WIRELESS_STACK_MAC_DIR)/src/mac_data_extract_mhr.c			\
	$(ASF_THIRDPARTY_WIRELESS_STACK_MAC_DIR)/src/mac_data_ind.c					\
	$(ASF_THIRDPARTY_WIRELESS_STACK_MAC_DIR)/src/mac_data_req.c					\
	$(ASF_THIRDPARTY_WIRELESS_STACK_MAC_DIR)/src/mac_disassociate.c				\
	$(ASF_THIRDPARTY_WIRELESS_STACK_MAC_DIR)/src/mac_dispatcher.c				\
	$(ASF_THIRDPARTY_WIRELESS_STACK_MAC_DIR)/src/mac_gts.c						\
	$(ASF_THIRDPARTY_WIRELESS_STACK_MAC_DIR)/src/mac_mcps_data.c				\
	$(ASF_THIRDPARTY_WIRELESS_STACK_MAC_DIR)/src/mac_misc.c						\
	$(ASF_THIRDPARTY_WIRELESS_STACK_MAC_DIR)/src/mac_orphan.c					\
	$(ASF_THIRDPARTY_WIRELESS_STACK_MAC_DIR)/src/mac_pib.c						\
	$(ASF_THIRDPARTY_WIRELESS_STACK_MAC_DIR)/src/mac_poll.c						\
	$(ASF_THIRDPARTY_WIRELESS_STACK_MAC_DIR)/src/mac_process_beacon_frame.c		\
	$(ASF_THIRDPARTY_WIRELESS_STACK_MAC_DIR)/src/mac_process_tal_tx_frame_status.c	\
	$(ASF_THIRDPARTY_WIRELESS_STACK_MAC_DIR)/src/mac_rx_enable.c				\
	$(ASF_THIRDPARTY_WIRELESS_STACK_MAC_DIR)/src/mac_scan.c						\
	$(ASF_THIRDPARTY_WIRELESS_STACK_MAC_DIR)/src/mac_security.c					\
	$(ASF_THIRDPARTY_WIRELESS_STACK_MAC_DIR)/src/mac_start.c					\
	$(ASF_THIRDPARTY_WIRELESS_STACK_MAC_DIR)/src/mac_sync.c						\
	$(ASF_THIRDPARTY_WIRELESS_STACK_MAC_DIR)/src/mac_tx_coord_realignment_command.c

CSRC	+=																		\
	$(ASF_THIRDPARTY_WIRELESS_STACK_MAC_DIR)/src/usr_mcps_data_conf.c			\
	$(ASF_THIRDPARTY_WIRELESS_STACK_MAC_DIR)/src/usr_mcps_data_ind.c			\
	$(ASF_THIRDPARTY_WIRELESS_STACK_MAC_DIR)/src/usr_mcps_purge_conf.c			\
	$(ASF_THIRDPARTY_WIRELESS_STACK_MAC_DIR)/src/usr_mlme_associate_conf.c		\
	$(ASF_THIRDPARTY_WIRELESS_STACK_MAC_DIR)/src/usr_mlme_associate_ind.c		\
	$(ASF_THIRDPARTY_WIRELESS_STACK_MAC_DIR)/src/usr_mlme_beacon_notify_ind.c	\
	$(ASF_THIRDPARTY_WIRELESS_STACK_MAC_DIR)/src/usr_mlme_comm_status_ind.c		\
	$(ASF_THIRDPARTY_WIRELESS_STACK_MAC_DIR)/src/usr_mlme_disassociate_conf.c	\
	$(ASF_THIRDPARTY_WIRELESS_STACK_MAC_DIR)/src/usr_mlme_disassociate_ind.c	\
	$(ASF_THIRDPARTY_WIRELESS_STACK_MAC_DIR)/src/usr_mlme_get_conf.c			\
	$(ASF_THIRDPARTY_WIRELESS_STACK_MAC_DIR)/src/usr_mlme_orphan_ind.c			\
	$(ASF_THIRDPARTY_WIRELESS_STACK_MAC_DIR)/src/usr_mlme_poll_conf.c			\
	$(ASF_THIRDPARTY_WIRELESS_STACK_MAC_DIR)/src/usr_mlme_reset_conf.c			\
	$(ASF_THIRDPARTY_WIRELESS_STACK_MAC_DIR)/src/usr_mlme_rx_enable_conf.c		\
	$(ASF_THIRDPARTY_WIRELESS_STACK_MAC_DIR)/src/usr_mlme_scan_conf.c			\
	$(ASF_THIRDPARTY_WIRELESS_STACK_MAC_DIR)/src/usr_mlme_set_conf.c			\
	$(ASF_THIRDPARTY_WIRELESS_STACK_MAC_DIR)/src/usr_mlme_start_conf.c			\
	$(ASF_THIRDPARTY_WIRELESS_STACK_MAC_DIR)/src/usr_mlme_sync_loss_ind.c

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
	$(ASF_THIRDPARTY_WIRELESS_STACK_MAC_DIR)/inc