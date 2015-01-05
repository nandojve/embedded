/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
*******************************************************************************/
/*******************************************************************************
* Copyright (C) 2013 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name     : bsc_userdef.c
* Device(s)     : RZ/A1H RSK+RZA1H
* H/W Platform  : RSK+RZA1H CPU Board
* Description   : Common driver (User define function)
*******************************************************************************/
/*******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 18.06.2013 1.00
*******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"                   /* Default  type definition header */
#include "dev_drv.h"                                 /* Device driver header */
#include "devdrv_common.h"                           /* Common Driver header */
#include "iodefine.h"                            /* I/O Register root header */
#include "bsc_userdef.h"                 /* Bus State Controller User header */
#include "compiler_settings.h"   /* Interchangeable compiler specific header */

/******************************************************************************
Typedef definitions
******************************************************************************/


/******************************************************************************
Macro definitions
******************************************************************************/
/* The address when writing in a SDRAM mode register */
#define SDRAM_MODE_CS2    (*(volatile uint16_t *)(0x3FFFD040))
#define SDRAM_MODE_CS3    (*(volatile uint16_t *)(0x3FFFE040))

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/


/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/


/******************************************************************************
Private global variables and functions
******************************************************************************/


/******************************************************************************
* Function Name: Userdef_BSC_CS0Init
* Description  : This is the user-defined function called by the R_BSC_Init 
*              : function. The setting for initialization of the BSC in the CS0 
*              : space is required. In this sample code, the setting to use 
*              : the NOR flash memory in the CS0 space is executed. Sets the BSC 
*              : to connect the Spansion NOR flash memory S29GL512S10T to the 
*              : CS0 space with 16-bit bus width.
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_BSC_CS0Init(void)
{
    /* ---- CS0BCR settings ---- */
    BSC.CS0BCR.LONG = 0x10000C00uL;
                                    /* Idle Cycles between Write-read Cycles    */
                                    /*  and Write-write Cycles : 1 idle cycle   */
                                    /* Data Bus Size: 16-bit                    */

    /* ---- CS0WCR settings ----  */
    BSC.CS0WCR.NORMAL.LONG = 0x00000B40uL;
                                    /* Number of Delay Cycles from Address,     */
                                    /*  CS0# Assertion to RD#,WEn Assertion     */
                                    /*  : 1.5 cycles                            */
                                    /* Number of Access Wait Cycles: 6 cycles   */
                                    /* Delay Cycles from RD,WEn# negation to    */
                                    /*  Address,CSn# negation: 0.5 cycles       */
}

/******************************************************************************
* Function Name: Userdef_BSC_CS1Init
* Description  : This is the user-defined function called by the R_BSC_Init 
*              : function. The setting for initialization of the BSC in the CS1 
*              : space is required. In this sample code, the setting to use 
*              : the NOR flash memory in the CS1 space is executed. Sets the BSC 
*              : to connect the Spansion NOR flash memory S29GL512S10T to the 
*              : CS1 space with 16-bit bus width.
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_BSC_CS1Init(void)
{
    /* ---- CS1BCR settings ---- */
    BSC.CS1BCR.LONG = 0x10000C00uL;
                                    /* Idle Cycles between Write-read Cycles    */
                                    /*  and Write-write Cycles : 1 idle cycle   */
                                    /* Data Bus Size: 16-bit                    */

    /* ---- CS1WCR settings ----  */
    BSC.CS1WCR.LONG = 0x00000B40uL;
                                    /* Number of Delay Cycles from Address,     */
                                    /*  CS1# Assertion to RD#,WEn Assertion     */
                                    /*  : 1.5 cycles                            */
                                    /* Number of Access Wait Cycles: 6 cycles   */
                                    /* Delay Cycles from RD,WEn# negation to    */
                                    /*  Address,CSn# negation: 0.5 cycles       */
}

