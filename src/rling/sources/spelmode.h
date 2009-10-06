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

#ifndef __SPEL_MODE_H__
#define __SPEL_MODE_H__

#if _MSC_VER > 1000
   #pragma once
#endif // _MSC_VER > 1000

/********************************************************************/
/**    S P E L L I N G   C O N F I G U R A T I O N  F I L E      ****/
/********************************************************************/

#include "spel_src.h"
//#define      WATCOM
//#undef       TURBO_C
//#undef       BC_FOR_WIN

/*********************************************************************/
/****************** Static  dictionary config  ***********************/

#define   APF_IN_BEG     /* Apostrof is a first in decoding list -prev dict*/

#undef   APF_IN_TAIL     /* Apostrov is a last in decoding list -new dict  */


/********************************************************************/

/********************* System configuration  *************************/
#define  PARTSPEC
#define  ART
#define  GLUE_CUT
#define  ENGLISH_L
#undef   OVERVOC
#undef   TEST
#undef   TIMETEST
#undef   RECOGTEST
#undef   ANOTHER_FILE        /* output to page0.ed */
#undef   V_RECOG             /* post spelling recog */
#define  BATCH
#define   DYNVOC              /* dynamic vocabulary */
#define  BASE_VOC


/*********************     Debugging printing  ***********************/

#undef   SYSPR            /* This const includes batch mode printing */

#undef   SYSPR_ERROR
#undef   OBJPR
#undef   OBJPR_REJECT
#undef   PARTPR
#undef  PARTPR_REJECT
#undef  PARTPR_ART_REJECT
#undef  WORDPR
#undef  WORDPR_ACCEPT
#undef  WORDPR_REJECT
#undef  WORDPR_STOP
#undef  WORDPR_REJECT_GEN
#undef  ARTPR
#undef  ARTPR_ALL
#undef  ARTPR_REJECT
#undef  EDPR_CORR
#undef  EDPR_CORRWORD
#undef  PRESS_ANY_KEY
#undef  STAT_OF_REPL_PR    /*     printing of replacement statistic       */

/***********************    Miscellaneous    *******************************/

#undef S_COMMENTS                   /* MUST BE ALWAYS UNDEF  */
#undef FOR_EDIT                     /* ---"----"------"-----*/

#undef SECOND_PASS        /*   constant switches on/off collection of    */
			  /*  statistic over first (general) pass        */
			  /*    and envokes second pass over word stack  */
			  /* Second pass isn't written. There are only */
			  /* some investigation dashes unusable in       */
			  /* the working mode.                           */
#endif //#ifndef __SPEL_MODE_H__
