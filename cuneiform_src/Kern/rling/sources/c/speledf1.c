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


  #include "spelmode.h"

  #if defined(TURBO_C)
    #include "tc_types.h"
  #elif defined(WATCOM)
    #include "spelwatc.h"
  #elif defined(THINK_C)
    #include "spelthin.h"
  #else
    #error   NO TOOOL SPECIFIED
  #endif

  #include <ctype.h>

  #include "speldefs.h"
  #include "spelfunc.h"

  #include "edtable.h"

  /////////////////////////
  // AK change  27.01.99
  #include "tigeremulate.h"
  void	ErrorExit(int Code);
  //end AK

  //AK  kill all stupid macros!!!!!!!!
  //#define t Q.t
  //#define  st Q.st
  //#define N Q.N
/**********************************************************************/
  extern dQ SPQ;

/**********************************************************************/
/******** L o k a l    f u n c t i o n s   p r o t o t y p e s ********/
/**********************************************************************/
 static int16_t mvbuf(uint16_t l);
 static int16_t set_descr(char c);
 static int16_t do_fragm(void);
 static void move_from_near_to_huge(char  *a, char *b, int16_t l);
 static int16_t create_segm(struct segm *prev_adr,uint16_t segm_size,uint16_t type);
 static int16_t n_l(void);
 static int16_t p_l(void);
 static int16_t prev_letter(void);

/***********************************************************************/
/*
    This subroutine is in charge with
                a/ getting file for disk
                b/ engendering all the necessaries structures
                c/ setting up the pointers
*/
int16_t begin(uchar batch_run)
{
	/* int16_t  *p;*/
	uint16_t i;
	uint16_t i1;
	/*  uint32_t l;*/

	SPQ.real_num        = 0;
	SPQ.sw_symb         = 0;
	SPQ.new_s           = 0 ;
	SPQ.beg_sheet       = 0; //sign new_sheet passed and there were no fragm
	SPQ.stack           = (struct state_stack_elem *)( SPQ.end_alloc_mem - sizeof(struct state_stack_elem));
	SPQ.tab_ptr         = (struct tab *)SPQ.free_alloc_mem;
	SPQ.free_alloc_mem  += sizeof(struct tab);

	//AK!!!!   wich stupid not attented bounds of array!!!!!!
	for (i=0; i < MAX_SHEET; i++)
		for (i1=0; i1 < MAX_FRAGM; i1++)
			SPQ.tab_ptr->tab_fragm[i][i1].first_segm = NULL;


	SPQ.st.cur_segm                = (struct segm *)(SPQ.free_alloc_mem);
	SPQ.st.cur_segm->busy_lth      = 0;
	SPQ.st.cur_segm->string_lth    = 0;
	SPQ.free_alloc_mem             += sizeof(struct segm);
	SPQ.str_ptr                    = SPQ.st.cur_segm->string;
	SPQ.st.cur_segm->next_in_sheet = NULL;
	SPQ.st.cur_segm->prev_in_sheet = NULL;
	SPQ.st.cur_segm->prev_in_fragm = NULL;
	SPQ.st.cur_segm->next_in_fragm = NULL;
	SPQ.end_l                      = 0;
	SPQ.last_del_line              = NULL;
	SPQ.stack_change_flag          = YES;

	memset(SPQ.fragm_flag,NONEXIST,sizeof(SPQ.fragm_flag));

	if(  read_file(batch_run)== FALSE)
		return FALSE;

	SPQ.st.cur_segm=(struct segm *)SPQ.tab_ptr->tab_fragm[0][0].first_segm;

	skip_letter_in_line(SPQ.st.cur_segm,0);

	SPQ.st.cur_symb=SPQ.ns_symb;

	create_new_stack();

	SPQ.stack->map_ref.row=0;
	SPQ.stack->map_ref.col=0;

	return TRUE;
 }
/*********************************************************************/