/******************************************************************************
* Function Name: Userdef_BSC_CS2Init
* Description  : This is the user-defined function called by the R_BSC_Init
*              : function. The setting for initialisation of the BSC in CS2 
*              : area is required. In this sample code, the setting to use
*              : the SDRAM in the CS2 and CS3 space is executed as per the 
*              : note below. The function sets the BSC to connect the Micron 
*              : MT48LC16M16A2P-75 to the CS2 space with 16-bit bus width. 
*              : It assumes a second (not fitted) SDRAM device in CS3 area.
*
*         Note : This configuration is invalid for a single SDRAM and is a 
*              : known limitation of the RSK+ board. The port pin used by 
*              : CS3 is configured for LED0. To allow SDRAM operation CS2 
*              : and CS3 must be configured to SDRAM. Option link R164 must
*              : NOT be fitted to avoid a Data Bus conflict on the SDRAM 
*              : and expansion buffers. In a new application with one SDRAM 
*              : always connect the SDRAM to CS3. On this RSK+ CS3 can not 
*              : be used in another configuration including the expansion 
*              : headers unless the SDRAM is completely disabled. For other 
*              : external memory mapped devices CS1 is available for use 
*              : with the expansion headers.
*              : See the hardware manual Bus State Controller 
*              : section 8.4.3 CS2WCR(SDRAM)
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_BSC_CS2Init(void)
{
    /* ==== CS2BCR settings ==== */
    /* Idle Cycles between Write-read Cycles  */
    /* and Write-write Cycles : 0 idle cycles */
    /* Memory type :SDRAM                     */
    /* Data Bus Size : 16-bit                 */
    BSC.CS2BCR.LONG = 0x00004C00ul;

    /* ==== CS3BCR settings ==== */
    /* SDRAM WORKAROUND - see Note */
    /* Idle Cycles between Write-read Cycles  */
    /* and Write-write Cycles : 0 idle cycles */
    /* Memory type :SDRAM                     */
    /* Data Bus Size : 16-bit                 */
    BSC.CS3BCR.LONG = 0x00004C00ul;

    /* ==== CS2/3WCR settings ==== */
    /* Precharge completion wait cycles: 1 cycle     */
    /* Wait cycles between ACTV command              */
    /* and READ(A)/WRITE(A) command : 1 cycles       */
    /* CAS latency for Area 3 : 2 cycles             */
    /* Auto-precharge startup wait cycles : 2 cycles */
    /* Idle cycles from REF command/self-refresh     */
    /* Release to ACTV/REF/MRS command : 5 cycles    */
    BSC.CS3WCR.SDRAM.LONG = 0x00004492ul;

    /* SDRAM WORKAROUND - see Note */
    BSC.CS2WCR.SDRAM.LONG = 0x00000480ul;

    /* ==== SDCR settings ==== */
    /* SDRAM WORKAROUND - see Note*/
    /* Row address for Area 2 : 13-bit    */
    /* Column Address for Area 2 : 9-bit  */
    /* Refresh Control :Refresh           */
    /* RMODE :Auto-refresh is performed   */
    /* BACTV :Auto-precharge mode         */
    /* Row address for Area 3 : 13-bit    */
    /* Column Address for Area 3 : 9-bit  */
    BSC.SDCR.LONG = 0x00110811ul;

    /* ==== RTCOR settings ==== */
    /* 7.64usec / 240nsec              */
    /*   = 128(0x80)cycles per refresh */
    BSC.RTCOR.LONG = 0xA55A0080ul;

    /* ==== RTCSR settings ==== */
    /* Initialization sequence start */
    /* Clock select B-phy/4          */
    /* Refresh count :Once           */
    BSC.RTCSR.LONG = 0xA55A0008ul;

    /* ==== SDRAM Mode Register ==== */
    /* Burst read (burst length 1)./Burst write */
    SDRAM_MODE_CS2 = 0;

    /* SDRAM WORKAROUND - see Note */
    SDRAM_MODE_CS3 = 0;
}

/******************************************************************************
* Function Name: Userdef_BSC_CS3Init
* Description  : This is the user-defined function called by the R_BSC_Init
*              : function. The setting for initialization of the BSC in the CS3
*              : space is required. In this sample code, the setting to use
*              : the SDRAM in the CS3 space is executed. Sets the BSC to 
*              : connect the ISSI IS42S16320B-75 to the CS3 space with 16-bit
*              : bus width.
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_BSC_CS3Init(void)
{
    /* SDRAM WORKAROUND - see Note Userdef_BSC_CS2Init */
}

/******************************************************************************
* Function Name: Userdef_BSC_CS4Init
* Description  : This is the user-defined function called by the R_BSC_Init
*              : function. The setting for initialization of the CS4 space is 
*              : required. 
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_BSC_CS4Init(void)
{
    /* Add processing when using CS4 space */
}

/******************************************************************************
* Function Name: Userdef_BSC_CS5Init
* Description  : This is the user-defined function called by the R_BSC_Init
*              : function The setting for initialization of the CS5 space is 
*              : required.
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_BSC_CS5Init(void)
{
    /* Add processing when using CS5 space */
}


/* End of File */

