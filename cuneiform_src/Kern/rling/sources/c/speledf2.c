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
#else
  #error   NO TOOOL SPECIFIED
#endif

/////#include <dos.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <setjmp.h>
/*#define BUFSIZE 8192*/
#define MAX_STRING_LTH 80

//#include "iolib.h"
#include "speldefs.h"
#include "spelfunc.h"

#include "edtable.h"

//////////////////////
// AK 27.01.99
#include "tigeremulate.h"
void	ErrorExit(int Code);
// end AK
/*************************************************************************/
/************ L o c a l   f u n c t i o n s   p r o t o t y p e s  *******/
/*************************************************************************/
static int16_t check_mem(int16_t l);
static int16_t push_stack(void);
static int16_t pop_stack(void);
static int16_t esc_next_symb(int16_t i,int16_t j ,int16_t k );
static int16_t check(int16_t i,int16_t j);
static int16_t check_letter(int16_t j);
static void copy_prev_level(void);
static void stack_to_null(void);
/*************************************************************************/
extern dQ SPQ;     //Q;

uchar  *next_symb(int16_t i, int16_t j, int16_t k,
      struct segm  *cur_segm, uchar  *cur_symb)
/*
    This procedure is in charge with sleathering through the string.
    i=YES means missing rule symbols,returns pointer to the next letter
	  of the string.
    j=YES means missing alternatives.
    k=YES means search in all segments of given line.
    k=NO  means searh in given segm
*/
 //int16_t i,j,k;
 //struct segm  *cur_segm;
 //uchar  *cur_symb;
 {
  uint16_t ret;
  uchar l;
  void * NsCache[8];
  uint16_t NsCount = 0;
  /* struct segm  *segm_ptr; */

  if (cur_symb == NULL)
    return(NULL);
  SPQ.ns_segm=cur_segm;
  SPQ.N.beg_next=YES;
  SPQ.N.end_altern=YES;
  SPQ.end_s=0;
  SPQ.end_l=0;
  ret=NO;
  SPQ.N.last_altern=NO;
  SPQ.ns_symb=cur_symb;
  if (check_mem(k) == YES)
    return(NULL);
  while (1)
   {
    switch (*SPQ.ns_symb)
     {
      case SS_SHEET_DESCR:
		       SPQ.end_altern=1;
		       l = (uchar)SPQ.tab_ptr->tab_sheet_descr[SPQ.cur_sheet].lth;
                       SPQ.ns_symb=&SPQ.ns_segm->string[l];
		       break;
      case SS_LANGUAGE  : SPQ.ns_symb=SPQ.ns_symb+sizeof(struct EdTagLanguage);
                            break;
      case SS_FRAGMENT:
                       SPQ.cur_fragm=((struct fragm_disk *)SPQ.ns_symb)->fragm_numb;
		       if (SPQ.stack_change_flag == YES)
      			 create_new_stack();
		       SPQ.end_altern=1;
		       SPQ.ns_symb=SPQ.ns_symb+sizeof(struct fragm_disk);
		       break;
      case SS_BITMAP_REF:
		       SPQ.h_bit_map_ref_ptr=
                                 (struct bit_map_ref  *)SPQ.ns_symb;

		       if (SPQ.stack_change_flag == YES)
			{
			 SPQ.stack->ns_ref_ptr=SPQ.ns_symb;
			 SPQ.stack->map_ref.row=
				 SPQ.h_bit_map_ref_ptr->row;
			 SPQ.stack->map_ref.col=
				 SPQ.h_bit_map_ref_ptr->col;
			 SPQ.stack->map_ref.width=
				 SPQ.h_bit_map_ref_ptr->width;
			 SPQ.stack->map_ref.height=
				 SPQ.h_bit_map_ref_ptr->height;
			}
		       SPQ.ns_symb=SPQ.ns_symb+sizeof(struct bit_map_ref);
		       break;
      case SS_STEP_BACK:
		       SPQ.ns_symb=SPQ.ns_symb+sizeof(struct step_back);
		       break;
      case SS_TEXT_REF:
                       SPQ.h_text_ref_ptr=(struct text_ref  *)SPQ.ns_symb;
		       if (SPQ.stack_change_flag == YES)
             switch(SPQ.h_text_ref_ptr->type)
			  {
			   case 0: SPQ.stack->letter_ref=
					SPQ.h_text_ref_ptr->object;break;
			   case 1: SPQ.stack->halo_ref=
					SPQ.h_text_ref_ptr->object;break;
			   case 2: SPQ.stack->word_ref=
					SPQ.h_text_ref_ptr->object;break;
			   case 3: SPQ.stack->line_ref=
					SPQ.text_ref_ptr->object; break;
			  }
		       SPQ.ns_symb=SPQ.ns_symb+sizeof(struct text_ref);
		       break;
      case SS_FONT_KEGL:
                       SPQ.h_font_kegl_ptr=(struct font_kegl  *)SPQ.ns_symb;
		       if (SPQ.stack_change_flag == YES)
			{
			 push_stack();
			 SPQ.stack->kegl = SPQ.h_font_kegl_ptr->new_kegl;
			 SPQ.stack->font = (uchar)SPQ.h_font_kegl_ptr->new_font;
			}
			 SPQ.ns_symb=SPQ.ns_symb+sizeof(struct font_kegl);
		       break;
      case SS_KEGL:
                       SPQ.h_kegl_ptr=(struct kegl  *)SPQ.ns_symb;
		       if (SPQ.stack_change_flag == YES)
			{
			 push_stack();
			 SPQ.stack->kegl=SPQ.h_kegl_ptr->new_kegl;
			}
		       SPQ.ns_symb=SPQ.ns_symb+sizeof(struct kegl);
		       break;
      case SS_SHIFT:
                       SPQ.h_shift_ptr=(struct shift_struct  *)SPQ.ns_symb;
		       if (SPQ.stack_change_flag == YES)
			{
			 push_stack();
			 SPQ.stack->stack_type=2;
			 l=SPQ.stack->line_row+SPQ.h_shift_ptr->kegl;
			 SPQ.stack->line_row=l;
			}
		       SPQ.ns_symb=SPQ.ns_symb+sizeof(struct shift_struct);
		       break;
      case SS_RETR_LEVEL:
		       if (SPQ.stack_change_flag == YES)
			{
			 pop_stack();
			 SPQ.stack->stack_type=2;
			}
		       SPQ.ns_symb=SPQ.ns_symb+sizeof(struct retrieve_level);
		       break;
      case SS_UNDERLINE:
                       SPQ.h_underline_ptr= (struct underline  *)SPQ.ns_symb;
		       if (SPQ.stack_change_flag == YES)
			{
			 if (SPQ.h_underline_ptr->type < 6)
			   push_stack();
			 if (SPQ.h_underline_ptr->type < 4)
			 SPQ.stack->font_type=SPQ.h_underline_ptr->type;
			 else
			 SPQ.stack->underline=SPQ.h_underline_ptr->type;
			}
		       SPQ.ns_symb=SPQ.ns_symb+sizeof(struct underline);
		       break;
      case SS_DENS_PRINT:
                       SPQ.h_dens_print_ptr=(struct dens_print  *)SPQ.ns_symb;
		       if (SPQ.stack_change_flag == YES)
			{
			 push_stack();
             SPQ.stack->density=SPQ.h_dens_print_ptr->dens_atr;
			}
		       SPQ.ns_symb=SPQ.ns_symb+sizeof(struct dens_print);
		       break;
      case SS_TABUL:
		       SPQ.ns_symb=SPQ.ns_symb+sizeof(struct tabul);
		       break;
      case SS_TABL_TABUL:
		       l=*(SPQ.ns_symb+1);
		       if(l==0)
		       l=1;
		       SPQ.ns_symb=SPQ.ns_symb+l;
		       break;
      case SS_LINE_BEG:
		       if (SPQ.stack_change_flag == YES)
			 create_new_stack();
		       SPQ.end_altern=1;
		       SPQ.st.position=0;
		       SPQ.ns_symb=SPQ.ns_symb+sizeof(struct line_beg);
		       break;
      case SS_POSITION:
                       SPQ.h_position_ptr=(struct position  *)SPQ.ns_symb;
		       if (SPQ.stack_change_flag == YES)
             SPQ.stack->position=SPQ.h_position_ptr->pos;
		       SPQ.ns_symb=SPQ.ns_symb+sizeof(struct position);
		       break;
      case SS_TABL_CONFORM_SIZES:
		       SPQ.ns_symb=SPQ.ns_symb+sizeof(struct tabl_conform_sizes);
		       break;
      case SS_GROUP_WORDS:
		       if (SPQ.stack_change_flag == YES)
			 SPQ.stack->stack_type=1;
		       SPQ.ns_symb=SPQ.ns_symb+sizeof(struct group_words);
		       break;
      case SS_GROUP_SYMBOLS:
		       if (SPQ.stack_change_flag == YES)
			 SPQ.stack->stack_type=1;
		       SPQ.ns_symb=SPQ.ns_symb+sizeof(struct group_symbols);
		       break;
      case SS_AKSANT:
		       SPQ.ns_symb=SPQ.ns_symb+sizeof(struct aksant);
		       break;
      case SS_NEG_HALF_SPACE:
		       SPQ.ns_symb=SPQ.ns_symb+sizeof(struct neg_half_space);
		       break;
      case SS_POS_HALF_SPACE:
		       SPQ.ns_symb=SPQ.ns_symb+sizeof(struct pos_half_space);
		       break;
      default:
		      if((*SPQ.ns_symb<0x20)&&(*SPQ.ns_symb!=0x15))
                          SPQ.ns_symb+=ED_LREC(SPQ.ns_symb);
		       else
		       {
		         if ((*(SPQ.ns_symb+1) & 0x1))
			   SPQ.N.end_altern=YES;
		         else
			   SPQ.N.end_altern=NO;
                         SPQ.ns_symb+=sizeof(LT);
                       }
		       break;
     }  			 /* end of big switch */
    ret=esc_next_symb(i,j,k);
    if (ret == YES)
      return(SPQ.ns_symb);

        // Allex 28/01/2000
       //временная заглушка на случай зацикливания
      NsCache[NsCount++] = (void *) SPQ.ns_symb;

      if ( NsCount > 1 )
        if ( NsCache[NsCount-2] != NsCache[NsCount-1] )
        {
            NsCount = 0;
        }

      if ( NsCount == 8 )
            ErrorExit(RLING_ERROR_WRONG_ED_FILE);
          //return(SPQ.ns_symb);
      // конец заглушки
   }                                   /* end of while */
/* return(SPQ.ns_symb);*/
 }

