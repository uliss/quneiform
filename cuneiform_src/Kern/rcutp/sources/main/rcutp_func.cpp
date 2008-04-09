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

#include <stdlib.h>
#include <string.h>

#include "nt_types.h"
#include "struct.h"
#include "cutstr.h"
#include "alik_rec.h"
#include "interval.h"
#include "diskrtab.h"
#include "recdefs.h"

extern  Int16      minrow,bbs1,bbs2,bbs3,bbs4,Nb1,Nb2,Nb3;
#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif


void snap_clear_screen(void);

extern BYTE  db_status;              // snap presence byte
INT up_position,dw_position;
static INT Alik_count_int_number(PCHAR Praster,INT len_row);
INT LeftDistance(BYTE *RASTER, INT NWIDTH);
INT RightDistance(BYTE *RASTER, INT NWIDTH);

static CHAR table_int[256]=
       {0,1,1,1,1,2,1,1,1,2,2,2,1,2,1,1,1,2,2,2,2,3,2,2,1,2,2,2,1,2,1,1,
        1,2,2,2,2,3,2,2,2,3,3,3,2,3,2,2,1,2,2,2,2,3,2,2,1,2,2,2,1,2,1,1,
        1,2,2,2,2,3,2,2,2,3,3,3,2,3,2,2,2,3,3,3,3,4,3,3,2,3,3,3,2,3,2,2,
        1,2,2,2,2,3,2,2,2,3,3,3,2,3,2,2,1,2,2,2,2,3,2,2,1,2,2,2,1,2,1,1,
        1,2,2,2,2,3,2,2,2,3,3,3,2,3,2,2,2,3,3,3,3,4,3,3,2,3,3,3,2,3,2,2,
        2,3,3,3,3,4,3,3,3,4,4,4,3,4,3,3,2,3,3,3,3,4,3,3,2,3,3,3,2,3,2,2,
        1,2,2,2,2,3,2,2,2,3,3,3,2,3,2,2,2,3,3,3,3,4,3,3,2,3,3,3,2,3,2,2,
        1,2,2,2,2,3,2,2,2,3,3,3,2,3,2,2,1,2,2,2,2,3,2,2,1,2,2,2,1,2,1,1};


static CHAR sum_table[256]= /* ¤®«¦­  ЇаҐ¤иҐбвў®ў вм в Ў«ЁжҐ fb_table  !!!!  */
      {0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,
       1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
       1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
       2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
       1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
       2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
       2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
       3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8};

static CHAR fb_table[512]=  /* в Ў«Ёж  ­®¬Ґа  б ¬®Ј® «Ґў®Ј® Ґ¤Ё­Ёз­®Ј® ЎЁв  */
     {0,7,6,6,5,5,5,5,4,4,4,4,4,4,4,4,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
      2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
      1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
      1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                    /* в Ў«Ёж  ­®¬Ґа  б ¬®Ј® Їа ў®Ј® Ґ¤Ё­Ёз­®Ј® ЎЁв */
      0,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
      5,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
      6,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
      5,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
      7,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
      5,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
      6,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
      5,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0};

void Alik_rus_bad_c(PCHAR raster_frag,PCHAR SourceRaster,INT hgt,INT rx_bite,
                    PCHAR product,PCHAR product_two,PINT penalty,
		    PINT cut_points,PBYTE CountCut,PBYTE UpBlackPoint)
{
#ifdef Alikc
CHAR snap[380],*buf=snap;
#endif
PBYTE PtrCountCut;
INT   i,Left,Right,CP;
PINT  Pcut_points,Pen;
PCHAR prod_two;

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
 *cut_points=(WORD)(Pcut_points-cut_points);
}

BYTE Alik_check_c(PBYTE PtrCountCut,PINT Pen,INT Right)
{
INT i;

 PtrCountCut+=Right;
 Pen+=Right;

 for(i=0; i< NUMB_STEP; i++,--PtrCountCut,--Pen)
  {
   if(*PtrCountCut!=1 || (WORD)*Pen>(WORD)*(Pen-1)) return 0;
  }
 return 1;
}

