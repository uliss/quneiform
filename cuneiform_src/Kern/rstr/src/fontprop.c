/*
Copyright (c) 1993-2008, Cognitive Technologies
All rights reserved.

–азрешаетс€ повторное распространение и использование как в виде исходного кода,
так и в двоичной форме, с изменени€ми или без, при соблюдении следующих условий:

      * ѕри повторном распространении исходного кода должны оставатьс€ указанное
        выше уведомление об авторском праве, этот список условий и последующий 
        отказ от гарантий. 
      * ѕри повторном распространении двоичного кода в документации и/или в 
        других материалах, поставл€емых при распространении, должны сохран€тьс€
        указанна€ выше информаци€ об авторском праве, этот список условий и
        последующий отказ от гарантий.  
      * Ќи название Cognitive Technologies, ни имена ее сотрудников не могут 
        быть использованы в качестве средства поддержки и/или продвижени€ 
        продуктов, основанных на этом ѕќ, без предварительного письменного
        разрешени€. 

Ё“ј ѕ–ќ√–јћћј ѕ–≈ƒќ—“ј¬Ћ≈Ќј ¬Ћјƒ≈Ћ№÷јћ» ј¬“ќ–— »’ ѕ–ј¬ »/»Ћ» ƒ–”√»ћ» Ћ»÷јћ» " ј 
ќЌј ≈—“№" Ѕ≈«  ј ќ√ќ-Ћ»Ѕќ ¬»ƒј √ј–јЌ“»…, ¬џ–ј∆≈ЌЌџ’ я¬Ќќ »Ћ» ѕќƒ–ј«”ћ≈¬ј≈ћџ’, 
¬ Ћё„јя √ј–јЌ“»»  ќћћ≈–„≈— ќ… ÷≈ЌЌќ—“» » ѕ–»√ќƒЌќ—“» ƒЋя  ќЌ –≈“Ќќ… ÷≈Ћ», Ќќ Ќ≈
ќ√–јЌ»„»¬јя—№ »ћ». Ќ» ¬Ћјƒ≈Ћ≈÷ ј¬“ќ–— »’ ѕ–ј¬ » Ќ» ќƒЌќ ƒ–”√ќ≈ Ћ»÷ќ,  ќ“ќ–ќ≈ 
ћќ∆≈“ »«ћ≈Ќя“№ »/»Ћ» ѕќ¬“ќ–Ќќ –ј—ѕ–ќ—“–јЌя“№ ѕ–ќ√–јћћ”, Ќ» ¬  ќ≈ћ —Ћ”„ј≈ Ќ≈ 
Ќ≈—®“ ќ“¬≈“—“¬≈ЌЌќ—“», ¬ Ћё„јя ЋёЅџ≈ ќЅў»≈, —Ћ”„ј…Ќџ≈, —ѕ≈÷»јЋ№Ќџ≈ »Ћ» 
ѕќ—Ћ≈ƒќ¬ј¬Ў»≈ ”Ѕџ“ », —¬я«јЌЌџ≈ — »—ѕќЋ№«ќ¬јЌ»≈ћ »Ћ» ѕќЌ≈—≈ЌЌџ≈ ¬—Ћ≈ƒ—“¬»≈ 
Ќ≈¬ќ«ћќ∆Ќќ—“» »—ѕќЋ№«ќ¬јЌ»я ѕ–ќ√–јћћџ (¬ Ћё„јя ѕќ“≈–» ƒјЌЌџ’, »Ћ» ƒјЌЌџ≈, 
—“ј¬Ў»≈ Ќ≈√ќƒЌџћ», »Ћ» ”Ѕџ“ » »/»Ћ» ѕќ“≈–» ƒќ’ќƒќ¬, ѕќЌ≈—≈ЌЌџ≈ »«-«ј ƒ≈…—“¬»… 
“–≈“№»’ Ћ»÷ »/»Ћ» ќ“ ј«ј ѕ–ќ√–јћћџ –јЅќ“ј“№ —ќ¬ћ≈—“Ќќ — ƒ–”√»ћ» ѕ–ќ√–јћћјћ», 
Ќќ Ќ≈ ќ√–јЌ»„»¬јя—№ Ё“»ћ» —Ћ”„јяћ»), Ќќ Ќ≈ ќ√–јЌ»„»¬јя—№ »ћ», ƒј∆≈ ≈—Ћ» “ј ќ… 
¬Ћјƒ≈Ћ≈÷ »Ћ» ƒ–”√ќ≈ Ћ»÷ќ ЅџЋ» »«¬≈ў≈Ќџ ќ ¬ќ«ћќ∆Ќќ—“» “ј »’ ”Ѕџ“ ќ¬ » ѕќ“≈–№.

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

#define ANDREW_BI

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nt_types.h"
#include "struct.h"
#include "func.h"
#include "ligas.h"
#include "msgerr.h"
#include "lang.h"
#include "linutil.h"
#include "prop_nar.h"

#include "compat_defs.h"

extern BOOL line_readyBL;
extern BYTE p2_active;
extern INT  nIncline;
extern INT  page_nIncline;
extern BYTE line_scale;
extern WORD actual_resolution;
extern BYTE fax1x2;

#define WORDMAX  100
struct word_inf {cell *c; INT dens,n;} bld[WORDMAX];
INT nw;

static void italic(cell *);
static INT linerev(INT,INT *);
static LONG inclin(INT,INT *);
static void serif(cell *);
static void underline();
static BYTE keg_word(cell *,cell *,INT,B_LINES *);
static void bold_word(cell *,cell *,struct word_inf *);
static INT dens_let(cell *);
static INT pitch();

// RCM.C
extern BYTE line_tabcell;
extern BOOL pass4_in;
static BYTE bad_italic[]="03ЗОСЭ•ІЃбнeocOC"; // a g m u..."03«ќ—ЁезосэeocOC"
static BYTE nei_italic[]="02356789()%ЗОСЭ•І©Ѓбн";	// "02356789()%«ќ—Ёезйосэ"
static BYTE one_italic[]="Ґ™";	// "вк"

#define STAT_PIT
#ifdef  STAT_PIT
struct B { INT mid; // е†а†™в•а®бв®™†
           INT real;
           INT max,min;
           WORD n; // з®бЂЃ б®ђҐЃЂЃҐ
           };
typedef struct B STAT;

STAT    stat_tabgi[256];
STAT    stat_tabss[256];
STAT    stat_tabgs[256];
STAT    stat_tabcr[256];
STAT    stat_tabmd[256];
STAT *  stat_tab;

void print_stat(FILE * f,STAT * stat)
{
register i,j,k;

 for(i=1;i<16;i++){
   for(j=0;j<16;j++){
   // ѓ•з†в†вм вЃЂм™Ѓ ≠• ѓгбвл• бваЃ™®
       BYTE * byte = (BYTE *)(stat+i*16+j);
       for(k=0;k<sizeof(STAT);k++)
            if(byte[k])
                {
  fprintf(f,  "\n\n    "); for(j=0;j<16;j++)fprintf(f," %3c",i*16+j);
  fprintf(f,  "\nmid ");   for(j=0;j<16;j++)fprintf(f," %3i",stat[i*16+j].mid);
  fprintf(f,  "\nreal");   for(j=0;j<16;j++)fprintf(f," %3i",stat[i*16+j].real);
  fprintf(f,  "\nmax ");   for(j=0;j<16;j++)fprintf(f," %3i",stat[i*16+j].max);
  fprintf(f,  "\nmin ");   for(j=0;j<16;j++)fprintf(f," %3i",stat[i*16+j].min);
  fprintf(f,"  \nn   ");   for(j=0;j<16;j++)fprintf(f," %3i",stat[i*16+j].n);
                break;
                }
  }
 }
 memset(stat,0,sizeof(STAT)*256);// ђ†бб®Ґ ђЃ¶•в °лвм ®бѓЃЂмІЃҐ†≠ ѓЃІ§≠••
}
#endif

void font_let()
 {
 cell *c;

 for (c=(cell_f())->next; c->next!=NULL; c=c->next)
  if (c->flg&c_f_let && tsimple(c))
   {
   italic(c);
   serif(c);
   }

 snap_newpass('i');

 if( snap_activity('i') ){
    snap_newcell((cell_f())->next);
    snap_show_text("FOR ITALIC DEBUG");
    snap_monitor();
 }

 }

static BYTE twinsl[]={"cCpPsSvVwVxXzZ0O1l"};
static BYTE twinsr[]={"ҐВ£Г§Д¶ЖІЗ®И©Й™КЂЛђМ≠НЃОѓПаРбСвТгУдФеХжЦзЧиШйЩкЪлЫмЬнЭоЮпЯ"};
//				      "в¬г√дƒж∆з«и»й…к лЋмћнЌоќпѕр–с—т“у”ф‘х’ц÷ч„шЎщўъЏыџь№эЁюё€я

#define NVAR         256 // Valdemar & Oleg
#define NLET         256
#define f_italic     4
#define f_italic_add 6
#define f_italic_lst 20
#define PROBMIN      30000
#define DPROB        500

static BYTE solid_italic[]="рсхчшэ"; // a g m u...
static BYTE strong_italic[]="рсхчш"; // a is not strong
//static BYTE non_inclinable_letters[]="СбОЃ†";
static void italic(cell *c)
 {
 s_glue GL;
 servBOX *s;
 indBOX *h;
 elmBOX *elm;
 BYTE let;
 PBYTE          twins;
 CHAR font[2*NVAR];
 WORD i,prob[2*NVAR];
 INT nansw,maxi,maxni,l;
 extern PCHAR tableBOX;
 INT sv_pos_inc , sv_stick_inc, sv_save_stick_inc;
 BOOL   bad_cur_ge, disable_it=FALSE;

if( c->pos_inc==erect_no )
    disable_it=TRUE;

 sv_pos_inc        = c->pos_inc        ;
 sv_stick_inc      = c->stick_inc      ;
 sv_save_stick_inc = c->save_stick_inc ;

 c->save_stick_inc = c->stick_inc;
 c->stick_inc = (INT)0x8000;
 bad_cur_ge = (c->vers[0].let==r_cu_z && 
    (c->recsource==c_rs_ev || c->recsource==(c_rs_ev|c_rs_deskr)) && 
    c->vers[0].prob==254);
 if(language==LANG_RUSSIAN &&
    (memchr(solid_italic,c->vers[0].let,sizeof(solid_italic))||
    p2_active&&sv_save_stick_inc>250&&sv_save_stick_inc!=NO_INCLINE)&& 
      !bad_cur_ge )
    { 
     c->font      |= c_fp_it;
     c->stick_inc |= 0x4000;
     goto  ret;
    }
 if( c->save_stick_inc == NO_INCLINE )
   calcErection(c);

 if( c->save_stick_inc != NO_INCLINE &&
     !memchr("$&#257/",c->vers[0].let,7) && 
      !(language==LANG_RUSSIAN && c->vers[0].let==r_cu_z) ){
    if( c->save_stick_inc > 250 /*&& !disable_it */)
    {
    if( !disable_it )
        {
        c->font      |= c_fp_it;
        c->stick_inc |= 0x2000;
        goto ret;
        }
    else
        {
        c->font_new  |= c_fp_it;
        goto ret;
        }
    }
 }

 GL.celist[0]=c;
 GL.celist[1]=NULL;
 GL.complist[0]=c->env;
 GL.complist[1]=NULL;
 GL.ncell=0;
 if ((s=make_broken_raster(&GL,1))!=NULL)
  {
  let=let_sans_acc[c->vers[0].let];
  c->dens=s->dens;

  if (letincl(c) || 
//	  let>=ligas_beg && let<=ligas_end ||
	  is_liga(let) ||  // 14.09.2000 E.P.
      memchr("$&#257/",let,7) || 
      language==LANG_RUSSIAN && let==r_cu_z )
  if( !memchr("ЂЛ",let,2) )
   {
      c->stick_inc |= 0x1000;
      goto ret;
   }
  else
      c->font^=c_fp_it;

  if (let=='I' || let=='i' || let==liga_i ||
	 language == LANG_TURKISH &&  // 30.05.2002 E.P.
		(let==i_sans_accent||let==II_dot_accent)
	 )
   let='l';
  h=(indBOX *)tableBOX+let;
  for (nansw=0,i=h->numel; !nansw || i; i=elm->list)
   {
   elm=(elmBOX *)(tableBOX+NLET*sizeof(indBOX))+i;  
   if( elm->bnd & 0x0100 )
     continue;
   if( !(c->pos_inc&erect_rot) && (elm->bnd & 0x0200) )
     continue;
   font[nansw]=elm->fnt;
   prob[nansw]=(*scalarf)(elm->vect,s->matrBOX.vect,s->matrBOX.isq_rt);
   nansw++;
   }
   if(language==LANG_RUSSIAN) twins=twinsr;
        else    twins=twinsl;
  for (l=strlen(twins),i=0; i<l; i++)
   if (let==twins[i])
    {let=twins[i^1]; break;}
  if (i<l)
   {
   h=(indBOX *)tableBOX+let;
   for (i=h->numel; !nansw || i; i=elm->list)
    {
    elm=(elmBOX *)(tableBOX+NLET*sizeof(indBOX))+i;
    if( ( elm->bnd & 0x0100 ) == 1 )
      continue;
    if( !(c->pos_inc&erect_rot) && (elm->bnd & 0x0200) )
      continue;
    font[nansw]=elm->fnt;
    prob[nansw]=(*scalarf)(elm->vect,s->matrBOX.vect,s->matrBOX.isq_rt);
    nansw++;
    }
   }
  for (maxi=maxni=i=0; i<nansw; i++)
    {
    if ( language == LANG_RUSSIAN && (font[i] & c_fp_it) ||
        language != LANG_RUSSIAN && (font[i]==f_italic || font[i]>=f_italic_add && font[i]<=f_italic_lst)
      )
        {
        if (maxi<prob[i]) 
            maxi=prob[i];
        }
    else
        if (maxni<prob[i]) 
            maxni=prob[i];
    }
  if( !erection_language(language) )
      {
      if (maxi>PROBMIN && maxi-maxni>DPROB)
       c->font|=c_fp_it;
      if (maxni>PROBMIN && maxni-maxi>DPROB)
       c->font|=c_fp_str;
      }
  else if( !(p2_active              && 
             sv_save_stick_inc>240  &&
             sv_save_stick_inc!=NO_INCLINE               
             ) )
       {       
       //if( !c->nvers|| !strchr(non_inclinable_letters,c->vers[0].let) )
       c->font|=c_fp_str;
       }
  }