int16_t check_mem(int16_t l)
/*
    This procedure controls placing in boundaries of current string
    and changing address of temporary segm due to overrunning
    in current_segm and permission to use next
*/
 {
  struct segm  * segm_ptr;
  char  *s;

  s=&SPQ.ns_segm->string[SPQ.ns_segm->busy_lth];
  if (SPQ.ns_symb < s)
    return(NO);
  else
   {
    if (l == NO)
      return(YES);
    while (1)
     {
      segm_ptr=SPQ.ns_segm->next_in_fragm;
      if ((segm_ptr != NULL) && (segm_ptr->type == CONT_LINE))
       {                               /* line continuation */
	SPQ.ns_segm=segm_ptr;
	SPQ.ns_symb=segm_ptr->string;
	if (segm_ptr->busy_lth > 0)
	  return(NO);
	else
	  continue;
       }
      else
       {
	SPQ.end_l=YES;
	return(YES);
       }
     }
   }
/* return(YES);*/
 }

int16_t esc_next_symb(int16_t i,int16_t j ,int16_t k )
/*
      This procedure controls exit from subroutine next_symb.
      i=YES means missing control symbols.
      j=YES means missing alternative symbols
      k=YES means permission to use all segments
*/
 {
  if (check_mem(k) == YES)
   {
    SPQ.ns_symb=NULL;
    return(YES);
   }
  return(check(i,j));
 }

