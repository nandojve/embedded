$Id: IMPORTANT_README_FIRST.txt 32270 2012-06-06 11:54:38Z karthik.p_u $

ATMEL Corporation Inc.

This file contains important information for the MAC used for
802.15.4 transceivers and microcontrollers of ATMEL Corp.

For license information see file:
EULA.txt.

For
- Release and version information of the MAC
- System requirements - Supported Compiler/Tools
see file:
Doc\MAC_Release_Notes.txt.

For the AVR2025 IEEE 802.15.4 MAC Reference Manual including the API see file:
MAC_readme.html.


Installation
=============
- Unzip release file to a top level folder, like "C:\" or "<A_Drive_Letter>:\".
  In case the package is installed in another subdirectory, issues may occur
  regarding the length of file path length during compilation. Therefore make
  sure the path to the package is as short as possible. It is recommended to
  extract at the root of the drive.

- The Windows USB Driver information (.inf) and/or corresponding installation
  files for various boards can be found at "PAL\Board_Utils".

- Please ensure the toolchains and the necessary patches are installed before
  using the projects and workspaces. For required toolchains please see the
  file Doc\MAC_Release_Notes.txt

Installation instructions
=========================
- Install the required toolchains as specified in Doc\MAC_Release_Notes.txt
- Patching procedure for the toolchain is given in following sections

For installing patch AVRStudio-5.1-part-pack-RF-509 (support for ATmega256RFR2 device):
---------------------------------------------------------------------------------------
- After the installation of `AVRStudio-5.1-part-pack-RF-509`:
    - Open C:\Program Files\Atmel\AVR Studio 5.1\extensions\Atmel\AVRGCC\3.3.1\AVRToolchain
    - Copy the directory `avr`
    - Open C:\Program Files\Atmel\AVR Studio 5.1\extensions\Atmel\AVRGCC\3.3.1.27\AVRToolchain
    - Paste the directory `avr` here. Say "Yes to all" for overwrite confirmation dialog.


For IARAVR611_RFA2_RFR2_20120206 (support for ATmega256RFR2 device):
--------------------------------------------------------------------
- For patching `IAR Embedded Workbench for AVR 6.11`, please do the following:
    - Unzip IARAVR611_RFA2_RFR2_20120206.zip to some location (here assume C:\My_IAR_Patch)
    - Open C:\My_IAR_Patch
    - Copy the directory `avr`
    - Open C:\Program Files\IAR Systems\Embedded Workbench for AVR 6.11\
    - Paste the directory `avr` here. Say "Yes to all" for overwrite confirmation.

NOTE:
=====
Before running the batch files available in `Build\Build_MAC` directory,
please ensure that following environment variables are present/created
with appropriate values. For example:

avr32_iar_build		:	"C:\Program Files\IAR Systems\IAREW_AVR32\common\bin\IarBuild.exe"
arm_iar_build		:	"C:\Program Files\IAR Systems\IAREW_ARM\common\bin\IarBuild.exe"

** Values would vary if installation location is different.

Besides, PATH variable should contain the locations of other necessary executables.
The required toolcahin are listed in Doc\MAC_release_notes.txt file.

Applications
============
-  The included example applications can be built by using the corresponding
   makefiles for GCC or the corresponding IAR project files.

-  Commands are shown below. (Use without quotes).

-  Makefile:
   =========
   make clean all -f <Makefile>

   IAR Project:
   ============
   <Builder_Executable> <Project_filename>.EWP -clean <Build_configuration>

   <Builder_Executable> can be either (without quotes): "IarBuild" or
   "%arm_iar_build%" or "%avr32_iar_build%", depending on used MCU type.

   For <Build_configuration>, please refer to the IAR project file (.ewp).
