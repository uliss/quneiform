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

#ifndef __CEDINT_H
#define __CEDINT_H

#include <stdio.h>

#ifndef __GLOBUS_H
#include "globus.h"
#endif
/*
#ifndef __RECDEFS_H
   #include "recdefs.h"
#endif
   #include "memfunc.h"
*/
#ifdef __cplusplus
	extern "C" {
#endif

//
#pragma pack (push,8)

#define CED_MAXNAME 260

#ifndef Handle
	#define Handle void *
#endif

#define CFIOHightCode 0x0001

#include "ced.h"
#include "ced_struct.h"
#include "cfio.h"
//#include "edfile.h"

#ifdef _DEBUG
#define EDASSERT(f) \
	{if (!(f)) \
		__asm { int 3 };}
#else
#define EDASSERT(f)
#endif

#include "compat_defs.h"

extern FNRDProc RDProced;//pointer to function, which processes raw data
void SetReturnCode_ced(uint32_t rc);//set error code
uint32_t GetReturnCode_ced();

//function, exported from CFIO
extern	FNCFIOReadMemoryFromFile    MemFromFile;              //19
extern	FNCFIOLockMemory					Lock;
extern	FNCFIOUnlockMemory				Unlock;
extern	FNCFIOFreeMemory					Free;
//extern	FNCFIOAllocMemory					Alloc;
extern	FNCFIOOpenFreeFile					Open;
extern	FNCFIOCloseFreeFile					Close;
extern	FNCFIOWriteToFile					Write;

#define DEC_FUN(a,b,c) a My##b c;a MyRawData##b c;extern FN##b b

DEC_FUN(void, CED_BitmapRef,(const bit_map_ref* pt));
DEC_FUN(void, CED_TextRef,(const text_ref* pt));
DEC_FUN(void, CED_FontKegl,(const font_kegl *pt));
DEC_FUN(void, CED_Kegl,(const kegl* pt));
DEC_FUN(void, CED_Shift,(const shift* pt));
DEC_FUN(void, CED_RetrieveLevel,(const retrieve_level* pt));
DEC_FUN(void, CED_Underline,(const underline* pt));
DEC_FUN(void, CED_DensPrint,(const dens_print* pt));
DEC_FUN(void, CED_Tabul,(const tabul* pt));
DEC_FUN(void, CED_TablTabul,(const tabl_tabul* pt));
DEC_FUN(void, CED_SheetDiskDescr,(const sheet_disk_descr* pt));
DEC_FUN(void, CED_FragmDiskDescr,(const fragm_disk_descr* pt));
DEC_FUN(void, CED_FragmDisk,(const fragm_disk* pt));
DEC_FUN(void, CED_StepBack,(const step_back* pt));
DEC_FUN(void, CED_LineBeg,(const line_beg* pt));
DEC_FUN(void, CED_Position,(const position* pt));
DEC_FUN(void, CED_EdTagLanguage,(const EdTagLanguage* pt));
DEC_FUN(void, CED_TableConformSizes,(const table_conform_sizes* pt));
DEC_FUN(void, CED_GroupWords,(const group_words* pt));
DEC_FUN(void, CED_GroupSymbols,(const group_symbols* pt));
DEC_FUN(void, CED_Border,(const border* pt));
DEC_FUN(void, CED_TableHeader,(const table_header* pt));
DEC_FUN(void, CED_ListOfFragments,(const list_of_fragments* pt));
DEC_FUN(void, CED_Extention,(const edExtention* pt,const void* ptExt));
DEC_FUN(void, CED_ExtentionNew,(const edExtentionNew* pt,const void* ptExt));
DEC_FUN(void, CED_Aksant,(const aksant* pt));
DEC_FUN(void, CED_Letter,(const letter* pt,const uint32_t alternatives));

#undef DEC_FUN


#define DEC_FUN(a,b,c) a b c

DEC_FUN(uint32_t, CED_ReadED,(char * file,Bool32 readFromFile, uint32_t bufLen));
DEC_FUN(void,CED_SetRawDataProc,(FNRDProc proc));
DEC_FUN(CEDPage*,CED_FormattedLoad,(char * file,Bool32 readFromFile, uint32_t bufLen));
DEC_FUN(Bool32,CED_FormattedWrite,(char * fileName, CEDPage *page));
DEC_FUN(void,CED_DeleteTree,(CEDPage * pg));
/*
#ifdef _DEBUG
DEC_FUN(void, CED_ShowTree,(char * name, Handle hEdPage));
#endif

DEC_FUN(Bool32, CED_IsEdFile, (char * file,Bool32 readFromFile, uint32_t bufLen));

DEC_FUN(Handle, CED_CreatePage,(char * _imageName,EDSIZE _sizeOfImage,EDSIZE _dpi,int _turn,int _pageNumber,EDSIZE _sizeInTwips, EDRECT _pageBordersInTwips, char _unrecogChar));
DEC_FUN(Bool32, CED_CreateFont,(Handle hEdPage, uchar fontNumber, uchar fontPitchAndFamily, uchar fontCharset,char* fontName));
DEC_FUN(Bool32, CED_CreatePicture,(Handle hEdPage, int pictNumber, EDSIZE pictSize, EDSIZE pictGoal, int pictAlign, int type, void * data, int len));
DEC_FUN(Handle, CED_CreateSection,(Handle hEdPage,EDRECT border, char sectionBreak, int width, int height, char orientation, int headerY, int footerY));
DEC_FUN(Handle, CED_CreateColumn,( Handle hEdSection,int width, int space));
DEC_FUN(Handle, CED_CreateParagraph,(Handle hEdSection, Handle hObject, int align, EDRECT indent, int UserNum, int FlagBorder, EDSIZE interval, EDBOX layout, int  color, int  shading, int spaceBetweenLines, char spcBtwLnsMult, char  keep));
DEC_FUN(Handle, CED_CreateLine,(Handle hEdParagraph));
DEC_FUN(Handle, CED_CreateChar,(Handle hEdLine, EDBOX layout, int fontHeight, int fontNum, int fontStyle, letterEx* alternatives));
DEC_FUN(Handle, CED_CreateFrame,(Handle hEdSection, Handle hObject, edBox rect, char position, uint32_t borderSpace, uint32_t dxfrtextx, uint32_t dxfrtexty));
DEC_FUN(Handle, CED_CreateCell,(Handle hEdSection,Handle hEdTable));
DEC_FUN(Handle, CED_CreateTable,(  Handle hEdSection, Handle hObject,EDSIZE sz, int * cx,int * cy, int * table,Bool32 * bHorShow,Bool32 * bVerShow));
DEC_FUN(void, CED_DeletePage,(Handle hEdPage));
DEC_FUN(Handle, CED_ReadFormattedEd,(char * lpEdFile,Bool32 readFromFile, uint32_t bufLen));
DEC_FUN(Bool32, CED_WriteFormattedEd,(char * lpEdFileName, Handle hEdPage));
DEC_FUN(char*, CED_GetPageImageName,(Handle hEdPage));
DEC_FUN(EDSIZE, CED_GetPageImageSize,(Handle hEdPage));
DEC_FUN(EDSIZE,CED_GetPageDpi,(Handle hEdPage));
DEC_FUN(uint32_t, CED_GetPageTurn,(Handle hEdPage));
DEC_FUN(EDSIZE,CED_GetPageSize,(Handle hEdPage));
DEC_FUN(char,CED_GetPageUnrecogChar,(Handle hEdPage));
DEC_FUN(uint32_t,CED_GetNumberOfParagraphs,(Handle hEdPage));
DEC_FUN(Handle, CED_GetParagraph,(Handle hEdPage,int _num));
DEC_FUN(Bool32, CED_GetFont,(Handle hEdPage,int number, uchar* fontNumber, uchar* fontPitchAndFamily, uchar* fontCharset,char* fontName));
DEC_FUN(int, CED_GetNumOfFonts,(Handle hEdPage));
DEC_FUN(uint32_t,CED_GetCountSection,(Handle hEdPage));
DEC_FUN(Handle, CED_GetSection,(Handle hEdPage, uint32_t number));
DEC_FUN(EDRECT, CED_GetSectionBorder,(Handle hEdSection));
DEC_FUN(uint32_t, CED_GetCountColumn,(Handle hEdSection));
DEC_FUN(Handle, CED_GetColumn,(Handle hEdSection,int number));
DEC_FUN(uint32_t, CED_GetColumnWidth,(Handle hEdColumn));
DEC_FUN(uint32_t, CED_GetColumnSpacing,(Handle hEdColumn));
//DEC_FUN(uint32_t, CED_GetCountFrame,(Handle hEdSection));
//DEC_FUN(Handle, CED_GetFrame,(Handle hEdSection,int number));
DEC_FUN(edBox, CED_GetFrameRect,(Handle hEdFrame));
DEC_FUN(Handle, CED_GetFirstObject,(Handle hObject));
DEC_FUN(Handle, CED_GetNextObject,(Handle hObject));
DEC_FUN(Bool32, CED_IsTable,(Handle hObject));
DEC_FUN(Bool32, CED_IsPicture,(Handle hObject));
DEC_FUN(Bool32, CED_IsFrame,(Handle hObject));
DEC_FUN(Bool32, CED_IsParagraph,(Handle hObject));
DEC_FUN(Bool32, CED_IsFictive,(Handle hEdParagraph));
DEC_FUN(uint32_t, CED_GetCountCell,(Handle hEdTable));
DEC_FUN(Handle, CED_GetCell,(Handle hEdTable, int number));
DEC_FUN(int32_t*,CED_GetLinesX,(Handle hEdTable));
DEC_FUN(int32_t *,CED_GetLinesY,(Handle hEdTable));
DEC_FUN(uint32_t*,CED_GetTableOfCells,(Handle hEdTable));
DEC_FUN(EDSIZE, CED_GetSize,(Handle hEdTable));
DEC_FUN(EDRECT, CED_GetIndent,(Handle hEdParagraph));
DEC_FUN(uint32_t,CED_GetAlignment,(Handle hEdParagraph));
DEC_FUN(EDBOX, CED_GetLayout,(Handle hEdParagraph));
DEC_FUN(uint32_t, CED_GetUserNumber,(Handle hEdParagraph));
DEC_FUN(EDSIZE, CED_GetInterval,(Handle hEdParagraph));
DEC_FUN(uint32_t, CED_GetCountLine,(Handle hEdParagraph));
DEC_FUN(Handle, CED_GetLine,(Handle hEdParagraph,int number));
DEC_FUN(uint32_t, CED_GetCountChar,(Handle hEdLine));
DEC_FUN(Handle, CED_GetChar,(Handle hEdLine,int number));
DEC_FUN(struct letterEx*, CED_GetAlternatives,(Handle hEdChar));
DEC_FUN(uint32_t, CED_GetFontHeight,(Handle hEdChar));
DEC_FUN(uint32_t, CED_GetFontStyle,(Handle hEdChar));
DEC_FUN(uint32_t, CED_GetFontLang,(Handle hEdChar));
DEC_FUN(EDBOX, CED_GetCharLayout,(Handle hEdChar));
*/
#undef DEC_FUN

CEDPage * Formattedload_96(char * file,Bool32 readFromFile, uint32_t bufLen);

#pragma pack(1)
struct pageDescr
{
        uint32_t paperw;
        uint32_t paperh;
        uint32_t margl;
        uint32_t margr;
        uint32_t margt;
        uint32_t margb;
	uchar  resizeToFit;
	uchar  recogLang;
};

#pragma pack(1)
struct fontDiscr
{
	uint16_t size;
    uchar fontNumber;
    uchar fontPitchAndFamily;
    uchar fontCharset;
};

#pragma pack(1)
struct pictDescr
{
        uint32_t size;
	uint16_t pictNumber;
	EDSIZE pictSize;
	EDSIZE pictGoal;
	uchar pictAlign;
	uchar type;
        uint32_t len;
};

#pragma pack(1)
struct originalImageDesc
{
	uint16_t resolutionX;
	uint16_t resolutionY;
	uint16_t inclune;
    uint16_t pageNum;
    uint32_t width;
    uint32_t height;
	uchar unrecogSymbol;
};

#pragma pack(1)
struct sectParams1
{
    uint32_t topMargin;
    uint32_t bottomMargin;
    uint32_t leftMargin;
    uint32_t rightMargin;
	uchar columns;
	uchar numSnakeCols;
    uint32_t colInterval;
};
#pragma pack(1)
struct sectParams2
{
	uchar sectionBreak;
    uint32_t width;
    uint32_t height;
	uchar orientation;
        uint32_t headerY;
        uint32_t footerY;
	uchar lineBetCol;
};

#pragma pack(1)
struct frameParam
{
	uchar position;
    uint32_t posx;
    uint32_t posy;
    uint32_t absw;
    uint32_t absh;
        uint32_t borderSpace;
        uint32_t dxfrtextx;
        uint32_t dxfrtexty;
	uchar flag;
};

#pragma pack(1)
struct rowParam
{
        uint32_t left;
        uint32_t rowHeight;
	uchar leftBrdrType;
        uint32_t leftBrdrWidth;
	uchar rightBrdrType;
        uint32_t rightBrdrWidth;
	uchar topBrdrType;
        uint32_t topBrdrWidth;
	uchar bottomBrdrType;
        uint32_t bottomBrdrWidth;
        uint32_t gaph;
	uchar position;
	uchar header;
};

#pragma pack(1)
struct cellParam
{
        uint32_t cellX;
	uchar merging;
	uchar vertTextAlign;
	uchar leftBrdrType;
        uint32_t leftBrdrWidth;
	uchar rightBrdrType;
        uint32_t rightBrdrWidth;
	uchar topBrdrType;
        uint32_t topBrdrWidth;
	uchar bottomBrdrType;
        uint32_t bottomBrdrWidth;
	EDBOX layout;
	uint16_t shading;
	uint16_t color;
	uchar flag;
};

#pragma pack(1)
struct paraParams
{
        uint32_t firstIndent;
        uint32_t leftIndent;
    uint32_t rightIndent;
    uchar  alignment;
	uint16_t  userNum;
	uint16_t  color;
	uint16_t  shading;
    uint32_t spaceBefore;
    uint32_t spaceAfter;
    uint32_t spaceBetweenLines;
	uchar  spcBtwLnsMult;
	uchar  keep;
	uchar leftBrdrType;
        uint32_t leftBrdrWidth;
	uchar rightBrdrType;
        uint32_t rightBrdrWidth;
	uchar topBrdrType;
        uint32_t topBrdrWidth;
	uchar bottomBrdrType;
        uint32_t bottomBrdrWidth;
	uchar brdrBtw;
};

#pragma pack(1)
struct charParams
{
        uint32_t fontNumber;
        uint32_t foregroundColor;
        uint32_t backgroundColor;
};
#pragma pack()

#define MAX_WIDTH 300
#define MAX_RTF_GROUPS     50

struct StrRtfColor {                       // color table in an rtf file
      COLORREF color;                      // color
      };

struct StrRtfOut {                        // Rtf output file processing block
      int  output;                        // rtr output type: RTF_FILE,RTF_BUF,RTF_CB
      HANDLE hFile;                        // stream if file is used
      HGLOBAL hBuf;                       // output buffer handle if using buffer output
      uchar *buf;                     // output buffer pointer if using buffer
      long BufLen;                        // allcated length of buffer
      long BufIndex;                      // index of next character in the buffer
      char text[MAX_WIDTH+1];             // temporary buffer to write text
      int  TextLen;                       // length of the text in the 'text' buffer
      Bool SpacePending;                  // TRUE if space needs to be written out after the last control
      Bool WritingControl;                // TRUE when writing a control word
      struct StrRtfColor *color;      // rtf color table pointer
      int  TotalColors;                   // total colors in the color table
      int  GroupLevel;
      uint flags;                         // ROFLAG_ flags
	  int RtfInHdrFtr;
	  Bool RtfInTable;
	  Bool RtfInFrame;
	  Bool wrtFrmSz;
	  CEDPage * page;
	  Bool reset;
	  CEDParagraph* PrevPfmt;
	  CEDParagraph* frm;
	  CEDChar PrevChar;
	  char * oldFile;					//data from file we write
	  int oldFileLen;					//their length
      int * table;						//table of correspondence between fonts of old and new rtf
	  int maxFntNum;
      Bool GroupBegin;                    // TRUE if the group begins
      Bool GroupEnd;                      // TRUE if the group ends
      Bool IsControlWord;                 // TRUE if control word
      long IntParam;                      // rtf control word parameter in integer numeric format
	  char CurWord[MAX_WIDTH+1];          // current word
      int  WordLen;                       // length of the current word
      int  TextIndex;                     // index if current uchar in the 'text' buffer
      char CurChar;                       // last character read
      Bool eof;                           // end of file
      char param[MAX_WIDTH/3];            // rtf control word parameter
      uchar stack[MAX_WIDTH+1];            // characters returned from previous operations
      int  StackLen;                      // number of characters in the stack

	};

struct StrRtfFont {                       // font table in an rtf file
//      Bool InUse;                         // TRUE when the slot is in use
      int  FontId;                        // font id
      char family[32];                    // font family
      char name[32];                      // font name
	  int  CharSet;		// Piter A
      };


#define RTF_FILE_INCOMPLETE 1
#define RTF_SYNTAX_ERROR    2
#define RTF_END_FILE        3

#define PARAM_NONE          0
#define PARAM_INT           1
#define PARAM_DBL           2

#define DEFAULT_CFMT       0x0
#define DEFAULT_PFMT		0x0

#define ROFLAG_IGNORE_TABLE   0x1      // ignore table information
#define ROFLAG_IGNORE_SLASH   0x2      // do not treat backslash as a special character
#define ROFLAG_IN_SUPSCR      0x4      // in superscript group
#define ROFLAG_IN_SUBSCR      0x8      // in subscript group

Bool BeginRtfGroup(/*PTERWND w,*/struct StrRtfOut *rtf);
Bool WriteRtfControl(/*PTERWND w,*/struct StrRtfOut *rtf, const char* control,int type, double val);
Bool WriteRtfFont(struct StrRtfOut *rtf, Bool head);
Bool WriteRtfMargin(/*PTERWND w,*/struct StrRtfOut *rtf);
Bool WriteRtfSection(/*PTERWND w,*/struct StrRtfOut *rtf, CEDSection* sect);
Bool WriteRtfCharFmt(/*PTERWND w,*/struct StrRtfOut *rtf,CEDChar* curChar);
Bool EndRtfGroup(/*PTERWND w,*/struct StrRtfOut *rtf);
Bool WriteRtfParaFmt(/*PTERWND w,*/struct StrRtfOut *rtf,CEDParagraph* NewPfmt,CEDParagraph* PrevPfmt/*, int NewCell, int PrevCell, int NewFID, int PrevFID*/);
Bool FlushRtfLine(/*PTERWND w,*/struct StrRtfOut *rtf);
Bool PutRtfChar(/*PTERWND w,*/struct StrRtfOut *rtf,uchar CurChar);
Bool WriteRtfText(/*PTERWND w,*/struct StrRtfOut *rtf, const char* text, int TextLen);
Bool WriteRtfRow(/*PTERWND w,*/struct StrRtfOut *rtf, CEDParagraph* NewCell,CEDParagraph * prevRow);
Bool WriteRtfCell(/*PTERWND w,*/struct StrRtfOut *rtf, CEDParagraph* NewCell);

Bool WriteRtfPara(struct StrRtfOut *rtf,CEDParagraph* p, Bool brk);
Bool WriteFrmPos(/*PTERWND w,*/struct StrRtfOut *rtf, CEDParagraph* frm, Bool writeWidth);
Bool WriteRtfDIB(/*PTERWND w,*/struct StrRtfOut *rtf,int pict);
Bool PutRtfHexChar(/*PTERWND w,*/struct StrRtfOut *rtf,uchar CurChar);
Bool WriteRtfMetafile(/*PTERWND w,*/struct StrRtfOut *rtf,int pict);
Bool WriteRtfMergedHeader(struct StrRtfOut *rtf, const char * name);
Bool WriteRtfParaBorder(struct StrRtfOut *rtf, CEDParagraph * para);
Bool WriteRtfColor(struct StrRtfOut *rtf,Bool head = TRUE);


extern char logName[_MAX_PATH];
extern FILE *logStream;



// error code


#pragma pack (pop)

#ifdef __cplusplus
            }
#endif

#endif
