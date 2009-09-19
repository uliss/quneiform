/*
Copyright (c) 1993-2008, Cognitive Technologies
All rights reserved.

пЮГПЕЬЮЕРЯЪ ОНБРНПМНЕ ПЮЯОПНЯРПЮМЕМХЕ Х ХЯОНКЭГНБЮМХЕ ЙЮЙ Б БХДЕ ХЯУНДМНЦН ЙНДЮ,
РЮЙ Х Б ДБНХВМНИ ТНПЛЕ, Я ХГЛЕМЕМХЪЛХ ХКХ АЕГ, ОПХ ЯНАКЧДЕМХХ ЯКЕДСЧЫХУ СЯКНБХИ:

      * оПХ ОНБРНПМНЛ ПЮЯОПНЯРПЮМЕМХХ ХЯУНДМНЦН ЙНДЮ ДНКФМШ НЯРЮБЮРЭЯЪ СЙЮГЮММНЕ
        БШЬЕ СБЕДНЛКЕМХЕ НА ЮБРНПЯЙНЛ ОПЮБЕ, ЩРНР ЯОХЯНЙ СЯКНБХИ Х ОНЯКЕДСЧЫХИ
        НРЙЮГ НР ЦЮПЮМРХИ.
      * оПХ ОНБРНПМНЛ ПЮЯОПНЯРПЮМЕМХХ ДБНХВМНЦН ЙНДЮ Б ДНЙСЛЕМРЮЖХХ Х/ХКХ Б
        ДПСЦХУ ЛЮРЕПХЮКЮУ, ОНЯРЮБКЪЕЛШУ ОПХ ПЮЯОПНЯРПЮМЕМХХ, ДНКФМШ ЯНУПЮМЪРЭЯЪ
        СЙЮГЮММЮЪ БШЬЕ ХМТНПЛЮЖХЪ НА ЮБРНПЯЙНЛ ОПЮБЕ, ЩРНР ЯОХЯНЙ СЯКНБХИ Х
        ОНЯКЕДСЧЫХИ НРЙЮГ НР ЦЮПЮМРХИ.
      * мХ МЮГБЮМХЕ Cognitive Technologies, МХ ХЛЕМЮ ЕЕ ЯНРПСДМХЙНБ МЕ ЛНЦСР
        АШРЭ ХЯОНКЭГНБЮМШ Б ЙЮВЕЯРБЕ ЯПЕДЯРБЮ ОНДДЕПФЙХ Х/ХКХ ОПНДБХФЕМХЪ
        ОПНДСЙРНБ, НЯМНБЮММШУ МЮ ЩРНЛ он, АЕГ ОПЕДБЮПХРЕКЭМНЦН ОХЯЭЛЕММНЦН
        ПЮГПЕЬЕМХЪ.

щрю опнцпюллю опеднярюбкемю бкюдекэжюлх юбрнпяйху опюб х/хкх дпсцхлх кхжюлх "йюй
нмю еярэ" аег йюйнцн-кхан бхдю цюпюмрхи, бшпюфеммшу ъбмн хкх ондпюгслебюелшу,
бйкчвюъ цюпюмрхх йнллепвеяйни жеммнярх х опхцндмнярх дкъ йнмйпермни жекх, мн ме
нцпюмхвхбюъяэ хлх. мх бкюдекеж юбрнпяйху опюб х мх ндмн дпсцне кхжн, йнрнпне
лнфер хглемърэ х/хкх онбрнпмн пюяопнярпюмърэ опнцпюллс, мх б йнел яксвюе ме
мея╗р нрберярбеммнярх, бйкчвюъ кчаше наыхе, яксвюимше, яоежхюкэмше хкх
онякеднбюбьхе сашрйх, ябъгюммше я хяонкэгнбюмхел хкх онмеяеммше бякедярбхе
мебнглнфмнярх хяонкэгнбюмхъ опнцпюллш (бйкчвюъ онрепх дюммшу, хкх дюммше,
ярюбьхе мецндмшлх, хкх сашрйх х/хкх онрепх днунднб, онмеяеммше хг-гю деиярбхи
рперэху кхж х/хкх нрйюгю опнцпюллш пюанрюрэ янблеярмн я дпсцхлх опнцпюллюлх,
мн ме нцпюмхвхбюъяэ щрхлх яксвюълх), мн ме нцпюмхвхбюъяэ хлх, дюфе еякх рюйни
бкюдекеж хкх дпсцне кхжн ашкх хгбеыемш н бнглнфмнярх рюйху сашрйнб х онрепэ.

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

#include <stdlib.h>
#include <string.h>

#include "struct.h"
#include "cutstr.h"
#include "alik_rec.h"
#include "interval.h"
#include "diskrtab.h"
#include "recdefs.h"

extern  int16_t      minrow,bbs1,bbs2,bbs3,bbs4,Nb1,Nb2,Nb3;

#include "minmax.h"

void snap_clear_screen(void);

extern uchar  db_status;              // snap presence byte
int16_t up_position,dw_position;
static int16_t Alik_count_int_number(pchar Praster,int16_t len_row);
int16_t LeftDistance(uchar *RASTER, int16_t NWIDTH);
int16_t RightDistance(uchar *RASTER, int16_t NWIDTH);

static char table_int[256]=
       {0,1,1,1,1,2,1,1,1,2,2,2,1,2,1,1,1,2,2,2,2,3,2,2,1,2,2,2,1,2,1,1,
        1,2,2,2,2,3,2,2,2,3,3,3,2,3,2,2,1,2,2,2,2,3,2,2,1,2,2,2,1,2,1,1,
        1,2,2,2,2,3,2,2,2,3,3,3,2,3,2,2,2,3,3,3,3,4,3,3,2,3,3,3,2,3,2,2,
        1,2,2,2,2,3,2,2,2,3,3,3,2,3,2,2,1,2,2,2,2,3,2,2,1,2,2,2,1,2,1,1,
        1,2,2,2,2,3,2,2,2,3,3,3,2,3,2,2,2,3,3,3,3,4,3,3,2,3,3,3,2,3,2,2,
        2,3,3,3,3,4,3,3,3,4,4,4,3,4,3,3,2,3,3,3,3,4,3,3,2,3,3,3,2,3,2,2,
        1,2,2,2,2,3,2,2,2,3,3,3,2,3,2,2,2,3,3,3,3,4,3,3,2,3,3,3,2,3,2,2,
        1,2,2,2,2,3,2,2,2,3,3,3,2,3,2,2,1,2,2,2,2,3,2,2,1,2,2,2,1,2,1,1};


static char sum_table[256]= /* ╓╝╚╕╜═ ╞Ю╔╓Х╔АБ╒╝╒═БЛ Б═║╚╗Ф╔ fb_table  !!!!  */
      {0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,
       1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
       1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
       2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
       1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
       2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
       2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
       3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8};

static char fb_table[512]=  /* Б═║╚╗Ф═ ╜╝╛╔Ю═ А═╛╝ё╝ ╚╔╒╝ё╝ ╔╓╗╜╗Г╜╝ё╝ ║╗Б═ */
     {0,7,6,6,5,5,5,5,4,4,4,4,4,4,4,4,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
      2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
      1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
      1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                    /* Б═║╚╗Ф═ ╜╝╛╔Ю═ А═╛╝ё╝ ╞Ю═╒╝ё╝ ╔╓╗╜╗Г╜╝ё╝ ║╗Б═*/
      0,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
      5,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
      6,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
      5,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
      7,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
      5,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
      6,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
      5,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0};

void Alik_rus_bad_c(pchar raster_frag,pchar SourceRaster,int16_t hgt,int16_t rx_bite,
                    pchar product,pchar product_two,pint16_t penalty,
		    pint16_t cut_points,puchar CountCut,puchar UpBlackPoint)
{
#ifdef Alikc
char snap[380],*buf=snap;
#endif
puchar PtrCountCut;
int16_t   i,Left,Right,CP;
pint16_t  Pcut_points,Pen;
pchar prod_two;

   raster_frag  = raster_frag;
   SourceRaster = SourceRaster;
   PtrCountCut  = CountCut;
   Pen          = penalty;
   prod_two     = product_two;
   product      = product;
   hgt          = hgt;
   Left=Right=0;
   if(rx_bite < MIN_WIDTH_RAS)  return;
   Pcut_points  = cut_points + *cut_points;

 for(i=0; i<rx_bite; i++,++product_two,++penalty,++CountCut,++UpBlackPoint)
  {
   if(*product_two>MIN_HEIGHT_FOR_C && *product_two<=MAX_HEIGHT_FOR_C &&
      *CountCut==1 && *penalty >0 && *penalty <= 100 &&
      *penalty>=*(penalty+1) &&
      abs((*UpBlackPoint-*(UpBlackPoint+1))<3))
    {
     if(!Left)    Left=Right=i;
     else         Right=i;
    }
   else
    {
     if(Right-Left>1 && Right>OTSTUP &&  Alik_check_c(PtrCountCut,Pen,Right))
      {
       Right++;
       CP=rx_bite-Right;
       if(CP>=MINCOL)
	{
	 if(!Alik_kill_left_right_points(cut_points,CP))
            *++Pcut_points = CP;
#ifdef Alikc
   if( db_status && snap_activity('j'))
      {
       buf=snap;
       buf+=SPRINTF(buf,"Letter C Left=%d,Right=%d ",Left,Right);
       Alik_snap_show_raster(raster_frag,SourceRaster,snap,hgt,rx_bite,
                             product,prod_two,Pen);
      }
#endif
	}
      }
     Left=Right=0;
    }
  }
 *cut_points=(uint16_t)(Pcut_points-cut_points);
}

uchar Alik_check_c(puchar PtrCountCut,pint16_t Pen,int16_t Right)
{
int16_t i;

 PtrCountCut+=Right;
 Pen+=Right;

 for(i=0; i< NUMB_STEP; i++,--PtrCountCut,--Pen)
  {
   if(*PtrCountCut!=1 || (uint16_t)*Pen>(uint16_t)*(Pen-1)) return 0;
  }
 return 1;
}

