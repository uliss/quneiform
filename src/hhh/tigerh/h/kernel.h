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

//	TIGER run options
#define RUN_SPELLER          1
#define FORCE_ONE_COLUMN     2
#define NO_FORMATING	     4
#define FAX100               8
#define DOTMATRIX           16
#define BCRFLAG             32
#define BCRLOUT             64
#define CUTLEFT            128
#define CUTRIGHT           256
#define SPACE_CORRECTION   512

//	Kernel functions
Bool kernel_init(void);
Bool bcr_init(void);
void kernel_run_options(uint16_t options);
Bool recognize(puchar ed_file_name);
Bool page_analysis(void);
Bool final_recognition(puchar ed_file_name);
Bool spelling_checker(puchar ed_file_name);
Bool kernel_reset(void);
Bool kernel_reload_vocs(void);
Bool kernel_set_temp_dir(uchar *tmp_dir);
Bool kernel_set_windows_dir(uchar *tmp_dir);
Bool kernel_valid_version(int16_t, int16_t);
Bool kernel_set_enable_table_recog(uchar table_recog);
Bool kernel_set_user_dict(uchar *user_dict);

//  Error status functions
pchar get_message(uint16_t code);
uint16_t get_error_status(void);
Bool bool_status(void);
void set_error_status(uint16_t group, uint16_t element);
void clear_error_status(void);

//	Convetrt to text
Bool convert_to_text(puchar edfile, puchar textfile, uint16_t options, uint16_t codepage,
		uchar badchar);
#define SAVE_TEXT_ED            0x01
#define SAVE_TEXT_ASCII         0x02
#define SAVE_TEXT_SMART_ASCII   0x04
#define SAVE_TEXT_RTF           0x08
#define SAVE_TEXT_ANSI          0x10 // Pit 8.26.94
#define SAVE_TEXT_SMART_ANSI    0x20 // Pit
#define SAVE_TEXT_APPEND            128  //  0x'80'
#define SAVE_TEXT_ASCII_APP         (SAVE_TEXT_ASCII + SAVE_TEXT_APPEND)
#define SAVE_TEXT_SMART_ASCII_APP   (SAVE_TEXT_SMART_ASCII + SAVE_TEXT_APPEND)
#define SAVE_TEXT_RTF_APP           (SAVE_TEXT_RTF + SAVE_TEXT_APPEND)

#define SAVE_TABLE_TXT		0x0100
#define SAVE_TABLE_CSV		0x0200
#define SAVE_TABLE_DBF		0x0400
#define SAVE_TABLE_ODBC 	0x0800
#define SAVE_TABLE_WKS		0x1000
#define SAVE_HTML               0x2000
#define SAVE_HOCR               0x4000

// wCodePage options... Piter
#define SAVE_TEXT               SAVE_TEXT_ASCII
#define SAVE_TEXT_SMART         SAVE_TEXT_SMART_ASCII

# define TIGER_CODE_UNKNOWN     0x0000
# define TIGER_CODE_ASCII       0x0001
# define TIGER_CODE_ANSI        0x0002
# define TIGER_CODE_KOI8        0x0004
# define TIGER_CODE_ISO         0x0008

void write_text(uchar);

//-------------------- Callback functions -------------------------

//	Image read functions

struct AttrImage {
	uint16_t PixelHeight;
	uint16_t PixelWidth;
	uint16_t ByteWidth;
	uint16_t LeftDisplacement;
	uint16_t Resolution_X;
	uint16_t Resolution_Y;
	uchar FotoMetrics;
	uchar bUnused;
	uint16_t AddX;
	uint16_t AddY;
};

Bool Tiger_ImageOpen(struct AttrImage * p);
Bool Tiger_ImageOpen_xy(struct AttrImage * p, uint16_t x, uint16_t y, uint16_t w, uint16_t h);
Bool Tiger_ImageClose(void);
int16_t Tiger_ImageRead(puchar buffer, uint16_t lth);
uint32_t Tiger_ProcessPictures(puchar buffer, uint32_t lth);

//	Immediate error report
void Tiger_ReportError(uint16_t status, puchar message);

// Progress Monitor
Bool Tiger_ProgressStart(void);
Bool Tiger_ProgressFinish(void);
Bool Tiger_ProgressSetPercentage(uint16_t wPercent);
Bool Tiger_ProgressSetStage(uint16_t wStageNumber, puchar pStageName, uint16_t wVolume);
