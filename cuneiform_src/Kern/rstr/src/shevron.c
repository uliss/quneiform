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
#include <stdlib.h>
#include <string.h>

#include "func.h"
#include "struct.h"
#include "status.h"		// 31.08.2000 E.P.
#include "lang_def.h"	// 31.08.2000 E.P.
#include "ligas.h"
#include "linutil.h"

#include "compat_defs.h"

extern uchar db_trace_flag   ;    // snap-detail presence byte
extern uchar  db_status;
extern uchar *MemForCutPointsTwo;

static cell *clbeg,*clend;
static void analysis_words(void);

uchar Alik_find_black_interval(puchar str,uchar len,puchar intrvl);
void shevron(cell* );
void check_contekst_shevron(void);
int16_t  find_word(void);
int16_t  check_shevron(cell *c,int16_t flag_qual);

void shevron(cell* WB)
{
 clbeg=clend=WB;
 while(find_word())
   {
    analysis_words();
    clbeg=clend->nextl;
   }
 clbeg=clend=WB;
 check_contekst_shevron();
}

#define MAX_PROB 235
void check_contekst_shevron()
{
cell *nextc,*clist[2];

 while((clend->nextl!=0) && (clend->nextl)->flg & c_f_let)
  {
   nextc=clend->nextl;
   if(clend->vers[0].let=='<' && clend->vers[0].prob>MAX_PROB &&
      nextc->vers[0].let=='<' && nextc->vers[0].prob>MAX_PROB)
    {

     if(db_status && snap_activity('e') )
      {
       snap_show_text("<< § ¬Ґ­пҐвбп ­  иҐўа®­");
       snap_monitor();
      }
     clist[0]=clend;
     clist[1]=nextc;
     compose_cell(2,clist,clend);

     clend->vers[0].let=left_quocket;
     clend->vers[0].prob=254;
     clend->vers[1].let=0;
     clend->nvers=1;
    }
   else
    if(clend->vers[0].let=='>' && clend->vers[0].prob>MAX_PROB &&
       nextc->vers[0].let=='>' && nextc->vers[0].prob>MAX_PROB)
    {
     if(db_status && snap_activity('e') )
      {
       snap_show_text(">> § ¬Ґ­пҐвбп ­  иҐўа®­");
       snap_monitor();
      }
     clist[0]=clend;
     clist[1]=nextc;
     compose_cell(2,clist,clend);

     clend->vers[0].let=right_quocket;
     clend->vers[0].prob=254;
     clend->vers[1].let=0;
     clend->nvers=1;
    }
   clend=clend->nextl;
  }
}

int16_t find_word()
{
   if(clbeg->nextl==NULL) return 0;
   clend=clbeg;
   while(1)
      {
       if((clend->next==NULL) || ((clend->next)->next==NULL) ||
          (((clend->next)->flg & c_f_space) &&
          ((clend->next)->vers[0].let==0x20)))  break;
       clend=clend->next;
      }

   while(1)
      {
       if( (clend->flg & c_f_let) || (clend->flg & c_f_bad) ||
           (clend->next==NULL) )  break;

	   // Nick
	   if( clend==clbeg) return 0;

       clend=clend->prev;
      }
return 1;
}

