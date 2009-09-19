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

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <fstream>
#include <cstring>
#include <time.h>

#include "stdafx.h"
#include "linedefs.h"
#include "lnsdefs.h"
#include "cpage.h"
#include "ctiimage.h"

#include "rshelllinescom.h"
#include "rline.h"

#include "dpuma.h"
#include "cline.h"
#include "ccom.h"
#include "pumadef.h"

#include "rsl.h"

#include "compat_defs.h"
#include "minmax.h"

# define INCLINE_FACTOR  2048
#define OffsetCoor 4

# define IDEAL_XY(x, y)   \
         {\
             y = (int16_t) (y - (int32_t) x * nIncline / INCLINE_FACTOR);\
             x = (int16_t) (x + (int32_t) y * nIncline / INCLINE_FACTOR);\
         }

# define REAL_XY(x, y)   \
         {\
             x = (int16_t) (x - (int32_t) y * nIncline / INCLINE_FACTOR);\
             y = (int16_t) (y + (int32_t) x * nIncline / INCLINE_FACTOR);\
		}

#define SHORT_EVENT 50
#define SHORT_DOTLINE 100
#define MAX_COUNT_BE 1000
#define RESULT 2
/*----------------------------------------------------------------------------*/

//Настройки


/*----------------------------------------------------------------------------*/

//Параметры

//extern Handle hUseCLine;

extern Handle Exit2;
extern Handle Exit3;
extern Handle MainWork2;
extern Handle MainWork3;
extern Handle Prep2;
extern Handle GetContainer3;
extern Handle PutContainer2;
extern Handle PutContainer3;
extern Handle Epilog2;
extern Handle hDotKilledD;
extern Handle hWriteLineInFile;
extern Handle hWriteLineInFile3;
extern Handle hDebugDrawAllLineFragments;
extern Handle hSkipDelSmallLines;
extern Handle hLinesPass2;
extern Handle    hPrint1Cr;
extern Handle    hPrint2Cr;
extern Handle    hPrint3Cr;
extern Handle    hPrint4Cr;
extern Handle hDrawCrRast;
extern Handle hRLINE_CorrectDrawLines;
extern int32_t hMinInterval;
extern int32_t h1Interval;
extern int32_t h2Interval;
extern int32_t hMaxInterval;
extern int32_t hSmallLineLen;
extern int32_t hLongLineLen;
extern int32_t hMaxDegreeDiff;
extern int32_t hMaxShift;
extern int32_t hRastDelta;
extern Handle hRLINE_CorrectDrawFrags;
extern Handle hRLINE_Pass2DrawLines;
extern Handle hRLINE_Pass2DrawFrags;
extern Handle hRLINE_Pass3DrawLines;
extern Handle hRLINE_Pass3DrawFrags;
extern Handle hDebugDrowFriendLines;
extern Handle hDebugDrowGroupOfExtensibleLines;
extern Handle hDebugDrawLosedVerticalLines;
extern Handle hRLINE_CorrectDebugFrag;
extern Handle hWriteLineInFile2;
extern Handle hLinesPass2DebugSkipDouble;
extern Handle hLinesPass2DebugSkipGlue;
extern Handle hDebugShortLinesPrint;
extern Handle hDebugShortLinesDrawRect;
extern Handle hDebugPrintCrossCheck;
extern Handle hShowCPLines;

/// BogDmitry
extern Bool32 gbRSLT;
// struct VerLine
// {
//     uint32_t wTbl;
//     Handle hCCOM;
//     Handle hCPAGE;
//     PBool32 pneed_clean;
//     Bool32 bCheck;
// };

#define RSL_HANDLE 1;
/// BogDmitry

Handle MainWindowD;

	//Almi 16.05.01
extern int MinHorLenForTrue, MinVerLenForTrue;
/*----------------------------------------------------------------------------*/




//My function's diclarations

CLINE_handle FindLine(CLINE_handle hContainer, CLINE_handle processedline, Bool vert);
//Bool MyGetLines(CPageLines* PLines,CLINE_handle hCLINE);
//Bool AddLenLineMas(LineInfo** ppRc,int& len,int add);
//void DelLineMas(LineInfo* masp);
Bool MyPutLines(CLINE_handle hContainerOut, CLINE_handle hCLINE, Bool dotline);
void DeleteBadDotLine(CLINE_handle hCLINE,CCOM_handle hCCOM,Handle hCPAGE);
void SetLineDegree(DLine* line);
//void DeletePLines(CPageLines* PLines);
Bool MyGetLines(/*CLINE_handle linecontainer,Handle hCPage,*/CLINE_handle hCLINE, int32_t* CountLines);
void PrintLines(CLINE_handle hContainer, const char* FileName);
Bool32 GlueLines(CLINE_handle hContainer, Handle hCCOM);
Bool32 FindExtLines(CLINE_handle* hLinesMass, int32_t CountLines, CLINE_handle hExtContainer, Bool32 IsHor, CCOM_comp *pCompMass, int32_t CountMass, CLINE_handle hContainer);
void MergeLines(CLINE_handle hLine, CLINE_handle hLineExt, CLINE_handle hContainer);
Bool32 CheckAllLines(Rect32* CurrLine, const Rect32* pLine, Bool32 IsHor, int32_t nIncline, CLINE_handle* hLinesMass, int32_t CountLines, int32_t interval, int32_t long_line);
int32_t getLineComps(CCOM_comp** pCompMass, int32_t max_width, int32_t max_height, Handle hCCOM);
int32_t countCompLen(CCOM_comp* pCompMass, int32_t CountComp, CPDLine pLine, CPDLine pLineExt, Bool32 IsHor);
int32_t findFirstComp(CCOM_comp* pCompMass, int32_t CountComp, Rect32 RastRect, Bool32 IsHor);
Bool32 CorrectDoubleLines(CLINE_handle hContainer);
void getLineIdealStrictRectangular(const NR_SimpLine *pdLine, Rect32* pRect, bool is_horiz, int32_t nIncline, int32_t spread = 0);
int32_t findLostLines(CLINE_handle hCLINE, PAGEINFO* info);
//Bool32 writeBin(char* file_name, int32_t nIncline, Rect16* aRect, uint32_t* aType, uint32_t* aNumber, int32_t aCount);
//PAGEINFO gl_page_info;
/*----------------------------------------------------------------------------*/

