# PAL makefile
###############################################################################

#list C source files
CSRC +=																			\
	$(PATH_PAL)/$(PAL_GENERIC_TYPE)/Generic/Src/pal.c							\
	$(PATH_PAL)/$(PAL_GENERIC_TYPE)/Generic/Src/pal_utils.c						\
	$(PATH_PAL)/$(PAL_GENERIC_TYPE)/Generic/Src/pal_uart.c						\
	$(PATH_PAL)/$(PAL_GENERIC_TYPE)/Generic/Src/pal_boot_flash.c				\
	$(PATH_PAL)/$(PAL_GENERIC_TYPE)/Generic/Src/pal_flash.c

CSRC +=																			\
	$(PATH_PAL)/$(PAL_GENERIC_TYPE)/Drivers/drivers.c							\
	$(PATH_PAL)/$(PAL_GENERIC_TYPE)/Drivers/twim_driver.c						\
	$(PATH_PAL)/$(PAL_GENERIC_TYPE)/Drivers/rs485_driver.c

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
## Include directories for PAL
EXTRAINCDIRS +=																	\
	$(PATH_PAL)/Inc/															\
	$(PATH_PAL)/$(PAL_GENERIC_TYPE)/Generic/Inc									\
	$(PATH_PAL)/$(PAL_GENERIC_TYPE)/Drivers

# Include Makefiles
include																			\
	$(PATH_PAL)/$(PAL_GENERIC_TYPE)/$(PAL_TYPE)/makefile.mk