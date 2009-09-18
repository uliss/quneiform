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

#ifndef __FARARRAY_H
#define __FARARRAY_H

#ifndef __LNSLANG_H
#  include "lnslang.h"
#endif
template<class Type> class TArray //: public TFarHeap
{
protected:
    Type * data;
    long int last; // == len - 1
public:
    Type & operator[](int32_t i) {
        assert( i <= last );
        return (*(data + i));
    }

    Type * ptr(int32_t num) {
        assert( num <= last );
        return (data + num);
    }

    TArray(void) {
        data = NULL;
    }

    TArray(int32_t of_count) {
        create(of_count);
    }

    ~TArray(void) {
        flash();
    }

    Type * lastPtr(void) {
        assert(!isEmpty());
        return (data + last);
    }

    Type * firstPtr(void) {
        //?assert(!isEmpty());
        return (data);
    }

    Bool create(int32_t of_count) {
        if (of_count != 0)
            data = (Type *) malloc(of_count * sizeof(Type));
        else
            data = (Type *) malloc(sizeof(Type));
        if (data != NULL)
            last = (of_count - 1);
        return (data != NULL);
    }

    Bool Grow(int32_t of_count = -1) {
        if (of_count == -1) {
            of_count = (last + 1) * 2;
        }

        if (of_count <= 0)
            of_count = 1;

        Type * new_data = (Type *) (malloc((int32_t) (of_count
                * (int32_t) sizeof(Type))));
        if (new_data != NULL) {
            if (data != NULL) {
                memcpy(new_data, data, (last + 1) * sizeof(Type));
                free(data);
                data = new_data;
                new_data = NULL;
                last = (of_count - 1);
            }
        }
        return (data != NULL);
    }

    void flash(void) {
        if (data != NULL)
            free((void*) data);
        data = NULL;
    }

    void flush(void) {
        flash();
    }

    int volume(void) {
        assert(data!=NULL);
        return (last + 1);
    }

    int lastHandle(void) {
        assert(data!=NULL);
        return (last);
    }

    Bool isEmpty(void) {
        return (data == NULL);
    }

    Bool isOk(void) {
        return (data != NULL);
    }

    void fill(Type * value, int32_t from = 0);

    Bool shrink(int32_t el_count) {
        return TRUE;
        /*
         assert(el_count < volume() );
         if (el_count > 0)
         {  void* new_data = data; TFarHeap::shrinkMemBlock( data, sizeof(Type)*(int32_t)el_count );
         if (new_data == NULL)
         return WRONG();  // can't shrink ??
         else {
         data = (Type *)new_data;
         last = el_count -1;
         };
         return TRUE;
         }
         else
         {  void* new_data = TFarHeap::shrinkMemBlock( data, (int32_t)sizeof(Type) );
         if (new_data == NULL)
         return WRONG();  // can't shrink ??
         else {
         data = (Type *)new_data;
         last = -1;
         };
         return TRUE;
         };
         */
    }

    void copy(TArray<Type> * arr) {
        assert( arr != NULL );
        assert( arr->isOk() );
        assert( isOk() );
        assert( arr->volume() <= volume() );
        MEMMOVE(data, arr->data, (arr->volume()) * (int32_t) (sizeof(Type)));
    }

    void copy(TArray<Type> * arr, int32_t count) {
        assert( arr != NULL );
        assert( arr->isOk() );
        assert( isOk() );
        assert( count <= arr->volume() );
        assert( count <= volume() );
        MEMMOVE(data, arr->data, count * (int32_t) (sizeof(Type)));
    }
};

template<class Type>
void TArray<Type>::fill(Type * value, int32_t from) {
    if(from == 0 && last < 0) // Cleaning function calls this even for a zero-sized array.
        data[0] = *value;
    else {
        assert(from <= last);
        for (from = from; from <= last; from++)
            data[from] = (*value);
    }
}

#endif // __FARARRAY_H
