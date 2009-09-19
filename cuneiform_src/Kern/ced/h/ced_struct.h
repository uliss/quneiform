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

//Filename: ced_struct.h
//Created 7.12.98 by Bozhenov Artem, (C)CT inc.

#ifndef _CED_STRUCT_
#define _CED_STRUCT_


/*#include <windows.h>*/
//#include "edfile.h"
#include "ced.h"

#define		FICTIVE			0xf000
#define		COLUMN_BEGIN	(1|FICTIVE)
#define		LAST_IN_COLUMN	(2|FICTIVE)
#define		FRAME_BEGIN		(3|FICTIVE)
#define		FRAME_END		(4|FICTIVE)
#define		TAB_BEGIN		(5|FICTIVE)
#define		TAB_CELL_BEGIN	(6|FICTIVE)
#define		TAB_END			(7|FICTIVE)
#define		TAB_ROW_BEGIN	(8|FICTIVE)
class CEDPage;
class CEDSection;
class CEDParagraph;
class CEDLine;
class CEDChar;

//all structures of ed... first field of Descr  is CEDParagraph* next, (in order to go along them

typedef struct edColDescr
{
	CEDParagraph* next;
}EDCOLDESCR;

typedef struct edFrameDescr
{
	CEDParagraph* last;
	EDBOX	rec;
	uchar position;
	int borderSpace;
	int dxfrtextx;
	int dxfrtexty;
	int flag;
} EDFRAMEDESCR;

typedef struct edTabDescr
{
	CEDParagraph* next,*last,*cur;
	int numOfRows;
	int *table,*linesX,*linesY;
	edSize size;
}
EDTABDESCR;
typedef struct edCellDescr
{
	CEDParagraph* next;
	int cellX;
	int merging;
	int vertTextAlign;
	int leftBrdrType, leftBrdrWidth;
	int rightBrdrType, rightBrdrWidth;
	int topBrdrType, topBrdrWidth;
	int bottomBrdrType, bottomBrdrWidth;
	EDBOX layout;
	int shading;
	int color;
	int flag;
}EDCELLDESCR;
typedef struct edRowDescr
{
	CEDParagraph* next,*last,*cur;//first cell, next paragraph with type RowBeg or TabEnd, cur - last of input cells
	int numOfCells;
	int left;
	int rowHeight;
	int leftBrdrType,leftBrdrWidth;
	int rightBrdrType, rightBrdrWidth;
	int topBrdrType, topBrdrWidth;
	int bottomBrdrType, bottomBrdrWidth;
	int gaph;
	int position;
	Bool32 header;
}EDROWDESCR;

typedef struct fontEntry
{
	uchar fontNumber;
    uchar fontPitchAndFamily;
    uchar fontCharset;
    char* fontName;
}FONTENTRY;
typedef struct pictEntry
{
	uint16_t pictNumber;
	EDSIZE pictSize;
	EDSIZE pictGoal;
	uchar pictAlign;
	uchar type;
        uint32_t len;
	void* data;
}PICTENTRY;
#define DEC_FUN(a,b,c) typedef a (*FN##b)c

DEC_FUN(Bool32,CED_FormattedWrite,(char * fileName, CEDPage *page));
DEC_FUN(CEDPage*,CED_FormattedLoad,(char * file,Bool32 readFromFile, uint32_t bufLen));
DEC_FUN(void,CED_DeleteTree,(CEDPage * pg));


#undef DEC_FUN

class CED_FUNC(CEDPage)
{
public:

	//picture data
	EDSIZE	sizeOfImage;		// The size of the original image in pixels
	EDSIZE	dpi;				//scanner resolution for this picture
	int		turn;				// Tangent angle on the vertical images * 2048
	char*	imageName;			// Filename image. If the path is not specified, is searched in one
	                            // Directory with the file ed

