CYCLONE_STACK_DEMO_COMMON_FREERTOS_PORTABLE_DIR = $(CYCLONE_STACK_DEMO_COMMON_FREERTOS_DIR)/portable

#list C source files
###############################################################################

CSRC	+=																		\
	$(CYCLONE_STACK_DEMO_COMMON_FREERTOS_DIR)/croutine.c						\
	$(CYCLONE_STACK_DEMO_COMMON_FREERTOS_DIR)/event_groups.c					\
	$(CYCLONE_STACK_DEMO_COMMON_FREERTOS_DIR)/list.c							\
	$(CYCLONE_STACK_DEMO_COMMON_FREERTOS_DIR)/queue.c							\
	$(CYCLONE_STACK_DEMO_COMMON_FREERTOS_DIR)/tasks.c							\
	$(CYCLONE_STACK_DEMO_COMMON_FREERTOS_DIR)/timers.c

#list C portable source files
###############################################################################

ifneq (, $(filter $(PLATFORM),sam))
	ifneq (, $(filter $(ARMTYPE),SAM4E))
		CYCLONE_STACK_DEMO_COMMON_FREERTOS_PORTABLE_DEV_DIR = $(CYCLONE_STACK_DEMO_COMMON_FREERTOS_PORTABLE_DIR)/gcc/arm_cm4f
	endif
endif

ifdef CYCLONE_STACK_DEMO_COMMON_FREERTOS_PORTABLE_DEV_DIR
	CSRC	+=																	\
		$(CYCLONE_STACK_DEMO_COMMON_FREERTOS_PORTABLE_DEV_DIR)/port.c

	ifneq (, $(filter $(CYCLONE_RTOS_HEAP),1 2 3 4 5))
		CSRC	+=																\
			$(CYCLONE_STACK_DEMO_COMMON_FREERTOS_PORTABLE_DIR)/memmang/heap_$(CYCLONE_RTOS_HEAP).c
	else
		$(error FreeRTOS must use one memory model. Please, define the variable 'CYCLONE_RTOS_HEAP' with values in range of 1 to 5)
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
	$(CYCLONE_STACK_DEMO_COMMON_FREERTOS_DIR)/include							\
	$(CYCLONE_STACK_DEMO_COMMON_FREERTOS_PORTABLE_DEV_DIR)
