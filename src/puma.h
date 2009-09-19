#ifndef PUMA_H
#define PUMA_H

#include <stdexcept>
#include "cttypes.h"
#include "types.h"

//    DEC_FUN(uint32_t, PUMA_GetReturnCode,());
//    DEC_FUN(char *, PUMA_GetReturnString,(uint32_t dwError));
//    DEC_FUN(Bool32, PUMA_GetExportData,(uint32_t dwType, void * pData));
//    DEC_FUN(Bool32, PUMA_SetImportData,(uint32_t dwType, void * pData));

//    DEC_FUN(Bool32, PUMA_XPageAnalysis, (void));
//    DEC_FUN(bool,   PUMA_XFinalRecognition, (void));
//    DEC_FUN(Bool32, PUMA_XSave,(const char * lpOutFileName, int32_t lnFormat, int32_t lnCode ));
//    DEC_FUN(int32_t,  PUMA_EnumLanguages,(int32_t nPrev ));
//    DEC_FUN(int32_t,  PUMA_EnumFormats,(int32_t nPrev ));
//    DEC_FUN(int32_t,  PUMA_EnumCodes,(int32_t format, int32_t nPrev ));
//    DEC_FUN(int32_t,  PUMA_EnumFormatMode,(int32_t nPrev ));
//    DEC_FUN(int32_t,  PUMA_EnumTable,(int32_t nPrev ));
//    DEC_FUN(int32_t,  PUMA_EnumPicture,(int32_t nPrev ));
//    DEC_FUN(Bool32, PUMA_XGetRotateDIB,(void ** lpDIB, Point32 * p));
//    DEC_FUN(void ,  PUMA_RenameImageName,(char * name));
//    DEC_FUN(Bool32, PUMA_XSetTemplate,(Rect32 rect));
//    DEC_FUN(Bool32, PUMA_XGetTemplate,(Rect32 *pRect));
//    DEC_FUN(Bool32, PUMA_Save,(Handle hEdPage, const char * lpOutFileName, int32_t lnFormat, int32_t lnCode, Bool32 bAppend ));
//    DEC_FUN(Bool32, PUMA_XOpenClbk,(PUMAIMAGECALLBACK CallBack,const char * lpFileName));
//    DEC_FUN(uint32_t, PUMA_SaveToMemory,(Handle hEdPage, int32_t lnFormat, int32_t lnCode, char * lpMem, uint32_t size ));
//    DEC_FUN(void ,  PUMA_GetSpecialBuffer,(char * szResult,int32_t *nResultLength));
//    DEC_FUN(Bool32, PUMA_SetSpecialProject,(uchar nSpecPrj));

namespace CIF
{
    class PumaException : public std::runtime_error
    {
    public:
        PumaException(const std::string& msg) : std::runtime_error(msg) {}
    };

    class Puma
    {
    public:
        Puma();
        ~Puma();

        void close();
        void open(char * dib, const std::string& filename);
    private:
        Handle page_;
        Handle ccom_;
        Handle ed_page_;
        char * input_dib_;
        char * recog_dib_;
        uchar * buffer_;
        uchar * work_mem_;
        char * dib_;

        std::string input_filename_;
        std::string layout_filename_;

        void modules_done();
        void modules_init();

        static std::string module_tmp_path();
        static std::string module_tmp_path_;
        static std::string module_path_;
        static std::string module_path();
    };


}

#endif // PUMA_H