ret:
c->pos_inc        = (BYTE)sv_pos_inc        ;
c->stick_inc      = sv_stick_inc      ;
c->save_stick_inc = sv_save_stick_inc ;
return;
}

// “аблица информирует о возможном расположении 'завитушек' на концах букв.
// ќни могут располагатьс€ справа-0x01 и/или слева-0x02 от буквы
static CHAR tabincl[256]={
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,    // 0  - 23
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,    // 24 - 47
//                        ! " # $ % & ' ( ) * + , - . /
        0,3,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,                  // 48 - 64
//      0 1 2 3 4 5 6 7 8 9 : ; < = > ? @
        0,2,0,2,2,2,0,3,3,3,2,3,3,3,0,2,0,2,0,3,3,0,0,0,0,0,// 65 - 90
//      A B C D E F G H I J K L M N O P Q R S T U V W X Y Z
        0,0,0,0,0,0,                                        // 91 - 96
//      [ \ ] ^ _ `
        0,2,0,1,0,0,0,2,3,3,2,3,3,3,0,2,1,2,0,3,3,0,0,0,0,0,// 97 - 122
//      a b c d e f g h i j k l m n o p q r s t u v w x y z
        0,0,0,0,0,0,2,2,2,0,2,3,0,3,3,2,1,3,3,0,3,2,0,3,3,0,3,3,//123 - 150
//      { | } ~  А Б В Г Д Е Ж З И Й К Л М Н О П Р С Т У Ф Х Ц
        3,3,3,2,3,2,0,2,1,0,0,2,2,0,0,3,0,3,3,2,1,3,3,0,3,0,// 151 - 176
//      Ч Ш Щ Ъ Ы Ь Э Ю Я † ° Ґ £ § • ¶ І ® © ™ Ђ ђ ≠ Ѓ ѓ ∞
        0, 0, 0, 0,  2, 2,0,0, 0, 2,1,0,0,                  // 177 - 189
//     fi fl ff ffi rt ri % % ffl i j ! ?
//      ±  ≤  ≥  і   µ  ґ Ј Є  є  Ї ї Љ љ
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 190 - 223
//      Њ њ ј Ѕ ¬ √ ƒ ≈ ∆ « » …   Ћ ћ Ќ ќ ѕ – — “ ” ‘ ’ ÷ „ Ў ў Џ џ № Ё ё я
        2,0,3,3,3,3,3,3,3,3,2,3,2,0,2,1,                    // 224 - 239
//      а б в г д е ж з и й к л м н о п
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};                   // 240 - 255
//      р с т у ф х ц ч ш щ ъ ы ь э ю
#define KEGMAX    63
#define KEGMIN    12
#define WMAX     128
#define RINCL      1
#define LINCL      2
#define BNDINCL  200
#define BNDNINCL 150

INT letincl(cell *c)
 {
 BYTE f,let;
 INT i,mini,maxi,h,nr,nl;
 INT left[KEGMAX],right[KEGMAX];
 lnhead *line;
 interval *intv0,*intv,*intve;
 LONG s;

 let=let_sans_acc[c->vers[0].let];
 //if( language==LANG_RUSSIAN || language==LANG_ENGLISH )
 if( erection_cyr_language(language) )
    return 0;
 if (c->env==NULL || (f=tabincl[let])==0 || c->h<KEGMIN)
  return 0;
 mini=c->h/4; maxi=3*mini;
 if (let=='t')
  maxi=c->h-1;
 h=maxi-mini+1;
/* printf("let=%c,h=%u,mini=%u,maxi=%u\n",let,c->h,mini,maxi);*/
 if (f&RINCL) {memset(right,0,h+h); nr=h;}
 if (f&LINCL) {for (i=0; i<h; i++) left[i]=WMAX; nl=h;}
 for (line=(lnhead *)((PCHAR)(c->env)+c->env->lines+sizeof(INT));
			 line->lth>0; line=(lnhead *)((PCHAR)line+line->lth))
  if (line->row<=maxi && line->row+line->h>mini)
   {
   intv0=(interval *)((PCHAR)line+sizeof(lnhead));
   intv=intv0+MAX(0,mini-line->row);
   intve=intv0+MIN(maxi+1-line->row,line->h);
/*   printf("y=%u,h=%u,b=%u,e=%u\n",line->row,line->h,
	  (intv-intv0)+line->row-mini,intve-intv0-1+line->row-mini);*/
   for (i=intv-intv0+line->row-mini; intv<intve; intv++,i++)
    {
    if (f&RINCL && right[i]<intv->e-1) right[i]=intv->e-1;
    if (f&LINCL && left[i]>intv->e-intv->l) left[i]=intv->e-intv->l;
    }
   }
 s=i=0;
 if (f&RINCL && (nr=linerev(nr,right))>=KEGMIN/2)
  {s+=inclin(nr,right); i++;}
 if (f&LINCL && (nl=linerev(nl,left))>=KEGMIN/2)
  {s+=inclin(nl,left); i++;}
/* scanf("%c",&i);*/
 if (!i) return 0;
 if ((s=s/i-page_nIncline)>=((let=='/')?3:1)*BNDINCL)
  {c->font_new|=c_fp_it; return 1*0;}
 if (s<=((let=='/')?2:1)*BNDNINCL)
  {c->font_new|=c_fp_str; return 1*0;}
 return 0;
 }

