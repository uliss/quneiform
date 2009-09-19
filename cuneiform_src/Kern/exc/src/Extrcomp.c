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

/*#include <windows.h>*/
#include <sys/stat.h>
/*#include <io.h>*/
#include <stdlib.h>
#include <setjmp.h>
#include <assert.h>
#include <stdio.h>
#include <fcntl.h>
/*#include <direct.h>*/

#include "compat_defs.h"

#include <string.h>

#include "globus.h"
#include "struct.h"
#include "v1comp.h"
#include "globus.h"
#include "imgaccs.h"
#include "msgerr.h"
#include "ccom.h"
#include "tuner.h"
#include "cpage.h"

#include "exc.h"
#include "evn.h"
#include "loc.h"
#include "excdefs.h"
//#include "rcutp.h"
#include "minmax.h"

#include "resource.h"
//static  uchar*  lnOcrPath;
static  Rect16  merge_frame;
static  Bool32  is_merge_frame=FALSE;
static  CCOM_handle NumContainer =0;
static  int32_t   curr_line=0, original_density;
static  uchar   original_begends;
static  uchar * rasterDIB4=NULL;
static  int32_t   DIB_Hei,    DIB_Wid,
                DIB_HRes,   DIB_VRes,
                DIB_TCol,   DIB_TRow,
                DIB_BW,     DIB_FM=1,
                DIB_TWid,   DIB_THei ;
static  Bool32  EnableTemplate=FALSE;
static  uchar mask_l[]     =
        {255,    127,   63,   31,   15,    7,    3,   1};
static  uchar mask_r[]     =
        {  128,  192,  224,  240,  248,  252,  254, 255};
static FNREXC_ProgressStart  fnProgressStart_exc =NULL;
static FNREXC_ProgressStep   fnProgressStep_exc  =NULL;
static FNREXC_ProgressFinish fnProgressFinish_exc=NULL;
//=========== Progress Monitor
static  uint32_t progress_vol=0, progress_rel=0;
void    progress_start (uint32_t volume)
{
progress_vol=volume;
if( fnProgressStart_exc )
    fnProgressStart_exc();
return ;
}

void    progress_stop(void)
{
progress_vol=0;
progress_rel=0;
if( fnProgressFinish_exc )
    fnProgressFinish_exc();
return ;
}

uint32_t  progress_set_percent (uint32_t step)
{
uint32_t  rel = (step/progress_vol)*100;
if( !progress_vol || rel<progress_rel+10 )
    return step;
progress_rel = rel;
if( fnProgressStep_exc )
    fnProgressStep_exc(rel);
return step;
}
 //------------------ Image attributes ---------------------
uint16_t image_disp_byte, image_disp_end;
uchar image_disp_mask;
int16_t  image_blth    ;  // pixels per line
int16_t  image_height  ;  // lines in file number
int16_t  image_lth     ;  // bytes per line
uchar image_black   ;  // mask for black pixels adding
uchar image_white   ;  // mask for wite pixels adding

static char image_file_status = -1;
static uchar image_invert = 0;
//========== Global func ==========
void extrcomp(void);
void save_component(ExtComponent *c, version *vs, version *ve,
                           uchar *lp, uint16_t lpl);
void invert_tiff (uchar *p, uint16_t lth);
void image_file_close(void);
Bool image_file_open (void);
int16_t source_read(uchar *start, uchar *ptr, uchar *end);

//========= Import func =========
// from GRA_REC.c
/*-Andrey: moved to RRecCom (recognition) and RNorm (autorotate)
//--------------------------------------------------------------
extern void     exc_ori_init(void);
extern void     exc_ori_add(void);
extern uchar    exc_ori_result(void);
-*/
// from COMPKIT.C
extern int16_t   MN_to_line(MN * mn);
//-extern uchar * make_raster();
// from ALPHABET.C
/*-Andrey: moved to RRecCom (recognition) and RNorm (autorotate)
//--------------------------------------------------------------
extern Bool16   rexc_set_alpha(uchar language, uchar *alphabet);
extern Bool16   rexc_load_tables(uchar language);
extern Bool16   rexc_is_language(uchar language);
extern int32_t    rexc_gra_type_ori(uchar lang);
extern int32_t    rexc_gra_type_rec(uchar lang);
extern void     exc_ori_recog(RecVersions *v);
-*/
// from MATRIX.C
extern int16_t  matrix_read(uchar *buff, uint16_t lth);
extern void matrix_open ();
extern void matrix_close();
extern void matrix_reset();
//========== Local func ==========
static void save_gcomp(ExtComponent *c);
static Bool process_comp(void * pool,uint32_t size);
static Bool flush_cache(void);
static void reset_cache(void);
static Bool init_dump_file(void);
static Bool write_dump(void * pool,int32_t size);
static Bool extrcomp_setup_memory(void);

//========== Import data
extern struct main_memory_str Q;
extern ExtComponent wcomp;
extern uint16_t lpool_lth;
extern uchar lpool[];
extern uchar work_raster[];
extern int32_t sz_work_raster, sz_work_raster_1;
extern BOX *dl_last_in_chain;
//========== Global data
uchar       language=-1;
int32_t       gra_type_ori =-1;
int32_t       gra_type_rec =-1;
TImageOpen  Tiger_ImageOpen;
TImageRead  Tiger_ImageRead;
TImageClose Tiger_ImageClose;
Tiger_ProcComp    Tiger_ProcessComp;
int32_t   box_number=BOX_NUMBER;
uchar   fax1x2=0,matrix=0;
uint16_t  actual_resolution;
uint16_t  comp_max_h, comp_max_w, comp_min_h, comp_min_w;
uchar   alphabet[256];
int16_t   nBlack,nAll,nWid;
int32_t ExControl;
//========== Local data

#define CACHESIZE       (64*1024)

static uchar cache[CACHESIZE],*cache_end,*cache_curr;
static char dumpfile[]= EXC_DUMP_FILE;
static int16_t  MaxScale;
static jmp_buf jumper;
static uint16_t            wHeightRC =                         0;
static uint16_t            wLowRC =                            REXC_ERR_NO;;

//============= Source CODE =============

EXC_FUNC(Bool32)  ExtrcompInit( void )
{
FNCCOM_MakeLP      MLP=(FNCCOM_MakeLP)REXCMakeLP;
int32_t                ccom_vers, vers=REXC_VERSION_CODE;

if( Q.boxstart )
    {
    ExtrcompDone();
    }

if( !CCOM_SetImportData(CCOM_FNIMP_MAKELP,(void*)MLP) )
    {
    wLowRC = REXC_ERR_NOIMPORT;
    return FALSE;
    }
if( !CCOM_GetExportData(CCOM_FNVERSION,(void*)&ccom_vers) )
    {
    wLowRC = REXC_ERR_NOEXPORT;
    return FALSE;
    }
if( ccom_vers!=vers )
    {
    wLowRC = REXC_ERR_DIFVERSIONS;
    return FALSE;
    }
return extrcomp_setup_memory();
}

EXC_FUNC(void)  ExtrcompDone(void)
{
if(Q.boxstart)
    free(Q.boxstart);
Q.boxstart=NULL;
if( ExControl & Ex_EvnRecog )
    EVNDone();
#ifdef _USE_GRA_
GraDone();
#endif
return;
}



EXC_FUNC(int32_t)  Extracomp(ExcControl ExCW,TImageOpen tio,
                                TImageClose tic, TImageRead tir,
                                Tiger_ProcComp tipc)
{
int rc;

if( !Q.boxstart )
    {
    ExtrcompInit();
    }
//-exc_ori_init();
if((rc=setjmp(jumper)) != 0)
    {  // clean up and exit
    image_file_close();
    reset_cache();
    progress_stop();
    if( rc==ExRc_NotEnoughMemory && box_number==BOX_NUMBER )
        {
        box_number *= 4;
        free(Q.boxstart);
        Q.boxstart=malloc(box_number * BOXSIZE);
        if( Q.boxstart== NULL)
            return ExRc_MemAllocFail;
        if( !(ExControl & Ex_NoContainer) )
            CCOM_ClearContatiner(NumContainer);
        }
    else
        return rc;
    }

Tiger_ImageOpen     = tio;
Tiger_ImageClose    = tic;
Tiger_ImageRead     = tir;
Tiger_ProcessComp   = tipc;
ExControl = ExCW.Control;

if(ExControl & Ex_DumpFile)
    init_dump_file();

if(ExCW.MaxCompWid > 0)
    comp_max_w = ExCW.MaxCompWid;
else
    comp_max_w = RASTER_MAX_WIDTH ;

if(ExCW.MaxCompHei > 0)
    comp_max_h = ExCW.MaxCompHei;
else comp_max_h = RASTER_MAX_HEIGHT;

if(ExCW.MinCompWid > 0)
    comp_min_w = ExCW.MinCompWid;
else
    comp_min_w = 0;

if(ExCW.MinCompHei > 0)
    comp_min_h = ExCW.MinCompHei;
else
    comp_min_h = 0;
if(ExCW.MaxScale > 0)
    MaxScale = ExCW.MaxScale;
else
    MaxScale = 5; //  for cuneiform pitures process

image_file_open(); // call progress_start();
nBlack=0;
nAll=image_height;
nWid=image_lth;
extrcomp();
//CCOM_CompressContatiner(NumContainer);
progress_stop();

image_file_close();
flush_cache();

return ExRc_OK;
}

