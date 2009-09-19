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
// This file cration date: 10.18.98
//
// FrmtFile.cpp
//
// ============================================================================
#include <stdlib.h>
#include "stdafx.h"
#include <cstring>
//#include <afxtempl.h>
#include "cpage.h"
#include "cpagetyps.h"
#include "cstr.h"
#include "rfrmt_prot.h"
#include "rfrmtfile.h"
#include "polyblock.h"
#include "aldebug.h"
#include "dpuma.h"
#include "resource.h"

#include "minmax.h"

#ifdef alDebug
extern std::vector<RECT>   *pInputArray;
#endif

int32_t	PageIncline2048 = 2048;
uint32_t  CountPict=0,CountTable=0;
void    SetReturnCode_rfrmt(uint16_t rc);
POINT   TemplateOffset;

extern "C" uint32_t   GetPictCount(void);
extern "C" uint32_t   GetTablCount(void);
extern  uchar       Frmt_CharSet;
extern  int16_t       CreateEmptyRtfFile(void);
extern  char        UnRecogSymbol;
extern  uint32_t      FlagMode;
extern  uint32_t      ExFlagMode;

#define HalfDefMargL   900 // Left margin in twips    (the default is 1800).
#define HalfDefMargT   720 // Top  margin in twips    (the default is 1440).

