# MAC 802.15.4 makefile
###############################################################################

#list C source files
CSRC +=																			\
	$(PATH_MAC)/Src/mac.c														\
	$(PATH_MAC)/Src/mac_api.c													\
	$(PATH_MAC)/Src/mac_associate.c												\
	$(PATH_MAC)/Src/mac_beacon.c												\
	$(PATH_MAC)/Src/mac_callback_wrapper.c										\
	$(PATH_MAC)/Src/mac_data_ind.c												\
	$(PATH_MAC)/Src/mac_data_req.c												\
	$(PATH_MAC)/Src/mac_disassociate.c											\
	$(PATH_MAC)/Src/mac_dispatcher.c											\
	$(PATH_MAC)/Src/mac_misc.c													\
	$(PATH_MAC)/Src/mac_orphan.c												\
	$(PATH_MAC)/Src/mac_pib.c													\
	$(PATH_MAC)/Src/mac_poll.c													\
	$(PATH_MAC)/Src/mac_process_beacon_frame.c									\
	$(PATH_MAC)/Src/mac_process_tal_tx_frame_status.c							\
	$(PATH_MAC)/Src/mac_rx_enable.c												\
	$(PATH_MAC)/Src/mac_scan.c													\
	$(PATH_MAC)/Src/mac_start.c													\
	$(PATH_MAC)/Src/mac_sync.c													\
	$(PATH_MAC)/Src/mac_tx_coord_realignment_command.c							\
	$(PATH_MAC)/Src/mac_mcps_data.c												\
	$(PATH_MAC)/Src/mac_security.c											\
	$(PATH_MAC)/Src/mac_data_extract_mhr.c

CSRC +=																			\
	$(PATH_MAC)/Src/usr_mcps_data_conf.c										\
	$(PATH_MAC)/Src/usr_mcps_data_ind.c											\
	$(PATH_MAC)/Src/usr_mcps_purge_conf.c										\

CSRC +=																			\
	$(PATH_MAC)/Src/usr_mlme_associate_conf.c									\
	$(PATH_MAC)/Src/usr_mlme_associate_ind.c									\
	$(PATH_MAC)/Src/usr_mlme_beacon_notify_ind.c								\
	$(PATH_MAC)/Src/usr_mlme_comm_status_ind.c									\
	$(PATH_MAC)/Src/usr_mlme_disassociate_conf.c								\
	$(PATH_MAC)/Src/usr_mlme_disassociate_ind.c									\
	$(PATH_MAC)/Src/usr_mlme_get_conf.c											\
	$(PATH_MAC)/Src/usr_mlme_orphan_ind.c										\
	$(PATH_MAC)/Src/usr_mlme_poll_conf.c										\
	$(PATH_MAC)/Src/usr_mlme_reset_conf.c										\
	$(PATH_MAC)/Src/usr_mlme_rx_enable_conf.c									\
	$(PATH_MAC)/Src/usr_mlme_scan_conf.c										\
	$(PATH_MAC)/Src/usr_mlme_set_conf.c											\
	$(PATH_MAC)/Src/usr_mlme_start_conf.c										\
	$(PATH_MAC)/Src/usr_mlme_sync_loss_ind.c

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
## Include directories for MAC
EXTRAINCDIRS +=																	\
	$(PATH_MAC)/Inc