	int		pageNumber;			// Number of Pages (= 0 not in batch mode)
	EDSIZE	pageSizeInTwips;	// The width of the page in twip (1dyuym = 1440tvipov) for text editor
	EDRECT	pageBordersInTwips;
	char	unrecogChar;
	char	recogLang;
	Bool32  resizeToFit;

	int		fontsUsed;			//РљThe number of fonts used in table
	int		fontsCreated;		//РљThe number of fonts created in the table
	fontEntry*	fontTable;		// Pointer to the table fonts
	int		picsUsed;			//РљNumber of images used in table
	int		picsCreated;		//РљNumber of images created in the table
	pictEntry*	picsTable;		// pointer to a table of images

	char *	extData;			// Data cat. will be recorded in the file after the title
	int		extDataLen;			// Its size

	CEDPage();
	~CEDPage();

	Bool32	FormattedWriteRtf(const char * fileName, Bool merge);

	CEDSection * GetSection(int _num);
	CEDParagraph * GetParagraph(int _num);
	CEDLine * GetLine(int _num);
	CEDChar * GetChar(int _num);

	Bool32	GoToNextSection();
	Bool32	GoToNextParagraph(Bool32 NonFictiveOnly);
	Bool32	GoToNextLine();
	Bool32	GoToNextChar();

	int	GetNumberOfSections();
	int	GetNumberOfParagraphs();
	int	GetNumberOfLines();
	int	GetNumberOfChars();

	Bool32 CreateFont(uchar fontNumber, uchar fontPitchAndFamily, uchar fontCharset,
				char* fontName);
	Bool32 GetFont(int number, uchar* fontNumber, uchar* fontPitchAndFamily, uchar* fontCharset,
				char** fontName);

	int GetFontByNum(uchar fontNumber);

	Bool32 CreatePicture(int pictNumber, EDSIZE pictSize, EDSIZE pictGoal, int pictAlign, int type, void * data, int len);

	CEDSection * InsertSection();	//inserts new section after current one. inserted one becomes current
								//sets pointer to the inserted one
//	CEDSection * DeleteSection(Bool32 _deleteSubItems);	//deletes current section. previous one becomes current
								//return it
				//_deleteSubItems - either delete all daughter elements or attach it to previous object
	CEDSection * SetCurSection(CEDSection* _sect);//sets new value of current section
	CEDSection * SetCurSection(int _number);//sets new value of current section

	CEDSection * GetCurSection();	//returns current section
	int		GetNumOfCurSection();	//returns current section

	CEDSection * NextSection();	//returns next section, 0 if last
	CEDSection * PrevSection();	//return previous section, 0 if first

	int NumberOfSections,NumberOfParagraphs,NumberOfLines,NumberOfChars;

	CEDSection * sections;		//connected list of sections
//	CEDParagraph * paragraphs;	//connected list of paragraphs
//	CEDLine * lines;
//	CEDChar * chars;

	CEDSection * curSect;		//current section
	};

class CED_FUNC(CEDSection)
{
public:
	int		numberOfColumns;	//Рљ The number of columns in the section (n pieces)
//	int		numberOfFrames;		//РљNumber of frames-1 in the section (n-1 pieces)
	//SIZE *	pColSize;		// Pointer to n-1 piece structures SIZE.cx-width columns,
								// SIZE.cy - the distance to the next column in twip
	                            // width_last_column width = page - width_fields --
	                            // width_columns_before - width_between_columns_itervalov

	EDRECT	borders;			// padding from the edge of paper
	int colInterval;
	char sectionBreak;
	int width;
	int height;
	char orientation;
	int headerY;
	int footerY;
	int numSnakeCols;
	Bool lineBetCol;
	EDCOL *colInfo;
	char *	extData;			// Data cat. will be recorded in the file after the title;
	int		extDataLen;			// Data size.

	CEDParagraph * CreateColumn();
	CEDParagraph * CreateFrame(CEDParagraph* hObject, edBox rect, char position=-1, int borderSpace=-1, int dxfrtextx=-1, int dxfrtexty=-1);
	CEDParagraph * CreateParagraph(CEDParagraph * hObject,int align, EDRECT indent, int UserNum,int FlagBorder,EDSIZE interval, edBox layout, int  color, int  shading,
						   int spaceBetweenLines, char spcBtwLnsMult,
						   char  keep);

