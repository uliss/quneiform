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


#include "struct.h"
#include "recdefs.h"
extern uchar lpool[];
extern uint16_t lpool_lth;
#include<string.h>
//#include"excdefs.h"
#include"lang_def.h"
#include "ligas.h"	// 04.06.2002 E.P.
#include "ccomdefs.h"
//------------------ FROM DIF.DLL
extern uint16_t DIF_typ_thin_stick(uchar *lin,int16_t dy,int16_t dx);
//------------------ Common working fields
extern c_comp wcomp;
extern uchar records_change;
extern int16_t evfulln;
extern version *start_rec, *rec_ptr;
static version records[128];
extern uchar work_raster[];
extern  uchar language;

//------------------ Common definitions
#define PURE_DUST_HEIGHT 3
#define PURE_DUST_WIDTH  3

//------------------ Common functions definitions
uint16_t events_recog();
uint16_t events_recog_rt();
uint16_t sort_events_vers();
void pidx_crit();
void save_wcomp();
//void sort_events_box(version *p, uint16_t n);
int16_t stick_w_4();

//------------------ Internal functions
static void setvers (uchar* p);
void recog_one();

//------------------ Recognition cases

#define cs_3_3			2	// less than 3*3 envelope size
#define cs_dash_defis_size	4	// dash or defis only by size
#define cs_may_cut		6	// merged components, detected by size
#define cs_punct_sw_size	8	// punctuation detected by size and
					// extended sidewais
#define cs_stick_by_size	10	// stick by size: height/4 >= width
#define cs_punct_up_size	12	// punctuation detected by size and
					// extended upward
#define cs_punct_sq_size	14	// punctuation detected by size and
					// has a square envelope
#define cs_mult_ev		16  // multiple answer by events only
#define cs_notltr_toll		18	// not a letter by events and no permit
					// for cutting because the component is toll
#define cs_letter_ev		22	// perfect letter by events
#define cs_net_toll		24	// net toll component
#define cs_net_cut		26  // net wide enough for cutting component
#define cs_one_line_ltr         28	// one line letter recognized by features
#define cs_one_line_notltr	30	// one line component not recognized
#define cs_features_ltr		32	// about one line component perfectly
					// determinated after features analizis
#define cs_features_mult	34	// about one line component determinated as
					// multiple possibilities letter
#define cs_features_not		36	// about one line component - events and
					// features gives almost different answers
#define cs_comma_dot		38	// determined as comma or dot by
					// comma_dot criterium

static uchar v_dash_defis[] = {ch_punct,cs_dash_defis_size,0};
static uchar v_side_punct[] = {ch_punct,cs_punct_sw_size,0};
static uchar v_up_punct[] = {ch_punct,cs_punct_up_size,0};
static uchar v_sq_punct[] = {ch_punct,cs_punct_sq_size,0};
//static uchar v_small_stick[] = {ch_letter,cs_stick_by_size,
//		'l','i','I','!','j','f','t',0};
//static uchar v_stick_by_size[] = {ch_letter,cs_stick_by_size,
//		'l','i','I','!','j',0};
//static uchar v_temp_one_line[] = {ch_stick,cs_one_line_ltr,0};
//static uchar v_comma_dot[] = {ch_punct,cs_comma_dot,0};

static uint32_t check_iIl1();	// 04.06.2002 E.P.
static uint32_t check_letter(uchar let); // 18.06.2002 E.P.


void recog_save_maden()
{
  start_rec = rec_ptr = records;
  wcomp.large = ch_underlined;
  save_wcomp();
}

void alone_comp()
{
 recog_one();
 save_wcomp();
}

#define s_ans(a) { rec_ptr->let = a; rec_ptr->prob = 254; rec_ptr++; }
int16_t stick_w_4()
{
 int16_t answ;
 answ = DIF_typ_thin_stick(lpool,wcomp.h,wcomp.w);
 rec_ptr = start_rec;
 if (answ == 0) goto ret;
 s_ans('!');
 if ((answ & 0xC0) == 0x80)   s_ans('1');
 if (language == LANG_RUSSIAN ){ s_ans('|'); goto ret; }
 if ((answ & 0x03) == 0x02)   s_ans('f');
 if ((answ & 0x0C) == 0x08)   s_ans('r');
 if ((answ & 0x30) == 0x20)   s_ans('t');

 if (language == LANG_POLISH )
	if ((answ & 0x300)== 0x300) s_ans(0xB3 ); // POLISH_l;

 if (rec_ptr - start_rec < 7) 	s_ans('j');
 s_ans('l'); s_ans('i'); s_ans('I');

 // Добавление турецких палок. 04.06.2002 E.P.
 if(language==LANG_TURKISH)
	 {
	 // 18.06.2002 E.P.
	 if (!check_letter(i_sans_accent))
		s_ans(i_sans_accent);

	 if (!check_letter(II_dot_accent))

		 s_ans(II_dot_accent);
	 }

ret:
 return (wcomp.nvers = rec_ptr - start_rec);
}

