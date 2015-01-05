# Cyclone Stack Makefile
###############################################################################

# Path variables
## Path to main project directory
CYCLONE_STACK_COMMON_DIR				= $(CYCLONE_STACK_DIR)/common
CYCLONE_STACK_CRYPTO_DIR				= $(CYCLONE_STACK_DIR)/cyclone_crypto
CYCLONE_STACK_SSL_DIR					= $(CYCLONE_STACK_DIR)/cyclone_ssl
CYCLONE_STACK_TCP_DIR					= $(CYCLONE_STACK_DIR)/cyclone_tcp
CYCLONE_STACK_DEMO_DIR					= $(CYCLONE_STACK_DIR)/demo

# List any extra directories to look for include files here.
#     Each directory must be seperated by a space.
## Include directories for general includes
#EXTRAINCDIRS +=

# Include Makefiles
include																			\
	$(CYCLONE_STACK_COMMON_DIR)/makefile.mk										\
	$(CYCLONE_STACK_CRYPTO_DIR)/makefile.mk										\
	$(CYCLONE_STACK_SSL_DIR)/makefile.mk										\
	$(CYCLONE_STACK_TCP_DIR)/makefile.mk										\
	$(CYCLONE_STACK_DEMO_DIR)/makefile.mk