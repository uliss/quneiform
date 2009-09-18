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

#ifdef _MSC_VER
#include<crtdbg.h>
#endif

#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include "cstr.h"
#include "cpage.h"
//#include "ccom.h"
#include "rcorrkegl.h"
//#include "..\resource.h"
#include "wrgb.h"
#include "lst3_win.h"
//#include "table.h"
#include "dpuma.h"
//#include "leodefs.h"
#include "cor_kegl.h"
#include "garbage.h"
#include "ligas.h"

#ifndef MAXINT32
#define  MAXINT32  0x7FFFFFFF
#endif
#ifndef MAXINT16
#define  MAXINT16  0x7FFF
#endif
#define  INCLINE_FACTOR  2048
#define  RELY_LET  220
#define  V_SIZE  10000  //макс. размер страницы по вертикали
#define  USER_FRAG       CPAGE_BLOCK_USER  //фрагмент, помеченный пользователем
#define  UNCERTAIN_FRAG  CPAGE_BLOCK_UNCERTAIN //неопределенный (хороший или плохой)

#define  MAX_KEG   127 //максимальный размер кегля в пикселах
#define  KEG_PAGE  10  //максимальное число кеглей на странице
#define  FRAG_PAGE MaxHorLines*MaxVerLines //максимальное число фрагментов на странице, включая ячейки таблиц
#define  BIG_FRAG_PAGE FRAG_PAGE //максимальное число фрагментов на странице
//#define  BIG_FRAG_PAGE 50 //максимальное число фрагментов на странице
#define  LINE_PAGE FRAG_PAGE //максимальное число строк на странице

typedef uint16_t KegStat[MAX_KEG+2];

typedef struct tagKegTabElement
{
  Int8  keg0;  //если keg0>0, то это надежно определенное значение,
  Int8  keg1;  // иначе -keg0 и -keg1 - возможные значения кегл
} KegTabElement;

typedef KegTabElement KegTab[MAX_KEG+1];

typedef struct tagPageWord  //слово на странице
{
  int32_t fragment;  //номер фрагмента
  int32_t line;     //номер строки в фрагменте
  int32_t word;     //номер слова в строке
  CSTR_rast beg,end;  //растры начала и следующий за концом
} PageWord;

typedef struct tagFontStat   //для определения серифности во фрагменте
{
  int32_t straight;            //всего прямых символов
  int32_t italic;              //всего курсивных
  int32_t it_ser;              //курсивных серифных
  int32_t it_gelv;             //курсивных несерифных
  int32_t st_ser;              //то же для прямых
  int32_t st_gelv;
} FontStat;

typedef struct tagFragStat   //для выявления мусора
{
  Handle hBlock;
  Rect32 rect;     //охватывающий прямоугольник
  int16_t nsym;      //всего символов во фрагменте
  int16_t nbig;      //больших (буквы, цифры, "?" и т.д.)
  int16_t nstick;    //палочных
  int16_t nlet;      //всего распознанных
  int16_t ndig;      // -"- цифр
  int16_t nconf;     //по эвентам или подтвержденных
  int16_t ngood;     //надежно распознанных
  int16_t nhigh;     //высокая оценка
  int16_t npunct;    //знаков препинани
  Word8 min_keg;
  Word8 max_keg;
  Word8 flag;
#define RS_GOOD 1
#define RS_BAD  2
#define RS_STRANGE 4
#define RS_ADD  128
} RecStat;

typedef struct tagLineNumber  //строка во фрагменте
{
  int32_t  n;                   //номер строки на странице
  struct tagLineNumber *next; //следующая строка фрагмента
} LineNumber;

typedef struct tagLineList       //список строк
{
  LineNumber pool[LINE_PAGE];    //куча
  LineNumber *node[FRAG_PAGE+1]; //начало списка для каждого фрагмента
} LineList;


extern Bool32      gbFax100;
extern Bool32 snap_enable;
extern Bool32 exit_enable;
extern Word8 language;

Bool32 gbGarbage = TRUE;
Handle hSnapMain = 0;
Handle hSnapEndPage = 0;
Handle hSnapGarbage = 0;
Handle hSnapGarbageLine = 0;
Handle hSnapTable = 0;

static Handle hCPAGE;
static int32_t nIncline;
static KegTab  keg_tab;
static int32_t num_keg,num_keg_opt;     //количество кеглей на странице исходное и оптимальное
static KegStat  keg_stats;            //статистика кеглей на исходной странице
static Word8 max_keg=0,min_keg=255;   //минимальный и максимальный кегли на страницы
static int32_t version;                 //версия строки
static uint32_t key=1;                  //для snap'а
static int32_t skew=0;                  //поворот страницы (или строки?)
static int32_t num_frag=0;              //число фрагментов на странице
static Bool fr_ovf;                   //на странице больше FRAG_PAGE фрагментов
static int16_t fragments[LINE_PAGE+FRAG_PAGE],*fragments_end=NULL,*cur_fragment;
/*     структура фрагментов на странице:
  n1    -  число строк в 1-ом фрагменте
  i[1]  -  номер первой строки
           . . .
  i[n1] -  номер последней строки
  n2    -  число строк в 2-ом фрагменте
           . . .
*/
static Word8 fragfont[FRAG_PAGE],*cur_font;
static RecStat recstat[FRAG_PAGE];

static PageWord cur_word;

int Snap_Console(const char *text)
{
	return LDPUMA_Console(text);
}

static Bool32 get_stats();
static void get_keg_tab();
static Bool make_correct();
static void  cor_cur_fragment();
static void  cor_fax_fragment();
static void  cor_gen_fragment();
static Bool rtf_correct();

static void  garbage_fragments();
static void draw_fragments(uint32_t color);
static void draw_fragment(Handle hBlock, uint32_t color, uint32_t key);
static void display_fragment(RecStat *rsti, uint32_t color, uint32_t key);
static Bool in_gap(int32_t top, int32_t bottom, Word8 *proj);
static Bool condition1(RecStat *rsti);
static Bool condition2(RecStat *rsti);
static Bool condition3(RecStat *rsti);
static Bool condition4(RecStat *rsti);
static Bool condition34(RecStat *rsti, int32_t ngood);

static void mark_word(uint32_t color);
static Bool set_cur_word(PageWord *pw);
static Bool next_word_inline(CSTR_rast *beg, CSTR_rast *end);
static Bool prev_word_inline(CSTR_rast *beg, CSTR_rast *end);
static PageWord  get_cur_word();
static Bool next_word();
static Bool prev_word();
static Bool cur_word_eq(PageWord *cmp);
static void set_word_keg(Word8 keg);
static Word8 get_word_keg();
static Bool contain(Rect32 *b, Rect32 *s);
static void to_real(Rect32 *rect);
static void to_real16(Rect16 *rect);

static Handle find_hBlock(int32_t fragment);
static void pull_rect(Rect32 *rect, Point32 *point);
static void cover_rect(Rect32 *main_area, Rect32 *rect);
static int32_t rect_dist(Rect32 *main, Rect32 *test);
static int32_t dist_border(Rect32 *rect);
static Bool add2list(LineNumber **frag_lines, int32_t fn, int32_t ln);

static void draw_keg(const char *str);
static void draw_rect(Rect32 *rect, uint32_t color, uint32_t key);

static void kegl_by_frag();
static void keg_frag_stats();

//uint32_t myMonitorProc(Handle wnd,Handle hwnd,uint32_t message,uint32_t wParam,uint32_t lParam);