EXC_FUNC(Bool32)  REXCExtracomp(ExcControl ExCW,TImageOpen tio,
                                TImageClose tic, TImageRead tir,
                                Tiger_ProcComp tipc)
{
int32_t ret;
ret = Extracomp(ExCW,tio,tic,tir,tipc);

switch (ret)
{
case    ExRc_OK:
    wLowRC = REXC_ERR_NO;
    return TRUE;
case    ExRc_InternalError   :
    wLowRC = REXC_ERR_INTERNAL;
    break;
case    ExRc_MemAllocFail    :
    wLowRC = REXC_ERR_NOMEMORY;
    break;
case    ExRc_DumpOpenFail    :
    wLowRC = REXC_ERR_DUMPOPEN;
    break;
case    ExRc_DumpWriteFail   :
    wLowRC = REXC_ERR_DUMPWRITE;
    break;
case    ExRc_ErrorInCallback :
    wLowRC = REXC_ERR_CALLBACK;
    break;
case    ExRc_IncorrectParam  :
    wLowRC = REXC_ERR_PARAM;
    break;
case    ExRc_NotEnoughMemory :
    wLowRC = REXC_ERR_NOTENOUGHTMEMORY;
    break;
default :
    wLowRC = REXC_ERR_INTERNAL;
    break;
}
return FALSE;
}



////////////////////////////////////////
// work with linked ContainerCOMponents
////////////////////////////////////////


////////////////////
// INPIT CALLBACKs
////////////////////
Bool16  EXC_DIBClose(void)
{
//rasterDIB4  = NULL;
curr_line   = 0;
EnableTemplate = FALSE;
return TRUE;
}

Bool16  EXC_DIBOpen(Tiger_ImageInfo *lpImageInfo)
{
if( !rasterDIB4 )
    return FALSE;
memset(lpImageInfo,0,sizeof(Tiger_ImageInfo));
lpImageInfo -> wImageHeight       = (uint16_t)DIB_Hei;
lpImageInfo -> wImageWidth        = (uint16_t)DIB_Wid;

lpImageInfo -> wImageByteWidth    = (lpImageInfo -> wImageWidth + 7) / 8;
lpImageInfo -> wResolutionX       = (uint16_t)DIB_HRes;
lpImageInfo -> wResolutionY       = (uint16_t)DIB_VRes;
lpImageInfo -> bFotoMetrics       = (uchar)DIB_FM; // inverted tiff image

return TRUE;
}

int16_t   EXC_DIBRead(uchar *lpImage, uint16_t wMaxSize)
{
int     d = (DIB_Wid+7)/8, len;

if( !EnableTemplate )
    { // no template zone
    wMaxSize -= d;
    for(len=0;len<wMaxSize && curr_line<DIB_Hei;len+=d, lpImage+=d, curr_line ++)
        {
        memcpy(lpImage,&rasterDIB4[curr_line*DIB_BW],d);
        }
    }
else
    {
    int dd = ((DIB_TCol+DIB_TWid)/8)-(DIB_TCol/8)+1;
    wMaxSize -= d;
    for(len=0;len<wMaxSize && curr_line<DIB_TRow;len+=d, lpImage+=d, curr_line ++)
        {
        memset(lpImage,DIB_FM?0xff:0,d);
        }
    for(;len<wMaxSize && curr_line<DIB_TRow+DIB_THei;len+=d, lpImage+=d, curr_line ++)
        {
        memset(lpImage,DIB_FM?0xff:0,d);
        memcpy(&lpImage[DIB_TCol/8],&rasterDIB4[curr_line*DIB_BW+(DIB_TCol/8)],dd);
        if( DIB_FM )
            {
            lpImage[DIB_TCol/8]              &= mask_l[ DIB_TCol&7 ];
            lpImage[(DIB_TCol+DIB_TWid)/8]   &= mask_r[ (DIB_TCol+DIB_TWid)&7 ];
            lpImage[DIB_TCol/8]              |= ~mask_l[ DIB_TCol&7 ];
            lpImage[(DIB_TCol+DIB_TWid)/8]   |= ~mask_r[ (DIB_TCol+DIB_TWid)&7 ];
            }
        else
            {
            lpImage[DIB_TCol/8]              &= mask_l[ DIB_TCol&7 ];
            lpImage[(DIB_TCol+DIB_TWid)/8]   &= mask_r[ (DIB_TCol+DIB_TWid)&7 ];
            }
        }
    for(;len<wMaxSize && curr_line<DIB_Hei;len+=d, lpImage+=d, curr_line ++)
        {
        memset(lpImage,DIB_FM?0xff:0,d);
        }
    }
return len;
}
// reverse order of string. DIB
int16_t   EXC_DIBReadReverse(uchar *lpImage, uint16_t wMaxSize)
{
int     d = (DIB_Wid+7)/8, len;
if( !EnableTemplate )
    { // no template zone
    wMaxSize -= d;
    for(len=0;len<wMaxSize && curr_line<DIB_Hei;len+=d, lpImage+=d, curr_line ++)
        {
        memcpy(lpImage,&rasterDIB4[(DIB_Hei-curr_line-1)*DIB_BW],d);
        }
    }
else
    {
    int dd = ((DIB_TCol+DIB_TWid)/8)-(DIB_TCol/8)+1;
    wMaxSize -= d;
    for(len=0;len<wMaxSize && curr_line<DIB_TRow;len+=d, lpImage+=d, curr_line ++)
        {
        memset(lpImage,DIB_FM?0xff:0,d);
        }
    for(;len<wMaxSize && curr_line<DIB_TRow+DIB_THei;len+=d, lpImage+=d, curr_line ++)
        {
        memset(lpImage,DIB_FM?0xff:0,d);
        memcpy(&lpImage[DIB_TCol/8],&rasterDIB4[(DIB_Hei-curr_line-1)*DIB_BW+(DIB_TCol/8)],dd);
        if( DIB_FM )
            {
            lpImage[DIB_TCol/8]              &= mask_l[ DIB_TCol&7 ];
            lpImage[(DIB_TCol+DIB_TWid)/8]   &= mask_r[ (DIB_TCol+DIB_TWid)&7 ];
            lpImage[DIB_TCol/8]              |= ~mask_l[ DIB_TCol&7 ];
            lpImage[(DIB_TCol+DIB_TWid)/8]   |= ~mask_r[ (DIB_TCol+DIB_TWid)&7 ];
            }
        else
            {
            lpImage[DIB_TCol/8]              &= mask_l[ DIB_TCol&7 ];
            lpImage[(DIB_TCol+DIB_TWid)/8]   &= mask_r[ (DIB_TCol+DIB_TWid)&7 ];
            }
        }
    for(;len<wMaxSize && curr_line<DIB_Hei;len+=d, lpImage+=d, curr_line ++)
        {
        memset(lpImage,DIB_FM?0xff:0,d);
        }
    }
return len;
}
/////////////////////
// OUTPUT CALLBACKs
/////////////////////
uint16_t          push_box_to_container(ExcBox *  g)
{
CCOM_New(NumContainer ,g->row, g->col, g->w, g->h);
return 1;
}

Bool    AcceptBoxes(void * pool, uint32_t size)
{
ExcBox* g , *ge;

for(    g = (ExcBox*)pool, ge = (ExcBox*)((char*)pool+size)  ;
        g<ge ;
        g++)
        {
//      push_box_to_container(g);
        }

return TRUE;
}

uint16_t          push_comp_to_container(ExtComponent *  g)
{
uchar           res[16];
int             nvers, i;
RecVersions     vers={0};
CCOM_comp    *  curr_comp;
CCOM_USER_BLOCK ublock[3];
uint16_t          lth;
uchar        *  lpool;

lth = *((uint16_t*)((uchar*)g + sizeof(ExtComponent))); // size
lpool = (uchar*) ((uchar*)g + g->lines); // linerepesentation
if( g->nvers )
    {
    nvers = g->nvers;
    memcpy(res,(uchar*)g + g->records, g->nvers);
    }
else
    nvers=res[0]=0;

if( g->cs==255 )
    {
    vers.lnAltCnt = nvers/2;
    for(i=0;i<vers.lnAltCnt;i++)
        {
        vers.Alt[i].Code    = res[2*i];
        vers.Alt[i].Prob    = res[2*i+1];
        vers.Alt[i].Method  = 13;
        } // network collection

    }
else
    {
    vers.lnAltCnt = nvers;
    for(i=0;i<nvers;i++)
        {
        vers.Alt[i].Code    = res[i];
        vers.Alt[i].Prob    = 255;
        vers.Alt[i].Method  = 5;
        } // event collection
    }
curr_comp = CCOM_New(NumContainer ,g->upper, g->left, g->w, g->h);
if( curr_comp  )
    {
    if( g->scale )
        {
        ublock[0].code=CCOM_UB_DENSITY;
        ublock[0].size=4;
        ublock[0].data=(uchar*)&g->dens;
        ublock[0].next_block=&ublock[1];
        ublock[1].code=CCOM_UB_BEGENDS;
        ublock[1].size=1;
        ublock[1].data=(uchar*)&g->begends;
        ublock[1].next_block=0;
        }
    if( is_merge_frame )
        {
        ublock[1].next_block=&ublock[2];
        ublock[2].code=CCOM_UB_MERGEFRAME;
        ublock[2].size=sizeof(merge_frame);
        ublock[2].data=(uchar*)&merge_frame;
        ublock[2].next_block=0;
        }
    CCOM_Store(curr_comp,0,
                        lth, lpool,  g->nl,
                        g->begs, g->ends,
                        &vers,
                        g->scale ? ublock : NULL);
    curr_comp->scale= g->scale;
    curr_comp->type = g->type;
    if( g->type==ch_great && !vers.lnAltCnt )               // non dust
                curr_comp->type=CCOM_CH_GREAT;
    curr_comp->cs   = g->cs;
    }
return lth ;
}