void Alik_Check_Rus_D(pchar raster_frag,pchar SourceRaster,int16_t dy,int16_t dx,
                      pint16_t cut_points,puchar IntBuf,int16_t row,pchar product,
		      pchar product_two,pint16_t penalty,puchar CountCut)
{
#ifdef Alikd
char snap[380],*buf=snap;
#endif

uchar  SumCol=0,Col,*CurIntBuf,lb,rb,lb1,rb1,nlb,ColOst,ColOsn;
pchar CurPos;
int16_t   i,j,*CurCP,hor_byte,up_ot_bbs3=0,CP,dif;

MasIntervals  MI[MAX_CUT_POINT_NUMBER];
MasIntervals1 MI1[MAX_CUT_POINT_NUMBER];

 penalty=penalty;
 product_two=product_two;
 product=product;
 raster_frag=raster_frag;

 hor_byte=(dx+7)>>3;
 i=((row+dy)-(minrow+bbs3)+1);
 dif=abs((row+dy+1)-(minrow+bbs3));
 if(i>2)
  {
   /*  ┌КГ╔А╚О╔БАО ╝А╜╝╒═╜╗╔ └ */
#ifdef Alikd
   if(db_status && snap_activity('j'))
      {
       buf=snap;
       buf+=SPRINTF(buf,"       ▌А╜╝╒═╜╗╔ └ \n");
      }
#endif

  while(up_ot_bbs3<3)
   {
    CurIntBuf=IntBuf;
    CurPos   =SourceRaster+(dy-(i+up_ot_bbs3))*hor_byte;
    Col=SumCol=Alik_find_black_interval((puchar)CurPos,(uchar)hor_byte,CurIntBuf);
    CurPos+=hor_byte;

    for(j=0,ColOsn=0;j<Col;j++)
     {
      lb=*(IntBuf+(j<<1));
      rb=*(IntBuf+(j<<1)+1);

      if(j<Col-1)
        {
	  /*А╙╚╔╗╒═╔╛ А╚╔╓. ╗╜Б╔Ю╒═╚ ╔А╚╗ Ю═АБ╝О╜╗╔ ╛═╚.*/
          nlb=*(IntBuf+((j+1)<<1));
          if(nlb-rb<=2) { j++; rb=*(IntBuf+(j<<1)+1); }
        }

      if(rb-lb >= MIN_OSNOV_D)
       {
	if(rb-lb<MIN_OSNOV_D+MINCOL && Alik_del_a(penalty,CountCut,lb,rb))
	   continue;
        MI1[ColOsn].osnl=lb;  MI1[ColOsn].osnr=rb;
        ColOsn++;
       }
     }
    up_ot_bbs3++;
    if(ColOsn) break;
   }

/* ⌠╓═╚ОН ╞╚╝Е╗╔ ЦГ═АБ╙╗ ╓╚О ╝А╜╝╒═╜╗О └*/
    for(j=0,ColOst=0;j<ColOsn;j++)
     {
      lb=MI1[j].osnl;
      rb=MI1[j].osnr;
      if(Alik_del_a_from_interval(penalty,CountCut,lb,rb,&lb1,&rb1))
       {
        if(lb1-lb >= MIN_OSNOV_D)
	 {
#ifdef Alikd
          if( db_status && snap_activity('j') )
          buf+=SPRINTF(buf," (%d,%d) ",lb-1,lb1-1);
#endif
          MI[ColOst].osnl=lb;  MI[ColOst].osnr=lb1;
          MI[ColOst].lb=MI[ColOst].rb=MI[ColOst].wf=0;
	  MI[ColOst].mb=MI[ColOst].cvl=MI[ColOst].cvr=MI[ColOst].cvm=0;
          ColOst++;
	 }

        if(rb-rb1 >= MIN_OSNOV_D)
	 {
#ifdef Alikd
          if( db_status && snap_activity('j') )
          buf+=SPRINTF(buf," (%d,%d) ",rb1-1,rb-1);
#endif
          MI[ColOst].osnl=rb1;  MI[ColOst].osnr=rb;
          MI[ColOst].lb=MI[ColOst].rb=MI[ColOst].wf=0;
	  MI[ColOst].mb=MI[ColOst].cvl=MI[ColOst].cvr=MI[ColOst].cvm=0;
          ColOst++;
	 }
       }
      else
       {
#ifdef Alikd
   if( db_status && snap_activity('j') )
       buf+=SPRINTF(buf," (%d,%d) ",lb-1,rb-1);
#endif
        MI[ColOst].osnl=lb;  MI[ColOst].osnr=rb;
	MI[ColOst].lb=MI[ColOst].rb=MI[ColOst].wf=0;
	MI[ColOst].mb=MI[ColOst].cvl=MI[ColOst].cvr=MI[ColOst].cvm=0;
        ColOst++;
       }
     }

#ifdef Alikd
   if( db_status && snap_activity('j') && ColOst)
       Alik_snap_show_raster(raster_frag,SourceRaster,snap,dy,dx,
                             product,product_two,penalty);
#endif

   if(ColOst)
    {

     /*  ┌КГ╔А╚ОНБАО ╚═╞╙╗ └ */
     CurPos   =SourceRaster + dy*hor_byte;

     for(j=1;j<i;j++)
      {
       CurPos-=hor_byte;
       CurIntBuf=IntBuf+(SumCol<<1);
       SumCol+=Alik_find_black_interval((puchar)CurPos,(uchar)hor_byte,CurIntBuf);
      }

     CurIntBuf=IntBuf+(Col<<1);
     for(j=0;j<SumCol-Col;j++)
      {
       lb=*CurIntBuf++; rb=*CurIntBuf++;
       for(i=0;i<ColOst;i++)
        {
         if(lb==rb && MI[i].wf>0)  continue;

         if((lb<=MI[i].osnl && rb<=MI[i].osnl)  ||
	    (lb>=MI[i].osnr && rb>=MI[i].osnr)) continue;

         /*  ▀╔╒═О  ╚═╞╙═ └ */
         if(!MI[i].cvl)     {MI[i].lb=lb; MI[i].cvl++; goto con;}
	 else
	  {
	   if(lb<=MI[i].lb)
	     {
	      if((MI[i].lb<=lb+4) && MI[i].wf>2 &&
	         (rb-lb+1 >= (MI[i].wf<<1) || rb-lb+1 > dif))
	                                continue;
	      MI[i].lb=lb; MI[i].cvl=1; goto con;;
	     }
	   if(lb-MI[i].lb<MINCOL) continue;
	  }
	 /*  ┘АБЛ АЮ╔╓╜ОО  ╚═╞╙═ └└ */
         if(MI[i].cvl && MI[i].cvr &&
	     lb > MI[i].lb + MAX((MIN_OSNOV_D>>1),(MI[i].rb-MI[i].lb)/3) &&
             rb < MI[i].rb - (MI[i].rb-MI[i].lb)/3)
              {MI[i].mb=lb+((rb-lb)>>1); MI[i].cvm++; goto con;}
         /*  ▐Ю═╒═О  ╚═╞╙═ └ */
         if(MI[i].cvl)
	  {
	   if(!MI[i].cvr)    {MI[i].rb=rb; MI[i].cvr++; goto con;}
	   else
	     if(rb>=MI[i].rb)
	      {
	       if((MI[i].rb+4 >= rb) && MI[i].wf>2 &&
	           (rb-lb+1 >= (MI[i].wf<<1) || rb-lb+1 > dif))     continue;

	       if(lb>MI[i].rb && lb-2<=MI[i].rb && rb-lb<MI[i].wf)  continue;
	        MI[i].rb=rb; MI[i].cvr=1;  goto con;
	      }
	  }
	 continue;
	  /* ╛═╙А╗╛═╚Л╜═О Х╗Ю╗╜═ ╜╝ё╗ */
con:	 MI[i].wf=MAX(MI[i].wf,rb-lb+1);
	}
      }

     CurCP = cut_points + *cut_points;

     for(i=0;i<ColOst;i++)
      {
       if(MI[i].lb<MI[i].osnl) MI[i].lb=MI[i].osnl;
       if(MI[i].rb>MI[i].osnr) MI[i].rb=MI[i].osnr;

  //  ***************  left position  ******************
       if(MI[i].cvl && (MI[i].cvm || MI[i].cvr))
        {
#ifdef Alikd
   if( db_status && snap_activity('j'))
      {
       buf=snap;
       buf+=SPRINTF(buf," ╓ left position=%3d ",MI[i].lb-1);
       Alik_snap_show_raster(raster_frag,SourceRaster,snap,dy,dx,
                             product,product_two,penalty);
      }
#endif
	 if(MI[i].wf>1)     CP=dx-MI[i].lb;
	 else               CP=dx-(MI[i].lb-1);

	 if(!Alik_kill_left_right_points(cut_points,CP))
	  *++CurCP=CP;
        }

   if(!MI[i].cvr || (MI[i].rb-MI[i].lb)>dy/3)
     {
  //  ***************  right position  ******************
     if(MI[i].cvl || MI[i].cvm || MI[i].cvr)
      {
#ifdef Alikd
   if( db_status && snap_activity('j'))
     {
       buf=snap;

       if(MI[i].cvl && !MI[i].cvm && !MI[i].cvr)
         buf+=SPRINTF(buf," Ф ╗╚╗ И right position=%3d ",MI[i].lb+MI[i].wf);
       else
         if(MI[i].cvr)
            buf+=SPRINTF(buf," ╓ right position=%3d ",MI[i].rb);
         else
            buf+=SPRINTF(buf," ╓ right position=%3d ",MI[i].mb);

       Alik_snap_show_raster(raster_frag,SourceRaster,snap,dy,dx,
                             product,product_two,penalty);
      }
#endif
       if(MI[i].cvl && !MI[i].cvm && !MI[i].cvr)
			  CP=MI[i].lb+MI[i].wf-1;
       else
         if(MI[i].cvr)    CP=MI[i].rb;
         else             CP=MI[i].mb;

       if(*(product_two+CP) > *(product_two+CP+1)*7) CP++;
       CP=dx-CP;
       if( !Alik_kill_left_right_points(cut_points,CP) )
        *++CurCP=CP;
      }

  //  ***************  seredina  ******************
       if(MI[i].cvm && MI[i].cvl && MI[i].cvr)
        {
#ifdef Alikd
   if( db_status && snap_activity('j'))
      {
       buf=snap;
       buf+=SPRINTF(buf," ╓ seredina=%3d ",MI[i].mb);
       Alik_snap_show_raster(raster_frag,SourceRaster,snap,dy,dx,
                             product,product_two,penalty);
      }
#endif
	 CP=dx-MI[i].mb;
//	 if(!Alik_kill_left_right_points(cut_points,CP))
           *++CurCP=CP;
        }
      }
     }
     *cut_points=(uint16_t)(CurCP-cut_points);
    }
  }
}

uchar Alik_del_a(pint16_t penalty,puchar CountCut,uchar lb,uchar rb)
{
uchar Sum,i;

 penalty=penalty+lb;
 CountCut=CountCut+lb;
 for(i=lb,Sum=0;i<rb;i++,penalty++,CountCut++)
  if(*CountCut>2 || *penalty>6)  Sum++;
 if(Sum>((rb-lb)>>1)) return 1;
 else                 return 0;
}

