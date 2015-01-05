# Generic Stack Makefile
###############################################################################

ifneq (, $(filter $(STACK_TYPE),LwMesh Cyclone ATMAC-802.15.4))

STACK_DIR								= $(ROOT_DIR)/stack/$(STACK_TYPE)

# Path variables
## Path to main project directory

# List any extra directories to look for include files here.
#     Each directory must be seperated by a space.
## Include directories for general includes
#EXTRAINCDIRS +=

# Include Makefiles
include $(STACK_DIR)/makefile.mk

endif