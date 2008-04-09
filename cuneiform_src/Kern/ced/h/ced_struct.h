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

#pragma message(__FILE__"(1)")  // BC doesn't understand it somewhy...

#ifndef _CED_STRUCT_
#define _CED_STRUCT_


#include <windows.h>
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

//Все структуры ed...Descr должны иметь первым полем 
//CEDParagraph* next, чтобы можно было по ним ходить

typedef struct edColDescr
{
	CEDParagraph* next;
}EDCOLDESCR;

typedef struct edFrameDescr
{
	CEDParagraph* last;
	EDBOX	rec;
	BYTE position;	
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
	CEDParagraph* next,*last,*cur;//Первая ячейка, след. абзац с типом RowBeg либо TabEnd,cur - последняя введенная ячейка
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
	BYTE fontNumber;
    BYTE fontPitchAndFamily;
    BYTE fontCharset;
    char* fontName;
}FONTENTRY;
typedef struct pictEntry
{
	WORD pictNumber;
	EDSIZE pictSize;
	EDSIZE pictGoal;
	BYTE pictAlign;
	BYTE type;
	DWORD len;
	void* data;
}PICTENTRY;
#define DEC_FUN(a,b,c) typedef a (*FN##b)c

DEC_FUN(Bool32,CED_FormattedWrite,(char * fileName, CEDPage *page));
DEC_FUN(CEDPage*,CED_FormattedLoad,(char * file,Bool32 readFromFile, Word32 bufLen));
DEC_FUN(void,CED_DeleteTree,(CEDPage * pg));


#undef DEC_FUN

class CED_FUNC(CEDPage)
{
public:
	
	//Данные о изображении
	EDSIZE	sizeOfImage;		//Размеры исходной картинки в пикселях
	EDSIZE	dpi;				//Разрешение сканера для этой картинки
	int		turn;				//Тангенс угла поворота картинки относительно вертикали*2048
	char*	imageName;			//Имя файла изображения. Если путь не указан, ищется в одном 
								//каталоге с ed файлом
	
	int		pageNumber;			//Номер страницы(=0 не в пакетном режиме)
	EDSIZE	pageSizeInTwips;	//Ширина страницы в твипах(1дюйм=1440твипов) для текстового редактора
	EDRECT	pageBordersInTwips;	//Поля страницы в твипах
	char	unrecogChar;
	char	recogLang;
	Bool32  resizeToFit;

	int		fontsUsed;			//Количество использованных шрифтов в таблице
	int		fontsCreated;			//Количество созданных шрифтов в таблице
	fontEntry*	fontTable;			//указатель на таблицу шрифтов
	int		picsUsed;			//Количество использованных картинок в таблице
	int		picsCreated;			//Количество созданных картинок в таблице
	pictEntry*	picsTable;			//указатель на таблицу картинок

	char *	extData;			//Данные, кот. будут записаны в файл после заголовка;
	int		extDataLen;			//Их длина

	CEDPage();
	~CEDPage();
	
	Bool32	FormattedWriteRtf(const char * fileName, BOOL merge);
	
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
	
	Bool32 CreateFont(BYTE fontNumber, BYTE fontPitchAndFamily, BYTE fontCharset,
				char* fontName);
	Bool32 GetFont(int number, BYTE* fontNumber, BYTE* fontPitchAndFamily, BYTE* fontCharset,
				char** fontName);
	
	int GetFontByNum(BYTE fontNumber);

	Bool32 CreatePicture(int pictNumber, EDSIZE pictSize, EDSIZE pictGoal, int pictAlign, int type, void * data, int len);

	CEDSection * InsertSection();	//Вставляет новую секцию после текущей. Текущей становится новая секция
								//Возвращает указатель на вновь созданную секцию
//	CEDSection * DeleteSection(Bool32 _deleteSubItems);	//Удаляет текущую секцию. Текущей становится предыдущая секция.
								//Она и возвращается 
				//_deleteSubItems - удалять ли все дочерние элементы, либо прикрепить их
				//к предыдущему объекту
	CEDSection * SetCurSection(CEDSection* _sect);//Устанавливает новое значение текущей секции
	CEDSection * SetCurSection(int _number);//Устанавливает новое значение текущей секции
	
	CEDSection * GetCurSection();	//Возвращает текущую секцию
	int		GetNumOfCurSection();	//Возвращает текущую секцию
	
