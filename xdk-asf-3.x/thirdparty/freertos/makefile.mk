ASF_THIRDPARTY_FREERTOS_SOURCE_DIR		= $(ASF_THIRDPARTY_FREERTOS_DIR)/freertos-$(FREERTOS_VERSION)/source
ASF_THIRDPARTY_FREERTOS_PORTABLE_DIR	= $(ASF_THIRDPARTY_FREERTOS_SOURCE_DIR)/portable/gcc

#list C source files
###############################################################################

ifneq (, $(filter $(FREERTOS_VERSION),7.3.0))
CSRC	+=																		\
	$(ASF_THIRDPARTY_FREERTOS_SOURCE_DIR)/FreeRTOS_CLI.c
else
CSRC	+=																		\
	$(ASF_THIRDPARTY_FREERTOS_SOURCE_DIR)/croutine.c
endif

CSRC	+=																		\
	$(ASF_THIRDPARTY_FREERTOS_SOURCE_DIR)/list.c								\
	$(ASF_THIRDPARTY_FREERTOS_SOURCE_DIR)/queue.c								\
	$(ASF_THIRDPARTY_FREERTOS_SOURCE_DIR)/tasks.c								\
	$(ASF_THIRDPARTY_FREERTOS_SOURCE_DIR)/timers.c

ifneq (, $(filter $(FREERTOS_VERSION),8.0.1 8.1.2))
CSRC	+=																		\
	$(ASF_THIRDPARTY_FREERTOS_SOURCE_DIR)/event_groups.c
endif

#list C portable source files
###############################################################################

ifneq (, $(filter $(PLATFORM),uc3))
	ifneq (, $(filter $(FREERTOS_VERSION),7.0.0 8.1.2))
		ASF_THIRDPARTY_FREERTOS_PORTABLE_DEV_DIR = $(ASF_THIRDPARTY_FREERTOS_PORTABLE_DIR)/avr32_uc3
	endif
endif

ifneq (, $(filter $(PLATFORM),sam))
	ifneq (, $(filter $(FREERTOS_VERSION),7.0.0 7.3.0))
		ifneq (, $(filter $(ARMTYPE),SAM4E))
			ASF_THIRDPARTY_FREERTOS_PORTABLE_DEV_DIR = $(ASF_THIRDPARTY_FREERTOS_PORTABLE_DIR)/sam_cm4f
		endif
		ifneq (, $(filter $(ARMTYPE),SAM4S))
			ASF_THIRDPARTY_FREERTOS_PORTABLE_DEV_DIR = $(ASF_THIRDPARTY_FREERTOS_PORTABLE_DIR)/sam
		endif
	endif
	ifneq (, $(filter $(FREERTOS_VERSION),8.1.2))
		ASF_THIRDPARTY_FREERTOS_PORTABLE_DEV_DIR = $(ASF_THIRDPARTY_FREERTOS_PORTABLE_DIR)/ARM_CM4F
	endif
endif

ifneq (, $(filter $(PLATFORM),sam0))
	ifneq (, $(filter $(FREERTOS_VERSION),7.4.2 7.5.2 8.0.1 8.1.2))
		ASF_THIRDPARTY_FREERTOS_PORTABLE_DEV_DIR = $(ASF_THIRDPARTY_FREERTOS_PORTABLE_DIR)/ARM_CM0
	endif
endif

ifdef ASF_THIRDPARTY_FREERTOS_PORTABLE_DEV_DIR
	CSRC	+=																	\
		$(ASF_THIRDPARTY_FREERTOS_PORTABLE_DEV_DIR)/port.c

	ifneq (, $(filter $(FREERTOS_HEAP),1 2 3 4))
		CSRC	+=																\
			$(ASF_THIRDPARTY_FREERTOS_SOURCE_DIR)/portable/memmang/heap_$(FREERTOS_HEAP).c
	endif

	ifneq (, $(filter $(PLATFORM),uc3))
		CSRC	+=																\
				$(ASF_THIRDPARTY_FREERTOS_PORTABLE_DEV_DIR)/read.c				\
				$(ASF_THIRDPARTY_FREERTOS_PORTABLE_DEV_DIR)/write.c

		ASRC	+= $(ASF_THIRDPARTY_FREERTOS_PORTABLE_DEV_DIR)/exception.S
	endif
else
	ifdef ARMTYPE
		$(error Platform $(PLATFORM) with ARM TYPE $(ARMTYPE) is not ported to FreeRTOS version $(FREERTOS_VERSION))
	else
		$(error Platform $(PLATFORM) is not ported to FreeRTOS version $(FREERTOS_VERSION))
	endif
endif

# List C++ source files here. (C dependencies are automatically generated.)
###############################################################################
CPPSRC += 

###############################################################################
# List Assembler source files here.
# Make them always end in a capital .S.  Files ending in a lowercase .s
# will not be considered source files but generated files (assembler
# output from the compiler), and will be deleted upon "make clean"!
# Even though the DOS/Win* filesystem matches both .s and .S the same,
# it will preserve the spelling of the filenames, and gcc itself does
# care about how the name is spelled on its command-line.
ASRC +=

###############################################################################
# List any extra directories to look for include files here.
#     Each directory must be seperated by a space.
EXTRAINCDIRS +=																	\
	$(ASF_THIRDPARTY_FREERTOS_SOURCE_DIR)/include								\
	$(ASF_THIRDPARTY_FREERTOS_PORTABLE_DEV_DIR)
