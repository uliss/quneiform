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
/*                      Alphabet actions                            */
/*                                                                  */
/********************************************************************/
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
#include "ligas.h"
#include "tuner.h"
#include "tm.h"
#include "linear.h"
#include "linutil.h"
#include "alphabet.h"
#include "leo.h"

#include "compat_defs.h"

static uchar digital_mode;            // see in module pass3.c
static uchar plusminus_mode;
static uchar alphabet_set[32];
static cell *clist[2];
int16_t  digital_string_penalty;
extern uchar CodePages[];

Bool isLikeDigit(uchar c)
{
// С учетом турецко-русских конфликтов. 21.05.2002 E.P.
 if( memchr("0123456789Ћ®Ў‡§’‚шl",c,19) &&
	  !is_russian_turkish_conflict(c)
    )
	return TRUE;

 return FALSE;
}

Bool digital_last_context(void)
{
  cell *curr,*tmp;
  char punct_list[]="'\"=:";
  char sign_list[]="+-_";
  int16_t  l = strlen(punct_list),num,num_dig,num_bad,num_broken;
  uchar c,p;

if( db_status && snap_activity('c') )
 {
 snap_show_text("DIG : Digital last context");
 snap_monitor();
 }

 tmp = cell_f();
  while(   (tmp=tmp->next)->next )
    {
    if( tmp->nvers && tmp->vers[0].prob==2 )
      set_bad_cell(tmp);
    }
  num=num_dig=num_bad=num_broken=0;

  tmp = cell_f();
  while(   (tmp=tmp->next)->next && !(tmp->flg&(c_f_let|c_f_bad)) ); /* find let */
  curr = cell_f();
  if( tmp->next )
  {
  while( (curr=curr->next)!=tmp )
    if( (curr->flg&c_f_punct) &&
       !(curr->nvers>0 && memchr(sign_list,curr->vers[0].let,sizeof(sign_list))) )
      {
      curr = del_cell(curr);        /* kill first punctuation-cell */
      if( curr->next==NULL )break;
      }
  }

  curr = cell_l();
  while(   (curr=curr->prev)->prev && !(curr->flg&(c_f_let|c_f_bad)) ); /* find let */
  if( curr->next )
  {
  while( (curr=curr->next)->next )
    if( (curr->flg&c_f_punct) )
      {
      curr = del_cell(curr);        /* kill last punctuation-cell */
      if( curr->next==NULL )break;
      }

  }

  curr = cell_f();
  while(   (curr=curr->next)->next )
    {
    if( curr->flg&c_f_bad ) num_bad++;
    if( !(curr->flg&(c_f_let|c_f_punct)))  continue;

    c = curr->vers[0].let;
    if( memchr(punct_list,c,l) ||
        c==left_quocket || c==right_quocket ||
        c==low_quotes  // macros E.P.
	)
        {
        curr = del_cell(curr);
        if( curr->next==NULL )break;
        continue;
        }
        /*
    if( !enable_setup_alphabet && !test_alphabet_elem(c) )
        {
        curr = del_cell(curr);
        if( curr->next==NULL )break;
        continue;
        }
         */
    if( (curr->flg&c_f_let) )
      {
      //if( memchr("0123456789Ћ®Ў‡§’‚шl",c,19) &&
      if( isLikeDigit(c) && // 21.05.2002 E.P.
		  curr->vers[0].prob > 150  // Nick 26.02.2001
		)
		  num_dig++;
      if( memchr("‘",c,1) )num_broken++;
      if( curr->flg&c_f_let )num++;
      }
    }

if( digital_mode==2     ||
    num_dig*4>=num*3 ||
    num_dig==2 && num==3 ||
    num_dig==1 && num==1 && num_bad==1 )
  { /* digital string */
  curr = cell_f();
  if( digital_mode==2 )
  while(   (curr=curr->next)->next )
      {
      if( !(curr->flg&(c_f_let|c_f_punct)))  continue;

      c = curr->vers[0].let;
      if ( c==liga_exm || c=='!' )
          curr=del_cell(curr);

      }

  curr = cell_f();
  while(   (curr=curr->next)->next )
      {
      if( curr->nvers<1 )  continue;

      c = curr->vers[0].let;
      p = curr->vers[0].prob;
      if( c==(uchar)'_' )
        curr->vers[0].let='-' ;
      if( c==(uchar)'ш' )
        curr->vers[0].let='2' ;
      if( c==(uchar)'Ћ' ||
			// 21.05.2002 E.P.
			( c==(uchar)'®')&& !is_russian_turkish_conflict(c)
		)
        curr->vers[0].let='0' ;
      if( c==(uchar)'Ў' )
        curr->vers[0].let='6' ;
      if( c==(uchar)'‡' || c==(uchar)'§')
        curr->vers[0].let='3' ;
      if( c==(uchar)'‚' )
        curr->vers[0].let='8' ;
      if( c==(uchar)'‘' && (p<190 || num_broken+num_dig==num ) )
        curr->vers[0].let='0' ;
      if( c==(uchar)'’' || c==liga_exm || c=='!' || c=='|' )
        curr->vers[0].let='1' ;
      if( c==(uchar)'l' )
        curr->vers[0].let='1' ;

      if( c=='$' && curr->nvers==1 )
        {
        curr->vers[0].let='5';
        curr->vers[1].let='$';
        curr->vers[1].prob=curr->vers[0].prob-10;
        curr->nvers=2;
        }

      if( c=='5' && curr->nvers>2 && curr->vers[1].let==(uchar)'Ў' &&
          p==curr->vers[1].prob && curr->vers[2].let=='6' &&
          curr->vers[2].prob>150 )
          { /* alternates : Ў56 -->>CONTEXT-->> 5Ў6 */
          curr->vers[0].let='6';
          curr->vers[2].let='5';
          }
      }
  }

digital_mode=0;
set_all_alphabet();
for(curr=cell_f()->nextl;curr!=cell_l();curr=curr->nextl)
    {
    if( curr->flg&c_f_bad )
        {
        s_glue GL={0};
        short_recog_cell(curr);
        if( curr->nvers<1 )
          {
          GL.celist[0]=curr;
          GL.maxlist[0]=curr;
          GL.complist[0]=curr->env;
          GL.maxlist[1]=GL.celist[1]=NULL;
          GL.complist[1]=NULL;
          GL.ncell=1;
          GL.row   = curr->row; GL.col    = curr->col;
          GL.width = curr->w;   GL.height = curr->h;

          if( crecell(curr,&GL,3)>=0)
            dmBOX(curr,&GL);
          }
        criteria(curr);
        levcut(curr,1);

        if( curr->nvers<1 || curr->vers[0].prob<220 )
          { // restore bad sybmol
          set_bad_cell(curr);
          }

        if( curr->nvers && curr->vers[0].let=='|' )
		{
            curr->vers[0].let='1';

			// Nick add 26.02.2001 - test artificial '1'
			levcut(curr,1);
            if( curr->nvers<1 || curr->nvers==1 && curr->vers[0].let!='1' || curr->vers[0].prob<220 )
			{ // restore bad sybmol
              set_bad_cell(curr);
			}
		}
      }
    }

    for(curr=cell_f()->nextl;curr!=cell_l();curr=curr->nextl)
    {
    if( curr->nvers && curr->vers[0].let=='(' &&
        curr->next &&
        curr->next->nvers && curr->next->vers[0].let==')' )
        {
        clist[0]=curr;
        clist[1]=curr->next;
        compose_cell(2,clist,curr);
        curr->vers[0].let='0';
        curr->vers[1].prob=220;
        curr->nvers=1;
        }
    }

if( db_status && snap_activity('c') )
 {
 snap_show_text("DIG : Digital last context end");
 snap_monitor();
 }

 num_dig = 0;   // Nick 26.02.2001
 num = 0;
 for(curr=cell_f()->nextl;curr!=cell_l();curr=curr->nextl,num++)
 {
 // 21.05.2002 E.P.
 Bool likeDigit = isLikeDigit(curr->vers[0].let);

 if( curr->nvers<1 || curr->vers[0].prob<150 && likeDigit)
    return 0;  // bad symbols or small propability

  if( likeDigit )
	  num_dig++;
 }

 if( num_dig*2 <= num )  // Nick 26.02.2001
	 return 0;

 return 1;
}