	CEDSection * NextSection();	//Возвращает секцию, следующую за текущей,0 - если секция последняя
	CEDSection * PrevSection();	//Возвращает секцию, предыдущую текущей,0 - если секция первая

	int NumberOfSections,NumberOfParagraphs,NumberOfLines,NumberOfChars;

	CEDSection * sections;		//Связный список из секций
//	CEDParagraph * paragraphs;	//Связный список абзатцев
//	CEDLine * lines;
//	CEDChar * chars;
	
	CEDSection * curSect;		//Текущая секуия
	};

class CED_FUNC(CEDSection)
{
public:
	int		numberOfColumns;	//Количество колонок в секции(n штук)
//	int		numberOfFrames;		//Количество фреймов-1 в секции(n-1 штук)
	//SIZE *	pColSize;			//Указатель на n-1 штуку структур SIZE.cx-ширина колонки,
								//SIZE.cy - расстояние до следующей колонки в твипах
								//Ширина_последней_колонки = ширина_страницы - ширина_полей -
								//- ширина_пред.колонок - ширина_итервалов_между_колонками
	EDRECT	borders;			//отступы от края бумаги
	int colInterval;
	char sectionBreak;
	int width;
	int height;
	char orientation;
	int headerY;
	int footerY;
	int numSnakeCols;
	BOOL lineBetCol;
	EDCOL *colInfo;
	char *	extData;			//Данные, кот. будут записаны в файл после заголовка;
	int		extDataLen;			//Их длина

	CEDParagraph * CreateColumn();
	CEDParagraph * CreateFrame(CEDParagraph* hObject, edBox rect, char position=-1, DWORD borderSpace=-1, DWORD dxfrtextx=-1, DWORD dxfrtexty=-1);
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
	
	CEDParagraph * InsertParagraph(Bool32 AfterCurrent=TRUE);	//Вставляет новый абзац после текущего. Текущей становится новый абзац
								//Возвращает указатель на вновь созданный абзац
//	CEDParagraph * DeleteParagraph(Bool32 _deleteSubItems);	//Удаляет текущий абзац. Текущим становится предыдущий абзац.
								//Он и возвращается 
				//_deleteSubItems - удалять ли все дочерние элементы, либо прикрепить их
				//к предыдущему объекту
	CEDParagraph *	SetCurParagraph(CEDParagraph* _para);//Устанавливает новое значение текущего абзаца
	CEDParagraph * SetCurParagraph(int _number);//Устанавливает новое значение текущей абзаца

	CEDParagraph * GetCurParagraph();	//Возвращает текущий абзац 
	int		GetNumOfCurParagraph();	//Возвращает текущий абзац
	
	CEDParagraph * NextParagraph(Bool32 _goThroughSect);	//Возвращает абзац, следующий за текущим,0 - если абзац последний
	CEDParagraph * PrevParagraph(Bool32 _goThroughSect);	//Возвращает абзац, предыдущий текущему,0 - если абзац первый
		//Если _goThroughSect = TRUE, то крайним считается последний абзац в файле, иначе- в секции

	CEDParagraph * paragraphs;	//Связный список абзацев
	CEDParagraph * columnsBeg,*columnsEnd,*columnsCur/*,*framesBeg,*framesEnd,*framesCur*/;

	CEDParagraph * curPara;//Текущий абзац
	CEDSection * prev,*next;		//Указатели на соседние эл-ты в списке
	int internalNumber;			//Порядковый номер абзаца он начала файла

friend CEDPage;
};

class CED_FUNC(CEDParagraph)
{
public:
	int		type;					//Тип абзаца
	int		alignment;				//Выравнивание абзатца
	EDRECT	indent;					//Отступы: left=левый, right=ширина,top=красн.строка(в твипах)
	int		userNumber;				//Номер, данный пользователем на этапе фрагментации
	int		border;					//Рамка вокруг абзатца
	EDSIZE	interval;				//cx-верхний отступ, cy- нижний

	edBox		layout;					//Расположение абзаца на странице
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

	void *	descriptor;			//Указатель на расширенный описатель специальных структур
	
