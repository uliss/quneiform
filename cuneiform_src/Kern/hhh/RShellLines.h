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

// RShellLines.h: interface for the ShellLine's functions.
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
//
//	Description: ShellLine's functions definition
//
//	Implemented: by B.M. Shahverdiev
//
//

#include "globus.h"

#ifdef __RSHELLLINES_EXPORTS__
  #define RSHELLLINES_FUNC  FUN_EXPO
  #define RSHELLLINES_CLASS CLASS_EXPO
#else
  #define RSHELLLINES_FUNC  FUN_IMPO
  #define RSHELLLINES_CLASS CLASS_IMPO
#endif

#define step             8
#define SizeWhitePicsels 1
#define SizeBlackPicsels 16
#define MAXRASTERSIZE    1000

int    SL_IsInPoly(Point32* a, CLINE_SL_POLY* p); //
Bool   SL_IsPointInAB(Point32 *P,Point32 *A,Point32 *B); //
void   CalculateRectForRaster(DLine* pCLine, PAGEINFO* page_info); //
void   CalculatePolynomia(DLine* pCLine, PAGEINFO* page_info); //
void   DrowRectForRaster(Rect32* rect); //
void   DrowPolynomia(DLine* pCLine); //
//void   DrowGetCorrectedLineStripesIntervals(CLine* pCLine, Handle HndMyWindow);
void   DrowVerticalLineStripesIntervals(CLINE_handle line, Handle HndMyWindow);
void   CleaningRaster(DLine* pCLine, char* Buffer); //
void   DrowCleanedRaster(DLine* pCLine, char* Buffer, CIMAGEBITMAPINFOHEADER* image_info, Handle* HndMyWindow); //
void   DrowVerticalLineRaster(DLine* pCLine, char* Buffer, CIMAGEBITMAPINFOHEADER* image_info, Handle* HndMyWindow); //
void   Transpose_bit_matrixes(pchar buf_in,pchar buf_out, Rect32* rect); //
Bool   GetLineStripesIntervals(CLINE_handle line, DLine* pCLine, pchar  pRaster, Bool FlagVerticalLine); //
//Bool   GetCorrectedLineStripesIntervals(CLine* pCLine);
void   FillingStripes(CLINE_handle line, int y, int16_t Count, uint16_t* pIntervals, Bool FlagVerticalLine, DLine* pCLine); //
void   CheckDotLines(DLine* pCLine, Bool FlagVerticalLine); //
int    Bits2Ints(uchar* pBits, int nByteWidth, int32_t* pIntervals); //
void   TypeIntervals(int y, int16_t Count, uint16_t* pIntervals); //
void   TypeStripe(CLINE_handle hStripe, DEvent* pStripe); //
Bool   GetLineFragments(CLINE_handle line, DLine* pCLine); //
Handle DrawLineFragments(CLINE_handle line, CPDLine pCLine, Handle HndMyWindow, int ColorIndex, Bool32 ForAllLines = FALSE); //
//void DrawSeparationPoints(CLine* pCLine, Handle HndMyWindow);
Bool   CorrectLineFragments(CLINE_handle line, DLine* pCLine); //
HANDLE GetTransposedRaster(DLine* pCLine, char* Buffer, char** pRaster); //
void   FreeTransposedBuffer(HANDLE hbuf, char* pRaster); //
void   DrowVerticalLineCutPoints(CLINE_handle line, Handle HndMyWindowVert); //
void   Drow2Rects(Rect32*  rect, Rect32*  rect1); //
void   DeleteNoiseEvents(CLINE_handle hLine, DLine* pLine);
void   CheckUnderlining(CLINE_handle hLine, DLine* pLine, char* pSourceRaster);
Bool32 CompareRasterParts(CPDLine pLine, char* pSourceRaster, Bool32 CheckSerif);
//////////////////////////////////////////////////////////////////////////////////
//
#ifdef __RSL_EXTERN__
	#define EXTERN
	#define VAL(a) = a
#else
	#define EXTERN extern
	#define VAL(a)
#endif


EXTERN Handle hDebugRSL					                   VAL( NULL );

EXTERN Handle hDebugFrag                                   VAL( NULL );
EXTERN Handle hDebugFragOwner                              VAL( NULL );
EXTERN Handle hDebugDrowPolynomia                          VAL( NULL );
EXTERN Handle hDebugDrowRectForRaster                      VAL( NULL );
EXTERN Handle hDebugDrowCleanedRaster                      VAL( NULL );
EXTERN Handle hDebugDrowGetCorrectedLineStripesIntervals   VAL( NULL );
//EXTERN Handle hDebugDrowFriendLines                        VAL( NULL );
//EXTERN Handle hDebugDrowGroupOfExtensibleLines             VAL( NULL );
EXTERN Handle hDebugDroweparationPoints                    VAL( NULL );
//EXTERN Handle hDebugDrawLosedVerticalLines                 VAL( NULL );


EXTERN Handle hDebugDrowVerticalLineRaster                 VAL( NULL );
EXTERN Handle hDebugDrowVerticalLineStripesIntervals       VAL( NULL );
EXTERN Handle hDebugDrowVerticalLineCutPoints              VAL( NULL );

EXTERN Handle hDebugTypeIntervals                          VAL( NULL );
EXTERN Handle hDebugTypeStripe                             VAL( NULL );
EXTERN Handle hDebugDrawLineFragments                      VAL( NULL );
//EXTERN Handle hDebugDrawAllLineFragments                   VAL( NULL );
EXTERN Handle hDebugDrawLineFragmentsOwnWindow             VAL( NULL );

EXTERN Handle hDebugAttr                                   VAL( NULL );
EXTERN Handle hDebugAttrOwner                              VAL( NULL );
EXTERN Handle hDebugTypeWidth                              VAL( NULL );
EXTERN Handle hDebugTypeDegree                             VAL( NULL );

//EXTERN Handle hDebugDrowPass3                              VAL( NULL );
//EXTERN Handle hDebugDrowTableZone                          VAL( NULL );


EXTERN Handle hDebugProfileRSL	      	        		   VAL( NULL );
//EXTERN Handle hDebugProfStart	      	        		   VAL( NULL );
EXTERN Handle hDebugProfFindLineFrag  	        		   VAL( NULL );
EXTERN Handle hDebugProfEndFindLineFrag                    VAL( NULL );
EXTERN Handle hDebugProfSL_GetRaster                       VAL( NULL );