define_lth(uchar  *symb)
/*
    This procedure calculates length of letter.
*/
 {
  LT  *let;
  int16_t n;

  let=(LT  *)symb;
  n=0;
  while (*symb >= 0x20)
   {
    n++;
    if (let->attr & 1)
      break;
    else
      let++;
   }
  n*=sizeof(LT);
  return(n);
 }
/***********************************************************************/
 struct segm  *next_line(char tt,struct segm  *cur_segm)
/*
    This procedure gives address of next line.
*/
 {
  if (tt == FRAGM)
   {                                   /* fragm */
    if (cur_segm->next_in_fragm == NULL)
      return(NULL);
    cur_segm=cur_segm->next_in_fragm;
    while (cur_segm->type != NEW_LINE)
      if (cur_segm->next_in_fragm == NULL)
        return(NULL);
    else
     cur_segm=cur_segm->next_in_fragm;
   }
  else
   {                                   /* sheet */
    if (cur_segm->next_in_sheet == NULL)
      return(NULL);
     cur_segm=cur_segm->next_in_sheet;
    while (cur_segm->type != NEW_LINE)
      if (cur_segm->next_in_sheet == NULL)
        return(NULL);
      else
        cur_segm=cur_segm->next_in_sheet;
   }
  return(cur_segm);
 }
/**********************************************************************/
int16_t processEdSymb(void)
{
	int16_t ret = YES;
	int16_t n;

	switch (*SPQ.buff_ptr)
	{
		case SS_BITMAP_REF: ret=mvbuf(sizeof(struct bit_map_ref));
							break;
		case SS_TEXT_REF  : ret=mvbuf(sizeof(struct text_ref));
							break;
		case SS_LANGUAGE  : ret=mvbuf(sizeof(struct EdTagLanguage));
							break;
		case SS_FONT_KEGL : ret=mvbuf(sizeof(struct font_kegl));
							break;
		case SS_KEGL      : ret=mvbuf(sizeof(struct kegl));
							break;
		case SS_SHIFT     : ret=mvbuf(sizeof(struct shift_struct));
							break;
		case SS_RETR_LEVEL : ret=mvbuf(sizeof(struct retrieve_level));
							break;
		case SS_UNDERLINE : ret=mvbuf(sizeof(struct underline));
							break;
		case SS_DENS_PRINT: ret=mvbuf(sizeof(struct dens_print));
							break;
		case SS_TABUL     : ret=mvbuf(sizeof(struct tabul));
							break;
		case SS_TABL_TABUL: if (SPQ.buff_ptr-(uchar*)SPQ.text_buff >= SPQ.buff_l-1)
								ret=NO;
							else
							 {
								n=*(SPQ.buff_ptr+1); /* length of tabl */
								ret=mvbuf((uint16_t)(n+1));
							 }
							break;
		case SS_SHEET_DESCR:
			ret=set_descr(SHEET);
			break;
		case SS_FRAGMENT  : ret=do_fragm();
							break;
		case  SS_STEP_BACK: ret=mvbuf(sizeof(struct step_back));
							break;
		case SS_LINE_BEG  : ret=set_descr(LINE);
							break;
		case  SS_POSITION : ret=mvbuf(sizeof(struct step_back));
							break;
		case SS_TABL_CONFORM_SIZES: ret=mvbuf(sizeof(struct  tabl_conform_sizes));
							break;
		case SS_GROUP_WORDS: ret=mvbuf(sizeof(struct group_words));
							break;
		case  SS_GROUP_SYMBOLS: ret=mvbuf(sizeof(struct group_symbols));
							break;
		case SS_AKSANT    : ret=mvbuf(sizeof(struct aksant));
							break;
		case SS_NEG_HALF_SPACE: ret=mvbuf(sizeof(struct neg_half_space));
							break;
		case SS_POS_HALF_SPACE: ret=mvbuf(sizeof(struct pos_half_space));
							break;
		default           :
				if(*SPQ.buff_ptr<0x20)
				  ret=mvbuf((uint16_t)(ED_LREC(SPQ.buff_ptr)));
				else
				  ret=mvbuf((uint16_t)(sizeof(LT)));/* letter */
							break;
       }                             /* end of switch */

	if ( ret == -1 )
	{
		ret = 0;
	}

	return ret;
 }
