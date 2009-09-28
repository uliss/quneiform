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

#include "struct.h"
#include "cstr.h"
#include "func.h"
#include "linutil.h"
#include "fon.h"
#include "fondefs.h"
#include "ctbdefs.h"
#include "wrgb.h"
#include "dpuma.h"
#include "ligas.h"
#include "status.h"
#include "minmax.h"

#define INCL_FAC 2048

extern Handle hSnapSerifTrace;
extern int16_t    erection_inc;
extern uchar   p2_active;

Bool32 p2_Line2Raster(c_comp *comp, RecRaster *rec);

static uint16_t tabserif[256]={
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        //  0-23
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 24-47
//                        ! " # $ % & ' ( ) * + , - . /
        0,0x00C0,0,0,0x000C,0,0,0,0,0,0,0,0,0,0,0,0,                      // 48-64
//      0      1 2 3      4 5 6 7 8 9 : ; < = > ? @
        0x0033,0x2020,0,0x2020,0x2020,0x2030,0,0x3333,0xC0C0,0xC000,0x3030,0x3030,0x2133,0x2331,0,0x2030,0,0x1030,0,0x000C,0,0x3300,0x0300,0,0x000C,0,    //65-90
//      A      B      C      D      E      F G      H      I      J      K      L      M      N O      P Q      R S      T U      V      W X      Y Z
        0,0,0,0,0,0,                                            //91-96
//      [ \ ] ^ _ `
        0,0,0,0x0001,0,0x00C0,0,0x0033,0x00C0,0,0x0033,0x00C0,0x003F,0x0033,0,0x2030,0x0103,0x00C0,0,0,0x2200,0x3300,0x3300,0,0x3300,0,    // 97-122
//      a b c      d e      f g      h      i j      k      l      m      n o      p      q      r s t      u      v      w x      y z
        0,0,0,0,0,0x0033,0x2020,0x2020,0x2030,0x2100,0x2020,0x0C0C,0,0x3123,0x0023,0x3030,0x0103,0x2133,0x3333,0,0x2133,0x2030,0,0x000C,0x3300,0x0C0C,0x0333,0x3300,//123-150
//      { | } ~       Ђ      Ѓ      ‚      ѓ      „      …      † ‡      €      ‰      Љ      ‹      Њ      Ќ Ћ      Џ      ђ ‘      ’      “      ”      •      –
        0x3303,0x3F21,0x3F00,0x1020,0x3323,0x3020,0,0x3030,0x0103,0,0,0x2020,0x2030,0x2100,0,0x0C0C,0,0x3123,0x0023,0x3030,0x0103,0x2133,0x3333,0,0x2133,0,    //151-176
//           —      �      ™      љ      ›      њ ќ      ћ      џ   Ў      ў      Ј      ¤ Ґ      ¦ §      Ё      ©      Є      «      ¬      ­ ®      Ї °
        0x0033,0x0033,0x0033,0x003F,0x0030,0x0033,0,0,0x003F,0x00C0,0,0,0,                      // 177-189
//          fi     fl     ff    ffi     rt     ri % %    ffl      i j ! ?
//      ±  І  і  ґ   µ  ¶ · ё  №  є » ј Ѕ
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 190 - 223
//      ѕ ї А Б В Г Д Е Ж З И Й К Л М Н О П Р С Т У Ф Х Ц Ч Ш Щ Ъ Ы Ь Э Ю Я
        0x2030,0,0x000C,0x3300,0x0C0C,0x3333,0x3300,0x3303,0x3F21,0x3F00,0x1020,0x3323,0x3020,0,0x3030,0x0103,                        // 224 - 239
//           а б      в      г      д      е      ж      з      и      й      к      л      м н      о      п
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};                       // 240 - 255
//      р с т у ф х ц ч ш щ ъ ы ь э ю

