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

// ============================================================================
// Written by Shahverdiev  Alik
// This file cration date: 17.08.99
//
// CreateRtf.cpp
//
// ============================================================================
//#include <afxtempl.h>
//#include <afxwin.h>
#include <stdio.h>
#include "stdafx.h"
#include "globus.h"
#include "creatertf.h"
#include "sys_prog.h"
#include "cpage.h"
#include "lst3_win.h"
#include "cfio.h"
#include "ced.h"
#include "edfile.h"
#include "dpuma.h"
#include "rtfedwrite.h"
#include "lnsdefs.h"
//#include "Rline.h"
#include "linedefs.h"
#include "normdefs.h"
#include "cstrdefs.h"
#include "cline.h"

#include "minmax.h"

extern "C" Bool  FullRtf( FILE *fpFileNameIn, const char *FileNameOut ,Handle* hEdTree);
extern "C" Bool  PageTree(FILE *fpFileNameIn, CRtfPage* RtfPage, const char *FileNameOut);
extern "C" Bool	 WriteTable( uint32_t IndexTable, RtfSectorInfo* SectorInfo, /*CString* TableString ,*/Bool OutPutMode);
extern "C" Bool	 WritePict( uint32_t IndexPict, RtfSectorInfo* SectorInfo/*, CString* PictString*/, Bool OutPutTypeFrame);
extern "C" { void GetTableRect( uint32_t NumberTable , Rect16* RectTable,uint32_t* UserNumber ); }
extern "C" { uchar GetPictRect ( uint32_t NumberPict  , Rect16* RectPict ,uint32_t* UserNumber ); }
extern  void RtfAssignRect_CRect_Rect16(RECT *s1,Rect16 *s2);
extern  void	RtfCalcRectSizeInTwips(RECT *s1, float Twips);
extern  void RtfUnionRect_CRect_CRect(RECT *s1,RECT *s2);
extern  void RtfAssignRect_CRect_CRect(RECT *s1,RECT *s2);

int16_t   CreateEmptyRtfFile(void);
void    PutC(char sym);
void    PutCom(const char *Command, int32_t value, int16_t space);
void    Put(const char *Data);
void    PutChar(uchar sym);
int16_t   get_font_name(int16_t FontNumber);

int16_t   GetRealSizeKegl( const char * /*CString**/ str, int16_t width, int16_t FontPointSize, int16_t FontNumber );
int16_t   GetRealSize( char* str,int16_t len,int16_t FontSize ,int16_t FontNumber,int16_t* strHeight);
Bool    ReadInternalFileRelease(FILE *fpFileNameIn, CRtfPage* RtfPage);
Handle  Rtf_CED_CreateParagraph(int16_t FirstIndent,int16_t LeftIndent,int16_t RightIndent,int16_t IntervalBefore, RtfSectorInfo *SectorInfo, int AlignParagraph,int shad, int LenthStringInTwips, int LengthFragmInTwips);
void    Rtf_CED_CreateChar( EDRECT* slayout, letterEx* Letter, CRtfChar* pRtfChar );
void    WriteCupDrop(CRtfChar* pRtfChar,int16_t font);
Bool    CheckLines(RECT* Rect, Bool FlagVer, RtfSectorInfo *SectorInfo);
void    Cleaning_LI_FRMT_Used_Flag(void);

float   Twips;
int16_t   K_TwipsInInch=1440;
uint16_t    FlagWriteRtfCoordinates =1 ;
char    WriteRtfPageNumber[MAX_PATH]="1";
uchar   Frmt_CharSet = (uchar)204;

extern  uint32_t FlagMode;
extern  char   UnRecogSymbol;
extern  uint32_t   gnLanguage;

extern  char   lpMyNameSerif[MAX_PATH];
extern  char   lpMyNameNonSerif[MAX_PATH];
extern  char   lpMyNameMono[MAX_PATH];
extern	 char   WriteRtfImageName[MAX_PATH];
extern	 char   RtfFileName[MAX_PATH];
extern  uint32_t CountPict;
extern  uint32_t CountTable;
extern  uint32_t RtfWriteMode;
extern  POINT  TemplateOffset;

#define  TwipsToEMU_Koef (360000 * 2.54)/1440

