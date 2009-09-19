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

// StatSearchBL.cpp : implementation file
//
#include <memory.h>
/*#include <windows.h>
#include <Winbase.h>*/
#include <string.h>
#include <stdlib.h>

#if defined(__APPLE__) && defined(__MACH__)
#include <malloc/malloc.h>
#define malloc_usable_size(a) malloc_size(a)

#elif defined(__FreeBSD__)
#include <malloc_np.h>

#elif defined(WIN32)
#include <malloc.h>
#define malloc_usable_size(a) _msize(a)

#else
#include <malloc.h>
#endif

//#include "puma.h"
#include "dpuma.h"
#include "statsearchbl.h"
#include "gistogramma.h"

#define Pb1 0.05
#define Pb2 0.93
#define Pb3 0.88
#define Pb4 0.11
#define	G   1
#define GE  2
#define LNCNST 100

#include "minmax.h"

int16_t minrow;

void stat_def_b3_b4(CSTR_line line, CSTR_attr *p_attr,
			   int16_t *p_Y2Arr, int16_t *p_FreqY2Arr, int16_t *Ns3, int16_t *Ns4)
{
	if(line == 0 || p_attr == NULL || p_Y2Arr == NULL || p_FreqY2Arr == NULL) return;

	ROW_STRUCT row_str;

	int16_t sz_FreqY2Arr = malloc_usable_size(p_FreqY2Arr) / sizeof(int16_t);
	int16_t     sz_Y2Arr = malloc_usable_size(p_Y2Arr) / sizeof(int16_t);
//	if(p_attr->number == 39) DebugBreak();// && p_attr->language == 0

	//главный максимум
	int16_t      min_Y2Arr = p_Y2Arr[stat_index_GlobMin(p_Y2Arr,sz_Y2Arr)];
	int16_t  max_FreqY2Arr = stat_index_GlobMax(p_FreqY2Arr,sz_FreqY2Arr, G);

	row_str.globMax = min_Y2Arr + max_FreqY2Arr;
	row_str.ValGM   = p_FreqY2Arr[max_FreqY2Arr];

	//правый максимум (относительно главного максимума)
	int16_t right_diff = stat_Right_diff_from_max(p_FreqY2Arr, max_FreqY2Arr);

	right_diff = MAX(3, right_diff);

	int16_t rightMax_FreqY2Arr = stat_index_RightLocMax(p_FreqY2Arr, sz_FreqY2Arr,
													max_FreqY2Arr + right_diff);
	//проверка уровня полуопущенных букв
	if(p_attr->language != 0 && rightMax_FreqY2Arr != -1 &&
		stat_control_semilevel(line, p_attr->row, min_Y2Arr + rightMax_FreqY2Arr))
	{
		int16_t itmp = rightMax_FreqY2Arr + 1;
		if(itmp < sz_FreqY2Arr)
			rightMax_FreqY2Arr = stat_index_RightLocMax(p_FreqY2Arr, sz_FreqY2Arr, itmp);
		else rightMax_FreqY2Arr = sz_FreqY2Arr;
	}

	row_str.rightLocMax = rightMax_FreqY2Arr != -1 ? min_Y2Arr + rightMax_FreqY2Arr : -1;
	row_str.ValRLM = rightMax_FreqY2Arr != -1 ? p_FreqY2Arr[rightMax_FreqY2Arr] : -1;

	//левый максимум (относительно главного максимума)
	int16_t  left_diff =  stat_Left_diff_from_max(p_FreqY2Arr, max_FreqY2Arr);
	left_diff = MAX(left_diff, 3);

	int16_t leftMax_FreqY2Arr = stat_index_LeftLocMax(p_FreqY2Arr, max_FreqY2Arr - left_diff);

	row_str.leftLocMax = leftMax_FreqY2Arr != -1 ? min_Y2Arr + leftMax_FreqY2Arr : -1;
	row_str.ValLLM     = leftMax_FreqY2Arr != -1 ? p_FreqY2Arr[leftMax_FreqY2Arr] : -1;

#define BSL4 4

	if(stat_control_b3_by_recog(line, p_attr->row, &row_str) < BSL4)
	{
		//третья линия
		p_attr->bs3 = row_str.globMax;
		Ns3[0] = row_str.ValGM;
		p_attr->Nb3 = (short)MIN(100,(float)100 * row_str.ValGM /
																(float)(sz_Y2Arr * Pb3));
		//четвертая линия
		if(row_str.rightLocMax != -1)
		{
			p_attr->bs4 = row_str.rightLocMax;
			Ns4[0] = row_str.ValRLM;
			p_attr->Nb4 = (short)MIN(100, (float)100 * row_str.ValRLM /
															(float)(sz_Y2Arr * Pb4));
		}
		else
		{
			p_attr->bs4 = -1;
			Ns4[0] = -1;
			p_attr->Nb4 = -1;
		}
	}
	else
	{
		//четвертая линия
		p_attr->bs4 = row_str.globMax;;
		Ns4[0] = row_str.ValGM;
		p_attr->Nb4 = (short)MIN(100,(float)100 * row_str.ValGM /
																(float)(sz_Y2Arr * Pb4));

		//третья линия
		if(row_str.leftLocMax != -1)
		{
			p_attr->bs3 = row_str.leftLocMax;
			Ns3[0] = row_str.ValLLM;
			p_attr->Nb3 = (short)MIN(100,(float)100 * row_str.ValLLM /
																(float)(sz_Y2Arr * Pb3));
		}
		else
		{
			p_attr->bs3 = p_attr->bs4 - 5;
			Ns3[0] = -1;
			p_attr->Nb3 = -1;
		}
	}
}

Bool stat_control_semilevel(CSTR_line line, int16_t ln_row, int16_t b4)
{
	CSTR_attr line_attr;
	int16_t iret = 0, iret4 = 0;

	char buff[] = "\\/дДщЩцЦ";
	Bool flg = FALSE;

	if(CSTR_GetLineAttr(line,&line_attr))
	{
		CSTR_rast  start = CSTR_GetFirstRaster(line), stop = CSTR_GetLastRaster(line);

		if(start == NULL || stop == NULL) return FALSE;

		for(CSTR_rast curr_raster = CSTR_GetNextRaster(start, CSTR_f_let);
		curr_raster && curr_raster!=stop;
		curr_raster = CSTR_GetNextRaster(curr_raster, CSTR_f_let))
		{
			CSTR_rast_attr rast_attr;
			UniVersions vers;

			if(CSTR_GetAttr(curr_raster, &rast_attr) &&
			   CSTR_GetCollectionUni(curr_raster, &vers))
			{
				if(vers.lnAltCnt <= 0 ||
					rast_attr.flg == CSTR_f_space)
				{
					continue;
				}

				int16_t drow4 = rast_attr.row + rast_attr.h - ln_row - b4;

				Bool16 b = FALSE;

				if(abs(drow4) <= 2)
				{
					for(int i = 0; i < vers.lnAltCnt; i++)
					{
						char c = vers.Alt[i].Code[0];
						char *p = strchr(buff, c);

						if(p)
						{
							b = TRUE;
							break;
						}
					}

					if(b) iret++;
//					else iret4++;
				}
			}
		}
	}

	if(iret != 0 || iret4 != 0) flg = iret > iret4 ? TRUE : FALSE;

	return flg;
}

Bool stat_interp_bsl_by_boxes(CSTR_line line, int16_t ln_row, int16_t b3, int16_t b4, ROW_STRUCT *row_str)
{
	CSTR_attr line_attr;
	if(!CSTR_GetLineAttr(line,&line_attr)) return FALSE;

	char buff[] = "\\/|<>йЙдДщЩцЦ";

	int16_t countLLM1 = 0, countGM1 = 0, countGM2 = 0, countRLM2 = 0;

	//интервалы
	int16_t pnt0, pnt1, pnt2, pnt3;

	if(row_str->leftLocMax != -1)
	{
		pnt0 = (int16_t)(row_str->leftLocMax - 0.5 * (row_str->globMax - row_str->leftLocMax));
		pnt1 = (int16_t)(row_str->globMax - 0.5 * (row_str->globMax - row_str->leftLocMax));
	}
	else
	{
		if(row_str->rightLocMax != -1)
			pnt1 = (int16_t)(row_str->globMax - 0.5 * (row_str->rightLocMax - row_str->globMax));
		else
			pnt1 = (int16_t)(row_str->globMax - 0.5 * (b3 - row_str->globMax));
	}

	if(row_str->rightLocMax != -1)
	{
		pnt2 = (int16_t)(row_str->globMax + 0.5 * (row_str->rightLocMax - row_str->globMax));
		pnt3 = (int16_t)(row_str->rightLocMax + 0.5 * (b3 - row_str->rightLocMax));
	}
	else
	{
		pnt2 = (int16_t)(row_str->globMax + 0.5 * (b3 - row_str->globMax));
		pnt3 = pnt2;
	}

	CSTR_rast  start = CSTR_GetFirstRaster(line), stop = CSTR_GetLastRaster(line);

	if(start == NULL || stop == NULL) return FALSE;

	for(CSTR_rast curr_raster = CSTR_GetNextRaster(start, CSTR_f_let| CSTR_f_bad); //отладка
	curr_raster && curr_raster!=stop;
	curr_raster = CSTR_GetNextRaster(curr_raster, CSTR_f_let | CSTR_f_bad)) //
	{
		CSTR_rast_attr rast_attr;

		if(CSTR_GetAttr(curr_raster, &rast_attr))
		{
			if(rast_attr.flg != CSTR_f_dust &&
			   rast_attr.flg != CSTR_f_punct &&
			   rast_attr.flg != CSTR_f_space)
			{
				Bool flg = TRUE;
				UniVersions vers;

				if(CSTR_GetCollectionUni(curr_raster, &vers))
				{
					for(int i = 0; i < vers.lnAltCnt; i++)
					{
						char c = vers.Alt[i].Code[0];
						char *p = strchr(buff, c);

						if(p)
						{
							flg = FALSE;
							break;
						}
					}
				}

				if(!flg) continue;

				int16_t drow = rast_attr.row - ln_row;
				int16_t  db3 = drow + rast_attr.h - b3;
				int16_t  db4 = b4 != -1 ? drow + rast_attr.h - b4 : -1;

				if(!(abs(db3) <= 3 || (abs(db4) <= 3 && b4 != -1))) continue;

				if(pnt1 <= drow && drow < pnt2)
				{
					if(row_str->AltGM == 1) countGM1++;
					else countGM2++;
				}
				else
				{
					if(row_str->rightLocMax != -1 && pnt2 <= drow && drow <= pnt3)
					{
						countRLM2++;
					}

					if(row_str->leftLocMax != -1 && pnt0 <= drow && drow <= pnt1)
					{
						countLLM1++;
					}
				}
			}
		}
	}

	if(countGM1 + countRLM2 != 0 || countLLM1 + countGM2 != 0)
	{
		if(countGM1 + countRLM2 > countLLM1 + countGM2)
		{
			row_str->AltGM  = 1;
			row_str->AltRLM = countRLM2 > 0 ? 2 : -1;
			row_str->AltLLM = -1;
		}
		else
		{
			row_str->AltGM  =  2;
			row_str->AltRLM = -1;
			if(countLLM1 > 0)
				row_str->AltLLM = 1;
		}
	}

	return TRUE;
}