Bool32 CorrectKegl(int32_t ver)
{
  uint32_t key=1;

  hCPAGE = CPAGE_GetHandlePage( CPAGE_GetCurrentPage());

  version=ver;
  get_stats();

  draw_keg("");
  make_correct();
  draw_keg("after correction");

  rtf_correct();

//выравнивание по фрагментам (kegf)
  kegl_by_frag();
  draw_keg("by fragments");
  rtf_correct();

  LDPUMA_DeleteRects(NULL, key);

  if (gbGarbage)
  {
    if (!fr_ovf)
      garbage_fragments();
    else
      if (snap_enable)  LDPUMA_Console("Мусор не чищен: много фраментов\n");
  }

  LDPUMA_DeleteRects(NULL, key);
  return TRUE;

}

static Bool32 get_stats()
{
  int32_t           n,  i,  j;
  CSTR_line       line;
  CSTR_rast       rst;
  CSTR_rast_attr  attr;
  UniVersions     uni;
  int32_t   frag_num[FRAG_PAGE+1];
  int32_t  frag_size[FRAG_PAGE+1]={0};
  LineList  frag_lines_pool;
#define  frag_lines  frag_lines_pool.node
  int32_t total_size=0;
  int16_t *fragj=fragments;
  FontStat font_stat[FRAG_PAGE]={0},*fsti;
  Word8 *ffi=fragfont;
  int32_t ser_fr=0,gelv_fr=0;
  RecStat *rsti;

  memset(&frag_lines_pool.node[1],0,FRAG_PAGE*sizeof(LineNumber *));
  frag_lines[0]=&frag_lines_pool.pool[0];

  memset(keg_stats,0,sizeof(keg_stats));
  max_keg=0,min_keg=255;
  num_frag=0;  fr_ovf=FALSE;

  memset(recstat,0,sizeof(recstat));
  fr_ovf = find_hBlock(-1) != NULL;

  n=CSTR_GetMaxNumber ();
  for(i=1;i<=n;i++)
  {
    line = CSTR_GetLineHandle (i, version);

	  if( exit_enable )
		  return FALSE;

//    if (!LDPUMA_SkipEx(hSnapMain,TRUE,FALSE,1))
//      LDPUMA_CSTR_Monitor(hSnapMain,line,0,myMonitorProc);
    if( line )
    {
      CSTR_attr  l_attr;
      int32_t nf;

      CSTR_GetLineAttr(line,&l_attr);
      if (l_attr.Flags & CSTR_STR_CapDrop)  continue;

      nf=l_attr.fragment;
      for (j=1; j<=num_frag && frag_num[j] != nf; j++);
      if (j>FRAG_PAGE)
      {
        fr_ovf=TRUE;
        j--;
      }
      else
        frag_num[j]=nf;
      if (!add2list(frag_lines,j,i))  break;
      frag_size[j]++;
      if (j>num_frag && num_frag<FRAG_PAGE)  num_frag++;
/*
      frag_lines[j][frag_size[j]]=i;
      if (frag_size[j]<LINE_PAGE-1)  frag_size[j]++;
*/
      j--;
      fsti=font_stat+j;
      rsti=recstat+j;

      rst = CSTR_GetFirstRaster(line);
      for(;rst;rst=CSTR_GetNext(rst))
      {
        UniAlt *alt;
        Word8 let,keg;

        CSTR_GetAttr(rst,&attr);
        CSTR_GetCollectionUni(rst,&uni);
        alt=&uni.Alt[0];   let=alt->Code[0];

      //для кегл
        attr.keg=MIN(attr.keg,MAX_KEG-1);
        if(!(attr.flg & CSTR_f_space) && attr.keg && uni.lnAltCnt)
        {
          keg=attr.kegf=attr.keg;
          max_keg=MAX(keg,max_keg);
          min_keg=MIN(keg,min_keg);
          keg_stats[keg]++;
        }
        CSTR_SetAttr(rst,&attr);

      //для серифности
        if (attr.flg & (CSTR_f_let | CSTR_f_bad) &&
            (attr.font & (CSTR_fp_ser | CSTR_fp_gelv)) != (CSTR_fp_ser | CSTR_fp_gelv))
          if (attr.font & CSTR_fp_str)
          {
            fsti->straight++;
            if (attr.font & CSTR_fp_ser)   fsti->st_ser++;
            else
            if (attr.font & CSTR_fp_gelv)  fsti->st_gelv++;
          }
          else
          if (attr.font & CSTR_fp_it)
          {
            fsti->italic++;
            if (attr.font & CSTR_fp_ser)   fsti->it_ser++;
            else
            if (attr.font & CSTR_fp_gelv)  fsti->it_gelv++;
          }

      //для мусора
        if (attr.flg & (CSTR_f_let | CSTR_f_bad | CSTR_f_punct))  rsti->nsym++;
        if (attr.flg & (CSTR_f_let | CSTR_f_bad))
        {
          rsti->nbig++;
          if (uni.lnAltCnt)
          {
            Bool stick = strchr((const char *)stick_like,let) != NULL;
            rsti->nlet++;
            if (stick)  rsti->nstick++;
            if (isdigit(let))  rsti->ndig++;
            if (alt->Prob>=RELY_LET)
            {
              if (!stick)  rsti->nhigh++;
              if (attr.RecogHistory & (CSTR_hi_evn | CSTR_hi_fon))
              {
                rsti->nconf++;
                if (!stick)  rsti->ngood++;
              }
            }
          }
        }
        if (attr.flg & CSTR_f_punct)  rsti->npunct++;
      }
      if (rsti->hBlock==NULL)
        rsti->hBlock=find_hBlock(l_attr.fragment);
    }
  }

  num_keg=max_keg;

  memset(fragfont,0,num_frag);
  for (i=1,fsti=font_stat; i<=num_frag; i++,ffi++,fsti++)
  {
    int32_t size=frag_size[i];
    total_size += size+1;
    if (total_size>LINE_PAGE+FRAG_PAGE)
    {
      size -= LINE_PAGE+FRAG_PAGE-total_size;
      total_size=LINE_PAGE+FRAG_PAGE;
    }
    if (size>=0)
    {
      LineNumber *cur_num=frag_lines[i];
      *fragj=(int16_t)size;
      fragj += size;
      while (cur_num)
      {
        *fragj-- = (int16_t)cur_num->n;
        cur_num=cur_num->next;
      }
      fragj += size+1;
    }

    //определяем серифность, где есть уверенность
    if (fsti->straight>3 && fsti->straight > fsti->italic/3)
      if (fsti->st_ser > 2*fsti->st_gelv)
      {
        *ffi=CSTR_fp_ser;
        if (fsti->st_ser>=10) ser_fr++;
      }
      else
      if (fsti->st_gelv > 2*fsti->st_ser)
      {
        *ffi=CSTR_fp_gelv;
        if (fsti->st_gelv>=10) gelv_fr++;
      }
  }
  fragments_end=fragj;

  //серифность для оставшихся фрагментов
  for (i=0,ffi=fragfont,fsti=font_stat; i<num_frag; i++,ffi++,fsti++)
    if (*ffi==0)
    {
      if ( (ser_fr==0)^(gelv_fr==0) )   //на странице один шрифт
        *ffi = (ser_fr==0) ? CSTR_fp_gelv : CSTR_fp_ser;
      else                              //нет единого шрифта
        *ffi = (fsti->it_ser+fsti->st_ser > fsti->it_gelv+fsti->st_gelv) ? CSTR_fp_ser : CSTR_fp_gelv;
    }

  get_keg_tab();
  return TRUE;
}