static uint16_t tabvserif[256]={
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        //  0-23
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 24-47
//                        ! " # $ % & ' ( ) * + , - . /
        0,0,0,0,0,0,0,0x0200,0,0,0,0,0,0,0,0,0,                      // 48-64
//      0 1 2 3 4 5 6      7 8 9 : ; < = > ? @
        0,0,0,0,0x0012,0x0002,0,0,0,0,0,0x0010,0,0,0,0,0,0,0,0x0202,0,0,0,0,0,0,    //65-90
//      A B C D      E      F G H I J K      L M N O P Q R S      T U V W X Y Z
        0,0,0,0,0,0,                                            //91-96
//      [ \ ] ^ _ `
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,    // 97-122
//      a b c d e f g h i j k l m n o p q r s t u v w x y z
        0,0,0,0,0,0,0x0002,0,0x0002,0,0x0012,0,0,0,0,0,0,0,0,0,0,0,0,0x0202,0,0,0,0,//123-150
//      { | } ~  Ђ      Ѓ ‚      ѓ „      … † ‡ € ‰ Љ ‹ Њ Ќ Ћ Џ ђ ‘      ’ “ ” • –
        0,0,0,0,0,0,0,0,0,0,0,0,0x0002,0,0,0,0,0,0,0,0,0,0,0,0,0,    //151-176
//      — � ™ љ › њ ќ ћ џ   Ў ў      Ј ¤ Ґ ¦ § Ё © Є « ¬ ­ ® Ї °
        0,  0, 0,  0, 0, 0,0,0,  0,0,0,0,0,                      // 177-189
//      fi fl ff ffi rt ri % % ffl i j ! ?
//      ±  І  і  ґ   µ  ¶ · ё  №  є » ј Ѕ
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 190 - 223
//      ѕ ї А Б В Г Д Е Ж З И Й К Л М Н О П Р С Т У Ф Х Ц Ч Ш Щ Ъ Ы Ь Э Ю Я
        0,0,0x0202,0,0,0,0,0,0,0,0,0,0,0,0,0,                        // 224 - 239
//      а б      в г д е ж з и й к л м н о п
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};                       // 240 - 255
//      р с т у ф х ц ч ш щ ъ ы ь э ю
static uint32_t key=2;                  //для snap'а

static cell *serif_word(cell *c);
static int32_t new_serif(cell *c);
static int32_t fon_test(cell *c);
static void find_serif(cell *c, uint16_t map, int32_t *meas, int32_t *np, int32_t *nm);
static int32_t downserif(c_comp *env, uchar shape, int16_t H, STICK *st);
static int32_t upserif(c_comp *env, uchar shape, int16_t H, STICK *st);
static interval *interval_fit(int16_t i, lnhead *line, int16_t H, STICK *st);
static void ideal2rc(Point16 *p);
static void bound_cell(cell *c, uint32_t color);

void serif_let()
{
  cell *c=cell_f()->next;
  while (c=serif_word(c));
//  LDPUMA_DeleteRects(NULL, key);
}

