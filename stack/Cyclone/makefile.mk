# Cyclone Makefile
###############################################################################

# Path variables
## Path to main project directory
CYCLONE_STACK_DIR				= $(STACK_DIR)/Cyclone_$(STACK_VERSION)

# List any extra directories to look for include files here.
#     Each directory must be seperated by a space.
## Include directories for general includes
#EXTRAINCDIRS +=

# Include Makefiles
include $(CYCLONE_STACK_DIR)/makefile.mk