RLINE_FUNC(Bool32) RLINE_LinesPass1(Handle hCPage,Handle hCCOM,void* phCLINE,PBool32 pgneed_clean_line, Bool32 sdl, uchar lang)
{
//int32_t* a = new int32_t;

 LDPUMA_Skip(Prep2);

 if((!hCPage)||(!hCCOM))
	 return FALSE;

/// BogDmitry
 if(gbRSLT)
 {
     uint32_t type = RSL_HANDLE;
     Handle* phCPAGE = &hCPage;
     void* aa = (void*) phCPAGE;
     RSL_SetImportData(type, aa);
     return RESULT;
 }
/// BogDmitry

 PAGEINFO info = {0};

 if(!GetPageInfo(hCPage,&info)) return FALSE;

 MainWindowD=NULL;
 MainWindowD=LDPUMA_GetWindowHandle ("Изображение после разворота");
 if(!MainWindowD)
    MainWindowD=LDPUMA_GetWindowHandle ("Main");


 int time=clock();
 CLINE_handle hCLINE=*((CLINE_handle*)phCLINE);
 int i;
 // LDPUMA_ConsoleN("");
// LDPUMA_ConsoleN("Линии 2 проход");

// CLINE_handle linecontainer = CLINE_CreateContainer(FALSE);
 int32_t CountLines = 0;

 if(!MyGetLines(/*linecontainer,hCPage,*/hCLINE, &CountLines))
	 return FALSE;

// LDPUMA_ConsoleN("Взятие из контейнера %d",clock()-time);

 time=clock();

 LDPUMA_Skip(MainWork2);

// int32_t CountLines = CLINE_GetLineCount(/*linecontainer*/hCLINE);

 if(!CountLines)
	 return TRUE;

 CLINE_handle* linesmass = new CLINE_handle[CountLines];
 CLINE_handle currentline, hLinePrev = NULL, hLineNext;
/* for(i = 0, currentline = CLINE_GetFirstLine(linecontainer); i < CountLines; i++, currentline = CLINE_GetNextLine(currentline))
     linesmass[i] = currentline;
*/
 for(i = 0, currentline = CLINE_GetFirstLine(hCLINE); i < CountLines; currentline = CLINE_GetNextLine(currentline))
 {
	 CPDLine pLine = CLINE_GetLineData(currentline);
	 if (!pLine) continue;
	 else if (pLine->Flags&LI_Pointed) continue;
	 else
	 {
		linesmass[i] = currentline;
		i++;
	 }
 }

 currentline = CLINE_GetFirstLine(/*linecontainer*/hCLINE);
 CPDLine clinedata;
 i = 0;
 /*****************  горизонтальные линии  ******************************/

	while (currentline)
	{
        clinedata = CLINE_GetLineData(currentline);

		if (!clinedata)
        {
            currentline = CLINE_GetNextLine(currentline);
            continue;
        }

		if (clinedata->Flags&LI_Pointed)
        {
            currentline = CLINE_GetNextLine(currentline);
            continue;
        }

        i++;

        if (i>CountLines) break;

//        clinedata = CLINE_GetLineData(currentline);
        if (clinedata->Dir != LD_Horiz)
        {
            currentline = CLINE_GetNextLine(currentline);
            continue;
        }

        if ((clinedata->Line.End_X-clinedata->Line.Beg_X) >= 100)//Almi
	    {
            DLine* oldlinedata = new DLine;
            memcpy(oldlinedata, clinedata, sizeof(DLine));
            CLINE_handle hwork_line = FindLine(/*linecontainer*/hCLINE, currentline, FALSE);
            if(hwork_line)
	        {//гнутые
                DLine* work_line = new DLine;
                memcpy(work_line, clinedata, sizeof(DLine));
		        if(work_line->Line.End_X-work_line->Line.Beg_X>1000&&work_line->Line.End_X<oldlinedata->Line.End_X&&oldlinedata->Line.End_X-work_line->Line.End_X<100)
		        {
                    hLineNext = CLINE_GetNextLine(currentline);
		            if(hLineNext)
		            {
		                CPDLine pLineNext = CLINE_GetLineData(hLineNext);
		                if(abs(oldlinedata->Line.End_X-pLineNext->Line.End_X)<100&&((abs(oldlinedata->Line.Beg_Y-pLineNext->Line.Beg_Y)+abs(oldlinedata->Line.End_Y-pLineNext->Line.End_Y))>>1)<10)
			            work_line->Line.End_X=oldlinedata->Line.End_X;
		            }
		            if(hLinePrev)
		            {
		                CPDLine pLinePrev = CLINE_GetLineData(hLinePrev);
		                if(abs(oldlinedata->Line.End_X-pLinePrev->Line.End_X)<100&&((abs(oldlinedata->Line.Beg_Y-pLinePrev->Line.Beg_Y)+abs(oldlinedata->Line.End_Y-pLinePrev->Line.End_Y))>>1)<10)
			            work_line->Line.End_X=oldlinedata->Line.End_X;
		            }
		        }

                CLINE_SetLineData(currentline, work_line);
                linesmass[i-1] = NULL;
                delete work_line;
	        }
/*          else
            {
                linesmass[i-1] = currentline;
            }
*/
            delete oldlinedata;
	    }

        hLinePrev = currentline;
        currentline = CLINE_GetNextLine(currentline);
	}
 /*****************  вертикальные линии  ******************************/
    currentline = CLINE_GetFirstLine(/*linecontainer*/hCLINE);
    i = 0;

	while (currentline)
	{
        clinedata = CLINE_GetLineData(currentline);

		if (!clinedata)
        {
            currentline = CLINE_GetNextLine(currentline);
            continue;
        }

		if (clinedata->Flags&LI_Pointed)
        {
            currentline = CLINE_GetNextLine(currentline);
            continue;
        }

        i++;
        if (i>CountLines) break;

//        clinedata = CLINE_GetLineData(currentline);
        if (clinedata->Dir != LD_Verti)
        {
            currentline = CLINE_GetNextLine(currentline);
            continue;
        }

        if ((clinedata->Line.End_Y-clinedata->Line.Beg_Y) >= 50)//Almi
	    {
            if(FindLine(/*linecontainer*/hCLINE, currentline, TRUE))
            {
                linesmass[i-1] = NULL;
//		DelLineMas(pLti->Hor.Lns);
//		DelLineMas(pLti->Ver.Lns);
//      return FALSE;
            }
	    }

        currentline = CLINE_GetNextLine(currentline);
	}

/*****************  search dot lines  II проход ******************************/
    if (sdl)
    {
//        LDPUMA_Console("in \n");
        FindDotLines(hCCOM, hCPage, /*linecontainer*/hCLINE);
    }
    else
    {
/*
        CLine* myDotLine;
//        CEvent* myDotLineEvent;
//        CInterval* myDotLineEventInterval;
        CLINE_handle hmyDotLine = CLINE_GetFirstLine(hCLINE);
        while (hmyDotLine)
        {
            CPDLine myDotLineData = CLINE_GetLineData(hmyDotLine);
            if (!(myDotLineData->Flags&LI_Pointed))
            {
                hmyDotLine = CLINE_GetNextLine(hmyDotLine);
                continue;
            }
            myDotLine = new CLine;
            myDotLine->Flags  = myDotLineData->Flags;
            myDotLine->Tail = myDotLineData->Tail;
            myDotLine->BeginPoint.x = myDotLineData->Line.Beg_X;
            myDotLine->BeginPoint.y = myDotLineData->Line.Beg_Y;
            myDotLine->EndPoint.x = myDotLineData->Line.End_X;
            myDotLine->EndPoint.y = myDotLineData->Line.End_Y;
            myDotLine->ProcessingType = myDotLineData->ProcessingType;
            myDotLine->Width = myDotLineData->Line.Wid10/10;
            myDotLine->LineEventsLength = myDotLineData->LineEventsLength;
            myDotLine->Degree = myDotLineData->Degree;
            myDotLine->RelationshipIndex = myDotLineData->RelationshipIndex;
            myDotLine->FlagCalculatedWidth = myDotLineData->FlagCalculatedWidth;
            myDotLine->FlagExtensible = myDotLineData->FlagExtensible;
            myDotLine->FlagDot = myDotLineData->FlagDot;
            myDotLine->FlagBad = myDotLineData->FlagBad;
            myDotLine->Line = myDotLineData->Line;
            myDotLine->Status = myDotLineData->Status;
            myDotLine->Type = myDotLineData->Type;
            myDotLine->Dir = myDotLineData->Dir;
            myDotLine->Qual = myDotLineData->Qual;
            myDotLine->Dens = myDotLineData->Dens;

            myDotLine->Specline = new NR_PoinLine;
            NR_PoinLine*  pSpecline = (NR_PoinLine*)myDotLine->Specline;
            pSpecline->Lef_0 = myDotLineData->Specline.point_line.data.Lef_0;
            pSpecline->Top_0 = myDotLineData->Specline.point_line.data.Top_0;
            pSpecline->Size = myDotLineData->Specline.point_line.data.Size;
            pSpecline->Step1000 = myDotLineData->Specline.point_line.data.Step1000;
            pSpecline->nRc = myDotLineData->Specline.point_line.data.nRc;

            myDotLine->poly.count = myDotLineData->poly.count;
            for(int cv=0; cv<CLINE_MaxVerticsNumber; cv++)
            {
                myDotLine->poly.Vertex[cv].x = myDotLineData->poly.Vertex[cv].x;
                myDotLine->poly.Vertex[cv].y = myDotLineData->poly.Vertex[cv].y;
            }

//-------------------------------------------------------------------------------
            CLINE_handle hmyDotLineEvent = CLINE_GetFirstEvent(hmyDotLine);
            myDotLine->m_arEvents.SetSize(0, myDotLineData->Specline.point_line.data.nRc);
            while (hmyDotLineEvent)
            {
                CPDEvent myDotLineEventData = CLINE_GetEventData(hmyDotLineEvent);
                myDotLineEvent = myDotLine->GetNewEvent();
                myDotLineEvent->Hori = myDotLineEventData->Hori;
                myDotLineEvent->Increase = myDotLineEventData->Increase;
                myDotLineEvent->Lev_0 = myDotLineEventData->Lev_0;
                myDotLineEvent->Width = myDotLineEventData->Width;
                myDotLineEvent->EventLength = myDotLineEventData->EventLength;

                CLINE_handle hmyDotLineEventInterval = CLINE_GetFirstEventInv(hmyDotLineEvent);
                myDotLineEvent->m_arIntervals.SetSize(0, myDotLineEventData->Width);
                while(hmyDotLineEventInterval)
                {
                    CPDInterval myDotLineEventIntervalData = CLINE_GetEventInvData(hmyDotLineEventInterval);
                    myDotLineEventInterval = myDotLineEvent->GetNewInterval();
                    myDotLineEventInterval->Pos = myDotLineEventIntervalData->Pos;
                    myDotLineEventInterval->Lent = myDotLineEventIntervalData->Lent;

                    hmyDotLineEventInterval = CLINE_GetNextEventInv(hmyDotLineEventInterval);
                }

                hmyDotLineEvent = CLINE_GetNextEvent(hmyDotLineEvent);
            }
//---------------------------------------------------------------------------------------------------

            PLines.m_arLines.Add(myDotLine);
            hmyDotLine = CLINE_GetNextLine(hmyDotLine);
        }
*/
    }
/*****************  Добавляем короткие линии  II проход ******************************/
    currentline = CLINE_GetFirstLine(/*linecontainer*/hCLINE);
    i = 0;

	while (currentline)
	{
        clinedata = CLINE_GetLineData(currentline);

		if (!clinedata)
        {
            currentline = CLINE_GetNextLine(currentline);
            continue;
        }

		if (clinedata->Flags&LI_Pointed)
        {
            currentline = CLINE_GetNextLine(currentline);
            continue;
        }

        i++;
        if (i>CountLines) break;

//        clinedata = CLINE_GetLineData(currentline);
/*        int CountEvents = CLINE_GetEventCount(currentline);
        if (CountEvents>0)
        {
            currentline = CLINE_GetNextLine(currentline);
            continue;
        }
*/

		if (!linesmass[i-1])
        {
            currentline = CLINE_GetNextLine(currentline);
            continue;
        }

        if((((clinedata->Line.End_X-clinedata->Line.Beg_X) < 50) && clinedata->Dir==LD_Horiz) || ((abs(clinedata->Line.End_Y-clinedata->Line.Beg_Y)<100) && (clinedata->Dir==LD_Verti)))
        {
            DLine* pCLine = new DLine;
            memcpy(pCLine, clinedata, sizeof(DLine));
/*
            pCLine->Line.Beg_X = pLns->A.x;
            pCLine->Line.Beg_Y = pLns->A.y;
            pCLine->Line.End_X = pLns->B.x;
            pCLine->Line.End_Y = pLns->B.y;
            pCLine->Line.Wid10 = 10 * pLns->Thickness;*/
            pCLine->Type = NR_DT_Unknown;
            pCLine->Dir  = LD_Unknown;
//            pCLine->Dens = (pLns->Quality*100)/255;
            pCLine->Status = ST_Basil_Short;
	        //Almi 06.04.01
	        pCLine->Flags = 0;
	        ////////////
	        //Almi 09.04.01
	        if (abs(pCLine->Line.End_X - pCLine->Line.Beg_X) > 2*abs(pCLine->Line.End_Y - pCLine->Line.Beg_Y))
		        pCLine->Dir  = LD_Horiz;
        	else if (abs(pCLine->Line.End_Y - pCLine->Line.Beg_Y) > 2*abs(pCLine->Line.End_X - pCLine->Line.Beg_X))
		        pCLine->Dir  = LD_Verti;
	        ////////////
			SetLineDegree(pCLine);
            CLINE_SetLineData(currentline, pCLine);
            linesmass[i-1] = NULL;
            delete pCLine;
        }
        currentline = CLINE_GetNextLine(currentline);
    }
//already added
/*****************  вертикальные линии    II проход ******************************/
/*
    n = pLti->Ver.Cnt;
  pLns = pLti->Ver.Lns;
  for (i=0; i<n; i++)
  {
   if((pLns->B.y-pLns->A.y) < 100)//Almi
   {
    pCLine = new CLine;

    pCLine->Line.Beg_X = pLns->A.x;
    pCLine->Line.Beg_Y = pLns->A.y;
    pCLine->Line.End_X = pLns->B.x;
    pCLine->Line.End_Y = pLns->B.y;
    pCLine->Line.Wid10 = 10 * pLns->Thickness;
    pCLine->Type = NR_DT_Unknown;
    pCLine->Dir  = LD_Unknown;
    pCLine->Dens = (pLns->Quality*100)/255;
    pCLine->Status = ST_Basil_Short;
	//Almi 06.04.01
	pCLine->Flags = 0;
	////////////
	//Almi 09.04.01
	if (abs(pCLine->Line.End_Y - pCLine->Line.Beg_Y) >
		2*abs(pCLine->Line.End_X - pCLine->Line.Beg_X))
		    pCLine->Dir  = LD_Verti;
	////////////
    PLines.m_arLines.Add( pCLine );
  }
   pLns++;
  }
*/

//delete unknown lines
  for(i = 0; i < CountLines; i++)
      if (linesmass[i])
	  {
		  CLINE_DelLine(/*linecontainer*/hCLINE, linesmass[i]);
		  linesmass[i] = NULL;
	  }

  //финальный проход по всем линиям
  const uint32_t My_False=~LI_IsTrue;
  int32_t CountShortLines = 0;
  uchar debug_flags = 0;
  int32_t cross_point[MAX_CROSS_POINTS];
  DCutPoint cut_point_obj;
  CLINE_handle hCutPoint;

  if (!LDPUMA_Skip(hDebugShortLinesPrint)) debug_flags |= 1;

  for(currentline = CLINE_GetFirstLine(/*linecontainer*/hCLINE); currentline; currentline = CLINE_GetNextLine(currentline))
  {
	  clinedata = CLINE_GetLineData(currentline);

	  if (!clinedata) continue;

	  if (clinedata->Flags&LI_Pointed) continue;

	  if(clinedata->Dir!=LD_Unknown && clinedata->Type!=NR_DT_LinePointed)
	  {
          DLine* pCLine = new DLine;
          memcpy(pCLine, clinedata, sizeof(DLine));
		  if(clinedata->Dir==LD_Horiz)
		  {
			  if(clinedata->Line.End_X-clinedata->Line.Beg_X<MinHorLenForTrue)
			  {
				if (RSL_VerifyShortLine(clinedata, hCCOM, &info, lang, debug_flags, cross_point))
				{
					pCLine->FlagBad = TRUE;
					pCLine->Flags = LI_IsFalse;
					pCLine->Status = ST_Bad;
				}
				else
				{
				  pCLine->Flags&=My_False;
				  pCLine->Status = ST_Basil_Short;

				  if (CLINE_GetCutPointCount(currentline)) CLINE_DelAllCutPoints(currentline);

				  for (int i = 0; i < MAX_CROSS_POINTS; i++)
					if (cross_point[i] > -1)
					{
						cut_point_obj.Level = cross_point[i];
						hCutPoint = CLINE_AddNewCutPoint(currentline);

						if (hCutPoint) CLINE_SetCutPointData(hCutPoint, &cut_point_obj);
					}
					else break;//cross points array is filled consequently
				}
			  }
		  }
		  else
		  {
			  if ((clinedata->Flags & LI_NOISE) && CountShortLines < CountLines)
			  {
				  linesmass[CountShortLines] = currentline;
				  CountShortLines++;
			  }

			  if(abs(clinedata->Line.Beg_Y-clinedata->Line.End_Y)<MinVerLenForTrue)
			  {
				if (RSL_VerifyShortLine(clinedata, hCCOM, &info, lang, debug_flags, cross_point))
				{
					pCLine->FlagBad = TRUE;
					pCLine->Flags = LI_IsFalse;
					pCLine->Status = ST_Bad;
				}
				else
				{
				  pCLine->Flags&=My_False;
				  pCLine->Status = ST_Basil_Short;

				  if (CLINE_GetCutPointCount(currentline)) CLINE_DelAllCutPoints(currentline);

				  for (int i = 0; i < MAX_CROSS_POINTS; i++)
					if (cross_point[i] > -1)
					{
						cut_point_obj.Level = cross_point[i];
						hCutPoint = CLINE_AddNewCutPoint(currentline);

						if (hCutPoint) CLINE_SetCutPointData(hCutPoint, &cut_point_obj);
					}
					else break;
				}
			  }
		  }

          CLINE_SetLineData(currentline, pCLine);
          delete pCLine;
	  }
  }

//delete short tails of vertical lines
  if (LDPUMA_Skip(hSkipDelSmallLines))
  {
	  for (int32_t it = 0; it < CountShortLines; it++)
		  CLINE_DelLine(/*linecontainer*/hCLINE, linesmass[it]);
  }

  if (!LDPUMA_Skip(hRLINE_CorrectDrawLines)) DrowAllLines(/*linecontainer*/hCLINE, hRLINE_CorrectDrawLines);
  if (!LDPUMA_Skip(hRLINE_CorrectDrawFrags)) DrawFragsForAllLines(hCLINE, hRLINE_CorrectDrawFrags);
// LDPUMA_ConsoleN("Функционирование %d",clock()-time);
 time=clock();
 LDPUMA_Skip(PutContainer2);

/*------------------------------------------------------------------------------*/
// Кладём новые линии

 i=CLINE_GetLineCount(/*linecontainer*/hCLINE);
 if(i<=0)
	 *pgneed_clean_line=FALSE;
 else

 {
	 *pgneed_clean_line=TRUE;
//	 if(!LDPUMA_Skip(hUseCLine))
//	     MyPutLines(linecontainer,hCLINE,TRUE);

 }

 if (sdl)
     DeleteBadDotLine(hCLINE,(CCOM_handle)hCCOM,hCPage);

// LDPUMA_ConsoleN("Складка в контейнер %d",clock()-time);
 LDPUMA_Skip(Epilog2);

// DelLineMas(pLti->Hor.Lns);
// DelLineMas(pLti->Ver.Lns);

 delete[] linesmass;
// CLINE_DeleteContainer(linecontainer);

 LDPUMA_Skip(Exit2);

 if (!LDPUMA_Skip(hWriteLineInFile))
	 PrintLines(hCLINE, "lines.res");

 return TRUE;
}


RLINE_FUNC(Bool32) RLINE_LinesPass3(Handle hCPAGE,CLINE_handle hCLINE, Handle hCCOM, uchar lang)
{
 return TRUE;
}

/***********************************************************************************************************/
CLINE_handle FindLine(CLINE_handle hContainer, CLINE_handle processedline,Bool vert)
{
 DLine* pCLine = new DLine;
// DLine* pCLineCopy = new DLine;
 CPDLine linedata = CLINE_GetLineData(processedline);

     memcpy(pCLine, linedata, sizeof(DLine));
//     memcpy(pCLineCopy, linedata, sizeof(DLine));
/*
     pCLine->Line.Beg_X = pLns->A.x;
	 pCLine->Line.Beg_Y = pLns->A.y;
	 pCLine->Line.End_X = pLns->B.x;
	 pCLine->Line.End_Y = pLns->B.y;
	 pCLine->Line.Wid10 = 10 * pLns->Thickness;*/
     pCLine->Type = NR_DT_Unknown;
     pCLine->Dir  = LD_Unknown;
/*     pCLine->Dens = (pLns->Quality*100)/255;
     pCLine->Flags=pLns->Flags;
     pCLine->PathNumber = SecondPath;
*/
     CLINE_SetLineData(processedline, pCLine);
/*
//Внимание!!!
//Для стабильной работы проекта STEPA_AM
//временно вместо "pCLine->Dir  = LD_Unknown;"
// внёс:
	 if(vert)
		 pCLine->Dir=LD_Verti;
	 else
	     pCLine->Dir=LD_Horiz;
*/

	 Bool32 IfNeedFrag = LDPUMA_Skip(hRLINE_CorrectDebugFrag) || !CLINE_GetEventCount(processedline);

	 if(FindLineFrag (processedline, TRUE, TRUE, hContainer, IfNeedFrag)==FALSE)
     {
//      CLINE_DelLine(hContainer, processedline);
      delete pCLine;
//      delete pCLineCopy;
	  return NULL;
     }
     else memcpy(pCLine, CLINE_GetLineData(processedline), sizeof(DLine));

/*
     if(pCLine->FlagDot)
         pPLines->FlagDot = TRUE;
*/
	 if(FindLineAttr (processedline, pCLine, FALSE)==FALSE)
     {
//      CLINE_DelLine(hContainer, processedline);
      delete pCLine;
//      delete pCLineCopy;
	  return NULL;
     }
     else CLINE_SetLineData(processedline, pCLine);

     if(pCLine->FlagBad == TRUE)
	 {
//       CLINE_DelLine(hContainer, processedline);
       delete pCLine;
//       delete pCLineCopy;
	   return NULL;
	 }
     else
     {//take'em away!
       Bool32 IsSeparationPoints = CheckSeparationPoints(processedline);
       if(IsSeparationPoints && pCLine->Tail==0)
       {/*
        for(int i=0; i<=CountSeparationPoints; i++)
        {
             pCLineNextPart = new CLine;

     	     pCLineNextPart->Line.Wid10 = 10 * pLns->Thickness;
             pCLineNextPart->Type = NR_DT_Unknown;
             pCLineNextPart->Dir  = LD_Unknown;
             pCLineNextPart->Dens = (pLns->Quality*100)/255;
             pCLineNextPart->Flags=pLns->Flags;
             pCLineNextPart->PathNumber = SecondPath;  */

             if(GetNextPartOfLine(hContainer, processedline/*, pCLineCopy pPLines,pCLine, pCLineNextPart, i*/))
			 {
//             CLINE_DelLine(hContainer, processedline);
        //}
				delete pCLine;
//             delete pCLineCopy;
				return NULL;
			 }
			 else
			 {
				 delete pCLine;
				 return processedline;
			 }
       }
       else
	   {
           delete pCLine;
//           delete pCLineCopy;
		   return processedline;
	   }
     }

}

