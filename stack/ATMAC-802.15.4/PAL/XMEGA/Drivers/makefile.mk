#list C source files
CSRC	+=																		\
	$(PLATFORM_DIR)/Drivers/drivers.c											\
	$(PLATFORM_DIR)/Drivers/ac_driver.c											\
	$(PLATFORM_DIR)/Drivers/adc_driver.c										\
	$(PLATFORM_DIR)/Drivers/aes_driver.c										\
	$(PLATFORM_DIR)/Drivers/awex_driver.c										\
	$(PLATFORM_DIR)/Drivers/clksys_driver.c										\
	$(PLATFORM_DIR)/Drivers/dac_driver.c										\
	$(PLATFORM_DIR)/Drivers/dma_driver.c										\
	$(PLATFORM_DIR)/Drivers/eeprom_driver.c										\
	$(PLATFORM_DIR)/Drivers/event_system_driver.c								\
	$(PLATFORM_DIR)/Drivers/hires_driver.c										\
	$(PLATFORM_DIR)/Drivers/ir_driver.c											\
	$(PLATFORM_DIR)/Drivers/pmic_driver.c										\
	$(PLATFORM_DIR)/Drivers/port_driver.c										\
	$(PLATFORM_DIR)/Drivers/qdec_driver.c										\
	$(PLATFORM_DIR)/Drivers/spi_driver.c										\
	$(PLATFORM_DIR)/Drivers/tc_driver.c											\
	$(PLATFORM_DIR)/Drivers/twi_master_driver.c									\
	$(PLATFORM_DIR)/Drivers/twi_slave_driver.c									\
	$(PLATFORM_DIR)/Drivers/usart_driver.c										\
	$(PLATFORM_DIR)/Drivers/wdt_driver.c

ifeq ($(MCU), __ATxmega128A1__)

CSRC	+=																		\
	$(PLATFORM_DIR)/Drivers/ebi_driver.c

endif

ifeq ($(MCU), __ATxmega256A3__)

CSRC	+=																		\
	$(PLATFORM_DIR)/Drivers/rtc32_driver.c										\
	$(PLATFORM_DIR)/Drivers/vbat.c

else

CSRC	+=																		\
	$(PLATFORM_DIR)/Drivers/rtc_driver.c

endif

# List C++ source files here. (C dependencies are automatically generated.)
CPPSRC += 

# List Assembler source files here.
# Make them always end in a capital .S.  Files ending in a lowercase .s
# will not be considered source files but generated files (assembler
# output from the compiler), and will be deleted upon "make clean"!
# Even though the DOS/Win* filesystem matches both .s and .S the same,
# it will preserve the spelling of the filenames, and gcc itself does
# care about how the name is spelled on its command-line.
#ASRC +=																			\
	$(PLATFORM_DIR)/Drivers/des_driver_size.S									\
	$(PLATFORM_DIR)/Drivers/des_driver_speed.S									\
	$(PLATFORM_DIR)/Drivers/sp_driver.S

# List any extra directories to look for include files here.
#     Each directory must be seperated by a space.
EXTRAINCDIRS += 