# Generic Stack Makefile
###############################################################################

# Path variables
## Path to main project directory

ifeq ($(strip $(STACK_TYPE)),LwMesh)
STACK_DIR = $(ROOT_DIR)/stack/$(STACK_TYPE)/$(STACK_VERSION)
else
STACK_DIR = $(ROOT_DIR)/stack/$(STACK_TYPE)
endif

# List any extra directories to look for include files here.
#     Each directory must be seperated by a space.
## Include directories for general includes
#EXTRAINCDIRS +=

# Include Makefiles
include																			\
	$(STACK_DIR)/makefile.mk