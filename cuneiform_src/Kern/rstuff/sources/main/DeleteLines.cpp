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

/*#include <windows.h>*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
/*#include <crtdbg.h>*/

#include "ctiimage.h"
#include "cpage.h"
#include "lns.h"
#include "rline.h"
#include "cline.h"
#include "dpuma.h"
#include "ctdib.h"
#include "linedefs.h"

#include "compat_defs.h"

    struct MyLine
	{
		int begx;
		int begy;
		int endx;
		int endy;
		int wide10;
	};

	extern int KVO;
//	extern Handle hUseCLine;
	extern Handle hNewLine;
//	extern Handle InsideKill;
	extern Handle hNotTestAlik;
	extern Handle NotKillPointed;
	extern Handle ObvKillLines;

	Bool fl_not_kill_pointed;
	Bool dphNotTestAlik;


Bool AddLenLineMas(MyLine** ppLines,int& len,int add);
Bool InitLineMas(MyLine** ppLines,int len);
void DelLineMas(MyLine *masp);
Bool DelOneHorLine(uchar* pmasp,const int bytewide,int num_str,int begx,int begy,int endx,int endy,int wide10);
Bool DelOneVerLine(uchar* pmasp,const int bytewide,int num_str,int begx,int begy,int endx,int endy,int wide10);
Bool DelLineFromInside(uchar* pmasp,const int bytewide,int num_str,CLINE_handle hline);