static void get_keg_tab()
{
  uint16_t *prev_max=NULL,*cur_min=NULL,*keg_statsi=NULL;
  int32_t prev_max_n=0,min_n=INT_MAX,prev_n=0;
//  int32_t prev_size=0,i_prev=0;
  int32_t i,j;
  Word8 rely1,rely2,extend2;
  KegTabElement *keg_tabi;
  Bool rise=TRUE;
  Word8 temp_tab[3*KEG_PAGE]; //таблица из num_keg_opt кеглей
  //структура temp_tab:
  //  0 - размер кегл
  //  1 - граница области неопределенности
  //  2 - граница области неопределенности
  //  3 - размер кегл
  //      . . .
  //  n - размер кегл

  Word8 *tab=temp_tab;

  num_keg_opt=0;
  if (num_keg==0)
    return;

  for (i=0,keg_statsi=keg_stats; i<=num_keg+1 && num_keg_opt<KEG_PAGE; i++,keg_statsi++)
  {                       //цикл включает нули по краям
    int32_t n=*keg_statsi;

    if (n>prev_n)
      rise=TRUE;
    else
    if (n<prev_n)
    {
      if (rise)
      {                  //найден локальный максимум
        int32_t tol=MIN(prev_max_n,prev_n)/3;
        uint16_t *cur_max=keg_statsi-1;
        if (num_keg_opt>0)       //не первый кегль
        {
          Bool rely_bound=FALSE;
          int32_t dk=cur_max-prev_max;
          if (min_n<tol && (!gbFax100 || dk>4) && 10*dk >= cur_max-keg_stats)         //надежная граница
          {
            uint16_t *j;
            for (j=prev_max; j<cur_min && *j>=tol; j++);
            *tab++=j-keg_stats;
            for (j=cur_max; j>cur_min && *j>=tol; j--);
            *tab++=j-keg_stats;
            *tab++=cur_max-keg_stats;
            if (2*tol > 3*(2-*(tab-2)-*(tab-3)))
            {
              rely_bound=TRUE;
              prev_max=cur_max;  prev_max_n=prev_n;
              num_keg_opt++;
            }
            else
              tab -= 3;
          }
          if (!rely_bound)
          {
            if (cur_max-prev_max>3 && 8*(cur_max-prev_max)>cur_max-keg_stats)  //большой разброс - надо разделить
            {
              *tab++=cur_min-keg_stats;
              *tab++=cur_min-keg_stats;
              *tab++=cur_max-keg_stats;
              prev_max=cur_max;  prev_max_n=prev_n;
              num_keg_opt++;
            }
            else
            if (prev_n>prev_max_n)   //тот же кегль, заменяем размер
            {
              *(tab-1)=cur_max-keg_stats;
              prev_max=cur_max;  prev_max_n=prev_n;
            }
          }
        }
        else
        {
          *tab++=cur_max-keg_stats;
          prev_max=cur_max;  prev_max_n=prev_n;
          num_keg_opt=1;
        }
      }
      min_n=n;  cur_min=keg_statsi;  rise=FALSE;
    }
    prev_n=n;
  }
  *tab++=MAX_KEG;  *tab++=MAX_KEG;

  j=1;  tab=temp_tab;
  rely1=0;
  for (i=1; i<=num_keg_opt; i++,tab += 3)
  {
    int32_t tol=(Int8)(keg_stats[*tab])/3;
    int32_t keg=0,n=0,k;
    rely2=*(tab+1)-1;  extend2=*(tab+2);  keg_tabi=keg_tab+j;
    for (k=rely1,keg_statsi=keg_stats+k; k<=rely2; k++,keg_statsi++)
      if (*keg_statsi>=tol)  { n += *keg_statsi;  keg += *keg_statsi*k; }
//    if (n>0)  keg=keg*2/n;  //размер шрифта от верха заглавной до низа опущенной
    if (n>0)  keg=(keg+n/2)/n;
    for ( ; j<rely1;    j++,keg_tabi++)  keg_tabi->keg1=-(Int8)keg;
    for ( ; j<=rely2;   j++,keg_tabi++)  keg_tabi->keg0= (Int8)keg;
    for ( ; j<=extend2; j++,keg_tabi++)  keg_tabi->keg0=-(Int8)keg;
    rely1=extend2+1;  j=rely2+1;
  }
}

static Bool make_correct()
//проставляет кегль для надежного слова (опорный) и далее влево и вправо,
// пока в вариантах есть опорный
{
  LDPUMA_DeleteRects(NULL, key);

  for (cur_fragment=fragments,cur_font=fragfont;
       cur_fragment<fragments_end;
       cur_fragment += (*cur_fragment)+1,cur_font++)
    cor_cur_fragment();

  return TRUE;
}

static void  cor_cur_fragment()
{
  if (gbFax100)  cor_fax_fragment();
  else           cor_gen_fragment();
}

static void  cor_fax_fragment()
{  //для факсов по всему фрагменту устанавливается единый кегль
  PageWord beg;
  Word8 stat[2*MAX_KEG+2]={0},max_keg=0,opt_keg=0;
  Word8 keg0;
  Int8  keg;
  int32_t i,max_n=0;

  beg.word=1;
  for (beg.line=1; beg.line<=*cur_fragment; beg.line++)
    if (set_cur_word(&beg))  break;

  do
  {
    keg0=get_word_keg();
    keg=keg_tab[keg0].keg0;
    if (keg>0)
    {
      max_keg=MAX(max_keg,keg);
      stat[keg]++;
    }
  }
  while(next_word());

  if (max_keg==0)
  {
    set_cur_word(&beg);
    do
    {
      keg0=get_word_keg();
      keg=keg_tab[keg0].keg0;
      keg=abs(keg);
      max_keg=MAX(max_keg,keg);
      stat[keg]++;
      keg=keg_tab[keg0].keg1;
      keg=abs(keg);
      max_keg=MAX(max_keg,keg);
      stat[keg]++;
    }
    while(next_word());
  }

  if (max_keg==0)
    return;

  for (i=1; i<=max_keg; i++)
    if (max_n<stat[i])
    {
      max_n=stat[i];  opt_keg=(Word8)i;
    }

  set_cur_word(&beg);
  do
  {
    set_word_keg(opt_keg);
    mark_word(RGB(0,0,0));
    if( snap_enable && !LDPUMA_SkipEx(hSnapMain,TRUE,FALSE,1))
    {
      CSTR_line line = CSTR_GetLineHandle (cur_fragment[cur_word.line], version);
      if (line)
      {
        Word8 msg[20];
        CSTR_attr  l_attr;
        CSTR_GetLineAttr(line,&l_attr);
        sprintf((char*)msg,"fragment=%d",l_attr.fragment);
        LDPUMA_Console((const char*)msg);
        LDPUMA_RasterText((char*)msg);
//        LDPUMA_CSTR_Monitor(hSnapMain,line,0,myMonitorProc);
      }
    }
  }
  while(next_word());
}

