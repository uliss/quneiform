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
// This file cration date: 12.08.99
//
// CreateRtf.h
//
// ============================================================================

#ifndef __CreateRtf_h__
#define __CreateRtf_h__

#include "rtfedwrite.h"
#include "recdefs.h"
#include "compat_defs.h"

#define TIRE                  0x97        //'-'

//Fragment output types
#define FOT_SINGLE            0
#define FOT_FRAME             1
#define FOT_SINGLE_IN_COLUMN  2

//Fragment types
#define FT_TEXT               0
#define FT_FRAME              1
#define FT_TABLE              2
#define FT_PICTURE            3

//HorizontalColumn types
#define HC_SingleTerminal     0
#define HC_AllTerminal        1
#define HC_FrameAndColumn     2
#define HC_AllFrame           3
#define HC_ONLY_PICTURE_TABLE 4

#define MaxFontSize           72
#define DefFontSize           24
#define MAX_BUFFER_SIZE       2048
#define ChangedKeglSize       6

//       wFont bits
#define TG_EDW_SERIF          1      //сериф.
#define TG_EDW_GELV           2      //без сериф.
#define TG_EDW_BOLD           4
#define TG_EDW_ITALIC         16
#define TG_EDW_UNDERLINE      64
#define TG_EDW_NARROW         128     // user defined

#define NOBOLD                0x0004
#define NOCURSIV              0x0008
#define NOSIZE                0x0020

// Formatting Mode
#define USE_NONE              0x0040   // no formatting
#define USE_FRAME_AND_COLUMN  0x0001   // use columns & frames
#define USE_FRAME             0x0002   // use only frames

// Size A4
#define DefaultWidthPage      11906
#define DefaultHeightPage     16838

#define DefMargL              1800 // Left   margin in twips    (the default is 1800).
#define DefMargR              1800 // Right  margin in twips    (the default is 1800).
#define DefMargT              1440 // Top    margin in twips    (the default is 1440).
#define DefMargB              1440 // Bottom margin in twips    (the default is 1440).

#define RTF_TP_LEFT_ALLIGN           0 //- выравниваение по левому краю
#define	RTF_TP_RIGHT_ALLIGN          1 //- выравнивание по правому краю
#define RTF_TP_LEFT_AND_RIGHT_ALLIGN 2 //- выравнивание по ширине
#define RTF_TP_CENTER                3 //- выравнивание по центру
#define RTF_TP_ONE                   4 //- каждая строка состоит из одного слова, выравнивание левому краю
#define RTF_TP_TYPE_LINE             5 //- список

class  CRtfSector;
class  CRtfFragment;
class  CRtfHorizontalColumn;
class  CRtfVerticalColumn;
class  CRtfParagraph;
class  CRtfString;
class  CRtfWord;
class  CRtfChar;

struct RtfPageElementCount
{
 uint16_t  RtfSectors;
 uint16_t  RtfTextFragments;
	uint16_t  RtfFrameTextFragments;
 uint16_t  RtfPictureFragments;
 uint16_t  RtfTableFragments;
 uint16_t  RtfStrings;
 uint16_t  RtfWords;
 uint16_t  RtfChars;
};

typedef struct tagKEGL {
   int16_t OldKegl;
   int16_t NewKegl;
			int16_t Count;
} KEGL;

#define FONT struct h_font
FONT { char *family;
       char *name;
       int16_t Bold,Italic,Underline;
     };
typedef struct hfont_cod {const char *name,*family;} FONT_COD;

typedef struct RTFLETTER
{
	    		unsigned char    alternative;          //  ASCII code.  >= ' '.
	    		unsigned char    probability;
} RTF_LETTER;

/////////////////////////////////////////////////////////////////////////////
// Page class
class CRtfPage //: public CObject
{
public:
	 CRtfPage();
 ~CRtfPage();

