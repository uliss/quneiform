/*
Copyright (c) 1993-2008, Cognitive Technologies
All rights reserved.

Ðàçðåøàåòñÿ ïîâòîðíîå ðàñïðîñòðàíåíèå è èñïîëüçîâàíèå êàê â âèäå èñõîäíîãî êîäà,
òàê è â äâîè÷íîé ôîðìå, ñ èçìåíåíèÿìè èëè áåç, ïðè ñîáëþäåíèè ñëåäóþùèõ óñëîâèé:

      * Ïðè ïîâòîðíîì ðàñïðîñòðàíåíèè èñõîäíîãî êîäà äîëæíû îñòàâàòüñÿ óêàçàííîå
        âûøå óâåäîìëåíèå îá àâòîðñêîì ïðàâå, ýòîò ñïèñîê óñëîâèé è ïîñëåäóþùèé
        îòêàç îò ãàðàíòèé.
      * Ïðè ïîâòîðíîì ðàñïðîñòðàíåíèè äâîè÷íîãî êîäà â äîêóìåíòàöèè è/èëè â
        äðóãèõ ìàòåðèàëàõ, ïîñòàâëÿåìûõ ïðè ðàñïðîñòðàíåíèè, äîëæíû ñîõðàíÿòüñÿ
        óêàçàííàÿ âûøå èíôîðìàöèÿ îá àâòîðñêîì ïðàâå, ýòîò ñïèñîê óñëîâèé è
        ïîñëåäóþùèé îòêàç îò ãàðàíòèé.
      * Íè íàçâàíèå Cognitive Technologies, íè èìåíà åå ñîòðóäíèêîâ íå ìîãóò
        áûòü èñïîëüçîâàíû â êà÷åñòâå ñðåäñòâà ïîääåðæêè è/èëè ïðîäâèæåíèÿ
        ïðîäóêòîâ, îñíîâàííûõ íà ýòîì ÏÎ, áåç ïðåäâàðèòåëüíîãî ïèñüìåííîãî
        ðàçðåøåíèÿ.

ÝÒÀ ÏÐÎÃÐÀÌÌÀ ÏÐÅÄÎÑÒÀÂËÅÍÀ ÂËÀÄÅËÜÖÀÌÈ ÀÂÒÎÐÑÊÈÕ ÏÐÀÂ È/ÈËÈ ÄÐÓÃÈÌÈ ËÈÖÀÌÈ "ÊÀÊ
ÎÍÀ ÅÑÒÜ" ÁÅÇ ÊÀÊÎÃÎ-ËÈÁÎ ÂÈÄÀ ÃÀÐÀÍÒÈÉ, ÂÛÐÀÆÅÍÍÛÕ ßÂÍÎ ÈËÈ ÏÎÄÐÀÇÓÌÅÂÀÅÌÛÕ,
ÂÊËÞ×Àß ÃÀÐÀÍÒÈÈ ÊÎÌÌÅÐ×ÅÑÊÎÉ ÖÅÍÍÎÑÒÈ È ÏÐÈÃÎÄÍÎÑÒÈ ÄËß ÊÎÍÊÐÅÒÍÎÉ ÖÅËÈ, ÍÎ ÍÅ
ÎÃÐÀÍÈ×ÈÂÀßÑÜ ÈÌÈ. ÍÈ ÂËÀÄÅËÅÖ ÀÂÒÎÐÑÊÈÕ ÏÐÀÂ È ÍÈ ÎÄÍÎ ÄÐÓÃÎÅ ËÈÖÎ, ÊÎÒÎÐÎÅ
ÌÎÆÅÒ ÈÇÌÅÍßÒÜ È/ÈËÈ ÏÎÂÒÎÐÍÎ ÐÀÑÏÐÎÑÒÐÀÍßÒÜ ÏÐÎÃÐÀÌÌÓ, ÍÈ Â ÊÎÅÌ ÑËÓ×ÀÅ ÍÅ
ÍÅÑ¨Ò ÎÒÂÅÒÑÒÂÅÍÍÎÑÒÈ, ÂÊËÞ×Àß ËÞÁÛÅ ÎÁÙÈÅ, ÑËÓ×ÀÉÍÛÅ, ÑÏÅÖÈÀËÜÍÛÅ ÈËÈ
ÏÎÑËÅÄÎÂÀÂØÈÅ ÓÁÛÒÊÈ, ÑÂßÇÀÍÍÛÅ Ñ ÈÑÏÎËÜÇÎÂÀÍÈÅÌ ÈËÈ ÏÎÍÅÑÅÍÍÛÅ ÂÑËÅÄÑÒÂÈÅ
ÍÅÂÎÇÌÎÆÍÎÑÒÈ ÈÑÏÎËÜÇÎÂÀÍÈß ÏÐÎÃÐÀÌÌÛ (ÂÊËÞ×Àß ÏÎÒÅÐÈ ÄÀÍÍÛÕ, ÈËÈ ÄÀÍÍÛÅ,
ÑÒÀÂØÈÅ ÍÅÃÎÄÍÛÌÈ, ÈËÈ ÓÁÛÒÊÈ È/ÈËÈ ÏÎÒÅÐÈ ÄÎÕÎÄÎÂ, ÏÎÍÅÑÅÍÍÛÅ ÈÇ-ÇÀ ÄÅÉÑÒÂÈÉ
ÒÐÅÒÜÈÕ ËÈÖ È/ÈËÈ ÎÒÊÀÇÀ ÏÐÎÃÐÀÌÌÛ ÐÀÁÎÒÀÒÜ ÑÎÂÌÅÑÒÍÎ Ñ ÄÐÓÃÈÌÈ ÏÐÎÃÐÀÌÌÀÌÈ,
ÍÎ ÍÅ ÎÃÐÀÍÈ×ÈÂÀßÑÜ ÝÒÈÌÈ ÑËÓ×ÀßÌÈ), ÍÎ ÍÅ ÎÃÐÀÍÈ×ÈÂÀßÑÜ ÈÌÈ, ÄÀÆÅ ÅÑËÈ ÒÀÊÎÉ
ÂËÀÄÅËÅÖ ÈËÈ ÄÐÓÃÎÅ ËÈÖÎ ÁÛËÈ ÈÇÂÅÙÅÍÛ Î ÂÎÇÌÎÆÍÎÑÒÈ ÒÀÊÈÕ ÓÁÛÒÊÎÂ È ÏÎÒÅÐÜ.

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
#define TG_EDW_SERIF          1      //ñåðèô.
#define TG_EDW_GELV           2      //áåç ñåðèô.
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

#define RTF_TP_LEFT_ALLIGN           0 //- âûðàâíèâàåíèå ïî ëåâîìó êðàþ
#define	RTF_TP_RIGHT_ALLIGN          1 //- âûðàâíèâàíèå ïî ïðàâîìó êðàþ
#define RTF_TP_LEFT_AND_RIGHT_ALLIGN 2 //- âûðàâíèâàíèå ïî øèðèíå
#define RTF_TP_CENTER                3 //- âûðàâíèâàíèå ïî öåíòðó
#define RTF_TP_ONE                   4 //- êàæäàÿ ñòðîêà ñîñòîèò èç îäíîãî ñëîâà, âûðàâíèâàíèå ëåâîìó êðàþ
#define RTF_TP_TYPE_LINE             5 //- ñïèñîê

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
   Int16 OldKegl;
   Int16 NewKegl;
			Int16 Count;
} KEGL;

#define FONT struct h_font
FONT { char *family;
       char *name;
       Int16 Bold,Italic,Underline;
     };
typedef struct hfont_cod {char *name,*family;} FONT_COD;

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
  BOOL                 ReadInternalFile(const char* FileNameIn);
		BOOL                 OpenOutputFile(const char* FileNameOut);
		void                 CloseOutputFile(void);
  void                 SetTwips(void);
		BOOL                 FindPageTree(const char* FileNameIn, const char* FileNameOut);
	 void                 ReCalcPageWidthAndHeight();
		BOOL                 WriteHeaderRtf(void);
	 void                 CorrectKegl(void);
  void                 ChangeKegl(void);
		void                 AddNewKegl( Int16 OldKegl, Int16 NewKegl );
  Int16                GetNewKegl( Int16 OldKegl );
  Int16                GetMinKegl( Int16 OldKegl );
		void                 AddPictures(void);
		void                 AddTables(void);
		void                 AddLines(void);
		void                 SortUserNumber(void);
  BOOL	                Write(const char* FileNameOut );
	 BOOL	                Write_USE_NONE( void );
	 BOOL	                Write_USE_FRAME( void );
	 BOOL	                Write_USE_FRAME_AND_COLUMN( void );
 	Int16               	GetFlagAndNumberFragment( BYTE* FragmentType, Int16* CurrentSectorNumber );
  void                 WriteSectorsHeader(Int16 i);
  void                 ToPlacePicturesAndTables(void);
  void                 Rtf_CED_CreatePage(void);
  void                 Rtf_CED_WriteFormattedEd( const char* FileNameOut, Handle* );
  WORD                 GetFreeSpaceBetweenSectors(CRtfSector* pRtfSector, CRtfSector* pRtfNextSector);
  void                 SetPaperSize(Int32 LeftPos,Int32 RightPos,Int32 TopPos,Int32 BottomPos,Int32* PaperW,Int32* PaperH,
																												        Int32* MargL,Int32* MargR,Int32* MargT,Int32* MargB);

  /*CObArray*/std::vector<CRtfFragment*>	            m_arFragments;
  /*CObArray*/std::vector<CRtfSector*>	            m_arSectors;
  /*CArray <KEGL,KEGL>*/std::vector<KEGL>   arKegls;

		RtfPageElementCount  Count;
	 WORD                 m_wDpi;
		float                m_fTwips;
  RECT	               m_rect;
  RECT	               m_rectReal;
  Int32                PaperW;
		Int32                PaperH;
		Int32                MargL;
		Int32                MargR;
		Int32                MargT;
		Int32                MargB;

		Int32                InitMargL;
		Int32                InitMargR;
		Int32                InitMargT;
		Int32                InitMargB;

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
  BOOL                 FWriteText(Int16 NumberCurrentFragment,RtfSectorInfo* SectorInfo, BOOL OutPutType);
  BOOL                 FWriteTable(Int16 NumberCurrentFragment,RtfSectorInfo* SectorInfo, BOOL OutPutType);
  BOOL                 FWritePicture(Int16 NumberCurrentFragment,RtfSectorInfo* SectorInfo, BOOL OutPutType);

  BOOL                 ProcessingUseNoneMode(void);
  void                 Init(RtfSectorInfo* SectorInfo);
  void                 ReInit(RtfSectorInfo* SectorInfo, int beg, int end);
  BOOL                 ProcessingOverLayedFragment(RtfSectorInfo* SectorInfo);
  BOOL                 DeterminationOfCentreJustification(int beg, int end);
  BOOL                 DeterminationOfRightJustification(int beg, int end);
  BOOL                 DeterminationOfListType(int beg, int end);
  void                 Done(void);

  BOOL                 DeterminationOfMixedFragment(RtfSectorInfo* SectorInfo);
  void                 GetNextFragmentBegEnd(Int32* beg, Int32* end, BOOL* Flag);

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
  void                 CalculationLengthAndCount(CRtfString* pRtfString, Int32* CountChars, Int32* LengthChars);
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
  DWORD                m_wInternalNumber;
  DWORD                m_wUserNumber;
  DWORD                m_wUserNumberForFormattedMode;
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

  Int16                m_l_fragment;
  Int16                m_r_fragment;
  Int16                m_l_fragmentLocal;
  Int16                m_r_fragmentLocal;

  Int16                m_LengthStringInTwips;
  Int16                m_WidthVerticalColumn;

  Int16                m_posx;
  Int16                m_posy;
  Int16                m_absw;
  Int16                m_absh;

  Int16                m_tmp_fi;
  Int16                m_li;
  Int16                m_ri;
  Int16                m_fi;
  Int16                m_sb;
  Int16                m_sa;
  Int16                m_sl;

  Int16                m_max_dist;
  BYTE                 m_Attr;
  BYTE                 m_FlagCarry;
  BYTE                 m_FlagLeft;
  BYTE                 m_FlagStrongLeft;
  BYTE                 m_FlagRight;
  BYTE                 m_FlagBigSpace;
  DWORD                m_Flag;
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
  Int32                GetCountAndRightBoundVTerminalColumns(void);
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
	Int32			m_VTerminalColumnNumber;
};