/////////////////////////////////////////////////////////////////////////////
//                    CreateInternalFileForFormatter
Bool CreateInternalFileForFormatter(FILE *pIFName)
{
 Bool         FReturnCode = TRUE;
 CSTR_line    line;
 CFPage       Page;
 PAGEINFO     PageInfo;

 Handle hCPAGE = CPAGE_GetHandlePage( CPAGE_GetCurrentPage());
 GetPageInfo(hCPAGE,&PageInfo);

 if(PageInfo.X && PageInfo.Y){
	 TemplateOffset.x = PageInfo.X;
     TemplateOffset.y = PageInfo.Y;
 }
 else{
	 TemplateOffset.x = 0;
     TemplateOffset.y = 0;
 }

 //Создание массива текстовых фрагментов
 Page.FragmentsArray.clear();
 Page.CreateArray_For_TextFragments( );

 //Чтение информации из хранилиша
 line = CSTR_FirstLine (1);

 while(line)
 {
	 if(CheckComingLine(&line) && Page.CheckComingLine_For_TextFragments(&line))
  	    Page.ProcessingComingLine(&line);
     line = CSTR_NextLine ( line, 1 );
 }

 CountPict  = GetPictCount();
// CountTable = GetTablCount();

 if(Page.Count.Chars>32000){
    ExFlagMode = TRUE;
 }

 if(!(Page.Count.Frags + (int)CountPict + (int)CountTable)){
	if(CreateEmptyRtfFile()<0){
 	   SetReturnCode_rfrmt(IDS_ERR_SIZEFRAGMENT);
 	   return FALSE;
    }
	return TRUE;
 }

 //Создания внутреннего файла для форматирования
 uint16_t  DistanceBetweenLeftMargin = 15;
 uint16_t  DistanceBetweenTopMargin  =  5;
 Page.m_wDpi             = (uint16_t)PageInfo.DPIX;
 PageIncline2048         = PageInfo.Incline2048;

 InternalRect VRect;
 fwrite(&Page.m_wDpi,              sizeof(uint16_t),1,pIFName); // ScanResolution
 fwrite(&Page.Count.Frags,         sizeof(uint16_t),1,pIFName); // число колонок
 fwrite(&Page.Count.Chars,         sizeof(uint16_t),1,pIFName); // число знакомест  in all page
 fwrite(&Page.Count.Words,         sizeof(uint16_t),1,pIFName); // число слов
 fwrite(&Page.Count.Strings,       sizeof(uint16_t),1,pIFName); // число строк
 fwrite(&DistanceBetweenLeftMargin,sizeof(uint16_t),1,pIFName); // distance between left margin letters- neighdoors from one word
 fwrite(&DistanceBetweenTopMargin, sizeof(uint16_t),1,pIFName); // distance between top margin

 CFragment* cFrag;
 CFString*  cString;
 CWord*     cWord;
 CChar*     cChar;

 // Processing Fragments
 for(int iPage=0; iPage < Page.Count.Frags; iPage++){
    cFrag = (CFragment*)Page.m_arFrags[iPage];
    VCopyRect( &VRect, &cFrag->m_rectFrag);
	if(CheckRect( &VRect ) == FALSE)
	  FReturnCode = FALSE ;
    fwrite(&VRect , sizeof( InternalRect ),1,pIFName);
    fwrite(&cFrag->m_wStringsCount, sizeof( uint16_t  ),1,pIFName);
    fwrite(&cFrag->m_wUserNumber  , sizeof( uint32_t ),1,pIFName);
    fwrite(&cFrag->m_Flags        , sizeof( uint32_t ),1,pIFName);

   // Processing Strings
   for(int iFrag=0; iFrag < cFrag->m_wStringsCount; iFrag++){
      cString = (CFString*)cFrag->m_arStrings[iFrag];
      VCopyRect( &VRect, &cString->m_rectBaseLine );
      fwrite( &VRect, sizeof( InternalRect ),1,pIFName);
      VCopyRect( &VRect, &cString->m_rectString );
      fwrite( &VRect, sizeof( InternalRect ),1,pIFName);
      fwrite( &cString->m_wWordsCount,  sizeof( uint16_t ),1,pIFName);
	  fwrite( &cString->S_Flags,  sizeof( uint32_t ),1,pIFName); //NEGA_STR
      // Processing Words
      for(int iString=0; iString < cString->m_wWordsCount; iString++){
         cWord = (CWord*)cString->m_arWords[iString];
         fwrite( &cWord->m_wCharsCount,   sizeof( uint16_t ),1,pIFName);
         fwrite( &cWord->m_wFontNumber,   sizeof( uint16_t ),1,pIFName);
         fwrite( &cWord->m_wFontPointSize,sizeof( uint16_t ),1,pIFName);

         // Processing Letters
         for(int iWord=0; iWord < cWord->m_wCharsCount; iWord++){
            cChar = (CChar*)cWord->m_arChars[iWord];
            VCopyRect( &VRect, &cChar->m_rectChar );
            fwrite( &VRect , sizeof( InternalRect ),1,pIFName);
            VCopyRect( &VRect, &cChar->m_RealRectChar );
            fwrite( &VRect , sizeof( InternalRect ),1,pIFName);
            fwrite( &cChar->m_wCountAlt ,    sizeof( uint16_t ),1,pIFName);
			for (int iAlt=0;iAlt<cChar->m_wCountAlt&&iAlt<REC_MAX_VERS;iAlt++)
			{
				fwrite( &cChar->m_chrVersions[iAlt].m_bChar ,sizeof( char ),1,pIFName);
				fwrite( &cChar->m_chrVersions[iAlt].m_bProbability , sizeof( char ),1,pIFName);
			}
            fwrite( &cChar->m_blanguage ,    sizeof( char ),1,pIFName);
            fwrite( &cChar->m_bFlg_spell_nocarrying, sizeof( char ),1,pIFName);
            fwrite( &cChar->m_bFlg_cup_drop ,sizeof( char ),1,pIFName);
            fwrite( &cChar->m_bFlg_spell,    sizeof( char ),1,pIFName);
            fwrite( &cChar->m_wFontNumber ,  sizeof( char ),1,pIFName);
         }
      }
   }
 }

 return	FReturnCode;
}

/////////////////////////////////////////////////////////////////////////////
// CPage class implementation
CFPage::CFPage()
{
  Count.Frags = Count.Strings = Count.Words = Count.Chars = 0;
  m_nIndex = 0;
  m_nPrevFragNumber = -1;
}

CFPage::~CFPage()
{
 CFragment* cFrag;

 for(int i=0; i<Count.Frags;i++){
	 cFrag = (CFragment*)m_arFrags[i];
	 delete cFrag;
 }
}