static void  cor_gen_fragment()
{
  PageWord fin,rely;
  Word8 keg0;        //кегль, который будет присвоен слову
  Int8  keg;         //текущий кегль
  Bool next=TRUE;

  fin.word=1;

  for (fin.line=1; fin.line<=*cur_fragment; fin.line++)
    if (set_cur_word(&fin))  break;

  if (fin.line > *cur_fragment)  //пустой фрагмент
    return;

  keg0=get_word_keg();
  keg=keg_tab[keg0].keg0;

  while (next)
  {
    Bool back=keg<0;
    fin=get_cur_word();

    while (keg<0 && next_word())    //до следующего надежного
      keg=keg_tab[get_word_keg()].keg0;

    if (keg<0)    //надежных нет - оставляем, как есть
      break;
/*
    if (keg<0)    //оставляем, как есть, только масштабирование
    {
      set_cur_word(&fin);
      do
      {
        keg0=2*get_word_keg();
        set_word_keg(keg0);
        mark_word(RGB(0,0,0));
      }
      while (next_word());
      break;
    }
*/
    rely=get_cur_word();
    keg0=keg;
    set_word_keg(keg0);
    mark_word(RGB(255,0,0));

    if (back)
    {
      do                      //от надежного влево
      {
        Word8  kegw;
        prev_word();
        kegw=get_word_keg();
        if (keg0==abs(keg_tab[kegw].keg0) || keg0==abs(keg_tab[kegw].keg1))
        {
          set_word_keg(keg0);
          mark_word(RGB(0,255,0));
        }
        else
          break;
      }
      while (!cur_word_eq(&fin));
      set_cur_word(&rely);
    }

    while (next=next_word())         //от надежного вправо
    {
      Word8 kegw=get_word_keg();
      keg=keg_tab[kegw].keg0;
      if (keg>0)  break;

      if (keg0==abs(keg) || keg0==abs(keg_tab[kegw].keg1))
      {
        set_word_keg(keg0);
        mark_word(RGB(0,0,255));
      }
      else
        break;

    }
    if( snap_enable && !LDPUMA_SkipEx(hSnapMain,TRUE,FALSE,1))
    {
      CSTR_line line = CSTR_GetLineHandle (cur_fragment[rely.line], version);
      if (line)
      {
        Word8 msg[20];
        CSTR_attr  l_attr;
        CSTR_GetLineAttr(line,&l_attr);
        sprintf((char*)msg,"fragment=%d",l_attr.fragment);
        LDPUMA_Console((const char*)msg);
        LDPUMA_RasterText((char*)msg);
//        LDPUMA_CSTR_Monitor(hSnapMain,line,0,myMonitorProc);
      }
    }
  }
}

static void mark_word(uint32_t color)
{
  CSTR_rast_attr  attr;
  if( snap_enable && !LDPUMA_SkipEx(hSnapMain,FALSE,TRUE,1))
  {
    CSTR_rast cur=cur_word.beg;

    if (!cur)
      return;

    for ( ; cur != cur_word.end; cur=CSTR_GetNextRaster(cur, flg_all))
    {
      Rect16 box;
      CSTR_GetAttr(cur,&attr);
      box.left=attr.r_col;  box.right=box.left+attr.w-1;
      box.top=attr.r_row;   box.bottom=box.top+attr.h-1;
      LDPUMA_DrawRect(NULL,&box,skew,color,1,key);
    }
  }
}

static Bool  set_cur_word(PageWord *pw)
{
  CSTR_line       line;
  CSTR_attr  l_attr;
  CSTR_rast    beg,end;
  int32_t i;

  cur_word.line=pw->line;  cur_word.word=pw->word;
  cur_word.beg=cur_word.end=NULL;
  if (pw->line > cur_fragment[0])
    return FALSE;

  line = CSTR_GetLineHandle (cur_fragment[pw->line], version);
  if( !line )
    return FALSE;
  CSTR_GetLineAttr(line,&l_attr);
  if (l_attr.Flags & CSTR_STR_CapDrop)
    return FALSE;

  beg=end=CSTR_GetFirstRaster(line);
  for (i=1; i<=pw->word; i++)
    if (!next_word_inline(&beg,&end))
    return FALSE;

  cur_word.beg=beg;  cur_word.end=end;
  return TRUE;
}

static Bool next_word_inline(CSTR_rast *beg, CSTR_rast *end)
{
  CSTR_rast_attr  attr;

  if (!end)
    return FALSE;

  *beg=*end;
  CSTR_GetAttr(*beg,&attr);
  while (space(attr) || fict(attr))
  {
    *beg=CSTR_GetNextRaster(*beg, flg_all) ;
    CSTR_GetAttr(*beg,&attr);
    if (fict(attr))
    {
      cur_word.beg=cur_word.end=NULL;
      return FALSE;
    }
  }

  *end=*beg;
  while (!(space(attr) || fict(attr)))
  {
    *end=CSTR_GetNextRaster(*end, flg_all) ;
    CSTR_GetAttr(*end,&attr);
  }
  return TRUE;
}

static Bool prev_word_inline(CSTR_rast *beg, CSTR_rast *end)
{
  CSTR_rast       cur;
  CSTR_rast_attr  attr;

  if (!beg)
    return FALSE;

  cur=CSTR_GetPrev(*beg);
  CSTR_GetAttr(cur,&attr);
  while (space(attr))
  {
    *end=cur;
    cur=CSTR_GetPrevRaster(cur, flg_all);
    CSTR_GetAttr(cur,&attr);
  }
  if (fict(attr))
  {
    cur_word.beg=cur_word.end=NULL;
    return FALSE;
  }

  while (!(space(attr) || fict(attr)))
  {
    *beg=cur;
    cur=CSTR_GetPrevRaster(cur, flg_all);
    CSTR_GetAttr(cur,&attr);
  }
  return TRUE;
}

static PageWord  get_cur_word()
{
  return cur_word;
}

static Bool next_word()
{
  CSTR_line       line;

  if (!cur_word.beg)
    return FALSE;

  if (next_word_inline(&cur_word.beg,&cur_word.end))
  {
    cur_word.word++;
    return TRUE;
  }

  cur_word.word=1;
  while (++cur_word.line <= *cur_fragment)
    if (line = CSTR_GetLineHandle (cur_fragment[cur_word.line], version))
    {
      CSTR_attr  l_attr;
      CSTR_GetLineAttr(line,&l_attr);
      if (!(l_attr.Flags & CSTR_STR_CapDrop))
      {
        cur_word.beg=cur_word.end = CSTR_GetFirstRaster(line);
        if (next_word_inline(&cur_word.beg,&cur_word.end))
          return TRUE;
      }
    }

  cur_word.beg=cur_word.end=NULL;
  return FALSE;
}

static Bool prev_word()
{
  CSTR_line       line;

  if (!cur_word.beg)
    return FALSE;

  if (cur_word.word>1)
  {
    prev_word_inline(&cur_word.beg,&cur_word.end);
    cur_word.word--;
    return TRUE;
  }

  while (--cur_word.line>0)
    if (line = CSTR_GetLineHandle (cur_fragment[cur_word.line], version))
    {
      CSTR_attr  l_attr;
      CSTR_GetLineAttr(line,&l_attr);
      if (!(l_attr.Flags & CSTR_STR_CapDrop))
      {
        PageWord  sav;
        cur_word.beg=cur_word.end = CSTR_GetFirstRaster(line);
        for (cur_word.word=0; next_word_inline(&cur_word.beg,&cur_word.end); cur_word.word++)
          sav=cur_word;
        if (cur_word.word>0)
        {
          cur_word=sav;  cur_word.word++;  return TRUE;
        }
      }
    }

  cur_word.beg=cur_word.end=NULL;
  return FALSE;
}

static Bool cur_word_eq(PageWord *cmp)
{
  return cur_word.line==cmp->line && cur_word.word==cmp->word;
}

static void set_word_keg(Word8 keg)
{
  CSTR_rast cur=cur_word.beg;
  CSTR_rast_attr  attr;
  Word8 font;

  if (!cur)
    return;

  for ( ; cur != cur_word.end; cur=CSTR_GetNextRaster(cur,flg_all))
  {
    CSTR_GetAttr(cur,&attr);
    attr.keg = keg;
    font=attr.font & (CSTR_fp_ser | CSTR_fp_gelv);
    if (attr.font & CSTR_fp_it || font==(CSTR_fp_ser | CSTR_fp_gelv) || font==0)
    attr.font = font = attr.font & ~(CSTR_fp_ser | CSTR_fp_gelv) | *cur_font;
    CSTR_SetAttr(cur,&attr);
  }

//тот же кегль - пробелам справа
  CSTR_GetAttr(cur,&attr);
  while (space(attr))
  {
    attr.keg = keg;
    attr.font = font;
    CSTR_SetAttr(cur,&attr);
    cur=CSTR_GetNextRaster(cur, flg_all) ;
    CSTR_GetAttr(cur,&attr);
  }

}

