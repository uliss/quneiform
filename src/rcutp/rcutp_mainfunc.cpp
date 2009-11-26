/*
Copyright (c) 1993-2008, Cognitive Technologies
All rights reserved.

����������� ��������� ��������������� � ������������� ��� � ���� ��������� ����,
��� � � �������� �����, � ����������� ��� ���, ��� ���������� ��������� �������:

      * ��� ��������� ��������������� ��������� ���� ������ ���������� ���������
        ���� ����������� �� ��������� �����, ���� ������ ������� � �����������
        ����� �� ��������.
      * ��� ��������� ��������������� ��������� ���� � ������������ �/��� �
        ������ ����������, ������������ ��� ���������������, ������ �����������
        ��������� ���� ���������� �� ��������� �����, ���� ������ ������� �
        ����������� ����� �� ��������.
      * �� �������� Cognitive Technologies, �� ����� �� ����������� �� �����
        ���� ������������ � �������� �������� ��������� �/��� �����������
        ���������, ���������� �� ���� ��, ��� ���������������� �����������
        ����������.

��� ��������� ������������� ����������� ��������� ���� �/��� ������� ������ "���
��� ����" ��� ������-���� ���� ��������, ���������� ���� ��� ���������������,
������� �������� ������������ �������� � ����������� ��� ���������� ����, �� ��
������������� ���. �� �������� ��������� ���� � �� ���� ������ ����, �������
����� �������� �/��� �������� �������������� ���������, �� � ���� ������ ��
��Ѩ� ���������������, ������� ����� �����, ���������, ����������� ���
������������� ������, ��������� � �������������� ��� ���������� ����������
������������� ������������� ��������� (������� ������ ������, ��� ������,
������� ���������, ��� ������ �/��� ������ �������, ���������� ��-�� ��������
������� ��� �/��� ������ ��������� �������� ��������� � ������� �����������,
�� �� ������������� ����� ��������), �� �� ������������� ���, ���� ���� �����
�������� ��� ������ ���� ���� �������� � ����������� ����� ������� � ������.

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

/*   (C)  Andrey V. Ivanov, 1992.  */
/*   (C)  Alik Shahverdiev, 1996.  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "struct.h"
#include "cuthdr.h"
#include "cutstr.h"
#include "dmconst.h"
#include "alik_rec.h"
#include "lang_def.h"
#include "recdefs.h"
#include "minmax.h"

extern  int16_t      minrow,bbs1,bbs2,bbs3,bbs4,Nb1,Nb2,Nb3;
extern  uchar      language;
#ifndef max
#define MAX(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define MIN(a,b)            (((a) < (b)) ? (a) : (b))
#endif

extern int16_t   up_position,dw_position;
uchar MemForCutPoints[65536];
uchar MemForCutPointsTwo[65536];
uchar ForRaster3[R_S];
extern uchar  db_status;  // snap presence byte
void snap_clear_screen(void);


static int16_t GDE_KAK[13][3] = {{1,0,0} ,{4,0,0}  ,{8,0,0} ,
                            {1,8,0} ,{8,1,0} ,{8,8,0},
                            {1,8,8},{8,8,1},{8,1,8},{8,8,8},
			    {4,8,8},{8,4,8},{8,8,4}};

int16_t Alik_define_cut_points(
  pchar raster_frag,
  struct own_cut *ans,
  int16_t dx,                  /* ࠡ��� �ਭ� ����          */
  int16_t dy,                   /* ࠡ��� ���� ����          */
  int16_t row
)