static INT linerev(INT n,INT *line)
 {
 INT i,beg,end;

 if (n<KEGMIN/2)
  return n;
 for (beg=n/2; beg; beg--)
  if (abs(line[beg]-line[beg-1])>1) break;
 for (end=n/2; end<n-1; end++)
  if (abs(line[end]-line[end+1])>1) break;
 for (i=beg; i<=end; i++) line[i-beg]=line[i];
/* for (i=0; i<=end-beg; i++) printf(" %u",line[i]); printf("\n");*/
 return end-beg+1;
 }

static LONG inclin(INT n,INT *line)
 {
 INT x;
 LONG sy,sxy,inc;

 for (sxy=sy=x=0; x<n; x++)
  {sy+=line[x]; sxy+=x*line[x];}
 inc=-12288*(2*sxy-(n-1)*sy)/(n*(n*n-1));
/* printf("sy=%ld,sxy=%ld,inc=%ld\n",sy,sxy,inc);*/
 return inc;
 }

#define LSER      1
#define HSER      2

// Т†°Ѓ®ж† ®≠дЃађ®аг•в Ѓ ҐЃІђЃ¶≠Ѓђ а†бѓЃЂЃ¶•≠®® б•а®дЃҐ ≠† °г™Ґ•
// О≠® ђЃ£гв а†ѓЃЂЃ£†вмбп Ґ ≠®¶≠•©-0x01 ®/®Ђ® Ґ•ае≠•©-0x02 з†бв® °г™Ґл
static CHAR tabserif[256]={
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        //  0-23
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 24-47
//                        ! " # $ % & ' ( ) * + , - . /
        0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,                      // 48-64
//      0 1 2 3 4 5 6 7 8 9 : ; < = > ? @
        1,3,0,3,3,3,0,3,3,2,3,3,3,3,0,3,0,3,0,1,0,2,2,0,1,0,    //65-90
//      A B C D E F G H I J K L M N O P Q R S T U V W X Y Z
        0,0,0,0,0,0,                                            //91-96
//      [ \ ] ^ _ `
        0,1,0,1,0,1,0,1,1,1,1,1,1,1,0,3,3,1,0,0,0,2,2,0,2,0,    // 97-122
//      a b c d e f g h i j k l m n o p q r s t u v w x y z
        0,0,0,0,0,1,3,3,3,1,3,0,0,3,3,3,1,3,3,0,1,3,0,1,2,3,0,3,//123-150
//      { | } ~  А Б В Г Д Е Ж З И Й К Л М Н О П Р С Т У Ф Х Ц
        3,0,0,3,3,3,0,0,3,0,0,3,3,1,0,0,0,3,3,3,1,3,3,0,1,0,    //151-176
//      Ч Ш Щ Ъ Ы Ь Э Ю Я † ° Ґ £ § • ¶ І ® © ™ Ђ ђ ≠ Ѓ ѓ ∞
        1, 1, 1, 1,  1, 1,0,0, 1, 1,0,0,0,                      // 177-189
//     fi fl ff ffi rt ri % % ffl i j ! ?
//      ±  ≤  ≥  і   µ  ґ Ј Є  є  Ї ї Љ љ
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 190 - 223
//      Њ њ ј Ѕ ¬ √ ƒ ≈ ∆ « » …   Ћ ћ Ќ ќ ѕ – — “ ” ‘ ’ ÷ „ Ў ў Џ џ № Ё ё я
        3,0,3,2,3,0,3,3,0,0,3,3,3,0,0,3,                        // 224 - 239
//      а б в г д е ж з и й к л м н о п
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};                       // 240 - 255
//      р с т у ф х ц ч ш щ ъ ы ь э ю


static void serif(cell *c)
 {
 BYTE let;
 lnhead *line;
 interval *i1,*i2;
 INT H,n1,n2,h,b1,b2,e1,e2;

 H=c->h;
 let=let_sans_acc[c->vers[0].let];
 for (n1=n2=0,line=(lnhead *)((PCHAR)(c->env)+c->env->lines+sizeof(INT));
			 line->lth>0; line=(lnhead *)((PCHAR)line+line->lth))
  {
  if (tabserif[let]&LSER && line->flg&l_fend && (h=line->h)>=5 && h>=H/4 &&
      line->row+h+2>=H && !(c->font&c_fp_it && memchr("BLb",let,3)))
   {
   i1=(interval *)((PCHAR)line+sizeof(lnhead))+(h-3);
   i2=i1-(H/4-3);
   b1=MIN(MIN(i1->e-i1->l,(i1+1)->e-(i1+1)->l),(i1+2)->e-(i1+2)->l);
   e1=MAX(MAX(i1->e,(i1+1)->e),(i1+2)->e);
   b2=(e2=i2->e)-i2->l;
   if (e1>e2 && b1<b2) n1++;
   if (abs(e1-b1-e2+b2)<=1 && abs(e1-e2)<=1 && abs(b1-b2)<=1) n2++;
   }
  if (tabserif[let]&HSER && line->flg&l_fbeg && (h=line->h)>=5 && h>=H/4 &&
      line->row<=2 && !(c->font&c_fp_it && let=='q'))
   {
   i1=(interval *)((PCHAR)line+sizeof(lnhead));
   i2=i1+(H/4-1);
   b1=MIN(MIN(i1->e-i1->l,(i1+1)->e-(i1+1)->l),(i1+2)->e-(i1+2)->l);
   e1=MAX(MAX(i1->e,(i1+1)->e),(i1+2)->e);
   b2=(e2=i2->e)-i2->l;
   if (e1>e2 && b1<b2) n1++;
   if (abs(e1-b1-e2+b2)<=1 && abs(e1-e2)<=1 && abs(b1-b2)<=1) n2++;
   }
  }
 if (n1>n2) c->font|=c_fp_ser;
 if (n1<n2) c->font|=c_fp_gelv;
 }

INT pitchsize=0;
INT averwid;

#define LSTRMAX 300
#define LSTRMIN 5
#define LSTRMINF 3
#define BNDOK    70
#define BND0    100
#define BND1    120
#define BND2    210
#define NLETOK  22
#define NCUTMAX  5
#define NTHLMAX  3
#define DELTMAX  4

static INT total_pitch=0;  // =0 Nick 04.01.2002
static INT base_prevstr=0; // =0 Nick 04.01.2002
static INT kegl_prevstr=0; // =0 Nick 04.01.2002

void checkpitch()
 {
 extern INT line_number;
 extern BOOL pass4_in;
 B_LINES bl;

 get_b_lines(NULL,&bl);
 if (!line_number || bl.b3<base_prevstr || bl.b3-base_prevstr>20*bl.ps ||
     bl.b3-base_prevstr>5*bl.ps && abs(kegl_prevstr-bl.ps)>1)
  total_pitch=0;  //  =0 Nick 04.01.2002  was = -1

 pitchsize=pitch();
 }
 
static INT pitch()
 {
 cell *c;
 BYTE let;
 INT nl,nc,ng,n,n1,n2,h,w,ww,wmin,wmax,i,j,sp,bad,d,p,mg;
 LONG s,min;
 WORD center[LSTRMAX],left[LSTRMAX],right[LSTRMAX];

 for (nl=nc=ng=0,c=(cell_f())->next; c->next!=NULL; c=c->next)
  if (c->flg&(c_f_let|c_f_bad))
   {
   nl++;
   if (!(c->cg_flag&(c_cg_cutr|c_cg_cutl)))
    nc++;
   let=c->vers[0].let;
   if (
//	   (let=c->vers[0].let)>=ligas_beg && let<=ligas_end &&
	  is_liga(let) &&  // 14.09.2000 E.P.

       let!=liga_i && 
		!(language == LANG_TURKISH &&  // 30.05.2002 E.P.
			(let==i_sans_accent||let==II_dot_accent)
		 ) &&
	   let!=liga_j && let!=liga_exm && let!=liga_qm
	   )
    ng++;
   }
 mg=(3*nc<2*nl)?1:0;
 for (n=0,c=(cell_f())->next; c->next!=NULL && n<LSTRMAX; c=c->next)
  if (!(c->flg&(c_f_dust|c_f_space)) && (let=c->vers[0].let)!='[' &&

//      (let<ligas_beg || let>ligas_end ||
	  (!is_liga(let) ||  // 14.09.2000 E.P.

       let==liga_i || 
		 language == LANG_TURKISH &&  // 30.05.2002 E.P.
			(let==i_sans_accent||let==II_dot_accent) ||
	   let==liga_j || let==liga_exm || let==liga_qm) &&
      (nl<NLETOK && c->flg&c_f_punct || c->flg&(c_f_let|c_f_bad)) &&
      (nc<NCUTMAX || !(c->cg_flag&(c_cg_cutr|c_cg_cutl))))
   {
   left[n]=c->col;
   right[n]=c->col+c->w;
   center[n]=c->col+(c->w)/2;
   if (((let=c->vers[0].let)=='l' || let=='1' || let=='I') &&
       !(c->font&c_fp_it) && c->font&c_fp_gelv && 4*c->w>c->h)
    center[n]+=c->w/3;
   if ((let=='i' && 5*c->w>2*c->h || 
		(let==liga_i ||
		 language == LANG_TURKISH &&  // 30.05.2002 E.P.
			(let==i_sans_accent||let==II_dot_accent)
		)&& 4*c->w>c->h) &&
       !(c->font&c_fp_it) && c->font&c_fp_gelv)
    center[n]+=c->w/5;
   if (let=='f' && !(c->font&c_fp_it) && c->font&c_fp_gelv && 2*c->w>c->h)
    center[n]-=c->w/5;
   n++;
   }
 if (n==1)
  return 0;
 h=get_size()+((fax1x2)?2:0);
 for (averwid=nl=0,min=10000,i=1; i<n; i++)
  {
  if (left[i]-right[i-1]<h)
   {averwid+=center[i]-center[i-1]; nl++;}
  if (min>center[i]-center[i-1])
   min=center[i]-center[i-1];
  }
 if (nl>LSTRMIN || nl && 2*averwid<5*nl*h)
  averwid/=nl;
 else
  averwid=h;
 if (averwid<h/2)
  return 0;

 if( averwid <= 1 )  // Nick 01.04.2002 - избежать делени€ на 0
	 return 0;

 wmin=2*averwid/3;
 wmax=3*averwid/2;
 if (wmax<min+1)
  wmax=min+1;
 for (p=j=bad=nl=0,min=1000000,w=wmin; w<=wmax; w++)
  {
  ww=3*w/4;
  for (sp=1,n2=n1=0,s=0,i=1; i<n; i++)
   if (left[i]-right[i-1]<ww)
    {
    d=center[i]-center[i-1]-w;
    if (d>-DELTMAX && 5*d<=2*w)
     {
     if (!fax1x2 || abs(d)>1)
      s+=d*d;
     n1++;
     if (!sp && bad)
      {
      d=(center[i-1]-center[j])%w;
      d=MIN(d,w-d);
      if (!fax1x2 || abs(d)>1)
       s+=d*d;
      n1++;
      }
     j=i;
     sp=bad=0;
     }
    else
     {bad=1; n2++;}
    }
   else
    sp=1;
  if (n2+ng<=NTHLMAX && n1>=LSTRMIN || !n2 && n1>=2)
   {
   s=1000l*s/(n1-1);
   if (s<=min || w==2*p && nl<n1 && s<5*min/2)
    {min=s; p=w; nl=n1;}
   }
  }
/* printf("p=%u,d=%u\n",p,s); scanf("%c",&i);*/
 if (!nl || min>BND2*h)
  return 0;
 if (mg)
  {
  if (abs(p-total_pitch)<=1)
   return total_pitch;
  else
   return 0;
  }
 if (abs(p-total_pitch)<=1)
  return p;
 if (abs(p/2-total_pitch)<=1)
  return total_pitch;
 if (p>2*h+((fax1x2)?1:0))
  return 0;
 if (min<BNDOK*h && nl>1 || min<BND0*h && nl>=LSTRMIN && total_pitch)
  return p;
 if (nl<LSTRMIN && !fax1x2 || nl<LSTRMINF || nl<n/2 ||
     !total_pitch && 3*(n-nl-1)>nl)
  return 0;
 for (n1=0,s=0,i=1; i<n; i++)
  {
  if (left[i]-right[i-1]>=3*p/4)
   continue;
  d=center[i]-center[i-1]-p;
  if (d>=-DELTMAX && 700l*d*d<min)
   {
   if (!fax1x2 || abs(d)>1)
    s+=d*d;
   n1++;
   }
  }
 if (n1>3 && n1>=n/2 && (12*n1>=7*n && nl-n1<=3 || 8*n1>=7*nl) &&
     (1000l*s/(n1-1)<BND0*h || total_pitch && 1000l*s/(n1-1)<BND1*h))
  return p;
 return 0;
 }
