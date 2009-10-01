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
// This file cration date: 24.02.99
//
// FrmtTabl.h
//
// ============================================================================
#ifndef __frmttabl_h__
#define __frmttabl_h__

#define  DefFontSize          24
#define  TablTrgaph           108
#define  TablMinKeglSize      12

#define  NOBOLD               0x0004
#define  NOCURSIV             0x0008
#define  NOSIZE               0x0020


//       wFont bits
#define  TG_EDW_SERIF         1
#define  TG_EDW_GELV          2      //без сериф.
#define  TG_EDW_BOLD          4
#define  TG_EDW_ITALIC        16
#define  TG_EDW_UNDERLINE     64
#define  TG_EDW_NARROW        128     // user defined

extern   float      Twips;
extern   uint32_t     FlagMode;
extern   char       lpMyNameSerif[MAX_PATH];
extern   char       lpMyNameNonSerif[MAX_PATH];
extern   char       lpMyNameMono[MAX_PATH];
extern   int16_t      get_font_name(int16_t FontNumber);

#ifdef __cplusplus
	extern "C" {
#endif

uint32_t GetTablCount(void);
void   ViewTable(void);
void   GetTableRect( uint32_t NumberTable , Rect16* RectTable,uint32_t* UserNumber );
Bool   WriteTable( uint32_t IndexTable, RtfSectorInfo* SectorInfo/*, CString* TableString*/ ,Bool OutPutMode );
//void   TablePutChar( CString* TableString, uchar sym );

#ifdef __cplusplus
	}
#endif

class CTableRow;
class CTablePhCell;
class CTableString;
class CTableWord;
class CTableChar;

void  GetFragmLines( int32_t m_NumberFragm , Point32* m_point );
Bool  CheckLine( CSTR_line* Comingline );
void  AddLetter( CSTR_rast* rast , uchar*  m_ucCode , uchar* m_ucFontNumber );
void  Write_SetPhCellParam( Handle h_Page, Handle h_Table, CTableRow* rTableRow, Point32 m_TempPhPoint,
																										 int32_t CountStrCellInPn, int32_t FlagPhisicalGroup , int32_t NumberPhysicalGroup );
/////////////////////////////////////////////////////////////////////////////
//Класс для таблицы
class CMyTable //: public CObject
{
public:
  CMyTable();
 ~CMyTable();

  CTableRow*    GetCurrentTableRow( void );
  void          Write(/*CString* TableString,*/RtfSectorInfo* SectorInfo,Bool OutPutMode);

  Rect32        RectTable;
  std::vector<CTableRow*>/*CObArray*/	    m_arTableRows;
  uint16_t          m_wIndex;
  int32_t         m_CountRows;
  int32_t         m_CountColumns;
};

/////////////////////////////////////////////////////////////////////////////
//Класс для строки таблицы
class CTableRow// : public CObject
{
public:
  CTableRow();
 ~CTableRow();

  CTablePhCell* GetCurrentPhCell( void );
  void	        Write(/*CString* TableString,*/RtfSectorInfo* SectorInfo);

  std::vector<CTablePhCell*>/*CObArray*/	    m_arPhCells;
  uint16_t		    m_wHeight;
  uint16_t          m_wPosX;
  uint16_t          m_wPosY;
  uint16_t          m_wIndex;
  uint16_t          m_wCountPhCells;
  Handle        m_hEdTableRow;
};

/////////////////////////////////////////////////////////////////////////////
//Класс для ячейки таблицы
class CTablePhCell// : public CObject
{
public:
  CTablePhCell();
 ~CTablePhCell();

  CTableString* GetCurrentString( void );
  void          SetPhCellParam(Bool32 FlagTerminalPhCell, int32_t   NumberPhysicalGroup ,
  Bool32        FlagPhisicalGroup , Point32 m_TempPhPoint      );
  void          FillPhCell(int32_t m_FragmentNumber);
  void          Write_Header(/*CString* TableString,*/ RtfSectorInfo* SectorInfo);
  void          Write(/*CString* TableString,*/ RtfSectorInfo* SectorInfo);
  void          SetPhCellFragmentFormattingAndAlignment();
  void          SetPhCellFlag( void );