int16_t stat_control_b3_by_recog(CSTR_line line, int16_t ln_row, ROW_STRUCT *row_str)
{
	CSTR_attr line_attr;
	int16_t iret = -1;

	char buff_figure[] = "_0123456789";

	char buff_eng_high[]= "ABCDEFGHIJKLMNORSTUVWXZ";
	char buff_eng_low[] = "abcdefhiklmnorstuvwxz";//g

	char buff_rus_high[]= "АБВГЕЖЗИЙКЛМНОПСТХЧЫЪЬЭЮЯ";
	char buff_rus_low[] = "абвгежзийклмнопстхчыъьэюя";

	int16_t countLLM3 = 0, countGM3 = 0, countRLM3 = 0;

	//интервалы
	int16_t pnt0, pnt1, pnt2, pnt3;

	if(row_str->leftLocMax != -1)
	{
		pnt0 = (int16_t)(row_str->leftLocMax - 0.5 * (row_str->globMax - row_str->leftLocMax));
		pnt1 = (int16_t)(row_str->globMax - 0.5 * (row_str->globMax - row_str->leftLocMax));
	}
	else
	{
		if(row_str->rightLocMax != -1)
			pnt1 = (int16_t)(row_str->globMax - 0.5 * (row_str->rightLocMax - row_str->globMax));
		else
			pnt1 = (int16_t)(row_str->globMax - 0.5 * (row_str->globMax));
		pnt0 = pnt1;
	}

	if(row_str->rightLocMax != -1)
	{
		pnt2 = (int16_t)(row_str->globMax + 0.5 * (row_str->rightLocMax - row_str->globMax));
		pnt3 = (int16_t)(row_str->rightLocMax + 0.5 * (row_str->rightLocMax - row_str->globMax));
	}
	else
	{
		pnt2 = (int16_t)(row_str->globMax + 0.5 * (row_str->globMax));
		pnt3 = pnt2;
	}

	if(CSTR_GetLineAttr(line,&line_attr))
	{
//		if(line_attr.number == 5) DebugBreak();//  && p_attr->language == 0
		CSTR_rast  start = CSTR_GetFirstRaster(line), stop = CSTR_GetLastRaster(line);

		if(start == NULL || stop == NULL) return -1;

		for(CSTR_rast curr_raster = CSTR_GetNextRaster(start, CSTR_f_let);
		curr_raster && curr_raster!=stop;
		curr_raster = CSTR_GetNextRaster(curr_raster, CSTR_f_let))
		{
			CSTR_rast_attr rast_attr;
			UniVersions vers;

			if(CSTR_GetAttr(curr_raster, &rast_attr) &&
			   CSTR_GetCollectionUni(curr_raster, &vers))
			{
				Bool flg = TRUE;
				int16_t iNl = 0, iWl = 0;

				if(vers.lnAltCnt == 0)
				{
					continue;
				}

				for(int i = 0; i < vers.lnAltCnt; i++)
				{
					if(vers.Alt[i].Code[0] == 0 || vers.Alt[i].Prob == 0)
					{
						flg = FALSE;
						break;
					}

					char c = vers.Alt[i].Code[0];

					char *peng_low = strchr(buff_eng_low, c);
					char *prus_low = strchr(buff_rus_low, c);
					char *pfig = strchr(buff_figure, c);
					char *peng_high = strchr(buff_eng_high, c);
					char *prus_high = strchr(buff_rus_high, c);

					if(peng_low == NULL && prus_low == NULL &&
					   pfig == NULL && peng_high == NULL && prus_high == NULL)
					{
/*						flg = FALSE;
						break;*/
						iWl++;
					}
					else
					{
						iNl++;
					}
				}

				if(iWl > 0)
				{
					if((float)iNl / (float)iWl <= 2) flg = FALSE;
				}

				if(flg)
				{
					int16_t d = rast_attr.row + rast_attr.h - ln_row;

					if(pnt1 <= d && d <= pnt2)
					{
						countGM3++;
						continue;
					}
					else
					{
						if(row_str->leftLocMax != -1 && pnt0 <= d && d < pnt1)
						{
							countLLM3++;
							continue;
						}
						else
						{
							if(row_str->rightLocMax != -1 && pnt2 < d && d <= pnt3)
							{
								countRLM3++;
							}
						}
					}
				}
			}
		}
	}

	if(countLLM3 > 0 && countGM3 == 0) iret = 4;

	if(countRLM3 > 0 && countGM3 >= 0 && row_str->ValRLM == row_str->ValGM)
	{
		row_str->globMax     = row_str->rightLocMax;
//		row_str->ValGM       = row_str->ValRLM;
		row_str->rightLocMax = -1;
		row_str->ValRLM      = -1;
		iret = 3;
	}

	if(countRLM3 == 0 && countGM3 > 0) iret = 3;

	return iret;
}