		void                 CRtfPageDelFragments(void);
  CRtfFragment*        GetNextFragment();
  Bool                 ReadInternalFile(FILE *FileNameIn);
		Bool                 OpenOutputFile(const char* FileNameOut);
		void                 CloseOutputFile(void);
  void                 SetTwips(void);
		Bool                 FindPageTree(FILE *FileNameIn, const char* FileNameOut);
	 void                 ReCalcPageWidthAndHeight();
		Bool                 WriteHeaderRtf(void);
	 void                 CorrectKegl(void);
  void                 ChangeKegl(void);
		void                 AddNewKegl( int16_t OldKegl, int16_t NewKegl );
  int16_t                GetNewKegl( int16_t OldKegl );
  int16_t                GetMinKegl( int16_t OldKegl );
		void                 AddPictures(void);
		void                 AddTables(void);
		void                 AddLines(void);
		void                 SortUserNumber(void);
  Bool	                Write(const char* FileNameOut );
	 Bool	                Write_USE_NONE( void );
	 Bool	                Write_USE_FRAME( void );
	 Bool	                Write_USE_FRAME_AND_COLUMN( void );
 	int16_t               	GetFlagAndNumberFragment( uchar* FragmentType, int16_t* CurrentSectorNumber );
  void                 WriteSectorsHeader(int16_t i);
  void                 ToPlacePicturesAndTables(void);
  void                 Rtf_CED_CreatePage(void);
  void                 Rtf_CED_WriteFormattedEd( const char* FileNameOut, Handle* );
  uint16_t                 GetFreeSpaceBetweenSectors(CRtfSector* pRtfSector, CRtfSector* pRtfNextSector);
  void                 SetPaperSize(int32_t LeftPos,int32_t RightPos,int32_t TopPos,int32_t BottomPos,int32_t* PaperW,int32_t* PaperH,
																												        int32_t* MargL,int32_t* MargR,int32_t* MargT,int32_t* MargB);

  /*CObArray*/std::vector<CRtfFragment*>	            m_arFragments;
  /*CObArray*/std::vector<CRtfSector*>	            m_arSectors;
  /*CArray <KEGL,KEGL>*/std::vector<KEGL>   arKegls;

		RtfPageElementCount  Count;
	 uint16_t                 m_wDpi;
		float                m_fTwips;
  RECT	               m_rect;
  RECT	               m_rectReal;
  int32_t                PaperW;
		int32_t                PaperH;
		int32_t                MargL;
		int32_t                MargR;
		int32_t                MargT;
		int32_t                MargB;

		int32_t                InitMargL;
		int32_t                InitMargR;
		int32_t                InitMargT;
		int32_t                InitMargB;

		uchar                 FlagBadColumn;
  int		                m_nIndex;
  int                  m_nCurSectorNumber;
  int                  m_nPrevSectorNumber;
  Handle               m_hED;
};

/////////////////////////////////////////////////////////////////////////////
// Fragment class
class CRtfFragment//:public CObject
{
public:
  CRtfFragment();
 ~CRtfFragment();

  CRtfPage*            pRtfParent;
  CRtfString*	       GetFirstString();
  CRtfString*	       GetNextString();
  Bool	               Write(RtfSectorInfo* SectorInfo);
  void                 InitFragment(RtfSectorInfo* SectorInfo);
  void                 SetFragmentAlignment(RtfSectorInfo* SectorInfo);
  void                 new_paragraph( Bool OutPutType );
  Bool                 FWriteText(int16_t NumberCurrentFragment,RtfSectorInfo* SectorInfo, Bool OutPutType);
  Bool                 FWriteTable(int16_t NumberCurrentFragment,RtfSectorInfo* SectorInfo, Bool OutPutType);
  Bool                 FWritePicture(int16_t NumberCurrentFragment,RtfSectorInfo* SectorInfo, Bool OutPutType);

  Bool                 ProcessingUseNoneMode(void);
  void                 Init(RtfSectorInfo* SectorInfo);
  void                 ReInit(RtfSectorInfo* SectorInfo, int beg, int end);
  Bool                 ProcessingOverLayedFragment(RtfSectorInfo* SectorInfo);
  Bool                 DeterminationOfCentreJustification(int beg, int end);
  Bool                 DeterminationOfRightJustification(int beg, int end);
  Bool                 DeterminationOfListType(int beg, int end);
  void                 Done(void);

  Bool                 DeterminationOfMixedFragment(RtfSectorInfo* SectorInfo);
  void                 GetNextFragmentBegEnd(int32_t* beg, int32_t* end, Bool* Flag);