static Word8 get_word_keg()
{
  CSTR_rast cur=cur_word.beg;
  CSTR_rast_attr  attr;

  if (!cur)
    return 0;

  CSTR_GetAttr(cur,&attr);
  return attr.keg;
}

static Bool rtf_correct()
//масштабирование
{
  CSTR_line       line;
  int32_t           n,i,dpi,dpi2;
  CSTR_rast       rst;
  CSTR_rast_attr  attr;
  PAGEINFO     PageInfo;

//  Handle hCPAGE = CPAGE_GetHandlePage( CPAGE_GetCurrentPage());
  GetPageInfo(hCPAGE,&PageInfo);
  dpi  = (WORD)PageInfo.DPIY;    //разрешение сканера по вертикали
  dpi2=2*dpi;

  n=CSTR_GetMaxNumber ();
  for(i=1;i<=n;i++)
  {
    line = CSTR_GetLineHandle (i, version);
    if( line )
    {
		  if( exit_enable )
			  return FALSE;

      rst = CSTR_GetFirstRaster(line);
      for(;rst;rst=CSTR_GetNext(rst))
      {
        Word8 keg;
        CSTR_GetAttr(rst,&attr);
        keg = (Word8)((attr.keg*2*72+dpi/2)/dpi);  //размер шрифта от верха заглавной до низа опущенной в пунктах (1/72 дюйма)
//        keg = (Word8)((attr.keg*72+dpi/2)/dpi);
/*
        if (keg>=12)
        {
          keg= (attr.keg*2*72+dpi)/dpi2*2;  //округление до ближайшего четного
//          keg= (attr.keg*72+dpi)/dpi2*2;  //округление до ближайшего четного
        }
*/
        attr.keg=attr.kegf;
        attr.kegf=keg;
        CSTR_SetAttr(rst,&attr);
      }
    }
    if (snap_enable && !LDPUMA_SkipEx(hSnapMain,FALSE,TRUE,1))
    {
      Word8 msg[80];
      sprintf((char*)msg,"Line %d corrected\n",i);
      LDPUMA_Console((const char*)msg);
      LDPUMA_RasterText((char*)msg);
//      LDPUMA_CSTR_Monitor(hSnapMain,line,0,myMonitorProc);
    }
  }
  return TRUE;
}

static Bool32 SomnitelnaLinia(int Nb)
{
			// Nbi <= 0 - ненадежная линия
			// Nbi == 127, 128 - сомнительная линия (получена только по гистограмме)
			// Nbi 127 - eng, 128 - rus

	if( Nb <= 0 )
		return TRUE;

	if( Nb == 127 || Nb == 128 )
		return TRUE;

	return FALSE;
}


static int cur_line=INT_MAX;

void next_fragment()
{
  cur_line=INT_MAX;
}