Bool stat_interp_bsl(CSTR_line line, CSTR_attr *p_attr, ROW_STRUCT *row_str)
{
	int16_t  rGM[3] = {0, 0, 0};
	int16_t rLLM[3] = {0, 0, 0};
	int16_t rRLM[3] = {0, 0, 0};

	Bool bret = FALSE;

	row_str->AltLLM = -1;
	row_str->AltRLM = -1;
	row_str->AltGM  = -1;

//	if(p_attr->number == 330) DebugBreak();// && p_attr->language == 0
	if(stat_interp_bsl_by_solid_recog(line, p_attr->row, p_attr->bs3, row_str))
	{
		if(row_str->AltGM != -1 &&
			row_str->leftLocMax  == -1 && row_str->ValLLM == -1 &&
			row_str->rightLocMax == -1 && row_str->ValRLM == -1)
		{
			bret = TRUE;
		}

		if(row_str->AltLLM != -1) rLLM[row_str->AltLLM]++;
		if(row_str->AltRLM != -1) rRLM[row_str->AltRLM]++;
		if(row_str->AltGM  != -1) rGM[row_str->AltGM]++;
	}

	if(!bret)
	{
		if(p_attr->Nb4 > 0)
		{
			if(stat_interp_bsl_by_b4(line, p_attr->row, p_attr->bs3, p_attr->bs4, row_str))
			{
				if(row_str->AltLLM != -1) rLLM[row_str->AltLLM]++;
				if(row_str->AltRLM != -1) rRLM[row_str->AltRLM]++;
				if(row_str->AltGM  != -1) rGM[row_str->AltGM]++;
			}
		}

		if(row_str->globMax != -1 && row_str->leftLocMax != -1 && row_str->rightLocMax != -1)
		{
			if(stat_interp_bsl_by_boxes(line, p_attr->row, p_attr->bs3, p_attr->bs4, row_str))
			{
				if(row_str->AltLLM != -1) rLLM[row_str->AltLLM]++;
				if(row_str->AltRLM != -1) rRLM[row_str->AltRLM]++;
				if(row_str->AltGM  != -1) rGM[row_str->AltGM]++;
			}
		}
		else
		{
			if(row_str->AltGM == -1 && row_str->leftLocMax == -1 && row_str->rightLocMax != -1)
			{
				rGM[1]++; rRLM[2]++;
			}

			if(row_str->AltGM == -1 && row_str->rightLocMax == -1 && row_str->leftLocMax != -1)
			{
				rGM[2]++; rLLM[1]++;
			}

			if(row_str->AltGM == -1 && row_str->rightLocMax == -1 && row_str->leftLocMax == -1)
			{
				int16_t hypothesis = stat_Hypothesis(p_attr->number, p_attr->bs3 - row_str->globMax);

				if(hypothesis != -1) rGM[hypothesis]++;
			}
		}
	}

	int16_t resGM  = stat_index_GlobMax(rGM, 3, GE);
	int16_t resRLM = stat_index_GlobMax(rRLM, 3, GE);
	int16_t resLLM = stat_index_GlobMax(rLLM, 3, GE);

	row_str->AltLLM = rLLM[resLLM] > 0 ? resLLM : -1;
	row_str->AltRLM = rRLM[resRLM] > 0 ? resRLM : -1;
	row_str->AltGM  =   rGM[resGM] > 0 ? resGM  : -1;

	return TRUE;
}
/**/
Bool stat_interp_bsl_by_solid_recog(CSTR_line line, int16_t ln_row, int16_t b3, ROW_STRUCT *row_str)
{
	CSTR_attr line_attr;

	char buff_eng_high1[] = "ABDEFGHIKLMNRTbdfghklpPyYjJ0O23456789";
	char buff_eng_t[]     = "t";
	char buff_eng_high2[] = "CQSUVWXZ";
	char buff_eng_low1[]  = "aeimnr";
	char buff_eng_low2[]  = "cosuvwxz";//q

	char buff_rus_high1[] = "АБЕбФфРрУу0О23З456789";
	char buff_rus_high2[] = "ВГЖИКЛМНОПСТХЧШЪЫЬЭЮЯ";
	char buff_rus_low1[]  = "ае";
	char buff_rus_low2[]  = "вгжзиклмнопстхчшъыьэюя";
	char buff_rus_unk[]   = "д"; //отладка

	char buff_warning[] = "\\/|<>(){}[]йЙ";

	int16_t iLow1 = 0, iHigh1 = 0, iLow2 = 0, iHigh2 = 0, iLvlt = 0, iWarn = 0, iUnk = 0;

	int16_t countLLM1 = 0, countLLM2 = 0, countGM1 = 0, countGM2 = 0, countRLM2 = 0, countRLM1 = 0,
		  countWarn1 = 0, countHighL1 = 0, countWarn2 = 0, countHighL2 = 0;

	//интервалы
	int16_t pnt0, pnt1, pnt2, pnt3;

	if(row_str->leftLocMax != -1 || row_str->ValLLM != -1)
	{
		pnt0 = (int16_t)(row_str->leftLocMax - 0.5 * (row_str->globMax - row_str->leftLocMax));
		pnt1 = (int16_t)(row_str->globMax - 0.5 * (row_str->globMax - row_str->leftLocMax));
	}
	else
	{
		if(row_str->rightLocMax != -1)
			pnt1 = (int16_t)(row_str->globMax - 0.5 * (row_str->rightLocMax - row_str->globMax));
		else
			pnt1 = (int16_t)(row_str->globMax - 0.5 * (b3 - row_str->globMax));
		pnt0 = pnt1;
	}

	if(row_str->rightLocMax != -1)
	{
		pnt2 = (int16_t)(row_str->globMax + 0.5 * (row_str->rightLocMax - row_str->globMax));
		pnt3 = (int16_t)(row_str->rightLocMax + 0.5 * (b3 - row_str->rightLocMax));
	}
	else
	{
		pnt2 = (int16_t)(row_str->globMax + 0.5 * (b3 - row_str->globMax));
		pnt3 = pnt2;
	}

	if(CSTR_GetLineAttr(line,&line_attr))
	{
		CSTR_rast  start = CSTR_GetFirstRaster(line), stop = CSTR_GetLastRaster(line);

		if(start == NULL || stop == NULL) return FALSE;

		for(CSTR_rast curr_raster = CSTR_GetNextRaster(start, CSTR_f_let);
		curr_raster && curr_raster!=stop;
		curr_raster = CSTR_GetNextRaster(curr_raster, CSTR_f_let))
		{
			CSTR_rast_attr rast_attr;
			UniVersions vers;

			if(CSTR_GetAttr(curr_raster, &rast_attr) &&
			   CSTR_GetCollectionUni(curr_raster, &vers))
			{
				if(vers.lnAltCnt <= 0 || rast_attr.flg == CSTR_f_space)
				{
					continue;
				}

				iWarn = 0; iUnk = 0, iHigh1 = 0; iLow1  = 0; iHigh2 = 0; iLow2  = 0, iLvlt = 0;

				for(int i = 0; i < vers.lnAltCnt; i++)
				{
					char *peng_low1, *prus_low1, *peng_high1, *prus_high1,
						 *peng_low2, *prus_low2, *peng_high2, *prus_high2,
						 *peng_t, *prus_unk, *p_warning;

					peng_low1 = prus_low1 = peng_high1 = prus_high1 =
					peng_low2 = prus_low2 = peng_high2 = prus_high2 =
					peng_t = prus_unk  = p_warning = NULL;

					for(int16_t j = 0; j < 3; j++)
					{
						char c = vers.Alt[i].Code[j];
						if(c == 0) break;

						prus_low1  = strchr(buff_rus_low1,  c);
						prus_high1 = strchr(buff_rus_high1, c);

						peng_low1  = strchr(buff_eng_low1,  c);
						peng_high1 = strchr(buff_eng_high1, c);
						peng_t     = strchr(buff_eng_t, c);

						prus_low2  = strchr(buff_rus_low2,  c);
						prus_high2 = strchr(buff_rus_high2, c);
						prus_unk   = strchr(buff_rus_unk,   c);

						peng_low2  = strchr(buff_eng_low2,  c);
						peng_high2 = strchr(buff_eng_high2, c);

						p_warning  = strchr(buff_warning,   c);

						if(p_warning) break;
					}

					if(p_warning)
					{
						iWarn++;
						iHigh1 = 0;
						iLow1  = 0;
						iHigh2 = 0;
						iLow2  = 0;
						break;
					}
					else
					{
						if(prus_high1 != NULL || peng_high1 != NULL)
						{
							iHigh1++;
							continue;
						}

						if(prus_high2 != NULL || peng_high2 != NULL)
						{
							iHigh2++;
							continue;
						}

						if(peng_t != NULL)
						{
							iLvlt++;
							continue;
						}

						if(prus_low1 != NULL || peng_low1 != NULL)
						{
							iLow1++;
							continue;
						}

						if(prus_low2 != NULL || peng_low2 != NULL)
						{
							iLow2++;
							continue;
						}

						if(prus_unk != NULL)
						{
							iUnk++;
							continue;
						}
					}
				}

				int16_t  db3 = rast_attr.row + rast_attr.h - ln_row - b3;

				if(abs(db3) <= 2 || iWarn > 0)
				{
					int16_t drow = rast_attr.row - ln_row;

					if(iLow1 > 0 && iHigh1 + iHigh2 == 0 && iUnk == 0)
					{
						if(pnt1 <= drow && drow <= pnt2)
						{
							countGM2++;
							continue;
						}
						else
						{
							if(row_str->rightLocMax != -1 && pnt2 < drow && drow <= pnt3)
							{
								countRLM2++;
								continue;
							}
							else
							{
								if(row_str->leftLocMax != -1 && pnt0 <= drow && drow < pnt1)
								{
									countLLM2++;
									continue;
								}
							}
						}
					}

					if(iLow1 + iLow2 == 0 && iHigh1 > 0 && iLvlt == 0)
					{
						if(pnt1 <= drow && drow <= pnt2)
						{
							countGM1++;
							continue;
						}
						else
						{
							if(row_str->leftLocMax != -1 && pnt0 <= drow && drow < pnt1)
							{
								countLLM1++;
								continue;
							}
							else
							{
								if(row_str->rightLocMax != -1 && pnt2 <= drow && drow <= pnt3)
								{
									countRLM1++;
									continue;
								}
							}
						}
					}

					if(iLow1 + iLow2 == 0 && iHigh1 >= 0 && iLvlt > 0)
					{
						if(iHigh1 == 0) drow -= 2;

						if(pnt1 <= drow && drow <= pnt2)
						{
							countGM1++;
							continue;
						}
						else
						{
							if(row_str->leftLocMax != -1 && pnt0 <= drow && drow < pnt1)
							{
								countLLM1++;
								continue;
							}
							else
							{
								if(row_str->rightLocMax != -1 && pnt2 <= drow && drow <= pnt3)
								{
									countRLM1++;
									continue;
								}
							}
						}
					}

					if(row_str->leftLocMax != -1 && pnt0 <= drow && drow < pnt1)
					{
						if(iWarn > 0) countWarn1++;
						else countHighL1++;
					}

					if(pnt1 <= drow && drow < pnt2)
					{
						if(iWarn > 0) countWarn2++;
						else countHighL2++;
					}
				}
			}
		}

		if(countGM1 + countRLM2 > 0 ||
			(countLLM1 + countGM2 > 0 &&
			(countLLM2 == 0 || row_str->globMax - row_str->leftLocMax >= 4)))
		{
			if(countGM1 + countRLM2 > countLLM1 + countGM2 ||
				(countGM2 == 0 && countGM1 > 1 && row_str->ValGM > (short)(1.1 * row_str->ValLLM)))// && row_str->rightLocMax != -1
			{
				row_str->AltGM  = 1;
				row_str->AltRLM = countRLM2 > 0 ? 2 : -1;

				row_str->AltLLM = -1;
				row_str->leftLocMax = -1;
				row_str->leftCoeff = -1;
				row_str->ValLLM = -1;
			}
			else
			{
				if(countGM1 + countRLM2 == countLLM1 + countGM2)
				{
					int16_t iHyp = stat_Hypothesis(line_attr.number, b3 - row_str->globMax);
					if(iHyp == 1)
					{
						row_str->AltGM  = 1;
						row_str->AltRLM = countRLM2 > 0 ? 2 : -1;
						row_str->AltLLM = -1;
					}
					else
					{
						row_str->AltGM  =  2;
						row_str->AltRLM = -1;
						row_str->AltLLM = countLLM1 > 0 ? 1 : -1;
					}
				}
				else
				{
					row_str->AltGM  =  2;
					row_str->AltRLM = -1;
					row_str->AltLLM = countLLM1 > 0 ? 1 : -1;
				}
			}
		}
		else
		{
			if(countGM1 == 0 && countRLM1 > 0)
				row_str->AltRLM = 1;
		}

		if((countHighL1 == 0 && countWarn1 > 0) || (countHighL2 == 0 && countWarn2 > 0) ||
			(countLLM2 > 0 && row_str->globMax - row_str->leftLocMax < 4))
		{
			if((countHighL1 == 0 && countWarn1 > 0) || (countHighL2 == 0 && countWarn2 > 0))
			{
				countWarn1 = 0, countHighL1 = 0, countWarn2 = 0, countHighL2 = 0;
				for(CSTR_rast curr_raster = CSTR_GetNextRaster(start, CSTR_f_let | CSTR_f_bad);
				curr_raster && curr_raster!=stop;
				curr_raster = CSTR_GetNextRaster(curr_raster, CSTR_f_let | CSTR_f_bad))
				{
					CSTR_rast_attr rast_attr;
					UniVersions vers;

					char *p_warning;

					if(CSTR_GetAttr(curr_raster, &rast_attr) &&
					   CSTR_GetCollectionUni(curr_raster, &vers))
					{
						if(rast_attr.flg == CSTR_f_space) continue;

						p_warning = NULL;

						for(int i = 0; i < vers.lnAltCnt; i++)
						{
							char c = vers.Alt[i].Code[0];

							p_warning = strchr(buff_warning, c);

							if(p_warning) break;
						}

						int16_t  db3 = rast_attr.row + rast_attr.h - ln_row - b3;

						if(abs(db3) <= 2 || p_warning)
						{
							int16_t drow = rast_attr.row - ln_row;

							if(row_str->leftLocMax != -1 && pnt0 <= drow && drow < pnt1)
							{
								if(p_warning) countWarn1++;
								else countHighL1++;
							}

							if(pnt1 <= drow && drow < pnt2)
							{
								if(p_warning) countWarn2++;
								else countHighL2++;
							}
						}
					}
				}
			}

			Bool bdel = FALSE;
			if(row_str->leftLocMax != -1 && row_str->AltLLM == -1 &&
				((row_str->ValLLM <= countWarn1 && countHighL1 == 0) ||
				(countLLM2 > 0 && row_str->globMax - row_str->leftLocMax < 4)))
			{
				if(countLLM2 > 0)
				{
					row_str->AltGM = 2;
					row_str->globMax = row_str->leftLocMax;
					row_str->ValGM   = row_str->ValLLM;
				}

				row_str->AltLLM = -1;
				row_str->leftLocMax = -1;
				row_str->leftCoeff = -1;
				row_str->ValLLM = -1;

				bdel = TRUE;
			}

			if(!bdel && row_str->leftLocMax == -1 && row_str->AltGM == -1 &&
				row_str->rightLocMax != -1 &&
				row_str->ValGM <= countWarn2 && countHighL2 == 0)
			{
				row_str->globMax = row_str->rightLocMax;
				row_str->AltGM   = row_str->AltRLM;
				row_str->ValGM   = row_str->ValRLM;

				row_str->rightLocMax = -1;
				row_str->AltRLM = -1;
				row_str->ValRLM = -1;
				row_str->rightCoeff = -1;
			}
		}
	}
	else return FALSE;

	return TRUE;
}