uchar Alik_del_a_from_interval(pint16_t penalty,puchar CountCut,uchar lb,uchar rb,
                              uchar* lb1,uchar* rb1)
{
uchar Sum,i;

 penalty=penalty+lb-1;
 CountCut=CountCut+lb-1;
 Sum=0;
 for(i=lb; i<=rb; i++,penalty++,CountCut++)
  {
   if(*CountCut>2 || *penalty>6 || *CountCut==2 && *penalty>0)
    {
     if(!Sum) *lb1=i;
     Sum++;
    }
   else
    {
     if(Sum>=MAX_BAD_INTERVAL) {*rb1=i; return 1;}
     Sum=0;
    }
  }
 if(Sum>=MAX_BAD_INTERVAL) {*rb1=rb; return 1;}
 return 0;
}

uchar  Alik_priznak_del_3_cut(puchar CountCut,int16_t CP,int16_t dx)
{
puchar  CurCutPoint;
int16_t    i,Sum;

 Sum=0;
 CP=dx-CP;
 if(*(CountCut+CP)==3)  Sum++;
 CurCutPoint = CountCut+CP-1;
 for(i=CP;i>1;--i,--CurCutPoint)
    if(*CurCutPoint==3) Sum++;
    else  break;

 if(Sum>3) return 1;

 CurCutPoint = CountCut+CP+1;
 for(i=CP;i<dx;++i,++CurCutPoint)
    if(*CurCutPoint==3) Sum++;
    else  break;

 if(Sum>3) return 1;
 else      return 0;
}

void Alik_def_cut_flg(cell *T,struct cut_elm *cpnt1,
                      struct cut_elm *cpnt2,int16_t height,uchar WhereCutten)
{
int16_t begin,end;

 T->cg_flag_fine=0;
 switch(WhereCutten)
  {
   case 0:
          begin = height-cpnt1->h;
          end   = begin+cpnt1->dh;
          if(begin>=0 && end<=(height>>2)) T->cg_flag_fine |= (c_cg_cut_tl);
          else if(begin>=(height>>2) && end<=(height>>2)*3)
                                           T->cg_flag_fine |= (c_cg_cut_ml);
               else if(begin>=(height>>2)*3 && end<=height)
                                           T->cg_flag_fine |= (c_cg_cut_bl);
                    else if(abs(cpnt1->dh-height)<3)
                                           T->cg_flag_fine |= (c_cg_cut_fl);
          break;
   case 1:
          begin = height-cpnt1->h;
          end   = begin+cpnt1->dh;
          if(begin>=0 && end<=(height>>2)) T->cg_flag_fine |= (c_cg_cut_tr);
          else if(begin>=(height>>2) && end<=(height>>2)*3)
                                           T->cg_flag_fine |= (c_cg_cut_mr);
               else if(begin>=(height>>2)*3 && end<=height)
                                           T->cg_flag_fine |= (c_cg_cut_br);
                    else if(abs(cpnt1->dh-height)<3)
                                           T->cg_flag_fine |= (c_cg_cut_fr);
          break;
   case 2:
          begin = height-cpnt1->h;
          end   = begin+cpnt1->dh;
          if(begin>=0 && end<=(height>>2)) T->cg_flag_fine |= (c_cg_cut_tl);
          else if(begin>=(height>>2) && end<=(height>>2)*3)
                                           T->cg_flag_fine |= (c_cg_cut_ml);
               else if(begin>=(height>>2)*3 && end<=height)
                                           T->cg_flag_fine |= (c_cg_cut_bl);
                    else if(abs(cpnt1->dh-height)<3)
                                           T->cg_flag_fine |= (c_cg_cut_fl);
          begin = height-cpnt2->h;
          end   = begin+cpnt2->dh;
          if(begin>=0 && end<=(height>>2)) T->cg_flag_fine |= (c_cg_cut_tr);
          else if(begin>=(height>>2) && end<=(height>>2)*3)
                                           T->cg_flag_fine |= (c_cg_cut_mr);
               else  if(begin>=(height>>2)*3 && end<=height)
                                           T->cg_flag_fine |= (c_cg_cut_br);
                     else if(abs(cpnt2->dh-height)<3)
                                           T->cg_flag_fine |= (c_cg_cut_fr);
          break;
  }
return;
}

void Alik_cut_short(pchar bufer,pchar bufer_two,int16_t ry,int16_t rx_bite,
                pchar product,pint16_t penalty, pint16_t cut_points,pchar prod_two)
{
uchar   flag_seredina;
int16_t    i,i1,i2,Sum,SumTwo;
pchar  Pproduct,PProdTwo,Pbufer,PBufTwo;
pint16_t   Pcut_points;

   *cut_points = 0;
   Pproduct    = product;
   Pbufer      = bufer;
   PBufTwo     = bufer_two;
   PProdTwo    = prod_two;

   while(1)
    {
      SumTwo=0;
      if(PProdTwo==prod_two)                             flag_seredina=0;
      if(PProdTwo>prod_two && PProdTwo<(prod_two+ ry-1)) flag_seredina=1;
      if(PProdTwo==prod_two+ry-1)                        flag_seredina=2;

      for(i=0; i<rx_bite; i++)
        {
          if(!flag_seredina)    *PBufTwo &= *(PBufTwo+rx_bite);
          else
           if(flag_seredina==1)
            {
             if(sum_table[(uchar)(*PBufTwo & (*(PBufTwo+rx_bite)))] >
                sum_table[(uchar)(*PBufTwo & (*(PBufTwo-rx_bite)))])
                   *PBufTwo &= *(PBufTwo+rx_bite);
             else  *PBufTwo &= *(PBufTwo-rx_bite);
            }
           else                 *PBufTwo &= *(PBufTwo-rx_bite);

          SumTwo+=sum_table[(uchar)*PBufTwo++];
        }
      *PProdTwo++ = (char)SumTwo;
      if(PProdTwo >= (prod_two + ry)) break;
    }

   while(1)
    {
      Sum=0;
      for(i=0; i<rx_bite; i++)
        {
         *Pbufer &= *(Pbufer+rx_bite);
          Sum+=sum_table[(uchar)*Pbufer++];
        }
      *Pproduct++ = (char)Sum;
      if(Pproduct >= (product + ry-1)) break;
    }


   i1=0; i2=-1;
   Pproduct = product;
   Pcut_points = cut_points;
   for(i=ry-1; i>0; i--)
    {
      penalty++;
      if(*Pproduct++ > BOUND_SH)   continue;
      if(i == i1)
        {
         if(*(Pproduct-1) > *(Pproduct-2)) continue;
         if((*(Pproduct-1) < *(Pproduct-2)) || (*penalty<i2)) goto f4b;
         if(*penalty>i2) goto f4c;
        }
      i1=i;
      Pcut_points++;
f4b:  i2=*penalty;
      *Pcut_points=i;
      if((uint16_t)i2 > NAV_BOUND)
        {
         Pcut_points--;
         i1=1;
        }
f4c:  i1--;
    }
    *cut_points=((uint16_t)((pchar)Pcut_points-(pchar)cut_points)>>1);
}

