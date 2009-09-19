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
#include "searchneg.h"



void DelIn(Handle hCPage,NegList** proot,int& nRc)
{
 NegList* root=*proot;
 NegList* nowin=root;
 NegList* nowout=root;
 NegList* temp;
 Bool fl_cont;
 Bool vert;
 uchar Data[1000];
 memset (Data, 0, sizeof (Data));
 uchar* pmasp;
 Rect16 Rc;
 double blackout;
 double blackin;
 while(nowout)
 {
	 if((nowout->neg).pRc[0].bottom-(nowout->neg).pRc[(nowout->neg).nRc-1].top>(nowout->neg).pRc[0].right-(nowout->neg).pRc[0].left)
		 vert=TRUE;
	 else
		 vert=FALSE;
	 while(nowin && nowout)
	 {
		 fl_cont=FALSE;
		 if(nowin==nowout)
		 {
			 fl_cont=TRUE;
			 nowin=nowin->next;
		 }
		 if(fl_cont)
			 continue;
		 if(vert)
			 ;
		 else
		 {
		  if(!(nowin->neg.pRc[(nowin->neg).nRc-1].top>=nowout->neg.pRc[0].bottom||nowout->neg.pRc[(nowout->neg).nRc-1].top>=nowin->neg.pRc[0].bottom))
          {
            if( ((nowout->neg).pRc[0].left<(nowin->neg).pRc[0].left)&&((nowout->neg).pRc[0].right>(nowin->neg).pRc[0].left))
			{
              Rc.bottom=(nowout->neg).pRc[0].bottom;
			  Rc.top=(nowout->neg).pRc[(nowout->neg).nRc-1].top;
			  Rc.left=(nowout->neg).pRc[0].left;
			  Rc.right=(nowin->neg).pRc[0].left;
			  pmasp=Data;
			  if(!GetMasP(&Rc,&pmasp))
				  blackout=.0;
			  else
			      blackout=NegBlack(pmasp,Rc);
              Rc.bottom=(nowin->neg).pRc[0].bottom;
			  Rc.top=(nowin->neg).pRc[(nowin->neg).nRc-1].top;
			  Rc.left=(nowout->neg).pRc[0].right;
			  Rc.right=(nowin->neg).pRc[0].right;
			  if(Rc.right<=Rc.left)
                  blackin=.0;
			  else
			  {
               pmasp=Data;
			   if(!GetMasP(&Rc,&pmasp))
				   blackin=.0;
               else
			       blackin=NegBlack(pmasp,Rc);
			  }
			  if(blackout>=blackin)
			  {
				  nRc--;
				  temp=nowin;
				  if(root==temp)
					  root=temp->next;
				  nowin=nowin->next;
				  delete temp;
				  fl_cont=TRUE;
			  }
			  else
			  {
				  nRc--;
				  temp=nowout;
				  if(root==temp)
					  root=temp->next;
				  nowout=nowout->next;
				  delete temp;
				  nowin=root;
				  fl_cont=TRUE;
			  }
			}
		  }
		 }
		 if(fl_cont)
			  ;
		 else
		     nowin=nowin->next;
	 }
	 if (nowout)
     {
        nowout=nowout->next;
	    nowin=root;
     }
 }
 (*proot)=root;
}

double NegBlack(uchar* pmasp,Rect16 Rc)
{
 int w=Rc.right-Rc.left+1;
 const int bytewide=(w+7)/8;
 int h=Rc.bottom-Rc.top+1;
 register int i=7-(bytewide*8-w);
 int k;
 int rect=w&7;
 if(!rect)
	 rect=8;
 register uchar bytep;
 register int count_black=w*h;
 register uchar* p=pmasp;
 register uchar* pend=p+h*bytewide;
 i=1;
 for(;p<pend;p++)
 {
   bytep=(*p);
	  if(i==bytewide)
	  {
       for(k=0;k<rect;k++)
          count_black-=((bytep&(128>>k))>>(7-k));
	   i=0;
	  }
	  else
	  {
	  count_black-=((bytep>>7)+((bytep&64)>>6)+((bytep&32)>>5)+((bytep&16)>>4)+((bytep&8)>>3)+((bytep&4)>>2)+((bytep&2)>>1)+(bytep&1));
	  }
   p++;
   i++;
 }
 return (double)(count_black)/(double)(w*h);
}