	char *	extData;			//Данные, кот. будут записаны в файл после заголовка;
	int		extDataLen;			//Их длина

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
	CEDLine * InsertLine();	//Вставляет новую строку после текущей. Текущей становится новая строкя
								//Возвращает указатель на вновь созданную строку
	//CEDLine * DeleteLine(Bool32 _deleteSubItems);	//Удаляет текущую строку. Текущей становится предыдущая строка.
								//Она и возвращается 
				//_deleteSubItems - удалять ли все дочерние элементы, либо прикрепить их
				//к предыдущему объекту

	void	SetCurLine(CEDLine* _line);//Устанавливает новое значение текущей строки
	CEDLine * SetCurLine(int _number);//Устанавливает новое значение текущей строки
	
	CEDLine * GetCurLine();	//Возвращает текущую строку
	int		GetNumOfCurLine();	//Возвращает текущую строку
	
	CEDLine * NextLine(Bool32 _goThroughPara);	//Возвращает строку, следующую за текущей,0 - если строка последняя
	CEDLine * PrevLine(Bool32 _goThroughPara);	//Возвращает строку, предыдущую текущей,0 - если строка первая
		//Если _goThroughSect = TRUE, то крайним считается последняя строка в файле, иначе- в абзаце

	CEDLine * lines;	//Связный список строк
	int		numOfLines;
	CEDLine * curLine;		//Текущая строка
	CEDParagraph * prev,*next;		//Указатели на соседние эл-ты в списке
	int internalNumber;			//Порядковый номер абзаца он начала файла
	int parentNumber;			//Порядковый номер родителя в файле
friend CEDSection;
friend CEDPage;
};

class CED_FUNC(CEDLine)
{
public:
	char *	extData;			//Данные, кот. будут записаны в файл после заголовка;
	int		extDataLen;			//Их длина
	Bool32	hardBreak;		
	int defChrFontHeight;

	CEDLine();
	~CEDLine();
	
	CEDChar*	GetChar(int _num);	
	int	GetCountChar();	

	CEDChar * InsertChar();	//Вставляет новый символ после текущего. Текущим становится новый символ
								//Возвращает указатель на вновь созданный символ
//	CEDChar * DeleteChar(Bool32 _deleteSubItems);	//Удаляет текущий символ. Текущим становится предыдущий символ.
								//Он и возвращается 
					//_deleteSubItems - удалять ли все дочерние элементы, либо прикрепить их
				//к предыдущему объекту

	void	SetCurChar(CEDChar* _char);//Устанавливает новое значение текущго символа
	CEDChar * SetCurChar(int _number);//Устанавливает новое значение текущго символа
	
	CEDChar * GetCurChar();	//Возвращает текущий символ
	int		GetNumOfCurChar();	//Возвращает текущий символ
	
	CEDChar * NextChar(Bool32 _goThroughLines);	//Возвращает символ, следующую за текущим,0 - если символ последний
	CEDChar * PrevChar(Bool32 _goThroughLines);	//Возвращает символ, предыдущий текущему,0 - если символ первый
		//Если _goThroughSect = TRUE, то крайним считается последний символ в файле, иначе- в строке

	CEDChar * chars;	//Связный список символов
	int		numOfChars;

	CEDChar * curChar;			//Текущий символ
	CEDLine * prev,*next;		//Указатели на соседние эл-ты в списке
	int internalNumber;			//Порядковый номер строки он начала файла
	int parentNumber;			//Порядковый номер родителя в файле
friend CEDParagraph;
friend CEDPage;
};

class CED_FUNC(CEDChar)
{
public:
	edRect		layout;			//Размещение символа на исходном изображении(в пикселях)
	int		fontHeight,fontAttribs;	//параметры шрифта
	int fontNum;
	int fontLang;
	int foregroundColor;
	int backgroundColor;
	letterEx * alternatives;	//Массив альтернатив
	int		numOfAltern;
	char *	extData;			//Данные, кот. будут записаны в файл после заголовка;
	int		extDataLen;			//Их длина

	CEDChar();
	~CEDChar();
	CEDChar * prev,*next;		//Указатели на соседние эл-ты в списке
protected:
//	int internalNumber;			//Порядковый номер строки он начала файла
	int parentNumber;			//Порядковый номер родителя в файле
friend CEDLine;
friend CEDPage;
friend void FormattedTR(const text_ref* pt);
friend void StripLines();
};


#endif _CED_STRUCT_