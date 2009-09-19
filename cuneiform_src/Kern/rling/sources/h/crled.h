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

// CRLEd.h: interface for the CRLEd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CRLED_H__994890B3_651D_11D3_A5C0_00C0F6A06129__INCLUDED_)
#define AFX_CRLED_H__994890B3_651D_11D3_A5C0_00C0F6A06129__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "globus.h"
#include "cstr.h"
#include "recdefs.h"
////////////////////////////////////////////////////////////

#pragma pack( push, 1 )

//////////////////////////////////////////////////////////////
// структурки взяты мной из модуля rpstr файл cor_spel.h 7.09.99
// в виду использования их только внутри данного класса, решил просто
// скопировать описание и изменить слегка имя
// хотя имело бы смысл использовать из файла, лежащего в данном модуле
//#include "cor_spell.h"

struct VersRef
{
    uchar   code;
    uchar   prob;
};

struct BitMapRef
{
    uchar code;
    uchar pos;
    uint16_t row;
    uint16_t col;
    uint16_t width;
    uint16_t height;
};

struct SheetDiskDescr
{
    uchar code;
    char quant_fragm;
    uint16_t sheet_numb;
    uint16_t descr_lth;
    uchar byte_flag;
    uint16_t resolution;
    uint16_t  incline;
    char tabl[13];
};

struct FragmDisk
{
    uchar code;
    uchar fragm_numb;
    uint16_t depth;
};

struct FragmDiskDescr
{
    uchar code;
    uint16_t row;
    uint16_t col;
    uint16_t height;
    uint16_t w_width;
    char   type;
    uchar  kegl;
    uchar  font;
    uchar  language;
    uchar  type_underl;
};

#pragma pack ( pop )

///////////////////////////////////////////////////////////////////
#define CRL_ED_BUFFER_SIZE                     32000
#define CRL_ED_RECT_ARRAY_SIZE                 100
#define CRL_ED_VERS_ARRAY_SIZE                 100

class CRLEd
{


public:
	RecVersions GetVersElement(uint32_t i, uint32_t * pNVers);
	Rect16 GetRectElement(uint32_t i);
	void * GetEdPool(void) { return (void *) mpEdBuffer; };
	void * GetEdOutPool(void) { return (void *) mpEdOutBuffer; };
	uint32_t GetEdPoolSize(void) { return mpEdFileEnd - mpEdBuffer; };
	Bool32 ExcludeToVers(int32_t size, char* pStr);
	Bool32 AddWord(CSTR_rast b, CSTR_rast e, puchar pLanguage);
	Bool32 MakeWord(CSTR_rast b, CSTR_rast e,puchar Language);
	void   Init();
	CRLEd();
	virtual ~CRLEd();

protected:
	Bool32 mbReadyToAdd;
	Rect16 mEdRect[CRL_ED_RECT_ARRAY_SIZE];
	RecVersions mEdVers[CRL_ED_VERS_ARRAY_SIZE];
	int32_t miEdNVers;
	Handle mhEdOutBuffer;
	Handle mhEdBuffer;
	puchar mpEdFileBound;
	puchar mpEdFileEnd;
	puchar mpEdOutBuffer;
	puchar mpEdBuffer;
	uchar  mHalfSpaces[3];
	struct SheetDiskDescr    mSdd;
	struct FragmDiskDescr    mFdd;
	struct FragmDisk         mFd;
	struct VersRef           mVr;
	struct BitMapRef         mBmr;
	//struct sheet_disk_descr    mSdd;
	//struct fragm_disk_descr    mFdd;
	//struct fragm_disk          mFd;
	//struct vers_ref            mVr;
	//struct bit_map_ref         mBmr;

private:
	void   Write(puchar pP, uint16_t wSize);

};


#endif // !defined(AFX_CRLED_H__994890B3_651D_11D3_A5C0_00C0F6A06129__INCLUDED_)
