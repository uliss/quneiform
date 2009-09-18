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
 WORD  RtfSectors;
 WORD  RtfTextFragments;
	WORD  RtfFrameTextFragments;
 WORD  RtfPictureFragments;
 WORD  RtfTableFragments;
 WORD  RtfStrings;
 WORD  RtfWords;
 WORD  RtfChars;
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
  BOOL                 ReadInternalFile(FILE *FileNameIn);
		BOOL                 OpenOutputFile(const char* FileNameOut);
		void                 CloseOutputFile(void);
  void                 SetTwips(void);
		BOOL                 FindPageTree(FILE *FileNameIn, const char* FileNameOut);
	 void                 ReCalcPageWidthAndHeight();
		BOOL                 WriteHeaderRtf(void);
	 void                 CorrectKegl(void);
  void                 ChangeKegl(void);
		void                 AddNewKegl( int16_t OldKegl, int16_t NewKegl );
  int16_t                GetNewKegl( int16_t OldKegl );
  int16_t                GetMinKegl( int16_t OldKegl );
		void                 AddPictures(void);
		void                 AddTables(void);
		void                 AddLines(void);
		void                 SortUserNumber(void);
  BOOL	                Write(const char* FileNameOut );
	 BOOL	                Write_USE_NONE( void );
	 BOOL	                Write_USE_FRAME( void );
	 BOOL	                Write_USE_FRAME_AND_COLUMN( void );
 	int16_t               	GetFlagAndNumberFragment( BYTE* FragmentType, int16_t* CurrentSectorNumber );
  void                 WriteSectorsHeader(int16_t i);
  void                 ToPlacePicturesAndTables(void);
  void                 Rtf_CED_CreatePage(void);
  void                 Rtf_CED_WriteFormattedEd( const char* FileNameOut, Handle* );
  WORD                 GetFreeSpaceBetweenSectors(CRtfSector* pRtfSector, CRtfSector* pRtfNextSector);
  void                 SetPaperSize(int32_t LeftPos,int32_t RightPos,int32_t TopPos,int32_t BottomPos,int32_t* PaperW,int32_t* PaperH,
																												        int32_t* MargL,int32_t* MargR,int32_t* MargT,int32_t* MargB);

  /*CObArray*/std::vector<CRtfFragment*>	            m_arFragments;
  /*CObArray*/std::vector<CRtfSector*>	            m_arSectors;
  /*CArray <KEGL,KEGL>*/std::vector<KEGL>   arKegls;

		RtfPageElementCount  Count;
	 WORD                 m_wDpi;
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

		BYTE                 FlagBadColumn;
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
  BOOL	               Write(RtfSectorInfo* SectorInfo);
  void                 InitFragment(RtfSectorInfo* SectorInfo);
  void                 SetFragmentAlignment(RtfSectorInfo* SectorInfo);
  void                 new_paragraph( BOOL OutPutType );
  BOOL                 FWriteText(int16_t NumberCurrentFragment,RtfSectorInfo* SectorInfo, BOOL OutPutType);
  BOOL                 FWriteTable(int16_t NumberCurrentFragment,RtfSectorInfo* SectorInfo, BOOL OutPutType);
  BOOL                 FWritePicture(int16_t NumberCurrentFragment,RtfSectorInfo* SectorInfo, BOOL OutPutType);

  BOOL                 ProcessingUseNoneMode(void);
  void                 Init(RtfSectorInfo* SectorInfo);
  void                 ReInit(RtfSectorInfo* SectorInfo, int beg, int end);
  BOOL                 ProcessingOverLayedFragment(RtfSectorInfo* SectorInfo);
  BOOL                 DeterminationOfCentreJustification(int beg, int end);
  BOOL                 DeterminationOfRightJustification(int beg, int end);
  BOOL                 DeterminationOfListType(int beg, int end);
  void                 Done(void);

  BOOL                 DeterminationOfMixedFragment(RtfSectorInfo* SectorInfo);
  void                 GetNextFragmentBegEnd(int32_t* beg, int32_t* end, BOOL* Flag);

  BOOL                 DeterminationOfLeftRightJustification(int beg, int end);
  BOOL                 CheckLeftRightJustification(int beg, int end);
  void                 SetFlagBeginParagraphForLeftRightJustification(int beg, int end);
  BOOL                 CheckStringForLeftRightJustification(int ns);

  BOOL                 DeterminationOfLeftJustification(int beg, int end, BOOL direct);
  BOOL                 CheckLeftJustification(int beg, int end);
  void                 SetFlagBeginParagraphForLeftJustification(int beg, int end);
  BOOL                 CheckStringForLeftJustification(int ns);

  void                 CheckOnceAgainImportancesFlagBeginParagraph(void);
  void                 DefineLineTransfer(void);
  int                  GetCountLine(int beg);
  void                 SetLineTransfer(int beg, int end);
  void                 SetFirstLeftAndRightIndentOfParagraph(void);
  void                 CorrectIndents(int beg, int end);
  void                 SetParagraphAlignment(int beg, int end, int AlignType);
  void                 CalculationLengthAndCount(CRtfString* pRtfString, int32_t* CountChars, int32_t* LengthChars);
  void                 GetCountEqual(int beg, int end, WORD* Count, int AlignType);
  BOOL                 GetFlagCarry(int beg, int end);
  BOOL                 GetFlagLeft(int beg, int end);
  BOOL                 GetFlagStrongLeft(int beg, int end);
  BOOL                 GetFlagRight(int beg, int end);
  BOOL                 GetFlagBigSpace(int beg, int end);
  BOOL                 CheckNumber(BYTE FirstChar);

  void                 PrintTheResult(const char*);

  /*CObArray*/std::vector<CRtfString*>	           m_arStrings;

  WORD     	           m_wStringsCount;
  WORD	               m_wIndex;
  RECT	               m_rect;
  RECT	               m_rectReal;
  RECT                m_rectFree;
  WORD	               m_wType;
