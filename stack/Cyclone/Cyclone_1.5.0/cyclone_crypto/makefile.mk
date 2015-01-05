# Cyclone Crypto Directory Makefile
###############################################################################

# Path variables
## Path to main project directory

#list C source files
CSRC +=																			\
	$(CYCLONE_STACK_CRYPTO_DIR)/aes.c											\
	$(CYCLONE_STACK_CRYPTO_DIR)/aria.c											\
	$(CYCLONE_STACK_CRYPTO_DIR)/asn1.c											\
	$(CYCLONE_STACK_CRYPTO_DIR)/base64.c										\
	$(CYCLONE_STACK_CRYPTO_DIR)/camellia.c										\
	$(CYCLONE_STACK_CRYPTO_DIR)/cipher_mode_cbc.c								\
	$(CYCLONE_STACK_CRYPTO_DIR)/cipher_mode_ccm.c								\
	$(CYCLONE_STACK_CRYPTO_DIR)/cipher_mode_cfb.c								\
	$(CYCLONE_STACK_CRYPTO_DIR)/cipher_mode_ctr.c								\
	$(CYCLONE_STACK_CRYPTO_DIR)/cipher_mode_ecb.c								\
	$(CYCLONE_STACK_CRYPTO_DIR)/cipher_mode_gcm.c								\
	$(CYCLONE_STACK_CRYPTO_DIR)/cipher_mode_ofb.c								\
	$(CYCLONE_STACK_CRYPTO_DIR)/des.c											\
	$(CYCLONE_STACK_CRYPTO_DIR)/des3.c											\
	$(CYCLONE_STACK_CRYPTO_DIR)/dh.c											\
	$(CYCLONE_STACK_CRYPTO_DIR)/dsa.c											\
	$(CYCLONE_STACK_CRYPTO_DIR)/ec.c											\
	$(CYCLONE_STACK_CRYPTO_DIR)/ec_curves.c										\
	$(CYCLONE_STACK_CRYPTO_DIR)/ecdh.c											\
	$(CYCLONE_STACK_CRYPTO_DIR)/ecdsa.c											\
	$(CYCLONE_STACK_CRYPTO_DIR)/hmac.c											\
	$(CYCLONE_STACK_CRYPTO_DIR)/idea.c											\
	$(CYCLONE_STACK_CRYPTO_DIR)/md2.c											\
	$(CYCLONE_STACK_CRYPTO_DIR)/md4.c											\
	$(CYCLONE_STACK_CRYPTO_DIR)/md5.c											\
	$(CYCLONE_STACK_CRYPTO_DIR)/mpi.c											\
	$(CYCLONE_STACK_CRYPTO_DIR)/oid.c											\
	$(CYCLONE_STACK_CRYPTO_DIR)/pem.c											\
	$(CYCLONE_STACK_CRYPTO_DIR)/pkcs5.c											\
	$(CYCLONE_STACK_CRYPTO_DIR)/rc4.c											\
	$(CYCLONE_STACK_CRYPTO_DIR)/rc6.c											\
	$(CYCLONE_STACK_CRYPTO_DIR)/ripemd128.c										\
	$(CYCLONE_STACK_CRYPTO_DIR)/ripemd160.c										\
	$(CYCLONE_STACK_CRYPTO_DIR)/rsa.c											\
	$(CYCLONE_STACK_CRYPTO_DIR)/seed.c											\
	$(CYCLONE_STACK_CRYPTO_DIR)/sha1.c											\
	$(CYCLONE_STACK_CRYPTO_DIR)/sha224.c										\
	$(CYCLONE_STACK_CRYPTO_DIR)/sha256.c										\
	$(CYCLONE_STACK_CRYPTO_DIR)/sha384.c										\
	$(CYCLONE_STACK_CRYPTO_DIR)/sha512.c										\
	$(CYCLONE_STACK_CRYPTO_DIR)/sha512_224.c									\
	$(CYCLONE_STACK_CRYPTO_DIR)/sha512_256.c									\
	$(CYCLONE_STACK_CRYPTO_DIR)/tiger.c											\
	$(CYCLONE_STACK_CRYPTO_DIR)/whirlpool.c										\
	$(CYCLONE_STACK_CRYPTO_DIR)/x509.c											\
	$(CYCLONE_STACK_CRYPTO_DIR)/yarrow.c

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
## Include directories for general includes
EXTRAINCDIRS +=																	\
	$(CYCLONE_STACK_CRYPTO_DIR)

# Include Makefiles