#define CHEREDOVON

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 FullRtf                                                        //
////////////////////////////////////////////////////////////////////////////////////////////////////
Bool FullRtf(FILE *fpFileNameIn, const char* FileNameOut, Handle* hEdTree)
{
	CRtfPage RtfPage;

	if(RtfWriteMode) // is activated ONLY in debug mode (нажать ??? в LDPUMA)
		Cleaning_LI_FRMT_Used_Flag(); //обнуление флажков, что линии между колонок

	if(FlagMode & USE_FRAME_AND_COLUMN)
	{
		if(!RtfPage.FindPageTree(fpFileNameIn,FileNameOut))
			return FALSE;

		RtfPage.SetTwips();
	}
	else
	{
 		if(!RtfPage.ReadInternalFile(fpFileNameIn))
			return FALSE;

		RtfPage.SetTwips();
		RtfPage.CorrectKegl();
		RtfPage.ChangeKegl();
	}

//	RtfPage.AddTables();
	RtfPage.AddPictures();

	if(FlagMode & USE_NONE)
        RtfPage.SortUserNumber();//в ручном layout user can establish own order of the fragments


#ifndef EdWrite
	RtfPage.Write(FileNameOut);
#else
	if(RtfPage.Write(FileNameOut) && 	!RtfWriteMode)
		RtfPage.Rtf_CED_WriteFormattedEd(FileNameOut , hEdTree);
#endif

	return TRUE;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 CRTFPAGE                                                       //
////////////////////////////////////////////////////////////////////////////////////////////////////
CRtfPage::CRtfPage()
{
	Count.RtfSectors            = 0;
 Count.RtfTextFragments      = 0;
 Count.RtfFrameTextFragments = 0;
 Count.RtfPictureFragments   = 0;
 Count.RtfTableFragments     = 0;
 Count.RtfStrings            = 0;
 Count.RtfWords              = 0;
 Count.RtfChars              = 0;
 FlagBadColumn               =	0;
	m_hED                       = NULL;
 SetRect(&m_rect, 32000, 32000, 0, 0 );
 SetRect(&m_rectReal, 32000, 32000, 0, 0 );
// m_arSectors.RemoveAll();
// m_arFragments.RemoveAll();
//	arKegls.RemoveAll();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 CRtfPage                                                       //
////////////////////////////////////////////////////////////////////////////////////////////////////
CRtfPage::~CRtfPage()
{
/* CRtfFragment* cFrag;
 CRtfSector*   cSector;
 uint32_t wCount, i;

	wCount = m_arSectors.GetSize();
	for( i=0; i<wCount; i++)
	{
		cSector = (CRtfSector*)m_arSectors[i];
		delete cSector;
	}

	wCount = m_arFragments.GetSize();
	for( i=0; i<wCount; i++)
	{
		cFrag = (CRtfFragment*)m_arFragments[i];
		delete cFrag;
	}
	*/
	for (std::vector<CRtfSector*>::iterator it=m_arSectors.begin();it<m_arSectors.end();it++)
		delete *it;
	for (std::vector<CRtfFragment*>::iterator it1=m_arFragments.begin();it1<m_arFragments.end();it1++)
		delete *it1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 Rtf_CED_CreatePage                                             //
////////////////////////////////////////////////////////////////////////////////////////////////////
void CRtfPage::Rtf_CED_CreatePage(void)
{
	if(RtfWriteMode)
		return;
#ifdef EdWrite
	int      PageNumber;
	EDSIZE   sizeOfImage;
	EDSIZE   sizeInTwips;
	EDSIZE   dpi;
	EDRECT   pageBordersInTwips;
	PAGEINFO PageInfo = {0};
	Bool     resizeToFit=FALSE;

	Handle hCPAGE = CPAGE_GetHandlePage( CPAGE_GetCurrentPage());
	GetPageInfo(hCPAGE,&PageInfo);
	PageNumber     = atoi((char*)WriteRtfPageNumber);
	sizeOfImage.cx = (int32_t)PageInfo.Width;
	sizeOfImage.cy = (int32_t)PageInfo.Height;
	sizeInTwips.cx = PaperW;
	sizeInTwips.cy = PaperH;
	dpi.cx         = (int32_t)PageInfo.DPIX;
	dpi.cy         = (int32_t)PageInfo.DPIY;

	pageBordersInTwips.left   = MargL;
	pageBordersInTwips.top    = MargT;
	pageBordersInTwips.right  = MargR;
	pageBordersInTwips.bottom = MargB;

	if(FlagMode & /*==*/ USE_NONE) //VMK1
		resizeToFit = FALSE;
	else
		resizeToFit = TRUE;

#ifdef _DEBUG
//	CED_SetLogFileName("ced.log");
#endif

	m_hED = CED_CreatePage((char*)WriteRtfImageName, sizeOfImage, dpi, (int)PageInfo.Incline2048,
			                     PageNumber, sizeInTwips, pageBordersInTwips, UnRecogSymbol,resizeToFit);
	CED_SetPageRecogLang(m_hED,(char)gnLanguage);
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 Rtf_CED_WriteFormattedEd                                       //
////////////////////////////////////////////////////////////////////////////////////////////////////
void CRtfPage::Rtf_CED_WriteFormattedEd(const char* RtfFileName, Handle* hEdTree)
{
#ifdef EdWrite
char lpEdFileName[MAX_PATH];
char lpEdTestFileName[MAX_PATH];

	if(RtfWriteMode)
	{
		return;
  strcpy( (char*)lpEdFileName, RtfFileName );
	 strncpy( (char*)lpEdFileName + strlen((char*)lpEdFileName) -3, "fed", 3 );

  strcpy( (char*)lpEdTestFileName, RtfFileName );
	 strncpy( (char*)lpEdTestFileName + strlen((char*)lpEdTestFileName) -3, "txt", 3 );
  CED_WriteFormattedEd( (char*)lpEdFileName, m_hED );
 	CED_DeletePage( m_hED );
	}
	else
  *hEdTree = m_hED;

#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 CRtfPageDelFragments                                           //
////////////////////////////////////////////////////////////////////////////////////////////////////
void CRtfPage::CRtfPageDelFragments(void)
{
/* CRtfFragment* cFrag;
 uint32_t wCount, i;

	wCount = m_arFragments.GetSize();
	for( i=0; i<wCount; i++)
	{
		cFrag = (CRtfFragment*)m_arFragments[i];
		delete cFrag;
	}
m_arFragments.RemoveAll();
*/
	for (std::vector<CRtfFragment*>::iterator it1=m_arFragments.begin();it1<m_arFragments.end();it1++)
		delete *it1;
	m_arFragments.clear();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 GetNextFragment                                                //
////////////////////////////////////////////////////////////////////////////////////////////////////
CRtfFragment* CRtfPage::GetNextFragment()
{
	m_arFragments.push_back( new CRtfFragment());
	return m_arFragments.back();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 OpenOutputFile                                                 //
////////////////////////////////////////////////////////////////////////////////////////////////////
Bool CRtfPage::OpenOutputFile(const char* FileNameOut)
{
 Bool Bisy  = TRUE;
	int  Count = 0;

#ifdef _DEBUG
	while(Bisy)
	{
	 if((out=fopen(FileNameOut, "w")) == NULL)
		{
		 MessageBoxA(NULL, "Can not open RTF file.\nPlease close file.",
		 "RTF", MB_ICONSTOP|MB_OK);
			SetFocus(NULL);
		}
		else
		{Bisy = FALSE; Count=0;}

		if(Count++>10)
			return FALSE;
 }
#else
 if((out=fopen(FileNameOut, "w")) == NULL)
		return FALSE;
#endif

	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 CloseOutputFile                                                //
////////////////////////////////////////////////////////////////////////////////////////////////////
void CRtfPage::CloseOutputFile(void)
{
 fclose(out);
}

//********* Чтение internal.vit
//*********  Частичное запольнения Page.
//*********  В Page записываютя:
//*********    текстовые фрагменты с содержимым
//*********    -- фреймы  ( врезки ),
//*********    -- обычные ( кандидаты на колонки )
//---------------------------------------
//*********    без содержимого
//*********    -- таблицы
//*********    -- картинки

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 ReadInternalFile                                               //
////////////////////////////////////////////////////////////////////////////////////////////////////
Bool CRtfPage::ReadInternalFile(FILE *fpFileNameIn)
{
	if(ReadInternalFileRelease( fpFileNameIn, this ))
	 return TRUE;
 return FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 ReadInternalFileRelease                                        //
////////////////////////////////////////////////////////////////////////////////////////////////////
Bool ReadInternalFileRelease(FILE *in, CRtfPage* RtfPage)
{
	CRtfFragment*   pRtfFragment;
	CRtfString*     pRtfString;
	CRtfWord*       pRtfWord;
	CRtfChar*       pRtfChar;

	int16_t  nc,ns,nw,nz,i;
	int16_t  tmp;
	uint32_t wtmp;
	Rect16 RectFragm;
	Rect16  SRect;

	rewind(in);

	fread(&tmp,2,1,in);
	RtfPage->m_wDpi = tmp;
	if(tmp)
	{
		Twips = ((float)K_TwipsInInch)/tmp;
		Twips = (float)((int)(Twips+0.5));
	}
	fread(&tmp,2,1,in);
	RtfPage->Count.RtfTextFragments = tmp;
	fread(&tmp,2,1,in);
	fread(&tmp,2,1,in);
	fread(&tmp,2,1,in);
	fread(&tmp,2,1,in);
	fread(&tmp,2,1,in);
	RtfPage->Count.RtfFrameTextFragments = 0;

	for(nc=0; nc < RtfPage->Count.RtfTextFragments; ++nc)
	{
		pRtfFragment = RtfPage->GetNextFragment();
		pRtfFragment->pRtfParent = RtfPage;
		pRtfFragment->m_wType = FT_TEXT;

		fread(&RectFragm,1,sizeof(Rect16),in);
		pRtfFragment->m_rect.left  = (int32_t)(RectFragm.left*Twips);
		pRtfFragment->m_rect.top   = (int32_t)(RectFragm.top*Twips);
		pRtfFragment->m_rect.right = (int32_t)(RectFragm.right*Twips);
		pRtfFragment->m_rect.bottom= (int32_t)(RectFragm.bottom*Twips);
		fread(&tmp,2,1,in);
		pRtfFragment->m_wStringsCount = tmp;
		fread(&wtmp,4,1,in);
		pRtfFragment->m_wUserNumber = wtmp;
		fread(&wtmp,4,1,in);

		for(ns=0; ns < pRtfFragment->m_wStringsCount; ++ns)
		{
			pRtfString = pRtfFragment->GetNextString();
			fread(&SRect,sizeof(Rect16),1,in);
			//Реальные коор. строки!
			fread(&SRect,sizeof(Rect16),1,in);
			fread(&tmp,2,1,in);
			pRtfString->m_wWordsCount = tmp;

			fread(&tmp,sizeof(uint32_t),1,in);//NEGA_STR
 			for(nw=0; nw < pRtfString->m_wWordsCount; ++nw)
			{
				pRtfWord = pRtfString->GetNextWord();
				fread(&tmp,2,1,in);
				pRtfWord->m_wCharsCount=tmp;
				fread(&tmp,2,1,in);
				pRtfWord->m_wFontNumber = (uint16_t)tmp;
				fread(&tmp,2,1,in);
				pRtfWord->m_wIdealFontPointSize = (uint16_t)tmp;

				for(nz=0; nz < pRtfWord->m_wCharsCount; ++nz)
				{
					uint16_t num;
     #pragma pack(1)
       struct ALT_TIGER1  {unsigned char let, prob;} alt1;
       struct ALT_TIGER2  {unsigned char language, spellnocarrying, FlagCapDrop, spell, base;} alt2;
     #pragma pack()

					pRtfChar = pRtfWord->GetNextChar();
					fread(&SRect,sizeof(Rect16),1,in);     //Ideal BOX
					pRtfChar->m_Idealrect.left           = SRect.left;
					pRtfChar->m_Idealrect.top            = SRect.top;
					pRtfChar->m_Idealrect.right          = SRect.right;
					pRtfChar->m_Idealrect.bottom         = SRect.bottom;

  					fread(&SRect,sizeof(Rect16),1,in);     //Real BOX
					pRtfChar->m_Realrect.left            = SRect.left;
					pRtfChar->m_Realrect.top             = SRect.top;
					pRtfChar->m_Realrect.right           = SRect.right;
					pRtfChar->m_Realrect.bottom          = SRect.bottom;

					fread(&num,sizeof(uint16_t),1,in);
					assert(num<=REC_MAX_VERS);
					pRtfChar->m_wCountAlt=MIN(num,REC_MAX_VERS);
					for(i=0; i<num; i++)
					{
						fread(&alt1,sizeof(struct ALT_TIGER1),1,in);
						if (i<REC_MAX_VERS)
						{
							pRtfChar->m_chrVersions[i].m_bChar = alt1.let;
							pRtfChar->m_chrVersions[i].m_bProbability = alt1.prob;
						}
					}
					fread(&alt2,sizeof(struct ALT_TIGER2),1,in);
					pRtfChar->m_blanguage             = alt2.language;
					pRtfChar->m_bFlg_spell_nocarrying = alt2.spellnocarrying;
					pRtfChar->m_bFlg_cup_drop         = alt2.FlagCapDrop;
					pRtfChar->m_bFlg_spell            = alt2.spell;

					pRtfChar->m_wFontNumber           = pRtfWord->m_wFontNumber;
					pRtfChar->m_wFontPointSize        = pRtfWord->m_wIdealFontPointSize;
				}
			}
		}
	}
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 SetTwips                                                       //
////////////////////////////////////////////////////////////////////////////////////////////////////
void CRtfPage::SetTwips(void)
{
	m_fTwips = Twips;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 AddTables                                                      //
////////////////////////////////////////////////////////////////////////////////////////////////////
/*void CRtfPage::AddTables(void)
{
	CRtfFragment* pRtfFragment;
 Rect16  RectPict;

	Count.RtfTableFragments = (uint16_t)CountTable;
 for(uint32_t i=0; i < (int)CountTable; i++ )
	{
  pRtfFragment = GetNextFragment();
  GetTableRect( i, &RectPict, (uint32_t*)&pRtfFragment->m_wUserNumber );
  RtfAssignRect_CRect_Rect16(&pRtfFragment->m_rect, &RectPict);
		RtfCalcRectSizeInTwips(&pRtfFragment->m_rect, Twips);
  pRtfFragment->m_wType = FT_TABLE;
 	if(FlagMode & USE_FRAME_AND_COLUMN)
		{
			pRtfFragment->m_wUserNumberForFormattedMode = pRtfFragment->m_wUserNumber;
   pRtfFragment->m_wUserNumber = i;
		}
	}
}*/

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 AddPictures                                                    //
////////////////////////////////////////////////////////////////////////////////////////////////////
void CRtfPage::AddPictures(void)
{
	CRtfFragment* pRtfFragment;
 Rect16  RectPict;

 Count.RtfPictureFragments = (uint16_t)CountPict;
 for(int i=0; i < (int)CountPict; i++ )
	{
  pRtfFragment = GetNextFragment();
  GetPictRect( i, &RectPict, (uint32_t*)&pRtfFragment->m_wUserNumber );
  RtfAssignRect_CRect_Rect16(&pRtfFragment->m_rect, &RectPict);
		RtfCalcRectSizeInTwips(&pRtfFragment->m_rect, Twips);
  pRtfFragment->m_wType = FT_PICTURE;
 	if(FlagMode & USE_FRAME_AND_COLUMN)
		{
			pRtfFragment->m_wUserNumberForFormattedMode = pRtfFragment->m_wUserNumber;
   pRtfFragment->m_wUserNumber = i;
		}
	}
}
//~ поиск межсекторных H_lines & межколонных V-Lines
////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 AddLines                                                      //
////////////////////////////////////////////////////////////////////////////////////////////////////
void CRtfPage::AddLines(void)
{
	CRtfSector* pRtfSector;
	CRtfSector* pRtfNextSector;
	RECT       Rect;

	int16_t CountSectors = m_arSectors.size();

	for( int i=0; i<CountSectors; i++ )
	{
		if(i==0)
		{
 			pRtfSector = m_arSectors[i];
			pRtfSector->SectorInfo.PaperW=PaperW;
			Rect.left=0;  Rect.right=PaperW;
			Rect.top =0;  Rect.bottom=MAX(0,pRtfSector->m_rect.top);
			if(CheckLines( &Rect, FALSE, &pRtfSector->SectorInfo ))
			{
				pRtfSector=*m_arSectors.insert( m_arSectors.begin()+i, new CRtfSector() );
				pRtfSector->m_bFlagLine = TRUE;
				pRtfSector->SectorInfo.PaperW=PaperW;
				CountSectors++;
				RtfAssignRect_CRect_CRect(&pRtfSector->m_rect, &Rect);
				RtfAssignRect_CRect_CRect(&pRtfSector->m_rectReal, &Rect);
			}
		}
		else
//		if( i < CountSectors )
		{
			pRtfSector = m_arSectors[i-1];
			pRtfSector->SectorInfo.PaperW=PaperW;
 	 		pRtfNextSector = m_arSectors[i];

			Rect.left=0;  Rect.right=PaperW;
			Rect.top =pRtfSector->m_rect.bottom;
			Rect.bottom=pRtfNextSector->m_rect.top;

			if(CheckLines( &Rect, FALSE, &pRtfSector->SectorInfo ))
			{
				pRtfSector=*m_arSectors.insert( m_arSectors.begin()+i, new CRtfSector() );
				pRtfSector->m_bFlagLine = TRUE;
				pRtfSector->SectorInfo.PaperW=PaperW;
				CountSectors++;
				RtfAssignRect_CRect_CRect(&pRtfSector->m_rect, &Rect);
			 	RtfAssignRect_CRect_CRect(&pRtfSector->m_rectReal, &Rect);
			}
		 }
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 SortUserNumber                                                 //
////////////////////////////////////////////////////////////////////////////////////////////////////
void	CRtfPage::SortUserNumber(void)
{
	CRtfFragment* pRtfFragment;
	uchar FlagChange;
 uint32_t mas[500],MinUserNumber=32000;
 int indexMinUserNumber,i,j;
 int16_t CountFragments;

	CountFragments = Count.RtfFrameTextFragments + Count.RtfTextFragments +
			               Count.RtfTableFragments     + Count.RtfPictureFragments;

 for( i=0; i<CountFragments; i++ )
	{
  MinUserNumber= 32000;
		FlagChange   = FALSE;
	 for( j=0; j<CountFragments; j++ )
		{
   pRtfFragment = (CRtfFragment*)m_arFragments[j];
	  if( pRtfFragment->m_wUserNumber < MinUserNumber )
			{
			 MinUserNumber       = pRtfFragment->m_wUserNumber;
				indexMinUserNumber  = j;
				FlagChange = TRUE;
			}
		}
  if(FlagChange)
		{
   pRtfFragment = (CRtfFragment*)m_arFragments[indexMinUserNumber];
			pRtfFragment->m_wUserNumber = 32000;
   mas[indexMinUserNumber] = i+1;
		}
	}
 for( i=0; i<CountFragments; i++ )
	{
  pRtfFragment = (CRtfFragment*)m_arFragments[i];
  pRtfFragment->m_wUserNumber = mas[i];
	}
}

//Режим работы: USE_FRAME_AND_COLUMN
//Разбиение страницы на сектора
//Секторы на горизантальные колонки
//Горизантальные колонки на вертикальные колонки
//Определение терминальности вертикальных колонок
////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 FindPageTree                                                   //
////////////////////////////////////////////////////////////////////////////////////////////////////
Bool CRtfPage::FindPageTree(FILE *fpFileNameIn, const char* FileNameOut)
{
	return	PageTree( fpFileNameIn, this ,FileNameOut);
}

//////////////////////////////////////////////////////////////////////////////////////////
//           Вычисления ширены и высоты листа                                           //
//////////////////////////////////////////////////////////////////////////////////////////
void CRtfPage::ReCalcPageWidthAndHeight(void)
{
	int32_t LeftPos = 32000, TopPos=32000, RightPos =-32000, BottomPos=-32000, Height=-32000, Width=0;
	CRtfSector*           pRtfSector;

	if( FlagMode & USE_NONE )
	{// Фрагменты отписываются по пользовательским номерам
		MargL = DefMargL;   MargR = DefMargR;
		MargT = DefMargT;   MargB = DefMargB;
		for(std::vector<CRtfFragment*>::iterator ppRtfFragment=m_arFragments.begin(); ppRtfFragment<m_arFragments.end(); ppRtfFragment++ )
		{
			Width = MAX( Width, (*ppRtfFragment)->m_rect.right - (*ppRtfFragment)->m_rect.left );
		}
		PaperW = MAX( DefaultWidthPage, (int32_t)(Width/** Twips*/) + MargL + MargR );
		PaperH = DefaultHeightPage;
	}
	else
	if( (FlagMode & USE_FRAME) || FlagBadColumn )
	{// Все фрагменты фреймы

		m_arSectors.push_back( new CRtfSector());
  		pRtfSector = m_arSectors.back();

		for(std::vector<CRtfFragment*>::iterator ppRtfFragment=m_arFragments.begin(); ppRtfFragment<m_arFragments.end(); ppRtfFragment++ )
		{
			LeftPos      = MIN( LeftPos  , (int16_t)(*ppRtfFragment)->m_rect.left   );
			TopPos       = MIN( TopPos   , (int16_t)(*ppRtfFragment)->m_rect.top    );
			RightPos     = MAX( RightPos , (int16_t)(*ppRtfFragment)->m_rect.right  );
			BottomPos    = MAX( BottomPos, (int16_t)(*ppRtfFragment)->m_rect.bottom );
		}

		pRtfSector->m_rectReal.left=pRtfSector->m_rect.left=LeftPos;
		pRtfSector->m_rectReal.right=pRtfSector->m_rect.right=RightPos;
		pRtfSector->m_rectReal.top=pRtfSector->m_rect.top=TopPos;
		pRtfSector->m_rectReal.bottom=pRtfSector->m_rect.bottom=BottomPos;

		SetPaperSize(LeftPos,RightPos,TopPos,BottomPos,&PaperW,&PaperH,&MargL,&MargR,&MargT,&MargB);
			InitMargL=MargL; InitMargR=MargR; InitMargT=MargT; InitMargB=MargB;
	}
	else
	{// Фрагменты отписываются после изучения структуры страницы

		for(std::vector<CRtfFragment*>::iterator ppRtfFragment=m_arFragments.begin(); ppRtfFragment<m_arFragments.end(); ppRtfFragment++ )
		{
			LeftPos      = MIN( LeftPos  , (int16_t)(*ppRtfFragment)->m_rect.left   );
			TopPos       = MIN( TopPos   , (int16_t)(*ppRtfFragment)->m_rect.top    );
			RightPos     = MAX( RightPos , (int16_t)(*ppRtfFragment)->m_rect.right  );
			BottomPos    = MAX( BottomPos, (int16_t)(*ppRtfFragment)->m_rect.bottom );
		}

		SetPaperSize(LeftPos,RightPos,TopPos,BottomPos,&PaperW,&PaperH,&MargL,&MargR,&MargT,&MargB);
			InitMargL=MargL; InitMargR=MargR; InitMargT=MargT; InitMargB=MargB;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
//           Вычисления ширены и высоты листа                                           //
//////////////////////////////////////////////////////////////////////////////////////////
void CRtfPage::SetPaperSize(int32_t  LeftPos, int32_t RightPos, int32_t TopPos, int32_t BottomPos,
																												int32_t* PaperW , int32_t* PaperH ,
																												int32_t* MargL  , int32_t* MargR  , int32_t* MargT, int32_t* MargB)
{
	int i,j;

 int32_t  MargL2 = DefMargL/2,   MargL10 = DefMargL/10,   MargL0  = 0;
 int32_t  MargR2 = DefMargR/2,   MargR10 = DefMargR/10,   MargR0  = 0;
 int32_t  MargT2 = DefMargT/2,   MargT10 = DefMargT/10,   MargT0  = 0;
 int32_t  MargB2 = DefMargB/2,   MargB10 = DefMargB/10,   MargB0  = 0;

 int32_t  MasMargL[]={DefMargL,MargL2,MargL10,MargL0};
 int32_t  MasMargR[]={DefMargR,MargR2,MargR10,MargR0};
 int32_t  MasMargT[]={DefMargT,MargT2,MargT10,MargT0};
 int32_t  MasMargB[]={DefMargB,MargB2,MargB10,MargB0};

	int32_t  WidthPage[]={11906,20163,23812,23812};   // A4,Legal,A3,A2
 int32_t  HeightPage[]={16838,12242,16838,33676};  // A4,Legal,A3,A2
 for(j=0;j<3;j++)
	{

		for(i=0;i<4;i++)
		{
			if(((RightPos + MasMargL[i] + MasMargR[i]) <= WidthPage[j]) &&
				  ((BottomPos + MasMargT[i] + MasMargB[i]) <= HeightPage[j]))
			{
				*MargL = MasMargL[i]; *MargR = MasMargR[i];
				if((BottomPos + 2*(MasMargT[i] + MasMargB[i])) >= HeightPage[j])
				{*MargT = MasMargT[3]; *MargB = MasMargB[3];}
    else
				{*MargT = MasMargT[i]; *MargB = MasMargB[i];}

				*PaperW = WidthPage[j];
	 		*PaperH = HeightPage[j];
				return;
			}
		}

		for(i=0;i<4;i++)
		{
			if(((RightPos + MasMargL[i] + MasMargR[i]) <= HeightPage[j]) &&
				  ((BottomPos + MasMargT[i] + MasMargB[i]) <= WidthPage[j]))
			{
				*MargL = MasMargL[i]; *MargR = MasMargR[i];
				if((BottomPos + 2*(MasMargT[i] + MasMargB[i])) >=  WidthPage[j])
				{*MargT = MasMargT[3]; *MargB = MasMargB[3];}
    else
				{*MargT = MasMargT[i]; *MargB = MasMargB[i];}
				*PaperW = HeightPage[j];
	 		*PaperH = WidthPage[j];
				return;
			}
		}

	}

 *MargL = MargL2; *MargR = MargR2;
	*MargT = MargT2; *MargB = MargB2;
 *PaperW = WidthPage[3] ;
 *PaperH = HeightPage[3];
}

//////////////////////////////////////////////////////////////////////////////////////////
//            Моя коррекция кегля с учетом размера строки и заданного фонта             //
//////////////////////////////////////////////////////////////////////////////////////////
void CRtfPage::CorrectKegl(void)
{
	CRtfFragment*   pRtfFragment;
	CRtfString*     pRtfString;
	CRtfWord*       pRtfWord;
	CRtfChar        *pRtfChar,*pLastChar,*pFirstChar;

	uint16_t CountTextFragments;
	uint16_t CountStrings;
	uint16_t CountWords;
	uint16_t CountChars;
	int16_t LenghtStr,Real_Size_Kegl;
	char tmp_str[MAX_BUFFER_SIZE] ;

	CountTextFragments = Count.RtfTextFragments + Count.RtfFrameTextFragments;
	for(int i=0; i<CountTextFragments; i++)
	{
		pRtfFragment = m_arFragments[i];
		CountStrings = pRtfFragment->m_wStringsCount;

		for(int ns=0; ns<CountStrings; ns++ )
		{
			pRtfString = pRtfFragment->m_arStrings[ns];
			CountWords = pRtfString->m_wWordsCount;

			//Считаем длину получившейся строки
			int len=0;
			for( int w=0; w < CountWords; w++ )
				len+=pRtfString->m_arWords[w]->m_wCharsCount+1;
			//Выделяем буфер под неё
			char* TmpString=new char[len+1];
			TmpString[0]=0;

			//Формируем полную строку
			for(int nw=0; nw<CountWords; nw++)
			{
			    int nz;
				pRtfWord   = pRtfString->m_arWords[nw];
				CountChars = pRtfWord->m_wCharsCount;
				for(nz=0; nz<CountChars; nz++)
				{
					pRtfChar = pRtfWord->m_arChars[nz];
					tmp_str[nz] = pRtfChar->m_chrVersions[0].m_bChar;
					if(!nz)
						pRtfChar->m_wFontPointSize = MIN(pRtfChar->m_wFontPointSize , MaxFontSize);
				}
				if(nw <CountWords-1)
					tmp_str[nz++]=' ';
				tmp_str[nz]=0;

				strcat(TmpString,tmp_str);
			}

			pRtfWord   = (CRtfWord*)pRtfString->m_arWords[0];
			pFirstChar = (CRtfChar*)pRtfWord->m_arChars[0];
			pRtfWord   = (CRtfWord*)pRtfString->m_arWords[CountWords-1];
			CountChars = pRtfWord->m_wCharsCount;
			pLastChar  = (CRtfChar*)pRtfWord->m_arChars[CountChars-1];

			LenghtStr  = (int16_t)( pLastChar->m_Idealrect.right - pFirstChar->m_Idealrect.left);
      // adjust kegl to the text line real width (Microsoft function)
			Real_Size_Kegl = GetRealSizeKegl( TmpString, LenghtStr,	pFirstChar->m_wFontPointSize, pFirstChar->m_wFontNumber );
			AddNewKegl(pLastChar->m_wFontPointSize,Real_Size_Kegl);
			delete []TmpString;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 ChangeKegl                                                     //
////////////////////////////////////////////////////////////////////////////////////////////////////
void CRtfPage::ChangeKegl(void)
{
	CRtfFragment*   pRtfFragment;
 CRtfString*     pRtfString;
 CRtfWord*       pRtfWord;

	uint16_t  CountTextFragments;
 uint16_t  CountStrings;
	uint16_t  CountWords;

	CountTextFragments = Count.RtfTextFragments + Count.RtfFrameTextFragments;
// по частоте встречаемости выбираем преобразование из реал. в идеал. кегль
 for(int i=0; i<CountTextFragments; i++)
	{
  pRtfFragment = (CRtfFragment*)m_arFragments[i];
  CountStrings = pRtfFragment->m_wStringsCount;

		for(int ns=0; ns<CountStrings; ns++ )
		{
   pRtfString = (CRtfString*)pRtfFragment->m_arStrings[ns];
			CountWords = pRtfString->m_wWordsCount;

			for(int nw=0; nw<CountWords; nw++)
			{
    pRtfWord   = (CRtfWord*)pRtfString->m_arWords[nw];
				if(CountStrings==1)
 				pRtfWord->m_wRealFontPointSize = GetMinKegl( pRtfWord->m_wIdealFontPointSize );
    else
	 			pRtfWord->m_wRealFontPointSize = GetNewKegl( pRtfWord->m_wIdealFontPointSize );
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 AddNewKegl                                                     //
////////////////////////////////////////////////////////////////////////////////////////////////////
void CRtfPage::AddNewKegl( int16_t OldKegl, int16_t NewKegl )
{
	KEGL tmp;

	for (std::vector<KEGL>::iterator kegl=arKegls.begin(); kegl<arKegls.end(); kegl++)
	{
		if( OldKegl == kegl->OldKegl &&  kegl->NewKegl == NewKegl )
		{
			kegl->Count++;
			return;
		}
	}

	tmp.OldKegl = OldKegl;
	tmp.NewKegl = NewKegl;
	tmp.Count   = 1;
	arKegls.push_back( tmp );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 GetNewKegl                                                     //
////////////////////////////////////////////////////////////////////////////////////////////////////
int16_t CRtfPage::GetNewKegl( int16_t OldKegl )
{
	int16_t FlagChange = 0,Count = 0,tmpKegl,FlagPenalty = 1;

	for (std::vector<KEGL>::iterator kegl=arKegls.begin(); kegl<arKegls.end(); kegl++)
	{
		if( OldKegl == kegl->OldKegl )
		{
			if(OldKegl <= 14)
			{
				if(Count < kegl->Count)
				{
					FlagChange = 1;
					Count      = kegl->Count;
					tmpKegl    = kegl->NewKegl;
				}
			}
			else
			{
				FlagChange = 1;
				tmpKegl    = MIN(kegl->NewKegl, OldKegl);
			}
		}
	}

	if( FlagChange )
	{
		for (std::vector<KEGL>::iterator __kegl=arKegls.begin(); __kegl<arKegls.end(); __kegl++)
		{
			if( OldKegl == __kegl->OldKegl )
			{
				if((__kegl->Count>(Count/4)) && __kegl->NewKegl<tmpKegl)
				{
					tmpKegl    = __kegl->NewKegl;
					FlagPenalty = 0;
				}
				else
					if(FlagPenalty && (__kegl->Count>(Count/10)) && __kegl->NewKegl<tmpKegl)
					{
						tmpKegl--;
						tmpKegl = MAX(tmpKegl,4);
					}
			}
		}
		OldKegl = tmpKegl;
	}
	OldKegl = MAX(4,OldKegl);
	return OldKegl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 GetMinKegl                                                     //
////////////////////////////////////////////////////////////////////////////////////////////////////
int16_t CRtfPage::GetMinKegl( int16_t OldKegl )
{
	int16_t FlagChange = 0,Count = 0,tmpKegl=72;

	for (std::vector<KEGL>::iterator kegl=arKegls.begin(); kegl<arKegls.end(); kegl++)
	{
		if( OldKegl == kegl->OldKegl )
		{
			FlagChange = 1;
			tmpKegl = MIN(kegl->NewKegl, tmpKegl);
		}
	}

	if( FlagChange )
		OldKegl = tmpKegl;
	return OldKegl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 Write                                                          //
////////////////////////////////////////////////////////////////////////////////////////////////////
Bool CRtfPage::Write(const char *FileNameOut)
{
	if(RtfWriteMode)
	{
		if(!OpenOutputFile( FileNameOut ))
			return FALSE;
	}

	if( FlagMode & USE_NONE )                      // Фрагменты отписываются по пользовательским номерам
	{
		ReCalcPageWidthAndHeight();
#ifdef EdWrite
		if(!RtfWriteMode)
			Rtf_CED_CreatePage();
#endif
		if(!WriteHeaderRtf())
	 		return FALSE;
		Write_USE_NONE();
	}
	else
	if( (FlagMode & USE_FRAME) || FlagBadColumn  ) // Все фрагменты фреймы
	{
		ReCalcPageWidthAndHeight();
#ifdef EdWrite
		if(!RtfWriteMode)
			Rtf_CED_CreatePage();
#endif

		if(!WriteHeaderRtf())
			return FALSE;

		Write_USE_FRAME();
	}
	else
	{
		ToPlacePicturesAndTables();
		ReCalcPageWidthAndHeight();
#ifdef EdWrite
		if(!RtfWriteMode)
 			Rtf_CED_CreatePage();
#endif
		if(!WriteHeaderRtf())
			return FALSE;

		AddLines();
		Write_USE_FRAME_AND_COLUMN();   // Фрагменты отписываются после изучения структуры страницы
	}

	if(RtfWriteMode)
 		CloseOutputFile();

	return TRUE;
}

// Фрагменты отписываются по пользовательским номерам
////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 Write_USE_NONE                                                 //
////////////////////////////////////////////////////////////////////////////////////////////////////
Bool CRtfPage::Write_USE_NONE()
{
	int16_t         NumberCurrentFragment,InGroupNumber;
	uchar          FragmentType;
	CRtfFragment  *pRtfFragment;
	CRtfSector    *pRtfSector;

	int16_t CountSectors = Count.RtfFrameTextFragments + Count.RtfTextFragments +
			                    Count.RtfTableFragments     + Count.RtfPictureFragments;
	for( int16_t i=0; i<CountSectors; i++ )
	{
		m_nCurSectorNumber=i;
		InGroupNumber = i;
 		NumberCurrentFragment = GetFlagAndNumberFragment( &FragmentType, &InGroupNumber );
 		m_arSectors.push_back( new CRtfSector());
		pRtfSector = m_arSectors.back();
		pRtfSector->SectorInfo.userNum = NumberCurrentFragment;
		pRtfSector->m_VTerminalColumnNumber=1;

		WriteSectorsHeader(i);

		pRtfFragment = (CRtfFragment*)m_arFragments[NumberCurrentFragment];

		if(FragmentType==FT_TABLE)
			pRtfFragment->FWriteTable( InGroupNumber, &pRtfSector->SectorInfo, FOT_SINGLE);
		else
		if(FragmentType==FT_PICTURE)
			pRtfFragment->FWritePicture( InGroupNumber, &pRtfSector->SectorInfo, FOT_SINGLE);
		else
		{
			PutCom("\\cols",1,0);
			PutCom("\\colno",1,0);
			PutCom("\\colw",PaperW,0);
			pRtfFragment->pRtfParent=this;
			pRtfFragment->FWriteText( InGroupNumber ,&pRtfSector->SectorInfo, FOT_SINGLE);
		}
	}
	Put("}");
	PutChar(1);
	return TRUE;
}

// Все фрагменты фреймы
////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 Write_USE_FRAME                                                //
////////////////////////////////////////////////////////////////////////////////////////////////////
Bool CRtfPage::Write_USE_FRAME()
{
	int16_t InGroupNumber;
 CRtfFragment*  pRtfFragment;
 RtfSectorInfo* SectorInfo;
	CRtfSector *pRtfSector;
	int32_t shpleft,shptop,shpright,shpbottom,shpwr=0;

#ifdef EdWrite
 EDBOX       EdFragmRect;
	Handle     	hParagraph=NULL;
	Handle     	hString=NULL;
 int         align;
	EDRECT      indent;
 EDSIZE      interval;
	EDBOX       playout;
#endif

 int16_t CountFragments = Count.RtfFrameTextFragments + Count.RtfTextFragments +
		                      Count.RtfTableFragments     + Count.RtfPictureFragments;

	WriteSectorsHeader(0);
	pRtfSector = (CRtfSector* )m_arSectors[0];
 SectorInfo = &pRtfSector->SectorInfo;
 SectorInfo->Offset.x=SectorInfo->Offset.y=0;
	Put("\\pard\\fs6\\par");

#ifdef EdWrite
 	 if(!RtfWriteMode && CountFragments)
		 {
    indent.left   =  0;
    indent.right  =  0;
    indent.top    =  0;
    indent.bottom =  0;
    interval.cx   =  0;
				interval.cy   =  0;
    playout.x     = -1;
    playout.w     = -1;
    playout.y     = -1;
    playout.h     = -1;

    align = TP_LEFT_ALLIGN;
    hParagraph = CED_CreateParagraph(SectorInfo->hEDSector, SectorInfo->hObject, align, indent,
 			                         SectorInfo->userNum, -1, interval, playout, -1, -1, -1, -1, FALSE);
    hString    = CED_CreateLine( hParagraph, FALSE , 6);
	  }
#endif


	for( int16_t i=0; i<CountFragments; i++ )
	{
  pRtfFragment = (CRtfFragment*)m_arFragments[i];

  if(pRtfFragment->m_wType==FT_TABLE)
		{
			InGroupNumber = i - (Count.RtfFrameTextFragments + Count.RtfTextFragments);
   pRtfFragment->FWriteTable( InGroupNumber, SectorInfo, FOT_FRAME);
		}
		else
   if(pRtfFragment->m_wType==FT_PICTURE)
			{
 			InGroupNumber = i - (Count.RtfFrameTextFragments + Count.RtfTextFragments + Count.RtfTableFragments);
    pRtfFragment->FWritePicture( InGroupNumber, SectorInfo, FOT_FRAME);
			}
   else
			{
 			Put("{\\pard\\plain\\slmult1\\nowidctlpar\\pvpage");
	 		shpleft   = pRtfFragment->m_rect.left;
		 	PutCom("\\posx",shpleft,0);
			 shptop    = pRtfFragment->m_rect.top;
			 PutCom("\\posy",shptop,0);
			 shpbottom = pRtfFragment->m_rect.bottom-pRtfFragment->m_rect.top;
			 PutCom("\\absh",shpbottom,0);
			 shpright  = pRtfFragment->m_rect.right-pRtfFragment->m_rect.left;
			 PutCom("\\absw",shpright,0);
			 Put("\\abslock1");
			 Put("\\dxfrtext86\\dfrmtxtx86\\dfrmtxty43\\adjustright");

    #ifdef EdWrite
  	 if(!RtfWriteMode)
    {
     EdFragmRect.x = pRtfFragment->m_rect.left;
     EdFragmRect.w = pRtfFragment->m_rect.right-pRtfFragment->m_rect.left;
     EdFragmRect.y = pRtfFragment->m_rect.top;
     EdFragmRect.h = pRtfFragment->m_rect.bottom-pRtfFragment->m_rect.top;
			  SectorInfo->hObject = CED_CreateFrame(SectorInfo->hEDSector, SectorInfo->hColumn, EdFragmRect, 0x22,-1, 86, 43);
    }
    #endif

    SectorInfo->FlagOverLayed=FALSE;
	pRtfFragment->pRtfParent=this;
    pRtfFragment->FWriteText( i, SectorInfo, FOT_FRAME);
			 Put("}");
   }
 }

 Put("}");
 PutChar(1);
 return TRUE;

}
//табл. помещаются в существующие сектора как frame (м.б.изменение размера колонки) или создаются для них новые сектора(терминал.колонка)
////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 ToPlacePicturesAndTables                                       //
////////////////////////////////////////////////////////////////////////////////////////////////////

void CRtfPage::ToPlacePicturesAndTables(void)
{
	CRtfSector*   pRtfSector;
	int j,CountSectors;

	for(std::vector<CRtfFragment*>::iterator ppRtfFragment=m_arFragments.begin(); ppRtfFragment<m_arFragments.end(); ppRtfFragment++ )
	{
		if((*ppRtfFragment)->m_wType==FT_PICTURE || (*ppRtfFragment)->m_wType==FT_TABLE)
		{
			CountSectors = m_arSectors.size();
   //страница пустая:создается новый сектор для них
			if(CountSectors == 0)
			{
				m_arSectors.push_back(new CRtfSector());
				pRtfSector=m_arSectors.front();
				RtfAssignRect_CRect_CRect(&pRtfSector->m_rect, &(*ppRtfFragment)->m_rect);
				RtfAssignRect_CRect_CRect(&pRtfSector->m_rectReal, &(*ppRtfFragment)->m_rect);
				pRtfSector->ToPlacePicturesAndTables((*ppRtfFragment));
				continue;
			}
   //картина или таблица ниже последного сектора:создается новый сектор для них
			pRtfSector = m_arSectors.back();
			if( (*ppRtfFragment)->m_rect.top >= pRtfSector->m_rectReal.bottom )
			{
				m_arSectors.push_back(new CRtfSector());
				pRtfSector=m_arSectors.back();
				RtfAssignRect_CRect_CRect(&pRtfSector->m_rect, &(*ppRtfFragment)->m_rect);
				RtfAssignRect_CRect_CRect(&pRtfSector->m_rectReal, &(*ppRtfFragment)->m_rect);
				pRtfSector->ToPlacePicturesAndTables((*ppRtfFragment));
				continue;
			}

			for( j=0; j<CountSectors; j++ )
			{//картина или таблица между секторами:создается новый сектор для них
				pRtfSector = m_arSectors[j];
				if( (*ppRtfFragment)->m_rect.bottom <= pRtfSector->m_rectReal.top )
				{
					pRtfSector=*m_arSectors.insert( m_arSectors.begin()+j, new CRtfSector() );
					RtfAssignRect_CRect_CRect(&pRtfSector->m_rect, &(*ppRtfFragment)->m_rect);
					RtfAssignRect_CRect_CRect(&pRtfSector->m_rectReal, &(*ppRtfFragment)->m_rect);
					pRtfSector->ToPlacePicturesAndTables((*ppRtfFragment));
					break;
				}
				else
				{//картина или таблица внутри сектора: добавляетс
					if(
					   ( (*ppRtfFragment)->m_rect.top    <   pRtfSector->m_rectReal.top &&
						    (*ppRtfFragment)->m_rect.bottom >   pRtfSector->m_rectReal.top) ||

						( (*ppRtfFragment)->m_rect.top    >=  pRtfSector->m_rectReal.top &&
						    (*ppRtfFragment)->m_rect.top    <   pRtfSector->m_rectReal.bottom)
					)
 					{
						pRtfSector->ToPlacePicturesAndTables((*ppRtfFragment));
						break;
					}
				}
			}

		}
	}
}

// Фрагменты отписываются после изучения структуры страницы
////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 Write_USE_FRAME_AND_COLUMN                                     //
////////////////////////////////////////////////////////////////////////////////////////////////////
Bool CRtfPage::Write_USE_FRAME_AND_COLUMN()
{
    int i;
	CRtfSector* pRtfSector;
	CRtfSector* pRtfNextSector;

	int16_t CountSectors = m_arSectors.size();

	//Считаем расстояния между секциями (кроме последней)
	for(i=0; i<CountSectors-1; i++ )
	{
		pRtfSector = m_arSectors[i];
		pRtfNextSector = m_arSectors[i+1];
		pRtfSector->SectorInfo.InterSectorDist = GetFreeSpaceBetweenSectors( pRtfSector, pRtfNextSector);
	}

	//Отписываем каждую секцию
	for( i=0; i<CountSectors; i++ )
	{
		m_nCurSectorNumber=i;
		pRtfSector = m_arSectors[i];
		pRtfSector->m_PagePtr = this;
		if(pRtfSector->m_bFlagLine == FALSE)
			pRtfSector->CalcSector();
		WriteSectorsHeader(i);
		pRtfSector->Write();
	}
	Put("}");
	PutChar(1);

	return TRUE;
}


// чтобы дать больший, чем default, вертикальный offset of the section

////////////////////////////////////////////////////////////////////////////////////////////////////
//                       GetFreeSpaceBetweenSectors                                               //
////////////////////////////////////////////////////////////////////////////////////////////////////
uint16_t CRtfPage::GetFreeSpaceBetweenSectors(CRtfSector* pRtfSector, CRtfSector* pRtfNextSector)
{
	CRtfFragment       *pRtfFragment;
	RECT              FreePlace;
	int                i,CountFragments;
	int32_t              FreePlaceHeight;

	FreePlace.left     = pRtfSector->m_rect.left;
	FreePlace.right    = pRtfSector->m_rect.right;
	FreePlace.bottom   = pRtfNextSector->m_rect.top  - 1;
	FreePlace.top      = pRtfSector->m_rect.bottom + 1;
	if(FreePlace.top  >= FreePlace.bottom)
		return 0;

	CountFragments = m_arFragments.size(); //это множество фрагментов pRtfNextSector
 	for( i=0; i<CountFragments; i++ )
	{
		pRtfFragment = m_arFragments[i];
		if( pRtfFragment->m_rect.bottom <= FreePlace.top    || pRtfFragment->m_rect.right <= FreePlace.left ||
				   pRtfFragment->m_rect.top    >= FreePlace.bottom || pRtfFragment->m_rect.left  >= FreePlace.right )
			continue;

		if( pRtfFragment->m_rect.top >= FreePlace.top && pRtfFragment->m_rect.top <= FreePlace.bottom )
			FreePlace.bottom = pRtfFragment->m_rect.top;
	}

	FreePlaceHeight = MAX(0,FreePlace.bottom - FreePlace.top);

	return (uint16_t)FreePlaceHeight;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 WriteHeaderRtf                                                 //
////////////////////////////////////////////////////////////////////////////////////////////////////
Bool CRtfPage::WriteHeaderRtf(void)
{
 int16_t  NumFont = 4, i;
	uint16_t cr=13/*0x0d*/,lf=10/*0x0a*/;
	const char *TitleRtf="\\rtf1\\ansi\\deff0\\deflang1024",*NameStyle="PUMA";
	char  Eol[3],Nname[260];

#ifdef EdWrite
	uchar fontPitchAndFamily;
#endif

	 Eol[0]=(char)cr; Eol[1]=(char)lf; Eol[2]=0;

	 PutChar(0); //Clear buffer

  Put("{");
		Put(TitleRtf);
		Put(" ");
		Put(Eol);
		//--WRITE Font Table--
  FONT   *Font;
	 FONT_COD  FontCod[12]={
		"Arial Cyr"            , "fswiss",     // NonSerif
		"Times New Roman Cyr"  , "froman",     // Serif
		"Courier Cyr"          , "fmodern",   // Fixed_Pitch
		"Arial Narrow"         , "fswiss"     // NonSerif
		};

		Font=(FONT*)malloc(NumFont*sizeof(FONT));
		if(!Font)
			return FALSE;
		for(i=0; i < NumFont; ++i)
		{
			Font[i].family=(char*)malloc(strlen_m(FontCod[i].family)+1);
			strcpy_m(Font[i].family,FontCod[i].family);
 		switch(i)
			{
				case 0:
								Font[i].name=(char*)malloc(strlen_m((const char*)lpMyNameNonSerif)+1);
											strcpy_m(Font[i].name,(char*)lpMyNameNonSerif);
	  					break;
				case 1:
								Font[i].name=(char*)malloc(strlen_m((char*)lpMyNameSerif)+1);
											strcpy_m(Font[i].name,(char*)lpMyNameSerif);
			  			break;
				case 2:
								Font[i].name=(char*)malloc(strlen_m((char*)lpMyNameMono)+1);
											strcpy_m(Font[i].name,(char*)lpMyNameMono);
								break;
				case 3:
								Font[i].name=(char*)malloc(strlen_m("Arial Narrow")+1);
											strcpy_m(Font[i].name,"Arial Narrow");
	  					break;
			}
			Font[i].Italic=0;
			Font[i].Bold=0;
			Font[i].Underline=0;
		}
  Put("{\\fonttbl"); Put(Eol);
  for(i=0; i < NumFont; ++i)
		{
			Put("{");
   PutCom("\\f",i,0);
			Put("\\");
			Put(Font[i].family);
		 PutCom("\\fcharset",Frmt_CharSet,0);
   Put(" ");
   Put(Font[i].name);
   Put(";}");
			Put(Eol);

#ifdef EdWrite
 		switch(i)
			{
				case 0:
					   fontPitchAndFamily = FF_SWISS;
	  					break;
				case 1:
					   fontPitchAndFamily = FF_ROMAN;
			  			break;
				case 2:
					   fontPitchAndFamily = FF_MODERN;
								break;
				case 3:
					   fontPitchAndFamily = FF_SWISS;
	  					break;
			}

   if(!RtfWriteMode)
    CED_CreateFont(m_hED, (uchar)i, fontPitchAndFamily, (uchar)Frmt_CharSet, Font[i].name);
#endif
		}
  Put("}");
  Put(Eol);
  Put("{\\info");
		Put("{\\title WinTiger}");
		Put("}");
		Put(Eol);

		for(i=0; i < NumFont; ++i)
		{
 		free(Font[i].family);
 		free(Font[i].name);
		}
		free(Font);

 //WRITE IMAGE INFORMATION
	Put("{\\*\\imagename \"");
 strcpy(Nname,(const char*)WriteRtfImageName);
	Put(Nname);
	Put("\" \\imagepage");
 strcpy(Nname,(const char*)WriteRtfPageNumber);
	Put(Nname);
 Put(";}");

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 WriteSectorsHeader                                             //
////////////////////////////////////////////////////////////////////////////////////////////////////
void CRtfPage::WriteSectorsHeader(int16_t i)
{
	CRtfSector *pRtfSector;
 int         CountHTerminalColumns;

#ifdef EdWrite
	int     j;
	int     EDCountHTerminalColumns;
	EDRECT  border;
	EDCOL   *pEDColumnFirst,*pEDColumn;
#endif

	if(i)  Put("\\sect");
	Put("\\sectd ");
	if(!i) Put("\\sbkpage");
	if(i)  Put("\\sbknone");

	PutCom("\\paperw",PaperW,0);
	PutCom("\\paperh",PaperH,0);

	pRtfSector = m_arSectors[i];
	if (i>0)							//!!!Art
		pRtfSector->SectorInfo.Offset.y = m_arSectors[i-1]->m_rectReal.bottom+m_arSectors[i-1]->SectorInfo.InterSectorDist;//!!!Art
	else								//!!!Art
		pRtfSector->SectorInfo.Offset.y = pRtfSector->m_rectReal.top;
	pRtfSector->SectorInfo.FlagOneString=FALSE;

	pRtfSector->SectorInfo.PaperW = PaperW;
	pRtfSector->SectorInfo.PaperH = PaperH;

    // m_bFlagLine (есть линии) => не пытаться сдвигать margL для красоты
	if( FlagMode & USE_FRAME_AND_COLUMN && pRtfSector->m_bFlagLine == FALSE)
	{
		pRtfSector->GetCountAndRightBoundVTerminalColumns();
		CountHTerminalColumns = pRtfSector->m_arWidthTerminalColumns.size();
		if(CountHTerminalColumns)
		{
			if(pRtfSector->m_FlagOneString == FALSE)
				MargL = pRtfSector->m_arRightBoundTerminalColumns[0];
			else
			{
				MargL =MIN(InitMargL, pRtfSector->m_arRightBoundTerminalColumns[0]);
				pRtfSector->SectorInfo.FlagOneString = TRUE;
			}
		}
		else
			MargL = MAX(pRtfSector->m_rectReal.left,0);
	}

	PutCom("\\margl", MargL,0);

	if(FlagMode & USE_FRAME_AND_COLUMN && pRtfSector->m_bFlagLine == FALSE)
	{
		if(CountHTerminalColumns)
		{
			if(pRtfSector->m_FlagOneString == FALSE)
 				MargR = PaperW - (pRtfSector->m_arRightBoundTerminalColumns[CountHTerminalColumns-1] +
				                  pRtfSector->m_arWidthTerminalColumns[CountHTerminalColumns-1]);
			else
				MargR = MIN(InitMargR, PaperW - (pRtfSector->m_arRightBoundTerminalColumns[CountHTerminalColumns-1] +
				                                 pRtfSector->m_arWidthTerminalColumns[CountHTerminalColumns-1]));
		}
		else
			MargR = PaperW - pRtfSector->m_rectReal.right;
	}

	PutCom("\\margr", MargR,0);
	PutCom("\\margt", MargT,0);
	PutCom("\\margb", MargB,0);
	if(!CountHTerminalColumns && !(FlagMode & USE_FRAME))
		Put("\\pard\\fs6\\par");

	pRtfSector->SectorInfo.Offset.x = MargL;

	pRtfSector->SectorInfo.MargL = MargL;
	pRtfSector->SectorInfo.MargR = MargR;

	if(RtfWriteMode)
		return;

#ifdef EdWrite
	border.left   = MargL;
	border.right  = MargR;
	border.top    = MargT;
	border.bottom = MargB;

	if(FlagMode & USE_FRAME_AND_COLUMN)
		pRtfSector->SectorInfo.userNum = -1;
	else
		CountHTerminalColumns =0;

	if( FlagMode & USE_FRAME_AND_COLUMN && pRtfSector->m_bFlagLine == TRUE)
		CountHTerminalColumns =0;

	EDCountHTerminalColumns = CountHTerminalColumns;
	if(!EDCountHTerminalColumns)
		EDCountHTerminalColumns = 1;
	pEDColumnFirst = pEDColumn = (EDCOL*)malloc( EDCountHTerminalColumns * sizeof(EDCOL) );
	if(!pEDColumn)
		return;
	if(!CountHTerminalColumns)
	{
		pEDColumn->width = PaperW - (MargL + MargR);
		pEDColumn->space = 0;
	}

	for(j=0; j<CountHTerminalColumns; j++)
	{
		if( FlagMode & USE_FRAME_AND_COLUMN && pRtfSector->SectorInfo.FlagOneString == TRUE )
 			pEDColumn->width = PaperW - (MargL + MargR);
		else
			pEDColumn->width = pRtfSector->m_arWidthTerminalColumns[j];
		if(j<CountHTerminalColumns-1)
		{
			pEDColumn->space = pRtfSector->m_arRightBoundTerminalColumns[j+1]-
				(pRtfSector->m_arRightBoundTerminalColumns[j]+pEDColumn->width);
		}
		else
			pEDColumn->space = 0;

		pEDColumn++;
	}

	pRtfSector->m_hEDSector = CED_CreateSection(m_hED, border, -1, EDCountHTerminalColumns,pEDColumnFirst,0,
		                                           PaperW, PaperH, 0, -1, -1);

	pRtfSector->SectorInfo.hEDSector    = pRtfSector->m_hEDSector;
	pRtfSector->SectorInfo.hEDPage      = m_hED;
	pRtfSector->SectorInfo.hFirstColumn = CED_CreateColumn( pRtfSector->SectorInfo.hEDSector );
	pRtfSector->SectorInfo.hColumn      = pRtfSector->SectorInfo.hFirstColumn;
	pRtfSector->SectorInfo.hObject      = pRtfSector->SectorInfo.hFirstColumn;

	free( pEDColumnFirst );
#endif

}

//Ишется следуюший фрагмент по пользовательскому номеру
////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 GetFlagAndNumberFragment                                       //
////////////////////////////////////////////////////////////////////////////////////////////////////
int16_t CRtfPage::GetFlagAndNumberFragment( uchar* FragmentType, int16_t* InGroupNumber )
{
 int16_t j,i,CountT,CountTT,CountTTP;
 CRtfFragment* pRtfFragment;

  *FragmentType   = FT_TEXT;
		i=*InGroupNumber;

		CountT   = Count.RtfTextFragments  + Count.RtfFrameTextFragments;
		CountTT  = CountT  + Count.RtfTableFragments;
		CountTTP = CountTT + Count.RtfPictureFragments;

  for(j=0; j<CountTTP; j++ )
		{
   pRtfFragment = (CRtfFragment*)m_arFragments[j];
   if((uint32_t)(i+1) == pRtfFragment->m_wUserNumber)
			{
				i=j;
				break;
			}
		}

		if( i < CountT )
		{
			*InGroupNumber = i;
   *FragmentType   = FT_TEXT;
		}
		else
		{
		 if( i < CountTT )
			{
			 *InGroupNumber = i - CountT;
    *FragmentType  = FT_TABLE;
			}
		 else
			{
			 *InGroupNumber = i - CountTT;
  	 *FragmentType = FT_PICTURE;
			}
		}

return i;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 CRTFSECTOR                                                     //
////////////////////////////////////////////////////////////////////////////////////////////////////
CRtfSector::CRtfSector()
{
	m_bFlagLine = FALSE;
	m_wHorizontalColumnsCount = 0;
	m_FlagOneString = FALSE;
	SectorInfo.Offset.x = DefMargL/2;
	SectorInfo.Offset.y = 32000;
	SectorInfo.CountFragments = 0;
	SectorInfo.InterSectorDist = 0;
	SetRect(&m_rect, 32000, 32000, 0, 0 );
	SetRect(&m_rectReal, 32000, 32000, 0, 0 );
//	m_arHorizontalColumns.RemoveAll();
//	m_arHTerminalColumnsIndex.RemoveAll();
//	m_arRightBoundTerminalColumns.RemoveAll();
//	m_arWidthTerminalColumns.RemoveAll();
}

CRtfSector::~CRtfSector()
{
	CRtfHorizontalColumn* cHorizontalColumn;
	m_wHorizontalColumnsCount = m_arHorizontalColumns.size();
	for( int i=0; i<m_wHorizontalColumnsCount; i++ )
	{
		cHorizontalColumn = m_arHorizontalColumns[i];
		delete cHorizontalColumn;
	}
//	m_arHorizontalColumns.RemoveAll();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 CalcSector                                                     //
////////////////////////////////////////////////////////////////////////////////////////////////////
void CRtfSector::CalcSector(void)
{
	CRtfHorizontalColumn* pRtfHorizontalColumn;
	m_wHorizontalColumnsCount = m_arHorizontalColumns.size();
	for(int ih=0; ih<m_wHorizontalColumnsCount; ih++)
	{
		pRtfHorizontalColumn	= m_arHorizontalColumns[ih];
		pRtfHorizontalColumn->CalcHorizontalColumn();
		if(pRtfHorizontalColumn->m_wType < HC_AllFrame)
			m_arHTerminalColumnsIndex.push_back(ih);
 }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 Write                                                          //
////////////////////////////////////////////////////////////////////////////////////////////////////
Bool CRtfSector::Write(void)
{
#ifdef EdWrite
	Handle  hParagraph=NULL;
	Handle  hString=NULL;	EDRECT  indent;
	EDBOX   playout;
	EDSIZE  interval;
	int     align;
#endif

	if(m_bFlagLine==FALSE)
	{
		FillingSectorInfo();
		WriteNonTerminalColumns(); //FRAMES привязанные к началу сектора (это работает только когда сектор целиком состоит из frames)
		WriteTerminalColumnsTablesAndPictures(); // каждая таблица может иметь leftOffs & topOffset
		WriteTerminalColumns(); //это настоящие (не Frames) колонки - каждая колонки может иметь leftOffs & topOffset of section
		Put("{\\pard\\fs6");
		PutCom("\\sa",SectorInfo.InterSectorDist,0);
		Put("\\par}");
	}
	else
	{
		Put("{\\pard\\plain\\nowidctlpar\\brdrt\\brdrs\\brdrw15\\adjustright\\fs6");
		PutCom("\\sa",SectorInfo.InterSectorDist,0);
		Put("\\par}");
	}

	if(RtfWriteMode)
		return TRUE;

#ifdef EdWrite
	indent.left   =  0;
	indent.right  =  0;
	indent.top    =  0;
	indent.bottom =  0;
	interval.cx   =  0;
	interval.cy   =  SectorInfo.InterSectorDist;
	playout.x     = -1;
	playout.w     = -1;
	playout.y     = -1;
	playout.h     = -1;

	align = TP_LEFT_ALLIGN;
	hParagraph = CED_CreateParagraph( SectorInfo.hEDSector, SectorInfo.hColumn,
		align, indent,SectorInfo.userNum, -1, interval, playout, -1, /*0*/ -1, -1, -1, FALSE );
	if(m_bFlagLine==TRUE)
	{
		CED_SetParaBorders(hParagraph,0,0,0,0,1,20,0,0,TRUE);
	}
	hString    = CED_CreateLine( hParagraph, FALSE , 6);
#endif

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 WriteTerminalColumnsTablesAndPictures                          //
////////////////////////////////////////////////////////////////////////////////////////////////////
void CRtfSector::WriteTerminalColumnsTablesAndPictures(void)
{
	int32_t CountHTerminalColumns,TerminalColumnNumber;
	CRtfHorizontalColumn *pRtfHorizontalColumn;

 	CountHTerminalColumns  = m_arHTerminalColumnsIndex.size();

	for(int i=0; i<CountHTerminalColumns; i++)
	{
		TerminalColumnNumber = m_arHTerminalColumnsIndex[i];
		pRtfHorizontalColumn = m_arHorizontalColumns[TerminalColumnNumber];
		pRtfHorizontalColumn->m_PagePtr = this->m_PagePtr;
		pRtfHorizontalColumn->WriteTerminalColumnsTablesAndPictures(&SectorInfo);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 WriteTerminalColumns                                           //
////////////////////////////////////////////////////////////////////////////////////////////////////
void CRtfSector::WriteTerminalColumns(void)
{
	int32_t CountHTerminalColumns,CountVTerminalColumns,TerminalColumnNumber;
	CRtfHorizontalColumn *pRtfHorizontalColumn;

	CountHTerminalColumns  = m_arHTerminalColumnsIndex.size();
	m_VTerminalColumnNumber = 0;
	m_arRightBoundTerminalColumns.clear();
	m_arWidthTerminalColumns.clear();

	if(CountHTerminalColumns > 0)
	{
		CountVTerminalColumns = GetCountAndRightBoundVTerminalColumns();
		PutCom("\\cols",CountVTerminalColumns,0);
		int32_t len_colsx = 709;
		PutCom("\\colsx",len_colsx,1);
		Put("\\endnhere");

		for(int i=0; i<CountHTerminalColumns; i++)
		{
			TerminalColumnNumber = m_arHTerminalColumnsIndex[i];
			pRtfHorizontalColumn = m_arHorizontalColumns[TerminalColumnNumber];
			SectorInfo.VerticalOffsetColumnFromSector =(uint16_t)(pRtfHorizontalColumn->m_rect.top - m_rect.top);
			pRtfHorizontalColumn->m_PagePtr = this->m_PagePtr;
			pRtfHorizontalColumn->WriteTerminalColumns(&m_arRightBoundTerminalColumns, &m_VTerminalColumnNumber,
				                                          CountVTerminalColumns,&SectorInfo);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 GetCountAndRightBoundVTerminalColumns                          //
////////////////////////////////////////////////////////////////////////////////////////////////////
int32_t CRtfSector::GetCountAndRightBoundVTerminalColumns(void)
{
	int32_t CountHTerminalColumns,CountVTerminalColumns,TerminalColumnNumber;
	CRtfHorizontalColumn *pRtfHorizontalColumn;

	CountVTerminalColumns=0;
	CountHTerminalColumns  = m_arHTerminalColumnsIndex.size();
	for(int i=0; i<CountHTerminalColumns; i++)
	{
		TerminalColumnNumber   = m_arHTerminalColumnsIndex[i];
		pRtfHorizontalColumn   = m_arHorizontalColumns[TerminalColumnNumber];
		CountVTerminalColumns += pRtfHorizontalColumn->GetCountAndRightBoundVTerminalColumns(&m_arRightBoundTerminalColumns, &m_arWidthTerminalColumns);
	}
	return CountVTerminalColumns;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 WriteNonTerminalColumns                                        //
////////////////////////////////////////////////////////////////////////////////////////////////////
void CRtfSector::WriteNonTerminalColumns(void)
{
	CRtfHorizontalColumn *pRtfHorizontalColumn;

	SectorInfo.FlagFictiveParagraph = TRUE;

	m_wHorizontalColumnsCount = m_arHorizontalColumns.size();
	for(int i=0; i<m_wHorizontalColumnsCount; i++)
	{
		pRtfHorizontalColumn = m_arHorizontalColumns[i];
		if(pRtfHorizontalColumn->m_wType >= HC_AllFrame)
		pRtfHorizontalColumn->WriteNonTerminalColumns(&SectorInfo);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 ToPlacePicturesAndTables                                       //
////////////////////////////////////////////////////////////////////////////////////////////////////
void CRtfSector::ToPlacePicturesAndTables(CRtfFragment* pRtfFragment)
{
	CRtfHorizontalColumn *pRtfHorizontalColumn;

	m_wHorizontalColumnsCount = m_arHorizontalColumns.size();
	if(m_wHorizontalColumnsCount == 0)
	{
		m_arHorizontalColumns.push_back(new CRtfHorizontalColumn());
		pRtfHorizontalColumn=m_arHorizontalColumns.back();
		pRtfHorizontalColumn->ToPlacePicturesAndTables(pRtfFragment);
		return;
	}

	pRtfHorizontalColumn = m_arHorizontalColumns.back();
	if( pRtfFragment->m_rect.left >= pRtfHorizontalColumn->m_rectReal.right )
	{
		m_arHorizontalColumns.push_back(new CRtfHorizontalColumn());
		pRtfHorizontalColumn=m_arHorizontalColumns.back();
		pRtfHorizontalColumn->ToPlacePicturesAndTables(pRtfFragment);
		return;
	}

	for(int j=0; j<m_wHorizontalColumnsCount; j++ )
	{
		pRtfHorizontalColumn = m_arHorizontalColumns[j];
		if( pRtfFragment->m_rect.right <= pRtfHorizontalColumn->m_rectReal.left )
		{
			pRtfHorizontalColumn=*m_arHorizontalColumns.insert( m_arHorizontalColumns.begin()+j, new CRtfHorizontalColumn() );
			pRtfHorizontalColumn->ToPlacePicturesAndTables(pRtfFragment);
			return;
		}
		else
		{
			if((pRtfFragment->m_rect.left  <  pRtfHorizontalColumn->m_rectReal.left    &&
			 	pRtfFragment->m_rect.right >  pRtfHorizontalColumn->m_rectReal.left)   ||

				(pRtfFragment->m_rect.left  >= pRtfHorizontalColumn->m_rectReal.left   &&
			 	pRtfFragment->m_rect.left  <  pRtfHorizontalColumn->m_rectReal.right))
 			{
				pRtfHorizontalColumn->ToPlacePicturesAndTables(pRtfFragment);
				return;
			}
		}
	}

}
//~ расчет размеров сектора
////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 FillingSectorInfo                                              //
////////////////////////////////////////////////////////////////////////////////////////////////////
void CRtfSector::FillingSectorInfo()
{ //~ тут происходит работа си старой (vmk) структурой колонок
	CRtfHorizontalColumn *pRtfHorizontalColumn;
	CRtfVerticalColumn   *pRtfVerticalColumn;
	CRtfFragment         *pRtfFragment;
	int                   CountVColumn,CountFragments;

	m_wHorizontalColumnsCount = m_arHTerminalColumnsIndex.size();
	if(m_wHorizontalColumnsCount)
	{
		pRtfHorizontalColumn	= m_arHorizontalColumns[m_arHTerminalColumnsIndex[0]];
 		if(m_FlagOneString == FALSE)
			SectorInfo.Offset.x  = MAX(pRtfHorizontalColumn->m_rect.left,0);
	}

	m_wHorizontalColumnsCount = m_arHorizontalColumns.size();
	for(int i=0; i<m_wHorizontalColumnsCount; i++)
	{
		pRtfHorizontalColumn	= m_arHorizontalColumns[i];
		CountVColumn = pRtfHorizontalColumn->m_arVerticalColumns.size();
		for(int i1=0; i1<CountVColumn; i1++)
		{
			pRtfVerticalColumn	= pRtfHorizontalColumn->m_arVerticalColumns[i1];
			pRtfVerticalColumn->m_PagePtr = m_PagePtr;
			CountFragments = pRtfVerticalColumn->m_arFragments.size();
			for(int i2=0; i2<CountFragments; i2++)
			{
				SectorInfo.CountFragments++;
				pRtfFragment = pRtfVerticalColumn->m_arFragments[i2];

//!!!Art   if(pRtfFragment->m_wType == FT_FRAME || pRtfFragment->m_wType == FT_TEXT )
//!!!Art   SectorInfo.Offset.y = MIN(SectorInfo.Offset.y, pRtfFragment->m_rect.top);
			}
		}
	}

}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 CRTFHORIZONTALCOLUMN                                           //
////////////////////////////////////////////////////////////////////////////////////////////////////
CRtfHorizontalColumn::CRtfHorizontalColumn()
{
	m_wVerticalColumnsCount = 0;
	m_wType  = HC_SingleTerminal;
	SetRect(&m_rect, 32000, 32000, 0, 0 );
	SetRect(&m_rectReal, 32000, 32000, 0, 0 );
//	m_arVerticalColumns.RemoveAll();
//	m_arSpacePlace.RemoveAll();
//	m_arVTerminalColumnsGroup.RemoveAll();
// m_arVTerminalColumnsIndex.RemoveAll();
// m_arOrderingNumber.RemoveAll();
}

CRtfHorizontalColumn::~CRtfHorizontalColumn()
{
	CRtfVerticalColumn* cVerticalColumn;
	uint16_t Count;
	int i;
	m_wVerticalColumnsCount = m_arVerticalColumns.size();
	for(i=0; i<m_wVerticalColumnsCount; i++ )
	{
		cVerticalColumn = m_arVerticalColumns[i];
		delete cVerticalColumn;
	}
//	m_arVerticalColumns.RemoveAll();
// m_arSpacePlace.RemoveAll();

	Count = m_arVTerminalColumnsGroup.size();
	for( i=0; i<Count; i++ )
	{
		vectorWord* pGroup = m_arVTerminalColumnsGroup[i];
		delete pGroup;
	}
//	m_arVTerminalColumnsGroup.RemoveAll();

	Count = m_arVTerminalColumnsIndex.size();
	for( i=0; i<Count; i++ )
	{
		vectorWord* pGroupIndex = m_arVTerminalColumnsIndex[i];
		delete pGroupIndex;
	}

// m_arVTerminalColumnsIndex.RemoveAll();
// m_arOrderingNumber.RemoveAll();
}
//~ здесь попытка деления на колонки
////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 CalcHorizontalColumn                                           //
////////////////////////////////////////////////////////////////////////////////////////////////////
void CRtfHorizontalColumn::CalcHorizontalColumn(void)
{

	if( m_wType == HC_FrameAndColumn || m_wType == HC_AllFrame )
	{
		if(CheckTermColumn() == FALSE) //~проверка вертикал. затенения колонок ("жертва" будет frame)
		{
   FindHeadingAndSetFrameFlag(); //это проверка, что после удаления жертвы все стало лучше
   DefineTerminalProperty(); //присвоение признака терминальности колонкам
		}
  else
   m_wType = HC_AllTerminal;
	}

	if(m_wType <= HC_FrameAndColumn)
        FillingVTerminalColumnsIndex(); //есть хорошие колонки
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 CheckTermColumn                                                //
////////////////////////////////////////////////////////////////////////////////////////////////////
Bool CRtfHorizontalColumn::CheckTermColumn(void)
{
	CRtfVerticalColumn *pRtfVerticalColumn,*pRtfPrevVerticalColumn;
	int i;

	m_wVerticalColumnsCount = m_arVerticalColumns.size();
	for(i=0; i<m_wVerticalColumnsCount; i++)
	{
		if(i)
		{
	  pRtfVerticalColumn     = (CRtfVerticalColumn*)m_arVerticalColumns[i];
			pRtfPrevVerticalColumn = (CRtfVerticalColumn*)m_arVerticalColumns[i-1];
   if(pRtfVerticalColumn->m_rectReal.top < pRtfPrevVerticalColumn->m_rectReal.bottom)
    return FALSE;
		}
 }

return TRUE;
}

//после удаления жертвы по гистограммам проверяем разделимость остальных колонок и если да,
// все они будут терминал.

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 FindHeadingAndSetFrameFlag                                     //
////////////////////////////////////////////////////////////////////////////////////////////////////
void CRtfHorizontalColumn::FindHeadingAndSetFrameFlag(void)
{
	CRtfVerticalColumn* pRtfVerticalColumn;
	int32_t               TmpBottom = 32000;
	std::vector<uchar>          Hist;
	int32_t               Left=32000,Right=0,Length,Left1,Right1;
	int                 i,j,FlagLeft,FlagRight,tmp;
	int32_t                MaxWidth=1,MaxHeight=1;

//	Hist.RemoveAll();
	m_wVerticalColumnsCount = m_arVerticalColumns.size();

	for(i=0; i<m_wVerticalColumnsCount; i++)
	{
		pRtfVerticalColumn = (CRtfVerticalColumn*)m_arVerticalColumns[i];
		if(pRtfVerticalColumn->m_wType == FT_TEXT || pRtfVerticalColumn->m_wType == FT_FRAME)
		{
			MaxWidth  = MAX(MaxWidth,  pRtfVerticalColumn->m_rectReal.right  - pRtfVerticalColumn->m_rectReal.left);
			MaxHeight = MAX(MaxHeight, pRtfVerticalColumn->m_rectReal.bottom - pRtfVerticalColumn->m_rectReal.top );
			pRtfVerticalColumn->m_bFlagSmall = FALSE; //в первую очередь жертвы ищутся среди малых
		}
	}

	for(i=0; i<m_wVerticalColumnsCount; i++)
	{
		pRtfVerticalColumn = (CRtfVerticalColumn*)m_arVerticalColumns[i];
		if(pRtfVerticalColumn->m_wType == FT_TEXT || pRtfVerticalColumn->m_wType == FT_FRAME)
		{
			if(((pRtfVerticalColumn->m_rectReal.right  - pRtfVerticalColumn->m_rectReal.left)*2<MaxWidth) &&
			((pRtfVerticalColumn->m_rectReal.bottom - pRtfVerticalColumn->m_rectReal.top )*2<MaxHeight))
				pRtfVerticalColumn->m_bFlagSmall = TRUE;
		}
	}

	for(i=0; i<m_wVerticalColumnsCount; i++)
	{
		pRtfVerticalColumn = (CRtfVerticalColumn*)m_arVerticalColumns[i];
		if((pRtfVerticalColumn->m_wType == FT_TEXT || pRtfVerticalColumn->m_wType == FT_FRAME) &&
			   pRtfVerticalColumn->m_bFlagSmall == FALSE)
		{
			Left  = MIN(Left,  pRtfVerticalColumn->m_rectReal.left);
			Right = MAX(Right, pRtfVerticalColumn->m_rectReal.right);
		}
	}
	Length = Right - Left + 2;

	for(i=0; i<Length; i++)
		Hist.push_back(0);

	for(i=0; i<m_wVerticalColumnsCount; i++)//~накопление гистограммы
	{
		pRtfVerticalColumn = (CRtfVerticalColumn*)m_arVerticalColumns[i];
		if((pRtfVerticalColumn->m_wType == FT_TEXT || pRtfVerticalColumn->m_wType == FT_FRAME) &&
			   pRtfVerticalColumn->m_bFlagSmall == FALSE)
		{
			Left1  = pRtfVerticalColumn->m_rectReal.left - Left;
			Right1 = pRtfVerticalColumn->m_rectReal.right- Left;
			for(j=Left1; j<Right1; j++)
				Hist[j] += 1;
		}
	}

	for(i=0; i<m_wVerticalColumnsCount; i++)
	{
	 pRtfVerticalColumn = (CRtfVerticalColumn*)m_arVerticalColumns[i];
		if(pRtfVerticalColumn->m_bFlagSmall)
		{
			pRtfVerticalColumn->m_wType = FT_FRAME;
			continue;
		}
		if(pRtfVerticalColumn->m_wType == FT_TEXT || pRtfVerticalColumn->m_wType == FT_FRAME)
		{

		 Left1  = pRtfVerticalColumn->m_rectReal.left - Left;
		 Right1 = pRtfVerticalColumn->m_rectReal.right- Left;

		FlagLeft=0; FlagRight =0;
		tmp=Hist[Left1];

			for(j=Left1; j<Right1; j++)
			{
    if( tmp < Hist[j] )
				{
				 if(FlagLeft==0)
				  tmp = Hist[j];
					else
						if(FlagLeft==1 && FlagRight==0)
						{
	 			  FlagRight=1;
							break;
						}
				}
    else
    if( tmp > Hist[j] )
				{
				 if(FlagLeft==0)
					{
				  tmp = Hist[j];
      FlagLeft=1;
					}
					else
						if(FlagLeft==1 && FlagRight==0)
 				  tmp = Hist[j];
				}
			}

	  if(FlagRight)
			 pRtfVerticalColumn->m_wType = FT_FRAME;
		 else
			 pRtfVerticalColumn->m_wType = FT_TEXT;

		}
	}
//	Hist.RemoveAll();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 DefineTerminalProperty                                         //
////////////////////////////////////////////////////////////////////////////////////////////////////
void CRtfHorizontalColumn::DefineTerminalProperty(void)
{ //~ recalculation of histogram after victim deletion
	std::vector<uchar>    Hist;
	vectorWord*   pGroup;
	CRtfVerticalColumn* pRtfVerticalColumn;
	int32_t Left=32000,Right=0,Length,Left1,Right1;
	int i,j,FlagSpace=0,CountColumn,MinLeft,MaxRight;

	m_wVerticalColumnsCount = m_arVerticalColumns.size();
	for(i=0; i<m_wVerticalColumnsCount; i++)
	{
	 pRtfVerticalColumn = (CRtfVerticalColumn*)m_arVerticalColumns[i];
  if(pRtfVerticalColumn->m_wType == FT_TEXT)
		{
		 Left  = MIN(Left,  pRtfVerticalColumn->m_rectReal.left);
		 Right = MAX(Right, pRtfVerticalColumn->m_rectReal.right);
		}
	}

	MinLeft = Left;
	MaxRight= Right;
 Length  = Right - Left;

	for(i=0; i<Length; i++)
		Hist.push_back(0);

	for(i=0; i<m_wVerticalColumnsCount; i++)
	{
		pRtfVerticalColumn = (CRtfVerticalColumn*)m_arVerticalColumns[i];
		if(pRtfVerticalColumn->m_wType == FT_TEXT)
		{
			Left1  = pRtfVerticalColumn->m_rectReal.left - Left;
			Right1 = pRtfVerticalColumn->m_rectReal.right- Left;
			for(j=Left1; j<Right1; j++)
				Hist[j] +=1;
		}
	}

	for(j=0; j<Length; j++)
	{
		if(Hist[j] == 0)
		{
			if(!FlagSpace)
			{
				FlagSpace = 1;
				m_arSpacePlace.push_back(j);
			}
		}
	 else
		{
	  if(FlagSpace)
		  FlagSpace = 0;
		}
	}

	CountColumn = m_arSpacePlace.size();
	if(CountColumn)
	{
		for(i=0; i<= CountColumn; i++)
		{
			m_arVTerminalColumnsGroup.push_back(new vectorWord());
			pGroup = (vectorWord* )m_arVTerminalColumnsGroup[i];
			if(i==0)
			{
				Left  = MinLeft;
				Right = MinLeft + m_arSpacePlace[i];
			}
			else
			if(i==CountColumn)
			{
 				Left  = MinLeft + m_arSpacePlace[i-1];
	 			Right = MaxRight;
			}
 			else
	 		{
		 		Left  = MinLeft + m_arSpacePlace[i-1];
				Right = MinLeft + m_arSpacePlace[i];
			}

			for(int j=0; j<m_wVerticalColumnsCount; j++)
			{
 				pRtfVerticalColumn = (CRtfVerticalColumn*)m_arVerticalColumns[j];
				if(pRtfVerticalColumn->m_wType == FT_TEXT && pRtfVerticalColumn->m_rectReal.left >= Left &&
							pRtfVerticalColumn->m_rectReal.right <= Right)
 				{
   					pGroup->push_back(j); //~слияние секторов в одну колонку по вертикали
				}
			}
		}
	}

//	Hist.RemoveAll();

	if(m_arSpacePlace.size())
	{
		m_wType = HC_FrameAndColumn;
			return;
	}
//~неудача деления => all columns become frames
	for(i=0; i<m_wVerticalColumnsCount; i++)
	{
	 pRtfVerticalColumn = (CRtfVerticalColumn*)m_arVerticalColumns[i];
  if(pRtfVerticalColumn->m_wType == FT_TEXT)
   pRtfVerticalColumn->m_wType = FT_FRAME;
	}

 int IndexMaxWidthFragment=0,MaxWidth=0;

	for(i=0; i<m_wVerticalColumnsCount; i++)
	{
	 pRtfVerticalColumn = (CRtfVerticalColumn*)m_arVerticalColumns[i];
  if(pRtfVerticalColumn->m_wType == FT_FRAME)
		{
			if((pRtfVerticalColumn->m_rectReal.right-pRtfVerticalColumn->m_rectReal.left)> MaxWidth)
			{
				MaxWidth = pRtfVerticalColumn->m_rectReal.right-pRtfVerticalColumn->m_rectReal.left;
    IndexMaxWidthFragment=i;
			}
		}
	}

	pRtfVerticalColumn = (CRtfVerticalColumn*)m_arVerticalColumns[IndexMaxWidthFragment];
	pRtfVerticalColumn->m_wType = FT_TEXT;
	m_arVTerminalColumnsGroup.push_back(new vectorWord());
	pGroup = m_arVTerminalColumnsGroup[0];
	pGroup->push_back(IndexMaxWidthFragment);

	m_wType = HC_FrameAndColumn;
}

//resorting of the fragment array (at first position should be located Frames, next ones are terminal columns)

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 FillingVTerminalColumnsIndex                                   //
////////////////////////////////////////////////////////////////////////////////////////////////////
void CRtfHorizontalColumn::FillingVTerminalColumnsIndex(void)
{
	int                 i,j,FlagChange;
	uint16_t		              index,Number;
	int32_t               Top;
	CRtfVerticalColumn *pRtfVerticalColumn;
	vectorWord         *pGroup,*pGroupNew;

	if(m_wType == HC_SingleTerminal)
	{
		m_arVTerminalColumnsIndex.push_back(new vectorWord());
		pGroup = m_arVTerminalColumnsIndex[0];
		pGroup->push_back(0);
	}

	if(m_wType == HC_AllTerminal) //section includes only terminal columns (! NO frames)
	{
		m_arVTerminalColumnsIndex.push_back(new vectorWord());
		pGroup = m_arVTerminalColumnsIndex[0];

		m_wVerticalColumnsCount = m_arVerticalColumns.size();
		for(j=0; j<m_wVerticalColumnsCount; j++)
		{
			FlagChange=0;
			Top = 320000;
			for(i=0; i<m_wVerticalColumnsCount; i++)
			{
 			pRtfVerticalColumn = (CRtfVerticalColumn*)m_arVerticalColumns[i];
				if(pRtfVerticalColumn->m_wType==FT_FRAME || pRtfVerticalColumn->m_bSortFlag ==1)
					continue;
				if(pRtfVerticalColumn->m_rectReal.top < Top)
				{
					Top = pRtfVerticalColumn->m_rectReal.top;
					index=i;
					FlagChange=1;
				}
			}
			if(FlagChange)
			{
				pGroup->push_back(index);
				pRtfVerticalColumn = (CRtfVerticalColumn*)m_arVerticalColumns[index];
				pRtfVerticalColumn->m_bSortFlag=1;
			}
		}
	}

	if(m_wType == HC_FrameAndColumn)
	{
		int CountColumn = m_arVTerminalColumnsGroup.size();

 		for(j=0; j<CountColumn; j++)
		{
			m_arVTerminalColumnsIndex.push_back(new vectorWord());
			pGroupNew = m_arVTerminalColumnsIndex[j];

 			pGroup = m_arVTerminalColumnsGroup[j];
			int CountInGroup = pGroup->size();
			for(i=0; i<CountInGroup;i++)
			{
				FlagChange=0;
				Top = 320000;
				for(int i1=0; i1<CountInGroup;i1++)
				{
					Number = (*pGroup)[i1];
  					pRtfVerticalColumn = (CRtfVerticalColumn*)m_arVerticalColumns[Number];
 					if(pRtfVerticalColumn->m_wType==FT_FRAME || pRtfVerticalColumn->m_bSortFlag ==1)
	 					continue;
					if(pRtfVerticalColumn->m_rectReal.top < Top)
					{
						Top = pRtfVerticalColumn->m_rectReal.top;
						index=Number;
	 					FlagChange=1;
					}
				}
				if(FlagChange)
				{
					pGroupNew->push_back(index);
					pRtfVerticalColumn = (CRtfVerticalColumn*)m_arVerticalColumns[index];
					pRtfVerticalColumn->m_bSortFlag=1;
				}
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 GetCountAndRightBoundVTerminalColumns                          //
////////////////////////////////////////////////////////////////////////////////////////////////////
int32_t CRtfHorizontalColumn::GetCountAndRightBoundVTerminalColumns(vectorWord* arRightBoundTerminalColumns,
														vectorWord* arWidthTerminalColumns)
{
	int32_t               CountVTerminalColumns=0;
	CRtfVerticalColumn *pRtfVerticalColumn;
	uint16_t                RightBound,index,WidthColumn,tmp;
	vectorWord         *pGroup;

	if(m_wType==HC_SingleTerminal || m_wType==HC_AllTerminal)
	{
		RightBound = (uint16_t)MAX(m_rectReal.left,0);
		WidthColumn = (uint16_t)(m_rectReal.right - m_rectReal.left);
		arRightBoundTerminalColumns->push_back(RightBound);
		arWidthTerminalColumns->push_back(WidthColumn);
		CountVTerminalColumns = 1;
		return CountVTerminalColumns;
	}

	if(m_wType==HC_FrameAndColumn)
	{
		CountVTerminalColumns = m_arVTerminalColumnsIndex.size();
		for(int i=0; i<CountVTerminalColumns; i++)
		{
 			pGroup = m_arVTerminalColumnsIndex[i];
 			int CountInGroup = pGroup->size();
			RightBound  = 32000;
			WidthColumn = 0;
			for(int j=0; j<CountInGroup; j++ )
			{
				index = (*pGroup)[j];
				pRtfVerticalColumn = (CRtfVerticalColumn*)m_arVerticalColumns[index];
				tmp = MAX(pRtfVerticalColumn->m_rectReal.left,0);
				RightBound = MIN(RightBound,tmp);
  				WidthColumn = MAX(WidthColumn,
				               (uint16_t)(pRtfVerticalColumn->m_rectReal.right - pRtfVerticalColumn->m_rectReal.left));
			}
			arRightBoundTerminalColumns->push_back(RightBound);
			arWidthTerminalColumns->push_back(WidthColumn);
		}
	}

 return CountVTerminalColumns;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 WriteTerminalColumnsTablesAndPictures                          //
////////////////////////////////////////////////////////////////////////////////////////////////////
void CRtfHorizontalColumn::WriteTerminalColumnsTablesAndPictures( RtfSectorInfo *SectorInfo )
{
	CRtfVerticalColumn* pRtfVerticalColumn;
	CRtfFragment*       pRtfFragment;

	int CountFrameInTerminalColumn = m_arVerticalColumns.size();
	for(int i=0; i<CountFrameInTerminalColumn; i++)
	{
		pRtfVerticalColumn = m_arVerticalColumns[i];
		pRtfFragment       = pRtfVerticalColumn->m_arFragments[0];
		if(pRtfFragment->m_wType == FT_TABLE || pRtfFragment->m_wType == FT_PICTURE )
		{
			if(m_wType<=HC_AllTerminal)
			{
				pRtfFragment->m_bFlagObjectInColumn       = TRUE;
				pRtfVerticalColumn->m_bFlagObjectInColumn = TRUE;
				SetFlagObjectInColumnForPageFragment(pRtfFragment);
			}
			else
			{
				SectorInfo->FlagInColumn                  = FALSE;
				pRtfFragment->m_bFlagObjectInColumn       = FALSE;
				pRtfVerticalColumn->m_bFlagObjectInColumn = FALSE;
				pRtfVerticalColumn->Write(FOT_SINGLE, SectorInfo);
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 SetFlagObjectInColumnForPageFragment                          //
////////////////////////////////////////////////////////////////////////////////////////////////////
void CRtfHorizontalColumn::SetFlagObjectInColumnForPageFragment( CRtfFragment* CurFragment )
{
	CRtfFragment*       pRtfFragment;

	int CountFragments = m_PagePtr->m_arFragments.size();
 	for(int i=0; i<CountFragments; i++ )
	{
		pRtfFragment = m_PagePtr->m_arFragments[i];

   if( pRtfFragment->m_wType == CurFragment->m_wType  &&
       pRtfFragment->m_rect.left == CurFragment->m_rect.left   && pRtfFragment->m_rect.right  == CurFragment->m_rect.right &&
       pRtfFragment->m_rect.top  == CurFragment->m_rect.top    && pRtfFragment->m_rect.bottom == CurFragment->m_rect.bottom )
				pRtfFragment->m_bFlagObjectInColumn = TRUE;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 WriteTerminalColumns                                           //
////////////////////////////////////////////////////////////////////////////////////////////////////
void CRtfHorizontalColumn::WriteTerminalColumns(vectorWord* arRightBoundTerminalColumns,
													int32_t* VTerminalColumnNumber,
													int32_t  CountVTerminalColumns,
													RtfSectorInfo *SectorInfo)
{
	int                 colsr(0),i(0),j(0),colw(0),CountInGroup(0);
	int32_t               CountTerminalColumns,NextColumnsLeft,CountFrameInTerminalColumn,Left,Right;
	CRtfVerticalColumn *pRtfVerticalColumn;
	CRtfFragment       *pRtfFragment;
	vectorWord         *pGroup;
	uint16_t                FreeSpace;
	int                 number;
	Bool                FlagFirstTerminalFragment = FALSE;
	int32_t               TopPositionFirstTerminalFragment;
	RECT               Rect;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//***********************        Tерминальная колонка из одного или нескольких фрагментов  ******************************************
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if(m_wType==HC_SingleTerminal || m_wType==HC_AllTerminal)
	{
		*VTerminalColumnNumber+=1;

		if(FlagMode&USE_FRAME_AND_COLUMN && *VTerminalColumnNumber==1 && CountVTerminalColumns>1)
		{
			Rect.top    = m_rectReal.top;
			Rect.bottom = m_rectReal.bottom;
			Rect.left   = m_rectReal.right;
			Rect.right  = (*arRightBoundTerminalColumns)[*VTerminalColumnNumber];
			if( CheckLines( &Rect,TRUE,SectorInfo ))
			{
  				if(RtfWriteMode)
  					Put("\\linebetcol");
				else
				CED_SetSectLineBetCol( SectorInfo->hEDSector, TRUE );
			}
		}

		PutCom("\\colno",*VTerminalColumnNumber,1);

		if( FlagMode & USE_FRAME_AND_COLUMN && SectorInfo->FlagOneString == TRUE )
			colw = MAX(0,SectorInfo->PaperW - (SectorInfo->MargL + SectorInfo->MargR));
		else
			colw = m_rectReal.right - m_rectReal.left;

		PutCom("\\colw",colw,1);
		colsr = -1;

		if(CountVTerminalColumns && *VTerminalColumnNumber<CountVTerminalColumns)
		{
			NextColumnsLeft = (*arRightBoundTerminalColumns)[*VTerminalColumnNumber];
			colsr = NextColumnsLeft - m_rectReal.right;
			PutCom("\\colsr",colsr,1);
		}

	#ifdef EdWrite
		if(!RtfWriteMode)
		{
			if(*VTerminalColumnNumber == 1)
				SectorInfo->hColumn = SectorInfo->hFirstColumn;
			else
				SectorInfo->hColumn = CED_CreateColumn( SectorInfo->hEDSector);

			SectorInfo->hObject = SectorInfo->hColumn;
		}
	#endif

	CountFrameInTerminalColumn = m_arVerticalColumns.size();
	SortFragments();

	for(i=0; i<CountFrameInTerminalColumn; i++)
	{
		number = i;
		if(m_arOrderingNumber.size())
			number = m_arOrderingNumber[i];
		pRtfVerticalColumn   = m_arVerticalColumns[number];
		pRtfFragment         = pRtfVerticalColumn->m_arFragments[0];

		FreeSpace = GetFreeSpaceBetweenPrevAndCurrentFragments(pRtfFragment->m_rect.top,SectorInfo);
		SectorInfo->VerticalOffsetFragmentInColumn = FreeSpace;

		if(pRtfFragment->m_wType != FT_TABLE &&  pRtfFragment->m_wType != FT_PICTURE )
		{//Text
			pRtfFragment->m_LeftOffsetFragmentFromVerticalColumn =
				pRtfFragment->m_rect.left - m_rect.left;

			pRtfFragment->m_RightOffsetFragmentFromVerticalColumn =
				m_rect.right - pRtfFragment->m_rect.right;

			pRtfFragment->m_WidthVerticalColumn = int16_t(m_rect.right - m_rect.left);
#ifdef EdWrite
			if(!RtfWriteMode)
				SectorInfo->hObject = SectorInfo->hColumn;
#endif

			 SectorInfo->FlagOverLayed=GetOverLayedFlag(i);
			pRtfVerticalColumn->Write( FOT_SINGLE, SectorInfo );
		}
		else
		{
			if((pRtfFragment->m_wType==FT_TABLE || pRtfFragment->m_wType==FT_PICTURE) && pRtfFragment->m_bFlagObjectInColumn)
			{//Picture,Table
				SectorInfo->FlagInColumn = TRUE;
				if (!SectorInfo->FlagOneString)//!!!Art
					SectorInfo->OffsetFromColumn.x = pRtfFragment->m_rect.left - m_rect.left;
				else//!!!Art
					SectorInfo->OffsetFromColumn.x = pRtfFragment->m_rect.left - SectorInfo->MargL;//!!!Art

				SectorInfo->OffsetFromColumn.y = pRtfFragment->m_wOffsetFromPrevTextFragment;
#ifdef EdWrite
				if(!RtfWriteMode)
					SectorInfo->hObject = SectorInfo->hColumn;
#endif
 				pRtfVerticalColumn->Write(FOT_SINGLE, SectorInfo);
			}
		}
	}

	if(*VTerminalColumnNumber<CountVTerminalColumns)
		Put("\\column");
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//***********************************                    Фреймы и колонки            ************************************************
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	else
	{
		CountTerminalColumns  = m_arVTerminalColumnsIndex.size();

		for(i=0; i<CountTerminalColumns; i++)
		{
			*VTerminalColumnNumber += 1;
			PutCom("\\colno",*VTerminalColumnNumber,1);
			Left = 32000; Right=0;

 			pGroup = m_arVTerminalColumnsIndex[i];
 			CountInGroup = pGroup->size();
			for(j=0; j<CountInGroup; j++ ) //~ V-columns in one H-col
			{
				int index = (*pGroup)[j];
				pRtfVerticalColumn = (CRtfVerticalColumn*)m_arVerticalColumns[index];

				if(i==0 && j==0)
				{
					FlagFirstTerminalFragment        = TRUE;
					TopPositionFirstTerminalFragment = pRtfVerticalColumn->m_rectReal.top;
				}
    Left  = MIN(Left,  pRtfVerticalColumn->m_rectReal.left);
				Right = MAX(Right, pRtfVerticalColumn->m_rectReal.right);
			}

			colw = Right - Left;
			PutCom("\\colw",colw,1);
			colsr = -1;
			if(*VTerminalColumnNumber<CountVTerminalColumns)
			{
				NextColumnsLeft = (*arRightBoundTerminalColumns)[*VTerminalColumnNumber];
				colsr = NextColumnsLeft - Right;
				PutCom("\\colsr",colsr,1);
			}

#ifdef EdWrite
	if(!RtfWriteMode)
	{
  if(*VTerminalColumnNumber == 1)
   SectorInfo->hColumn = SectorInfo->hFirstColumn;
		else
   SectorInfo->hColumn = CED_CreateColumn( SectorInfo->hEDSector);

  SectorInfo->hObject = SectorInfo->hColumn;
	}
#endif

	if(!i) //noisy fragment or picture are made as frame,frames привязаны к первой терминал. колонке сектора
 	  WriteFramesInTerminalColumn(SectorInfo, FlagFirstTerminalFragment, TopPositionFirstTerminalFragment);

	for(j=0; j<CountInGroup; j++ )
	{
		int index = (*pGroup)[j];
		pRtfVerticalColumn = (CRtfVerticalColumn*)m_arVerticalColumns[index];
 		FreeSpace = GetFreeSpaceBetweenPrevAndCurrentFragments(pRtfVerticalColumn->m_rect.top,SectorInfo);
		SectorInfo->VerticalOffsetFragmentInColumn = FreeSpace;

#ifdef EdWrite
		if(!RtfWriteMode)
			SectorInfo->hObject = SectorInfo->hColumn;
#endif

			 SectorInfo->FlagOverLayed=GetOverLayedFlag(index);
 			pRtfVerticalColumn->Write(FOT_SINGLE, SectorInfo);
			}

			if(*VTerminalColumnNumber<CountVTerminalColumns)
 			Put("\\column");
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                       GetFreeSpaceBetweenPrevAndCurrentFragments                               //
////////////////////////////////////////////////////////////////////////////////////////////////////
uint16_t CRtfHorizontalColumn::GetFreeSpaceBetweenPrevAndCurrentFragments(int TopPosCurFragment, RtfSectorInfo *SectorInfo)
{
	CRtfFragment       *pRtfFragment;
	RECT              FreePlace;
	int                i,CountFragments;
	int32_t              FreePlaceHeight;

	FreePlace.left     = m_rect.left;
	FreePlace.right    = m_rect.right;
	FreePlace.bottom   = TopPosCurFragment - 1;
	FreePlace.top      = MAX(0, m_rect.top - SectorInfo->VerticalOffsetColumnFromSector);
	if(FreePlace.top  >= FreePlace.bottom)
		return 0;

	CountFragments = m_PagePtr->m_arFragments.size();
 	for( i=0; i<CountFragments; i++ )
	{
		pRtfFragment = m_PagePtr->m_arFragments[i];

		if( pRtfFragment->m_rect.bottom <= FreePlace.top    || pRtfFragment->m_rect.right <= FreePlace.left ||
				   pRtfFragment->m_rect.top    >= FreePlace.bottom || pRtfFragment->m_rect.left  >= FreePlace.right )
			continue;

		if(pRtfFragment->m_rect.bottom >= FreePlace.top && pRtfFragment->m_rect.bottom <= FreePlace.bottom)
			FreePlace.top = pRtfFragment->m_rect.bottom;
	}
  FreePlaceHeight = MAX(0,FreePlace.bottom - FreePlace.top);

		return (uint16_t)FreePlaceHeight;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                          GetOverLayedFlag                                      //
////////////////////////////////////////////////////////////////////////////////////////////////////
Bool CRtfHorizontalColumn::GetOverLayedFlag(int CurFragmentNumber)
{
	CRtfVerticalColumn *pRtfVerticalColumn;
	CRtfFragment       *pRtfFragment;
	RECT              CurFragmentRect;
	int                i,number,CountFragments;
	POINT pt;

	number = CurFragmentNumber;
	if(m_arOrderingNumber.size())
		number = m_arOrderingNumber[CurFragmentNumber];
	pRtfVerticalColumn = m_arVerticalColumns[number];
	pRtfFragment       = pRtfVerticalColumn->m_arFragments[0];

	CurFragmentRect.left     = pRtfFragment->m_rect.left;
	CurFragmentRect.right    = pRtfFragment->m_rect.right;
	CurFragmentRect.bottom   = pRtfFragment->m_rect.bottom;
	CurFragmentRect.top      = pRtfFragment->m_rect.top;

	CountFragments = m_PagePtr->m_arFragments.size();
 	for( i=0; i<CountFragments; i++ )
	{
		pRtfFragment = m_PagePtr->m_arFragments[i];

		if(!pRtfFragment->m_wType)
			continue;
		pt.x = pRtfFragment->m_rect.left;
		pt.y = pRtfFragment->m_rect.top;
		if(PtInRect(&CurFragmentRect,pt))
			return TRUE;

		pt.x = pRtfFragment->m_rect.right;
		if(PtInRect(&CurFragmentRect,pt))
			return TRUE;

		pt.x = pRtfFragment->m_rect.left;
		pt.y = pRtfFragment->m_rect.bottom;
		if(PtInRect(&CurFragmentRect,pt))
			return TRUE;

		pt.x = pRtfFragment->m_rect.right;
		if(PtInRect(&CurFragmentRect,pt))
			return TRUE;
	}

	return FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                               SortFragments                                    //
////////////////////////////////////////////////////////////////////////////////////////////////////
void CRtfHorizontalColumn::SortFragments()
{
	int32_t               CountFrameInTerminalColumn;
	CRtfVerticalColumn *pRtfVerticalColumn;
	CRtfFragment       *pRtfFragment,*pRtfFragmentFirst;
	int                 /*NextTop=-32000,*/size,number;
	Bool                FlagInserted = FALSE;

	CountFrameInTerminalColumn = m_arVerticalColumns.size();
	for(int i=0; i<CountFrameInTerminalColumn; i++)
	{
		pRtfVerticalColumn = m_arVerticalColumns[i];
		pRtfFragment       = pRtfVerticalColumn->m_arFragments[0];
		FlagInserted       = FALSE;

		if( i==0 )
			m_arOrderingNumber.push_back(i);
		else
		{
			size =m_arOrderingNumber.size();
			int m;
			for(m=0; m<size; m++ )
			{
				number=m_arOrderingNumber[m];
				pRtfVerticalColumn = m_arVerticalColumns[number];
				pRtfFragmentFirst   = pRtfVerticalColumn->m_arFragments[0];

				//Если фрагмент выше другого
				if( pRtfFragment->m_rect.top < pRtfFragmentFirst->m_rect.top )
				{
  					m_arOrderingNumber.insert( m_arOrderingNumber.begin()+m, i );
					if(pRtfFragment->m_wType==FT_TABLE || pRtfFragment->m_wType==FT_PICTURE)
						  pRtfFragment->m_wOffsetFromPrevTextFragment =
							  GetOffsetFromPrevTextFragment( pRtfFragment );
						FlagInserted = TRUE;
						break;
				}
				else
					//Если таблица/картинка покрывается текстовым фрагментом
				 if( (pRtfFragment->m_wType==FT_TABLE || pRtfFragment->m_wType==FT_PICTURE) &&
								 pRtfFragmentFirst->m_wType==FT_TEXT                         &&
					pRtfFragment->m_rect.top >= pRtfFragmentFirst->m_rect.top               &&
									pRtfFragment->m_rect.top < pRtfFragmentFirst->m_rect.bottom)
					{
   						m_arOrderingNumber.insert(m_arOrderingNumber.begin()+ m, i );
						pRtfFragment->m_wOffsetFromPrevTextFragment = (uint16_t)(pRtfFragment->m_rect.top - pRtfFragmentFirst->m_rect.top);
						FlagInserted = TRUE;
						break;
					}
			}
			if(FlagInserted == FALSE)
			{
 				m_arOrderingNumber.push_back(m);
			}
		}
	}
	//Надо оттестировать
/*	//Выставляем расстояние для картинок и таблиц, которые пойдут
	//во фреймы после последнго текстового фрагмента
	for( int m=m_arOrderingNumber.size()-1; m>=0; m-- )
	{
		pRtfVerticalColumn = m_arVerticalColumns[m_arOrderingNumber[m]];
		pRtfFragmentFirst   = pRtfVerticalColumn->m_arFragments[0];
		if (pRtfFragmentFirst->m_wType==FT_TEXT)
			break;
	}
	if (m<0)
		return;
	//нашли последний текстовый фрагмент - выставляем до него расстояние
	int lastTxtNum=m;
	//Ищем высоту фиктивного абзаца+разрыва колонки/раздела
	int16_t parHeight=0,brkHeight=0;
	GetRealSize("A",0,6,0,&parHeight);
	GetRealSize("A",0,24,0,&brkHeight);
	parHeight/=Twips;
	brkHeight/=Twips;
	for( m=m_arOrderingNumber.size()-1; m>lastTxtNum; m-- )
	{
		pRtfVerticalColumn = m_arVerticalColumns[m_arOrderingNumber[m]];
		pRtfFragment   = pRtfVerticalColumn->m_arFragments[0];
		if( (pRtfFragment->m_wType==FT_TABLE || pRtfFragment->m_wType==FT_PICTURE))
		{
			//вычитаем 2 высоты - фикт. абзаца и разрыва
			//если меньше, чем высота разрава, то чтобы он не упрыгнул вниз
			pRtfFragment->m_wOffsetFromPrevTextFragment = (uint16_t)MAX(brkHeight,pRtfFragment->m_rect.top - pRtfFragmentFirst->m_rect.bottom-brkHeight-parHeight);
		}
	}
*/}

uint16_t CRtfHorizontalColumn::GetOffsetFromPrevTextFragment(CRtfFragment *pRtfFragment)
{
	int32_t               CountFrameInTerminalColumn;
	CRtfVerticalColumn *pRtfVerticalColumn;
	CRtfFragment       *pRtfFragmentNext;
	uint16_t                VerOffset=0;
	CountFrameInTerminalColumn = m_arVerticalColumns.size();
	for(int i=0; i<CountFrameInTerminalColumn; i++)
	{
		pRtfVerticalColumn = m_arVerticalColumns[i];
		pRtfFragmentNext   = pRtfVerticalColumn->m_arFragments[0];

		if(	pRtfFragmentNext->m_wType == FT_TEXT                                &&
			pRtfFragment->m_rect.top  >= pRtfFragmentNext->m_rect.top           &&
							pRtfFragment->m_rect.top  <  pRtfFragmentNext->m_rect.bottom)
			VerOffset = (uint16_t)(pRtfFragment->m_rect.top - pRtfFragmentNext->m_rect.top);
	}
	return VerOffset;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 WriteFramesInTerminalColumn                                    //
////////////////////////////////////////////////////////////////////////////////////////////////////
void CRtfHorizontalColumn::WriteFramesInTerminalColumn(RtfSectorInfo* SectorInfo,Bool FlagFirstTerminalFragment,int32_t TopPositionFirstTerminalFragment)
{
	CRtfVerticalColumn* pRtfVerticalColumn;
	int32_t shpleft,shptop,shpright,shpbottom,shpwr=0,fri=0;

#ifdef EdWrite
 EDBOX EdFragmRect;

	Handle     	hParagraph=NULL;
	Handle     	hString=NULL;
 int         align;
	EDRECT      indent;
 EDSIZE      interval;
	EDBOX       playout;
#endif

	m_wVerticalColumnsCount = m_arVerticalColumns.size();
	for(int i=0; i<m_wVerticalColumnsCount; i++)
	{
		if(!fri)
		{
			Put("\\pard\\fs6\\par");
			fri=1;
#ifdef EdWrite
 			if(!RtfWriteMode)
			{
				indent.left   =  0;
				indent.right  =  0;
				indent.top    =  0;
				indent.bottom =  0;
				interval.cx   =  0;
				interval.cy   =  0;
				playout.x     = -1;
				playout.w     = -1;
				playout.y     = -1;
				playout.h     = -1;

				align = TP_LEFT_ALLIGN;
				hParagraph = CED_CreateParagraph( SectorInfo->hEDSector, SectorInfo->hObject, align, indent,
																											           SectorInfo->userNum, -1, interval, playout, -1, -1, -1, -1, FALSE );
				hString    = CED_CreateLine( hParagraph, FALSE , 6);
			}
#endif
		}

	 pRtfVerticalColumn = (CRtfVerticalColumn*)m_arVerticalColumns[i];
		if( pRtfVerticalColumn->m_wType == FT_FRAME)
		{
			Put("{\\pard\\plain\\slmult1\\nowidctlpar\\pvpara");
			shpleft   = pRtfVerticalColumn->m_rectReal.left   - m_rectReal.left;
			PutCom("\\posx",shpleft,0);
			shptop    = pRtfVerticalColumn->m_rectReal.top    - m_rectReal.top;
			PutCom("\\posy",shptop,0);
			shpbottom = pRtfVerticalColumn->m_rectReal.bottom - pRtfVerticalColumn->m_rectReal.top;
			PutCom("\\absh",shpbottom,0);
			shpright  = pRtfVerticalColumn->m_rectReal.right  - pRtfVerticalColumn->m_rectReal.left;
			PutCom("\\absw",shpright,0);
			Put("\\abslock1");
			Put("\\dxfrtext86\\dfrmtxtx86\\dfrmtxty43\\adjustright");

#ifdef EdWrite
  	if(!RtfWriteMode)
   {
    EdFragmRect.x = pRtfVerticalColumn->m_rectReal.left - m_rectReal.left;
    EdFragmRect.w = pRtfVerticalColumn->m_rectReal.right - pRtfVerticalColumn->m_rectReal.left;
    EdFragmRect.y = pRtfVerticalColumn->m_rectReal.top - m_rectReal.top;
    EdFragmRect.h = pRtfVerticalColumn->m_rectReal.bottom - pRtfVerticalColumn->m_rectReal.top;
			 SectorInfo->hObject = CED_CreateFrame(SectorInfo->hEDSector, SectorInfo->hColumn, EdFragmRect, 0x22,-1, 86, 43);
   }
#endif

   SectorInfo->FlagOverLayed=FALSE;
   pRtfVerticalColumn->Write(FOT_FRAME,SectorInfo);
			Put("}");
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 WriteNonTerminalColumns                                        //
////////////////////////////////////////////////////////////////////////////////////////////////////
void CRtfHorizontalColumn::WriteNonTerminalColumns(RtfSectorInfo* SectorInfo)
{
	CRtfVerticalColumn* pRtfVerticalColumn;

	m_wVerticalColumnsCount = m_arVerticalColumns.size();
	for(int i=0; i<m_wVerticalColumnsCount; i++)
	{
		pRtfVerticalColumn = m_arVerticalColumns[i];
		if( pRtfVerticalColumn->m_wType > FT_FRAME)
		{
			SectorInfo->FlagInColumn = FALSE;
			pRtfVerticalColumn->Write(FOT_FRAME, SectorInfo);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 ToPlacePicturesAndTables                                       //
////////////////////////////////////////////////////////////////////////////////////////////////////
void CRtfHorizontalColumn::ToPlacePicturesAndTables(CRtfFragment* pRtfFragment)
{
	CRtfVerticalColumn* pRtfVerticalColumn;
	CRtfFragment*       pRtfFragmentNew;

	m_arVerticalColumns.push_back( new CRtfVerticalColumn() );
	m_wVerticalColumnsCount = m_arVerticalColumns.size();
	if(m_wVerticalColumnsCount == 1)
		m_wType = HC_ONLY_PICTURE_TABLE;

	pRtfVerticalColumn	= m_arVerticalColumns.back();
	pRtfVerticalColumn->m_wFragmentsCount = 1;
	pRtfVerticalColumn->m_wType = pRtfFragment->m_wType;
	pRtfVerticalColumn->m_arFragments.push_back( new CRtfFragment() );

	pRtfFragmentNew=pRtfVerticalColumn->m_arFragments.back();
	pRtfFragmentNew->m_wType = pRtfFragment->m_wType;
	pRtfFragmentNew->m_wUserNumber = pRtfFragment->m_wUserNumber;
	pRtfFragmentNew->m_wUserNumberForFormattedMode = pRtfFragment->m_wUserNumberForFormattedMode;

	RtfUnionRect_CRect_CRect(&pRtfFragmentNew->m_rect, &pRtfFragment->m_rect);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 CRTFVERTICALCOLUMN                                             //
////////////////////////////////////////////////////////////////////////////////////////////////////
CRtfVerticalColumn::CRtfVerticalColumn()
{
	m_bSortFlag=0;
	m_wType =FT_TEXT;
	m_wFragmentsCount = 0;
	SetRect(&m_rect, 32000, 32000, 0, 0 );
//	m_arFragments.RemoveAll();
	m_bFlagObjectInColumn=0;
	m_PagePtr=0;
}

CRtfVerticalColumn::~CRtfVerticalColumn()
{
	CRtfFragment* cFrag;

	m_wFragmentsCount = m_arFragments.size();
	for(uint32_t i=0; i<m_wFragmentsCount; i++)
	{
		cFrag = m_arFragments[i];
		delete cFrag;
	}
//	m_arFragments.RemoveAll();
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 Write                                                          //
////////////////////////////////////////////////////////////////////////////////////////////////////
Bool CRtfVerticalColumn::Write(Bool OutPutType, RtfSectorInfo* SectorInfo)
{
	CRtfFragment* pRtfFragment;

	m_wFragmentsCount = m_arFragments.size();
	for(int i=0; i<m_wFragmentsCount; i++)
	{
		pRtfFragment = (CRtfFragment*)m_arFragments[i];
		if((pRtfFragment->m_wType == FT_TABLE || pRtfFragment->m_wType == FT_PICTURE) && pRtfFragment->m_bFlagUsed == TRUE)
	 		continue;
		if(pRtfFragment->m_wType == FT_TABLE)
		{
			SectorInfo->userNum = pRtfFragment->m_wUserNumberForFormattedMode;
			pRtfFragment->FWriteTable((int)pRtfFragment->m_wUserNumber, SectorInfo, OutPutType);
			pRtfFragment->m_bFlagUsed = TRUE;
		}
		else
		if(pRtfFragment->m_wType == FT_PICTURE)
		{
			SectorInfo->userNum = pRtfFragment->m_wUserNumberForFormattedMode;
			pRtfFragment->FWritePicture((int)pRtfFragment->m_wUserNumber, SectorInfo, OutPutType);
 			pRtfFragment->m_bFlagUsed = TRUE;
		}
		else
		{
			if( !pRtfFragment->m_LeftOffsetFragmentFromVerticalColumn && !pRtfFragment->m_RightOffsetFragmentFromVerticalColumn )
			{
				pRtfFragment->m_LeftOffsetFragmentFromVerticalColumn  = pRtfFragment->m_rect.left - m_rect.left;
				pRtfFragment->m_RightOffsetFragmentFromVerticalColumn = m_rect.right-pRtfFragment->m_rect.right;
			}
			if(!pRtfFragment->m_WidthVerticalColumn)
				 pRtfFragment->m_WidthVerticalColumn = (int16_t)(m_rect.right - m_rect.left);

			pRtfFragment->pRtfParent=m_PagePtr;
			pRtfFragment->FWriteText( 0, SectorInfo, OutPutType);
		}
	}
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 SetSpaceRect                                                   //
////////////////////////////////////////////////////////////////////////////////////////////////////
void CRtfVerticalColumn::SetSpaceRect(CRtfFragment* CurrentFragment, RtfSectorInfo* SectorInfo)
{
	CRtfFragment*    pRtfFragment;
	RECT            LeftFreePlace,RightFreePlace,TopFreePlace,BottomFreePlace,CurrentFragmentRect,RectInter;
	int              i,CountFragments;
	int32_t            LeftFree,RightFree,TopFree,BottomFree;


	LeftFree  = CurrentFragment->m_rect.left;
	RightFree = SectorInfo->PaperW - CurrentFragment->m_rect.right;
	TopFree   = CurrentFragment->m_rect.top;
	BottomFree = SectorInfo->PaperH - CurrentFragment->m_rect.bottom;

	LeftFreePlace.left     = 0;
	LeftFreePlace.right    = MAX(0,CurrentFragment->m_rect.left-1);
	LeftFreePlace.top      = CurrentFragment->m_rect.top;
	LeftFreePlace.bottom   = CurrentFragment->m_rect.bottom;

	RightFreePlace.left    = CurrentFragment->m_rect.right+1;
	RightFreePlace.right   = SectorInfo->PaperW;
	RightFreePlace.top     = CurrentFragment->m_rect.top;
	RightFreePlace.bottom  = CurrentFragment->m_rect.bottom;

	TopFreePlace.left      = CurrentFragment->m_rect.left;
	TopFreePlace.right     = CurrentFragment->m_rect.right;
	TopFreePlace.top       = 0;
	TopFreePlace.bottom    = MAX(0,CurrentFragment->m_rect.top-1);

	BottomFreePlace.left   = CurrentFragment->m_rect.left;
	BottomFreePlace.right  = CurrentFragment->m_rect.right;
	BottomFreePlace.top    = CurrentFragment->m_rect.bottom+1;
	BottomFreePlace.bottom = SectorInfo->PaperH;

	CountFragments = m_PagePtr->m_arFragments.size();
 	for( i=0; i<CountFragments; i++ )
	{
		pRtfFragment = m_PagePtr->m_arFragments[i];
		if( pRtfFragment->m_wType == FT_PICTURE || pRtfFragment->m_wType == FT_TABLE )
			continue;

		CurrentFragmentRect.left   = pRtfFragment->m_rect.left;
		CurrentFragmentRect.right  = pRtfFragment->m_rect.right;
		CurrentFragmentRect.top    = pRtfFragment->m_rect.top;
		CurrentFragmentRect.bottom = pRtfFragment->m_rect.bottom;

		if(IntersectRect(&RectInter,&CurrentFragmentRect, &LeftFreePlace))
			LeftFree   = MIN(LeftFree, LeftFreePlace.right - CurrentFragmentRect.right);

		if(IntersectRect(&RectInter,&CurrentFragmentRect, &RightFreePlace))
			RightFree  = MIN(RightFree, CurrentFragmentRect.left - RightFreePlace.left);

		if(IntersectRect(&RectInter,&CurrentFragmentRect, &TopFreePlace))
			TopFree    = MIN(TopFree, TopFreePlace.bottom - CurrentFragmentRect.bottom);

		if(IntersectRect(&RectInter,&CurrentFragmentRect, &BottomFreePlace))
			BottomFree = MIN(BottomFree, CurrentFragmentRect.top - BottomFreePlace.top);
	}

  SectorInfo->m_rectFree.left   = CurrentFragment->m_rectFree.left   = (int32_t)(MAX(0,LeftFree)   * TwipsToEMU_Koef);
  SectorInfo->m_rectFree.right  = CurrentFragment->m_rectFree.right  = (int32_t)(MAX(0,RightFree)  * TwipsToEMU_Koef);
  SectorInfo->m_rectFree.top    = CurrentFragment->m_rectFree.top    = (int32_t)(MAX(0,TopFree)    * TwipsToEMU_Koef);
  SectorInfo->m_rectFree.bottom = CurrentFragment->m_rectFree.bottom = (int32_t)(MAX(0,BottomFree) * TwipsToEMU_Koef);

}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 CRTFFRAGMENT                                                   //
////////////////////////////////////////////////////////////////////////////////////////////////////
CRtfFragment::CRtfFragment()
{
	m_wStringsCount = 0;
	m_CountLeftEqual = 0;
	m_CountRightEqual = 0;
	m_CountLeftRightEqual = 0;
	m_CountCentreEqual = 0;

	SetRect(&m_rect, 32000, 32000, 0, 0 );
	//m_arStrings.RemoveAll();
	m_bOutPutType = FOT_FRAME;
	m_LeftOffsetFragmentFromVerticalColumn  =0;
	m_RightOffsetFragmentFromVerticalColumn =0;
 m_WidthVerticalColumn = 0;
 m_bFlagObjectInColumn = 0;
	m_wOffsetFromPrevTextFragment = 0;
	m_bFlagUsed = 0;
 m_Attr=0;
 m_FlagCarry=0;
 m_Flag=0;
 pRtfParent=0;
}

CRtfFragment::~CRtfFragment()
{
	CRtfString* cString;

	m_wStringsCount = m_arStrings.size();
	for( int i=0; i<m_wStringsCount; i++ )
	{
		cString = m_arStrings[i];
		delete cString;
	}
//	m_arStrings.RemoveAll();
}

CRtfString* CRtfFragment::GetNextString()
{
	m_arStrings.push_back( new CRtfString());
	return m_arStrings.back();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 FWriteText                                                     //
////////////////////////////////////////////////////////////////////////////////////////////////////
Bool CRtfFragment::FWriteText(int16_t NumberCurrentFragment,RtfSectorInfo *SectorInfo, Bool OutPutType )
{
	CRtfWord*   pRtfWord;
	CRtfString* pRtfString;
	CRtfChar*   pRtfChar;
	uint16_t        CountWords;
	uint16_t        CountChars;
	int16_t       flag_end_word_with_hiphen;
	int16_t       tmp_font_name;
	Bool        boPrevNega, boNega;

#ifdef EdWrite
	int         EDFontAttribs,EDFontPointSize;
	Handle     	hParagraph=NULL;
	Handle     	hString=NULL;
	EDRECT      slayout;
	letterEx    Letter[REC_MAX_VERS];
    int         shading = -1;
#endif

	InitFragment(SectorInfo);

//--- Цикл по строкам
 boPrevNega = false; //NEGA_STR
 for( int ns=0; ns < m_wStringsCount; ns++ )
 {
  pRtfString = (CRtfString*)m_arStrings[ns];
  pRtfWord   = (CRtfWord*)pRtfString->m_arWords[0];
  pRtfChar   = (CRtfChar*)pRtfWord->m_arChars[0];
  if(pRtfChar->m_bFlg_cup_drop==TRUE) //заносим буквицы во frame
  {
   if((FlagMode & USE_FRAME) || OutPutType)
    pRtfChar->m_bFlg_cup_drop=FALSE;
   else
    pRtfString->m_wFlagBeginParagraph = TRUE;
  }

  // чтобы не смешивать в одном абзаце негатив. и позитив. строки, при смене
  // цвета стартуем новый абзац
  boNega = pRtfString->S_Flags & CSTR_STR_NEGATIVE; //NEGA_STR

  if (boNega != boPrevNega)
	  pRtfString->m_wFlagBeginParagraph = true;

  if( pRtfString->m_wFlagBeginParagraph == TRUE)
  {
	if(ns)
	{
		if( OutPutType )
			Put("}");
		PutCom("\\fs",m_wprev_font_size*2,1);
		Put("\\par");
	}
	if(pRtfString->m_wAlignment == RTF_TP_TYPE_LINE)
	{
		pRtfString->m_wAlignment = RTF_TP_LEFT_AND_RIGHT_ALLIGN;
		m_fi = -pRtfString->m_wFirstIndent;
	}
	else
		m_fi = pRtfString->m_wFirstIndent;

	m_wvid_parag          = pRtfString->m_wAlignment;
	m_li                  = pRtfString->m_wLeftIndent;
	m_ri                  = pRtfString->m_wRightIndent;
	m_sb                  = pRtfString->m_wSpaceBefore;
#ifdef EdWrite
	if(!RtfWriteMode && OutPutType)
		m_sb = 0;
#endif
  	m_wprev_font_size     = pRtfWord->m_wRealFontPointSize;

	if( FlagMode & USE_FRAME_AND_COLUMN )
	{
		if(	SectorInfo->FlagOneString == TRUE)
		{
			  m_li = 0;
					m_fi = MAX(0,(int16_t)(m_rect.left - SectorInfo->MargL));
			  m_ri = 0;
		}
	}
	new_paragraph( OutPutType );
#ifdef EdWrite
	if(!RtfWriteMode)
	{
		pRtfWord   = (CRtfWord*)pRtfString->m_arWords[0];
		pRtfChar   = (CRtfChar*)pRtfWord->m_arChars[0];

		int colWidth=0;

		if (pRtfParent&&!(FlagMode & USE_NONE))
		{
			CRtfSector*  curSect=(CRtfSector* )pRtfParent->m_arSectors[pRtfParent->m_nCurSectorNumber];

			//Если пишем с форматированием и однострочная колонка
			if( FlagMode & USE_FRAME_AND_COLUMN && curSect->SectorInfo.FlagOneString == TRUE )
 				colWidth = SectorInfo->PaperW - (SectorInfo->MargL + SectorInfo->MargR);
			//Если пишем в колонку
			else if (SectorInfo->hColumn==SectorInfo->hObject)
			{
				colWidth=curSect->m_arWidthTerminalColumns[curSect->m_VTerminalColumnNumber-1];
			}

		}
		//Если ничего не помогло
		if (!colWidth)
			colWidth=m_rectReal.right - m_rectReal.left;

		if(!pRtfChar->m_bFlg_cup_drop )
			hParagraph = Rtf_CED_CreateParagraph( m_fi, m_li, m_ri, m_sb, SectorInfo, m_wvid_parag, /*m_Flag*/ pRtfString->S_Flags, pRtfString->m_LengthStringInTwips, colWidth); //NEGA_STR

	}
#endif
  }

#ifdef EdWrite
  if(!RtfWriteMode && !pRtfChar->m_bFlg_cup_drop)

#ifdef CHEREDOV
	  hString = CED_CreateLine(hParagraph,pRtfString->m_bLineTransfer,(int)((pRtfWord->m_wRealFontPointSize-1)*2));
#else
  if((FlagMode & NOSIZE) &&  !(FlagMode & USE_FRAME))
	  hString = CED_CreateLine(hParagraph,pRtfString->m_bLineTransfer,DefFontSize); //line is text line
  else
	  hString = CED_CreateLine(hParagraph,pRtfString->m_bLineTransfer,(int)(pRtfWord->m_wRealFontPointSize*2));
#endif
#endif

//--- Цикл по словам
	CountWords = pRtfString->m_wWordsCount;
	for(int nw=0; nw<CountWords; nw++)
	{
		pRtfWord   = (CRtfWord*)pRtfString->m_arWords[nw];
		pRtfChar   = (CRtfChar*)pRtfWord->m_arChars[0];
		Put("{");

		tmp_font_name =  get_font_name(pRtfWord->m_wFontNumber);
		if( m_wprev_font_name != tmp_font_name)
		{
			switch(tmp_font_name)
			{
				case 0:
					PutCom("\\f",0,0);
							    break;
				case 1:
					PutCom("\\f",1,0);
							    break;
				case 2:
					PutCom("\\f",2,0);
							    break;
				case 3:
					PutCom("\\f",3,0);
							    break;

				default:
					PutCom("\\f",1,0);
									  break;
			}
		}

#ifdef EdWrite
		EDFontAttribs = 0;
#endif

		if(pRtfWord->m_wFontNumber & TG_EDW_UNDERLINE)
		{
			Put("\\ul");
#ifdef EdWrite
			EDFontAttribs = EDFontAttribs | TG_EDW_UNDERLINE;
#endif
		}
		if( !(FlagMode & NOBOLD)   && (pRtfWord->m_wFontNumber & TG_EDW_BOLD))
		{
			Put("\\b");
#ifdef EdWrite
			EDFontAttribs = EDFontAttribs | TG_EDW_BOLD;
#endif
		}
		if( !(FlagMode & NOCURSIV) && (pRtfWord->m_wFontNumber & TG_EDW_ITALIC))
		{
			Put("\\i");
#ifdef EdWrite
			EDFontAttribs = EDFontAttribs | TG_EDW_ITALIC;
#endif
		}
		if(m_wprev_lang !=1024)
			PutCom("\\lang",1024,0);
		if((FlagMode & NOSIZE) &&  !(FlagMode & USE_FRAME))
			PutCom("\\fs",DefFontSize,1);
		else
			PutCom("\\fs",pRtfWord->m_wRealFontPointSize*2,1);
		flag_end_word_with_hiphen=0;
		pRtfWord->get_coordinates_and_probability();
		Put("{");
		PutCom("\\wcl",pRtfWord->m_wcl,0);
		PutCom("\\wcr",pRtfWord->m_wcr,0);
		PutCom("\\wct",pRtfWord->m_wct,0);
		PutCom("\\wcb",pRtfWord->m_wcb,0);
		PutCom("\\wcp",pRtfWord->m_wcp,0);
		if(!pRtfWord->m_wcs)
			Put("\\wcs");
		Put("{");
  //--- Цикл по буквам

		CountChars = pRtfWord->m_wCharsCount;
		for(int nz=0; nz<CountChars; nz++)
		{
			pRtfChar   = (CRtfChar*)pRtfWord->m_arChars[nz];
#ifdef EdWrite
			if(!pRtfWord->m_wcs)
				pRtfChar->m_chrVersions[0].m_bProbability = 0;

			if(nw==0 && nz==0 && pRtfChar->m_bFlg_cup_drop )
#ifdef CHEREDOV
				EDFontPointSize = (int)((pRtfChar->m_wFontPointSize-1)*2);
#else
			if((FlagMode & NOSIZE) &&  !(FlagMode & USE_FRAME))
				EDFontPointSize = DefFontSize;
			else
				EDFontPointSize = (int)(pRtfChar->m_wFontPointSize*2);
#endif
			else
#ifdef CHEREDOV
				EDFontPointSize = (int)((pRtfWord->m_wRealFontPointSize-1)*2);
#else
			if((FlagMode & NOSIZE) &&  !(FlagMode & USE_FRAME))
				EDFontPointSize = DefFontSize;
			else
				EDFontPointSize = (int)(pRtfWord->m_wRealFontPointSize*2);
#endif

#endif
			flag_end_word_with_hiphen = 0;
			if( nw==(CountWords-1) && nz==(CountChars-1) && pRtfChar->m_chrVersions[0].m_bChar=='-')
				flag_end_word_with_hiphen = 1;

			if(pRtfChar->m_chrVersions[0].m_bChar)
			{
				if( pRtfString->m_bLineTransfer == TRUE )
				{
#ifdef EdWrite
					if(!RtfWriteMode)
					{
						if(nw==0 && nz==0 && pRtfChar->m_bFlg_cup_drop )
						{
							slayout.left    = 0;
							slayout.right   = 0;
							slayout.top     = 0;
							slayout.bottom  = 0;
							EDBOX   playout__  ={0,0,0,0};
							Handle hObject__ = CED_CreateFrame(SectorInfo->hEDSector, SectorInfo->hColumn, playout__, 0x22,-1, -1, -1);
							CED_SetFrameFlag(hObject__, ED_DROPCAP);
							EDSIZE  interval__ ={0,0};
//							if(m_Flag & CSTR_STR_NEGATIVE) //nega
							if(pRtfString->S_Flags & CSTR_STR_NEGATIVE) //NEGA_STR
								shading = 10000;
							Handle  hParagraph__ = CED_CreateParagraph(SectorInfo->hEDSector, hObject__, TP_LEFT_ALLIGN, slayout,
	 	                                         0, -1, interval__, playout__, -1, shading, -1, -1, FALSE);

							Handle  hString__    = CED_CreateLine( hParagraph__, FALSE , 6);
							Rtf_CED_CreateChar( &slayout, Letter, pRtfChar );
							CED_CreateChar(hString__, slayout, Letter, EDFontPointSize, (int)tmp_font_name,
								EDFontAttribs, pRtfChar->m_blanguage, -1, -1);

							hParagraph = Rtf_CED_CreateParagraph( m_fi, m_li, m_ri, m_sb, SectorInfo, m_wvid_parag,/*m_Flag*/ pRtfString->S_Flags, pRtfString->m_LengthStringInTwips, m_rectReal.right - m_rectReal.left ); //NEGA_STR
     #ifdef CHEREDOV
							hString = CED_CreateLine(hParagraph,pRtfString->m_bLineTransfer,(int)((pRtfWord->m_wRealFontPointSize-1)*2));
     #else
							if((FlagMode & NOSIZE) &&  !(FlagMode & USE_FRAME))
								hString = CED_CreateLine(hParagraph,pRtfString->m_bLineTransfer,DefFontSize);
							else
								hString = CED_CreateLine(hParagraph,pRtfString->m_bLineTransfer,(int)(pRtfWord->m_wRealFontPointSize*2));
     #endif

						}
						else
						{
							Rtf_CED_CreateChar( &slayout, Letter, pRtfChar );
							CED_CreateChar(hString, slayout, Letter, EDFontPointSize, (int)tmp_font_name,
								EDFontAttribs, pRtfChar->m_blanguage, -1, -1);
						}
					}
#endif
					if(nw==0 && nz==0 && pRtfChar->m_bFlg_cup_drop )
						WriteCupDrop(pRtfChar,tmp_font_name);
					else
						PutChar(pRtfChar->m_chrVersions[0].m_bChar);
				}
				else if(!((m_wvid_parag == RTF_TP_LEFT_AND_RIGHT_ALLIGN || m_wvid_parag == RTF_TP_LEFT_ALLIGN) &&
						  flag_end_word_with_hiphen && pRtfChar->m_bFlg_spell_nocarrying))
				{
#ifdef EdWrite
					if(!RtfWriteMode)
					{
						if(nw==0 && nz==0 && pRtfChar->m_bFlg_cup_drop)
						{
							slayout.left    = 0;
							slayout.right   = 0;
							slayout.top     = 0;
							slayout.bottom  = 0;
							EDBOX   playout__  ={0,0,0,0};
							Handle hObject__ = CED_CreateFrame(SectorInfo->hEDSector, SectorInfo->hColumn, playout__, 0x22,-1, -1, -1);
							CED_SetFrameFlag(hObject__, ED_DROPCAP);
							EDSIZE  interval__ ={0,0};
							Handle  hParagraph__ = CED_CreateParagraph(SectorInfo->hEDSector, hObject__, TP_LEFT_ALLIGN, slayout,
								0, -1, interval__, playout__, -1, -1, -1, -1, FALSE);
							Handle  hString__    = CED_CreateLine( hParagraph__, FALSE , 6);
							Rtf_CED_CreateChar( &slayout, Letter, pRtfChar );
							CED_CreateChar(hString__, slayout, Letter, EDFontPointSize, (int)tmp_font_name,
		                    EDFontAttribs, pRtfChar->m_blanguage, -1, -1);

							hParagraph = Rtf_CED_CreateParagraph( m_fi, m_li, m_ri, m_sb, SectorInfo, m_wvid_parag,/*m_Flag*/ pRtfString->S_Flags, pRtfString->m_LengthStringInTwips, m_rectReal.right - m_rectReal.left ); //NEGA_STR
     #ifdef CHEREDOV
							hString = CED_CreateLine(hParagraph,pRtfString->m_bLineTransfer,(int)((pRtfWord->m_wRealFontPointSize-1)*2));
     #else
							if((FlagMode & NOSIZE) &&  !(FlagMode & USE_FRAME))
								hString = CED_CreateLine(hParagraph,pRtfString->m_bLineTransfer,DefFontSize);
							else
								hString = CED_CreateLine(hParagraph,pRtfString->m_bLineTransfer,(int)(pRtfWord->m_wRealFontPointSize*2));
     #endif

						}
						else
						{
							Rtf_CED_CreateChar( &slayout, Letter, pRtfChar );
							CED_CreateChar(hString, slayout, Letter, EDFontPointSize, (int)tmp_font_name,
								EDFontAttribs, pRtfChar->m_blanguage, -1, -1);
						}
					}
#endif
					if(nw==0 && nz==0 && pRtfChar->m_bFlg_cup_drop )
						WriteCupDrop(pRtfChar,tmp_font_name);
		 			else
						PutChar(pRtfChar->m_chrVersions[0].m_bChar);
				}
				else
				{
#ifdef EdWrite
					if(!RtfWriteMode)
					{
						EDFontAttribs = EDFontAttribs | 0x02;
						Rtf_CED_CreateChar( &slayout, Letter, pRtfChar );
						CED_CreateChar(hString, slayout, Letter, EDFontPointSize, (int)tmp_font_name,
							EDFontAttribs, pRtfChar->m_blanguage, -1, -1);
					}
#endif
				}
	}
  }
//--- Конец цикла по буквам
  if(nw < CountWords-1)
  {
#ifdef EdWrite
	  if(!RtfWriteMode && !pRtfChar->m_bFlg_cup_drop)
	  {
		  Rtf_CED_CreateChar( &slayout, Letter, NULL );
#ifdef CHEREDOV
		  CED_CreateChar(hString, slayout, Letter, (int)((pRtfWord->m_wRealFontPointSize-1)*2), (int)tmp_font_name,
			  EDFontAttribs, -1, -1, -1);
#else

		  if((FlagMode & NOSIZE) &&  !(FlagMode & USE_FRAME))
			  CED_CreateChar(hString, slayout, Letter, DefFontSize, (int)tmp_font_name, EDFontAttribs, -1, -1, -1);
		  else
			  CED_CreateChar(hString, slayout, Letter, (int)(pRtfWord->m_wRealFontPointSize*2), (int)tmp_font_name,
                   EDFontAttribs, -1, -1, -1);
#endif
	  }
#endif
	  if(!pRtfChar->m_bFlg_cup_drop)
		  PutChar(' '); //InterWord Space
  }
  else if((ns < m_wStringsCount-1) && (nw == CountWords-1) && (m_wvid_parag==RTF_TP_LEFT_AND_RIGHT_ALLIGN || m_wvid_parag==RTF_TP_LEFT_ALLIGN)
				  && !flag_end_word_with_hiphen )
  {
#ifdef EdWrite
	  if(!RtfWriteMode)
	  {
		  Rtf_CED_CreateChar( &slayout, Letter, NULL );
#ifdef CHEREDOV
		  CED_CreateChar(hString, slayout, Letter, (int)((pRtfWord->m_wRealFontPointSize-1)*2), (int)tmp_font_name,
                    EDFontAttribs, -1, -1, -1);
#else
		  if((FlagMode & NOSIZE) &&  !(FlagMode & USE_FRAME))
			  CED_CreateChar(hString, slayout, Letter, DefFontSize, (int)tmp_font_name, EDFontAttribs, -1, -1, -1);
		  else
			  CED_CreateChar(hString, slayout, Letter, (int)(pRtfWord->m_wRealFontPointSize*2), (int)tmp_font_name,
                    EDFontAttribs, -1, -1, -1);
#endif
	  }
#endif

	  PutChar(' '); //InterWord Space
  }

  Put("}");
  Put("}");
  Put("}");
}
//--- Конец цикла по словам
  if( pRtfString->m_bLineTransfer == TRUE )
   Put("\\line ");

  boPrevNega = boNega; //NEGA_STR
}
//--- Конец цикла по строкам
  if( OutPutType )
	  Put("}");
  PutCom("\\fs",m_wprev_font_size*2,1);
  Put("\\par");
  return TRUE;
}


void WriteCupDrop(CRtfChar* pRtfChar,int16_t font)
{
	Put("{\\pard\\fs6\\par}");
 Put("{\\pard\\plain\\slmult0\\keepn\\widctlpar\\pvpara\\dropcapli3\\dropcapt1\\cgrid{");
	PutCom("\\fs",pRtfChar->m_wFontPointSize*2,1);
	PutCom("\\f",font,1);
	PutCom("\\dn",9,1);
	PutCom("\\lang",1024,1);
 PutChar(pRtfChar->m_chrVersions[0].m_bChar);
 Put("\\par}}");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 InitFragment                                                   //
////////////////////////////////////////////////////////////////////////////////////////////////////
void CRtfFragment::InitFragment(RtfSectorInfo* SectorInfo)
{
 CRtfWord*   pRtfWord;
 CRtfString* pRtfString;
 int         PenaltyForCheredov=0;
 int			      CountWords;

	pRtfString = (CRtfString*) m_arStrings[0];
	pRtfWord   = (CRtfWord*)   pRtfString->m_arWords[0];
	CountWords = pRtfString->m_wWordsCount;

	if(m_wStringsCount==1 && SectorInfo->CountFragments>1)
	{
		if(pRtfWord->m_wRealFontPointSize>=14)
   PenaltyForCheredov = 4;
  else
		 if(pRtfWord->m_wRealFontPointSize<14 && pRtfWord->m_wRealFontPointSize>10 )
    PenaltyForCheredov = 2;
			else
		 if(pRtfWord->m_wRealFontPointSize<10 && pRtfWord->m_wRealFontPointSize>5 )
    PenaltyForCheredov = 1;
	}

	for(int nw=0; nw<CountWords; nw++)
	{
  pRtfWord   = (CRtfWord*)pRtfString->m_arWords[nw];
		if(pRtfWord->m_wRealFontPointSize>5 )
  	pRtfWord->m_wRealFontPointSize -= PenaltyForCheredov;
	}

 m_wprev_font_name     = get_font_name(pRtfWord->m_wFontNumber);
	m_wprev_Underline     = pRtfWord->m_wFontNumber & TG_EDW_UNDERLINE;
	m_wprev_Bold          = pRtfWord->m_wFontNumber & TG_EDW_BOLD;
	m_wprev_Italic        = pRtfWord->m_wFontNumber & TG_EDW_ITALIC;
	m_wprev_lang          = 1024;
	m_wprev_font_size     = pRtfWord->m_wRealFontPointSize;

	SetFragmentAlignment(SectorInfo);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 FWriteTable                                                    //
////////////////////////////////////////////////////////////////////////////////////////////////////
Bool CRtfFragment::FWriteTable(int16_t NumberCurrentFragment,RtfSectorInfo *SectorInfo, Bool OutPutType)
{
// CString  TableString;
//	uint32_t   CountTableElem;
//	uint32_t   Tindex;
//	char     Tsym;

//	WriteTable((uint32_t)NumberCurrentFragment, SectorInfo/*, &TableString */, OutPutType);
/*	if(RtfWriteMode)
	{
		CountTableElem = TableString.GetLength();
		for( Tindex=0;  Tindex<CountTableElem; Tindex++ )
		{
			Tsym=(char)TableString.GetAt(Tindex);
			if(Tsym)
				PutC(Tsym);
		}
	}*/
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 FWritePicture                                                  //
////////////////////////////////////////////////////////////////////////////////////////////////////
Bool CRtfFragment::FWritePicture(int16_t NumberCurrentFragment,RtfSectorInfo *SectorInfo, Bool OutPutType)
{
//	CString  PictString;
//	uint32_t   Pindex;
//	uint32_t  CountPictElem;
//	char     Psym;

	WritePict( (uint32_t)NumberCurrentFragment, SectorInfo,/* &PictString,*/ OutPutType );
/*	if(RtfWriteMode)
	{
		CountPictElem = PictString.GetLength();
		for( Pindex=0;  Pindex<CountPictElem; Pindex++ )
		{
			Psym=(char)PictString.GetAt(Pindex);
			if(Psym)
				PutC(Psym);
		}
	}
*/	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 new_paragraph                                                  //
////////////////////////////////////////////////////////////////////////////////////////////////////
void CRtfFragment::new_paragraph( Bool OutPutType )
{
 if( OutPutType )
	{
		switch(m_wvid_parag)
		{
			case RTF_TP_LEFT_ALLIGN:
										Put("\\ql");
	  							break;
			case RTF_TP_RIGHT_ALLIGN:
										Put("\\qr");
		  						break;
			case RTF_TP_LEFT_AND_RIGHT_ALLIGN:
										Put("\\qj");
		  						break;
			case RTF_TP_CENTER:
										if(FlagMode & USE_NONE)
											Put("\\ql");
										else
											Put("\\qc");
		  						break;
 		case RTF_TP_ONE:
										Put("\\ql");
		  						break;
		}
 PutCom("\\li",m_li,0);
 PutCom("\\ri",m_ri,0);
	if(m_fi<0)
  PutCom("\\fi-",abs(m_fi),0);
	else
  PutCom("\\fi",m_fi,0);
 PutCom("\\sb",m_sb,0);
 PutCom("\\sa",0,0);
	PutCom("\\sl",0,0);


  Put("{");
	}
 Put("\\pard");
	Put("\\plain");

	switch(m_wprev_font_name)
	{
		case 0:
         PutCom("\\f",0,0);
							  break;
		case 1:
         PutCom("\\f",1,0);
							  break;
  case 2:
         PutCom("\\f",2,0);
							  break;
  case 3:
         PutCom("\\f",3,0);
							  break;
		default:
         PutCom("\\f",1,0);
									break;
	}

 PutCom("\\lang",m_wprev_lang,0);
	if((FlagMode & NOSIZE) &&  !(FlagMode & USE_FRAME))
  PutCom("\\fs",DefFontSize,1);
	else
  PutCom("\\fs",m_wprev_font_size*2,1);

 if( OutPutType )
  return;

	switch(m_wvid_parag)
	{
	 case RTF_TP_LEFT_ALLIGN:
		       Put("\\ql");
	  					 break;
	 case RTF_TP_RIGHT_ALLIGN:
		       Put("\\qr");
		  				 break;
	 case RTF_TP_LEFT_AND_RIGHT_ALLIGN:
         Put("\\qj");
		  				 break;
	 case RTF_TP_CENTER:
			      if(FlagMode & USE_NONE)
          Put("\\ql");
									else
          Put("\\qc");
		  				 break;
 	case RTF_TP_ONE:
         Put("\\ql");
		  				 break;
	}
 PutCom("\\li",m_li,0);
 PutCom("\\ri",m_ri,0);
	if(m_fi<0)
  PutCom("\\fi-",abs(m_fi),0);
	else
  PutCom("\\fi",m_fi,0);
 PutCom("\\sb",m_sb,0);
 PutCom("\\sa",0,0);
	PutCom("\\sl",0,0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 CRTFSTRING                                                     //
////////////////////////////////////////////////////////////////////////////////////////////////////
CRtfString::CRtfString()
{
 m_wWordsCount = 0;
//	m_arWords.RemoveAll();
 m_wLeftIndent=0;
 m_wRightIndent=0;
 m_wFirstIndent=0;
 m_wFlagBeginParagraph=0;
 m_wAlignment=0;
 m_wPrevAlignment=0;
 m_wLeftBorderEqual=0;
 m_wRightBorderEqual=0;
 m_wCentreEqual=0;
	m_bLineTransfer=0;
 m_FlagCarry=0;
 S_Flags=0;
 m_LengthStringInTwips=0;
}

CRtfString::~CRtfString()
{
	CRtfWord* cWord;

	m_wWordsCount = m_arWords.size();
	for( int i=0; i<m_wWordsCount; i++ )
	{
		cWord = m_arWords[i];
		delete cWord;
	}
//	m_arWords.RemoveAll();
}

int16_t CRtfString::GetStringSizeInTwips()
{
 CRtfWord*       pRtfWord;
 CRtfChar        *pLastChar,*pFirstChar;
	uint16_t CountChars;

	pRtfWord   = (CRtfWord*)m_arWords[0];
 pFirstChar = (CRtfChar*)pRtfWord->m_arChars[0];
 pRtfWord   = (CRtfWord*)m_arWords[m_wWordsCount-1];
	CountChars = pRtfWord->m_wCharsCount;
 pLastChar  = (CRtfChar*)pRtfWord->m_arChars[CountChars-1];
	int16_t LenghtStr  = (int16_t)(( pLastChar->m_Idealrect.right - pFirstChar->m_Idealrect.left)*Twips);
	return LenghtStr;
}

uint16_t CRtfString::GetRealStringSize(void)
{
//	CString         TmpString;
	char tmp_str[MAX_BUFFER_SIZE] ;
	CRtfWord*       pRtfWord;
	CRtfChar        *pRtfChar;

	uint16_t  CountChars,RealSize;
	int16_t strHeight;
	int   index=0;

	for(int nw=0; nw<m_wWordsCount; nw++)
	{
		pRtfWord   = (CRtfWord*)m_arWords[nw];
		CountChars = pRtfWord->m_wCharsCount;
		for(int nz=0; nz<CountChars; nz++)
		{
			pRtfChar = (CRtfChar*)pRtfWord->m_arChars[nz];
			tmp_str[index++] = pRtfChar->m_chrVersions[0].m_bChar;

		}
		tmp_str[index++]=' ';
	}

	tmp_str[index]=0;
	pRtfWord = (CRtfWord*)m_arWords[0];
	pRtfChar = (CRtfChar*)pRtfWord->m_arChars[0];

	RealSize = GetRealSize((char*)&tmp_str, strlen(tmp_str), pRtfWord->m_wRealFontPointSize, pRtfChar->m_wFontNumber, &strHeight);
	return RealSize;
}

CRtfWord* CRtfString::GetNextWord()
{
	m_arWords.push_back( new CRtfWord());
	return m_arWords.back();
}

uint16_t CRtfString:: get_max_font_size()
{
 uint16_t nw,str_max_font=3;
 CRtfWord* pRtfWord;

 for(nw=0; nw<m_wWordsCount; nw++ )
	{
  pRtfWord = (CRtfWord*)m_arWords[nw];
  str_max_font=MAX(str_max_font, pRtfWord->m_wRealFontPointSize);
	}
 return str_max_font;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 CRTFWORD                                                       //
////////////////////////////////////////////////////////////////////////////////////////////////////
CRtfWord::CRtfWord()
{
	m_wCharsCount = 0;
//	m_arChars.RemoveAll();
}

CRtfWord::~CRtfWord()
{
	CRtfChar* cChar;

	m_wCharsCount = m_arChars.size();
	for( int i=0; i<m_wCharsCount; i++ )
	{
		cChar = m_arChars[i];
		delete cChar;
	}
//	m_arChars.RemoveAll();
}

CRtfChar* CRtfWord::GetNextChar()
{
	m_arChars.push_back( new CRtfChar());
	return m_arChars.back();
}

void CRtfWord::get_coordinates_and_probability()
{
 int16_t nz;
	int16_t t,l,b,r;

 CRtfChar  *pRtfChar, *pRtfCharFirst, *pRtfCharLast;
 PAGEINFO  PageInfo;

	Handle hCPAGE = CPAGE_GetHandlePage( CPAGE_GetCurrentPage());
	GetPageInfo(hCPAGE,&PageInfo);

 m_wcl = m_wct = 32000;
 m_wcr = m_wcb = 0;
 m_wcs = 1;
 m_wcp = 254;

 pRtfCharFirst = (CRtfChar*)m_arChars[0];
 pRtfCharLast  = (CRtfChar*)m_arChars[m_wCharsCount-1];

 m_wcl = (int16_t)pRtfCharFirst->m_Realrect.left;
	m_wcr = (int16_t)pRtfCharLast->m_Realrect.right;
	m_wct = MIN((int16_t)pRtfCharFirst->m_Realrect.top,    (int16_t)pRtfCharLast->m_Realrect.top);
	m_wcb = MAX((int16_t)pRtfCharFirst->m_Realrect.bottom, (int16_t)pRtfCharLast->m_Realrect.bottom);

	for( nz=0; nz<m_wCharsCount; nz++ )
	{
		pRtfChar = (CRtfChar*)m_arChars[nz];
		m_wcp = MIN( m_wcp, pRtfChar->m_chrVersions[0].m_bProbability );
		m_wcs = MIN( m_wcs, pRtfChar->m_bFlg_spell );
	}

	if(PageInfo.Angle)
	{
		t=m_wct; r=m_wcr; b=m_wcb; l=m_wcl;
		if( PageInfo.Angle == 90 )  //270
			{m_wcl=t; m_wcr=b;  m_wct=(int16_t)PageInfo.Height-r; m_wcb=(int16_t)PageInfo.Height-l;}
		else
			if( PageInfo.Angle == 270 ) //90
			{
				m_wcl=(int16_t)PageInfo.Width-b;
				m_wcr=(int16_t)PageInfo.Width-t;
				m_wct=l;
				m_wcb=r;
			}
			else
			if( PageInfo.Angle == 180 )
			{
				m_wcl=(int16_t)PageInfo.Width-r;
				m_wcr=(int16_t)PageInfo.Width-l;
				m_wct=(int16_t)PageInfo.Height-b;
				m_wcb=(int16_t)PageInfo.Height-t;
			}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 UTILS                                                          //
////////////////////////////////////////////////////////////////////////////////////////////////////
int16_t GetRealSizeKegl( /*CString**/const char* str, int16_t width, int16_t FontPointSize, int16_t FontNumber )
{
	char*  sz;
	uint16_t  PenaltyKeglForString=0;
	int16_t strHeight;
	float koef=1.;

	int len=strlen(str);
	if(FontPointSize>30) //~ в больших кеглях большая ошибка в Microsoft function
//		*str +="     ";
		len+=5;
	else
	if(FontPointSize>20)
		len++;
//		*str +=" ";

	if( !(FlagMode & NOBOLD) && ((char)FontNumber & TG_EDW_BOLD))
		koef = float(4./5.);

	sz=new char[len+1];
//	if( str->GetLength() < MAX_BUFFER_SIZE )
	strcpy(sz, str);
//	else
//		return FALSE;
	//Заполняем хвост строки пробелами
	memset(sz+strlen(str),' ',len-strlen(str));
	sz[len]=0;

	if( FontPointSize > ChangedKeglSize )
	{
		int Count = FontPointSize-ChangedKeglSize;
		for( int i=0; i<Count; i++ )
		{
			int16_t RealSize=GetRealSize(sz, strlen(sz), FontPointSize, FontNumber, &strHeight);
			if( RealSize > (int16_t)(width*Twips*koef) ) //криретий окончания итераций- чтобы все символы влезли в строку по ширине
			{
				PenaltyKeglForString++;
				FontPointSize--;
			}
			else
				break;
		}
	}
	delete [] sz;
	return FontPointSize;
}

//==Command - сама команда, value - числовой аргумент (-1 - нет)
void PutCom(const char *Command,int32_t value,int16_t space)
//==
{ char Num[10]; int16_t i,len;
	if(RtfWriteMode == FALSE)
  return;
  len=strlen_m(Command); do0(i,0,len-1) PutC(Command[i]);
  if(value >= 0)
  { sprintf(Num, "%d", value);
    len=strlen_m(Num); do0(i,0,len-1) PutChar(Num[i]);
  }
  if(space) PutChar(' ');
}
//==
void Put(const char *Data)
//==
{ int16_t i,len;
	if(RtfWriteMode == FALSE)
  return;
 len=strlen_m(Data); do0(i,0,len-1) PutC(Data[i]);
}

//==
void PutChar(uchar sym)
{ char s[10]; int16_t i,len;
	if(RtfWriteMode == FALSE)
  return;
  if(sym == '{' || sym == '}') return;
  if((uint)sym < (uint)0xC0 || sym == '}')
		{
			 if(sym == '\\')
     PutC(sym);
    PutC(sym);
		}
  else //Cyrillic
  { uint cc=(uint)sym;
    sprintf(s, "%x", (uint) sym);
    //itoa((uint)sym,s,16);
    PutC('\\');
    PutC('\'');
    len=strlen_m(s); do0(i,0,len-1) PutC(s[i]);
  }
}

#define SIZE_BLOC 1024
//==Return: >0 - код символа, 0 - END_OF_FILE
void PutC(char sym)
//==
{ static char *b; static int16_t len=-1,pos=-1;
	if(RtfWriteMode == FALSE)
  return;
  if(sym==0)
    {b=(char*)malloc(SIZE_BLOC);pos=-1;return;} //Init
  if(sym==1) //Вывод буферов
    {if(pos>=0) fwrite(b,pos+1,1,out); free(b); return;}
  if(pos == SIZE_BLOC-1)
    {fwrite(b,SIZE_BLOC,1,out);pos=-1;}
  b[++pos]=sym;
}

int16_t GetRealSize( char* str,int16_t len,int16_t FontSize,int16_t FontNumber,int16_t* strHeight )
{
	HFONT       testFont;
	SIZE        size;
	int         n_Weight=600,fn;
	uchar        bItalic;
//	TEXTMETRIC  tm;

	if( !(FlagMode & NOBOLD) && ((char)FontNumber & TG_EDW_BOLD))
		n_Weight=800;

	if(FlagMode & NOSIZE) FontSize=DefFontSize/2;

	if(!(FlagMode & NOCURSIV) && ((char)FontNumber & TG_EDW_ITALIC))
		bItalic = TRUE;
	else
		bItalic = FALSE;

//	CClientDC dc(AfxGetMainWnd());
	HWND wnd=GetActiveWindow();
	HDC dc=GetDC(wnd);

	fn = get_font_name(FontNumber);

	if( fn == 0 ) //without serif
		testFont=CreateFont((-20)*FontSize, 0, 0, 0, n_Weight, bItalic, FALSE, 0,
                      ANSI_CHARSET, OUT_DEFAULT_PRECIS,
                      CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                      DEFAULT_PITCH | FF_SWISS, "Arial");
	else
	if( fn == 3 ) //without serif
		testFont=CreateFont((-20)*FontSize, 0, 0, 0, n_Weight, bItalic, FALSE, 0,
                      ANSI_CHARSET, OUT_DEFAULT_PRECIS,
                      CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                      DEFAULT_PITCH | FF_SWISS, "Arial Narrow");

	else
	if( fn == 1 ) //with serif
		testFont=CreateFont((-20)*FontSize, 0, 0, 0, n_Weight, bItalic, FALSE, 0,
                      ANSI_CHARSET, OUT_DEFAULT_PRECIS,
                      CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                      DEFAULT_PITCH | FF_ROMAN , "Times New Roman");
	else          // fixed-pitch
		testFont=CreateFont((-20)*FontSize, 0, 0, 0, n_Weight, FALSE, FALSE, 0,
                     ANSI_CHARSET, OUT_DEFAULT_PRECIS,
                     CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                     DEFAULT_PITCH | FF_MODERN, "Courier");

	HFONT pOldFont = (HFONT)SelectObject(dc,testFont);
//	GetTextMetrics(dc,&tm);
	GetTextExtentPoint32(dc,str,strlen(str),&size);
	*strHeight = (int16_t)size.cy;
	SelectObject(dc,pOldFont);
	DeleteObject(testFont);
	ReleaseDC(wnd,dc);
	return (int16_t)size.cx; //in twips
}

int16_t get_font_name(int16_t FontNumber)
{
 if(FontNumber & TG_EDW_NARROW)
		 return 3;

 if(FontNumber & TG_EDW_GELV )
   return 0;

	if(FontNumber & TG_EDW_SERIF)
   return 1;

	return 2;
}


int16_t CreateEmptyRtfFile(void)
{
 const char  *TitleRtf="\\rtf1\\ansi \\deff0\\deflang1024";
 char Eol[3],Nname[260];
 int16_t i;
 FONT_COD  FontCod[9]={
 	"Arial Cyr"            , "fswiss",     // NonSerif
		"Times New Roman Cyr"  , "froman",     // Serif
		"Courier Cyr"          , "fmodern"     // Fixed_Pitch
	};
	uint16_t cr=13/*0x0d*/,lf=10/*0x0a*/;
 if(RtfWriteMode)
 {
	Eol[0]=(char)cr; Eol[1]=(char)lf; Eol[2]=0;
	if((out=fopen((char*)RtfFileName, "w")) == NULL) 	return -6;
 PutChar(0);

  Put("{");
		Put(TitleRtf);
		Put(" ");
		Put(Eol);
		//--WRITE Font Table--
  Put("{\\fonttbl"); Put(Eol);
  for(i=0; i < 3; ++i)
		{
			Put("{");
   PutCom("\\f",i,0);
			Put("\\");
			Put(FontCod[i].family);
		 PutCom("\\fcharset",0,0);
   Put(" ");
   Put(FontCod[i].name);
   Put(";}");
			Put(Eol);
		}
  Put("}");
  Put(Eol);
  Put("{\\info"); Put("{\\title WinTiger}"); Put("}"); Put(Eol);

//WRITE IMAGE INFORMATION
	Put("{\\*\\imagename \"");
 strcpy(Nname,(char*)WriteRtfImageName);
	Put(Nname);
	Put("\" \\imagepage");
// strcpy(Nname,WriteRtfPageNumber);
	Put("1");
 Put(";}");
 Put("}");

 PutChar(1);
  fclose(out);
	}
return TRUE;
}

Handle Rtf_CED_CreateParagraph(int16_t FirstIndent, int16_t LeftIndent,int16_t RightIndent,int16_t IntervalBefore, RtfSectorInfo *SectorInfo, int AlignParagraph, int m_Flag, int LengthStringInTwips, int LengthFragmInTwips)
{
	EDRECT  indent;
	EDBOX   playout;
	EDSIZE  interval;
	int     align;
	int     shad=-1;

	indent.left   = LeftIndent;
	indent.right  = RightIndent;
	indent.top    = FirstIndent;
	indent.bottom = 0;
	interval.cx = IntervalBefore;
	interval.cy = 0;
	playout.x   = -1;
	playout.w   = -1;
	playout.y   = -1;
	playout.h   = -1;

	align = AlignParagraph;
	if(align == RTF_TP_ONE)
	  align = RTF_TP_LEFT_ALLIGN;
	switch(align)
	{
	case RTF_TP_LEFT_ALLIGN:
			align = TP_LEFT_ALLIGN;
			break;
	case RTF_TP_RIGHT_ALLIGN:
			align = TP_RIGHT_ALLIGN;
			break;
	case RTF_TP_LEFT_AND_RIGHT_ALLIGN:
			align = TP_LEFT_ALLIGN | TP_RIGHT_ALLIGN;
			break;
	case RTF_TP_CENTER:
			align = TP_CENTER;
			break;
	}
	if(m_Flag & CSTR_STR_NEGATIVE){//nega
	   align = TP_CENTER;
	   shad  = 10000;
//#ifdef VMK002
		if((m_Flag & CSTR_STR_UPDOWN) || (m_Flag & CSTR_STR_DOWNUP))
		{
//			int addIndent =  SectorInfo->PaperW - SectorInfo->MargL - SectorInfo->MargR -
//				             LenthStringInTwips - indent.left - indent.right;
			int addIndent =  LengthFragmInTwips - LengthStringInTwips;
			if (addIndent > 0)
			{
				addIndent     = (int) (.9 * addIndent);
				indent.left  += addIndent / 2;
				indent.right += addIndent / 2;
				indent.top=0;
			}
		}
//#endif
	}
	return CED_CreateParagraph(SectorInfo->hEDSector, SectorInfo->hObject, align, indent,
 							    SectorInfo->userNum, -1, interval, playout, -1, shad, -1, -1, FALSE);
}

void  Rtf_CED_CreateChar( EDRECT* slayout, letterEx* Letter, CRtfChar* pRtfChar )
{
	if(RtfWriteMode)
		return;
	if(pRtfChar)
	{
	    int i;
		slayout->left       = pRtfChar->m_Realrect.left   +  TemplateOffset.x;
		slayout->right      = pRtfChar->m_Realrect.right  +  TemplateOffset.x;
		slayout->top        = pRtfChar->m_Realrect.top    +  TemplateOffset.y;
		slayout->bottom     = pRtfChar->m_Realrect.bottom +  TemplateOffset.y;
		for (i=0;i<pRtfChar->m_wCountAlt;i++)
		{
			Letter[i].alternative = pRtfChar->m_chrVersions[i].m_bChar;
			Letter[i].probability = pRtfChar->m_chrVersions[i].m_bProbability|1;
		}
		Letter[i-1].probability &= 0xFE;
	}
	else
	{
		slayout->left   = -1;
		slayout->right  = -1;
		slayout->top    = -1;
		slayout->bottom = -1;
		Letter->alternative = ' ';
		Letter->probability = 0;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 CheckVerLines                                                  //
////////////////////////////////////////////////////////////////////////////////////////////////////
#define LMin  500

Bool CheckLines(RECT* Rect, Bool FlagVer, RtfSectorInfo *SectorInfo)
{
// LinesTotalInfo     lti;
// Handle             hVH;
// Handle             hBlock;
// uint32_t             size;
// LineInfo           lineinfo;
 Handle             hPage;
 int32_t              VCentre,HCentre;
// extern Handle hUseCLine;
 uint32_t size_line_com=sizeof(LINE_COM);

	 if(FlagVer==TRUE && Rect->bottom-Rect->top<LMin/2)
   return FALSE;

  hPage  = CPAGE_GetHandlePage ( CPAGE_GetCurrentPage()  );
  CLINE_handle hCLINE = CLINE_GetMainContainer();
  HCentre = Rect->left+(Rect->right-Rect->left)/2;
  VCentre = Rect->top+(Rect->bottom-Rect->top)/2;

//   if(!LDPUMA_Skip(hUseCLine))
//   {
	 CLINE_handle hline;
	 CPDLine cpdata;
	 DLine data;
	 hline=CLINE_GetFirstLine(hCLINE);
	 if(!hline)
		 return FALSE;
     while(hline)
	 {
	  cpdata=CLINE_GetLineData(hline);
      if((!cpdata)||(FlagVer&&(cpdata->Dir==LD_Horiz))||((!FlagVer)&&(cpdata->Dir!=LD_Horiz)))
         hline=CLINE_GetNextLine(hline);
      else
	  {
        if( (cpdata->Flags  & LI_IsTrue)    &&
            !(cpdata->Flags & LI_IsAtTable) &&
            !(cpdata->Flags & LI_Used)      &&
            !(cpdata->Flags & LI_FRMT_Used) )
		{
  			if(FlagVer)
			{
					 if(cpdata->Line.Beg_X*Twips > Rect->left   && cpdata->Line.End_X*Twips > Rect->left   &&
					    cpdata->Line.Beg_X*Twips < Rect->right  && cpdata->Line.End_X*Twips < Rect->right  &&
						VCentre >cpdata->Line.Beg_Y*Twips       && VCentre < cpdata->Line.End_Y*Twips      &&
					    abs(cpdata->Line.Beg_Y - cpdata->Line.End_Y)*Twips > (Rect->bottom-Rect->top)/2 )
                          return TRUE;
			}
			else
			{//Hor
					 if( ((cpdata->Line.Beg_Y*Twips > Rect->top   && cpdata->Line.Beg_Y*Twips < Rect->bottom) ||
						 (cpdata->Line.End_Y*Twips > Rect->top   && cpdata->Line.End_Y*Twips < Rect->bottom)) &&
						 abs(cpdata->Line.Beg_X - cpdata->Line.End_X)*Twips > SectorInfo->PaperW/2 )
					 {
					  if(cpdata->Line.Beg_Y*Twips > Rect->top   && cpdata->Line.Beg_Y*Twips < Rect->bottom)
					  {
					   Rect->top = (int32_t)(cpdata->Line.Beg_Y*Twips);
					   Rect->bottom = (int32_t)(cpdata->Line.Beg_Y*Twips+10);
					  }
					  else
					  {
					   Rect->top = (int32_t)(cpdata->Line.End_Y*Twips);
					   Rect->bottom = (int32_t)(cpdata->Line.End_Y*Twips+10);
					  }
					  CLINE_CopyData(&data,cpdata,sizeof(DLine));
					  data.Flags |= LI_FRMT_Used;
                      CLINE_SetLineData(hline,(CPDLine)(&data));
                      return TRUE;
					 }
			}
		}
 	   hline=CLINE_GetNextLine(hline);
	  }
	 }
/*   }
   else
   {
   hBlock = CPAGE_GetBlockFirst ( hPage, RLINE_BLOCK_TYPE );
		while(hBlock)
		{
   size = CPAGE_GetBlockData( hPage, hBlock, RLINE_BLOCK_TYPE, &lti, sizeof(LinesTotalInfo));
   if ( size!=sizeof(LinesTotalInfo) )
			{ return FALSE; }

			if(FlagVer)
    hVH = CPAGE_GetBlockFirst ( hPage, (uint32_t)lti.Ver.Lns );
			else
    hVH = CPAGE_GetBlockFirst ( hPage, (uint32_t)lti.Hor.Lns );

    while(hVH)
			{
 			if(FlagVer)
     size = CPAGE_GetBlockData( hPage, hVH, (uint32_t)lti.Ver.Lns, &lineinfo, sizeof(lineinfo));
				else
     size = CPAGE_GetBlockData( hPage, hVH, (uint32_t)lti.Hor.Lns, &lineinfo, sizeof(lineinfo));

    if ( size!=sizeof(lineinfo) )
				{ return FALSE;	}

    if( (lineinfo.Flags  & LI_IsTrue)    &&
        !(lineinfo.Flags & LI_IsAtTable) &&
        !(lineinfo.Flags & LI_Used)      &&
        !(lineinfo.Flags & LI_FRMT_Used)
								)
				{
  			if(FlagVer)
					{
					 if( lineinfo.A.x*Twips > Rect->left   && lineinfo.B.x*Twips > Rect->left   &&
							   lineinfo.A.x*Twips < Rect->right  && lineinfo.B.x*Twips < Rect->right  &&
										VCentre >lineinfo.A.y*Twips       && VCentre < lineinfo.B.y*Twips      &&
							   abs(lineinfo.A.y - lineinfo.B.y)*Twips > (Rect->bottom-Rect->top)/2 )
      return TRUE;
					}
					else
     {//Hor
					 if( ((lineinfo.A.y*Twips > Rect->top   && lineinfo.A.y*Twips < Rect->bottom) ||
							    (lineinfo.B.y*Twips > Rect->top   && lineinfo.B.y*Twips < Rect->bottom)) &&
							   abs(lineinfo.A.x - lineinfo.B.x)*Twips > SectorInfo->PaperW/2 )
						{
						 if(lineinfo.A.y*Twips > Rect->top   && lineinfo.A.y*Twips < Rect->bottom)
							{
					   Rect->top = (int32_t)(lineinfo.A.y*Twips);
								Rect->bottom = (int32_t)(lineinfo.A.y*Twips+10);
								lineinfo.Flags |= LI_FRMT_Used;
        CPAGE_SetBlockData( hPage, hVH, (uint32_t)lti.Hor.Lns, &lineinfo, sizeof(lineinfo));
							}
							else
       {
					   Rect->top = (int32_t)(lineinfo.B.y*Twips);
								Rect->bottom = (int32_t)(lineinfo.B.y*Twips+10);
        lineinfo.Flags |= LI_FRMT_Used;
        CPAGE_SetBlockData( hPage, hVH, (uint32_t)lti.Hor.Lns, &lineinfo, sizeof(lineinfo));
							}
       return TRUE;
						}
					}
				}

 			if(FlagVer)
     hVH = CPAGE_GetBlockNext ( hPage,hVH, (uint32_t)lti.Ver.Lns );
				else
     hVH = CPAGE_GetBlockNext ( hPage,hVH, (uint32_t)lti.Hor.Lns );
			}
   hBlock = CPAGE_GetBlockNext( hPage, hBlock, RLINE_BLOCK_TYPE );
		}
   }
*/
 return FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                         Cleaning_LI_FRMT_Used_Flag                                             //
////////////////////////////////////////////////////////////////////////////////////////////////////
void Cleaning_LI_FRMT_Used_Flag(void)
{
// LinesTotalInfo     lti;
// Handle             hVH;
// Handle             hBlock;
// uint32_t             size;
// LineInfo           lineinfo;
 Handle             hPage;
// extern Handle hUseCLine;
 uint32_t size_line_com=sizeof(LINE_COM);

 hPage  = CPAGE_GetHandlePage ( CPAGE_GetCurrentPage()  );
 CLINE_handle hCLINE=CLINE_GetMainContainer();

//  if(!LDPUMA_Skip(hUseCLine))
//  {
  CLINE_handle hline;
  CPDLine cpdata;
  DLine data;
  hline=CLINE_GetFirstLine(hCLINE);
  if(!hline)
	return;
  while(hline)
  {
   cpdata=CLINE_GetLineData(hline);
   if(!cpdata)
         hline=CLINE_GetNextLine(hline);
      else
	  {
	   if(cpdata->Flags & LI_FRMT_Used)
	   {
		CLINE_CopyData(&data,cpdata,sizeof(DLine));
        data.Flags &= 0xffff7fff;
        CLINE_SetLineData(hline,(CPDLine)(&data));
	   }
	   hline=CLINE_GetNextLine(hline);
	  }
  }
/*  }
  else
  {
  hBlock = CPAGE_GetBlockFirst ( hPage, RLINE_BLOCK_TYPE );

		while(hBlock)
		{
   size = CPAGE_GetBlockData( hPage, hBlock, RLINE_BLOCK_TYPE, &lti, sizeof(LinesTotalInfo));
   if ( size!=sizeof(LinesTotalInfo) )
			{ return; }

    hVH = CPAGE_GetBlockFirst ( hPage, (uint32_t)lti.Hor.Lns );

    while(hVH)
			 {
     size = CPAGE_GetBlockData( hPage, hVH, (uint32_t)lti.Hor.Lns, &lineinfo, sizeof(lineinfo));
     if ( size!=sizeof(lineinfo) )
				  return;

     if(lineinfo.Flags & LI_FRMT_Used)
				 {
      lineinfo.Flags &= 0xffff7fff;
      CPAGE_SetBlockData( hPage, hVH, (uint32_t)lti.Hor.Lns, &lineinfo, sizeof(lineinfo));
				 }
     hVH = CPAGE_GetBlockNext ( hPage,hVH, (uint32_t)lti.Hor.Lns );
				}
	   hBlock = CPAGE_GetBlockNext( hPage, hBlock, RLINE_BLOCK_TYPE );
		}
 }*/
}