Bool32 DeleteLines(Handle hCPage,void* phCLINE, const char* ImageDelLines)
{
//	LDPUMA_ConsoleN("Активизируется функция тривиального удаления линий");
	int time=clock();
	CLINE_handle* pCLINE=(CLINE_handle*)phCLINE;
    MyLine* pHorLines=NULL;
	MyLine* pVerLines=NULL;
	int nHorLines=0;
	int nVerLines=0;
	int len_hor_mas=0;
	int len_ver_mas=0;
	int add_len_mas=50;
	uint32_t size_lineinfo=sizeof(LineInfo);
	if(!LDPUMA_Skip(NotKillPointed))
		fl_not_kill_pointed=TRUE;
	else
		fl_not_kill_pointed=FALSE;
	dphNotTestAlik=!LDPUMA_Skip(hNotTestAlik);


	CIMAGEIMAGECALLBACK         cbk;
	CIMAGEIMAGECALLBACK         cbk1;
//	LinesTotalInfo          lti;
//	LineInfo			  linfo;     // собственно, что и надо
	PAGEINFO              info = {0};	 // Описание страницы
//	Handle                pBlock;
//	uint32_t				  size32;
	int					  i;
//	uint32_t    HorType;
//    uint32_t    VerType;
	Bool fl_cont;
    uchar ImageName[CPAGE_MAXNAME];
	// Получаем PAGEINFO текущей страницы
	GetPageInfo(hCPage,&info);

	// Копируем в pImage из PInfo указатель на изображение,
	// связанное со страницей
	for(i=0;i<CPAGE_MAXNAME;i++)
		ImageName[i]=info.szImageName[i];

	if (!CIMAGE_GetCallbackImage(ImageName,&cbk))
		return FALSE;

	cbk1.CIMAGE_ImageOpen  = cbk.CIMAGE_ImageOpen;
	cbk1.CIMAGE_ImageRead  = cbk.CIMAGE_ImageRead;
	cbk1.CIMAGE_ImageClose = cbk.CIMAGE_ImageClose;

	if (!CIMAGE_WriteCallbackImage((uchar*)ImageDelLines,cbk1))
	{
	    return FALSE;
	}

    Handle lpDIB;
    if(!CIMAGE_ReadDIB((uchar*)ImageDelLines,&lpDIB,1))
	{
	  CIMAGE_DeleteImage((uchar*)ImageDelLines);
	  return FALSE;
	}

    CTDIB* ctdib = new CTDIB;
	if(!ctdib)
	{
	  CIMAGE_DeleteImage((uchar*)ImageDelLines);
	  return FALSE;
	}

    ctdib->SetDIBbyPtr (lpDIB);
    const int bytewide=ctdib->GetLineWidthInBytes();
    int num_str=ctdib->GetLinesNumber ();
//   int bytesize=ctdib->GetImageSizeInBytes ();
    uchar* pmasp=(uchar*)(ctdib->GetPtrToBitFild());
//	uchar* pos=(uchar*)(ctdib->GetPtrToPixel (pHorLines[0].begx,pHorLines[0].begy));
//	uchar* pos2=(uchar*)(ctdib->GetPtrToPixel (pHorLines[0].begx,pHorLines[0].begy));

	uint32_t size_line_com=sizeof(LINE_COM);
	CLINE_handle hline;

   if(LDPUMA_Skip(ObvKillLines))
   {
	   for(hline=CLINE_GetFirstLine(*pCLINE);hline;hline=CLINE_GetNextLine(hline))
	   {
		DelLineFromInside(pmasp,bytewide,num_str,hline);
	   }
   }
   else//тривиальное снятие линий
   {
//	if(!LDPUMA_Skip(hUseCLine))
//	{
	 hline = CLINE_GetFirstLine(*pCLINE);
	 nHorLines=0;
     len_hor_mas=50;
	 nVerLines=0;
	 len_ver_mas=50;

     if(!InitLineMas(&pHorLines,len_hor_mas))
	 {
		 ctdib->ResetDIB ();
	     delete ctdib;
		 CIMAGE_DeleteImage((uchar*)ImageDelLines);
		 return FALSE;
	 }
	 if(!InitLineMas(&pVerLines,len_ver_mas))
	 {
         DelLineMas(pHorLines);
		 ctdib->ResetDIB ();
	     delete ctdib;
		 CIMAGE_DeleteImage((uchar*)ImageDelLines);
		 return FALSE;
	 }

     while(hline)
	 {
	  CPDLine cpdata;
	  fl_cont=FALSE;
	  cpdata=CLINE_GetLineData(hline);
      if(!cpdata)
	     hline=CLINE_GetNextLine(hline);
      else
	  {
        if((cpdata->Flags & LI_IsTrue)&&(!fl_not_kill_pointed||(fl_not_kill_pointed&&cpdata->Type!=NR_DT_LinePointed)))
        {
		  if(cpdata->Dir==LD_Horiz)
		  {
           if(nHorLines>=len_hor_mas)
		   {
			if(!AddLenLineMas(&pHorLines,len_hor_mas,add_len_mas))
			{
				fl_cont=TRUE;
			    hline=CLINE_GetNextLine(hline);
			}
		   }
		   if(!fl_cont)
		   {
            pHorLines[nHorLines].begx=cpdata->Line.Beg_X;
	        pHorLines[nHorLines].begy=cpdata->Line.Beg_Y;
		    pHorLines[nHorLines].endx=cpdata->Line.End_X;
		    pHorLines[nHorLines].endy=cpdata->Line.End_Y;
		    pHorLines[nHorLines].wide10=cpdata->Line.Wid10;
		    nHorLines++;
			hline=CLINE_GetNextLine(hline);
		   }
		  }
		  else
		  {
		   if(nVerLines>=len_ver_mas)
		   {
			if(!AddLenLineMas(&pVerLines,len_ver_mas,add_len_mas))
			{
				fl_cont=TRUE;
			    hline=CLINE_GetNextLine(hline);
		    }
           }
		   if(!fl_cont)
		   {
            pVerLines[nVerLines].begx=cpdata->Line.Beg_X;
	        pVerLines[nVerLines].begy=cpdata->Line.Beg_Y;
		    pVerLines[nVerLines].endx=cpdata->Line.End_X;
		    pVerLines[nVerLines].endy=cpdata->Line.End_Y;
		    pVerLines[nVerLines].wide10=cpdata->Line.Wid10;
		    nVerLines++;
			hline=CLINE_GetNextLine(hline);
		   }
		  }
		}
		else
			hline=CLINE_GetNextLine(hline);
	  }
	 }
/*	}
	else
	{
     pBlock=NULL;
	 pBlock = CPAGE_GetBlockFirst (hCPage, RLINE_BLOCK_TYPE );
	 if( pBlock==NULL)
	 {
		 info.Images|=IMAGE_DELLINE;
		 SetPageInfo(hCPage,info);
		 ctdib->ResetDIB ();
	     delete ctdib;
		 return TRUE;
	 }
	  size32 = CPAGE_GetBlockData(hCPage, pBlock, RLINE_BLOCK_TYPE, &lti, sizeof(LinesTotalInfo));
	  if (size32 != sizeof(LinesTotalInfo) )
	  {
		 ctdib->ResetDIB ();
	     delete ctdib;
		 CIMAGE_DeleteImage((uchar*)ImageDelLines);
		 return FALSE;
	  }

	 HorType = (uint32_t)lti.Hor.Lns;
	 VerType = (uint32_t)lti.Ver.Lns;


/////////////////////////////////////////////////////////////////////////////////////////////
     nHorLines=0;
     len_hor_mas=50;

     if(!InitLineMas(&pHorLines,len_hor_mas))
	 {
		 ctdib->ResetDIB ();
	     delete ctdib;
		 CIMAGE_DeleteImage((uchar*)ImageDelLines);
		 return FALSE;
	 }

     pBlock=NULL;
	 pBlock = CPAGE_GetBlockFirst(hCPage,HorType);

	 while (pBlock)
	 {
		fl_cont=FALSE;
		size32 = CPAGE_GetBlockData(hCPage,pBlock,HorType,&linfo,size_lineinfo);
		if(size32!=size_lineinfo)
		{
			fl_cont=TRUE;
			pBlock = CPAGE_GetBlockNext(hCPage,pBlock,HorType);
		}
		if(fl_cont)
		  continue;
		if(!(linfo.Flags&LI_IsTrue)||(fl_not_kill_pointed&&(linfo.Flags&LI_Pointed)))
		{
			fl_cont=TRUE;
            pBlock = CPAGE_GetBlockNext(hCPage,pBlock,HorType);
		}
		if(fl_cont)
			continue;

        if(nHorLines>=len_hor_mas)
		{
			if(!AddLenLineMas(&pHorLines,len_hor_mas,add_len_mas))
			{
				fl_cont=TRUE;
		    	pBlock = CPAGE_GetBlockNext(hCPage,pBlock,HorType);
			}
		}
		if(fl_cont)
			continue;

		pHorLines[nHorLines].begx=linfo.A.x;
		pHorLines[nHorLines].begy=linfo.A.y;
		pHorLines[nHorLines].endx=linfo.B.x;
		pHorLines[nHorLines].endy=linfo.B.y;
		pHorLines[nHorLines].wide10=(linfo.Thickness)*10;
		nHorLines++;

		pBlock = CPAGE_GetBlockNext(hCPage,pBlock,HorType);

	 }

/////////////////////////////////////////////////////////////////////////////////////////////

     nVerLines=0;
     len_ver_mas=50;

	 pBlock=NULL;
	 pBlock = CPAGE_GetBlockFirst(hCPage,VerType);

     if(!InitLineMas(&pVerLines,len_ver_mas))
	 {
		 ctdib->ResetDIB ();
	     delete ctdib;
		 CIMAGE_DeleteImage((uchar*)ImageDelLines);
		 DelLineMas(pHorLines);
		 return FALSE;
	 }

	 while (pBlock)
	 {
		fl_cont=FALSE;
		size32 = CPAGE_GetBlockData(hCPage,pBlock,VerType,&linfo,size_lineinfo);
		if(size32!=size_lineinfo)
		{
			fl_cont=TRUE;
            pBlock = CPAGE_GetBlockNext(hCPage,pBlock,VerType);
		}
		if(fl_cont)
		  continue;

		if(!(linfo.Flags&LI_IsTrue)||(fl_not_kill_pointed&&(linfo.Flags&LI_Pointed)))
		{
			fl_cont=TRUE;
            pBlock = CPAGE_GetBlockNext(hCPage,pBlock,VerType);
		}
		if(fl_cont)
			continue;

        if(nVerLines>=len_ver_mas)
		{
			if(!AddLenLineMas(&pVerLines,len_ver_mas,add_len_mas))
			{
				fl_cont=TRUE;
			    pBlock = CPAGE_GetBlockNext(hCPage,pBlock,VerType);
			}
		}
		if(fl_cont)
			continue;

		pVerLines[nVerLines].begx=linfo.A.x;
		pVerLines[nVerLines].begy=linfo.A.y;
		pVerLines[nVerLines].endx=linfo.B.x;
		pVerLines[nVerLines].endy=linfo.B.y;
		pVerLines[nVerLines].wide10=(linfo.Thickness)*10;
		nVerLines++;

		pBlock = CPAGE_GetBlockNext(hCPage,pBlock,VerType);

	 }
    }
*/

/////////////////////////////////////////////////////////////////////////////////////////////


    int time_hor=clock();
	for(i=0;i<nHorLines;i++)
	{
		if(pHorLines[i].begx>pHorLines[i].endx)
			DelOneHorLine(pmasp,bytewide,num_str,pHorLines[i].endx,pHorLines[i].endy,pHorLines[i].begx,pHorLines[i].begy,pHorLines[i].wide10);
		else
			DelOneHorLine(pmasp,bytewide,num_str,pHorLines[i].begx,pHorLines[i].begy,pHorLines[i].endx,pHorLines[i].endy,pHorLines[i].wide10);
	}
    time_hor=clock()-time_hor;
//	LDPUMA_ConsoleN("Time hor=%d",time_hor);

    int time_ver=clock();
	for(i=0;i<nVerLines;i++)
	{
		if(pVerLines[i].begy>pVerLines[i].endy)
			DelOneVerLine(pmasp,bytewide,num_str,pVerLines[i].endx,pVerLines[i].endy,pVerLines[i].begx,pVerLines[i].begy,pVerLines[i].wide10);
		else
			DelOneVerLine(pmasp,bytewide,num_str,pVerLines[i].begx,pVerLines[i].begy,pVerLines[i].endx,pVerLines[i].endy,pVerLines[i].wide10);
	}
    time_ver=clock()-time_ver;
//	LDPUMA_ConsoleN("Time ver=%d",time_ver);
    DelLineMas(pHorLines);
	DelLineMas(pVerLines);
   }//конец тривиального удаления

    ctdib->ResetDIB ();
	delete ctdib;

	info.Images|=IMAGE_DELLINE;
	SetPageInfo(hCPage,info);
	time=clock()-time;
//	LDPUMA_ConsoleN("Time work=%d",time);
	return TRUE;
}


