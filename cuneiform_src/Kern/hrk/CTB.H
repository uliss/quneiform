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

#ifndef __CTB_H
#define __CTB_H

#include "globus.h"
#include "ctbdefs.h"
#include "recdefs.h"
#include "memfunc.h"

#include <string.h>

#ifdef __CTB__
#define CTB_FUNC  FUN_EXPO
#else
#define CTB_FUNC  FUN_IMPO
#endif

#ifdef __cplusplus
extern "C" {
#endif

CTB_FUNC(int32_t) CTB_GetVersion(void );
//***************** open/close function prototypes **********************
CTB_FUNC(Bool32) CTB_create(char *file_name,uchar *data);
CTB_FUNC(Bool32) CTB_create_gray(char *file_name,uchar *data);
CTB_FUNC(void) CTB_done(void );
CTB_FUNC(Bool32) CTB_open(char *file_name,CTB_handle *hnd,char *attr);
CTB_FUNC(void) CTB_close(CTB_handle *hnd);

//***************** read function prototypes ****************************
CTB_FUNC(int32_t) CTB_volume(CTB_handle *hnd );
CTB_FUNC(int32_t) CTB_volume_all(char *file_name );
CTB_FUNC(int32_t) CTB_read(CTB_handle *hnd, int32_t num , uchar *ima, uchar *data );
CTB_FUNC(Bool32) CTB_read_global_data(CTB_handle *hnd,uchar *data);
CTB_FUNC(Bool32) CTB_read_data(CTB_handle *hnd,int32_t num,uchar *data);

//***************** write function prototypes ***************************
CTB_FUNC(Bool32) CTB_write( CTB_handle *hnd, int32_t num,uchar *ima, uchar *data);
CTB_FUNC(Bool32) CTB_write_mark( CTB_handle *hnd, int32_t num,uchar *ima, uchar *data, Bool32 mark);
CTB_FUNC(Bool32) CTB_insert( CTB_handle *hnd,int32_t num,uchar *ima, uchar *data);
CTB_FUNC(Bool32) CTB_delete( CTB_handle *hnd,int32_t num);
CTB_FUNC(Bool32) CTB_kill(CTB_handle *hnd,int32_t num);
CTB_FUNC(Bool32) CTB_mark(CTB_handle *hnd, int32_t num);
CTB_FUNC(Bool32) CTB_write_data(CTB_handle *hnd,int32_t num, uchar *data);
CTB_FUNC(Bool32) CTB_write_global_data(CTB_handle *hnd,uchar *data);
CTB_FUNC(Bool32) CTB_swap(CTB_handle *hnd,int32_t num1,int32_t num2);

//***************** file/memory convertion functions protoypes ************
CTB_FUNC(int32_t) CTB_compress(char *file_name);
CTB_FUNC(int32_t) CTB_copy(char *newn, char *oldn);
CTB_FUNC(int32_t) CTB_move(char *newn, char *oldn);
CTB_FUNC(int32_t) CTB_rename(char *newn, char *oldn);
CTB_FUNC(void) CTB_unlink(char *newn);
CTB_FUNC(void) CTB_align8_lines(uchar *bin,int32_t w, int32_t h);
CTB_FUNC(void) CTB_align1_lines(uchar *bin,int32_t w, int32_t h);
CTB_FUNC(void) CTB_align8_124lines(uchar *bin,int32_t w, int32_t h,
		int32_t alin,uchar init_byte);
CTB_FUNC(void) CTB_align41(uchar *sbin,int32_t w, int32_t h);
//*************** Gray images    ********* *********************************
CTB_FUNC(uchar) CTB_BestLevelInGray(uchar *image,int wx,int wy);
CTB_FUNC(Bool32) CTB_GrayBound(RecRasterEx *PaintRaster,int GLevel);
//*************** access to LEMAN protocol *********************************
CTB_FUNC(Bool32) CTB_prot_open(char *name);
CTB_FUNC(Bool32) CTB_prot_read(unsigned char *PROT_raster,
		int *PROT_w, int *PROT_h,
		unsigned char *PROT_let,unsigned char *PROT_data);
CTB_FUNC(void) CTB_prot_close(void);
CTB_FUNC(Bool32) CTB_prot_write(char *file_name,
		unsigned char *raster,
		int Width, int Height,
		unsigned char code,int align,
		unsigned char *PROT_data );
CTB_FUNC(Bool32) CTB_prot_gray_read(unsigned char *PROT_raster,
		unsigned char *PROT_data);
CTB_FUNC(Bool32) CTB_prot_gray_write(char *file_name,
		unsigned char *PROT_raster,unsigned char let, int w, int h,
		int align);
//**************** CT FormReader data I/O **********************************
CTB_FUNC(int32_t) CTB_AddRecRaster(char *fname, RecRaster *r,uchar let);
CTB_FUNC(int32_t) CTB_AddRecRaster_data(char *fname, RecRaster *r,uchar *data);
CTB_FUNC(Bool32) CTB_ReadRecRaster(CTB_handle *hnd, int32_t num ,
		uchar *let,RecRaster *r, uchar *data );
CTB_FUNC(Bool32) CTB_GetRecRaster(char *fname, int32_t id_rast ,
		RecRaster *r);

//***************** error message *****************************
CTB_FUNC(int32_t) CTB_get_error(void);
CTB_FUNC(char *) CTB_get_error_name(char *str);

#define CTB_ERR_NONE			0
#define CTB_ERR_OPEN_CTB		1
#define CTB_ERR_SEEK            2
#define CTB_ERR_SIGN            3
#define CTB_ERR_HEAD            4
#define CTB_ERR_VERS            5
#define CTB_ERR_MEM             6
#define CTB_ERR_OPEN_NDX        7
#define CTB_ERR_READ            8
#define CTB_ERR_NDX             9
#define CTB_ERR_NOT_OPEN        10
#define CTB_ERR_ZERO_DATA       11
#define CTB_ERR_KILLED          12
#define CTB_ERR_UNKNOWN_PACK    13
#define CTB_ERR_WRITE           14
#define CTB_ERR_ALIGN           15
#define CTB_ERR_DIF_VERSION     16

#ifdef __cplusplus
}
#endif

#endif