{
 int16_t    hor_byte,ver_byte,nshort,CP,i,j,bl_up,bl_dw,tret_h;
 pint16_t   penalty,cut_points,adr_cut_points,my_penalty;
 pchar  adrw,adrw_two,product,product_two,trace,adr_raster,stek,adr_ras_two,
        SourceRaster;
 puchar  IntBuf,CountCut,UpBlackPoint;
 struct own_cut *ans_ptr;
#ifdef AlikBl
char snap[380],*buf=snap;
#endif

 Z=Z;
 bl_up=bbs2;
 bl_dw=bbs3;
 adr_cut_points=(pint16_t)ForRaster3;
 ans_ptr=ans;
 hor_byte=(dx+7)>>3;
 ver_byte=(dy+7)>>3;
 CP=ver_byte*dx;
 i=hor_byte*dy;
 tret_h = (dy<<1)/3;

 adr_raster=(pchar)MemForCutPoints;        /* ��� ��室�� �࠭ᯮ��஢���� ���� */
 adrw=adr_raster+CP;                /* ��� ��ࠡ�⠭�� �࠭ᯮ��஢���� ���� */
 trace=adrw+(CP>i?CP:i);            /* ��� ��室  */
 penalty=(pint16_t)trace+dx;            /* ��� ����� */
 product=(pchar)(penalty+dx);       /* ��� �ந�������� ��᫥����⥫��� �⮫�殢 */
 cut_points=(pint16_t)(product+dx);     /* ��� �窨 ࠧ१���� */

 adrw_two=(pchar)MemForCutPointsTwo;
 adr_ras_two = adrw_two+(CP>i?CP:i);
 product_two = adr_ras_two+(CP>i?CP:i); /* ��� �ந�������� ��᫥����⥫��� �⮫�殢 ᫥�� � �ࠢ�*/
 UpBlackPoint= (puchar)(product_two+dx);
 CountCut    = (puchar)(UpBlackPoint+dx);
 SourceRaster= (pchar)(CountCut+dx);
 my_penalty  = (pint16_t)(SourceRaster+(CP>i?CP:i));
 IntBuf      = (puchar)(my_penalty+dx);

 CP = (dx<3||dy<3)? 0 : 1;          /* १��� �� �㤥�, �᫨ ���� ��� �� ������ �� ���ࠢ����� */

if(CP)
 {
   memcpy(SourceRaster,raster_frag,hor_byte*dy);
   memset((puchar)adr_cut_points,0,R_S);
   memset(CountCut,0,dx);

   Alik_tr_bit_matr(ver_byte,dy,raster_frag,adr_raster,hor_byte,dx); /* �࠭ᯮ��஢���� ��室���� ���� */
   memset((pint16_t)trace,0,sizeof(int16_t)*dx);      /* ���㫥��� ���� ���⮢�� ���ᨢ�� */
   memset(penalty,0,sizeof(int16_t)*dx);          /* ���㫥��� ���ᨢ� ���䮢 */
   CP=Alik_del_detail(raster_frag,dx,(int16_t)(dy*hor_byte),penalty);
   if(!CP && dx>20 && dx<=128) CP=1;   //10-09-96 03:24pm  Alik  cten33.tif
   if(!CP)                        /* ���� �ᯠ��� */
    {
     memset(penalty,0,sizeof(int16_t)*dx);        /* ���㫥��� ���ᨢ� ���䮢 */
     memcpy(adr_raster,adrw,ver_byte*dx);
    }
   else
     Alik_tr_bit_matr(ver_byte,dy,raster_frag,adrw,hor_byte,dx);    /* �࠭ᯮ��஢���� ��⮢�� ������ */

   if(CP) CP=Alik_search_bound_point_in_raster(adrw,ver_byte,dx);

   if(CP)       /*  ���� ������ */
    {
     Alik_form_bound(adrw,dx,dy,ver_byte,trace,0);
     memcpy(adr_ras_two,adrw,ver_byte*dx);
     memcpy(adrw_two,adrw,ver_byte*dx);
     memcpy(my_penalty,penalty,sizeof(int16_t)*dx);

     Alik_CountCut(adrw,dy,dx,CountCut,IntBuf);
     Alik_UpBlackPoint(adrw,dy,dx,UpBlackPoint);
     Alik_cut_short(adrw,adrw_two,dx,ver_byte,product,penalty,cut_points,product_two);
     nshort=*cut_points;
     Alik_cut_hole(trace,dx,cut_points,nshort,dy,product,penalty,0x00);
     Alik_del_equal_hole(cut_points,product,(pint16_t)penalty,dx,dy,nshort);

#ifdef AlikBl
     if( db_status && snap_activity('j') && snap_activity('a'))
      Alik_snap_show_raster(raster_frag,SourceRaster,"raster mode",dy,dx,
                           product,product_two,penalty);
#endif
     if(language==LANG_RUSSIAN)
       Alik_find_brus(raster_frag,SourceRaster,adr_ras_two,(int16_t)(bl_dw-bl_up),dy,dx,
                      product,product_two,penalty,cut_points,CountCut,
		      UpBlackPoint);
#ifdef AlikBl
     buf=snap;
     buf+=sprintf(buf,"bl_up=%2d,bl_dw=%2d   ",bl_up,bl_dw);
#endif
     Alik_my_correct_base_lines(&bl_up,&bl_dw,dy,dx,row,(puchar)adr_ras_two,
                                (PWORD)penalty,IntBuf);
#ifdef AlikBl
     buf+=sprintf(buf,"bl_up=%2d,bl_dw=%2d   ",bl_up,bl_dw);
#endif
     Alik_correct_base_lines(trace,dx,dy,&bl_up,&bl_dw);
#ifdef AlikBl
     buf+=sprintf(buf,"bl_up=%2d,bl_dw=%2d \n",bl_up,bl_dw); //,up_position); // Bound
     buf+=sprintf(buf,"bbs1=%2d, bbs2=%2d, bbs3=%2d, bbs4=%2d,  minrow=%2d,  row=%2d \n",
                  bbs1,bbs2,bbs3,bbs4,minrow,row);
     buf+=sprintf(buf,"Nb1=%2d,  Nb2=%2d,  Nb3=%2d,  Nb4=%2d,  up=%2d,  dw=%2d",
                  Nb1,Nb2,Nb3,Nb4,up_position,dw_position);
     if( db_status && snap_activity('j'))
      Alik_snap_show_raster(raster_frag,SourceRaster,snap,dy,dx,
                           product,product_two,penalty);
#endif

     if(bl_up>=0 && bl_dw<=dy)
      {
       Alik_double_serif(cut_points,trace,product,dx,dy,bl_up,bl_dw,penalty,
                         (puchar)adr_ras_two,IntBuf);
       Alik_cut_d(trace+dx,cut_points,product,dx,dy,bl_up,bl_dw,(pchar)penalty);
       Alik_defis(trace,cut_points,dx,dy,bl_up,bl_dw,penalty);
    if(language==LANG_ENGLISH)
       Alik_cut_l(cut_points,trace,dx,bl_dw,penalty);
      }

    if(language==LANG_RUSSIAN)
      Alik_cut_y(raster_frag,SourceRaster,adr_ras_two,dy,dx,cut_points,
                 IntBuf,row,product,product_two,penalty,CountCut);
    if(language==LANG_RUSSIAN)
      Alik_rus_bad_c(raster_frag,SourceRaster,dy,dx,product,product_two,
                     penalty,cut_points,CountCut,UpBlackPoint);

     Alik_del_equal(cut_points);
     Alik_del_3_cut(adrw,cut_points,nshort,ver_byte,dx,CountCut);
    if(abs((row+dy)-(minrow+bbs3))>3)
     Alik_del_p_cut(adrw,cut_points,dx,dy,nshort,(char)(bl_dw-bl_up-3));
    if(language==LANG_RUSSIAN)
     Alik_Check_Rus_D(raster_frag,SourceRaster,dy,dx,cut_points,IntBuf,row,
                      product,product_two,penalty,CountCut);
     Alik_del_equal(cut_points);
     CP=*cut_points;

     if(CP)
      {
       CP=Alik_sort(cut_points,(pint16_t)adr_cut_points,dx);
       if(CP)  CP=Alik_del_doubl_cut((pint16_t)adr_cut_points,product,penalty,CP);
      }
     CP=MIN(STK_H-2,CP);

    }
   if (!CP)  Alik_tr_bit_matr(hor_byte,dx,adr_raster,raster_frag,ver_byte,dy);

 if(CP)
  {
   Alik_cor_pnt((pint16_t)adr_cut_points,penalty,cut_points,CP,dx,dy,ver_byte,(pchar)adrw,(pchar)trace);
   Alik_form_bound(adr_raster,dx,dy,ver_byte,trace,1);
 	 stek=(pchar)(penalty+((CP+1)<<2)-(CP+1));  /* ��९��뢠�� �ࠣ����� ����� ��室� */
 	 cut_points=penalty+3*(CP-1)+1;
 	 for (j=CP; j>0; j--, cut_points-=4,stek++)
      {
       *stek=(char)*cut_points++;
       if (*stek) (*stek)--;
       *++stek=dy-*cut_points;
       if (*stek) (*stek)--;
      }
   trace=(pchar)(penalty+((CP+1)<<2)-(CP+1));

   for(j=CP-1,i=0; j>=0; j--,i+=2)
    {
     ans_ptr->x   = (char)*((pint16_t)adr_cut_points+j);
     ans_ptr->h   = dy - *(trace+i);
     ans_ptr->dh  = dy+1 - *(trace+i+1)-*(trace+i);
     ans_ptr->dh  = (char)MIN(ans_ptr->dh,dy);
     ans_ptr->dh  = MIN(ans_ptr->dh,ans_ptr->h);
     if (ans_ptr->dh < tret_h)   ans_ptr->var=1;
     else                        ans_ptr->var=8;
     ans_ptr++;
    }
   ans_ptr->x=127;

   qsort((void *)(ans),CP,sizeof(struct own_cut),Alik_sort_function);
   Alik_set_method_for_cut_points(ans,dy,dx,CountCut);
   Alik_cor_height_and_var(CP,ans,my_penalty,dx,dy);
   Alik_new_points(&CP,ans,ans_ptr,dy,dx,my_penalty,product);
   //AK! posible stack crash there when ans pointed to last vector element!
   qsort((void *)(ans),CP,sizeof(struct own_cut),Alik_sort_function);
  }
 }
 return CP;
}