	CEDParagraph * CreateTable(CEDParagraph * hObject);
//	CEDParagraph * ReCreateTable(CEDParagraph * hTable,EDSIZE sz, int * cx,int * cy, int * table,
//				   Bool32 * bHorShow,Bool32 * bVerShow);
	CEDParagraph * CreateTableRow(CEDParagraph * hTable,
		int left, int rowHeight,
		int leftBrdrType, int leftBrdrWidth,
		int rightBrdrType, int rightBrdrWidth,
		int topBrdrType, int topBrdrWidth,
		int bottomBrdrType, int bottomBrdrWidth,
		int gaph, int position, Bool32 header);

	CEDParagraph * CreateCell(CEDParagraph* hRow, int cellX, int merging, int vertTextAlign, int leftBrdrType, int leftBrdrWidth, int rightBrdrType, int rightBrdrWidth, int topBrdrType, int topBrdrWidth, int bottomBrdrType, int bottomBrdrWidth, EDBOX layout, int shading, int color);

	CEDParagraph * GetColumn( int _num);
//	CEDParagraph * GetFrame( int _num);

	CEDSection();
	~CEDSection();

	CEDParagraph * InsertParagraph(Bool32 AfterCurrent=TRUE);	// Inserts new paragraph after the current. Current becomes a new paragraph
								// Returns pointer to the newly created bullet
//	CEDParagraph * DeleteParagraph(Bool32 _deleteSubItems);	//deletes current paragraph. previous one becomes current
								//return it
				//_deleteSubItems - either delete all daughter elements or attach it to previous object
	CEDParagraph *	SetCurParagraph(CEDParagraph* _para);//sets new value of current paragraph
	CEDParagraph * SetCurParagraph(int _number);//sets new value of current paragraph

	CEDParagraph * GetCurParagraph();	//returns current paragraph
	int		GetNumOfCurParagraph();	//returns current paragraph

	CEDParagraph * NextParagraph(Bool32 _goThroughSect);	//returns next paragraph, 0 if last
	CEDParagraph * PrevParagraph(Bool32 _goThroughSect);	//returns previous paragraph, 0 if first
		//If _goThroughSect = TRUE, then we consider boundary paragraphs in file, othrwise in section

	CEDParagraph * paragraphs;	//connected list of paragraphs
	CEDParagraph * columnsBeg,*columnsEnd,*columnsCur/*,*framesBeg,*framesEnd,*framesCur*/;

	CEDParagraph * curPara;//current paragraph
	CEDSection * prev,*next;		//pointer to neibor elements in connected list
	int internalNumber;			//number of paragraph from start of the file

friend class CEDPage;
};

class CED_FUNC(CEDParagraph)
{
public:
	int		type;					// Type paragraph
	int		alignment;				// Alignment abzattsa
	EDRECT	indent;					// Indentation: left = left, right = width, top = red.line (in twip)
	int		userNumber;				// ID number, the user at the stage of fragmentation
	int		border;					//  frame around abzattsa
	EDSIZE	interval;				//  cx-upper indentation, cy-bottom

	edBox		layout;					//  Location paragraph on page
	int		color;
	int		shading;
	int		spaceBetweenLines;
	char	spcBtwLnsMult;
	char	keep;

	int leftBrdrType;
	int rightBrdrType;
	int topBrdrType;
	int bottomBrdrType;
	int leftBrdrWidth;
	int rightBrdrWidth;
	int topBrdrWidth;
	int bottomBrdrWidth;
	int brdrBtw;

	void *	descriptor;			// Pointer to advanced descriptor of special structures

	char *	extData;			// Data cat. will be recorded in the file after the title;
	int		extDataLen;			// Its size