/************************************************************************/
void puff_last_segm(void)
{
 SPQ.st.cur_segm->string_lth+=DELTA;
 SPQ.free_alloc_mem+=DELTA;
 check_free_mem();
}
/************************************************************************/
int16_t mvbuf(uint16_t l)
/*
     This subroutine is in charge with setting down symbols
     into string of concerned segm.
*/
 {
  if( l==0 )  return -1; // Oleg : need stop if unknown tag in ED
  if (SPQ.buff_ptr-(uchar*)SPQ.text_buff+l > SPQ.buff_l)
    return(NO);
  SPQ.l+=l;
  move_from_near_to_huge(SPQ.str_ptr, SPQ.buff_ptr,l);
  SPQ.buff_ptr+=l-1;         /* after break adds 1 to SPQ.buff_ptr */
  SPQ.str_ptr+=l;
  SPQ.free_alloc_mem= SPQ.str_ptr+1;
  SPQ.st.cur_segm->string_lth+=l;
  SPQ.st.cur_segm->busy_lth+=l;
  return(YES);
 }
/*************************************************************************/
void fill_fragm(int16_t n_fragm)
/*
     This procedure fills out fragment description table.
*/
 {
  struct fragm_descr  *df;

  df=&(SPQ.tab_ptr->tab_fragm[SPQ.cur_sheet][n_fragm]);
  SPQ.fragm_disk_descr_ptr=(struct fragm_disk_descr *)(SPQ.buff_ptr+24+(n_fragm)*
  sizeof(struct fragm_disk_descr));
  df->el_type=FRAGM_EL_TYPE;
  df->row=SPQ.fragm_disk_descr_ptr->row;
  df->col=SPQ.fragm_disk_descr_ptr->col;
  df->height=SPQ.fragm_disk_descr_ptr->height;
  df->w_width=SPQ.fragm_disk_descr_ptr->w_width;
  df->kegl=SPQ.fragm_disk_descr_ptr->kegl;
  df->type=SPQ.fragm_disk_descr_ptr->type;
  return;
 }                                     /* end of fill_fragm */

