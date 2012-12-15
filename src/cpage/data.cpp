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

#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <cassert>

#include "cpage.h"
#include "data.h"
#include "mymem.h"
#include "backup.h"
#include "common/log.h"

namespace cf {
namespace cpage {

Data::Data() :
    size_(0),
    data_(NULL)
{
    type_ = reinterpret_cast<void*> (-1);
}

Data::Data(const Data& data) :
    size_(data.size_),
    type_(data.type_),
    data_(NULL)
{
    setData(data.type_, data.data_, data.size_);
}

Data::~Data()
{
    delete []data_;
}

void Data::setData(Handle type, const void * src, uint32_t size)
{
    type_ = type;
    size_ = size;

    if (data_) {
        delete [] data_;
        data_ = NULL;
    }

    if (src && size) {
        data_ = new char[size];
        memcpy(data_, src, size);
    }
}

bool Data::empty() const
{
    return !size_ || !data_;
}

bool Data::getDataPtr(Handle type, void **lpdata)
{
    assert(lpdata);

    if (type == type_) {
        *lpdata = data_;
        return true;
    }

    return false;
}

Handle Data::type() const
{
    return type_;
}

void Data::setType(Handle type)
{
    type_ = type;
}

uint32_t Data::getData(Handle type, void * dest, uint32_t size)
{
    if (type == type_) {
        if (dest == NULL) {
            cfError(MODULE_CPAGE) << "NULL destination pointer given";
            return 0;
        }

        if (size_ && data_)
            memcpy(dest, data_, size_);
    }
    else
        return Convert(type, dest, size);

    return size_;
}

Data& Data::operator=(const Data& data)
{
    setData(data.type_, data.data_, data.size_);
    return *this;
}

bool Data::operator==(const Data& data)
{
    if (type_ == data.type_ && size_ == data.size_) {
        if (data_ == data.data_ && data_ == NULL)
            return true;

        return memcmp(data_, data.data_, size_) == 0;
    }

    return false;
}

bool Data::save(Handle to) const
{
    const char * name = CPAGE_GetNameInternalType(type_);
    if(!name) {
        cfError(MODULE_CPAGE) << "NULL internal name";
        return false;
    }

    const uint32_t len = strlen(name) + 1;
    FILE * f = static_cast<FILE*>(to);

    if (fwrite(&len, 1, sizeof(len), f) == sizeof(len) &&
            fwrite(name, 1, len, f) == len &&
            fwrite(&size_, 1, sizeof(size_), f) == sizeof(size_) &&
            (size_ == 0 ||  fwrite(data_, 1, size_, f) == size_))
        return true;

    return false;
}

bool Data::restore(Handle from)
{
    uint32_t len = 0;
    char name[260] = { 0 };

    if (myRead(from, &len, sizeof(len)) != sizeof(len))
        return false;

    if(len >= sizeof(name)) {
        cfError(MODULE_CPAGE) << "data name is too long:" << len;
        return false;
    }

    if(myRead(from, name, len) != len)
        return false;

    type_ = CPAGE_GetInternalType(name);

    if (myRead(from, &size_, sizeof(size_)) != sizeof(size_))
        return false;

    if (!size_)
        return true;

    if (data_) {
        delete [] data_;
        data_ = NULL;
    }

    data_ = new char[size_];
    return myRead(from, data_, size_) == size_;
}

bool Data::saveCompress(Handle to) const
{
    if (size_ == 0)
        return save(to);

    char *compressedData, *lpDataSave = data_;
    uint32_t compressedSize, SizeSave = size_;
    CleanData(type_, data_, size_);

    if (!Compress(data_, size_, &compressedData, &compressedSize))
        return false;

    data_ = compressedData;
    size_ = compressedSize;
    bool rv = save(to);
    data_ = lpDataSave;
    size_ = SizeSave;
    delete []compressedData;
    return rv;
}

bool Data::restoreCompress(Handle from)
{
    if (!restore(from))
        return false;

    if (size_ == 0)
        return true;

    char *decomData;
    uint32_t decomSize;

    if (!Decompress(data_, size_, &decomData, &decomSize))
        return false;

    if (data_)
        delete [] data_;

    data_ = decomData;
    size_ = decomSize;

    if (!ComplianceVersions(type_, &data_, &size_))
        return false;

    return true;
}
}

}