static cell *serif_word(cell *c)
{
  int32_t np=0,nm=0,meas=0;
  int32_t serif=0;
  Bool reliable;
//  int32_t tol;
  cell *beg=c,*end;
  B_LINES my_bases; //Ў §®ўлҐ «Ё­ЁЁ

  if (!c->next)
    return NULL;
  get_b_lines(NULL,&my_bases);
//  tol=my_bases.ps/16+1;

  do
  {
    if (snap_activity('k'))
    {
      snap_newcell(c);
      snap_monitor();
    }

    if (c->flg & (c_f_let | c_f_bad))
    {
      uchar let;
      c->font &= ~(c_fp_gelv | c_fp_ser);
      if (!(c->font & c_fp_it) || is_upper(let=c->vers[0].let) ||
			strchr("Јав",let) &&
			!is_russian_baltic_conflict(let)&&// 17.07.2001 E.P.
			!is_russian_turkish_conflict(let) // 21.05.2002 E.P.
		 )
      {
        int32_t m=new_serif(c);
        if (m>0)  np++;
        else
        if (m<0)  nm++;
        if (m != 0)
        {
          if (m>0)  c->font |= c_fp_ser;
          else      c->font |= c_fp_gelv;
          if (!LDPUMA_SkipEx(hSnapSerifTrace,FALSE,TRUE,1))
            bound_cell(c,(m>0) ? wRGB(255,0,0) : wRGB(0,255,0));
        }
//        if (!(c->font&c_fp_it))
        meas += m;
      }
    }
    c=c->next;
  }
  while (!(c->flg & c_f_fict));
//  while (!(c->flg & (c_f_space | c_f_fict)));
  end=c;

  serif=meas;  reliable = MAX(np,nm) >= 2*MIN(np,nm);

  if (!reliable && p2_active)  //let's use database
  {
    c=beg;  meas=np=nm=0;
    do
    {
      if (snap_activity('k'))
      {
        snap_newcell(c);
        snap_monitor();
      }

      if (c->flg & (c_f_let | c_f_bad))
      {
        uchar let;
        if (!(c->font & c_fp_it) || is_upper(let=c->vers[0].let) ||
				strchr("Јав",let) &&
					!is_russian_baltic_conflict(let)&&// 17.07.2001 E.P.
					!is_russian_turkish_conflict(let) // 21.05.2002 E.P.
		   )
        {
          int32_t m=fon_test(c);
          if (m==0)            //no match found, use old flags
          {
            if (c->font & c_fp_ser)  m++;
            if (c->font & c_fp_gelv) m--;
          }
          if (m>0)  { np++; m = 1; }
          else
          if (m<0)  { nm++; m =-1; }
          if (m != 0 && !LDPUMA_SkipEx(hSnapSerifTrace,FALSE,TRUE,1))
            bound_cell(c,(m>0) ? wRGB(255,0,0) : wRGB(0,255,0));
  //        if (!(c->font&c_fp_it))
            meas += m;
        }
      }
      c=c->next;
    }
    while (!(c->flg & c_f_fict));
    serif=meas;  reliable = MAX(np,nm) >= 2*MIN(np,nm);
  }

  if (serif != 0)
  {
    uchar font;
    if (reliable)  font = (serif>0) ? c_fp_ser : c_fp_gelv;
    else           font = c_fp_ser | c_fp_gelv;

    for (c=beg; c!=end; c=c->next)
//      if (c->flg & (c_f_let | c_f_bad) && !(c->font & c_fp_it))
      if (c->flg & (c_f_let | c_f_bad))
      {
        c->font &= ~(c_fp_gelv | c_fp_ser);
        c->font |= font;
      }

    if( !LDPUMA_SkipEx(hSnapSerifTrace,FALSE,TRUE,1))
    {
      cell *last=end->prev;
      Rect16 box;
      Point16 pa={beg->col,my_bases.b1-1},pb={last->col+last->w,my_bases.b4+1};
      ideal2rc(&pa);   ideal2rc(&pb);
      box.left=pa.x;  box.top=pa.y;
      box.right=pb.x; box.bottom=pb.y;
      LDPUMA_DrawRect(NULL,&box,0/*nIncline*/,(serif>0) ? wRGB(255,0,0) : wRGB(0,255,0),1,key);
    }
  }

  return c;
}

static int32_t new_serif(cell *c)
{
  uchar let=let_sans_acc[c->vers[0].let];
  uint16_t map=tabserif[let];
  int32_t meas=0,np=0,nm=0;

  if (map==0)
    return 0;

  find_serif(c,map,&meas,&np,&nm);

  if (map=tabvserif[let])
  {
    uchar *rast=save_raster(c);
    int16_t t_height=c->w,t_width_b = (c->h+7)/8;
    MN   *mn;

    c_rastror(rast,t_raster(),c->w,c->h);
    mn=c_locomp(t_raster(),t_width_b,t_height,c->r_row,c->r_col);
    if (mn)
    {
      cell *CC=create_cell(mn,c,0,0);
      if (snap_activity('k'))
      {
        snap_newcell(CC);
        snap_monitor();
      }
      find_serif(CC,map,&meas,&np,&nm);
      del_cell(CC);
    }
  }

  if ( (np != 0)^(nm != 0) )   //no ambiguity
    return meas;

  if (abs(meas)<2)  meas=0;
  if (meas==0 && p2_active)  meas=fon_test(c);
  return meas;
/*
  else
  {
    int32_t nmin=(np,nm),nmax=MAX(np,nm);
    if (nmin>1 || nmax-nmin<2)
      return 0;

    meas= (meas+nfoot/2)/nfoot;
    return (abs(meas) > c->h/8) ? meas : 0;
  }
*/
}

static int32_t fon_test(cell *c)
{
  FonTestInfo testInfo[MAXCHECKALT];
  RecRaster recRast;
  uchar let=let_sans_acc[c->vers[0].let];
  uint16_t map=tabserif[let];
  int32_t i,nitem=0,serific,nbit=8*sizeof(map);

  if (map==0)
    return 0;

  if ( !c->env )
    return 0;

  // Nick 20.9.99
  // уже искались атрибуты ?
  if( c->clu_attr )
      testInfo[0].flag = (uchar)c->clu_attr;
  else
  {
    p2_Line2Raster(c->env,&recRast);
#ifdef _USE_FON_
    if (!FONTestChar(&recRast,let,testInfo,0))
#endif
      return 0;

    c->clu_attr =  testInfo[0].flag;
  }

  if (testInfo[0].flag & CTB_PRINT_SERIFIC)  serific=1;
  else
  if (testInfo[0].flag & CTB_PRINT_GELV)  serific=-1;
  else                                    serific= 0;

  if (serific != 0)
    for (i=0; i<nbit; i++,map=map>>1)
      nitem += map&1;

  return (serific>=0) ? nitem : -nitem;
}

