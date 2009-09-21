#include "ocrengine.h"
#include "puma/puma.h"
#include "puma/mpuma.h"

#include "paragraph.h"

#include <iostream>
#include <fstream>
#include <cstring>

#include <Magick++.h>

using namespace std;

namespace CIF {

    OcrEngine::OcrEngine() : dib_(NULL), filename_("test.bmp"), language_(LANG_ENGLISH), puma_(NULL)
    {
        puma_ = new Puma;
    }

    OcrEngine::~OcrEngine()
    {
        if(dib_)
            delete []dib_;
        delete puma_;
    }

    OcrEngine::Language OcrEngine::language() const
    {
        return language_;
    }

    void OcrEngine::read_file(const string& fname)
    {
        using namespace Magick;

        Blob blob;
        size_t data_size;
        try {
            Magick::Image image(fname);
            // Write to BLOB in BMP format
            image.write(&blob, "DIB");
        } catch(Exception &error_) {
            cerr << error_.what() << endl;
            throw std::runtime_error("Error opening file: " + fname);
        }

        data_size = blob.length();
        dib_ = new char[data_size];
        memcpy(dib_, blob.data(), data_size);
    }

    bool OcrEngine::recognize(Document& doc)
    {
        read_file(filename_);

        int lang = LANG_ENGLISH;

        Bool32 dotmatrix = FALSE;
        Bool32 fax = FALSE;
        Bool32 onecolumn = FALSE;
        PUMA_SetImportData(PUMA_Word32_Language, &lang);
        PUMA_SetImportData(PUMA_Bool32_DotMatrix, &dotmatrix);
        PUMA_SetImportData(PUMA_Bool32_Fax100, &fax);
        PUMA_SetImportData(PUMA_Bool32_OneColumn, &onecolumn);

        puma_->open(dib_, filename_);

        if(!PUMA_XFinalRecognition()) {
            cerr << "PUMA_XFinalrecognition failed." << endl;
            return false;
        }

        if(!PUMA_XSave("test.txt", PUMA_TOTEXT, PUMA_CODE_UTF8)) {
            cerr << "PUMA_XSave failed." << endl;
            return false;
        }

        ifstream f("test.txt");
        ostringstream txt;
        txt << f.rdbuf();

        Page * p = new Page;
        doc.push(make_element(p));

        Paragraph * par = new Paragraph(txt.str());
        p->push(make_element(par));

        return true;
    }

    void OcrEngine::setFilename(const std::string& fname)
    {
        filename_ = fname;
    }

    void OcrEngine::setLanguage(Language lang)
    {
        PUMA_SetImportData(PUMA_Word32_Language, &language_);
    }
}