/*********************************************************************************************************/
Bool MyGetLines(/*CLINE_handle linecontainer, Handle hCPage, */CLINE_handle hCLINE, int32_t* CountLines)
{
//	Bool fl_cont;

// if(!LDPUMA_Skip(hUseCLine))
// {

	CLINE_handle hline=CLINE_GetFirstLine(hCLINE);
    CPDLine cpdata;

	while(hline)
	{
		cpdata=CLINE_GetLineData(hline);
		if (!cpdata) hline=CLINE_GetNextLine(hline);
		else if (cpdata->Flags&LI_Pointed) hline=CLINE_GetNextLine(hline);
        else
		{
//            CLINE_handle newline = CLINE_AddNewLine(linecontainer);
			(*CountLines)++;
            DLine* newlinedata = new DLine;
            memcpy(newlinedata, cpdata, sizeof(DLine));
            InitLine(newlinedata);
//            CLINE_SetLineData(newline, newlinedata);
            CLINE_SetLineData(hline, newlinedata);
    		hline=CLINE_GetNextLine(hline);
            delete newlinedata;
		}
	}
/* }
 else
 {
	uint32_t    HorType;
    uint32_t    VerType;
	Handle    pBlock;
	uint32_t	  size32;
	uint32_t size_lineinfo=sizeof(LineInfo);

    pBlock=NULL;
	pBlock = CPAGE_GetBlockFirst (hCPage, RLINE_BLOCK_TYPE );
	if ( pBlock == NULL)
		return TRUE;

	size32 = CPAGE_GetBlockData(hCPage, pBlock, RLINE_BLOCK_TYPE, pLti, sizeof(LinesTotalInfo));
	if (size32 != sizeof(LinesTotalInfo) )
		return TRUE;

	HorType = (uint32_t)pLti->Hor.Lns;
	VerType = (uint32_t)pLti->Ver.Lns;

	pLti->Hor.Lns=NULL;
	pLti->Ver.Lns=NULL;

/////////////////////////////////////////////////////////////////////////////////////////////
    len_hor_mas=pLti->Hor.Cnt;

	if(pLti->Hor.Cnt)
	{
     if(!InitLineMas(&(pLti->Hor.Lns),len_hor_mas))
		return FALSE;
	}

    pBlock=NULL;
	pBlock = CPAGE_GetBlockFirst(hCPage,HorType);

	for(int i=pLti->Hor.Cnt-1;i>=0;i--)
	{
		fl_cont=FALSE;
		size32 = CPAGE_GetBlockData(hCPage,pBlock,HorType,&(pLti->Hor.Lns[i]),size_lineinfo);
		if(size32!=size_lineinfo)
		{
			fl_cont=TRUE;
			pBlock = CPAGE_GetBlockNext(hCPage,pBlock,HorType);
		}
		if(fl_cont)
		  continue;
//		if(!(pLti->Hor.Lns[i].Flags&LI_IsTrue))
//		{
//			fl_cont=TRUE;
//            pBlock = CPAGE_GetBlockNext(hCPage,pBlock,HorType);
//		}
//		if(fl_cont)
//			continue;

		pBlock = CPAGE_GetBlockNext(hCPage,pBlock,HorType);
	}

/////////////////////////////////////////////////////////////////////////////////////////////

    len_ver_mas=pLti->Ver.Cnt;

	pBlock=NULL;
	pBlock = CPAGE_GetBlockFirst(hCPage,VerType);

	if(pLti->Ver.Cnt)
	{
     if(!InitLineMas(&(pLti->Ver.Lns),len_ver_mas))
	 {
		DelLineMas(pLti->Hor.Lns);
		return FALSE;
	 }
	}

	for(i=pLti->Ver.Cnt-1;i>=0;i--)
	{
		fl_cont=FALSE;
		size32 = CPAGE_GetBlockData(hCPage,pBlock,VerType,&(pLti->Ver.Lns[i]),size_lineinfo);
		if(size32!=size_lineinfo)
		{
			fl_cont=TRUE;
			pBlock = CPAGE_GetBlockNext(hCPage,pBlock,VerType);
		}
		if(fl_cont)
		  continue;
//		if(!(pLti->Ver.Lns[i].Flags&LI_IsTrue))
//		{
//			fl_cont=TRUE;
//            pBlock = CPAGE_GetBlockNext(hCPage,pBlock,HorType);
//		}
//		if(fl_cont)
//			continue;

		pBlock = CPAGE_GetBlockNext(hCPage,pBlock,VerType);
	}
 }*/
	return TRUE;
}


Bool MyPutLines(CLINE_handle hContainerOut,CLINE_handle hCLINE,Bool dotline)
{
 CPDLine line;
 CPDEvent event;
 CPDCutPoint cupoint;
 CPDComponent comp;
 CPDInterval inter;
 DLine data_line;
 DEvent data_event;
 DCutPoint data_cupoint;
 DComponent data_comp;
 DInterval data_inv;
 CLINE_handle hinv;
 int count_poly;
 Bool fl_good_event;
 struct BigEvents
 {
	 int left;
	 int right;
	 int top;
	 int bottom;
 };

 int DELTA_Y;
 int Beg_Y;
 int Beg_X;
 int DELTA_X;
 int Wide;
 int Y;
 std::ofstream outfile, textfile;
 char* buffer;
 int totalevents = 0, totaleventinvs = 0, totalcps = 0, totalcpinvs = 0, totalcomps = 0;
 Handle draw_window = NULL;

 if (!LDPUMA_Skip(hWriteLineInFile))
 {
     outfile.open("lines.res", std::ios::out|std::ios::binary|std::ios::app);
     char* name = LDPUMA_GetFileName(NULL);
     char szFileName[MAX_PATH];
     for(int ff=0; ff<MAX_PATH; ff++) szFileName[ff]=0;
     strcpy(szFileName, name);
     outfile.write(szFileName, MAX_PATH);
     buffer = new char[sizeof(int)];
     int sizes = CLINE_GetLineCount(hContainerOut);
     memcpy(buffer, &sizes, sizeof(int));
     outfile.write(buffer, sizeof(int));
     textfile.open("lines.txt", std::ios::out);
     textfile << "number of lines = " << sizes << "\n";
/*     sizes = sizeof(DEvent);
     memcpy(buffer, &sizes, sizeof(int));
     outfile.write(buffer, sizeof(int));
     sizes = sizeof(DInterval);
     memcpy(buffer, &sizes, sizeof(int));
     outfile.write(buffer, sizeof(int));
     sizes = sizeof(DCutPoint);
     memcpy(buffer, &sizes, sizeof(int));
     outfile.write(buffer, sizeof(int));
     sizes = sizeof(DComponent);
     memcpy(buffer, &sizes, sizeof(int));
     outfile.write(buffer, sizeof(int));
*/   delete[] buffer;
 }

// int time=clock();

//Прочисмтим контейнер

 CLINE_handle del_line = CLINE_GetFirstLine(hCLINE);
 while (del_line)
 {
     CPDLine del_line_data = CLINE_GetLineData(del_line);
     if (!(del_line_data->Flags&LI_Pointed))
     {
         CLINE_handle next_line = CLINE_GetNextLine(del_line);
         CLINE_DelLine(hCLINE, del_line);
         del_line = next_line;
     }
     else del_line = CLINE_GetNextLine(del_line);
 }

//________commented by Andrey______________
/*
 FILE *f;
  f=fopen("lines.res","a");
  if(f!=NULL)
  {
     char* name=LDPUMA_GetFileName (NULL);
     fprintf(f,"%s\n",name);
 	 fprintf(f, " %d dotted,", CLINE_GetLineCount(hCLINE));
  }
*/

// CLINE_DelAllLines(hCLINE);
// LDPUMA_ConsoleN("Прочистка конейнера %d",clock()-time);
// time=clock();
 //Обычные линии

 for(CLINE_handle hLine = CLINE_GetFirstLine(hContainerOut); hLine; hLine = CLINE_GetNextLine(hLine))
 {
    line = CLINE_GetLineData(hLine);
	if(!line)
		continue;

    //DEBUG
//    uint32_t hhh = (uint32_t)line;
//    DLine* ddl = (DLine*)hhh;

    if (!LDPUMA_Skip(hWriteLineInFile))
    {
        buffer = new char[sizeof(DLine)];
        memcpy(buffer, line, sizeof(DLine));
        outfile.write(buffer, sizeof(DLine));
        delete[] buffer;
        buffer = new char[sizeof(int)];
        int CountEvents = CLINE_GetEventCount(hLine);
        memcpy(buffer, &CountEvents, sizeof(int));
        outfile.write(buffer, sizeof(int));
        delete[] buffer;
        textfile << "####################################################################################### \n";
        textfile << "(" << line->Line.Beg_X << "," << line->Line.Beg_Y << "); (" << line->Line.End_X << "," << line->Line.End_Y <<") \n";
        textfile << "number of events = " << CountEvents << "\n";
        totalevents+=CountEvents;
    }

	data_line.Degree=line->Degree;
	data_line.Dens=line->Dens;
	data_line.Dir=line->Dir;
	data_line.FlagBad=line->FlagBad;
	data_line.FlagCalculatedWidth=line->FlagCalculatedWidth;
	data_line.FlagDot=line->FlagDot;
	data_line.FlagExtensible=line->FlagExtensible;
//	data_line.Flags=(line->Flags)|LI_IsTrue; //???? Almi 06.04.01
	data_line.Flags=line->Flags;
	data_line.Line.Beg_X=line->Line.Beg_X;
	data_line.Line.Beg_Y=line->Line.Beg_Y;
	data_line.Line.End_X=line->Line.End_X;
	data_line.Line.End_Y=line->Line.End_Y;
	data_line.Line.Wid10=line->Line.Wid10;
	data_line.LineEventsLength=line->LineEventsLength;
	count_poly=line->poly.count-1;
	if(count_poly>CLINE_MaxVerticsNumber-1)
		count_poly=CLINE_MaxVerticsNumber-1;
	data_line.poly.count=count_poly+1;
	Point16* to=(data_line.poly.Vertex);
	const Point16* from=(line->poly.Vertex);
	for(;count_poly>=0;count_poly--)
	{
		to[count_poly].x=(int16_t)from[count_poly].x;
		to[count_poly].y=(int16_t)from[count_poly].y;
	}
    data_line.ProcessingType=line->ProcessingType;
	data_line.Qual=line->Qual;
	data_line.rect.bottom=line->rect.bottom;
	data_line.rect.top=line->rect.top;
	data_line.rect.left=line->rect.left;
	data_line.rect.right=line->rect.right;
	data_line.RelationshipIndex=reinterpret_cast<Handle> (-1);
//	if(line->Specline)
//	{
	 if(line->Type==NR_DT_LinePointed)
	 {

      NR_PoinLine* pl_to=&data_line.Specline.point_line.data;
	  const NR_PoinLine* pl_from=&line->Specline.point_line.data;
	  pl_to->Lef_0=pl_from->Lef_0;
	  pl_to->Top_0=pl_from->Top_0;
	  pl_to->Size=pl_from->Size;
	  pl_to->Step1000=pl_from->Step1000;
	  pl_to->nRc=pl_from->nRc;

	 }
	 else
	 {
		if(line->Type==NR_DT_LineDefis)
		{
		 NR_DefiLine* dl_to=&data_line.Specline.defi_line.data;
	     const NR_DefiLine* dl_from=&line->Specline.defi_line.data;
		 dl_to->Rc_0.Lef=dl_from->Rc_0.Lef;
		 dl_to->Rc_0.Top=dl_from->Rc_0.Top;
		 dl_to->Rc_0.Bot=dl_from->Rc_0.Bot;
		 dl_to->Rc_0.Rig=dl_from->Rc_0.Rig;
		 dl_to->Rc_0.Skew10=dl_from->Rc_0.Skew10;
		 dl_to->Step1000=dl_from->Step1000;
		 dl_to->nRc=dl_from->nRc;
		}
	 }
//	}
	data_line.Status=line->Status;
	data_line.Tail=line->Tail;
	data_line.Type=line->Type;
	if(line->Type==NR_DT_LinePointed)
		data_line.Flags|=LI_IsTrue;

	CLINE_handle hline=CLINE_AddNewLine(hCLINE);
	if(!hline)
		continue;
	CLINE_SetLineData(hline,(CPDLine)(&data_line));

	if(dotline)
	{
		Beg_Y=line->Line.Beg_Y;
		DELTA_Y=line->Line.End_Y-Beg_Y + 1;
		Beg_X=line->Line.Beg_X;
		DELTA_X=line->Line.End_X-Beg_X + 1;
		Wide=(line->Line.Wid10)/20;
	}

	//Ивенты
	for(CLINE_handle hEvent = CLINE_GetFirstEvent(hLine); hEvent; hEvent = CLINE_GetNextEvent(hEvent))
	{
	  event=CLINE_GetEventData(hEvent);
	  if(!event)
		 continue;

      if (!LDPUMA_Skip(hWriteLineInFile))
      {
          buffer = new char[sizeof(DEvent)];
          memcpy(buffer, event, sizeof(DEvent));
          outfile.write(buffer, sizeof(DEvent));
          delete[] buffer;
      }

	  CLINE_handle hInv = CLINE_GetFirstEventInv(hEvent);
      fl_good_event=TRUE;
	  //Проверка: не лажовый ли ивент?
	  if(line->Type==NR_DT_LineSimple)
	  {
	  if(dotline)
	  {
	   if(event->EventLength<SHORT_EVENT)
	   {
        if(hInv)
		{
		  inter=CLINE_GetEventInvData(hInv);
		  if(inter)
		  {
		   if(line->Dir==LD_Horiz)
		   {
			   Y=(DELTA_Y*(inter->Pos-Beg_X))/DELTA_X+Beg_Y;
			   if((event->Lev_0>Y+Wide)||(event->Lev_0+event->Width-1<Y-Wide))
				   fl_good_event=FALSE;
		   }
		   else
		   {
			   Y=(DELTA_X*(inter->Pos-Beg_Y))/DELTA_Y+Beg_X;
			   if((event->Lev_0>Y+Wide)||(event->Lev_0+((event->Width-1)>>1)<Y-Wide))
				   fl_good_event=FALSE;
		   }
		  }
		}
	   }
	  }
	  }
	  if(!fl_good_event)
      {
          buffer = new char[sizeof(int)];
          int c = 0;
          memcpy(buffer, &c, sizeof(int));
          outfile.write(buffer, sizeof(int));
          delete[] buffer;
  		  continue;
      }

      if (!LDPUMA_Skip(hWriteLineInFile))
      {
          buffer = new char[sizeof(int)];
          int CountIntervals = CLINE_GetEventInvCount(hEvent);
          memcpy(buffer, &CountIntervals, sizeof(int));
          outfile.write(buffer, sizeof(int));
          delete[] buffer;
          totaleventinvs+=CountIntervals;
      }

	  data_event.EventLength=event->EventLength;
	  data_event.Hori=event->Hori;
	  data_event.Increase=event->Increase;
	  data_event.Lev_0=event->Lev_0;
	  data_event.Width=event->Width;


	  CLINE_handle hevent=CLINE_AddNewEvent(hline);
	  if(!hevent)
		  continue;

	  CLINE_SetEventData(hevent,(CPDEvent)(&data_event));

	  //Интервалы ивентов
	  for(hInv = CLINE_GetFirstEventInv(hEvent); hInv; hInv = CLINE_GetNextEventInv(hInv))
	  {
		inter=CLINE_GetEventInvData(hInv);
		if(!inter)
			continue;

        if (!LDPUMA_Skip(hWriteLineInFile))
        {
            buffer = new char[sizeof(DInterval)];
            memcpy(buffer, inter, sizeof(DInterval));
            outfile.write(buffer, sizeof(DInterval));
            delete[] buffer;
        }

		data_inv.Lent=inter->Lent;
		data_inv.Pos=inter->Pos;

        hinv=CLINE_AddNewEventInv(hevent);
		if(!hinv)
			continue;

		CLINE_SetEventInvData(hinv,(CPDInterval)(&data_inv));
	  }
	}

	//Точки сечений

    if (!LDPUMA_Skip(hWriteLineInFile))
    {
       buffer = new char[sizeof(int)];
       int CountCutPoints = CLINE_GetCutPointCount(hLine);
       memcpy(buffer, &CountCutPoints, sizeof(int));
       outfile.write(buffer, sizeof(int));
       delete[] buffer;
       textfile << "number of cut points = " << CountCutPoints << "\n";
       totalcps+=CountCutPoints;
    }

    for(CLINE_handle hCP = CLINE_GetFirstCutPoint(hLine); hCP; hCP = CLINE_GetNextCutPoint(hCP))
	{
	  cupoint=CLINE_GetCutPointData(hCP);
	  if(!cupoint)
		 continue;

      if (!LDPUMA_Skip(hWriteLineInFile))
      {
          buffer = new char[sizeof(DCutPoint)];
          memcpy(buffer, cupoint, sizeof(DCutPoint));
          outfile.write(buffer, sizeof(DCutPoint));
          delete[] buffer;
          buffer = new char[sizeof(int)];
          int CountIntervals = CLINE_GetCutPointInvCount(hCP);
          memcpy(buffer, &CountIntervals, sizeof(int));
          outfile.write(buffer, sizeof(int));
          delete[] buffer;
          totalcpinvs+=CountIntervals;
      }

	  data_cupoint.Direction=cupoint->Direction;
	  data_cupoint.Level=cupoint->Level;

	  CLINE_handle hcupoint=CLINE_AddNewCutPoint(hline);
	  if(!hcupoint)
		  continue;

	  CLINE_SetCutPointData(hcupoint,(CPDCutPoint)(&data_cupoint));

	  //Интервалы точек сечений
	  for(CLINE_handle hCPInv = CLINE_GetFirstCutPointInv(hCP); hCPInv; hCPInv = CLINE_GetNextCutPointInv(hCPInv))
	  {
		inter=CLINE_GetCutPointInvData(hCPInv);
		if(!inter)
			continue;

        if (!LDPUMA_Skip(hWriteLineInFile))
        {
            buffer = new char[sizeof(DInterval)];
            memcpy(buffer, inter, sizeof(DInterval));
            outfile.write(buffer, sizeof(DInterval));
            delete[] buffer;
        }

        data_inv.Lent=inter->Lent;
		data_inv.Pos=inter->Pos;

        hinv=CLINE_AddNewCutPointInv(hcupoint);
		if(!hinv)
			continue;

		CLINE_SetCutPointInvData(hinv,(CPDInterval)(&data_inv));
	  }
	}

	//Компоненты

    if (!LDPUMA_Skip(hWriteLineInFile))
    {
       buffer = new char[sizeof(int)];
       int CountComps = CLINE_GetCompCount(hLine);
       memcpy(buffer, &CountComps, sizeof(int));
       outfile.write(buffer, sizeof(int));
       delete[] buffer;
       textfile << "number of components = " << CountComps << "\n";
       totalcomps+=CountComps;
    }

    for(CLINE_handle hComp = CLINE_GetFirstComp(hLine); hComp; hComp = CLINE_GetNextComp(hComp))
	{
        comp=CLINE_GetCompData(hComp);
	  if(!comp)
		 continue;

      if (!LDPUMA_Skip(hWriteLineInFile))
      {
          buffer = new char[sizeof(DComponent)];
          memcpy(buffer, comp, sizeof(DComponent));
          outfile.write(buffer, sizeof(DComponent));
          delete[] buffer;
      }

      data_comp.BeginPoint=comp->BeginPoint;
	  data_comp.CrossPointFlag=comp->CrossPointFlag;
	  data_comp.EndPoint=comp->EndPoint;

	  CLINE_handle hcomp=CLINE_AddNewComp(hline);
	  if(!hcomp)
		  continue;

	  CLINE_SetCompData(hcomp,(CPDComponent)(&data_comp));
	}
 }
// LDPUMA_ConsoleN("Обычные линии %d",clock()-time);
// time=clock();
/*
  if(f!=NULL)
  {
    fprintf(f, " %d all \n", CLINE_GetLineCount(hCLINE));
    fclose(f);
  }
*/

 if (outfile) outfile.close();
 if (textfile)
 {
     textfile << "total number of events = " << totalevents << "\n";
     textfile << "total number of event intervals = " << totaleventinvs << "\n";
     textfile << "total number of cut points = " << totalcps << "\n";
     textfile << "total number of cut point intervals = " << totalcpinvs << "\n";
     textfile << "total number of components = " << totalcomps << "\n";
     textfile.close();
 }

// if (!LDPUMA_Skip(hRLINE_CorrectDrawFrags)) DrawFragsForAllLines(hContainerOut, hRLINE_CorrectDrawFrags);

 return TRUE;
}

