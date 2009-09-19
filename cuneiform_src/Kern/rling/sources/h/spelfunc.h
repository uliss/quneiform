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

/****************************************************************************/
/*****                        S P E L L E R                             *****/
/*********~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*********/
/***************   F U N C T I O N S    P R O T O T Y P E S  ****************/
/****************************************************************************/
#ifndef __SPEL_FUNCS_H__
#define __SPEL_FUNCS_H__



#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/*** TG_SPELL ***/

LONG read_all_file(char * name, char *buff); /* atal  .lib */
LONG write_all_file(char * name, char *, uint32_t size);
/*** SPELABC  ***/

void initcode(void);
void initstdn(void);
uchar _2cap(uchar c);
uchar _2low(uchar c);
void dectable_init(void);
Bool InitializeAlphabet(INT CountryCode);
Bool IsVowel(uchar c);

/*** SPELSPEC ***/

uchar * load_specABC(uchar *point, INT Country);
INT check_art_dict(char word[], INT * wordlth, INT * vockind);
INT test_apppostrof(uchar word[], SWORD *wrd, INT *l, INT *r);

/*** SPELLFUN ***/

INT setwrd(SOBJ *, LTIMG **);
INT setimg(SOBJ *, LTIMG *);
INT setdef(LTIMG *, LTIMG **, INT *);
INT nextwrd(SOBJ *, LTIMG **);
INT check_compound(SOBJ *, LTIMG **, struct dict_state *, user_voc *, INT);
INT check_dict_voc(SOBJ *, LTIMG **, struct dict_state *, user_voc *, INT);
INT prtobj(SOBJ *, INT, INT);
INT prtwrd(SOBJ *, SWORD *, INT, INT);
INT symcode(char *);
INT findpart(SOBJ*, SPART[], INT, INT, INT *);
INT findparte(SPART *, INT, INT, INT*);
INT getstdn(char *);

/*** SPELLCHK ***/

INT selectopt(SOBJ *, SPART[]);
INT checkpart(SOBJ*);
INT checkobj(SOBJ *);
INT checkwrd(SOBJ *);
INT selectwrd(SOBJ *, LTIMG **);
INT selectpart(SOBJ *);
INT selectobj(SOBJ *, INT, SPART[]);
INT CheckContext(SOBJ *obj, LTIMG *wrddef[], INT lth);

/*** SPELLART ***/

INT wordchange(SOBJ*);
INT partspec(SOBJ *, SPART[]);
INT partgc(SOBJ *, SPART[]);
INT partbrk(SOBJ*, SPART[]);
INT init_specpos(SOBJ*);
Bool loadArtBase(INT);

/*** SPELLSET ***/

INT genwrd(SOBJ*);
INT readobj(INT status, SOBJ *);
void genobj(SOBJ *);
INT setpart(SOBJ *, SPART[]);
INT genpart(SOBJ *);

/*** SPELLOUT ***/

char * find_byte_flag(void);
INT outobj(SOBJ *, SPART *);
INT setobj_blue(SOBJ *);
INT setpart_blue(SOBJ *obj, INT beg, INT end);
INT outpos_exch(SOBJ * obj, INT pos, INT anew);

/*** STATDICT ***/

INT search(KEYTYPE *word, INT *wordsize, LTIMG * wrddef[],
		struct dict_state * dict);

/*** EDFORMAT ***/

INT begin(uchar batch_run);
struct segm * next_line(char tt, struct segm *cur_segm);
void skip_letter_in_line(struct segm *segm_ptr, INT x);
void shift(char dir, INT v_s, struct segm *cur_segm, char *cur_symb);
INT test_spare_space(struct segm * segm, INT need_size);
INT load_fragm(void);
INT insert_symb(struct segm *, char *, LT *);
INT processEdSymb(void);
void puff_last_segm(void);

/*** NEXT_SYM ***/

uchar * next_symb(INT i, INT j, INT k, struct segm *cur_segm, uchar * cur_symb);
void create_new_stack(void);
INT check_free_mem(void);

/*** FINDWORD ***/

INT findstat(INT *, LTIMG **, struct dict_state *);

/*** FINDVOC  ***/

INT look_through_voc(INT * currw, LTIMG * wrddef[], voc_state * voc,
		INT vocmode, INT incr, WTOKEN * * wtoken);

/***   VOC    ***/

INT voc_init(voc_state*);
void voc_open(voc_state*);
INT voc_(voc_state *voc, LTIMG **wrddef, INT *lth, INT weight, INT mode,
		WTOKEN * *wtoken);

/*** IN_TREEI ***/

void init_stat_dict(struct dict_state *);/* in_treei.c */

// 08-13-93 08:59pm, Mike
// Second parameter has been removed.
uchar * load_stat_dict(char *point);

// 08-13-93 09:00pm, Mike
// Return type has been changed.
void load_user_dicts(char * list_name, char * point);

LONG read_all_voc(INT seqn, char *name, char *p);

#ifdef SECOND_PASS

INT pull_to_stat(INT status,SOBJ*obj,SPART *part,
		LTIMG wrdimg[],LTIMG *wrddef[]);
void investigate_word(INT status,SOBJ *obj,LTIMG *wrddef[],
		WTOKEN *token);
void set_context(WTOKEN *left,WTOKEN *right);
INT set_img_for_found(SOBJ *obj,SPART *part,LTIMG wrdimg[]);
INT set_img_for_notfound(SOBJ *obj,SPART *part,LTIMG wrdimg[]);

INT stat_replacements(SOBJ *obj);

#endif

/*** IOED_TF ~~ IOED_TST  ***/
INT read_file(uchar batch_run);
INT save_ed_file(uchar batch_run);

#endif    //#ifndef __SPEL_FUNCS_H__