/////////////////////////////////////////////////////////////////////////////
// HorizontalColumn class
class CRtfHorizontalColumn//:public CObject
{
public:
	 CRtfHorizontalColumn();
 ~CRtfHorizontalColumn();

	 CRtfPage             *m_PagePtr;

  BOOL	                Write(/*CWordArray*/vectorWord* arRightBoundTerminalColumns, Int32* VTerminalColumnNumber);
  void                 WriteTerminalColumns(/*CWordArray*/vectorWord* arRightBoundTerminalColumns,
		                                          Int32* VTerminalColumnNumber,
																																												Int32  CountVTerminalColumns,
																																												RtfSectorInfo* SectorInfo);
  void                 WriteFramesInTerminalColumn(RtfSectorInfo* SectorInfo,BOOL FlagFirstTerminalFragment,Int32 TopPositionFirstTerminalFragment);
		void                 WriteNonTerminalColumns(RtfSectorInfo* SectorInfo);
  void                 WriteTerminalColumnsTablesAndPictures(RtfSectorInfo *SectorInfo);
  void                 CalcHorizontalColumn(void);
  Int32                GetCountAndRightBoundVTerminalColumns(/*CWordArray*/vectorWord* arRightBoundTerminalColumns,vectorWord/*CWordArray*/* arWidthTerminalColumns);
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
  Int16                GetStringSizeInTwips   (void);
  WORD                 GetRealStringSize      (void);

  /*CObArray*/std::vector<CRtfWord*>             m_arWords;

  WORD  	           m_wWordsCount;
  WORD	               m_wIndex;
  CRtfFragment*        m_Frag;
  RECT	               m_rect;
  WORD	               m_wType;


  Int32                m_LeftBorder;
  Int32                m_RightBorder;
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
  Word32               S_Flags; //NEGA_STR vmk 10-06-2001
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

  Int16                m_wcl;
  Int16                m_wct;
  Int16                m_wcr;
  Int16                m_wcb;
  Int16                m_wcs;
  Int16                m_wcp;

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
