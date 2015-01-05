/**
 * @file rza1_eth.h
 * @brief Renesas RZ/A1 Ethernet MAC controller
 *
 * @section License
 *
 * Copyright (C) 2010-2014 Oryx Embedded. All rights reserved.
 *
 * This file is part of CycloneTCP Open.
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

#ifndef _RZA1_ETH_H
#define _RZA1_ETH_H

//Dependencies
#include "core/nic.h"

//Number of TX buffers
#ifndef RZA1_ETH_TX_BUFFER_COUNT
   #define RZA1_ETH_TX_BUFFER_COUNT 8
#elif (RZA1_ETH_TX_BUFFER_COUNT < 1)
   #error RZA1_ETH_TX_BUFFER_COUNT parameter is not valid
#endif

//TX buffer size
#ifndef RZA1_ETH_TX_BUFFER_SIZE
   #define RZA1_ETH_TX_BUFFER_SIZE 1536
#elif (RZA1_ETH_TX_BUFFER_SIZE != 1536)
   #error RZA1_ETH_TX_BUFFER_SIZE parameter is not valid
#endif

//Number of RX buffers
#ifndef RZA1_ETH_RX_BUFFER_COUNT
   #define RZA1_ETH_RX_BUFFER_COUNT 8
#elif (RZA1_ETH_RX_BUFFER_COUNT < 1)
   #error RZA1_ETH_RX_BUFFER_COUNT parameter is not valid
#endif

//RX buffer size
#ifndef RZA1_ETH_RX_BUFFER_SIZE
   #define RZA1_ETH_RX_BUFFER_SIZE 1536
#elif (RZA1_ETH_RX_BUFFER_SIZE != 1536)
   #error RZA1_ETH_RX_BUFFER_SIZE parameter is not valid
#endif

//Ethernet interrupt priority
#ifndef RZA1_ETH_IRQ_PRIORITY
   #define RZA1_ETH_IRQ_PRIORITY 25
#elif (RZA1_ETH_IRQ_PRIORITY < 0)
   #error RZA1_ETH_IRQ_PRIORITY parameter is not valid
#endif

//EESR0 register
#define EDMAC_EESR0_TWB    0xC0000000
#define EDMAC_EESR0_TC1    0x20000000
#define EDMAC_EESR0_TUC    0x10000000
#define EDMAC_EESR0_ROC    0x08000000
#define EDMAC_EESR0_TABT   0x04000000
#define EDMAC_EESR0_RABT   0x02000000
#define EDMAC_EESR0_RFCOF  0x01000000
#define EDMAC_EESR0_ECI    0x00400000
#define EDMAC_EESR0_TC0    0x00200000
#define EDMAC_EESR0_TDE    0x00100000
#define EDMAC_EESR0_TFUF   0x00080000
#define EDMAC_EESR0_FR     0x00040000
#define EDMAC_EESR0_RDE    0x00020000
#define EDMAC_EESR0_RFOF   0x00010000
#define EDMAC_EESR0_RMAF   0x00000080
#define EDMAC_EESR0_RRF    0x00000010
#define EDMAC_EESR0_RTLF   0x00000008
#define EDMAC_EESR0_RTSF   0x00000004
#define EDMAC_EESR0_PRE    0x00000002
#define EDMAC_EESR0_CERF   0x00000001

//Transmit DMA descriptor flags
#define EDMAC_TD0_TACT     0x80000000
#define EDMAC_TD0_TDLE     0x40000000
#define EDMAC_TD0_TFP_SOF  0x20000000
#define EDMAC_TD0_TFP_EOF  0x10000000
#define EDMAC_TD0_TFE      0x08000000
#define EDMAC_TD0_TWBI     0x04000000
#define EDMAC_TD0_TFS_MASK 0x00000300
#define EDMAC_TD0_TFS_TUC  0x00000200
#define EDMAC_TD0_TFS_TABT 0x00000100
#define EDMAC_TD1_TDL      0xFFFF0000
#define EDMAC_TD2_TBA      0xFFFFFFFF

//Receive DMA descriptor flags
#define EDMAC_RD0_RACT     0x80000000
#define EDMAC_RD0_RDLE     0x40000000
#define EDMAC_RD0_RFP_SOF  0x20000000
#define EDMAC_RD0_RFP_EOF  0x10000000
#define EDMAC_RD0_RFE      0x08000000
#define EDMAC_RD0_RCSE     0x04000000
#define EDMAC_RD0_RFS_MASK 0x02DF0000
#define EDMAC_RD0_RFS_RFOF 0x02000000
#define EDMAC_RD0_RFS_RMAF 0x00800000
#define EDMAC_RD0_RFS_RUAF 0x00400000
#define EDMAC_RD0_RFS_RRF  0x00100000
#define EDMAC_RD0_RFS_RTLF 0x00080000
#define EDMAC_RD0_RFS_RTSF 0x00040000
#define EDMAC_RD0_RFS_PRE  0x00020000
#define EDMAC_RD0_RFS_CERF 0x00010000
#define EDMAC_RD0_RCS      0x0000FFFF
#define EDMAC_RD1_RBL      0xFFFF0000
#define EDMAC_RD1_RDL      0x0000FFFF
#define EDMAC_RD2_RBA      0xFFFFFFFF

//Serial Management Interface
#define SMI_SYNC           0xFFFFFFFF
#define SMI_START          0x00000001
#define SMI_WRITE          0x00000001
#define SMI_READ           0x00000002
#define SMI_TA             0x00000002


/**
 * @brief Transmit DMA descriptor
 **/

typedef struct
{
   uint32_t td0;
   uint32_t td1;
   uint32_t td2;
   uint32_t padding;
} Rza1TxDmaDesc;


/**
 * @brief Receive DMA descriptor
 **/

typedef struct
{
   uint32_t rd0;
   uint32_t rd1;
   uint32_t rd2;
   uint32_t padding;
} Rza1RxDmaDesc;


//RZ/A1 Ethernet MAC driver
extern const NicDriver rza1EthDriver;

//RZ/A1 Ethernet MAC related functions
error_t rza1EthInit(NetInterface *interface);
void rza1EthInitGpio(NetInterface *interface);
void rza1EthInitDmaDesc(NetInterface *interface);

void rza1EthTick(NetInterface *interface);

void rza1EthEnableIrq(NetInterface *interface);
void rza1EthDisableIrq(NetInterface *interface);
void rza1EthIrqHandler(uint32_t intSense);
void rza1EthEventHandler(NetInterface *interface);

error_t rza1EthSetMacFilter(NetInterface *interface);

error_t rza1EthSendPacket(NetInterface *interface,
   const ChunkedBuffer *buffer, size_t offset);

error_t rza1EthReceivePacket(NetInterface *interface,
   uint8_t *buffer, size_t size, size_t *length);

void rza1EthWritePhyReg(uint8_t phyAddr, uint8_t regAddr, uint16_t data);
uint16_t rza1EthReadPhyReg(uint8_t phyAddr, uint8_t regAddr);

void rza1EthWriteSmi(uint32_t data, uint_t length);
uint32_t rza1EthReadSmi(uint_t length);

#endif