void add_digital_versions(void)
{
cell *curr;
uchar c,p;

curr = cell_f();
while(   (curr=curr->next)->next )
    {
    if( !(curr->flg&(c_f_let|c_f_bad)) )  continue;
    c = curr->vers[0].let;
    p = curr->vers[0].prob;
    if( c=='|' && curr->nvers==2 )
      {
      c = curr->vers[1].let;
      p = curr->vers[1].prob;
      }
    if( (curr->nvers==1 || c==(uchar)'’' && curr->nvers==2)&& p>220 )
        switch( c )
          {
          case (uchar)'’' : add_stick_vers (curr, '1', p);break;
          case (uchar)'Ў' : add_stick_vers (curr, '6', p);break;
          default        : break;
          }
    }

return;
}


void setup_digital_mode_pass2(str_info *str, int16_t first_pass)
{
cell *curr;
int16_t  num_dig,num_let,num_bad,num_all, num_dig_let;
///char dig_list[]="|1234567890ЎЋ®‡§‚шў", 21.05.2002 E.P.
char dig_let_list[]="036",let_dig_list[]="‚ўш";
// letters_ini пунктуацию переводит в letter !!!
char punct_list[]="'\"=:!ј?\\.,-"; // remove (){}[]/
uchar c,p;

digital_mode=0;
set_all_alphabet();
curr = cell_f();
num_all=num_dig=num_let=num_bad=num_dig_let=0;
while(   (curr=curr->next)->next )
    {
    if( !(curr->flg&c_f_let) ) //!(curr->flg&(c_f_let|c_f_bad)) )
        continue;
    num_all++;
    if( (curr->flg&c_f_bad) )
        num_bad++;
    else
      {
      c = curr->vers[0].let;
      p = curr->vers[0].prob;
      if( memchr(punct_list,c,sizeof(punct_list)) )
        {
        num_all--;
        continue;
        }
      if( !isLikeDigit(c) )
	  {
        if( p>220 )
          num_let++;
		if( p > 240 ) // Nick 11.05.2001
		{
			//num_let++;
		    if( curr->nvers < 2  ||
				!isLikeDigit(curr->vers[1].let) ||
				curr->vers[1].prob < 220
			  )
			  num_dig--; // штраф на цифровой
		}
	  }
      if( isLikeDigit(c) )
        {
        num_dig++;
        if( memchr(let_dig_list,c,sizeof(let_dig_list)) && p>220 )
          num_let++;
        }
      if( memchr(dig_let_list,c,sizeof(dig_let_list)) )
        num_dig_let++;
      }
    }


str->all += num_all;

//if( num_dig < 0 )
//   num_dig = 0;

str->dig += num_dig;
str->fuzzy_dig += num_dig_let;

return;
}


