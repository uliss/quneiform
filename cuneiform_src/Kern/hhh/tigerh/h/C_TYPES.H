/*
Copyright (c) 1993-2008, Cognitive Technologies
All rights reserved.

Разрешается повторное распространение и использование как в виде исходного кода,
так и в двоичной форме, с изменениями или без, при соблюдении следующих условий:

      * При повторном распространении исходного кода должны оставаться указанное
        выше уведомление об авторском праве, этот список условий и последующий 
        отказ от гарантий. 
      * При повторном распространении двоичного кода в документации и/или в 
        других материалах, поставляемых при распространении, должны сохраняться
        указанная выше информация об авторском праве, этот список условий и
        последующий отказ от гарантий.  
      * Ни название Cognitive Technologies, ни имена ее сотрудников не могут 
        быть использованы в качестве средства поддержки и/или продвижения 
        продуктов, основанных на этом ПО, без предварительного письменного
        разрешения. 

ЭТА ПРОГРАММА ПРЕДОСТАВЛЕНА ВЛАДЕЛЬЦАМИ АВТОРСКИХ ПРАВ И/ИЛИ ДРУГИМИ ЛИЦАМИ "КАК
ОНА ЕСТЬ" БЕЗ КАКОГО-ЛИБО ВИДА ГАРАНТИЙ, ВЫРАЖЕННЫХ ЯВНО ИЛИ ПОДРАЗУМЕВАЕМЫХ, 
ВКЛЮЧАЯ ГАРАНТИИ КОММЕРЧЕСКОЙ ЦЕННОСТИ И ПРИГОДНОСТИ ДЛЯ КОНКРЕТНОЙ ЦЕЛИ, НО НЕ
ОГРАНИЧИВАЯСЬ ИМИ. НИ ВЛАДЕЛЕЦ АВТОРСКИХ ПРАВ И НИ ОДНО ДРУГОЕ ЛИЦО, КОТОРОЕ 
МОЖЕТ ИЗМЕНЯТЬ И/ИЛИ ПОВТОРНО РАСПРОСТРАНЯТЬ ПРОГРАММУ, НИ В КОЕМ СЛУЧАЕ НЕ 
НЕСЁТ ОТВЕТСТВЕННОСТИ, ВКЛЮЧАЯ ЛЮБЫЕ ОБЩИЕ, СЛУЧАЙНЫЕ, СПЕЦИАЛЬНЫЕ ИЛИ 
ПОСЛЕДОВАВШИЕ УБЫТКИ, СВЯЗАННЫЕ С ИСПОЛЬЗОВАНИЕМ ИЛИ ПОНЕСЕННЫЕ ВСЛЕДСТВИЕ 
НЕВОЗМОЖНОСТИ ИСПОЛЬЗОВАНИЯ ПРОГРАММЫ (ВКЛЮЧАЯ ПОТЕРИ ДАННЫХ, ИЛИ ДАННЫЕ, 
СТАВШИЕ НЕГОДНЫМИ, ИЛИ УБЫТКИ И/ИЛИ ПОТЕРИ ДОХОДОВ, ПОНЕСЕННЫЕ ИЗ-ЗА ДЕЙСТВИЙ 
ТРЕТЬИХ ЛИЦ И/ИЛИ ОТКАЗА ПРОГРАММЫ РАБОТАТЬ СОВМЕСТНО С ДРУГИМИ ПРОГРАММАМИ, 
НО НЕ ОГРАНИЧИВАЯСЬ ЭТИМИ СЛУЧАЯМИ), НО НЕ ОГРАНИЧИВАЯСЬ ИМИ, ДАЖЕ ЕСЛИ ТАКОЙ 
ВЛАДЕЛЕЦ ИЛИ ДРУГОЕ ЛИЦО БЫЛИ ИЗВЕЩЕНЫ О ВОЗМОЖНОСТИ ТАКИХ УБЫТКОВ И ПОТЕРЬ.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.
    * Neither the name of the Cognitive Technologies nor the names of its
      contributors may be used to endorse or promote products derived from this
      software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE 
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*
  ЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫ
  ЫЫ                                                              ЫЫ
  ЫЫ     Copyright (C) 1990 Cognitive Technology Corporation.     ЫЫ
  ЫЫ	 All rights reserved. This program is proprietary and     ЫЫ
  ЫЫ     a trade secret of Cognitive Technology Corporation.      ЫЫ
  ЫЫ                                                              ЫЫ
  ЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫ
*/