/************************************************************************/
int16_t set_descr(char c)
/*
    This procedure sets up sheet_descr or head of line.
    c=SHEET means sheet_descriptor; c=LINE means line_descriptor.
*/
 {
  int16_t i  /*,k*/;
  struct segm  *segm_ptr;
  /* char * return_str_ptr;*/
  check_free_mem();
  if (c == SHEET)
   {                                   /* sheet */
    SPQ.sheet_disk_descr_ptr=(struct sheet_disk_descr *)SPQ.buff_ptr;

    if ((uint32_t)(SPQ.buff_ptr - (uchar*)SPQ.text_buff) > (uint32_t)(SPQ.buff_l - sizeof(struct sheet_disk_descr)))
      return(NO);

	SPQ.qt_fm=SPQ.sheet_disk_descr_ptr->quant_fragm;
    if(SPQ.qt_fm >= MAX_FRAGM )                         return TOO_MANY_FRAGMS;

    if (SPQ.buff_ptr-(uchar*)SPQ.text_buff >=
	 SPQ.buff_l-SPQ.sheet_disk_descr_ptr->descr_lth)
			 /* not enough room in buffer for all fragm descr */
      return(NO);
    SPQ.cur_sheet_number=SPQ.sheet_disk_descr_ptr->sheet_numb;
    SPQ.cur_sheet=0;
    SPQ.sheet_flag[SPQ.cur_sheet]=EXIST;
    SPQ.tab_ptr->tab_sheet_descr[SPQ.cur_sheet].el_type=2;
    SPQ.tab_ptr->tab_sheet_descr[SPQ.cur_sheet].lth=
	   SPQ.sheet_disk_descr_ptr->descr_lth;
    SPQ.tab_ptr->tab_sheet_descr[SPQ.cur_sheet].first_segm=SPQ.st.cur_segm;
    for (i=0; i < SPQ.qt_fm; i++)
      fill_fragm(i);              /* fills descriptors of i-th fragment */
    mvbuf(SPQ.sheet_disk_descr_ptr->descr_lth);
   }
  else
   {                                   /* line */
     SPQ.st.cur_segm->string_lth+=DELTA;
     SPQ.free_alloc_mem+=DELTA;
   check_free_mem();
    if ( (uint32_t)(SPQ.buff_ptr - (uchar*)SPQ.text_buff) > (uint32_t)(SPQ.buff_l - sizeof(struct line_beg)) )
      return(NO);
    SPQ.ptr_next_segm=(struct segm *)SPQ.free_alloc_mem;
    SPQ.str_ptr=SPQ.ptr_next_segm->string;
    SPQ.free_alloc_mem=( char  *)(SPQ.ptr_next_segm+1);
    segm_ptr=(struct segm *) SPQ.tab_ptr->
		tab_fragm[SPQ.cur_sheet][SPQ.cur_fragm].last_segm;
    segm_ptr->next_in_fragm=SPQ.ptr_next_segm;
    SPQ.ptr_next_segm->prev_in_fragm = (struct segm  *)
    SPQ.tab_ptr->tab_fragm[SPQ.cur_sheet][SPQ.cur_fragm].last_segm;
    SPQ.ptr_next_segm->prev_in_sheet=SPQ.st.cur_segm;
    SPQ.st.cur_segm->next_in_sheet=SPQ.ptr_next_segm;
    SPQ.ptr_next_segm->prev_in_sheet=SPQ.st.cur_segm;
    SPQ.st.cur_segm=SPQ.ptr_next_segm;
    SPQ.st.cur_segm->next_in_sheet=NULL;
    SPQ.st.cur_segm->next_in_fragm=NULL;
    SPQ.st.cur_segm->type=1;
    SPQ.st.cur_segm->string_lth=0;
    SPQ.st.cur_segm->busy_lth=0;
    SPQ.st.cur_segm->q_vis_symb=0;
    SPQ.st.cur_segm->q_v_s_l=0;
    SPQ.str_ptr=SPQ.ptr_next_segm->string;
    SPQ.tab_ptr->tab_fragm[SPQ.cur_sheet][SPQ.cur_fragm].last_segm =
	   SPQ.ptr_next_segm;
    mvbuf(sizeof(struct line_beg));
   }
  return(YES);
 }


/*------------------------------------------------------------------*/
int16_t do_fragm(void)
/*
    This procedure sets up fragm_descr or head of line.
*/
 {
  SPQ.st.cur_segm->string_lth+=DELTA;
  SPQ.free_alloc_mem+=DELTA;

  check_free_mem();

  if ( (uint32_t)(SPQ.buff_ptr - (uchar*)SPQ.text_buff) > (uint32_t)(SPQ.buff_l - sizeof(struct fragm_disk)) )
    return(NO);
  SPQ.fragm_disk_ptr=(struct fragm_disk *)SPQ.buff_ptr;
  SPQ.cur_fragm=SPQ.fragm_disk_ptr->fragm_numb;
  SPQ.ptr_next_segm=(struct segm  *)SPQ.free_alloc_mem;
  SPQ.str_ptr=SPQ.ptr_next_segm->string;
  SPQ.st.cur_segm->next_in_fragm=SPQ.ptr_next_segm;
  SPQ.free_alloc_mem=(char  *)(SPQ.ptr_next_segm+1);
  SPQ.ptr_next_segm->prev_in_fragm=
	    SPQ.tab_ptr->tab_fragm[SPQ.cur_sheet][SPQ.cur_fragm].last_segm;
  if ((SPQ.sheet_flag[SPQ.cur_sheet] == EXIST) &&
	SPQ.fragm_flag[SPQ.cur_fragm]==NONEXIST);
   {
    SPQ.tab_ptr->tab_fragm[SPQ.cur_sheet][SPQ.cur_fragm].
	first_segm=SPQ.ptr_next_segm;
    SPQ.fragm_flag[SPQ.cur_fragm]=EXIST;
    SPQ.ptr_next_segm->prev_in_fragm=NULL;
   }
  SPQ.ptr_next_segm->busy_lth=0;
  SPQ.ptr_next_segm->string_lth=0;
  SPQ.ptr_next_segm->next_in_fragm=NULL;
  SPQ.ptr_next_segm->next_in_sheet=NULL;
  SPQ.ptr_next_segm->prev_in_sheet=SPQ.st.cur_segm;
  if (SPQ.st.cur_segm != NULL)
    SPQ.st.cur_segm->next_in_sheet=SPQ.ptr_next_segm;
  SPQ.st.cur_segm=SPQ.ptr_next_segm;
  SPQ.st.cur_segm->type=1;
  SPQ.st.cur_segm->q_vis_symb=0;
  SPQ.st.cur_segm->q_v_s_l=0;
  SPQ.tab_ptr->tab_fragm[SPQ.cur_sheet][SPQ.cur_fragm].last_segm=SPQ.st.cur_segm;
  mvbuf(sizeof(struct fragm_disk));
  return(YES);
 }
