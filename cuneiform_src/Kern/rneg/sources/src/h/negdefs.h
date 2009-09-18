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
/*#include <windows.h>*/
#include "recdefs.h"
#include "ccom.h"

#define MAX_STR 1000

struct Rotating
{
	Rect16 Rc;
	uchar* pmasp;
	int16_t* begx;
	int16_t* movey;
	uchar* flmovey;
	int* hi;
	Rotating()
	{
		pmasp=NULL;
		begx=movey=NULL;
		flmovey=NULL;
		hi=NULL;
	}
	~Rotating()
	{
	 delete[] pmasp;
	 delete[] begx;
	 delete[] movey;
	 delete[] flmovey;
	 delete[] hi;
	}
};

#define FlVert 1
#define FlDown2Up 2

struct Negativ
{
	Rect16* pRc;
	int nRc;
	int len_mas;
	double p;
	double p_str;
	double p_rec;
    int Flags;
	Rotating rot;
	Handle hCCOM[MAX_STR];
	Negativ(Rect16 init);
	~Negativ()
	{
		delete[] pRc;
		for(int i=MAX_STR-1;i>=0;i--)
		{
			if(hCCOM[i])
			CCOM_DeleteContainer(hCCOM[i]);
		}
	}
};

class NegList;

class NegList
{
public:

   Negativ neg;

   NegList *prev;
   NegList *next;

	NegList(Rect16 init);

	~NegList();

	NegList* GetNext(NegList* neg_l)
	{
		if(!neg_l)
			return NULL;
		else
			return neg_l->next;
	}

    NegList* GetPrev(NegList* neg_l)
	{
		if(!neg_l)
			return NULL;
		else
			return neg_l->prev;
	}

	void AddNeg(NegList *add);

};
struct RotImageFlags
{
	Rect16 Rc;
	int16_t* begx;
	int16_t* movey;
	uchar* fl_movey;
	int* hi;
	RotImageFlags()
	{
		begx=NULL;
		movey=NULL;
		fl_movey=NULL;
		hi=NULL;
	}
	~RotImageFlags()
	{
		delete[] begx;
		delete[] movey;
		delete[] fl_movey;
		delete[] hi;
	}
};

struct NegImage
{
	int Wide;
	int Height;
	int bytewide;
	uchar* lp;
	Bool fl_delete;
	uchar* pmasp;
	int size_mas;
	RotImageFlags rotate;
	NegImage();
	~NegImage();
	Bool SetDibPtr(uchar* lpDibData,int wide,int heght,int bwide);
	uchar* GetPmasp(Rect16* pRc);
};