void Alik_find_brus(pchar raster_frag,pchar SourceRaster,pchar bufer,int16_t ry,
                    int16_t hgt,int16_t rx_bite,pchar product,pchar product_two,
		    pint16_t penalty,pint16_t cut_points,puchar CountCut,
		    puchar UpBlackPoint)
{
#if defined (AlikBrus) || defined (Alikt)
char snap[380],*buf=snap;
#endif
uchar CountBrus=0,Col=0,flag_point,foot_size,interval,max_h,
     flag_brus,tolshina[MaxHeightBrus],j,n;
int16_t    i,min_height_t;
pchar  Pproduct,Pproduct_two,Pbufer,pp,ppt;
pint16_t   Pcut_points,PPen;
Coor_brus CB[MaxCountBrus+1];  // +1 Nick 24.08.2002, was bug

   raster_frag  = raster_frag;
   SourceRaster = SourceRaster;
   ry=ry;

   if(rx_bite < MIN_WIDTH_RAS)  return;
   Pproduct     = product;
   Pproduct_two = product_two;
   Pbufer       = bufer;
   Pcut_points  = cut_points + *cut_points;
   PPen         = penalty;
   memset(CB,0,sizeof(struct tag_brus) * MaxCountBrus);
   min_height_t=((bbs3-bbs2)>>1)-2;
   for(i=0; i<rx_bite; i++)
    {
     if(*Pproduct_two>0 && *Pproduct_two<=MaxHeightBrus && *CountCut==1 &&
         abs((*Pproduct_two-*(Pproduct_two+1))<3) && *PPen <= 6 &&
	 abs((*UpBlackPoint-*(UpBlackPoint+1))<MaxHeightBrus))
      {
       if(!CB[CountBrus].L)  CB[CountBrus].L=CB[CountBrus].R=i;
       else                  CB[CountBrus].R=i;
      }
     else
      {
       if((CB[CountBrus].R-CB[CountBrus].L+1)>=MIN_WIDTH_WINGS)
          {
           pp=product+CB[CountBrus].L;
           ppt=product_two+CB[CountBrus].L;
           interval=CB[CountBrus].R-CB[CountBrus].L;

           for(j=0; j<MaxHeightBrus;j++)   tolshina[j]=0;

           for(j=0;j<=interval;j++,pp++,ppt++)
            if(*pp>0 && *pp<=MaxHeightBrus && *ppt>0 && *ppt<=MaxHeightBrus)
                                           tolshina[(uchar)*ppt-1]++;

           for(j=0,max_h=0;j<MaxHeightBrus;j++)
            if(tolshina[j]>tolshina[max_h]) max_h=j;

           CB[CountBrus].H =++max_h;
           flag_brus=1;
           pp=product+CB[CountBrus].L;
           ppt=product_two+CB[CountBrus].L;

           for(j=0;j<=interval;j++,pp++,ppt++)
            if(abs(*pp-max_h)>4 || abs(*ppt-max_h)>4) {flag_brus=0; break;}

           CB[CountBrus].P=Alik_set_position_brus(bufer,hgt,CB[CountBrus].L,
                                                            CB[CountBrus].R);
           if(CB[CountBrus].P<0 ||
              (CB[CountBrus].P !=2 && CB[CountBrus].H==MaxHeightBrus))
                                                          flag_brus=0;
           if(flag_brus)  CountBrus++;
          }
       CB[CountBrus].L=CB[CountBrus].R=CB[CountBrus].H=CB[CountBrus].P=0;
       if(CountBrus>=MaxCountBrus) break;
      }
     ++Pproduct;
     ++Pproduct_two;
     ++PPen;
     ++CountCut;
     ++UpBlackPoint;
    }

   for(i=CountBrus;i>=0;i--)  if((CB[i].R-CB[i].L+1)>=MIN_WIDTH_WINGS) Col++;

   CountBrus=Col;
   if(CountBrus<1) return;

#ifdef AlikBrus
   if( db_status && snap_activity('j') && CountBrus)
      {
       buf=snap;
       buf+=SPRINTF(buf,"LRHP \n");
       for(j=1;j<=CountBrus;j++)
             buf+=SPRINTF(buf,"(%2d,%2d,%d,%d) ",
                CB[j-1].L,CB[j-1].R,CB[j-1].H,CB[j-1].P);
       Alik_snap_show_raster(raster_frag,SourceRaster,snap,hgt,rx_bite,
                             product,product_two,penalty);
      }
#endif

   for(j=0; j<CountBrus; ++j)
      {
       if(CB[j].P==2)    continue;

       if(rx_bite-CB[j].R<=MINCOL || (CB[j+1].L-CB[j].R)>MAX_WIDTH_STOLB ||
          (j==CountBrus-1 && CB[j].P!=2))            continue;
       else
       {
         Pproduct_two=product_two+(uint16_t)CB[j].R+1;
         interval=CB[j+1].L-CB[j].R;

         for(i=0,foot_size=0;i<interval;++i,Pproduct_two++)
          if(*Pproduct_two > min_height_t)  ++foot_size;

         flag_point=(foot_size+((interval+1)>>1))/interval;
         if(flag_point)
          {
           interval=MIN(CB[j].R-CB[j].L+1,CB[j+1].R-CB[j+1].L+1);
           if(CB[j].P && !CB[j+1].P)    //╚
            {
             n=MIN(2,CB[j].R-CB[j].L);
             for(; n<= CB[j].R-CB[j].L;)
              if((*(product_two+CB[j].R-(n+1))<=*(product_two+CB[j].R-n))
                  && (*(penalty+CB[j].R-(n+1))<=*(penalty+CB[j].R-n))) n++;
              else  break;
              interval=n;
#ifdef AlikBrus
   if( db_status && snap_activity('j'))
      {
       buf=snap; buf+=SPRINTF(buf," ╚ position=%3d ",CB[j].R-interval-1);
       Alik_snap_show_raster(raster_frag,SourceRaster,snap,hgt,rx_bite,
                             product,product_two,penalty);
      }
#endif
             *++Pcut_points = rx_bite-(CB[j].R-interval);
             j++;
            }
// lett
	 else        //****************   T  *****************
	 {
	  if(!CB[j].P && !CB[j+1].P)
	   {
		if(!Alik_Check_T(bufer,hgt,(int16_t)(CB[j].R-interval+1),(int16_t)(CB[j+1].L+interval-1)))
			continue;
	/*
		if(((CB[j].R-CB[j].L)>>1)>(CB[j+1].R-CB[j+1].L))
		   CB[j].L=CB[j].R-(CB[j+1].R-CB[j+1].L)-2;
	*/
		if(*(penalty+CB[j].L)==0 && *(product_two+CB[j].L-1)<=MaxHeightBrus)
			--CB[j].L;

		if(*(penalty+CB[j+1].R)==0)       ++CB[j+1].R;

	#ifdef Alikt
	   if( db_status && snap_activity('j'))
		  {
		   buf=snap;
		   buf+=SPRINTF(buf," Б position1=%3d position2=%3d",CB[j].L,CB[j+1].R);
		   Alik_snap_show_raster(raster_frag,SourceRaster,snap,hgt,rx_bite,
								 product,product_two,penalty);
		  }
	#endif

	if(CB[j].L>MINCOL+2 &&
	   !Alik_kill_right_points(rx_bite,product,product_two,cut_points,CB[j].L))
		*++Pcut_points = rx_bite-CB[j].L;

	if(CB[j+1].R<rx_bite-(MINCOL+2) &&
	   !Alik_kill_left_points(rx_bite,cut_points,CB[j+1].R))
		*++Pcut_points = rx_bite-CB[j+1].R;

		  j++;
	   }
	 }

          }
      }
     }

  if((CB[CountBrus-1].R-CB[CountBrus-1].L)>=5 && CB[CountBrus-1].H >=3)
   {
#ifdef AlikBrus
   if( db_status && snap_activity('j'))
      {
       buf=snap;
       buf+=SPRINTF(buf," serif position=%3d ",CB[CountBrus-1].L);
       Alik_snap_show_raster(raster_frag,SourceRaster,snap,hgt,rx_bite,
                             product,product_two,penalty);
      }
#endif
       *++Pcut_points = rx_bite-CB[CountBrus-1].L-1;
   }

  *cut_points=(uint16_t)(Pcut_points-cut_points);
}

uchar Alik_kill_right_points(int16_t dx,pchar prod,pchar prod2,pint16_t cut_points,
                            int16_t CP)
{
uchar flag_point;
int16_t  i,j,x,*Ptr;

 x=dx-CP;
 flag_point=0;
 i=*cut_points;
 Ptr=cut_points+1;
 for(j=0; j<i; j++,Ptr++)
  if(x-*Ptr>=0 && x-*Ptr<=2 && *(prod2+(dx-*Ptr))>1 && *(prod+(dx-*Ptr-1)))
    {*Ptr=x; flag_point=1;}

 return flag_point;
}

uchar Alik_kill_left_points(int16_t dx,pint16_t cut_points,int16_t CP)
{
uchar flag_point;
int16_t  i,j,x,*Ptr;

 x=dx-CP;
 flag_point=0;
 i=*cut_points;
 Ptr=cut_points+1;
 for(j=0; j<i; j++,Ptr++)
  if(*Ptr-x>=0 && *Ptr-x<=2)
    {*Ptr=x; flag_point=1;}

 return flag_point;
}

uchar Alik_kill_left_right_points(pint16_t cut_points,int16_t CP)
{
uchar flag_point;
int16_t  i,j,*Ptr;

 flag_point=0;
 i=*cut_points;
 Ptr=cut_points+1;
 for(j=0; j<i; j++,Ptr++)
  if(abs(*Ptr-CP)<=3)
    {*Ptr=CP; flag_point=1;}

 return flag_point;
}

int16_t Alik_set_position_brus(pchar bufer,int16_t hgt,int16_t Left,int16_t Right)
{
pchar CurPos;
int16_t   i,ver_byte,interval,l1,r1,minl,maxl,minr,maxr,tret;

 tret=hgt/3;
 ver_byte = (hgt+7)>>3;
 interval = Right-Left;
 CurPos   = bufer + Left * ver_byte;

 for(i=0; i<=interval; i++,CurPos+=ver_byte)
  {
   l1=LeftDistance((puchar)CurPos,ver_byte);
   r1=RightDistance((puchar)CurPos,ver_byte);
   if(l1<0 || r1<0) return -1;
   if(i>0)
    {
     minl=MIN(minl,l1); minr=min(minr,r1);
     maxl=MAX(maxl,l1); maxr=max(maxr,r1);
    }
   else
    {
     minl=maxl=l1;  minr=maxr=r1;
    }
  }

 if(maxl<=tret && maxr<=tret)                                      return 2;
 if(minl==maxl && minr==maxr && (hgt-2*MaxHeightBrus)>(maxl+maxr)) return 2;
 if(maxl-minl>MaxHeightBrus || maxr-minr>MaxHeightBrus ||
    abs(hgt-(minl+minr) >= 2*MaxHeightBrus))                        return -1;
 if(minr<tret)                                                      return 1;
 else                                                               return 0;
}

int16_t Alik_Check_T(pchar bufer,int16_t hgt,int16_t Left,int16_t Right)
{
pchar CurPos;
int16_t   i,ver_byte,interval,dist,mind,maxd;

 ver_byte = (hgt+7)>>3;
 interval = Right-Left;
 CurPos   = bufer + Left*ver_byte;

 for(i=0; i<=interval; i++,CurPos+=ver_byte)
  {
   dist=LeftDistance((puchar)CurPos,ver_byte);
   if(i>0) { mind=MIN(mind,dist);  maxd=MAX(maxd,dist); }
   else      mind=maxd=dist;
  }

 if(maxd-mind>MaxHeightBrus)  return 0;
 else                         return 1;
}


void Alik_cut_y(pchar raster_frag,pchar SourceRaster,pchar bufer,int16_t dy,
                int16_t dx,pint16_t cut_points,puchar IntBuf,int16_t row,pchar product,
		pchar product_two,pint16_t penalty,puchar CountCut)
{
#ifdef Aliky
char snap[380],*buf=snap;
#endif

uchar  Col,*CurIntBuf,lb,rb,left,right,*SumStr,l_bound,r_bound,flag_near;
char  min_prod_two;
pchar CurPos,prod,prod_two;
int16_t   i,j,k,hor_byte,ver_byte,dw_ot_bbs3=3,width_tail,*pen,*CurCP,flag_y,
      LeftDist,RightDist;
uint16_t  min_pen;

 left=right=flag_near=0;
 raster_frag=raster_frag;
 hor_byte =(dx+7)>>3;
 ver_byte =(dy+7)>>3;
 i=(row+dy)-(minrow+bbs3);
 if(i>=5)
  {
   if(i==5)  dw_ot_bbs3=2;
   /*  ┌КГ╔А╚О╔БАО Е╒╝АБ╗╙ ╝Б ⌠ */
#ifdef Aliky
   if(db_status && snap_activity('j'))
      {
       buf=snap;
       buf+=SPRINTF(buf,"       •╒╝АБ╗╙ ⌠ \n");
      }
#endif

 CurPos = SourceRaster+(dy-i+dw_ot_bbs3)*hor_byte;
 memset(IntBuf,0,hor_byte);

 for(j=0;j<i-dw_ot_bbs3;j++)
   {
    SumStr=IntBuf;
    for(k=0;k<hor_byte;k++,SumStr++,CurPos++)
      *SumStr |= *CurPos;
   }

 SumStr=IntBuf;
 CurIntBuf=IntBuf+hor_byte;
 Col=Alik_find_black_interval(SumStr,(uchar)hor_byte,CurIntBuf);

 for(j=0;j<Col;j++)
   {
    lb=*CurIntBuf++;
    rb=*CurIntBuf++;
    if((rb-lb>right-left) && (rb-lb+1>=MIN_WIDTH_TAIL))
      {
       for(i=lb,flag_y=1;i<rb;i++)
	 if(*(CountCut+i)>1) {flag_y=0; break;}
       if(flag_y)
	{ right=rb-1;  left=lb-1; }
      }
   }

  if(right>left)
   {
    CurCP = cut_points + *cut_points;
    width_tail=right-left;
#ifdef Aliky
    if( db_status && snap_activity('j'))
     {
      buf+=SPRINTF(buf,"left=%d  , right=%d \n",left,right);
      Alik_snap_show_raster(raster_frag,SourceRaster,snap,dy,dx,product,
                            product_two,penalty);
     }
#endif
/*************     ▌╞Ю╔╓╔╚О╔╛ ╚╔╒ЦН Б╝Г╙Ц      *************/
  if(left >= MINCOL)
   {
    l_bound=MAX(0,left-1);
    r_bound=MAX(l_bound,right - LEFT_STEP);
    pen=penalty  + l_bound;
    prod_two = product_two + l_bound;
    prod = product + l_bound;
    min_prod_two=*prod_two;
    min_pen=(uint16_t)(*pen);
    for(i=l_bound; i<r_bound; i++,pen++,prod_two++,prod++)
     {
      if(*prod_two < min_prod_two ||  *prod < min_prod_two ||
        (*prod_two == min_prod_two && (uint16_t)*pen < min_pen))
       {
        left=(uchar)i;
        min_prod_two=MIN(*prod_two,*prod);
        min_pen=(uint16_t)(*pen);
       }
     }
    if(left >= MINCOL) { *++CurCP=dx-left; *cut_points+=1;}
   }
  CurPos  =bufer + left*ver_byte;
  LeftDist=LeftDistance((puchar)CurPos,ver_byte);
/*************     ▌╞Ю╔╓╔╚О╔╛ ╞Ю═╒ЦН Б╝Г╙Ц      *************/
  if(dx-right >= MINCOL)
   {
    l_bound=MIN(dx,right + Y_CUT);
    r_bound=MIN(dx,l_bound + Y_CUT + LEFT_STEP);
    right=l_bound;
    pen=penalty  + l_bound;
    prod_two = product_two + l_bound;
    prod     = product + l_bound;
    min_prod_two=*prod_two;
    min_pen=(uint16_t)(*pen);
    for(i=l_bound; i<r_bound; i++,pen++,prod_two++,prod++)
     {
      if(*prod_two < min_prod_two || *prod < min_prod_two ||
        (*prod_two == min_prod_two && (uint16_t)*pen < min_pen))
        {
         right=(uchar)i;
         min_prod_two=MIN(*prod_two,*prod);
         min_pen=(uint16_t)(*pen);
        }
      CurPos =bufer + i*ver_byte;
      RightDist=LeftDistance((puchar)CurPos,ver_byte);
      if(abs(RightDist-LeftDist)<=1) flag_near++;
      if(flag_near>5) {right=(uchar)i; break;}
     }
    if(dx-right>=MINCOL) { *++CurCP=dx-right;*cut_points+=1;}
   }
#ifdef Aliky
    if( db_status && snap_activity('j'))
     {
      buf=snap;
      buf+=SPRINTF(buf,"letter y: left=%d,right=%d, LeftDist=%d,RightDist=%d,flag_near=%d\n",
                   left,right,LeftDist,RightDist,flag_near);
      Alik_snap_show_raster(raster_frag,SourceRaster,snap,dy,dx,product,
                            product_two,penalty);
     }
#endif
   }
  }
}