static void find_serif(cell *c, uint16_t map, int32_t *meas, int32_t *np, int32_t *nm)
{
  STICK *st,*sti;
  int16_t i,nstick;
  uchar shape;
  int16_t w0=c->w,dw=0,w3,w23;  //w0 - width of untilted letter

  if (c->n_baton==255)
    nstick=sticks_in_letter(c,0,&st);
  else
  {
    nstick=c->n_baton;  st=c->save_baton;
  }

  if (nstick<=0)
    return;

  if (c->font & c_fp_it)
  {
    int16_t stick_inc=c->stick_inc;
    if (stick_inc==NO_INCLINE)
      if (erection_inc != 0)
        stick_inc=erection_inc;
      else
      {
        for (i=stick_inc=0,sti=st; i<nstick; i++,sti++)
          stick_inc += sti->incl;
        stick_inc /= nstick;
      }
    dw  = stick_inc*c->h/INCL_FAC;
    w0 -= dw;
  }
  w3=w0*2/5,w23=2*w0/3;

  shape=map&3;
  if (shape)  //right-down
  {
    sti=st+nstick-1;
    if (sti->y<=2 && w23 < sti->x && sti->x < c->w)
    {
      int32_t m=downserif(c->env,shape,c->h,sti);
      if (m>0)  (*np)++;
      else
      if (m<0)  (*nm)++;
      *meas += m;
    }
  }

  map >>= 2;
  shape=map&3;
  if (shape)  //middle-down
  {
    for (i=0,sti=st; i<nstick; i++,sti++)
      if (sti->y<=2 && w3 <= sti->x && sti->x < w23)
      {
        int32_t m=downserif(c->env,shape,c->h,sti);
        if (m>0)  (*np)++;
        else
        if (m<0)  (*nm)++;
        *meas += m;
        break;
      }
  }

  map >>= 2;
  shape=map&3;
  if (shape)  //left-down
  {
    sti=st;
    if (sti->y<=2 && sti->x < w3)
    {
      int32_t m=downserif(c->env,shape,c->h,sti);
      if (m>0)  (*np)++;
      else
      if (m<0)  (*nm)++;
      *meas += m;
    }
  }

  map >>= 2;
  shape=map&3;
  if (shape && nstick==1)  //stick like
  {
    int32_t m=downserif(c->env,shape,c->h,st);
    if (m>0)  (*np)++;
    else
    if (m<0)  (*nm)++;
    *meas += m;
  }

  map >>= 2;
  shape=map&3;
  if (shape)  //right-up
  {
    sti=st+nstick-1;
    if (sti->y+sti->l+2>=c->h)
    {
      int16_t x=sti->x+sti->l*sti->incl/INCL_FAC-dw;
      if (w23 < x && x < c->w)
      {
        int32_t m=upserif(c->env,shape,c->h,sti);
        if (m>0)  (*np)++;
        else
        if (m<0)  (*nm)++;
        *meas += m;
      }
    }
  }

  map >>= 2;
  shape=map&3;
  if (shape)  //middle-up
  {
    for (i=0,sti=st; i<nstick; i++,sti++)
      if (sti->y+sti->l+2>=c->h)
      {
        int16_t x=sti->x+sti->l*sti->incl/INCL_FAC-dw;
        if (w3 <= x && x < w23)
        {
          int32_t m=upserif(c->env,shape,c->h,sti);
          if (m>0)  (*np)++;
          else
          if (m<0)  (*nm)++;
          *meas += m;
          break;
        }
      }
  }

  map >>= 2;
  shape=map&3;
  if (shape)  //left-up
  {
    for (i=0,sti=st; i<MIN(2,nstick); i++,sti++)   //in russian 'i' left stick maybe not first
      if (sti->y+sti->l+2>=c->h)
      {
        int16_t x=sti->x+sti->l*sti->incl/INCL_FAC-dw;
        if (x < w3)
        {
          int32_t m=upserif(c->env,shape,c->h,sti);
          if (m>0)  (*np)++;
          else
          if (m<0)  (*nm)++;
          *meas += m;
          break;
        }
      }
  }

  map >>= 2;
  shape=map&3;
  if (shape && nstick==1)  //stick like
  {
    int32_t m=upserif(c->env,shape,c->h,st);
    if (m>0)  (*np)++;
    else
    if (m<0)  (*nm)++;
    *meas += m;
  }
}

