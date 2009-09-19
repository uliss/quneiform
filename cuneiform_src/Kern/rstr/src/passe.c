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

/********************************************************************/
/*                                                                  */
/*       Recognition english word in russian string                 */
/*                                                                  */
/********************************************************************/
#include "tuner.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "struct.h"
#include "func.h"
#include "status.h"
#include "msgerr.h"
#include "lang.h"
#include "layout.h"
#include "tigrun.h"
#include "linutil.h"
#include "ligas.h"
#include "user_voc.h"
#include "minmax.h"

extern uchar db_trace_flag;
extern uchar decode_ASCII_to_[256][4];
extern uchar valid_word_number;
extern int16_t  text_findstat(char * w);
extern int16_t  text_findstat_aux(char * w);
extern int16_t  text_findstat_agressive(char * w);
static int16_t  get_right_coord(cell *c);
static int16_t  get_left_coord(cell *c);
static uchar is_english_word(cell *c,cell *e);
static uchar change_Il1(cell *b, cell *e);
static uchar eng_recognize(cell *b, cell *e);
static uchar english_cell_recognize(cell *cc);
static void filtrate_language(cell *b,cell *e);
static void mark_two_lang_word(cell *b, cell *e, uchar val);
static Bool bad_chars_in_word(cell *b,cell *e);
static Bool small_prob_in_word(cell *b,cell *e) ;
static Bool more_alt(cell *b,cell *e);
static Bool II_place(cell *c);
static Bool II_config(cell *c);
Bool russian_word(uchar *wrd);
Bool english_word(uchar *wrd);
Bool russian_word_all(uchar *wrd,uchar language,uchar nextlet);
Bool english_word_all(uchar *wrd,uchar language);
static Bool exist_symbol(cell *b,cell *e,uchar ch) ;
static Bool left_over(cell *b,int16_t limit);

//Alik 04-25-96 06:53pm
//============== Import func ===========
//int16_t text_findstat(char * word);
//============== Local func =================
static Bool russian_dictionary_word(cell * first, cell * last,puchar);
Bool _spell(pchar s,uchar lang);
Bool _spell_agressive(pchar s,uchar lang);
Bool short_spell(uchar *wrd,uchar language, uchar nextlet );
extern void final_descriminate(cell *b, cell *e);
extern void del_word_for2lang(int16_t left_limit,int16_t right_limit);
extern int16_t line_number;         // number of line in ed_file

#define MAX_LEN_WORD  48
#define GOOD_PROB     200

Bool short_spell(uchar *wrd,uchar language, uchar nextlet )
{
Bool    short_word_solid;
switch( language )
    {
    case    LANG_RUSSIAN:
        short_word_solid=russian_word_all(wrd,language,nextlet);
        break;
    case    LANG_ENGLISH:
        short_word_solid=english_word_all(wrd,language);
        break;
    default:
        short_word_solid=FALSE;
        break;
    }
return short_word_solid;
}

Bool short_spell_re(uchar *wrd,uchar language )
{
Bool    short_word_solid;
switch( language )
    {
    case    LANG_RUSSIAN:
        short_word_solid=russian_word(wrd);
        break;
    case    LANG_ENGLISH:
        short_word_solid=english_word(wrd);
        break;
    default:
        short_word_solid=FALSE;
        break;
    }
return short_word_solid;
}

static uchar  cells_get_language(cell * c,cell * e)
{
cell *  r;
uchar    lang=c->language;
for(r=c->next;r && r!=e;r=r->next)
    {
    if( r->language!=lang )
        return 255;
    }
return lang;
}

static Bool  cells_get_solid(cell * rus, cell * ruse)
{
cell *r;

for(r=rus;r && r!=ruse;r=r->next)
    {
    if( !(r->flg&c_f_solid) )
        return FALSE;
    }
return TRUE;
}

Bool cells_left_solid(cell *c)
{
int16_t sol=0;
for(c=c->prev;(c->flg&(c_f_punct));c=c->prev);
if( c->flg&c_f_fict )
    return TRUE;
for(c=c->prev;!(c->flg&(c_f_fict|c_f_space));c=c->prev)
    {
    if( c->flg&c_f_solid )
        sol++;
    if( sol>1 )
        return TRUE;
    }
return (c->flg&c_f_fict)?TRUE:FALSE;
}


Bool cells_right_solid(cell *c)
{
int16_t sol=0;
for(c=c->next;(c->flg&(c_f_punct));c=c->next);
if( c->flg&c_f_fict )
    return TRUE;
for(c=c->next;!(c->flg&(c_f_fict|c_f_space));c=c->next)
    {
    if( c->flg&c_f_solid )
        sol++;
    if( sol>1 )
        return TRUE;
    }
return (c->flg&c_f_fict)?TRUE:FALSE;
}