void setup_digital_mode(str_info *str, int16_t first_pass)
{
cell *curr;
int16_t  num_dig,num_let,num_bad,num_all, num_dig_let;
int16_t  num_asOne;
//char dig_list[]="|1234567890ЎЋ®‡§‚шў",
char   dig_let_list[]="036",let_dig_list[]="‚ўш";
//char punct_list[]="'\"=:!(){}[]!ј?\\/";
char punct_list[]="'\"=:;";
char asOne_list[]="1[]/!|";
uchar c,p;

digital_mode=0;
set_all_alphabet();
curr = cell_f();
num_all=num_dig=num_let=num_bad=num_dig_let=0;
num_asOne = 0;

while(   (curr=curr->next)->next )
    {
    if( !(curr->flg&(c_f_let|c_f_bad)) )
		continue;

    num_all++;
    if( (curr->flg&c_f_bad) )
		num_bad++;
    else
      {
      c = curr->vers[0].let;
      p = curr->vers[0].prob;

      if( memchr(punct_list,c,sizeof(punct_list)) )
        {
        num_all--;
        continue;
        }

	  if( memchr(asOne_list,c,sizeof(asOne_list)) )
	  {
		  num_asOne++;
	  }

      if( !isLikeDigit(c) && p>220 ) // 21.05.2002 E.P.
        num_let++;
      if( isLikeDigit(c) )
        {
        num_dig++;
        if( memchr(let_dig_list,c,sizeof(let_dig_list)) && p>220 )
          num_let++;
        }
      if( memchr(dig_let_list,c,sizeof(dig_let_list)) )
        num_dig_let++;
      }
    }

if( str->alphabet==ALPHA_ALL )
  {
  int16_t dig=0;
  switch( num_all )
    {
    case 0  : break;
    case 1  :
      if( num_dig>0 && num_dig > num_asOne )
        dig = 2;
      break;
    case 2  :
      if( num_dig>0 && num_dig > num_asOne && num_let==0 )
        dig = 2;
      break;
    case 3  :
      if( num_dig>1 && num_dig > num_asOne && num_let==0 )
        dig = 2;
      break;
    case 4  :
      if( num_dig>2 && num_let==0 && num_dig > num_asOne )
        dig = 2;
      break;

    default :
      if( num_dig>MAX(2,(3*num_all)/4) && num_let==0 ||
          num_dig>0 && num_dig > num_asOne && num_bad==0 && num_let==0
		 )
        dig = 2;
      else if( num_dig*10>num_all*6 && num_all>5 /*&& num_bad*/)
        dig = 1;
      break;
  }

  if( dig==1 && num_dig*10>num_all*6 && num_all>5 /*&& num_bad*/)
    {
    digital_mode=1;
    }

  if( dig==2 &&
      (num_dig>num_all/2&&num_all>5 || str->dig*4>str->all || first_pass) )
    {
    digital_mode=2;
    set_digital_alphabet();
    if( str->dollars )
      add_alphabet_elem('$');
    }
  }
  else if (str->alphabet==ALPHA_DIGITAL || str->alphabet==ALPHA_DIGITAL_TRUE)
  {
	if( num_dig>=MAX(1,num_all/2) && num_let<MAX(1,num_all/4) ||
      num_dig>0 && num_bad==0 && num_let==0)
      {
      digital_mode=2;
      set_digital_alphabet();
      if( str->dollars )
        add_alphabet_elem('$');
      }
  }

  if( digital_mode==2 )
  {
    digital_string_penalty = (num_dig_let!=num_dig)?254:20;
    for(curr = cell_f();curr!=cell_l();curr=curr->nextl)
    {
    if( !(curr->flg&c_f_let) )
		continue;

    if( curr->nvers>0 &&
	    (
         memchr(punct_list,curr->vers[0].let,sizeof(punct_list)) ||
		 curr->vers[0].let != '1' && memchr(asOne_list,curr->vers[0].let,sizeof(asOne_list))
		)
	  )
      {
          if( curr->vers[0].prob>220 && !memchr("()",curr->vers[0].let,2) )
            add_alphabet_elem(curr->vers[0].let);
          continue;
      }

    if( !is_digit(curr->vers[0].let) )
      down_all_versions(curr,100);
    }
  }

  return;
}