  std::vector<CTableString*>/*CObArray*/	    m_arStrings;
  std::vector <RECT> RectFragmentInPhCell;

  Rect32        RectPhCell;//Здесь хранятся координаты прямоугольника - ГРАНИЦ ячейки (кажется в пикселях)
  Rect32        RectTable;
  Point32       m_PhCellPoint;

  uint16_t	        m_wIndex;
  Bool32	    m_FlagTerminalPhCell;
  Bool32        m_FlagPhisicalGroup;
  int32_t         m_NumberPhysicalGroup;
  int32_t         m_FragmentNumber;
  int32_t         m_LeftIndent;
  uint16_t     	    m_wCountStrings;
  Bool32        m_PhCellFragmentFormatting;       //Hor. formatting
  Bool32        m_PhCellFragmentAlignment;        //Ver. formatting
  Handle        m_hEdTableCell;
uint32_t         m_Flags;

  union
  {
	uchar		m_u8BoundsType[4];//Информация о типе границ ячейки:L,R,T,B !!!Art
	uint32_t		m_u32BoundsType;
  };
};

/////////////////////////////////////////////////////////////////////////////
// Класс для строки ячейки таблицы
class CTableString//: public CObject
{
public:
  CTableString();
 ~CTableString();

  CTableWord*   GetCurrentWord( void );
  void  	    ExtractWordsFromString( CSTR_line line );
  void          ExtractNextWord( CSTR_line* line );
  void	        Write(/*CString* TableString,*/ RtfSectorInfo* SectorInfo);
  void          MyCorrectKegl( int16_t width );
  int16_t         GetRealSizeForKegl( char* str,int16_t FontSize,int16_t FontNumber );

  /*CObArray*/std::vector<CTableWord*>      m_arWords;

  uint16_t	        m_wIndex;
  uint16_t          m_wCountWords;
uint32_t         m_Flags;
};

/////////////////////////////////////////////////////////////////////////////
// CTableWord class
class CTableWord//: public CObject
{
public:
  CTableWord();
 ~CTableWord();

  CTableChar*   GetCurrentChar(void);
  void          AddLetter2Word( CSTR_rast* rast );
  void          SetWordRectAndProb( CSTR_rast* rast );
  void	        Write(/*CString* TableString,*/ RtfSectorInfo* SectorInfo);
  void          WriteWordForCorrectKegl(char* TmpTableString);

  std::vector<CTableChar*>/*CObArray*/      m_arChars;
  RECT	        m_rectWord;

  uint16_t	        m_wCountChars;
  uint16_t	        m_wIndex;
  uint16_t	        m_wEDIndex;
  uint16_t	        m_wFontNumber;
  uint16_t	        m_wFontPointSize;
  uint16_t          m_wUnderline;
  uint16_t          m_wBold;
  uint16_t          m_wItalic;
  uint16_t          m_wProb;
  uchar          m_bSpell;
uint32_t         m_Flags;
};

/////////////////////////////////////////////////////////////////////////////
// CTableChar class
class CTableChar//: public CObject
{
public:
  CTableChar();
 ~CTableChar();

  void          AddingLetter( CSTR_rast* rast ,int index );
  Bool	        Write();

  RECT	        m_rectChar;
  struct
  {
  uchar	        m_bChar;
  uchar	        m_bProbability;
  } m_chrVersions[REC_MAX_VERS];//!!! Art
  uchar          m_blanguage;
  uchar          m_bFlg_spell;
  uint16_t          m_wCountAlt;
  uint16_t          m_wFontNumber;
  uint16_t	        m_wIndex;
uint32_t         m_Flags;
 };

#endif