Bool InitLineMas(MyLine** ppLines,int len)
{
	(*ppLines)=NULL;
	if(!((*ppLines)=new MyLine[len]))
		return FALSE;
	return TRUE;
}


void DelLineMas(MyLine *masp)
{
	delete[] masp;
}


Bool AddLenLineMas(MyLine** ppLines,int& len,int add)
{
 MyLine *dop;
 int i;
 if(!(InitLineMas(&dop,len)) )
	 return FALSE;
 for(i=0;i<len;i++)
	 dop[i]=(*ppLines)[i];
 DelLineMas(*ppLines);
 if(!(InitLineMas(ppLines,len+add)) )
 {
	 (*ppLines)=dop;
	 return FALSE;
 }
 for(i=0;i<len;i++)
	 (*ppLines)[i]=dop[i];
 len+=add;
 DelLineMas(dop);
 return TRUE;
}

Bool DelOneHorLine(uchar* pmasp,int bytewide,int num_str,int begx,int begy,int endx,int endy,int wide10)
{
 int wide;
 int x1;
 int x2;
 int y1;
 int y2;
 int old_x1;

 if((wide10%10))
    wide=wide10/10+1+(KVO<<1);
 else
	wide=wide10/10+(KVO<<1);
 if(!(wide%2))
	 wide++;

 x1=begx;
 if(x1<0)
	 x1=0;
 int startbyte=(x1>>3)+1;
 old_x1=x1;
 x1= startbyte<<3;
 x2=endx;
 if(x2>=(bytewide<<3))
	 x2=(bytewide<<3)-1;

 y1=num_str-1-(begy-(wide>>1));
 y2=num_str-1-(begy+(wide>>1));
 if(begy<endy)
 {
  if(begy-(wide>>1)<0)
  {
	 y1=num_str-1;
     wide=y1-y2+1;
  }
  if(endy+(wide>>1)>num_str-1)
  {
	  wide=(num_str-1-endy)<<1;
	  y2=y1-wide+1;
  }
 }
 else
 {
  if(begy+(wide>>1)>num_str-1)
  {
	 y2=0;
	 wide=y1-y2+1;
  }
  if(endy-(wide>>1)<0)
  {
	 wide=endy<<1;
	 y1=y2+wide-1;
  }
 }

 int i;
 int j=y1*bytewide+(old_x1>>3);
 uchar* now=pmasp+j;
 uchar* end;

 switch(old_x1&7)
 {
 case 0:
	 for(i=y1;i>=y2;i--)
	 {
		 (*now)=255;
		 now-=bytewide;
	 }
	 break;
 case 1:
	 for(i=y1;i>=y2;i--)
	 {
		 (*now)|=127;
		 now-=bytewide;
	 }
	 break;
 case 2:
	 for(i=y1;i>=y2;i--)
	 {
		 (*now)|=63;
		 now-=bytewide;
	 }
	 break;
 case 3:
	 for(i=y1;i>=y2;i--)
	 {
		 (*now)|=31;
		 now-=bytewide;
	 }
	 break;
 case 4:
	 for(i=y1;i>=y2;i--)
	 {
		 (*now)|=15;
		 now-=bytewide;
	 }
	 break;
 case 5:
	 for(i=y1;i>=y2;i--)
	 {
		 (*now)|=7;
		 now-=bytewide;
	 }
	 break;
 case 6:
	 for(i=y1;i>=y2;i--)
	 {
		 (*now)|=3;
		 now-=bytewide;
	 }
	 break;
 case 7:
	 for(i=y1;i>=y2;i--)
	 {
		 (*now)|=1;
		 now-=bytewide;
	 }
	 break;
 default:break;
 }

 now=pmasp+y1*bytewide+(x2>>3);

 switch(x2&7)
 {
 case 0:
	 for(i=y1;i>=y2;i--)
	 {
		 (*now)|=128;
		 now-=bytewide;
	 }
	 break;
 case 1:
	 for(i=y1;i>=y2;i--)
	 {
		 (*now)|=192;
		 now-=bytewide;
	 }
	 break;
 case 2:
	 for(i=y1;i>=y2;i--)
	 {
		 (*now)|=224;
		 now-=bytewide;
	 }
	 break;
 case 3:
	 for(i=y1;i>=y2;i--)
	 {
		 (*now)|=240;
		 now-=bytewide;
	 }
	 break;
 case 4:
	 for(i=y1;i>=y2;i--)
	 {
		 (*now)|=248;
		 now-=bytewide;
	 }
	 break;
 case 5:
	 for(i=y1;i>=y2;i--)
	 {
		 (*now)|=252;
		 now-=bytewide;
	 }
	 break;
 case 6:
	 for(i=y1;i>=y2;i--)
	 {
		 (*now)|=254;
		 now-=bytewide;
	 }
	 break;
 case 7:
	 for(i=y1;i>=y2;i--)
	 {
		 (*now)=255;
		 now-=bytewide;
	 }
	 break;
 default:break;
 }

 int w=x2-x1+1;
 int ownerbytewide = x2%8 ? (x2>>3)+1 : x2>>3;
 int spusk;
 int nowspusk;

 if(abs(begy-endy)<2)
 {
  j=y1*bytewide+startbyte;
  end=pmasp+y1*bytewide+ownerbytewide;
  const int size_t=ownerbytewide-startbyte;
  now=pmasp+j;

  if(size_t>0)
  {
  for(i=y1;i>=y2;i--)
  {
	  memset(now,255,size_t);
      now-=bytewide;
//	  for(;now<end;now++)
//	    (*now)=255;
//	  j-=bytewide;
//	  end-=bytewide;
  }
  }
  return TRUE;
 }

 if(begy<endy)
 {
  spusk=(w+endy-begy-1)/(endy-begy);
  end=pmasp+y1*bytewide+startbyte;
  for(i=y1;i>=y2 && end >= pmasp;i--)
  {
	  now=end;
	  nowspusk=spusk/2;
	  for(j=startbyte;j<ownerbytewide;j++)
	  {
	   //k=0
		  (*now)|=128;
		  nowspusk--;
		  if(!nowspusk)
		  {
			  nowspusk=spusk;

			  if ((now-=bytewide) < pmasp)
				  break;
		  }
	   //k=1
		  (*now)|=64;
		  nowspusk--;
		  if(!nowspusk)
		  {
			  nowspusk=spusk;

			  if ((now-=bytewide) < pmasp)
				  break;
		  }
       //k=2
		  (*now)|=32;
		  nowspusk--;
		  if(!nowspusk)
		  {
			  nowspusk=spusk;

			  if ((now-=bytewide) < pmasp)
				  break;
		  }
		//k=3
		  (*now)|=16;
		  nowspusk--;
		  if(!nowspusk)
		  {
			  nowspusk=spusk;

			  if ((now-=bytewide) < pmasp)
				  break;
		  }
		//k=4
		  (*now)|=8;
		  nowspusk--;
		  if(!nowspusk)
		  {
			  nowspusk=spusk;

			  if ((now-=bytewide) < pmasp)
				  break;
		  }
		 //k=5
		  (*now)|=4;
		  nowspusk--;
		  if(!nowspusk)
		  {
			  nowspusk=spusk;

			  if ((now-=bytewide) < pmasp)
				  break;
		  }
		//k=6
		  (*now)|=2;
		  nowspusk--;
		  if(!nowspusk)
		  {
			  nowspusk=spusk;

			  if ((now-=bytewide) < pmasp)
				  break;
		  }
		//k=7
		  (*now)|=1;
		  nowspusk--;
		  if(!nowspusk)
		  {
			  nowspusk=spusk;

			  if ((now-=bytewide) < pmasp)
				  break;
		  }
		  now++;
	  }
	  end-=bytewide;
  }
  return TRUE;
 }
 else
 {
  uchar* end_byte = pmasp + bytewide*num_str - 1;
  end=pmasp+y1*bytewide+startbyte;
  spusk=(w+begy-endy-1)/(begy-endy);
  for(i=y1;i>=y2 && end >= pmasp;i--)
  {
	  now=end;
	  nowspusk=spusk/2;
	  for(j=startbyte;j<ownerbytewide;j++)
	  {
	   //k=0
		  (*now)|=128;
		  nowspusk--;
		  if(!nowspusk)
		  {
			  nowspusk=spusk;

			  if ((now+=bytewide) > end_byte)
				  break;
		  }
	   //k=1
		  (*now)|=64;
		  nowspusk--;
		  if(!nowspusk)
		  {
			  nowspusk=spusk;

			  if ((now+=bytewide) > end_byte)
				  break;
		  }
       //k=2
		  (*now)|=32;
		  nowspusk--;
		  if(!nowspusk)
		  {
			  nowspusk=spusk;

			  if ((now+=bytewide) > end_byte)
				  break;
		  }
		//k=3
		  (*now)|=16;
		  nowspusk--;
		  if(!nowspusk)
		  {
			  nowspusk=spusk;

			  if ((now+=bytewide) > end_byte)
				  break;
		  }
		//k=4
		  (*now)|=8;
		  nowspusk--;
		  if(!nowspusk)
		  {
			  nowspusk=spusk;

			  if ((now+=bytewide) > end_byte)
				  break;
		  }
		 //k=5
		  (*now)|=4;
		  nowspusk--;
		  if(!nowspusk)
		  {
			  nowspusk=spusk;

			  if ((now+=bytewide) > end_byte)
				  break;
		  }
		//k=6
		  (*now)|=2;
		  nowspusk--;
		  if(!nowspusk)
		  {
			  nowspusk=spusk;

			  if ((now+=bytewide) > end_byte)
				  break;
		  }
		//k=7
		  (*now)|=1;
		  nowspusk--;
		  if(!nowspusk)
		  {
			  nowspusk=spusk;

			  if ((now+=bytewide) > end_byte)
				  break;
		  }
		  now++;
	  }
	  end-=bytewide;
  }
  return TRUE;
 }
}