/////////////////////////////////////////////////////////////////////////////
//                    CFPage::CreateArray_For_TextFragments
void CFPage::CreateArray_For_TextFragments( )
{
 Handle hPage  = CPAGE_GetHandlePage( CPAGE_GetCurrentPage() );
 Handle hBlock = CPAGE_GetBlockFirst( hPage , 0 );

 while( hBlock )
 {
    if(CPAGE_GetBlockType( hPage , hBlock ) == TYPE_TEXT){
       uint32_t BlockNumber = CPAGE_GetBlockInterNum( hPage, hBlock);
       FragmentsArray.push_back( BlockNumber );
    }
	hBlock = CPAGE_GetBlockNext( hPage , hBlock , 0);
 }
}

/////////////////////////////////////////////////////////////////////////////
//                    CFPage::CheckComingLine_For_TextFragments
Bool CFPage::CheckComingLine_For_TextFragments( CSTR_line* line )
{
 int32_t CurFragNumber,SizeArrayTextFragments,i;
 CSTR_attr  attr;

 if( !CSTR_GetLineAttr( *line , &attr ) )
   return 0;
 CurFragNumber = attr.fragment;
 SizeArrayTextFragments = FragmentsArray.size();

 for( i=0; i<SizeArrayTextFragments; i++ )
  if( CurFragNumber ==(int32_t)FragmentsArray[i])
    return 1;

 return 0;
}

CFragment* CFPage::GetFirstFrag()
{
 m_nIndex = 0;
 return (CFragment*)m_arFrags[m_nIndex];
}

CFragment* CFPage::GetNextFrag()
{
 if(++m_nIndex==Count.Frags)
    return NULL;
 return (CFragment*)m_arFrags[m_nIndex];
}

/////////////////////////////////////////////////////////////////////////////
//                    CFPage::ProcessingComingLine
void CFPage::ProcessingComingLine( CSTR_line* Comingline )
{
 CSTR_line  line;
 CSTR_attr  attr;
 CFragment* Fragment;
 Handle     hCPAGE;
 Handle     hBlock;
 POLY_      poly;

 line = *Comingline;
 if( !CSTR_GetLineAttr( line , &attr ) || attr.Flags&CSTR_STR_EMPTY )
   return;
 m_nCurFragNumber = attr.fragment;

 if( m_nCurFragNumber != m_nPrevFragNumber ){
    m_arFrags.push_back( new CFragment() );
	Fragment = m_arFrags[Count.Frags];
    assert(Fragment);

	hCPAGE        = CPAGE_GetHandlePage( CPAGE_GetCurrentPage());
	hBlock        = CPAGE_GetBlockFirst( hCPAGE, NULL );

	while(hBlock)
    {
		if(CPAGE_GetBlockInterNum(hCPAGE,hBlock)==(uint32_t)m_nCurFragNumber){
		   Fragment->m_wUserNumber = (uint32_t)CPAGE_GetBlockUserNum( hCPAGE, hBlock  );
           Fragment->m_Flags = attr.Flags; //nega
           if(attr.Flags == CSTR_STR_NEGATIVE){ //nega_str
 		      CPAGE_GetBlockData(hCPAGE,hBlock,TYPE_TEXT, &poly, sizeof(POLY_));
              Fragment->m_rectFrag.left   = poly.com.Vertex[0].x - TemplateOffset.x;
              Fragment->m_rectFrag.right  = poly.com.Vertex[2].x - TemplateOffset.x;
              Fragment->m_rectFrag.top    = poly.com.Vertex[0].y - TemplateOffset.y;
              Fragment->m_rectFrag.bottom = poly.com.Vertex[2].y - TemplateOffset.y;
           }
		   break;
        }
		hBlock = CPAGE_GetBlockNext(hCPAGE,hBlock, NULL );
    }
 	assert(hBlock!=NULL);
    ++Count.Frags;
 }

 AddString(Comingline);
 m_nPrevFragNumber = m_nCurFragNumber;
}

/////////////////////////////////////////////////////////////////////////////
//                    CFPage::AddString
void CFPage::AddString(CSTR_line* Comingline)
{
 CFragment*     Fragment;
 Fragment = (CFragment*)m_arFrags[Count.Frags-1];
 assert(Fragment);
 Fragment->AddString( Comingline, &Count);
}

