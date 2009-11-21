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

#include <string.h>
#include "negdefs.h"

Negativ::Negativ(Rect16 init)
{
		nRc=1;
        Flags = 0;
		len_mas=5;
		pRc=NULL;
		pRc=new Rect16[len_mas];
		pRc[0].bottom=init.bottom;
		pRc[0].top=init.top;
		pRc[0].left=init.left;
		pRc[0].right=init.right;
		p=.0;
		p_str=.0;
		p_rec=.0;
		for(int i=MAX_STR-1;i>=0;i--)
			hCCOM[i]=0;
}



NegList::NegList(Rect16 init) : neg(init)
{
	prev=next=NULL;
}

NegList::~NegList()
{
	if(!this)
		return;
	if(this->prev)
		this->prev->next=this->next;
	if(this->next)
		 this->next->prev=this->prev;
}

void NegList::AddNeg(NegList* add)
{
	if(!add)
		return;
	if(this->prev)
		this->prev->next=add;
	add->prev=this->prev;
	this->prev=add;
	add->next=this;
}




NegImage::NegImage()
{
	Wide=Height=bytewide=size_mas=0;
	lp=pmasp=NULL;
	fl_delete=FALSE;
}

NegImage::~NegImage()
{
	if(pmasp)
		delete[] pmasp;
	if(fl_delete)
		delete[] lp;
}

Bool NegImage::SetDibPtr(uchar* lpDibData,int wide,int height,int bwide)
{
	if(lp!=NULL||lpDibData==NULL)
		return FALSE;
	if(wide<0||height<0||(wide>>3)>bwide)
		return FALSE;

	lp=lpDibData;
	Wide=wide;
	Height=height;
	bytewide=bwide;
	return TRUE;
}

uchar* NegImage::GetPmasp(Rect16* pRc)
{
	int left=pRc->left;
	int right=pRc->right;
	int top=pRc->top;
	int bottom=pRc->bottom;
	int w=right-left+1;
	const int bwide=(w+7)>>3;
	int h=bottom-top+1;
	const int cbytewide=bytewide;
	const Bool fl_danger=!(cbytewide-(right>>3));

	if(left<0||top<0||right>=Wide||bottom>=Height)
		return NULL;

	if(!size_mas)
	{
		size_mas=(bytewide+1)*Height;
		delete[] pmasp;
		pmasp = new uchar[size_mas];
		if(!pmasp)
		{
			size_mas=0;
			return NULL;
		}
	}

	register uchar* to=pmasp;
	register uchar* from=lp+(Height-top-1)*cbytewide+(left>>3);
	register uchar* now;
	register int i;
	register int j;
	switch(left&7)
	{
	case 0:
		for(i=h;i>0;i--,from-=cbytewide,to+=bwide)
			memcpy(to,from,bwide);
		break;
	case 1:
		for(i=h;i>0;i--,from-=cbytewide)
		{
			for(j=bwide-1,now=from;j>0;j--,to++)
			{
				(*to)=(*now)<<1;
				now++;
				(*to)|=((*now)>>7);
			}
			(*to)=(*now)<<1;
			now++;
			if(fl_danger)
				(*to)|=1;
			else
				(*to)|=((*now)>>7);
			to++;
		}
		break;
	case 2:
		for(i=h;i>0;i--,from-=cbytewide)
		{
			for(j=bwide-1,now=from;j>0;j--,to++)
			{
				(*to)=(*now)<<2;
				now++;
				(*to)|=((*now)>>6);
			}
			(*to)=(*now)<<2;
			now++;
			if(fl_danger)
				(*to)|=3;
			else
				(*to)|=((*now)>>6);
			to++;
		}
		break;
	case 3:
		for(i=h;i>0;i--,from-=cbytewide)
		{
			for(j=bwide-1,now=from;j>0;j--,to++)
			{
				(*to)=(*now)<<3;
				now++;
				(*to)|=((*now)>>5);
			}
			(*to)=(*now)<<3;
			now++;
			if(fl_danger)
				(*to)|=7;
			else
				(*to)|=((*now)>>5);
			to++;
		}
		break;
	case 4:
		for(i=h;i>0;i--,from-=cbytewide)
		{
			for(j=bwide-1,now=from;j>0;j--,to++)
			{
				(*to)=(*now)<<4;
				now++;
				(*to)|=((*now)>>4);
			}
			(*to)=(*now)<<4;
			now++;
			if(fl_danger)
				(*to)|=15;
			else
				(*to)|=((*now)>>4);
			to++;
		}
		break;
	case 5:
		for(i=h;i>0;i--,from-=cbytewide)
		{
			for(j=bwide-1,now=from;j>0;j--,to++)
			{
				(*to)=(*now)<<5;
				now++;
				(*to)|=((*now)>>3);
			}
			(*to)=(*now)<<5;
			now++;
			if(fl_danger)
				(*to)|=31;
			else
				(*to)|=((*now)>>3);
			to++;
		}
		break;
	case 6:
		for(i=h;i>0;i--,from-=cbytewide)
		{
			for(j=bwide-1,now=from;j>0;j--,to++)
			{
				(*to)=(*now)<<6;
				now++;
				(*to)|=((*now)>>2);
			}
			(*to)=(*now)<<6;
			now++;
			if(fl_danger)
				(*to)|=63;
			else
				(*to)|=((*now)>>2);
			to++;
		}
		break;
	case 7:
		for(i=h;i>0;i--,from-=cbytewide)
		{
			for(j=bwide-1,now=from;j>0;j--,to++)
			{
				(*to)=(*now)<<7;
				now++;
				(*to)|=((*now)>>1);
			}
			(*to)=(*now)<<7;
			now++;
			if(fl_danger)
				(*to)|=127;
			else
				(*to)|=((*now)>>1);
			to++;
		}
		break;
	default:break;
    }

	return pmasp;
}