void CutNegStr(Handle hCPage,NegList** proot,int& nRc,int skew)
{
 if(nRc<=0)
	 return;

 int h,w;
 uchar* pmasp;
 NegList* now=(*proot);
 NegList* temp;
 uchar Data[1000];
 Bool fl_cont=FALSE;
 Rect16 Rc;
 memset (Data, 0, sizeof (Data));
 while(now->next)
	 now=now->next;
 while(now)
 {
  fl_cont=FALSE;

  w=(now->neg).pRc[0].right-(now->neg).pRc[0].left+1;
  h=(now->neg).pRc[0].bottom-(now->neg).pRc[0].top+1;
  if( (h<=inf_neg_h*2) || (w<=inf_neg_w ) || (h>w) )
  {
	  now=now->prev;
	  fl_cont=TRUE;
  }
  if(fl_cont)
	  continue;

  if((now->neg).rot.pmasp)
    pmasp=(now->neg).rot.pmasp;
  else
  {
	pmasp = Data;
   if(!(GetMasP((now->neg).pRc,&pmasp)) )
   {
	  now=now->prev;
	  fl_cont=TRUE;
   }
  }
  if(fl_cont)
	  continue;

  NegInvert(pmasp,(now->neg).pRc[0]);
  Rc.top=now->neg.pRc->top;
  Rc.bottom=now->neg.pRc->bottom;
  RSELSTR_CutStr(&((now->neg).pRc),(now->neg).nRc,(now->neg).len_mas,pmasp,DPIX,DPIY,30);

  if(!(now->neg).nRc)
  {
	  if(!now->prev)
		  (*proot)=now->next;
	  temp=now;
	  now=now->prev;
	  delete temp;
	  nRc--;
  }
  else
  {
	 now->neg.pRc[now->neg.nRc-1].top=Rc.top;
	 now->neg.pRc->bottom=Rc.bottom;
     now=now->prev;
  }
 }

}


void CutNeg(Handle hCPage,NegList** proot,int& nRc,int skew)
{
 int k;
 int h,w;
 uchar* pmasp;
 NegList* root=(*proot);
 NegList* now=root;
 uchar Data[1000];
 Bool fl_cont=FALSE;
 int bytewide;
 int rect;
 int top;
 int oldtop;
 register uchar bytep;
 register int count_black;
 double proc;
 Bool neg;
 Rect16 Rc;
 NegList* temp;
 int stop;
 register uchar* p;
 register uchar* pend;
 int neg_top;
 int neg_bottom;
 int neg_left;
 int neg_right;
 const int size_Rect16=sizeof(Rect16);

 memset (Data, 0, sizeof (Data));
 while(now)
 {
  fl_cont=FALSE;


/*  if(IsNegBlack(hCPage,now))
  {
	 if((now->neg).rot.pmasp)
           RSELSTR_UnRotateRect(-skew,&((now->neg).pRc[0]),1,(now->neg).rot.Rc,(now->neg).rot.begx,(now->neg).rot.movey,(now->neg).rot.flmovey,(now->neg).rot.hi);
	 DeleteRotateMas(&((now->neg).rot.pmasp),&((now->neg).rot.begx),&((now->neg).rot.movey),&((now->neg).rot.flmovey),&((now->neg).rot.hi));
     fl_cont=TRUE;
	 now=now->next;
  }
  if(fl_cont)
	  continue;
*/
  w=(now->neg).pRc[0].right-(now->neg).pRc[0].left+1;
  h=(now->neg).pRc[0].bottom-(now->neg).pRc[0].top+1;
  if( (h<=inf_neg_h*3) || (w<=inf_neg_w ) )
  {
	 if((now->neg).rot.pmasp)
           RSELSTR_UnRotateRect(-skew,&((now->neg).pRc[0]),1,(now->neg).rot.Rc,(now->neg).rot.begx,(now->neg).rot.movey,(now->neg).rot.flmovey,(now->neg).rot.hi);
	 DeleteRotateMas(&((now->neg).rot.pmasp),&((now->neg).rot.begx),&((now->neg).rot.movey),&((now->neg).rot.flmovey),&((now->neg).rot.hi));
	  now=now->next;
	  fl_cont=TRUE;
  }
  if(fl_cont)
	  continue;

  if((now->neg).rot.pmasp)
    pmasp=(now->neg).rot.pmasp;

  else
  {
	pmasp = Data;
   if(!(GetMasP(now->neg.pRc,&pmasp)) )
   {
	   now=now->next;
	  fl_cont=TRUE;
   }
  }
  if(fl_cont)
	  continue;

    bytewide=(w+7)/8;
    rect=w&7;
	if(!rect)
		rect=8;
    top=(now->neg).pRc[0].top;
    oldtop=top;
    count_black=0;
    neg=TRUE;
	pend=pmasp+(bytewide-1);
	p=pmasp;
	neg_top=(now->neg).pRc[0].top;
	neg_bottom=(now->neg).pRc[0].bottom;
	neg_left=(now->neg).pRc[0].left;
	neg_right=(now->neg).pRc[0].right;
	stop=neg_bottom-top+1-inf_neg_h;

    while(stop>0)
	{
	 while(root->prev)
		 root=root->prev;

     count_black=w;
     for(;p<pend;p++)
	 {
      bytep=(*p);

	  count_black-=((bytep>>7)+((bytep&64)>>6)+((bytep&32)>>5)+((bytep&16)>>4)+((bytep&8)>>3)+((bytep&4)>>2)+((bytep&2)>>1)+(bytep&1));

	 }
     bytep=(*p);
       for(k=0;k<rect;k++)
          count_black-=((bytep&(128>>k))>>(7-k));
	 p++;
     proc=((double)(count_black))/((double)(w));

     if(neg==TRUE)
	 {
	   if(proc<=neg_cut_white)
	   {
		neg=FALSE;
		if(IsNegSize(top-neg_top+1,w)&&(!(IfNegIn(root,nRc,oldtop,top,neg_left,neg_right))) )
		{
            Rc.top=oldtop;
			Rc.bottom=top;
			Rc.left=(now->neg).pRc[0].left;
			Rc.right=(now->neg).pRc[0].right;
			if((now->neg).rot.pmasp)
            {
              RSELSTR_UnRotateRect(-skew,&Rc,1,(now->neg).rot.Rc,(now->neg).rot.begx,(now->neg).rot.movey,(now->neg).rot.flmovey,(now->neg).rot.hi);
            }
			temp=NULL;
			temp=new NegList(Rc);
            memcpy(&(temp->neg.rot.Rc),&(now->neg.rot.Rc),size_Rect16);
			now->AddNeg(temp);
            PostRotate(temp,skew);
			nRc++;
		}
	   }
	 }
     else
	 {
	  if(proc>=neg_cut_black)
	  {
		neg=TRUE;
        oldtop=top;
	  }
	  else
		 oldtop=top+1;
	 }
     top++;
	 stop--;
	 pend+=bytewide;
	}

    while(root->prev)
		root=root->prev;
    if(((now->neg).pRc[0].bottom-oldtop+1<inf_neg_h)||((IfNegIn(root,nRc,oldtop,top,(now->neg).pRc[0].left,(now->neg).pRc[0].right))) )
	{
		temp=now;
		now=now->next;
		if(!temp->prev)
			root=now;
	    delete temp;
		nRc--;
	}
	else
	{
	  (now->neg).pRc[0].top=oldtop;
	  if((now->neg).rot.pmasp)
      {
           RSELSTR_UnRotateRect(-skew,&((now->neg).pRc[0]),1,(now->neg).rot.Rc,(now->neg).rot.begx,(now->neg).rot.movey,(now->neg).rot.flmovey,(now->neg).rot.hi);
           PostRotate(now,skew);
      }
	  DeleteRotateMas(&((now->neg).rot.pmasp),&((now->neg).rot.begx),&((now->neg).rot.movey),&((now->neg).rot.flmovey),&((now->neg).rot.hi));
      now=now->next;
	}
 }

 (*proot)=root;


}