uchar Alik_find_black_interval(puchar str,uchar len,puchar intrvl)
{
uchar  Flag_P=0,Col=0;
int16_t   i,j,offset;
MasInterval *Ptr;

for(i=0; i<len; ++i,++str)
 {
  j=0;  Ptr=&P[*str];  offset=(i<<3);
  if(Flag_P && Ptr->L)
    {*(intrvl-1)=Ptr->I[1]+offset; j=1;}
  Col+=Ptr->C-j;
  for(; j<Ptr->C; ++j)
   {
    *intrvl++=Ptr->I[j<<1]    +offset;
    *intrvl++=Ptr->I[(j<<1)+1]+offset;
   }
  Flag_P=Ptr->R;
 }
return Col;
}

void Alik_form_bound(pchar raster,uint16_t ry,uint16_t rx,uint16_t xbytes,
                     pchar adr_bound,uint16_t prizn)
{
 int16_t    i,one=1,tmp_xbytes;
 pchar  Praster,Praster1;
 char   c1,Ah,Al,*Ptable,crx;

  tmp_xbytes = xbytes;
  c1 = -8;
  Ptable=fb_table;
  Praster1=raster;
  crx=(char)rx;

  while(1)
   {
    Praster=Praster1;
    for(i=1; i<=ry; i++)
     {
      Ah = c1;
      while(1)
       {
        Ah += 8;
        if(Ah >= crx)   {Al=0; goto j3b;}
        Al=*Praster; Praster+=one;
        if(Al)  break;
       }
      Al=Ptable[(uchar)Al]+Ah;
j3b:  if(prizn != 0  && !tmp_xbytes)  Al=rx-Al;
      *adr_bound++ =Al;
      Praster=(pchar)Praster1 + i*xbytes;
     }
     if(!tmp_xbytes)  break;
     Ptable += 256;
     Praster1 = (pchar)raster + xbytes-1;
     c1=(char)((-1)*((xbytes<<3)-rx+8));
     one = -1;  tmp_xbytes=0;   rx--;
   }
}

/******************************************************************/
#pragma warning( disable : 4245 )					 //AK. 07.03.97
void  Alik_cut_hole(pchar trace,int16_t rx,pint16_t cut_points,int16_t first,int16_t ry,
                    pchar product,pint16_t penalty,uchar ind)
{
int16_t   i;
uchar  HRastr;
pint16_t  Pcut_points,Pcut_points1,Ppenalty,Ppenalty1;
pchar Pproduct,Pproduct1,Ptrace,Ptrace1;
uchar  Comp,AL;
uint16_t  DX,DI,Offset,Count;
   penalty=penalty;
   first=first;
   HRastr = (uchar)((ry*3)>>2);
b0:
   /* АД╝Ю╛╗Ю╝╒═╚╗ ═╓Ю╔А ╓╚О ЕЮ═╜╔╜╗О Б╝Г╔╙ Ю═╖Ю╔╖═*/
   Pcut_points=Pcut_points1=(pint16_t)((pchar)cut_points+((int16_t)(*cut_points)<<1));
   Ptrace = Ptrace1 = trace;
   Pproduct = Pproduct1 = product + rx-1;
   Ppenalty = Ppenalty1 = (pint16_t)((pchar)penalty + ((int16_t)rx<<1));

   Comp = 255;  DX = 0;  Count=rx;
b1:
   for( i=Count; i>0; --i )  { Count--;  if(*Ptrace++ != Comp)   goto b2; }
   goto b10;
b2:
   if (Comp < *(Ptrace-1))    { DX = Count;  goto b9;}
   if (!DX)  goto b9;
   if((DX-Count) > MAX_HOLE)  { DX -= Count; goto b9;}
   DX++;
   Pproduct  = Pproduct1;
   Ppenalty  = Ppenalty1;
   Ppenalty  = (pint16_t)((pchar)Ppenalty-(DX<<1));
   Pproduct -= DX;
   Ptrace1   = Ptrace;
   DI = -1;
   Pcut_points1 = Pcut_points;
   AL = HRastr;
while(1)
 {
  if(AL >= *Pproduct)
   {
    if(AL > *Pproduct)  {Pcut_points = Pcut_points1; DI = -1;}
    if(*Ppenalty <= MIN(NAV_BOUND,DI))
     {
      if(*Ppenalty < DI)
                        Pcut_points = Pcut_points1;
      Pcut_points++;
      *Pcut_points = DX;
      AL = *Pproduct;
      DI = *Ppenalty;
     }
   }
   Pproduct++;
   Ppenalty++;
   if(--DX <= Count) break;
 }
   Ptrace = Ptrace1;
   Offset = (int16_t)((pchar)Pcut_points - (pchar)Pcut_points1);
   if(Offset <= 6)  { DX=0; goto b9;}
   Pcut_points = Pcut_points1;
   Offset = (Offset>>1)-3;
   *(Pcut_points+3) -= Offset; /*6*/
   Offset = Offset>>1;
   *(Pcut_points+2) -= Offset; /*2*/
   Pcut_points += 3;
   DX=0;
b9:
   Comp = *(Ptrace-1);
   if(Count) goto b1;
b10:
   *cut_points =((uint16_t)((pchar)Pcut_points-(pchar)cut_points)>>1);
   if(!ind)
    {
     trace += rx;
     Alik_del_y_cut(trace,rx,cut_points,first);
     ind=1;
     goto b0;
    }
}

void  Alik_del_y_cut(pchar trace,int16_t rx,pint16_t cut_points,int16_t first)
{
pint16_t  Pcut_points,Ptr;
pchar BegTrace,CurTrace,TraceEnd;
int16_t   Count,i,j;

 if((Count=*cut_points-first)>0)
   {
    Pcut_points =(pint16_t)((pchar)cut_points + ((first+1)<<1));
    BegTrace = trace + rx - 3;
    for(i=Count; i>0; i--)
     {
      Ptr = Pcut_points++; CurTrace = BegTrace - *Ptr;
      if(*Ptr<=3 || *(trace-*Ptr-1)<=Y_CUT || CurTrace<trace) continue;
      TraceEnd = CurTrace + 5;
      while(1)
       {
        if(*CurTrace>=*(CurTrace+1)) goto end;
        if(++CurTrace > TraceEnd)    break;
       }
      for(j=i;j>0;j--)   {*Ptr = *(Ptr+1); Ptr++;}
      Pcut_points--;
end:;
     }
   *cut_points =((uint16_t)((pchar)Pcut_points-(pchar)cut_points)>>1)-1;
   }
}

void Alik_del_equal_hole(pint16_t cut_points,pchar product,pint16_t penalty,
                         int16_t size_x,int16_t size_y,int16_t first)
{
pint16_t  CurCutPoints,LastCutPoints,FindCutPoints,last,Ppenalty;
pchar Pproduct;
int16_t   Count,LastCount,i,NextCutPoint,cur_sdvig,abs_sdvig;

 if((Count = *cut_points - first) > 0)
  {
   last = (pint16_t)((pchar)cut_points + (*cut_points<<1));
   LastCutPoints = cut_points;
   size_y >>=1;
   product += size_x-1;
   penalty = (pint16_t)((pchar)penalty + (size_x<<1));
   CurCutPoints = (pint16_t)((pchar)cut_points + (first<<1));
   first = -1;
   while(1)
    {
     CurCutPoints++;
     while(1)
      {
       NextCutPoint = *CurCutPoints++;
       LastCount = Count--;
       FindCutPoints = CurCutPoints;
       if(!Count) { if(first<0) goto at2c;  break; }
at2:   for(i=Count; i>0; i--)
          if(*FindCutPoints++ == NextCutPoint)  {Count=i-1; goto at2b;}
       Count=0;
       if(first>=0)  goto at4;
at2c:  Pproduct = product - NextCutPoint;
       cur_sdvig = -1;  abs_sdvig = 0;
       Ppenalty = (pint16_t)((pchar)penalty - (NextCutPoint<<1));
       Count=size_y;
       while(1)
        {
         if(*Pproduct <= (char)Count && *Ppenalty <= NAV_BOUND) goto at4;
         if(abs_sdvig == MAX_SDVIG)
          {
          if(cur_sdvig == 1)  break;
          NextCutPoint -= MAX_SDVIG;
          Pproduct += MAX_SDVIG;
          Ppenalty = (pint16_t)((pchar)Ppenalty + (MAX_SDVIG<<1));
          cur_sdvig = 1;  abs_sdvig = 0;
          }
         abs_sdvig++;
         NextCutPoint -= cur_sdvig;
         *(CurCutPoints-1) = NextCutPoint;
         Pproduct += cur_sdvig;
         Ppenalty = (pint16_t)((pchar)Ppenalty + (cur_sdvig<<1));
        }
       CurCutPoints--;  //at2ab:
       FindCutPoints=CurCutPoints;
       goto at3a;
at2b:  if(Count)
        {
         FindCutPoints--;
at3a:    *FindCutPoints = *last;
        }
       last--;
       LastCount--;
       if(LastCount<=0) break;
       if(first>=0) goto at2;
at4:   Count = LastCount -1;
       if(!Count) break;
      }
      CurCutPoints = LastCutPoints; //at5:
      Count = ((uint16_t)((pchar)last-(pchar)CurCutPoints)>>1);
      *CurCutPoints = Count;
      if(first>=0 || Count<=1) break;
      LastCutPoints = CurCutPoints;
      first=1;
    }
  }
}