#define SMAX_PROB 240
#define PROB_OK   170
#define MIN_PROB  150
#define MID_PROB  200
#define ok_K_hgt  10
static uchar ruslet[]  ="‰–“Љ…Ќѓ™‡•љ”›‚ЂЏђЋ‹„†ќџ—‘Њ€’њЃћгҐл ®пЁо";
//					   "ЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮуеыаояию"
/***************************************************************/
void analysis_words(void)
{
	int16_t   left,right,flag=0,flag_qual;
	cell  *WB,*WE,*cl;
	char  snap[380],*buf=snap;

	left=right=0;
	WB=clbeg;
	WE=clend;

	//AK! add crashguard
	while( (WB->flg & c_f_dust) || (WB->flg & c_f_fict) ||
		   (WB->flg & c_f_punct) )
	{
		if ((WB->next == cell_l() || WB->next == NULL) )
			return;
		WB=WB->next;
	}

	//AK! add c/g
	while( (WE->flg & c_f_dust) || (WB->flg & c_f_fict) ||
		   (WB->flg & c_f_punct))
	{
		if ((WE->prev == cell_f() || WE->prev == NULL))
			return;
		WE=WE->prev;
	}

  if(WB==WE) return;
     /*  ---------------- LEFT -----------------------------*/
  if(WB->vers[0].let==left_quocket) left=100;
  else
   if((WB->flg & c_f_bad)                                                 ||
      (memchr("п ҐўэЄ",WB->vers[0].let,6) &&
		!is_russian_baltic_conflict(WB->vers[0].let) &&	// 17.07.2001 E.P.
		!is_russian_turkish_conflict(WB->vers[0].let)&& // 21.05.2002 E.P.
		WB->vers[0].prob < MAX_PROB) ||
      (memchr("Cc",WB->vers[0].let,2)     && WB->vers[0].prob < MIN_PROB) ||
      (memchr("u",WB->vers[0].let,1)      && WB->vers[0].prob <=SMAX_PROB &&
       WB->h <= ok_K_hgt)                                                 ||
      (WB->vers[0].let== r_cu_u           && WB->vers[0].prob < MID_PROB) ||
      (memchr("ae",WB->vers[0].let,2)     && WB->vers[0].prob < MAX_PROB)
     )
      {
       if(WB->vers[0].prob<MIN_PROB && (WB->h>>1)*3 <= (WB->nextl)->h)
             flag_qual=1;
       else  flag_qual=0;
       if(WB->vers[0].let=='1') flag_qual=0;
       left=check_shevron(WB,flag_qual);
      }

     /*  ---------------- RIGHT -----------------------------*/
  if(WE->vers[0].let==right_quocket)
	 right=100;
   else
   if((WE->flg & c_f_bad)                                                 ||
      (memchr("пў н¬Ґ",WE->vers[0].let,6) &&
		!is_russian_baltic_conflict(WE->vers[0].let) &&	// 17.07.2001 E.P.
		WE->vers[0].prob < MAX_PROB) ||
      (memchr("sк",WE->vers[0].let,2)     && WE->vers[0].prob < MIN_PROB) ||
      (memchr("м",WE->vers[0].let,1)      &&
		!is_russian_baltic_conflict(WE->vers[0].let) &&	// 17.07.2001 E.P.
		WE->vers[0].prob < PROB_OK)  ||
      (WE->vers[0].let== r_cu_u           && WE->vers[0].prob <= MIN_PROB &&
       WE->h <= ok_K_hgt)                                                 ||
      (memchr("e",WE->vers[0].let,1)      && WE->vers[0].prob < MAX_PROB)
     )
      {
       if((WE->vers[0].prob<MIN_PROB && (WE->h>>1)*3 <= (WE->prevl)->h)   ||
          (WE->vers[0].let==(uchar)'н' &&
			!is_russian_baltic_conflict(WE->vers[0].let) &&	// 17.07.2001 E.P.
			WE->vers[0].prob<MIN_PROB        &&
	      (WE->prevl)->vers[0].prob>=230)                                 ||
          (WE->vers[0].let==(uchar)'н' &&
			!is_russian_baltic_conflict(WE->vers[0].let) &&	// 17.07.2001 E.P.
			WE->vers[0].prob-5<PROB_OK       &&
	      memchr(ruslet,(WE->prevl)->vers[0].let,sizeof(ruslet) ) &&
			!is_russian_baltic_conflict((WE->prevl)->vers[0].let)	// 17.07.2001 E.P.
			)         ||
          (WE->vers[0].let==(uchar)'н' &&
			!is_russian_baltic_conflict(WE->vers[0].let) &&	// 17.07.2001 E.P.
			WE->vers[0].prob<MAX_PROB        &&
	      memchr(ruslet,(WE->prevl)->vers[0].let,sizeof(ruslet)) &&
			!is_russian_baltic_conflict((WE->prevl)->vers[0].let) &&	// 17.07.2001 E.P.
			left>=3)
	     )
         flag_qual=1;
        else
		  flag_qual=0;
       if(WE->vers[0].let=='1') flag_qual=0;
       right=check_shevron(WE,flag_qual);
      }

  if(left>0 || right>0 && db_status && snap_activity('e') )
     {
      buf+=sprintf(buf,"Shevron: Word: ");
      cl=WB;
      while(1)
       {
        *buf++ = cl->vers[0].let;
        if(cl==WE) break;
        cl=cl->next;
       }
      *buf++ = '\n';
     }

  if(left!=100 &&
     ((right>=3 && left>=2 && WB->vers[0].prob<PROB_OK) || left>=3))
   {
    flag=1;
    if( db_status && snap_activity('e') )
     {
      *buf++ = WB->vers[0].let;
       buf+=sprintf(buf,"--> § ¬Ґ­пҐвбп ­  << ЇаЁ§­ Є иҐўа®­ =%d \n",left);
     }
    WB->vers[0].let=left_quocket;
    WB->vers[0].prob=254;
    WB->vers[1].let=0;
    WB->nvers=1;
    WB->flg=c_f_let;
   }

  if(right!=100 &&
     ((left>=3 && right>=2 && WE->vers[0].prob<PROB_OK) || right>=3))
   {
    flag=1;
    if( db_status && snap_activity('e') )
     {
      *buf++ = WE->vers[0].let;
       buf+=sprintf(buf,"--> § ¬Ґ­пҐвбп ­  >> ЇаЁ§­ Є иҐўа®­ =%d \n",right);
     }
    WE->vers[0].let=right_quocket;
    WE->vers[0].prob=254;
    WE->vers[1].let=0;
    WE->nvers=1;
    WE->flg=c_f_let;
   }

  if(flag && db_status && snap_activity('e') )
   {
    snap_show_text(snap);
    snap_monitor();
   }
}
/*************************************************************/
#define  AlikSevron ON
#define  otkritost  3
int16_t check_shevron(cell *c,int16_t flag_qual)
{
int16_t   i,i1,i2,penalty,hor_byte,sum,priznak,width,hgt,hgt2,hgt3,hgt23,l_priz;
uchar  Col,*CurPos,*I,bound[2],prev_b[4],povedenie,max_interval,priz2,
      inter1,inter2,priz_12,prev_col,up_max,dw_max,priz_seredini,tmp_max,
      l_min[3],r_min[3],l_m[2],r_m[2],priz_k,prev_left_dist;
puchar raster;

#ifdef AlikSevron
int16_t   b1,b2,b3,b4,r1,r2;
char  snap[380],*buf=snap;
  buf+=sprintf(buf,"step0: let=%c,w=%d,h=%d \n ",c->vers[0].let,c->w,c->h);
#endif
#ifdef _bc_
char  snap[380],*buf=snap;
#endif

 width=c->w; hgt=c->h;
 if(width<=5 || hgt<=5)  return 0;
 hgt2 =(hgt>>1);
 hgt3 =hgt/3;
 hgt23=(hgt<<1)/3;

 raster=save_raster(c);
 hor_byte=(width+7)>>3;
 I=MemForCutPointsTwo;
 sum=0; povedenie=0; priznak=0; max_interval=0; priz2=0; l_priz=0;
 inter1=0; inter2=0; up_max=0;  dw_max=0; priz_seredini=0;
 priz_k=1; prev_left_dist=(uchar)width;
 for(i=0;i<3;i++)
   {
    l_min[i]=0; r_min[i]=0;
    if(i<2) { l_m[i]=0; r_m[i]=0;}
   }
 CurPos=raster;

//-------------------------step1--------------------------------------------
 for(i=0; i<hgt; i++)
  {
   Col=Alik_find_black_interval(CurPos,(uchar)hor_byte,I);
   CurPos+=hor_byte;
   switch(Col)
    {
     case  0: break;
     case  1: if(povedenie<=1)
	       {povedenie=1; sum++; bound[0]=I[0]; bound[1]=I[1];}
	      break;
     case  2:
	     prev_b[0]=I[1];
	     prev_b[1]=I[2];
	     if(I[2]-I[1]-1==1) inter1=1;
	     if(Alik_find_black_interval(CurPos,(uchar)hor_byte,I)==1 && i==0 && hgt>ok_K_hgt)
                goto step2;
	     if(!povedenie)
	      {
	       if(i < hgt3) {sum+=10; priznak++;}
	       goto step2;
	      }
	     else
	      {
	       if(prev_b[0]+1>=bound[0] && prev_b[1]-1<=bound[1]) goto step2;
	       else
	        {
	         if(i < hgt3) {sum+=10; priznak++;}
		 goto step2;
		}
	      }
     default: sum-=5;
    }
  }

//-------------------------step2--------------------------------------------
step2:
#ifdef AlikSevron
  buf+=sprintf(buf,"step1: priznak=%d,sum=%d,flag_qual=%d\n ",priznak,sum,
               flag_qual);
#endif

#ifdef _bc_
 if(flag_qual)
  {
   sprintf(snap,"step1: priznak=%d,sum=%d ",priznak,sum);
   outtextxy(10,20,snap);
  }
#endif

 if(sum<0) return -1;
 povedenie=0;
 CurPos=raster+(hgt-1)*hor_byte;
 for(i=0; i<hgt; i++)
  {
   Col=Alik_find_black_interval(CurPos,(uchar)hor_byte,I);
   CurPos-=hor_byte;
   switch(Col)
    {
     case  0: break;
     case  1: if(povedenie<=1)
	       {povedenie=1; sum++; bound[0]=I[0]; bound[1]=I[1];}
	      break;
     case  2:
	     prev_b[0]=I[1];
	     prev_b[1]=I[2];
	     if(I[2]-I[1]-1==1) inter2=1;
	     if(Alik_find_black_interval(CurPos,(uchar)hor_byte,I)==1 && i==0 && hgt>ok_K_hgt)
                goto step3;
	     if(!povedenie)
	      {
	       if(i < hgt3) {sum+=10; priznak++;}
	       goto step3;
	      }
	     else
	      {
	       if(prev_b[0]+1>=bound[0] && prev_b[1]-1<=bound[1]) goto step3;
	       else
	        {
	         if(i < hgt3) {sum+=10; priznak++;}
		 goto step3;
		}
	      }
     default: sum-=5;
    }
  }

//-------------------------step3--------------------------------------------
step3:
#ifdef AlikSevron
  buf+=sprintf(buf,"step2: priznak=%d,sum=%d,inter1=%d,inter2=%d ",
                    priznak,sum,inter1,inter2);
#endif
#ifdef _bc_
  if(flag_qual)
  {
   sprintf(snap,"step2: priznak=%d,sum=%d,inter1=%d,inter2=%d ",priznak,sum,
                 inter1,inter2);
   outtextxy(10,30,snap);
  }
#endif

 if((sum<0 || !priznak) && !flag_qual)  priznak=-1;
 priz_12=1;
 prev_col=0;
 CurPos=raster;
 for(i=0; i<hgt; i++)
  {
   Col=Alik_find_black_interval(CurPos,(uchar)hor_byte,I);
   CurPos+=hor_byte;

   switch(Col)
    {
     case  0:  prev_col=0;
               break;
     case  1:  if(i>0 && prev_col==2 &&
	          prev_b[1]+1>=I[0] && prev_b[2]-1<=I[1])
	                         {priz_12=0; goto step4;}
	       prev_b[0]=I[0]; prev_b[1]=I[1];
	       prev_col=1;
               break;
     case  2:  if(prev_col==1 &&
	          I[1]+1>=prev_b[0] && I[2]-1<=prev_b[1])
	                         {priz_12=0; goto step4;}
               if(prev_col==2 &&
	          (I[2]-1<=prev_b[1] || I[1]+1>=prev_b[2]))
	                         {priz_12=0; goto step4;}
	       prev_b[0]=I[0]; prev_b[1]=I[1];
	       prev_b[2]=I[2]; prev_b[3]=I[3];
	       prev_col=2;
	       break;
     default:
	       priz_12=0; goto step4;
    }
  }

//-------------------------step4--------------------------------------------
step4:
#ifdef AlikSevron
  buf+=sprintf(buf,"step3: priz_12=%d\n ",priz_12);
#endif
#ifdef _bc_
  if(flag_qual)
  {
   sprintf(snap,"step3: priz_12=%d ",priz_12);
   outtextxy(10,40,snap);
  }
#endif

 sum=0;
 CurPos=raster;
 for(i=0; i<hgt; i++)
  {
   Col=Alik_find_black_interval(CurPos,(uchar)hor_byte,I);
   CurPos+=hor_byte;
   if(Col)
    {
     if(priz_k)
      {
       if(I[0]>prev_left_dist) priz_k=0;
       prev_left_dist=I[0];
      }

     if(I[0]<=2)
      {
       if(i<=2)     l_m[0]=1;
       if(i>=hgt-3) l_m[1]=1;
      }

     if(I[0]<=1)            {if(hgt>ok_K_hgt) l_priz+=2;  else l_priz++;}
     else if(I[0]<=3)       {if(hgt>ok_K_hgt) l_priz++; }
          else  if(I[0]==4) {if(hgt>ok_K_hgt) l_priz--; else l_priz-=2;}
       	        else         l_priz-=2;
    }
   switch(Col)
    {
     case  0: break;
     case  1:
              if(i+1 < hgt2)   up_max=MAX(up_max,I[1]-I[0]+1);
	      else
	       if(i+1 > hgt2)  dw_max=MAX(dw_max,I[1]-I[0]+1);
	       else
		   {
		     up_max=MAX(up_max,I[1]-I[0]+1);
		     dw_max=MAX(dw_max,I[1]-I[0]+1);
		   }
              sum++;
              if(I[0]==1)
		{
		 if(i<hgt2-2)   l_min[0]=1;
		 else
		  if(i>hgt2+1)  l_min[2]=1;
		  else          l_min[1]=1;
		}

	      if(width-I[1]<=1)
		{
		 if(i<=2)     r_m[0]=1;
		 if(i>=hgt-3) r_m[1]=1;
		}

	      if(width-I[1]==0)
		{
		 if(i<hgt2-2)   r_min[0]=1;
		 else
		  if(i>hgt2+1)  r_min[2]=1;
		  else          r_min[1]=1;
		}
	      break;
     case  2:
#ifdef AlikSevron
	     b1=I[0]; b2=I[1]; b3=I[2]; b4=I[3];
	     r1=I[1]-I[0]+I[3]-I[2]+2; r2=I[2]-I[1]-1;
#endif
	     if(width-I[3]<=1)
	      {
	       if(i<=2)     r_m[0]=1;
	       if(i>=hgt-3) r_m[1]=1;
	      }

             if(i<=hgt2)
	        up_max  =MAX(up_max,MAX(I[1]-I[0]+1,I[3]-I[2]+1));
	     else
	        dw_max=MAX(dw_max,MAX(I[1]-I[0]+1,I[3]-I[2]+1));

             if(I[1]-I[0]+I[3]-I[2]+1>I[2]-I[1]) sum+=2;
             else
              if(!priz_12)                       sum-=2;

	     if(!priz_12 && i>2 && i<hgt-2)
                       max_interval+=(I[2]-I[1]-1)/5;

	     priz2++;
	     break;
    default: sum-=5;
             max_interval+=2;
	     if(i<3 || i>hgt-3)  --priznak;
    }
  }
tmp_max=MAX(up_max,dw_max);
if(priz2<=1 && !max_interval && abs(up_max-dw_max)<=3 && tmp_max>(width<<1)/3)
 {
  if((l_min[0] && !l_min[1] && l_min[2] && !r_min[0] && r_min[1] && !r_min[2])
				||
    (!l_min[0] && l_min[1] && !l_min[2] && r_min[0] && !r_min[1] && r_min[2]))
    { priznak=3;  goto end; }
  if(flag_qual)
  {
  if((l_min[0] && l_min[1] && l_min[2] && !r_min[0] && r_min[1] && !r_min[2])
				||
    (!l_min[0] && l_min[1] && !l_min[2] && r_min[0] && r_min[1] && r_min[2]))
    { priznak=3;  goto end; }

  }
 }

#ifdef AlikSevron
  buf+=sprintf(buf,"step4: priznak=%d,sum=%d,max_interval=%d,l_priz=%d,up_max=%d,dw_max=%d\n ",
                           priznak,sum,max_interval,l_priz,up_max,dw_max);
  buf+=sprintf(buf,"step4a: (%d %d %d %d %d %d) (%d %d %d %d) priz_k=%d \n ",
  l_min[0],l_min[1],l_min[2],r_min[0],r_min[1],r_min[2],l_m[0],l_m[1],r_m[0],r_m[1],priz_k);

#endif
#ifdef _bc_
 if(flag_qual)
  {
   sprintf(snap,"step4: priznak=%d,sum=%d,max_interval=%d,l_priz=%d,up_max=%d,dw_max=%d ",
                        priznak,sum,max_interval,l_priz,up_max,dw_max);
   outtextxy(10,50,snap);
  }
#endif

//-------------------------step5--------------------------------------------
/*step5:*/
if(!priz_12 && ((up_max>>1)>=dw_max || (dw_max>>1)>=up_max) && max_interval>2)
                                                                 --priznak;
if(((width-up_max<3 && width-dw_max>3) || (width-dw_max<3 && width-up_max>3))
   && priznak<2 && !flag_qual)                                                 --priznak;
if(((width==up_max || width==dw_max) && abs(dw_max-up_max)>2) && priznak<2)
                                                                 --priznak;
if((inter1 || inter2) && max_interval)                           --priznak;
if(!priz_12 && max_interval )                                    --priznak;

#ifdef AlikSevron
  buf+=sprintf(buf,"step5: priznak=%d,priz2=%d ",priznak,priz2);
#endif
#ifdef _bc_
 if(flag_qual)
  {
   sprintf(snap,"step5: priznak=%d,priz2=%d ",priznak,priz2);
   outtextxy(10,60,snap);
  }
#endif

//-------------------------step6--------------------------------------------
/*step6:*/
   penalty=0;
   i1=hgt3; i2=hgt23;
   CurPos=raster+i1*hor_byte;
   for(i=i1; i<i2; i++)
   {
    Col=Alik_find_black_interval(CurPos,(uchar)hor_byte,I);
    CurPos+=hor_byte;
    switch(Col)
     {
      case 1:
#ifdef AlikSevron
	     b1=I[0]; b2=I[1];
#endif
             if(max_interval &&
	         (I[1]-I[0]<3 || !priznak && I[1]-I[0]<(width>>1) ||
  	         (priznak==1 && max_interval>2 && (I[1]-I[0])<width/3)))
		 penalty++;
	     if(width-up_max<=3 && width-dw_max<=3 && (up_max/3)>=I[1]-I[0])
		priz_seredini++;
                break;
      case 2:
	     if(width-up_max<=3 && width-dw_max<=3 && (up_max/3)>=I[3]-I[0])
		priz_seredini++;
		break;
      default : break;
     }
   }

 if(priz2 && priz_k)  priznak--;
 if(priz_seredini>3)  priznak--;
 if(max_interval==4 && (sum<0 || priznak<2))     priznak-=2;
 if(max_interval>4) priznak-=2;
 if(penalty>1) priznak-=2;
 if(flag_qual && priznak>=2 && sum<0) sum=0;
 if(sum<0 ) --priznak;
 else       ++priznak;

 if(priz2<=1 && !flag_qual) priznak-=2;
// if(hgt && (l_priz+3)/hgt && priznak<2)  priznak-=2;
 if(((l_m[0] && l_m[1] && (r_m[0] || r_m[1]))  ||
     (r_m[0] && r_m[1] && (l_m[0] || l_m[1]))) && hgt>ok_K_hgt)   priznak--;
 if(!flag_qual && l_priz>hgt) priznak--;

#ifdef AlikSevron
  if(flag_qual && priznak>0) ++priznak;
  buf+=sprintf(buf,"step6: priznak=%d ",priznak);
#endif
#ifdef _bc_
if(flag_qual)
  {
   sprintf(snap,"step6: priznak=%d ",priznak);
   outtextxy(10,70,snap);
  }
#endif

end:

#ifdef AlikSevron
  buf+=sprintf(buf,"end: priznak=%d ",priznak);
  if(db_status && (db_trace_flag & 2) &&  snap_activity('e') )
   {
    snap_newcell(c);
    snap_show_text(snap);
    snap_monitor();
   }
#endif

 return priznak;
}