  Bool                 DeterminationOfLeftRightJustification(int beg, int end);
  Bool                 CheckLeftRightJustification(int beg, int end);
  void                 SetFlagBeginParagraphForLeftRightJustification(int beg, int end);
  Bool                 CheckStringForLeftRightJustification(int ns);

  Bool                 DeterminationOfLeftJustification(int beg, int end, Bool direct);
  Bool                 CheckLeftJustification(int beg, int end);
  void                 SetFlagBeginParagraphForLeftJustification(int beg, int end);
  Bool                 CheckStringForLeftJustification(int ns);

  void                 CheckOnceAgainImportancesFlagBeginParagraph(void);
  void                 DefineLineTransfer(void);
  int                  GetCountLine(int beg);
  void                 SetLineTransfer(int beg, int end);
  void                 SetFirstLeftAndRightIndentOfParagraph(void);
  void                 CorrectIndents(int beg, int end);
  void                 SetParagraphAlignment(int beg, int end, int AlignType);
  void                 CalculationLengthAndCount(CRtfString* pRtfString, int32_t* CountChars, int32_t* LengthChars);
  void                 GetCountEqual(int beg, int end, uint16_t* Count, int AlignType);
  Bool                 GetFlagCarry(int beg, int end);
  Bool                 GetFlagLeft(int beg, int end);
  Bool                 GetFlagStrongLeft(int beg, int end);
  Bool                 GetFlagRight(int beg, int end);
  Bool                 GetFlagBigSpace(int beg, int end);
  Bool                 CheckNumber(uchar FirstChar);

  void                 PrintTheResult(const char*);

  /*CObArray*/std::vector<CRtfString*>	           m_arStrings;

  uint16_t     	           m_wStringsCount;
  uint16_t	               m_wIndex;
  RECT	               m_rect;
  RECT	               m_rectReal;
  RECT                m_rectFree;
  uint16_t	               m_wType;
uint32_t                m_wInternalNumber;
uint32_t                m_wUserNumber;
uint32_t                m_wUserNumberForFormattedMode;
  int32_t                 m_LeftOffsetFragmentFromVerticalColumn;
  int32_t                 m_RightOffsetFragmentFromVerticalColumn;
  uint16_t                 m_wOffsetFromPrevTextFragment;

  uint16_t                 m_wBegStr;
  uint16_t                 m_wEndStr;

  Bool                 m_bFlagObjectInColumn;
  Bool                 m_bFlagUsed;
  Bool                 m_bOutPutType;
  Bool                 m_bflag_new_column;
  Bool                 m_bflag_new_parag;
  Bool                 m_bflag_new_str;
  Bool                 m_bflag_new_line;
  Bool                 m_bflag_change;
  Bool                 m_bflag_LineTransfer;
  uint16_t                 m_wprev_font_name;
  uint16_t                 m_wprev_Underline;
  uint16_t                 m_wprev_Bold;
  uint16_t                 m_wprev_Italic;
  uint16_t                 m_wprev_lang;
  uint16_t                 m_wprev_font_size;
  uint16_t                 m_wvid_parag;

  uint16_t                 m_CountLeftEqual;
  uint16_t                 m_CountRightEqual;
  uint16_t                 m_CountLeftRightEqual;
  uint16_t                 m_CountCentreEqual;

  int16_t                m_l_fragment;
  int16_t                m_r_fragment;
  int16_t                m_l_fragmentLocal;
  int16_t                m_r_fragmentLocal;

  int16_t                m_LengthStringInTwips;
  int16_t                m_WidthVerticalColumn;

  int16_t                m_posx;
  int16_t                m_posy;
  int16_t                m_absw;
  int16_t                m_absh;

  int16_t                m_tmp_fi;
  int16_t                m_li;
  int16_t                m_ri;
  int16_t                m_fi;
  int16_t                m_sb;
  int16_t                m_sa;
  int16_t                m_sl;

  int16_t                m_max_dist;
  uchar                 m_Attr;
  uchar                 m_FlagCarry;
  uchar                 m_FlagLeft;
  uchar                 m_FlagStrongLeft;
  uchar                 m_FlagRight;
  uchar                 m_FlagBigSpace;
uint32_t                m_Flag;
};

/////////////////////////////////////////////////////////////////////////////
// Sector class
class CRtfSector//:public CObject
{
public:
	 CRtfSector();
 ~CRtfSector();