Bool    AcceptComps(void * pool, uint32_t size)
{
ExtComponent *  g , *ge;
uint16_t          lth;

for( g = (ExtComponent*)pool, ge = (ExtComponent*)((char*)pool+size);
g<ge    ;
g=(ExtComponent*)((char*)g+sizeof(uint16_t)+sizeof(ExtComponent)+lth+g->nvers))
    {
    lth = *((uint16_t*)((uchar*)g + sizeof(ExtComponent))); // size
       //push_comp_to_container(g);
    }
return TRUE;
}


Bool32  REXCExtra(ExcControl ExCW,
    uchar *lpRaster, int32_t BWid, Bool32 ReverseOrder,
    int32_t Wid, int32_t Hei, int32_t HRes, int32_t VRes,
    int32_t TemplCol, int32_t TemplRow, int32_t TemplWid, int32_t TemplHei, Bool32 FotoMetr )

{
int32_t ret;
if( Wid<=0 || BWid<=0 || Hei<=0 ||
    HRes<WORLD_MIN_RESOLUTION || VRes<WORLD_MIN_RESOLUTION  )
    {
    wLowRC = REXC_ERR_IMAGEINFO;
    return FALSE;
    }
if( !lpRaster  ||  TemplCol<0 || TemplRow<0 ||
    TemplCol+TemplWid>Wid || TemplRow+TemplHei>Hei )
    {
    wLowRC = REXC_ERR_PARAM;
    return FALSE;
    }

DIB_Hei  = Hei;
DIB_Wid  = Wid;
DIB_HRes = HRes; // HRes - resolution pixels/meter
DIB_VRes = VRes; // VRes - resolution pixels/meter
DIB_HRes = DIB_HRes*254/10000;
if( DIB_HRes % 10 ==9 )
    DIB_HRes++;
DIB_VRes = DIB_VRes*254/10000;
if( DIB_VRes % 10 ==9 )
    DIB_VRes++;
DIB_TCol = TemplCol;
DIB_TRow = TemplRow;
DIB_TWid = TemplWid;
DIB_THei = TemplHei;
DIB_BW   = BWid;
DIB_FM   = FotoMetr;
if( ExCW.Control&Ex_Invert )
    DIB_FM = 1 - DIB_FM;
rasterDIB4 = lpRaster;
if( DIB_TWid && DIB_THei )
    EnableTemplate=TRUE;
if( !(ExControl & Ex_NoContainer) )
    {
    NumContainer = CCOM_CreateContainer();
    CCOM_SetLanguage(NumContainer,language);
    if( NumContainer==0 )
        {
        wLowRC = REXC_ERR_INTERNAL;
        return FALSE;
        }
    }

if( ReverseOrder )
    {
    if( ExCW.Control&Ex_ExtraComp )
        ret = Extracomp(ExCW,EXC_DIBOpen,EXC_DIBClose,EXC_DIBReadReverse,AcceptComps);
    else
        ret = Extracomp(ExCW,EXC_DIBOpen,EXC_DIBClose,EXC_DIBReadReverse,AcceptBoxes);
    }
else
    {
    if( ExCW.Control&Ex_ExtraComp )
        ret = Extracomp(ExCW,EXC_DIBOpen,EXC_DIBClose,EXC_DIBRead,AcceptComps);
    else
        ret = Extracomp(ExCW,EXC_DIBOpen,EXC_DIBClose,EXC_DIBRead,AcceptBoxes);
    }
switch (ret)
{
case    ExRc_OK:
    wLowRC = REXC_ERR_NO;
    if( !(ExControl & Ex_NoContainer) )
        {
        if( !CCOM_GetFirst(NumContainer ,NULL) ||
            !CCOM_GetContainerVolume(NumContainer) )
            {
            wLowRC = REXC_ERR_NOCOMP;
            //break;
            }
        }
    return TRUE;
case    ExRc_InternalError   :
    wLowRC = REXC_ERR_INTERNAL;
    break;
case    ExRc_MemAllocFail    :
    wLowRC = REXC_ERR_NOMEMORY;
    break;
case    ExRc_DumpOpenFail    :
    wLowRC = REXC_ERR_DUMPOPEN;
    break;
case    ExRc_DumpWriteFail   :
    wLowRC = REXC_ERR_DUMPWRITE;
    break;
case    ExRc_ErrorInCallback :
    wLowRC = REXC_ERR_CALLBACK;
    break;
case    ExRc_IncorrectParam  :
    wLowRC = REXC_ERR_PARAM;
    break;
case    ExRc_NotEnoughMemory :
    wLowRC = REXC_ERR_NOTENOUGHTMEMORY;
    break;
default:
    wLowRC = REXC_ERR_INTERNAL;
    break;
}
return FALSE;
}

Bool32  REXCExtracomp3CB(ExcControl ExCW,TImageOpen tio,
                                TImageClose tic, TImageRead tir)
{
int32_t ret;
  if( !(ExControl & Ex_NoContainer) )
    {
    NumContainer = CCOM_CreateContainer();
    CCOM_SetLanguage(NumContainer,language);
    if( NumContainer==0 )
        {
        wLowRC = REXC_ERR_INTERNAL;
        return FALSE;
        }
    }
if( ExCW.Control&Ex_ExtraComp )
    ret = Extracomp(ExCW,tio,tic,tir,AcceptComps);
else
    ret = Extracomp(ExCW,tio,tic,tir,AcceptBoxes);

switch (ret)
{
case    ExRc_OK:
    wLowRC = REXC_ERR_NO;
    if( !(ExControl & Ex_NoContainer) )
        {
        if( !CCOM_GetFirst(NumContainer ,NULL) ||
            !CCOM_GetContainerVolume(NumContainer) )
            {
            wLowRC = REXC_ERR_NOCOMP;
            break;
            }
        }
    return TRUE;
case    ExRc_InternalError   :
    wLowRC = REXC_ERR_INTERNAL;
    break;
case    ExRc_MemAllocFail    :
    wLowRC = REXC_ERR_NOMEMORY;
    break;
case    ExRc_DumpOpenFail    :
    wLowRC = REXC_ERR_DUMPOPEN;
    break;
case    ExRc_DumpWriteFail   :
    wLowRC = REXC_ERR_DUMPWRITE;
    break;
case    ExRc_ErrorInCallback :
    wLowRC = REXC_ERR_CALLBACK;
    break;
case    ExRc_IncorrectParam  :
    wLowRC = REXC_ERR_PARAM;
    break;
case    ExRc_NotEnoughMemory :
    wLowRC = REXC_ERR_NOTENOUGHTMEMORY;
    break;
default :
    wLowRC = REXC_ERR_INTERNAL;
    break;
}
return FALSE;
}

Bool32  REXCExtraDIB(ExcControl ExCW,
     uchar *lp_DIB,
     int32_t TemplCol, int32_t TemplRow, int32_t TemplWid, int32_t TemplHei)

{
REXC_DIB        *lpDIB = (REXC_DIB *)lp_DIB;
REXC_RGBQUAD    *pal=(REXC_RGBQUAD    *)((uchar*)(lpDIB)+sizeof(REXC_DIB ));
Bool32           foto_metric = TRUE;

if( pal[0].rgbBlue!=0 && pal[0].rgbRed!=0 && pal[0].rgbGreen!=0 &&
    pal[1].rgbBlue==0 && pal[1].rgbRed==0 && pal[1].rgbGreen==0 )
    foto_metric = FALSE;

return REXCExtra(ExCW,
    (uchar*)(lpDIB)+sizeof(REXC_DIB )+2*sizeof(REXC_RGBQUAD),
    ((((lpDIB->biWidth+7)/8)+3)/4)*4, 1,
    lpDIB->biWidth, lpDIB->biHeight,
    lpDIB->biXPelsPerMeter, lpDIB->biYPelsPerMeter,
    TemplCol, TemplRow, TemplWid, TemplHei, foto_metric );
}

Handle  REXCGetContainer(void)
{
return NumContainer ;
}


Bool extrcomp_setup_memory(void)
{
void * mem;
if((mem = malloc(box_number * BOXSIZE)) == NULL)
    error_exit(ERR_comp,13);
Q.boxstart     = (BOX*)mem;
cache_end      = cache + CACHESIZE;
reset_cache();

return TRUE;
}

void image_file_close(void)
{
 if (image_file_status != -1)
 {
   Tiger_ImageClose();
 }
 if( matrix )
    matrix_close();
 image_file_status = -1;
}

static uchar rest_line_mask_tab[8]=
 {0xff,0x80,0xc0,0xe0,0xf0,0xf8,0xfc,0xfe};
static uchar start_line_mask_tab[8]=
 {0xff,0x7f,0x3f,0x1f,0x0f,0x07,0x03,0x01};