static void garbage_fragments()
{
  RecStat *rsti;
  CSTR_line line=CSTR_GetLineHandle (1, version);
#define  HOLE32 {0x7FFFFFF, 0x7FFFFFF,0,0}
  Rect32 main_area=HOLE32,hole=HOLE32,*page_num=NULL;
  Bool main_found=FALSE;
  int32_t mainsize;
  Bool add=FALSE,ingap;
  Handle  hBlock;
  uint32_t k_cur=key+1;
  Word8 vproj[V_SIZE]={0};

  min_keg=255,max_keg=0;

  if( snap_enable && !LDPUMA_SkipEx(hSnapGarbage,FALSE,TRUE,1) )
  {
    draw_fragments(wRGB(255,0,255));
//    if (line)  LDPUMA_CSTR_Monitor(hSnapGarbage,line,0,myMonitorProc);
    LDPUMA_DeleteLines(NULL, key);
  }

//маркировка хороших фрагментов и охватывающий прямоугольник
  for (cur_fragment=fragments,rsti=recstat;
       cur_fragment<fragments_end;
       cur_fragment += (*cur_fragment)+1,rsti++)
  {
    int32_t j,hmax;
    Rect32 *rect=&rsti->rect;
    uint32_t bl_flg;

    if (!rsti->hBlock)  continue;

    bl_flg=CPAGE_GetBlockFlags(hCPAGE,rsti->hBlock);
    if (bl_flg & USER_FRAG)           //выделен вручную
      rsti->flag = RS_GOOD;
    else
    if (rsti->ngood>=G_GOOD0 ||
        (rsti->nbig==rsti->nconf || G_HIGH_DENOM*rsti->nhigh > G_HIGH_NOM*(rsti->nlet - rsti->nstick)) &&
        rsti->ngood>=G_GOOD1 &&
        (rsti->npunct<=rsti->nbig && rsti->nbig<=G_BIG || G_PUNCT*rsti->npunct<=rsti->nbig))
      rsti->flag = RS_GOOD;
    else
    if (rsti->nlet==0 || rsti->ngood==0 && rsti->npunct>rsti->nlet)
      rsti->flag = RS_BAD;
    else
      CPAGE_SetBlockFlags(hCPAGE,rsti->hBlock,bl_flg | UNCERTAIN_FRAG);

  //min и max кегли и охватывающий прямоугольник сегмента
    *rect=hole;
    rsti->min_keg=255;   rsti->max_keg=0;
    for (cur_line=1; cur_line<=*cur_fragment; cur_line++)
    {
      CSTR_line line = CSTR_GetLineHandle (cur_fragment[cur_line], version);
      if (line)
      {
        CSTR_attr  l_attr;
        CSTR_rast rst;

        CSTR_GetLineAttr(line,&l_attr);
        if (l_attr.Flags & CSTR_STR_CapDrop)  continue;

        rst = CSTR_GetFirstRaster(line);
        rst=CSTR_GetNext(rst);
        for(;rst;rst=CSTR_GetNext(rst))
        {
          UniVersions uni;
          Word8 let;
          CSTR_rast_attr  attr;

          CSTR_GetAttr(rst,&attr);
          CSTR_GetCollectionUni(rst,&uni);
          let=uni.Alt[0].Code[0];

          if(!(attr.flg & CSTR_f_space) && attr.keg && uni.lnAltCnt)
          {
            rect->top=MIN(rect->top,attr.r_row);
            rect->left=MIN(rect->left,attr.r_col);
            rect->bottom=MAX(rect->bottom,attr.r_row+attr.h);
            rect->right=MAX(rect->right,attr.r_col+attr.w);

            rsti->max_keg=MAX(attr.keg,rsti->max_keg);
            rsti->min_keg=MIN(attr.keg,rsti->min_keg);
            if (rsti->flag & RS_GOOD)
            {
              max_keg=MAX(attr.keg,max_keg);
              min_keg=MIN(attr.keg,min_keg);
            }
          }
        }
      }
    }

    if (rsti->flag & RS_GOOD)
      if (rect->top<0 || rect->top >= rect->bottom || rect->left<0 || rect->left >= rect->right)
        rsti->flag = RS_STRANGE;

    //охватывающий прямоугольник и проекци
    if (rsti->flag & RS_GOOD)
    {
      //номер страницы?
      if (rsti->ndig>0 && rsti->ndig<=4 && rsti->ndig==rsti->nbig)
        if ( (rect->bottom<main_area.top || rect->top>main_area.bottom) &&
             rect->left>=main_area.left && rect->right<=main_area.right)
          page_num=&rsti->rect;

      cover_rect(&main_area,rect);
      main_found=TRUE;
      hmax=MIN(main_area.bottom,V_SIZE-1);
      for (j=main_area.top; j<=hmax; j++)  vproj[j]=1;

      if (page_num && page_num->top != main_area.top && page_num->bottom != main_area.bottom)
        page_num=NULL;
    }

    if (snap_enable && !LDPUMA_SkipEx(hSnapGarbage,FALSE,TRUE,1) )
    {
      uint32_t color=wRGB(255,0,255);
      if (rsti->flag & RS_GOOD)  color=wRGB(0,255,0);
      else
      if (rsti->flag & RS_BAD)   color=wRGB(255,255,0);
      else
      if (rsti->flag & RS_STRANGE)
      {
        color=wRGB(255,196,0);
 				Snap_Console("Ошибки в атрибутах растра\n");
      }
      display_fragment(rsti,color,key);
    }
  }


  if (page_num && page_num->top != main_area.top && page_num->bottom != main_area.bottom)
    page_num=NULL;

/*  if( snap_enable && !LDPUMA_SkipEx(hSnapGarbage,FALSE,TRUE,1) )
    if (line)  LDPUMA_CSTR_Monitor(hSnapGarbage,line,0,myMonitorProc);*/

//добавление фрагментов
  while(1)
  {
    int32_t maxsize=0;
    int16_t *maxfrag;
    RecStat *maxrst;
    int32_t d;
    Rect32 *rect;

    add=FALSE;

  //выбираем максимальный фрагмент
    for (cur_fragment=fragments,rsti=recstat;
         cur_fragment<fragments_end;
         cur_fragment += (*cur_fragment)+1,rsti++)
      if (rsti->flag==0)
      {
        Rect32 *rect=&rsti->rect;
        int32_t size=rect->right-rect->left+rect->bottom-rect->top;
        if (size>maxsize)
        {
          maxsize=size; maxfrag=cur_fragment;   maxrst=rsti;
        }
      }

    if (maxsize==0)  break;

    cur_fragment=maxfrag;  rsti=maxrst;

    if (main_found)    //есть костяк из надежный фрагментов
    {
      rsti->flag=RS_BAD;
      if( snap_enable && !LDPUMA_SkipEx(hSnapGarbage,FALSE,TRUE,1) )
      {
        display_fragment(rsti,wRGB(255,0,0),k_cur);
      }

      rect=&rsti->rect;
      d=MIN(rect->bottom-rect->top,rect->right-rect->left);
      mainsize=MIN(main_area.right-main_area.left,main_area.bottom-main_area.top);
      ingap=in_gap(rect->top,rect->bottom,vproj);

      if (main_area.top-rect->bottom>d || rect->top-main_area.bottom>d)   //далеко вверху или внизу
      {
        int32_t d0=dist_border(rect);
        d=rect_dist(&main_area,rect);
        if (d>d0)   //очень далеко
        {
          if (condition4(rsti))  rsti->flag=RS_ADD;
        }
        else
          if (condition2(rsti))  rsti->flag=RS_ADD;
      }
      else
      if (main_area.left-rect->right>d || rect->left-main_area.right>d) //далеко слева или справа
      {
        int32_t d0=dist_border(rect);
        d=rect_dist(&main_area,rect);
        if (d>d0) //очень далеко
        {
          if (condition4(rsti))  rsti->flag=RS_ADD;
        }
        else
          if (ingap)
            if (condition2(rsti))  rsti->flag=RS_ADD;
          else
            if (condition3(rsti))  rsti->flag=RS_ADD;
      }
      else  //близко или внутри
      {
        int32_t size=MAX(rect->right-rect->left,rect->bottom-rect->top);
        if (SMALL_FRAG*size>mainsize)
        {
          if (rsti->min_keg>=min_keg && rsti->max_keg<=max_keg)  rsti->flag=RS_ADD;
          else
          if (condition1(rsti))  rsti->flag=RS_ADD;
        }
        else   //маленькие
        if (ingap)                    rsti->flag=RS_ADD;
        else   if (condition3(rsti))  rsti->flag=RS_ADD;
      }

    //номер страницы?
      if (rsti->ndig>0 && rsti->ndig<=4 && rsti->ndig==rsti->nbig && !page_num)
      {
        if ( (rect->bottom<main_area.top || rect->top>main_area.bottom) &&
             rect->left>=main_area.left && rect->right<=main_area.right &&
             rsti->min_keg >= min_keg-min_keg/PN_KEGL &&
             rsti->max_keg <= max_keg+max_keg/PN_KEGL )
        {
          rsti->flag=RS_ADD;  page_num=&rsti->rect;
        }
      }
    }
    else     //надежных фрагментов нет - добавляем все сомнительные
      rsti->flag=RS_ADD;

    if (rsti->flag & RS_ADD)
    {
      int32_t j, hmax=0;

      rsti->flag = RS_GOOD;

  // расширение прямоугольника и коррекция проекции
      cover_rect(&main_area,&rsti->rect);
      hmax=MIN(main_area.bottom,V_SIZE-1);
      for (j=main_area.top; j<=hmax; j++)  vproj[j]=1;

      if (page_num && page_num->top != main_area.top && page_num->bottom != main_area.bottom)
        page_num=NULL;

    }
    if( snap_enable && !LDPUMA_SkipEx(hSnapGarbage,FALSE,TRUE,1) )
      draw_fragment(rsti->hBlock,(rsti->flag & RS_GOOD) ? wRGB(0,0,255) : wRGB(0,255,255),key);

  }

/*  if( snap_enable && !LDPUMA_SkipEx(hSnapGarbage,FALSE,TRUE,1) )
    if (line)  LDPUMA_CSTR_Monitor(hSnapMain,line,0,myMonitorProc);*/

//удаление фрагментов

	hBlock = CPAGE_GetBlockFirst(hCPAGE,TYPE_TEXT);
	while(hBlock)
	{
		Handle hNext = CPAGE_GetBlockNext(hCPAGE,hBlock,TYPE_TEXT);
    int32_t i;
    uint32_t bl_flg=CPAGE_GetBlockFlags(hCPAGE,hBlock);
    if (!(bl_flg & USER_FRAG) || rsti->flag==RS_STRANGE)
    {
      for (i=0,rsti=recstat; i<num_frag; i++,rsti++)
        if (hBlock==rsti->hBlock)  break;

      if (i==num_frag || rsti->flag==0 || rsti->flag==RS_BAD || rsti->flag==RS_STRANGE)  //фиктивный (без строк), грязь или с ошибками
         CPAGE_DeleteBlock(hCPAGE,hBlock);
    }
		hBlock = hNext;
	}

  if( snap_enable && !LDPUMA_SkipEx(hSnapGarbage,FALSE,TRUE,1) )
  {
    LDPUMA_DeleteRects(NULL, key);
    draw_fragments(wRGB(0,0,0));
    line=CSTR_GetLineHandle (1, version);
//    if (line)  LDPUMA_CSTR_Monitor(hSnapGarbage,line,0,myMonitorProc);
    LDPUMA_DeleteRects(NULL, key);
  }

}

static Bool in_gap(int32_t top, int32_t bottom, Word8 *proj)
{
  int32_t i=(top+bottom)/2;

//  return proj[i]==0;

  for (i=top; i<=bottom; i++)
    if (proj[i] != 0)
      return FALSE;
  return TRUE;
}

