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

/********************************************************************/
/*                                                                  */
/*                common functions prototypes                       */
/*                                                                  */
/********************************************************************/

#ifndef KERNEL_H_INCLUDE
#define KERNEL_H_INCLUDE

#include "struct.h"
#include "extract.h"
#include "stick.h"

extern uchar *let_linpos, *let_lindef, *let_lincomp, *let_linshape,
		*let_sans_acc, *let_lindef3, *accent_tab;

// module diffrv.c
void r_criteria(cell *c, const s_glue * gl);

// module prscan.c
int16_t brigt(int16_t, int16_t, int16_t, int16_t);

// module names.c
pchar full_name(pchar w, pchar n);

// module kernel.c
void correct_dir_path(puchar p);

// module proimage.c
void image_file_close();
Bool image_file_open();
int16_t source_read(puchar start, puchar ptr, puchar end);

// module progress.c
void set_percentage(void);
void progress_next_step(void);
uint32_t progress_set_step(uint32_t volume);
uint32_t progress_set_percent(uint32_t volume);
void progress_finish(void);

// module matrix.c
void matrix_open();
void matrix_close();
int16_t matrix_read(puchar buff, uint16_t lth);

// module rules.c
void rules_open();
int16_t rules(MN *);
int16_t frame(MN *);
void rules_closed();
void short_undln();
void del_root(int16_t, int16_t, int16_t, int16_t);

// module rulsh.c
int32_t rules_shaving(MN *, int16_t);
int16_t shaving_let();

// module rulbr.c
void cutoffs();

// module sweeper.c
void sweeper_ini();
uint16_t sweeper(int16_t);
void sweeper_end();
void read_comp(uint32_t, pchar, int32_t);
void comp_file_close();
int16_t delgarb();
void del_shav();

// module skew.c
void skew_corr(str *, int16_t);
void skew_end();
void ideal_rc(cell *);

// module pass3.c
void pass3(int16_t nCall);
void accept_cell(cell *, c_comp *);
int16_t revision();
// module alphabet.c
Bool is_digital_string(void);
void serbian_J2j(void);

// module snap.c
Bool snap_init();
Bool snap_finish();
Bool snap_newline();
Bool snap_newpass(uchar);
Bool snap_show_text(puchar);
Bool snap_show_text1(puchar);
Bool snap_monitor();
Bool snap_newcell(cell *);
Bool snap_stopcell(cell *, cell *);
Bool snap_activity(uchar);
Bool snap_show_raster(puchar, int16_t, int16_t);
Bool Alik_snap_show_raster(puchar, puchar, puchar, int16_t, int16_t, pchar, pchar, pint16_t);
void snap_newcut();
void snap_show_cuts(cell *C, struct cut_elm *cut_points);
void snap_newpoint(int16_t i);
void snap_stop_cuts();

// module prop.c
void prop(int16_t);
int16_t prop_index(int16_t h, int16_t w);

// module diff.c
int16_t ij_dot(cell *);
void criteries();
void criteria(cell *);
cell *dot_ri(cell *);
cell *dot_ij(cell *);
cell *dot_excl(cell *);
cell *dot_inv(cell *);
int16_t ij_test(cell *);
void v_to_y(cell * c, uchar compose);
void c_to_ctail(cell * c, uchar compose);

// module accents.c
void pass_accent(void);
int16_t bottom_accent(cell *c);
int16_t accent(cell *);
int16_t type_acc(cell *);
int16_t find_bottom_accent(cell *c);

// module findiff.c
void final_crit();

// module linpos.c
int16_t obtain_diff(int16_t);
int16_t def_locbas(cell *);
void inter_diff(cell *);
int16_t if_dust(cell *);
int16_t if_dust_size(cell *);
void linpos(str *);
void cell_by_base();
void cell_position(cell *);
int16_t levcut(cell *, int16_t);
void get_b_lines(cell *, B_LINES *);
int32_t get_size();
int16_t get_bsm(); //AK 04.03.97 ? to int32_t
int16_t is_defis(cell *);
void basedraft(str *);
void basefin(str *);
int16_t cut_by_pos(cell *C, uchar c, int16_t wp, uchar fl, uchar arg);

// module dmacc.c
int16_t try_cut_accent(cell *C1, B_LINES *my_bases, int16_t flag);
int16_t try_cut_top_accent(cell *C1, B_LINES *my_bases, int16_t flag);
int16_t try_cut_bot_accent(cell *C1, B_LINES *my_bases, int16_t flag);