int16_t check(int16_t i,int16_t j)
/*
     This procedure checks presence of next symbol.
*/
 {
  if (i==YES)
   {
    if ((*SPQ.ns_symb < SS_NEG_HALF_SPACE) && (*SPQ.ns_symb != 0x15) )
     {
      SPQ.N.rec=NO;
      SPQ.N.end_altern=YES;
      return(NO);
     }
    else
      return(check_letter(j));         /* letter */
   }
  else                                 /* don't miss control symbols*/
   {
    if ((*SPQ.ns_symb >= SS_NEG_HALF_SPACE) || (*SPQ.ns_symb == 0x15) )
      return(check_letter(j));         /* letter */
    else                               /* control symbol */
     {
      SPQ.N.rec=NO;
      SPQ.N.end_altern=YES;
      return(YES);
     }
   }
 }

int16_t check_letter(int16_t j)
/*
     This procedure does checking for letter.
*/
 {
  int16_t k;

  if (j == YES)
   {                                   /* skip alternatives */
    if (SPQ.N.end_altern == YES)
      k=YES;
    else
      k=NO;
    if ((*(SPQ.ns_symb+1) & 0x1))
      SPQ.N.end_altern=YES;
    else  SPQ.N.end_altern=NO;
      SPQ.N.rec=NO;
    return(k);
   }
  else                                 /* don't skip alternatives */
   {
    if (*(SPQ.ns_symb+1) & 0x1)
      SPQ.N.end_altern=YES;
    else
      SPQ.N.end_altern=NO;
    return(YES);
   }
 }

int16_t push_stack(void)
/*
    This procedure creates new level in stack and copies to it previous one.
*/
 {
 /*
  SPQ.stack-=sizeof(struct state_stack_elem);
 */
  SPQ.stack--;
  SPQ.st.stack_level++;
  copy_prev_level();
  check_free_mem();
 return(OK);
 }

void copy_prev_level(void)
/*
   This procedure copies stack from preceeding level to current one.
*/
 {
  char  *from;
  char  *to;
  int16_t i;

  if (SPQ.st.stack_level == 0)
    return;
  to=(char  *)SPQ.stack;
  from=to+sizeof(struct state_stack_elem);
  for (i=0; i <= sizeof(struct state_stack_elem); i++)
    *to++=*from++;

 }
