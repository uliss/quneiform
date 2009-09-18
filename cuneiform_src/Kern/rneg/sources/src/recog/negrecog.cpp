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
#include <stdlib.h>
#include "negrecog.h"
#include "rneg.h"
//-------------------------------------------
#include "pumadef.h"
//-------------------------------------
#include "compat_defs.h"

extern Bool dpPrintResConsole;
extern Bool dpNegResD;
extern Bool dpNegRecD;
extern Bool dpRecOneLetter;

#define TYPE_RNEG_TEMP_PHSTR CPAGE_GetInternalType("TYPE_RNEG_TEMP_PHSTR")


void NegRecog(Handle hCPage,NegList** proot,int& nRC,int skew)
{
 Word32 code_recd=512<<19;
 Word32 code_resd=512<<19+512<<16;
 CCOM_handle hNegCCOM;
 CCOM_comp * pcomp;
 Rect16* pN;
 Rect16* pRc;
 int i,j,r;
 int medium_w;
 int nN=0;
 int len_neg_mas=100;
 int add_len_mas=50;
 Bool vertical;
 char Alf[256];
 Bool nomem=FALSE;
 NegList* root=(*proot);
 NegList* now=root;
 NegList* temp;
 Word8 Prob[3];
 double neg_str_control;
 int len_result;
 double prec;
 double prob0[2], prob1[2];

 for(i=1;i<nRC;i++)
	 now=now->next;

 NegSetAlf(Alf);

 while(now)
 {
  if((now->neg).rot.pmasp)
  {
	  if((now->neg).nRc>10)
	  {
		  delete[] (now->neg).rot.hi;
		  (now->neg).rot.hi= new int[(now->neg).nRc];
	  }
	  RSELSTR_UnRotateRect(-skew,(now->neg).pRc,(now->neg).nRc,(now->neg).rot.Rc,(now->neg).rot.begx,(now->neg).rot.movey,(now->neg).rot.flmovey,(now->neg).rot.hi);
  }

  pRc=(now->neg).pRc;
  vertical=FALSE;

  if(!((pRc[(now->neg).nRc-1].bottom-pRc[0].top)*2<(pRc[0].right-pRc[0].left)))
  {
   vertical=TRUE;
  }

  for(i=(now->neg).nRc-1;i>=0;i--)
  {

   nomem=FALSE;
   len_neg_mas=100;
   nN=0;

   hNegCCOM=0;
   hNegCCOM=GetNegCCOM(hCPage,pRc,i);
   if(!hNegCCOM)
	   continue;

   if(i<MAX_STR)
	   now->neg.hCCOM[i]=hNegCCOM;

   pcomp = CCOM_GetFirst (hNegCCOM, NULL);
   if(pcomp==NULL)
   {
	  NegMoveMas((now->neg).pRc,(now->neg).nRc,i);
	  NegMoveMas(now->neg.hCCOM,now->neg.nRc,i);
	  if(i==0)
	  {
       (now->neg).p_rec/=(double)((now->neg).nRc);
	   (now->neg).p_str/=(double)((now->neg).nRc);
	   (now->neg).p=((now->neg).p_rec+(now->neg).p_str)/2.;
       if(dpPrintResConsole)
	   {
	    LDPUMA_ConsoleN("");
	    LDPUMA_Console("Негатив с вероятностью :");
	    LDPUMA_ConsoleN("");
	    LDPUMA_Console("по внутренней структуре ");
	    NegPrintConsol((now->neg).p_str);
	    LDPUMA_Console("по результатам распознования ");
	    NegPrintConsol((now->neg).p_rec);
	    LDPUMA_Console("итого ");
        NegPrintConsol((now->neg).p);
        LDPUMA_ConsoleN("площадь=%d",(int)((now->neg).pRc[0].right-(now->neg).pRc[0].left+1)*(int)((now->neg).pRc[0].bottom-(now->neg).pRc[(now->neg).nRc-1].top+1));
	   }
	   if(dpNegResD )
	   {
        Rect16 Rect;
        Rect.bottom=(now->neg).pRc[0].bottom;
		Rect.left=(now->neg).pRc[0].left;
		Rect.right=(now->neg).pRc[0].right+1;
		Rect.top=(now->neg).pRc[(now->neg).nRc-1].top-1;
		LDPUMA_DrawRect (MainWindowD, &(Rect),0,RGB(0,0,255), 1,code_resd );
        LDPUMA_WaitUserInput (NegResD,MainWindowD);
        LDPUMA_DeleteRects (MainWindowD,code_resd);
	   }
	  }
   }
   if(pcomp==NULL)
	  continue;



   if(!(InitNegMas(&pN,len_neg_mas)))
    continue;

   InitRc(pN,nN,pcomp);

   nN++;
   for(;;)
   {
    pcomp = CCOM_GetNext (pcomp, NULL);
    if (pcomp==NULL)
	 break;
    if (nN>=len_neg_mas)
    {
		 AddLenNegMas(&pN,len_neg_mas,add_len_mas);

    }
    InitRc(pN,nN,pcomp);

    nN++;
   }

   if(i>=MAX_STR)
      CCOM_DeleteContainer(hNegCCOM);

   if((now->neg).rot.pmasp)
   {
    RSELSTR_CleanStr(pN,NULL,nN,pRc[i].top,pRc[i].left,(now->neg).rot.hi[i],pRc[i].right-pRc[i].left+1,skew,FALSE);
   }
   if(!nN)
	  DelNegMas(&pN);
   if(!nN)
	   continue;

  /*Прорисовка компонент в негативе*/

   if(nN>0)
   {
    if(dpNegRecD)
    {
     for(j=0;j<nN;j++)
      NegDrawRect(MainWindowD,code_recd,RGB(0,0,255),pN[j]);
     LDPUMA_WaitUserInput (NegRecD,MainWindowD);
     if(LDPUMA_Skip (NegRecDC) )
      LDPUMA_DeleteRects (MainWindowD,code_recd);
    }
   }

   SortLetter(pN,nN,vertical);

   neg_str_control=NegStrControl(pN,nN,vertical,&(now->neg.pRc[i]));
   (now->neg).p_str+=neg_str_control;
   RecVersions* vs;
   UniVersions* uvs;

   if(!(InitNegMas(&vs,nN)))
   {
    nomem=TRUE;
    DelNegMas(&pN);
   }
   if(nomem==TRUE)
	  continue;

   if(!(InitNegMas(&uvs,nN)))
   {
    nomem=TRUE;
    DelNegMas(&pN);
    DelNegMas(vs);
   }
   if(nomem==TRUE)
	  continue;

   RecRaster rec;
   Int16 Language;
   int turn;

  if(neg_str_control/2.>inf_prob-.5)
  {
   if( !(CCOM_GetLanguage(hNegCCOM, &Language)) )
	 Language=7;
   for(j=0; j<2; j++)
   {
       prob0[j] = 0;
       prob1[j] = 0;
   }

   Bool32 ShowNegByOne = !LDPUMA_Skip(hShowNegComps);
   Handle comp_window = NULL;

   for(j=0;j<nN;j++)
   {
    if( NegGetRaster(hCPage,pN[j],&rec,vertical) )
    {
     if(!dpRecOneLetter)
	 {
//         if (vertical) TurnOverNeg(&rec);
         for(turn=0; turn<2; turn++)
         {

//-----------------------------------------------------------
            if (ShowNegByOne/* && vertical*/)
            {
               BITMAPINFOHEADER   lpBI;     // Pointer to DIB info structure
               RGBQUAD            Palette1;
               RGBQUAD            Palette2;
	           uint32_t              bfSize, dwDIBSize;
	           HANDLE             hDIB;
	           Word8*              pDIB;
	           Word8*              pTmpDIB;
	           Word8*              pTmpBuffer;
               WORD               Height, Width, ByteWidth;
               CIMAGEBITMAPINFOHEADER image_info = {0};

               Height    = (WORD)rec.lnPixHeight;
               Width     = (WORD)rec.lnPixWidth;

               ByteWidth = (Width+7)/8;
               dwDIBSize = 64 * Height;
               bfSize    = dwDIBSize + sizeof(BITMAPINFOHEADER) + 2*sizeof(RGBQUAD);

               /////////////////  Bitmap  information header   //////////////////////////////////
               lpBI.biSize           = sizeof(BITMAPINFOHEADER);
               lpBI.biWidth          = 64;
               lpBI.biHeight         = Height;
               lpBI.biPlanes         = 0x1;
               lpBI.biBitCount       = 0x1;
               lpBI.biCompression    = 0;
               lpBI.biSizeImage      = dwDIBSize;
               CIMAGE_GetImageInfo((PWord8)PUMA_IMAGE_USER, &image_info);
               lpBI.biXPelsPerMeter  = image_info.biXPelsPerMeter;
               lpBI.biYPelsPerMeter  = image_info.biYPelsPerMeter;
               lpBI.biClrUsed        = 0;
               lpBI.biClrImportant   = 0;

               ///////////////    Palette   ///////////////////////////////////////////////////////////
               Palette1.rgbBlue      = 0xFF;
               Palette1.rgbGreen     = 0xFF;
               Palette1.rgbRed       = 0xFF;
               Palette1.rgbReserved  = 0;

               Palette2.rgbBlue      = 0;
               Palette2.rgbGreen     = 0;
               Palette2.rgbRed       = 0;
               Palette2.rgbReserved  = 0;

               hDIB = calloc(1, bfSize);
 	           if (hDIB != 0)
               {
                    pDIB = static_cast<Word8*> (hDIB);
                    pTmpDIB = pDIB;

                    /////////  filling Dib   ///////////////////////////////////////////////////////////

                    memcpy(pTmpDIB, &lpBI, sizeof(BITMAPINFOHEADER));
                    pTmpDIB += sizeof(BITMAPINFOHEADER);

                    memcpy(pTmpDIB, &Palette1, sizeof(RGBQUAD));
                    pTmpDIB += sizeof(RGBQUAD);

                    memcpy(pTmpDIB, &Palette2, sizeof(RGBQUAD));
                    pTmpDIB += sizeof(RGBQUAD);

                    pTmpBuffer = rec.Raster;
					Word8* pTempDib = pTmpDIB;

                    for(int i=0; i<Height; i++ )
                    {
                        memcpy(pTmpDIB, pTmpBuffer, 8);
                        pTmpDIB += 8;
                        pTmpBuffer += 8;
                    }

					TurnOverNeg(rec, pTempDib);

                    const char *pText;
                    pText = turn ? "Component up-down" : "Component down-up";
                    comp_window = LDPUMA_CreateWindow(pText, pDIB);
//                    LDPUMA_WaitUserInput(hShowNegComps, comp_window);
                    free(hDIB);
               }
            }
//----------------------------------------------------------------------

        	  if( !(RSTR_RecogOneLetter_all (&rec,Alf, &(vs[j]),(int32_t)(1)) ) )
                Prob[1]=0;
	          else
              {
		        if (!turn) Prob[1]=vs[j].Alt[0].Prob;
                prob1[turn] += NegRecControl((int)vs[j].Alt[0].Prob);

				if (ShowNegByOne)
					LDPUMA_ConsoleN("handprint symbol: %c , probability = %d", (char)vs[j].Alt[0].Code, vs[j].Alt[0].Prob);
              }
//	  if( !(RSTR_RecogOneLetter_all (&rec,Alf, &(vs[j]),(int32_t)(2)) ) )
//            Prob[2]=0;
//	  else
//		  Prob[2]=vs[j].Alt[0].Prob;
	          if( !(RSTR_RecogOneLetter_all (&rec,Alf, &(vs[j]),(int32_t)(0)) ) )
                Prob[0]=0;
	          else
              {
		        if (!turn) Prob[0]=vs[j].Alt[0].Prob;
                prob0[turn] += NegRecControl((int)vs[j].Alt[0].Prob);

				if (ShowNegByOne)
					LDPUMA_ConsoleN("print symbol: %c , probability = %d", (char)vs[j].Alt[0].Code, vs[j].Alt[0].Prob);
              }

			 if (ShowNegByOne)
				LDPUMA_WaitUserInput(hShowNegComps, comp_window);

             if (!vertical) break;
             if (!turn) TurnRaster(&rec);
         }


         vs[j].Alt[0].Prob=(Word8)( ((int)(Prob[0])+(int)(Prob[1])/*+(int)(Prob[2])*/)/2/*3*/);


	  /*else
	  {
		  vs[j].lnAltMax =uvs[j].lnAltMax;
		  vs[j].lnAltCnt=uvs[j].lnAltCnt;
		  for(int alt=0;alt<uvs[j].lnAltCnt;alt++)
		  {
			  vs[j].Alt[alt].Code=uvs[j].Alt[alt].Code[0];
              vs[j].Alt[alt].Prob=uvs[j].Alt[alt].Prob;
		  }

	  }*/

	 }
     else
	 {

	  if( !(RSTR_RecogOneLetter (&rec, (Word8)(Language), &(vs[j])) ) )
       vs[j].Alt[0].Prob=0;

	 }
    }
    else
       vs[j].Alt[0].Prob=0;
   }
  }

   if(vertical)
	  ToHoriz(pN,nN);


   medium_w=GetMediumW(pN,nN);
   int medium_h=GetMediumH(pN,nN);

   Word8* result=NULL;
   if(!(result=new Word8[nN*2]))
   {
    nomem=TRUE;
    DelNegMas(&pN);
    DelNegMas(vs);
    DelNegMas(uvs);
   }
   if(nomem==TRUE)
	  continue;

   len_result=0;
   NegPutLetter(result,len_result,vs[0].Alt[0],FALSE);
  if(neg_str_control/2.>inf_prob-.5)
  {
   prec=NegRecControl(vs[0].Alt[0].Prob);
   for(j=1;j<nN;j++)
   {
    prec+=NegRecControl(vs[j].Alt[0].Prob);
/*    if( (pN[j].left-pN[j-1].right)>(medium_w/2)*3+2)
	{
		NegPutLetter(result,len_result,vs[j].Alt[0],TRUE);
		if( (pN[j].right-pN[j].left+1>(pN[j].bottom-pN[j].top+1)*2) && (pN[j].bottom-pN[j].top+1<medium_h/2) )
			result[len_result-1]='-';
		if( (pN[j].top<pN[j-1].top) && (pN[j].bottom-pN[j].top+1<medium_h/2) )
		{
				len_result--;
				if( IfExistI(vs[j-1]) )
				 result[len_result-1]=256+'Й';
		}
		if((pN[j].bottom-pN[j].top+1<(medium_h/3)*2)&&( (result[len_result-1]=='I')||(result[len_result-1]=='|')||((result[len_result-1]=='?') && (IfExistDef(vs[j]))) ) )
			result[len_result-1]=',';
	}
    else
	{
		NegPutLetter(result,len_result,vs[j].Alt[0],FALSE);
		if( (pN[j].right-pN[j].left+1>(pN[j].bottom-pN[j].top+1)*2) && (pN[j].bottom-pN[j].top+1<medium_h/2) )
			result[len_result-1]='-';
		if( (pN[j].top<pN[j-1].top) && (pN[j].bottom-pN[j].top+1<medium_h/2) )
		{
				len_result--;
				if( IfExistI(vs[j-1]) )
				 result[len_result-1]=256+'Й';
		}
		if((pN[j].bottom-pN[j].top+1<(medium_h/3)*2)&&( (result[len_result-1]=='I')||(result[len_result-1]=='|')||((result[len_result-1]=='?') && (IfExistDef(vs[j]))) ) )
			result[len_result-1]=',';
	}*/
   }
   r=1;
   (now->neg).p_rec+=prec/(double)(nN);
  }

/*   for(j=1;j<nN;j++)
   {

    if(result[r]!='?')
    {
	  if(result[r]!=' ')
	  {
       if(result[r]=='|')
		   result[r]='I';
	   if(Language==7)
	   {
		   if( ((result[r-1]==256+'ь')||(result[r-1]==256+'Ь'))&&((result[r]=='I' )||(result[r]=='i')||(result[r]=='1')||(result[r]=='l')) )
		   {
			   if(result[r-1]==256+'ь')
				   result[r-1]=256+'ы';
			   else
				   result[r-1]=256+'Ы';
			   NegMoveResult(result,len_result,r);
			   r--;
		   }
		   if( (result[r-1]!=' ')&&(result[r-1]!='?')&&(!(IfGl(result[r-1])))&&(result[r]==256+'Д') )
		   {
		    if(IfExistA(vs[j]))
			{
			 if(j<nN-1)
			 {
				if(!(IfGl(result[r+1])))
				result[r]=256+'А';
			 }
			 else
				 result[r]=256+'А';
			}
		   }
       }

	    if( (pN[j].right-pN[j].left+1>pN[j].bottom-pN[j].top+1) && (result[r]=='I') )
		   result[r]='-';
        r++;

	  }
      else
	  {
		  j--;
		  r++;
	  }
    }
    else
	   r++;
   }
*/
//Andrey
  (now->neg).Flags = 0;
  if (vertical)
  {
	if(dpPrintResConsole)
    {
      LDPUMA_Console("\n down-up print:");
      NegPrintConsol((double)prob0[0]/nN);
      LDPUMA_Console("down-up handprint:");
      NegPrintConsol((double)prob1[0]/nN);
      LDPUMA_Console("up-down print:");
      NegPrintConsol((double)prob0[1]/nN);
      LDPUMA_Console("up-down handprint:");
      NegPrintConsol((double)prob1[1]/nN);
    }
      double down_up = (prob0[0]+prob1[0])/2;
      double up_down = (prob0[1]+prob1[1])/2;
      (now->neg).Flags |= FlVert;
      if (down_up > up_down) (now->neg).Flags |= FlDown2Up;
  }

  if(i==(now->neg).nRc-1)
   {
	if(dpPrintResConsole)
	  NegPrintConsol(pRc[0].left,pRc[(now->neg).nRc-1].top,pRc[0].right,pRc[0].bottom);
   }

   if(dpPrintResConsole)
    NegPrintConsol(result,len_result);

   if(i==0)
   {
	(now->neg).p_rec/=(double)((now->neg).nRc);
	(now->neg).p_str/=(double)((now->neg).nRc);
	(now->neg).p=((now->neg).p_rec+(now->neg).p_str)/2.;
    if(dpPrintResConsole)
	{
	  LDPUMA_ConsoleN("");
	  LDPUMA_Console("Негатив с вероятностью :");
	  LDPUMA_ConsoleN("");
	  LDPUMA_Console("по внутренней структуре ");
	  NegPrintConsol((now->neg).p_str);
	  LDPUMA_Console("по результатам распознования ");
	  NegPrintConsol((now->neg).p_rec);
	  LDPUMA_Console("итого ");
      NegPrintConsol((now->neg).p);
      LDPUMA_ConsoleN("площадь=%d",(int)((now->neg).pRc[0].right-(now->neg).pRc[0].left+1)*(int)((now->neg).pRc[0].bottom-(now->neg).pRc[(now->neg).nRc-1].top+1));
	}
   }

   if(i==0)
   {
    if(dpNegResD)
    {
        Rect16 Rect;
        Rect.bottom=(now->neg).pRc[0].bottom;
		Rect.left=(now->neg).pRc[0].left;
		Rect.right=(now->neg).pRc[0].right+1;
		Rect.top=(now->neg).pRc[(now->neg).nRc-1].top-1;
		LDPUMA_DrawRect (MainWindowD, &(Rect),0,RGB(0,0,255), 1,code_resd );
        LDPUMA_WaitUserInput (NegResD,MainWindowD);
        LDPUMA_DeleteRects (MainWindowD,code_resd);
    }
   }

   DelNegMas(vs);
   DelNegMas(uvs);
   delete[] result;
   DelNegMas(&pN);
  }
//  if((now->neg).rot.pmasp)
//   DeleteRotateMas(&((now->neg).rot.pmasp),&((now->neg).rot.begx),&((now->neg).rot.movey),&((now->neg).rot.flmovey),&((now->neg).rot.hi));
  if(!(now->neg).nRc)
  {
	  temp=now;
	  if(!temp->prev)
		  root=temp->next;
       now=now->prev;
	   delete temp;
	   nRC--;
  }
  else
   now=now->prev;
 }

 (*proot)=root;

}