// module dm1.c
void dm1();
int16_t discrid(cell *, int16_t);
int16_t decidust(cell *);
int16_t estcomp(char, cell *, SVERS *, int16_t, int16_t, int16_t, s_glue *, cut_pos*, cut_pos*,
		char, char, char);
void snBOX(cell *);
void est_snap(char user, cell *C, pchar txt);
void promote(uchar, cell *, uchar, int16_t);
void collect_pat(cell *, s_glue *, cell *);

// module dm2.c
void dm2();
void glsnap(char, cell *, pchar);

// module dms.c
void save_vers(cell *, SVERS *);
void min3(v_val *, uchar, v_val *);
void rest_vers(cell *, SVERS *);
int16_t signif_line_count(cell*);
int16_t compare_vect(v_val *, v_val *);
int16_t cell_is_BOX_solid(cell*);

// module locomp.asm
/*#define c_locomp(a, b, c, d, e)        EVN_CLocomp((a), (b), (c), (d), (e))*/
MN *c_locomp(puchar, int32_t, int32_t, int16_t, int16_t);

// module v0compgl.asm
void invert_tiff(puchar c, uint16_t lth);

// module cutgraph.c
int16_t make_graph();
void excl_connect();
void find_path();

// module cutpoint.c
int16_t cut_points(int16_t, int16_t, puchar, struct cut_elm *);
int16_t Alik_cut_points(int16_t, int16_t, puchar, struct cut_elm *, int16_t);
MN* cut_rast(puchar, int16_t, int16_t, int16_t, int16_t, struct cut_elm *, char, char, pchar,
		cut_pos*);

// module alcutfun.c
void Alik_def_cut_flg(cell *, struct cut_elm *, struct cut_elm *, int16_t, uchar);

// module glue.c
MN *glue(c_comp **, int16_t);

// module dmBOX.c
int16_t dmBOX(cell *, s_glue *);
int16_t dmiBOX(cell *, s_glue *, int16_t);
servBOX *make_broken_raster(s_glue *, int16_t);
int16_t crecell(cell *, s_glue *, int16_t);
int16_t crepat(cell *, s_glue *, int16_t, int16_t);
void SBOX_to_static();
void SBOX_to_GL(s_glue *);
void GL_to_SBOX(s_glue *);
void static_to_SBOX();
void dens_to_cell(cell *);
void static_to_cell(cell *);

// module embBOX.c
void embBOX(servBOX *, SVERS *, Bool);
void embBOXF(servBOX *, int16_t, Bool);

// module scalar.asm
int16_t proport(uint16_t, uint16_t, uint16_t, int16_t, int16_t);
extern uint16_t (*scalarf)(PWORD, PWORD, uint16_t);
extern int32_t (*scalar)(PWORD, PWORD);
int16_t long_sqrt(uint32_t);

// module funcBOX.asm
void comp_to_box(PWORD, c_comp *, uint16_t, uint16_t, uint16_t, uint16_t);

// module abris.c
void abris_reset();
int16_t abris(s_glue *, cell *, uchar, int16_t);
int16_t h_filt(cell *, int16_t, s_glue *, uchar Let);
int16_t rtf(cell *cl, int16_t prob, s_glue *GL);
void apply_monus(cell *);

// module perc.c
void perc();
void number();

// module atlant.c
void atlant(cell *, s_glue *, int16_t, int16_t);

// module ltmain.c
void LayoutPart1(void);
void LayoutPart2(void);

// module ltpage.c
void SpecCompPut(MN *mn);

// module spec_sym.c
void star();
void bullet();
int16_t chkbullet(uchar);
Bool abris_curve(uchar fun[], int16_t n, int16_t w);
int16_t find_minimum(uchar fun[], int16_t n, uchar *_imin);

// module S_TOOLS.C
void new_vers(cell *c, uchar vers, uchar prob);
void down_all_versions(cell *c, int16_t monus);
void filtr_shave(uchar fun[], int16_t len);
Bool set_prob(cell *c, char let, uchar prob);
Bool check_let(cell *c, char let);
void add_stick_vers(cell *c, char let, uchar prob);
void filtr_short(uchar fun[], int16_t n, int16_t lev);
void filtr121(uchar fun[], int16_t n);
void filtr_bullet(uchar fun[], int16_t len);
void add_thick_stick_versions(cell *c, int16_t dx, int16_t wide, int16_t inc, int16_t typ_snap,
		STICK_CHARS *left_chars, STICK_CHARS *right_chars,
		STICK_SIGNUMS *signums);