//////////////////////////////////////////////////////////////////////////////////////////////////
//This procedure creats new stack.
void create_new_stack(void)
{
	int16_t i;
	char  * symb;
	struct segm  * segm;

	symb=SPQ.ns_symb;
	check_free_mem();
	segm      = SPQ.st.cur_segm;
	SPQ.stack = (struct state_stack_elem  *)(SPQ.end_alloc_mem - sizeof(struct state_stack_elem));
	stack_to_null();
	SPQ.st.stack_level = 0;
	SPQ.st.cur_segm    = SPQ.tab_ptr->tab_sheet_descr[SPQ.cur_sheet].first_segm;
	SPQ.ns_symb        = SPQ.st.cur_segm->string;

	while (*SPQ.ns_symb  != SS_SHEET_DESCR)
	{
		if(SPQ.ns_symb == NULL)
		{
#ifdef SYSPR_ERROR
			PRINTF("Wrong file...\n");
#endif
			ErrorExit(RLING_ERROR_WRONG_ED_FILE);
		}

		SPQ.ns_symb = next_symb(NO,YES,NO,SPQ.ns_segm,SPQ.ns_symb);
	}

	SPQ.h_fragm_disk_descr_ptr = (struct fragm_disk_descr *)(SPQ.ns_symb + sizeof(struct sheet_disk_descr));

	for (i=0; i < SPQ.cur_fragm; i++)
		SPQ.h_fragm_disk_descr_ptr++;

	SPQ.stack->kegl      = SPQ.h_fragm_disk_descr_ptr->kegl;
	SPQ.stack->font      = SPQ.h_fragm_disk_descr_ptr->font;
	SPQ.stack->language  = SPQ.h_fragm_disk_descr_ptr->language;//03-29-94 02:54pm,Pit
	SPQ.stack->font_type = SPQ.h_fragm_disk_descr_ptr->font_type;
	SPQ.ns_symb          = symb;
	SPQ.st.cur_segm      = segm;
}
/////////////////////////////////////////////////////////////////////////
void stack_to_null(void)
/*
    This procedure clears given stack;
*/
 {
  SPQ.stack->stack_type=0;                     /* ??????? */
  SPQ.stack-> line_row=0;                      /* ??????? */
  SPQ.stack->position=0;
  SPQ.stack->font=0;                           /* font */
  SPQ.stack->language=0;                       //03-29-94 02:53pm,Pit
  SPQ.stack->kegl=0;                           /* kegl */
  SPQ.stack-> font_type=0;                     /* font type */
  SPQ.stack->density=0;                        /* printing density */
  SPQ.stack->underline=0;                      /* presence of underline */
  SPQ.stack->map_ref.row=0;                 /* address of cur_symb in bit_map */
  SPQ.stack->map_ref.col=0;
  SPQ.stack->map_ref.width=0;
  SPQ.stack->map_ref.height=0;
  SPQ.stack->letter_ref=0;                      /* ??????? */
  SPQ.stack->halo_ref=0;                        /* ??????? */
  SPQ.stack->word_ref=0;                        /*  */
  SPQ.stack->line_ref=0;                        /* numb of symb incl. all symb */
  SPQ.stack->next=NULL;                         /* addres of stack next level */
 }

int16_t check_free_mem(void)
/*
     This procedure checks possibility of intersection
     memory for stack and memory for text.
*/
 {
  if ((char  *)(SPQ.stack) < SPQ.free_alloc_mem +
		      sizeof(struct state_stack_elem))
   {
#ifdef FOR_EDIT
    EGA_clr_wnd(SPQ.w_y-1,SPQ.w_x,SPQ.w_height,SPQ.w_width);
    EGA_close();
#endif

#ifdef SYSPR_ERROR
    PRINTF("Too few memory\n");
#endif
    ErrorExit(RLING_ERROR_MEMORY_ERROR);
   }
 return (OK);
 }

int16_t pop_stack(void)
/*
   This procedure subtract stack_level by 1.
*/
 {
  if ((--(SPQ.st.stack_level)) >= 0)
    SPQ.stack++;
  else
   {
#ifdef SYSPR_ERROR
    PRINTF("ERROR: level of stack < 0\n");
#endif
    ErrorExit(RLING_ERROR_WRONG_ED_FILE);
   }
 return(OK);
 }
/***********************************************************/
void next_fragm(void)
/*
 This procedure loads new fragm.
 */
 {
  SPQ.cur_fragm++;
  if(load_fragm()==NO)
  SPQ.cur_fragm--;
 }

 void prev_fragm(void)
 /*
 This procedure loads previouse fragm.
 */
 {
 if(SPQ.cur_fragm==0)return;
 SPQ.cur_fragm--;
 if(load_fragm()==NO)
 SPQ.cur_fragm++;
 }