/********************************************************************/
/*
    This procedure gives address of first segment in current line.
*/
struct segm  *cur_line(char tt, struct segm * cur_segm)
 {
  if (tt == FRAGM)
   {                                   /* fragm */
    if (cur_segm->prev_in_fragm == NULL)
      return(cur_segm);
    if (cur_segm->type == NEW_LINE)
      return(cur_segm);
    else
      cur_segm=cur_line(FRAGM,cur_segm->prev_in_fragm);
   }
  else
   {                                   /* sheet */
    if (cur_segm->prev_in_sheet == NULL)
      return(cur_segm);
    if (cur_segm->type == NEW_LINE)
      return(cur_segm);
    else
      cur_segm=cur_line(SHEET,cur_segm->prev_in_sheet);
   }
 return(cur_segm);
 }


void skip_letter_in_line(struct segm  *segm_ptr,int16_t x)
/*
    This procedure sets up to SPQ.ns_symb adrress of letter
    with given x-coordinate in line.
    segm_ptr - adrress of some segment in line.
    x - x-coordinate.
    SPQ.ns_segm will be address of segment containing *SPQ.ns_symb.
    SPQ.skip_line_x - actual x-coordinate.
*/
 {
  int16_t i;
  int16_t max;

  uchar  *symb,  *c;
  struct segm  *segm_c;
  SPQ.end_l=NO;
  segm_ptr=cur_line(FRAGM,segm_ptr);
  symb=segm_ptr->string;
  i=1;
  if (*symb < 20)
    i=0;
  if (x == LAST_LETTER)
    max=MAX_LINE_LTH;
  else
    max=x;
  for (; i <= max; i++)
   {
    segm_c=segm_ptr;
    c=symb;
    symb=next_symb(YES,YES,YES,segm_ptr,symb);
    segm_ptr=SPQ.ns_segm;
    if (symb == NULL)
     {
      if (x == LAST_LETTER)
        if (c != NULL && *c > 20)
         {
          symb=c;
          SPQ.ns_segm=segm_c;
         }
      else
       {
        if (c != NULL)
          symb=c+define_lth(c);
       }
      SPQ.end_l=YES;
      break;
     }
   }
  SPQ.ns_symb=symb;
  SPQ.skip_line_x=i-1;
  if (SPQ.skip_line_x < 0)
    SPQ.skip_line_x=0;
 }