// find and marked words similar to english words
void   set_spell_solid(void)
{
cell   *c, *e, *cc, *ce, *ee, *t;
uchar    word_len,buf[MAX_LEN_WORD+40], *wrd, *pwrd,
        buff[MAX_LEN_WORD+40], llanguage,
//      left_terms[]="<(Ч\"\'/[",
        left_terms[]="//<(\"\'/[",
//      right_terms[]="/)>ХШј\"!\';?\x1e\x1f",
        right_terms[]="///)>ј\"!\';?\x1e\x1f",	// 31.05.2001 E.P.
		nextlet;
int16_t		l,n=sizeof(buf)-1;

// Лиги задаются макросами и зависят от языка. 31.05.2001 E.P.
left_terms[0] = low_quotes;
left_terms[1] = left_quocket;

right_terms[0] = low_quotes;
right_terms[1] = right_quocket;

for(c=cell_f()->next;c!=cell_l();c=c->next)
    {
    c->solid_no=0;
    }
c=cell_f();
#define Etap 'l'

if( db_status && snap_activity(Etap) )
 {
 snap_show_text("Begin spell word solid");
 snap_monitor();
 }
while(1)
  {
  if( (c  = next_word(c))==NULL )break;
  ee= e = end_word(c,buf,&word_len,"/,(");
  l=strlen(buf);
  pwrd = buf+l;
  wrd  = buf;
  cc=c;
  while(   l<n && (c->flg==c_f_punct ||
            c->nvers && strchr(left_terms,c->vers[0].let) ) &&
            c->next!=e )
    {
    c=c->next;
    wrd++;
	l--;
    }
  ce=e;
  while(   l>0  && (e->prev->flg==c_f_punct ||
            e->prev->nvers && strchr(right_terms,e->prev->vers[0].let) ) &&
            e->prev!=c )
    {
    e=e->prev;
    pwrd--;
	l--;
    }

  *pwrd = 0;
  nextlet=( (ce->flg&c_f_punct) && ce->nvers)?ce->vers[0].let:'\x0';
  llanguage = cells_get_language(c,e);

  if( llanguage!=255 )
      {
      int16_t   u, d;
      cell  *cu, *cd;
      if ( !cells_get_solid(c,e) &&
            (short_spell(wrd,llanguage,nextlet ) ||
            //_spell_agressive(wrd,llanguage) ||
            _spell(wrd,llanguage) ||
            short_spell_re(wrd,llanguage )) )
        {
        if( !p2_active )
            {
            cc=c;
            ce=e;
            }
        if( cc->prev->nvers && strchr("\x1e\x1f",cc->prev->vers[0].let) )
            cc->prev->flg_new = c_fn_space ;
            //cc->prev->vers[0].let=' ';
        if( ce->nvers && strchr("\x1e\x1f",ce->vers[0].let) )
            ce->flg_new = c_fn_space ;
            //ce->vers[0].let=' ';
        valid_word_number++;
        for(cu=cd=NULL,u=d=0,t=cc;t!=ce;t=t->next)
            {
            t->flg |= c_f_solid;
            t->solid_no=valid_word_number;
            if( is_upper(t->vers[0].let) ) {u++;cu=t;}
            if( is_lower(t->vers[0].let) ) {d++;cd=t;}
            if( (t->flg&c_f_space) && t->nvers && t->vers[0].prob==75 )
                t=del_cell(t);

            }
        if( language==LANG_RUSSIAN && u>2 && d==1 && cd )
            {
            if( !strchr(" ЎҐ\xf0\xf1\xf5\xf7\xf8\xfd",cd->vers[0].let) )
                {
                cd->vers[0].let=to_upper(cd->vers[0].let);
                }
            }

        if( db_status && snap_activity(Etap) )
             {
             strcpy( buff, "Solid word : ");
             strcat( buff, wrd);
             snap_show_text(buff);
             snap_monitor();
             }
        }
      }
  if( ee==NULL )
    break;
  c = ee;
  }

c=cell_f();
while(1)
  {
  if( (c  = next_word(c))==NULL )break;
  ee= e = end_word(c,buf,&word_len,"");
  l=strlen(buf);
  pwrd = buf+l;
  wrd  = buf;
  while(   l<n&&(c->flg==c_f_punct ||
            c->nvers && strchr(left_terms,c->vers[0].let) ) &&
            c->next!=e )
    {
    c=c->next;
    wrd++;
	l--;
    }
  while(  l>0 && (e->prev->flg==c_f_punct ||
            e->prev->nvers && strchr(right_terms,e->prev->vers[0].let) ) &&
            e->prev!=c )
    {
    e=e->prev;
    pwrd--;
	l--;
    }

  *pwrd = 0;

  if( strlen(wrd)==1 && (c->flg&c_f_solid) &&
      !cells_left_solid(c) && !cells_right_solid(c) )
    {
    c->flg ^= c_f_solid;
    if( db_status && snap_activity(Etap) )
         {
         strcpy( buff, "Clear solid one-letter word : ");
         strcat( buff, wrd);
         snap_show_text(buff);
         snap_monitor();
         }
    }

  if( ee==NULL )
    break;
  c = ee;
  }

if( db_status && snap_activity(Etap) )
 {
 snap_show_text("End spell word solid");
 snap_monitor();
 }
#undef Etap

return;
}

cell * get_nonpunct_cell(cell *e)
{
cell *c=e;

while( c->flg!=c_f_fict && c->nvers && strchr("-.,",c->vers[0].let) )
    c=c->next;
return c;
}

cell *get_nonlet_cell(cell *c)
{
cell *p;
if( c==cell_f() )
    return c;
p = c->prev;
if( (c->flg&(c_f_let|c_f_bad)) &&
    (p->flg!=c_f_fict) && p->nvers &&
    strchr("-.,",p->vers[0].let))
    {
    c=p;
    }
return c;
}

Bool    nonpunct_interval(cell *b,cell *e)
{
cell *c;
for(c=b;c!=e;c=c->next)
    {
    if( c->flg&(c_f_let|c_f_bad|c_f_fict) )
        return TRUE;
    }
return FALSE;
}