/////////////////////////////////////////////////////////////////////////////
//                    CFPage::Write
Bool CFPage::Write()
{
 for( CFragment* pFrag = GetFirstFrag(); pFrag!=NULL; pFrag=GetNextFrag() )
 {
	 assert(pFrag);
	 pFrag->Write();
 }
 return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CFragment class implementation
CFragment::CFragment()
{
 m_Flags = 0;
 m_wStringsCount = 0;
 SetRect(&m_rectFrag, 0, 0, 0, 0 );
}

CFragment::~CFragment()
{
 CFString* cString;

 for( int i=0; i<m_wStringsCount; i++ )
 {
	 cString = (CFString*)m_arStrings[i];
	 delete cString;
 }
}

/////////////////////////////////////////////////////////////////////////////
//                    CFragment::AddString
void CFragment::AddString( CSTR_line* Comingline, PageElementCount* Count)
{
 CFPage    Page;
 CSTR_line line;
 CSTR_attr line_attr;
 CFString* String;
 RECT     TmpRect,*PrevStringRect;
 uchar      Flag_Continuation_Strings = FALSE;
 uchar      dist;

 if(CheckLineForFilling(Comingline)==FALSE)
	return;
 line = *Comingline;
 CSTR_GetLineAttr(line , &line_attr);
 SetRect(&TmpRect, line_attr.col - TemplateOffset.x ,
                 line_attr.row - TemplateOffset.y,
                 line_attr.col - TemplateOffset.x+ line_attr.wid,
  				 line_attr.row - TemplateOffset.y + line_attr.hei);
 UnionRect(&m_rectFrag, &m_rectFrag, &TmpRect);
 dist = line_attr.hei/2;

 if( m_wStringsCount>0 ) {
   String = (CFString*)m_arStrings[m_wStringsCount-1];
   PrevStringRect=&String->m_rectString ;
   if( TmpRect.top < PrevStringRect->bottom  &&  TmpRect.left > PrevStringRect->right &&
	   abs(TmpRect.top    - PrevStringRect->top)    <=dist &&
       abs(TmpRect.bottom - PrevStringRect->bottom) <=dist )
       Flag_Continuation_Strings = TRUE;
 }

 if(!Flag_Continuation_Strings)	{
	++m_wStringsCount;
	++Count->Strings;
	m_arStrings.push_back( new CFString() );
	String = m_arStrings[m_wStringsCount-1];
	String->S_Flags = line_attr.Flags; //NEGA_STR
 }

 assert(String);
 String->ExtractWordsFromString( Comingline ,Count );
}

/////////////////////////////////////////////////////////////////////////////
//                    CheckLineForFilling
Bool CheckLineForFilling( CSTR_line* Comingline )
{
 CSTR_line	     line;
 CSTR_rast       rast;
 RecVersions     vers;
 CSTR_rast_attr	 rast_attr;

 line = *Comingline;
 rast =  CSTR_GetFirstRaster( line );                              // I фикт. растр
 rast =  CSTR_GetNextRaster( rast , CSTR_f_all );

 while( rast )
 {
	 CSTR_GetCollection( rast,&vers );
     CSTR_GetAttr( rast,&rast_attr );

     if((rast_attr.flg&CSTR_f_dust) ||  (rast_attr.flg&CSTR_f_fict ) ||//Дусты и
	    ((rast_attr.flg&CSTR_f_space) &&
	    (!vers.Alt[0].Code || vers.Alt[0].Code == 0x1e ||
		vers.Alt[0].Code == 0x1f)))                                 //полупробелы  выкидываем.
     goto next_raster;
     return TRUE;

     next_raster:
	  rast = CSTR_GetNextRaster( rast , CSTR_f_all );
 }

 return FALSE;
}


Bool CFragment::Write()
{
 return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CFString class implementation
CFString::CFString()
{
 m_wWordsCount = 0;
}

CFString::~CFString()
{
 CWord* cWord;

 for( int i=0; i<m_wWordsCount; i++ )
 {
	 cWord = (CWord*)m_arWords[i];
	 delete cWord;
 }
}

/////////////////////////////////////////////////////////////////////////////
//                   CFString::ExtractWordsFromString
void CFString::ExtractWordsFromString( CSTR_line* Comingline , PageElementCount* Count)
{
 CSTR_rast_attr	 rast_attr;
 CSTR_rast       rast;
 RecVersions     vers;
 CSTR_line	      line;
 CSTR_attr       line_attr;
 CWord*          CurrentWord;
 const char*     SeparatorsWord=" ";
 const char*     result=NULL;
 int16_t             FlagWord;
 Bool            FlagString,FlagCapDrop;

 line        = *Comingline;
 FlagWord    = 0;
 FlagString  = FALSE;
 FlagCapDrop = FALSE;

 CSTR_GetLineAttr( line , &line_attr );
 if(line_attr.Flags & CSTR_STR_CapDrop) //буквица - см. первая буква в сказках
    FlagCapDrop = TRUE;

 SetRect(&m_rectBaseLine, line_attr.bs1, line_attr.bs2,
                        line_attr.bs3, line_attr.bs4); //don't used now
 SetRect(&m_rectString, line_attr.col - TemplateOffset.x,
                      line_attr.row - TemplateOffset.y,
	                  line_attr.col - TemplateOffset.x + line_attr.wid,
                      line_attr.row - TemplateOffset.y + line_attr.hei);

 #ifdef alDebug //obsolete option
 {
  RECT rect;
  SetRect(&rect,line_attr.col, line_attr.row,
                         line_attr.col + line_attr.wid,
					     line_attr.row + line_attr.hei);
  pInputArray->push_back(rect);
 }
 #endif

 rast =  CSTR_GetFirstRaster( line );            // I фикт. растр
 rast =  CSTR_GetNextRaster( rast , CSTR_f_all );

 while( rast )
 {
	 CSTR_GetCollection( rast,&vers );
     CSTR_GetAttr( rast,&rast_attr );
     //#define NEG_HALF_SPACE  0x1e
     //#define POS_HALF_SPACE  0x1f
     //#define REGULAR_SPACE   0x20

     if((rast_attr.flg&CSTR_f_dust) ||  (rast_attr.flg&CSTR_f_fict ) ||//Дусты и
	    ((rast_attr.flg&CSTR_f_space) &&
	    (!vers.Alt[0].Code || vers.Alt[0].Code == 0x1e ||
	    vers.Alt[0].Code == 0x1f)))//полупробелы  выкидываем.
      goto next_raster;

      FlagString = TRUE;
	  result = strchr( SeparatorsWord, vers.Alt[0].Code );
	  if( FlagWord ){   // word openly
	    if(result)      // word is broken
	       FlagWord =0;
	    else{           // word lasts
    	   assert( CurrentWord );
           CurrentWord->AddLetter2Word( &rast , Count ,&FlagCapDrop );
        }
      }
	  else{             // expect new word
        if(!result){  // beginning word
          ++m_wWordsCount;
          ++Count->Words;
	      FlagWord = 1;
          m_arWords.push_back( new CWord() );
          CurrentWord = m_arWords[m_wWordsCount-1];
          assert( CurrentWord );

   	      if(rast_attr.font_spec == CSTR_fs_courier)
             CurrentWord->m_wFontNumber     = rast_attr.font & 0xFC;
          else
             CurrentWord->m_wFontNumber     = rast_attr.font;

		  CurrentWord->m_wFontPointSize  = rast_attr.keg;
          CurrentWord->AddLetter2Word( &rast , Count ,&FlagCapDrop );
          if(FlagCapDrop){
            CurrentWord->m_wFontPointSize = 14;
		    FlagCapDrop = FALSE;
		    FlagWord    = 0;
          }
        }
      }
      next_raster:
	   rast = CSTR_GetNextRaster( rast , CSTR_f_all );
 }

 if(FlagString == FALSE)
   Count->Strings--;
}

/////////////////////////////////////////////////////////////////////////////
// CWord class implementation
CWord::CWord()
{
 m_wCharsCount = 0;
}

CWord::~CWord()
{
CChar* cChar;

 for( int i=0; i<m_wCharsCount; i++ )
 {
	 cChar = (CChar*)m_arChars[i];
	 delete cChar;
 }
}

/////////////////////////////////////////////////////////////////////////////
//                   CWord::AddLetter2Word
void CWord::AddLetter2Word( CSTR_rast* rast , PageElementCount* Count ,Bool* FlagCapDrop)
{
 CChar* CurrentChar;
 UniVersions	vers;

 CSTR_GetCollectionUni( *rast,&vers );
 for(int i=0; i<3; i++)
 {
	if(!vers.Alt[0].Code[i])
      break;
	++m_wCharsCount;
    ++Count->Chars;

    m_arChars.push_back( new CChar() );
    CurrentChar = m_arChars[m_wCharsCount-1];
    assert( CurrentChar );
    CurrentChar->AddingLetter( rast ,i,FlagCapDrop);
 }
}

/////////////////////////////////////////////////////////////////////////////
// CChar class implementation
CChar::CChar()
{
}

CChar::~CChar()
{
}

/////////////////////////////////////////////////////////////////////////////
//                   CChar::AddingLetter
void CChar::AddingLetter(CSTR_rast* rast, int index, Bool* FlagCapDrop)
{
 CSTR_rast_attr	 rast_attr;
 UniVersions	vers;

 CSTR_GetCollectionUni( *rast,&vers );
 CSTR_GetAttr( *rast,&rast_attr );

 SetRect(&m_rectChar, rast_attr.col - TemplateOffset.x,
 	                rast_attr.row - TemplateOffset.y,
	                rast_attr.col - TemplateOffset.x + rast_attr.w,
                    rast_attr.row - TemplateOffset.y + rast_attr.h);

 SetRect(&m_RealRectChar, rast_attr.r_col - TemplateOffset.x,
 	                    rast_attr.r_row - TemplateOffset.y,
	                    rast_attr.r_col - TemplateOffset.x + rast_attr.w,
                        rast_attr.r_row - TemplateOffset.y + rast_attr.h);

 m_wCountAlt  = MIN(vers.lnAltCnt,REC_MAX_VERS);
 m_bFlg_spell = 0;

 if(!vers.lnAltCnt)
 {
	m_wCountAlt=1;
	m_chrVersions[0].m_bChar = UnRecogSymbol;
	m_chrVersions[0].m_bProbability = 0;
 }
 else
 {
	for (int i=0;i<m_wCountAlt;i++)
	{
		m_chrVersions[i].m_bChar = vers.Alt[i].Code[index];
		m_chrVersions[i].m_bProbability = vers.Alt[i].Prob;
	}
	Frmt_CharSet = vers.Alt[0].Charset;
 }

 if( rast_attr.flg_spell & CSTR_fa_spell_nocarrying )
   m_bFlg_spell_nocarrying = 0;
 else
   m_bFlg_spell_nocarrying = 1;

 if(rast_attr.flg_spell & CSTR_fa_spell_reject)
   m_bFlg_spell = 0;
 else
   m_bFlg_spell = 1;

 m_blanguage = rast_attr.language;

 if(rast_attr.font_spec == CSTR_fs_courier)
   m_wFontNumber = rast_attr.font & 0xFC;
 else
   m_wFontNumber = rast_attr.font;

 if(*FlagCapDrop)
 	m_bFlg_cup_drop = TRUE;
 else
 	m_bFlg_cup_drop = FALSE;
}

void VCopyRect(InternalRect* Inner, RECT* Outer)
{
 Inner->top    = (uint16_t)Outer->top;
 Inner->bottom = (uint16_t)Outer->bottom;
 Inner->left   = (uint16_t)Outer->left;
 Inner->right  = (uint16_t)Outer->right;
}

/////////////////////////////////////////////////////////////////////////////
//                   CheckRect
Bool CheckRect(InternalRect* Inner)
{
 char      str[500];

 if( Inner->top >= Inner->bottom || Inner->left >= Inner->right ){
   sprintf(str, " Left = %d, Right = %d, Top = %d, Bottom = %d ",
   Inner->left,Inner->right,Inner->top,Inner->bottom );

   LDPUMA_Console(str);
   SetReturnCode_rfrmt(IDS_ERR_SIZEFRAGMENT);
   return FALSE;
 }

 return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//                   CheckComingLine
Bool CheckComingLine( CSTR_line* Comingline )
{
 CSTR_rast       rast;
 CSTR_line	     line;

  line     = *Comingline;
  rast =  CSTR_GetFirstRaster( line );
  assert( rast );
  rast = CSTR_GetNextRaster( rast , CSTR_f_all );
  if(rast)
    return TRUE;

  return FALSE;
}




