	 CRtfPage             *m_PagePtr;

  Bool	                Write(void);
  void                 WriteTerminalColumns(void);
  void                 WriteNonTerminalColumns(void);
  void                 WriteTerminalColumnsTablesAndPictures(void);
  void                 CalcSector(void);
  int32_t                GetCountAndRightBoundVTerminalColumns(void);
  void                 ToPlacePicturesAndTables(CRtfFragment* Frament);
  void                 FillingSectorInfo(void);

  /*CObArray*/std::vector<CRtfHorizontalColumn*>	            m_arHorizontalColumns;
		vectorWord/*CWordArray*/           m_arHTerminalColumnsIndex;
		vectorWord/*CWordArray*/           m_arRightBoundTerminalColumns;
		vectorWord/*CWordArray*/           m_arWidthTerminalColumns;
  RtfSectorInfo        SectorInfo;

  uint16_t     	           m_wHorizontalColumnsCount;
  uint16_t     	           m_wFragmentsCount;
  uint16_t     	           m_wTextFragmentsCount;
  uint16_t     	           m_wPictureFragmentsCount;
  uint16_t     	           m_wTableFragmentsCount;
  uint16_t	                m_wIndex;
  RECT	               m_rect;
  RECT	               m_rectReal;
  uint16_t	                m_wType;
  Handle               m_hEDSector;
		Bool                 m_FlagOneString;
		Bool                 m_bFlagLine;
	int32_t			m_VTerminalColumnNumber;
};

/////////////////////////////////////////////////////////////////////////////
// HorizontalColumn class
class CRtfHorizontalColumn//:public CObject
{
public:
	 CRtfHorizontalColumn();
 ~CRtfHorizontalColumn();

	 CRtfPage             *m_PagePtr;

  Bool	                Write(/*CWordArray*/vectorWord* arRightBoundTerminalColumns, int32_t* VTerminalColumnNumber);
  void                 WriteTerminalColumns(/*CWordArray*/vectorWord* arRightBoundTerminalColumns,
		                                          int32_t* VTerminalColumnNumber,
																																												int32_t  CountVTerminalColumns,
																																												RtfSectorInfo* SectorInfo);
  void                 WriteFramesInTerminalColumn(RtfSectorInfo* SectorInfo,Bool FlagFirstTerminalFragment,int32_t TopPositionFirstTerminalFragment);
		void                 WriteNonTerminalColumns(RtfSectorInfo* SectorInfo);
  void                 WriteTerminalColumnsTablesAndPictures(RtfSectorInfo *SectorInfo);
  void                 CalcHorizontalColumn(void);
  int32_t                GetCountAndRightBoundVTerminalColumns(/*CWordArray*/vectorWord* arRightBoundTerminalColumns,vectorWord/*CWordArray*/* arWidthTerminalColumns);
  void                 FindHeadingAndSetFrameFlag(void);
  void                 DefineTerminalProperty(void);
  void                 FillingVTerminalColumnsIndex(void);
  void                 ToPlacePicturesAndTables(CRtfFragment* Frament);
  Bool	                CheckTermColumn(void);
  void                 SortFragments(void);
  uint16_t                 GetFreeSpaceBetweenPrevAndCurrentFragments(int TopPosCurFrag, RtfSectorInfo *SectorInfo);
  uint16_t                 GetOffsetFromPrevTextFragment(CRtfFragment *pRtfFragment);
  Bool                 GetOverLayedFlag(int CurFragmentNumber);
  void                 SetFlagObjectInColumnForPageFragment( CRtfFragment* CurFragment );

  /*CObArray*/std::vector<CRtfVerticalColumn*>	            m_arVerticalColumns;
  /*CObArray*/std::vector<vectorWord*>	            m_arVTerminalColumnsGroup;
  /*CObArray*/std::vector<vectorWord*>	            m_arVTerminalColumnsIndex;
  /*CWordArray*/vectorWord           m_arSpacePlace;
		/*CByteArray*/std::vector<uchar>           m_arOrderingNumber;

  uint16_t     	           m_wVerticalColumnsCount;
  uint16_t	                m_wIndex;
  RECT	               m_rect;
  RECT	               m_rectReal;
  uint16_t	                m_wType;
};