uchar   english_word_recognize(void)
{
cell *c, *e;
uchar buf[MAX_LEN_WORD+40],is_eng,word_len,eng,r1,r2,val,BadWord;
uchar wrd[MAX_LEN_WORD+40];
int16_t  left_limit, right_limit, last_wrd;
cell    *right_cell, *left_cell  ;

if( db_status && snap_activity('c') )
 {
 snap_show_text("R&E : Begin english word select");
 snap_monitor();
 }

c=cell_f();
eng=0;
left_limit  = -16000;
left_cell   = cell_f();
right_limit = 0x7fff;
right_cell  = cell_l();
last_wrd=1; // russian
while(1)
  {
  BadWord = 0;
  if( (c  = next_word(c))==NULL )break;
  if( db_status && snap_activity('c') )
    strcpy(buf,"R&E : Word : ");
  e = end_word(c,wrd,&word_len,"");
  if( db_status && snap_activity('c') )
    strcpy(&buf[13],wrd);
  val = 0;

  if(!spec_camera&&russian_dictionary_word(c,e->prev,&BadWord))
    {
    BadWord = is_eng = 0;
    }
  else
    {
    is_eng=is_english_word(c,e);
    if(!is_eng && bad_chars_in_word(c,e))                BadWord = 1;
    if(!is_eng && !BadWord && more_alt(c,e))             BadWord = 2;
    if(!is_eng && !BadWord && small_prob_in_word(c,e))   BadWord = 3;
    }

  if( is_eng || BadWord)
    {
    eng++;
    val = 1;
    if( is_eng==1 )
      {
     // eng++;
      r1 = change_Il1(c,e);
      r2 = eng_recognize(c,e);
      if( r1 || r2 ) val = 1; // may be english word
      if( r2>1 )     val = 2; // english word
      }

    switch(BadWord)
      {
       case 0:           break;
       case 1:  val=1;
        if( db_status && snap_activity('c') )
        if( strlen(buf)+25<sizeof(buf) )
          strcat(buf," - non recog cell");    break;
       case 2:  val=3;
        if( db_status && snap_activity('c') )
        if( strlen(buf)+20<sizeof(buf) )
          strcat(buf," - many alternates");  break;
       case 3:  val=0;
        if( db_status && snap_activity('c') )
        if( strlen(buf)+20<sizeof(buf) )
          strcat(buf," - small prob");       break;
      }

    if( russian_word(wrd) )
      {
        is_eng=val=0;
        BadWord=4;
      }

    mark_two_lang_word(c,e,val);
    right_limit = get_left_coord(c);
    right_cell = c;
    if( !(c->prev->flg&c_f_fict) && nonpunct_interval(left_cell,right_cell) )
	  del_word_for2lang(left_limit,right_limit);
    e=get_nonpunct_cell(e);
    left_limit = get_right_coord(e);
    left_cell  = e;
    if( e->prev->save_stick_inc!=NO_INCLINE && !left_over(e,(int16_t)(left_limit+1)) )
      left_limit++;
    e=get_nonlet_cell(e);
    }
  switch( is_eng )
    {
    case 2  :
      if( db_status && snap_activity('c') )
      if( strlen(buf)+25<sizeof(buf) )
        strcat(buf," - english or russian");
      final_descriminate(c,e);
      break;
    case 1  :
      if( db_status && snap_activity('c') )
      if( strlen(buf)+15<sizeof(buf) )
        strcat(buf," - english");
      if( exist_symbol(c,e,'$') )
        final_descriminate(c,e);
      break;
    case 0  :
    default :
      if( db_status && snap_activity('c') )
      if( strlen(buf)+20<sizeof(buf) )
        strcat(buf," - russain");
      break;
    }


if( !is_eng && !BadWord )
  {
   // nonenglish word - auxilary methodes

  if( c->vers[0].let==(uchar)'Џ' && II_place(c) && II_config(c) )
    { // а §®аў ­­ п ЎгЄў  Џ
    eng++;
    down_all_versions(c, 22);
    mark_two_lang_word(c,e,1);
    right_limit = get_left_coord(c);
    right_cell  = c;
    if( db_status && snap_activity('c') )
    if( strlen(buf)+20<sizeof(buf) )
      strcat(buf," - broken II");
    if( !(c->prev->flg&c_f_fict) && nonpunct_interval(left_cell  ,right_cell  ))
      del_word_for2lang(left_limit,right_limit);
    e=get_nonpunct_cell(e);
    left_limit = get_left_coord(e);
    left_cell  = e;
    if( e->prev->save_stick_inc!=NO_INCLINE && !left_over(e,(int16_t)(left_limit+1)) )
      left_limit++;
    e=get_nonlet_cell(e);
    is_eng=1; // for SNAP
    }
  } // end of nonenglish word

  if( (is_eng || BadWord) && db_status && snap_activity('c') )
      {
      snap_newcell(c);
      snap_show_text(buf);
      snap_monitor();
      }
  last_wrd = (c->rus_eng_word==0);
  if( e==NULL )break;
  c = e;
  }

if( eng && !(e->prev->r_col==left_limit) && nonpunct_interval(left_cell  ,cell_l() ))
  { // kill last interval
  del_word_for2lang(left_limit,0x7fff);
  }
if( db_status && snap_activity('c') )
 {
 snap_show_text("R&E : End english word select");
 snap_monitor();
 }

return( eng!=0 );
}

