# TAL 802.15.4 Makefile
###############################################################################

# Path variables
## Path to main project directory
RADIO_DIR = $(ROOT_DIR)/802.15.4/ATMAC-802.15.4
PATH_PAL = $(RADIO_DIR)/PAL
PATH_RESOURCE = $(RADIO_DIR)/Resources
PATH_TAL = $(RADIO_DIR)/TAL
PATH_TFA = $(RADIO_DIR)/TFA

PAL_TYPE = $(strip $(MCU))
PAL_GENERIC_TYPE = $(strip $(PLATFORM))
PAL_GENERIC_TYPE = $(PAL_GENERIC_TYPE)
HIGHEST_STACK_LAYER = TAL

# List any extra directories to look for include files here.
#     Each directory must be seperated by a space.
## Include directories for general includes
EXTRAINCDIRS +=																	\
	$(RADIO_DIR)/Include

## Include directories for TAL
EXTRAINCDIRS +=																	\
	$(PATH_TAL)/Inc																\
	$(PATH_TAL)/$(TAL_TYPE)/Inc

# Include Makefiles
include																			\
	$(PATH_PAL)/$(PAL_GENERIC_TYPE)/makefile.mk									\
	$(PATH_RESOURCE)/makefile.mk												\
	$(PATH_TFA)/makefile.mk														\
	$(PATH_TAL)/makefile.mk