void recog_cutlong()
{
 recog_one();
}
extern Bool32  enable_save_stat;
void recog_one()
{
 int16_t nvers;

 //// Reset	/////
 wcomp.cs = 0;
 wcomp.nvers = 0;
 records_change = 0;
 wcomp.reasno = 0;
 wcomp.large = 0;
// wcomp.scale = 0;
 start_rec = rec_ptr = records;

 // Size of component analysis

 if ((wcomp.h <= PURE_DUST_HEIGHT) && (wcomp.w <= PURE_DUST_WIDTH))
    { wcomp.type = ch_perfect+ch_dust; wcomp.cs = cs_3_3; return; }
 if (wcomp.h == wcomp.w)
  {
   if (wcomp.h <= SMALL_SIZE) {setvers(v_sq_punct); return;}
   goto usual;
  }
 if (wcomp.h < wcomp.w)
  {
   if (wcomp.w <= SMALL_SIZE) {setvers(v_side_punct); return;}
   if (wcomp.w * 2 >= wcomp.h * 5)
    {
     if (wcomp.h <= SMALL_SIZE) {setvers(v_dash_defis); return;}
     else { wcomp.type = ch_merge; wcomp.cs = cs_net_cut; return; }
    }
   goto usual;
  }
 else
  {
   if (wcomp.h <= SMALL_SIZE) { setvers(v_up_punct); return; }
   if (wcomp.w > 4) goto usual;
   wcomp.type = ch_letter; wcomp.cs = cs_stick_by_size;
   stick_w_4();
   goto no_sort;
  }

// Component recognition
usual:
 nvers = events_recog();
 if (!enable_save_stat && nvers <=0)
    goto not_letter;

 nvers = events_recog_rt();

 if (!enable_save_stat && nvers <=0)
    goto not_letter;
 if (nvers == 1) goto perfect_letter;

 // Добавление турецких палок. 04.06.2002 E.P.
 if(language==LANG_TURKISH &&
		nvers < VERS_IN_CELL - 2 &&
		check_iIl1() >= 3
   )
	 {
	 // 18.06.2002 E.P.
	 if (!check_letter(i_sans_accent))
		 {s_ans(i_sans_accent);nvers++;}

	 if (!check_letter(II_dot_accent))
		 {s_ans(II_dot_accent);nvers++;}
	 }

 wcomp.type = ch_letter; wcomp.cs = cs_mult_ev; wcomp.nvers = nvers;

criteria:

no_sort:
 if (wcomp.nvers < 16) return;
// sort_events_box (start_rec,wcomp.nvers);
 wcomp.nvers = 15; rec_ptr = start_rec+15;
 return;

perfect_letter:
 wcomp.type = ch_letter+ch_perfect; wcomp.cs = cs_letter_ev;
 wcomp.nvers = nvers; goto criteria;

not_letter:
 wcomp.type = ch_merge; wcomp.cs = cs_net_cut; return;
}

static void setvers(uchar* p)
{
 version *v = start_rec;
 wcomp.type = *p++; wcomp.cs = *p++;
 while (*p)  { v->let = *p++; (v++)->prob = 254; }
 wcomp.nvers = (rec_ptr = v) - start_rec;
}

uint32_t recog_letter()
{
 recog_one();
 return wcomp.nvers;
}


uint32_t recog_letter_lp(/*ExtComponent*/CCOM_comp *ec, uchar * lp,uint16_t lth)
{
memset(&wcomp,0,sizeof(wcomp));
wcomp.h=      ec->h;
wcomp.w=      ec->w;
wcomp.rw=     ec->rw;

wcomp.nl=     ec->nl;
wcomp.begs=   ec->begs;
wcomp.ends=   ec->ends;
wcomp.scale=   ec->scale;

lpool_lth = lth;
memcpy(lpool, lp, lth);
recog_one();
return wcomp.nvers;
}


uint32_t check_iIl1()
{
// Выдает количество палочных версий iIl1
version *p = start_rec;
long n=0;

while ( p < rec_ptr )
	{
	if( strchr("iIl1", p->let) )
		n++;

	p++;
	}

return n;
}

uint32_t check_letter(uchar let)
{
// Проверяет наличие версии. 18.06.2002 E.P.
version *p = start_rec;

while ( p < rec_ptr )
	{
	if( p->let == let )
		return 1;

	p++;
	}

return 0;
}
