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
INT brigt(INT, INT, INT, INT);

// module names.c
pchar full_name(pchar w, pchar n);

// module kernel.c
void correct_dir_path(puchar p);

// module proimage.c
void image_file_close();
Bool image_file_open();
INT source_read(puchar start, puchar ptr, puchar end);

// module progress.c
void set_percentage(void);
void progress_next_step(void);
uint32_t progress_set_step(uint32_t volume);
uint32_t progress_set_percent(uint32_t volume);
void progress_finish(void);

// module matrix.c
void matrix_open();
void matrix_close();
INT matrix_read(puchar buff, uint16_t lth);

// module rules.c
void rules_open();
INT rules(MN *);
INT frame(MN *);
void rules_closed();
void short_undln();
void del_root(INT, INT, INT, INT);

// module rulsh.c
LONG rules_shaving(MN *, INT);
INT shaving_let();

// module rulbr.c
void cutoffs();

// module sweeper.c
void sweeper_ini();
uint16_t sweeper(INT);
void sweeper_end();
void read_comp(uint32_t, pchar, LONG);
void comp_file_close();
INT delgarb();
void del_shav();

// module skew.c
void skew_corr(str *, INT);
void skew_end();
void ideal_rc(cell *);

// module pass3.c
void pass3(INT nCall);
void accept_cell(cell *, c_comp *);
INT revision();
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
Bool snap_show_raster(puchar, INT, INT);
Bool Alik_snap_show_raster(puchar, puchar, puchar, INT, INT, pchar, pchar, PINT);
void snap_newcut();
void snap_show_cuts(cell *C, struct cut_elm *cut_points);
void snap_newpoint(INT i);
void snap_stop_cuts();

// module prop.c
void prop(INT);
INT prop_index(INT h, INT w);

// module diff.c
INT ij_dot(cell *);
void criteries();
void criteria(cell *);
cell *dot_ri(cell *);
cell *dot_ij(cell *);
cell *dot_excl(cell *);
cell *dot_inv(cell *);
INT ij_test(cell *);
void v_to_y(cell * c, uchar compose);
void c_to_ctail(cell * c, uchar compose);

// module accents.c
void pass_accent(void);
INT bottom_accent(cell *c);
INT accent(cell *);
INT type_acc(cell *);
INT find_bottom_accent(cell *c);

// module findiff.c
void final_crit();

// module linpos.c
INT obtain_diff(INT);
INT def_locbas(cell *);
void inter_diff(cell *);
INT if_dust(cell *);
INT if_dust_size(cell *);
void linpos(str *);
void cell_by_base();
void cell_position(cell *);
INT levcut(cell *, INT);
void get_b_lines(cell *, B_LINES *);
LONG get_size();
INT get_bsm(); //AK 04.03.97 ? to LONG
INT is_defis(cell *);
void basedraft(str *);
void basefin(str *);
INT cut_by_pos(cell *C, uchar c, INT wp, uchar fl, uchar arg);

// module dmacc.c
INT try_cut_accent(cell *C1, B_LINES *my_bases, INT flag);
INT try_cut_top_accent(cell *C1, B_LINES *my_bases, INT flag);
INT try_cut_bot_accent(cell *C1, B_LINES *my_bases, INT flag);

// module dm1.c
void dm1();
INT discrid(cell *, INT);
INT decidust(cell *);
INT estcomp(char, cell *, SVERS *, INT, INT, INT, s_glue *, cut_pos*, cut_pos*,
		char, char, char);
void snBOX(cell *);
void est_snap(char user, cell *C, pchar txt);
void promote(uchar, cell *, uchar, INT);
void collect_pat(cell *, s_glue *, cell *);

// module dm2.c
void dm2();
void glsnap(char, cell *, pchar);

// module dms.c
void save_vers(cell *, SVERS *);
void min3(v_val *, uchar, v_val *);
void rest_vers(cell *, SVERS *);
INT signif_line_count(cell*);
INT compare_vect(v_val *, v_val *);
INT cell_is_BOX_solid(cell*);

// module locomp.asm
/*#define c_locomp(a, b, c, d, e)        EVN_CLocomp((a), (b), (c), (d), (e))*/
MN *c_locomp(puchar, LONG, LONG, INT, INT);

// module v0compgl.asm
void invert_tiff(puchar c, uint16_t lth);

// module cutgraph.c
INT make_graph();
void excl_connect();
void find_path();