	CEDParagraph();
	~CEDParagraph();

	CEDLine*	GetLine(int _num);
	int	GetCountLine();

	CEDParagraph* GetFirstObject();
	CEDParagraph*	GetNextObject();
	CEDParagraph* GetCell(int cell);
	CEDParagraph* GetRow(int row);
	CEDParagraph* GetLogicalCell(int number);
	int GetCountLogicalCell();
	void CreateTableOfCells();
	CEDLine * InsertLine();	//inserts new line after current one. new line becomes current
								//returns pointer to new line
	//CEDLine * DeleteLine(Bool32 _deleteSubItems);	//deletes current line. previous one becomes current
								//return it
				//_deleteSubItems - either delete all daughter elements or attach it to previous object

	void	SetCurLine(CEDLine* _line);//sets new value of current line
	CEDLine * SetCurLine(int _number);//sets new value of current line

	CEDLine * GetCurLine();	//returns current line
	int		GetNumOfCurLine();	//returns current line

	CEDLine * NextLine(Bool32 _goThroughPara);	//returns next line, 0 if last
	CEDLine * PrevLine(Bool32 _goThroughPara);	//returns previous line, 0 if first
		//If _goThroughSect = TRUE, then we consider boundary lines in file, otherwise in paragraph

	CEDLine * lines;	//connected list of lines
	int		numOfLines;
	CEDLine * curLine;		//current line
	CEDParagraph * prev,*next;		//pointer to neibor elements in connected list
	int internalNumber;			//number of paragraph from start of file
	int parentNumber;			//number of parent in file
friend class CEDSection;
friend class CEDPage;
};

class CED_FUNC(CEDLine)
{
public:
	char *	extData;			//data to be written in file after header
	int		extDataLen;			//their length
	Bool32	hardBreak;
	int defChrFontHeight;

	CEDLine();
	~CEDLine();

	CEDChar*	GetChar(int _num);
	int	GetCountChar();

	CEDChar * InsertChar();	//inserts new symbol after current one. new symbol becomes current
								//returns pointer to new symbol
//	CEDChar * DeleteChar(Bool32 _deleteSubItems);	//deletes current symbol. previous one becomes current
								//return it
					//_deleteSubItems - either delete all daughter elements or attach it to previous object
	void	SetCurChar(CEDChar* _char);//set new value of current symbol
	CEDChar * SetCurChar(int _number);//set new value of current symbol

	CEDChar * GetCurChar();	//return current symbol
	int		GetNumOfCurChar();	//return current symbol

	CEDChar * NextChar(Bool32 _goThroughLines);	//returns next symbol, 0 if last
	CEDChar * PrevChar(Bool32 _goThroughLines);	//returns previous symbol, 0 if first
		//if _goThroughSect = TRUE, then we consider last symbol in file, otherwise in line

	CEDChar * chars;	//connected list of symbols
	int		numOfChars;

	CEDChar * curChar;			//current symbol
	CEDLine * prev,*next;		//pointer to neibor elements in connected list
	int internalNumber;			//number of line from start of file
	int parentNumber;			//number of parent in a file
friend class CEDParagraph;
friend class CEDPage;
};

class CED_FUNC(CEDChar)
{
public:
	edRect		layout;			//layout of symbol in input image (in pixel)
	int		fontHeight,fontAttribs;	//font parameters
	int fontNum;
	int fontLang;
	int foregroundColor;
	int backgroundColor;
	letterEx * alternatives;	//array of alternatives
	int		numOfAltern;
	char *	extData;			//data to be written in file after header
	int		extDataLen;			//their length

	CEDChar();
	~CEDChar();
	CEDChar * prev,*next;		//pointer to neibor elements in connected list
protected:
//	int internalNumber;			//number of lines from start of file
	int parentNumber;			//number of parent in a file
friend class CEDLine;
friend class CEDPage;
friend void FormattedTR(const text_ref* pt);
friend void StripLines();
};


#endif// _CED_STRUCT_
