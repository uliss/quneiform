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

// StatSearchBL.h : header file
//
#ifndef __STAT_SEARCH_BL_H__

#include "cttypes.h"
#include "cstr.h"

#ifdef __cplusplus
extern "C" {
#endif

///////
typedef struct tagROW_STRUCT {
	int16_t globMax; //глобальный максимум
	int16_t ValGM; //значение глобального максимума
	int16_t AltGM; //значение альтернативы (1-я или 2-я БЛ)

	int16_t rightLocMax;
	int16_t ValRLM;
	int16_t AltRLM;
	float rightCoeff;

	int16_t leftLocMax;
	int16_t ValLLM;
	int16_t AltLLM;
	float leftCoeff;

} ROW_STRUCT;

//////
int16_t stat_Mode_diff_b2_b3(int32_t line_number, int16_t diff,
		int16_t fl_b2_or_b1);
int16_t stat_Mode_diff_b2_b1(int32_t line_number, int16_t diff_b3_b2);
int16_t stat_Mode_diff_b3_b4(int32_t line_number, int16_t diff_b3_b2);
//////

//////
int16_t stat_Hypothesis(int32_t line_number, int16_t diff_b3_bsl);
//////

//////
int16_t stat_Right_diff_from_max(int16_t *p_FreqArr, int16_t max_FreqArr);
int16_t stat_Left_diff_from_max(int16_t *p_FreqArr, int16_t max_FreqArr);
//////

//////
void stat_def_b3_b4(CSTR_line line, CSTR_attr *p_attr, int16_t *pY2Arr,
		int16_t *pFreqY2Arr, int16_t *Ns3, int16_t *Ns4);
void stat_def_b1_b2(CSTR_line line, CSTR_attr *p_attr, int16_t *pY1Arr,
		int16_t *pY2Arr, int16_t *pFreqY1Arr, int16_t *Ns1, int16_t *Ns2);
void stat_def_imaginary_bl(CSTR_attr *p_attr, int16_t *Ns1, int16_t *Ns2,
		int16_t *Ns4);
//////

//////
Bool stat_interp_bsl(CSTR_line line, CSTR_attr *p_attr, ROW_STRUCT *row_str);
Bool stat_interp_bsl_by_solid_recog(CSTR_line line, int16_t ln_row, int16_t b3,
		ROW_STRUCT *row_str);
Bool stat_interp_bsl_by_b4(CSTR_line line, int16_t ln_row, int16_t b3,
		int16_t b4, ROW_STRUCT *row_str);
Bool stat_interp_bsl_by_boxes(CSTR_line line, int16_t ln_row, int16_t b3,
		int16_t b4, ROW_STRUCT *row_str);
//////

//////
Bool stat_control_t_level(CSTR_line line, int16_t ln_row, int16_t b3,
		int16_t b4);
int16_t stat_control_b3_by_recog(CSTR_line line, int16_t ln_row,
		ROW_STRUCT *row_str);
Bool stat_control_semilevel(CSTR_line line, int16_t ln_row, int16_t b4);
//////

//////
void stat_FormArrays(CSTR_line line, int16_t row, int16_t *pY1Arr,
		int16_t *pY2Arr);
Bool stat_FormCSTR_attrArray(int32_t line_start, int32_t line_stop,
		CSTR_attr *pCSTR_attrArr);
int16_t stat_LineLength(CSTR_line line);
//////

//////
void StatSearchBL(CSTR_line line, CSTR_attr *p_attr, int16_t *Ns1,
		int16_t *Ns2, int16_t *Ns3, int16_t *Ns4);
//////

#ifdef __cplusplus
}
#endif

#define	 __STAT_SEARCH_BL_H__
#endif