// module cutpoint.c
INT cut_points(INT, INT, puchar, struct cut_elm *);
INT Alik_cut_points(INT, INT, puchar, struct cut_elm *, INT);
MN* cut_rast(puchar, INT, INT, INT, INT, struct cut_elm *, char, char, pchar,
		cut_pos*);

// module alcutfun.c
void Alik_def_cut_flg(cell *, struct cut_elm *, struct cut_elm *, INT, uchar);

// module glue.c
MN *glue(c_comp **, INT);

// module dmBOX.c
INT dmBOX(cell *, s_glue *);
INT dmiBOX(cell *, s_glue *, INT);
servBOX *make_broken_raster(s_glue *, INT);
INT crecell(cell *, s_glue *, INT);
INT crepat(cell *, s_glue *, INT, INT);
void SBOX_to_static();
void SBOX_to_GL(s_glue *);
void GL_to_SBOX(s_glue *);
void static_to_SBOX();
void dens_to_cell(cell *);
void static_to_cell(cell *);

// module embBOX.c
void embBOX(servBOX *, SVERS *, Bool);
void embBOXF(servBOX *, INT, Bool);

// module scalar.asm
INT proport(uint16_t, uint16_t, uint16_t, INT, INT);
extern uint16_t (*scalarf)(PWORD, PWORD, uint16_t);
extern LONG (*scalar)(PWORD, PWORD);
INT long_sqrt(uint32_t);

// module funcBOX.asm
void comp_to_box(PWORD, c_comp *, uint16_t, uint16_t, uint16_t, uint16_t);

// module abris.c
void abris_reset();
INT abris(s_glue *, cell *, uchar, INT);
INT h_filt(cell *, INT, s_glue *, uchar Let);
INT rtf(cell *cl, INT prob, s_glue *GL);
void apply_monus(cell *);

// module perc.c
void perc();
void number();

// module atlant.c
void atlant(cell *, s_glue *, INT, INT);

// module ltmain.c
void LayoutPart1(void);
void LayoutPart2(void);

// module ltpage.c
void SpecCompPut(MN *mn);

// module spec_sym.c
void star();
void bullet();
INT chkbullet(uchar);
Bool abris_curve(uchar fun[], INT n, INT w);
INT find_minimum(uchar fun[], INT n, uchar *_imin);

// module S_TOOLS.C
void new_vers(cell *c, uchar vers, uchar prob);
void down_all_versions(cell *c, INT monus);
void filtr_shave(uchar fun[], INT len);
Bool set_prob(cell *c, char let, uchar prob);
Bool check_let(cell *c, char let);
void add_stick_vers(cell *c, char let, uchar prob);
void filtr_short(uchar fun[], INT n, INT lev);
void filtr121(uchar fun[], INT n);
void filtr_bullet(uchar fun[], INT len);
void add_thick_stick_versions(cell *c, INT dx, INT wide, INT inc, INT typ_snap,
		STICK_CHARS *left_chars, STICK_CHARS *right_chars,
		STICK_SIGNUMS *signums);
void add_thin_stick_versions(cell *c, uchar left[], uchar right[], INT dy,
		INT dx, INT typ_inc, STICK_CHARS *left_chars, STICK_CHARS *right_chars,
		STICK_SIGNUMS *signums);
Bool sign_neck(uchar left[], uchar right[], INT n, INT base, INT mode);
INT conv_prob(INT prob);
INT shift_raster(uchar *r, INT dy, INT dx, INT tab[], INT max_shift, uchar *res,
		INT dir);
/******************************
 void full_snap(cell *c,STICK_CHARS *l,STICK_CHARS *r,STICK_SIGNUMS *s,
 INT tab_angle[],
 INT left_mode,INT right_mode,INT opt,INT wide,
 INT inc_v,INT t);
 void short_snap(char *s,INT t);
 *******************************/

// module dis_stic.c
INT discrim_all_sticks(cell *c, STICK_CHARS *left_chars,
		STICK_CHARS *right_chars, STICK_SIGNUMS *signums);