static int32_t downserif(c_comp *env, uchar shape, int16_t H, STICK *st)
{
  int16_t h,H8=H/8,H3=H/3;
  lnhead *line;
  int32_t rv=0;

  for (line=(lnhead *)((pchar)(env)+env->lines+sizeof(int16_t));
			 line->lth>0;
       line=(lnhead *)((pchar)line+line->lth))
    if (line->flg&l_fend && (h=line->h) > H3 && line->row+h+2 >= H)
    {
      int16_t x1,x2,i,i0=h-H3-1,in,begl=0,begr=0;
      int32_t lsum=0,rsum=0,imaxl=0,vmaxl=0,imaxr=0,vmaxr=0;
      interval *intv;

      i0=MAX(i0,0);
      i=h-H8-1;              //from line top
      if (!(intv=interval_fit(i,line,H,st)))
        continue;             //line not corresponds to stick

      //to stick base width
      for ( ;  i>=i0 && intv->l != st->w;  i--,intv--);
      if (i<i0)
        continue;             //strange width

      if (!interval_fit(i,line,H,st))
        continue;             //line not corresponds to stick

      x2=intv->e;  x1=x2-intv->l;
      in=h-i;
      for (i=0;  i<in;  i++,intv++)
//        if (intv->l<=w2 || intv->e<x1 || intv->e-intv->l>=x2)
//          break;              //a tump on the line end
//        else
        {
          int16_t dx=(i*st->incl+INCL_FAC/2)/INCL_FAC,s;
          if (shape&2)
          {
            s=x1-(intv->e-intv->l+dx);
            if (s>=vmaxl)  { vmaxl=s;  imaxl=i; }
            if (s>0)
            {
              lsum += s;
              if (begl==0)  begl=i;
            }
          }
          if (shape&1)
          {
            s=intv->e+dx-x2;
            if (s>vmaxr)  { vmaxr=s;  imaxr=i; }
            if (s>0)
            {
              rsum += s;
              if (begr==0)  begr=i;
            }
          }
        }

      if (shape&2)
        if (vmaxl<1)  rv -= 2;
        else
        if (vmaxl>0 && st->incl==0 || vmaxl>1)
//        if (vmaxl>0)
        {
          if (i-begl<=st->w)
          {
            rv++;
            if (vmaxl>=i-imaxl-1 || lsum>=(H+8)/16+1) rv++;
          }
        }

      if (shape&1)
        if (vmaxr<1)  rv -= 2;
        else
        if (vmaxr>0 && st->incl==0 || vmaxr>1)
//        if (vmaxr>0)
        {
          if (i-begr<=st->w)
          {
            rv++;
            if (vmaxr>=i-imaxr-1 || rsum>=(H+8)/16+1) rv++;
          }
        }

      if (rv==1 && lsum+rsum >= (H+8)/16+1)  rv=2;
      rv /= 2;

//      rv = lsum+rsum;
//      if (shape>2)  rv /= 2;
//      rv -= H/16+1;

      if (rv != 0)
        if( !LDPUMA_SkipEx(hSnapSerifTrace,FALSE,TRUE,1))
        {
          Point16 pa={env->left+st->x,env->upper+env->h-st->y},pb={pa.x+st->incl*H/4/INCL_FAC,pa.y-H/4};
          LDPUMA_DrawLine(NULL,&pa,&pb,0/*nIncline*/,(rv>0) ? wRGB(255,0,0) : wRGB(0,255,0),1,1);
        }
      return rv;
    }
  return 0;
}

