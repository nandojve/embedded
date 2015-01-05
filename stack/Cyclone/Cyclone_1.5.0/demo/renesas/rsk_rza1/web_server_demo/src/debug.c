/**
 * @file debug.c
 * @brief Debugging facilities
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
#include "iodefine.h"
#include "scif_iobitmask.h"
#include "debug.h"

//Peripheral clock 1 (66.67 MHz)
#define PCLK1_HZ (400000000 / 6)


/**
 * @brief Debug UART initialization
 * @param[in] baudrate UART baudrate
 **/

void debugInit(uint32_t baudrate)
{
   uint32_t status;

   //Enable SCIF2 peripheral clock
   CPG.STBCR4.BIT.MSTP45 = 0;
   //Disable SCIF2 module
   SCIF2.SCSCR.WORD = 0;

   //Reset transmit and receive FIFO
   SCIF2.SCFCR.WORD |= SCIF2_SCFCR_TFRST | SCIF2_SCFCR_RFRST;

   //Read serial status register
   status = SCIF2.SCFSR.WORD;
   //Clear ER, BRK and DR flags
   SCIF2.SCFSR.WORD = status & ~(SCIF2_SCFSR_ER | SCIF2_SCFSR_BRK | SCIF2_SCFSR_DR);

   //Read line status register
   status = SCIF3.SCLSR.WORD;
   //Clear ORER flag
   SCIF2.SCLSR.WORD = status & ~SCIF2_SCLSR_ORER;

   //Select internal clock
   SCIF2.SCSCR.WORD &= ~SCIF2_SCSCR_CKE;

   //Configure UART (8 bits, no parity, 1 stop bit)
   SCIF2.SCSMR.WORD = 0;

   //Enable double-speed mode
   SCIF2.SCEMR.WORD = SCIF2_SCEMR_BGDM;

   //Set baudrate
   SCIF2.SCBRR.BYTE = PCLK1_HZ / (16 * baudrate) - 1;

   //Configure transmit and receive FIFO triggers
   SCIF2.SCFCR.WORD = 0;

   //Clear TFRST and RFRST bits
   SCIF3.SCFCR.WORD &= ~(SCIF2_SCFCR_TFRST | SCIF2_SCFCR_RFRST);

   //Configure TXD2 (P3_0)
   PORT3.PMCn.BIT.PMCn0 = 1;
   PORT3.PFCn.BIT.PFCn0 = 1;
   PORT3.PFCEn.BIT.PFCEn0 = 0;
   PORT3.PFCAEn.BIT.PFCAEn0 = 1;
   PORT3.PIPCn.BIT.PIPCn0 = 1;

   //Configure RXD2 (P3_2)
   PORT3.PMCn.BIT.PMCn2 = 1;
   PORT3.PFCn.BIT.PFCn2 = 1;
   PORT3.PFCEn.BIT.PFCEn2 = 1;
   PORT3.PFCAEn.BIT.PFCAEn2 = 0;
   PORT3.PIPCn.BIT.PIPCn2 = 1;

   //Enable transmission and reception
   SCIF2.SCSCR.WORD = SCIF2_SCSCR_TE | SCIF2_SCSCR_RE;
}


/**
 * @brief Display the contents of an array
 * @param[in] stream Pointer to a FILE object that identifies an output stream
 * @param[in] prepend String to prepend to the left of each line
 * @param[in] data Pointer to the data array
 * @param[in] length Number of bytes to display
 **/

void debugDisplayArray(FILE *stream,
   const char_t *prepend, const void *data, size_t length)
{
   uint_t i;

   for(i = 0; i < length; i++)
   {
      //Beginning of a new line?
      if((i % 16) == 0)
         fprintf(stream, "%s", prepend);
      //Display current data byte
      fprintf(stream, "%02" PRIX8 " ", *((uint8_t *) data + i));
      //End of current line?
      if((i % 16) == 15 || i == (length - 1))
         fprintf(stream, "\r\n");
   }
}


/**
 * @brief Write character to stream
 * @param[in] c The character to be written
 * @param[in] Pointer to a FILE object that identifies an output stream
 * @return On success, the character written is returned. If a writing
 *   error occurs, EOF is returned
 **/

int_t fputc(int_t c, FILE *stream)
{
   //Standard output?
   if(stream == stdout)
   {
      //Display current character
      lcdPutChar(c);

      //On success, the character written is returned
      return c;
   }
   //Standard error output?
   else if(stream == stderr)
   {
      //Wait for the transmit FIFO to be available for writing
      while(!(SCIF2.SCFSR.WORD & SCIF2_SCFSR_TDFE));
      //Send character
      SCIF2.SCFTDR.BYTE = c;
      //Clear TDFE flag
      SCIF2.SCFSR.WORD &= ~SCIF2_SCFSR_TDFE;

      //On success, the character written is returned
      return c;
   }
   //Unknown output?
   else
   {
      //If a writing error occurs, EOF is returned
      return EOF;
   }
}
