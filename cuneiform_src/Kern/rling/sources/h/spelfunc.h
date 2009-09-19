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

int32_t read_all_file(char * name, char *buff); /* atal  .lib */
int32_t write_all_file(char * name, char *, uint32_t size);
/*** SPELABC  ***/

void initcode(void);
void initstdn(void);
uchar _2cap(uchar c);
uchar _2low(uchar c);
void dectable_init(void);
Bool InitializeAlphabet(int16_t CountryCode);
Bool IsVowel(uchar c);

/*** SPELSPEC ***/

uchar * load_specABC(uchar *point, int16_t Country);
int16_t check_art_dict(char word[], int16_t * wordlth, int16_t * vockind);
int16_t test_apppostrof(uchar word[], SWORD *wrd, int16_t *l, int16_t *r);

/*** SPELLFUN ***/

int16_t setwrd(SOBJ *, LTIMG **);
int16_t setimg(SOBJ *, LTIMG *);
int16_t setdef(LTIMG *, LTIMG **, int16_t *);
int16_t nextwrd(SOBJ *, LTIMG **);
int16_t check_compound(SOBJ *, LTIMG **, struct dict_state *, user_voc *, int16_t);
int16_t check_dict_voc(SOBJ *, LTIMG **, struct dict_state *, user_voc *, int16_t);
int16_t prtobj(SOBJ *, int16_t, int16_t);
int16_t prtwrd(SOBJ *, SWORD *, int16_t, int16_t);
int16_t symcode(char *);
int16_t findpart(SOBJ*, SPART[], int16_t, int16_t, int16_t *);
int16_t findparte(SPART *, int16_t, int16_t, int16_t*);
int16_t getstdn(char *);

/*** SPELLCHK ***/

int16_t selectopt(SOBJ *, SPART[]);
int16_t checkpart(SOBJ*);
int16_t checkobj(SOBJ *);
int16_t checkwrd(SOBJ *);
int16_t selectwrd(SOBJ *, LTIMG **);
int16_t selectpart(SOBJ *);
int16_t selectobj(SOBJ *, int16_t, SPART[]);
int16_t CheckContext(SOBJ *obj, LTIMG *wrddef[], int16_t lth);

/*** SPELLART ***/

int16_t wordchange(SOBJ*);
int16_t partspec(SOBJ *, SPART[]);
int16_t partgc(SOBJ *, SPART[]);
int16_t partbrk(SOBJ*, SPART[]);
int16_t init_specpos(SOBJ*);
Bool loadArtBase(int16_t);

/*** SPELLSET ***/

int16_t genwrd(SOBJ*);
int16_t readobj(int16_t status, SOBJ *);
void genobj(SOBJ *);
int16_t setpart(SOBJ *, SPART[]);
int16_t genpart(SOBJ *);

/*** SPELLOUT ***/

char * find_byte_flag(void);
int16_t outobj(SOBJ *, SPART *);
int16_t setobj_blue(SOBJ *);
int16_t setpart_blue(SOBJ *obj, int16_t beg, int16_t end);
int16_t outpos_exch(SOBJ * obj, int16_t pos, int16_t anew);

/*** STATDICT ***/

int16_t search(KEYTYPE *word, int16_t *wordsize, LTIMG * wrddef[],
		struct dict_state * dict);

/*** EDFORMAT ***/

int16_t begin(uchar batch_run);
struct segm * next_line(char tt, struct segm *cur_segm);
void skip_letter_in_line(struct segm *segm_ptr, int16_t x);
void shift(char dir, int16_t v_s, struct segm *cur_segm, char *cur_symb);
int16_t test_spare_space(struct segm * segm, int16_t need_size);
int16_t load_fragm(void);
int16_t insert_symb(struct segm *, char *, LT *);
int16_t processEdSymb(void);
void puff_last_segm(void);

/*** NEXT_SYM ***/

uchar * next_symb(int16_t i, int16_t j, int16_t k, struct segm *cur_segm, uchar * cur_symb);
void create_new_stack(void);
int16_t check_free_mem(void);

/*** FINDWORD ***/

int16_t findstat(int16_t *, LTIMG **, struct dict_state *);

/*** FINDVOC  ***/

int16_t look_through_voc(int16_t * currw, LTIMG * wrddef[], voc_state * voc,
		int16_t vocmode, int16_t incr, WTOKEN * * wtoken);

/***   VOC    ***/

int16_t voc_init(voc_state*);
void voc_open(voc_state*);
int16_t voc_(voc_state *voc, LTIMG **wrddef, int16_t *lth, int16_t weight, int16_t mode,
		WTOKEN * *wtoken);

/*** IN_TREEI ***/

void init_stat_dict(struct dict_state *);/* in_treei.c */

// 08-13-93 08:59pm, Mike
// Second parameter has been removed.
uchar * load_stat_dict(char *point);

// 08-13-93 09:00pm, Mike
// Return type has been changed.
void load_user_dicts(char * list_name, char * point);

int32_t read_all_voc(int16_t seqn, char *name, char *p);

#ifdef SECOND_PASS

int16_t pull_to_stat(int16_t status,SOBJ*obj,SPART *part,
		LTIMG wrdimg[],LTIMG *wrddef[]);
void investigate_word(int16_t status,SOBJ *obj,LTIMG *wrddef[],
		WTOKEN *token);
void set_context(WTOKEN *left,WTOKEN *right);
int16_t set_img_for_found(SOBJ *obj,SPART *part,LTIMG wrdimg[]);
int16_t set_img_for_notfound(SOBJ *obj,SPART *part,LTIMG wrdimg[]);

int16_t stat_replacements(SOBJ *obj);

#endif

/*** IOED_TF ~~ IOED_TST  ***/
int16_t read_file(uchar batch_run);
int16_t save_ed_file(uchar batch_run);

#endif    //#ifndef __SPEL_FUNCS_H__