int NegGetCountOfStr(Rect16* pRc,int num)
{
	int i;
	for(i=num-1;i>=0;i--)
	{
		if((pRc[i].left!=pRc[num].left)||(pRc[i].right!=pRc[num].right))
			break;
		if(pRc[i+1].bottom+1!=pRc[i].top)
			break;
	}
	return num-i;
}

double NegRecControl(int p)
{
	if(p>200)
		return 1.+(double)(p-255)/540.;
	if((p>=100)&&(p<=200))
		return .9+(double)(p-200)/250.;
	return .5+(double)(p-99)/99./2.;
}

double NegStrControl(Rect16* pRc,int n,Bool vertical,Rect16* pRect)
{
 double res;
 int count_pogr=0;
 const int delta=3;
 if(vertical)
 {
	if(pRc->bottom>=pRect->bottom-1)
		count_pogr++;
	if(pRc[n-1].top<=pRect->top+1)
		count_pogr++;
	for(int i=1;i<n;i++)
	{
		if((pRc[i].bottom-delta>pRc[i-1].top)&&!((pRc[i].right<pRc[i-1].left)&&(pRc[i].right-pRc[i].left<(pRc[i-1].right-pRc[i-1].left)/3)  ) )
			count_pogr++;
		else
		{
		 if((pRc[i].left>pRc[i-1].right)&&!((i>1)&&(pRc[i-1].right<pRc[i-2].left)&&(pRc[i-1].right-pRc[i-1].left<(pRc[i-2].right-pRc[i-2].left)/3)))
			count_pogr++;
		 else
		 {
		  if(pRc[i].right<pRc[i-1].left)
		  {
			if(!(pRc[i].right-pRc[i].left<(pRc[i-1].right-pRc[i-1].left)/3))
			{
				if(!((i>1)&&(pRc[i].right-pRc[i].left<(pRc[i-2].right-pRc[i-2].left)/3)))
					count_pogr++;
			}
		  }
		 }
		}
	}
 }
 else
 {
    const int up=pRect->top;//+1;
    const int down=pRect->bottom+2;//-1;
	if(pRc->top<=up || pRc->bottom>=down)
		count_pogr++;
	for(int i=1;i<n;i++)
	{
		if((pRc[i].left+delta<pRc[i-1].right)&&!((pRc[i].bottom<pRc[i-1].top)&&(pRc[i].bottom-pRc[i].top<(pRc[i-1].bottom-pRc[i-1].top)/3)  )|| pRc[i].top<=up || pRc[i].bottom>=down )
			count_pogr++;
		else
		{
		 if((pRc[i].top>pRc[i-1].bottom)&&!((i>1)&&(pRc[i-1].bottom<pRc[i-2].top)&&(pRc[i-1].bottom-pRc[i-1].top<(pRc[i-2].bottom-pRc[i-2].top)/3)))
			count_pogr++;
		 else
		 {
		  if(pRc[i].bottom<pRc[i-1].top)
		  {
			if(!(pRc[i].bottom-pRc[i].top<(pRc[i-1].bottom-pRc[i-1].top)/3))
			{
				if(!((i>1)&&(pRc[i].bottom-pRc[i].top<(pRc[i-2].bottom-pRc[i-2].top)/3)))
					count_pogr++;
			}
		  }
		 }
		}
	}
 }
 if(count_pogr>n-1)
	 return .0;
 if(n>1)
	 res=1.-(double)(count_pogr)/(double)(n-1);
 else
	 res=.5;
 if((CountLetter(pRc,n,vertical)-count_pogr<3)&&(res>.5))
	 return .5;
 return res;
}