/*
void DelLineMas(LineInfo* masp)
{
	delete[] masp;
}
*/
/*
Bool AddLenLineMas(LineInfo** ppRc,int& len,int add)
{
 LineInfo* dop;
 int i;
 int size=sizeof((*ppRc)[0]);
 uchar* bytein;
 uchar* byteout;
 if(!(InitLineMas(&dop,len)) )
	 return FALSE;
 bytein=(uchar*)(*ppRc);
 byteout=(uchar*)(dop);
 for(i=size*len;i>0;i--)
 {
    byteout=bytein;
	byteout++;
	bytein++;
 }
 DelLineMas(*ppRc);
 if(!(InitLineMas(ppRc,len+add)) )
 {
	 (*ppRc)=dop;
	 return FALSE;
 }
 byteout=(uchar*)(*ppRc);
 bytein=(uchar*)(dop);
 for(i=size*len;i>0;i--)
 {
    byteout=bytein;
	byteout++;
	bytein++;
 }
 len+=add;
 DelLineMas(dop);
 return TRUE;
}
*/
/*
Bool MyGetLines(CPageLines* PLines,CLINE_handle hCLINE)
{
 CLine* line;
 CEvent* event;
 CCutPoint* cupoint;
 CComponent* comp;
 CInterval* inter;
 CPDLine cpdline;
 CPDEvent cpdevent;
 CPDCutPoint cpdcupoint;
 CPDComponent cpdcomp;
 CPDInterval cpdinv;
 CLINE_handle hinv;
 CLINE_handle hevent;
 CLINE_handle hline;
 CLINE_handle hcupoint;
 CLINE_handle hcomp;
 int count_poly;
 int time=clock();

 //Обычные линии
 for(hline=CLINE_GetFirstLine(hCLINE);hline;hline=CLINE_GetNextLine(hline))
 {
	cpdline=CLINE_GetLineData(hline);
	if(!cpdline)
		continue;
	line=PLines->GetNewLine();
	if(!line)
		continue;

	line->Degree=cpdline->Degree;
	line->Dens=cpdline->Dens;
	line->Dir=cpdline->Dir;
	line->FlagBad=cpdline->FlagBad;
	line->FlagCalculatedWidth=cpdline->FlagCalculatedWidth;
	line->FlagDot=cpdline->FlagDot;
	line->FlagExtensible=cpdline->FlagExtensible;
	line->Flags=cpdline->Flags;
	line->BeginPoint.x=line->Line.Beg_X=cpdline->Line.Beg_X;
	line->BeginPoint.y=line->Line.Beg_Y=cpdline->Line.Beg_Y;
	line->EndPoint.x=line->Line.End_X=cpdline->Line.End_X;
	line->EndPoint.y=line->Line.End_Y=cpdline->Line.End_Y;
	line->Width=line->Line.Wid10=cpdline->Line.Wid10;
	line->LineEventsLength=cpdline->LineEventsLength;
	count_poly=cpdline->poly.count-1;
	if(count_poly>MaxNumber-1)
		count_poly=MaxNumber-1;
	line->poly.count=count_poly+1;
	Point32* to=(line->poly.Vertex);
	Point16* from=(Point16*)(cpdline->poly.Vertex);
	for(;count_poly>=0;count_poly--)
	{
		to[count_poly].x=from[count_poly].x;
		to[count_poly].y=from[count_poly].y;
	}
    line->ProcessingType=cpdline->ProcessingType;
	line->Qual=cpdline->Qual;
	line->rect.bottom=cpdline->rect.bottom;
	line->rect.top=cpdline->rect.top;
	line->rect.left=cpdline->rect.left;
	line->rect.right=cpdline->rect.right;
	line->RelationshipIndex=cpdline->RelationshipIndex;
    if(line->SetLineNormDataTypes(cpdline->Type))
	{
	 if(cpdline->Type==NR_DT_LinePointed)
	 {
	  NR_PoinLine* pl_to=(NR_PoinLine*)line->Specline;
	  const NR_PoinLine* pl_from=&(cpdline->Specline.point_line.data);
	  pl_to->Lef_0=pl_from->Lef_0;
	  pl_to->Top_0=pl_from->Top_0;
	  pl_to->Size=pl_from->Size;
	  pl_to->Step1000=pl_from->Step1000;
	  pl_to->nRc=pl_from->nRc;
	 }
	 else
	 {
		if(line->Type==NR_DT_LineDefis)
		{
		 NR_DefiLine* dl_to=(NR_DefiLine*)line->Specline;
	     const NR_DefiLine* dl_from=&(cpdline->Specline.defi_line.data);
		 dl_to->Rc_0.Lef=dl_from->Rc_0.Lef;
		 dl_to->Rc_0.Top=dl_from->Rc_0.Top;
		 dl_to->Rc_0.Bot=dl_from->Rc_0.Bot;
		 dl_to->Rc_0.Rig=dl_from->Rc_0.Rig;
		 dl_to->Rc_0.Skew10=dl_from->Rc_0.Skew10;
		 dl_to->Step1000=dl_from->Step1000;
		 dl_to->nRc=dl_from->nRc;
		}
	 }
	}
	line->Status=cpdline->Status;
	line->Tail=cpdline->Tail;

    //Ивенты
    for(hevent=CLINE_GetFirstEvent(hline);hevent;hevent=CLINE_GetNextEvent(hevent))
	{
	  event=line->GetNewEvent();
	  if(!event)
		 continue;
	  cpdevent=CLINE_GetEventData(hevent);
	  if(!cpdevent)
		  continue;

	  event->EventLength=cpdevent->EventLength;
	  event->Hori=cpdevent->Hori;
	  event->Increase=cpdevent->Increase;
	  event->Lev_0=cpdevent->Lev_0;
	  event->Width=cpdevent->Width;

	  //Интервалы ивентов
	  for(hinv=CLINE_GetFirstEventInv(hevent);hinv;hinv=CLINE_GetNextEventInv(hinv))
	  {
		inter=event->GetNewInterval();
		if(!inter)
			continue;
		cpdinv=CLINE_GetEventInvData(hinv);
		if(!cpdinv)
			continue;

		inter->Lent=cpdinv->Lent;
		inter->Pos=cpdinv->Pos;
	  }
	}
	//Точки сечений
	for(hcupoint=CLINE_GetFirstCutPoint(hline);hcupoint;hcupoint=CLINE_GetNextCutPoint(hcupoint))
	{
	  cupoint=line->GetNewCutPoint();
	  if(!cupoint)
		 continue;
	  cpdcupoint=CLINE_GetCutPointData(hcupoint);
	  if(!cpdcupoint)
		  continue;

	  cupoint->Direction=cpdcupoint->Direction;
	  cupoint->Level=cpdcupoint->Level;

	  //Интервалы точек сечений
	  for(hinv=CLINE_GetFirstCutPointInv(hcupoint);hinv;hinv=CLINE_GetNextCutPointInv(hinv))
	  {
		inter=cupoint->GetNewInterval();
		if(!inter)
			continue;
		cpdinv=CLINE_GetCutPointInvData(hinv);
		if(!cpdinv)
			continue;

		inter->Lent=cpdinv->Lent;
		inter->Pos=cpdinv->Pos;
	  }
	}

	//Компоненты
	for(hcomp=CLINE_GetFirstComp(hline);hcomp;hcomp=CLINE_GetNextComp(hcomp))
	{
	  comp=line->GetNewComponent();
	  if(!comp)
		 continue;
	  cpdcomp=CLINE_GetCompData(hcomp);
	  if(!cpdcomp)
		  continue;

      comp->BeginPoint=cpdcomp->BeginPoint;
	  comp->CrossPointFlag=cpdcomp->CrossPointFlag;
	  comp->EndPoint=cpdcomp->EndPoint;
	}
 }

// LDPUMA_ConsoleN("Обычные линии %d",clock()-time);
 time=clock();

 return TRUE;
}
*/
void DeleteBadDotLine(CLINE_handle hCLINE,CCOM_handle hCCOM,Handle hCPAGE)
{

 int count_comp=0;
 const int My_False=~((int)LI_IsTrue);
 const Bool fl_show=!LDPUMA_Skip(hDotKilledD);
 Bool WasKilled=FALSE;
 Point16 start;
 Point16 end;
 count_comp=CCOM_GetContainerVolume(hCCOM);

 if(count_comp>50000||count_comp==0)
	 return;

 PAGEINFO info={0};
 GetPageInfo(hCPAGE,&info);

 int i=10;
 i=i*(info.DPIY-1)/300;
 const int min_letter_h=i;

 int line_count=CLINE_GetLineCount(hCLINE);
 if(line_count>1000||line_count==0)
	 return;


 CPDLine cpdata;
 int level;
 int delta;
 int right;
 int left;
 Bool fl_break;
 CCOM_comp* comp=NULL;
 DLine data;
 const uint32_t size_line=sizeof(DLine);

 for(CLINE_handle hline=CLINE_GetFirstLine(hCLINE);hline;hline=CLINE_GetNextLine(hline))
 {
	 cpdata=CLINE_GetLineData(hline);
	 if(!cpdata)
		 continue;

	 if(cpdata->Type==NR_DT_LinePointed)
	 {
		 if(cpdata->Dir==LD_Horiz&&(cpdata->Flags&LI_IsTrue))
		 {
			 left=cpdata->Line.Beg_X;
			 right=cpdata->Line.End_X;
			 if(right-left<SHORT_DOTLINE)
			 {
			  level=((cpdata->Line.Beg_Y+cpdata->Line.End_Y)>>1);
			  delta=((abs(cpdata->Line.Beg_Y-cpdata->Line.End_Y))>>1)+5;
			  fl_break=FALSE;

              for(comp=CCOM_GetFirst(hCCOM,NULL);comp;comp=CCOM_GetNext(comp,NULL))
			  {
	            if(comp->h<=min_letter_h)
				   continue;

		        if(comp->left>left&&comp->left+comp->w<right)
				{
					if(abs(comp->upper-level)<delta||abs(comp->upper+comp->h-level)<delta)
					{
					  if(CLINE_CopyData(&data,cpdata,size_line))
					  {
						data.Flags&=My_False;
						CLINE_SetLineData(hline,&data);
						fl_break=TRUE;
						if(fl_show)
						{
							WasKilled=TRUE;
							start.x=(int16_t)left;
							end.x=(int16_t)right;
							start.y=(int16_t)cpdata->Line.Beg_Y;
							end.y=(int16_t)cpdata->Line.End_Y;
							LDPUMA_DrawLine(MainWindowD,&start,&end,0,RGB(255,0,0),(int16_t)(-10*cpdata->Line.Wid10),117);
						}
					  }
					}
				}
				if(fl_break)
					break;
			  }
			 }
		 }
	 }
 }

 if(fl_show&&WasKilled)
 {
	 LDPUMA_WaitUserInput(hDotKilledD,MainWindowD);
	 LDPUMA_DeleteLines(MainWindowD,117);
 }

}
/************************************************************************************************/
/*void DeletePLines(CPageLines* PLines)
{
    int i, j, k, count, intervals_count;
    CLine* line;
    CStripe* stripe;
    CEvent* event;
    CCutPoint* point;
    CComponent* component;
    CInterval* interval;

    int lines_count = PLines->GetCountLines();

    for (i=0; i<lines_count; i++)
    {
        line = PLines->GetLine(0);
        count = line->GetCountStripes();

        for (j=0; j<count; j++)
        {
            stripe = line->GetStripe(0);
            intervals_count = stripe->GetCountIntervals();

            for (k=0; k<intervals_count; k++)
            {
                interval = stripe->GetInterval(0);
                stripe->m_arIntervals.RemoveAt(0);
                delete interval;
            }

            line->m_arStripes.RemoveAt(0);
            delete stripe;
        }

        count = line->GetCountStripesVerticalLine();

        for (j=0; j<count; j++)
        {
            stripe = line->GetStripeVerticalLine(0);
            intervals_count = stripe->GetCountIntervals();

            for (k=0; k<intervals_count; k++)
            {
                interval = stripe->GetInterval(0);
                stripe->m_arIntervals.RemoveAt(0);
                delete interval;
            }

            line->m_arStripesVerticalLine.RemoveAt(0);
            delete stripe;
        }

        count = line->GetCountEvents();

        for (j=0; j<count; j++)
        {
            event = line->GetEvent(0);
            intervals_count = event->GetCountIntervals();

            for (k=0; k<intervals_count; k++)
            {
                interval = event->GetInterval(0);
                event->m_arIntervals.RemoveAt(0);
                delete interval;
            }

            line->m_arEvents.RemoveAt(0);
            delete event;
        }

        count = line->GetCountCutPoints();

        for (j=0; j<count; j++)
        {
            point = line->GetCutPoint(0);
            intervals_count = point->GetCountIntervals();

            for (k=0; k<intervals_count; k++)
            {
                interval = point->GetInterval(0);
                point->m_arIntervals.RemoveAt(0);
                delete interval;
            }

            line->m_arCutPoints.RemoveAt(0);
            delete point;
        }

        count = line->GetCountComponents();

        for (j=0; j<count; j++)
        {
            component = line->GetComponent(0);
            line->m_arComponents.RemoveAt(0);
            delete component;
        }

        PLines->m_arLines.RemoveAt(0);
        delete line;
    }
}
*/