Bool stat_control_t_level(CSTR_line line, int16_t ln_row, int16_t b3, int16_t bl)
{
	CSTR_attr line_attr;
	Bool bret = FALSE;
	int16_t i1 = 0, i2 = 0;

	if(CSTR_GetLineAttr(line,&line_attr))
	{
		CSTR_rast  start = CSTR_GetFirstRaster(line), stop = CSTR_GetLastRaster(line);

		if(start == NULL || stop == NULL) return FALSE;

		for(CSTR_rast curr_raster = CSTR_GetNextRaster(start, CSTR_f_let);
		curr_raster && curr_raster!=stop;
		curr_raster = CSTR_GetNextRaster(curr_raster, CSTR_f_let))
		{
			CSTR_rast_attr rast_attr;
			UniVersions vers;

			if(CSTR_GetAttr(curr_raster, &rast_attr) &&
			   CSTR_GetCollectionUni(curr_raster, &vers))
			{
				if(vers.lnAltCnt != 1)
				{
					continue;
				}

				Bool flg = FALSE;

				for(int i = 0; i < 4; i++)//отладка vers.lnAltCnt
				{
					char c = vers.Alt[0].Code[i];
					if(c == 0) break;
					else
						if(c == 't') {flg = TRUE; break;}
				}

				if(flg)
				{
					int16_t drow3 = rast_attr.row + rast_attr.h - ln_row - b3;

					if(vers.Alt[0].Prob >= 230 && abs(drow3) <= 2)
					{
						int16_t drowbl = rast_attr.row - ln_row - bl;// + drow3

						if(abs(drowbl) <= 1)
						{
							i1++;
						}
						else
							i2++;
					}
				}
			}
		}
	}

	bret = i1 >= i2 && i1 != 0 ? TRUE : FALSE;
	return bret;
}

Bool stat_interp_bsl_by_b4(CSTR_line line, int16_t ln_row, int16_t b3, int16_t b4, ROW_STRUCT *row_str)
{
	CSTR_attr line_attr;
	if(!CSTR_GetLineAttr(line,&line_attr)) return FALSE;

	char buff1[] = "фФ";
	char buff2[] = "дДщЩцЦ";
	char buff_warning[] = "1\\|/(){}[]";


	int16_t countLLM1 = 0, countGM1 = 0, countGM2 = 0, countRLM2 = 0;
	//интервалы
	int16_t pnt0, pnt1, pnt2, pnt3;

	if(row_str->leftLocMax != -1)
	{
		pnt0 = (int16_t)(row_str->leftLocMax - 0.5 * (row_str->globMax - row_str->leftLocMax));
		pnt1 = (int16_t)(row_str->globMax - 0.5 * (row_str->globMax - row_str->leftLocMax));
	}
	else
	{
		if(row_str->rightLocMax != -1)
			pnt1 = (int16_t)(row_str->globMax - 0.5 * (row_str->rightLocMax - row_str->globMax));
		else
			pnt1 = (int16_t)(row_str->globMax - 0.2 * (b3 - row_str->globMax));//0.5
	}

	if(row_str->rightLocMax != -1)
	{
		pnt2 = (int16_t)(row_str->globMax + 0.5 * (row_str->rightLocMax - row_str->globMax));
		pnt3 = (int16_t)(row_str->rightLocMax + 0.5 * (b3 - row_str->rightLocMax));
	}
	else
	{
		pnt2 = (int16_t)(row_str->globMax + 0.5 * (b3 - row_str->globMax));
		pnt3 = pnt2;
	}

	CSTR_rast  start = CSTR_GetFirstRaster(line), stop = CSTR_GetLastRaster(line);

	if(start == NULL || stop == NULL) return FALSE;

	for(CSTR_rast curr_raster = CSTR_GetNextRaster(start, CSTR_f_let);
	curr_raster && curr_raster!=stop;
	curr_raster = CSTR_GetNextRaster(curr_raster, CSTR_f_let))
	{
		CSTR_rast_attr rast_attr;
		UniVersions vers;

		if(CSTR_GetAttr(curr_raster, &rast_attr) &&
		   CSTR_GetCollectionUni(curr_raster, &vers))
		{
			if(vers.lnAltCnt <= 0 || rast_attr.flg == CSTR_f_space) //отладка
			{
				continue;
			}

			int16_t  db4 = rast_attr.row + rast_attr.h - ln_row - b4;
			int16_t drow = rast_attr.row - ln_row;
			int16_t pnt = b3 != 0 ? (b4 - b3) / 2 : 3;

			if(-pnt <= db4 && db4 <= 3)//отладка vers.Alt[0].Prob >= 230 &&
			{
				Bool flg1 = FALSE, flg2 = FALSE, flg_warning = FALSE;

				for(int i = 0; i < vers.lnAltCnt; i++)
				{
					char c = vers.Alt[i].Code[0];
					char *p1 = strchr(buff1, c);
					char *p2 = strchr(buff2, c);
					char *p_warning = strchr(buff_warning, c);

					if(p1)
					{
						flg1 = TRUE;
						break;
					}

					if(p2 || c == 0)
					{
						flg2 = TRUE;
						break;
					}

					if(p_warning)
					{
						flg_warning = TRUE;
						break;
					}
				}

				if(flg2) continue;

				if(!flg1)
				{
					if(pnt1 <= drow && drow < pnt2)
					{
						if(row_str->AltGM == 1 || flg_warning) countGM1++;//отладка && row_str->leftLocMax == -1
						else countGM2++;
					}
					else
					{
						if(row_str->leftLocMax != -1 && pnt0 <= drow && drow < pnt1)
						{
/*							if(flg_warning)
							{
								drowWarn = drow;
								countWarn++;
							}
							else
*/								countLLM1++;
						}

						if(row_str->rightLocMax != -1 && pnt2 <= drow && drow <= pnt3)
						{
							countRLM2++;
						}
					}
				}
				else
				{
					if(pnt1 <= drow && drow < pnt2)
					{
						countGM1++;
					}
					else
					{
						if(row_str->leftLocMax != -1 && pnt0 <= drow && drow <= pnt1)
						{
							countLLM1++;
						}
					}
				}
			}
		}
	}

	if(countGM1 + countRLM2 != 0 || countLLM1 + countGM2 != 0)
	{
		if(countGM1 + countRLM2 > countLLM1 + countGM2)
		{
			if(countGM1 > 0 || row_str->leftLocMax == -1) row_str->AltGM = 1;
			row_str->AltRLM = countRLM2 > 0 ? 2 : -1;
			row_str->AltLLM = -1;
		}
		else
		{
			if(countGM2 > 0 || row_str->rightLocMax == -1) row_str->AltGM = 2;
			row_str->AltRLM = -1;
			row_str->AltLLM = countLLM1 > 0 ? 1 : -1;

		}
	}

/*	if(row_str->AltLLM == -1 && countWarn > 0 && row_str->ValLLM <= countWarn)
	{
		pnt1 = (int16_t)(row_str->globMax - 0.3 * (b3 - row_str->globMax));

		if(pnt1 <= drowWarn)
		{
			row_str->AltGM = 1;
		}

		row_str->AltLLM = -1;
		row_str->leftLocMax = -1;
		row_str->leftCoeff = -1;
		row_str->ValLLM = -1;
	}
*/
	if(row_str->AltLLM == 1 && row_str->AltGM == 1 && row_str->AltRLM == 2)
	{
		row_str->AltLLM = -1;
		row_str->leftLocMax = -1;
		row_str->leftCoeff = -1;
		row_str->ValLLM = -1;
	}

	return TRUE;
}