int CountLetter(Rect16* pRc,int n,Bool vertical)
{
 int count=0;
 const int DY=10*(DPIY+1)/300-1;
 const int DX=3*(DPIX+1)/300-1;

 if(vertical)
 {
	for(int i=n;i>=0;i--)
	{
		if((pRc[i].bottom-pRc[i].top>DX)&&(pRc[i].right-pRc[i].left>DY))
			count++;
	}
 }
 else
 {
	for(int i=n;i>=0;i--)
	{
		if((pRc[i].bottom-pRc[i].top>DY)&&(pRc[i].right-pRc[i].left>DX))
			count++;
	}
 }
	return count;
}


void ToHoriz(Rect16* pRc,int nRc)
{
	int oldtop,oldbot;
	int bot=pRc[0].bottom;
	for(int i=0;i<nRc;i++)
	{
		oldtop=pRc[i].top;
		oldbot=pRc[i].bottom;
		pRc[i].top=pRc[i].left;
        pRc[i].bottom=pRc[i].right;
		pRc[i].left=bot-oldbot;
		pRc[i].right=bot-oldtop;
	}
}

Bool IfBadResult(Word8* result,int n)
{
 int i;
 int count_q=0;
 int count_letter=0;
 double proc;

 for(i=0;i<n;i++)
 {
	 if(result[i]!=' ')
	 {
		 count_letter++;
		 if(result[i]=='?')
			 count_q++;
	 }
 }
 proc=((double)(count_q))/((double)(count_letter));
 if(proc<.5)
	 return FALSE;
 return TRUE;
}