Bool IfNegIn(NegList* root,int nRc,int top,int bottom,int left,int right)
{
 int i;
 int d_top,d_bot;
 NegList* now=root;
 int s_top=3*(DPIY+1)/300;
 int s_bot=3*(DPIY+1)/300;
 int neg_top;
 int neg_bottom;
 int neg_left;
 int neg_right;

	 for(i=0;i<nRc;i++)
	 {
		 neg_top=(now->neg).pRc[0].top;
		 neg_bottom=(now->neg).pRc[0].bottom;
		 neg_left=(now->neg).pRc[0].left;
		 neg_right=(now->neg).pRc[0].right;
		 if((neg_top==top)&&(neg_bottom==bottom)&&(neg_left==left)&&(neg_right==right))
			 continue;
         d_top=top-neg_top;
		 d_bot=neg_bottom-bottom;
		 if((d_top<s_top)&&(d_bot<s_bot))
		 {
           if( ((neg_left<=left)&&(neg_right>left)) || ((neg_left>left)&&(right>neg_left)) )
			   return TRUE;
		 }
		now=now->next;
	 }
	 return FALSE;
}



Bool SearchNegBySize(CCOM_handle hCCOM, Handle hCPage,NegList** ROOT,int& nRC)
{
 NegList* root=NULL;
 NegList* temp=NULL;
 Rect16 Rc;
 CCOM_comp * pcomp;
 pcomp = CCOM_GetFirst (hCCOM, NULL);
 if(pcomp==NULL)
	 return FALSE;
 PAGEINFO info = {0};
 GetPageInfo(hCPage,&info);
 DPIX=info.DPIX;
 DPIY=info.DPIY;

 InitRc(&Rc,pcomp);
 if( IsNegSize(Rc.bottom-Rc.top+1,Rc.right-Rc.left+1) )
 {
   if(!(root=new NegList(Rc) ))
	  return FALSE;
   nRC++;
 }

 for(;;)
 {
  pcomp = CCOM_GetNext (pcomp, NULL);

  if (pcomp==NULL)
	break;

  InitRc(&Rc,pcomp);
  if( IsNegSize(Rc.bottom-Rc.top+1,Rc.right-Rc.left+1) )
  {
   if(!root)
   {
    if(!(root=new NegList(Rc) ))
	  return FALSE;
	nRC++;
   }
   else
   {
    temp=NULL;
    temp=new NegList(Rc);
    root->AddNeg(temp);
    if(temp)
	{
		root=temp;
        nRC++;
	}
   }
  }
 }

 (*ROOT)=root;
 return TRUE;
}


