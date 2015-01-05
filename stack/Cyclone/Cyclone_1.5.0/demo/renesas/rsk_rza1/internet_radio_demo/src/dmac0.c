/**
 * @file dmac0.c
 * @brief DMAC (Direct Access Memory Controller)
 *
 * @section License
 *
 * Copyright (C) 2010-2014 Oryx Embedded. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * @author Oryx Embedded (www.oryx-embedded.com)
 * @version 1.5.0
 **/

//Dependencies
#include <stdio.h>
#include <string.h>
#include "iodefine.h"
#include "dev_drv.h"
#include "devdrv_intc.h"
#include "dmac0.h"
#include "debug.h"

#include "max9856.h"
#include "ssif0.h"
#include "resource_manager.h"


/**
 * @brief Initialize DMAC0 controller
 **/

void dmac0Init(void)
{
   //Debug message
   TRACE_INFO("Initializing DMAC0 controller...\r\n");

   //Select DMA mode (register mode)
   DMAC0.CHCFG.BIT.DMS = 0;
   //DMA transfers are continued using the next register set
   DMAC0.CHCFG.BIT.REN = 1;
   //Invert RSEL automatically after a DMA transaction
   DMAC0.CHCFG.BIT.RSW = 1;
   //Execute the next 0 register set
   DMAC0.CHCFG.BIT.RSEL = 0;
   //Stop the DMA transfer without sweeping the buffer
   DMAC0.CHCFG.BIT.SBE = 0;
   //Mask the DMA transfer end interrupt
   DMAC0.CHCFG.BIT.DEM = 1;
   //Set DMA transfer mode (single transfer mode)
   DMAC0.CHCFG.BIT.TM = 0;
   //Destination address counting direction (fixed)
   DMAC0.CHCFG.BIT.DAD = 1;
   //Source address counting direction (increment)
   DMAC0.CHCFG.BIT.SAD = 0;
   //Destination data size (32 bits)
   DMAC0.CHCFG.BIT.DDS = 2;
   //Source data size (32 bits)
   DMAC0.CHCFG.BIT.SDS = 2;
   //Set DMAACK output mode (level mode)
   DMAC0.CHCFG.BIT.AM = 1;
   //Detect a DMA request based on the level of the signal
   DMAC0.CHCFG.BIT.LVL = 1;
   //Detect a request when the signal is at the high level
   DMAC0.CHCFG.BIT.HIEN = 1;
   //Do not detect a request even when the signal is at the low level
   DMAC0.CHCFG.BIT.LOEN = 0;
   //Request direction (DMAACK is to become active when written)
   DMAC0.CHCFG.BIT.REQD = 1;
   //Set DMAC channel (CH0/CH8)
   DMAC0.CHCFG.BIT.SEL = 0;

   //Transfer interval for the DMA channel
   DMAC0.CHITVL.LONG = 0;
   //Clear channel extension register
   DMAC0.CHEXT.LONG = 0;
   //Clear channel control register
   DMAC0.CHCTRL.LONG = 0;

   //DMA resource selectors (SSIF0 peripheral)
   DMAC01.DMARS.BIT.CH0_MID = 0x38;
   DMAC01.DMARS.BIT.CH0_RID = 0x01;

   //Use level output for the DMA transfer end interrupt
   DMAC07.DCTRL.BIT.LVINT = 1;
   //Set transfer priority control mode (fixed priority)
   DMAC07.DCTRL.BIT.PR = 0;

   //Register DMAINT0 interrupt handler
   R_INTC_RegistIntFunc(INTC_ID_DMAINT0, dmac0IrqHandler);
   //Configure DMAINT0 interrupt priority
   R_INTC_SetPriority(INTC_ID_DMAINT0, DMAC0_IRQ_PRIORITY);
}


/**
 * @brief Configure Next0 register set
 * @param[in] srcAddr Source start address
 * @param[in] destAddr Destination start address
 * @param[in] byteCount Transfer size
 **/

void dmac0SetNext0(const void *srcAddr, const void *destAddr, size_t byteCount)
{
   //Set source start address
   DMAC0.N0SA.LONG = (uint32_t) srcAddr;
   //Set destination start address
   DMAC0.N0DA.LONG = (uint32_t) destAddr;
   //Set transfer size
   DMAC0.N0TB.LONG = byteCount;
}


/**
 * @brief Configure Next1 register set
 * @param[in] srcAddr Source start address
 * @param[in] destAddr Destination start address
 * @param[in] byteCount Transfer size
 **/

void dmac0SetNext1(const void *srcAddr, const void *destAddr, size_t byteCount)
{
   //Set source start address
   DMAC0.N1SA.LONG = (uint32_t) srcAddr;
   //Set destination start address
   DMAC0.N1DA.LONG = (uint32_t) destAddr;
   //Set transfer size
   DMAC0.N1TB.LONG = byteCount;
}


/**
 * @brief Start DMAC0 controller
 **/

void dmac0Start(void)
{
   uint32_t dummy;

   //Make sure that both the EN bit and TACT bit are set to 0
   if(!DMAC0.CHSTAT.BIT.EN && !DMAC0.CHSTAT.BIT.TACT)
   {
      //Reset channel status register
      DMAC0.CHCTRL.BIT.SWRST = 1;
      //Dummy read
      dummy = DMAC0.CHCTRL.LONG;

      //DMA transfers are continued using the next register set
      DMAC0.CHCFG.BIT.REN = 1;
      //Execute the next 0 register set
      DMAC0.CHCFG.BIT.RSEL = 0;
      //An interrupt is issued when the DMA transaction is completed
      DMAC0.CHCFG.BIT.DEM = 0;

      //Enable DMA transfer
      DMAC0.CHCTRL.BIT.SETEN = 1;
   }

   //Enable DMAINT0 interrupt
   R_INTC_Enable(INTC_ID_DMAINT0);
}


/**
 * @brief Stop DMAC0 controller
 **/

void dmac0Stop(void)
{
   //Disable DMAINT0 interrupt
   R_INTC_Disable(INTC_ID_DMAINT0);

   //Check whether the DMA channel is enabled
   if(DMAC0.CHSTAT.BIT.EN)
   {
      //Suspend the current DMA transfer
      DMAC0.CHCTRL.BIT.SETSUS = 1;
      //Wait for the DMA transfer to be suspended
      while(!DMAC0.CHSTAT.BIT.SUS);

      //Stop the DMA transfer
      DMAC0.CHCTRL.BIT.CLREN = 1;
      //Wait until the DMA channel is completely inactive
      while(DMAC0.CHSTAT.BIT.TACT);
   }
}


/**
 * @brief Enable DMAC0 interrupt
 **/

void dmac0EnableIrq(void)
{
   //Enable DMAINT0 interrupt
   R_INTC_Enable(INTC_ID_DMAINT0);
}


/**
 * @brief Enable DMAC0 interrupt
 **/

void dmac0DisableIrq(void)
{
   //Disable DMAINT0 interrupt
   R_INTC_Disable(INTC_ID_DMAINT0);
}