void clear_digital_mode(void)
{
digital_mode=0;
set_all_alphabet();
digital_string_penalty=0;
return;
}

void add_alphabet_elem(uchar let)
{
uchar add_mask[]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
alphabet_set[let/8] |= add_mask[ let%8 ];

return;
}

void del_alphabet_elem(uchar let)
{
uchar del_mask[]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
alphabet_set[let/8] &= ~del_mask[ let%8 ];

return;
}

Bool test_alphabet_elem(uchar let)
{
uchar test_mask[]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};

return (alphabet_set[let/8] & test_mask[ let%8 ]);
}

void copy_alphabet(puchar alph)
{
memcpy(alphabet_set,alph,32);
//enable_setup_alphabet = FALSE; //???

return;
}

void set_all_alphabet(void)
{
  int16_t i;
  for(i=0;i<32;i++)
    alphabet_set[i] = 0xFF;

  del_alphabet_elem(0);

/*
  // В болгарском нет ЫыЭэE:e: 08.09.2000 E.P.
  if (language==LANG_RUSSIAN && langBul)
	{
	del_alphabet_elem(r_EE_2dot);
	del_alphabet_elem(r_e_2dot);
	del_alphabet_elem(155); // Ы
	del_alphabet_elem(235);	// ы
	del_alphabet_elem(157);	// Э
	del_alphabet_elem(237); // э
	}
*/


  return;
}