Bool IfExistDef(RecVersions vs)
{
	int i;
	for(i=0;i<vs.lnAltCnt;i++)
	{
		if( (vs.Alt[i].Code=='I')||(vs.Alt[i].Code=='|') )
			return TRUE;
	}
	return FALSE;
}

Bool IfExistI(RecVersions vs)
{
	int i;
	for(i=0;i<vs.lnAltCnt;i++)
	{
		if( vs.Alt[i].Code==256+'И' )
			return TRUE;
	}
	return FALSE;
}

Bool IfGl(Word8 c)
{
	if( (c==256+'А')||(c==256+'Е')||(c==256+'У')||(c==256+'Ы')||(c==256+'О')||(c==256+'Э')||(c==256+'Я')||(c==256+'И')||(c==256+'Ю') )
		return TRUE;
	else
		return FALSE;
}

Bool IfExistA(RecVersions vs)
{
	int i;
	for(i=0;i<vs.lnAltCnt;i++)
	{
		if( (vs.Alt[i].Code==256+'А')&&(vs.Alt[0].Prob-vs.Alt[i].Prob<50) )
			return TRUE;
	}
	return FALSE;
}
void NegPrintConsol(Word8* result,int len)
{
  int k;

  for(k=0;k<len;k++)
  {
    if(result[k]!=0)
		LDPUMA_Console("%c",result[k]);
	else
        LDPUMA_Console("?");
  }


}

