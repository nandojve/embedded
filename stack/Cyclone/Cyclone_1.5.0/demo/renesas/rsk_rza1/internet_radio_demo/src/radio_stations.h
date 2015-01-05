/**
 * @file radio_stations.h
 * @brief List of Internet radio station
 *
 * @section License
 *
 * Copyright (C) 2010-2013 Oryx Embedded. All rights reserved.
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
 * @version 1.3.5
 **/

#ifndef _RADIO_STATIONS_H
#define _RADIO_STATIONS_H

//RTL
#if 1
   //Icecast server name
   #define ICECAST_SERVER_NAME "streaming.radio.rtl.fr"
   //Icecast server port
   #define ICECAST_SERVER_PORT 80
   //Requested recource
   #define ICECAST_RESOURCE "/rtl-1-44-96"
#endif

//RTL2
#if 0
   //Icecast server name
   #define ICECAST_SERVER_NAME "streaming.radio.rtl2.fr"
   //Icecast server port
   #define ICECAST_SERVER_PORT 80
   //Requested recource
   #define ICECAST_RESOURCE "/rtl2-1-44-96"
#endif

//OUI FM
#if 0
   //Icecast server name
   #define ICECAST_SERVER_NAME "ouifm.ice.infomaniak.ch"
   //Icecast server port
   #define ICECAST_SERVER_PORT 80
   //Requested recource
   #define ICECAST_RESOURCE "/ouifm-high.mp3"
#endif

//Le Mouv'
#if 0
   //Icecast server name
   #define ICECAST_SERVER_NAME "mp3lg-out-b.tdf-cdn.com"
   //Icecast server port
   #define ICECAST_SERVER_PORT 80
   //Requested recource
   #define ICECAST_RESOURCE "/lemouv/all/lemouvhautdebit.mp3"
#endif

//France Info
#if 0
   //Icecast server name
   #define ICECAST_SERVER_NAME "mp3lg-out-b.tdf-cdn.com"
   //Icecast server port
   #define ICECAST_SERVER_PORT 80
   //Requested recource
   #define ICECAST_RESOURCE "/franceinter/all/franceinterhautdebit.mp3"
#endif

//FIP
#if 0
   //Icecast server name
   #define ICECAST_SERVER_NAME "mp3lg-out-b.tdf-cdn.com"
   //Icecast server port
   #define ICECAST_SERVER_PORT 80
   //Requested recource
   #define ICECAST_RESOURCE "/fip/all/fiphautdebit.mp3"
#endif

//Couleur 3
#if 0
   //Icecast server name
   #define ICECAST_SERVER_NAME "streaming.swisstxt.ch"
   //Icecast server port
   #define ICECAST_SERVER_PORT 80
   //Requested recource
   #define ICECAST_RESOURCE "/m/couleur3/mp3_128"
#endif

//Alpes 1
#if 0
   //Icecast server name
   #define ICECAST_SERVER_NAME "alpes1grenoble.ice.infomaniak.ch"
   //Icecast server port
   #define ICECAST_SERVER_PORT 80
   //Requested recource
   #define ICECAST_RESOURCE "/alpes1grenoble-high.mp3"
#endif

//Alouette Radio
#if 0
   //Icecast server name
   #define ICECAST_SERVER_NAME "broadcast.infomaniak.net"
   //Icecast server port
   #define ICECAST_SERVER_PORT 80
   //Requested recource
   #define ICECAST_RESOURCE "/alouette-high.mp3"
#endif

//Radio Classique
#if 0
   //Icecast server name
   #define ICECAST_SERVER_NAME "str20.creacast.com"
   //Icecast server port
   #define ICECAST_SERVER_PORT 80
   //Requested recource
   #define ICECAST_RESOURCE "/classique1"
#endif

//Radio Nova
#if 0
   //Icecast server name
   #define ICECAST_SERVER_NAME "ice3.infomaniak.ch"
   //Icecast server port
   #define ICECAST_SERVER_PORT 80
   //Requested recource
   #define ICECAST_RESOURCE "/radionova-high.mp3"
#endif

#endif