void SearchNegByBlack(Handle hCPage,NegList** proot,int& nRC)
{
 NegList* now=(*proot);
 NegList* temp;
 while(now)
 {
  if(!(IsNegBlack(hCPage,now)) )
  {
   temp=now;
   now=now->next;
   if(!temp->prev)
	   (*proot)=now;
   delete temp;
   nRC--;
  }
  else
	  now=now->next;
 }
}

/*
Bool IfDrob(Handle hCPage,Rect16* pRc,int num,CCOM_comp * pdr)
{
 int inteps=2;
 int delta=5;
 int oldbot=pRc[num].bottom;
 int oldtop=pRc[num].top;
 int left=pdr->left;
 int oldright=pRc[num].right;
 int oldleft=pRc[num].left;
 int delta_top=pRc[num].top-pdr->upper;
 if(delta_top<0) delta_top=-delta_top;
 int delta_bottom=pRc[num].bottom-pdr->upper-pdr->h+1;
 if(delta_bottom<0) delta_bottom=-delta_bottom;

 if( (left-oldright)>inteps || (left-oldright)<0 )
	 return FALSE;
 if( (delta_top>delta) || (delta_bottom>delta) )
	 return FALSE;

 if(oldtop>pdr->upper) pRc[num].top=pdr->upper;
 if(oldbot<pdr->upper+pdr->h-1) pRc[num].bottom=pdr->upper+pdr->h-1;
 pRc[num].left=oldright;
 pRc[num].right=left;
 if( !(IsNegBlack(hCPage,now,num)) )
 {
  pRc[num].top=oldtop;
  pRc[num].bottom=oldbot;
  pRc[num].right=oldright;
  pRc[num].left=oldleft;
  return FALSE;
 }
 else
 {
  pRc[num].left=oldleft;
  pRc[num].right=pdr->left+pdr->w-1;
 }

 return TRUE;
}
*/



