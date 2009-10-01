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

#include <assert.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include "compat_defs.h"
/*************************************************************************/
/* tg_spell.c -  general spelling loop. It scans ED-file,
 reads objects, i.e. info between reliable spaces (don't confuse
 with suspected ones), and estimates words generated from the objects
 to choose the only best word for each object. The best word is being
 outputed as the first alternatives for object positions
 */
/*************************************************************************/

//
// 08-13-93 07:58pm, Mike.
// Comments:  Updating for multi-lingual version of CuneiForm ( release
//            28-June-93 ). Release has been received from Joe by FEDERAL
//            mail.
//
// The main target: To use new stream technology : IOLIB.H.
//
// Process indicator:
//  1. Preliminary updation. 08-13-93 06:40pm, Mike
//
#include "tuner.h"
#include "spelmode.h"

#include "tigeremulate.h"
/*
 #if defined(TURBO_C)
 #include "tc_types.h"
 #include <dir.h>
 #elif defined(WATCOM)
 #include "spelwatc.h"
 #elif defined(THINK_C)
 #include "spelthin.h"
 #else
 #error   NO TOOOL SPECIFIED
 #endif
 */
#include <ctype.h>
#include <time.h>

#include "speldefs.h"
#include "spelfunc.h"
//  #include "iolib.h"        // IO stream library
#include "lang.h"         // language trigger
#include "compat_defs.h"
///////////////////////////////////////////////////////////////////////////////
//
#ifdef _DEBUG
#ifdef RLING_SECONDARY
//FILE *dbg_fs;
#define TPR(x)  /*   dbg_fs=fopen("rlings.dbg","a");\
                   fprintf(dbg_fs,"%s \t %d \t %p \t %s \n",#x ,obj_number, SPQ.ns_symb,obj.wordchar );\
                   fclose(dbg_fs);
				*/
#else
//FILE *dbg_f;
#define TPR(x)  /*   dbg_f=fopen("rling.dbg","a");\
                   fprintf(dbg_f,"%s \t %d \t %p \t %s \n",#x ,obj_number, SPQ.ns_symb,obj.wordchar );\
                   fclose(dbg_f);
				*/
#endif
#else
#define TPR(x)
#endif

//////////////////////////////////////////////////////////////////////////////////////
extern int16_t total_words;
extern user_voc voc_array[];
extern int16_t real_voc_no;

uint32_t progress_set_step(uint32_t volume);
uint32_t progress_set_percent(uint32_t volume);
////////////////////////////////
void ErrorExit(int Code);
////////////////////////////////
#ifdef SECOND_PASS

extern RSTAT rst[];
extern int16_t rst_last;
extern uchar ast[MAX_ARTS];
extern ARTS artbase[];

#endif

// AK kill all stupid macros
//#define st Q.st


/********DEBUG*********/
int16_t db = 187;
/*int16_t env; */
/* =================================================================== */
/*                      Functions' Definition                          */
/* =================================================================== */

static int16_t batch_setup(void);
static int16_t run_page(void);
static int16_t ed_conv(struct dict_state *, user_voc *, int16_t);
static int16_t anal_part_wrd(SOBJ*, LTIMG**, struct dict_state*, user_voc*, int16_t);

/***********************************************************************/
/*                                                                     */
/*                  common used (global) variables                     */
/*                                                                     */
/***********************************************************************/
int16_t back_flag = 0; /* nonzero if back_recog required */
int16_t debug_obj_nmb = 1;

#ifdef TIMETEST
static char time_buf[80]; /* buff to output time */
#endif

static char *q_free_alloc_mem; /* poiter to save SPQ.free_alloc_mem */

extern dQ SPQ; //Q;
extern int16_t vocs_NOK; // 08-13-93 07:45pm, Mike
extern char spell_buff_pool[]; // 08-13-93 07:40pm, Mike
extern char spell_buff_out[]; // 08-13-93 07:40pm, Mike

#ifdef SECOND_PASS
voc_state wvoc;
WSTACK wstack;
#endif

#ifdef  RUS_ENG_LANG
extern uchar multy_language;
#endif

/************************************************************************/
int16_t spelling(uchar *beg, int32_t size) {
	int16_t ret;
	SPQ.beg_alloc_mem = SPQ.free_alloc_mem = beg;
	SPQ.end_alloc_mem = (char *) beg + size * 16;
	SPQ.alloc_size = size;
	ret = batch_setup();
	if (ret)
		return ret;
	ret = run_page();
	return ret;
}
/**************************************************************************/
/*  Batch_setup                                                           */
/**************************************************************************/
static int16_t batch_setup(void) {
#ifdef S_COMMENTS
	initcode(); /* see spellfun : these are symb */
	initstdn(); /*                convert tables */
#endif
	/*  Static dictionary initialisation */

	init_stat_dict(&SPQ.d_state);

	q_free_alloc_mem = SPQ.free_alloc_mem; /* SAVE ! */

	// 08-13-93 07:41pm, Mike
	// SPQ.text_buff = SPQ.text_buff_pool;
	SPQ.text_buff = spell_buff_pool;

	return 0; /* It's OK */
}

/**************************************************************************/
/*  Run one page                                                          */
/**************************************************************************/
static int16_t run_page() /* run one page */
{
	int16_t obj_nmb = 0;

	SPQ.free_alloc_mem = q_free_alloc_mem; /* RESTORE ! */

	if (begin(0) == FALSE) {
		ErrorExit(RLING_ERROR_WRONG_ED_FILE);
		//return WRONG_FILE;
	}

	// !!! And now we must test ed-file  to preserve it
	//   from secondary spelling checking (something converted to blue !)
	// ??? ACT !!!
	if (*find_byte_flag() & FIR_CHECKED) {
		ErrorExit(RLING_ERROR_ED_ALREADY_CHEKED);
		//return(TWICE_ERR);
	}

	// TG_SPELL
	obj_nmb = ed_conv(&SPQ.d_state, voc_array, real_voc_no);

#ifdef  RUS_ENG_LANG
	if (!multy_language||language!=LANG_RUSSIAN)
#endif
	//Set up spelling flag
	*find_byte_flag() |= FIR_CHECKED;

	save_ed_file(0);
	return (0);
}

#ifdef WATCOM

int32_t read_all_vfile( char *path, char *buff)
{
	int f;
	int32_t lth;

	f=open(path,O_RDONLY|O_BINARY,S_IREAD);
	if(f==-1) return(-1);
	lth=read(f,buff, filelength(f));
	close(f);

	return(lth);
}
int32_t write_all_vfile( char *path, char *buff,uint32_t lth)
{
	int16_t f;

	f=open(path,O_WRONLY|O_BINARY|O_CREAT|O_TRUNC,S_IWRITE);
	if(f==-1) return(-1);
	lth=write(f,buff,lth);
	close(f);

	return(lth);
}

int32_t read_all_vtab( int16_t tabn, char *buff)
{
	int f;
	int32_t lth;

	if (vocs_NOK)
	return -1;

	f=TBOPEN((int16_t)tabn,(int16_t)language,(int16_t)(O_RDONLY|O_BINARY),(int16_t)(S_IREAD));

	if(f==-1)
	{
		vocs_NOK = vocs_NOK | (1 << tabn);
		return(-1);
	}

	lth=TGREAD((int16_t)(f), buff, (int32_t)(TGFILELTH((int16_t)f)));
	TGCLOSE((int16_t)f);

	return(lth);
}

#endif  /* WATCOM */

/***********************************************************************/
/*                                                                     */
/*              routine ed_conv is a general spelling loop             */
/*                                                                     */
/***********************************************************************/

int16_t perebor_threshold;
uchar * tempword;
#define MAX_PART_VARS 36
int16_t part_var_counter;

int16_t check_cancel( uint16_t);
static int16_t obj_number, sp_n;
//////////////////////////////////////////////////////////////////////////
int16_t ed_conv(struct dict_state * dict, user_voc voc_array[], int16_t voc_no)
/* extern insted of static -- from Mike */
{
	/* basic structure of the whole object being considered: */
	extern SOBJ obj;
	/* fundamental parts of the object: */
	extern SPART part[MAX_PARTS];
	/* best words within object fundamental parts (mixed buf for parts):  */
	static uchar word[MAX_WORDS * sizeof(SWORD)];

	extern LTIMG wrdimg[MAX_WORD_SIZE];
	extern LTIMG * wrddef[MAX_WORD_SIZE];

	int16_t ret; /* return flag: check=proc results       */
	//int16_t sperc;

	/* dbg_f=fopen("dbg.dbg","a");
	 fprintf(dbg_f,"ed_conv entered\n");
	 fclose(dbg_f);
	 */
#ifdef V_RECOG
	back_flag=1; /* back_recog required                  */
#endif

#ifdef SECOND_PASS
	rst_last=0;
	memset(ast,0,sizeof(ast));
#endif

	obj.nmb = 0;

	/************************ F I R S T    P A S S  ****************************/

	obj_number = 0;
	//sperc = (int16_t)progress_set_step (total_words);
	while (readobj(S_NEWOBJ, &obj)) {
		int16_t bpart, epart;
		obj_number++;

		//if (obj_number >= sperc)  sperc = progress_set_percent (obj_number);
		genobj(&obj); /* set object init state via its positions    */

		if (multy_language && language == LANG_RUSSIAN)
			if ((obj.type & (T_CAP | T_LOW)) == 0)
				continue;
		/*  printf ( "Obj # %d\n", obj.nmb ); */

		TPR(genobj)
		if (checkobj(&obj) == No) /* object rejected */
			continue;

		obj.word = (SWORD *) &word[0];
		tempword = (puchar) word + sizeof(word) - sizeof(SWORD);
		TPR(chkobj=yes)
		/*Start_part:*/
		sp_n = 0;
		part_var_counter = 0;

		for (bpart = 0; bpart <= obj.pos_part_nmb; bpart++) {
			obj.part_beg = obj.pos_part[bpart];
			obj.part_begi = bpart;

			for (epart = obj.pos_part_nmb; epart > bpart; epart--) {
				if (++part_var_counter >= MAX_PART_VARS)
					goto Select;

				perebor_threshold = 0;
				obj.part_end = obj.pos_part[epart];
				obj.part_endi = epart;

				obj.type_art &= ~(T_REPL | T_GC | T_BRK);
				obj.type_orig = 0;

				if (!(setpart(&obj, part)))
					continue;

				TPR(setp=yes)
				obj.word->next = NULL;
				obj.allowedlth = 0;
				/* ---------------- start words generation for a given part ---------------- */
				Start_wrd_gen: obj.nmb_wrdfound = 0;

				if (checkpart(&obj) == No) /* part rejected                          */
					goto No_part;

				TPR(chkpart=yes)
				/* ----- dict (static) : ----- */
				setimg(&obj, wrdimg); /* set part image                         */
				TPR(setimg)
				setdef(wrdimg, wrddef, &(obj.lthok)); /* set part wrddef               */
				TPR(setdef)
				obj.voc_kind = 2; /* dict is currently being used           */

				if (anal_part_wrd(&obj, wrddef, dict, voc_array, voc_no) == No)
					goto SetBlue;

				TPR(analpart=yes)
				sp_n++;
				ret = selectpart(&obj); /* have we gathered smth worth ?    */
				TPR(selectpart=?)

				if (ret == Ok) /* Yes, dynamic spelling not needed */
					goto Ok_part;

				TPR(selectpart not ok)

				if (ret == No) { /* No, try part change & glue-cut: */
#ifdef PARTSPEC

					/*  if (obj.nmb_wrdfound)     if smth found in a regular way =>      */
					/*    goto No_part;           then don't touch it                    */

					if (partspec(&obj,part)) /* Try to apply special replacements    */
					/* like "m"->"rn", "rn"->"m", ...       */
					goto Start_wrd_gen; /* restart the changed part treatment   */
#endif

#ifdef GLUE_CUT
					/* ???? obj.part_max--; */
					/*remove the part & set once more(partspec changes!)*/

					if (!(obj.type_art & T_GC)) /* 1-st glue-cut of the part => :       */
					{
						if (!(setpart (&obj,part))) /*alloc new part buff                  */
						break; /* buff overflowed !!!                 */
						TPR(setpart glue == yes)
					}

					if (partgc(&obj,part)) /* Try to apply glue-cut of the part    */
					goto Start_wrd_gen; /* restart the changed part treatment   */

					TPR(partgc == no)

					if(!(obj.type_art&T_BRK))
					{
						if (!(setpart (&obj,part))) /*alloc new part buff                  */
						break; /* buff overflowed !!!                 */

						TPR(setpart partgc = yes)
					}

					if(!(obj.type_sp&T_BLANK))
					{
						if(partbrk(&obj,part))
						goto Start_wrd_gen;

						TPR(partbrk = no)
					}

#endif
					No_part: TPR(NO part)
					obj.part_max--; /* remove the part:             */
					continue;
				} else
					/* Ok - select-part :              */
					Ok_part: {
						TPR(OK part)

						if (obj.part_end == obj.pos_part[obj.pos_part_nmb]) /* last part ?  */
							if (ret = selectobj(&obj, bpart, part)) /* select best parts-chain,    */
								goto Out_obj;
						/*  ending with this part      */
						ret = Ok;
					}
				/* change part of complex obj and restart wrd generation : */
			} /* ==================== end of for loop ========================= */
		} /* ==================== end of for loop ========================= */
		Select: ret = selectopt(&obj, part);/*find opt parts-chain, no matter entire or not*/
		TPR(selectopt)

		if (ret == Ok) /* best part-chain found                            */
			Out_obj: {
				TPR(before outobj)
				outobj(&obj, part); /* modify ed, if any best alts differ from originals*/
TPR			(outobj done)
		}
#ifndef SECOND_PASS
		else
		{
			if (ret==No) /* no good words found */
			SetBlue:
			{
				TPR(setblue)
				setobj_blue (&obj); /* the object is to be blue (not reliable) */
				TPR(setblue done)
			}
		}
#endif

#ifdef SECOND_PASS
		TPR(pull to stat)
		pull_to_stat(ret,&obj,part,wrdimg,wrddef);
		TPR(pull to stat done)
#endif
	}
	/* ====================== end of readobj loop =========================== */
	ret=obj.nmb;
	/*dbg_f=fopen("dbg.dbg","a");
	 fprintf(dbg_f,"before 2nd pass\n");
	 fclose(dbg_f);
	 */
	/****************        S E C O N D     P A S S      **********************/

#ifdef  SECOND_PASS

	while(wstack.depth>0)
	{
		wstack.depth--;
		wstack.last--;
		if((*wstack.last)->voc) continue;/* do not consider word from dictionary*/
		SPQ.ns_segm=(*wstack.last)->segm;
		SPQ.ns_symb=(*wstack.last)->symb;
		obj.nmb=(*wstack.last)->nmb-1;
		readobj(S_OLDOBJ,&obj);
		stat_replacements(&obj);
		setobj_blue (&obj);
	}
#endif
	return (ret);
}

/*************************************************************************/
/* This procedure generates all posiible words of a part given,
 wrdimg & wrddef, as well.
 Best words are selected in the part->wrd->... list.
 A dictionary kind to be used (static, called "dict",
 or dynamic, called "voc") is specified by obj->voc_kind.

 A decision may be currently made to stop the part-words generation,
 if it is expected to be not worth.
 */
/*************************************************************************/
#define TEMP_PEREBOR_THRESHOLD 512
//////////////////////////////////////////////////////////////////////////////
int16_t anal_part_wrd(SOBJ * obj, LTIMG * wrddef[], struct dict_state * dict,
		user_voc voc_array[], int16_t voc_no) {
	int16_t found;

	obj->type_art &= ~T_ALTS;

	do {
		if (++perebor_threshold >= TEMP_PEREBOR_THRESHOLD)
			return (obj -> nmb_wrdfound) ? Stop : No;

		if (!setwrd(obj, wrddef))
			continue;

		/* Mike */
		/*  printf( "%s\n", obj->wordchar ); */

		if (!(genwrd(obj)))
			continue;

		switch (checkwrd(obj)) {
		case Stop:
			return Stop; /* no worth words expected more */

		case No:
			continue; /* generate next word           */
		}

		do {
			found = (obj->word->type & T_DELIM) ? check_compound(obj, wrddef,
					dict, voc_array, voc_no) : check_dict_voc(obj, wrddef,
					dict, voc_array, voc_no);
		} while ((!found) && wordchange(obj));

		if (obj -> allowedlth < obj -> lthok)
			obj -> allowedlth = obj -> lthok;

		if (selectwrd(obj, wrddef) == No)
			continue;

		obj->part->word = obj->word;

		if ((puchar) obj ->word < tempword)
			(++obj->word)->next = obj->part->word;
		else
			return Stop;
	} while (obj->type_art |= T_ALTS, nextwrd(obj, wrddef) != Stop);

	return (Ok);
}
///////////////////////////////////////////////////////////////////////////
#ifdef SECOND_PASS
/**************************************************************************/
/*    Function put_to_stat() puts one-part object into statistical        */
/*        vocabulary and word stack for spelling second pass.             */
/**************************************************************************/

int16_t pull_to_stat(int16_t status,SOBJ*obj,SPART *part,
		LTIMG wrdimg[],LTIMG *wrddef[])
{
	register int16_t i;
	int16_t lth;

	if(obj->pos_part_nmb!=1)
	return(No);

	if(wstack.depth==MAX_WSTACK_DTH)
	error_exit(ERR_voc,MEM_ERROR);

	lth= ( status == Ok ) ?
	set_img_for_found(obj,part,wrdimg)
	:
	set_img_for_notfound(obj,part,wrdimg);

	for(i=0;wrdimg[i].lt;i++)
	wrddef[i]=wrdimg+i;
	wrddef[i]=NULL;

	if(look_through_voc(&lth,wrddef,&wvoc,VOC_W,1,wstack.last)<0)
	error_exit(ERR_voc,MEM_ERROR);

	investigate_word(status,obj,wrddef,*wstack.last);

	if((wstack.depth)&&((*(wstack.last-1))->nmb==(obj->nmb-1)))
	set_context(*(wstack.last-1),*wstack.last);
	wstack.depth++;
	wstack.last++;
	return(Ok);

}
/**************************************************************************/
/*  Function investigate_word() sets up word token according to object    */
/*  and wrddef description.                                               */
/**************************************************************************/

void investigate_word(int16_t status,SOBJ *obj,LTIMG *wrddef[],
		WTOKEN *token)
{	register char fst_lt=1;
	token->nmb=obj->nmb;
	token->segm=obj->tif_ref.segm;
	token->symb=obj->tif_ref.symb;
	token->voc= ( status == No ) ? 0 : obj->part->word->voc_kind;
	token->r_cont=C_UNREC;
	token->l_cont= (obj->type & T_STMT ) ? C_STBEG : C_UNREC;
	token->type=0;
	if(*wrddef)
	do
	switch(symcode((*wrddef)->lt))
	{	case E_CP: if(fst_lt)
		token->type|=W_NAME;
		else
		token->type|=W_CAP;
		break;

		case E_OD : token->type|=W_LOW;
		break;

		case _WDV:
		case E_DL: token->type|=W_DELIM;
		break;

		case _BLK: token->type|=W_BAD;
		break;

		case _DIG: token->type|=W_DIG;
		break;

	}
	while(fst_lt=0,*(++wrddef));
	if(
			((token->type&W_LOW)&&(token->type&W_CAP))
			||
			((token->type&W_LOW)&&(token->type&W_DIG))
			||
			((token->type&W_CAP)&&(token->type&W_DIG))
	)
	{	token->type&=~(W_DIG|W_CAP|W_LOW);
		token->type|=W_MIXED;
	}
	if(token->type&W_CAP)
	token->type&=~W_NAME;
}
/*****************************************************************************/
/*  Function set_context(T1,T2) sets context flags between word 1 and 2      */
/*****************************************************************************/
void set_context(WTOKEN *left,WTOKEN *right)
{	uchar flags;
	if( right->l_cont & C_STBEG )
	left->r_cont=C_STEND;
	else
	{
		right->l_cont=left->r_cont=0;
		if( left->l_cont & C_STBEG)
		right->l_cont|=C_FIRST;
		flags=(left->type)&(~(W_DELIM|W_CHECK));
		right->l_cont|=flags;
		flags=(right->type)&(~(W_DELIM|W_CHECK));
		left->r_cont|=flags;

	}
}
/****************************************************************************/
/*           set_img_for found() sets up wrdimg for word found              */
/****************************************************************************/

int16_t set_img_for_found(SOBJ *obj,SPART *part,LTIMG wrdimg[])
{	register int16_t in_obj,in_part=0,in_word=0;
	int16_t in_art;
	for( in_obj= part->beg; in_obj < part->end; in_obj++,in_part++,in_word++)
	{	wrdimg[in_word].blank=0;
		wrdimg[in_word].lt=(LT *)&wrdimg[in_word].std;
		if ( in_art=part->art[in_part] )
		{	wrdimg[in_word].lt->code=artbase[in_art].obj1;
			wrdimg[in_word].lt->attr=EOLIST;

			if(artbase[in_art].sr2)
			{	in_obj++;
				in_part++;
			}
			if(artbase[in_art].obj2)
			{	in_word++;
				wrdimg[in_word].blank=0;
				wrdimg[in_word].lt=(LT *)&wrdimg[in_word].std;
				wrdimg[in_word].lt->code=artbase[in_art].obj2;
				wrdimg[in_word].lt->attr=EOLIST;
			}
		}
		else
		{
			wrdimg[in_word].lt->code=obj->pos[in_obj].alt[
			part->word->altn[in_word]].orig.code;
			wrdimg[in_word].lt->attr=obj->pos[in_obj].alt[
			part->word->altn[in_word]].orig.attr|EOLIST;

		}
	}
	wrdimg[in_word].lt=NULL;
	return in_word-1; /* It seems it's true... */
}
/****************************************************************************/
/* set_img_for_notfound() sets up wrdimg according original first           */
/*    alternatives. BLANK is treated as ordinal letter.                     */
/****************************************************************************/
int16_t set_img_for_notfound(SOBJ *obj,SPART *part,LTIMG wrdimg[])
{	register int16_t in_obj,in_word=0;

	for( in_obj= part->beg; in_obj < part->end; in_obj++,in_word++)
	{	wrdimg[in_word].blank=0;
		wrdimg[in_word].lt=(LT *)&obj->pos[in_obj].orig;
	}
	wrdimg[in_word].lt=NULL;
	return in_word-1;
}
/***************************************************************************/
/*  stat_replacements() looks for possibility to invoke replacements of    */
/* alternatives  occured over page.                                        */
/***************************************************************************/
int16_t stat_replacements(SOBJ *obj)
{	register int16_t in_obj,in_tab,in_pos;
	int16_t ret=No;
	for(in_obj=0;in_obj<obj->pos_part[obj->pos_part_nmb];in_obj++)
	for(in_tab=0; in_tab<rst_last;in_tab++)
	if( obj->pos[in_obj].orig.code==rst[in_tab].was)
	for(in_pos=1;in_pos<obj->pos[in_obj].alt_nmb;in_pos++)
	if(obj->pos[in_obj].alt[in_pos].orig.code==rst[in_tab].be)
	{	if(obj->pos[in_obj].orig.attr-
				obj->pos[in_obj].alt[in_pos].orig.attr<DIF_for_stat_repl)
		{	outpos_exch (obj,in_obj,in_pos);
			ret=Ok;
		}
		break;
	}
	return ret;
}
#endif