void Alik_new_points(int16_t *CP,struct own_cut *a,struct own_cut *ptr,int16_t dy,
                     int16_t dx,pint16_t pen,pchar prod)
{
int16_t i,j,count,min_pen,min_prod,real_x,Ix,IIx;

 count = *CP;
 for(i=0; i<count; i++)
  {
   if((*CP)>STK_H-2) break;
   Ix=(a+i)->x;
   IIx=(a+i+1)->x;
   if(!i && Ix> NEW_POINTS_NAV_BOUND) {Ix=0; IIx=(a+i)->x;}
   else if(i==count-1)  IIx=dx;
   if(IIx - Ix > NEW_POINTS_NAV_BOUND)
    {
     real_x  = Ix+((IIx-Ix)>>1);
     Ix+=MINCOL; IIx-=MINCOL;
     min_pen = *(pen+Ix);
     min_prod= *(prod+Ix);
     for(j=IIx;j>Ix;j--)
      {
       if((*(prod+j)<min_prod  && *(pen+j)-10000<min_pen) ||
          (*(prod+j)==min_prod && *(pen+j)<min_pen))
	{
	 real_x=j;
         min_pen =*(pen+j);
         min_prod=*(prod+j);
	}
      }
     if(real_x>MINCOL && real_x<dx-MINCOL)
      {
       ptr->x   = real_x+1;
       ptr->h   = (char)dy;
       ptr->dh  = (char)dy;
       ptr->var = 40;
       ++ptr; (*CP)++;
      }
    }
  }
 ptr->x=127;
}