void NormNeg(Handle hCPage,NegList* now,NegList** proot,int& nRc)
{
 double black_del;
 int i,j,k;
 uchar* pmasp;
 Rect16* Rc=(now->neg).pRc;
 int w;
 int h;
 int rect;
 register uchar bytep;
 register int count_black;
 int topp,lp;
 double proc;
 int stop;
 register uchar* p;
 register uchar* pend;
 uchar* pbeg;


 if((now->neg).rot.pmasp)
 {
	 pmasp=(now->neg).rot.pmasp;
 }
 else
 {
  uchar Data[1000];
  memset (Data, 0, sizeof (Data));
	pmasp = Data;

  if(!(GetMasP(Rc,&pmasp)) )
  {

	 //попытка - не пытка

   Bool top=FALSE,left=FALSE,right=FALSE,bottom=FALSE;

   	  while((*Rc).top<=0)
		  ((*Rc).top)++;
	  while( ((*Rc).bottom+1)>Height )
          ((*Rc).bottom)--;


	  while((*Rc).left<0)
		  ((*Rc).left)++;
	  while( ((*Rc).right+1)>Width )
          ((*Rc).right)--;

   int prewide=(((*Rc).right-(*Rc).left+8)/8)*8;
   if( ((*Rc).left+prewide)>Width )
	    (*Rc).right-=8;

   pmasp = Data;
   if(!(GetMasP(Rc,&pmasp)) )
   {
//Внимание!!! Здесь не должно останавливаться
    if((*Rc).top<5) top=TRUE;
    if((*Rc).left<5) left=TRUE;
    if((*Rc).right>(int16_t)(Width)-5) right=TRUE;
    if((*Rc).bottom>(int16_t)(Height)-5) bottom=TRUE;
    k=10;
    for(i=0;i<k;i++)
    {
	  if(top) ((*Rc).top)++;
	  if(bottom) ((*Rc).bottom)--;
	  if(left) ((*Rc).left)++;
	  if(right) ((*Rc).right)--;
      pmasp = Data;
      if( GetMasP(Rc,&pmasp) )
		break;
    }

    if(i==k)
    {
     if(now==(*proot))
	  (*proot)=now->next;
     delete now;
     nRc--;
    return;
    }
   }
  }
 }
 w=(*Rc).right-(*Rc).left+1;
 h=(*Rc).bottom-(*Rc).top+1;
 const int bytewide=(w+7)/8;

 i=7-(bytewide*8-w);
 j=128;
 rect=w&7;
 if(!rect)
	 rect=8;
 topp=0;
 lp=0;
 stop=h-topp-inf_neg_h;

 black_del=sup_neg_black_del;
 p=pmasp;
 pend=p+(bytewide-1);
 i=w-lp;
 while(stop>0)
 {
//  if( (h-topp)>(w-lp) ) black_del=sup_neg_black_del;
//  else black_del=weak_neg_black_del;
  count_black=i;
  for(;p<pend;p++)
  {
	  bytep=(*p);
	  count_black-=((bytep>>7)+((bytep&64)>>6)+((bytep&32)>>5)+((bytep&16)>>4)+((bytep&8)>>3)+((bytep&4)>>2)+((bytep&2)>>1)+(bytep&1));
  }
  bytep=(*p);
       for(k=0;k<rect;k++)
          count_black-=((bytep&(128>>k))>>(7-k));
  p++;
  proc=((double)(count_black))/((double)(i));
  if(proc>black_del)
	  break;
  topp++;
  stop--;
  pend+=bytewide;
 }

 if( !(IsNegSize(h-topp,w-lp)) )
 {
  if(now==(*proot))
	 (*proot)=now->next;
  delete now;
  nRc--;
  return;
 }

 stop=(h-topp)-inf_neg_h;
 black_del=sup_neg_black_del;
 pend=pmasp+(h-1)*bytewide;
 p=pend+(bytewide-1);
 i=w-lp;
 while(stop>0)
 {
//  if( (h-topp)>(w-lp) ) black_del=sup_neg_black_del;
//  else black_del=weak_neg_black_del;
  count_black=i;
  bytep=(*p);
       for(k=0;k<rect;k++)
          count_black-=((bytep&(128>>k))>>(7-k));
  p--;
  for(;p>=pend;p--)
  {
	  bytep=(*p);
	  count_black-=((bytep>>7)+((bytep&64)>>6)+((bytep&32)>>5)+((bytep&16)>>4)+((bytep&8)>>3)+((bytep&4)>>2)+((bytep&2)>>1)+(bytep&1));
  }
  proc=((double)(count_black))/((double)(i));
  if(proc>black_del)
	  break;
  h--;
  stop--;
  pend-=bytewide;
 }

 if( !(IsNegSize(h-topp,w-lp))  )
 {
  if(now==(*proot))
	 (*proot)=now->next;
  delete now;
  nRc--;
  return;
 }

 if( (h-topp)<(w-lp) )
	 black_del=sup_neg_black_del-.2;
 else
	 black_del=weak_neg_black_del;

 stop=(w-lp-inf_neg_h);
 pbeg=pmasp+topp*bytewide+(lp>>3);
 i=h-topp-1;
 pend=pbeg+i*bytewide;
 i=h-topp;
 while(stop>0)
 {
  pbeg=pmasp+topp*bytewide+(lp>>3);
  pend=pbeg+i*bytewide;
  count_black=i;
  switch(lp&7)
  {
  case 0:
  for(p=pbeg;p<=pend;p+=bytewide)
  {
   count_black-=((*p)>>7);
  }
  break;
  case 1:
  for(p=pbeg;p<=pend;p+=bytewide)
  {
   count_black-=(((*p)&64)>>6);
  }
  break;
  case 2:
  for(p=pbeg;p<=pend;p+=bytewide)
  {
   count_black-=(((*p)&32)>>5);
  }
  break;
  case 3:
  for(p=pbeg;p<=pend;p+=bytewide)
  {
   count_black-=(((*p)&16)>>4);
  }
  break;
  case 4:
  for(p=pbeg;p<=pend;p+=bytewide)
  {
   count_black-=(((*p)&8)>>3);
  }
  break;
  case 5:
  for(p=pbeg;p<=pend;p+=bytewide)
  {
   count_black-=(((*p)&4)>>2);
  }
  break;
  case 6:
  for(p=pbeg;p<=pend;p+=bytewide)
  {
   count_black-=(((*p)&2)>>1);
  }
  break;
  case 7:
  for(p=pbeg;p<=pend;p+=bytewide)
  {
   count_black-=((*p)&1);
  }
  pbeg++;
  pend++;
  break;
  default:break;
  }
  proc=((double)(count_black))/((double)(h-topp));
  if(proc>black_del)
	  break;
  lp++;
  stop--;
 }
 if( !(IsNegSize(h-topp,w-lp)) )
 {
  if(now==(*proot))
	 (*proot)=now->next;
  delete now;
  nRc--;
   return;
 }

 if( (h-topp)<(w-lp) )
	 black_del=sup_neg_black_del-.2;
 else
	 black_del=weak_neg_black_del;

 stop=(w-lp-inf_neg_h);
 pbeg=pmasp+topp*bytewide+((w-1)>>3);
 i=h-topp-1;
 pend=pbeg+i*bytewide;
 i=h-topp;
 while(stop>0)
 {
  pbeg=pmasp+topp*bytewide+((w-1)>>3);
  pend=pbeg+i*bytewide;
  count_black=i;
  switch(w&7)// real need w-1!!!
  {
  case 1:
  for(p=pbeg;p<=pend;p+=bytewide)
  {
   count_black-=((*p)>>7);
  }
  pbeg--;
  pend--;
  break;
  case 2:
  for(p=pbeg;p<=pend;p+=bytewide)
  {
   count_black-=(((*p)&64)>>6);
  }
  break;
  case 3:
  for(p=pbeg;p<=pend;p+=bytewide)
  {
   count_black-=(((*p)&32)>>5);
  }
  break;
  case 4:
  for(p=pbeg;p<=pend;p+=bytewide)
  {
   count_black-=(((*p)&16)>>4);
  }
  break;
  case 5:
  for(p=pbeg;p<=pend;p+=bytewide)
  {
   count_black-=(((*p)&8)>>3);
  }
  break;
  case 6:
  for(p=pbeg;p<=pend;p+=bytewide)
  {
   count_black-=(((*p)&4)>>2);
  }
  break;
  case 7:
  for(p=pbeg;p<=pend;p+=bytewide)
  {
   count_black-=(((*p)&2)>>1);
  }
  break;
  case 0:
  for(p=pbeg;p<=pend;p+=bytewide)
  {
   count_black-=((*p)&1);
  }
  break;
  default:break;
  }
  proc=((double)(count_black))/((double)(h-topp));
  if(proc>black_del)
	  break;
  w--;
  stop--;
 }
 if( !(IsNegSize(h-topp-2,w-lp-2)) )
 {
  if(now==(*proot))
	 (*proot)=now->next;
  delete now;
  nRc--;
  return;
 }

 (*Rc).right=(*Rc).left+w-1-1;
 (*Rc).left+=lp+1;
 (*Rc).bottom=(*Rc).top+h-1-1;
 (*Rc).top+=topp+1;
}