static int32_t upserif(c_comp *env, uchar shape, int16_t H, STICK *st)
{
  int16_t h,H8=H/8,H3=H/3;
  lnhead *line;
  int32_t rv=0;

  for (line=(lnhead *)((pchar)(env)+env->lines+sizeof(int16_t));
			 line->lth>0;
       line=(lnhead *)((pchar)line+line->lth))
    if (line->flg&l_fbeg && (h=line->h) > H3 && line->row <= 2)
    {
      int16_t x1,x2,i,i0=MIN(H3,h),begl=0,begr=0;
      int32_t lsum=0,rsum=0,imaxl=0,vmaxl=0,imaxr=0,vmaxr=0;
      interval *intv;

      i=H8+1;                //from line top
      if (!(intv=interval_fit(i,line,H,st)))
        continue;             //line not corresponds to stick

      //to stick base width
      for ( ;  i<=i0 && intv->l != st->w;  i++,intv++);
      if (i>i0)
        continue;             //strange width

      if (!interval_fit(i,line,H,st))
        continue;             //line not corresponds to stick

      x2=intv->e;  x1=x2-intv->l;
//      i0=env->h-st->y-st->l;        //in 'p' stick not begins on top
      i0=MAX(line->row,env->h-st->y-st->l);  //finish interval in serif search
      i0=i-i0;                               //number of intervals in serif search
      for (i=0;  i<=i0 && shape != 0;  i++,intv--)
//        if (intv->l<=w2 || intv->e<x1 || intv->e-intv->l>=x2)
//          break;               //a tump on the line begin
//        else
        {
          int16_t dx=(i*st->incl+INCL_FAC/2)/INCL_FAC,s;
          if (shape&2)
          {
            s=x1-(intv->e-intv->l-dx);
/*
            if (s>w3)  //top of russian 'p'
            {
              vmaxl=lsum=0;   shape &= 0xFD;
            }
            else
*/
            {
              if (s>=vmaxl)  { vmaxl=s;  imaxl=i; }
              if (s>0)
              {
                lsum += s;
                if (begl==0)  begl=i;
              }
            }
          }
          if (shape&1)
          {
            s=intv->e-dx-x2;
/*
            if (s>w3)  //top of russian 'p'
            {
              vmaxr=rsum=0;  shape &= 0xFE;
            }
            else
*/
            {
              if (s>vmaxr)  { vmaxr=s;  imaxr=i; }
              if (s>0)
              {
                rsum += s;
                if (begr==0)  begr=i;
              }
            }
          }
        }

      if (shape&2)
        if (vmaxl<1)  rv -= 2;
        else
        if (vmaxl>0 && st->incl==0 || vmaxl>1)
//        if (vmaxl>0)
        {
          if (i-begl<=st->w)
          {
            rv++;
            if (vmaxl>=i-imaxl-1 || lsum>=(H+8)/16+1) rv++;
          }
        }

      if (shape&1)
        if (vmaxr<1)  rv -= 2;
        else
        if (vmaxr>0 && st->incl==0 || vmaxr>1)
//        if (vmaxr>0)
        {
          if (i-begr<=st->w)
          {
            rv++;
            if (vmaxr>=i-imaxr-1 || rsum>=(H+8)/16+1) rv++;
          }
        }

      if (rv==1 && lsum+rsum >= (H+8)/16+1)  rv=2;
      rv /= 2;

//      rv = lsum+rsum;
//      if (shape>2)  rv /= 2;
//      rv -= H/16+1;

      if (rv != 0)
        if( !LDPUMA_SkipEx(hSnapSerifTrace,FALSE,TRUE,1))
        {
          Point16 pa={env->left+st->x+st->incl*st->l/INCL_FAC,env->upper+env->h-st->y-st->l};
          Point16 pb={pa.x-st->incl*H/4/INCL_FAC,pa.y+H/4};
          LDPUMA_DrawLine(NULL,&pa,&pb,0/*nIncline*/,(rv>0) ? wRGB(255,0,0) : wRGB(0,255,0),1,1);
        }
      return rv;
    }
  return 0;
}

static interval *interval_fit(int16_t i, lnhead *line, int16_t H, STICK *st)
//return pointer to i-th interval of the line if it corresponds to stick st;
{
  int16_t h0=H-(line->row+i);    //from raster bottom
  int16_t x0=st->x+(h0-st->y)*st->incl/INCL_FAC;
  interval *intv=(interval *)((pchar)line+sizeof(lnhead))+i;
  if (x0 > intv->e || x0 < intv->e-intv->l)  intv=NULL;
  return intv;
}

static void ideal2rc(Point16 *p)
{
  int16_t y=p->y;
  p->y=y+(int16_t)((int32_t)nIncline*p->x/2048);
  p->x=p->x-(int16_t)((int32_t)nIncline*y/2048);
}

static void bound_cell(cell *c, uint32_t color)
{
  Rect16 box;
  Point16 pa={c->col,c->row},pb={c->col+c->w-1,c->row+c->h-1};
  ideal2rc(&pa);   ideal2rc(&pb);
  box.left=pa.x;  box.top=pa.y;
  box.right=pb.x; box.bottom=pb.y;
  LDPUMA_DrawRect(NULL,&box,0/*nIncline*/,color,1,key);
}