INT discrim_stick(uchar, STICK_CHARS *, STICK_CHARS *, STICK_SIGNUMS *, INT);
INT del_sticks(cell *c, char let);
INT similar_0xBA(STICK_CHARS *, STICK_CHARS *, STICK_SIGNUMS *);
INT similar_0xBB(STICK_CHARS *l, STICK_CHARS *r, STICK_SIGNUMS *s);
INT similar_1(STICK_CHARS *, STICK_CHARS *, STICK_SIGNUMS *);
INT similar_l_stroked(STICK_CHARS *, STICK_CHARS *, STICK_SIGNUMS *);
INT similar_lcb(STICK_CHARS *, STICK_CHARS *, STICK_SIGNUMS *);
INT similar_rcb(STICK_CHARS *, STICK_CHARS *, STICK_SIGNUMS *);
INT similar_excl(STICK_CHARS *, STICK_CHARS *, STICK_SIGNUMS *);
INT similar_excl_with_dot(STICK_CHARS *, STICK_CHARS *, STICK_SIGNUMS *);
uchar similar_wide_frt1(STICK_CHARS *left_chars, STICK_CHARS *right_chars);

// module def_stic.c
INT stick_center_study(cell *c, s_glue *GL, INT typ);
uint16_t typ_thin_stick(lnhead *lin, INT dy, INT dx);
INT erector(cell *c, INT enable_erect, INT enable_shave, INT enable_snap);

// module cha_stic.c
INT set_stick_char(uchar left[], uchar right[], INT tails[], INT dy, INT dx,
		INT opt, INT wid, INT corr_mode, INT skip_ul, INT skip_dl, INT skip_ur,
		INT skip_dr, INT inc, STICK_CHARS *left_chars,
		STICK_CHARS *right_chars, STICK_SIGNUMS *signums, INT *l_mode,
		INT *r_mode);
INT calc_right_f_symptom(uchar fun[], INT n, INT m);
INT calc_left_f_symptom(uchar fun[], INT n, INT nx, Bool c_ft);
void num_of_skip_line(uchar left[], uchar right[], INT dy, INT opt, INT wide,
		INT *up_l, INT *up_r, INT *down_l, INT *down_r);

void set_stick_aux_signums(cell *c, uchar left[], uchar right[], INT dy,
		INT wide_up, STICK_CHARS *left_chars, STICK_CHARS *right_chars,
		STICK_SIGNUMS *signums, INT dis_left_brace, INT dis_right_brace,
		INT dis_slash, INT l_f_sym, INT r_f_sym, INT T_c, uint16_t T_2_3,
		uint16_t T_skok, B_LINES *bl, INT nlines, INT incline);

void
		calc_skipped_lines(INT hist_int[], uchar left[], uchar right[], INT dy,
				INT dx, INT wide, INT opt, Bool c_r, INT *ul, INT *ur, INT *dl,
				INT *dr);
void correct_f_symptoms(INT hist_int[], INT dy, INT *l_f_sym, INT *r_f_sym);
void correct_upper_peaks(STICK_CHARS *l, STICK_CHARS *r, INT dy);

// module differ.c
void differ();

// module bold.c
Bool boldshave(cell *, INT); //AK: at .ms as Bool
Bool pimpleshave(cell *C, INT shave, INT inc); //AK: at .ms as Bool


// module shaving.c
INT shaving(INT);

// module punct.c
void punct();
void clear_punct_twin_flags(void);
void clean_punct();
void clean_line();
void recover_dusts(void);
void hide_dusts(uchar reg);

// module il1peak.c
void extremum(pchar, INT, INT, INT, INT, char, char);

// module il1tg.c
void il1();

// module space.c
void space_size(INT);
void set_space_size(INT);
void space_cell();
void cont_space();
void showgap(cell *);
INT get_space_size(void);
INT get_gap(void);

// module context.c
void context();
void context_proc_line();
void complex_word();

// module rcontext.c
void correct_case(void);

// module spec_sym.c
Bool abris_curve(uchar fun[], INT n, INT w);

// module fontprop.c
void font_let();
void font_str();
void checkpitch();
void avdens();
INT letincl(cell *);

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
INT ed_scan(Bool(*)(puchar, INT), Bool(*)(puchar, INT), puchar, puchar);
Bool scan_skip(puchar p, INT lth);

// module edp.c
void edp_main(puchar memory, LONG lth);

// module edpsave.c
void save_ed(void);

// module edprproc.c
void preprocess();

// module writepro.c
void write_prot(cell *);