void stat_def_b1_b2(CSTR_line line, CSTR_attr *p_attr, int16_t *p_Y1Arr,
			   int16_t *p_Y2Arr, int16_t *p_FreqY1Arr, int16_t *Ns1, int16_t *Ns2)
{
	if(line == 0 || p_attr == NULL || p_Y1Arr == NULL ||p_Y2Arr == NULL || p_FreqY1Arr == NULL) return;

	ROW_STRUCT row_str;

	int16_t sz_FreqY1Arr = malloc_usable_size(p_FreqY1Arr) / sizeof(int16_t);
	int16_t     sz_Y1Arr = malloc_usable_size(p_Y1Arr) / sizeof(int16_t);
	int16_t    min_Y1Arr = p_Y1Arr[stat_index_GlobMin(p_Y1Arr, sz_Y1Arr)];

// 	if(p_attr->number == 161) DebugBreak(); // && p_attr->language == 0
	//глобальный максимум
	int16_t max_FreqY1Arr = stat_index_GlobMax(p_FreqY1Arr,sz_FreqY1Arr, G);

	row_str.globMax = min_Y1Arr + max_FreqY1Arr;
	row_str.ValGM   = p_FreqY1Arr[max_FreqY1Arr];

	int16_t diff_b3_GlobMax = p_attr->bs3 - row_str.globMax;

/*	switch(hipothesis)
	{
		case -1:  //похожая строка не найдена;

		case 12:  //найдены похожие строки, и
		          //глобальный максимум может быть как и первой БЛ, так и второй БЛ;
		case  1:  //похожая строка(и) найдена(ы),
				  //и, предположительно, глобальный максимум - первая БЛ;
		case  2:
		default: //похожая строка(и) найдена(ы),
				 //и, предположительно, глобальный максимум - вторая БЛ.
	}
*/

	//левый (относительно глобального) максимум
	int16_t  left_diff =  stat_Left_diff_from_max(p_FreqY1Arr, max_FreqY1Arr);
	left_diff = MAX(left_diff, 3);

	int16_t leftMax_FreqY1Arr = stat_index_LeftLocMax(p_FreqY1Arr, max_FreqY1Arr - left_diff);
	//проверка t-уровня
	if(p_attr->language == 0 && leftMax_FreqY1Arr != -1)
	{
		if(stat_control_t_level(line, p_attr->row, p_attr->bs3, min_Y1Arr + leftMax_FreqY1Arr)
			/* && leftMax_FreqY1Arr > 0*/)
		{
			int16_t itmp = leftMax_FreqY1Arr - 1;
			if(itmp > 0) leftMax_FreqY1Arr = stat_index_LeftLocMax(p_FreqY1Arr, itmp);
			else leftMax_FreqY1Arr = 0;
		}
	}

	int16_t diff_leftLocMax_GlobMax;

	row_str.leftLocMax		= leftMax_FreqY1Arr != -1 ?  min_Y1Arr + leftMax_FreqY1Arr : -1;
	row_str.ValLLM			= leftMax_FreqY1Arr != -1 ? p_FreqY1Arr[leftMax_FreqY1Arr] : -1;
	diff_leftLocMax_GlobMax = leftMax_FreqY1Arr != -1 ?
												  row_str.globMax - row_str.leftLocMax : -1;

	//пропорции (b2-b1)/(b3-b2)
	if(diff_leftLocMax_GlobMax != -1 && diff_b3_GlobMax != 0)
		row_str.leftCoeff = (float) diff_leftLocMax_GlobMax / diff_b3_GlobMax;

	if(row_str.leftCoeff != -1 && row_str.leftCoeff < 0.15)
	{
		row_str.leftLocMax		= -1;
		row_str.ValLLM			= -1;
		row_str.leftCoeff		= -1;
		diff_leftLocMax_GlobMax = -1;
	}

	//правый максимум
	int16_t right_diff = stat_Right_diff_from_max(p_FreqY1Arr, max_FreqY1Arr);
	right_diff = MAX(right_diff, 3);

	int16_t rightMax_FreqY1Arr = stat_index_RightLocMax(p_FreqY1Arr, sz_FreqY1Arr, max_FreqY1Arr + right_diff);

	int16_t diff_GlobMax_rightLocMax;

	row_str.rightLocMax = rightMax_FreqY1Arr != -1 ?  min_Y1Arr + rightMax_FreqY1Arr : -1;
	row_str.ValRLM      = rightMax_FreqY1Arr != -1 ? p_FreqY1Arr[rightMax_FreqY1Arr] : -1;
	diff_GlobMax_rightLocMax = rightMax_FreqY1Arr != -1 ?
											   row_str.rightLocMax - row_str.globMax : -1;


	//пропорции (b2-b1)/(b3-b2)
	if(diff_GlobMax_rightLocMax != -1 && diff_b3_GlobMax - diff_GlobMax_rightLocMax != 0)
		row_str.rightCoeff = (float)
				diff_GlobMax_rightLocMax / (diff_b3_GlobMax - diff_GlobMax_rightLocMax);

	if(row_str.rightCoeff != -1 && row_str.rightCoeff < 0.2)
	{
		row_str.rightLocMax		 = -1;
		row_str.ValRLM			 = -1;
		row_str.rightCoeff	     = -1;
		diff_GlobMax_rightLocMax = -1;
	}
/*	if(coeff > 1)
	{
		coeff = (float)0.2;
		mode_diff_b2_b1 = (int16_t) coeff * diff_b3_bsl;
	}
*/
	//интерпретация максимумов гистограммы

//	if(p_attr->number == 82) DebugBreak();// && p_attr->language == 0

	if(stat_interp_bsl(line, p_attr, &row_str))
	{
		switch(row_str.AltGM)
		{
			case 1:	p_attr->bs1 = row_str.globMax;
 					Ns1[0] = row_str.ValGM;
					p_attr->Nb1 = (int16_t)MIN(100,
									(float)100 * row_str.ValGM/(float)(sz_Y1Arr * Pb1));

					if(row_str.ValRLM != -1 &&
						0.2 <= row_str.rightCoeff && row_str.rightCoeff <= 1)
					{
						p_attr->bs2 = row_str.rightLocMax;
						Ns2[0] = row_str.ValRLM;
						p_attr->Nb2 = (int16_t)MIN(100,
								   (float)100 * row_str.ValRLM/(float)(sz_Y1Arr * Pb2));
					}
					else
					{
						p_attr->bs2 = -1;
						Ns2[0] = -1;
						p_attr->Nb2 = -1;
					}
					break;
			case 2:	;
			default:p_attr->bs2 = row_str.globMax;
 					Ns2[0] = row_str.ValGM;
					p_attr->Nb2 = (int16_t)MIN(100,
									(float)100 * row_str.ValGM/(float)(sz_Y1Arr * Pb2));

					if(row_str.ValLLM != -1 &&
						0.2 <= row_str.leftCoeff && row_str.leftCoeff <= 1)
					{
						p_attr->bs1 = row_str.leftLocMax;
						Ns1[0] = row_str.ValLLM;
						p_attr->Nb1 = (int16_t)MIN(100,
								   (float)100 * row_str.ValLLM/(float)(sz_Y1Arr * Pb1));
					}
					else
					{
						p_attr->bs1 = -1;
						Ns1[0] = -1;
						p_attr->Nb1 = -1;
					}
					break;
		}
	}
}

int16_t stat_LineLength(CSTR_line line)
{
	int16_t i = 0;

	CSTR_attr line_attr;
	if(CSTR_GetLineAttr(line,&line_attr))
	{
		CSTR_rast  start = CSTR_GetFirstRaster(line), stop = CSTR_GetLastRaster(line);

		if(start == NULL || stop == NULL) return i;

		for(CSTR_rast curr_raster = CSTR_GetNextRaster(start, CSTR_f_let | CSTR_f_bad);
		curr_raster && curr_raster!=stop;
		curr_raster = CSTR_GetNextRaster(curr_raster, CSTR_f_let | CSTR_f_bad))
		{
			CSTR_rast_attr rast_attr;
			if(CSTR_GetAttr(curr_raster, &rast_attr))
			{
				if(rast_attr.flg != CSTR_f_dust && rast_attr.flg != CSTR_f_punct)
				{
					CCOM_comp *comp;
					if((comp = CSTR_GetComp(curr_raster)) == NULL) continue;
					if(comp->type == CSTR_f_dust || comp->type == CSTR_f_punct) continue;

					i++;
				}
			}
		}
	}

	return i;
}