static Bool condition1(RecStat *rsti)
{
  if (rsti->nhigh >= CON1_HIGH &&
      (CON1_HIGH_M_DENOM*rsti->nhigh > CON1_HIGH_M_NOM*rsti->nbig ||
       CON1_HIGH_L_DENOM*rsti->nhigh > CON1_HIGH_L_NOM*rsti->nbig && rsti->nlet==rsti->nbig) &&
      CON1_PUNCT_NOM*rsti->nlet>=CON1_PUNCT_DENOM*rsti->npunct)
    return TRUE;

  if (CON1_LET_DENOM*rsti->nlet > CON1_LET_NOM*rsti->nbig &&
      (rsti->nlet<CON1_LET && rsti->nlet+1>=rsti->npunct || rsti->nlet>CON1_PUNCT*rsti->npunct))
    return TRUE;
  return FALSE;
}

static Bool condition2(RecStat *rsti)
{
  if (rsti->min_keg>=min_keg && rsti->max_keg<=max_keg )
    return (rsti->nstick==rsti->nlet) ? condition3(rsti) : condition1(rsti);
  return FALSE;
}

static Bool condition3(RecStat *rsti)
{
  return condition34(rsti,CON3_GOOD);
}

static Bool condition4(RecStat *rsti)
{
  return condition34(rsti,CON4_GOOD);
}

static Bool condition34(RecStat *rsti, int32_t ngood)
{
  if ((rsti->ngood>=ngood ||
       rsti->nhigh>=CON3_HIGH && CON3_HIGH_DEHOM*rsti->nhigh > CON3_HIGH_NOM*rsti->nbig) &&
      rsti->nbig==rsti->nlet &&
      rsti->npunct <= rsti->nbig/CON3_PUNCT + CON3_PUNCT_MIN)
//      (rsti->npunct<=CON3_PUNCT_MIN && rsti->nbig<=CON3_BIG || CON3_PUNCT*rsti->npunct<=rsti->nbig))
    return TRUE;
  return FALSE;
}

static void draw_fragments(uint32_t color)
{
	if(hCPAGE)
  {
	  Handle hBlock = CPAGE_GetBlockFirst(hCPAGE,TYPE_TEXT);
	  while(hBlock)
	  {
      draw_fragment(hBlock,color,key);
		  hBlock = CPAGE_GetBlockNext (hCPAGE,hBlock,TYPE_TEXT);
	  }
/*    if (line)
      LDPUMA_CSTR_Monitor(hSnapMain,line,0,myMonitorProc);*/
  }
}

static void display_fragment(RecStat *rsti, uint32_t color, uint32_t key)
{
  Word8 msg[90];

  draw_fragment(rsti->hBlock,color,key);

  sprintf((char*)msg,"nsym=%d\n nbig=%d\n nlet=%d\n ndig=%d\n ngood=%d\n npunct=%d\n",
               rsti->nsym, rsti->nbig, rsti->nlet, rsti->ndig, rsti->ngood, rsti->npunct);
  LDPUMA_Console((const char*)msg);
  LDPUMA_RasterText((char*)msg);

  for (cur_line=1; cur_line<=*cur_fragment; cur_line++)
  {
    CSTR_line line = CSTR_GetLineHandle (cur_fragment[cur_line], version);
//    if (line)  LDPUMA_CSTR_Monitor(hSnapGarbage,line,0,myMonitorProc);
    if (LDPUMA_SkipEx(hSnapGarbageLine,FALSE,TRUE,1))  break;
  }
}

static void draw_fragment(Handle hBlock, uint32_t color, uint32_t key)
{
	POLY_ poly;
  uint32_t  v;

  if (!hBlock)
    return;

  v=CPAGE_GetBlockData(hCPAGE,hBlock,TYPE_TEXT, &poly, sizeof(POLY_));
	if(v==sizeof(POLY_))
	{
  //....
	  COMMON *com=&poly.com;
    Point32 p32=com->Vertex[0];
    Point16 cv,pv,v0;
    int32_t   i;
    v0.x=(int16_t)p32.x;  v0.y=(int16_t)p32.y;
    cv=v0;
    for (i=1; i<com->count; i++)
    {
      Point32 p32=com->Vertex[i];
      pv=cv;
      cv.x=(int16_t)p32.x;  cv.y=(int16_t)p32.y;
      LDPUMA_DrawLine(NULL,&pv,&cv,0,color,1,key);
    }
    LDPUMA_DrawLine(NULL,&v0,&cv,0,color,1,key);
    if( snap_enable && !LDPUMA_SkipEx(hSnapGarbage,FALSE,TRUE,1) )
    {
      Word8 msg[80];
      sprintf((char*)msg,"draw=%d handle=%x\n",com->number,CPAGE_GetHandleBlock(hCPAGE, com->number));
      LDPUMA_Console((const char*)msg);
      LDPUMA_RasterText((char*)msg);
    }
	}
	else
	{
//			  ASSERT(0);
	}
}

static Handle find_hBlock(int32_t fragment)
{
  static int32_t  number[BIG_FRAG_PAGE+1]; int32_t  *n=number;
  static Handle handle[BIG_FRAG_PAGE+1]; Handle *h=handle;

  if (fragment<0)    //инициализация
  {
    int32_t i;
	  Handle hBlock = CPAGE_GetBlockFirst(hCPAGE,TYPE_TEXT);
    for (i=0; hBlock && i<BIG_FRAG_PAGE; i++)
	  {
      *h++=hBlock;
      *n++=CPAGE_GetBlockInterNum(hCPAGE,hBlock);
		  hBlock = CPAGE_GetBlockNext (hCPAGE,hBlock,TYPE_TEXT);
	  }
    *h=0;
    return  hBlock;
  }

  while(*h && *n != fragment)  { h++; n++; }
  return  *h;
}

static Bool set_frag_ptrs(int32_t *num_frag, Handle frag_hdl[], int32_t frag_num[])
{
	Handle hBlock = CPAGE_GetBlockFirst(hCPAGE,TYPE_TEXT);
  *num_frag=0;
	while(hBlock)
	{
    if (*num_frag>=FRAG_PAGE)  return FALSE;
    frag_hdl[*num_frag]=hBlock;
    frag_num[*num_frag]=CPAGE_GetBlockInterNum(hCPAGE,hBlock);
    (*num_frag)++;
		hBlock = CPAGE_GetBlockNext (hCPAGE,hBlock,TYPE_TEXT);
	}
  return  TRUE;
}

static void pull_rect(Rect32 *rect, Point32 *point)
{
  if (point->x<rect->left)   rect->left=point->x;
  else
  if (point->x>rect->right)  rect->right=point->x;

  if (point->y<rect->top)   rect->top=point->y;
  else
  if (point->y>rect->bottom)  rect->bottom=point->y;
}

static void cover_rect(Rect32 *main_area, Rect32 *rect)
{
  main_area->top=MIN(main_area->top,rect->top);
  main_area->left=MIN(main_area->left,rect->left);
  main_area->right=MAX(main_area->right,rect->right);
  main_area->bottom=MAX(main_area->bottom,rect->bottom);
}

static int32_t rect_dist(Rect32 *main, Rect32 *test)
{
  int32_t x,y;
  int32_t x1=main->left-test->right;
  int32_t x2=test->left-main->right;
  int32_t y1=main->top-test->bottom;
  int32_t y2=test->top-main->bottom;
  if (x1>0 || x2>0 || y1>0 || y2>0)   //не пересекаютс
  {
    x=MAX(x1,x2);  y=max(y1,y2);
    if (x<0)  return y;
    if (y<0)  return x;
    return MIN(x,y);
  }

//пересекаютс
  x1=main->left-test->left;
  x2=test->right-main->right;
  y1=main->top-test->top;
  y2=test->bottom-main->bottom;
  x=MAX(x1,x2);  y=max(y1,y2);
  x=MAX(x,y);
  return (x>0) ? x : 0;
}

static Bool contain(Rect32 *big, Rect32 *little)
{
  return big->left<=little->left && big->top<=little->top &&
         big->right>=little->right && big->bottom>=little->bottom;
}