Bool russian_dictionary_word(cell * first, cell * last,puchar BadWord)
{
#define DELTA            80
#define MINI_PROB	150
#define FINE_LET	200

char word[76],*pw;
int16_t  cl;
cell *c,*roll;

 for(; first != last; first=first->next)               // skip head punct
   if(first->flg & (c_f_bad|c_f_let)) break;

 for(; last != first; last=last->prev)                 // skip tail punct
  if(last->flg & (c_f_bad|c_f_let))   break;

 last = last->nextl;          // set  artifical value for termination only

 for(cl=0,c=first; c!=last; c=c->nextl,cl++)
   // check word for validity
   if((c->flg & (c_f_bad|c_f_punct)) ||
      (c->vers[0].prob < MINI_PROB)  ||
      is_digit(c->vers[0].let))         return FALSE;


 for(roll=first; roll != last; roll=roll->nextl) // loop through letters
    {
     version *v;
     for( v=(roll == first) ? roll->vers : roll->vers+1;
          v - roll->vers < roll->nvers &&
          roll->vers[0].prob - v->prob <= DELTA ; v++) // vers
      {
		        // Nick 29.10.2000
        // don't use small probabilities (only for first word MINI_PROB)
        if( v->prob <  FINE_LET &&
			v != roll->vers
		  )
		  break;


        for(c=first,pw=word; c != last; c=c->nextl)
         if( c != roll ) *pw++ = c->vers[0].let;
         else *pw++ = v->let;

        *pw=0; // terminate string

       if(_spell(word,language))
       {
        if( db_status && snap_activity('c') && (db_trace_flag & 2) )
            {
              snap_show_text(word);
              snap_monitor();
            }

         if(cl<5)   *BadWord=4;
         else       *BadWord=0;

		 if(cl==2 && !memcmp(word," §",2))
			return FALSE;

         return TRUE;
       }
      }
    }
 return FALSE;
}

Bool _spell(pchar s,uchar lang)
{

char        w[76]="",*pw; uchar ss;
int16_t         ret;

for(pw=w; *s; s++)
 {
 ss = *s;
 if( lang==LANG_RUSSIAN )
     {
     //                ..  ..
     // recode russian E   e
     if( ss==r_e_2dot )   ss=(uchar)'Ґ';
     if( ss==r_EE_2dot )  ss=(uchar)'…';

     }
 if( ss==liga_i ) ss='i';
 if( ss==liga_j ) ss='j';
 //strcat(pw,decode_ASCII_to_[ss]); // recode to MS Windows code table
 if( strlen(pw) + strlen(decode_ASCII_to_[ss])<75 )
	strcat(pw,decode_ASCII_to_[ss]);    // recode to MS Windows code table
 else
	break;
 }
if( strlen(w)<4 )
    return 0;
if( lang==LANG_ENGLISH && multy_language )
    ret = text_findstat_aux( w);
else
    ret = text_findstat(w);
return  ret > 0;
}

Bool _spell_agressive(pchar s,uchar lang)
{

char        w[76]="",*pw; uchar ss;
int16_t         ret;

if( lang!=LANG_RUSSIAN )
    return FALSE;

for(pw=w; *s; s++)
 {
 ss = *s;
 //strcat(pw,decode_ASCII_to_[ss]); // recode to MS Windows code table
 if( strlen(pw) + strlen(decode_ASCII_to_[ss])<75 )
	strcat(pw,decode_ASCII_to_[ss]);    // recode to MS Windows code table
 else
	break;
 }
if( strlen(w)<4 )
    return 0;
ret = text_findstat_agressive( w);
return  ret > 0;
}

static uchar russian_letters[] ="©ж­ийкдлў«¤¦нп¬ЁвмЎ‰–“ѓ™љ”Џ‹„†ќџ—€ЃрсчшАИ";
static uchar english_letters[] ="Ili";
static uchar two_lang_letters[]="ю234567890г“ЄЉҐ…ЌЈ§‡е•‚ ЂЇађ®Ћзб‘Њ’њх";
//static uchar eng_lang_letters[]="ю234567890yYkKeEHr33xXBaAnpPoOvcCMTbm";
static uchar punct_letters[]   ="(){}[]!ј,.\"':?\\/-_";
static uchar russian_prepositions[]=" ўб®ЄгЂ‚‘ЋЉ“эч";
static uchar critical_digitals[]="183";



// return : 0 - russian, 1 - english, 2 - russian or english
uchar is_english_word(cell *b,cell *e)
{
cell *c;
int16_t n,r,re,r1,rd,rp;
uchar ch,pr;

if( b==e->prev )
  { // one letter in word
  ch = b->vers[0].let;
  pr = b->vers[0].prob;
  if( strchr(punct_letters,ch) ) //,sizeof(punct_letters)) )
    return 0;
  if( ch==(uchar)'ю' )
	return 0;
  if( is_digit(ch) )
	return 0;
  if( strchr(russian_letters,ch) && //,sizeof(russian_letters)) &&
    pr>GOOD_PROB )
    return 0;
  if( strchr(russian_prepositions,ch) && //,sizeof(russian_prepositions)) &&
    pr>160 )
    return 0;
  }

for(rp=rd=r1=re=r=n=0,c=b;c!=e;c=c->next)
  {
  if( c->nvers<1 )  continue;
    //return 2; // may be english or russian
  ch = c->vers[0].let;
  pr = c->vers[0].prob;
  if( memchr(punct_letters,ch,sizeof(punct_letters)) )   continue;
  n++;
  if( memchr(russian_letters,ch,sizeof(russian_letters)) && pr>GOOD_PROB )
      r++;
  if( memchr(two_lang_letters,ch,sizeof(two_lang_letters)) && pr>GOOD_PROB )
      re++;
  if( memchr("оћ",ch,2) && pr>GOOD_PROB && !(c->cg_flag&c_cg_comp) )
      re++;
  if( memchr("оћ",ch,2) && pr>GOOD_PROB+30 && (c->cg_flag&c_cg_comp) )
      re++;
  if( memchr(russian_prepositions,ch,sizeof(russian_prepositions)) && pr>GOOD_PROB )
      rp++;
  if( is_digit(ch) )
      rd++;
  if( memchr(critical_digitals,ch,sizeof(critical_digitals)) &&
      pr<GOOD_PROB+20 )  // Oleg : 28.12.94 : '1','8' too similar to 'I','S'
      r1++;
  if( ch==(uchar)'ј' && pr<GOOD_PROB && c!=e->prev )
      r1++;              // ! whith glued . too similar 'I'
  }
if( n==1 && rp==1 ) return 0;
if( rd==n ) return (r1>0);
if( re==n ) return 2;
return r<1 || r==1 && r1;
}