void NegMoveMas(Rect16* pRc,int& nRc,int num)
{
 int i;
  for(i=num;i<(nRc-1);i++)
  {
	  pRc[i].left=pRc[i+1].left;
      pRc[i].top=pRc[i+1].top;
	  pRc[i].right=pRc[i+1].right;
	  pRc[i].bottom=pRc[i+1].bottom;
  }
  nRc--;
}
/*
void NegMoveMas(Rotating* rot,int nRc,int num)
{
  Rotating r;
  r.begx=rot[num].begx;
  r.flmovey=rot[num].flmovey;
  r.hi=rot[num].hi;
  r.movey=rot[num].movey;
  r.pmasp=rot[num].pmasp;
	for(int i=num;i<nRc-1;i++)
	{
		rot[i].begx=rot[i+1].begx;
		rot[i].flmovey=rot[i+1].flmovey;
		rot[i].hi=rot[i+1].hi;
		rot[i].movey=rot[i+1].movey;
		rot[i].pmasp=rot[i+1].pmasp;
	}
   rot[i].begx=r.begx;
   rot[i].flmovey=r.flmovey;
   rot[i].hi=r.hi;
   rot[i].movey=r.movey;
   rot[i].pmasp=r.pmasp;
   DeleteRotateMas(rot[i].pmasp,rot[i].begx,rot[i].movey,rot[i].flmovey,rot[i].hi);
}
*/
void PMoveMas(double* p,int nRc,int num)
{
 int i;
  for(i=num;i<(nRc-1);i++)
	  p[i]=p[i+1];

}

void NegMoveMasR(Rect16* pRc,int& nRc,int num)
{
 int i;
  for(i=nRc;i>num;i--)
  {
	  pRc[i].left=pRc[i-1].left;
      pRc[i].top=pRc[i-1].top;
	  pRc[i].right=pRc[i-1].right;
	  pRc[i].bottom=pRc[i-1].bottom;
  }
  nRc++;
}


Bool IsNegSize(int16_t h,int16_t w)
{
 if( (h<inf_neg_h) || (w<inf_neg_h) ) return FALSE;
 if( (h<inf_neg_w) && (w<inf_neg_w) ) return FALSE;
 return TRUE;
}

Bool IsNegBlack(Handle hCPage,NegList* now)
{
 int i,j,k;
 uchar *pmasp;
 if((now->neg).rot.pmasp)
   pmasp=(now->neg).rot.pmasp;
 else
 {
  uchar Data[1000];
  memset (Data, 0, sizeof (Data));
	pmasp = Data;
  if(!(GetMasP(now->neg.pRc,&pmasp)) )
  {
   while(IsNegSize((now->neg).pRc[0].bottom-(now->neg).pRc[0].top+1,(now->neg).pRc[0].right-(now->neg).pRc[0].left+1) )
   {
	  ((now->neg).pRc[0].top)++;
	  ((now->neg).pRc[0].bottom)--;
	  ((now->neg).pRc[0].left)++;
	  ((now->neg).pRc[0].right)--;
       pmasp = Data;
      if( GetMasP((now->neg).pRc,&pmasp) )
		break;
   }
   pmasp = Data;
   if(!(GetMasP((now->neg).pRc,&pmasp)) )
    return FALSE;
  }
 }
 int w=(now->neg).pRc[0].right-(now->neg).pRc[0].left+1;
 int h=(now->neg).pRc[0].bottom-(now->neg).pRc[0].top+1;
 const int bytewide=(w+7)/8;
 register int count_black=w*h;
 register uchar bytep;
 i=7-(bytewide*8-w);
 j=128;
 int rect=w&7;
 if(!rect)
	 rect=8;

 register uchar* p=pmasp;
 const uchar* pend=p+h*bytewide;
 i=1;
 for(;p<pend;p++)
 {
   bytep=(*p);
	  if(i==bytewide)
	  {
       for(k=0;k<rect;k++)
          count_black-=((bytep&(128>>k))>>(7-k));
	   i=0;
	  }
	  else
	  {
	  count_black-=((bytep>>7)+((bytep&64)>>6)+((bytep&32)>>5)+((bytep&16)>>4)+((bytep&8)>>3)+((bytep&4)>>2)+((bytep&2)>>1)+(bytep&1));
	  }
   p++;
   i++;
 }
 double proc;
 proc=((double)(count_black))/((double)(w*h));

 if(proc>inf_neg_black)
     return TRUE;
 else
	 return FALSE;
}