void set_digital_alphabet(void)
{
  memset(alphabet_set,0,32);
  add_alphabet_elem('0');
  add_alphabet_elem('+');
  add_alphabet_elem('1');
  add_alphabet_elem('2');
  add_alphabet_elem('3');
  add_alphabet_elem('4');
  add_alphabet_elem('5');
  add_alphabet_elem('6');
  add_alphabet_elem('7');
  add_alphabet_elem('8');
  add_alphabet_elem('9');

  // 21.05.2002 E.P.
  if (!is_turkish_language(language))
	add_alphabet_elem((uchar)'®');

  add_alphabet_elem((uchar)'Ћ');
  add_alphabet_elem((uchar)'Ў');
  add_alphabet_elem((uchar)'‡');
  return;
}

void set_digital_mode(void)
{
digital_mode=2;
set_digital_alphabet();
//digital_string_penalty=0;
return;
}
extern puchar save_raster_align8(cell *c);
void save_alphabet_information(str_info *str,int16_t column,CSTR_line ln)
{
uchar            buf[256]={0};
cell *          c;
RecObject       ro={0};
CSTR_rast       rst;
RecRaster       rrst;
RecVersions     ver;
LeoFieldSetup   fs={0};
CSTR_attr       lattr;
CSTR_rast_attr  attr;
UniVersions     uvs;

if( db_status && snap_activity('n') )
 {
 snap_show_text("DIG : start selection");
 snap_monitor();
 }
str[column].all++;
fs.nStyle = LS_PRINT;
buf['0']=buf['1']=buf['2']=buf['3']=buf['4']=
buf['5']=buf['6']=buf['7']=buf['8']=buf['9']=1;
memcpy(fs.AlphaTable,buf,256);
#ifdef _USE_LEO_
LEOSetupField(&fs);
for(c=cell_f()->nextl;c!=cell_l();c=c->nextl)
    {
    if( c->nvers==0 && c->w*2>c->h && c->h*2>c->w )
        {
        puchar           wr;
        // to work_rst
        wr=save_raster_align8(c);
        // rst -->> rrst
        rrst.lnPixHeight = c->h;
        rrst.lnPixWidth  = c->w;
        rrst.lnRasterBufSize = REC_MAX_RASTER_SIZE;
        memcpy(rrst.Raster,wr,((c->w+63)/64)*8*c->h);
        memcpy(&ro.recData.recRaster,&rrst,REC_MAX_RASTER_SIZE);
        LEORecogPrnMethod( &ro , REC_METHOD_3X5, 1);
        memcpy(&ver,&ro.recResults,sizeof(ver));
        if( ver.lnAltCnt && ver.Alt[0].Prob>220 )
            {
            c->nvers=1;
            c->vers[0].let=ver.Alt[0].Code;
            c->vers[0].prob=ver.Alt[0].Prob;
            c->vers[1].let=0;
            c->flg=c_f_let;
            }
        }
    }
#endif
setup_digital_mode(&str[column], 1);
for(c=cell_f()->next;c!=cell_l();c=c->next)
  if( c->nvers>0 && c->vers[0].let=='$' && c->vers[0].prob>220 )
    {
    str[column].dollars++;
    break;
    }
if( digital_mode==2 )
  {
  str[column].dig++;
  if( db_status && snap_activity('n') )
    {
    sprintf(buf,"DIG : digital str, col %d, all %d, dig %d, plus_minus %d" ,
                                    column, str[column].all,str[column].dig,str[column].pm);
    snap_show_text(buf);
    snap_monitor();
    }

  }
else if( digital_mode==1 )
  {
  str[column].fuzzy_dig++;
  if( db_status && snap_activity('n') )
    {
    sprintf(buf,"DIG : fuzzy digital str, col %d, all %d, dig %d, plus_minus %d" ,
                                    column, str[column].all,str[column].dig,str[column].pm);
    snap_show_text(buf);
    snap_monitor();
    }

  }
else
    {
    plusminus_mode=0;
    if( cell_f()->next && cell_f()->next==cell_l()->prev )
        {
        c=cell_f()->next;
        rst=CSTR_GetNextRaster(CSTR_GetFirstRaster(ln),CSTR_f_bad|CSTR_f_let);

        if( rst && CSTR_GetImage(rst,(uchar *)&rrst,CSTR_TYPE_IMAGE_RS))
            {
            if( c->w>c->h*5
				|| c->w>c->h*3 && c->env && c->env->nl == 1 // Nick 18.01.2002
			  )
                {
                c->nvers=1;
                c->vers[0].let='-';
                c->vers[0].prob=254;
                ver.Alt[0].Code='-';
                ver.Alt[0].Prob=255;
                ver.lnAltCnt =1;
                CSTR_StoreCollection(rst,&ver);
                CSTR_GetAttr(rst,&attr);
                //attr.keg=Ps<<line_scale;
                CSTR_SetAttr(rst,&attr);
                CSTR_GetLineAttr(ln,&lattr);
                lattr.Flags |= CSTR_STR_ReadyResults ;
                CSTR_SetLineAttr(ln,&lattr);
                plusminus_mode=1;
                }
            else if( abs(c->w-c->h)<5  )
                {
                #ifdef _USE_LEO_
                fs.nStyle = LS_PRINT;
                buf['+']=1;
                memcpy(fs.AlphaTable,buf,256);
                LEOSetupField(&fs);
                memcpy(&ro.recData.recRaster,&rrst,REC_MAX_RASTER_SIZE);
                LEORecogPrnMethod( &ro , REC_METHOD_3X5, 1);
                memcpy(&ver,&ro.recResults,sizeof(ver));
                if( ver.lnAltCnt && ver.Alt[0].Code=='+' && ver.Alt[0].Prob>220 )
                    {
                    c->nvers=1;
                    c->vers[0].let='+';
                    c->vers[0].prob=254;
                    ver.lnAltCnt =1;

                    CSTR_StoreCollection(rst,&ver);
                    CSTR_GetAttr(rst,&attr);
                    //attr.keg=Ps<<line_scale;
                    CSTR_SetAttr(rst,&attr);
                    CSTR_GetLineAttr(ln,&lattr);
                    lattr.Flags |= CSTR_STR_ReadyResults ;
                    CSTR_SetLineAttr(ln,&lattr);
                    plusminus_mode=1;
                    }
                #endif
                }
            }
        }
    if( plusminus_mode )
      {
      str[column].pm++;
      rst=CSTR_GetNextRaster(CSTR_GetFirstRaster(ln),CSTR_f_bad|CSTR_f_let);
      CSTR_GetCollectionUni(rst,&uvs);
      if( uvs.lnAltCnt )
          {
          uvs.Alt[0].Charset=CodePages[language];
          CSTR_StoreCollectionUni(rst,&uvs);
          }
      if( db_status && snap_activity('n') )
        {
        sprintf(buf,"DIG : plus_minus str , col %d, all %d, dig %d, plus_min %d" ,
                                        column, str[column].all,str[column].dig,str[column].pm);
        snap_show_text(buf);
        snap_monitor();
        }

      }
    else
        {
        if( db_status && snap_activity('n') )
            {
            sprintf(buf,"DIG : unknown alphabet , col %d, all %d, dig %d, plus_min %d" ,
                                            column, str[column].all,str[column].dig,str[column].pm);
            snap_show_text(buf);
            snap_monitor();
            }
        }
    }

return;
}