uchar change_Il1(cell *b, cell *e)
{
cell *c;
int16_t  upper,n,digit,ret = FALSE, rc;
uchar let;
uchar saveN, saveV[VERS_IN_CELL*sizeof(version)];

for(n=digit=upper=0,c=b;c!=e;c=c->next,n++)
  {
  let = c->vers[0].let ;
  if( is_upper(let) || memchr("\\/",let,2) ) upper++;
  if( is_digit(let) ) digit++;
  }
if( digit==n )  return FALSE;
upper += digit;
upper = (upper==n);
if( n==3 && b->vers[0].let==(uchar)'Ђ' ) upper=0;
for(c=b;c!=e;c=c->next)
  if( (c->flg&c_f_let) && c->nvers>0 && memchr("1!ј",c->vers[0].let,3) )
    {
    saveN = (uchar)c->nvers;
    memcpy(saveV,c->vers,VERS_IN_CELL*sizeof(version));
    if( upper || c==b ) let='I';
    else                let='l';
    c->vers[0].let=let; c->vers[0].prob=254;
    c->vers[1].let=c->vers[1].prob=0;
    stick_center_study(c,NULL,1);
    rc = c->vers[0].prob;
    c->nvers = saveN ;
    memcpy(c->vers,saveV,VERS_IN_CELL*sizeof(version));
    if( rc>254-30 )
      ret = TRUE;
    }
return (uchar)ret;
}

uchar eng_recognize(cell *b, cell *e)
{
cell *c,cc;
int16_t n;
uchar let,two_l,eng_l,bad_l;

for(bad_l=eng_l=two_l=0,n=0,c=b;c!=e;c=c->next,n++)
  {
  let = c->vers[0].let;
  if( memchr(punct_letters,let,sizeof(punct_letters)) )
    {
    bad_l++;
    continue;
    }
  if( memchr(two_lang_letters,let,sizeof(two_lang_letters)) &&
      !memchr("§‡",let,2)  )
    {
    if( c->vers[0].prob>GOOD_PROB )
      two_l++;
    else
      {
      cc  = *c;
      if( english_cell_recognize(c) )
        eng_l++;
      *c=cc;
      }
    continue;
    }
  if( memchr(english_letters,let,sizeof(english_letters)) )
   {
   eng_l++;
   continue;
   }
  if( c->nvers<1 || memchr(russian_letters,let,sizeof(russian_letters)) ||
      memchr("§‡",let,2) )
   {
     if( c->vers[0].prob>GOOD_PROB )
      return 0; /* not english word : good russian letter */
   else
    {  /* bad russian letter */
    cc = *c;
    if( english_cell_recognize(c) )
      eng_l++;
    *c=cc;
    if( c->nvers<1 )bad_l++;
    } /* end of bad russian letter */
   } /* end of rus letter */

  } /* end of for */
return ( two_l+bad_l==n ? 1 : 2);
}

uchar english_cell_recognize(cell *cc)
{
s_glue GL={0};
uchar prob=cc->vers[0].prob, ccflg=(uchar)cc->flg;

if( !(cc->flg&(c_f_let|c_f_bad)) )
  return 0;

language = LANG_ENGLISH;
short_recog_cell(cc);

if( cc->nvers==0 && (ccflg&(c_f_let|c_f_bad)))
  {
  GL.celist[0]=cc;
  GL.maxlist[0]=cc;
  GL.complist[0]=cc->env;
  GL.maxlist[1]=GL.celist[1]=NULL;
  GL.complist[1]=NULL;
  GL.ncell=1;
  GL.row   = cc->row; GL.col    = cc->col;
  GL.width = cc->w;   GL.height = cc->h;

  if(crecell(cc,&GL,3)>=0)
    {
    dmBOX(cc,&GL);
    }
  }
cc->flg = ccflg;
criteria(cc);

language = LANG_RUSSIAN;
levcut(cc,1);

return  (cc->vers[0].prob>GOOD_PROB)||
        (cc->vers[0].prob>prob && cc->vers[0].prob>GOOD_PROB-30) ;
}

void mark_two_lang_word(cell *b, cell *e, uchar val)
{
cell *c;
for(c=b;c!=e;c=c->next)
  c->rus_eng_word = val;
return;
}


Bool bad_chars_in_word(cell *b,cell *e)
{
cell *c;
for(c=b;c!=e;c=c->next)
  if( c->nvers<1 )return TRUE;
return FALSE;
}

Bool small_prob_in_word(cell *b,cell *e)
{
cell *c;
for(c=b;c!=e;c=c->next)
  if( c->nvers<1 || c->nvers>0 && c->vers[0].prob<150 )return TRUE;
return FALSE;
}

uchar   small_english_str(void)
{
cell *c;
uchar non_base_define_letters[]="TYUuOoSsKZzXxCcVvHB3Ii1°0";
int16_t n,m;

for(c=cell_f()->nextl,n=0;c!=cell_l();c=c->nextl,n++);
for(c=cell_f()->nextl,m=0;c!=cell_l();c=c->nextl)
  if( c->nvers<1 || c->nvers>0 &&
      memchr(non_base_define_letters,c->vers[0].let,
             sizeof(non_base_define_letters)) )
      m++;
m = n-m;
if( n<=8 )
return (m<MAX(2,n-3)||n<3);
return( m<3 );
}