/***********************************************************************************************/
void PrintLines(CLINE_handle hContainer, const char* FileName)
{
	std::ofstream outfile;
	outfile.open(FileName, std::ios::out|std::ios::binary|std::ios::app);
	if (outfile.fail()) return;

    char* buff = LDPUMA_GetFileName(NULL);
    char szFileName[MAX_PATH];
	memset(szFileName, 0, MAX_PATH);
    strcpy(szFileName, buff);
    outfile.write(szFileName, MAX_PATH);
	int count = CLINE_GetLineCount(hContainer);
	outfile.write((char*)&count, sizeof(int));

	for (CLINE_handle hLine = CLINE_GetFirstLine(hContainer); hLine; hLine = CLINE_GetNextLine(hLine))
	{
		CPDLine pLine = CLINE_GetLineData(hLine);

		if (pLine->Flags & LI_Pointed) continue;

		outfile.write((char*)pLine, sizeof(DLine));
		count = CLINE_GetEventCount(hLine);
		outfile.write((char*)&count, sizeof(int));

		for (CLINE_handle hEvent = CLINE_GetFirstEvent(hLine); hEvent; hEvent = CLINE_GetNextEvent(hEvent))
		{
			CPDEvent pEvent = CLINE_GetEventData(hEvent);
			outfile.write((char*)pEvent, sizeof(DEvent));
			count = CLINE_GetEventInvCount(hEvent);
			outfile.write((char*)&count, sizeof(int));

			for (CLINE_handle hInv = CLINE_GetFirstEventInv(hEvent); hInv; hInv = CLINE_GetNextEventInv(hInv))
			{
				CPDInterval pInv = CLINE_GetEventInvData(hInv);
				outfile.write((char*)pInv, sizeof(DInterval));
			}
		}

		count = CLINE_GetCutPointCount(hLine);
		outfile.write((char*)&count, sizeof(int));

		for (CLINE_handle hCutPoint = CLINE_GetFirstCutPoint(hLine); hCutPoint; hCutPoint = CLINE_GetNextCutPoint(hCutPoint))
		{
			CPDCutPoint pCutPoint = CLINE_GetCutPointData(hCutPoint);
			outfile.write((char*)pCutPoint, sizeof(DCutPoint));
			count = CLINE_GetCutPointInvCount(hCutPoint);
			outfile.write((char*)&count, sizeof(int));

			for (CLINE_handle hInv = CLINE_GetFirstCutPointInv(hCutPoint); hInv; hInv = CLINE_GetNextCutPointInv(hInv))
			{
				CPDInterval pInv = CLINE_GetCutPointInvData(hInv);
				outfile.write((char*)pInv, sizeof(DInterval));
			}
		}

		count = CLINE_GetCompCount(hLine);
		outfile.write((char*)&count, sizeof(int));

		for (CLINE_handle hComp = CLINE_GetFirstComp(hLine); hComp; hComp = CLINE_GetNextComp(hComp))
		{
			CPDComponent pComp = CLINE_GetCompData(hComp);
			outfile.write((char*)pComp, sizeof(DComponent));
		}
	}
}

/**********************************************************************************************/
static int CompareByY( const void *elem1, const void *elem2 )
{
//compare vertical lines by length
	CPDLine pLine1 = CLINE_GetLineData(*(CLINE_handle*)elem1);
	CPDLine pLine2 = CLINE_GetLineData(*(CLINE_handle*)elem2);
	int32_t length1 = pLine1->Line.End_Y - pLine1->Line.Beg_Y;
	int32_t length2 = pLine2->Line.End_Y - pLine2->Line.Beg_Y;

  return length2 - length1;
}

/**********************************************************************************************/
static int CompareByX( const void *elem1, const void *elem2 )
{
//compare horizontal lines by length
	CPDLine pLine1 = CLINE_GetLineData(*(CLINE_handle*)elem1);
	CPDLine pLine2 = CLINE_GetLineData(*(CLINE_handle*)elem2);
	int32_t length1 = pLine1->Line.End_X - pLine1->Line.Beg_X;
	int32_t length2 = pLine2->Line.End_X - pLine2->Line.Beg_X;

  return length2 - length1;
}

/**********************************************************************************************/
static int CompareByTop( const void *elem1, const void *elem2 )
{
//compare vertical lines by length
	CPDLine pLine1 = CLINE_GetLineData(*(CLINE_handle*)elem1);
	CPDLine pLine2 = CLINE_GetLineData(*(CLINE_handle*)elem2);

  return pLine1->Line.Beg_Y - pLine2->Line.Beg_Y;
}

/**********************************************************************************************/
static int CompareByLeft( const void *elem1, const void *elem2 )
{
//compare vertical lines by length
	CPDLine pLine1 = CLINE_GetLineData(*(CLINE_handle*)elem1);
	CPDLine pLine2 = CLINE_GetLineData(*(CLINE_handle*)elem2);

  return pLine1->Line.Beg_X - pLine2->Line.Beg_X;
}

/**********************************************************************************************/
static int CompareCompByLeft( const void *elem1, const void *elem2 )
{
//compare components by left side
	CCOM_comp* pcomp1 = (CCOM_comp*)elem1;
	CCOM_comp* pcomp2 = (CCOM_comp*)elem2;

  return pcomp1->left - pcomp2->left;
}

/**********************************************************************************************/
static int CompareCompByUpper( const void *elem1, const void *elem2 )
{
//compare components by upper side
	CCOM_comp* pcomp1 = (CCOM_comp*)elem1;
	CCOM_comp* pcomp2 = (CCOM_comp*)elem2;

  return pcomp1->upper - pcomp2->upper;
}

/***********************************************************************************************/
RLINE_FUNC(Bool32) RLINE_LinesPass2(Handle hCCOM,void* phCLINE, Handle hCPAGE)
{
	if (!LDPUMA_Skip(hLinesPass2)) return TRUE;

	if (!hCPAGE)
		return FALSE;

	PAGEINFO info = {0};

	if(!GetPageInfo(hCPAGE,&info))
		return FALSE;

	Bool32 ret = TRUE;
    CLINE_handle hCLINE=*((CLINE_handle*)phCLINE);

	if (!hCLINE)
		return TRUE;

	const int32_t max_loop = 5;
	int32_t i = 0;
	int32_t nLines = CLINE_GetLineCount(hCLINE);
	int32_t nOldLines1, nOldLines2, nOldLines3;

	do
	{
		i++;
		nOldLines1 = nLines;

		ret = CorrectDoubleLines(hCLINE);

		nOldLines2 = CLINE_GetLineCount(hCLINE);

		if (ret) ret = GlueLines(hCLINE, hCCOM);

		nOldLines3 = CLINE_GetLineCount(hCLINE);

		if (ret)
		{
			if (!LDPUMA_Skip(hShowCPLines))
				DrowAllLines(hCLINE, hShowCPLines);

			ret = findLostLines(hCLINE, &info);
		}

		nLines = CLINE_GetLineCount(hCLINE);

	} while (ret && (nOldLines1 != nLines || nOldLines2 != nLines || nOldLines3 != nLines) && i < max_loop);

	if (ret && !LDPUMA_Skip(hWriteLineInFile2)) PrintLines(hCLINE, "lines2.res");

	if (!LDPUMA_Skip(hRLINE_Pass2DrawLines)) DrowAllLines(hCLINE, hRLINE_Pass2DrawLines);
	if (!LDPUMA_Skip(hRLINE_Pass2DrawFrags)) DrawFragsForAllLines(hCLINE, hRLINE_Pass2DrawFrags);

	return ret;
}

/***********************************************************************************************/
Bool32 CorrectDoubleLines(CLINE_handle hContainer)
{
	if (!LDPUMA_Skip(hLinesPass2DebugSkipDouble)) return TRUE;

	CLINE_handle hLine1, hLine2;
	CPDLine cpLine1, cpLine2;
	uint32_t PageNumber = CPAGE_GetCurrentPage();
	Handle h_Page = CPAGE_GetHandlePage(PageNumber);
	PAGEINFO page_info = {0};

	if (!GetPageInfo(h_Page,&page_info)) return FALSE;

	const double coef_x = ((double)page_info.DPIX + 1)/300, coef_y = ((double)page_info.DPIY + 1)/300;
	const int32_t interval_x = (int32_t)(h1Interval*coef_x)/2;
	const int32_t interval_y = (int32_t)(h1Interval*coef_y)/2;

	for (hLine1 = CLINE_GetFirstLine(hContainer); hLine1; hLine1 = CLINE_GetNextLine(hLine1))
	{
		cpLine1 = CLINE_GetLineData(hLine1);

		if (!cpLine1) continue;
		if (!(cpLine1->Flags & LI_COMPLEX) || cpLine1->Flags & LI_Pointed || cpLine1->Flags == LI_IsFalse) continue;

		//нашлась похожая на двойную линия, на которую первый проход повесил соответствующий флаг
		int32_t lUp, lDown, lLeft, lRight;

		if (cpLine1->Dir == LD_Horiz)
		{
			lLeft = cpLine1->Line.Beg_X;
			lRight = cpLine1->Line.End_X;

			if (cpLine1->Line.Beg_Y < cpLine1->Line.End_Y)
			{
				lUp = cpLine1->Line.Beg_Y - interval_y;
				lDown = cpLine1->Line.End_Y + interval_y;
			}
			else
			{
				lUp = cpLine1->Line.End_Y - interval_y;
				lDown = cpLine1->Line.Beg_Y + interval_y;
			}
		}
		else
		{
			lUp = cpLine1->Line.Beg_Y;
			lDown = cpLine1->Line.End_Y;

			if (cpLine1->Line.Beg_X < cpLine1->Line.End_X)
			{
				lLeft = cpLine1->Line.Beg_X - interval_x;
				lRight = cpLine1->Line.End_X + interval_x;
			}
			else
			{
				lLeft = cpLine1->Line.End_X - interval_x;
				lRight = cpLine1->Line.Beg_X + interval_x;
			}
		}

		//ищем вторую линию из связки
		for (hLine2 = CLINE_GetFirstLine(hContainer); hLine2; hLine2 = CLINE_GetNextLine(hLine2))
		{
			if (hLine1 == hLine2) continue;

			cpLine2 = CLINE_GetLineData(hLine2);

			if (!cpLine2) continue;
			if (cpLine1->Dir != cpLine2->Dir || cpLine2->Flags & LI_Pointed || cpLine2->Flags == LI_IsFalse) continue;

			int32_t min_level, max_level;

			if (cpLine2->Line.Beg_X < cpLine2->Line.End_X)
			{
				min_level = cpLine2->Line.Beg_X;
				max_level = cpLine2->Line.End_X;
			}
			else
			{
				max_level = cpLine2->Line.Beg_X;
				min_level = cpLine2->Line.End_X;
			}

			//отсев по оси X
			if (min_level > lRight || max_level < lLeft) continue;

			if (cpLine2->Line.Beg_Y < cpLine2->Line.End_Y)
			{
				min_level = cpLine2->Line.Beg_Y;
				max_level = cpLine2->Line.End_Y;
			}
			else
			{
				max_level = cpLine2->Line.Beg_Y;
				min_level = cpLine2->Line.End_Y;
			}

			//отсев по оси Y
			if (min_level > lDown || max_level < lUp) continue;

			//вот и вторая линия из связки нашлась (а возможно, их больше)
			if (RSL_CorrectDoubleLines(hLine1, hLine2)) break;
		}

		if (!hLine2) RSL_SplitLine(hLine1, hContainer);
	}

	return TRUE;
}