static int32_t dist_border(Rect32 *rect)
{
  int32_t rv=MAXINT32;
  PAGEINFO     PageInfo;
  GetPageInfo(hCPAGE,&PageInfo);
  rv=MIN(rect->left,rect->top);
  rv=MIN(rv,(int32_t)PageInfo.Width-rect->right);
  rv=MIN(rv,(int32_t)PageInfo.Height-rect->bottom);
  return rv;
}

static void to_real(Rect32 *rect)
{
  IDEAL_XY(rect->top, rect->left);
  IDEAL_XY(rect->bottom, rect->right);
}

static void to_real16(Rect16 *rect)
{
  IDEAL_XY(rect->top, rect->left);
  IDEAL_XY(rect->bottom, rect->right);
}

static void draw_rect(Rect32 *rect, uint32_t color, uint32_t key)
{
  Point16 v1,v2,v3,v4;
  v1.x=(int16_t)rect->left;   v1.y=(int16_t)rect->top;
  v2.x=(int16_t)rect->right;  v2.y=(int16_t)rect->top;
  v3.x=(int16_t)rect->right;  v3.y=(int16_t)rect->bottom;
  v4.x=(int16_t)rect->left;   v4.y=(int16_t)rect->bottom;
  LDPUMA_DrawLine(NULL,&v1,&v2,0,color,1,key);
  LDPUMA_DrawLine(NULL,&v2,&v3,0,color,1,key);
  LDPUMA_DrawLine(NULL,&v3,&v4,0,color,1,key);
  LDPUMA_DrawLine(NULL,&v4,&v1,0,color,1,key);
}

static Bool add2list(LineNumber **frag_lines_list, int32_t fn, int32_t ln)
{
  LineNumber *cur_num=*frag_lines_list;
  if (cur_num != (LineNumber *)frag_lines_list)  //есть место
  {
    cur_num->n=ln;                  //данные
    cur_num->next=frag_lines_list[fn];
    frag_lines_list[fn]=cur_num;         //добавляем к списку фрагмента
    (*frag_lines_list)++;                //свободное место
    return TRUE;
  }
  return FALSE;
}

static void draw_keg(const char *str)
{
  if( snap_enable &&
      ( !LDPUMA_SkipEx(hSnapMain,FALSE,TRUE,1) || !LDPUMA_SkipEx(hSnapEndPage,FALSE,TRUE,1) )
    )
  {
    CSTR_line line = CSTR_GetLineHandle (1, version);
    CSTR_rast       rst;
    CSTR_rast_attr  attr;
    Word8 msg[1024],*s=msg,*se=msg+950;
    Word8 keg_range=max_keg-min_keg+1;
    int32_t i;
    int32_t n=CSTR_GetMaxNumber();

    s += sprintf((char*)s,"%s\n",str);
//      s += sprintf(s,"Фрагменты:\n");

    LDPUMA_DeleteRects(NULL, key);
    for(i=1;i<=n;i++)
    {
      CSTR_line line = CSTR_GetLineHandle (i, version);
      if( line )
      {
        CSTR_attr  l_attr;
        CSTR_GetLineAttr(line,&l_attr);
        if (l_attr.Flags & CSTR_STR_CapDrop)  continue;

        rst = CSTR_GetFirstRaster(line);
        for(;rst;rst=CSTR_GetNext(rst))
        {
          CSTR_GetAttr(rst,&attr);
          if (attr.keg)
          {
            Rect16 box;
            int32_t keg = attr.keg;
            Word8 green;
//             if (j==1)  keg /= 2;
            keg=MIN(keg,max_keg);
            green=(Word8)(255*(max_keg-keg)/keg_range);

            box.left=attr.r_col;  box.right=box.left+attr.w-1;
            box.top=attr.r_row;   box.bottom=box.top+attr.h-1;
            LDPUMA_DrawRect(NULL,&box,skew,wRGB(255-green,green,0),1,key);
          }
        }
/*
        if (s<se)
        {
          CSTR_attr  l_attr;
          CSTR_GetLineAttr(line,&l_attr);
          if ((i-1)%10==0)  s += sprintf(s,"\n");
          s += sprintf(s," %d",l_attr.fragment);
        }
*/
      }
    }
//      s += sprintf(s,"\n");
    for (i=1; i<=num_keg && keg_stats[i]==0; i++);
    for (   ; i<=num_keg && s<se; i++)
      if (keg_stats[i])
        s += sprintf((char*)s,"keg=%d num=%d\n",i,keg_stats[i]);
    LDPUMA_Console((const char*)msg);
    LDPUMA_RasterText((char*)msg);
//    LDPUMA_CSTR_Monitor(hSnapMain,line,0,myMonitorProc);
  }
}

void kegl_snap_init()
{
  LDPUMA_Registry(&hSnapMain,"Коррекция кегля...",NULL);
  LDPUMA_RegistryHelp(hSnapMain,"Коррекция кегля...",FALSE);
  LDPUMA_Registry(&hSnapEndPage,"Конец страницы",hSnapMain);
  LDPUMA_RegistryHelp(hSnapEndPage,"Конец страницы",FALSE);
//  LDPUMA_Registry(&hSnapTable,"Единый кегль в таблице",hSnapMain);
  LDPUMA_Registry(&hSnapGarbage,"Удаление мусора",NULL);
  LDPUMA_Registry(&hSnapGarbageLine,"Просмотр строк",hSnapGarbage);
  LDPUMA_RegistryHelp(hSnapGarbage,"Удаление мусора",FALSE);
  LDPUMA_RegistryHelp(hSnapGarbageLine,"Остановка на каждой строке фрагмента",FALSE);
  LDPUMA_RegVariable(hSnapGarbage,"Чистить мусор",&gbGarbage,"long");
  LDPUMA_RegVariable(hSnapMain,"Факс",&gbFax100,"long");
}

static void kegl_by_frag()
{
  for (cur_fragment=fragments,cur_font=fragfont;
       cur_fragment<fragments_end;
       cur_fragment += (*cur_fragment)+1,cur_font++)
  {
    keg_frag_stats();
    cor_cur_fragment();
  }
}

static void keg_frag_stats()
{
  int32_t nl,sv_num_keg=num_keg;
  Word8 max_keg=0;
  memset(keg_stats,0,sizeof(keg_stats));
  for (nl=1; nl<=*cur_fragment; nl++)
  {
    CSTR_line line = CSTR_GetLineHandle (cur_fragment[nl], version);
    if( line )
    {
      CSTR_attr  l_attr;
      CSTR_rast rst;

      CSTR_GetLineAttr(line,&l_attr);
      if (l_attr.Flags & CSTR_STR_CapDrop)  continue;

      rst = CSTR_GetFirstRaster(line);
      for(;rst;rst=CSTR_GetNext(rst))
      {
        CSTR_rast_attr  attr;
        UniVersions     uni;
        CSTR_GetAttr(rst,&attr);
        CSTR_GetCollectionUni(rst,&uni);
        if(!(attr.flg & CSTR_f_space) && attr.keg && uni.lnAltCnt)
        {
          Word8 keg=attr.keg;
          max_keg=MAX(keg,max_keg);
          keg_stats[keg]++;
        }
      }
    }
  }
  num_keg=max_keg;
  get_keg_tab();
  num_keg=sv_num_keg;
}

/*
    // delete all
    	Handle hBlock = CPAGE_GetBlockFirst(hCPAGE,TYPE_TEXT);
	    while(hBlock)
	    {
		    Handle hNext = CPAGE_GetBlockNext(hCPAGE,hBlock,TYPE_TEXT);
		    CPAGE_DeleteBlock(hCPAGE,hBlock);
		    hBlock = hNext;
	    }
*/