/* Ў« Ј®ЇаЁпв­®Ґ ¬Ґбв®а бЇ®«®¦Ґ­ЁҐ ¤«п Ї ал II */
Bool II_place(cell *c)
{
if( c->prev  && (c->prev->flg & c_f_space) &&
    c->next  && (c->next->flg & c_f_space) &&
    c->prevl && (c->prevl->flg & (c_f_let|c_f_bad)) &&
    c->nextl && (c->nextl->flg & (c_f_let|c_f_bad)) &&
    ( c->nextl->next && (c->nextl->next->flg & (c_f_let|c_f_bad)) ||
      c->prevl->prev && (c->prevl->prev->flg & (c_f_let|c_f_bad)) )
   )  return TRUE;

return FALSE;
}

Bool II_config(cell *c)
{
return c->broken_II;
}

Bool more_alt(cell *b,cell *e)
{
cell *c;
int16_t min_prob=1000;
int16_t flag_nvers=0;
int16_t flag_big_nvers=0;
for(c=b;c!=e;c=c->next)
 {
  if( c->nvers<1 )continue;
  if((c->flg & c_f_let) && ((min_prob > c->vers[0].prob) || (min_prob==1000)))
      min_prob=c->vers[0].prob;
  if((c->flg & c_f_let) && (c->nvers>1))
     {
       if(c->nvers > 7)  flag_big_nvers=1;
       flag_nvers=1;
     }
 }
if(flag_nvers && (min_prob< 220 || flag_big_nvers ))   return  TRUE;
else                              return FALSE;
}




static uchar russian_ligas[] ="рсхчшэАИ";
static uchar russian_ligas_recode[] ="¤¤вЁЈ Ґ…";
Bool russian_word(uchar *wrd)
{
int16_t i,ii,iv;
uchar w[MAX_LEN_WORD],c,*oc;

for(ii=i=0;wrd[i]!='\0'&&i<MAX_LEN_WORD-1;i++)
  if( !memchr(punct_letters,wrd[i],sizeof(punct_letters)) )
    {
    c = to_lower(wrd[i]);
    oc = memchr(russian_ligas,c,sizeof(russian_ligas));
    if( oc )
      c = russian_ligas_recode[oc-russian_ligas];
    w[ii++] = c;
    }
w[ii]=0;

for(i=0;russian_voc[i][0]!='\0';i++)
    {
    iv=strlen(russian_voc[i]);
    if( ii==iv &&
        !memcmp(russian_voc[i],w,iv) )
        return TRUE;
    }

return FALSE;
}

Bool russian_word_all(uchar *wrd,uchar language,uchar nextlet)
{
int16_t i,ii,iv;
uchar ww[MAX_LEN_WORD],*w=&ww[0],c;

if( language!=LANG_RUSSIAN )
    return FALSE;
*w=0;
for(ii=i=0;i<MAX_LEN_WORD-1&&wrd[i]!='\0';i++)
  if( !memchr(punct_letters,wrd[i],sizeof(punct_letters)) )
    {
    c = to_lower(wrd[i]);
    //strcat(w,decode_ASCII_to_[c]);
	if( strlen(w) + strlen(decode_ASCII_to_[c])<MAX_LEN_WORD-1 )
		strcat(w,decode_ASCII_to_[c]);
	else
		break;
    }

ii=strlen(w);
for(i=0;russian_voc_all[i][0]!='\0';i++)
    {
    iv=strlen(russian_voc_all[i]);
    if( ii==iv &&
        !memcmp(russian_voc_all[i],w,iv) )
        return TRUE;

    }
if( nextlet=='.' )
{
for(i=0;russian_voc_short[i][0]!='\0';i++)
    {
    iv=strlen(russian_voc_short[i]);
    if( ii==iv &&
        !memcmp(russian_voc_short[i],w,iv) )
        return TRUE;

    }
}
return FALSE;
}


Bool english_word(uchar *wrd)
{
int16_t i,ii,iv;
uchar w[MAX_LEN_WORD];

for(ii=i=0;wrd[i]!='\0'&&i<MAX_LEN_WORD-1;i++)
  if( !memchr(punct_letters,wrd[i],sizeof(punct_letters)) )
    w[ii++] = to_lower(wrd[i]);
w[ii]=0;

for(i=0;english_voc[i][0]!='\0';i++)
    {
    iv=strlen(english_voc[i]);
    if( ii==iv &&
        !memcmp(english_voc[i],w,iv) )
        return TRUE;
    }
return FALSE;
}

Bool english_word_all(uchar *wrd,uchar language)
{
int16_t i,ii,iv;
uchar w[MAX_LEN_WORD];
if( language!=LANG_ENGLISH )
    return FALSE;
for(ii=i=0;wrd[i]!='\0'&&i<MAX_LEN_WORD-1;i++)
  if( !memchr(punct_letters,wrd[i],sizeof(punct_letters)) )
    w[ii++] = to_lower(wrd[i]);
w[ii]=0;

for(i=0;english_voc_all[i][0]!='\0';i++)
    {
    iv=strlen(english_voc_all[i]);
    if( ii==iv &&
        !memcmp(english_voc_all[i],w,iv) )
        return TRUE;
    }
return FALSE;
}

static uchar rus_two_lang_letters[]="…’“ЋђЂЌЉ•‘‚ЊҐгЁ®а ебЇ1234567890,юЧШ!._<>;";
static uchar eng_two_lang_letters[]="ETYOPAHKXCBMeyuopaxcn1234567890,юЧШ!._<>;";
static uchar rus_two_lang_letters1[]="…’“ЋђЂЌЉ•‘‚ЊҐгЁ®а ебЇ";
static uchar eng_two_lang_letters1[]="ETYOPAHKXCBMeyuopaxcn";