/*****************************************************/
void move_from_near_to_huge(char  *a, char *b, int16_t l)
{
 while (l--) *a++=*b++;
}
void move_from_cur_to_new(struct segm  *cur_segm, char  *cur_symb)
/*
    This procedure relocates string from current segm to new,
    as well as changes busy and string lengths & SPQ.free_alloc_mem
    if it's necessary.
*/
 {
 uchar  *s_old;
 uchar  *s;
 uchar  * s_new;

  s_new=&SPQ.new_segm->string[SPQ.new_segm->busy_lth];
  s_old=&cur_segm->string[cur_segm->busy_lth];
  s=cur_symb;
  while (s != s_old)
   {
    *s_new++=*s++;
    SPQ.new_segm->busy_lth++;
    cur_segm->busy_lth--;
    if (SPQ.new_segm->busy_lth == SPQ.new_segm->string_lth)
     {
      SPQ.new_segm->string_lth+=SIZE_NEW_SEGM;
      SPQ.free_alloc_mem+=SIZE_NEW_SEGM;
     }
   }
 }
 /*------------------------*/
void shift(char dir,int16_t v_s,struct segm *cur_segm,char *cur_symb)
/*
    This procedure transfers text.
    dir - RIGHT/LEFT.
    v_s - value of shift.
    cur_symb - address of cut.
*/
 {
  char  *c;
  /*int16_t l;*/
  char  *from;
  char  *to;
  char  *end;

  c=cur_symb;
  if (c == NULL)
   {
#ifdef SYSPR_ERROR
    PRINTF("ERROR: in shift - current symbol is absent\n");
#endif
    ErrorExit(RLING_ERROR_WRONG_ED_FILE);
   }
  if (dir == RIGHT)
   {
    from=(char  *)cur_segm;
    from+=sizeof(struct segm)+cur_segm->busy_lth-1;
    to=from+v_s;
    while (from >= cur_symb)
      *to--=*from--;
    cur_segm->busy_lth +=v_s;
   }
  else
   {
    end=(char  *)cur_segm;
    end+=sizeof(struct segm)+cur_segm->busy_lth;
    from=(char  *)cur_symb;
    to=from-v_s;
    while (from < end)
      *to++=*from++;
    cur_segm->busy_lth -=v_s;
   }
 }

 int16_t create_segm(struct segm *prev_adr,uint16_t segm_size,uint16_t type)
/*
    This procedure allocates new segment and connects it with old ones.
*/
 {
  struct segm *sNew;
  struct segm *nf;
  struct segm *ns;
  check_free_mem();

  if (prev_adr == NULL)
	  return(NO);

  sNew=(struct segm  *)SPQ.free_alloc_mem;
  nf=prev_adr->next_in_fragm;
  ns=prev_adr->next_in_sheet;
  prev_adr->next_in_fragm=sNew;
  prev_adr->next_in_sheet=sNew;
  if (nf != NULL)
   {
    nf->prev_in_fragm=sNew;
    nf->prev_in_sheet=sNew;
   }
  else
    if (ns != NULL)
      ns->prev_in_sheet=sNew;
  if (SPQ.tab_ptr->tab_fragm[SPQ.cur_sheet][SPQ.cur_fragm].last_segm == prev_adr)
    SPQ.tab_ptr->tab_fragm[SPQ.cur_sheet][SPQ.cur_fragm].last_segm=sNew;
  sNew->next_in_sheet=ns;
  sNew->next_in_fragm=nf;
  sNew->prev_in_fragm=prev_adr;
  sNew->prev_in_sheet=prev_adr;
  sNew->string_lth=segm_size;
  sNew->busy_lth=0;
  sNew->type=(char)type;
  SPQ.new_segm=sNew;
  SPQ.free_alloc_mem += sizeof(struct segm)+segm_size;
  return(YES);
 }