uint32_t                m_wInternalNumber;
uint32_t                m_wUserNumber;
uint32_t                m_wUserNumberForFormattedMode;
  LONG                 m_LeftOffsetFragmentFromVerticalColumn;
  LONG                 m_RightOffsetFragmentFromVerticalColumn;
  WORD                 m_wOffsetFromPrevTextFragment;

  WORD                 m_wBegStr;
  WORD                 m_wEndStr;

  BOOL                 m_bFlagObjectInColumn;
  BOOL                 m_bFlagUsed;
  BOOL                 m_bOutPutType;
  BOOL                 m_bflag_new_column;
  BOOL                 m_bflag_new_parag;
  BOOL                 m_bflag_new_str;
  BOOL                 m_bflag_new_line;
  BOOL                 m_bflag_change;
  BOOL                 m_bflag_LineTransfer;
  WORD                 m_wprev_font_name;
  WORD                 m_wprev_Underline;
  WORD                 m_wprev_Bold;
  WORD                 m_wprev_Italic;
  WORD                 m_wprev_lang;
  WORD                 m_wprev_font_size;
  WORD                 m_wvid_parag;

  WORD                 m_CountLeftEqual;
  WORD                 m_CountRightEqual;
  WORD                 m_CountLeftRightEqual;
  WORD                 m_CountCentreEqual;

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
  BYTE                 m_Attr;
  BYTE                 m_FlagCarry;
  BYTE                 m_FlagLeft;
  BYTE                 m_FlagStrongLeft;
  BYTE                 m_FlagRight;
  BYTE                 m_FlagBigSpace;
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

  BOOL	                Write(void);
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

  WORD     	           m_wHorizontalColumnsCount;
  WORD     	           m_wFragmentsCount;
  WORD     	           m_wTextFragmentsCount;
  WORD     	           m_wPictureFragmentsCount;
  WORD     	           m_wTableFragmentsCount;
  WORD	                m_wIndex;
  RECT	               m_rect;
  RECT	               m_rectReal;
  WORD	                m_wType;
  Handle               m_hEDSector;
		BOOL                 m_FlagOneString;
		BOOL                 m_bFlagLine;
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

  BOOL	                Write(/*CWordArray*/vectorWord* arRightBoundTerminalColumns, int32_t* VTerminalColumnNumber);
  void                 WriteTerminalColumns(/*CWordArray*/vectorWord* arRightBoundTerminalColumns,
		                                          int32_t* VTerminalColumnNumber,
																																												int32_t  CountVTerminalColumns,
																																												RtfSectorInfo* SectorInfo);
  void                 WriteFramesInTerminalColumn(RtfSectorInfo* SectorInfo,BOOL FlagFirstTerminalFragment,int32_t TopPositionFirstTerminalFragment);
		void                 WriteNonTerminalColumns(RtfSectorInfo* SectorInfo);
  void                 WriteTerminalColumnsTablesAndPictures(RtfSectorInfo *SectorInfo);
  void                 CalcHorizontalColumn(void);
  int32_t                GetCountAndRightBoundVTerminalColumns(/*CWordArray*/vectorWord* arRightBoundTerminalColumns,vectorWord/*CWordArray*/* arWidthTerminalColumns);
  void                 FindHeadingAndSetFrameFlag(void);
  void                 DefineTerminalProperty(void);
  void                 FillingVTerminalColumnsIndex(void);
  void                 ToPlacePicturesAndTables(CRtfFragment* Frament);
  BOOL	                CheckTermColumn(void);
  void                 SortFragments(void);
  WORD                 GetFreeSpaceBetweenPrevAndCurrentFragments(int TopPosCurFrag, RtfSectorInfo *SectorInfo);
  WORD                 GetOffsetFromPrevTextFragment(CRtfFragment *pRtfFragment);
  BOOL                 GetOverLayedFlag(int CurFragmentNumber);
  void                 SetFlagObjectInColumnForPageFragment( CRtfFragment* CurFragment );

  /*CObArray*/std::vector<CRtfVerticalColumn*>	            m_arVerticalColumns;
  /*CObArray*/std::vector<vectorWord*>	            m_arVTerminalColumnsGroup;
  /*CObArray*/std::vector<vectorWord*>	            m_arVTerminalColumnsIndex;
  /*CWordArray*/vectorWord           m_arSpacePlace;
		/*CByteArray*/std::vector<BYTE>           m_arOrderingNumber;

  WORD     	           m_wVerticalColumnsCount;
  WORD	                m_wIndex;
  RECT	               m_rect;
  RECT	               m_rectReal;
  WORD	                m_wType;
};