Bool DelOneVerLine(uchar* pmasp,int bytewide,int num_str,int begx,int begy,int endx,int endy,int wide10)
{
 int wide;
 int x1;
 int x2;
 int y1;
 int y2;

 if((wide10%10))
    wide=wide10/10+1+(KVO<<1);
 else
	wide=wide10/10+(KVO<<1);
 if(!(wide%2))
	 wide++;

 y1=num_str-1-begy;
 y2=num_str-1-endy;

 if(y1>=num_str)
	 y1=num_str-1;
 if(y2<0)
	 y2=0;

 x1=begx-(wide>>1);
 x2=begx+(wide>>1);
 if(begx<endx)
 {
  if(x1<0)
  {
	 x1=0;
	 wide=x2-x1+1;
  }
  if(endx+(wide>>1)>(bytewide<<3)-1)
  {
	  wide=((bytewide<<3)-1-endx)<<1;
	  x2=x1+wide-1;
  }
 }
 else
 {
  if(begx+(wide>>1)>(bytewide<<3)-1)
  {
	  x2=(bytewide<<3)-1;
	  wide=x2-x1+1;
  }
  if(endx-(wide>>1)<0)
  {
	  wide=endx>>1;
	  x1=x2-wide+1;
  }
 }

 int w=y1-y2+1;
 int spusk;
 int nowspusk;
 int i;
 int j;
 int startbyte=y1*bytewide+(x1>>3);
 int nowbyte;
 int startrealp=x1%8;
 int realp;

 if(abs(begx-endx)<2)
 {
  for(i=y1;i>=y2;i--)
  {
	 nowbyte=startbyte;
	 realp=startrealp;
	 for(j=x1;j<=x2;j++)
	 {
		 pmasp[nowbyte]|=(128>>realp);
		 realp++;
		 if(realp==8)
		 {
			 realp=0;
			 nowbyte++;
		 }
	 }
	 startbyte-=bytewide;
  }
  return TRUE;
 }

 if(begx<endx)
 {
  spusk=(w+endx-begx-1)/(endx-begx);
  startbyte=y1*bytewide+(x1>>3);
  startrealp=x1%8;
  nowspusk=spusk>>1;
  for(i=y1;i>=y2;i--)
  {
	 nowbyte=startbyte;
	 realp=startrealp;
	 for(j=x1;j<=x2;j++)
	 {
		 pmasp[nowbyte]|=(128>>realp);
		 realp++;
		 if(realp==8)
		 {
			 realp=0;
			 nowbyte++;
		 }
	 }
	 nowspusk--;
	 if(!nowspusk)
	 {
      nowspusk=spusk;
	  startrealp++;
	  if(startrealp==8)
	  {
		  startrealp=0;
		  startbyte++;
	  }
	 }
	 startbyte-=bytewide;
  }
  return TRUE;
 }
 else
 {
  spusk=(w+begx-endx-1)/(begx-endx);
  startbyte=y1*bytewide+(x1>>3);
  startrealp=x1%8;
  nowspusk=spusk>>1;
  for(i=y1;i>=y2;i--)
  {
	 nowbyte=startbyte;
	 realp=startrealp;
	 for(j=x1;j<=x2;j++)
	 {
		 pmasp[nowbyte]|=(128>>realp);
		 realp++;
		 if(realp==8)
		 {
			 realp=0;
			 nowbyte++;
		 }
	 }
	 nowspusk--;
	 if(!nowspusk)
	 {
      nowspusk=spusk;
	  startrealp--;
	  if(startrealp==-1)
	  {
		  startrealp=7;
		  startbyte--;
	  }
	 }
	 startbyte-=bytewide;
  }
  return TRUE;
 }
}