/***********************************************************************************************/
Bool32 GlueLines(CLINE_handle hContainer, Handle hCCOM)
{
	if (!LDPUMA_Skip(hLinesPass2DebugSkipGlue)) return TRUE;

//	int32_t CountHor, CountVert;
	int32_t CountLines;
	CPDLine pLine;
//	CLINE_handle *hHorLines, *hVertLines;
	CLINE_handle *hLinesMass = NULL;
	CLINE_handle hExtContainer = CLINE_CreateContainer(FALSE);
	Bool32 NeedNextLoop = FALSE;
	CCOM_comp* pCompMass = NULL;
	int32_t CountComps, max_line_width = 0;

	// проход по гориз. линиям
	do
	{
	    CLINE_handle hLine;
		CountLines = 0;
		for (hLine = CLINE_GetFirstLine(hContainer); hLine; hLine = CLINE_GetNextLine(hLine))
		{
			pLine = CLINE_GetLineData(hLine);
			if (pLine)
			{
				if (pLine->Flags&LI_Pointed || pLine->Flags == LI_IsFalse) continue;

				if (pLine->Dir == LD_Horiz)
				{
					CountLines++;
					if (!NeedNextLoop && max_line_width < pLine->Line.Wid10)
						max_line_width = pLine->Line.Wid10;
				}
			}
		}

		if (!CountLines) break;

		if (!NeedNextLoop)
		{
			CountComps = getLineComps(&pCompMass, 0, max_line_width/10 + 2, hCCOM);
			if (CountComps) qsort(pCompMass, CountComps, sizeof(CCOM_comp), CompareCompByLeft);
		}

		hLinesMass = new CLINE_handle[CountLines];

		CountLines = 0;

		for (hLine = CLINE_GetFirstLine(hContainer); hLine; hLine = CLINE_GetNextLine(hLine))
		{
			pLine = CLINE_GetLineData(hLine);
			if (pLine)
			{
				if (pLine->Flags&LI_Pointed || pLine->Flags == LI_IsFalse) continue;

				if (pLine->Dir == LD_Horiz) hLinesMass[CountLines++] = hLine;
			}
		}

		qsort(hLinesMass, CountLines, sizeof(CLINE_handle), CompareByX);

		if (!FindExtLines(hLinesMass, CountLines, hExtContainer, TRUE, pCompMass, CountComps, hContainer))
		{
			delete[] hLinesMass;
			CLINE_DeleteContainer(hExtContainer);
			if (pCompMass) delete[] pCompMass;

			return FALSE;
		}

		NeedNextLoop = SetExtLines(hExtContainer, hContainer, hLinesMass, CountLines);

		if (NeedNextLoop)
		{
			delete[] hLinesMass;
			hLinesMass = NULL;
			CLINE_CleanContainer(hExtContainer);
		}
	} while(NeedNextLoop);

	if (hLinesMass)
	{
		CLINE_CleanContainer(hExtContainer);
		delete[] hLinesMass;
		hLinesMass = NULL;
	}

	NeedNextLoop = FALSE;
	max_line_width = 0;
	if (pCompMass)
	{
		delete[] pCompMass;
		pCompMass = NULL;
	}
	// проход по верт. линиям
	do
	{
	    CLINE_handle hLine;
		CountLines = 0;
		for (hLine = CLINE_GetFirstLine(hContainer); hLine; hLine = CLINE_GetNextLine(hLine))
		{
			pLine = CLINE_GetLineData(hLine);
			if (pLine)
			{
				if (pLine->Flags&LI_Pointed || pLine->Flags == LI_IsFalse) continue;

				if (pLine->Dir != LD_Horiz)
				{
					CountLines++;
					if (!NeedNextLoop && max_line_width < pLine->Line.Wid10)
						max_line_width = pLine->Line.Wid10;
				}
			}
		}

		if (!CountLines) break;

		if (!NeedNextLoop)
		{
			CountComps = getLineComps(&pCompMass, max_line_width/10 + 2, 0, hCCOM);
			if (CountComps) qsort(pCompMass, CountComps, sizeof(CCOM_comp), CompareCompByUpper);
		}

		hLinesMass = new CLINE_handle[CountLines];

		CountLines = 0;

		for (hLine = CLINE_GetFirstLine(hContainer); hLine; hLine = CLINE_GetNextLine(hLine))
		{
			pLine = CLINE_GetLineData(hLine);
			if (pLine)
			{
				if (pLine->Flags&LI_Pointed || pLine->Flags == LI_IsFalse) continue;

				if (pLine->Dir != LD_Horiz) hLinesMass[CountLines++] = hLine;
			}
		}

		qsort(hLinesMass, CountLines, sizeof(CLINE_handle), CompareByY);

		if (!FindExtLines(hLinesMass, CountLines, hExtContainer, FALSE, pCompMass, CountComps, hContainer))
		{
			delete[] hLinesMass;
			CLINE_DeleteContainer(hExtContainer);
			if (pCompMass) delete[] pCompMass;

			return FALSE;
		}

		NeedNextLoop = SetExtLines(hExtContainer, hContainer, hLinesMass, CountLines);

		if (NeedNextLoop)
		{
			delete[] hLinesMass;
			hLinesMass = NULL;
			CLINE_CleanContainer(hExtContainer);
		}
	} while(NeedNextLoop);
/*
	for (CLINE_handle hLine = CLINE_GetFirstLine(hContainer); hLine; hLine = CLINE_GetNextLine(hLine))
	{
		pLine = CLINE_GetLineData(hLine);
		if (pLine)
		{
			if (pLine->Flags&LI_Pointed) continue;

			if (pLine->Dir == LD_Horiz) CountHor++;
			else CountVert++;
		}
	}

	hHorLines = new CLINE_handle[CountHor];
	hVertLines = new CLINE_handle[CountVert];
	CountHor = CountVert = 0;

	for (hLine = CLINE_GetFirstLine(hContainer); hLine; hLine = CLINE_GetNextLine(hLine))
	{
		pLine = CLINE_GetLineData(hLine);
		if (pLine)
		{
			if (pLine->Flags&LI_Pointed) continue;

			if (pLine->Dir == LD_Horiz) hHorLines[CountHor++] = hLine;
			else hVertLines[CountVert++] = hLine;
		}
	}

	qsort(hHorLines, CountHor, sizeof(CLINE_handle), CompareByX);
	qsort(hVertLines, CountVert, sizeof(CLINE_handle), CompareByY);

	CLINE_handle hExtContainer = CLINE_CreateContainer(FALSE);

	if (!FindExtLines(hHorLines, CountHor, hExtContainer, TRUE))
	{
		delete[] hHorLines;
		delete[] hVertLines;
		return FALSE;
	}
//------------------------
	SetExtLines(hExtContainer, hContainer, hHorLines, CountHor);
	CLINE_CleanContainer(hExtContainer);

	if (!FindExtLines(hVertLines, CountVert, hExtContainer, FALSE))
	{
		delete[] hHorLines;
		delete[] hVertLines;
		return FALSE;
	}
//------------------------
	SetExtLines(hExtContainer, hContainer, hVertLines, CountVert);

	delete[] hHorLines;
	delete[] hVertLines;
*/
	if (hLinesMass) delete[] hLinesMass;

	if (pCompMass) delete[] pCompMass;

	CLINE_DeleteContainer(hExtContainer);

	return TRUE;
}

/**********************************************************************************************/
Bool32 FindExtLines(CLINE_handle* hLinesMass, int32_t CountLines, CLINE_handle hExtContainer, Bool32 IsHor,
					CCOM_comp *pCompMass, int32_t CountMass, CLINE_handle hContainer)
{
	CPDLine pLine, pLineExt;
	CLINE_handle hLineExt = NULL;
	int32_t LineBeg, LineEnd, LineBegS, LineEndS, LineLen, spread = 0, simple_shift = 0;
	uint32_t PageNumber = CPAGE_GetCurrentPage();
	Handle h_Page = CPAGE_GetHandlePage(PageNumber);
	PAGEINFO page_info = {0};
	int32_t dpi;

	if (!GetPageInfo(h_Page,&page_info)) return FALSE;

	dpi = IsHor ? page_info.DPIX : page_info.DPIY;

	const double coef = ((double)dpi + 1)/300;
	const int32_t min_interval = (int32_t)(hMinInterval*coef);
	const int32_t first_interval = (int32_t)(h1Interval*coef);
	const int32_t second_interval = (int32_t)(h2Interval*coef);
	const int32_t max_interval = (int32_t)(hMaxInterval*coef);
	const int32_t small_line = (int32_t)(hSmallLineLen*coef);
	const int32_t long_line = (int32_t)(hLongLineLen*coef);
	const double maxdegreediff = (double)hMaxDegreeDiff/100;
	const int32_t max_shift = (int32_t)(hMaxShift*coef);

	for (int32_t i = 0; i < CountLines; i++)
	{
		pLine = CLINE_GetLineData(hLinesMass[i]);
		if (!pLine) return FALSE;

		if (IsHor)
		{
			LineBeg = pLine->Line.Beg_X;
			LineEnd = pLine->Line.End_X;
			LineBegS = pLine->Line.Beg_Y;
			LineEndS = pLine->Line.End_Y;
		}
		else
		{
			LineBeg = pLine->Line.Beg_Y;
			LineEnd = pLine->Line.End_Y;
			LineBegS = pLine->Line.Beg_X;
			LineEndS= pLine->Line.End_X;
		}

		LineLen = LineEnd - LineBeg + 1;

		for (hLineExt = CLINE_GetFirstLine(hExtContainer); hLineExt; hLineExt = CLINE_GetNextLine(hLineExt))
		{//у линий из hExtContainer rect.left заменяет LineBeg, rect.rigth - LineEnd, rect.top - LineBegS, rect.bottom - LineEndS
			pLineExt = CLINE_GetLineData(hLineExt);
			if (!pLineExt) continue;

			//отсекаем по углу
			if (pLine->Degree - pLineExt->Degree > maxdegreediff ||
				pLineExt->Degree - pLine->Degree > maxdegreediff)
				continue;

			//отсекаем по уровню
			if (LineBeg >= pLineExt->rect.left && LineEnd <= pLineExt->rect.right) continue;

			if (LineBeg < pLineExt->rect.left)
			{
				spread = pLineExt->rect.left - LineEnd;
				simple_shift = abs(pLineExt->rect.top - LineEndS);
			}
			else
			{
				spread = LineBeg - pLineExt->rect.right;
				simple_shift = abs(LineBegS - pLineExt->rect.bottom);
			}

			if (spread > max_interval || spread < 0 && abs(spread) > min_interval
				|| simple_shift > max_interval)
				continue;

//считаем сдвиг между линиями = расстоянию между серединой pLine и прямой, задаваемой pLineExt
//Vertex[0] - вектор, задающий прямую, LineEventsLength - длина вектора в квадрате
			Point32 delta;

			delta.x = pLineExt->Line.Beg_X - ((pLine->Line.Beg_X + pLine->Line.End_X)>>1);
			delta.y = pLineExt->Line.Beg_Y - ((pLine->Line.Beg_Y + pLine->Line.End_Y)>>1);

			int32_t temp = delta.x*(int32_t)pLineExt->poly.Vertex[0].y - delta.y*(int32_t)pLineExt->poly.Vertex[0].x;
			int32_t squareshift = (int32_t)(temp*((double)temp/pLineExt->LineEventsLength));

			//отсекаем по сдвигу
			if (squareshift > max_shift) continue;

			//проверка критериев на склеивание
			if (spread <= 3 || spread <= min_interval && LineLen >= small_line ||
				spread <= first_interval && LineLen >= long_line)
			{
				if (!LDPUMA_Skip(hPrint1Cr))
				{
					LDPUMA_ConsoleN("(%d,%d)(%d,%d) присоединяется к (%d,%d)(%d,%d) согласно", pLine->Line.Beg_X, pLine->Line.Beg_Y, pLine->Line.End_X, pLine->Line.End_Y, pLineExt->Line.Beg_X, pLineExt->Line.Beg_Y, pLineExt->Line.End_X, pLineExt->Line.End_Y);
					LDPUMA_ConsoleN("1 критерий(dpi = %d): расст. - %d, длина - %d", dpi, spread, LineLen);
				}

				break;
			}

			if (spread <= first_interval && LineLen < long_line)
			{
				Rect32 CurrLine;

				CurrLine.left = LineBeg;
				CurrLine.top = LineBegS;
				CurrLine.right = LineEnd;
				CurrLine.bottom = LineEndS;

				if (CheckAllLines(&CurrLine, &pLineExt->rect, IsHor, page_info.Incline2048, hLinesMass, CountLines, min_interval, long_line))
				{
					if (!LDPUMA_Skip(hPrint2Cr))
					{
						LDPUMA_ConsoleN("(%d,%d)(%d,%d) присоединяется к (%d,%d)(%d,%d) согласно", pLine->Line.Beg_X, pLine->Line.Beg_Y, pLine->Line.End_X, pLine->Line.End_Y, pLineExt->Line.Beg_X, pLineExt->Line.Beg_Y, pLineExt->Line.End_X, pLineExt->Line.End_Y);
						LDPUMA_ConsoleN("2 критерий(dpi = %d): расст. - %d, длина - %d, гр. проверка полож.", dpi, spread, LineLen);
					}

					break;
				}
			}

			int32_t CountBlack = -1, CountBlackR = -1;

			if (spread <= second_interval)
			{
				CountBlack = countCompLen(pCompMass, CountMass, pLine, pLineExt, IsHor);
				//CountBlack = CountBlackRaster(pLine, pLineExt, IsHor, hRastDelta, hDrawCrRast);

				Bool32 IfMerge = 2*CountBlack >= spread ||
								 spread - CountBlack <= first_interval && LineLen >= long_line ||
								 spread - CountBlack <= min_interval && LineLen >= small_line;

				if (!IfMerge)
				{
						CountBlackR = CountBlackRaster(pLine, pLineExt, IsHor, hRastDelta, hDrawCrRast);
						IfMerge = spread - CountBlackR <= min_interval && LineLen >= small_line;
				}
/*				if (!IfMerge && spread - CountBlack + 10 <= first_interval && LineLen >= long_line ||
					spread - CountBlack + 10 <= min_interval && LineLen >= small_line)
				{
				}
*/
				if (IfMerge)
				{
					if (!LDPUMA_Skip(hPrint3Cr))
					{
						LDPUMA_ConsoleN("(%d,%d)(%d,%d) присоединяется к (%d,%d)(%d,%d) согласно", pLine->Line.Beg_X, pLine->Line.Beg_Y, pLine->Line.End_X, pLine->Line.End_Y, pLineExt->Line.Beg_X, pLineExt->Line.Beg_Y, pLineExt->Line.End_X, pLineExt->Line.End_Y);
						LDPUMA_ConsoleN("3 критерий(dpi = %d): расст. - %d, длина - %d, почерн.(по комп.) - %d, почерн.(по раст.) - %d", dpi, spread, LineLen, CountBlack, CountBlackR);
					}

					break;
				}
			}

			if (spread <= max_interval)
			{
				Rect32 CurrLine;

				CurrLine.left = LineBeg;
				CurrLine.top = LineBegS;
				CurrLine.right = LineEnd;
				CurrLine.bottom = LineEndS;

				if (CheckAllLines(&CurrLine, &pLineExt->rect, IsHor, page_info.Incline2048, hLinesMass, CountLines, min_interval, long_line))
				{
					if (CountBlack == -1)
						CountBlack = countCompLen(pCompMass, CountMass, pLine, pLineExt, IsHor);
						//CountBlack = CountBlackRaster(pLine, pLineExt, IsHor, hRastDelta, hDrawCrRast);

					if (5*CountBlack >= spread/* || spread - CountBlack <= second_interval*/)
					{
						if (!LDPUMA_Skip(hPrint4Cr))
						{
							LDPUMA_ConsoleN("(%d,%d)(%d,%d) присоединяется к (%d,%d)(%d,%d) согласно", pLine->Line.Beg_X, pLine->Line.Beg_Y, pLine->Line.End_X, pLine->Line.End_Y, pLineExt->Line.Beg_X, pLineExt->Line.Beg_Y, pLineExt->Line.End_X, pLineExt->Line.End_Y);
							LDPUMA_ConsoleN("4 критерий(dpi = %d): расст. - %d, длина - %d, почерн. - %d. гр. проверка полож.", dpi, spread, LineLen, CountBlack);
						}

						break;
					}
				}
			}
		}

		if (hLineExt) MergeLines(hLinesMass[i], hLineExt, hContainer);
		else if (LineLen >= long_line)
		{
			CLINE_handle hNewExt = CLINE_AddNewLine(hExtContainer);
			DLine pNewExt, pCurrLine;

			memcpy(&pCurrLine, pLine, sizeof(DLine));
			pCurrLine.RelationshipIndex = hNewExt;
			CLINE_SetLineData(hLinesMass[i], &pCurrLine);

			memcpy(&pNewExt.Line, &pLine->Line, sizeof(pNewExt.Line));
			pNewExt.Degree = pLine->Degree;
			pNewExt.n_event = 1;
			pNewExt.poly.Vertex[0].x = (int16_t)(pNewExt.Line.End_X - pNewExt.Line.Beg_X);
			pNewExt.poly.Vertex[0].y = (int16_t)(pNewExt.Line.End_Y - pNewExt.Line.Beg_Y);
			pNewExt.LineEventsLength = (int32_t)pNewExt.poly.Vertex[0].x*(int32_t)pNewExt.poly.Vertex[0].x + (int32_t)pNewExt.poly.Vertex[0].y*(int32_t)pNewExt.poly.Vertex[0].y;
			if (IsHor)
			{
				pNewExt.rect.left = pLine->Line.Beg_X;
				pNewExt.rect.top = pLine->Line.Beg_Y;
				pNewExt.rect.right = pLine->Line.End_X;
				pNewExt.rect.bottom = pLine->Line.End_Y;
			}
			else
			{
				pNewExt.rect.left = pLine->Line.Beg_Y;
				pNewExt.rect.top = pLine->Line.Beg_X;
				pNewExt.rect.right = pLine->Line.End_Y;
				pNewExt.rect.bottom = pLine->Line.End_X;
			}

			CLINE_SetLineData(hNewExt, &pNewExt);
		}
	}

	return TRUE;
}