/////////////////////////////////////////////////////////////////////////////
// VerticalColumn class
class CRtfVerticalColumn//:public CObject
{
public:
	 CRtfVerticalColumn();
 ~CRtfVerticalColumn();

  Bool	                Write( Bool OutPutType ,RtfSectorInfo* SectorInfo);
  void                 SetSpaceRect(CRtfFragment* CurrentFragment, RtfSectorInfo* SectorInfo);

  /*CObArray*/std::vector<CRtfFragment*>	            m_arFragments;
	 CRtfPage             *m_PagePtr;

  uint16_t     	           m_wFragmentsCount;
  uint16_t	                m_wIndex;
		Bool                 m_bSortFlag;
  RECT	               m_rect;
  RECT	               m_rectReal;
  uint16_t	                m_wType;
  Bool                 m_bFlagObjectInColumn;
		Bool                 m_bFlagSmall;
};

/////////////////////////////////////////////////////////////////////////////
// String class
class CRtfString//:public CObject
{
public:
  CRtfString  ();
  ~CRtfString ();

  CRtfWord*            GetFirstWord           (void);
  CRtfWord*            GetNextWord            (void);
  Bool	               Write                  (void);
  uint16_t                 get_max_font_size      (void);
  int16_t                GetStringSizeInTwips   (void);
  uint16_t                 GetRealStringSize      (void);

  /*CObArray*/std::vector<CRtfWord*>             m_arWords;

  uint16_t  	           m_wWordsCount;
  uint16_t	               m_wIndex;
  CRtfFragment*        m_Frag;
  RECT	               m_rect;
  uint16_t	               m_wType;


  int32_t                m_LeftBorder;
  int32_t                m_RightBorder;
  uint16_t	               m_wLeftIndent;
  uint16_t	               m_wRightIndent;
  uint16_t	               m_wCentre;
  uint16_t	               m_wFirstIndent;
  uint16_t	               m_wFlagBeginParagraph;
  uint16_t	               m_wAlignment;
  uint16_t	               m_wPrevAlignment;
  uint16_t	               m_LengthStringInTwips;
  uint16_t	               m_wSpaceBefore;
  uchar                 m_wLeftBorderEqual;
  uchar                 m_wRightBorderEqual;
  uchar                 m_wCentreEqual;
  uchar                 m_bLineTransfer;
  uchar                 m_LastChar;
  uchar                 m_FirstChar;
  uchar                 m_Attr;
  uchar                 m_FlagCarry;
  uint32_t               S_Flags; //NEGA_STR vmk 10-06-2001
};

/////////////////////////////////////////////////////////////////////////////
// Word class
class CRtfWord//:public CObject
{
public:
	 CRtfWord();
 ~CRtfWord();

  CRtfChar*            GetFirstChar();
  CRtfChar*            GetNextChar();
  Bool	                Write();
  void                 get_coordinates_and_probability(void);

  /*CObArray*/std::vector<CRtfChar*>             m_arChars;

		uint16_t	                m_wCharsCount;
  uint16_t	                m_wIndex;
  CRtfString*          m_String;
  RECT	               m_rect;

  int16_t                m_wcl;
  int16_t                m_wct;
  int16_t                m_wcr;
  int16_t                m_wcb;
  int16_t                m_wcs;
  int16_t                m_wcp;

  uint16_t	                m_wFontNumber;
  uint16_t	                m_wIdealFontPointSize;
  uint16_t	                m_wRealFontPointSize;
};

/////////////////////////////////////////////////////////////////////////////
// Char class
class CRtfChar//:public CObject
{
public:
	 CRtfChar(){};
 ~CRtfChar(){};
  Bool	                Write();

  struct
  {
  uchar	        m_bChar;
  uchar	        m_bProbability;
  } m_chrVersions[REC_MAX_VERS];//!!! Art
  RECT	               m_Idealrect;
  RECT	               m_Realrect;
		uchar                 m_blanguage;
		uchar                 m_bFlg_spell;
		uchar                 m_bFlg_spell_nocarrying;
		uchar                 m_bFlg_cup_drop;
	 uint16_t                 m_wCountAlt;
	 uint16_t                 m_wFontNumber;
  uint16_t                 m_wFontPointSize;
  uint16_t	                m_wIndex;
 };

#endif // __CreateRtf_h__