int16_t  dichotomy_array(int16_t value,int16_t *array,int16_t start,int16_t end)
{
int16_t mid;

do{
  mid = (start+end)/2;
  if( value<array[mid] )
    end   = mid;
  else
    start = mid;
}while( end-start>1 );
return start;
}

int16_t  GetColumn(int16_t *VertLines,int16_t VertNum )
{
int16_t start_col, end_col;
int16_t col;
start_col = cell_f()->next->col;
end_col   = cell_l()->prev->col;
if( start_col<VertLines[0] || start_col>=VertLines[VertNum-1] ) return -1;
col = dichotomy_array(start_col,VertLines,0,VertNum);
if( end_col>VertLines[col+1] ) return -1;
return col;
}

void make_simples_diff(int16_t lang)
{
extern char db_pass;
cell *c,*e=cell_l();
int16_t  dbp = db_pass;

db_pass=0;
for(c=cell_f()->next;c!=e;c=c->next)
  if( !c->env->scale )
    {
    criteria(c);
    if( lang==LANG_RUSSIAN )
      r_criteria(c,NULL);
    }
db_pass = (uchar)dbp;
return;
}

void set_column_alphabets(str_info *str_inf,int16_t VertNum)
{
int16_t i;
for(i=0;i<VertNum;i++)
  {
  if( str_inf[i].all>10 )
    {
    if( str_inf[i].dig*2>=(str_inf[i].all-str_inf[i].pm) )
        str_inf[i].alphabet=ALPHA_DIGITAL;      // digital column
    if( str_inf[i].fuzzy_dig*10>(str_inf[i].all-str_inf[i].pm)*6 )
        str_inf[i].alphabet=ALPHA_DIGITAL;
    if( str_inf[i].dig*4>(str_inf[i].all-str_inf[i].pm)*3 ||
        str_inf[i].all-str_inf[i].dig<3 )
        str_inf[i].alphabet=ALPHA_DIGITAL_TRUE; // digital column
	// was *4 > *3 - bad on Yel_015_r (Nick 15.05.2001)
    if( str_inf[i].pm*5>str_inf[i].all*4 ||
        str_inf[i].all-str_inf[i].pm<3)
        str_inf[i].alphabet=ALPHA_PLUS_MINUS;
    }
  else if( str_inf[i].all>0 )
    { // small column
    if( str_inf[i].dig*3>=str_inf[i].all*2 )
        str_inf[i].alphabet=ALPHA_DIGITAL;      // digital column
    if( str_inf[i].fuzzy_dig*10>str_inf[i].all*6 )
        str_inf[i].alphabet=ALPHA_DIGITAL;
    if( str_inf[i].all>5 && str_inf[i].dig*10>=str_inf[i].all*9 ||
        str_inf[i].all>3 && str_inf[i].dig==str_inf[i].all )
        str_inf[i].alphabet=ALPHA_DIGITAL_TRUE;
    if( str_inf[i].pm*2>str_inf[i].all*1 )
        str_inf[i].alphabet=ALPHA_PLUS_MINUS;
    }
  }
return;
}


