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

#include <memory.h>

#include "block.h"
#include "convert.h"
#include "cpage_debug.h"

namespace cf {
namespace cpage {

Block::Block() :
    user_num_(0),
    flags_(0),
    internal_num_(0)
{}

Block::Block(const Block& b) :
    Data(b),
    user_num_(b.user_num_),
    flags_(b.flags_),
    internal_num_(b.internal_num_)
{}

Block::~Block()
{}

void Block::set(Handle type, uint32_t userNum , uint32_t flags , const void * src , uint32_t size)
{
    user_num_ = userNum;
    flags_ = flags;
    internal_num_ = 0;
    setData(type, src, size);
}

Block& Block::operator=(const Block& block)
{
    user_num_ = block.user_num_;
    flags_ = block.flags_;
    internal_num_ = block.internal_num_;
    *(Data *)this = block;
    return *this;
}

bool Block::operator==(const Block& block)
{
    if (user_num_ == block.user_num_ &&
            flags_ == block.flags_ &&
            internal_num_ == block.internal_num_ &&
            *(Data*)this == block)
        return true;

    return false;
}

bool Block::operator!=(const Block& block)
{
    return ! this->operator==(block);
}

bool Block::save(std::ostream& os) const
{
    if(os.bad()) {
        CPAGE_ERROR_FUNC << ": bad stream";
        return false;
    }

    os.write((char*) &user_num_, sizeof(user_num_));
    os.write((char*) &flags_, sizeof(flags_));
    os.write((char*) &internal_num_, sizeof(internal_num_));

    if(os.fail()) {
        CPAGE_ERROR_FUNC << ": failed";
        return false;
    }

    return Data::save(os);
}

bool Block::restore(std::istream& is)
{
    if(is.bad()) {
        CPAGE_ERROR_FUNC << ": bad stream";
        return false;
    }

    is.read((char*) &user_num_, sizeof(user_num_));
    is.read((char*) &flags_, sizeof(flags_));
    is.read((char*) &internal_num_, sizeof(internal_num_));

    if(is.fail()) {
        CPAGE_ERROR_FUNC << ": failed";
        return false;
    }

    return Data::restore(is);
}

namespace {
DataConvertor blockConvertor(DefConvertBlock);
}

DataConvertor Block::setConvertor(const DataConvertor& convertor)
{
    DataConvertor old = blockConvertor;
    blockConvertor = convertor;
    return old;
}

uint32_t Block::Convert(Handle type, void * lpdata, uint32_t size)
{
    return blockConvertor(type_, data_, size_, type, lpdata, size);
}

}
}