void Alik_cor_height_and_var(int16_t CP,struct own_cut *ans,pint16_t pen,int16_t dx,
                             int16_t dy)
{
int16_t i,Count;

 Count=CP;
 for(i=0;i<Count;++i,++ans)
  {
   if(*(pen+ans->x)<0 ||
     (ans->x>1 && *(pen+ans->x-1)<0) ||
     (ans->x<dx-1 && *(pen+ans->x+1)<0))
     {
      ans->h   = (char)dy;
      ans->dh  = (char)dy;
      ans->var = 40;
     }
  }
}

int Alik_sort_function( const void *a, const void *b)
{
 if ((((mas_cpnt *)a)->x > ((mas_cpnt *)b)->x ) ||
     ((((mas_cpnt *)a)->x == ((mas_cpnt *)b)->x ) &&
      (((mas_cpnt *)a)->var > ((mas_cpnt *)b)->var)))     return 1;
 if (((mas_cpnt *)a)->x < ((mas_cpnt *)b)->x )            return -1;
 return 0;
}

void Alik_set_method_for_cut_points(struct own_cut *ans,int16_t dy,int16_t dx,
                                    puchar CountCut)
{
uchar CountGrupElem,n,i,j,m,gde,flag4;
int16_t  min_dh;
struct own_cut oct[STK_H],*ptr;

 min_dh=dy;
 for (ptr=ans,n=0; n<STK_H-1; n++,ptr++)
     {
      oct[n].x  =ptr->x;
      oct[n].h  =ptr->h;
      oct[n].dh =ptr->dh;
      oct[n].var=ptr->var;
      if(oct[n].x==127)  break;
      min_dh=MIN(min_dh,ptr->dh);
     }

 for (i=0; i<=n; i++)
  {
   for (j=i; j<=n; j++)
    {
     if(oct[j].x==127 || oct[j+1].x-oct[j].x > 4)  break;
    }
   CountGrupElem=j-i+1;
   if(CountGrupElem<=3)
   {                                        // OLEG : case CountGrupElem==0 ????
     gde=Alik_gde_i_kak_naxodjatsa_tochki((uchar)(CountGrupElem-1),&oct[i],dy,min_dh);
     for(m=i;m<=j;m++)  oct[m].var=(char)GDE_KAK[gde][m-i];
    }
   else
    {
     flag4=0;
     for(m=i;m<=j;m++)
      {
       if(!flag4 && (oct[m].dh<=(dy>>2)+1 || oct[m].dh==min_dh))
                               {oct[m].var=1; flag4=1; continue;}
       if(m==i+(CountGrupElem>>1) && (oct[m].dh<=dy/3 || oct[m].dh<min_dh+3))
                               {oct[m].var=4;continue;}
       if((m==i || m==j) && (oct[m].dh<=(dy>>2)+1 || oct[m].dh<min_dh+3))
                               {oct[m].var=4;continue;}
       oct[m].var = 8;
     }
    }
   i=j;
  }

 for (ptr=ans,n=0; n<STK_H-1; n++,ptr++)
  {
   if(oct[n].x==127)  break;
   if(*(CountCut+oct[n].x)==3)                          ptr->var=40;
    else
     if(oct[n].x <= OTSTUP || (dx-oct[n].x) <= OTSTUP)  ptr->var=40;
     else                                               ptr->var=oct[n].var;
  }
}