/*
Bool GetMasP(Handle hCPage,uchar* ImageName,int x1,int y1,int x2,int y2,uchar** ppmasp)
{
    int prewide;
	int left=x1;
	int h=y2-y1+1;
	int w=x2-x1+1;
	int upper=y1;
	prewide=((w+7)>>3)<<3;

	PAGEINFO info = {0};
	CIMAGEInfoDataInGet DataInto = {0};
	CIMAGEInfoDataOutGet DataOut = {0};

	Bool ret;

	DataInto.dwHeight   = (uint32_t)(h);
	DataInto.dwWidth    = (uint32_t)(prewide);
	DataInto.wByteWidth = (uint16_t)(prewide/8);
	DataInto.dwX        = left;
	DataInto.dwY        = upper;
	DataInto.MaskFlag   = 0x00;
    GetPageInfo(hCPage,&info);

	DataOut.dwWidth    = DataInto.dwWidth;
	DataOut.dwHeight   = DataInto.dwHeight;
	DataOut.wByteWidth = DataInto.wByteWidth;
	DataOut.byBit      = (uint16_t)info.BitPerPixel;
	DataOut.lpData     = *ppmasp;
	ret = CIMAGE_GetData (ImageName, &DataInto, &DataOut);
	if (!ret)
		return FALSE;
	if (DataOut.lpData==NULL)
		return FALSE;
	*ppmasp = DataOut.lpData;
	return TRUE;
}
*/

