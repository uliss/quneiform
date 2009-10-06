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


#ifdef _DEBUG
#define SOURCE_FILE_NAME "FindLostLines.cpp"
#endif
/***********************************************************************************************/
#include <cstdlib>
#include <string.h>
#include "stdafx.h"
#include "rshelllinescom.h"
#include "rline.h"
#include "dpuma.h"
#include "pumadef.h"

#ifdef _DEBUG
#include <fstream>
using namespace std;
#endif

#include "compat_defs.h"

/***********************************************************************************************/
#define INCLINE_FACTOR 2048
#define DEFAULT_DPI 300
#define FULL_BLACK 100

# define IDEAL_XY(x, y)   \
         {\
             y = (int16_t) (y - (int32_t) x * nIncline / INCLINE_FACTOR);\
             x = (int16_t) (x + (int32_t) y * nIncline / INCLINE_FACTOR);\
         }

# define IDEAL_X(x, y)   \
         {\
             x = (int16_t) (x + (int32_t) y * nIncline / INCLINE_FACTOR);\
         }

# define IDEAL_Y(x, y)   \
         {\
             y = (int16_t) (y - (int32_t) x * nIncline / INCLINE_FACTOR);\
         }

# define REAL_XY(x, y)   \
         {\
             x = (int16_t) (x - (int32_t) y * nIncline / INCLINE_FACTOR);\
             y = (int16_t) (y + (int32_t) x * nIncline / INCLINE_FACTOR);\
		}
/***********************************************************************************************/
typedef struct tagLineData
{
	CLINE_handle hLine;
	Rect32 rect;
	uint32_t flag;
#define FL_GOOD 1
#define FL_HAS_CP 2
} LineData;

typedef struct tagFLLData
{
	CLINE_handle hCLINE;
	CLINE_handle hTempContainer;
	int32_t line_distance_x;
	int32_t line_distance_y;
	PAGEINFO* pInfo;
	int32_t page_beg_x;
	int32_t page_beg_y;
	int32_t page_end_x;
	int32_t page_end_y;
	bool bShowCP;
	bool bShowCheckedRects;
	bool bPrintCheckedRects;
	Handle hWnd;
	LineData* aBadHoriLines;
	LineData* aBadVertLines;
	int32_t hori_bad_lines;
	int32_t vert_bad_lines;
} FLLData;

const int line_distance = 10;
const int size_Point32 = sizeof(Point32);
const int size_int32_t = sizeof(int32_t);
const int size_DLine = sizeof(DLine);
static const uchar bits_array[] = {128, 64, 32, 16, 8, 4, 2, 1};

extern int32_t hRLINE_Pass2Lost1;
extern int32_t hRLINE_Pass2Lost2;
extern Handle hShowCP;
extern Handle hLinesPass2DebugSkipFindLost;
extern Handle hShowCheckedRects;
extern Handle hPrintCheckedRects;
/***********************************************************************************************/
void getLineIdealStrictRectangular(const NR_SimpLine *pdLine, Rect32* pRect, bool is_horiz,
								   int32_t nIncline, int32_t spread = 0);
Bool32 addLine(CPDLine pLine, LineData* aLines, int32_t *used_lines, int32_t *nCP, int32_t nLines,
			   bool is_horiz, int32_t nIncline, CLINE_handle hLine, int32_t spread = 0);
Bool32 getLostLines(const FLLData *work_data, const LineData* aCPLines, const int32_t nCPLines,
					const LineData* aContLines, const int32_t nContLines, const bool is_horiz,
					Point32* aCP, const int32_t nCP);
Bool32 extractLines(const FLLData *work_data, Point32 *aCP, const int32_t nCP,
					const LineData* aContLines, const int32_t nContLines, const bool is_horiz);
Bool32 checkBlack(int32_t black, bool is_good);
int32_t getRasterBlack(NR_SimpLine* term, bool is_horiz, const FLLData* work_data);
Bool32 getEnds(Point32* cur_end, const Point32* finish_end, double angle, bool is_horiz,
			 int32_t lend1, int32_t lend2, const FLLData* work_data, bool checkupper);
void updateLine(/*CLINE_handle* hLine, */const FLLData* work_data, bool is_horiz, Point32* begin,
				Point32* end);
Bool32 checkCross(const Rect32* rect1, const Rect32* rect2, bool is_horiz);

#ifdef _DEBUG
void PrintErrorString(char* errstring);
#endif

static int CompareX( const void *elem1, const void *elem2 )
{

  Point32 *a = (Point32*) elem1;
  Point32 *b = (Point32*) elem2;

  return a->x - b->x;
}

static int CompareY( const void *elem1, const void *elem2 )
{

  Point32 *a = (Point32*) elem1;
  Point32 *b = (Point32*) elem2;

  return a->y - b->y;
}

static int CompareAsc( const void *elem1, const void *elem2 )
{

  int32_t *a = (int32_t*) elem1;
  int32_t *b = (int32_t*) elem2;

  return *a - *b;
}

static int CompareDesc( const void *elem1, const void *elem2 )
{

  int32_t *a = (int32_t*) elem1;
  int32_t *b = (int32_t*) elem2;

  return *b - *a;
}