//Vertext[0](1), count(1), n_event, rect, Line, degree(1), RelationshipIndex(~)
/************************************************************************************************/
void MergeLines(CLINE_handle hLine, CLINE_handle hLineExt, CLINE_handle hContainer)
{
	DLine pLine, pLineExt;
	DLine pLostLine;
	Bool32 get_lost_frags = FALSE, get_line_frags = FALSE;
	int size_DLine = sizeof(DLine);

	InitLine(&pLostLine);
	memset(&pLostLine.Line, 0, sizeof(pLostLine.Line));
	memcpy(&pLine, CLINE_GetLineData(hLine), size_DLine);
	memcpy(&pLineExt, CLINE_GetLineData(hLineExt), size_DLine);

	pLine.RelationshipIndex = hLineExt;
	CLINE_SetLineData(hLine, &pLine);

	pLineExt.n_event++;
	if (pLine.Dir == LD_Horiz)
	{
		if (pLine.Line.Beg_X < pLineExt.Line.Beg_X)
		{
			pLostLine.Line.Beg_X = pLine.Line.End_X + 1;
			pLostLine.Line.Beg_Y = pLine.Line.End_Y;
			pLostLine.Line.End_X = pLineExt.Line.Beg_X - 1;
			pLostLine.Line.End_Y = pLineExt.Line.Beg_Y;
			pLineExt.Line.Beg_X = pLine.Line.Beg_X;
			pLineExt.Line.Beg_Y = pLine.Line.Beg_Y;
			pLineExt.rect.left = pLineExt.Line.Beg_X;
			pLineExt.rect.top = pLineExt.Line.Beg_Y;
		}
		else if (pLine.Line.End_X > pLineExt.Line.End_X)
		{
			pLostLine.Line.Beg_X = pLineExt.Line.End_X + 1;
			pLostLine.Line.Beg_Y = pLineExt.Line.End_Y;
			pLostLine.Line.End_X = pLine.Line.Beg_X - 1;
			pLostLine.Line.End_Y = pLine.Line.Beg_Y;
			pLineExt.Line.End_X = pLine.Line.End_X;
			pLineExt.Line.End_Y = pLine.Line.End_Y;
			pLineExt.rect.right = pLineExt.Line.End_X;
			pLineExt.rect.bottom = pLineExt.Line.End_Y;
		}

		if (!CLINE_GetEventCount(hLine) && pLine.Line.End_X - pLine.Line.Beg_X > 6)
			get_line_frags = TRUE;

		if (pLostLine.Line.End_X - pLostLine.Line.Beg_X > 6) get_lost_frags = TRUE;
	}
	else
	{
		if (pLine.Line.Beg_Y < pLineExt.Line.Beg_Y)
		{
			pLostLine.Line.Beg_X = pLine.Line.End_X;
			pLostLine.Line.Beg_Y = pLine.Line.End_Y + 1;
			pLostLine.Line.End_X = pLineExt.Line.Beg_X;
			pLostLine.Line.End_Y = pLineExt.Line.Beg_Y - 1;
			pLineExt.Line.Beg_X = pLine.Line.Beg_X;
			pLineExt.Line.Beg_Y = pLine.Line.Beg_Y;
			pLineExt.rect.left = pLineExt.Line.Beg_Y;
			pLineExt.rect.top = pLineExt.Line.Beg_X;
		}
		else if (pLine.Line.End_Y > pLineExt.Line.End_Y)
		{
			pLostLine.Line.Beg_X = pLineExt.Line.End_X;
			pLostLine.Line.Beg_Y = pLineExt.Line.End_Y + 1;
			pLostLine.Line.End_X = pLine.Line.Beg_X;
			pLostLine.Line.End_Y = pLine.Line.Beg_Y - 1;
			pLineExt.Line.End_X = pLine.Line.End_X;
			pLineExt.Line.End_Y = pLine.Line.End_Y;
			pLineExt.rect.right = pLineExt.Line.End_Y;
			pLineExt.rect.bottom = pLineExt.Line.End_X;
		}

		if (!CLINE_GetEventCount(hLine) && pLine.Line.End_Y - pLine.Line.Beg_Y > 6)
			get_line_frags = TRUE;

		if (pLostLine.Line.End_Y - pLostLine.Line.Beg_Y > 6) get_lost_frags = TRUE;
	}

	if (get_line_frags) //порядок get_..._frags не менять! (т.к. потерянные фрагменты складываются в приклеиваемую линию)
	{
		DLine pLineCopy;

		memcpy(&pLineCopy, &pLine, sizeof(DLine));
		InitLine(&pLine);
		CLINE_SetLineData(hLine, &pLine);
		if (!FindLineFrag(hLine, FALSE, TRUE+1, NULL))
			CLINE_SetLineData(hLine, &pLineCopy);
		else if (!CLINE_GetEventCount(hLine))
			CLINE_SetLineData(hLine, &pLineCopy);
		else
		{
			memcpy(&pLine, CLINE_GetLineData(hLine), sizeof(DLine));
			pLine.RelationshipIndex = hLineExt;
			CLINE_SetLineData(hLine, &pLine);
		}
	}

	if (get_lost_frags)
	{
		CLINE_handle hLostLine = CLINE_AddNewLine(hContainer);

		CLINE_SetLineData(hLostLine, &pLostLine);
		if (FindLineFrag(hLostLine, FALSE, TRUE+1, NULL))
		{
			if (CLINE_GetEventCount(hLostLine))
			{
				for(CLINE_handle hEvent = CLINE_GetFirstEvent(hLostLine); hEvent; hEvent = CLINE_GetNextEvent(hEvent))
				{
					CLINE_handle hNewEvent = CLINE_AddNewEvent(hLine);
					DEvent pNewEvent;
					memcpy(&pNewEvent, CLINE_GetEventData(hEvent), sizeof(DEvent));
					CLINE_SetEventData(hNewEvent, &pNewEvent);

					for (CLINE_handle hInv = CLINE_GetFirstEventInv(hEvent); hInv; hInv = CLINE_GetNextEventInv(hInv))
					{
						CLINE_handle hNewInv = CLINE_AddNewEventInv(hNewEvent);
						DInterval pNewInv;
						memcpy(&pNewInv, CLINE_GetEventInvData(hInv), sizeof(DInterval));
						CLINE_SetEventInvData(hNewInv, &pNewInv);
					}
				}
			}
		}

		CLINE_DelLine(hContainer, hLostLine);
	}

	CLINE_SetLineData(hLineExt, &pLineExt);
}

/************************************************************************************************/
Bool32 CheckAllLines(Rect32* CurrLine, const Rect32* pLine, Bool32 IsHor, int32_t nIncline,
					 CLINE_handle* hLinesMass, int32_t CountLines, int32_t interval, int32_t long_line)
{
	Rect32 tempRect;
	memcpy(&tempRect, pLine, sizeof(Rect32));

	if (IsHor)
	{
		IDEAL_XY(CurrLine->left, CurrLine->top);
		IDEAL_XY(CurrLine->right, CurrLine->bottom);
		IDEAL_XY(tempRect.left, tempRect.top);
		IDEAL_XY(tempRect.right, tempRect.bottom);
	}
	else
	{
		IDEAL_XY(CurrLine->top, CurrLine->left);
		IDEAL_XY(CurrLine->bottom, CurrLine->right);
		IDEAL_XY(tempRect.top, tempRect.left);
		IDEAL_XY(tempRect.bottom, tempRect.right);
	}

	int32_t Level1, Level2, Level3, Level4;

	if (CurrLine->left < tempRect.left)
	{
		Level1 = CurrLine->left;
		Level2 = CurrLine->right;
		Level3 = tempRect.left;
		Level4 = tempRect.right;
	}
	else
	{
		Level3 = CurrLine->left;
		Level4 = CurrLine->right;
		Level1 = tempRect.left;
		Level2 = tempRect.right;
	}

	if (Level2 >= Level3) return TRUE;

	CPDLine pNowLine;
	int32_t CountAllLines = 0, CountVotedLines = 0, CountLines1_2 = 0, CountLines3_4 = 0;

	for (int32_t i = 0; i < CountLines; i++)
	{
		pNowLine = CLINE_GetLineData(hLinesMass[i]);
		if (!pNowLine) return FALSE;

		if (IsHor)
		{
			tempRect.left = pNowLine->Line.Beg_X;
			tempRect.top = pNowLine->Line.Beg_Y;
			tempRect.right = pNowLine->Line.End_X;
			tempRect.bottom = pNowLine->Line.End_Y;
			IDEAL_XY(tempRect.left, tempRect.top);
			IDEAL_XY(tempRect.right, tempRect.bottom);
		}
		else
		{
			tempRect.left = pNowLine->Line.Beg_Y;
			tempRect.top = pNowLine->Line.Beg_X;
			tempRect.right = pNowLine->Line.End_Y;
			tempRect.bottom = pNowLine->Line.End_X;
			IDEAL_XY(tempRect.top, tempRect.left);
			IDEAL_XY(tempRect.bottom, tempRect.right);
		}

		if (tempRect.right - tempRect.left + 1 < long_line) continue;
/*
		if (tempRect.left > Level1 - interval && tempRect.left < Level1 + interval)
		{
			CountAllLines++;
			if (tempRect.right > Level4 - interval && tempRect.right < Level4 + interval)
				CountVotedLines++;

			if (tempRect.right > Level2 - interval && tempRect.right < Level2 + interval)
				CountLines1_2++;
		}

		if (tempRect.left > Level3 - interval && tempRect.left < Level3 + interval &&
			tempRect.right > Level4 - interval && tempRect.right < Level4 + interval)
			CountLines3_4++;
*/
		if (tempRect.left < Level1 + interval && tempRect.right > Level4 - interval)
			CountVotedLines++;
		else if (tempRect.left > Level1 - interval && tempRect.right < Level2 + interval ||
				 tempRect.left > Level3 - interval && tempRect.right < Level4 + interval)
				 CountAllLines++;
	}
/*
	if (CountVotedLines && CountAllLines - CountVotedLines < 3 ||
		CountLines1_2 > 1 && CountLines3_4 > 1 && abs(CountLines1_2 - CountLines3_4) < 2)
		return TRUE;
*/
	if (CountVotedLines > 1 && CountVotedLines > CountAllLines) return TRUE;

	return FALSE;
}

/**********************************************************************************************/
int32_t getLineComps(CCOM_comp** pCompMass, int32_t max_width, int32_t max_height, Handle hCCOM)
{
	int32_t CountComps = 0;
	CCOM_comp *hcomp;
	const int size_comp = sizeof(CCOM_comp);

	for (hcomp = CCOM_GetFirst(hCCOM, NULL); hcomp; hcomp = CCOM_GetNext(hcomp, NULL))
		if (hcomp->w > 0 && hcomp->h > 0 && (!max_width || hcomp->w <= max_width) &&
			(!max_height || hcomp->h <= max_height))
			CountComps++;

	if (!CountComps) return 0;

	*pCompMass = new CCOM_comp[CountComps];
	CountComps = 0;
	for (hcomp = CCOM_GetFirst(hCCOM, NULL); hcomp; hcomp = CCOM_GetNext(hcomp, NULL))
		if (hcomp->w > 0 && hcomp->h > 0 && (!max_width || hcomp->w <= max_width) &&
			(!max_height || hcomp->h <= max_height))
			memcpy(&((*pCompMass)[CountComps++]), hcomp, size_comp);

	return CountComps;
}

