# Generic Device Driver
###############################################################################

ifneq (, $(filter $(DEVICES),AT24C0x AT24MACxxx AT30TSE75x))
	CSRC +=	$(DEVICES_DIR)/AT24C0x/AT24C0x.c
	EXTRAINCDIRS += $(DEVICES_DIR)/AT24C0x
endif
ifneq (, $(filter $(DEVICES),AT24MACxxx))
	CSRC +=	$(DEVICES_DIR)/AT24MACxxx/AT24MAC602.c
	EXTRAINCDIRS += $(DEVICES_DIR)/AT24MACxxx
endif
ifneq (, $(filter $(DEVICES),AT30TSE75x))
	CSRC +=	$(DEVICES_DIR)/AT30TSE75x/AT30TSE752.c
	EXTRAINCDIRS += $(DEVICES_DIR)/AT30TSE75x
endif
ifneq (, $(filter $(DEVICES),CC2D33S))
	CSRC +=	$(DEVICES_DIR)/CC2D33S/CC2D33S.c
	EXTRAINCDIRS += $(DEVICES_DIR)/CC2D33S
endif
ifneq (, $(filter $(DEVICES),MPL115A2))
	CSRC +=	$(DEVICES_DIR)/MPL115A2/MPL115A2.c
	EXTRAINCDIRS += $(DEVICES_DIR)/MPL115A2
endif
ifneq (, $(filter $(DEVICES),VCNL40xx))
	CSRC +=	$(DEVICES_DIR)/VCNL40xx/VCNL40xx.c
	EXTRAINCDIRS += $(DEVICES_DIR)/VCNL40xx
endif