Bool DelLineFromInside(uchar* pmasp,const int bytewide,int num_str,CLINE_handle hline)
{
 CLINE_handle hevent;
 CLINE_handle hinv;
 CPDLine data_line;
 CPDEvent data_event;
 CPDInterval data_inv;
 int Level;
 int Beg;
 int End;
 uchar* p;
 uchar* pUp;
 uchar* pDown;
 uchar* p0;
 int end_str=num_str-1;
 int endw=(bytewide<<3)-1;
 int byte_count=end_str*bytewide;
 int size_t;
// int j;
 uchar* pmaspend=pmasp+bytewide*num_str;

 data_line=CLINE_GetLineData(hline);

 if(!(data_line->Flags&LI_IsTrue))
	 return TRUE;

#ifdef _DEBUG
 if(dphNotTestAlik)
 {
 _ASSERT(data_line->Line.Beg_Y>=0&&data_line->Line.End_Y>=0);
 _ASSERT(data_line->Line.Beg_X>=0&&data_line->Line.End_X>=0);
 _ASSERT(data_line->Line.Beg_Y<num_str&&data_line->Line.End_Y<num_str);
 _ASSERT(data_line->Line.Beg_X<=endw&&data_line->Line.End_X<=endw);
 }
#endif

 if(!data_line)
	 return FALSE;

 if(data_line->Type==NR_DT_LinePointed)
 {
	 if(fl_not_kill_pointed)
		 return TRUE;

	 if(data_line->Dir==LD_Horiz)
		 DelOneHorLine(pmasp,bytewide,num_str,data_line->Line.Beg_X,data_line->Line.Beg_Y,data_line->Line.End_X,data_line->Line.End_Y,data_line->Line.Wid10);
	 else
		 DelOneVerLine(pmasp,bytewide,num_str,data_line->Line.Beg_X,data_line->Line.Beg_Y,data_line->Line.End_X,data_line->Line.End_Y,data_line->Line.Wid10);
	 return TRUE;
 }

 if(data_line->Dir==LD_Horiz)
 {
  if(data_line->rect.top<=0||data_line->rect.bottom>=end_str)
  {
    for(hevent=CLINE_GetFirstEvent(hline);hevent;hevent=CLINE_GetNextEvent(hevent))
    {
     data_event=CLINE_GetEventData(hevent);
     if(!data_event)
	   return FALSE;

	  Level=(end_str-data_event->Lev_0)*bytewide;
	  for(hinv=CLINE_GetFirstEventInv(hevent);hinv;hinv=CLINE_GetNextEventInv(hinv),Level-=bytewide)
	  {
		if(Level<0||Level>=byte_count)
		   continue;

		data_inv=CLINE_GetEventInvData(hinv);
		if(!data_inv)
			return FALSE;

//алгоритм
		 Beg=data_inv->Pos;
		 End=data_inv->Lent+Beg;
		 End--;
/*		 if ((End+7)/8 < bytewide)
			End += 7;
*/
		 p0=pmasp+(Level+(Beg>>3));
		 p=pmasp+(Level+(End>>3));
		 size_t=p-p0-1;
		 pUp=p-bytewide;
		 pDown=p+bytewide;
		 if(pUp<pmasp)
			 pUp+=bytewide;
		 if(pDown>=pmaspend)
			 pDown-=bytewide;

#ifdef _DEBUG
		 _ASSERT(pUp>=pmasp&&pDown<pmaspend);
#endif
/*
		 switch(End&7)
		 {
		 case 7:(*p)=255;(*pUp)=255;(*pDown)=255;break;
		 case 6:(*p)|=127;(*pUp)|=127;(*pDown)|=127;break;
		 case 5:(*p)|=63;(*pUp)|=63;(*pDown)|=63;break;
		 case 4:(*p)|=31;(*pUp)|=31;(*pDown)|=31;break;
		 case 3:(*p)|=15;(*pUp)|=15;(*pDown)|=15;break;
		 case 2:(*p)|=7;(*pUp)|=7;(*pDown)|=7;break;
		 case 1:(*p)|=3;(*pUp)|=3;(*pDown)|=3;break;
		 case 0:(*p)|=1;(*pUp)|=1;(*pDown)|=1;break;
		 default:break;
		 }
*/
		 if (p == p0)
		 {
			 for (int i = 7 - (Beg - ((Beg>>3)<<3)); i >= 7 - (End - ((End>>3)<<3)); i--)
			 {
				 (*p) |= (1<<i);
				 (*pUp) |= (1<<i);
				 (*pDown) |= (1<<i);
			 }
		 }
		 else
		 {
		 switch(End&7)
		 {
		 case 7:(*p)=255;(*pUp)=255;(*pDown)=255;break;
		 case 6:(*p)|=254;(*pUp)|=254;(*pDown)|=254;break;
		 case 5:(*p)|=252;(*pUp)|=252;(*pDown)|=252;break;
		 case 4:(*p)|=248;(*pUp)|=248;(*pDown)|=248;break;
		 case 3:(*p)|=240;(*pUp)|=240;(*pDown)|=240;break;
		 case 2:(*p)|=224;(*pUp)|=224;(*pDown)|=224;break;
		 case 1:(*p)|=192;(*pUp)|=192;(*pDown)|=192;break;
		 case 0:(*p)|=128;(*pUp)|=128;(*pDown)|=128;break;
		 default:break;
		 }

/*		 for(p--,pUp--,pDown--;p>p0;p--,pUp--,pDown--)
		 {
			 (*p)=255;
			 (*pUp)=255;
			 (*pDown)=255;
		 }
*/
		 if(size_t>0)
		 {
		  p-=size_t;
		  pUp-=size_t;
		  pDown-=size_t;
		  memset(p,255,size_t);
		  memset(pUp,255,size_t);
		  memset(pDown,255,size_t);
		 }

		 if(p>p0)
		 {
		  p--;
		  pUp--;
		  pDown--;
		 }

		 switch(Beg&7)
		 {
		 case 0:(*p)=255;(*pUp)=255;(*pDown)=255;break;
		 case 1:(*p)|=127;(*pUp)|=127;(*pDown)|=127;break;
		 case 2:(*p)|=63;(*pUp)|=63;(*pDown)|=63;break;
		 case 3:(*p)|=31;(*pUp)|=31;(*pDown)|=31;break;
		 case 4:(*p)|=15;(*pUp)|=15;(*pDown)|=15;break;
		 case 5:(*p)|=7;(*pUp)|=7;(*pDown)|=7;break;
		 case 6:(*p)|=3;(*pUp)|=3;(*pDown)|=3;break;
		 case 7:(*p)|=1;(*pUp)|=1;(*pDown)|=1;break;
		 default:break;
		 }

		 }
//конец алгоритма
	  }
   }
  }
  else
  {
    for(hevent=CLINE_GetFirstEvent(hline);hevent;hevent=CLINE_GetNextEvent(hevent))
    {
     data_event=CLINE_GetEventData(hevent);
     if(!data_event)
	   return FALSE;

	  Level=(end_str-data_event->Lev_0)*bytewide;
	  hinv=CLINE_GetFirstEventInv(hevent);
	  while(hinv)
	  {
		data_inv=CLINE_GetEventInvData(hinv);
		if(!data_inv)
			return FALSE;

//алгоритм
		 Beg=data_inv->Pos;
		 End=data_inv->Lent+Beg;
		 End--;
/*
		 if ((End+7)/8 < bytewide)
			 End +=7;
*/
		 p0=pmasp+(Level+(Beg>>3));
		 p=pmasp+(Level+(End>>3));
		 size_t=p-p0-1;
		 pUp=p-bytewide;
		 pDown=p+bytewide;
// Beg Almi 12-08-2003
//#ifdef _DEBUG
//		 _ASSERT(pUp>=pmasp&&pDown<pmaspend);
//#endif
	if (pUp<pmasp)
	{
#ifdef _DEBUG
		 _ASSERT(pUp>=pmasp);
#endif
		return FALSE;
	}
	else
	{
#ifdef _DEBUG
		 _ASSERT(pDown<pmaspend);
#endif
		pDown-=bytewide;
		if ((pDown<=pUp)||(pDown>=pmaspend))
			return FALSE;
	}
// End Almi 12-08-2003

		 if (p == p0)
		 {
			 for (int i = 7 - (Beg - ((Beg>>3)<<3)); i >= 7 - (End - ((End>>3)<<3)); i--)
			 {
				 (*p) |= (1<<i);
				 (*pUp) |= (1<<i);
				 (*pDown) |= (1<<i);
			 }
		 }
		 else
		 {
		 switch(End&7)
		 {
		 case 7:(*p)=255;(*pUp)=255;(*pDown)=255;break;
		 case 6:(*p)|=254;(*pUp)|=254;(*pDown)|=254;break;
		 case 5:(*p)|=252;(*pUp)|=252;(*pDown)|=252;break;
		 case 4:(*p)|=248;(*pUp)|=248;(*pDown)|=248;break;
		 case 3:(*p)|=240;(*pUp)|=240;(*pDown)|=240;break;
		 case 2:(*p)|=224;(*pUp)|=224;(*pDown)|=224;break;
		 case 1:(*p)|=192;(*pUp)|=192;(*pDown)|=192;break;
		 case 0:(*p)|=128;(*pUp)|=128;(*pDown)|=128;break;
		 default:break;
		 }

/*		 for(p--,pUp--,pDown--;p>p0;p--,pUp--,pDown--)
		 {
			 (*p)=255;
			 (*pUp)=255;
			 (*pDown)=255;
		 }
*/
		 if(size_t>0)
		 {
		  p-=size_t;
		  pUp-=size_t;
		  pDown-=size_t;
		  memset(p,255,size_t);
		  memset(pUp,255,size_t);
		  memset(pDown,255,size_t);
		 }

		 if(p>p0)
		 {
		  p--;
		  pUp--;
		  pDown--;
		 }

		 switch(Beg&7)
		 {
		 case 0:(*p)=255;(*pUp)=255;(*pDown)=255;break;
		 case 1:(*p)|=127;(*pUp)|=127;(*pDown)|=127;break;
		 case 2:(*p)|=63;(*pUp)|=63;(*pDown)|=63;break;
		 case 3:(*p)|=31;(*pUp)|=31;(*pDown)|=31;break;
		 case 4:(*p)|=15;(*pUp)|=15;(*pDown)|=15;break;
		 case 5:(*p)|=7;(*pUp)|=7;(*pDown)|=7;break;
		 case 6:(*p)|=3;(*pUp)|=3;(*pDown)|=3;break;
		 case 7:(*p)|=1;(*pUp)|=1;(*pDown)|=1;break;
		 default:break;
		 }

		 }

//конец алгоритма
		 Level-=bytewide;
		 hinv=CLINE_GetNextEventInv(hinv);
	  }
   }
  }
 }
 else
 {
  if(data_line->rect.left<=0||data_line->rect.right>=endw)
  {
   for(hevent=CLINE_GetFirstEvent(hline);hevent;hevent=CLINE_GetNextEvent(hevent))
   {
    data_event=CLINE_GetEventData(hevent);
    if(!data_event)
	   return FALSE;

	  Level=data_event->Lev_0;
	  for(hinv=CLINE_GetFirstEventInv(hevent);hinv;Level++,hinv=CLINE_GetNextEventInv(hinv))
	  {
		if(Level<0||Level>endw)
			 continue;

		data_inv=CLINE_GetEventInvData(hinv);
		if(!data_inv)
			 return FALSE;

//алгоритм
		Beg=end_str-data_inv->Pos;
		End=Beg-data_inv->Lent;
		End++;

		p0=pmasp+(End*bytewide+(Level>>3));
		p=p0+(Beg-End)*bytewide;
		_ASSERT(p0>=pmasp&&p<pmaspend);

		if(!Level)
		{
		 while(p>=p0)
		 {
		  (*p)|=192;
		  p-=bytewide;
		 }
		}
		else
		{
		 if(Level==endw)
		 {
		  while(p>=p0)
		  {
		   (*p)|=3;
		   p-=bytewide;
		  }
		 }
		 else
		 {
		  switch(Level&7)
		  {
		  case 0:while(p>=p0)
				{
					(*p)|=192;
					(*(p-1))|=1;
					p-=bytewide;
				}
			 break;
		  case 1:while(p>=p0)
				{
					(*p)|=224;
					p-=bytewide;
				}
			 break;
		  case 2:while(p>=p0)
				{
					(*p)|=112;
					p-=bytewide;
				}
			 break;
		  case 3:while(p>=p0)
				{
					(*p)|=56;
					p-=bytewide;
				}
			 break;
		  case 4:while(p>=p0)
				{
					(*p)|=28;
					p-=bytewide;
				}
			 break;
		  case 5:while(p>=p0)
				{
					(*p)|=14;
					p-=bytewide;
				}
			 break;
		  case 6:while(p>=p0)
				{
					(*p)|=7;
					p-=bytewide;
				}
			 break;
		  case 7:while(p>=p0)
				{
					(*p)|=3;
					(*(p+1))|=128;
					p-=bytewide;
				}
			 break;
		  default:break;
		  }
		 }
		}
//конец алгоритма
	  }
   }
  }
  else
  {
   for(hevent=CLINE_GetFirstEvent(hline);hevent;hevent=CLINE_GetNextEvent(hevent))
   {
    data_event=CLINE_GetEventData(hevent);
    if(!data_event)
	   return FALSE;

	  Level=data_event->Lev_0;
	  hinv=CLINE_GetFirstEventInv(hevent);
	  while(hinv)
	  {
		 data_inv=CLINE_GetEventInvData(hinv);
		 if(!data_inv)
			 return FALSE;

//алгоритм
		 Beg=end_str-data_inv->Pos;
		 End=Beg-data_inv->Lent;
		 End++;

		 p0=pmasp+(End*bytewide+(Level>>3));
		 p=p0+(Beg-End)*bytewide;
		 _ASSERT(p0>=pmasp&&p<pmaspend);

		 switch(Level&7)
		 {
		 case 0:while(p>=p0)
				{
					(*p)|=192;
					(*(p-1))|=1;
					p-=bytewide;
				}
			 break;
		 case 1:while(p>=p0)
				{
					(*p)|=224;
					p-=bytewide;
				}
			 break;
		 case 2:while(p>=p0)
				{
					(*p)|=112;
					p-=bytewide;
				}
			 break;
		 case 3:while(p>=p0)
				{
					(*p)|=56;
					p-=bytewide;
				}
			 break;
		 case 4:while(p>=p0)
				{
					(*p)|=28;
					p-=bytewide;
				}
			 break;
		 case 5:while(p>=p0)
				{
					(*p)|=14;
					p-=bytewide;
				}
			 break;
		 case 6:while(p>=p0)
				{
					(*p)|=7;
					p-=bytewide;
				}
			 break;
		 case 7:while(p>=p0)
				{
					(*p)|=3;
					(*(p+1))|=128;
					p-=bytewide;
				}
			 break;
		 default:break;
		 }
//конец алгоритма
		 Level++;
		 hinv=CLINE_GetNextEventInv(hinv);
	  }
   }
  }
 }
 return TRUE;
}

