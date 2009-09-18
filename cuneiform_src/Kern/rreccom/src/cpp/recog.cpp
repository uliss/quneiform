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

//#define _USE_GRA_ 1
/*********************************************************************************************/
#ifdef _MSC_VER
#include <windows.h>
#include <direct.h>
#else
#include <unistd.h>
#endif
#include <string.h>
#include <stdlib.h>
#include "rreccom.h"
#include "evn.h"
#include "ccom.h"
#include "r35.h"
#include "excdefs.h"
#include "compat_defs.h"
/*********************************************************************************************/
const int32_t max_raster = REC_MAX_RASTER_SIZE;//2048*32;

uchar alphabet[256];
int32_t gra_type_rec = -1;
uint16_t comp_max_h, comp_max_w, comp_min_h, comp_min_w;
int16_t MaxScale;
uchar work_raster[max_raster];

static uchar make_fill[] = {0, 1, 3, 7, 15, 31, 63, 127, 255};

extern uint16_t gwLowRC_rrec;
extern uchar*  lnOcrPath;
/*********************************************************************************************/
Bool32 rec_init(RRecComControl control, char *spath, uchar lang);
void recog(Handle hCCOM, uint32_t flags);
void recog_evn(CCOM_comp* pcomp, bool if_use_gra);
void make_raster(CCOM_comp* pcomp);
//void recog_gra(RecVersions *v, CCOM_comp* pcomp);
void getExtComp(CCOM_comp* pcomp, /*ExtComponent*/CCOM_comp* ec);

static void align8_lines(uchar *bin,int32_t w, int32_t h);