void add_thin_stick_versions(cell *c, uchar left[], uchar right[], int16_t dy,
		int16_t dx, int16_t typ_inc, STICK_CHARS *left_chars, STICK_CHARS *right_chars,
		STICK_SIGNUMS *signums);
Bool sign_neck(uchar left[], uchar right[], int16_t n, int16_t base, int16_t mode);
int16_t conv_prob(int16_t prob);
int16_t shift_raster(uchar *r, int16_t dy, int16_t dx, int16_t tab[], int16_t max_shift, uchar *res,
		int16_t dir);
/******************************
 void full_snap(cell *c,STICK_CHARS *l,STICK_CHARS *r,STICK_SIGNUMS *s,
 int16_t tab_angle[],
 int16_t left_mode,int16_t right_mode,int16_t opt,int16_t wide,
 int16_t inc_v,int16_t t);
 void short_snap(char *s,int16_t t);
 *******************************/

// module dis_stic.c
int16_t discrim_all_sticks(cell *c, STICK_CHARS *left_chars,
		STICK_CHARS *right_chars, STICK_SIGNUMS *signums);
int16_t discrim_stick(uchar, STICK_CHARS *, STICK_CHARS *, STICK_SIGNUMS *, int16_t);
int16_t del_sticks(cell *c, char let);
int16_t similar_0xBA(STICK_CHARS *, STICK_CHARS *, STICK_SIGNUMS *);
int16_t similar_0xBB(STICK_CHARS *l, STICK_CHARS *r, STICK_SIGNUMS *s);
int16_t similar_1(STICK_CHARS *, STICK_CHARS *, STICK_SIGNUMS *);
int16_t similar_l_stroked(STICK_CHARS *, STICK_CHARS *, STICK_SIGNUMS *);
int16_t similar_lcb(STICK_CHARS *, STICK_CHARS *, STICK_SIGNUMS *);
int16_t similar_rcb(STICK_CHARS *, STICK_CHARS *, STICK_SIGNUMS *);
int16_t similar_excl(STICK_CHARS *, STICK_CHARS *, STICK_SIGNUMS *);
int16_t similar_excl_with_dot(STICK_CHARS *, STICK_CHARS *, STICK_SIGNUMS *);
uchar similar_wide_frt1(STICK_CHARS *left_chars, STICK_CHARS *right_chars);

// module def_stic.c
int16_t stick_center_study(cell *c, s_glue *GL, int16_t typ);
uint16_t typ_thin_stick(lnhead *lin, int16_t dy, int16_t dx);
int16_t erector(cell *c, int16_t enable_erect, int16_t enable_shave, int16_t enable_snap);

// module cha_stic.c
int16_t set_stick_char(uchar left[], uchar right[], int16_t tails[], int16_t dy, int16_t dx,
		int16_t opt, int16_t wid, int16_t corr_mode, int16_t skip_ul, int16_t skip_dl, int16_t skip_ur,
		int16_t skip_dr, int16_t inc, STICK_CHARS *left_chars,
		STICK_CHARS *right_chars, STICK_SIGNUMS *signums, int16_t *l_mode,
		int16_t *r_mode);
int16_t calc_right_f_symptom(uchar fun[], int16_t n, int16_t m);
int16_t calc_left_f_symptom(uchar fun[], int16_t n, int16_t nx, Bool c_ft);
void num_of_skip_line(uchar left[], uchar right[], int16_t dy, int16_t opt, int16_t wide,
		int16_t *up_l, int16_t *up_r, int16_t *down_l, int16_t *down_r);

void set_stick_aux_signums(cell *c, uchar left[], uchar right[], int16_t dy,
		int16_t wide_up, STICK_CHARS *left_chars, STICK_CHARS *right_chars,
		STICK_SIGNUMS *signums, int16_t dis_left_brace, int16_t dis_right_brace,
		int16_t dis_slash, int16_t l_f_sym, int16_t r_f_sym, int16_t T_c, uint16_t T_2_3,
		uint16_t T_skok, B_LINES *bl, int16_t nlines, int16_t incline);

void
		calc_skipped_lines(int16_t hist_int[], uchar left[], uchar right[], int16_t dy,
				int16_t dx, int16_t wide, int16_t opt, Bool c_r, int16_t *ul, int16_t *ur, int16_t *dl,
				int16_t *dr);