void stat_FormArrays(CSTR_line line, int16_t row, int16_t *p_Y1Array, int16_t *p_Y2Array)
{
	if(p_Y1Array == NULL || p_Y2Array == NULL) return;

	int16_t  sz_Y1Array = malloc_usable_size(p_Y1Array) / sizeof(int16_t);
	int16_t  sz_Y2Array = malloc_usable_size(p_Y2Array) / sizeof(int16_t);

	if(sz_Y1Array == 0 || sz_Y2Array == 0) return;

	CSTR_attr line_attr;
	if(CSTR_GetLineAttr(line,&line_attr))
	{
		int16_t line_row = row;

		CSTR_rast  start = CSTR_GetFirstRaster(line), stop = CSTR_GetLastRaster(line);

		if(start == NULL || stop == NULL)
		{
			memset(p_Y1Array, 0, sz_Y1Array);
			memset(p_Y2Array, 0, sz_Y2Array);
			return;
		}

		int16_t i = 0;

		for(CSTR_rast curr_raster = CSTR_GetNextRaster(start, CSTR_f_let | CSTR_f_bad); //отладка | CSTR_f_fict
		curr_raster && curr_raster!=stop;
		curr_raster = CSTR_GetNextRaster(curr_raster, CSTR_f_let | CSTR_f_bad)) // | CSTR_f_fict
		{
			CSTR_rast_attr rast_attr;
			if(CSTR_GetAttr(curr_raster, &rast_attr))
			{
				if(rast_attr.flg != CSTR_f_dust && rast_attr.flg != CSTR_f_punct)
				{
					CCOM_comp *comp;
					if((comp = CSTR_GetComp(curr_raster)) == NULL) continue;
					if(comp->type == CSTR_f_dust || comp->type == CSTR_f_punct) continue;

//					p_Y1Array = (int16_t *)realloc(p_Y1Array, (i+1) * sizeof(int16_t));
//					p_Y2Array = (int16_t *)realloc(p_Y2Array, (i+1) * sizeof(int16_t));

//					if(p_Y1Array == NULL || p_Y2Array == NULL)
//						return;
					if(i >= sz_Y1Array || i >= sz_Y2Array) break;

					p_Y1Array[i] = rast_attr.row - line_row;
					p_Y2Array[i] = rast_attr.row + rast_attr.h - line_row;

					i++;
				}
			}
		}
		return;
	}
	else
	{
		memset(p_Y1Array, 0, sz_Y1Array);
		memset(p_Y2Array, 0, sz_Y2Array);
		return;
	}
}

void StatSearchBL(CSTR_line line, CSTR_attr *p_attr,
				  int16_t *Ns1, int16_t *Ns2, int16_t *Ns3, int16_t *Ns4)
{
	int16_t  n = stat_LineLength(line);  //malloc_usable_size(pY1Array) / sizeof(int16_t) - размерность pY1Array и pY2Array
	int16_t *pY1Array = (int16_t *) malloc(sizeof(int16_t) * n);
	int16_t *pY2Array = (int16_t *) malloc(sizeof(int16_t) * n);

	if(pY2Array == NULL || pY1Array == NULL)
	{
		p_attr = (CSTR_attr *) NULL;
		return;
	}

//	DebugBreak();
	stat_FormArrays(line, p_attr->row, pY1Array, pY2Array);

	if(pY1Array == NULL || pY2Array == NULL)
	{
		p_attr = (CSTR_attr *) NULL;
		return;
	}

	//значения гистограммы
	int16_t *pFrequencyY1Array = 	stat_gistoGramma(pY1Array, n);
	int16_t *pFrequencyY2Array =	stat_gistoGramma(pY2Array, n);

	if(pFrequencyY1Array != NULL && pFrequencyY2Array != NULL)
	{
		CSTR_attr line_attr;
		if(CSTR_GetLineAttr(line, &line_attr))
		{
			p_attr->number = line_attr.number;//номер текущей строки
			if(line == 0 || p_attr->number == 0 || p_attr->number > CSTR_GetMaxNumber())
			{
				p_attr = (CSTR_attr *) NULL;
				return;
			}

			//собственно поиск БЛ
			stat_def_b3_b4(line, p_attr, pY2Array, pFrequencyY2Array, Ns3, Ns4);
			stat_def_b1_b2(line, p_attr, pY1Array,
				pY2Array, pFrequencyY1Array, Ns1, Ns2);
			stat_def_imaginary_bl(p_attr, Ns1, Ns2, Ns4);
		}
		else
		{
			p_attr = (CSTR_attr *) NULL;
			return;
		}

		free(pFrequencyY1Array);
		free(pY1Array);

		free(pFrequencyY2Array);
		free(pY2Array);
	}
	else
		p_attr = (CSTR_attr *) NULL;
}

int16_t stat_Hypothesis(int32_t line_number, int16_t diff_b3_bsl)
{
	int16_t iFound_b2 = 0, iFound_b1 = 0;

	if(line_number <= CSTR_GetMaxNumber())
	{
		int32_t line_start = MAX(1, line_number - LNCNST);

		CSTR_attr *pCSTR_attrArray = (CSTR_attr *)malloc((line_number - line_start) * sizeof(CSTR_attr));

		if(pCSTR_attrArray != NULL)
		{
			Bool bRet = stat_FormCSTR_attrArray(line_start, line_number, pCSTR_attrArray);

			if(bRet)
			{
				for(int16_t l = 0; l < line_number - line_start ; l++)
				{
					Bool b = abs(pCSTR_attrArray[l].bs3 - pCSTR_attrArray[l].bs2 - diff_b3_bsl) <= 2;//отладка 1
					if(b && pCSTR_attrArray[l].Nb2 != -1)
						iFound_b2++;

					b = abs(pCSTR_attrArray[l].bs3 - pCSTR_attrArray[l].bs1 - diff_b3_bsl) <= 2;
					if(b && pCSTR_attrArray[l].Nb1 != -1)
						iFound_b1++;
				}
			}
		}

		free(pCSTR_attrArray);
	}

	int16_t ret;

	if(iFound_b1 != 0 || iFound_b2 != 0) ret = iFound_b1 > iFound_b2 ? 1 : 2;
	else ret = -1;

	return ret;
}

Bool stat_FormCSTR_attrArray(int32_t line_start, int32_t line_stop, CSTR_attr *pCSTR_attrArray)
{
	if(line_stop > CSTR_GetMaxNumber() || pCSTR_attrArray == NULL) return FALSE;

	int16_t sz = malloc_usable_size(pCSTR_attrArray) / sizeof(CSTR_attr);
	if(sz < line_stop - line_start) return FALSE;

	for(int32_t l = 0; l < line_stop - line_start; l++)
	{
		CSTR_attr m_attr;

		if(CSTR_GetLineAttr(CSTR_GetLineHandle(l, CSTR_LINVERS_MAINOUT), &m_attr))
			pCSTR_attrArray[l-1] = m_attr;
//		else
//			pCSTR_attrArray[l-1] = (CSTR_attr )NULL;
	}

	return TRUE;
}

void stat_def_imaginary_bl(CSTR_attr *p_attr, int16_t *Ns1, int16_t *Ns2, int16_t *Ns4)
{
	if(p_attr->number < 1) return;

	if(p_attr->bs1 == -1 && p_attr->Nb1 == -1 && p_attr->bs2 != -1)
	{
		int16_t mode_diff_b2_b1 = stat_Mode_diff_b2_b1(p_attr->number, p_attr->bs3 - p_attr->bs2);

		if(mode_diff_b2_b1 != -1)
		{
			int16_t bs1 = p_attr->bs2 - mode_diff_b2_b1;
			//(float)(p_attr->bs2 - mode_diff_b2_b1)/(float)(p_attr->bs3 - p_attr->bs2)
			p_attr->bs1 = bs1 != -1 ? bs1 : 0;
		}
		else
		{
			p_attr->bs1 = (short)(p_attr->bs2 - 0.4 * (p_attr->bs3 - p_attr->bs2));
		}
	}

	if(p_attr->bs1 != -1 && p_attr->bs2 == -1)
	{
		int16_t diff_b2_b3 = stat_Mode_diff_b2_b3(p_attr->number, p_attr->bs3 - p_attr->bs1, 1);

		if(diff_b2_b3 != -1)
		{
			p_attr->bs2 = p_attr->bs3 - diff_b2_b3;
		}
		else
		{
			p_attr->bs2 = (short)(p_attr->bs3 - 0.6 * (p_attr->bs3 - p_attr->bs1));
		}
	}

	if(p_attr->bs4 != -1 && p_attr->bs2 != -1)
	{
		float coeff = 0;
		int16_t diff_b3_b2 = p_attr->bs3 - p_attr->bs2;
		if(diff_b3_b2 != 0)
			coeff = (float)(p_attr->bs4 - p_attr->bs3) / (float) (diff_b3_b2);
		if(coeff < 0.2)
			p_attr->bs4 = (short)(p_attr->bs3 + 0.4 * (p_attr->bs3 - p_attr->bs2));
	}

	if(p_attr->bs4 == -1 && p_attr->bs2 != -1)
	{
		int16_t diff_b3_b4 = stat_Mode_diff_b3_b4(p_attr->number, p_attr->bs3 - p_attr->bs2);

		if(diff_b3_b4 != -1)
		{
			p_attr->bs4 = p_attr->bs3 + diff_b3_b4;
		}
		else
			if(p_attr->bs2 != -1)
			{
				p_attr->bs4 = (short)(p_attr->bs3 + 0.4 * (p_attr->bs3 - p_attr->bs2));
			}
	}
}

