# Light Weght Mesh Makefile
###############################################################################

# Path variables
## Path to main project directory
LWMESH_DIR = $(STACK_DIR)/$(STACK_VERSION)
PATH_NWK = $(LWMESH_DIR)/nwk
PATH_PHY = $(LWMESH_DIR)/phy
PATH_SERVICE = $(LWMESH_DIR)/service
PATH_SYS = $(LWMESH_DIR)/sys

HAL_TYPE = $(strip $(MCU))
PHY_TYPE = $(shell echo $(TAL_TYPE) | tr [:upper:] [:lower:])

# List any extra directories to look for include files here.
#     Each directory must be seperated by a space.
## Include directories for general includes
#EXTRAINCDIRS +=

# Include Makefiles
include																			\
	$(PATH_NWK)/makefile.mk														\
	$(PATH_PHY)/$(PHY_TYPE)/makefile.mk											\
	$(PATH_SERVICE)/makefile.mk													\
	$(PATH_SYS)/makefile.mk