static Bool mixed_eng_rus_word(cell *b, cell *e);
static Bool mixed_rus_eng_word(cell *b, cell *e);
static Bool eng_word_to_rus_word(cell *b, cell *e);
static Bool rus_word_to_eng_word(cell *b, cell *e);
static Bool russian_letter(cell *b,cell *e);
static Bool english_letter(cell *b,cell *e);

Bool mixed_eng_rus_word(cell *b, cell *e)
{
  cell *c;
  uchar *p;
  int16_t n,u,d;
  uchar wrd[MAX_LEN_WORD];

  for(c=b;c!=e;c=c->next)
    if( c->language!=LANG_RUSSIAN )
      break;      // another language
  if( c==e )
    return FALSE; // all cell haved actual language


  for(u=n=d=0,c=b;c!=e;c=c->next,n++)
    if( c->nvers>0 )
      {
      p=memchr(eng_two_lang_letters,c->vers[0].let,sizeof(eng_two_lang_letters)) ;
      u += is_upper(c->vers[0].let);
      d += (is_digit(c->vers[0].let) );
      if( p==NULL )
        return FALSE;
      if( n<MAX_LEN_WORD-1 )
        wrd[n]=c->vers[0].let;
      }
wrd[n]=0;

return ( u!=n && d!=n && !english_word(wrd) );
}

Bool mixed_rus_eng_word(cell *b, cell *e)
{
  cell *c;
  uchar *p;
  int16_t n,u,d;
  uchar wrd[MAX_LEN_WORD];

  for(c=b;c!=e;c=c->next)
    if( c->language!=LANG_ENGLISH )
      break;      // another language
  if( c==e )
    return FALSE; // all cell haved actual language


  for(u=n=d=0,c=b;c!=e;c=c->next,n++)
    if( c->nvers>0 )
      {
      p=memchr(rus_two_lang_letters1,c->vers[0].let,sizeof(rus_two_lang_letters1)) ;
      u += is_upper(c->vers[0].let);
      d += (is_digit(c->vers[0].let) );
      if( p==NULL )
        return FALSE;
      if( n<MAX_LEN_WORD-1 )
        wrd[n]=c->vers[0].let;
      }
wrd[n]=0;

return ( u!=n && d!=n && !russian_word(wrd) );
}


Bool eng_word_to_rus_word(cell *b, cell *e)
{
  cell *c;
  uchar *p,ch,pp;
//  uchar rot=0;
  for(c=b;c!=e;c=c->next)
    if( c->nvers>0 )
      {
      p=memchr(eng_two_lang_letters,c->vers[0].let,sizeof(eng_two_lang_letters)) ;
      if( p==NULL )
        return FALSE;
      pp = p-eng_two_lang_letters;
      ch = *(rus_two_lang_letters+pp);
      c->vers[0].let = ch;
      c->vers[1].let = c->vers[1].prob=0;
      c->language=LANG_RUSSIAN;
//      if( c->pos_inc&erect_old )  rot=1;
      c->nvers=1;
      }
//if( rot )
//	erection_one_incline_word(b, e);
return TRUE;
}

Bool rus_word_to_eng_word(cell *b, cell *e)
{
  cell *c;
  uchar *p,ch,pp;
//  uchar rot=0;
  for(c=b;c!=e;c=c->next)
    if( c->nvers>0 )
      {
      p=memchr(rus_two_lang_letters1,c->vers[0].let,sizeof(rus_two_lang_letters1)) ;
      if( p==NULL )
        return FALSE;
      pp = p-rus_two_lang_letters1;
      ch = *(eng_two_lang_letters1+pp);
      c->vers[0].let = ch;
      c->vers[1].let = c->vers[1].prob=0;
      c->language=LANG_ENGLISH;
//      if( c->pos_inc&erect_old )  rot=1;
      c->nvers=1;
      }
//if( rot )
//  erection_one_incline_word(b, e);
return TRUE;
}

Bool russian_letter(cell *b,cell *e)
{
cell *c;
for(c=b;c!=e;c=c->next)
    {
    if( c->nvers>0 &&
      (is_russian(c->vers[0].let)||
       memchr(punct_letters,c->vers[0].let,sizeof(punct_letters)) )
     )
        return TRUE;
    }
return FALSE;
}

Bool russian_letter_context(cell *b,cell *e)
{
cell *c;
for(c=b;c!=e;c=c->next)
    {
    if( c->nvers>0 && (is_russian(c->vers[0].let)||is_digit(c->vers[0].let)) )
        return TRUE;
    }
return FALSE;
}

Bool english_letter(cell *b,cell *e)
{
cell *c;
for(c=b;c!=e;c=c->next)
    {
    if( !(c->nvers>0 && is_english(c->vers[0].let)) )
        return FALSE;
    }

return TRUE;
}

Bool    special_eng_to_rus(cell *b,cell *e)
{
Bool ret=FALSE;

if( b->flg&c_f_fict )
    return ret;
b=b->prev;
if( e!=b && e->nvers && e->vers[0].let=='.' )
    {
    e=e->prev;
    if( e->nvers && e->vers[0].let=='r' )
        {
        if( e==b ||
            e!=b && e->prev && e->prev->nvers && strchr("0123456789I",e->prev->vers[0].let) )
            {
            e->nvers =1;
            e->vers[0].let='Ј';
            return TRUE;
            }
        }
    }
return ret;
}