/***********************************************************************************************/
Bool32 findLostLines(CLINE_handle hCLINE, PAGEINFO* info)
{
	if (!LDPUMA_Skip(hLinesPass2DebugSkipFindLost))
		return TRUE;

	if (!hCLINE || !info)
	{
#ifdef _DEBUG
		PrintErrorString("null pointer to the line container or to the info");
#endif

		return TRUE;
	}

	int32_t nLines = CLINE_GetLineCount(hCLINE), hori_lines = 0, vert_lines = 0, hori_bad_lines = 0, vert_bad_lines = 0;

	if (nLines < 2)
		return TRUE;

	LineData *aHoriLines = new LineData[nLines];
	LineData *aVertLines = new LineData[nLines];
	LineData *aBadHoriLines = new LineData[nLines];
	LineData *aBadVertLines = new LineData[nLines];

	if (!aHoriLines || !aVertLines || !aBadHoriLines || !aBadVertLines)
	{
#ifdef _DEBUG
		PrintErrorString("unable to create line arrays");
#endif

		if (aHoriLines)
			delete[] aHoriLines;

		if (aVertLines)
			delete[] aVertLines;

		if (aBadHoriLines)
			delete[] aBadHoriLines;

		if (aVertLines)
			delete[] aBadVertLines;

		return FALSE;
	}

	//initializing
	//------------
	CPDLine pLine;
	uint32_t ignore_flags = LI_IsFalse | LI_Pointed;
	Bool32 pc = TRUE;
	int32_t nIncline = info->Incline2048;
	int32_t hori_cp = 0, vert_cp = 0, bad_cp = 0;
	const int32_t spread = 5;
	const int32_t break_cp = 500;

	for (CLINE_handle hLine = CLINE_GetFirstLine(hCLINE); hLine; hLine = CLINE_GetNextLine(hLine))
	{
		if (!(pLine = CLINE_GetLineData(hLine)))
			continue;

		if (pLine->Flags & ignore_flags || pLine->FlagBad)
		{
			if(!(pc = pLine->Dir == LD_Horiz ?
				   addLine(pLine, aBadHoriLines, &hori_bad_lines, &bad_cp, nLines, true, nIncline, hLine, 5) :
				   addLine(pLine, aBadVertLines, &vert_bad_lines, &bad_cp, nLines, false, nIncline, hLine, 5)))
				break;
		}
		else if (!(pc = pLine->Dir == LD_Horiz ?
				   addLine(pLine, aHoriLines, &hori_lines, &hori_cp, nLines, true, nIncline, hLine, 5) :
				   addLine(pLine, aVertLines, &vert_lines, &vert_cp, nLines, false, nIncline, hLine, 5)))
				break;
	}//end hLine loop

	if (pc)
	{
		int32_t max_cp = MAX(hori_cp, vert_cp);

		if (max_cp > 1)//we need at least 2 cross points to reconstruct a line
		{
			Point32 *aCrossPoints = new Point32[max_cp];

			if ((pc = aCrossPoints != NULL))
			{
				FLLData fll_data;

				fll_data.hCLINE = hCLINE;
				fll_data.hTempContainer = CLINE_CreateContainer(FALSE);
				fll_data.pInfo = info;
				fll_data.aBadHoriLines = aBadHoriLines;
				fll_data.aBadVertLines = aBadVertLines;
				fll_data.hori_bad_lines = hori_bad_lines;
				fll_data.vert_bad_lines = vert_bad_lines;
				fll_data.bShowCP = !LDPUMA_Skip(hShowCP);
				fll_data.bShowCheckedRects = !LDPUMA_Skip(hShowCheckedRects);
				fll_data.bPrintCheckedRects = !LDPUMA_Skip(hPrintCheckedRects);

				fll_data.hWnd = LDPUMA_GetWindowHandle (PUMA_IMAGE_BINARIZE);

				if (!fll_data.hWnd) fll_data.hWnd = LDPUMA_GetWindowHandle (NAME_IMAGE_ORTOMOVE);

				if (!fll_data.hWnd) fll_data.hWnd = LDPUMA_GetWindowHandle (PUMA_IMAGE_TURN);

				if (!fll_data.hWnd) fll_data.hWnd = LDPUMA_CreateWindow(NULL, NULL);

				if ((info->Angle==90) || (info->Angle==270))
				{
					fll_data.page_beg_y = info->X;
					fll_data.page_beg_x = info->Y;
					fll_data.page_end_y = info->X + info->Width - 1;
					fll_data.page_end_x = info->Y + info->Height - 1;
					fll_data.line_distance_x = line_distance*(info->DPIX + 1)/DEFAULT_DPI;
					fll_data.line_distance_y = line_distance*(info->DPIY + 1)/DEFAULT_DPI;
				}
				else
				{
					fll_data.page_beg_y = info->Y;
					fll_data.page_beg_x = info->X;
					fll_data.page_end_y = info->Y + info->Height - 1;
					fll_data.page_end_x = info->X + info->Width - 1;
					fll_data.line_distance_y = line_distance*(info->DPIX + 1)/DEFAULT_DPI;
					fll_data.line_distance_x = line_distance*(info->DPIY + 1)/DEFAULT_DPI;
				}

				//analyse cross points
				//--------------------
				if (hori_cp > 1 && hori_cp < break_cp)
					pc = getLostLines(&fll_data, aHoriLines, hori_lines, aVertLines, vert_lines, true, aCrossPoints, max_cp);

				if (vert_cp > 1 && vert_cp < break_cp && pc)
					pc = getLostLines(&fll_data, aVertLines, vert_lines, aHoriLines, hori_lines, false, aCrossPoints, max_cp);

				delete[] aCrossPoints;

				//remove crossed lines
				//--------------------
				CLINE_handle hNewLine;
				for (hNewLine = CLINE_GetFirstLine(fll_data.hTempContainer); hNewLine; hNewLine = CLINE_GetNextLine(hNewLine))
				{
					CPDLine pNewLine = CLINE_GetLineData(hNewLine);

					if (!pNewLine)
						continue;

					Rect32 rect;

					getLineIdealStrictRectangular(&pNewLine->Line, &rect, pNewLine->Dir == LD_Horiz, 0);

					rect.left -= spread;
					rect.right += spread;
					rect.top -= spread;
					rect.bottom += spread;

					CLINE_handle hLine = CLINE_GetFirstLine(hCLINE);

					while(hLine)
					{
						CPDLine pLine = CLINE_GetLineData(hLine);

						if (!pLine)
						{
							hLine = CLINE_GetNextLine(hLine);

							continue;
						}

						CLINE_handle hTemp = CLINE_GetNextLine(hLine);

						if (pLine->Line.Beg_X >= rect.left && pLine->Line.End_X >= rect.left &&
							pLine->Line.Beg_Y >= rect.top && pLine->Line.End_Y >= rect.top &&
							pLine->Line.Beg_X <= rect.right && pLine->Line.End_X <= rect.right &&
							pLine->Line.Beg_Y <= rect.bottom && pLine->Line.End_Y <= rect.bottom)
							CLINE_DelLine(hCLINE, hLine);

						hLine = hTemp;
					}
				}

				hNewLine = CLINE_GetFirstLine(fll_data.hTempContainer);

				while (hNewLine)
				{
					CLINE_handle hTemp = CLINE_GetNextLine(hNewLine);

					if (!CLINE_MoveLine(hCLINE, fll_data.hTempContainer, hNewLine))
					{
#ifdef _DEBUG
						PrintErrorString("unable to move line");
#endif

						break;
					}

					hNewLine = hTemp;
				}

				CLINE_DeleteContainer(fll_data.hTempContainer);
			}
		}//end max_cp check
	}

	delete[] aHoriLines;
	delete[] aVertLines;
	delete[] aBadHoriLines;
	delete[] aBadVertLines;

	return TRUE;
}
/********************************************************************************************/
#ifdef _DEBUG
void PrintErrorString(char* errstring)
{
	if (!LDPUMA_IsActive())
		return;

	ofstream f("naverror.log", ios::out|ios::app);

	if (f.fail())
		return;

	f << LDPUMA_GetFileName(NULL) <<": " << errstring <<" - " << SOURCE_FILE_NAME <<"\n";
	f.close();
}
#endif
/********************************************************************************************/
Bool32 addLine(CPDLine pLine, LineData* aLines, int32_t *used_lines, int32_t *nCP, int32_t nLines,
			   bool is_horiz, int32_t nIncline, CLINE_handle hLine, int32_t spread)
{
	if (*used_lines >= nLines)
	{
#ifdef _DEBUG
		PrintErrorString("lines number exceeded");
#endif

		return FALSE;
	}

	getLineIdealStrictRectangular(&pLine->Line, &aLines[*used_lines].rect, is_horiz, nIncline, spread);

	int32_t cross_points = CLINE_GetCutPointCount(hLine);

	aLines[*used_lines].hLine = hLine;
	aLines[*used_lines].flag = (pLine->Flags == LI_IsTrue  && pLine->Status != ST_Basil_Short ? FL_GOOD : 0) |
							   (cross_points > 0 ? FL_HAS_CP : 0);
	(*used_lines)++;
	*nCP += cross_points;

	return TRUE;
}
/********************************************************************************************/
Bool32 getLostLines(const FLLData *work_data, const LineData* aCPLines, const int32_t nCPLines,
					const LineData* aContLines, const int32_t nContLines, const bool is_horiz,
					Point32* aCP, const int32_t nCP)
{
	//check input data
	//----------------
	if (!work_data || !aCPLines || !aContLines || !aCP)
	{
#ifdef _DEBUG
		PrintErrorString("invalid input data for getLostLines function");
#endif

		return FALSE;
	}

	//fill cross points array
	//-----------------------
	const int32_t hyst_spread = is_horiz ? work_data->line_distance_x>>1 : work_data->line_distance_y>>1;
	const uint32_t good_line = FL_GOOD | FL_HAS_CP;
	CLINE_handle hCP;
	CPDCutPoint pCP;
	int32_t cp = 0, cp_level;
	int32_t nIncline = work_data->pInfo->Incline2048;
	bool bshowcp = work_data->bShowCP;
	Handle hWnd = work_data->hWnd;
	int32_t i;

	for (i = 0; i < nCPLines; i++)
	{
		if ((aCPLines[i].flag & good_line) != good_line)
			continue;

		CPDLine pLine = CLINE_GetLineData(aCPLines[i].hLine);

		if (!pLine)
			continue;

		double tangle = is_horiz ? (double)(pLine->Line.End_Y - pLine->Line.Beg_Y)/(pLine->Line.End_X - pLine->Line.Beg_X) :
								(double)(pLine->Line.Beg_X - pLine->Line.End_X)/(pLine->Line.End_Y - pLine->Line.Beg_Y);

		for (hCP = CLINE_GetFirstCutPoint(aCPLines[i].hLine); hCP; hCP = CLINE_GetNextCutPoint(hCP))
		{
			if (!(pCP = CLINE_GetCutPointData(hCP)))
				continue;

			if (cp >= nCP)
			{
#ifdef _DEBUG
				PrintErrorString("cross point array overflow");
#endif

				return FALSE;
			}

			cp_level = pCP->Level + (CLINE_GetCutPointInvCount(hCP)>>1);

			if (is_horiz)
			{
				aCP[cp].x = cp_level;
				aCP[cp].y = pLine->Line.Beg_Y + (int32_t)(tangle*(cp_level - pLine->Line.Beg_X));
			}
			else
			{
				aCP[cp].y = cp_level;
				aCP[cp].x = pLine->Line.Beg_X - (int32_t)(tangle*(cp_level - pLine->Line.Beg_Y));;
			}

			if (bshowcp)
			{
				Rect16 rect_cp;

				rect_cp.top = aCP[cp].y - 3;
				rect_cp.bottom = aCP[cp].y + 3;
				rect_cp.left = aCP[cp].x - 3;
				rect_cp.right = aCP[cp].x + 3;

				LDPUMA_DrawRect(hWnd, &rect_cp, 0, RGB(0, 255, 0), -100, 1);
			}

			IDEAL_XY(aCP[cp].x, aCP[cp].y);

			if (!is_horiz)
			{
				int32_t t = aCP[cp].x;

				aCP[cp].x = aCP[cp].y;
				aCP[cp].y = t;
			}

			cp++;
		}
	}

	if (cp < 1)
		return TRUE;

	if (bshowcp)
	{
		LDPUMA_WaitUserInput(hShowCP, hWnd);
		LDPUMA_DeleteRects(hWnd, 1);
	}

	//analysing cross points
	//----------------------
	qsort(aCP, cp, size_Point32, CompareX);

	int32_t min_cp = aCP[0].x;
	int32_t max_cp = aCP[cp - 1].x + 1;
	int32_t cp_size = max_cp - min_cp;
	int32_t *cp_hyst = new int32_t[cp_size];
	int32_t *cp_dist = new int32_t[cp_size];

	if (!cp_hyst || !cp_dist)
	{
#ifdef _DEBUG
		PrintErrorString("unable to create an array for cross points hystogramm");
#endif

		if (cp_hyst)
			delete[] cp_hyst;

		if (cp_dist)
			delete[] cp_dist;

		return FALSE;
	}

	memset(cp_hyst, 0, cp_size*size_int32_t);
	memset(cp_dist, 0, cp_size*size_int32_t);

	int32_t endj;

	for (i = 0; i < cp; i++)
	{
		endj = MIN(max_cp, aCP[i].x + hyst_spread) - min_cp;
		cp_dist[aCP[i].x - min_cp]++;

		for (int32_t j = MAX(0, aCP[i].x - min_cp - hyst_spread); j < endj; j++)
			cp_hyst[j]++;
	}

	int32_t start, sum_cp = 0;

	for (i = 0; i < cp_size; i++)
		if (cp_hyst[i] > 0)
		{
			if (sum_cp == 0)
			{
				start = i;
				sum_cp = cp_dist[i];
			}
			else sum_cp += cp_dist[i];
		}
		else if (sum_cp > 0)
		{
			if (sum_cp > 1)
			{
				int32_t start_cp = 0;

				for (int32_t j = 0; j < start; j++)
					start_cp += cp_dist[j];

				if (!extractLines(work_data, &aCP[start_cp], sum_cp, aContLines, nContLines, is_horiz))
					break;
			}

			sum_cp = 0;
		}

	if (sum_cp > 0)
	{
		int32_t start_cp = 0;

		for (int32_t j = 0; j < start; j++)
			start_cp += cp_dist[j];

		extractLines(work_data, &aCP[start_cp], sum_cp, aContLines, nContLines, is_horiz);
	}

	delete[] cp_hyst;
	delete[] cp_dist;

	return TRUE;
}
/********************************************************************************************/
Bool32 extractLines(const FLLData *work_data, Point32 *aCP, const int32_t nCP,
					const LineData* aContLines, const int32_t nContLines, const bool is_horiz)
{
	//check input data
	//----------------
	if (!work_data || !aContLines || !aCP)
	{
#ifdef _DEBUG
		PrintErrorString("invalid input data for extractLines function");
#endif

		return FALSE;
	}

	//extracting lines
	//-----------------
	int32_t *aCPStatus = new int32_t[nCP];
//status switches
#define SW_WORK 1
#define SW_DELAYED 2
#define SW_EXAMINED 3

	if (!aCPStatus)
	{
#ifdef _DEBUG
		PrintErrorString("unable to create cross point status array");
#endif

		return FALSE;
	}

	memset(aCPStatus, 0, nCP*size_int32_t);
	qsort(aCP, nCP, size_Point32, CompareY);

	PAGEINFO* page_info = work_data->pInfo;
	Point32 plast_cp;
	int32_t nWork, first_cp = 0, finish_cp = nCP, last_cp;
	int32_t nIncline = page_info->Incline2048;
	int32_t page_beg_x = work_data->page_beg_x;
	int32_t page_beg_y = work_data->page_beg_y;
	int32_t page_end_x = work_data->page_end_x;
	int32_t page_end_y = work_data->page_end_y;
	int32_t line_distance_x = work_data->line_distance_x;
	int32_t line_distance_y = work_data->line_distance_y;
	int32_t near_ld_x = line_distance_x>>1;
	int32_t near_ld_y = line_distance_y>>1;
	bool bShowCR = work_data->bShowCheckedRects;
	bool bPrintCR = work_data->bPrintCheckedRects;

	do
	{
	    int32_t i;
		nWork = 0;

		//filtrate cross points
		//---------------------
		for (i = first_cp; i < finish_cp; i++)
		{
			if (aCPStatus[i] == SW_EXAMINED)
				continue;

			if (nWork == 0)
			{
				aCPStatus[i] = SW_WORK;
				nWork = 1;
				first_cp = last_cp = i;
				plast_cp = aCP[i];
			}
			else if (aCP[i].y - plast_cp.y <= near_ld_y && abs(aCP[i].x - plast_cp.x) > near_ld_x || abs(aCP[i].x - plast_cp.x) > line_distance_x)
				aCPStatus[i] = SW_DELAYED;
			else if (aCP[i].y - plast_cp.y <= near_ld_y && abs(aCP[i].x - plast_cp.x) <= near_ld_x)
				aCPStatus[i] = SW_EXAMINED;
			else
			{
				aCPStatus[i] = SW_WORK;
				nWork++;
				plast_cp = aCP[i];
				last_cp = i;
			}
		}

		//create intervals
		//----------------
		if (nWork > 1)
		{
			nWork--;

			struct tagInterval
			{
				Point32 beg, end;
				NR_SimpLine term;
				int32_t black;
//				int32_t base_line;
				Rect32 rect;
			} *aInterval = new tagInterval[nWork];

			if (!aInterval)
			{
#ifdef _DEBUG
				PrintErrorString("unable to create intervals array");
#endif

				break;
			}

			int32_t cur_cp = first_cp, ind = 0;
			const int32_t interval_spread = 2;

			for (i = first_cp + 1; i <= last_cp; i++)
				if (aCPStatus[i] == SW_WORK)
				{
					aInterval[ind].beg = aCP[cur_cp];
					aInterval[ind].end = aCP[i];
					aInterval[ind].term.Beg_X = aCP[cur_cp].x;
					aInterval[ind].term.Beg_Y = aCP[cur_cp].y;
					aInterval[ind].term.End_X = aCP[i].x;
					aInterval[ind].term.End_Y = aCP[i].y;
					aInterval[ind].black = 0;
//					aInterval[ind].base_line = -1;//-1 means no base line

					getLineIdealStrictRectangular(&aInterval[ind].term, &aInterval[ind].rect, false, 0);

					aInterval[ind].rect.left -= interval_spread;
					aInterval[ind].rect.top -= interval_spread;
					aInterval[ind].rect.right += interval_spread;
					aInterval[ind].rect.bottom += interval_spread;
					ind++;
					cur_cp = i;
				}

			//analyse intervals by lines
			//--------------------------
			int32_t based_intervals = 0;

			for (i = 0; i < nContLines; i++)
				for (int32_t j = 0; j < nWork; j++)
				{
					if (aInterval[j].black == FULL_BLACK ||
						!checkCross(&aInterval[j].rect, &aContLines[i].rect, is_horiz))
						continue;

					if (is_horiz)
					{
						if (aInterval[j].beg.y >= aContLines[i].rect.top &&
							aInterval[j].end.y <= aContLines[i].rect.bottom)
						{
							aInterval[j].black = FULL_BLACK;
							based_intervals++;
						}

//						aInterval[j].base_line = i;
					}
					else
					{
						if (aInterval[j].beg.y >= aContLines[i].rect.left &&
							aInterval[j].end.y <= aContLines[i].rect.right)
						{
							aInterval[j].black = FULL_BLACK;
							based_intervals++;
						}

//						aInterval[j].base_line = i;
					}
				}

			if (based_intervals < nWork)
			{
				bool was_drawn = false;

				//check raster of nonbased intervals
				//----------------------------------
				for (i = 0; i < nWork; i++)
				{
					if (aInterval[i].black == FULL_BLACK)
						continue;

					if ((aInterval[i].black = getRasterBlack(&aInterval[i].term, is_horiz, work_data)) == -1)
						break;

					if (bShowCR)
					{
						Rect32 r;
						Rect16 dr;

						getLineIdealStrictRectangular(&aInterval[i].term, &r, !is_horiz, 0);

						dr.top = (int16_t)r.top;
						dr.left = (int16_t)r.left;
						dr.bottom = (int16_t)r.bottom;
						dr.right = (int16_t)r.right;
						was_drawn = true;

						LDPUMA_DrawRect(work_data->hWnd, &dr, 0, RGB(0, 255, 0), -100, 2);
					}

					if (bPrintCR)
						LDPUMA_ConsoleN("interval (%d, %d)-(%d, %d), black=%d", aInterval[i].term.Beg_X, aInterval[i].term.Beg_Y, aInterval[i].term.End_X, aInterval[i].term.End_Y, aInterval[i].black);
				}

				if (was_drawn)
				{
					LDPUMA_WaitUserInput(hShowCheckedRects, work_data->hWnd);
					LDPUMA_DeleteRects(work_data->hWnd, 2);
				}

				if (bPrintCR)
					LDPUMA_ConsoleN("-------------- Finish Line ---------------");

				//extract lines by intervals
				//--------------------------
				for (i = 0; i < nWork; i++)
				{
				    int32_t j, k;
//					if (!checkBlack(aInterval[i].black, aInterval[i].base_line == -1 ? false : (aContLines[aInterval[i].base_line].flag & FL_GOOD)))
					if (!checkBlack(aInterval[i].black, false))
						continue;

					for (j = i + 1; j < nWork; j++)
//						if (!checkBlack(aInterval[j].black, aInterval[j].base_line == -1 ? false : (aContLines[aInterval[j].base_line].flag & FL_GOOD)))
						if (!checkBlack(aInterval[j].black, false))
							break;

					for (k = i; k < j; k++)
						if (aInterval[k].black < FULL_BLACK)
							break;

					if (k == j)
					{
						i = j;

						continue;
					}

					Point32 start_line = aInterval[i].beg;
					Point32 finish_line = aInterval[j - 1].end;

					if (start_line.y == finish_line.y)
					{
						i = j;

						continue;
					}

					int32_t lend1, lend2;
					int32_t* aLEnd1 = new int32_t[nContLines];
					int32_t* aLEnd2 = new int32_t[nContLines];

					if (!aLEnd1 || !aLEnd2)
					{
#ifdef _DEBUG
						PrintErrorString("unable to create free end arrays");
#endif

						if (aLEnd1)
							delete[] aLEnd1;

						if (aLEnd2)
							delete[] aLEnd2;

						break;
					}

					double angle = (double)(start_line.x - finish_line.x)/(finish_line.y - start_line.y);
					int32_t nLEnd1 = 0, nLEnd2 = 0;
					Point32 cur_line = finish_line;

					//try to find free ends
					//---------------------
					for (k = 0; k < nContLines; k++)
					{
						if (is_horiz)
						{
							lend1 = aContLines[k].rect.top;
							lend2 = aContLines[k].rect.bottom;
						}
						else
						{
							lend1 = aContLines[k].rect.left;
							lend2 = aContLines[k].rect.right;
						}

						if (lend1 < start_line.y && lend2 > start_line.y)
						{
							aLEnd1[nLEnd1] = lend1;
							nLEnd1++;
						}

						if (lend1 < finish_line.y && lend2 > finish_line.y)
						{
							aLEnd2[nLEnd2] = lend2;
							nLEnd2++;
						}
					}

					if (nLEnd1 > 0)
					{
						qsort(aLEnd1, nLEnd1, size_int32_t, CompareAsc);

						for (k = 0; k < nLEnd1; k++)
							if (getEnds(&start_line, &cur_line, angle, is_horiz, aLEnd1[k], finish_line.y, work_data, true))
								break;
					}

					if (nLEnd2 > 0)
					{
						qsort(aLEnd2, nLEnd2, size_int32_t, CompareDesc);

						for (k = 0; k < nLEnd2; k++)
							if (getEnds(&finish_line, &cur_line, angle, is_horiz, start_line.y, aLEnd2[k], work_data, false))
								break;
					}

					delete[] aLEnd1;
					delete[] aLEnd2;

					updateLine(work_data, is_horiz, &start_line, &finish_line);
/*
					//get line crosses
					//----------------
					int32_t strip_len = finish_line.y - start_line.y + 1;
					uchar* aStrip = new uchar[strip_len];

					if (!aStrip)
					{
#ifdef _DEBUG
						PrintErrorString("unable to create strip array");
#endif

						break;
					}

					memset(aStrip, 0, strip_len);

					Rect32 strip_rect;

					strip_rect.top = start_line.y;
					strip_rect.bottom = finish_line.y;

					if (start_line.x < finish_line.x)
					{
						strip_rect.left = start_line.x;
						strip_rect.right = finish_line.x;
					}
					else
					{
						strip_rect.right = start_line.x;
						strip_rect.left = finish_line.x;
					}

					for (k = 0; k < nContLines; k++)
					{
						if (!(aContLines[k].flag & FL_GOOD) ||
							!checkCross(&strip_rect, &aContLines[k].rect, is_horiz))
							continue;

						int32_t beg, end;

						if (is_horiz)
						{
							beg = MAX(start_line.y, aContLines[k].rect.top);
							end = MIN(finish_line.y, aContLines[k].rect.bottom);
						}
						else
						{
							beg = MAX(start_line.y, aContLines[k].rect.left);
							end = MIN(finish_line.y, aContLines[k].rect.right);
						}

						beg -= start_line.y;
						end -= start_line.y;

						if (beg < 0 || end >= strip_len)
						{
#ifdef _DEBUG
							PrintErrorString("strip bounds exceeded");
#endif

							continue;
						}

						for (int32_t l = beg; l <= end; l++)
							aStrip[l] = 1;
					}

					//get lines
					//---------
					Point32 start_pos, end_pos;

					angle = (double)(start_line.x - finish_line.x)/(finish_line.y - start_line.y);

					for (k = 0; k < strip_len; k++)
					{
						if (aStrip[k] > 0)
							continue;

						for (int32_t l = k + 1; l < strip_len && aStrip[l] == 0; l++);

						start_pos.y = k + start_line.y;
						end_pos.y = l - 1 + start_line.y;
						start_pos.x = finish_line.x + (int32_t)(angle*(finish_line.y - start_pos.y));
						end_pos.x = finish_line.x + (int32_t)(angle*(finish_line.y - end_pos.y));

						updateLine(work_data, is_horiz, &start_pos, &end_pos);

						k = l;
					}

					delete[] aStrip;
*/
/*
					for (k = i; k < j; k++)
					{
//						if (aInterval[k].beg.y < start_line.y || aInterval[k].base_line == -1)
						if (aInterval[k].beg.y < start_line.y)
							continue;

						if (!(aContLines[aInterval[k].base_line].flag & FL_GOOD))
							continue;

						int32_t line_top = is_horiz ? aContLines[aInterval[k].base_line].rect.top : aContLines[aInterval[k].base_line].rect.left;

						cur_line.y = MAX(start_line.y, is_horiz ? aContLines[aInterval[k].base_line].rect.top : aContLines[aInterval[k].base_line].rect.left);
						cur_line.x = finish_line.x + (int32_t)(angle*(finish_line.y - cur_line.y));

						if (start_line.y < cur_line.y)
							updateLine(work_data, is_horiz, &start_line, &cur_line);

						start_line.y = MIN(finish_line.y, is_horiz ? aContLines[aInterval[k].base_line].rect.bottom : aContLines[aInterval[k].base_line].rect.right);
						start_line.x = finish_line.x + (int32_t)(angle*(finish_line.y - start_line.y));
						cur_line = finish_line;
					}

					if (start_line.y < cur_line.y)
						updateLine(work_data, is_horiz, &start_line, &cur_line);
*/
					i = j;
				}
			}

			delete[] aInterval;
		}

		//update status array
		//-------------------
		if (nWork > 0)
		{
			for (i = first_cp; i < finish_cp; i++)
				if (aCPStatus[i] == SW_WORK)
					aCPStatus[i] = SW_EXAMINED;

			for (i = first_cp + 1; i < finish_cp; i++)
				if (aCPStatus[i] == SW_DELAYED)
					break;

			first_cp = i;

			for (i = finish_cp - 1; i > first_cp; i--)
				if (aCPStatus[i] == SW_DELAYED)
					break;

			finish_cp = i + 1;
		}
	}
	while (nWork > 0);

	delete[] aCPStatus;

	return TRUE;
}
/********************************************************************************************/
Bool32 checkBlack(int32_t black, bool is_good)
{
	if (black < hRLINE_Pass2Lost1 || black < hRLINE_Pass2Lost2 && !is_good)
		return FALSE;

	return TRUE;
}
/********************************************************************************************/
int32_t getRasterBlack(NR_SimpLine* term, bool is_horiz, const FLLData* work_data)
{
	PAGEINFO* page_info = work_data->pInfo;
	int32_t page_beg_x = work_data->page_beg_x;
	int32_t page_beg_y = work_data->page_beg_y;
	int32_t page_end_x = work_data->page_end_x;
	int32_t page_end_y = work_data->page_end_y;
	int32_t nIncline = page_info->Incline2048;
	Rect32 rterm;

	if (!is_horiz)
	{
		int32_t tbeg = term->Beg_X;
		int32_t tend = term->End_X;

		term->Beg_X = term->Beg_Y;
		term->End_X = term->End_Y;
		term->Beg_Y = tbeg;
		term->End_Y = tend;
	}

	REAL_XY(term->Beg_X, term->Beg_Y);
	REAL_XY(term->End_X, term->End_Y);

	//check negative for horizonal intervals
	//--------------------------------------
	if (!is_horiz)
	{
		const int32_t spread = 10;

		getLineIdealStrictRectangular(term, &rterm, !is_horiz, 0, spread);//we get real rectangular here

		rterm.left += spread;
		rterm.right -= spread;

		if (rterm.left < page_beg_x)
			rterm.left = page_beg_x;

		if (rterm.right > page_end_x - 1)
			rterm.right = page_end_x - 1;

		if (rterm.top < page_beg_y)
			rterm.top = page_beg_y;

		if (rterm.bottom > page_end_y - 1)
			rterm.bottom = page_end_y - 1;

		if (rterm.left > rterm.right || rterm.top > rterm.bottom)
		{
#ifdef _DEBUG
			PrintErrorString("invalid extended interval rectangular");
#endif
			return -1;
		}

		char* pRaster = NULL;

		if (!SL_GetRaster(&rterm, (uchar**)&pRaster, page_info/*, NULL*/))
		{
#ifdef _DEBUG
			PrintErrorString("failed to get interval raster");
#endif

			return -1;
		}

		int32_t rast_size = ((rterm.right - rterm.left + 8)>>3)*(rterm.bottom - rterm.top + 1);
		int32_t black_byte = 0;

		for (int32_t i = 0; i < rast_size; i++)
			if ((uchar)pRaster[i] == 255)
				black_byte++;

		if (black_byte*3 > rast_size)
			return 0;
	}

	getLineIdealStrictRectangular(term, &rterm, !is_horiz, 0);//we get real rectangular here

	if (rterm.left < page_beg_x)
		rterm.left = page_beg_x;

	if (rterm.right > page_end_x - 1)
		rterm.right = page_end_x - 1;

	if (rterm.top < page_beg_y)
		rterm.top = page_beg_y;

	if (rterm.bottom > page_end_y - 1)
		rterm.bottom = page_end_y - 1;

	if (rterm.left > rterm.right || rterm.top > rterm.bottom)
	{
#ifdef _DEBUG
		PrintErrorString("invalid interval rectangular");
#endif

		return -1;
	}

	char* pRaster = NULL;

	if (!SL_GetRaster(&rterm, (uchar**)&pRaster, page_info/*, NULL*/))
	{
#ifdef _DEBUG
		PrintErrorString("failed to get interval raster");
#endif

		return -1;
	}

	//analyse raster
	//--------------
	int32_t rast_w = rterm.right - rterm.left + 1;
	int32_t rast_h = rterm.bottom - rterm.top + 1;
	int32_t rast_bytew = (rast_w + 7)>>3;
	int32_t black = 0;
	int32_t begj, start, k, rast_len, j;
	double tan_angle;
	int32_t max_white = 0, white = 0;
	int32_t max_white_len = is_horiz ? work_data->line_distance_y : work_data->line_distance_x;

	if (!is_horiz)
	{
		if (term->End_X == term->Beg_X)
		{
#ifdef _DEBUG
			PrintErrorString("horizontal line ends coincedence");
#endif

			return -1;
		}

		rast_len = rast_w;
		tan_angle = (double)(term->End_Y - term->Beg_Y)/(term->End_X - term->Beg_X);
		begj = term->Beg_Y - rterm.top;

		for (j = 0; j < rast_w; j++)
		{
			start = begj + (int32_t)(j*tan_angle);

			if (start < 0) start = 0;
			else if (start >= rast_h) start = rast_h - 1;

			if (pRaster[start*rast_bytew + (j>>3)] & bits_array[j&7])
			{
				if (max_white < white)
				{
					max_white = white;

					if (max_white > max_white_len)
						return 0;
				}

				black++;
				white = 0;
			}
			else if (start < rast_h - 1)
			{
				if (pRaster[(start + 1)*rast_bytew + (j>>3)] & bits_array[j&7])
				{
					if (max_white < white)
					{
						max_white = white;

						if (max_white > max_white_len)
							return 0;
					}

					white = 0;
					black++;
				}
				else white++;
			}
			else white++;
		}
	}
	else
	{
		if (term->End_Y == term->Beg_Y)
		{
#ifdef _DEBUG
			PrintErrorString("vertical line ends coincedence");
#endif

			return -1;
		}

		rast_len = rast_h;
		tan_angle = (double)(term->Beg_X - term->End_X)/(term->End_Y - term->Beg_Y);
		begj = tan_angle > 0 ? term->Beg_X - rterm.left - 1 : term->Beg_X - rterm.left;

		for (j = 0; j < rast_h; j++)
		{
			start = begj - (int32_t)(j*tan_angle);
			k = j*rast_bytew;

			if (start < 0) start = 0;
			else if (start >= rast_w) start = rast_w - 1;

			if (pRaster[k + (start>>3)] & bits_array[start&7])
			{
				if (max_white < white)
				{
					max_white = white;

					if (max_white > max_white_len)
						return 0;
				}

				black++;
				white = 0;
			}
			else if (start < rast_w - 1)
			{
				if (pRaster[k + ((start + 1)>>3)] & bits_array[(start + 1)&7])
				{
					if (max_white < white)
					{
						max_white = white;

						if (max_white > max_white_len)
							return 0;
					}

					white = 0;
					black++;
				}
				else white++;
			}
			else white++;
		}
	}

	if (rast_len < 1)
	{
#ifdef _DEBUG
		PrintErrorString("invalid length of interval");
#endif

		return -1;
	}

	if (max_white > max_white_len)
		return 0;

	return black*(FULL_BLACK - 1)/rast_len;
}
/********************************************************************************************/
Bool32 getEnds(Point32* cur_end, const Point32* finish_end, double angle, bool is_horiz,
			 int32_t lend1, int32_t lend2, const FLLData* work_data, bool checkupper)
{
	NR_SimpLine term;
	int32_t new_end;

	if (cur_end->y > lend1 && cur_end->y < lend2)
	{
		if (checkupper)
		{
			term.End_X = cur_end->x;
			term.End_Y = cur_end->y;
			term.Beg_Y = lend1;
			term.Beg_X = new_end = finish_end->x + (int32_t)(angle*(finish_end->y - term.Beg_Y));
		}
		else
		{
			term.Beg_X = cur_end->x;
			term.Beg_Y = cur_end->y;
			term.End_Y = lend2;
			term.End_X = new_end = finish_end->x + (int32_t)(angle*(finish_end->y - term.End_Y));
		}

		if (checkBlack(getRasterBlack(&term, is_horiz, work_data), false))
		{
			cur_end->x = new_end;
			cur_end->y = checkupper ? lend1 : lend2;

			return TRUE;
		}
	}

	return FALSE;
}
/********************************************************************************************/
void updateLine(/*CLINE_handle* hLine, */const FLLData* work_data, bool is_horiz, Point32* begin,
				Point32* end)
{
	int32_t nIncline = work_data->pInfo->Incline2048;
	DLine dLine;
//	CLINE_handle hContainer = work_data->hCLINE;
	CLINE_handle hContainer = work_data->hTempContainer;
	int32_t bad_lines;
	LineData *aBadLines;
	Rect32 rect;
	int32_t i;

	InitLine(&dLine);

	dLine.Line.Wid10 = 10;

	if (is_horiz)
	{
		dLine.Line.Beg_X = begin->x;
		dLine.Line.Beg_Y = begin->y;
		dLine.Line.End_X = end->x;
		dLine.Line.End_Y = end->y;
		bad_lines = work_data->vert_bad_lines;
		aBadLines = work_data->aBadVertLines;
	}
	else
	{
		dLine.Line.Beg_X = begin->y;
		dLine.Line.Beg_Y = begin->x;
		dLine.Line.End_X = end->y;
		dLine.Line.End_Y = end->x;
		bad_lines = work_data->hori_bad_lines;
		aBadLines = work_data->aBadHoriLines;
	}

	//check bad line
	//--------------
	getLineIdealStrictRectangular(&dLine.Line, &rect, !is_horiz, 0);

	for (i = 0; i < bad_lines; i++)
		if (rect.top <= aBadLines[i].rect.bottom &&
			rect.left <= aBadLines[i].rect.right &&
			rect.bottom >= aBadLines[i].rect.top &&
			rect.right >= aBadLines[i].rect.left)
			break;

	if (i < bad_lines)
		return;

	REAL_XY(dLine.Line.Beg_X, dLine.Line.Beg_Y);
	REAL_XY(dLine.Line.End_X, dLine.Line.End_Y);

	if (!is_horiz && dLine.Line.End_X - dLine.Line.Beg_X <= abs(dLine.Line.End_Y - dLine.Line.Beg_Y) ||
		is_horiz && dLine.Line.End_Y - dLine.Line.Beg_Y <= abs(dLine.Line.End_X - dLine.Line.Beg_X))
		return;

	CLINE_handle hTempLine = CLINE_AddNewLine(hContainer);

	CLINE_SetLineData(hTempLine, &dLine);

	if (!FindLineFrag(hTempLine, TRUE, TRUE, hContainer, TRUE, TRUE))
		CLINE_DelLine(hContainer, hTempLine);
	else
	{
		CPDLine pLine = CLINE_GetLineData(hTempLine);

		if (!pLine)
			CLINE_DelLine(hContainer, hTempLine);
		else if (!(pLine->Flags & LI_IsTrue))
			CLINE_DelLine(hContainer, hTempLine);
	}
/*
	CPDLine pTempLine = CLINE_GetLineData(hTempLine);
	CPDLine pLine = CLINE_GetLineData(*hLine);

	if (!pTempLine || !pLine)
		return;

	memcpy(&dLine, pLine, size_DLine);
*/
}
/********************************************************************************************/
Bool32 checkCross(const Rect32* rect1, const Rect32* rect2, bool is_horiz)
{
	if (is_horiz)
	{
		if (rect1->left > rect2->right ||
			rect1->right < rect2->left ||
			rect1->top > rect2->bottom ||
			rect1->bottom < rect2->top)
			return FALSE;
	}
	else
	{
		if (rect1->left > rect2->bottom ||
			rect1->right < rect2->top ||
			rect1->top > rect2->right ||
			rect1->bottom < rect2->left)
			return FALSE;;
	}

	return TRUE;
}