Bool GetMasP(Handle hCPage,Rect16 Rc,uchar** ppmasp)
{
    int prewide;
	int left=Rc.left;
	int h=Rc.bottom-Rc.top+1;
	int w=Rc.right-Rc.left+1;
	int upper=Rc.top-1;
	if( (w%8)==0)
		prewide=w;
	else
	{prewide=w/8+1;
	 prewide*=8;
	}

	PAGEINFO info = {0};
	CIMAGEInfoDataInGet DataInto = {0};
	CIMAGEInfoDataOutGet DataOut = {0};
	uchar Name[CPAGE_MAXNAME];
	Bool ret;
	int i;

	/*  1. Подготовка к запросу части изображения.  */
	DataInto.dwHeight   = (uint32_t)(h);
	DataInto.dwWidth    = (uint32_t)(prewide);
	DataInto.wByteWidth = (uint16_t)(prewide/8);
	DataInto.dwX        = left;
	DataInto.dwY        = upper;
	DataInto.MaskFlag   = 0x00;
    GetPageInfo(hCPage,&info);
	for (i=0; i<CPAGE_MAXNAME; i++)
		Name[i] = ImageName[i];
	DataOut.dwWidth    = DataInto.dwWidth;
	DataOut.dwHeight   = DataInto.dwHeight;
	DataOut.wByteWidth = DataInto.wByteWidth;
	DataOut.byBit      = (uint16_t)info.BitPerPixel;
	DataOut.lpData     = *ppmasp;

	/*  5. Чтение части изображения.  */
	ret = CIMAGE_GetData (Name, &DataInto, &DataOut);
	if (!ret)
		return FALSE;
	if (DataOut.lpData==NULL)
		return FALSE;
	*ppmasp = DataOut.lpData;
	return TRUE;
}

Bool GetMasP(Rect16* pRc,uchar** ppmasp)
{
	*ppmasp=pNegImage->GetPmasp(pRc);
	if(*ppmasp==NULL)
		return FALSE;
	return TRUE;
}

void PrintFrameComp(Handle hCPage,Rect16* pRc,int num)
{
 int i,j,k,bytewide;
 uchar bytep;
 uchar Data[1000];

 int h=pRc[num].bottom-pRc[num].top+1;
 int w=pRc[num].right-pRc[num].left+1;
 bytewide=(w)/8;
 if( ((w)%8) != 0)
	 bytewide++;
 uchar* pmasp;
 memset (Data, 0, sizeof (Data));
 pmasp = Data;
 if( !(GetMasP(&(pRc[num]),&pmasp)) )
	 return;
 printf("up:");
 i=7-(bytewide*8-w);
 j=128;
 int rect=j>>i;
 for(j=0;j<(bytewide-1);j++)
	 {
	  bytep=pmasp[j];
	  for(k=128;k>=1;k=k>>1)
	   if( (bytep&k)==0 ) printf("b");
	   else printf("w");
	 }
	 bytep=pmasp[j];
     for(k=128;k>=rect;k=k>>1)
       if( (bytep&k)==0 ) printf("b");
	   else printf("w");

 printf("\ndown:");
 for(j=0;j<(bytewide-1);j++)
	 {
	  bytep=pmasp[(h-1)*bytewide+j];
	  for(k=128;k>=1;k=k>>1)
	   if( (bytep&k)==0 ) printf("b");
	   else printf("w");
	 }
	 bytep=pmasp[(h-1)*bytewide+j];
     for(k=128;k>=rect;k=k>>1)
       if( (bytep&k)==0 ) printf("b");
	   else printf("w");

 printf("\nleft:");
 for(i=0;i<h;i++)
  {bytep=pmasp[i*bytewide];
       if( (bytep&128)==0 ) printf("b");
	   else printf("w");
  }

 printf("\nright:");
 for(i=1;i<=h;i++)
  {bytep=pmasp[i*bytewide-1];
       if( (bytep&rect)==0 ) printf("b");
	   else printf("w");
  }
 printf("\n");
}

Bool InitNegMas(Rect16** ppRc,int len)
{
	(*ppRc)=NULL;
	if(!((*ppRc)=new Rect16[len]))
		return FALSE;
	return TRUE;
}


Bool InitNegMas(RecVersions** ppRc,int len)
{
	(*ppRc)=NULL;
	if(!((*ppRc)= new RecVersions[len]))
		return FALSE;
	return TRUE;
}

Bool InitNegMas(UniVersions** ppRc,int len)
{
	(*ppRc)=NULL;
	if(!((*ppRc)= new UniVersions[len]))
		return FALSE;
	return TRUE;
}


Bool InitNegMas(CCOM_comp*** ppRc,int len)
{
	(*ppRc)=NULL;
	if(!((*ppRc)=new CCOM_comp*[len]))
		return FALSE;
	return TRUE;
}

