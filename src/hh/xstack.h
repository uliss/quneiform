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

#ifndef __XSTACK_H
#define __XSTACK_H

#ifndef __XPOOL_H
#  include "xpool.h"
#endif

template <class T>
class XStack : private XPool
{
    public:
        XStack( int32_t max_cnt = 0, int32_t cur_cnt = 0 ):
                XPool(), MaxCnt(0), CurCnt(0) {
            Create(max_cnt, cur_cnt);
        };
        ~XStack(void) {
            Destroy();
        };
        Bool     Create( int32_t max_cnt, int32_t cur_cnt = 0 ); // differs of Resize by no realloc
        Bool     CreateOf( int32_t init_cnt ) {
            return Create( init_cnt, init_cnt );
        }
        void     Destroy();
        Bool     Resize( int32_t new_max_cnt ); // reallocation
        void     SetCurCnt( int32_t new_cnt );  // does not resize!
        Bool     operator !() const {
            return   ( !*(XPool*)this )     ||
                     ( CurCnt > MaxCnt )    ||
                     ( GetVolume() != (int32_t)(MaxCnt*sizeof(T)) );
        };
        T&       operator  []( int32_t i ) const {
            assert(GetData() != NULL);
            assert(i < CurCnt);
            return *((T*)(GetData()) + i);
        };
        T*       GetPtr()  const {
            return (T*)(GetData());
        };
        int32_t    GetCurCnt() const {
            return CurCnt;
        };
        int32_t    GetMaxCnt() const {
            return MaxCnt;
        };
        T*       Push( int32_t* no ); // fix space for new element
        Bool     Push( T* t, int32_t* no );
        Bool     Push( T& t, int32_t* no ) {
            return Push(&t, no);
        };
        T*       Pop();

        T*       Top(); // returns temporary pointer to last element;
        // later element can be relocated (after push())

        Err16    Read( XFile & bf, Bool32 swap_bytes = FALSE );
        Bool     Write( XFile & bf ) const;

        void     MemSet( uchar pattern ) {
            XPool::MemSet(pattern);
        };

    private:
        int32_t   MaxCnt;
        int32_t   CurCnt;
        Bool     Copy( T* dst, T* src ) { //*dst=*src;
            memcpy( dst, src, sizeof(T) );
            return TRUE;
        };

        /** 22.03.97 22:07
          public:          // The obsolete methods: for backward compatibility.
                void     flush  () {Destroy();};
        //      T      * ptr( uint num )        { return &(*this)[num]; }
                T      * firstPtr(void)         { return (T *)GetData();}
                uint32_t   volume( void )         {  return MaxCnt;}
                T      & GetLast(void)          {  return (*this)[(uint)(CurCnt -1)];}
        **/
};

template < class T >
Bool     XStack< T >::Create( int32_t max_cnt, int32_t cur_cnt )
{
    assert(max_cnt >= cur_cnt);

    if ( !XPool::Create( max_cnt*sizeof(T) ) ) {
        CurCnt = 0;
        MaxCnt = 0;
        RETFALSE;
    }

    CurCnt = cur_cnt;
    MaxCnt = max_cnt;
    return TRUE;
}

template < class T >
void     XStack< T >::SetCurCnt( int32_t new_cnt ) // does not resize!
{
    assert( new_cnt <= MaxCnt );
    CurCnt = new_cnt;
}

template < class T >
void     XStack< T >::Destroy()
{
    XPool::Destroy();
    CurCnt = MaxCnt = 0;
}


template < class T >
Bool     XStack< T >::Resize( int32_t new_max_cnt )
{
    if ( !XPool::Realloc( new_max_cnt*sizeof(T) ) )
        RETFALSE;

    MaxCnt = new_max_cnt;
    CurCnt =  MIN( CurCnt, new_max_cnt );
    return TRUE;
}

template < class T >
T*       XStack< T >::Push( int32_t* no ) // fix space for new element
{
    assert( CurCnt <= MaxCnt );

    if ( CurCnt == MaxCnt )
        if (!Resize( MAX( CurCnt*2, 1 ) ))
            RETNULL;

    assert( CurCnt < MaxCnt );
    T* pt = (T*)GetData();

    if (pt == NULL)
        RETNULL;

    pt += (uint)CurCnt;

    if (no)
        *no = CurCnt;

    CurCnt++;
    return pt;
}

template < class T >
Bool     XStack< T >::Push( T* t, int32_t* no )
{
    assert( CurCnt <= MaxCnt );

    if ( CurCnt == MaxCnt )
        if (!Resize( MAX( CurCnt*2, 1 ) ))
            RETFALSE;

    assert( CurCnt < MaxCnt );
    T* pt = (T*)GetData();

    if (pt == NULL)
        RETFALSE;

    pt += (uint)CurCnt;

    if ( ( t != NULL ) && !Copy( pt, t) )
        RETFALSE;

    if (no)
        *no = CurCnt;

    CurCnt++;
    return TRUE;
}

template < class T >
T*       XStack< T >::Pop(void)
{
    if (CurCnt == 0) RETNULL;

    T* pt = (T*)GetData();
    assert(pt != NULL);
    CurCnt--;
    pt += (uint)CurCnt;
    return pt;
}

template < class T >
T*       XStack< T >::Top(void)
{
    if (CurCnt == 0)
        return NULL;

    T* pt = (T*)GetData();
    assert(pt != NULL);
    pt += (uint)(CurCnt - 1);
    return pt;
}

template < class T >
Err16 XStack< T >::Read( XFile & bf, Bool32 swap_bytes )
{
    RETIFERR( XPool::Read(bf, swap_bytes) );

    if ( (GetVolume() % sizeof(T)) != 0)
        RETERR( ER_CORRUPTED );

    CurCnt = MaxCnt = (GetVolume() / sizeof(T));
    assert(!!*this);
    return ER_NONE;
}

template < class T >
Bool     XStack< T >::Write( XFile & bf ) const
{
    if (!XPool::Write(bf, CurCnt*sizeof(T)))   RETFALSE;

    return TRUE;
}

#endif // __TAGSTACK_H