/********************************************************************/
/*                                                                  */
/*	       Basis data types and functions definitions           */
/*		for 32-bit flat WATCOM model aplications            */
/*                                                                  */
/********************************************************************/

#ifndef	TYPES_H_DEFINE
 #define TYPES_H_DEFINE
 #include "c_trans.h"
 #include "tech.h"

/*--------------------- Include standart h-files -----------------------*/

 #include <stdio.h>
 #include <string.h>

#include "cttypes.h"

#if 0
 #ifndef ___WIN386_INCLUDED__

  #ifdef _IBMC_
   #define near
   #define far
  #endif

  #ifdef __MAC__
   #define near
   #define far
  #endif

  #ifdef  PC_MS
   #define near
   #define far
  #endif
  
  #pragma warning( disable : 4244 4245 4018 ) 
  #pragma warning( disable : 4761 4305 )

/*----------------- Signed and unsigned integer types ------------------*/

/*			Byte size data                                */

  typedef signed char           CHAR;
  typedef unsigned char         BYTE;
  typedef signed char near *	PCHAR;
  typedef unsigned char near *	PBYTE;
  typedef unsigned char near *	PSTR;

/*			Word size data                          */

  typedef signed short int		    INT;
  typedef unsigned short int		WORD;
  typedef signed short int near *	PINT;
  typedef unsigned short int near *	PWORD;

/*			Boolean data                            */
  typedef INT             BOOL;
  #ifndef TRUE
   #define TRUE 1
  #endif
  #ifndef FALSE
   #define FALSE 0
  #endif

/*			Double word size data                      */

  typedef signed long int			LONG;
  typedef unsigned long int		    DWORD;
  typedef signed long int near *	PLONG;
  typedef unsigned long int near *	PDWORD;

/*								 */
 #else


/*			Byte size data */

  typedef signed char		CHAR;
  typedef signed char *		PCHAR;

/*			Word size data */

  typedef signed short int		INT;

/*			Float types */

  typedef float		    FLOAT;
  typedef double		DOUBLE;
  typedef float *       PFLOAT;
  typedef double *	    PDOUBLE;

#endif


/*--------------- Allocation memory stuff ------------------------------*/

 #define CALLOC_HEADER_SIZE    40	/* size of header before each*/
					/*   allocated item*/
 #define CALLOC_CLUSTER_SIZE   0x1000	/* minimal allocated memory size*/

//#define alloc_memory(a)	      malloc(a)	/* allocation emulation           */
 #define alloc_memory(a)	      TigerAllocateMemory(a)	// allocation emulation
 #define free_memory(a)	          TigerFreeMemory(a)	// free emulation           

 void * TigerAllocateMemory(DWORD size);
 void   TigerFreeMemory(void * mem);
 void * TigerReallocateMemory(void * mem,DWORD size);


/*----------------- Strings long prototypes ----------------------------*/

 #define MEMSET  memset
 #define MEMCPY  memcpy
 #define MEMCMP  memcmp
 #define MEMCHR	memchr
 #define STRCPY	strcpy
 #define STRCMP	strcmp
 #define STRLEN	strlen
 #define STRCHR	strchr
 #define STRCAT	strcat
 #define STRRCHR	strrchr

/*----------------- Formatting I/O prototypes ---------------------------*/

 #define PRINTF  printf
 #define SCANF   scanf
 #define SPRINTF sprintf
 #define SSCANF  sscanf

/*----------------- Math functions prototypes ---------------------------*/

 #define ABS  abs
 #define MAX  max
 #define MIN  min

/* min and max macros */
 #define max(a,b)  (((a) > (b)) ? (a) : (b))
 #define min(a,b)  (((a) < (b)) ? (a) : (b))

/*------------------------------------------------------------------------*/

 #include "typesa.h"
#endif
#endif