void correct_f_symptoms(int16_t hist_int[], int16_t dy, int16_t *l_f_sym, int16_t *r_f_sym);
void correct_upper_peaks(STICK_CHARS *l, STICK_CHARS *r, int16_t dy);

// module differ.c
void differ();

// module bold.c
Bool boldshave(cell *, int16_t); //AK: at .ms as Bool
Bool pimpleshave(cell *C, int16_t shave, int16_t inc); //AK: at .ms as Bool


// module shaving.c
int16_t shaving(int16_t);

// module punct.c
void punct();
void clear_punct_twin_flags(void);
void clean_punct();
void clean_line();
void recover_dusts(void);
void hide_dusts(uchar reg);

// module il1peak.c
void extremum(pchar, int16_t, int16_t, int16_t, int16_t, char, char);

// module il1tg.c
void il1();

// module space.c
void space_size(int16_t);
void set_space_size(int16_t);
void space_cell();
void cont_space();
void showgap(cell *);
int16_t get_space_size(void);
int16_t get_gap(void);

// module context.c
void context();
void context_proc_line();
void complex_word();

// module rcontext.c
void correct_case(void);

// module spec_sym.c
Bool abris_curve(uchar fun[], int16_t n, int16_t w);

// module fontprop.c
void font_let();
void font_str();
void checkpitch();
void avdens();
int16_t letincl(cell *);

// module lineout.c
void lineout();
void lineout_fragment();
void lines_out_init();
void rules_to_ed();
void ed_load();
void ed_flip();
void ed_out_finish();
void set_initial_edout(puchar pool, uint32_t lth);
void ed_save(void);
void init_sheet_tag(void);

// module lout_bcr.c
void lout_bcr();
void loutf_bcr();
void louti_bcr();

// module edscan.c
int16_t ed_scan(Bool(*)(puchar, int16_t), Bool(*)(puchar, int16_t), puchar, puchar);
Bool scan_skip(puchar p, int16_t lth);

// module edp.c
void edp_main(puchar memory, int32_t lth);

// module edpsave.c
void save_ed(void);

// module edprproc.c
void preprocess();

// module writepro.c
void write_prot(cell *);

// module tools.c
void set_bad_cell(cell *);
void sort_vers(cell *);
void sort_v_vect(int16_t, version *);
void setup_string();
void kit_init();
puchar il1_pool_ptr();
puchar t_raster();
c_comp * comp_vers_to_kit(MN * mn, c_comp *c);
c_comp * dust_to_kit(dust_comp *);
c_comp * comp_from_file(uint32_t, int32_t);
c_comp * compose_comp(int16_t, c_comp **);
Bool compose_cell(int16_t, cell **, cell *);
Bool compose_cell_save(int16_t, cell **, cell *);
void del_retain_cell(cell *);
cell *del_cell(cell *);
puchar del_save_cell(cell *);
cell *rest_cell(void *, cell *);
cell *cell_f();
cell *cell_l();
////////////////////////////////////////////////
//AK add 3 funcs for check chain of cells:
void AKTryChainRecover(cell **);
void AKCheckChain();
int16_t AKCellInChain(cell *);
void AKClearVers(void);
////////////////////////////////////////////////
cell *new_cell();
void free_cell(cell *c);
void insert_cell(cell *c, cell *ci);
void insert_cell1(cell *c, cell *ci);
void adjust_links(cell *c);
cell *create_cell(MN *, cell *, char, char);
cell *create_cell1(MN *, cell *, char, char);
cell *create_cell_work(MN * mn, cell * ci, char bdiff, char dflag);
int16_t short_recog_cell(cell *);
int16_t recog_cell(cell *);
int16_t recop_cell(cell *);
int16_t reco2_cell(cell *);
puchar save_raster(cell *);
puchar give_kit_addr();
void take_kit_addr(puchar a);
void add_vers(cell *, version *);
void del_version(cell *c, uchar let);
void online_comp(c_comp *);
Bool save_MN(MN *);
void cell_bonus_let(cell *C, char Let, int16_t BonVal);
void cell_bonus(cell *C, version *pVer, int16_t BonVal);

// module baton.c
int16_t sticks_in_letter(cell *, int16_t, STICK **);
void set_tab_incl();

