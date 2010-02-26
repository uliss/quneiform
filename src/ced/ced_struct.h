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

#include "common/size.h"
#include "ced.h"

#define     FICTIVE         0xf000
#define     COLUMN_BEGIN    (1|FICTIVE)
#define     LAST_IN_COLUMN  (2|FICTIVE)
#define     FRAME_BEGIN     (3|FICTIVE)
#define     FRAME_END       (4|FICTIVE)
#define     TAB_BEGIN       (5|FICTIVE)
#define     TAB_CELL_BEGIN  (6|FICTIVE)
#define     TAB_END         (7|FICTIVE)
#define     TAB_ROW_BEGIN   (8|FICTIVE)
class CEDPage;
class CEDSection;
class CEDParagraph;
class CEDLine;
class CEDChar;

//all structures of ed... first field of Descr  is CEDParagraph* next, (in order to go along them

typedef struct edColDescr {
    CEDParagraph* next;
} EDCOLDESCR;

typedef struct edFrameDescr {
    CEDParagraph* last;
    EDBOX   rec;
    uchar position;
    int borderSpace;
    int dxfrtextx;
    int dxfrtexty;
    int flag;
} EDFRAMEDESCR;

typedef struct edTabDescr {
    CEDParagraph* next, *last, *cur;
    int numOfRows;
    int *table, *linesX, *linesY;
    edSize size;
}
EDTABDESCR;
typedef struct edCellDescr {
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
} EDCELLDESCR;
typedef struct edRowDescr {
    CEDParagraph* next, *last, *cur;//first cell, next paragraph with type RowBeg or TabEnd, cur - last of input cells
    int numOfCells;
    int left;
    int rowHeight;
    int leftBrdrType, leftBrdrWidth;
    int rightBrdrType, rightBrdrWidth;
    int topBrdrType, topBrdrWidth;
    int bottomBrdrType, bottomBrdrWidth;
    int gaph;
    int position;
    Bool32 header;
} EDROWDESCR;

typedef struct fontEntry {
    uchar fontNumber;
    uchar fontPitchAndFamily;
    uchar fontCharset;
    char* fontName;
} FONTENTRY;

typedef struct pictEntry {
    uint16_t pictNumber;
    CIF::Size pictSize;
    EDSIZE pictGoal;
    uchar pictAlign;
    uchar type;
    uint32_t len;
    void* data;
} PICTENTRY;

#define DEC_FUN(a,b,c) typedef a (*FN##b)c

DEC_FUN(Bool32, CED_FormattedWrite, ( const char * fileName, CEDPage *page));
DEC_FUN(CEDPage*, CED_FormattedLoad, (char * file, Bool32 readFromFile, uint32_t bufLen));
DEC_FUN(void, CED_DeleteTree, (CEDPage * pg));


#undef DEC_FUN

class CED_FUNC(CEDPage)
{
    public:

        //picture data
        EDSIZE  sizeOfImage;        // The size of the original image in pixels
        EDSIZE  dpi;                //scanner resolution for this picture
        int     turn;               // Tangent angle on the vertical images * 2048
        char*   imageName;          // Filename image. If the path is not specified, is searched in one
        // Directory with the file ed

        int     pageNumber;         // Number of Pages (= 0 not in batch mode)
        EDSIZE  pageSizeInTwips;    // The width of the page in twip (1dyuym = 1440tvipov) for text editor
        EDRECT  pageBordersInTwips;
        char    unrecogChar;
        char    recogLang;
        Bool32  resizeToFit;

        int     fontsUsed;          //РљThe number of fonts used in table
        int     fontsCreated;       //РљThe number of fonts created in the table
        fontEntry*  fontTable;      // Pointer to the table fonts
        int     picsUsed;           //РљNumber of images used in table
        int     picsCreated;        //РљNumber of images created in the table
        pictEntry*  picsTable;      // pointer to a table of images

        char *  extData;            // Data cat. will be recorded in the file after the title
        int     extDataLen;         // Its size

        CEDPage();
        ~CEDPage();

        Bool32  FormattedWriteRtf(const char * fileName, Bool merge);

        CEDSection * GetSection(int _num);
        CEDParagraph * GetParagraph(int _num);
        CEDLine * GetLine(int _num);
        CEDChar * GetChar(int _num);

        Bool32  GoToNextSection();
        Bool32  GoToNextParagraph(Bool32 NonFictiveOnly);
        Bool32  GoToNextLine();
        Bool32  GoToNextChar();

        int GetNumberOfSections();
        int GetNumberOfParagraphs();
        int GetNumberOfLines();
        int GetNumberOfChars();

        Bool32 CreateFont(uchar fontNumber, uchar fontPitchAndFamily, uchar fontCharset,
                          char* fontName);
        Bool32 GetFont(int number, uchar* fontNumber, uchar* fontPitchAndFamily, uchar* fontCharset,
                       char** fontName);

        int GetFontByNum(uchar fontNumber);

        Bool32 CreatePicture(int pictNumber, const CIF::Size& pictSize, EDSIZE pictGoal, int pictAlign, int type, void * data, int len);

        CEDSection * InsertSection();   //inserts new section after current one. inserted one becomes current
        //sets pointer to the inserted one
//  CEDSection * DeleteSection(Bool32 _deleteSubItems); //deletes current section. previous one becomes current
        //return it
        //_deleteSubItems - either delete all daughter elements or attach it to previous object
        CEDSection * SetCurSection(CEDSection* _sect);//sets new value of current section
        CEDSection * SetCurSection(int _number);//sets new value of current section

        CEDSection * GetCurSection();   //returns current section
        int     GetNumOfCurSection();   //returns current section

        CEDSection * NextSection(); //returns next section, 0 if last
        CEDSection * PrevSection(); //return previous section, 0 if first

        int NumberOfSections, NumberOfParagraphs, NumberOfLines, NumberOfChars;

        CEDSection * sections;      //connected list of sections
//  CEDParagraph * paragraphs;  //connected list of paragraphs
//  CEDLine * lines;
//  CEDChar * chars;

        CEDSection * curSect;       //current section
};

#endif// _CED_STRUCT_