void NegPrintConsol(int left,int top,int right,int bottom)
{

  LDPUMA_ConsoleN("");
  LDPUMA_Console(" (%d,",left);
  LDPUMA_Console("%d)",top);
  LDPUMA_Console(" (%d,",right);
  LDPUMA_Console("%d): ",bottom);

}


void NegPrintConsol(double p)
{
 int i;

  if((int)(p)==1)
  {
	  LDPUMA_ConsoleN("100%%");
	  return;
  }
      i=(int)(p*100.);
	  if((int)(p*1000.)-10*i>4)
	    i++;
  LDPUMA_ConsoleN("%d%%",i);
}

void NegMoveResult(Word8* result,int& len,int num)
{
 int i;
 for(i=num;i<len-1;i++)
	 result[i]=result[i+1];
 len--;
}



void NegPutLetter(Word8* result,int& len_result,RecAlt Alt,Bool Flag)
{
  if(Flag==TRUE)
  {
   result[len_result]=' ';
   len_result++;
  }

    if(Alt.Prob>50)
	{
      result[len_result]=Alt.Code;
      len_result++;
	}
    else
	{
	 result[len_result]='?';
     len_result++;
	}
}

int GetMediumW(Rect16* pN,int n)
{
 int medium=0,i=0,oldr;
 oldr=pN[i].right;
 for(i=1;i<n;i++)
 {
	 medium+=(pN[i].left-oldr);
	 oldr=pN[i].right;
 }
 if(n>1)
 return (medium/(n-1));
 else
	 return 0;
}


int GetMediumH(Rect16* pN,int n)
{
 int medium=0,i=0;

 for(i=0;i<n;i++)
 {
	 medium+=(pN[i].bottom-pN[i].top+1);

 }
 return medium/n;
}