uchar Alik_gde_i_kak_naxodjatsa_tochki(uchar CountGrupElem,struct own_cut *ans,
                                      int16_t height,int16_t min_dh)
{
uchar i;
int16_t  max_h,h_h,tret_h,dve_tret_h,chetvert_h,two_min_dh,begin[3],end[3];

 h_h        = ((height+1)>>1);
 tret_h     = height/3 +1;
 dve_tret_h = (height<<1)/3;
 chetvert_h = (height>>2)+1;
 two_min_dh = min_dh<<1;

 for(i=0;i<=CountGrupElem;i++)
   {
    begin[i] = height   - (ans+i)->h;
    end[i]   = begin[i] + (ans+i)->dh;
   }


 switch(CountGrupElem)
  {
   /******************** ���� �窠*******************************/
   /*        0       1         2                                 */
   /*    {1,0,0} ,{4,0,0} ,{8,0,0}                              */
   /**************************************************************/
   case 0: /* ���� �窠 */
        if(ans->dh < min_dh+3)     return 0;
        if(ans->dh <= two_min_dh)  return 1;
                                   return 2;
   /******************** ��� �窨 *******************************/
   /*        3         4          5                              */
   /*    {1,8,0} ,{8,1,0} ,{8,8,0}                           */
   /**************************************************************/
   case 1:
	/* ���� min */
        if(ans->dh<min_dh+3 || (ans+1)->dh<min_dh+3 )
	 {
          if(ans->dh<(min_dh+3) && ans->dh<(ans+1)->dh)  return 3;
          return 4;
	 }

	/* ������� ��� ���⪨� ���������� */
        if((ans->dh<=min_dh || (ans+1)->dh<=min_dh) &&
           end[0]<tret_h    &&  end[1]  <tret_h)
         {
          if(ans->dh<(ans+1)->dh)  return 3;
          else                     return 4;
         }

        /* � �।��� ��� ���⪨� ���������� */
        if((ans->dh<=min_dh  || (ans+1)->dh<=min_dh) &&
          begin[0]>=tret_h   && begin[1]>=tret_h    &&
          end[0]<=dve_tret_h && end[1]<=dve_tret_h)
           {
            if(ans->dh<(ans+1)->dh)  return 3;
            else                     return 4;
           }

        /* � �।��� ��� ������� */
        if(ans->dh>=tret_h      && (ans+1)->dh>=tret_h     &&
           begin[0]>=chetvert_h && begin[1]>=chetvert_h    &&
           ans->h  >=chetvert_h && (ans+1)->h>=chetvert_h)
        return 5;

        /*����� ��� ���⪨� ����������*/
        if(ans->h <= chetvert_h && (ans+1)->h <= chetvert_h)
         {
          if(ans->dh<(ans+1)->dh)  return 4;
          else                     return 3;
         }

        if(ans->dh     <= min_dh) return 3;
        if((ans+1)->dh <= min_dh) return 4;

        return 5;

   /******************** �� �窨 *******************************/
   /*          6         7         8          9                  */
   /*   {1,8,8},{8,8,4},{8,1,8},{8,8,8}                 */
   /**************************************************************/
   case 2:
	/* ���� min */
        if(ans->dh==min_dh || (ans+1)->dh==min_dh || (ans+2)->dh==min_dh)
	 {
          if(ans->dh==min_dh)      return 6;
          if((ans+1)->dh==min_dh)  return 8;
          return 7;
	 }
	/* ���� min+2 */
	if(ans->dh<min_dh+3 || (ans+1)->dh<min_dh+3 || (ans+2)->dh<min_dh+3)
	 {
          if(ans->dh<min_dh+3)      return 10;
          if((ans+1)->dh<min_dh+3)  return 11;
          return 12;
	 }

        /*������� �� ���⪨� ����������*/
        if(end[0]<=tret_h && end[1]<=tret_h && end[2]<=tret_h)
         {
           if(ans->h==min_dh && (ans+1)->h==min_dh && (ans+2)->h==min_dh)
                                   return 8;
           if(ans->h==min_dh)      return 6;
           if((ans+1)->h==min_dh)  return 8;
           if((ans+2)->h==min_dh)  return 7;
           return 8;
         }

        /*� �।��� �� ���⪨� ���������� � ���ꥬ��*/
        if(ans->dh<chetvert_h && (ans+1)->dh<chetvert_h && (ans+2)->dh<chetvert_h &&
           begin[0]>=tret_h   && begin[1]>=tret_h       && begin[2]>=tret_h       &&
           end[0]<=dve_tret_h && end[1]<=dve_tret_h     && end[2]<=dve_tret_h     &&
           ans->h<(ans+1)->h  && (ans+1)->h < (ans+2)->h)
        return 9;

        /*� �।��� �� �������*/
        if(ans->dh>=tret_h  && (ans+1)->dh>=tret_h &&  (ans+2)->dh>=tret_h &&
           begin[0]>=chetvert_h && begin[1]>=chetvert_h && begin[2]>=chetvert_h &&
           ans->h  >=chetvert_h && (ans+1)->h>=chetvert_h && (ans+2)->h>=chetvert_h)
        return 9;

        /*�� �ᥩ ���� �� �������*/
        if(abs(ans->dh-height)<4 && abs((ans+1)->dh-height)<4 &&
           abs((ans+2)->dh-height)<4)
        return 8;

        /*����� �� ���⪨� ����������*/
        if(ans->h<=chetvert_h && (ans+1)->h<=chetvert_h && (ans+2)->h<=chetvert_h)
         {
           max_h=MAX(MAX(ans->h,(ans+1)->h),MAX((ans+1)->h,(ans+2)->h));
           if(ans->h==max_h)      return 6;
           if((ans+1)->h==max_h)  return 8;
           else                   return 7;
         }
        return 9;
  }
return 9;
}