Bool image_file_open (void)
{
 Tiger_ImageInfo p={0};


 image_file_status = 1;
 if (!Tiger_ImageOpen (&p))
    error_exit (ERR_image,ExRc_ErrorInCallback);
 image_blth = p.wImageWidth;
 if (image_blth > WORLD_MAX_WIDTH)
    image_blth = WORLD_MAX_WIDTH-1;
 image_height = p.wImageHeight;
 if (image_height > WORLD_MAX_HEIGHT)
    image_height = WORLD_MAX_HEIGHT-1;
 image_lth = p.wImageByteWidth;
 if (image_blth <= 0)
        error_exit (ERR_image,ExRc_IncorrectParam);
 image_invert = p.bFotoMetrics;
 image_black = rest_line_mask_tab[image_blth & 7];
 image_white = ~image_black;
 actual_resolution = p.wResolutionX;
 fax1x2 = 0; //(p.wResolutionX == 200 && p.wResolutionY == 100);
 if ((actual_resolution < WORLD_MIN_RESOLUTION) || (actual_resolution > WORLD_MAX_RESOLUTION))
 {
	 //Никитин А. 11.12.2002
	 //если разрешение не в пределах нормы локально выставим 300
	 actual_resolution = 300;
//        error_exit (ERR_image,ExRc_IncorrectParam);
 }

 image_disp_byte = p.wImageDisplacement / 8;
 image_disp_mask = start_line_mask_tab [p.wImageDisplacement & 7];
 image_disp_end  = (p.wImageDisplacement + image_blth + 7)/8 - image_disp_byte;
 if ( image_disp_end + image_disp_byte > image_lth )
   error_exit (ERR_image,ExRc_IncorrectParam);

 image_blth += p.wImageDisplacement & 7;
 if( matrix )
    matrix_open ();
 progress_start(p.wImageHeight);
 return TRUE;
}


int16_t source_read(uchar* start, uchar* ptr, uchar* end)
{
 int16_t i, l;

 i=end-ptr; /* length of rest of last string */
 if (i)  { memcpy (start,ptr,i); start += i; }

 l = Tiger_ImageRead (start, 0x4000);
 if (l < 0) error_exit (ERR_image,ExRc_ErrorInCallback);

 if (l && image_invert)
    invert_tiff (start,l);
 if (l && matrix)
    matrix_read (start,l);
 return l;
}


void error_exit(int16_t group, int16_t element)
{
group=element;
longjmp(jumper,element);
}

void alone_comp(void)
{
// Oleg : 23-06-1998 -1 for right coord
//wcomp.upper--;
    if(ExControl & Ex_ExtraComp)
        save_component(&wcomp,NULL,NULL,lpool,lpool_lth);
    else
        save_gcomp(&wcomp);
}

void save_component(ExtComponent *c, version *vs, version *ve, uchar* lp, uint16_t lpl)
{
char pool[64*1024];
char *p=pool;
//Bool32  mkrs=FALSE;
//RecVersions gravers;

//int     i,j;
//unsigned char evn_res[17]="",gra_res[17]="";
#define OLEG_DEBUG3
#ifdef OLEG_DEBUG
static  int acc=0;
acc++;
if( acc==OLEG_DEBUG )
        {
        acc+=0;
        }
#endif
 vs=ve; /* avoid warn */
 c->size = sizeof(ExtComponent)+sizeof(uint16_t)+lpl;
 c->lines= sizeof(ExtComponent);
 if( lpl>sizeof(pool)-10 )
        {
        c->lines=0;
        goto proc;
        }
/*-Andrey: moved to RRecCom (recognition) and RNorm (autorotate)
//--------------------------------------------------------------
if( ExControl & Ex_EvnRecog )
if(  c->scale<3 && (c->w>>c->scale)<comp_max_w && (c->h>>c->scale)<comp_max_h )
    {
    int w=c->w,h=c->h,wb=c->rw;

    if( c->scale )
        {
        c->w >>=c->scale;
        c->h >>=c->scale;
        c->rw  =(c->w+7)/8;
        }
    c->nvers=  (int16_t)EVNRecog_lp(  (ExtComponent *)c, lp, lpl, evn_res   );
    c->records = (int16_t)(sizeof(ExtComponent)+sizeof(uint16_t)+lpl);
    mkrs=FALSE;

    if( ((ExControl & Ex_NetRecog)||c->scale) &&
        !(c->nvers==1&&evn_res[0]=='$') )
        {
        make_raster();
        mkrs=TRUE;
        exc_ori_recog(&gravers);
        if( gravers.lnAltCnt && gravers.Alt[0].Prob>200 && gravers.Alt[0].Code!='~' )
            {
            gravers.lnAltCnt=MIN(gravers.lnAltCnt,8);
            for(j=i=0;j<16&&i<gravers.lnAltCnt;i++)
                {
                if( alphabet[ gravers.Alt[i].Code ] )
                    {
                    gra_res[j]=gravers.Alt[i].Code;
                    gra_res[j+1]=gravers.Alt[i].Prob;
                    j+=2;
                    }
                }

            gra_res[j]=0;
            if( gra_res[0] )
                {
                c->nvers=(int16_t)j;
                memcpy(evn_res, gra_res,j);
                c->cs=255; // gra versions
                }
            }
        }
    if( ExControl & Ex_Orient )
        {
        if( !mkrs )
            make_raster();
        exc_ori_add();
        }

    // restore sizes
    c->w=w;
    c->h=h;
    c->rw=wb;
    }
-*/
if( c->scale )
    {
    c->dens=original_density;
    c->begends=original_begends;
    }
memcpy(p,c   ,sizeof(ExtComponent));  p += sizeof(ExtComponent);
memcpy(p,&lpl,sizeof(uint16_t));  p += sizeof(uint16_t);
memcpy(p,lp,lpl);               p += lpl;
/*-
if( (ExControl & Ex_EvnRecog) && c->nvers>0 )
    {
    memcpy(p,evn_res,c->nvers);
    p += c->nvers;
    }
-*/
if( !(ExControl & Ex_NoContainer) )
    push_comp_to_container((ExtComponent *)pool);

proc:;
if( ExControl & Ex_NoContainer )
 process_comp(pool,p - pool);
}

//======= Dummy funcs

void SpecCompPut (MN *mn){mn=mn;}
MN * accept_greate_picture(MN * mn){return mn;}
void set_active_MN(MN * mnnew,MN * mnold){mnnew=mnold;}

void tech_picture(MN *mn){mn=mn;};

//=========== Local funcs ==========

void save_gcomp(ExtComponent *c)
{
gcomp g;
g.row = c->upper; g.col = c->left;
g.h = c->h; g.w = c->w;
g.user=g.flag=0;
if( !(ExControl & Ex_NoContainer) )
 push_box_to_container((ExcBox *)&g);
if( ExControl & Ex_NoContainer )
 process_comp(&g,sizeof(gcomp));
}

Bool process_comp(void * pool,uint32_t size)
{
 assert(size < CACHESIZE);
 assert(pool && cache);

 if(cache_curr + size > cache_end || size == 0)
 {
    if(!Tiger_ProcessComp(cache,cache_curr-cache))
       error_exit(0,ExRc_ErrorInCallback);
    if(ExControl & Ex_DumpFile)
      write_dump(cache,cache_curr-cache);

    reset_cache();
 }
 memcpy(cache_curr,pool,size);
 cache_curr += size;
 return TRUE;
}

Bool flush_cache(void)
{   return process_comp(cache,0); /* clean up cache */ }

void reset_cache(void)
{ assert(cache < cache_end); cache_curr = cache; }

Bool init_dump_file(void)
{ return remove(dumpfile) == 0; }

Bool write_dump(void * pool,int32_t size)
{
int h;

 if((h=open(dumpfile, O_CREAT|O_RDWR|O_BINARY, S_IREAD|S_IWRITE)) == -1)
  error_exit(0,ExRc_DumpOpenFail);
 lseek(h,0,SEEK_END); // append to the end
 if(write(h,pool,size) != size) error_exit(0,ExRc_DumpWriteFail);
 return close(h)==0;
}

EXC_FUNC(Bool32)  REXCMakeLP( RecRaster   *rRaster , uchar *lp, int16_t *lp_size, int16_t *numcomp)
{
uchar  *l, *ls;
int     len, numc;
int16_t  *llen;
ls=l=EVNMakeLine( rRaster , 2);
if( !l )
    {
    wLowRC = REXC_ERR_INTERNAL;
    return FALSE;
    }
for(numc=len=0;;)
    {
    llen = (int16_t*)l;
    len += *llen;
    if( *llen==0 )
        break;
    numc++;
    l+=*llen;
    }
*lp_size = (int16_t)len;
*numcomp = (int16_t)numc;
memcpy(lp,ls,len);
return TRUE;
}

/*-Andrey: moved to RNorm (autorotate)
//--------------------------------------------------------------
EXC_FUNC(Bool32) REXC_GetOrient(uchar *ori)
{
if( !ori )
    {
    wLowRC = REXC_ERR_PARAM;
    return FALSE;
    }
*ori  = exc_ori_result();

if (*ori == 5)
{
	const double formatA4 = 1.4;
	uint32_t PageNumber = CPAGE_GetCurrentPage();
	Handle h_Page = CPAGE_GetHandlePage(PageNumber);
	PAGEINFO page_info = {0};
    if(GetPageInfo(h_Page,&page_info))
	{
		if ((double)page_info.Height/page_info.Width > formatA4) *ori=0;
		else *ori = 4;
	}
}

if( *ori==4 )
    wLowRC =REXC_ERR_GETORIENT;
return (*ori<4);
}
-*/

EXC_FUNC(Bool32) REXC_GetInvertion(uchar *inv)
{
if( !(ExControl & Ex_Background) )
    {
    *inv=2;
    return FALSE;
    }
if( !inv )
    {
    wLowRC = REXC_ERR_PARAM;
    return FALSE;
    }
if( nAll<50 )
    {
    *inv=2;
    return FALSE;
    }
if( nBlack*10>9*nAll)
    *inv=1;
else
    *inv=0;
return TRUE;
}