Bool NegGetRaster(Handle hCPage,Rect16 N,RecRaster* rec,Bool vert)
{
 int i,j,k;
 Word8 Data[1000];
 Word8 bytep;
 int h=N.bottom-N.top+1;
 int w=N.right-N.left+1;
 int bytewide=(w)/8;
 if( ((w)%8) != 0)
	 bytewide++;
 int vbytewide=(h)/8;
 if( ((h)%8) != 0)
	 vbytewide++;
 Word8* pmasp;
 memset (Data, 0, sizeof (Data));
 pmasp=Data;
 if( !(GetMasP(&N,&pmasp)) )
	 return FALSE;
 if( ((bytewide>8)&&(!vert))||((vbytewide>8)&&vert) )
 {
	 if(!vert)
	 {
		 while(bytewide>8)
		 {
			 NegAr2(pmasp,h,w);
			 w=w/2;
			 h=h/2;
			 bytewide=(w)/8;
             if( ((w)%8) != 0)
	            bytewide++;
		 }
	 }
	 else
	 {
		 while(vbytewide>8)
		 {
			 NegAr2(pmasp,h,w);
			 w=w/2;
			 h=h/2;
			 vbytewide=(h)/8;
             if( ((h)%8) != 0)
	            vbytewide++;
		 }
	 }
 }
 Word8* pin;
 Word8* pfrom;
 Word8* pend;
 if( ((8*h> REC_MAX_RASTER_SIZE)&&(!vert))||((8*w> REC_MAX_RASTER_SIZE)&&vert) )
	 return FALSE;
 if(!vert)
 {
  pin=(*rec).Raster;
  pfrom=pmasp;
  pend=pmasp+(h*bytewide);
  switch(8-bytewide)
  {
   case 0:
	  while(pfrom<pend)
	  {
	   (*pin)=(*pfrom);pin++;pfrom++;
	   (*pin)=(*pfrom);pin++;pfrom++;
	   (*pin)=(*pfrom);pin++;pfrom++;
	   (*pin)=(*pfrom);pin++;pfrom++;
	   (*pin)=(*pfrom);pin++;pfrom++;
	   (*pin)=(*pfrom);pin++;pfrom++;
	   (*pin)=(*pfrom);pin++;pfrom++;
	   (*pin)=(*pfrom);pin++;pfrom++;
	  }
	   break;
   case 1:
	  while(pfrom<pend)
	  {
	   (*pin)=(*pfrom);pin++;pfrom++;
	   (*pin)=(*pfrom);pin++;pfrom++;
	   (*pin)=(*pfrom);pin++;pfrom++;
	   (*pin)=(*pfrom);pin++;pfrom++;
	   (*pin)=(*pfrom);pin++;pfrom++;
	   (*pin)=(*pfrom);pin++;pfrom++;
	   (*pin)=(*pfrom);pin++;pfrom++;
	   (*pin)=0;pin++;
	  }
	   break;
   case 2:
	  while(pfrom<pend)
	  {
	   (*pin)=(*pfrom);pin++;pfrom++;
	   (*pin)=(*pfrom);pin++;pfrom++;
	   (*pin)=(*pfrom);pin++;pfrom++;
	   (*pin)=(*pfrom);pin++;pfrom++;
	   (*pin)=(*pfrom);pin++;pfrom++;
	   (*pin)=(*pfrom);pin++;pfrom++;
	   (*pin)=0;pin++;
	   (*pin)=0;pin++;
	  }
	   break;
   case 3:
	  while(pfrom<pend)
	  {
	   (*pin)=(*pfrom);pin++;pfrom++;
	   (*pin)=(*pfrom);pin++;pfrom++;
	   (*pin)=(*pfrom);pin++;pfrom++;
	   (*pin)=(*pfrom);pin++;pfrom++;
	   (*pin)=(*pfrom);pin++;pfrom++;
	   (*pin)=0;pin++;
	   (*pin)=0;pin++;
	   (*pin)=0;pin++;
	  }
	   break;
   case 4:
	  while(pfrom<pend)
	  {
	   (*pin)=(*pfrom);pin++;pfrom++;
	   (*pin)=(*pfrom);pin++;pfrom++;
	   (*pin)=(*pfrom);pin++;pfrom++;
	   (*pin)=(*pfrom);pin++;pfrom++;
	   (*pin)=0;pin++;
	   (*pin)=0;pin++;
	   (*pin)=0;pin++;
	   (*pin)=0;pin++;
	  }
	   break;
   case 5:
	  while(pfrom<pend)
	  {
	   (*pin)=(*pfrom);pin++;pfrom++;
	   (*pin)=(*pfrom);pin++;pfrom++;
	   (*pin)=(*pfrom);pin++;pfrom++;
	   (*pin)=0;pin++;
	   (*pin)=0;pin++;
	   (*pin)=0;pin++;
	   (*pin)=0;pin++;
	   (*pin)=0;pin++;
	  }
	   break;
   case 6:
	  while(pfrom<pend)
	  {
	   (*pin)=(*pfrom);pin++;pfrom++;
	   (*pin)=(*pfrom);pin++;pfrom++;
	   (*pin)=0;pin++;
	   (*pin)=0;pin++;
	   (*pin)=0;pin++;
	   (*pin)=0;pin++;
	   (*pin)=0;pin++;
	   (*pin)=0;pin++;
	  }
	   break;
   case 7:
	  while(pfrom<pend)
	  {
	   (*pin)=(*pfrom);pin++;pfrom++;
	   (*pin)=0;pin++;
	   (*pin)=0;pin++;
	   (*pin)=0;pin++;
	   (*pin)=0;pin++;
	   (*pin)=0;pin++;
	   (*pin)=0;pin++;
	   (*pin)=0;pin++;
	  }
	   break;
   default:break;
  }
  (*rec).lnPixHeight=h;
  (*rec).lnPixWidth=w;
  (*rec).lnRasterBufSize=REC_MAX_RASTER_SIZE;
 }
 else
 {
  int rectj;
  int delj;
  int nowbyte;
  int nowbyte2;
  for(j=0;j<w;j++)
  {
   nowbyte=j<<3;
   rectj=j%8;
   delj=j>>3;
   for(i=vbytewide;i<8;i++)
	 (*rec).Raster[nowbyte+i]=0;
   for(i=0;i<vbytewide-1;i++)
   {
	bytep=0;
	nowbyte2=h-1-(i<<3);

	   bytep|=( ( (128>>rectj) & pmasp[bytewide*(nowbyte2)+delj] )<<rectj );
	   bytep|=( ( (128>>rectj) & pmasp[bytewide*(nowbyte2-1)+delj] )<<rectj )>>1;
       bytep|=( ( (128>>rectj) & pmasp[bytewide*(nowbyte2-2)+delj] )<<rectj )>>2;
	   bytep|=( ( (128>>rectj) & pmasp[bytewide*(nowbyte2-3)+delj] )<<rectj )>>3;
	   bytep|=( ( (128>>rectj) & pmasp[bytewide*(nowbyte2-4)+delj] )<<rectj )>>4;
	   bytep|=( ( (128>>rectj) & pmasp[bytewide*(nowbyte2-5)+delj] )<<rectj )>>5;
       bytep|=( ( (128>>rectj) & pmasp[bytewide*(nowbyte2-6)+delj] )<<rectj )>>6;
	   bytep|=( ( (128>>rectj) & pmasp[bytewide*(nowbyte2-7)+delj] )<<rectj )>>7;

    (*rec).Raster[nowbyte+i]=bytep;
   }
   bytep=0;
   nowbyte2=h-1-(i<<3);
   for(k=0;k<h-((vbytewide-1)<<3);k++)
	   bytep|=( ( (128>>rectj) & pmasp[bytewide*(nowbyte2-k)+delj] )<<rectj )>>k;
	(*rec).Raster[nowbyte+i]=bytep;
  }
  (*rec).lnPixHeight=w;
  (*rec).lnPixWidth=h;
  (*rec).lnRasterBufSize=REC_MAX_RASTER_SIZE;
 }
 return TRUE;
}

void TurnOverNeg(RecRaster rec, Word8* Raster)
{
//   int wide = (*rec).lnPixWidth;
    int hwide = rec.lnPixHeight;
    int bytewide = 8;
    for(int i=0; i<hwide/2; i++)
        for(int j=0; j<bytewide; j++)
        {
            Word8 temp = Raster[i*bytewide+j];
            Raster[i*bytewide+j] = Raster[(hwide-i-1)*bytewide+j];
            Raster[(hwide-i-1)*bytewide+j] = temp;
        }
}

void TurnRaster(RecRaster* rec)
{
    int wideinp = (*rec).lnPixWidth;
    int wide = wideinp%8 ? (wideinp>>3)+1 : wideinp>>3;
    int hwide = (*rec).lnPixHeight;
    int bytewide = 8;
    //переворачиваем растр вокруг горизонтальной оси
    TurnOverNeg(*rec, rec->Raster);

    //переворачиваем растр вокруг вертикальной оси
    int i(0);
    int j(0);
    for(i=0; i<bytewide/2; i++)
        for(j=0; j<hwide; j++)
        {
            Word8 temp1 = (*rec).Raster[j*bytewide+i];
            Word8 temp2 = (*rec).Raster[j*bytewide+bytewide-i-1];
            Word8 rot1 = 0, rot2 = 0;
            for(int k=0; k<4; k++)
            {
                rot1 |= (temp1&(1<<k))<<(7-k*2);
                rot2 |= (temp2&(1<<k))<<(7-k*2);
                rot1 |= (temp1&(1<<(7-k)))>>(7-k*2);
                rot2 |= (temp2&(1<<(7-k)))>>(7-k*2);
            }
            (*rec).Raster[j*bytewide+i] = rot2;
            (*rec).Raster[j*bytewide+bytewide-i-1] = rot1;
        }
    //сдвиг влево
    int rest = wide*8-wideinp-2;
    for(i=0; i<hwide; i++)
    {
        for(j=0; j<wide; j++) (*rec).Raster[i*bytewide+j] = (*rec).Raster[i*bytewide+j+bytewide-wide];
        for(j=wide; j<bytewide; j++) (*rec).Raster[i*bytewide+j] = 0;
        if (rest > 0)
            for(int j=0; j<wide; j++)
            {
                Word8 t = (*rec).Raster[i*bytewide+j];
                Word8 t1 = (*rec).Raster[i*bytewide+j+1];
                Word8 res = 0;
                for(int k=0; k<8; k++)
                {
                    if (7-rest-k >= 0) res |= (t&(1<<(7-k-rest)))<<rest;
                    else res |= (t1&(1<<(15-rest-k)))>>(8-rest);
                }
                (*rec).Raster[i*bytewide+j] = res;
            }
    }
}