/**********************************************************************************************/
int32_t countCompLen(CCOM_comp* pCompMass, int32_t CountComp, CPDLine pLine, CPDLine pLineExt,
				   Bool32 IsHor)
{
	Rect32 RastRect;

	//ищем растр
	if (IsHor)
	{
		if (pLine->Line.Beg_X <= pLineExt->Line.Beg_X)
		{
			RastRect.left = pLine->Line.End_X + 1;
			RastRect.right = pLineExt->Line.Beg_X - 1;
			if (pLine->Line.End_Y < pLineExt->Line.Beg_Y)
			{
				RastRect.top = pLine->Line.End_Y - (pLineExt->Line.Wid10 + 10)/20 - hRastDelta;
				RastRect.bottom = pLineExt->Line.Beg_Y + (pLineExt->Line.Wid10 + 10)/20 + hRastDelta;
			}
			else
			{
				RastRect.bottom = pLine->Line.End_Y + (pLineExt->Line.Wid10 + 10)/20 + hRastDelta;
				RastRect.top = pLineExt->Line.Beg_Y - (pLineExt->Line.Wid10 + 10)/20 - hRastDelta;
			}
		}
		else
		{
			RastRect.left = pLineExt->Line.End_X + 1;
			RastRect.right = pLine->Line.Beg_X - 1;
			if (pLineExt->Line.End_Y < pLine->Line.Beg_Y)
			{
				RastRect.top = pLineExt->Line.End_Y - (pLineExt->Line.Wid10 + 10)/20 - hRastDelta;
				RastRect.bottom = pLine->Line.Beg_Y + (pLineExt->Line.Wid10 + 10)/20 + hRastDelta;
			}
			else
			{
				RastRect.bottom = pLineExt->Line.End_Y + (pLineExt->Line.Wid10 + 10)/20 + hRastDelta;
				RastRect.top = pLine->Line.Beg_Y - (pLineExt->Line.Wid10 + 10)/20 - hRastDelta;
			}
		}
	}
	else
	{
		if (pLine->Line.Beg_Y <= pLineExt->Line.Beg_Y)
		{
			RastRect.top = pLine->Line.End_Y + 1;
			RastRect.bottom = pLineExt->Line.Beg_Y - 1;
			if (pLine->Line.End_X < pLineExt->Line.Beg_X)
			{
				RastRect.left = pLine->Line.End_X - (pLineExt->Line.Wid10 + 10)/20 - hRastDelta;
				RastRect.right = pLineExt->Line.Beg_X + (pLineExt->Line.Wid10 + 10)/20 + hRastDelta;
			}
			else
			{
				RastRect.right = pLine->Line.End_X + (pLineExt->Line.Wid10 + 10)/20 + hRastDelta;
				RastRect.left = pLineExt->Line.Beg_X - (pLineExt->Line.Wid10 + 10)/20 - hRastDelta;
			}
		}
		else
		{
			RastRect.top = pLineExt->Line.End_Y + 1;
			RastRect.bottom = pLine->Line.Beg_Y - 1;
			if (pLineExt->Line.End_X < pLine->Line.Beg_X)
			{
				RastRect.left = pLineExt->Line.End_X - (pLineExt->Line.Wid10 + 10)/20 - hRastDelta;
				RastRect.right = pLine->Line.Beg_X + (pLineExt->Line.Wid10 + 10)/20 + hRastDelta;
			}
			else
			{
				RastRect.right = pLineExt->Line.End_X + (pLineExt->Line.Wid10 + 10)/20 + hRastDelta;
				RastRect.left = pLine->Line.Beg_X - (pLineExt->Line.Wid10 + 10)/20 - hRastDelta;
			}
		}
	}

	if (RastRect.left >= RastRect.right || RastRect.top >= RastRect.bottom) return 0;

	int32_t ncomp = findFirstComp(pCompMass, CountComp, RastRect, IsHor);

	if (ncomp == -1) return 0;

	int32_t countLen = 0;
	int32_t max_width = pLineExt->Line.Wid10/10 + 2;

	for (int32_t i = ncomp; i < CountComp; i++)
		if (IsHor)
		{
			if (pCompMass[i].left > RastRect.right) break;

			if (pCompMass[i].left + pCompMass[i].w - 1 > RastRect.right + 7 ||
				pCompMass[i].left + pCompMass[i].w - 1 < RastRect.left ||
				pCompMass[i].upper < RastRect.top ||
				pCompMass[i].upper + pCompMass[i].h - 1 > RastRect.bottom ||
				pCompMass[i].h > max_width)
				continue;

			countLen += MIN(pCompMass[i].left + pCompMass[i].w - 1, RastRect.right) - MAX(pCompMass[i].left, RastRect.left) + 1;
		}
		else
		{
			if (pCompMass[i].upper > RastRect.bottom) break;

			if (pCompMass[i].upper + pCompMass[i].h - 1 > RastRect.bottom + 7 ||
				pCompMass[i].upper + pCompMass[i].h - 1 < RastRect.top ||
				pCompMass[i].left < RastRect.left ||
				pCompMass[i].left + pCompMass[i].w - 1 > RastRect.right ||
				pCompMass[i].w > max_width)
				continue;

			countLen += MIN(pCompMass[i].upper + pCompMass[i].h - 1, RastRect.bottom) - MAX(pCompMass[i].upper, RastRect.top) + 1;
		}

	return countLen;
}

/*********************************************************************************************/
int32_t findFirstComp(CCOM_comp* pCompMass, int32_t CountComp, Rect32 RastRect, Bool32 IsHor)
{
	if (CountComp < 1) return -1;

	if (IsHor && (pCompMass[0].left > RastRect.right || pCompMass[CountComp - 1].left < RastRect.left - 7) ||
		!IsHor && (pCompMass[0].upper > RastRect.bottom || pCompMass[CountComp - 1].upper < RastRect.top - 7))
		return -1;

	int32_t old_min = 0, old_max = CountComp - 1, curr = CountComp - 1;

	if (IsHor && pCompMass[0].left >= RastRect.left - 7 && pCompMass[0].left <= RastRect.right ||
		!IsHor && pCompMass[0].upper >= RastRect.top - 7 && pCompMass[0].upper <= RastRect.bottom)
		return 0;

	if (!(IsHor && pCompMass[old_max].left >= RastRect.left - 7 && pCompMass[old_max].left <= RastRect.right ||
		!IsHor && pCompMass[old_max].upper >= RastRect.top - 7 && pCompMass[old_max].upper <= RastRect.bottom))
	{
		curr = old_max>>1;
		while(1)
		{
			if (IsHor && pCompMass[curr].left < RastRect.left - 7 ||
				!IsHor && pCompMass[curr].upper < RastRect.top - 7)
			{
				old_min = curr;
				curr = (old_min + old_max)>>1;
				if (old_min == curr) return -1;

				continue;
			}

			if (IsHor && pCompMass[curr].left > RastRect.right ||
				!IsHor && pCompMass[curr].upper > RastRect.bottom)
			{
				old_max = curr;
				curr = (old_min + old_max)>>1;
				if (old_max == curr) return -1;

				continue;
			}

			break;
		}
	}

	int32_t i;
	for (i = curr; i > 0; i--)
		if ((IsHor && pCompMass[i].left >= RastRect.left - 7 ||
			!IsHor && pCompMass[i].upper >= RastRect.top - 7) &&
			(IsHor && pCompMass[i-1].left < RastRect.left - 7 ||
			!IsHor && pCompMass[i-1].upper < RastRect.top - 7))
			break;

	return i;
}

/***********************************************************************************************/
void SetLineDegree(DLine* line)
{
double Lx,Ly;

 Ly = line->Line.End_Y - line->Line.Beg_Y+1;
 Lx = line->Line.End_X - line->Line.Beg_X+1;

 line->Degree = atan2(Ly,Lx);
}
/***********************************************************************************************/
void getLineIdealStrictRectangular(const NR_SimpLine *pdLine, Rect32* pRect, bool is_horiz, int32_t nIncline, int32_t spread)
{
	if (!pdLine || !pRect)
		return;

	Point32 lBeg, lEnd;

	lBeg.x = pdLine->Beg_X;
	lBeg.y = pdLine->Beg_Y;
	lEnd.x = pdLine->End_X;
	lEnd.y = pdLine->End_Y;

	IDEAL_XY(lBeg.x, lBeg.y);
	IDEAL_XY(lEnd.x, lEnd.y);

	if (is_horiz)
	{
		pRect->left = lBeg.x;
		pRect->right = lEnd.x;

		if (lBeg.y < lEnd.y)
		{
			pRect->top = lBeg.y;
			pRect->bottom = lEnd.y;
		}
		else
		{
			pRect->top = lEnd.y;
			pRect->bottom = lBeg.y;
		}
	}
	else
	{
		pRect->top = lBeg.y;
		pRect->bottom = lEnd.y;

		if (lBeg.x < lEnd.x)
		{
			pRect->left = lBeg.x;
			pRect->right = lEnd.x;
		}
		else
		{
			pRect->left = lEnd.x;
			pRect->right = lBeg.x;
		}
	}

	if (spread > 0)
	{
		pRect->left -= spread;
		pRect->top -= spread;
		pRect->right += spread;
		pRect->bottom += spread;
	}
}
/***********************************************************************************************/
/*typedef struct tagCompressHeader
{
	Bool16 bCompressed;
	uchar cRepeater;
	uchar reserved;
	uint32_t wCount;
} CompressHeader;

Bool32 Compress(char * lpData, uint32_t Size, char ** compressedData, uint32_t * compressedSize)
{
// Заменяем группу из не менее MIN_REPEAT одинаковых символов на счетчик повторений
#define MIN_REPEAT 2*sizeof(CompressHeader)

	if (Size==0)
		return FALSE;

   char *newData = new char[Size+sizeof(CompressHeader)]; //размер станет таким, если уплотнить не получилось,
   if (!newData)										  //иначе - не больше исходного
	   return FALSE;
   char *lpNewData = newData;

	// Находим пару - обычный фрагмент и фрагмент, заполненный одинаковыми символами;
	// затем оба отписываем
	char * ordinary=lpData,	  //обычный фрагмент
		 * end=ordinary+Size;
	do
	{
		uint32_t count=1;
		char * current=ordinary+1,
			 * repeating=ordinary; //фрагмент, заполненный одинаковыми символами;
		while (current<end)
		{
			if (*current != *repeating)
			{
				if (current-repeating>=MIN_REPEAT)  break;
				repeating=current;
			}
			current++;
		}
		count=current-repeating;

		if (count<MIN_REPEAT)  //дошли до конца, а повторений мало - отвергаем
		{
			repeating += count;  count=0;
		}
		if (repeating>ordinary)  //обычный фрагмент
		{
			CompressHeader head={0};
			head.bCompressed=FALSE;
			head.wCount=repeating-ordinary;
			memcpy(lpNewData,&head,sizeof(head));    lpNewData += sizeof(head);
			memcpy(lpNewData,ordinary,head.wCount); lpNewData += head.wCount;
		}
		if (count)				 //фрагмент, заполненный символом (*repeated)
		{
			CompressHeader head={0};
			head.bCompressed=TRUE;
			head.cRepeater=*repeating;
			head.wCount=count;
			memcpy(lpNewData,&head,sizeof(head));    lpNewData += sizeof(head);
		}
		ordinary=current;
	}
	while(ordinary<end);
	*compressedData = newData;  *compressedSize = lpNewData-newData;
	return TRUE;
}

Bool32 mywrite(void* of, const void* lpdata, int32_t size)
{
	ofstream *ofile = (ofstream*)of;

	if (ofile->write((char*)lpdata, size).fail())
		return FALSE;

	return TRUE;
}

#define VERSION_FILE            0xF002
#define VERSION_FILE_COMPRESSED 0xF003
#define TYPE_text				"TYPE_TEXT"
#define TYPE_image				"TYPE_IMAGE"
#define TYPE_page				"__PageInfo__"
const uint32_t type_text_len = strlen(TYPE_text) + 1;
const uint32_t type_image_len = strlen(TYPE_image) + 1;
const uint32_t type_page_len = strlen(TYPE_page) + 1;
const uint32_t data_size = sizeof(POLY_);
const uint32_t page_size = sizeof(PAGEINFO);

Bool32 writeBin(char* file_name, int32_t nIncline, Rect16* aRect, uint32_t* aType, uint32_t* aNumber, int32_t aCount)
{
	ofstream bin_file(file_name, ios::out|ios::binary);

	if (bin_file.fail())
		return FALSE;

	uint32_t vers = VERSION_FILE_COMPRESSED;
	Bool32 rc =	mywrite(&bin_file, &vers, sizeof(vers));
	int count = 1;//number of pages
	uint32_t flags = 0;

	if (rc)
		rc = mywrite(&bin_file, &count, sizeof(count));

	count = 0;//something from BACKUPPAGE

	if (rc)
		rc = mywrite(&bin_file, &count, sizeof(count));

	count = aCount;//number of blocks

	if (rc)
		rc = mywrite(&bin_file, &count, sizeof(count));

	for (int32_t i = 0; i < aCount && rc; i++)
	{//from BLOCK
//		rc = mywrite(&bin_file, &aNumber[i], sizeof(aNumber[i]));//UserNum
		rc = mywrite(&bin_file, &flags, sizeof(flags));//UserNum

		if (rc)
			rc = mywrite(&bin_file, &flags, sizeof(flags));

		if (rc)
		{//from DATA
			if (aType[i] == 0)//text
			{
				rc = mywrite(&bin_file, &type_text_len, sizeof(type_text_len));

				if (rc)
					rc = mywrite(&bin_file, TYPE_text, type_text_len);
			}
			else if (aType[i] == 1)//image
			{
				rc = mywrite(&bin_file, &type_image_len, sizeof(type_image_len));

				if (rc)
					rc = mywrite(&bin_file, TYPE_image, type_image_len);
			}
			else rc = FALSE;

			if (rc)
			{
				char* compressed_data;
				uint32_t compressed_size;

				POLY_ block;

				memset(&block, 0, data_size);

				block.com.count = 4;
				block.com.Vertex[0].x = aRect[i].left-5;
				block.com.Vertex[0].y = aRect[i].top-5;
				block.com.Vertex[1].x = aRect[i].right+5;
				block.com.Vertex[1].y = aRect[i].top-5;
				block.com.Vertex[2].x = aRect[i].right+5;
				block.com.Vertex[2].y = aRect[i].bottom+5;
				block.com.Vertex[3].x = aRect[i].left-5;
				block.com.Vertex[3].y = aRect[i].bottom+5;

				rc = Compress((char*)&block, data_size, &compressed_data, &compressed_size);

				if (rc)
					rc = mywrite(&bin_file, &compressed_size, sizeof(compressed_size));
				else compressed_data = NULL;

				if (rc)
					rc = mywrite(&bin_file, compressed_data, compressed_size);

				if (compressed_data)
					delete compressed_data;
			}
		}

		if (rc)
//			rc = mywrite(&bin_file, &aNumber[i], sizeof(aNumber[i]));//InterNum
			rc = mywrite(&bin_file, &flags, sizeof(flags));//UserNum
	}

	if (rc)
		rc = mywrite(&bin_file, &type_page_len, sizeof(type_page_len));

	if (rc)
		rc = mywrite(&bin_file, TYPE_page, type_page_len);

	if (rc)
	{//from PAGEINFO
		PAGEINFO info;
		char* compressed_data;
		uint32_t compressed_size;

		memset(&info, 0, page_size);

		info.Incline2048 = nIncline;

		rc = Compress((char*)&info, page_size, &compressed_data, &compressed_size);

		if (rc)
			rc = mywrite(&bin_file, &compressed_size, sizeof(compressed_size));
		else compressed_data = NULL;

		if (rc)
			rc = mywrite(&bin_file, compressed_data, compressed_size);

		if (compressed_data)
			delete compressed_data;
	}

	bin_file.close();

	return rc;
}
*/
/***********************************************************************************************/