/////////////////////////////////////////////////////////////////////////////
// VerticalColumn class
class CRtfVerticalColumn//:public CObject
{
public:
	 CRtfVerticalColumn();
 ~CRtfVerticalColumn();

  BOOL	                Write( BOOL OutPutType ,RtfSectorInfo* SectorInfo);
  void                 SetSpaceRect(CRtfFragment* CurrentFragment, RtfSectorInfo* SectorInfo);

  /*CObArray*/std::vector<CRtfFragment*>	            m_arFragments;
	 CRtfPage             *m_PagePtr;

  WORD     	           m_wFragmentsCount;
  WORD	                m_wIndex;
		BOOL                 m_bSortFlag;
  RECT	               m_rect;
  RECT	               m_rectReal;
  WORD	                m_wType;
  BOOL                 m_bFlagObjectInColumn;
		BOOL                 m_bFlagSmall;
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
  BOOL	               Write                  (void);
  WORD                 get_max_font_size      (void);
  int16_t                GetStringSizeInTwips   (void);
  WORD                 GetRealStringSize      (void);

  /*CObArray*/std::vector<CRtfWord*>             m_arWords;

  WORD  	           m_wWordsCount;
  WORD	               m_wIndex;
  CRtfFragment*        m_Frag;
  RECT	               m_rect;
  WORD	               m_wType;


  int32_t                m_LeftBorder;
  int32_t                m_RightBorder;
  WORD	               m_wLeftIndent;
  WORD	               m_wRightIndent;
  WORD	               m_wCentre;
  WORD	               m_wFirstIndent;
  WORD	               m_wFlagBeginParagraph;
  WORD	               m_wAlignment;
  WORD	               m_wPrevAlignment;
  WORD	               m_LengthStringInTwips;
  WORD	               m_wSpaceBefore;
  BYTE                 m_wLeftBorderEqual;
  BYTE                 m_wRightBorderEqual;
  BYTE                 m_wCentreEqual;
  BYTE                 m_bLineTransfer;
  BYTE                 m_LastChar;
  BYTE                 m_FirstChar;
  BYTE                 m_Attr;
  BYTE                 m_FlagCarry;
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
  BOOL	                Write();
  void                 get_coordinates_and_probability(void);

  /*CObArray*/std::vector<CRtfChar*>             m_arChars;

		WORD	                m_wCharsCount;
  WORD	                m_wIndex;
  CRtfString*          m_String;
  RECT	               m_rect;

  int16_t                m_wcl;
  int16_t                m_wct;
  int16_t                m_wcr;
  int16_t                m_wcb;
  int16_t                m_wcs;
  int16_t                m_wcp;

  WORD	                m_wFontNumber;
  WORD	                m_wIdealFontPointSize;
  WORD	                m_wRealFontPointSize;
};

/////////////////////////////////////////////////////////////////////////////
// Char class
class CRtfChar//:public CObject
{
public:
	 CRtfChar(){};
 ~CRtfChar(){};
  BOOL	                Write();

  struct
  {
  BYTE	        m_bChar;
  BYTE	        m_bProbability;
  } m_chrVersions[REC_MAX_VERS];//!!! Art
  RECT	               m_Idealrect;
  RECT	               m_Realrect;
		BYTE                 m_blanguage;
		BYTE                 m_bFlg_spell;
		BYTE                 m_bFlg_spell_nocarrying;
		BYTE                 m_bFlg_cup_drop;
	 WORD                 m_wCountAlt;
	 WORD                 m_wFontNumber;
  WORD                 m_wFontPointSize;
  WORD	                m_wIndex;
 };

#endif // __CreateRtf_h__
