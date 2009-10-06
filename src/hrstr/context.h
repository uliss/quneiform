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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "struct.h"
#include "lang.h"
#include "ligas.h"
#include "func.h"

#ifdef  MAIN_CONTEXT_MODULE
# define    CONT_EXTERN
#else
# define    CONT_EXTERN extern
#endif

/****** Global Variables: *************/
extern uchar language;
extern int16_t line_number;

/******* Context Internal Type Definitions: **********/
typedef cell * pCell;
typedef struct din_var {
	cell *C;
	uchar pos; // the version number
	uchar class; // the kind of symbol : big, small, digit, empty and undef
	uchar first;
	uchar res;
	int16_t est; // current estimation
	int16_t back; // refference to previous element
} dv;
/*######## Constants Defenitions : ############*/
#define APOSTROPH       0x27
#define NEG_HALF_SPACE  0x1e
#define POS_HALF_SPACE  0x1f
#define REGULAR_SPACE   0x20
/*** Words types : *****/
#define UINT            0x01
#define NUMBER          0x02
#define UPPER           0x04
#define LOWER           0x08
#define AFTERDOT        0x10
#define AFTERAPOSTROPHE 0x20
#define COMPLEX         0x40
#define AFTERDOTCOM     0x80
#define BAD_SYM                  1
#define ALPHANUM                 2
#define SPEC_SYM                 4
#define DOT_SPACE                8
#define SPEC_SYM_SLASH        0x10
#define NOT_EXPECT_DELIM       0x80
#define DELIM_WORD_IS_POSSIBLE 0x01
#define DELIM_DIG_IS_POSSIBLE  0x02
#define DELIM_REGULAR          0x04
#define REAL_WORD_DELIM        0x10
#define REAL_DIG_DELIM         0x20
#define BONUS       16        // bonus for probability
#define MONUS       50        // antibonus for probability
#define MONUS1I     20        // antibonus for 1-i-l case
#define BONUS01      6        // bonus for 0 and 1 for neigboring digits
#define BONUS_SLASH  30       // bonus or monus for SLASH being between
// two letters
#define MAX_VERS      8             // maximal number of alternatives
#define MAX_PROB      254           // maximal probability of letter
#define MIN_PROB      2             // minimal probability of letter
#define MAX_SYMB      80            // maximal admissible number of
// symbols in word
#define MAX_NUM_WORDS (MAX_SYMB/2)  // maximum admissible number of
// words in line
#undef COMPLEX_PROT                 /* Complex_word Protocol Flag */
#define COMP_NVAR     3             // Number of variants in Complex_word
/***** Complex_word --> Types of conserning symbols: *****/
#define L_BIG         0
#define L_SMALL       1
#define L_NUM         2
#define L_UNDEF       3
#define L_EMPTY       4
/******* Complex_word --> Penalties types: ************/
#define JMP          35      // Jump penalty (was 25)
#define JMP_SN        8      // Additional jump penalty
// from small to numeric
#define ADD_JMP_SN    12     // One more additional jump penalty from small
// to numeric, if small is single version
#define NUM_INC       5      // Increase numeric power (was 10)
#define SMALL_INC     3      // Increase small power
/******  Context Penalties:  **********/
#define REFUSE_BOUND     66      // Refuse boundary for version selection
#define ADM_PROB         120
#define CONS_VOWEL_JMP   50    // penalty for conson. letter after 'l' and
// for vowel letter after 'I'
#define TITLE_JMP        25      // after dot small letter penalty
#define BON_DIG_CHAR     50
#define FRANCH_ART_BONUS   30
#define GER_DOUBLE_S_MONUS 40
#define SWED_LJU_BONUS     50
/********  Irish Name Parsing **********************/
#define  NO_IRISH_NAME      0
#define  CAPITAL_M         'M'
#define  LOWER_A           'a'
#define  LOWER_C           'c'
#define  AFTER_C            1
/****** Numeral object Processing Variants *********/
#define VAR_1st      1
#define VAR_2nd      2
#define VAR_3rd      4
#define VAR_09th     8
#define CONTINUE_ACTION     0
#define NO_ACTION           1
/***** Common variables and functions **********/
CONT_EXTERN uchar word_flag;
CONT_EXTERN int16_t n_ltr; // number of letters in word
CONT_EXTERN int16_t total_words;
CONT_EXTERN int16_t roman_sym_num;
CONT_EXTERN uchar swed_lju_flag;
CONT_EXTERN uchar fl_beg_irish_name;

void vers_to_first_place(cell *A, int16_t nver);
int16_t get_nvers(cell *A, int16_t Let);
int16_t check_numb_2_9(uchar);
int16_t check_numb_0_9(uchar);
int16_t check_upper(uchar);
int16_t check_lower(uchar);
int16_t check_alphanum(uchar);
int16_t check_lowerconson(uchar);
void test_O_and_slash();
int16_t english_context_process(cell* C);
int16_t franch_context_process(cell* C);
int16_t german_context_process(cell* C);
int16_t swedish_context_process(cell* C);
int16_t spanish_context_process(cell* C);
void test_roma_num_sym(cell *C);
void test_irish_name_cur_sym(int16_t Let);
/*========================================================*/

#ifdef MAIN_CONTEXT_MODULE
static int16_t find_dig_chr_var ();
static void make_dig_chr_chain ();
static void up_substr ( pchar substr );
static void up_rest_str ();
static int16_t get_est_substr_in_word ( pchar );
static int16_t est_rest_dig ( dv *);
static int16_t test_var ( pchar );
/*****************/
static dv * cdv, *last_dv, *last_dig_dv, *beg_dv;
static int16_t best_l, num_v, admvar = 0, maxest;
int16_t delim_flag, possible_delim_flag;
/***********************************************************************/

cell *WB; // first cell in word
cell *EB; // last cell in word
uchar after_word_flag;
uchar fl_cap_first; // first letter has capital version flag
int16_t num_dig, num_upper, num_lower, num_bad, num_let;
uchar irish_name_pos;
static pCell use_word_flags[MAX_NUM_WORDS];
static pCell pDotComCell;

static int16_t find_and_class_word (cell *);
static uchar classify (cell *);
static Bool find_delim ( int16_t );
static void set_after_word_flag ( int16_t );
static void set_word_flag ( int16_t );
static void check_vers (cell *);
static void set_word_type ();
static Bool real_delim ();
static void Livers (cell *);
static Bool Lbs_word ();
static void cut_lower_ver (cell *);
static Bool roma_num (void);
static Bool is_roma_num (void);
static void make_roma_num (void);
static int16_t best_estimation (void);
static void estimate_chain (int16_t);
static void estimate_letter (cell *, int16_t);
static int16_t class_of_letter (char);
static void setup_complex (void);
static int16_t make_best_choice (int16_t);
/*******************
 static int16_t is_cons_jmp          ( dv *prv, dv *crn );
 static int16_t is_vowel_jmp         ( dv *prv, dv *crn );
 ********************/
#endif