// module tools.c
void set_bad_cell(cell *);
void sort_vers(cell *);
void sort_v_vect(INT, version *);
void setup_string();
void kit_init();
puchar il1_pool_ptr();
puchar t_raster();
c_comp * comp_vers_to_kit(MN * mn, c_comp *c);
c_comp * dust_to_kit(dust_comp *);
c_comp * comp_from_file(uint32_t, LONG);
c_comp * compose_comp(INT, c_comp **);
Bool compose_cell(INT, cell **, cell *);
Bool compose_cell_save(INT, cell **, cell *);
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
INT AKCellInChain(cell *);
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
INT short_recog_cell(cell *);
INT recog_cell(cell *);
INT recop_cell(cell *);
INT reco2_cell(cell *);
puchar save_raster(cell *);
puchar give_kit_addr();
void take_kit_addr(puchar a);
void add_vers(cell *, version *);
void del_version(cell *c, uchar let);
void online_comp(c_comp *);
Bool save_MN(MN *);
void cell_bonus_let(cell *C, char Let, INT BonVal);
void cell_bonus(cell *C, version *pVer, INT BonVal);

// module baton.c
INT sticks_in_letter(cell *, INT, STICK **);
void set_tab_incl();

// module filesys.c
void file_string(STRING *); // accept string into file
str *first_string();
str *next_string();
INT read_string();
void setup_memory();
void convert_rtf();
void maden_comp(puchar, INT, INT, INT, INT);
c_comp *get_comp(PROOT);
void Pass2(void);
void read_rec_file(INT seqn, puchar pool, puchar * end);
///////////////////INT cond_open(INT seqn, pchar name, uint16_t b1, uint16_t b2);
puchar seq_nam(INT seqn);
INT to_voc(void);
void to_edp(void);
void make_rtf(void);
void make_ASCII(void);
void make_smart_ASCII(void);
void LayoutSetup(void);
void load_vocs(void);

// module msgerr
void error_exit(INT group, INT element);
void error_exit_asm(INT element);
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
INT recog_letter();
void recog_save_maden();

// module proport.c
void v2_pidx_crit(cell *);
void pidx_crit();
Bool pidx_skip(INT h, INT w, uchar t_let);
uint16_t el_pidx_crit(uint16_t t_let, INT pidx);

// module lang_fil.c
void ev_lang_filter();
Bool proplet(uchar);

// module compkit.c
INT MN_to_line(MN *);
puchar make_raster();
puchar make_extended_raster(c_comp *);
void save_dust_comp(dust_comp *, c_comp *, puchar);
INT read_dust_comp(dust_comp *, puchar);

// module picture.c
INT scaled(INT value, uchar scale);

// module large.asm
puchar compress_component(c_comp *c, uchar scale);
puchar compress_large_component(c_comp *c, uchar scale);
puchar compress_dust_component(c_comp *c, uchar scale);
INT filling_MN(MN *mn, uint16_t h, uint16_t w);

// 	module tg_spell
INT spelling(puchar memory, LONG size);

// module in_treei.c
uchar * load_stat_dict(puchar ptr);
void load_user_dicts(char * list_name, puchar ptr);
void user_voc_init(void);

// module corners.c
void discrim_un(cell *, s_glue *, INT);

// module rtf.c
void rtf_main(puchar memory, LONG memory_lth);

// module rtfdebug.c
void print_fragments();

// module ascisave.c
void smart_ascii_save(puchar pool, LONG lth);
void ascii_save(puchar pool, LONG lth);

// module writepro
void write_prot(cell *);

// module scanjet
INT give_scan_handle(void);
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
INT erection_compose_inc(INT n, cell **clist);
INT erect_original_dens(cell *c);
INT erect_original_width(cell *c);
cell* erect_cell_value(cell *c, INT inc, INT shave, Bool cutting);
cell* erect_cell_table(cell *c, INT tab_angle[], INT shave, Bool cutting);
Bool erect_make_tab(cell *c, INT tab_angle[]);
void erect_shift_intervals(void *addr, INT h, INT tab_angle[]);
INT erect_raster_value(uchar *raster, INT dx, INT dy, INT inc);

// module shevron.c
void shevron(cell*);
/*-------------------------- Extern -------------------*/

Bool check_cancel(uint16_t perc);

MN *cell_cut(cell *);

void set_page_name(puchar);

void setdrmode(INT);

void c_rastror(puchar, puchar, uint16_t, uint16_t);

lnhead *c_boxln(MN *);

// module html.c
void html_main(puchar memory, LONG memory_lth);

// virtual function...
uint16_t Tiger_BIOSKEY(uint16_t);
#endif