void   russian_english_context(void)
{
cell *c, *e, *c1, *e1;
uchar buf[MAX_LEN_WORD+40],word_len,word_len1;
uchar wrd1[MAX_LEN_WORD+40];
int16_t  prev_wrd, next_wrd, curr_wrd;


if( db_status && snap_activity('c') )
 {
 snap_show_text("R&E : Begin russian & english context");
 snap_monitor();
 }

c=cell_f();
prev_wrd=1; // russian
while(1)
  {
  if( (c  = next_word(c))==NULL )break;
  if( db_status && snap_activity('c') )
    strcpy(buf,"R&E : Word : ");
  e = end_word(c,wrd1,&word_len,"");
  if( db_status && snap_activity('c') )
    strcpy(&buf[13],wrd1);
  curr_wrd = mixed_eng_rus_word(c,e);
  special_eng_to_rus(c,e);
  if( e!=NULL )
    {
    if( (c1  = next_word(e))!=NULL )
      {
      e1 = end_word(c1,wrd1,&word_len1,"");
      next_wrd = russian_letter_context(c1,e1);
      }
    else
      next_wrd=1;
    }
  else
    next_wrd=1;
  //if( p2_active || !(c->dupstart||e->prev->dupend) )
  if( prev_wrd && curr_wrd && next_wrd )
    { // prev is russian, curr is mixed, next is russian
    eng_word_to_rus_word(c, e);
    if( !p2_active &&  c->dupstart && e->prev->dupend )
        {
        c->dupstart=0;
        e->prev->dupend=0;
        }

    if( db_status && snap_activity('c') )
      {
      if( strlen(buf)+25<sizeof(buf) )
        strcat(buf," - english to russian");
      snap_newcell(c);
      snap_show_text(buf);
      snap_monitor();
      }
    }
  prev_wrd = russian_letter_context(c,e);
  if( e==NULL )break;
  c = e;
  }

c=cell_f();
prev_wrd=0; // russian
while(1)
  {
  if( (c  = next_word(c))==NULL )break;
  if( db_status && snap_activity('c') )
    strcpy(buf,"R&E : Word : ");
  e = end_word(c,wrd1,&word_len,"");
  if( db_status && snap_activity('c') )
    strcpy(&buf[13],wrd1);
  curr_wrd = mixed_rus_eng_word(c,e);
  if( e!=NULL )
    {
    if( (c1  = next_word(e))!=NULL )
      {
      e1 = end_word(c1,wrd1,&word_len1,"");
      next_wrd = english_letter(c1,e1);
      }
    else
      next_wrd=1;
    }
  else
    next_wrd=1;
  //if( p2_active || !(c->dupstart||e->prev->dupend) )
  if( prev_wrd && curr_wrd && next_wrd )
    { // prev is russian, curr is mixed, next is russian
    rus_word_to_eng_word(c, e);
    if( !p2_active &&  c->dupstart && e->prev->dupend )
        {
        c->dupstart=0;
        e->prev->dupend=0;
        }
    if( db_status && snap_activity('c') )
      {
      if( strlen(buf)+25<sizeof(buf) )
        strcat(buf," - russian to english ");
      snap_newcell(c);
      snap_show_text(buf);
      snap_monitor();
      }
    }
  prev_wrd = english_letter(c,e);
  if( e==NULL )break;
  c = e;
  }
/*
if( language == LANG_RUSSIAN && multy_language )
for(c=cell_f();c!=cell_l();c=c->next )
    {
    if(!( c->flg&(c_f_let|c_f_bad)) &&  c->language!=LANG_RUSSIAN )
        {
        c->language=LANG_RUSSIAN;
        }
    }
*/
if( db_status && snap_activity('c') )
 {
 snap_show_text("R&E : End russian & english context");
 snap_monitor();
 }

return;
}

Bool exist_symbol(cell *b,cell *e,uchar ch)
{
cell *c;
for(c=b;c!=e;c=c->next)
  if( c->nvers>0 && c->vers[0].let==ch )
    return TRUE;
return FALSE;
}


//static uchar left_limit_word[] =" .";
//static uchar right_limit_word[]=" -.РЭЮ.";
static uchar left_limit_word[] =" \x1f";
static uchar right_limit_word[]=" -\x1f\x2e"; // 31.05.2001 E.P.

cell * next_word(cell *cs)
{
cell *c=cs;

if( c==cell_l() ) return NULL;
do{
  c = c->next;
  }while( strchr(left_limit_word,c->vers[0].let) && //,sizeof(left_limit_word)) &&
          !(c->flg&c_f_fict) );

return (c->flg&c_f_fict)?NULL:c;
}


cell * end_word(cell *cs,uchar *str,uchar *word_len,uchar *add_letters)
{
cell *c=cs;
int16_t i=0;
int16_t let;

do{
  *str++  = c->vers[0].let;
  c = c->next;
  i++;
  }while( (!c->nvers ||
		!strchr(right_limit_word,
				(let=c->vers[0].let)
				) &&
		(
		// Лиги задаются макросами и зависят от языка. 31.05.2001 E.P.
		 let != liga_TM &&
		 let != liga_CR &&
		 let != liga_CC
		) &&
          !strchr(add_letters,c->vers[0].let)) &&
          !(c->flg&c_f_fict) && i<MAX_LEN_WORD-1);
*str='\0';
*word_len = (uchar)i;
return c;
}

Bool left_over(cell *b,int16_t limit)
{
cell *c,*e=cell_l()->prev;
for(c=b;c!=e;c=c->next )
  if( c->flg&(c_f_let|c_f_bad|c_f_punct|c_f_fict) ) break;

if( c->flg&c_f_fict ) return FALSE;

return c->r_col<=limit;
}

int16_t  get_left_coord(cell *b)
{
cell *c=b;
while( !(c->flg&(c_f_let|c_f_bad|c_f_fict|c_f_dust|c_f_punct)) )c=c->next;
return c->r_col;
}

int16_t  get_right_coord(cell *b)
{
cell *c=b->prev;
while( !(c->flg&(c_f_let|c_f_bad|c_f_fict|c_f_dust|c_f_punct)) )c=c->prev;
return c->r_col+c->w;
}