void Alik_del_3_cut(pchar raster,pint16_t cut_points,int16_t first,int16_t len_row,
                    int16_t nrow,puchar CountCut)
{
pint16_t   CurCutPoints,Ptr;
int16_t    Count,i,j,SumCutPoints;
pchar  Praster;

 if((Count = *cut_points - first) > 0)
  {
    CurCutPoints  =(pint16_t)((pchar)cut_points + ((first+1)<<1));
    raster += (uint16_t)len_row * (nrow-1);
    for(i=Count;i>0;i--)
     {
      Ptr = CurCutPoints++;
      Praster = raster - (uint16_t)(*Ptr) * len_row;
      SumCutPoints=Alik_count_int_number(Praster,len_row);
      if(SumCutPoints < 3)
        {
         if(SumCutPoints != 1)  *Ptr=-*Ptr;
         continue;
        }
      if(!Alik_priznak_del_3_cut(CountCut,*Ptr,nrow))
         continue;
      for(j=i;j>0;j--)   {*Ptr = *(Ptr+1); Ptr++;}  /*-  sdvig_cut_point -*/
      CurCutPoints--;
     }
 *cut_points =((uint16_t)((pchar)CurCutPoints-(pchar)cut_points)>>1)-1;
  }
}

int16_t Alik_count_int_number(pchar Praster,int16_t len_row)
{
int16_t SumCutPoints,j;
char   c,c1;
pchar  Ptable_int;

  Ptable_int = table_int;
  SumCutPoints=0; c=0;
  for(j=len_row; j>0; j--)
   {
    if(((c1=*Praster++) & 0x80) && (c & 1)) SumCutPoints--;
    c=c1;
    SumCutPoints+=Ptable_int[(uchar)c1];
   }
  return SumCutPoints;
}

void Alik_CountCut(pchar bufer,int16_t dy,int16_t dx,puchar  CountCut,puchar IntBuf)
{
int16_t    i,ver_byte;
 ver_byte=(dy+7)>>3;
 for(i=0; i<dx; i++)
   {
    *CountCut++=Alik_find_black_interval((puchar)bufer,(uchar)ver_byte,IntBuf);
    bufer+=ver_byte;
   }
}

//upp
void Alik_UpBlackPoint(pchar bufer,int16_t dy,int16_t dx,puchar  UpBlackPoint)
{
int16_t    i,ver_byte;
 ver_byte=(dy+7)>>3;
 for(i=0; i<dx; i++)
   {
    *UpBlackPoint++=(uchar)LeftDistance((puchar)bufer,ver_byte);;
    bufer+=ver_byte;
   }
}

#define SERIF_POGRESHNOST     8
#define SERIF_POGRESHNOST_IN  4

int16_t Alik_up_position_double_serif(puchar bufer,int16_t dy,int16_t dx,
                                  PWORD penalty,puchar IntBuf,int16_t hgt)
{
int16_t    i,ver_byte,pos,hgt_bbs;
PWORD  PPen;
uchar   I_begin,I_end,II_begin,II_end,in_up,in_dw,out_up,out_dw;
puchar  CurPos;

 CurPos=bufer;
 PPen=penalty;
 ver_byte=(dy+7)>>3;
 hgt_bbs=MIN(bbs3-bbs2,hgt);
 up_position=0; dw_position=0;
 in_up=0; in_dw=0; out_up=0; out_dw=0;

 for(i=0; i<dx; i++,PPen++)
  {
   pos=Alik_find_black_interval(CurPos,(uchar)ver_byte,IntBuf);
   if(*PPen==0 && pos==2)
    {
     I_begin  = *IntBuf++; I_end  = *IntBuf++;
     II_begin = *IntBuf++; II_end = *IntBuf;
     if(II_begin - I_end<SERIF_POGRESHNOST_IN) continue;
     if(((abs(hgt_bbs - (II_begin-I_end))<=SERIF_POGRESHNOST) ||
        (abs(dy-(II_begin - I_end))<=SERIF_POGRESHNOST)) && II_end<hgt+3)
      {
       if((II_begin-I_end>in_dw-in_up) &&
         (abs(hgt_bbs-(II_end-I_begin))<=abs(hgt_bbs-(dw_position-up_position))))
	  {
	   in_up=I_end; in_dw=II_begin;
           up_position=I_begin;
           dw_position=II_end;
	  }
      }
     if(II_end-I_begin > out_dw-out_up) { out_up=I_begin; out_dw=II_end; }
    }
   CurPos+=ver_byte;
  }

 if(up_position) return up_position;
 if(out_dw-out_up > (dy>>1))
	 {
          up_position=out_up;
          dw_position=out_dw;
          return up_position;
         }
return -1;
}

#define POGRESHNOST 3
void Alik_my_correct_base_lines(pint16_t bl_up,pint16_t bl_dw,int16_t dy,int16_t dx,int16_t row,
        puchar bufer,PWORD penalty,puchar IntBuf)
{
int16_t hgt;
 up_position=0; dw_position=0;
 hgt=dy-abs((row+dy)-(minrow+bbs3));
 up_position=Alik_up_position_double_serif(bufer,dy,dx,penalty,IntBuf,hgt);
 if(up_position<=1) up_position=0;
 if(
   ((Nb1==127 || Nb1==-1) && (Nb2==127 || Nb2==-1) && (Nb3==127 || Nb3==-1))
     ||
   (Nb2<=0 || (abs(row-minrow)<POGRESHNOST &&
              abs(hgt-(bbs3-bbs1))<POGRESHNOST)))
      {
       if((up_position<POGRESHNOST && dw_position!=dy)|| !dw_position)
                                   {*bl_up=0; *bl_dw=hgt; return;}
       else  {*bl_up=up_position;  *bl_dw=dw_position;    return;}
      }

 if((Nb1==127 || Nb1==-1) && abs(row-(minrow+bbs2))<POGRESHNOST &&
               abs(hgt-(bbs3-bbs2))<POGRESHNOST)
      {
       if((up_position<POGRESHNOST && abs(dw_position-hgt)<POGRESHNOST
          && dw_position!=dy) || !dw_position)
            { *bl_up=0;           *bl_dw=hgt;          return;}
       else
	    { *bl_up=up_position; *bl_dw=dw_position;  return;}
      }
 else
   if(Nb1==127 || Nb1==-1)
      {
       if((up_position<POGRESHNOST && abs(hgt-(bbs3-bbs2))<POGRESHNOST &&
               dw_position!=dy)|| !dw_position)
                                   {*bl_up=0; *bl_dw=hgt; return;}
       else  {*bl_up=up_position;  *bl_dw=dw_position;    return;}
      }
  if(dw_position) {*bl_up=up_position;  *bl_dw=dw_position; return;}
}

void Alik_correct_base_lines(pchar trace,int16_t size_x,int16_t size_y,int16_t* adr_ubl,
                             int16_t* adr_lbl)
{
pchar PTrace,Ptr;
int16_t*  Adr;
char  c,c1;
uint16_t  CountPoints,i,Chastnoe,c2;

  PTrace = trace;
  *adr_lbl = size_y - *adr_lbl;
  Adr = adr_ubl;
  while(1)
   {
    c=(char)(*Adr)+TO_CENTER;
    if(Adr==adr_ubl) c1=(char)(*Adr)-FROM_CEN_UP;
    else             c1=(char)(*Adr)-FROM_CEN_DW;
    if(c1<0)  c1=0;
    c2=0; CountPoints=0;
    for(i=size_x;i>0;i--)
     {
      Ptr=PTrace++;
      if((uchar)*Ptr>(uchar)c || (uchar)*Ptr<c1) continue;
      c2 += *Ptr;  CountPoints++;
     }
    if(CountPoints)
     {
      Chastnoe=c2/CountPoints;
      if((CountPoints>>1) < (c2%CountPoints))  Chastnoe++;
      *Adr = Chastnoe;
     }
    if(Adr==adr_lbl) break;
    Adr=adr_lbl;
   }
  *Adr=size_y-*Adr;
}


void Alik_double_serif(pint16_t cut_points,pchar trace,pchar product,int16_t rx,
                       int16_t ry,int16_t bl_up,int16_t bl_dw,pint16_t penalty,
		       puchar bufer,puchar IntBuf)
{
pint16_t   BegCP,CurCP,OldCP,Ppenalty;
char   BasLin,Granisa;
uchar   c,c1,*CurPos,pos;
pchar  PTrace,Pproduct;
int16_t    i,j,BX,tmp,ver_byte;

  ver_byte=(ry+7)>>3;
  BegCP = OldCP = CurCP = (pint16_t)((pchar)cut_points + (*cut_points<<1));
  PTrace = trace;
  c=(uchar)((bl_dw-bl_up)>>1);
  bl_dw = ry-bl_dw;
  c1=0xFE;
  Pproduct=product;
  Ppenalty=penalty;
  for(i=rx-1; i>0; i--)
   {
     Ppenalty=++penalty;
     if((uchar)*Pproduct > c)    goto v9;
     if((uint16_t)*Ppenalty > NAV_BOUND)  goto v9a;
     BasLin = (char)bl_dw;     BX=rx;
v1a: Granisa = *(PTrace+BX) - *(PTrace+BX+1);
     if(Granisa)
      {
       if(Granisa<0) Granisa=-Granisa;
       if(Granisa != 1) goto v9;
      }
     Granisa = *(PTrace+BX);
     if(Granisa != BasLin)
      {
       if(Granisa < BasLin) Granisa++;
       else Granisa--;
       if(Granisa != BasLin) goto v9;
      }
     BasLin=(char)bl_up;
     if(!(BX-=rx)) goto v1a;
     if((uchar)*Pproduct>(c1+1)) goto v9;  //06-24-96 08:40pm
     CurCP=BegCP;
     if((uchar)*Pproduct < c1)             //06-24-96 08:41pm
        {c1=(uchar)*Pproduct; CurCP=OldCP;}
     *++CurCP=i;   BegCP=CurCP;
     goto v9a;
v9:  if(c1==0xFE) goto v9a;
     c1=0xFE;
     tmp=(int16_t)((pchar)OldCP-(pchar)CurCP);
     if(tmp<-4)
      {
       CurCP=(pint16_t)((pchar)CurCP+tmp)+1;
       *CurCP=(*CurCP+i+2)>>1;

       CurPos=bufer+(rx-i)*ver_byte;
       for(j=0; j<STEP_SER; j++)
        {
	 pos=Alik_find_black_interval(CurPos,(uchar)ver_byte,IntBuf);
         if(pos != 2)  break;
         CurPos-=ver_byte;
        }

       PTrace  +=j;
       Pproduct+=j;
       if((i-=j)>0)   goto v9b;
       else   break;
      }
     if(tmp==-4) CurCP--;
v9b: OldCP=BegCP=CurCP;
v9a: PTrace++;  Pproduct++;
   }
 *cut_points=((uint16_t)((pchar)CurCP - (pchar)cut_points)>>1);
}