int16_t stat_Mode_diff_b2_b1(int32_t line_number, int16_t diff_b3_b2)
{
	int16_t iMode = 0;
	Bool bFound_mode = FALSE;

	if(line_number <= CSTR_GetMaxNumber())
	{
		int32_t line_start = MAX(1, line_number - LNCNST);

		CSTR_attr *pCSTR_attrArray = (CSTR_attr *)malloc((line_number - line_start) * sizeof(CSTR_attr));

		if(pCSTR_attrArray != NULL)
		{
			int16_t *p_diff = (int16_t *)malloc((line_number - line_start) * sizeof(int16_t));

			if(p_diff != NULL)
			{
				Bool bRet = stat_FormCSTR_attrArray(line_start, line_number, pCSTR_attrArray);

				if(bRet)
				{
					int16_t jCount = 0;

					for(int16_t l = 0; l < line_number - line_start; l++)
					{
						if(abs(pCSTR_attrArray[l].bs3 - pCSTR_attrArray[l].bs2 - diff_b3_b2) <= 1)
						{
							if(pCSTR_attrArray[l].Nb1 != -1 && pCSTR_attrArray[l].Nb2 != -1)
							{
								p_diff[jCount] =
											pCSTR_attrArray[l].bs2 - pCSTR_attrArray[l].bs1;
								jCount++;
							}
						}
					}

					if(jCount !=0)
					{
						p_diff = (int16_t *) realloc(p_diff, jCount * sizeof(int16_t));
						int16_t *pFrequencyArray = stat_gistoGramma(p_diff, jCount);
						if(pFrequencyArray != NULL)
						{
							int16_t szFrequencyArray = malloc_usable_size(pFrequencyArray) / sizeof(int16_t);
							int16_t          minArray = p_diff[stat_index_GlobMin(p_diff,jCount)];
							int16_t maxFrequencyArray = stat_index_GlobMax(pFrequencyArray,szFrequencyArray, G);
							if(szFrequencyArray != 0) free(pFrequencyArray);
							iMode = minArray + maxFrequencyArray;
							bFound_mode = TRUE;
						}
					}
				}

				free(p_diff);
			}

			free(pCSTR_attrArray);
		}
	}

	iMode = bFound_mode ? iMode : -1;

	return iMode;
}

int16_t stat_Left_diff_from_max(int16_t *p_FrequencyArray, int16_t max_FrequencyArray)
{
	int16_t jret = -1;
	int16_t derivative = 0;
	int16_t i;

	for(i = max_FrequencyArray - 1; i >= 0; i--)
	{
		if(p_FrequencyArray[i] == 0)
		{
			jret = max_FrequencyArray - i + 1;
			break;
		}
	}

	if(jret == -1)
	{
		for(i = max_FrequencyArray - 1; i >= 0; i--)
		{
			derivative = p_FrequencyArray[i+1] - p_FrequencyArray[i];

			if(derivative < 0) // <=
			{
				jret = max_FrequencyArray - i + 1;
				break;
			}
		}
	}

	return jret;
}

int16_t stat_Right_diff_from_max(int16_t *p_FrequencyArray, int16_t max_FrequencyArray)
{
	int16_t sz = malloc_usable_size(p_FrequencyArray) / sizeof(int16_t);
	int16_t jret = sz - max_FrequencyArray + 1;
	int16_t derivative = 0;
	int16_t i;

	for(i = max_FrequencyArray + 1; i < sz; i++)
	{
		if(p_FrequencyArray[i] == 0)
		{
			jret = i - max_FrequencyArray;
			break;
		}
	}

	if(jret == sz - max_FrequencyArray + 1)
	{
		for(i = max_FrequencyArray + 1; i < sz; i++)
		{
			derivative = p_FrequencyArray[i] - p_FrequencyArray[i-1];

			if(derivative > 0) // >=
			{
				jret = i - max_FrequencyArray;
				break;
			}
		}
	}

	return jret;
}

int16_t stat_Mode_diff_b3_b4(int32_t line_number, int16_t diff_b3_b2)
{
	int16_t iMode = 0;
	Bool bFound_mode = FALSE;

	if(line_number <= CSTR_GetMaxNumber())
	{
		int32_t line_start = MAX(1, line_number - LNCNST);

		CSTR_attr *pCSTR_attrArray = (CSTR_attr *)malloc((line_number - line_start) * sizeof(CSTR_attr));

		if(pCSTR_attrArray != NULL)
		{
			int16_t *p_diff = (int16_t *)malloc((line_number - line_start) * sizeof(int16_t));

			if(p_diff != NULL)
			{
				Bool bRet = stat_FormCSTR_attrArray(line_start, line_number, pCSTR_attrArray);

				if(bRet)
				{
					int16_t jCount = 0;

					for(int16_t l = 0; l < line_number - line_start; l++)
					{
						if(abs(pCSTR_attrArray[l].bs3 - pCSTR_attrArray[l].bs2 - diff_b3_b2) <= 1)
						{
							if(pCSTR_attrArray[l].Nb4 != -1)
							{
								p_diff[jCount] =
											pCSTR_attrArray[l].bs4 - pCSTR_attrArray[l].bs3;
								jCount++;
							}
						}
					}

					if(jCount !=0)
					{
						p_diff = (int16_t *) realloc(p_diff, jCount * sizeof(int16_t));
						int16_t  *pFrequencyArray = stat_gistoGramma(p_diff, jCount);
						if(pFrequencyArray != NULL)
						{
							int16_t  szFrequencyArray = malloc_usable_size(pFrequencyArray) / sizeof(int16_t);
							int16_t          minArray = p_diff[stat_index_GlobMin(p_diff,jCount)];
							int16_t maxFrequencyArray = stat_index_GlobMax(pFrequencyArray,szFrequencyArray, G);
							if(szFrequencyArray != 0) free(pFrequencyArray);
							iMode = minArray + maxFrequencyArray;
							bFound_mode = TRUE;
						}
					}
				}

				free(p_diff);
			}

			free(pCSTR_attrArray);
		}
	}

	iMode = bFound_mode ? iMode : -1;

	return iMode;
}

int16_t stat_Mode_diff_b2_b3(int32_t line_number, int16_t diff, int16_t fl_b2_or_b1)
{
	int16_t iMode = 0;
	Bool bFound_mode = FALSE;

	if(line_number <= CSTR_GetMaxNumber())
	{
		int32_t line_start = MAX(1, line_number - LNCNST);

		CSTR_attr *pCSTR_attrArray = (CSTR_attr *)malloc((line_number - line_start) * sizeof(CSTR_attr));

		if(pCSTR_attrArray != NULL)
		{
			int16_t *p_diff = (int16_t *)malloc((line_number - line_start) * sizeof(int16_t));

			if(p_diff != NULL)
			{
				Bool bRet = stat_FormCSTR_attrArray(line_start, line_number, pCSTR_attrArray);

				if(bRet)
				{
					int16_t jCount = 0;
					Bool b;
					for(int16_t l = 0; l < line_number - line_start; l++)
					{
						switch(fl_b2_or_b1)
						{
							case 1: b = abs(pCSTR_attrArray[l].bs3 - pCSTR_attrArray[l].bs1 - diff) <= 1;
									break;
							case 2: b = abs(pCSTR_attrArray[l].bs3 - pCSTR_attrArray[l].bs2 - diff) <= 1;
									break;
							default:;
						}

						if(pCSTR_attrArray[l].Nb1 != -1 && pCSTR_attrArray[l].Nb2 != -1 && pCSTR_attrArray[l].bs2 != -1 && b)//отладка
						{//
							p_diff[jCount] =
										pCSTR_attrArray[l].bs3 - pCSTR_attrArray[l].bs2;
							jCount++;
						}
					}
					if(jCount !=0)
					{
						p_diff = (int16_t *) realloc(p_diff, jCount * sizeof(int16_t));
						int16_t  *pFrequencyArray = stat_gistoGramma(p_diff, jCount);
						if(pFrequencyArray != NULL)
						{
							int16_t  szFrequencyArray = malloc_usable_size(pFrequencyArray) / sizeof(int16_t);
							int16_t          minArray = p_diff[stat_index_GlobMin(p_diff,jCount)];
							int16_t maxFrequencyArray = stat_index_GlobMax(pFrequencyArray,szFrequencyArray, G);
							if(szFrequencyArray != 0) free(pFrequencyArray);
							iMode = minArray + maxFrequencyArray;
							bFound_mode = TRUE;
						}
					}
				}

				free(p_diff);
			}

			free(pCSTR_attrArray);
		}
	}

	iMode = bFound_mode ? iMode : -1;

	return iMode;
}