EXC_FUNC(Bool32) REXC_GetExportData(uint32_t dwType, void * pData)
{
#define CASE_DATA(a,b,c)        case a: *(b *)pData = c; break
        Bool32 rc = TRUE;

  wLowRC = REXC_ERR_NO;
        switch(dwType)
        {
        CASE_DATA(REXC_Word8_Matrix                     ,uchar,matrix);
        CASE_DATA(REXC_Word8_Fax1x2                     ,uchar,fax1x2);
        CASE_DATA(REXC_Word16_ActualResolution  ,uint16_t,actual_resolution);
/*-Andrey: moved to RRecCom (recognition) and RNorm (autorotate)
//--------------------------------------------------------------
        case REXC_FNEVNPROPERT:
            *(uint32_t*)pData =          (uint32_t)REXC_SetEVNProperties;
            break;
-*/
        case REXC_FNEXTRACOMP :
            *(uint32_t*)pData =          (uint32_t)REXCExtracomp;
            break;
        case REXC_FNEXTRA :
            *(uint32_t*)pData =          (uint32_t)REXCExtra;
            break;
        case REXC_FNEXTRADIB:
            *(uint32_t*)pData =          (uint32_t)REXCExtraDIB;
            break;

        case    REXC_FNGETCONTAINER:
            *(uint32_t*)pData =          (uint32_t)REXCGetContainer;
            break;

        case    REXC_FNGETLENEREP:
            *(uint32_t*)pData =          (uint32_t)REXCMakeLP;
            break;
        case    REXC_FNVERSION:
            *(uint32_t*)pData =          (uint32_t)REXC_VERSION_CODE;
            break;
/*-Andrey: moved to RRecCom (recognition) and RNorm (autorotate)
//--------------------------------------------------------------
        case    REXC_FNREX_ISLANGUAGE:
            *(uint32_t*)pData =          (uint32_t)REXC_IsLanguage;
            break;
        case    REXC_FNGETORIENT:
            *(uint32_t*)pData =          (uint32_t)REXC_GetOrient;
            break;
-*/
        case    REXC_FNMN2CCOM:
            *(uint32_t*)pData =          (uint32_t)REXC_MN2CCOM;
            break;
        case    REXC_FNGETINVERTION:
            *(uint32_t*)pData =          (uint32_t)REXC_GetInvertion;
            break;
/*-Andrey: moved to RRecCom (recognition) and RNorm (autorotate)
//--------------------------------------------------------------
        case    REXC_FNEVNALPHABET:     // установка алфавита для евент
            *(uint32_t*)pData =          (uint32_t)REXC_SetEVNAlphabet;
            break;
-*/
        case    REXC_FNEXTRACOMP3CB:    // 3 коллбэка
            *(uint32_t*)pData =          (uint32_t)REXCExtracomp3CB;
            break;
/*-Andrey: moved to RRecCom (recognition) and RNorm (autorotate)
//--------------------------------------------------------------
        case    REXC_FNEXTGETRECRASTERORIENT: // ориентация рабочего растра
            *(uint32_t*)pData =          (uint32_t)exc_get_rec_raster_orient;
            break;
        case    REXC_FNSETALLALPHAGRA: // алфавит нейронной сети
            *(uint32_t*)pData =          (uint32_t)exc_set_all_alphabet_gra;
            break;
-*/
        default:
                wLowRC = REXC_ERR_NOTIMPLEMENT;
                rc = FALSE;
        }
#undef CASE_DATA
return rc;
}

EXC_FUNC(Bool32) REXC_SetImportData(uint32_t dwType, void * pData)
{
#define CASE_DATA(a,b,c)        case a: c = *(b *)pData; break
#define CASE_PDATA(a,b,c)       case a: c = (b)pData;    break
wLowRC = REXC_ERR_NO;
switch(dwType)
    {
        CASE_DATA(REXC_Word8_Matrix             ,uchar,matrix);
        CASE_DATA(REXC_Word8_Fax1x2             ,uchar,fax1x2);
    CASE_DATA(REXC_Word16_ActualResolution,uint16_t,actual_resolution);
    CASE_PDATA(REXC_ProgressStart,      FNREXC_ProgressStart, fnProgressStart_exc);
        CASE_PDATA(REXC_ProgressStep,   FNREXC_ProgressStep,  fnProgressStep_exc);
        CASE_PDATA(REXC_ProgressFinish, FNREXC_ProgressFinish,fnProgressFinish_exc);
//    CASE_PDATA(REXC_OcrPath,    uchar*, lnOcrPath);
    default:
            wLowRC = REXC_ERR_NOTIMPLEMENT;
            return FALSE;
    }
#undef CASE_DATA
#undef CASE_PDATA
return TRUE;
}

EXC_FUNC(uint32_t)   REXC_GetReturnCode(void)
{
if( wLowRC == REXC_ERR_NO )
  return 0;
return (wHeightRC<<16)|(wLowRC-REXC_ERR_MIN);
}

EXC_FUNC(char*)   REXC_GetReturnString(uint32_t dwError)
{
        uint16_t rc = (uint16_t)((dwError & 0xFFFF) );
        static char szBuffer[512];

        if( dwError >> 16 != wHeightRC)
                wLowRC = REXC_ERR_NOTIMPLEMENT;

        if( rc > 0 && rc <= REXC_ERR_MAX-REXC_ERR_MIN )
                strcpy((char *)szBuffer,REXC_error_name [rc]);
        else
                return NULL;

        return szBuffer;
}

EXC_FUNC(Bool32) REXC_Init(uint16_t wHeightCode, Handle hStorage)
{
if(Q.boxstart)
    {
    //wLowRC = REXC_ERR_USE;
    //return FALSE;
    ExtrcompDone();
    }
wHeightRC = wHeightCode;
//-exc_ori_init();
NumContainer =0;
return ExtrcompInit();
}

EXC_FUNC(void)   REXC_Done(void)
{
ExtrcompDone();
language=-1;
NumContainer =0;
return;
}
//////////// process large comps

struct cut_pnt
{
 int16_t xl;
 int16_t xr;
 int16_t est;
 int16_t ref;
};

typedef struct cut_pnt CP;
#define MAX_NUM_CUTPN 2048
struct big_merge_struct
{
 int16_t vh[2*RASTER_MAX_HEIGHT+2];
 char eh[MAX_NUM_CUTPN];
 char sh[MAX_NUM_CUTPN];
 uint16_t np;
 int16_t min_est;
 CP cp[MAX_NUM_CUTPN];
};

typedef struct big_merge_struct BM;

BM W;

static void frame_hist (MN *mn);
uchar* frame_cut_MN(int16_t, int16_t);
static Bool32 frame_cut_points();
static void frame_select();
static Bool16 frame_check();
static int32_t frame_cut(MN *mn);
static Bool16 save_picture_scale (MN *mn);

static int16_t big_merge(MN *mn)
{

 if ((wcomp.h > comp_max_h) || (wcomp.w >= sizeof (W.eh)))
    return 0;
//Andrey: определение точек разрезания вынесено в модуль RCutP - откат

 frame_hist (mn);
 if (!frame_check())
    return 0;
 if( !frame_cut_points() )
    return 0;
 frame_select();

// if (!RCUTP_RExc_CP(mn, &W, &wcomp, actual_resolution)) return 0;

 is_merge_frame=FALSE;
 if( !frame_cut(mn) )
    return 0;
 is_merge_frame=FALSE;
return 1;
}

static void frame_hist (MN *mn)
{
 BOX *box;
 LNSTRT * ls;
 BOXINT * iv;
 int16_t i,j, row, end, lth;
 memset (&W,0,sizeof(W));
 i = mn->mnboxcnt;      box = (BOX *)(mn->mnfirstbox);
 while (--i >= 0)
  {
   ls = (LNSTRT *)(box + 1);
   row = ls->y - wcomp.upper;
   end = ls->x - wcomp.left;
   lth = ls->l;
   W.vh[row] += lth;    W.eh[end]++;    W.sh[end-lth]++;
   j = (box->boxptr - sizeof(BOX) - sizeof(LNSTRT)) >> 2;
   if (j == 0) { box = box->boxnext; continue; }
   iv = (BOXINT *) (ls+1);
next_box:;
   while (j--)
    {
     end += iv->d; lth = iv->l; row++; iv++;
     W.vh[row] += lth;  W.eh[end]++;    W.sh[end-lth]++;
    }
   if (box->boxflag & BOXEND) {box = box->boxnext; continue;}
   box = box->boxnext; iv = (BOXINT *)(box+1);
   j = (box->boxptr - sizeof(BOX)) >> 2;
   i--; goto next_box;
  }

 for (i = 1; i<wcomp.w; i++)
  {
   W.sh[i] = W.sh[i-1] + W.sh[i] - W.eh[i];
   W.eh[i-1] = W.sh[i-1] - W.eh[i];
  }
 W.eh[wcomp.w - 1] = 0;
}


static Bool16 frame_check()
{
 uint16_t hist[2*RASTER_MAX_HEIGHT+2];
 uint16_t s,bound,i;
 uchar* pe,*pb;
 uint16_t out_4max;
 memset (hist,0,sizeof(hist));
 pb = W.sh; pe = W.sh+wcomp.w;
 while (pb != pe) hist[*(pb++)]++;
 bound = (wcomp.w * 70)/100;
 out_4max =0;
 s = hist[0] + hist[1] + hist[2] + hist[3];
 if (s > bound) goto reject;
 for (i = 4; i<=wcomp.h; i++)
  {
   s+=hist[i]-hist[i-4];
   if (s > out_4max) out_4max = s;
   if (s > bound) goto reject;
  }

 return TRUE;
reject:;
 return FALSE;
}