void Alik_del_equal(pint16_t cut_points)
{
pint16_t LastCP,CurCP,Ptr;
int16_t  i,j,Count,FindCP;

 Count=*cut_points;
 if(Count>1)
  {
   CurCP=cut_points + 1;
   LastCP=(pint16_t)((pchar)cut_points + (Count<<1));
   for(i=--Count;i>0;i--)
    {
     FindCP=*CurCP++;   Ptr=CurCP;
     for(j=i-1; j>=0; j--)
      if(*Ptr++==FindCP)
       {
        if(j)  *(--Ptr)=*LastCP;
        LastCP--;
        if(!(--i)) goto end;
       }
    }
end: *cut_points=((uint16_t)((pchar)LastCP - (pchar)cut_points )>>1);
  }
}

void Alik_defis(pchar trace,pint16_t cut_points,int16_t size_x,int16_t size_y,
                int16_t bl_up,int16_t bl_dw,pint16_t penalty)
{
pchar PTrace,OldTrace;
int16_t   i,Count;
char  c,c1,CTrace,cbl;

 OldTrace = trace-2;
 Count = size_x;
 cbl=(char)bl_up; c1=0;
 while(1)
  {
   cbl+=3;
   PTrace = OldTrace + Count;
   c=*(PTrace-2);
   for(i=Count-1;i>0;i--)
     {
       CTrace = *PTrace--;
       if(CTrace <= cbl) break;
       if(CTrace == c) continue;
       if(CTrace > c)  c++;
       else            c--;
       if(CTrace != c) break;
     }
   if(c1) break;
   cbl=(char)(size_y-bl_dw);
   c1 =(char)(Count-i);
   OldTrace += Count;
  }
  Count-=i;
  if((char)Count>c1)  Count=(int16_t)c1;
  if( Count>=DEFIS_LEN  &&
      (*(pint16_t)((pchar)penalty+((size_x-Count)<<1))) <= NAV_BOUND)
    {
     *cut_points+=1;
     cut_points =(pint16_t)((pchar)cut_points + (*cut_points<<1));
     *cut_points = Count;
    }
}

void Alik_cut_l(pint16_t cut_points,pchar trace,int16_t size_x,int16_t bl_dw,pint16_t penalty)
{
pchar PTrace,CurTrace,PtrTrace;
int16_t   i,Count,BLine,NewPoint;
char  c,c1;
pint16_t  CurCP,LastCP,Ppenalty;

 if(*cut_points)
  {
   Count=*cut_points;
   CurCP=cut_points+1;
   LastCP=(pint16_t)((pchar)cut_points + (Count<<1));
   penalty = (pint16_t)((pchar)penalty + (size_x<<1));
   PTrace=trace+size_x;
   BLine=bl_dw-OTSTUP_L;
   for(i=Count;i>0;i--)
    {
      CurTrace=PTrace - *CurCP++;
      c=*CurTrace;
      if(c<BLine) continue;
      PtrTrace=CurTrace-SDVIG_L;
      c1=BLine-SDVIG_H;
ak2:  if(*--CurTrace>c)  continue;
      if(*CurTrace<=c1)
       {
        NewPoint=(int16_t)(PTrace-CurTrace)-2;
        if((*(CurCP-1)+2) < NewPoint)
         {
          Ppenalty=(pint16_t)((pchar)penalty - (NewPoint<<1));
          if(*Ppenalty <= NAV_BOUND)   *++LastCP=NewPoint;
         }
        continue;
       }
     if(PtrTrace < CurTrace) goto ak2;
    }
 *cut_points=((uint16_t)((pchar)LastCP - (pchar)cut_points)>>1);
  }
}

void Alik_del_p_cut(pchar raster,pint16_t cut_points,int16_t size_x,int16_t size_y,
                    int16_t first,char h_row)
{
pchar  PRaster,PRasterOld,Pfb_table;
pint16_t   CurCP;
int16_t    step,i,Count,CP,HgtRaster,HgtRasterOld,Sum;
char   c,c1,flag=0;

 if(*cut_points!=first)
  {
     CurCP=(pint16_t)((pchar)cut_points + ((first+1)<<1));
     Count=*cut_points-first;  step=1;  Pfb_table=fb_table;
     PRaster=raster;   first=((size_y+7)>>3);
     raster+=(uint16_t)first*(size_x-1);  size_y=first<<3;
r1:  for(i=Count;i>0;i--)
      {
        CP=*CurCP++;
        if(CP<0)
            {
               PRasterOld=PRaster=raster + CP*first;
               c=NUMB_STEP;
               HgtRaster=size_y;
               goto r3;
            }
      }
     goto r8;
r1b: if(!(--c)) {flag=1; goto r6;}
     HgtRasterOld=HgtRaster;  PRaster-=first;
//r1c:
     HgtRaster=size_y;   PRasterOld=PRaster;
r3:  c1=*PRaster;
     if(!c1) {HgtRaster-=8; PRaster+=step; goto r3;}
     c1=Pfb_table[(uchar)c1];
     HgtRaster-=c1; PRaster=PRasterOld;  step*=-1;
     if(step<=0) { Pfb_table+=256; PRaster+=first-1; goto r3; }
     c1=(char)HgtRaster;  HgtRaster<<=2;   Pfb_table-=256;
     if(c!=NUMB_STEP)
      {
       HgtRaster-=c1;
       if(HgtRasterOld>HgtRaster) { HgtRaster=HgtRasterOld;  goto r1b; }
       HgtRaster=HgtRasterOld;
       Sum=Alik_count_int_number(PRaster,first);
       if(Sum != 1) goto r1b;
       if(c)  flag=0;
       else   flag=1;
       goto r6;
      }
     if(c1>h_row) goto r1b;
     c=0;flag=1;
r6:  if(!flag) {--CurCP; *CurCP=(int16_t)(*((pchar)CurCP + (i<<1) - 2));}
     if((--i)) { Count=i; goto r1;}
r8:  *cut_points=((uint16_t)((pchar)CurCP - (pchar)cut_points)>>1)-1;
  }
}

void Alik_cut_d(pchar dw_trace,pint16_t cut_points,pchar product,int16_t size_x,
                int16_t size_y,int16_t bl_up,int16_t bl_dw,pchar penalty)
{
pchar  PTrace,Ptr;
pint16_t   CurCP;
int16_t    i,Count,I1;
char   c,c1,Trace,TraceOld;

  size_y -= bl_dw;
  c =(char)(size_y - MIN_D);
  if(c>=0)
   {
    if(c > LEN_D)    c1 = c-LEN_D;
    else             c1=0;
    bl_up=bl_dw-bl_up-3;
    bl_dw=size_y+3;
    PTrace=dw_trace+LEFT_STEP;
    CurCP=(pint16_t)((pchar)cut_points + (*cut_points<<1));
    product+=size_x-1;
    penalty+=(size_x<<1);
    Count=size_x-LEFT_STEP;
    if(Count>0)
     {
      for(i=Count;i>0;i--)
      {
        if((Trace=*PTrace++)>c || Trace<c1)  continue;
        I1=-LEFT_STEP;
        while(1)
         {
          if((char)bl_dw <= *(PTrace+I1-1)) goto ad4d;
          if(!(++I1))            break;
         }
        while(1)
         {
          I1--;
          if((char)size_y < *(PTrace+I1-1))
                                {--I1; break;}
          if(I1 < -3)            break;
         }
        I1=I1-i+1;
ad3:    TraceOld=Trace;
ad3a:   if(!(--i))               break;
        Trace=*PTrace++;
        if(TraceOld>=Trace)      goto ad3;
        if(Trace>*PTrace)        goto ad3a;
        if(TraceOld < c1)        goto ad4b;
        goto ad4a;
ad4:    Trace=*PTrace++;
        if(Trace<=c)             goto ad4a;
        if(TraceOld>Trace)       continue;
        if(TraceOld==Trace)
         {
          I1=-I1-i-2;
          if(I1 <= MIN_FOOT)     continue;
          if(I1 <= MAX_FOOT)     I1=0;
          I1=(I1+(i<<1))>>1;
          Ptr=product-I1;
          while(*--Ptr>*(Ptr-1));
ad8:      I1=(int16_t)(product-Ptr);
          if(I1<MINCOL)          break;
          if(I1<size_x)
           {
             Ptr=penalty-(I1<<1);
             if((uint16_t)*(pint16_t)Ptr <= NAV_BOUND)
                        *++CurCP=I1;
           }
ad4d:     i-=MAX_FOOT; PTrace+=MAX_FOOT;
          if(i<=0)               break;
          continue;
ad4b:     Ptr=product+I1;
          while(*++Ptr>(char)bl_up);  Ptr--;
          while(*++Ptr>=*(Ptr+1) || *Ptr>*(Ptr+2) || *Ptr>*(Ptr+3));
          goto ad8;
         }
        if(Trace >= (char)bl_dw) goto ad4b;
ad4a:   TraceOld=Trace;
        if(--i)                  goto ad4;
        break;
      }
     }
    *cut_points=((uint16_t)((pchar)CurCP - (pchar)cut_points)>>1);
   }
}

int16_t  Alik_sort(pint16_t cut_points,pint16_t res_points,int16_t x)
{
pint16_t  CurCP,OldCP,ResP;
int16_t   CP,left_b,Count,i,I1;

 ResP=res_points; CurCP=cut_points; Count=*cut_points; left_b=x-MINCOL;

 for(i=Count;i>0;i--)
    {
     CurCP++;
     if(*CurCP==0) *CurCP=x;
    }
 CurCP=cut_points;

 while(1)
  {
   OldCP=++CurCP;  I1=0;
   for(i=Count;i>0;i--)
    {
     if((CP=*CurCP++)<0)  CP=-CP;
     if(I1>CP)  continue;
     if(I1==CP) { Count--;  I1=*((pint16_t)((pchar)CurCP+(i<<1)-4)); }
     *(CurCP-1)=I1;
     I1=CP;
    }
   CurCP=OldCP;
   if(I1<MINCOL)    break;
   if(I1<=left_b)  { I1=x-I1;
                     *ResP++=I1; }
   if(!(--Count))  break;
  }
return ((int16_t)((pchar)ResP - (pchar)res_points)>>1);
}

