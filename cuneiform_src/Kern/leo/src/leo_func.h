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

#ifndef __LEO_FUNC_H__

#define __LEO_FUNC_H__
// functions from module leo.c
void    leo_kill(RecVersions *v,uchar *kill_list);
void    leo_reduce_typefaces(RecVersions *v);
void    leo_clear( RecVersions *v);
int     leo_sort_vers_prob(RecVersions *v);
int     leo_sort_vers_info(RecVersions *v);
void    leo_compress( RecVersions *v);
void    leo_decode_to_ansi( RecVersions *v);
void    leo_decode_to_acsii( RecVersions *v);
void    LeoAddPVers(int Probs[],RecVersions *v);
void    leoProbsToVer_expert(int Probs[],RecVersions *v,RecVersions *mod);
void    LeoProbsToVer(int Probs[],RecVersions *v);
Bool32  leo_add_vers(RecVersions *dst, RecVersions *src);
int     leo_exist_code(RecVersions *ver, uchar code);
void    leo_set_max_vers(RecVersions *v,int p);
int     leo_comp_codes(uchar c1, uchar c2);
uchar * leo_strchr_codes(uchar *c1, uchar c2);
uchar * leo_strchr_codes_ansi(uchar *c1, uchar c2);
void    leo_over( RecVersions *ver, RecVersions *alph);
int     leo_exist_codes(RecVersions *ver, uchar *codes);
void    add_to_prob_array(uchar prob[],RecVersions *v);
void    VersToAlph(RecVersions *ver,uchar alphabet0[]);
void    leo_expert_prob( RecVersions *v,uint16_t *v3x5, RecRaster *r ,
                         uchar prob_3x5[], uchar prob_iva[], int32_t prn,
                         Bool32 bonus, Bool32 enable5x3);

/// functions from module leo_stick
int     leo_get_global_incline(void);
void    leo_set_simple_sticks_group(RecVersions *v,unsigned char al[],int32_t nose);
Bool32  leo_small_object(RecObject *object,int wlim, int hlim);
int32_t   leo_wide_object(RecObject *object);
Bool32  leo_big_object(RecObject *object,int wlim, int hlim);
Bool32  leoRecogSimpleStick( RecObject*  object );
void    leo_add_inc(RecObject*  object);
void    leo_clear_stat(void);
int32_t   leo_stick_horiz_hist(int h);
Bool32  leo_is_stick(RecObject*  object);
void    leo_setup_inc(RecObject*  object);
void    leo_erect(RecObject* object,int inc);
void    leo_set_sticks_group(   RecVersions *v, int32_t ret,
                                unsigned char alph[], int32_t simple_st, int32_t nose_1);
int32_t   leo_recog_stick(uint16_t *lpool, int w, int h);
void    leo_diskrim_stick(RecVersions *ver);

/// functions from module leo_cont
void    leo_close_cont(void);
Bool32  leo_cont_new_page(int32_t id_page);
int16_t   leo_cont_store(RecRaster *r,uchar let, uchar nLns,Rect16 *rect, uchar IsPrn,
		uchar Prob, uchar Valid, RecVersions *ver,uchar control);
Bool32  leo_cont_restore(RecRaster *r, int32_t id_page, int16_t id_rast, uchar *nLns,Rect16 *rect, uchar *IsPrn,uchar *datao);
void    leo_close_cont_temp(void);
int     leo_open_cont_temp(void);
Bool32  leo_cont_set_valid(int32_t id_page, int16_t id_rast,uchar code, uchar valid,uchar control);
Bool32  leo_cont_del_final(int32_t id_page, int16_t id_rast);
Bool32  leo_cont_store_collection(RecVersions *ver);

/// functions from module leo_prn
Bool32  leoRecogPrintChar( RecObject*  object );
Bool32 leoRecogCharPRN_expert(RecRaster*  recRaster, RecVersions* over);


// functions from module LEO_DLL.C
void    leo_snapChar(RecVersions *ver, char *tit, int enable);
void    leo_snapChar3x5(RecVersions *ver, char *tit, int enable);
void    leo_snapRes2Str(RecVersions *ver, char *buf);
Bool32  leoSnpInRect( Rect16* pRect, int32_t nSkew );

void    leo_cuneiform(RecObject *rec);
void    leo_SnpLog(char *tmp);
void    leo_snapRaster(RecObject* object, SnpTreeNode *stnRecog);
void    leo_snapSimpleKey(char *str, SnpTreeNode *stnRecog);
Bool32  leo_init_prn(void);

// function from module LEO_SIZE.C
uchar leo_proport_penalty(uchar let,uchar prob, int32_t w, int32_t h);
uchar leo_sizes_penalty( int32_t w, int32_t h);
void  leo_sizes_init_stat(void);
#endif