static Bool32 frame_cut_points()
{
 int16_t ncp,ndown,nups,nupe,nw,nstart;
 int16_t add_est;
 int16_t thick_b;
 W.min_est = 0x7fff; W.np = 1; thick_b = (wcomp.h < 34) ? 2 : 3;

if( actual_resolution>400 )
 thick_b <<= 1;

 for (ncp = 8; ncp < wcomp.w-8; ncp++)
  {
   if (W.sh[ncp] > W.eh[ncp-1]) continue;

//      nstart - first point where sh[nstart+1] == eh[nstart]
//      ndown - last point where eh[ndown-1] == sh[ndown] > eh[ndown]
//      nups - first point where eh[nups-1] < sh[nups] == eh[nups]
//      nupe - first point where previous condition doesn't meet
//      ncp < ndown < nups < nupe

//      Found the conical decrease
   nw = ncp; nstart = nw-1;
   while ((nw < wcomp.w) &&
    ( (W.sh[nw] > W.eh[nw]) &&
     ((W.sh[nw] == W.eh[nw-1]) ||
     ((W.sh[nw] + 4 <= W.sh[nw-1]) && (W.sh[nw]-1 == W.eh[nw-1])))
    ) )
     nw++;
   if( nw>MAX_NUM_CUTPN )
    return 0;
   if (nw >= wcomp.w-8) break;
   if (W.sh[nw] > W.eh[nw-1])
    {
     if (W.sh[nw] > W.eh[nw]) {ncp = nw; continue;}
     ndown = nw-1;
     if (W.eh[nw-1] < thick_b)
       { add_est = W.eh[nw-1]; ncp = nw; goto force_cut; }
     goto way_up;
    }
//      Found a flat part
flat_part:
   ndown = nw-1; if (ndown == nstart) {ncp = nw; continue;}
   nw++;
   while ((nw < wcomp.w) && (W.sh[nw] == W.eh[nw-1]) && (W.sh[nw] == W.eh[nw]))
     nw++;
   if( nw>MAX_NUM_CUTPN )
    return 0;
   if (nw >= wcomp.w-8) break;
   if (W.sh[nw-1] <= thick_b)
    {
      if (W.sh[nw] > W.sh[nw-1])
          { add_est = W.sh[nw-1]; ncp = nw; goto force_cut; }
      if (W.sh[nw] < W.sh[nw-1]) goto flat_part;
    }
   if (!((W.sh[nw] > W.eh[nw-1]) && (W.sh[nw] == W.eh[nw])))
      { ncp = nw; continue; }

//      Found a way up - conical increase
way_up:;
   nups = nw;
   while ((nw < wcomp.w) && (W.sh[nw] > W.eh[nw-1]) &&
   ((W.sh[nw] == W.eh[nw]) ||
   ((W.sh[nw]-1 ==W.eh[nw]) && (W.sh[nw]+4 <= W.sh[nw+1])))
   )
   nw++;
   if( nw>MAX_NUM_CUTPN )
    return 0;
   nupe = nw; if (nups == nupe) {ncp = nw; continue;}

//      Rough check of cut point and write a point
   ncp = nw; add_est =0;
   if (nups - ndown > 3) continue;
   if ((nups - ndown == 3) &&
      ((W.sh[ndown-1] - W.eh[ndown] <=3) || (W.sh[nups+1] -W.eh[ndown] <=3)))
          continue;
   if ((W.sh[nstart] - W.eh[ndown] <=2) || (W.sh[nupe] - W.eh[ndown] <= 2))
       continue;
   if (W.sh[nstart] - W.eh[ndown] <=3) add_est += 50;
   if (W.sh[nupe] - W.eh[ndown] <= 3)  add_est += 50;
   nw = W.np++;
   if( nw>MAX_NUM_CUTPN )
    return 0;
   W.cp[nw].xl = ndown+1; W.cp[nw].xr = nups;

//      Estimate point
   W.cp[nw].est = add_est +
        wcomp.h + wcomp.h - W.sh[nstart] - W.sh[nupe] + W.eh[ndown] +
        ((nupe - nstart < 10) ? 10-nupe+nstart : 0);
   switch (nups - ndown)
    {
     case 1: break;
     case 2: W.cp[nw].est += (W.cp[nw].est-add_est+4) >> 3; break;
     case 3: W.cp[nw].est += (W.cp[nw].est-add_est+2) >> 2; break;
    }
setpoint:
   if (W.min_est > W.cp[nw].est) W.min_est = W.cp[nw].est;
   continue;

//      Forcing cut point
force_cut:
   nups = (nw+ndown+1)/2; nw = W.np++;
   W.cp[nw].xl = W.cp[nw].xr = nups; W.cp[nw].est = add_est * 10 ; goto setpoint;
  }
return 1;
}


static int16_t dist_est (int16_t est)
{
      if (est < 64) return 0;
      else
       {
        if (est > 128) return 200+est*2;
        if (est > 108) return 100+est/2;
        else return est/2;
       }
}

static void frame_select()
{
 int16_t i, j, est, mest, ref, we, wr;
 if (W.np == 1) return;
 W.min_est--;
 W.cp[0].est = W.min_est;
 for (i = W.np-1; i >= 0; i--)
  {
    ref = 0;
    we = W.cp[i].est - W.min_est;
    wr = W.cp[i].xr;
    mest = we + dist_est((int16_t)(wcomp.w - wr));
    for (j = i+1; j<W.np; j++)
     {
      est = W.cp[j].est + we + dist_est((int16_t)(W.cp[j].xl - wr));
      if (est < mest) { ref = j; mest = est; }
     }
    W.cp[i].est = mest; W.cp[i].ref = ref;
  }
}
//----------------------- Frame cut ---------------------------------------

MN * frame_comp;
int16_t frame_upper, frame_comp_col, frame_height;
BOX * frame_end;
static void frame_put (uchar* p, int16_t x, int16_t l, int16_t from, int16_t to);

void frame_cut_MN_set (MN *mn)                  //AK 14.03.97 void
{
 frame_comp = mn; frame_end = dl_last_in_chain;
 frame_upper = mn->mnupper; frame_comp_col = wcomp.left;
 frame_height = wcomp.h;
}

uchar* frame_cut_MN (int16_t from, int16_t to)
{
 int16_t bw, col, x, cnt;
 BOX *bp;
 uchar* p;
 LNSTRT *lp;
 BOXINT *ip;
 to -= from; bw = (to+7) >> 3;
 memset (work_raster,0, bw*frame_height);
 col = frame_comp_col + from; bp = frame_comp->mnfirstbox;
 while (1)
  {
   lp = (LNSTRT *) (bp+1); p = work_raster + (lp->y - frame_upper) * bw;
   frame_put (p, x = lp->x, lp->l, col, to);
   cnt = (bp->boxptr - sizeof(BOX) - sizeof(LNSTRT))/sizeof(BOXINT);
   ip = (BOXINT *)(lp+1);
box_cont:
   while (cnt--)
    { frame_put (p += bw, x += ip->d, ip->l, col, to); ip++; }
   if ((bp->boxflag & BOXEND) == 0)
    {
     bp = bp->boxnext; ip = (BOXINT *)(bp+1);
     cnt = (bp->boxptr - sizeof(BOX))/sizeof(BOXINT);
     goto box_cont;
    }
   if (bp == frame_end) break;
   bp = bp->boxnext;
  }
 return work_raster;
}
static uchar start_shift[] = {255, 127, 63, 31, 15, 7, 3 ,1};
static uchar end_shift[] = {0,0x80,0xc0,0xe0,0xf0,0xf8,0xfc,0xfe};
static void frame_put (uchar* p, int16_t x, int16_t l, int16_t from, int16_t to)
{
 uchar b;
 x -= l + from; if (x < 0) { if ((l+=x) < 0) return; x = 0; }
 if ( x >= to) return; if (to - x < l) l = to - x;
 p += x >> 3; b = start_shift [x & 7]; l+= x&7;
 while (l >= 8) { *p++ |= b; b=0xff; l-=8;}
 b &= end_shift[l]; *p |= b;
}

static Bool16 exclude_one_piece(int16_t xl, int16_t xr, int16_t x0, int16_t y0, int16_t xmax, int16_t h)
{
#ifdef _USE_LOC_
MN * wm;
uchar* raster;


     raster = frame_cut_MN(xl,xr);
     wm = LOC_CLocomp (raster, (xr - xl + 7)>>3, h, y0, (int16_t)(x0 + xl));

     while (wm && wm->mnfirstbox)
       {
       MN_to_line (wm);
       wcomp.large = ((xl) ? ch_cut_left : 0) +
                        ((xr != xmax) ? ch_cut_right : 0);
       alone_comp();
       wm = wm->mnnext;
       }
return TRUE;
#else
return FALSE;
#endif
}