int16_t Alik_search_bound_point_in_raster(pchar raster_tr,int16_t width_byte,int16_t nrows)
{
int16_t    Count,i;
pchar  PRaster;

 Count=width_byte * nrows;
 PRaster=raster_tr+Count-1;
 for(i=Count;i>0;i--)   if(*PRaster--)    return 1;
 return 0;
}

void Alik_tr_bit_matr(int16_t byte_ver,int16_t size_y,pchar buf_in,pchar buf_out,
                      int16_t byte_hor,int16_t size_x)
{
int16_t    ByteHor,i,Count;
pchar  BufOut,Ptr;
uchar   POutCurBit,PNextBit,CurOutByte,CountPicsel,c,c1;

 ByteHor=byte_hor;
 BufOut =buf_out;
 do
   {
     PNextBit=0x80;
     do
       {
         CountPicsel=(char)size_y;
         Ptr=buf_in;
         do
           {
             CurOutByte=0;
             POutCurBit=0x80;
             do
               {
                 if(PNextBit & (*Ptr))
                                  CurOutByte ^= POutCurBit;
                 Ptr+=byte_hor;
                 if(!(--CountPicsel))  break;
               }
               while(POutCurBit>>=1);
             *BufOut++=CurOutByte;
           }
           while(CountPicsel);
       }
       while(PNextBit>>=1);
       buf_in++;
   }
   while(--ByteHor);
   ByteHor=byte_hor;
   Count=(byte_ver<<3)-size_y;
   if(Count)
    {
      Count=8-Count;
      c=0x80; c1=0;
      for(i=Count;i>0;i--)  { c1 |= c; c>>=1;}
      BufOut =buf_out+byte_ver-1;
      for(i=size_x;i>0;i--)
       {
        *BufOut &= c1;
        BufOut+=byte_ver;
       }
    }
}

void Alik_cor_pnt(pint16_t old_points,pint16_t new_points,pint16_t bufer,int16_t np,
                  int16_t x_size,int16_t y_size,int16_t y_byte,pchar raster,pchar trace)
{
int16_t   i,flag;
pint16_t  POldPoints,PNewPoints,PBufer;
pchar Pfb_table,Ptr,PtrOld,PTrace;
char  Ras,c,c1,tmp;

 c = tmp =(uchar) (y_size - ((y_size+7) & 0x0f8) - 8);
 y_size--;
 POldPoints=old_points;
 PBufer=bufer;
 for(i=np;i>0;i--) *PBufer++ = *POldPoints++;
 flag=1;
 PNewPoints=new_points;
 while(1)
  {
      PBufer=bufer++;
      *PNewPoints=*PBufer; PNewPoints+=flag;
      PtrOld=raster + (*PBufer) * y_byte;
      Ptr=PtrOld-y_byte;
      Pfb_table=fb_table;
      c=-8;
bb4:  c1=c;
bb5:  c1+=8;
      if(c1 <= y_size)
       {
        Ras=*Ptr; Ptr+=flag;
        if(!Ras)  goto bb5;
        Ras=Pfb_table[(uchar)Ras];
        Ras+=c1;  flag=1;
        if(Pfb_table!=fb_table) Ras=y_size-Ras;
        *PNewPoints++=Ras;
        if(Pfb_table!=fb_table) goto bb10;
        Pfb_table+=256; flag=-1;
        c=tmp;
        Ptr=PtrOld; Ptr--;
        goto bb4;
       }
      PTrace=trace + *(PNewPoints-1) -1;
      flag=1;
      *PNewPoints++= (int16_t)*PTrace;
      *PNewPoints++= (int16_t)(y_size - *(PTrace + x_size));
bb10: if(!(--np)) break;
  }
 *PNewPoints=x_size;
}

int16_t  Alik_del_doubl_cut(pint16_t cut_points,pchar product,pint16_t penalty,
                        int16_t npoints)
{
int16_t   dob,i,Count,CP;
char  c,c1;
pint16_t  CurCP,PPen,Ptr;
pchar PProd,PProdNew;

 if(npoints <= 1)    return npoints;
 Count=npoints; c1=(char)npoints; --Count; PProd=product-4; CurCP=cut_points;
    do
      {
       CP = *CurCP++;
       if((++CP) != *CurCP)      continue;
       PProdNew=PProd+(++CP);  c=*(PProdNew+1);
       if(Count==1 || CP != *(CurCP+1))
        {
         if(c <= *(PProdNew+2))
          {
           if(c < *(PProdNew+2))                                goto u2a;

	   PPen=(pint16_t)((pchar)penalty + (CP=(CP<<1)));
           if(*PPen <= *(PPen+1))
            {
             if(*PPen < *(PPen+1) || *PProdNew < *(PProdNew+3)) goto u2a;
             if(*PProdNew == *(PProdNew+3))
              {
               if(*(PProdNew-1) == *(PProdNew+4))  continue;
               if(*(PProdNew-1) < *(PProdNew+4))
                {
u2a:             CurCP++; dob=0;
                 if(--Count)                                    goto u3a;
                 --c1;      break;
                }
              }
            }
          }
u3:      dob=2;
u3a:     for(i=Count,Ptr=--CurCP;i>0;i--)
                           {*Ptr = *(Ptr+1); Ptr++;}
         CurCP=(pint16_t)((pchar)CurCP + dob);  --c1;  continue;
        }
u1a0:  if(c != *(PProdNew+2))
        {
         if(c <  *(PProdNew+2))                                 goto u2a;
         else                                                   goto u3;
        }
       if(c != *(PProdNew+3))
        {
         if((c=(c>>2)) <  *(PProdNew+3))
          {
           if((c=*(PProdNew+3)>>2) >= *(PProdNew+2))            goto u1a1;
           CurCP++;   --Count;   PProdNew++;                    goto u1a0;
          }
         for(i=Count,Ptr=--CurCP;i>0;i--) {*Ptr = *(Ptr+1); Ptr++;}
         --c1;  --Count; CurCP++;                               goto u3;
        }
u1a1:  dob=2;
       do
        {
         for(i=Count,Ptr=--CurCP;i>0;i--) {*Ptr = *(Ptr+1);  Ptr++;}
         if(!(--dob))     break;
         CurCP+=2; c1-=2;
        } while(Count-=2);
       if(dob)   break;
      } while (--Count);
 return (int16_t)c1;
}

int16_t  Alik_del_detail(pchar bufer,int16_t rx,int16_t vol_raster,pint16_t penalty)
{
pint16_t   WPen,WPenNew;
pchar  PBuf,PBuf1,CurBound,Ptr;
char   Proxod=6,flag,ctmp,index=0;
int16_t    i,ByteLenStr,BitLenStr,I2,I1;
uchar   c;

 ++penalty;
do
 {                                           /*a0:*/
 PBuf=bufer; WPen=penalty; BitLenStr=rx; ByteLenStr=(rx+7)>>3; c=0x80;flag=1;
 if(index & 1)
  {
   flag=0;
   WPen=(pint16_t)((pchar)WPen+((rx-2)<<1));
   PBuf+=ByteLenStr-1;
   if(BitLenStr &= 7)     c=256>>BitLenStr;
   else                   c=1;
  }
 if(index & 2)                               /*a1:*/
  {PBuf-=ByteLenStr; ByteLenStr=-ByteLenStr; PBuf1=PBuf; PBuf+=vol_raster;}
 else  PBuf1=PBuf+vol_raster;                /* a2 */
 CurBound=PBuf1; ctmp=0;                     /*a2aa:*/
 if(index >= 2)
   while(1)
    {
     PBuf1-=ByteLenStr;                      /*a2c:*/
     if(PBuf1 == PBuf) break;
     if(*PBuf1 & c)  ctmp=1;                 /*a2a:*/
     else  if(ctmp)  break;                  /*a2b:*/
    }
 else PBuf1=PBuf;


  I1=0;  WPenNew=WPen;                      /*a2d:*/
  for(i=rx;i>0;--i)
   {
    if(*PBuf1 & c)                           /*a3:*/
     do
      {
       if(PBuf1==PBuf) {I1=0; goto a10a;}   /*a5:*/
       PBuf1-=ByteLenStr;
      }while(*PBuf1 & c);
    else
      {
       do
        {
         PBuf1+=ByteLenStr;                  /*a4:*/
         if(PBuf1==CurBound) return 0;       /*a4a:*/
        }while(!(*PBuf1 & c));               /*a4b:*/
       PBuf1-=ByteLenStr;
      }
    Ptr=PBuf1; I2=I1;                       /*a6:*/
    while(Ptr != PBuf)                      /*a8:*/
     {
      Ptr-=ByteLenStr;                      /*a7:*/
      if(*Ptr & c) { I1++; *Ptr ^= c; }
     }
     if((uint16_t)I1<=(uint16_t)I2) {I1=0; goto a10a;}  /*a9:*/
     *WPenNew+=I1;                          /*a10:*/
     if(I1 & 0x8000) I1=10*NAV_BOUND;
     I1<<=1;
a10a: if(flag){++WPenNew; if(!(c>>=1)) {c=0x80; ++PBuf; ++PBuf1; ++CurBound;}}
      else    {--WPenNew; if(!(c<<=1)) { c=1;   --PBuf; --PBuf1; --CurBound;}}
   }

  if(Proxod==3)     index=0;
  else
   {
    if(Proxod>3)    ++index;
    else            index+=2;
   }
 } while(--Proxod);
 return 1;
}

/* LeftDistance - Ю═ААБ╝О╜╗╔ ╓╝ ╞╔Ю╒╝ё╝ А╚╔╒═ ║╗Б═			*/
int16_t LeftDistance(uchar *RASTER, int16_t NWIDTH)
{
int16_t i;

for(i=0;i<NWIDTH && (*RASTER)==0;i++,RASTER++);

if( i==NWIDTH)
	return(-1);

return( (i<<3)+start_pos[*RASTER] );
}

/* RightDistance -Ю═ААБ╝О╜╗╔ ╓╝ ╞╔Ю╒╝ё╝ А╞Ю═╒═ ║╗Б═			*/
int16_t RightDistance(uchar *RASTER, int16_t NWIDTH)
{
int16_t i;

RASTER += NWIDTH-1;

for(i=0;i<NWIDTH && (*RASTER)==0;i++,RASTER--);

if( i==NWIDTH)
	return(-1);

return( (i<<3)+last_pos[*RASTER] );
}