void Alik_Check_Rus_D(PCHAR raster_frag,PCHAR SourceRaster,INT dy,INT dx,
                      PINT cut_points,PBYTE IntBuf,INT row,PCHAR product,
		      PCHAR product_two,PINT penalty,PBYTE CountCut)
{
#ifdef Alikd
CHAR snap[380],*buf=snap;
#endif

BYTE  SumCol=0,Col,*CurIntBuf,lb,rb,lb1,rb1,nlb,ColOst,ColOsn;
PCHAR CurPos;
INT   i,j,*CurCP,hor_byte,up_ot_bbs3=0,CP,dif;

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
   /*  ‚лзҐб«пҐвбп ®б­®ў ­ЁҐ „ */
#ifdef Alikd
   if(db_status && snap_activity('j'))
      {
       buf=snap;
       buf+=SPRINTF(buf,"       Ћб­®ў ­ЁҐ „ \n");
      }
#endif

  while(up_ot_bbs3<3)
   {
    CurIntBuf=IntBuf;
    CurPos   =SourceRaster+(dy-(i+up_ot_bbs3))*hor_byte;
    Col=SumCol=Alik_find_black_interval((PBYTE)CurPos,(BYTE)hor_byte,CurIntBuf);
    CurPos+=hor_byte;

    for(j=0,ColOsn=0;j<Col;j++)
     {
      lb=*(IntBuf+(j<<1));
      rb=*(IntBuf+(j<<1)+1);

      if(j<Col-1)
        {
	  /*бЄ«ҐЁў Ґ¬ б«Ґ¤. Ё­вҐаў « Ґб«Ё а бв®п­ЁҐ ¬ «.*/
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

/* “¤ «по Ї«®еЁҐ гз бвЄЁ ¤«п ®б­®ў ­Ёп „*/
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

     /*  ‚лзҐб«повбп « ЇЄЁ „ */
     CurPos   =SourceRaster + dy*hor_byte;

     for(j=1;j<i;j++)
      {
       CurPos-=hor_byte;
       CurIntBuf=IntBuf+(SumCol<<1);
       SumCol+=Alik_find_black_interval((PBYTE)CurPos,(BYTE)hor_byte,CurIntBuf);
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

         /*  ‹Ґў п  « ЇЄ  „ */
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
	 /*  …бвм баҐ¤­пп  « ЇЄ  „„ */
         if(MI[i].cvl && MI[i].cvr &&
	     lb > MI[i].lb + max((MIN_OSNOV_D>>1),(MI[i].rb-MI[i].lb)/3) &&
             rb < MI[i].rb - (MI[i].rb-MI[i].lb)/3)
              {MI[i].mb=lb+((rb-lb)>>1); MI[i].cvm++; goto con;}
         /*  Џа ў п  « ЇЄ  „ */
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
	  /* ¬ ЄбЁ¬ «м­ п иЁаЁ­  ­®ЈЁ */
con:	 MI[i].wf=max(MI[i].wf,rb-lb+1);
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
       buf+=SPRINTF(buf," ¤ left position=%3d ",MI[i].lb-1);
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
         buf+=SPRINTF(buf," ж Ё«Ё й right position=%3d ",MI[i].lb+MI[i].wf);
       else
         if(MI[i].cvr)
            buf+=SPRINTF(buf," ¤ right position=%3d ",MI[i].rb);
         else
            buf+=SPRINTF(buf," ¤ right position=%3d ",MI[i].mb);

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
       buf+=SPRINTF(buf," ¤ seredina=%3d ",MI[i].mb);
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
     *cut_points=(WORD)(CurCP-cut_points);
    }
  }
}

BYTE Alik_del_a(PINT penalty,PBYTE CountCut,BYTE lb,BYTE rb)
{
BYTE Sum,i;

 penalty=penalty+lb;
 CountCut=CountCut+lb;
 for(i=lb,Sum=0;i<rb;i++,penalty++,CountCut++)
  if(*CountCut>2 || *penalty>6)  Sum++;
 if(Sum>((rb-lb)>>1)) return 1;
 else                 return 0;
}

BYTE Alik_del_a_from_interval(PINT penalty,PBYTE CountCut,BYTE lb,BYTE rb,
                              BYTE* lb1,BYTE* rb1)
{
BYTE Sum,i;

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

BYTE  Alik_priznak_del_3_cut(PBYTE CountCut,INT CP,INT dx)
{
PBYTE  CurCutPoint;
INT    i,Sum;

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
                      struct cut_elm *cpnt2,INT height,BYTE WhereCutten)
{
INT begin,end;

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

void Alik_cut_short(PCHAR bufer,PCHAR bufer_two,INT ry,INT rx_bite,
                PCHAR product,PINT penalty, PINT cut_points,PCHAR prod_two)
{
BYTE   flag_seredina;
INT    i,i1,i2,Sum,SumTwo;
PCHAR  Pproduct,PProdTwo,Pbufer,PBufTwo;
PINT   Pcut_points;

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
             if(sum_table[(BYTE)(*PBufTwo & (*(PBufTwo+rx_bite)))] >
                sum_table[(BYTE)(*PBufTwo & (*(PBufTwo-rx_bite)))])
                   *PBufTwo &= *(PBufTwo+rx_bite);
             else  *PBufTwo &= *(PBufTwo-rx_bite);
            }
           else                 *PBufTwo &= *(PBufTwo-rx_bite);

          SumTwo+=sum_table[(BYTE)*PBufTwo++];
        }
      *PProdTwo++ = (CHAR)SumTwo;
      if(PProdTwo >= (prod_two + ry)) break;
    }

   while(1)
    {
      Sum=0;
      for(i=0; i<rx_bite; i++)
        {
         *Pbufer &= *(Pbufer+rx_bite);
          Sum+=sum_table[(BYTE)*Pbufer++];
        }
      *Pproduct++ = (CHAR)Sum;
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
      if((WORD)i2 > NAV_BOUND)
        {
         Pcut_points--;
         i1=1;
        }
f4c:  i1--;
    }
    *cut_points=((WORD)((PCHAR)Pcut_points-(PCHAR)cut_points)>>1);
}

void Alik_find_brus(PCHAR raster_frag,PCHAR SourceRaster,PCHAR bufer,INT ry,
                    INT hgt,INT rx_bite,PCHAR product,PCHAR product_two,
		    PINT penalty,PINT cut_points,PBYTE CountCut,
		    PBYTE UpBlackPoint)
{
#if defined (AlikBrus) || defined (Alikt)
CHAR snap[380],*buf=snap;
#endif
BYTE CountBrus=0,Col=0,flag_point,foot_size,interval,max_h,
     flag_brus,tolshina[MaxHeightBrus],j,n;
INT    i,min_height_t;
PCHAR  Pproduct,Pproduct_two,Pbufer,pp,ppt;
PINT   Pcut_points,PPen;
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
                                           tolshina[(BYTE)*ppt-1]++;

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
         Pproduct_two=product_two+(WORD)CB[j].R+1;
         interval=CB[j+1].L-CB[j].R;

         for(i=0,foot_size=0;i<interval;++i,Pproduct_two++)
          if(*Pproduct_two > min_height_t)  ++foot_size;

         flag_point=(foot_size+((interval+1)>>1))/interval;
         if(flag_point)
          {
           interval=min(CB[j].R-CB[j].L+1,CB[j+1].R-CB[j+1].L+1);
           if(CB[j].P && !CB[j+1].P)    //«
            {
             n=min(2,CB[j].R-CB[j].L);
             for(; n<= CB[j].R-CB[j].L;)
              if((*(product_two+CB[j].R-(n+1))<=*(product_two+CB[j].R-n))
                  && (*(penalty+CB[j].R-(n+1))<=*(penalty+CB[j].R-n))) n++;
              else  break;
              interval=n;
#ifdef AlikBrus
   if( db_status && snap_activity('j'))
      {
       buf=snap; buf+=SPRINTF(buf," « position=%3d ",CB[j].R-interval-1);
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
		if(!Alik_Check_T(bufer,hgt,(INT)(CB[j].R-interval+1),(INT)(CB[j+1].L+interval-1)))
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
		   buf+=SPRINTF(buf," в position1=%3d position2=%3d",CB[j].L,CB[j+1].R);
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

  *cut_points=(WORD)(Pcut_points-cut_points);
}

BYTE Alik_kill_right_points(INT dx,PCHAR prod,PCHAR prod2,PINT cut_points,
                            INT CP)
{
BYTE flag_point;
INT  i,j,x,*Ptr;

 x=dx-CP;
 flag_point=0;
 i=*cut_points;
 Ptr=cut_points+1;
 for(j=0; j<i; j++,Ptr++)
  if(x-*Ptr>=0 && x-*Ptr<=2 && *(prod2+(dx-*Ptr))>1 && *(prod+(dx-*Ptr-1)))
    {*Ptr=x; flag_point=1;}

 return flag_point;
}

BYTE Alik_kill_left_points(INT dx,PINT cut_points,INT CP)
{
BYTE flag_point;
INT  i,j,x,*Ptr;

 x=dx-CP;
 flag_point=0;
 i=*cut_points;
 Ptr=cut_points+1;
 for(j=0; j<i; j++,Ptr++)
  if(*Ptr-x>=0 && *Ptr-x<=2)
    {*Ptr=x; flag_point=1;}

 return flag_point;
}

BYTE Alik_kill_left_right_points(PINT cut_points,INT CP)
{
BYTE flag_point;
INT  i,j,*Ptr;

 flag_point=0;
 i=*cut_points;
 Ptr=cut_points+1;
 for(j=0; j<i; j++,Ptr++)
  if(abs(*Ptr-CP)<=3)
    {*Ptr=CP; flag_point=1;}

 return flag_point;
}

INT Alik_set_position_brus(PCHAR bufer,INT hgt,INT Left,INT Right)
{
PCHAR CurPos;
INT   i,ver_byte,interval,l1,r1,minl,maxl,minr,maxr,tret;

 tret=hgt/3;
 ver_byte = (hgt+7)>>3;
 interval = Right-Left;
 CurPos   = bufer + Left * ver_byte;

 for(i=0; i<=interval; i++,CurPos+=ver_byte)
  {
   l1=LeftDistance((PBYTE)CurPos,ver_byte);
   r1=RightDistance((PBYTE)CurPos,ver_byte);
   if(l1<0 || r1<0) return -1;
   if(i>0)
    {
     minl=min(minl,l1); minr=min(minr,r1);
     maxl=max(maxl,l1); maxr=max(maxr,r1);
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

INT Alik_Check_T(PCHAR bufer,INT hgt,INT Left,INT Right)
{
PCHAR CurPos;
INT   i,ver_byte,interval,dist,mind,maxd;

 ver_byte = (hgt+7)>>3;
 interval = Right-Left;
 CurPos   = bufer + Left*ver_byte;

 for(i=0; i<=interval; i++,CurPos+=ver_byte)
  {
   dist=LeftDistance((PBYTE)CurPos,ver_byte);
   if(i>0) { mind=min(mind,dist);  maxd=max(maxd,dist); }
   else      mind=maxd=dist;
  }

 if(maxd-mind>MaxHeightBrus)  return 0;
 else                         return 1;
}


void Alik_cut_y(PCHAR raster_frag,PCHAR SourceRaster,PCHAR bufer,INT dy,
                INT dx,PINT cut_points,PBYTE IntBuf,INT row,PCHAR product,
		PCHAR product_two,PINT penalty,PBYTE CountCut)
{
#ifdef Aliky
CHAR snap[380],*buf=snap;
#endif

BYTE  Col,*CurIntBuf,lb,rb,left,right,*SumStr,l_bound,r_bound,flag_near;
CHAR  min_prod_two;
PCHAR CurPos,prod,prod_two;
INT   i,j,k,hor_byte,ver_byte,dw_ot_bbs3=3,width_tail,*pen,*CurCP,flag_y,
      LeftDist,RightDist;
WORD  min_pen;

 left=right=flag_near=0;
 raster_frag=raster_frag;
 hor_byte =(dx+7)>>3;
 ver_byte =(dy+7)>>3;
 i=(row+dy)-(minrow+bbs3);
 if(i>=5)
  {
   if(i==5)  dw_ot_bbs3=2;
   /*  ‚лзҐб«пҐвбп еў®бвЁЄ ®в “ */
#ifdef Aliky
   if(db_status && snap_activity('j'))
      {
       buf=snap;
       buf+=SPRINTF(buf,"       •ў®бвЁЄ “ \n");
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
 Col=Alik_find_black_interval(SumStr,(BYTE)hor_byte,CurIntBuf);

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
/*************     ЋЇаҐ¤Ґ«пҐ¬ «Ґўго в®зЄг      *************/
  if(left >= MINCOL)
   {
    l_bound=max(0,left-1);
    r_bound=max(l_bound,right - LEFT_STEP);
    pen=penalty  + l_bound;
    prod_two = product_two + l_bound;
    prod = product + l_bound;
    min_prod_two=*prod_two;
    min_pen=(WORD)(*pen);
    for(i=l_bound; i<r_bound; i++,pen++,prod_two++,prod++)
     {
      if(*prod_two < min_prod_two ||  *prod < min_prod_two ||
        (*prod_two == min_prod_two && (WORD)*pen < min_pen))
       {
        left=(BYTE)i;
        min_prod_two=min(*prod_two,*prod);
        min_pen=(WORD)(*pen);
       }
     }
    if(left >= MINCOL) { *++CurCP=dx-left; *cut_points+=1;}
   }
  CurPos  =bufer + left*ver_byte;
  LeftDist=LeftDistance((PBYTE)CurPos,ver_byte);
/*************     ЋЇаҐ¤Ґ«пҐ¬ Їа ўго в®зЄг      *************/
  if(dx-right >= MINCOL)
   {
    l_bound=min(dx,right + Y_CUT);
    r_bound=min(dx,l_bound + Y_CUT + LEFT_STEP);
    right=l_bound;
    pen=penalty  + l_bound;
    prod_two = product_two + l_bound;
    prod     = product + l_bound;
    min_prod_two=*prod_two;
    min_pen=(WORD)(*pen);
    for(i=l_bound; i<r_bound; i++,pen++,prod_two++,prod++)
     {
      if(*prod_two < min_prod_two || *prod < min_prod_two ||
        (*prod_two == min_prod_two && (WORD)*pen < min_pen))
        {
         right=(BYTE)i;
         min_prod_two=min(*prod_two,*prod);
         min_pen=(WORD)(*pen);
        }
      CurPos =bufer + i*ver_byte;
      RightDist=LeftDistance((PBYTE)CurPos,ver_byte);
      if(abs(RightDist-LeftDist)<=1) flag_near++;
      if(flag_near>5) {right=(BYTE)i; break;}
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


BYTE Alik_find_black_interval(PBYTE str,BYTE len,PBYTE intrvl)
{
BYTE  Flag_P=0,Col=0;
INT   i,j,offset;
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

void Alik_form_bound(PCHAR raster,WORD ry,WORD rx,WORD xbytes,
                     PCHAR adr_bound,WORD prizn)
{
 INT    i,one=1,tmp_xbytes;
 PCHAR  Praster,Praster1;
 CHAR   c1,Ah,Al,*Ptable,crx;

  tmp_xbytes = xbytes;
  c1 = -8;
  Ptable=fb_table;
  Praster1=raster;
  crx=(CHAR)rx;

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
      Al=Ptable[(BYTE)Al]+Ah;
j3b:  if(prizn != 0  && !tmp_xbytes)  Al=rx-Al;
      *adr_bound++ =Al;
      Praster=(PCHAR)Praster1 + i*xbytes;
     }
     if(!tmp_xbytes)  break;
     Ptable += 256;
     Praster1 = (PCHAR)raster + xbytes-1;
     c1=(CHAR)((-1)*((xbytes<<3)-rx+8));
     one = -1;  tmp_xbytes=0;   rx--;
   }
}

/******************************************************************/
#pragma warning( disable : 4245 )					 //AK. 07.03.97
void  Alik_cut_hole(PCHAR trace,INT rx,PINT cut_points,INT first,INT ry,
                    PCHAR product,PINT penalty,BYTE ind)
{
INT   i;
BYTE  HRastr;
PINT  Pcut_points,Pcut_points1,Ppenalty,Ppenalty1;
PCHAR Pproduct,Pproduct1,Ptrace,Ptrace1;
BYTE  Comp,AL;
WORD  DX,DI,Offset,Count;
   penalty=penalty;
   first=first;
   HRastr = (BYTE)((ry*3)>>2);
b0:
   /* бд®а¬Ёа®ў «Ё  ¤аҐб ¤«п еа ­Ґ­Ёп в®зҐЄ а §аҐ§ */
   Pcut_points=Pcut_points1=(PINT)((PCHAR)cut_points+((INT)(*cut_points)<<1));
   Ptrace = Ptrace1 = trace;
   Pproduct = Pproduct1 = product + rx-1;
   Ppenalty = Ppenalty1 = (PINT)((PCHAR)penalty + ((INT)rx<<1));

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
   Ppenalty  = (PINT)((PCHAR)Ppenalty-(DX<<1));
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
    if(*Ppenalty <= min(NAV_BOUND,DI))
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
   Offset = (INT)((PCHAR)Pcut_points - (PCHAR)Pcut_points1);
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
   *cut_points =((WORD)((PCHAR)Pcut_points-(PCHAR)cut_points)>>1);
   if(!ind)
    {
     trace += rx;
     Alik_del_y_cut(trace,rx,cut_points,first);
     ind=1;
     goto b0;
    }
}

void  Alik_del_y_cut(PCHAR trace,INT rx,PINT cut_points,INT first)
{
PINT  Pcut_points,Ptr;
PCHAR BegTrace,CurTrace,TraceEnd;
INT   Count,i,j;

 if((Count=*cut_points-first)>0)
   {
    Pcut_points =(PINT)((PCHAR)cut_points + ((first+1)<<1));
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
   *cut_points =((WORD)((PCHAR)Pcut_points-(PCHAR)cut_points)>>1)-1;
   }
}

void Alik_del_equal_hole(PINT cut_points,PCHAR product,PINT penalty,
                         INT size_x,INT size_y,INT first)
{
PINT  CurCutPoints,LastCutPoints,FindCutPoints,last,Ppenalty;
PCHAR Pproduct;
INT   Count,LastCount,i,NextCutPoint,cur_sdvig,abs_sdvig;

 if((Count = *cut_points - first) > 0)
  {
   last = (PINT)((PCHAR)cut_points + (*cut_points<<1));
   LastCutPoints = cut_points;
   size_y >>=1;
   product += size_x-1;
   penalty = (PINT)((PCHAR)penalty + (size_x<<1));
   CurCutPoints = (PINT)((PCHAR)cut_points + (first<<1));
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
       Ppenalty = (PINT)((PCHAR)penalty - (NextCutPoint<<1));
       Count=size_y;
       while(1)
        {
         if(*Pproduct <= (CHAR)Count && *Ppenalty <= NAV_BOUND) goto at4;
         if(abs_sdvig == MAX_SDVIG)
          {
          if(cur_sdvig == 1)  break;
          NextCutPoint -= MAX_SDVIG;
          Pproduct += MAX_SDVIG;
          Ppenalty = (PINT)((PCHAR)Ppenalty + (MAX_SDVIG<<1));
          cur_sdvig = 1;  abs_sdvig = 0;
          }
         abs_sdvig++;
         NextCutPoint -= cur_sdvig;
         *(CurCutPoints-1) = NextCutPoint;
         Pproduct += cur_sdvig;
         Ppenalty = (PINT)((PCHAR)Ppenalty + (cur_sdvig<<1));
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
      Count = ((WORD)((PCHAR)last-(PCHAR)CurCutPoints)>>1);
      *CurCutPoints = Count;
      if(first>=0 || Count<=1) break;
      LastCutPoints = CurCutPoints;
      first=1;
    }
  }
}

void Alik_del_3_cut(PCHAR raster,PINT cut_points,INT first,INT len_row,
                    INT nrow,PBYTE CountCut)
{
PINT   CurCutPoints,Ptr;
INT    Count,i,j,SumCutPoints;
PCHAR  Praster;

 if((Count = *cut_points - first) > 0)
  {
    CurCutPoints  =(PINT)((PCHAR)cut_points + ((first+1)<<1));
    raster += (WORD)len_row * (nrow-1);
    for(i=Count;i>0;i--)
     {
      Ptr = CurCutPoints++;
      Praster = raster - (WORD)(*Ptr) * len_row;
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
 *cut_points =((WORD)((PCHAR)CurCutPoints-(PCHAR)cut_points)>>1)-1;
  }
}

INT Alik_count_int_number(PCHAR Praster,INT len_row)
{
INT SumCutPoints,j;
CHAR   c,c1;
PCHAR  Ptable_int;

  Ptable_int = table_int;
  SumCutPoints=0; c=0;
  for(j=len_row; j>0; j--)
   {
    if(((c1=*Praster++) & 0x80) && (c & 1)) SumCutPoints--;
    c=c1;
    SumCutPoints+=Ptable_int[(BYTE)c1];
   }
  return SumCutPoints;
}

void Alik_CountCut(PCHAR bufer,INT dy,INT dx,PBYTE  CountCut,PBYTE IntBuf)
{
INT    i,ver_byte;
 ver_byte=(dy+7)>>3;
 for(i=0; i<dx; i++)
   {
    *CountCut++=Alik_find_black_interval((PBYTE)bufer,(BYTE)ver_byte,IntBuf);
    bufer+=ver_byte;
   }
}

//upp
void Alik_UpBlackPoint(PCHAR bufer,INT dy,INT dx,PBYTE  UpBlackPoint)
{
INT    i,ver_byte;
 ver_byte=(dy+7)>>3;
 for(i=0; i<dx; i++)
   {
    *UpBlackPoint++=(BYTE)LeftDistance((PBYTE)bufer,ver_byte);;
    bufer+=ver_byte;
   }
}

#define SERIF_POGRESHNOST     8
#define SERIF_POGRESHNOST_IN  4

INT Alik_up_position_double_serif(PBYTE bufer,INT dy,INT dx,
                                  PWORD penalty,PBYTE IntBuf,INT hgt)
{
INT    i,ver_byte,pos,hgt_bbs;
PWORD  PPen;
BYTE   I_begin,I_end,II_begin,II_end,in_up,in_dw,out_up,out_dw;
PBYTE  CurPos;

 CurPos=bufer;
 PPen=penalty;
 ver_byte=(dy+7)>>3;
 hgt_bbs=min(bbs3-bbs2,hgt);
 up_position=0; dw_position=0;
 in_up=0; in_dw=0; out_up=0; out_dw=0;

 for(i=0; i<dx; i++,PPen++)
  {
   pos=Alik_find_black_interval(CurPos,(BYTE)ver_byte,IntBuf);
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
void Alik_my_correct_base_lines(PINT bl_up,PINT bl_dw,INT dy,INT dx,INT row,
        PBYTE bufer,PWORD penalty,PBYTE IntBuf)
{
INT hgt;
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

void Alik_correct_base_lines(PCHAR trace,INT size_x,INT size_y,INT* adr_ubl,
                             INT* adr_lbl)
{
PCHAR PTrace,Ptr;
INT*  Adr;
CHAR  c,c1;
WORD  CountPoints,i,Chastnoe,c2;

  PTrace = trace;
  *adr_lbl = size_y - *adr_lbl;
  Adr = adr_ubl;
  while(1)
   {
    c=(CHAR)(*Adr)+TO_CENTER;
    if(Adr==adr_ubl) c1=(CHAR)(*Adr)-FROM_CEN_UP;
    else             c1=(CHAR)(*Adr)-FROM_CEN_DW;
    if(c1<0)  c1=0;
    c2=0; CountPoints=0;
    for(i=size_x;i>0;i--)
     {
      Ptr=PTrace++;
      if((BYTE)*Ptr>(BYTE)c || (BYTE)*Ptr<c1) continue;
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


void Alik_double_serif(PINT cut_points,PCHAR trace,PCHAR product,INT rx,
                       INT ry,INT bl_up,INT bl_dw,PINT penalty,
		       PBYTE bufer,PBYTE IntBuf)
{
PINT   BegCP,CurCP,OldCP,Ppenalty;
CHAR   BasLin,Granisa;
BYTE   c,c1,*CurPos,pos;
PCHAR  PTrace,Pproduct;
INT    i,j,BX,tmp,ver_byte;

  ver_byte=(ry+7)>>3;
  BegCP = OldCP = CurCP = (PINT)((PCHAR)cut_points + (*cut_points<<1));
  PTrace = trace;
  c=(BYTE)((bl_dw-bl_up)>>1);
  bl_dw = ry-bl_dw;
  c1=0xFE;
  Pproduct=product;
  Ppenalty=penalty;
  for(i=rx-1; i>0; i--)
   {
     Ppenalty=++penalty;
     if((BYTE)*Pproduct > c)    goto v9;
     if((WORD)*Ppenalty > NAV_BOUND)  goto v9a;
     BasLin = (CHAR)bl_dw;     BX=rx;
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
     BasLin=(CHAR)bl_up;
     if(!(BX-=rx)) goto v1a;
     if((BYTE)*Pproduct>(c1+1)) goto v9;  //06-24-96 08:40pm
     CurCP=BegCP;
     if((BYTE)*Pproduct < c1)             //06-24-96 08:41pm
        {c1=(BYTE)*Pproduct; CurCP=OldCP;}
     *++CurCP=i;   BegCP=CurCP;
     goto v9a;
v9:  if(c1==0xFE) goto v9a;
     c1=0xFE;
     tmp=(INT)((PCHAR)OldCP-(PCHAR)CurCP);
     if(tmp<-4)
      {
       CurCP=(PINT)((PCHAR)CurCP+tmp)+1;
       *CurCP=(*CurCP+i+2)>>1;

       CurPos=bufer+(rx-i)*ver_byte;
       for(j=0; j<STEP_SER; j++)
        {
	 pos=Alik_find_black_interval(CurPos,(BYTE)ver_byte,IntBuf);
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
 *cut_points=((WORD)((PCHAR)CurCP - (PCHAR)cut_points)>>1);
}

void Alik_del_equal(PINT cut_points)
{
PINT LastCP,CurCP,Ptr;
INT  i,j,Count,FindCP;

 Count=*cut_points;
 if(Count>1)
  {
   CurCP=cut_points + 1;
   LastCP=(PINT)((PCHAR)cut_points + (Count<<1));
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
end: *cut_points=((WORD)((PCHAR)LastCP - (PCHAR)cut_points )>>1);
  }
}

void Alik_defis(PCHAR trace,PINT cut_points,INT size_x,INT size_y,
                INT bl_up,INT bl_dw,PINT penalty)
{
PCHAR PTrace,OldTrace;
INT   i,Count;
CHAR  c,c1,CTrace,cbl;

 OldTrace = trace-2;
 Count = size_x;
 cbl=(CHAR)bl_up; c1=0;
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
   cbl=(CHAR)(size_y-bl_dw);
   c1 =(CHAR)(Count-i);
   OldTrace += Count;
  }
  Count-=i;
  if((CHAR)Count>c1)  Count=(INT)c1;
  if( Count>=DEFIS_LEN  &&
      (*(PINT)((PCHAR)penalty+((size_x-Count)<<1))) <= NAV_BOUND)
    {
     *cut_points+=1;
     cut_points =(PINT)((PCHAR)cut_points + (*cut_points<<1));
     *cut_points = Count;
    }
}

void Alik_cut_l(PINT cut_points,PCHAR trace,INT size_x,INT bl_dw,PINT penalty)
{
PCHAR PTrace,CurTrace,PtrTrace;
INT   i,Count,BLine,NewPoint;
CHAR  c,c1;
PINT  CurCP,LastCP,Ppenalty;

 if(*cut_points)
  {
   Count=*cut_points;
   CurCP=cut_points+1;
   LastCP=(PINT)((PCHAR)cut_points + (Count<<1));
   penalty = (PINT)((PCHAR)penalty + (size_x<<1));
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
        NewPoint=(INT)(PTrace-CurTrace)-2;
        if((*(CurCP-1)+2) < NewPoint)
         {
          Ppenalty=(PINT)((PCHAR)penalty - (NewPoint<<1));
          if(*Ppenalty <= NAV_BOUND)   *++LastCP=NewPoint;
         }
        continue;
       }
     if(PtrTrace < CurTrace) goto ak2;
    }
 *cut_points=((WORD)((PCHAR)LastCP - (PCHAR)cut_points)>>1);
  }
}

void Alik_del_p_cut(PCHAR raster,PINT cut_points,INT size_x,INT size_y,
                    INT first,CHAR h_row)
{
PCHAR  PRaster,PRasterOld,Pfb_table;
PINT   CurCP;
INT    step,i,Count,CP,HgtRaster,HgtRasterOld,Sum;
CHAR   c,c1,flag=0;

 if(*cut_points!=first)
  {
     CurCP=(PINT)((PCHAR)cut_points + ((first+1)<<1));
     Count=*cut_points-first;  step=1;  Pfb_table=fb_table;
     PRaster=raster;   first=((size_y+7)>>3);
     raster+=(WORD)first*(size_x-1);  size_y=first<<3;
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
     c1=Pfb_table[(BYTE)c1];
     HgtRaster-=c1; PRaster=PRasterOld;  step*=-1;
     if(step<=0) { Pfb_table+=256; PRaster+=first-1; goto r3; }
     c1=(CHAR)HgtRaster;  HgtRaster<<=2;   Pfb_table-=256;
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
r6:  if(!flag) {--CurCP; *CurCP=(INT)(*((PCHAR)CurCP + (i<<1) - 2));}
     if((--i)) { Count=i; goto r1;}
r8:  *cut_points=((WORD)((PCHAR)CurCP - (PCHAR)cut_points)>>1)-1;
  }
}

void Alik_cut_d(PCHAR dw_trace,PINT cut_points,PCHAR product,INT size_x,
                INT size_y,INT bl_up,INT bl_dw,PCHAR penalty)
{
PCHAR  PTrace,Ptr;
PINT   CurCP;
INT    i,Count,I1;
CHAR   c,c1,Trace,TraceOld;

  size_y -= bl_dw;
  c =(CHAR)(size_y - MIN_D);
  if(c>=0)
   {
    if(c > LEN_D)    c1 = c-LEN_D;
    else             c1=0;
    bl_up=bl_dw-bl_up-3;
    bl_dw=size_y+3;
    PTrace=dw_trace+LEFT_STEP;
    CurCP=(PINT)((PCHAR)cut_points + (*cut_points<<1));
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
          if((CHAR)bl_dw <= *(PTrace+I1-1)) goto ad4d;
          if(!(++I1))            break;
         }
        while(1)
         {
          I1--;
          if((CHAR)size_y < *(PTrace+I1-1))
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
ad8:      I1=(INT)(product-Ptr);
          if(I1<MINCOL)          break;
          if(I1<size_x)
           {
             Ptr=penalty-(I1<<1);
             if((WORD)*(PINT)Ptr <= NAV_BOUND)
                        *++CurCP=I1;
           }
ad4d:     i-=MAX_FOOT; PTrace+=MAX_FOOT;
          if(i<=0)               break;
          continue;
ad4b:     Ptr=product+I1;
          while(*++Ptr>(CHAR)bl_up);  Ptr--;
          while(*++Ptr>=*(Ptr+1) || *Ptr>*(Ptr+2) || *Ptr>*(Ptr+3));
          goto ad8;
         }
        if(Trace >= (CHAR)bl_dw) goto ad4b;
ad4a:   TraceOld=Trace;
        if(--i)                  goto ad4;
        break;
      }
     }
    *cut_points=((WORD)((PCHAR)CurCP - (PCHAR)cut_points)>>1);
   }
}

INT  Alik_sort(PINT cut_points,PINT res_points,INT x)
{
PINT  CurCP,OldCP,ResP;
INT   CP,left_b,Count,i,I1;

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
     if(I1==CP) { Count--;  I1=*((PINT)((PCHAR)CurCP+(i<<1)-4)); }
     *(CurCP-1)=I1;
     I1=CP;
    }
   CurCP=OldCP;
   if(I1<MINCOL)    break;
   if(I1<=left_b)  { I1=x-I1;
                     *ResP++=I1; }
   if(!(--Count))  break;
  }
return ((INT)((PCHAR)ResP - (PCHAR)res_points)>>1);
}

INT Alik_search_bound_point_in_raster(PCHAR raster_tr,INT width_byte,INT nrows)
{
INT    Count,i;
PCHAR  PRaster;

 Count=width_byte * nrows;
 PRaster=raster_tr+Count-1;
 for(i=Count;i>0;i--)   if(*PRaster--)    return 1;
 return 0;
}

void Alik_tr_bit_matr(INT byte_ver,INT size_y,PCHAR buf_in,PCHAR buf_out,
                      INT byte_hor,INT size_x)
{
INT    ByteHor,i,Count;
PCHAR  BufOut,Ptr;
BYTE   POutCurBit,PNextBit,CurOutByte,CountPicsel,c,c1;

 ByteHor=byte_hor;
 BufOut =buf_out;
 do
   {
     PNextBit=0x80;
     do
       {
         CountPicsel=(CHAR)size_y;
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

void Alik_cor_pnt(PINT old_points,PINT new_points,PINT bufer,INT np,
                  INT x_size,INT y_size,INT y_byte,PCHAR raster,PCHAR trace)
{
INT   i,flag;
PINT  POldPoints,PNewPoints,PBufer;
PCHAR Pfb_table,Ptr,PtrOld,PTrace;
CHAR  Ras,c,c1,tmp;

 c = tmp =(BYTE) (y_size - ((y_size+7) & 0x0f8) - 8);
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
        Ras=Pfb_table[(BYTE)Ras];
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
      *PNewPoints++= (INT)*PTrace;
      *PNewPoints++= (INT)(y_size - *(PTrace + x_size));
bb10: if(!(--np)) break;
  }
 *PNewPoints=x_size;
}

INT  Alik_del_doubl_cut(PINT cut_points,PCHAR product,PINT penalty,
                        INT npoints)
{
INT   dob,i,Count,CP;
CHAR  c,c1;
PINT  CurCP,PPen,Ptr;
PCHAR PProd,PProdNew;

 if(npoints <= 1)    return npoints;
 Count=npoints; c1=(CHAR)npoints; --Count; PProd=product-4; CurCP=cut_points;
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

	   PPen=(PINT)((PCHAR)penalty + (CP=(CP<<1)));
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
         CurCP=(PINT)((PCHAR)CurCP + dob);  --c1;  continue;
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
 return (INT)c1;
}

INT  Alik_del_detail(PCHAR bufer,INT rx,INT vol_raster,PINT penalty)
{
PINT   WPen,WPenNew;
PCHAR  PBuf,PBuf1,CurBound,Ptr;
CHAR   Proxod=6,flag,ctmp,index=0;
INT    i,ByteLenStr,BitLenStr,I2,I1;
BYTE   c;

 ++penalty;
do
 {                                           /*a0:*/
 PBuf=bufer; WPen=penalty; BitLenStr=rx; ByteLenStr=(rx+7)>>3; c=0x80;flag=1;
 if(index & 1)
  {
   flag=0;
   WPen=(PINT)((PCHAR)WPen+((rx-2)<<1));
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
     if((WORD)I1<=(WORD)I2) {I1=0; goto a10a;}  /*a9:*/
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

/* LeftDistance - а ббв®п­ЁҐ ¤® ЇҐаў®Ј® б«Ґў  ЎЁв 			*/
INT LeftDistance(BYTE *RASTER, INT NWIDTH)
{
INT i;

for(i=0;i<NWIDTH && (*RASTER)==0;i++,RASTER++);

if( i==NWIDTH)
	return(-1);

return( (i<<3)+start_pos[*RASTER] );
}

/* RightDistance -а ббв®п­ЁҐ ¤® ЇҐаў®Ј® бЇа ў  ЎЁв 			*/
INT RightDistance(BYTE *RASTER, INT NWIDTH)
{
INT i;

RASTER += NWIDTH-1;

for(i=0;i<NWIDTH && (*RASTER)==0;i++,RASTER--);

if( i==NWIDTH)
	return(-1);

return( (i<<3)+last_pos[*RASTER] );
}



