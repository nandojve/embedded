/**
 * @file ssif0.c
 * @brief SSIF (Serial Sound Interface)
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
#include "ssif0.h"
#include "debug.h"


/**
 * @brief Initialize SSIF0 interface
 **/

void ssif0Init(void)
{
   //Debug message
   TRACE_INFO("Initializing SSIF0 interface...\r\n");

   //Enable SSIF0 peripheral clock
   CPG.STBCR11.BIT.MSTP115 = 0;

   //SSIF0 software reset
   CPG.SWRSTCR1.BIT.SRST16 = 1;
   CPG.SWRSTCR1.BIT.SRST16 = 0;

   //Configure SSISCK0 (P4_4)
   PORT4.PMCn.BIT.PMCn4 = 1;
   PORT4.PFCn.BIT.PFCn4 = 0;
   PORT4.PFCEn.BIT.PFCEn4 = 0;
   PORT4.PFCAEn.BIT.PFCAEn4 = 1;
   PORT4.PIPCn.BIT.PIPCn4 = 1;

   //Configure SSIWS0 (P4_5)
   PORT4.PMCn.BIT.PMCn5 = 1;
   PORT4.PFCn.BIT.PFCn5 = 0;
   PORT4.PFCEn.BIT.PFCEn5 = 0;
   PORT4.PFCAEn.BIT.PFCAEn5 = 1;
   PORT4.PIPCn.BIT.PIPCn5 = 1;

   //Configure SSIRXD0 (P4_6)
   PORT4.PMCn.BIT.PMCn6 = 1;
   PORT4.PFCn.BIT.PFCn6 = 0;
   PORT4.PFCEn.BIT.PFCEn6 = 0;
   PORT4.PFCAEn.BIT.PFCAEn6 = 1;
   PORT4.PIPCn.BIT.PIPCn6 = 1;

   //Configure SSITXD0 (P4_7)
   PORT4.PMCn.BIT.PMCn7 = 1;
   PORT4.PFCn.BIT.PFCn7 = 0;
   PORT4.PFCEn.BIT.PFCEn7 = 0;
   PORT4.PFCAEn.BIT.PFCAEn7 = 1;
   PORT4.PIPCn.BIT.PIPCn7 = 1;

   //Clock source for oversampling (AUDIO_X1)
   SSIF0.SSICR.BIT.CKS = 0;
   //Number of channels per system word (1 channel)
   SSIF0.SSICR.BIT.CHNL = 0;
   //Number of bits in a data word (16 bits)
   SSIF0.SSICR.BIT.DWL = 1;
   //Number of bits in a system word (16 bits)
   SSIF0.SSICR.BIT.SWL = 1;
   //Serial bit clock direction (master mode)
   SSIF0.SSICR.BIT.SCKD = 1;
   //Serial WS direction (master mode)
   SSIF0.SSICR.BIT.SWSD = 1;
   //Serial bit clock polarity
   SSIF0.SSICR.BIT.SCKP = 0;
   //Serial WS polarity
   SSIF0.SSICR.BIT.SWSP = 0;
   //Serial padding polarity
   SSIF0.SSICR.BIT.SPDP = 0;
   //Serial data alignment
   SSIF0.SSICR.BIT.SDTA = 0;
   //Parallel data alignment
   SSIF0.SSICR.BIT.PDTA = 0;
   //Serial data delay
   SSIF0.SSICR.BIT.DEL = 1;
   //Serial oversampling clock division ratio (16)
   SSIF0.SSICR.BIT.CKDV = 4;
   //Disable mute
   SSIF0.SSICR.BIT.MUEN = 0;

   //Enable WS continue mode
   SSIF0.SSITDMR.BIT.CONT = 1;
   //Disable TDM mode
   SSIF0.SSITDMR.BIT.TDM = 0;

   //Reset transmit FIFO
   SSIF0.SSIFCR.BIT.TFRST = 1;
   //Transmit data trigger number (7)
   SSIF0.SSIFCR.BIT.TTRG = 0;
}


/**
 * @brief Start SSIF0 interface
 **/

void ssif0Start(void)
{
   //Take TX FIFO out of reset
   SSIF0.SSIFCR.BIT.TFRST = 0;

   //Clear error flags
   SSIF0.SSISR.BIT.TUIRQ = 0;
   SSIF0.SSISR.BIT.TOIRQ = 0;

   //Enable error interrupts
   SSIF0.SSICR.BIT.TUIEN = 1;
   SSIF0.SSICR.BIT.TOIEN = 1;

   //Enable transmit interrupt
   SSIF0.SSIFCR.BIT.TIE = 1;

   //Enable transmit operation
   SSIF0.SSICR.BIT.TEN = 1;
}


/**
 * @brief Stop SSIF0 interface
 **/

void ssif0Stop(void)
{
   //Disable transmit operation
   SSIF0.SSICR.BIT.TEN = 0;
   //Reset TX FIFO
   SSIF0.SSIFCR.BIT.TFRST = 1;
}