///////////////////////////////////////////////////////////////////////////////////////////////////////
// AK 19.02.98
void font_str()
{
	INT ni1,ni2,ns1,ns2,nu,n,trp;
	cell *c,*c1,*c2;
	BYTE fnt,fntu,fntprev;
	BYTE keg,kegprev;
	B_LINES bl;

	
	struct word_inc
	{ 
		cell *c1,*c; 
		INT bool,n; 
		BYTE fnt;
	} w_inc[WORDMAX];
	
	INT nwi=1; // 0 is fictive as left neigbor

	snap_newpass('i');

	// ********* font calculation **************

	underline();
	get_b_lines(NULL,&bl);
	total_pitch=pitchsize;
	base_prevstr=bl.b3;
	kegl_prevstr=bl.ps;
	fntprev=0;
 
	for (c1=NULL,c=(cell_f())->next; c!=NULL; c=c->next)
	{
		if (c1==NULL)
		{
			ni1=ni2=ns1=ns2=nu=n=0; 
			c1=c;
		}
		
		if (c->flg&c_f_let)
		{

            if( erection_language(language) )
			{
				if (c->font&c_fp_it)
				{
					ni1++;
					if( memchr(strong_italic,c->vers[0].let,sizeof(strong_italic)) )
                        {
                        if( c->vers[0].prob>180 )
						    ni1 += 2;
                        else if( c->vers[0].prob>70 )
                            ni1 += 1;
                        }
				}
   
				if (c->font&c_fp_str)
				{
					ni2++;
					if( memchr(bad_italic,c->vers[0].let,sizeof(bad_italic)) &&
						!is_russian_baltic_conflict(c->vers[0].let)&&// 17.07.2001 E.P.
						!is_russian_turkish_conflict(c->vers[0].let) // 21.05.2002 E.P.
					  )
						ni2 -= 1;
				}
			}

			if (c->font&c_fp_ser)
				ns1++;
			if (c->font&c_fp_gelv)
				ns2++;
			if (c->font&c_fp_undrln)
				nu++;
			n++;
		}
  
		if (c->flg==c_f_space && c->vers[0].let!=0x1e || c->flg==c_f_fict)
		{
			if (!(c1->flg&c_f_space))
			{
				fnt=fntu=0;
				for (c2=c1; c2!=c && c2->vers[0].let=='/'; c2=c2->next) 
				{
				}

				if (c2!=c)
				{
					if (ni1>ni2)
					fnt|=c_fp_it;
					if (ni1<ni2)
					fnt|=c_fp_str;

					{
						CHAR snap[80],sg[30];

						if( ni1 == 0 && ni2 == 0 )
							strcpy(sg," NO ");
						else 
							if( ni1 > ni2 )
								strcpy(sg," ITA");
							else
								strcpy(sg," STR");
						
						sprintf(sg,"%s  it = %3d st = %3d",sg,ni1,ni2);
						
						if( snap_activity('i') )
						{
							snap_newcell(c2);
							sprintf(snap,"Word italic: %s",sg);
							snap_show_text(snap);
							snap_monitor();
						}
					}
				}
				if (ns1>ns2)
					fnt|=c_fp_ser;
				if (ns1<ns2)
					fnt|=c_fp_gelv;
				if (nu>3 || nu>0 && nu>n-3)
					fntu|=c_fp_undrln;
				if (!(fnt&(c_fp_it+c_fp_str)))
					fnt|=fntprev&(c_fp_it+c_fp_str); //???
				if (!(fnt&(c_fp_ser+c_fp_gelv)))
					fnt|=fntprev&(c_fp_ser+c_fp_gelv);//???

    
				//save prew word :
				{
					INT  bool=0;

					for (c2=c1; c2!=c; c2=c2->next)
					{
						if (c2->flg!=c_f_space)
						{
							if ( n == 1 && 
								 memchr(one_italic,c2->vers[0].let,sizeof(one_italic)) &&
								!is_russian_baltic_conflict(c2->vers[0].let)&&// 17.07.2001 E.P.
								!is_russian_turkish_conflict(c2->vers[0].let) // 21.05.2002 E.P.
							   )
								bool = 1;
					
							if( memchr(nei_italic,c2->vers[0].let,sizeof(nei_italic)) &&
								!is_russian_baltic_conflict(c2->vers[0].let)&&// 17.07.2001 E.P.
								!is_russian_turkish_conflict(c2->vers[0].let) // 21.05.2002 E.P.
							  )
								bool++;
						}
					}
					if( bool == n )
						bool = 1;
					else
						bool = 0;

					if ( nwi < WORDMAX )
					{
						w_inc[nwi].fnt = fnt & (c_fp_it+c_fp_str);
						w_inc[nwi].bool = bool;
						w_inc[nwi].c1 = c1;
						w_inc[nwi].c = c;
						w_inc[nwi].n = n;
					}

					if( nwi < WORDMAX-1 )
						nwi++;
				}
    if( (fnt&c_fp_it) && !(fnt&c_fp_str) )
        {
        for (c2=c1; c2!=c; c2=c2->next)
            {
            if( c2->flg!=c_f_space && c2->font&c_fp_str )
                c2->font-=c_fp_str;
            }
        }
    if( !(fnt&c_fp_it) && (fnt&c_fp_str) )
        {
        for (c2=c1; c2!=c; c2=c2->next)
            {
            if( c2->flg!=c_f_space && c2->font&c_fp_it )
                c2->font-=c_fp_it;
            }
        }
    
				for (c2=c1; c2!=c; c2=c2->next)
				{
					if (c2->flg!=c_f_space)
						c2->font=(c2->font/*&c_fp_undrln*/)|fnt;
				}
    
				for ( c2=c->prev; 
				      c2!=c1->prev && !(c2->flg&c_f_let); 
					  c2=c2->prev) 
				{
				}
    
				for ( ; c2!=c1->prev; c2=c2->prev)
				{
					if (c2->flg!=c_f_space)
						c2->font|=fntu;
					//AK add c/g
					if (c2->prev == cell_f() || c2->prev == NULL)
						break;
				}
				fntprev=fnt;
			}
			c1=NULL;
		}
	}

 
	for (fnt=0,c=(cell_f())->next; c->next!=NULL; c=c->next)
		if (c->flg!=c_f_space && c->font&(c_fp_it+c_fp_str))
		{
			fnt=c->font&(c_fp_it+c_fp_str); 
			break;
		}
 
	if (!fnt)
		fnt=c_fp_str;
 
	for (c1=(cell_f())->next; c1!=c; c1=c1->next)
		if (c1->flg!=c_f_space)
			c1->font|=fnt;

	for (fnt=0,c=(cell_f())->next; c->next!=NULL; c=c->next)
		if (c->flg!=c_f_space && c->font&(c_fp_ser+c_fp_gelv))
		{
			fnt=c->font&(c_fp_ser+c_fp_gelv); 
			break;
		}
 
	for (c1=(cell_f())->next; c1!=c; c1=c1->next)
		if (c1->flg!=c_f_space)
		{
			if (fnt)
				c1->font|=fnt;
			else
				c1->font|=((c1->font&c_fp_it)?c_fp_gelv:c_fp_ser);
		}

    
	//use neigbor information for italic :
    if(0)
	{
		INT  i,fl,fr,fnt;

		w_inc[0].fnt = 0;
		w_inc[0].bool = 0;
		if ( nwi > 0 && nwi < WORDMAX )
		{
			w_inc[nwi].fnt = 0;
			w_inc[nwi].bool = 0;
		}
		else 
			nwi = WORDMAX / 2;

		for(i=1;i<nwi;i++)
		{
			if( !w_inc[i].bool )
				continue;
		
			if( !w_inc[i-1].bool )
				fl = w_inc[i-1].fnt;
			else
				fl = 0;
		
			if( !w_inc[i+1].bool )
				fr = w_inc[i+1].fnt;
			else
				fr = 0;
		
			fnt = fl | fr;
			if( fnt && (fnt^(c_fp_it+c_fp_str)) )
			{
				w_inc[i].bool = 0;
				w_inc[i].fnt = (BYTE)fnt;
				{
					CHAR snap[80],sg[30];

					if( fnt & c_fp_it )
						strcpy(sg," ITA");
					else 
						if (fnt &  c_fp_str )
							strcpy(sg," STR");
						else
							strcpy(sg," NO ");
					
					sprintf(sg,"%s  ",sg);
					if( snap_activity('i') )
					{
						snap_newcell(w_inc[i].c1);
						sprintf(snap,"Word italic after neigbor: %s",sg);
						snap_show_text(snap);
						snap_monitor();
					}
				}

				for (c2=w_inc[i].c1; c2!=w_inc[i].c; c2=c2->next)
				{
					if (c2->flg!=c_f_space)
					{
						c2->font &= (~(c_fp_it+c_fp_str));
						c2->font |= fnt;
					}
				}
			}
		}
	}


	// ********* kegel calculation ************

	kegprev=0;
	for (c1=NULL,c=(cell_f())->next; c!=NULL; c=c->next)
	{
		if (c->flg&c_f_let)
			n++;
		if (c1==NULL)
		{
			n=0; 
			c1=c;
		}
		
		if (c->flg==c_f_space && c->vers[0].let!=0x1e || c->flg==c_f_fict)
		{
			if (!(c1->flg&c_f_space))
			{
				get_b_lines(c1,&bl);
				keg=keg_word(c1,c,n,&bl)<<line_scale;
				if (!keg) 
					keg=kegprev;
				for (c2=c1; c2!=c; c2=c2->next)
					if (c2->flg!=c_f_space)
						c2->keg=keg;
				kegprev=keg;
			}
			c1=NULL;
		}
	}

	for (keg=0,c=(cell_f())->next; c->next!=NULL; c=c->next)
		if (c->flg!=c_f_space && c->keg)
		{
			keg=c->keg; 
			break;
		}

	if (!keg)
	{
		get_b_lines(NULL,&bl);
		keg=(bl.ps+((fax1x2)?2:0))<<line_scale;
	}

	for (c1=(cell_f())->next; c1!=c; c1=c1->next)
		if (c1->flg!=c_f_space)
			c1->keg=keg;



	// ********* boldness calculation ***********

	for (nw=0,c1=NULL,c=(cell_f())->next; c!=NULL; c=c->next)
	{
		if (c1==NULL)
		c1=c;
		trp=(c->vers[0].let=='.' && c->prev->vers[0].let=='.' &&
		c->prev->prev!=NULL && c->prev->prev->vers[0].let=='.' &&
		c->next->vers[0].let!='.')?1:0;
		if (c->flg==c_f_space && c->vers[0].let!=0x1e || c->flg==c_f_fict || trp)
		{
			if (!(c1->flg&c_f_space))
			{
				if (trp)
					c=c->next;
				bold_word(c1,c,&bld[nw]);



				nw++;
				if (trp)
					c=c->prev;
			}
			c1=NULL;
		}
	}

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
static void underline()
 {
//	return; 
extern STRLN page_lines[];	
extern Int32 num_of_lines;

 INT nl=(INT)num_of_lines;
 
 STRLN *lines=page_lines;
 INT i,bx,by,ex,ey,x;
 B_LINES bl;
 cell *c,*cf,*cl;

 get_b_lines(NULL,&bl);
 cf=(cell_f())->next; cl=(cell_l())->prev;
 for (i=0; i<nl; i++)
  {
  if (lines[i].type&FRM_LN || !(lines[i].type&HOR_LN)) 
    continue;
  bx=lines[i].beg.col>>line_scale; 
  by=lines[i].beg.row>>line_scale;
  ex=lines[i].end.col>>line_scale; 
  ey=lines[i].end.row>>line_scale;
  by-=(INT)((LONG)nIncline*bx/2048);
  ey-=(INT)((LONG)nIncline*ex/2048);
  if (MIN(by,ey)<bl.b3 || MAX(by,ey)>bl.b3+bl.ps/2 ||
      cf->r_col-bl.ps>bx || cl->r_col+cl->w+bl.ps<ex)
    continue;
  for (c=(cell_f())->next; c->next!=NULL; c=c->next)
    {
    if ((x=c->r_col+c->w/2)>=bx && x<=ex) 
        c->font|=c_fp_undrln;
    }
  lines[i].type|=UNDRLN;
  }
  
 }

#define BNDPROB 100
/*
static CHAR tabpos[256]={
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//                        ! " # $ % & ' ( ) * + , - . /
	2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,
//      0 1 2 3 4 5 6 7 8 9 : ; < = > ? @
	2,2,2,2,2,2,2,2,2,0,2,2,2,2,2,2,0,2,2,2,2,2,2,2,2,2,
//      A B C D E F G H I J K L M N O P Q R S T U V W X Y Z
	0,0,0,0,0,0,
//      [ \ ] ^ _ `
	1,2,1,2,1,0,3,2,0,3,2,2,1,1,1,3,3,1,1,4,1,1,1,1,3,1,
//      a b c d e f g h i j k l m n o p q r s t u v w x y z
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//      { | } ~
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0, 0, 0, 0,  4, 1,0,0, 0, 2,0,0,0};
//     fi fl ff ffi rt ri % % ffl i j ! ?
  */
// Т†°Ђ®ж† ®≠дЃађ®аг•в Ѓ ҐЃІђЃ¶≠Ѓђ а†бѓЃЂЃ¶•≠®® °г™Ґл Ѓв≠Ѓб®в•Ђм≠Ѓ
//  4-е °†ІЃҐле Ђ®≠®©.
// ___________
// ___________   2                     0,4 -- ??????
// ___________ 1 2 3
// ___________     3
//
static CHAR tabpos[256]={
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,     //  0-23
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,     // 24-47
//                        ! " # $ % & ' ( ) * + , - . /
        2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,                   // 48-64
//      0 1 2 3 4 5 6 7 8 9 : ; < = > ? @
        2,2,2,2,2,2,2,2,2,0,2,2,2,2,2,2,0,2,2,2,2,2,2,2,2,2, //65-90
//      A B C D E F G H I J K L M N O P Q R S T U V W X Y Z
        0,0,0,0,0,0,                                         //91-96
//      [ \ ] ^ _ `
        1,2,1,2,1,0,3,2,0,3,2,2,1,1,1,3,3,1,1,4,1,1,1,1,3,1, // 97-122
//      a b c d e f g h i j k l m n o p q r s t u v w x y z
        0,0,0,0,0,2,2,2,2,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,//123-150                ,0,
//      { | } ~  А Б В Г Д Е Ж З И Й К Л М Н О П Р С Т У Ф Х Ц
        2,2,0,2,2,2,2,2,2,1,2,1,1,0,1,1,1,1,0,1,1,1,1,1,1,0,// 151 - 176
//      Ч Ш Щ Ъ Ы Ь Э Ю Я † ° Ґ £ § • ¶ І ® © ™ Ђ ђ ≠ Ѓ ѓ ∞
        0, 0, 0, 0,  4, 1,0,0, 0, 2,0,0,0,                  // 177-189
//     fi fl ff ffi rt ri % % ffl i j ! ?
//      ±  ≤  ≥  і   µ  ґ Ј Є  є  Ї ї Љ љ
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 190 - 223
//      Њ њ ј Ѕ ¬ √ ƒ ≈ ∆ « » …   Ћ ћ Ќ ќ ѕ – — “ ” ‘ ’ ÷ „ Ў ў Џ џ № Ё ё я
        3,1,1,3,3,1,0,1,1,0,1,1,1,1,1,1,                    // 224 - 239
//      а б в г д е ж з и й к л м н о п
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};                   // 240 - 255
//      р с т у ф х ц ч ш щ ъ ы ь э ю
// from pass3.c
static BYTE keg_word(cell *c1,cell *c2,INT n,B_LINES *bl)
 {
 INT s[4],m[4],i,ss,bm1,bm2;
 cell *c;
 version *v;

 if (!n) return 0;
 if (bl->n2>=3 && bl->n3>=3)
  return (BYTE)bl->ps;
 if( bl->n1>=3 && bl->n3>=3 && bl->n2<1 && bl->n4<1 )
  return (BYTE)bl->ps;
 if( line_readyBL )
  return (BYTE)bl->ps;
 for (ss=s[0]=s[1]=s[2]=s[3]=m[0]=m[1]=m[2]=m[3]=bm1=bm2=0,c=c1;
							 c!=c2; c=c->next)
  if (c->flg&c_f_let)
   {
   for (i=-1,v=c->vers; v->let; v++)
    {
    if (v->prob<BNDPROB)
     break;
    if (i<0)
     i=tabpos[let_sans_acc[v->let]];
    else
     if (i!=tabpos[let_sans_acc[v->let]])
      {i=-1; break;}
    }
   if (i>0)
    {
    m[i-1]++;
    s[i-1]+=c->h+((fax1x2)?2:0);
    if (bl->n1>0 && i==2 && (abs(c->row-bl->b1-c->bdiff)+
			     abs(c->row+c->h-bl->b3-c->bdiff)>3))
     bm1++;
    if (bl->n4>0 && i==3 && (abs(c->row-bl->b2-c->bdiff)+
			     abs(c->row+c->h-bl->b4-c->bdiff)>3))
     bm2++;
    }
   }
/* printf("m1=%u,s1=%u,m2=%u,fl=%u,bm1=%u,s2=%u,m3=%u,s3=%u,m4=%u,s4=%u\n",
			m[0],s[0],m[1],fg,bm1,s[1],m[2],s[2],m[3],s[3]);*/
 if ((bl->n1>0 || !m[1]) && (bl->n4>0 || !m[2]) &&
     m[1]+m[2] && m[0]+m[3]<=(m[1]+m[2])/2 &&
     bm1<=m[1]/2 && bm2<=m[2]/2)
  ss=(bl->n2>0 && bl->n3>0 || !(pitchsize && c1->font&c_fp_ser))?
						    bl->ps:(6*bl->ps+2)/5;
 else
  {
  if (m[0]==n || m[0]>=2)
   ss=(s[0]+m[0]/2)/m[0];
  else
   {
   if (bl->n1>0 && bl->n2>0 && bl->n3>0 && 3*bl->ps/2+2>=bl->b3-bl->b1)
    {
    if (m[0]+m[1]+m[2]==n || m[0]+m[1]+m[2]>=3)
     ss=(s[0]+((pitchsize)?(4*(s[1]+s[2]+2)/5):(2*(s[1]+s[2])+1)/3)+
					(m[0]+m[1]+m[2])/2)/(m[0]+m[1]+m[2]);
    else
     {
     if (m[0]+m[1]+m[2] && m[3])
      ss=(s[0]+((pitchsize)?(4*(s[1]+s[2]+s[3]+2)/5):
			(2*(s[1]+s[2])+1)/3+(4*s[3]+2)/5)+
			    (m[0]+m[1]+m[2]+m[3])/2)/(m[0]+m[1]+m[2]+m[3]);
     }
    }
   }
  }
 if ((bl->n2<0 || bl->n3<0) && bl->n1>0 && pitchsize)
  bl->ps=(6*bl->ps+2)/5;
 if (ss && abs(ss-(bl->ps+((fax1x2)?2:0)))<=1)
  ss=bl->ps+((fax1x2)?2:0);
 return (BYTE)ss;
 }

#define NOTAPPL -200
#define DIFMAX    10
#define DIFFLET   10
#define MINPROB  180
#define NMIN      10
#define DENSMAX   40

static void bold_word(cell *c1,cell *c2,struct word_inf *inf)
 {
 cell *c;
 INT sd,nd,td;

 for (sd=nd=0,c=c1; c!=c2; c=c->next)
  {
  if ( !(c->flg & c_f_let) || c->vers[0].prob<MINPROB)
   continue;
  if ((td=dens_let(c))!=NOTAPPL)
   {sd+=td; nd++;}
  }
 if (nd==1 && c1->vers[0].let=='/')
  nd=sd=0;
 if (nd)
  sd=(sd+((sd>0)?nd/2:-nd/2))/nd;
 if (sd>DENSMAX)
  sd=DENSMAX;
 if (sd<-DENSMAX)
  sd=-DENSMAX;
 inf->c=c1;
 inf->dens=sd;
 inf->n=nd;
if(0)
{
   CHAR snap[80],sg[30];
   INT  ser,gel;

   ser = gel = 0;
   for(c=c1; c!=c2; c=c->next){
      if ( !(c->flg & c_f_let) || c->vers[0].prob<MINPROB)
        continue;
      if( c->font & c_fp_ser )
        ser++;
      if( c->font & c_fp_gelv )
        gel++;
   }
   if( gel == 0 && ser == 0 )
     strcpy(sg," NO");
   else if( gel == 0 )
     strcpy(sg," SERIF");
   else if( ser == 0 )
     strcpy(sg," GELVE");
   else
     sprintf(sg," SG S = %3d G = %3d",ser,gel);
   if( snap_activity('i') ){
      snap_newcell(c1);
      sprintf(snap,"Word density: dens = %3d   ( n = %3d )  %s",sd,nd,sg);
      snap_show_text(snap);
      snap_monitor();
   }
}
 }

static CHAR aver_dens;

void avdens()
 {
 //str *ln;
 LONG /*l,*/s,n;
 //c_comp **curr,**last,*cmp;
 lnhead *line;
 interval *ic,*ie;
 //extern PBYTE box_pool,string_curr;
 //extern PBYTE comp_file,comp_curr;
 //extern WORD comp_lth;
 cell *c,*c1=cell_f(),*c2=cell_l();

 total_pitch=0;  // =0 Nick 04.01.2002 was =-1
for (c=c1,n=s=0; c!=c2; c=c->next)// Piter add init s=0
	{
	if ( !(c->flg & (c_f_let|c_f_bad)) )
		continue;
	n+=(c->env->h)*(c->env->w);
	for( line=(lnhead *)((PCHAR)(c->env)+c->env->lines+sizeof(INT));
			 line->lth>0; line=(lnhead *)((PCHAR)line+line->lth))
		{
		for (ie=(ic=(interval *)((PBYTE)line+sizeof(lnhead)))+line->h;
			  ic<ie; ic++)
			{
			s+=ic->l;
			}
		}
	}
 /*
 for (s=n=0,ln=(str *)box_pool; (PBYTE)ln!=string_curr;
					  ln=(str *)((PBYTE)ln+ln->lth))
  {
  last=(c_comp **)((PBYTE)ln+ln->dust);

  for (curr=&(ln->c_comp); curr<last; curr++) //AK 04.03.97 ? for address
   {
   cmp=*curr;
   if (cmp->size==1 && !((file_comp *)cmp)->h && !((file_comp *)cmp)->w)
    {
    l=((LONG)((file_comp *)cmp)->lth)<<4;
    if ((comp_curr-comp_file+l)>>4>comp_lth)
     error_exit(ERR_comp,2);
    read_comp(((file_comp *)cmp)->offset,comp_curr,l);
    cmp=(c_comp *)comp_curr;
    }
   if (cmp->h>=KEGMAX || cmp->w>=WMAX) continue;
   n+=(cmp->h)*(cmp->w);
   for (line=(lnhead *)((PBYTE)cmp+cmp->lines+sizeof(INT));
		  line->lth>0; line=(lnhead *)((PBYTE)line+line->lth))
    for (ie=(ic=(interval *)((PBYTE)line+sizeof(lnhead)))+line->h;
							  ic<ie; ic++)
     s+=ic->l;
   }
  }
  */
 if (!n)
  {aver_dens=16; return;}
 aver_dens=(16*s+n/2)/n+9;
 if (aver_dens>=20)
  aver_dens--;
 if (aver_dens>=23)
  aver_dens--;
 if (aver_dens<=14)
  aver_dens++;
/* printf("s=%ld,n=%ld,dens=%u",s,n,aver_dens); scanf("%c",&l);*/
 }

#define NODENS   255
#define STDKEG    22
#define MKEGDENS  65
#define MDENS      4
/*
static CHAR tabss[256]={
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,2,-7,1,0,0,0,0,0,0,0,0,-8,
//                    ! " # $  % & ' ( ) * + , - .  /
    3,0,1,0,-1,0,2,-4,3,2,0,0,-4,0,-4,-2,2,
//  0 1 2 3  4 5 6  7 8 9 : ;  < =  >  ? @
   -3,4,-4,-1,0,0,-3,2,0,-3,-1,-4,0,-2,-1,-1,-2,2,1,-4,-3,-6,-2,-3,-5,0,
//  A B  C  D E F  G H I  J  K  L M  N  O  P  Q R S  T  U  V  W  X  Y Z
    2,0,2,0,0,0,
//  [ \ ] ^ _ `
    4,-1,0,0,3,0,2,1,0,-1,-1,0,3,4,3,-1,-1,1,3,0,4,-2,0,2,-2,2,
//  a  b c d e f g h i  j  k l m n o  p  q r s t u  v w x  y z
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//  { | } ~
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    3, 3, 1, 3,  3, 4,0,0, 3, 0,0,0,-2};
// fi fl ff ffi rt ri % % ffl i j !  ?

static CHAR tabgs[256]={
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,1,-7,-1,0,0,0,0,0,0,0,0,-11,
//                    ! " # $  %  & ' ( ) * + , - .  /
    0,0,-1,-2,-2,-1,1,-6,3,1,0,0,-4,0,-4,-2,2,
//  0 1  2  3  4  5 6  7 8 9 : ;  < =  >  ? @
   -2,3,-4,0,1,-2,-1,1,0,-4,-1,-5,2,2,-3,0,-2,0,-1,-6,-3,-4,-2,-3,-6,-2,
//  A B  C D E  F  G H I  J  K  L M N  O P  Q R  S  T  U  V  W  X  Y  Z
    0,-4,0,0,0,0,
//  [  \ ] ^ _ `
    3,0,-1,0,2,-1,3,0,0,0,-1,0,1,3,1,1,0,0,2,0,1,-2,1,0,-3,2,
//  a b  c d e  f g h i j  k l m n o p q r s t u  v w x  y z
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//  { | } ~
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    3, 3, 1, 3, -2, 0,0,0, 3, 0,0,0,-2};
// fi fl ff ffi rt ri % % ffl i j !  ?

static CHAR tabgi[256]={
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,1,-7,-1,0,0,0,0,0,0,0,0,-9,
//                    ! " # $  %  & ' ( ) * + , - .  /
    1,4,-2,-2,-2,0,1,-5,3,0,0,0,-4,0,-4,-2,2,
//  0 1  2  3  4 5 6  7 8 9 : ;  < =  >  ? @
   -1,3,-6,0,0,-3,-1,0,0,-5,-1,-5,-1,-1,-2,-1,-2,1,0,-4,-5,-2,-4,-3,-5,-2,
//  A B  C D E  F  G H I  J  K  L  M  N  O  P  Q R S  T  U  V  W  X  Y  Z
    0,0,0,0,0,0,
//  [ \ ] ^ _ `
    3,-2,-2,-2,3,-5,0,-2,0,0,-2,0,1,2,2,-1,-3,-2,2,0,2,-3,2,-1,-4,0,
//  a  b  c  d e  f g  h i j  k l m n o  p  q  r s t u  v w  x  y z
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//  { | } ~
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    2, 3, 0, 2,  2, 3,0,0, 2, 0,0,0,-2};
// fi fl ff ffi rt ri % % ffl i j !  ?

static CHAR tabcr[256]={
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,1,-7,-1,0,0,0,0,0,0,0,0,-8,
//                    ! " # $  %  & ' ( ) * + , - .  /
    0,0,-1,-1,0,-1,2,-3,3,2,0,0,-4,0,-4,-2,2,
//  0 1  2  3 4  5 6  7 8 9 : ;  < =  >  ? @
    1,3,-3,1,2,-2,-2,1,0,-3,-1,-3,2,1,-1,-1,-1,0,0,-1,-3,-3,-1,-1,-3,-2,
//  A B  C D E  F  G H I  J  K  L M N  O  P  Q R S  T  U  V  W  X  Y  Z
    2,-4,2,0,0,0,
//  [  \ ] ^ _ `
    5,1,1,1,6,1,2,0,0,0,-2,0,5,2,2,2,1,0,6,0,0,-1,3,3,-2,2,
//  a b c d e f g h i j  k l m n o p q r s t u  v w x  y z
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//  { | } ~
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    3, 3, 1, 3,  3, 4,0,0, 3, 0,0,0,-2};
// fi fl ff ffi rt ri % % ffl i j !  ?

static CHAR tabmd[256]={
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,2,-7,-1,0,0,0,0,0,0,0,0,-8,
//                    ! " # $  %  & ' ( ) * + , - .  /
    2,0,-1,-2,0,-1,1,-3,3,2,0,0,-4,0,-4,-2,2,
//  0 1  2  3 4  5 6  7 8 9 : ;  < =  >  ? @
    2,6,-3,3,2,-1,1,3,0,-3,-1,-3,5,7,3,-2,-1,3,-1,-4,0,0,7,-1,-3,-2,
//  A B  C D E  F G H I  J  K  L M N O  P  Q R  S  T U V W  X  Y  Z
    2,-4,2,0,0,0,
//  [  \ ] ^ _ `
    5,1,0,3,5,-2,3,1,0,0,0,0,8,3,3,2,1,-2,1,0,3,0,6,0,-2,1,
//  a b c d e  f g h i j k l m n o p q  r s t u v w x  y z
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//  { | } ~
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    3, 3, 1, 3,  3, 4,0,0, 3, 0,0,0,-2};
// fi fl ff ffi rt ri % % ffl i j !  ?
  */

// “аблицы корректировки плотности буквы в зависимости от
// разных типов шрифтов.

//  ≈сли шрифт имеет тип 'италик'.
static CHAR tabgi[256]={
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,1,-7,-1,0,0,0,0,0,0,0,0,-9,
//                    ! " # $  %  & ' ( ) * + , - .  /
    1,4,-2,-2,-2,0,1,-5,3,0,0,0,-4,0,-4,-2,2,
//  0 1  2  3  4 5 6  7 8 9 : ;  < =  >  ? @
   -1,3,-6,0,0,-3,-1,0,0,-5,-1,-5,-1,-1,-2,-1,-2,1,0,-4,-5,-2,-4,-3,-5,-2,
//  A B  C D E  F  G H I  J  K  L  M  N  O  P  Q R S  T  U  V  W  X  Y  Z
    0,0,0,0,0,0,
//  [ \ ] ^ _ `
    3,-2,-2,-2,3,-5,0,-2,0,0,-2,0,1,2,2,-1,-3,-2,2,0,2,-3,2,-1,-4,0,
//  a  b  c  d e  f g  h i j  k l m n o  p  q  r s t u  v w  x  y z
        0,0,0,0,0,-1,2,3,-4,-1,-2,-1,-2,-1,-1,-1,-2, 0,-2,-1,-1,-1,-3,-2,-3, 1,-2,-3,//123-150                ,0,
//      { | } ~   А Б В  Г  Д  Е  Ж  З  И  Й  К  Л  М  Н  О  П  Р  С  Т  У  Ф  Х  Ц
        -1,-1,-2, 3, 1, 0,-2, 0,2,3,-1, 6,-2,2,3,2,1,2,2,1,1,2,2,2,2,0,// 151 - 176
//       Ч  Ш  Щ  Ъ  Ы  Ь  Э  Ю Я †  °  Ґ  £ § • ¶ І ® © ™ Ђ ђ ≠ Ѓ ѓ ∞
        2, 3, 0, 2,  2, 3,0,0, 2, 0,0,0,-2,
//     fi fl ff ffi rt ri % % ffl i j ! ?
//      ±  ≤  ≥  і   µ  ґ Ј Є  є  Ї ї Љ љ
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 190 - 223
//      Њ њ ј Ѕ ¬ √ ƒ ≈ ∆ « » …   Ћ ћ Ќ ќ ѕ – — “ ” ‘ ’ ÷ „ Ў ў Џ џ № Ё ё я
        -1,-2,-2,-2,-2, 1,1,1,3,1, 5,4,4,0,4,4,                    // 224 - 239
//       а  б  в  г  д  е ж з и й  к л м н о п
        0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0};                   // 240 - 255
//      р с т у ф х ц ч ш щ ъ ы ь э ю



// Д†Ђ•• ®§гв ђ†бб®Ґл •бЂ® иа®дв НЕ ИТАЛИК.
//  Ш®а®≠† °г™Ґл ≠•Ѓѓа•§•Ђ•≠†. Ша®дв б•а®д≠л©.
static CHAR tabss[256]={
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,2,-7,1,0,0,0,0,0,0,0,0,-8,
//                    ! " # $  % & ' ( ) * + , - .  /
    3,0,1,0,-1,0,2,-4,3,2,0,0,-4,0,-4,-2,2,
//  0 1 2 3  4 5 6  7 8 9 : ;  < =  >  ? @
   -3,4,-4,-1,0,0,-3,2,0,-3,-1,-4,0,-2,-1,-1,-2,2,1,-4,-3,-6,-2,-3,-5,0,
//  A B  C  D E F  G H I  J  K  L M  N  O  P  Q R S  T  U  V  W  X  Y Z
    2,0,2,0,0,0,
//  [ \ ] ^ _ `
    4,-1,0,0,3,0,2,1,0,-1,-1,0,3,4,3,-1,-1,1,3,0,4,-2,0,2,-2,2,
//  a  b c d e f g h i  j  k l m n o  p  q r s t u  v w x  y z
        0,0,0,0,0,-3,2,4,-2,-3,3,-2,0, 0, 0,-1,-4,0,0,-2,0, 0,-3,-3,-4, 0,-2,-3,//123-150                ,0,
//      { | } ~   А Б В  Г  Д Е  Ж З  И  Й  К  Л М Н  О П  Р  С  Т  У  Ф  Х Ц
        -2, 0,-2,-1, 0, 1, 0,-1,1,3,2,6,1,0,4,2,2,4,4, 3,-2,3,3,1,4,0,// 151 - 176
//       Ч  Ш  Щ  Ъ  Ы  Ь  Э  Ю Я † ° Ґ £ § • ¶ І ® ©  ™  Ђ ђ ≠ Ѓ ѓ ∞
        3, 3, 1, 3,  3, 4,0,0, 3, 0,0,0,-2,
//     fi fl ff ffi rt ri % % ffl i j ! ?
//      ±  ≤  ≥  і   µ  ґ Ј Є  є  Ї ї Љ љ
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 190 - 223
//      Њ њ ј Ѕ ¬ √ ƒ ≈ ∆ « » …   Ћ ћ Ќ ќ ѕ – — “ ” ‘ ’ ÷ „ Ў ў Џ џ № Ё ё я
         1,1,0,-4,-1,3,0,1,2,0, 0,4, 2,2,2,4,                    // 224 - 239
//       а б в  г  д е ж з и й  к л  м н о п
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};                   // 240 - 255
//      р с т у ф х ц ч ш щ ъ ы ь э ю


//  Ш®а®≠† °г™Ґл ≠•Ѓѓа•§•Ђ•≠†. Ша®дв НЕ б•а®д≠л©.
static CHAR tabgs[256]={
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,1,-7,-1,0,0,0,0,0,0,0,0,-11,
//                    ! " # $  %  & ' ( ) * + , - .  /
    0,0,-1,-2,-2,-1,1,-6,3,1,0,0,-4,0,-4,-2,2,
//  0 1  2  3  4  5 6  7 8 9 : ;  < =  >  ? @
   -2,3,-4,0,1,-2,-1,1,0,-4,-1,-5,2,2,-3,0,-2,0,-1,-6,-3,-4,-2,-3,-6,-2,
//  A B  C D E  F  G H I  J  K  L M N  O P  Q R  S  T  U  V  W  X  Y  Z
    0,-4,0,0,0,0,
//  [  \ ] ^ _ `
    3,0,-1,0,2,-1,3,0,0,0,-1,0,1,3,1,1,0,0,2,0,1,-2,1,0,-3,2,
//  a b  c d e  f g h i j  k l m n o p q r s t u  v w x  y z
        0,0,0,0,0,-2,2,4,-4,-2,0,-1,-1,2,2,-1,-2,2,0,-2,1,-1,-3,-5,-3,-1,-2,-3,//123-150                ,0,
//      { | } ~   А Б В  Г  Д Е  Ж  З И Й  К  Л М Н  О П  Р  С  Т  У  Ф  Х  Ц
        -3, 0,-3,-2, 1,0,-1,0,1,4,2,8,-1,1,4,3,3,6,4, 4, 1,5,4,2,4,0,// 151 - 176
//       Ч  Ш  Щ  Ъ  Ы Ь  Э Ю Я † ° Ґ  £ § • ¶ І ® ©  ™  Ђ ђ ≠ Ѓ ѓ ∞
        3, 3, 1, 3, -2, 0,0,0, 3, 0,0,0,-2,
//     fi fl ff ffi rt ri % % ffl i j ! ?
//      ±  ≤  ≥  і   µ  ґ Ј Є  є  Ї ї Љ љ
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 190 - 223
//      Њ њ ј Ѕ ¬ √ ƒ ≈ ∆ « » …   Ћ ћ Ќ ќ ѕ – — “ ” ‘ ’ ÷ „ Ў ў Џ џ № Ё ё я
        1, 1,-4,-3,-1,2,-0,1,3,-1,-0,5,3,2,3,4,                    // 224 - 239
//      а  б  в  г  д е  ж з и  й  к л м н о п
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};                   // 240 - 255
//      р с т у ф х   ц ч ш щ ъ ы ь э   ю



//   . Ш®а®≠† °г™Ґл °ЃЂми• з•ђ а†Іђ•а ™•£Ђп.
static CHAR tabcr[256]={
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,1,-7,-1,0,0,0,0,0,0,0,0,-8,
//                    ! " # $  %  & ' ( ) * + , - .  /
    0,0,-1,-1,0,-1,2,-3,3,2,0,0,-4,0,-4,-2,2,
//  0 1  2  3 4  5 6  7 8 9 : ;  < =  >  ? @
    1,3,-3,1,2,-2,-2,1,0,-3,-1,-3,2,1,-1,-1,-1,0,0,-1,-3,-3,-1,-1,-3,-2,
//  A B  C D E  F  G H I  J  K  L M N  O  P  Q R S  T  U  V  W  X  Y  Z
    2,-4,2,0,0,0,
//  [  \ ] ^ _ `
    5,1,1,1,6,1,2,0,0,0,-2,0,5,2,2,2,1,0,6,0,0,-1,3,3,-2,2,
//  a b c d e f g h i j  k l m n o p q r s t u  v w x  y z
        0,0,0,0,0,1,2,3,-3,-2,2,-2,-1,1,1,-1,-2,2,1,-1,1,-1,-3,-1,-3,-2,-1,-3,//123-150                ,0,
//      { | } ~  А Б В  Г  Д Е  Ж  З И Й  К  Л М Н  О П  Р  С  Т  У  Ф  Х  Ц
        -2, 0,-2,0, 0,-1,-1,0,0,5,4,7,0,4,4,4,4,5,6, 5, 0,5,4,1,6,0,// 151 - 176
//       Ч  Ш  Щ Ъ  Ы  Ь  Э Ю Я † ° Ґ £ § • ¶ І ® ©  ™  Ђ ђ ≠ Ѓ ѓ ∞
        3, 3, 1, 3,  3, 4,0,0, 3, 0,0,0,-2,
//     fi fl ff ffi rt ri % % ffl i j ! ?
//      ±  ≤  ≥  і   µ  ґ Ј Є  є  Ї ї Љ љ
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 190 - 223
//      Њ њ ј Ѕ ¬ √ ƒ ≈ ∆ « » …   Ћ ћ Ќ ќ ѕ – — “ ” ‘ ’ ÷ „ Ў ў Џ џ № Ё ё я
        3,-1,-3,-2,1,5,2,3,4,2,2,6,4,4,4,6,                    // 224 - 239
//      а  б  в  г д е ж з и й к л м н о п
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};                   // 240 - 255
//      р с т у ф х ц ч ш щ ъ ы ь э ю


//  . Ш®а®≠† °г™Ґл ђ•≠ми• ®Ђ® а†Ґ≠Ѓ а†Іђ•аг ™•£Ђп.
static CHAR tabmd[256]={
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,2,-7,-1,0,0,0,0,0,0,0,0,-8,
//                    ! " # $  %  & ' ( ) * + , - .  /
    2,0,-1,-2,0,-1,1,-3,3,2,0,0,-4,0,-4,-2,2,
//  0 1  2  3 4  5 6  7 8 9 : ;  < =  >  ? @
    2,6,-3,3,2,-1,1,3,0,-3,-1,-3,5,7,3,-2,-1,3,-1,-4,0,0,7,-1,-3,-2,
//  A B  C D E  F G H I  J  K  L M N O  P  Q R  S  T U V W  X  Y  Z
    2,-4,2,0,0,0,
//  [  \ ] ^ _ `
    5,1,0,3,5,-2,3,1,0,0,0,0,8,3,3,2,1,-2,1,0,3,0,6,0,-2,1,
//  a b c d e  f g h i j k l m n o p q  r s t u v w x  y z
        0,0,0,0,0,2,4,6,-3,0,2,0,-2,7,7,-1,-2,5,3,3,3,-2,-3,-4,-3,-1,-1,0,//123-150                ,0,
//      { | } ~  А Б В  Г Д Е Ж  З И Й  К  Л М Н О П  Р  С  Т  У  Ф  Х Ц
        0,2,0,1,4, 4, 2,3,3,5,3,8,-2,1,4,2,2,5,5,3,2,6,3,1,3,0,// 151 - 176
//      Ч Ш Щ Ъ Ы  Ь  Э Ю Я † ° Ґ  £ § • ¶ І ® © ™ Ђ ђ ≠ Ѓ ѓ ∞
        3, 3, 1, 3,  3, 4,0,0, 3, 0,0,0,-2,
//     fi fl ff ffi rt ri % % ffl i j ! ?
//      ±  ≤  ≥  і   µ  ґ Ј Є  є  Ї ї Љ љ
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 190 - 223
//      Њ њ ј Ѕ ¬ √ ƒ ≈ ∆ « » …   Ћ ћ Ќ ќ ѕ – — “ ” ‘ ’ ÷ „ Ў ў Џ џ № Ё ё я
        1,0,-4,-2,-2,1,-1,0,2,-2,-1,4,2,2,2,0,                    // 224 - 239
//      а б  в  г  д е  ж з и  й  к л м н о п
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};                   // 240 - 255
//      р с т у ф х ц ч ш щ ъ ы ь э ю

static INT dens_let(cell *c)
 {
 INT d,h;
 PCHAR tab;
 BYTE let,fnt;
 if (c->dens>100)  // 11-10-94 05:59pm Pit for debug
  return NOTAPPL;  //
 if (c->dens==NODENS)
  return NOTAPPL;
 let=let_sans_acc[c->vers[0].let];
 fnt=c->font;
 if (strchr("*#!ijltI1()[]{}",let)!=NULL ||
     let==liga_bull || 
	 let==liga_i || 
	 language == LANG_TURKISH &&  // 30.05.2002 E.P.
		(let==i_sans_accent||let==II_dot_accent) ||
	 let==liga_j || let==liga_exm ||
     let=='r' && 5*c->w<3*c->h)
  return NOTAPPL;
 if (fnt&c_fp_it)
  tab=tabgi;
 else
  {
  if ( language != LANG_RUSSIAN && pitchsize )
   {
   if (pitchsize>c->keg)
    tab=tabcr;
   else
    tab=tabmd;
   }
  else
   {
   if (fnt&c_fp_ser)
    tab=tabss;
   else
    tab=tabgs;
   }
  }
 d=aver_dens+tab[let];
 if (let=='f' && fnt&c_fp_it && c->cpos&ce_p_Ql)
  d-=3;
 if (let=='f' && tab==tabgs)
  {
  if (c->w>c->h/2)
   d-=2;
  if (c->w<c->h/3)
   d++;
  }
 if (let=='r' && 17*c->w<=11*c->h)
  d++;
 d+=(d<<line_scale)/5;
 if (c->keg<<line_scale>STDKEG)
  {
  if (c->h<=3*c->keg/2+2)
   h=c->keg>>line_scale;
  else
   h=(2*c->h+1)/3;
  d-=(d*((h<<line_scale)-STDKEG)+MKEGDENS/2)/MKEGDENS;
  }
 if (c->keg<=16) d++;
 if (c->keg<=12) d++;
 if (fax1x2) d++;

#ifdef STAT_PIT
//  С°Ѓа ба•§≠•†а®дђ•в®з•б™Ѓ£Ѓ Ѓ ѓЂЃв≠Ѓбв® б®ђҐЃЂЃҐ
//

 if(tab==tabgi) stat_tab = stat_tabgi;
 if(tab==tabss) stat_tab = stat_tabss;
 if(tab==tabgs) stat_tab = stat_tabgs;
 if(tab==tabcr) stat_tab = stat_tabcr;
 if(tab==tabmd) stat_tab = stat_tabmd;

 stat_tab[let].mid  = (stat_tab[let].mid * stat_tab[let].n  + c->dens) /
                      (stat_tab[let].n + 1);
 stat_tab[let].real = ((int)stat_tab[let].real * stat_tab[let].n + (int)c->dens - (int)d) /
                      (stat_tab[let].n + 1);
 if(stat_tab[let].min > c->dens
    || !stat_tab[let].min)            stat_tab[let].min = c->dens;
 if(stat_tab[let].max < c->dens)      stat_tab[let].max = c->dens;
 stat_tab[let].n++;

#endif

#ifdef ANDREW_BI
 return  (c->dens-tab[let]);
#else
 return MDENS*(c->dens-d);
#endif
 }

void font_narrow(void)
{
cell *  c;
BYTE    slanguage=language;
INT     prop, nall, nval, narrow, pn;

if( line_tabcell || !line_scale )
    return;
for(nval=nall=narrow=0,c=cell_f()->nextl;c!=cell_l();c=c->nextl)
    {
    if( c->nvers==0 || !(c->flg&c_f_let) )
        continue;
    language = c->language;
    nall++;    
    prop = prop_index(c->h, c->w);
    pn = prop_narrow[ (INT)c->vers[0].let ];
    if( pn )
        nval++;
    if( (c->font & c_fp_narrow) )
        narrow++;
    else if(  pn &&  pn> prop )
        narrow++;        
    }
if( narrow*10 > nall*6 || narrow*10 > nval*6 && nval*2>=nall) 
    {
    for(nall=narrow=0,c=cell_f()->nextl;c!=cell_l();c=c->nextl)
        {
        c->font |= c_fp_narrow;
        }
    }
language=slanguage;
return;
}
