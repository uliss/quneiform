/***************************************************************************
 *   Copyright (C) 2009 by Serge Poltavsky                                 *
 *   serge.poltavski@gmail.com                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program. If not, see <http://www.gnu.org/licenses/>   *
 ***************************************************************************/

#ifndef PAGE_H_
#define PAGE_H_

#include <string>
#include "ced_struct.h"
#include "size.h"

namespace CIF {

class CEDSection;
class CEDLine;
class CEDChar;

class CEDPage
{
public:
    int turn; // Tangent angle on the vertical images * 2048
    int pageNumber; // Number of Pages (= 0 not in batch mode)
    Size pageSizeInTwips; // The width of the page in twip (1dyuym = 1440tvipov) for text editor
    Rect pageBordersInTwips;
    char unrecogChar;
    char recogLang;
    Bool resizeToFit;

    int fontsUsed; //РљThe number of fonts used in table
    int fontsCreated; //РљThe number of fonts created in the table
    fontEntry* fontTable; // Pointer to the table fonts
    int picsUsed; //РљNumber of images used in table
    int picsCreated; //РљNumber of images created in the table
    pictEntry* picsTable; // pointer to a table of images

    char * extData; // Data cat. will be recorded in the file after the title
    int extDataLen; // Its size

    CEDPage();
    ~CEDPage();

    Bool FormattedWriteRtf(const char * fileName);

    CEDSection * GetSection(int _num);
    CEDParagraph * GetParagraph(int _num);
    CEDLine * GetLine(int _num);
    CEDChar * GetChar(int _num);

    Bool GoToNextSection();
    Bool GoToNextParagraph(Bool32 NonFictiveOnly);
    Bool GoToNextLine();
    Bool GoToNextChar();

    int GetNumberOfSections();
    int GetNumberOfParagraphs();
    int GetNumberOfLines();
    int GetNumberOfChars();

    Bool CreateFont(uchar fontNumber, uchar fontPitchAndFamily, uchar fontCharset, char* fontName);
    Bool GetFont(int number, uchar* fontNumber, uchar* fontPitchAndFamily, uchar* fontCharset,
            char** fontName);

    int GetFontByNum(uchar fontNumber);

    Bool CreatePicture(int pictNumber, const Size& pictSize, const Size& pictGoal, int pictAlign,
            int type, void * data, int len);

    CEDSection * InsertSection(); //inserts new section after current one. inserted one becomes current
    //sets pointer to the inserted one
    CEDSection * SetCurSection(CEDSection* _sect);//sets new value of current section
    CEDSection * SetCurSection(int _number);//sets new value of current section

    CEDSection * GetCurSection(); //returns current section
    int GetNumOfCurSection(); //returns current section

    CEDSection * NextSection(); //returns next section, 0 if last
    CEDSection * PrevSection(); //return previous section, 0 if first

    int NumberOfSections, NumberOfParagraphs, NumberOfLines, NumberOfChars;

    CEDSection * sections; //connected list of sections
    CEDSection * curSect; //current section

    /**
     * scanner resolution for this picture
     */
    Resolution dpi() const;

    /**
     * @return Filename image
     */
    std::string imageFilename() const;

    /**
     * Returns size of the original image in pixels
     */
    Size imageSize() const;

    void setDpi(const Resolution& dpi);
    void setImageFilename(const std::string& filename);
    void setImageSize(const Size& sz);

private:
    Size image_size_;
    Resolution dpi_;
    // Filename image. If the path is not specified, is searched in one ed directory
    std::string image_filename_;
};

}

#endif /* PAGE_H_ */
