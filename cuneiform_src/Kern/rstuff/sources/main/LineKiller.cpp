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

/**********  Заголовок  ****************************************************************************/
/*  Автор,                                                                                         */
/*  комментарии                                                                                    */
/*  правка     : Алексей Коноплёв                                                                  */
/*  Редакция   :  15.11.00                                                                         */
/*  Файл       :  'LineKiller.cpp'                                                                 */
/*  Содержание :  Удаление остатков линий                                                          */
/*  Назначение :  Удаление остатков линий после снятия линий                                       */
/*  Комментарий:                                                                                   */
/*                                                                                                 */
/*-------------------------------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include "rstuff.h"
#include "rsfunc.h"
#include "rsglobaldata.h"
#include "dpuma.h"

#include "mpumatime.h"
#include "pumadef.h"
#include "ccom.h"
#include "cpage.h"
#include "rline.h"
#include "cline.h"
#include "linedefs.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32 LineKiller(PRSPreProcessImage Image)
{
	LinesTotalInfo LTInfo;
	LineInfo       LHorLineInfo[2];
	LineInfo       LVerLineInfo[2];

	LTInfo.Hor.Lns = LHorLineInfo;
	LTInfo.Ver.Lns = LVerLineInfo;

	return SearchAndKill( Image, &LTInfo );

}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32 SearchAndKill ( PRSPreProcessImage Image, LinesTotalInfo *LTInfo )
{
	Bool32         bRet = TRUE;
	uint32_t         nTagSize;
	int32_t          j;
	char           str[255];
	Rect16         ZoomRect;
	int32_t          LineCount;
	Point16        LinePoints[4];
	Point16        KillPoints[4];
	int32_t          HalfThickness;
	int32_t          HalfThicknessB;

	nTagSize = sizeof (LinesTotalInfo);

	uint32_t size_line_com=sizeof(LINE_COM);
	Handle hCPage=Image->hCPAGE;
	LineInfo linfo;
	CLINE_handle* pCLINE=(CLINE_handle*)(Image->phCLINE);

	int count_comp=CCOM_GetContainerVolume(*(Image->phCCOM));
	int count_line=CLINE_GetLineCount(*pCLINE);
	if(count_comp>10000||count_line>600)
		return TRUE;

	 CLINE_handle hline;
	 CPDLine cpdata;
	 LineCount=0;
	 hline=CLINE_GetFirstLine(*pCLINE);

	 if(!hline)
		 return TRUE;
	 hLineKillerWindow = LDPUMA_GetWindowHandle(NAME_IMAGE_DELLINE);

	 if ( !LDPUMA_Skip(hDebugKillLines) )
	 {
		bShowLineDebug = TRUE;
	 }
	 else
	 {
		bShowLineDebugData = bShowStepLineDebug = bShowLineDebug = FALSE;
	 }

	 if ( !LDPUMA_Skip(hDebugKillLinesShowComponentsBefore) )
	 {
		DebugDPumaShowComponents( Image, hLineKillerWindow,0xfefe00, 1, 314);
		LDPUMA_Console("Вот, то что было сначала.. теперь нажми что нибудь и пошли дальше...\n");
		LDPUMA_WaitUserInput( NULL, hLineKillerWindow );
		LDPUMA_DeleteRects(hLineKillerWindow, 314);
	 }
	 while(hline)
	 {
      cpdata=CLINE_GetLineData(hline);
      if(!cpdata)
	     hline=CLINE_GetNextLine(hline);
      else
	  {
	   if(cpdata->Dir==LD_Horiz)
	   {
            if ( gKillComponents == 2 || (cpdata->Flags & LI_IsTrue) )
			{
				if ( !LDPUMA_Skip(hDebugKillLinesStep) )
				{
					bShowStepLineDebug = TRUE;
				}
				else
				{
					bShowStepLineDebug = FALSE;
				}

				if ( !LDPUMA_Skip(hDebugKillLinesData) )
				{
					bShowLineDebugData = TRUE;
				}
				else
				{
					bShowLineDebugData = FALSE;
				}

				if ( LineCount++ != 0 && bShowStepLineDebug )
				{
					LDPUMA_WaitUserInput( NULL, hLineKillerWindow );
					//LDPUMA_DeleteRects(hLineKillerWindow, 316);
				}

				if ( bShowLineDebug || bShowStepLineDebug)
				{
					HalfThickness = cpdata->Line.Wid10/20;
					HalfThicknessB = (cpdata->Line.Wid10/10 + 1) / 2;

					LinePoints[0].x = (int16_t)(cpdata->Line.Beg_X);
					LinePoints[0].y = (int16_t)(cpdata->Line.Beg_Y) - (int16_t)HalfThickness;
					LinePoints[1].x = (int16_t)(cpdata->Line.End_X);
					LinePoints[1].y = (int16_t)(cpdata->Line.End_Y) - (int16_t)HalfThickness;
					LinePoints[2].x = (int16_t)(cpdata->Line.End_X);
					LinePoints[2].y = (int16_t)(cpdata->Line.End_Y) + (int16_t)HalfThicknessB;
					LinePoints[3].x = (int16_t)(cpdata->Line.Beg_X);
					LinePoints[3].y = (int16_t)(cpdata->Line.Beg_Y) + (int16_t)HalfThicknessB;

					j = sprintf( str, "LineKiller: - Линия:< %4.4i, %4.4i > < %4.4i, %4.4i > x %3.3i",
								 cpdata->Line.Beg_X,
								 cpdata->Line.Beg_Y,
								 cpdata->Line.End_X,
								 cpdata->Line.End_Y,
								 cpdata->Line.Wid10/10);

					j += sprintf ( str + j, "\n");

					if ( bShowLineDebugData )
						LDPUMA_Console(str);

					//LDPUMA_DrawLine(hLineKillerWindow, &pLHor[1].A, &pLHor[1].B, 0/*LtiB->Skew1024*/, 0x00ff00, (pLHor[1].Thickness * (-100)), 315 );
					DebugDPumaDrawRect(hLineKillerWindow, LinePoints, 0x00ff00, 1, 315 );

					if ( gKillZone > 0 )
					{
						KillPoints[0].x = LinePoints[0].x - (int16_t)gKillZone;
						KillPoints[0].y = LinePoints[0].y - (int16_t)gKillZone;
						KillPoints[1].x = LinePoints[1].x + (int16_t)gKillZone;
						KillPoints[1].y = LinePoints[1].y - (int16_t)gKillZone;
						KillPoints[2].x = LinePoints[2].x + (int16_t)gKillZone;
						KillPoints[2].y = LinePoints[2].y + (int16_t)gKillZone;
						KillPoints[3].x = LinePoints[3].x - (int16_t)gKillZone;
						KillPoints[3].y = LinePoints[3].y + (int16_t)gKillZone;

						DebugDPumaDrawRect(hLineKillerWindow, KillPoints, 0x0fef00, -25, 315 );
					}
				}
				linfo.A.x=(int16_t)(cpdata->Line.Beg_X);
				linfo.A.y=(int16_t)(cpdata->Line.Beg_Y);
				linfo.B.x=(int16_t)(cpdata->Line.End_X);
				linfo.B.y=(int16_t)(cpdata->Line.End_Y);
				linfo.Thickness=cpdata->Line.Wid10/10;
				ComponentFilter(Image, &linfo);
			}
			hline=CLINE_GetNextLine(hline);
		  }
		  else
		  {
		    if ( gKillComponents == 2 || (cpdata->Flags & LI_IsTrue) )
			{
				if ( !LDPUMA_Skip(hDebugKillLinesStep) )
				{
					bShowStepLineDebug = TRUE;
				}
				else
				{
					bShowStepLineDebug = FALSE;
				}

				if ( !LDPUMA_Skip(hDebugKillLinesData) )
				{
					bShowLineDebugData = TRUE;
				}
				else
				{
					bShowLineDebugData = FALSE;
				}

				if ( LineCount++ != 0 && bShowStepLineDebug )
				{
					LDPUMA_WaitUserInput( NULL, hLineKillerWindow );
					//LDPUMA_DeleteRects(hLineKillerWindow, 316);
				}

				if ( bShowLineDebug || bShowStepLineDebug)
				{
					HalfThickness = cpdata->Line.Wid10 / 20;
					HalfThicknessB = (cpdata->Line.Wid10/10 + 1) / 2;

					LinePoints[0].x = (int16_t)(cpdata->Line.Beg_X) + (int16_t)HalfThicknessB;
					LinePoints[0].y = (int16_t)(cpdata->Line.Beg_Y);
					LinePoints[1].x = (int16_t)(cpdata->Line.End_X) + (int16_t)HalfThicknessB;
					LinePoints[1].y = (int16_t)(cpdata->Line.End_Y);
					LinePoints[2].x = (int16_t)(cpdata->Line.End_X) - (int16_t)HalfThickness;
					LinePoints[2].y = (int16_t)(cpdata->Line.End_Y);
					LinePoints[3].x = (int16_t)(cpdata->Line.Beg_X) - (int16_t)HalfThickness;
					LinePoints[3].y = (int16_t)(cpdata->Line.Beg_Y);

					j = sprintf( str, "LineKiller: | Линия: < %4.4i, %4.4i > < %4.4i, %4.4i > x %3.3i",
								 cpdata->Line.Beg_X,
								 cpdata->Line.Beg_Y,
								 cpdata->Line.End_X,
								 cpdata->Line.End_Y,
								 cpdata->Line.Wid10/10);


					j += sprintf ( str + j, "\n");

					if ( bShowLineDebugData )
						LDPUMA_Console(str);

					//LDPUMA_DrawLine(hLineKillerWindow, &pLVer[1].A, &pLVer[1].B, 0/*LtiB->Skew1024*/, 0x00ff00, ( pLVer[1].Thickness * (-100) ), 315 );
					DebugDPumaDrawRect(hLineKillerWindow, LinePoints, 0x00ff00, -25, 315 );

					if ( gKillZone > 0 )
					{
						KillPoints[0].x = LinePoints[0].x + (int16_t)gKillZone;
						KillPoints[0].y = LinePoints[0].y - (int16_t)gKillZone;
						KillPoints[1].x = LinePoints[1].x + (int16_t)gKillZone;
						KillPoints[1].y = LinePoints[1].y + (int16_t)gKillZone;
						KillPoints[2].x = LinePoints[2].x - (int16_t)gKillZone;
						KillPoints[2].y = LinePoints[2].y + (int16_t)gKillZone;
						KillPoints[3].x = LinePoints[3].x - (int16_t)gKillZone;
						KillPoints[3].y = LinePoints[3].y - (int16_t)gKillZone;

						DebugDPumaDrawRect(hLineKillerWindow, KillPoints, 0x00ef00, 1, 315 );
					}
				}

				linfo.A.x=(int16_t)(cpdata->Line.Beg_X);
				linfo.A.y=(int16_t)(cpdata->Line.Beg_Y);
				linfo.B.x=(int16_t)(cpdata->Line.End_X);
				linfo.B.y=(int16_t)(cpdata->Line.End_Y);
				linfo.Thickness=cpdata->Line.Wid10/10;
				ComponentFilter(Image, &linfo);
			}
		 hline=CLINE_GetNextLine(hline);
		}
	  }
	 }
	 if ( bShowLineDebug || bShowStepLineDebug )
	 {
		if ( bShowStepLineDebug )
		{
			LDPUMA_Console("LineKiller: Нажми на что нибудь и пойдем дальше...\n");
			LDPUMA_WaitUserInput( NULL, hLineKillerWindow );
		}

		LDPUMA_DeleteLines(hLineKillerWindow, 315);
		LDPUMA_DeleteRects(hLineKillerWindow, 316);
		ZoomRect.top    = 0;
		ZoomRect.bottom = 0;

		//if ( bShowStepLineDebug )
			//LDPUMA_ZoomToRect(NULL, &ZoomRect);
	 }
	 if ( !LDPUMA_Skip(hDebugKillLinesShowComponentsAfter) )
	 {
		DebugDPumaShowComponents( Image, hLineKillerWindow,0xfefe00, -100, 314);
		LDPUMA_Console("Вот, то что стало после.. теперь нажми что нибудь и пошли дальше...\n");
		LDPUMA_WaitUserInput( NULL, hLineKillerWindow );
		LDPUMA_DeleteRects(hLineKillerWindow, 314);
	 }
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32 ComponentFilter( PRSPreProcessImage Image, LineInfo *Line)
{
	CCOM_comp * pcomp;
	CCOM_comp * pdeadcom;
	Bool32 GoodComp;
	Rect16 Rc;
	Rect16 Rl;
	int32_t nRc = 0;
	int32_t Filter = 0;
	int32_t j = 0;
	char str[255];
	int16_t Thick = ( Line->Thickness / 2 ) + (int16_t)gKillZone;
	Bool32 bDieComponent = FALSE;


	Rl.left   = Line->A.x;
	Rl.top    = Line->A.y;
	Rl.right  = Line->B.x;
	Rl.bottom = Line->B.y;

	if ( Rl.left <= Rl.right )
	{
		Rl.left  -= Thick;
		Rl.left = Rl.left < 0 ? 0 : Rl.left;
		Rl.right += Thick;
	}
	else
	{
		Rl.left  += Thick;
		Rl.right -= Thick;
		Rl.right = Rl.right < 0 ? 0 : Rl.right;
	}

	if ( Rl.bottom <= Rl.top )
	{
		Rl.bottom  -= Thick;
		Rl.bottom = Rl.bottom < 0 ? 0 : Rl.bottom;
		Rl.top += Thick;
	}
	else
	{
		Rl.bottom  += Thick;
		Rl.top -= Thick;
		Rl.top = Rl.top < 0 ? 0 : Rl.top;
	}

	pdeadcom = CCOM_GetFirst (*Image->phCCOM, NULL);
	do
	{
		pcomp = CCOM_GetNext (pdeadcom, NULL);
		GoodComp = true;//CompIsGood (pcomp, Filter);

		if (GoodComp)
		{
			Rc.left   = pdeadcom->left;
			Rc.right  = pdeadcom->left + pdeadcom->w /*- 1*/;
			Rc.top    = pdeadcom->upper;
			Rc.bottom = pdeadcom->upper + pdeadcom->h /*- 1*/;
			nRc++;

			if ( IsRectIntersect(&Rl, &Rc) )
			{
				if ( TuneFilter(Line, &Rc, gKillZone, gKillRate ) )
				{
					if ( gKillComponents )
					{
						bDieComponent = CCOM_Delete(*Image->phCCOM, pdeadcom);
					}

					if ( bShowLineDebug || bShowStepLineDebug )
					{
						j = sprintf( str, "LineKiller: компоненту под ней нашли: < %4.4i, %4.4i > < %4.4i, %4.4i >", Rc.left, Rc.top, Rc.right, Rc.bottom );

						if ( bDieComponent )
							j += sprintf( str + j, " +dead+" );

						j += sprintf( str + j, "\n" );

						if ( bShowLineDebugData )
							LDPUMA_Console(str);

						LDPUMA_DrawRect(hLineKillerWindow, &Rc, 0, 0xff0000, -50, 316);
						bDieComponent = FALSE;
					}
				}
			}
		}

		pdeadcom = pcomp;
	} while( pcomp != NULL );

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32 IsRectIntersect(Rect16 *A, Rect16 *B)
{
	Bool32 rc = FALSE;

	int32_t M1 = A->bottom >= A->top  ? A->bottom : A->top;
	int32_t M2 = A->right  >= A->left ? A->right  : A->left;
	int32_t m1 = A->bottom >= A->top  ? A->top    : A->bottom;
	int32_t m2 = A->right  >= A->left ? A->left   : A->right;
	int32_t M3 = B->bottom >= B->top  ? B->bottom : B->top;
	int32_t M4 = B->right  >= B->left ? B->right  : B->left;
	int32_t m3 = B->bottom >= B->top  ? B->top    : B->bottom;
	int32_t m4 = B->right  >= B->left ? B->left   : B->right;

	if ( ( ( ( B->top >= m1 ) && ( B->top <= M1 ) ) || ( ( B->bottom >= m1 ) && ( B->bottom <= M1 ) ) ) &&
		 ( ( ( B->left >= m2 ) && ( B->left <= M2 ) ) || ( ( B->right >= m2 ) && ( B->right <= M2 ) ) ) )
	{
		rc = TRUE;
	}

	if ( ( ( ( A->top >= m3 ) && ( A->top <= M3 ) ) || ( ( A->bottom >= m3 ) && ( A->bottom <= M3 ) ) ) &&
		 ( ( ( A->left >= m4 ) && ( A->left <= M4 ) ) || ( ( A->right >= m4 ) && ( A->right <= M4 ) ) ) )
	{
		rc = TRUE;
	}

	return rc;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32 TuneFilter(LineInfo *Line, Rect16 *Rect, uint32_t KillZone, uint32_t Rate)
{
	Bool32 bRet;

	bRet = ChekComponentAndLine(Line, Rect, KillZone);

	if ( bRet )
		bRet = CheckSquare(Line, Rect, KillZone, Rate);

	return bRet;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32 ChekComponentAndLine(LineInfo *Line, Rect16 *Rect, uint32_t KillZone)
{
	int32_t A0, B0, A1, B1;              // начало и конец линии
	int32_t LineType;                    // Горизонтальная = 1 ( || > 1 ) или вертикальная = -1 ( || < 1 )
	int32_t As, Bs, C;
	Bool32 bRet = FALSE;
	uint32_t wN, wP, wZ;
	Point16  pPoints[4];
	int32_t Zone = KillZone;

	if ( Line->A.x == Line->B.x || Line->A.y == Line->B.y )
	{
		bRet = TRUE;
	}

	if ( ! bRet )
	{
		float temp = (float)(Line->A.x - Line->B.x) / (float)(Line->A.y - Line->B.y);
		int32_t iTemp;
		uint32_t SecondHand = 1;

		LineType = (temp > -1) && (temp < 1 ) ? -1 : 1;

		pPoints[3].x = pPoints[0].x = Rect->left;
		pPoints[1].y = pPoints[0].y = Rect->top;
		pPoints[2].x = pPoints[1].x = Rect->right;
		pPoints[3].y = pPoints[2].y = Rect->bottom;

		wN = wP = wZ = 0;

		do
		{

			switch ( SecondHand )
			{
			case 1:
				Zone = - ( ( (Line->Thickness + 1) / 2 ) + (int32_t)KillZone );
				break;
			case 0:
				Zone = ( ( (Line->Thickness + 1) / 2 ) + (int32_t)KillZone );
				break;
			default:
				Zone = 0;
			}

			A0 = Line->A.x + (LineType <= 0 ? Zone : 0);
			B0 = Line->A.y + (LineType >= 0 ? Zone : 0);
			A1 = Line->B.x + (LineType <= 0 ? Zone : 0);
			B1 = Line->B.y + (LineType >= 0 ? Zone : 0);

			As = A0 - A1;
			Bs = B0 - B1;
			C  = (B0 * As) - (A0 * Bs);

			for ( int32_t i = 0; i < 4; i++ )
			{
				iTemp = ( pPoints[i].x * Bs ) - ( pPoints[i].y * As ) + C;

				if ( iTemp == 0 )
				{
					wZ++;
					bRet = TRUE;
				}
				else
					if ( iTemp > 0 )
						wP++;
					else
						wN++;
			}



		} while ( SecondHand-- && ( bRet == FALSE ) );

		if ( wP < 8 && wN < 8 )
			bRet = TRUE;

		if ( wZ != 0 && ( wP == 0 || wN == 0 ) )
			bRet = FALSE;

	}

	return bRet;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32 CheckSquare(LineInfo *Line, Rect16 *Rect, uint32_t KillZone, uint32_t Rate)
{
	Bool32 bRet = FALSE;

	float temp;
	float Halfs = 1.0;
	int32_t LineType;
	Rect32 S;
	uint32_t SRect;
	uint32_t SLine;
	int32_t  iTemp;
	int32_t A0 = Line->A.x;
//	int32_t B0 = Line->B.y;
	int32_t B0 = Line->A.y; //Almi 30.11.00
	int32_t As = A0 - Line->B.x;
	int32_t Bs = B0 - Line->B.y;
	int32_t Thick = ((Line->Thickness + 1) / 2 ) + KillZone;

	if (Rate > 255)
		Rate = 255;

	if ( Line->A.y == Line->B.y )
		LineType = 1;
	else
		if ( Line->A.x == Line->B.x )
			LineType = -1;
		else
		{
			temp = (float)(Line->A.x - Line->B.x) / (float)(Line->A.y - Line->B.y);
			LineType = (temp > -1) && (temp < 1 ) ? -1 : 1;
		}


	if ( LineType > 0 )
	{
		// горизонтальные линии

		S.left = Line->A.x - KillZone;
		S.right = Line->B.x + KillZone;

		if ( Line->A.y > Line->B.y )
		{
			S.top = Line->B.y - Thick;
			S.bottom = Line->A.y + Thick;
		}
		else
		{
//			S.bottom = Line->B.y - Thick;
//			S.top = Line->A.y + Thick;
			S.top = Line->A.y - Thick;
			S.bottom = Line->B.y + Thick; //Almi 01.12.00
		}

		//смотрим точки пересечения компоненты с линией
/*		if ( !(Rect->left < S.left && Rect->right > S.left) )
		{
			S.left = Rect->left;
		}

		if ( !(Rect->left < S.right && Rect->right > S.right) )
		{
			S.right = Rect->right;
		}*/ //Всё неправильно! Almi 01.12.00
		if (S.left <= Rect->left)
			S.left = Rect->left;
		if (S.right >= Rect->right)
			S.right  = Rect->right;
		if (S.right <= S.left)
			S.right  = S.left; //End Almi


		S.bottom = S.top = B0 - (int32_t)( ( (float)Bs / (float)As ) * (float)(A0 - S.left) );
		iTemp = B0 - (int32_t)( ( (float)Bs / (float)As ) * (float)(A0 - S.right) );

		S.top = S.top < iTemp ? S.top - Thick : iTemp - Thick;
		S.bottom = S.bottom > iTemp ? S.bottom + Thick : iTemp + Thick;

/*		if ( !(Rect->top <= S.top && Rect->bottom >= S.top) )
		{
			S.top = Rect->top;
			Halfs = 0.5;
		}

		if ( !(Rect->top <= S.bottom && Rect->bottom >= S.bottom) )
		{
			S.bottom = Rect->bottom;
			Halfs = (float)(Halfs < 1.0 ? 0 : 0.5);
		}*/ //Всё неправильно! Almi 01.12.00
		if (S.top <= Rect->top)
			S.top = Rect->top;
		if (S.bottom >= Rect->bottom)
			S.bottom  = Rect->bottom;
		if (S.bottom <= S.top)
			S.bottom  = S.top; //End Almi

		//считаем площади
		iTemp = Rect->bottom - Rect->top;
		SRect = 1 + (iTemp < 0 ? -iTemp : iTemp);
		iTemp = Rect->right - Rect->left;
		SRect *= 1 + (iTemp < 0 ? -iTemp : iTemp);

		iTemp = S.bottom - S.top;
		SLine = 1 + (iTemp < 0 ? -iTemp : iTemp);
		iTemp = S.right - S.left;
		SLine *= 1 + (iTemp < 0 ? -iTemp : iTemp);
/*		iTemp = (int32_t)(Halfs * (float)iTemp / temp);
		SLine -= iTemp < 0 ? -iTemp : iTemp;*/

		// отношение
		if ( SLine != 0 )
		{
			temp = (float) SLine / (float) SRect;
			temp *= (float)255;

			if ( (uint32_t)temp >= Rate )
				bRet = TRUE;
		}
	}
	else
	{
		// вертикальные линии
		S.top = Line->A.y - KillZone;
		S.bottom = Line->B.y + KillZone;

		if ( Line->A.x > Line->B.x )
		{
			S.left = Line->B.x - Thick;
			S.right = Line->A.x + Thick;
		}
		else
		{
//			S.right = Line->B.x - Thick;
//			S.left = Line->A.x + Thick;
			S.left = Line->A.x - Thick;
			S.right = Line->B.x + Thick; //Almi 01.12.00
		}

		//смотрим точки пересечения компоненты с линией
/*		if ( !(Rect->top <= S.top && Rect->bottom >= S.top) )
		{
			S.top = Rect->top;
		}

		if ( !(Rect->top <= S.bottom && Rect->bottom >= S.bottom) )
		{
			S.bottom = Rect->bottom;
		}*/ //Всё неправильно! Almi 01.12.00
		if (S.top <= Rect->top)
			S.top = Rect->top;
		if (S.bottom >= Rect->bottom)
			S.bottom  = Rect->bottom;
		if (S.bottom <= S.top)
			S.bottom  = S.top; //End Almi

		S.left = S.right = A0 - (int32_t)( ( (float)As / (float)Bs ) * (float)(B0 - S.top) );
		iTemp = A0 - (int32_t)( ( (float)As / (float)Bs ) * (float)(B0 - S.bottom) );

		S.left = S.left < iTemp ? S.left - Thick : iTemp - Thick;
		S.right = S.right > iTemp ? S.right + Thick : iTemp + Thick;

/*		if ( !(Rect->left <= S.left && Rect->right >= S.left) )
		{
			S.left = Rect->left;
			Halfs = 0.5;
		}

		if ( !(Rect->left <= S.right && Rect->right >= S.right) )
		{
			S.right = Rect->right;
			Halfs = (float)(Halfs < 1.0 ? 0 : 0.5);
		}*/ //Всё неправильно! Almi 01.12.00
		if (S.left <= Rect->left)
			S.left = Rect->left;
		if (S.right >= Rect->right)
			S.right  = Rect->right;
		if (S.right <= S.left)
			S.right  = S.left; //End Almi

		//считаем площади
		iTemp = Rect->right - Rect->left;
		SRect = 1 + ( iTemp < 0 ? -iTemp : iTemp );
		iTemp = Rect->bottom - Rect->top;
		SRect *= 1 + ( iTemp < 0 ? -iTemp : iTemp );

		iTemp = S.right - S.left;
		SLine = 1 + iTemp < 0 ? -iTemp : iTemp;
		iTemp = 1 + S.bottom - S.top;
		SLine *= iTemp < 0 ? -iTemp : iTemp;
/*		iTemp = (int32_t)(Halfs * (float)iTemp / temp);
		SLine -= iTemp < 0 ? -iTemp : iTemp;*/ //Almi

		// отношение
		if ( SLine != 0 )
		{
			temp = (float) SLine / (float) SRect;
			temp *= (float)255;

			if ( (uint32_t)temp >= Rate )
				bRet = TRUE;
		}
	}

	return bRet;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//
void DebugDPumaDrawRect(Handle hWindow, Point16 * Points, uint32_t Color, int32_t Thickness, uint32_t Group)
{
	LDPUMA_DrawLine(hWindow, &Points[0], &Points[1], 0, Color, (int16_t)Thickness, Group );
	LDPUMA_DrawLine(hWindow, &Points[1], &Points[2], 0, Color, (int16_t)Thickness, Group );
	LDPUMA_DrawLine(hWindow, &Points[2], &Points[3], 0, Color, (int16_t)Thickness, Group );
	LDPUMA_DrawLine(hWindow, &Points[3], &Points[0], 0, Color, (int16_t)Thickness, Group );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//
void DebugDPumaShowComponents(PRSPreProcessImage Image, Handle hWindow, uint32_t Color, int32_t Thickness, uint32_t Group)
{
	CCOM_comp * pcomp;
	Rect16 Rc;

	pcomp = CCOM_GetFirst (*Image->phCCOM, NULL);
	if(!pcomp)
		return;

	do
	{
		Rc.left   = pcomp->left;
		Rc.right  = pcomp->left + pcomp->w /*- 1*/;
		Rc.top    = pcomp->upper;
		Rc.bottom = pcomp->upper + pcomp->h /*- 1*/;

		LDPUMA_DrawRect(hWindow, &Rc, 0, Color, (int16_t)Thickness, Group);

		pcomp = CCOM_GetNext (pcomp, NULL);

	} while( pcomp != NULL );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
// end of file