int16_t insert_symb(cur_segm, cur_symb,new_symb)
/*
    This procedure inserts symbol in current position.
    If the string doesn't contain letters we place new letter
    from string[cur_segm->busy_lth]
    new_symb ---place for new symb.
    cur_segm & cur_symb addresse ,that new symb'll be placed after it.
*/
 struct segm  *cur_segm;
 char  * cur_symb;
 LT  *new_symb;
 {
  int16_t l;
  char shift_flag;
 LT  *new_symb1;
  int16_t flag_n;
  int16_t flag_b;
  flag_b=NO;
  flag_n=NO;
  if (SPQ.ins_status == NO)
   {
    if ((SPQ.flag_out == NO) &&
     ((new_symb1 =
           (LT *)next_symb(YES,YES,YES,cur_segm,cur_symb)) != NULL))
     {
      l=define_lth(cur_symb);
      shift(LEFT,(int16_t)(l-sizeof(LT)),cur_segm,cur_symb+l);
      new_symb1=(LT  *)cur_symb;
      return(NO);
     }
   }
  shift_flag=YES;
  if ((uint32_t)(cur_segm->busy_lth + sizeof(LT)) > (uint32_t)(cur_segm->string_lth))
   {                                   /* segment is full */
   flag_b=YES;
    create_segm(cur_segm,SIZE_NEW_SEGM,2);
    SPQ.ns_segm=SPQ.new_segm;
    SPQ.ns_symb=(char *)(SPQ.ns_segm->string);
    SPQ.ns_symb+=0;
    flag_n=YES;
    shift_flag=NO;
    if (SPQ.flag_out == NO)
     {
      move_from_cur_to_new(cur_segm,cur_symb);
      cur_segm->busy_lth+=sizeof(LT);
     }
    else
     {
      cur_symb=SPQ.new_segm->string;
      cur_segm=SPQ.new_segm;
     }
   }
                                       /* writing in tail */
  if (cur_symb == NULL)
    cur_symb=&cur_segm->string[cur_segm->busy_lth];
  else                                 /* cur_symb != NULL */
   {
    if ((SPQ.flag_out == NO) && (flag_n != YES))
     {
      if(shift_flag == NO)
      cur_segm->busy_lth += sizeof(LT);
     }
    else
      if ((SPQ.flag_out == YES) && (shift_flag==NO))
        cur_segm->busy_lth+=sizeof(LT);
   }
  if (shift_flag == YES)
    shift(RIGHT,sizeof(LT),cur_segm,cur_symb);
  new_symb1=(LT  *)cur_symb;
  new_symb1->code=new_symb->code;
 new_symb1->attr=new_symb->attr;
 return(flag_b);
 }

move_to_first( int16_t dir,uchar let,int16_t num)
/*
 This procedure moves to first space symbol.
 dir -- direction
 num =YES means that we searches in all lines.
 */
{
 /*char  *symb;*/
/* int16_t rec;*/
 SPQ.E.miss=NO;
 if(dir== RIGHT)
  {
  while(1)
   {
   if(SPQ.E.miss==NO)
   {
    SPQ.ns_symb=next_symb(YES,YES,YES,SPQ.ns_segm,SPQ.ns_symb);
    SPQ.E.x_a++;
    SPQ.E.x++;
    }
    SPQ.E.miss=NO;
    if(let == SPACE)                  /* RIGHT SPACE*/
     {
      if(*SPQ.ns_symb != SPACE)
      {
      if(SPQ.ns_symb==NULL)
      {
      if(num==NO)return(NO);
      if(n_l() == NO)return(NO);
      }
       continue;
       }
      else
	break;
     }
    else                                  /* RIGHT NONSPACE*/
     {
      if(*SPQ.ns_symb == SPACE )
       continue;
       else
	{
	 if(SPQ.ns_symb==NULL)
	 {
	 if(num==NO)return(NO);
	 if(n_l() == NO)
	  return(NO);
	 else
	  continue;
	  }
	break;
       }
     }
   }                      /* END of WHILE*/
  }
 else                                  /* going to the left*/
 {
 while(1)
  {
  if(SPQ.E.miss==NO)prev_letter();
  SPQ.E.miss=NO;
  if(let ==SPACE)
   {
   if(*SPQ.ns_symb !=SPACE)
   {
    if(SPQ.ns_symb==NULL)
    {
     if(num== NO)return(NO);
     if(p_l() == NO)
      return(NO);
      }
    continue;
   }
    else
     break;
   }
  else                                 /* searcing for NONSPACE*/
   {
   if(*SPQ.ns_symb == SPACE || *SPQ.ns_symb < 20)
    continue;
    else
     {
      if(SPQ.ns_symb==NULL)
      {
      if(num==NO)return(NO);
      if(p_l()==NO) return(NO);
      else
       continue;
       }
      break;
     }
   }                                   /* WHILE*/
  }
 }
   return(YES);
}