void NegAr2(Word8* pmasp,int h,int w)
{
 int i,j;
 Word8 bytep;
 int newh=h/2;
 int neww=w/2;
 int newbytewide=(neww)/8;
 if( ((neww)%8) != 0)
	 newbytewide++;
 int bytewide=(w)/8;
 if( ((w)%8) != 0)
	 bytewide++;
 int nowbyte;
 int nowbyte2;
 for(i=0;i<h;i++)
 {
  nowbyte=i*bytewide;
  nowbyte2=i*newbytewide;
  for(j=0;j<bytewide>>1;j++)
  {
   bytep=0;

     bytep|=(128)&(pmasp[nowbyte+(j<<1)]|(pmasp[nowbyte+(j<<1) ]<<1));
     bytep|=(64)&((pmasp[nowbyte+(j<<1)]<<1)|(pmasp[nowbyte+(j<<1) ]<<2));
     bytep|=(32)&((pmasp[nowbyte+(j<<1)]<<2)|(pmasp[nowbyte+(j<<1) ]<<3));
     bytep|=(16)&((pmasp[nowbyte+(j<<1)]<<3)|(pmasp[nowbyte+(j<<1) ]<<4));


	 bytep|=(8)&((pmasp[nowbyte+(j<<1)+1]>>4)|(pmasp[nowbyte+(j<<1)+1]>>3));
	 bytep|=(4)&((pmasp[nowbyte+(j<<1)+1]>>3)|(pmasp[nowbyte+(j<<1)+1]>>2));
	 bytep|=(2)&((pmasp[nowbyte+(j<<1)+1]>>2)|(pmasp[nowbyte+(j<<1)+1]>>2));
	 bytep|=(1)&((pmasp[nowbyte+(j<<1)+1]>>1)|pmasp[nowbyte+(j<<1)+1]);

   pmasp[nowbyte2+j]=bytep;
  }
  if(j<newbytewide)
   pmasp[nowbyte2+j]=pmasp[nowbyte+bytewide-1];
 }

 for(i=0;i<h/2;i++)
 {
	 nowbyte=i*newbytewide;
     nowbyte2=(i*newbytewide)<<1;
	 for(j=0;j<newbytewide;j++)
		 pmasp[nowbyte+j]=pmasp[nowbyte2+j]|pmasp[nowbyte2+newbytewide+j];

 }
}



void NegInvert(RecRaster* rec,Rect16 N)
{
 int i,j;
 int h=N.bottom-N.top+1;

 int bytewide=8;
 j=h*bytewide-1;

  for(i=0;i<j;i++)
	(*rec).Raster[i]=~( (*rec).Raster[i] );

}



Handle GetNegCCOM(Handle hCPage,Rect16* pRc,int i)
{
 int min_h,min_w,max_h,max_w;
 ExcControl Control;
 int j;
 Word8 Name[CPAGE_MAXNAME];
 for (j=0; j<CPAGE_MAXNAME; j++)
		Name[j] = ImageName[j];
 Handle lpDIB;
 if(!CIMAGE_ReadDIB(Name,&lpDIB,1))
	 return 0;
 if( pRc[i].bottom-pRc[i].top > pRc[i].right-pRc[i].left )
 {
	  min_h=min_let_w;
	  min_w=min_let_h;
	  max_w=pRc[i].right-pRc[i].left+1;
	  max_h=max_w*2;

 }
 else
 {
      min_h=min_let_h;
	  min_w=min_let_w;
	  max_h=pRc[i].bottom-pRc[i].top+1;
	  max_w=max_h*2;
 }
 Control.MinCompHei=min_h;
 Control.MinCompWid=min_w;
 Control.MaxCompHei=max_h;
 Control.MaxCompWid=max_w;
 Control.MaxScale=1;
 Control.Control=Ex_ExtraComp|Ex_DisableCut|Ex_Invert;

 if(REXCExtraDIB(Control,(Word8*)(lpDIB),pRc[i].left,pRc[i].top,pRc[i].right-pRc[i].left+1,pRc[i].bottom-pRc[i].top+1))
    return REXCGetContainer();
 return 0;
/*
 Word8* pmasp;
 if(!GetMasP(&(pRc[i]),&pmasp))
	 return 0;

 if( pRc[i].bottom-pRc[i].top > pRc[i].right-pRc[i].left )
 {
	  min_h=min_let_w;
	  min_w=min_let_h;
	  max_w=pRc[i].right-pRc[i].left+1;
	  max_h=max_w*2;

 }
 else
 {
      min_h=min_let_h;
	  min_w=min_let_w;
	  max_h=pRc[i].bottom-pRc[i].top+1;
	  max_w=max_h*2;
 }

 int bytewide=(pRc[i].right-pRc[i].left+8)/8;
 Bool fotomet=FALSE;
 Bool RevOv=TRUE;

 Control.MinCompHei=min_h;
 Control.MinCompWid=min_w;
 Control.MaxCompHei=max_h+1;
 Control.MaxCompWid=bytewide*8;
 Control.MaxScale=0;
 Control.Control=Ex_ExtraComp|Ex_EvnRecog;


 if(REXCExtra(Control,pmasp,bytewide,RevOv,bytewide*8,pRc[i].bottom-pRc[i].top+1,(DPIX*10000)/254,(DPIY*10000)/254,0,0,0,0,fotomet))
    return REXCGetContainer();
 return 0;
 */
}