Bool is_digital_string(void)
{
  return(digital_mode!=0);
}
/////////////////////////////////////////////
// SERBIAN section
/////////////////////////////////////////////
static int16_t geom_neck(int16_t neck, cell *c)
{
uchar inter_l,h, nl;
lnhead   *line;
interval *inter;
int16_t ll, pos_n;

if( !c->env )
  return 0;
for (pos_n=nl=0,inter_l=255,line=(lnhead *)((pchar)(c->env)+c->env->lines+sizeof(int16_t));
		(ll=line->lth)>0; line=(lnhead *)((pchar)line+ll))
	{
	h=(uchar)line->h;
  if( nl>1 )
    return( 0 );
  if( h>1 )
    nl ++;

  for( inter=(interval *)((pchar)line+sizeof(lnhead));
       h ;h--,inter++)        /* one line     */
		{
    if( nl && inter_l > inter->l && h<line->h-3 && h>line->h/2 )
      {
      inter_l = inter->l;
      pos_n   = line->h-h;
      }
		}
  }
return (abs(pos_n-neck)<3)?pos_n:0;
}


void serbian_J2j(void)
{
B_LINES my_bases;
cell *c,*e;
int16_t   n;
char bb[90];

get_b_lines(NULL,&my_bases);
for(c=cell_f()->nextl,e=cell_l();c!=e;c=c->nextl)
  if( (c->flg&c_f_let) && c->nvers && c->w*3<=c->h &&
      (c->vers[0].let=='J' ||
        memchr("[]!",c->vers[0].let,3)&&
          c->row<(my_bases.b2 + my_bases.b1)/2&&
          c->row+c->h>(my_bases.b3 + my_bases.b4)/2) )
    {
    if( (n=geom_neck((int16_t)(my_bases.b2 - my_bases.b1),c))!=0 )
      {
      if( db_status && snap_activity('c') )
        {
        sprintf (bb,"serbian reverse J-->>j. bs= %d %d (%d). find neck on %d",
          my_bases.b1, my_bases.b2, my_bases.b2 - my_bases.b1, n);
        snap_show_text(bb);
        snap_monitor();
        }
      c->vers[0].let='j';

      }
    if( c->nvers && c->vers[0].let=='j' && check_let(c, '/') )
        del_version(  c,'/');
    }
return;
}

