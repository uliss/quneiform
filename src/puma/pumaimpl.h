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

#include "cfcompat.h"
#include "rfrmt/formatoptions.h"
#include "specprj.h"
#include "common/rect.h"
#include "common/exception.h"
#include "common/memorybuffer.h"
#include "common/outputformat.h"
#include "common/image.h"
#include "common/recognizeoptions.h"
#include "cimage/imageinfo.h"
#include "export/exporter.h"
#include "globus.h"

class CTIControl;
struct CCOM_cont;

namespace CIF {

class RMarker;
class ComponentExtractor;
class CEDPage;
class Formatter;
class CRtfPage;

bool IsUpdate(uint32_t flg);
void SetUpdate(uint32_t flgAdd, uint32_t flgRemove);

class CLA_EXPO PumaImpl
{
    public:
        PumaImpl();
        ~PumaImpl();

        /**
         * Append recognition results to specified file
         * @note not all formats support this type of saving
         * @throw PumaException on error
         */
        void append(const std::string& filename, format_t format) const;

        /**
         * Clears recognized output formatting
         */
        void clearFormat();

        /**
         * Make some cleanup after recognition
         */
        void close();

        /**
         * Returns format options
         */
        FormatOptions formatOptions() const;

        /**
         * Returns pointer to formatted page
         * @return NULL if not formatted
         */
        CRtfPage * formatPage();

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
         * Saves to file recognition result in specified format
         * @param filename - file to save
         * @param format - document format
         */
        void save(const std::string& filename, format_t format) const;

        /**
         * Saves to stream recognition result in specified format
         * @note not all export abilities are available in this mode
         * (such as saving pictures) it depends from format
         */
        void save(std::ostream& os, format_t format) const;

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

        void setImageOutputDir(const std::string& path);

        /**
         * Sets working region on input image
         */
        void setPageTemplate(const Rect& r);
        void setSpecialProject(special_project_t SpecialProject);
    public:
        static unsigned char * mainBuffer();
        static unsigned char * workBuffer();
        static const size_t MainBufferSize = 500000;
        static const size_t WorkBufferSize = 180000;
    private:
        void binarizeImage();
        void clearAll();
        void extractComponents();
        void extractStrings();
        void getImageInfo(const std::string& image_name);
        void layout();
        void loadLayoutFromFile(const std::string& fname);
        ExporterPtr makeExporter(format_t format) const;
        void modulesDone();
        void modulesInit();
        const char * modulePath() const;
        const char * moduleTmpPath() const;
        void normalize();
        void pass1();
        void pass2();
        void pass2special();
        void preOpenInitialize();
        void preprocessImage();
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
        static FixedBuffer<unsigned char, MainBufferSize> main_buffer_;
        static FixedBuffer<unsigned char, WorkBufferSize> work_buffer_;
    private:
        std::auto_ptr<CTIControl> cimage_;
        std::auto_ptr<RMarker> rmarker_;
        std::auto_ptr<ComponentExtractor> comp_extractor_;
        std::auto_ptr<Formatter> formatter_;
        BitmapInfoHeader info_;
        Rect rect_template_;
        std::string input_filename_;
        std::string layout_filename_;
        std::string output_image_dir_;
        FormatOptions format_options_;
        RecognizeOptions recognize_options_;
        void * input_dib_;
        void * recog_dib_;
        int tables_num_;
        CCOM_cont * ccom_;
        Handle cpage_;
        Handle lines_ccom_;
        Handle cline_;
        CEDPage * ed_page_;
        Bool32 rc_line_;
        Bool32 kill_vsl_components_;
        Bool32 need_clean_line_;
        const char * recog_name_;
        special_project_t special_project_;
};

typedef RuntimeExceptionImpl<PumaImpl> PumaException;

}

#endif /* PUMAIMPL_H_ */