void SortLetter(Rect16 *pRc,int n,Bool vert)
{
 Int16 left;
 Int16 right;
 Int16 top;
 Int16 bottom;
 int j,i;

 if(vert)
 {
  for(i=n-1 ; i>0 ; i--)
  {
   for(j=1 ; j<=i ; j++)
   {

	  if(pRc[j-1].bottom<pRc[j].bottom)
	  {
		  bottom=pRc[j].bottom;
		  left=pRc[j].left;
		  right=pRc[j].right;
		  top=pRc[j].top;
		  pRc[j].bottom=pRc[j-1].bottom;
		  pRc[j].left=pRc[j-1].left;
		  pRc[j].right=pRc[j-1].right;
		  pRc[j].top=pRc[j-1].top;
		  pRc[j-1].bottom=bottom;
		  pRc[j-1].left=left;
		  pRc[j-1].right=right;
		  pRc[j-1].top=top;
	  }
   }
  }
 }
 else
 {
  for(i=n-1 ; i>0 ; i--)
  {
   for(j=1 ; j<=i ; j++)
   {

	  if(pRc[j-1].left>pRc[j].left)
	  {
		  bottom=pRc[j].bottom;
		  left=pRc[j].left;
		  right=pRc[j].right;
		  top=pRc[j].top;
		  pRc[j].bottom=pRc[j-1].bottom;
		  pRc[j].left=pRc[j-1].left;
		  pRc[j].right=pRc[j-1].right;
		  pRc[j].top=pRc[j-1].top;
		  pRc[j-1].bottom=bottom;
		  pRc[j-1].left=left;
		  pRc[j-1].right=right;
		  pRc[j-1].top=top;
	  }
   }
  }
 }
}

void NegDrawRect(Handle wnd,Word32 OperCode,Word32 color,Rect16 Rc)
{

	Rect16 Rect;

		Rect.bottom=Rc.bottom;
		Rect.left=Rc.left;
		Rect.right=Rc.right+1;
		Rect.top=Rc.top-1;
		LDPUMA_DrawRect (wnd, &(Rect),0, color, 1,OperCode );


}

void NegPrintInFile(FILE* f,NegList* root,int nRC)
{
 int dob;
 int k;
 char* name=LDPUMA_GetFileName (NULL);
 fprintf(f,"%s\n",name);
 while(root)
 {
  fprintf(f,"(%d,%d),(%d,%d): ",(root->neg).pRc[0].left,(root->neg).pRc[(root->neg).nRc-1].top,(root->neg).pRc[0].right,(root->neg).pRc[0].bottom);
  dob=20-NegCountNumbers((root->neg).pRc[0].left)-NegCountNumbers((root->neg).pRc[0].right);
  dob-=NegCountNumbers((root->neg).pRc[0].bottom)+NegCountNumbers((root->neg).pRc[(root->neg).nRc-1].top);
  for(k=0;k<dob;k++)
	  fprintf(f," ");
  if((root->neg).pRc[0].bottom-(root->neg).pRc[(root->neg).nRc-1].top>(root->neg).pRc[0].right-(root->neg).pRc[0].left)
	  fprintf(f,"vertical ");
  else
	  fprintf(f,"horizont ");
   k=(int)((root->neg).p*100.);
	  if((int)((root->neg).p*1000.)-10*k>4)
	    k++;
  fprintf(f,"%d%%\n",k);
  root=root->next;

 }

}

int NegCountNumbers(int num)
{
	int count=0;
	if(num==0)
		return 1;
	for(num;num>0;num=num/10)
		count++;
	return count;
}

void NegSetAlf(char* buf)
{
	int size=0,i;
	for(i=192;i<256;i++)
	{
		buf[size]=i;
		size++;
	}
	for(i=65;i<91;i++)
	{
		buf[size]=i;
		size++;
	}
	for(i=97;i<123;i++)
	{
		buf[size]=i;
		size++;
	}
	for(i=48;i<58;i++)
	{
		buf[size]=i;
		size++;
	}
	buf[size]=40;
		size++;
	buf[size]=41;
		size++;
    buf[size]=47;
		size++;
    buf[size]=92;
		size++;
    for(size;size<255;size++)
		buf[size]=97;
	buf[size]='\0';
}

void NegMoveMas(Handle* hCCOM,int n,int num)
{
 int i;
 CCOM_DeleteContainer(hCCOM[num]);

  for(i=num;i<n-1;i++)
  {
	  hCCOM[i]=hCCOM[i+1];
  }
  hCCOM[n-1]=0;
}

void PutTempToCPAGE(Handle hCPage,NegList* root)
{
 NegList* now;
 NegTemp temp;
 Bool fl_cont;
 int size_phstr=sizeof(NegTemp);

 for(now=root;now;now=now->next)
 {
   fl_cont=FALSE;
   if((now->neg).p<=inf_prob)
	   continue;
   int i(0);
   for(i=1;i<now->neg.nRc;i++)
   {
	if(!UnifCont(now->neg.hCCOM[0],now->neg.hCCOM[i]))
		fl_cont=TRUE;
   }
   if(fl_cont)
	   continue;

  if(now->neg.nRc>Max_Str_Count)
	 continue;
  else
	  temp.nRc=now->neg.nRc;

  if(now->neg.rot.pmasp)
  {
	  temp.fl_rotate=TRUE;
	  for(int i=temp.nRc;i>=0;i--)
		 temp.phi[i]=now->neg.rot.hi[i];
  }
  else
	  temp.fl_rotate=FALSE;

  for(i=temp.nRc;i>=0;i--)
  {
	 temp.prc[i].bottom=now->neg.pRc[i].bottom;
	 temp.prc[i].top=now->neg.pRc[i].top;
	 temp.prc[i].left=now->neg.pRc[i].left;
	 temp.prc[i].right=now->neg.pRc[i].right;
  }

  temp.top=now->neg.pRc[now->neg.nRc-1].top;
  temp.oldleft=temp.left;
  temp.h=now->neg.pRc[0].bottom-temp.top+1;
  temp.left=now->neg.pRc[0].left;
  temp.w=now->neg.pRc[0].right-temp.left+1;

  if(temp.h>temp.w)
	  temp.vertical=TRUE;
  else
	  temp.vertical=FALSE;

  temp.hStrCCOM=now->neg.hCCOM[0];

  if(CPAGE_CreateBlock(hCPage,TYPE_RNEG_TEMP_PHSTR,0,0,&temp,size_phstr))
	  now->neg.hCCOM[0]=0;
 }
}

Bool UnifCont(CCOM_handle to,CCOM_handle from)
{
 if(!to||!from)
	return FALSE;

 CCOM_comp* comp;
 CCOM_comp* new_comp;

 for(comp=CCOM_GetFirst(from,NULL);comp;comp=CCOM_GetNext(comp,NULL))
 {
	 new_comp=CCOM_New(to,comp->upper,comp->left,comp->w,comp->h);
	 if(!new_comp)
		 return FALSE;
	 if(!CCOM_Copy(new_comp,comp))
		 return FALSE;
 }
 return TRUE;
}
