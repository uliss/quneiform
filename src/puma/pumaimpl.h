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

#ifndef PUMAIMPL_H_
#define PUMAIMPL_H_

#include <string>
#include <iosfwd>
#include <memory>
#include <boost/shared_ptr.hpp>

#include "cfcompat.h"
#include "specprj.h"
#include "common/formatoptions.h"
#include "common/rect.h"
#include "common/exception.h"
#include "common/outputformat.h"
#include "common/image.h"
#include "common/recognizeoptions.h"
#include "cimage/imageinfo.h"
#include "export/exporter.h"
#include "globus.h"

class CTIControl;
struct CCOM_cont;

namespace cf {

class PageMarker;
class ComponentExtractor;
class CEDPage;
class Formatter;
class CRtfPage;
class RStuff;

class PumaImpl
{
    public:
        PumaImpl();
        ~PumaImpl();

        /**
          * Returns pointer to CEDPage
          */
        CEDPagePtr cedPage();

        /**
         * Make some cleanup after recognition
         */
        void close();

        /**
         * Returns format options
         */
        FormatOptions formatOptions() const;

        /**
         * Formats results of recognition to CEDPage * tree
         */
        void formatResult();

        /**
         * Opens image for recognition
         */
        void open(ImagePtr img);

        /**
         * Returns image working area
         */
        Rect pageTemplate() const;

        /**
         * Recognizes image
         */
        void recognize();

        /**
         * Sets format options
         * @see setRecognizeOptions()
         */
        void setFormatOptions(const FormatOptions& opt);

        /**
         * Sets recognize options
         * @see setFormatOptions()
         */
        void setRecognizeOptions(const RecognizeOptions& opt);

        /**
         * Sets working region on input image
         */
        void setPageTemplate(const Rect& r);
        void setSpecialProject(special_project_t SpecialProject);
    private:
        void binarizeImage();
        void clearAll();
        void debugPrintCpage();
        void extractComponents();
        void extractStrings();
        void getImageInfo(const std::string& image_name);
        void layout();
        void loadLayoutFromFile(const std::string& fname);
        void markup();
        void modulesDone();
        void modulesInit();
        const char * modulePath() const;
        const char * moduleTmpPath() const;
        void normalize();
        void pass1();
        void pass2();
        void pass2special();
        void preOpenInitialize();
        void printResult(std::ostream& os);
        void printResultLine(std::ostream& os, size_t lineNumber);
        void postOpenInitialize();
        void recognizeCorrection();
        void recognizePass1();
        void recognizePass2();
        void recognizeSetup();
        void recognizeSpecial();
        void rotate(void * dib, Point * p);
        void saveCSTR(int pass);
        void saveLayoutToFile(const std::string& fname);
        void saveToText(std::ostream& os) const;
        void saveToText(const std::string& filename) const;
        void spellCorrection();
    private:
        static bool hasUpdateFlag(uint32_t flg);
        static void setUpdateFlag(uint32_t flg);
        static void unsetUpdateFlag(uint32_t flg);
        static void SetUpdate(uint32_t flgAdd, uint32_t flgRemove);
    private:
        static uint32_t update_flags_;
    private:
        std::auto_ptr<ComponentExtractor> comp_extractor_;
        std::auto_ptr<Formatter> formatter_;
        boost::shared_ptr<PageMarker> marker_;
        boost::shared_ptr<RStuff> rstuff_;
        BitmapInfoHeader info_;
        Rect rect_template_;
        std::string input_filename_;
        std::string layout_filename_;
        FormatOptions format_options_;
        RecognizeOptions recognize_options_;
        void * input_dib_;
        void * recog_dib_;
        int tables_num_;
        CCOM_cont * ccom_;
        Handle cpage_;
        Handle lines_ccom_;
        Handle cline_;
        CEDPagePtr ed_page_;
        Bool32 rc_line_;
        Bool32 need_clean_line_;
        const char * recog_name_;
        special_project_t special_project_;
};

typedef RuntimeExceptionImpl<PumaImpl> PumaException;

}

#endif /* PUMAIMPL_H_ */