// module filesys.c
void file_string(STRING *); // accept string into file
str *first_string();
str *next_string();
int16_t read_string();
void setup_memory();
void convert_rtf();
void maden_comp(puchar, int16_t, int16_t, int16_t, int16_t);
c_comp *get_comp(PROOT);
void Pass2(void);
void read_rec_file(int16_t seqn, puchar pool, puchar * end);
///////////////////int16_t cond_open(int16_t seqn, pchar name, uint16_t b1, uint16_t b2);
puchar seq_nam(int16_t seqn);
int16_t to_voc(void);
void to_edp(void);
void make_rtf(void);
void make_ASCII(void);
void make_smart_ASCII(void);
void LayoutSetup(void);
void load_vocs(void);

// module msgerr
void error_exit(int16_t group, int16_t element);
void error_exit_asm(int16_t element);
void clear_error_status(void);

// module testsnap
void snap_events(cell *);
void write_events(cell *);
void * snap_ev_res();
void snap_ev_reset();
void snap_keep(uchar user, puchar addr, uint16_t lth);
void snap_sticks(cell *);

// module v0compmn.asm
void extrcomp(void);

// module v0comprq.asm
int16_t recog_letter();
void recog_save_maden();

// module proport.c
void v2_pidx_crit(cell *);
void pidx_crit();
Bool pidx_skip(int16_t h, int16_t w, uchar t_let);
uint16_t el_pidx_crit(uint16_t t_let, int16_t pidx);

// module lang_fil.c
void ev_lang_filter();
Bool proplet(uchar);

// module compkit.c
int16_t MN_to_line(MN *);
puchar make_raster();
puchar make_extended_raster(c_comp *);
void save_dust_comp(dust_comp *, c_comp *, puchar);
int16_t read_dust_comp(dust_comp *, puchar);

// module picture.c
int16_t scaled(int16_t value, uchar scale);

// module large.asm
puchar compress_component(c_comp *c, uchar scale);
puchar compress_large_component(c_comp *c, uchar scale);
puchar compress_dust_component(c_comp *c, uchar scale);
int16_t filling_MN(MN *mn, uint16_t h, uint16_t w);

// 	module tg_spell
int16_t spelling(puchar memory, int32_t size);

// module in_treei.c
uchar * load_stat_dict(puchar ptr);
void load_user_dicts(char * list_name, puchar ptr);
void user_voc_init(void);

// module corners.c
void discrim_un(cell *, s_glue *, int16_t);

// module rtf.c
void rtf_main(puchar memory, int32_t memory_lth);

// module rtfdebug.c
void print_fragments();

// module ascisave.c
void smart_ascii_save(puchar pool, int32_t lth);
void ascii_save(puchar pool, int32_t lth);

// module writepro
void write_prot(cell *);

// module scanjet
int16_t give_scan_handle(void);
uint16_t scan_close(void);

// module dllentry or run
void write_text(uchar);

// module celldump.c
void setup_dump(void);
void cell_to_dump(cell *);
void dump_to_string(void);

//module tm.c
void trade_marks(void);

//module passe.c
cell * end_word(cell *cs, uchar *str, uchar *word_len);
cell * next_word(cell *cs);
Bool russian_word(uchar *wrd);

//module erection.c
uchar erection_incline_words(void);
uchar erection_one_incline_word(cell *c, cell *e);
void erection_delete(void);
void erection_restore(void);
void erection_delete_seq(cell *b, cell *e);
void erection_restore_seq(cell *b, cell *e);
int16_t erection_compose_inc(int16_t n, cell **clist);
int16_t erect_original_dens(cell *c);
int16_t erect_original_width(cell *c);
cell* erect_cell_value(cell *c, int16_t inc, int16_t shave, Bool cutting);
cell* erect_cell_table(cell *c, int16_t tab_angle[], int16_t shave, Bool cutting);
Bool erect_make_tab(cell *c, int16_t tab_angle[]);
void erect_shift_intervals(void *addr, int16_t h, int16_t tab_angle[]);
int16_t erect_raster_value(uchar *raster, int16_t dx, int16_t dy, int16_t inc);

// module shevron.c
void shevron(cell*);
/*-------------------------- Extern -------------------*/

Bool check_cancel(uint16_t perc);

MN *cell_cut(cell *);

void set_page_name(puchar);

void setdrmode(int16_t);

void c_rastror(puchar, puchar, uint16_t, uint16_t);

lnhead *c_boxln(MN *);

// module html.c
void html_main(puchar memory, int32_t memory_lth);

// virtual function...
uint16_t Tiger_BIOSKEY(uint16_t);
#endif