int16_t n_l(void)
{
SPQ.ns_segm=next_line(SHEET,SPQ.ns_segm);
if(SPQ.ns_segm== NULL)
 return(NO);
 else
 {
  skip_letter_in_line(SPQ.ns_segm,0);
  SPQ.E.y_a++;
  SPQ.E.y++;
  SPQ.E.x_a=0;
  SPQ.E.x=SPQ.w_x;
  }
 return(YES);
}

int16_t p_l(void)
{
 int16_t shift,shift1;
 SPQ.ns_segm=cur_line(FRAGM,SPQ.ns_segm)->prev_in_sheet;
 if(SPQ.ns_segm== NULL)
  return(NO);
 else
 {
 shift=SPQ.E.x_a-SPQ.E.x+SPQ.w_x;
 skip_letter_in_line(SPQ.ns_segm,LAST_LETTER);
 if(*SPQ.ns_symb != SPACE)SPQ.E.miss=YES;
 SPQ.E.x_a=SPQ.skip_line_x;
 shift1=SPQ.E.x_a-shift;
 if(shift1 <= SPQ.w_width)
  SPQ.E.x=SPQ.w_x+shift1;
 else
  SPQ.E.x=SPQ.w_x+SPQ.E.x_a;
 }
 SPQ.E.y_a--;
 SPQ.E.y--;
return(YES);
}

int16_t prev_letter(void)
/*
 This procedure moves to previos letter in line and changes
 SPQ.E.x,SPQ.E.x_a,SPQ.E.y,SPQ.E,y_a,SPQ.ns_symb,SPQ.ns_segm.
 */
{
 if (SPQ.E.x_a <= 0)
    {
     SPQ.ns_symb=NULL;
     return(NO);
     }
    SPQ.E.x_a--;

      if(SPQ.ns_symb==NULL)
      if( SPQ.flag_out == YES)
       {
       skip_letter_in_line(SPQ.ns_segm,LAST_LETTER);
        SPQ.E.x_a=SPQ.skip_line_x;
	}
      else return(NO);
       SPQ.E.x--;
      skip_letter_in_line(SPQ.ns_segm,SPQ.E.x_a);
 return(YES);
 }
 int16_t load_fragm()
 /*
 This procedure loads givev fragm atherwise returns NO.
 */
 {
 struct segm * sgm_ptr;
 if(  SPQ.tab_ptr->tab_fragm[SPQ.cur_sheet][SPQ.cur_fragm].first_segm== NULL)
return(NO);
 else
 SPQ.st.cur_segm=SPQ.tab_ptr->tab_fragm[SPQ.cur_sheet][SPQ.cur_fragm].first_segm;
  skip_letter_in_line(SPQ.st.cur_segm,0);
  SPQ.st.cur_symb=SPQ.ns_symb;
  SPQ.curs_y_a=0;
  SPQ.curs_x_a=0;
  SPQ.curs_y_w=SPQ.w_y;
  SPQ.curs_x_w=SPQ.w_x;
  sgm_ptr= SPQ.tab_ptr->tab_fragm[SPQ.cur_sheet][0].first_segm;
  while(sgm_ptr != SPQ.tab_ptr->tab_fragm[SPQ.cur_sheet][SPQ.cur_fragm].first_segm)
  {
  SPQ.curs_y_a++;
  sgm_ptr=next_line(SHEET,sgm_ptr);
  }
 /*
  refresh_window();
  EGA_curs(SPQ.curs_y_w,SPQ.curs_x_w);
 */
  create_new_stack();
  SPQ.stack->map_ref.row=0;
  SPQ.stack->map_ref.col=0;
  return(YES);
 }
/******************************************************************/
 int16_t test_spare_space(struct segm * segm,int16_t need_size)
 {
   return
     (segm->busy_lth + need_size > segm->string_lth)? NO : YES  ;
 }