///////////
void save_alphabet_information_pass2(str_info *str,int16_t column,CSTR_line ln)
{
uchar            buf[256]={0};

if( db_status && snap_activity('n') )
 {
 snap_show_text("DIG : start selection");
 snap_monitor();
 }
//str[column].all++;
/*
fs.nStyle = LS_PRINT;
buf['0']=buf['1']=buf['2']=buf['3']=buf['4']=
buf['5']=buf['6']=buf['7']=buf['8']=buf['9']=1;
memcpy(fs.AlphaTable,buf,256);
#ifdef _USE_LEO_
LEOSetupField(&fs);
for(c=cell_f()->nextl;c!=cell_l();c=c->nextl)
    {
    if( c->nvers==0 && c->w*2>c->h && c->h*2>c->w )
        {
        puchar           wr;
        // to work_rst
        wr=save_raster_align8(c);
        // rst -->> rrst
        rrst.lnPixHeight = c->h;
        rrst.lnPixWidth  = c->w;
        rrst.lnRasterBufSize = REC_MAX_RASTER_SIZE;
        memcpy(rrst.Raster,wr,((c->w+63)/64)*8*c->h);
        memcpy(&ro.recData.recRaster,&rrst,REC_MAX_RASTER_SIZE);
        LEORecogPrnMethod( &ro , REC_METHOD_3X5, 1);
        memcpy(&ver,&ro.recResults,sizeof(ver));
        if( ver.lnAltCnt && ver.Alt[0].Prob>220 )
            {
            c->nvers=1;
            c->vers[0].let=ver.Alt[0].Code;
            c->vers[0].prob=ver.Alt[0].Prob;
            c->vers[1].let=0;
            c->flg=c_f_let;
            }
        }
    }
#endif
*/
setup_digital_mode_pass2(&str[column], 1);
if( digital_mode==2 )
  {
  str[column].dig++;
  if( db_status && snap_activity('n') )
    {
    sprintf(buf,"DIG : digital str, col %d, all %d, dig %d, plus_minus %d" ,
                                    column, str[column].all,str[column].dig,str[column].pm);
    snap_show_text(buf);
    snap_monitor();
    }

  }


return;
}

void set_column_alphabets_pass2(str_info *str_inf,int16_t VertNum)
{
int16_t i;
for(i=0;i<VertNum;i++)
  {
  if( str_inf[i].all>10 )
    {
	  // Nick - new vers.
	  // old bad on En_18,t_t_14,saus32
     if( str_inf[i].dig*100 > str_inf[i].all*92 )
      str_inf[i].alphabet=ALPHA_DIGITAL_TRUE; // digital column
/*
    if( str_inf[i].dig*2>=(str_inf[i].all-str_inf[i].pm) &&
        str_inf[i].dig  >=str_inf[i].fuzzy_dig*2)
        str_inf[i].alphabet=ALPHA_DIGITAL_TRUE; // digital column
    }
  else if( str_inf[i].all>0 )
    { // small column
    if( str_inf[i].dig*3>=str_inf[i].all*2 )
        str_inf[i].alphabet=ALPHA_DIGITAL_TRUE;
*/
    }

  }
return;
}