/*
Bool stat_interp_bsl_by_solid_recog(CSTR_line line, int16_t ln_row, int16_t b3, ROW_STRUCT *row_str)
{
	CSTR_attr line_attr;

	char buff_figure[]    = "23456789";

	char buff_eng_high[]  = "ABDEFGHIKLMNRTbdfgklpPtyYjJ";
	char buff_eng_high2[] = "ABDEFGHIKLMNRTbdfgklpPtyYjJ0OpPyYjJg23456789";
	char buff_eng_low[]   = "aeimnr";//

	char buff_rus_high[]  = "АБЕбФфРУу";
	char buff_rus_high2[] = "АБЕбФфРрУу0О23З456789";
	char buff_rus_low[]   = "ае";

	char buff_warning[] = "\\/|<>(){}[]йЙ"; //дДщЩцЦ

	char *peng_low, *prus_low, *pfig, *peng_high, *prus_high,
		 *p_high1, *p_high2, *p_low1, *p_low2, *p_warning;

	int16_t countLLM1 = 0, countGM1 = 0, countGM2 = 0, countRLM2 = 0, countRLM1 = 0,
		  countWarn1 = 0, countHighL1 = 0, countWarn2 = 0, countHighL2 = 0;

	//интервалы
	int16_t pnt0, pnt1, pnt2, pnt3;

	if(row_str->leftLocMax != -1)
	{
		pnt0 = (int16_t)(row_str->leftLocMax - 0.5 * (row_str->globMax - row_str->leftLocMax));
		pnt1 = (int16_t)(row_str->globMax - 0.5 * (row_str->globMax - row_str->leftLocMax));
	}
	else
	{
		if(row_str->rightLocMax != -1)
			pnt1 = (int16_t)(row_str->globMax - 0.5 * (row_str->rightLocMax - row_str->globMax));
		else
			pnt1 = (int16_t)(row_str->globMax - 0.5 * (b3 - row_str->globMax));
		pnt0 = pnt1;
	}

	if(row_str->rightLocMax != -1)
	{
		pnt2 = (int16_t)(row_str->globMax + 0.5 * (row_str->rightLocMax - row_str->globMax));
		pnt3 = (int16_t)(row_str->rightLocMax + 0.5 * (b3 - row_str->rightLocMax));
	}
	else
	{
		pnt2 = (int16_t)(row_str->globMax + 0.5 * (b3 - row_str->globMax));
		pnt3 = pnt2;
	}

	if(CSTR_GetLineAttr(line,&line_attr))
	{
		CSTR_rast  start = CSTR_GetFirstRaster(line), stop = CSTR_GetLastRaster(line);

		if(start == NULL || stop == NULL) return FALSE;

		for(CSTR_rast curr_raster = CSTR_GetNextRaster(start, CSTR_f_let);
		curr_raster && curr_raster!=stop;
		curr_raster = CSTR_GetNextRaster(curr_raster, CSTR_f_let))
		{
			CSTR_rast_attr rast_attr;
			UniVersions vers;

			if(CSTR_GetAttr(curr_raster, &rast_attr) &&
			   CSTR_GetCollectionUni(curr_raster, &vers))
			{
				if(vers.lnAltCnt <= 0 || rast_attr.flg == CSTR_f_space)
				{
					continue;
				}

				peng_low = prus_low = pfig = peng_high =
					prus_high = p_high1 = p_high2 = p_low1 = p_low2 = p_warning = NULL;

				for(int i = 0; i < vers.lnAltCnt; i++)
				{
					char c = vers.Alt[i].Code[0];

					p_warning = strchr(buff_warning, c);

					if(p_warning) break;
				}

				int16_t  db3 = rast_attr.row + rast_attr.h - ln_row - b3;

				if(abs(db3) <= 2 || p_warning)
				{
					int16_t drow = rast_attr.row - ln_row;

					if(vers.lnAltCnt == 1)
					{
						if(vers.Alt[0].Prob >= 230)
						{
							for(int16_t j = 0; j < 2; j++)
							{
								char c = vers.Alt[0].Code[j];
								if(c == 0) break;

								prus_low = strchr(buff_rus_low, c);
								prus_high = strchr(buff_rus_high, c);

								pfig = strchr(buff_figure, c);

								peng_low = strchr(buff_eng_low, c);
								peng_high = strchr(buff_eng_high, c);
							}
						}

						if(prus_high != NULL || pfig != NULL || peng_high != NULL)
						{
							prus_low = NULL;
							peng_low = NULL;
						}
					}
					else
					{
						if(vers.lnAltCnt == 2)
						{
							char c1 = vers.Alt[0].Code[0];
							char c2 = vers.Alt[1].Code[0];

							p_high1 = strchr(buff_rus_high2, c1);
							p_high2 = strchr(buff_rus_high2, c2);

							if(!p_high1) p_high1 = strchr(buff_eng_high2, c1);
							if(!p_high2) p_high2 = strchr(buff_eng_high2, c2);

							if(p_high1 == NULL && p_high2)
							{
								p_low1 = strchr(buff_rus_low, c1);
								p_low2 = strchr(buff_rus_low, c2);

								if(!p_low1) p_low1 = strchr(buff_eng_low, c1);
								if(!p_low2) p_low2 = strchr(buff_eng_low, c2);
							}
						}
					}

					if(peng_low != NULL || prus_low != NULL ||
						(p_low1 != NULL && p_low2 != NULL))
					{
						if(pnt1 <= drow && drow <= pnt2)
						{
							countGM2++;
							continue;
						}
						else
						{
							if(row_str->rightLocMax != -1 && pnt2 < drow && drow <= pnt3)
							{
								countRLM2++;
								continue;
							}
						}
					}

					if(pfig != NULL || peng_high != NULL || prus_high != NULL ||
						(p_high1 != NULL && p_high2 != NULL))
					{
						if(pnt1 <= drow && drow <= pnt2)
						{
							countGM1++;
							continue;
						}
						else
						{
							if(row_str->leftLocMax != -1 && pnt0 <= drow && drow < pnt1)
							{
								countLLM1++;
								continue;
							}
							else
							{
								if(row_str->rightLocMax != -1 && pnt2 <= drow && drow <= pnt3)
								{
									countRLM1++;
									continue;
								}
							}
						}
					}

					if(row_str->leftLocMax != -1 && pnt0 <= drow && drow < pnt1)
					{
						if(p_warning) countWarn1++;
						else countHighL1++;
					}

					if(pnt1 <= drow && drow < pnt2)
					{
						if(p_warning) countWarn2++;
						else countHighL2++;
					}
				}
			}
		}

		if(countGM1 + countRLM2 > 0 || countLLM1 + countGM2 > 0)
		{
			if(countGM1 + countRLM2 > countLLM1 + countGM2 ||
				(countGM1 > 0 && countGM1 == countLLM1 && row_str->rightLocMax != -1))
			{
				row_str->AltGM  = 1;
				row_str->AltRLM = countRLM2 > 0 ? 2 : -1;
				row_str->AltLLM = -1;
			}
			else
			{
				row_str->AltGM  =  2;
				row_str->AltRLM = -1;
				row_str->AltLLM = countLLM1 > 0 ? 1 : -1;
			}
		}
		else
		{
			if(countGM1 == 0 && countRLM1 > 0)
			{
				row_str->AltRLM = 1;
			}
		}

		if((countHighL1 == 0 && countWarn1 > 0) || (countHighL2 == 0 && countWarn2 > 0))
		{
			countWarn1 = 0, countHighL1 = 0, countWarn2 = 0, countHighL2 = 0;
			for(CSTR_rast curr_raster = CSTR_GetNextRaster(start, CSTR_f_let | CSTR_f_bad);
			curr_raster && curr_raster!=stop;
			curr_raster = CSTR_GetNextRaster(curr_raster, CSTR_f_let | CSTR_f_bad))
			{
				CSTR_rast_attr rast_attr;
				UniVersions vers;

				if(CSTR_GetAttr(curr_raster, &rast_attr) &&
				   CSTR_GetCollectionUni(curr_raster, &vers))
				{
					if(rast_attr.flg == CSTR_f_space) continue;

					p_warning = NULL;

					for(int i = 0; i < vers.lnAltCnt; i++)
					{
						char c = vers.Alt[i].Code[0];

						p_warning = strchr(buff_warning, c);

						if(p_warning) break;
					}

					int16_t  db3 = rast_attr.row + rast_attr.h - ln_row - b3;

					if(abs(db3) <= 2 || p_warning)
					{
						int16_t drow = rast_attr.row - ln_row;

						if(row_str->leftLocMax != -1 && pnt0 <= drow && drow < pnt1)
						{
							if(p_warning) countWarn1++;
							else countHighL1++;
						}

						if(pnt1 <= drow && drow < pnt2)
						{
							if(p_warning) countWarn2++;
							else countHighL2++;
						}
					}
				}
			}

			Bool bdel = FALSE;
			if(row_str->leftLocMax != -1 && row_str->AltLLM == -1 &&
				row_str->ValLLM <= countWarn1 && countHighL1 == 0)
			{
				row_str->AltLLM = -1;
				row_str->leftLocMax = -1;
				row_str->leftCoeff = -1;
				row_str->ValLLM = -1;
				bdel = TRUE;
			}

			if(!bdel && row_str->leftLocMax == -1 && row_str->AltGM == -1 &&
				row_str->rightLocMax != -1 &&
				row_str->ValGM <= countWarn2 && countHighL2 == 0)
			{
				row_str->globMax = row_str->rightLocMax;
				row_str->AltGM   = row_str->AltRLM;
				row_str->ValGM   = row_str->ValRLM;

				row_str->rightLocMax = -1;
				row_str->AltRLM = -1;
				row_str->ValRLM = -1;
				row_str->rightCoeff = -1;
			}
		}
	}
	else return FALSE;

	return TRUE;
}
*/
