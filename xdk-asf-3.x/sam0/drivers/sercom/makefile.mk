ASF_DRIVERS_SERCOM_DIR			= $(ASF_DRIVERS_DIR)/sercom

ifneq (, $(filter $(ASF_SERCOM_DRIVER),1))

#list C source files
CSRC	+=																		\
	$(ASF_DRIVERS_SERCOM_DIR)/sercom.c											\
	$(ASF_DRIVERS_SERCOM_DIR)/sercom_interrupt.c

#	$(ASF_DRIVERS_SERCOM_DIR)/i2c/i2c_master.c									\
	$(ASF_DRIVERS_SERCOM_DIR)/i2c/i2c_master_interrupt.c						\
	$(ASF_DRIVERS_SERCOM_DIR)/i2c/i2c_slave.c									\
	$(ASF_DRIVERS_SERCOM_DIR)/i2c/i2c_slave_interrupt.c							\
	$(ASF_DRIVERS_SERCOM_DIR)/spi/spi.c											\
	$(ASF_DRIVERS_SERCOM_DIR)/spi/spi_interrupt.c								\
#
CSRC	+=																		\
	$(ASF_DRIVERS_SERCOM_DIR)/usart/usart.c										\
	$(ASF_DRIVERS_SERCOM_DIR)/usart/usart_interrupt.c

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
	$(ASF_DRIVERS_SERCOM_DIR)													\
	$(ASF_DRIVERS_SERCOM_DIR)/i2c												\
	$(ASF_DRIVERS_SERCOM_DIR)/spi												\
	$(ASF_DRIVERS_SERCOM_DIR)/usart

endif