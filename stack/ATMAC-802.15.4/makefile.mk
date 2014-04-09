# MAC 802.15.4 Makefile
###############################################################################

# Path variables
## Path to main project directory
RADIO_DIR = $(ROOT_DIR)/Stack/ATMAC-802.15.4
PATH_PAL = $(RADIO_DIR)/PAL
PATH_RESOURCE = $(RADIO_DIR)/Resources
PATH_TAL = $(RADIO_DIR)/TAL
PATH_TFA = $(RADIO_DIR)/TFA
PATH_MAC = $(RADIO_DIR)/MAC

_PAL_TYPE = $(strip $(MCU))
_PAL_GENERIC_TYPE = $(strip $(PLATFORM_OLD))
PAL_GENERIC_TYPE = $(_PAL_GENERIC_TYPE)
_HIGHEST_STACK_LAYER = MAC

# List any extra directories to look for include files here.
#     Each directory must be seperated by a space.
## Include directories for general includes
EXTRAINCDIRS +=																	\
	$(RADIO_DIR)/Include

# Include Makefiles
include																			\
	$(PATH_PAL)/$(PAL_GENERIC_TYPE)/makefile.mk									\
	$(PATH_RESOURCE)/makefile.mk												\
	$(PATH_TAL)/makefile.mk														\
	$(PATH_TFA)/makefile.mk														\
	$(PATH_MAC)/makefile.mk