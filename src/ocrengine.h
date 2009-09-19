#ifndef OCRENGINE_H
#define OCRENGINE_H

#include <string>
#include "types.h"
#include "puma.h"
#include "document.h"

namespace CIF {

    class OcrEngine
    {
    public:
        enum Language {
            LANG_ENGLISH,
            LANG_RUSSIAN
        };

        OcrEngine();
        ~OcrEngine();

        std::string filename() const {return filename_;}
        Language language() const;
        bool recognize(Document& doc);
        void setFilename(const std::string& fname);
        void setLanguage(Language lang);
    private:
        char * dib_;
        std::string filename_;
        Language language_;
        Puma * puma_;

        void read_file(const std::string& fname);

        Handle storage_;
    };

}

#endif // OCRENGINE_H