extern Bool16 rec_is_language(uchar);
extern Bool16 rec_set_alpha(uchar, uchar*);
extern Bool16 rec_load_tables(uchar);
//extern int32_t rec_gra_type_rec(uchar);
/*********************************************************************************************/
RRECCOM_FUNC(Bool32)  RRECCOM_Recog(Handle hCCOM, RRecComControl Control,char *spath, uchar lang)
{
	if (!rec_init(Control, (char*)lnOcrPath, lang)) return FALSE;

	recog(hCCOM, Control.flags);

#ifdef _USE_GRA_
//	if (Control.flags & RECOG_GRA) GraDone();
#endif
	return TRUE;
}
/*********************************************************************************************/
Bool32 rec_init(RRecComControl control, char *spath, uchar lang)
{
	if (control.MaxCompWid > 0) comp_max_w = control.MaxCompWid;
	else comp_max_w = RASTER_MAX_WIDTH ;

	if (control.MaxCompHei > 0) comp_max_h = control.MaxCompHei;
	else comp_max_h = RASTER_MAX_HEIGHT;

	if (control.MinCompWid > 0) comp_min_w = control.MinCompWid;
	else comp_min_w = 0;

	if (control.MinCompHei > 0) comp_min_h = control.MinCompHei;
	else comp_min_h = 0;

	if (control.MaxScale > 0) MaxScale = control.MaxScale;
	else MaxScale = 5; //  for cuneiform pitures process

	if (control.flags & RECOG_EVN)
		if (chdir(spath) == 0)
		{
			if (!rec_is_language(lang))
			{
				gwLowRC_rrec=RRECCOM_ERR_NOLANGUAGE;
				return FALSE;
			}

			if (!rec_set_alpha(lang, alphabet))
			{
				gwLowRC_rrec=RRECCOM_ERR_NOSETALPHABET;
				return FALSE;
			}

			EVNSetAlphabet((char*)alphabet);

			if (!rec_load_tables(lang))
			{
				gwLowRC_rrec=RRECCOM_ERR_NOINITEV;
				return FALSE;
			}
		}

	return TRUE;
}
/*********************************************************************************************/
void recog(Handle hCCOM, uint32_t flags)
{
	CCOM_comp* pcomp;

	pcomp = CCOM_GetFirst(hCCOM, NULL);

	while (pcomp)
	{
		if ((flags & RECOG_EVN) != 0) recog_evn(pcomp, (flags & RECOG_GRA) != 0);

		pcomp = CCOM_GetNext(pcomp, NULL);
	}
}
/*********************************************************************************************/
void recog_evn(CCOM_comp* pcomp, bool if_use_gra)
{
	unsigned char evn_res[17]="", gra_res[17]="";
	int32_t nvers = 0;
	CCOM_comp comp = *pcomp;
	int i;//, j;
	CCOM_comp ec;

	comp.scale = pcomp->scale;
	comp.w = pcomp->w;
	comp.rw = pcomp->rw;
	comp.h = pcomp->h;

	if (comp.scale < 3 && (comp.w>>comp.scale) < comp_max_w &&
		(comp.h>>comp.scale) < comp_max_h)
    {
		int w = comp.w, h = comp.h, wb = comp.rw;

		if (comp.scale)
        {
			comp.w >>= comp.scale;
			comp.h >>= comp.scale;
			comp.rw = (comp.w+7)/8;
        }

		getExtComp(&comp, &ec);

		nvers = (int16_t)EVNRecog_lp(&ec, comp.linerep + sizeof(int16_t), comp.size_linerep - sizeof(int16_t), evn_res);
//-		nvers = (int16_t)EVNRecog_lp(&comp, comp.linerep + sizeof(int16_t), comp.size_linerep - sizeof(int16_t), evn_res);

//		pcomp->type |= ec.type;
		pcomp->type = ec.type;
		pcomp->cs = ec.cs;
//-		pcomp->type = comp.type;
//-		pcomp->cs = comp.cs;
    }

	if (nvers)
	{
		if (!pcomp->vers)
		{
			pcomp->vers = (RecVersions*)malloc(sizeof(RecVersions));
			memset(pcomp->vers, 0, sizeof(RecVersions));
		}

		if (pcomp->cs == 255) nvers >>= 1;

		int32_t vers_beg = pcomp->vers->lnAltCnt;

		if (nvers + pcomp->vers->lnAltCnt > REC_MAX_VERS)
			nvers = REC_MAX_VERS - pcomp->vers->lnAltCnt;

		pcomp->vers->lnAltCnt += nvers;

		if (pcomp->cs==255)
			for(i=0; i < nvers; i++)
			{
				pcomp->vers->Alt[vers_beg+i].Code = evn_res[2*i];
				pcomp->vers->Alt[vers_beg+i].Prob = evn_res[2*i+1];
				pcomp->vers->Alt[vers_beg+i].Method = 13;
			} // network collection
		else
			for(i=0; i < nvers; i++)
		    {
				pcomp->vers->Alt[vers_beg+i].Code    = evn_res[i];
		        pcomp->vers->Alt[vers_beg+i].Prob    = 255;
			    pcomp->vers->Alt[vers_beg+i].Method  = 5;
			} // event collection
	}
}
/*********************************************************************************************/
void make_raster(CCOM_comp* pcomp)
{
	CCOM_lnhead* lp;
	CCOM_interval* ip;
	uchar *p, *pp;
	int16_t x, l, sh;
	uint16_t w;

	memset (work_raster, 0, pcomp->rw*pcomp->h);
	lp = (CCOM_lnhead*)((char *)pcomp->linerep + sizeof(int16_t));

	while (lp->lth)
	{
		pp = work_raster + lp->row*pcomp->rw;
		ip = (CCOM_interval*)&lp[1];

		while (x = ip->e, (l = ip->l) != 0)
		{
			p = pp + (x>>3); sh = x & 7;

			while (l > 8) { w = 0xff00 >> sh; *p |= w & 0xff; *(--p) |= w>>8; l-=8; }

			w = make_fill[l]; w = w << (8-sh); *p |= w & 0xff; *(p-1) |= w >> 8;
			pp += pcomp->rw;
			ip++; //+= sizeof(CCOM_interval);
		}

		lp = (CCOM_lnhead*)(ip + 1);
	}
}
/*********************************************************************************************/
static void align8_lines(uchar *bin,int32_t w, int32_t h)
{
	int i,ii,iii, wb=(w+7)/8, wb_new=((w+63)/64)*8;
	uchar   buf[256];

	memset(buf,0,wb_new);

	for(iii=(h-1)*wb_new,ii=(h-1)*wb,i=0;i<h;i++,ii-=wb,iii-=wb_new)
    {
		memcpy(buf,&bin[ii],wb);
		memcpy(&bin[iii],buf,wb_new);
    }
}
/*********************************************************************************************/
void getExtComp(CCOM_comp* pcomp, /*ExtComponent*/CCOM_comp* ec)
{
	memset(ec, 0, sizeof(ExtComponent));

	ec->h = pcomp->h;
	ec->w = pcomp->w;
	ec->rw = pcomp->rw;
	ec->nl = pcomp->nl;
	ec->begs = pcomp->begs;
	ec->ends = pcomp->ends;
	ec->scale = pcomp->scale;
}
/*********************************************************************************************/
RRECCOM_FUNC(Bool32) RRECCOM_IsLanguage(uchar language)
{
	chdir((char*)lnOcrPath);

	return rec_is_language(language);
}