static int32_t frame_cut(MN *mn)
{
 int16_t xl, xr, ncp,cutn;
 int16_t x0 = wcomp.left, y0=wcomp.upper, xmax = wcomp.w, h=wcomp.h;


 if (W.cp[0].ref == 0)
    tech_picture(mn);

 xl=0; ncp = 0;cutn=0;
 is_merge_frame     = TRUE;
 merge_frame.bottom = y0+h;
 merge_frame.left   = x0;
 merge_frame.right  = x0+xmax;
 merge_frame.top    = y0;
 while (1)
   {
   ncp = W.cp[ncp].ref;
   xr = (ncp) ? W.cp[ncp].xl : xmax;
   if ( xr - xl < comp_max_w)
    {
    cutn++;
    if( !exclude_one_piece(xl, xr, x0, y0, xmax, h) )
        break;
    }
   else if( xr - xl>=comp_max_w && xr - xl < 2*comp_max_w)
    {
    if( !exclude_one_piece(xl, (int16_t)((xl+xr)/2), x0, y0, xmax, h) )
        break;
    if( !exclude_one_piece((int16_t)((xl+xr)/2), xr, x0, y0, xmax, h) )
        break;
    cutn++;
    }
   else
       return 0;

   if (ncp == 0)
    break;
   xl = W.cp[ncp].xr;
   }
is_merge_frame=FALSE;
return cutn;
}

void picture_process(MN *mn)
{
 if( (mn->mnflag & mnpicture) || (ExControl&Ex_DisableCut) ||
     ( wcomp.h < comp_max_h && wcomp.w>2*comp_max_w )
     )    // fotograph ?
        { // old loop of Vald
        tech_pict:
        tech_picture(mn);
        save_picture_scale(mn);
        return;
        }

 if (wcomp.h < comp_max_h )
          // if not scalable component
        {
        frame_cut_MN_set (mn);
        if (!big_merge(mn))
          goto tech_pict; // if not frame regard as big glued
        return;
        }
else
        goto tech_pict;
return;
}


static void save_prot(void)
        { // protocol for exernal viewer
         uchar* p;
         int16_t l;
         FILE *ev;
         uchar wr_ltr;
         wr_ltr = 'c';
         ev=fopen("sqprot.pro","a");
         if (ev==NULL) ev=fopen("sqprot.pro","w");

         fprintf(ev,"-2 '%c' 1\n00 %d\n-1 %d %d\n",wr_ltr,wr_ltr,(wcomp.w+7)>>3,wcomp.h);
         p=work_raster;
         l=((wcomp.w+7)>>3)*wcomp.h;
         while(--l >= 0)
          {
           if (*p < 16) fprintf(ev,"0%1x",*p);
           else         fprintf(ev,"%2x",*p);
           p++;
          }
         fputc('\n',ev);
         fclose(ev);
        }


extern void WriteInterval ( uchar *p, int x , int l);
static void WriteOneInterval ( int h, int b, int e, int bw, int scale)
{
if( b<0 )
    b=0;
if( e<0 )
    e=0;
if( b>e )
    b=e;
b>>=scale;
e>>=scale;
if( b==e )
    {
    if( b )
        b--;
    else
        e++;
    }
WriteInterval ( &work_raster[(h>>scale)*bw] , e , e-b);
return;
}



// compress MN to work_raster
static Bool16 scaleMN2work_raster(  MN *mn,int16_t upper, int16_t left, int16_t w, int16_t h,
                        int     scale_2)
{
BOX    *pBox;
int    nBox;
LNSTRT *pLine;
BOXINT *pInts;
BOXINT *pAfterInts;
BOXINT *pInt;
int             xEnd, xBeg, y, hmin, hmax;
int16_t     top=upper+h, right=left+w;


memset( work_raster,0, wcomp.rw*wcomp.h);
hmin=64000;
hmax=0;
for (pBox = (BOX *) mn -> mnfirstbox, nBox = 0;
             nBox < mn -> mnboxcnt;
                 pBox = pBox -> boxnext, nBox++)
        {
    if (pBox -> boxflag & BOXBEG)
        { // new line
        pLine = (LNSTRT *) ((uchar *) pBox + sizeof (BOX));

        xEnd = pLine -> x-wcomp.left;
        y    = pLine -> y-wcomp.upper;
        if( y<hmin )    hmin=y;
        if( y>hmax )    hmax=y;
        xBeg = MAX(xEnd - pLine -> l,0);
        WriteOneInterval ( y, xBeg, MIN(xEnd,right) ,wcomp.rw,scale_2);
        pInts = (BOXINT *) ((uchar *) pBox + sizeof (BOX) + sizeof (LNSTRT));
        }
    else
        { // continue BOX
        pInts = (BOXINT *) ((uchar *) pBox + sizeof (BOX));
        }

    pAfterInts = (BOXINT *) ((uchar *) pBox + pBox -> boxptr);

    for (pInt = pInts;   ;     pInt++)
        { // cont line
        int s=(uchar *) pAfterInts - (uchar *) pInt;
        if( s<sizeof (BOXINT) )
            break;
        xEnd += pInt -> d;
        y++;
        if( y<hmin )    hmin=y;
        if( y>hmax )    hmax=y;
        xBeg  = MAX(xEnd - pInt -> l,0);
        WriteOneInterval (y, xBeg, MIN(xEnd,right), wcomp.rw,scale_2);
        }
    }
if( hmin==0 && hmax>=h-1 )
    original_begends=0;
return TRUE;
}

// extract MN from work_raster, convert MN to lines and store lines
static MN* work_raster2MN( void )
{
#ifdef _USE_LOC_
return LOC_CLocomp (work_raster, wcomp.rw, wcomp.h, wcomp.upper, wcomp.left);
#else
return 0;
#endif
}

static store_MN(MN *locmn,int16_t upper, int16_t left, int16_t w, int16_t h,
                        int     scale_2)
{
if( !locmn )
    return FALSE;
if( 0 )
    save_prot();
MN_to_line(locmn);


wcomp.scale=scale_2;
wcomp.type=ch_great;
// restore old sizes
wcomp.upper =upper;
wcomp.left  =left ;
wcomp.h     =h    ;
wcomp.w     =w    ;
alone_comp();

return TRUE;
}

EXC_FUNC(CCOM_comp*)   REXC_MN2CCOM(Handle hCCOM,Handle hmn)
{
MN *locmn=(MN*)hmn;
CCOM_handle hccom=(CCOM_handle)hCCOM;
CCOM_comp   *curr_comp;
if( ExControl & Ex_NoContainer )
    return (CCOM_comp*)0;
if( !locmn )
    return (CCOM_comp*)0;
MN_to_line(locmn);

if( wcomp.w>=comp_max_w && wcomp.h>=comp_max_h  )
    return (CCOM_comp*)0;
memcpy(&work_raster[0],&lpool_lth,2);
memcpy(&work_raster[2],lpool,lpool_lth);

curr_comp = CCOM_New(hccom,wcomp.upper, wcomp.left,
                wcomp.w, wcomp.h);
if( !curr_comp  )
    return (CCOM_comp*)0;

CCOM_Store(curr_comp,0,  lpool_lth, work_raster,  wcomp.nl,
                    wcomp.begs, wcomp.ends,
                    NULL,  NULL);

curr_comp->scale= wcomp.scale;
curr_comp->type = wcomp.type;
curr_comp->cs   = wcomp.cs;

return curr_comp;
}

// check bound 255 in MN intervals
static Bool16 check_long_intervals(  MN *mn, int scale_2)
{
BOX    *pBox;
int    nBox;
LNSTRT *pLine;
BOXINT *pInts;
BOXINT *pAfterInts;
BOXINT *pInt;
int             xEnd;

for (pBox = (BOX *) mn -> mnfirstbox, nBox = 0;
             nBox < mn -> mnboxcnt;
                 pBox = pBox -> boxnext, nBox++)
        {
    if (pBox -> boxflag & BOXBEG)
        { // new line
        pLine = (LNSTRT *) ((uchar *) pBox + sizeof (BOX));

        xEnd = pLine -> x-wcomp.left;
        if( (xEnd>>scale_2)>255 || (pLine->l>>scale_2)>255 )
            return TRUE;
        pInts = (BOXINT *) ((uchar *) pBox + sizeof (BOX) + sizeof (LNSTRT));
        }
    else
            { // continue BOX
        pInts = (BOXINT *) ((uchar *) pBox + sizeof (BOX));
        }

    pAfterInts = (BOXINT *) ((uchar *) pBox + pBox -> boxptr);

    for (pInt = pInts;     ;      pInt++)
        { // cont line
        int s=(uchar *) pAfterInts - (uchar *) pInt;
        if( s<sizeof (BOXINT) )
            break;
        xEnd += pInt -> d;
        if( (xEnd>>scale_2)>255 || (pInt->l>>scale_2)>255 )
            return TRUE;
        }
    }

return FALSE;
}

int32_t sizeMN(MN *mn)
{
int32_t    s;
BOX    *pBox;
int    nBox;
LNSTRT *pLine;
BOXINT *pInts;
BOXINT *pAfterInts;
BOXINT *pInt;
int             hh, start=1;

for (s=0, pBox = (BOX *) mn -> mnfirstbox, nBox = 0;
             nBox < mn -> mnboxcnt;
                 pBox = pBox -> boxnext, nBox++)
        {
    if (pBox -> boxflag & BOXBEG)
        { // new line
        if( !start )
            {
            s+=2*4+4*(hh+1);   // intervals+fictive interval
            }
        else
            {
            start=0;
            }
        pLine = (LNSTRT *) ((uchar *) pBox + sizeof (BOX));
        hh=1;
        pInts = (BOXINT *) ((uchar *) pBox + sizeof (BOX) + sizeof (LNSTRT));
        }
    else
        { // continue BOX
        pInts = (BOXINT *) ((uchar *) pBox + sizeof (BOX));
        }

    pAfterInts = (BOXINT *) ((uchar *) pBox + pBox -> boxptr);

    for (pInt = pInts;      ;   pInt++)
        { // cont line
        int s=(uchar *) pAfterInts - (uchar *) pInt;
        if( s<sizeof (BOXINT) )
            break;
        hh++;
        }


    }
s+=2*4+4*(hh+1);
return s;
}

