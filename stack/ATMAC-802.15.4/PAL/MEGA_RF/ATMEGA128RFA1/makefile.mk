# PAL MEGA128RFA1 makefile
###############################################################################

#list C source files
CSRC +=																			\
	$(PATH_PAL)/$(PAL_GENERIC_TYPE)/$(PAL_TYPE)/Src/pal_irq.c					\
	$(PATH_PAL)/$(PAL_GENERIC_TYPE)/$(PAL_TYPE)/Src/pal_mcu_generic.c			\
	$(PATH_PAL)/$(PAL_GENERIC_TYPE)/$(PAL_TYPE)/Src/pal_sio_hub.c				\
	$(PATH_PAL)/$(PAL_GENERIC_TYPE)/$(PAL_TYPE)/Src/pal_timer.c					\
	$(PATH_PAL)/$(PAL_GENERIC_TYPE)/$(PAL_TYPE)/Src/pal_usb_ftdi.c				\
	$(PATH_PAL)/$(PAL_GENERIC_TYPE)/$(PAL_TYPE)/Src/pal_lcd.c					\
	$(PATH_PAL)/$(PAL_GENERIC_TYPE)/$(PAL_TYPE)/Src/pal_nvm_multi_write.c

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
	$(PATH_PAL)/$(PAL_GENERIC_TYPE)/$(PAL_TYPE)/Inc

## Include directories for specific boards type
EXTRAINCDIRS +=																	\
	$(PATH_PAL)/$(PAL_GENERIC_TYPE)/$(PAL_TYPE)/Boards