/************************************************************************************************/
Bool32 DeleteDotLines(void* phCLINE, const char* ImageDelLines)
{
	CLINE_handle* pCLINE=(CLINE_handle*)phCLINE;

	CIMAGEIMAGECALLBACK         cbk;
	CIMAGEIMAGECALLBACK         cbk1;

    if (!CIMAGE_GetCallbackImage((uchar*)ImageDelLines,&cbk))
		return FALSE;

	cbk1.CIMAGE_ImageOpen  = cbk.CIMAGE_ImageOpen;
	cbk1.CIMAGE_ImageRead  = cbk.CIMAGE_ImageRead;
	cbk1.CIMAGE_ImageClose = cbk.CIMAGE_ImageClose;

	if (!CIMAGE_WriteCallbackImage((uchar*)ImageDelLines,cbk1))
	{
	    return FALSE;
	}

    Handle lpDIB;
    if(!CIMAGE_ReadDIB((uchar*)ImageDelLines,&lpDIB,1))
	{
	  CIMAGE_DeleteImage((uchar*)ImageDelLines);
	  return FALSE;
	}

    CTDIB* ctdib = new CTDIB;
	if(!ctdib)
	{
	  CIMAGE_DeleteImage((uchar*)ImageDelLines);
	  return FALSE;
	}

    ctdib->SetDIBbyPtr (lpDIB);
    const int bytewide=ctdib->GetLineWidthInBytes();
    int num_str=ctdib->GetLinesNumber ();

    uchar* pmasp=(uchar*)(ctdib->GetPtrToBitFild());
	CLINE_handle hline;

    for(hline=CLINE_GetFirstLine(*pCLINE);hline;hline=CLINE_GetNextLine(hline))
    {
        CPDLine data_line=CLINE_GetLineData(hline);
        if(data_line->Type==NR_DT_LinePointed && data_line->Dir==LD_Horiz)
    		 DelOneHorLine(pmasp,bytewide,num_str,data_line->Line.Beg_X,data_line->Line.Beg_Y,data_line->Line.End_X,data_line->Line.End_Y,data_line->Line.Wid10);
    }

    ctdib->ResetDIB ();
	delete ctdib;

    return TRUE;
}