// store long MN intervals to container
static Bool16 longMN2container(  MN *mn,int16_t upper, int16_t left, int16_t w, int16_t h,
                        int     scale_2)
{
BOX    *pBox;
int    nBox;
LNSTRT *pLine;
BOXINT *pInts;
BOXINT *pAfterInts;
BOXINT *pInt;
int             xEnd, xBeg, y;
int32_t   size;
int             hh, y0, start=1;
CCOM_lnhead *lnh={0};
CCOM_comp   *cmp;

if( ExControl & Ex_NoContainer )
    return FALSE;
size = sizeMN(mn);
cmp=CCOM_LargeNew(NumContainer ,upper, left, w, h, size);
for (pBox = (BOX *) mn -> mnfirstbox, nBox = 0;
             nBox < mn -> mnboxcnt;
                 pBox = pBox -> boxnext, nBox++)
        {
    if (pBox -> boxflag & BOXBEG)
        { // new line
        if( !start )
            {
            CCOM_LargeNewInterval(cmp,(int16_t)0,(int16_t)0); // fictive interval
            lnh->lth=4*2+4*(hh+1); // header + intervals + fictive interval
            lnh->h      =hh; // store actual line header
            }
        else
            {
            start=0;
            }
        pLine = (LNSTRT *) ((uchar *) pBox + sizeof (BOX));
        hh=1;
        xEnd = pLine -> x-wcomp.left;
        y0=y    = pLine -> y-wcomp.upper;
        xBeg = MAX(xEnd - pLine -> l,0);

        CCOM_LargeNewLn(cmp,&lnh);
        lnh->lth=0;
        lnh->h  =0; // store actual line header
        lnh->row        =y;
        lnh->flg        =pBox->boxflag;
        CCOM_LargeNewInterval(cmp,(int16_t)xEnd,(int16_t)pLine -> l);
        pInts = (BOXINT *) ((uchar *) pBox + sizeof (BOX) + sizeof (LNSTRT));
        }
    else
        { // continue BOX
        lnh->flg        |=pBox->boxflag;
        pInts = (BOXINT *) ((uchar *) pBox + sizeof (BOX));
        }

    pAfterInts = (BOXINT *) ((uchar *) pBox + pBox -> boxptr);

    for (pInt = pInts;  ;  pInt++)
        { // cont line
        int s=(uchar *) pAfterInts - (uchar *) pInt;
        if( s<sizeof (BOXINT) )
            break;
        xEnd += pInt -> d;
        y++;
        hh++;
        CCOM_LargeNewInterval(cmp,(int16_t)xEnd,(int16_t)pInt -> l);
        }

    }
CCOM_LargeNewInterval(cmp,(int16_t)0,(int16_t)0); // fictive interval
lnh->lth=4*2+4*(hh+1); // header + intervals + fictive interval
lnh->h  =hh; // store actual line header
CCOM_LargeClose(cmp);
cmp->scale = CCOM_LONGLINES|scale_2 ;
{
	CCOM_USER_BLOCK ublock;
	ublock.code = CCOM_UB_SIZELINEREP;
	ublock.data = (uchar*)&size;
	ublock.size = 4;
	CCOM_SetUserBlock(cmp, &ublock);
}
return TRUE;
}


static int32_t mn_sum(  MN *mn )
{
BOX    *pBox;
int    nBox;
LNSTRT *pLine;
BOXINT *pInts;
BOXINT *pAfterInts;
BOXINT *pInt;
int32_t     sum;

sum=0;
for (pBox = (BOX *) mn -> mnfirstbox, nBox = 0;
             nBox < mn -> mnboxcnt;
                 pBox = pBox -> boxnext, nBox++)
        {
    if (pBox -> boxflag & BOXBEG)
        { // new line
        pLine = (LNSTRT *) ((uchar *) pBox + sizeof (BOX));

        sum += pLine -> l;
        pInts = (BOXINT *) ((uchar *) pBox + sizeof (BOX) + sizeof (LNSTRT));
        }
    else
            { // continue BOX
        pInts = (BOXINT *) ((uchar *) pBox + sizeof (BOX));
        }

    pAfterInts = (BOXINT *) ((uchar *) pBox + pBox -> boxptr);

    for (pInt = pInts;   ;    pInt++)
        { // cont line
        int s=(uchar *) pAfterInts - (uchar *) pInt;
        if( s<sizeof (BOXINT) )
                break;
        sum += pInt->l;
        }
    }
return sum;
}

Bool16 save_picture_scale (MN *mn)
{
int32_t     scale_2, scale;
int16_t     sv_upper, sv_left, sv_w, sv_h;
MN      *locmn;

if( !(ExControl&Ex_Picture) )
        return FALSE;
if( wcomp.w>=1024*16 || wcomp.h>=1024*16 )
    return FALSE; // too large comp

scale_2=0;  // log2( scale )
scale=0;    // 2**scale_2

if     ( wcomp.w< comp_max_w     )    scale_2=0;
else if( wcomp.w<(comp_max_w<<1) )    scale_2=1+0;
else if( wcomp.w<(comp_max_w<<2) )    scale_2=1+1;
else if( wcomp.w<(comp_max_w<<3) )    scale_2=1+2;
else if( wcomp.w<(comp_max_w<<4) )    scale_2=1+3;
else if( wcomp.w<(comp_max_w<<5) )    scale_2=1+4;
else if( wcomp.w<(comp_max_w<<6) )    scale_2=1+5;
else if( wcomp.w<(comp_max_w<<7) )    scale_2=1+6;
else                           scale_2=1+7;

if     ( wcomp.h< comp_max_h     )    scale=0;
else if( wcomp.h<(comp_max_h<<1) )    scale=1+0;
else if( wcomp.h<(comp_max_h<<2) )    scale=1+1;
else if( wcomp.h<(comp_max_h<<3) )    scale=1+2;
else if( wcomp.h<(comp_max_h<<4) )    scale=1+3;
else if( wcomp.h<(comp_max_h<<5) )    scale=1+4;
else if( wcomp.h<(comp_max_h<<6) )    scale=1+5;
else if( wcomp.h<(comp_max_h<<7) )    scale=1+6;
else if( wcomp.h<(comp_max_h<<8) )    scale=1+7;
else                           scale=1+8;
original_density = mn_sum(mn);
scale_2 = MAX(scale_2,scale);
if( scale_2==0 )
    { // normal sizes
    if( MaxScale==1 && (ExControl&Ex_PictureLarge) )
        {
        if( ExControl & Ex_NoContainer )
            return FALSE;
        if( !(ExControl&Ex_ExtraComp) )
            {
            CCOM_New(NumContainer ,wcomp.upper, wcomp.left, wcomp.w, wcomp.h);
            return TRUE;
            }
        longMN2container(mn,wcomp.upper, wcomp.left, wcomp.w, wcomp.h, scale_2);
        return TRUE;
        }
    return FALSE;
    }
scale_2 = MIN(scale_2,MaxScale-1);
scale = 1<<scale_2 ;
original_begends=(uchar)(MIN(wcomp.begs+wcomp.ends,255));
// COMPRESSION PICTURES TO comp_max_w:comp_max_h
sv_upper    =wcomp.upper;
sv_left     =wcomp.left ;
sv_w        =wcomp.w;
sv_h        =wcomp.h;
wcomp.h =(wcomp.h+scale-1)>>scale_2;
wcomp.w =(wcomp.w+scale-1)>>scale_2;
wcomp.rw  =(wcomp.w+7)/8;
if( wcomp.w>=comp_max_w || wcomp.h>=comp_max_h )
    {
    if( !(ExControl & Ex_PictureLarge) ||
         (ExControl & Ex_NoContainer) )
        return FALSE; // can't store scaling comp to sizes
    if( !(ExControl&Ex_ExtraComp) )
        {
        CCOM_New(NumContainer ,sv_upper, sv_left, sv_w, sv_h);
        return TRUE;
        }
    // store picture with long intervals;

    if( check_long_intervals(mn, scale_2) )
        { // process large intervals
        if( scale_2 && wcomp.rw*wcomp.h<=sz_work_raster+sz_work_raster_1 )
            {
            scaleMN2work_raster(mn,sv_upper, sv_left, sv_w, sv_h, scale_2);
            locmn = work_raster2MN(); // long MN
            if( !locmn )
                return FALSE;
            }
        else
            locmn = mn;
        if( !NumContainer )
            return FALSE;
        longMN2container(locmn,sv_upper, sv_left, sv_w, sv_h, scale_2);
        return TRUE;
        }
    // else run normal scaling
    }

// normal scaling
if( ExControl & Ex_NoContainer )
    return FALSE;
if( !(ExControl&Ex_ExtraComp) )
    {
    CCOM_New(NumContainer, sv_upper, sv_left, sv_w, sv_h);
    return TRUE;
    }
scaleMN2work_raster(mn,sv_upper, sv_left, sv_w, sv_h, scale_2);
locmn = work_raster2MN();
if( locmn )
    {
    store_MN(locmn,sv_upper, sv_left, sv_w, sv_h,  scale_2);
    }
else if( (wcomp.w>=comp_max_w || wcomp.h>=comp_max_h) &&
        (ExControl&Ex_PictureLarge) && mn )
    {
    longMN2container(mn,sv_upper, sv_left, sv_w, sv_h, 0);
    }

return TRUE;
}