void DelNegMas(Rect16 **masp)
{
	delete[] (*masp);
	(*masp)=NULL;
}

void DelNegMas(RecVersions *masp)
{
	delete[] masp;
}

void DelNegMas(UniVersions *masp)
{
	delete[] masp;
}

void DelNegMas(CCOM_comp **masp)
{
	delete[] masp;
}


Bool AddLenNegMas(Rect16** ppRc,int& len,int add)
{Rect16 *dop;
 int i;
 if(!(InitNegMas(&dop,len)) )
	 return FALSE;
 for(i=0;i<len;i++)
	 dop[i]=(*ppRc)[i];
 DelNegMas(ppRc);
 if(!(InitNegMas(ppRc,len+add)) )
 {
	 (*ppRc)=dop;
	 return FALSE;
 }
 for(i=0;i<len;i++)
	 (*ppRc)[i]=dop[i];
 len+=add;
 DelNegMas(&dop);
 return TRUE;
}

Bool AddLenNegMas(CCOM_comp*** ppRc,int& len,int add)
{CCOM_comp **dop;
 int i;
 if(!(InitNegMas(&dop,len)) )
	 return FALSE;
 for(i=0;i<len;i++)
	 dop[i]=(*ppRc)[i];
 DelNegMas(*ppRc);
 if(!(InitNegMas(ppRc,len+add)) )
 {
	 (*ppRc)=dop;
	 return FALSE;
 }
 for(i=0;i<len;i++)
	 (*ppRc)[i]=dop[i];
 len+=add;
 DelNegMas(dop);
 return TRUE;
}

void InitRc(Rect16* pRc,CCOM_comp * pcomp)
{
 pRc->left   = pcomp->left;
 pRc->right  = pcomp->left + pcomp->w - 1;
 pRc->top    = pcomp->upper;
 pRc->bottom = pcomp->upper + pcomp->h - 1;
}


void InitRc(Rect16* pRc,int nRc,CCOM_comp * pcomp)
{
 pRc[nRc].left   = pcomp->left;
 pRc[nRc].right  = pcomp->left + pcomp->w - 1;
 pRc[nRc].top    = pcomp->upper;
 pRc[nRc].bottom = pcomp->upper + pcomp->h - 1;
}

void NegInvert(uchar* pmasp,Rect16 N)
{
	uchar* p=pmasp;
	uchar* pend=pmasp+(N.bottom-N.top+1)*( (N.right-N.left+8)/8 );
	for(;p<pend;p++)
		(*p)=~(*p);
}

int absolut(int x)
{
	if(x<0)
		return -x;
	else
		return x;
}

void PostRotate(NegList* now,int skew)
{
    Rect16 Rc;
    Rect16 Rect;
    const int size_Rect16=sizeof(Rect16);
    memcpy(&Rc,&(now->neg.rot.Rc),size_Rect16);
    memcpy(&Rect,(now->neg.pRc),size_Rect16);
    const int h=Rc.bottom-Rc.top+1;

    if(h<3*(Rect.bottom-Rect.top))
        return;

    if(skew>0)
    {
        Rect.left+=((Rc.bottom-Rect.bottom)*skew)/2048;
        Rect.right-=((Rect.top-Rc.top)*skew)/2048;
    }
    else
    {
        Rect.left+=((Rc.top-Rect.top)*skew)/2048;
        Rect.right-=((Rect.bottom-Rc.bottom)*skew)/2048;
    }

    if(Rect.left>=Rect.right)
        Rect.right=Rect.left+1;

    memcpy(now->neg.pRc,&Rect,size_Rect16);
}



/*union of two strogo more massivs

#include<stdio.h>
#include<stdlib.h>
void un(float [],float [],float [],int);

main()
{float *a,*b,*c,s;int n,i=0;FILE *f,*g;
printf("Enter N\n");scanf("%d",&n);
a=(float*)malloc(n*sizeof(float));
b=(float*)malloc(n*sizeof(float));
c=(float*)malloc((2*n+1)*sizeof(float));
do{f=fopen("a.dat","r");g=fopen("b.dat","r");}while((f==NULL)||(g==NULL));
while(fscanf(f,"%f",&s)>0){a[i]=s;i++;printf("%f ",s);}fclose(f);
i=0;printf("\n");
while(fscanf(g,"%f",&s)>0){b[i]=s;i++;printf("%f ",s);}fclose(g);printf("\n");
un(a,b,c,n);i=0;
while(c[i]!='\0'){printf("%f ",c[i]);i++;}
return;}

void un(float a[],float b[],float c[],int n)
{int i,j,l=0,m=0,d,e=0,k;float x;
for(i=0; i<n; i++){x=b[i];d=n;
while(e!=d){k=(d+e)/2;if(x>a[k]) e=k+1;else d=k;}
if(x==a[d]){c[l]=a[m];l++;m++;continue;}
for(j=m ; j<d ; j++){c[l]=a[m];l++;m++;}
c[l]=x;l++;} c[l]='\0';
return;}

*/
