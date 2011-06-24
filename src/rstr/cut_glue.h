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

//#define  MY_DEBUG

#define  MAXINT   32767     //макс. целое число
#define  MININT  -32767     //мин.  целое число
#define  MAX_COL  10000     //макс. значение столбца
#define  MAX_CUTS  63       //макс. число сечений
#define  MAX_SECT  MAX_CUTS //макс. число cell'ов после разрезания
#define  RASTER_WIDTH   128 //размеры растра
#define  RASTER_HEIGHT   64
#define  MAX_RO   254  //макс. вероятность
#define  MAX_SEG_VERS  256

#define  NOT_CUT  128  //флаг: запретить разрез
#define  united     1  //компоненты секции объединялись
#define  glued      2  //компоненты секции склеивались
#define  up_dust    4  //использованы верхние dust'ы
#define  low_dust   8  //нижние
#define  all  up_dust+low_dust   //все компоненты использованы
#define  gl_sticks 16  //склеиваются палки
//---------------------  types   --------------------------------------

struct seg_vers_struct { //версии сегмента
    struct seg_vers_struct *next; //следующая версия
    int16_t px; //левое сечение сегмента
    char gvar; //вариант склейки
    int16_t ro; //мера
    int16_t width; //ширина
    SVERS vers; //версии
};
typedef struct seg_vers_struct seg_vers;

struct dp_vers_struct { //куча версий dp
    seg_vers pool[5*MAX_SEG_VERS]; //версии
    seg_vers *node[MAX_SEG_VERS]; //указатель на версии для каждого сечения
};

struct raster_struct { //растр cell'а
    int16_t w; //ширина
    int16_t h; //высота
    int16_t top; //строка левого верхнего угла
    int16_t left; //столбец  -""-
    uchar pict[RASTER_WIDTH * RASTER_HEIGHT / 8]; //растр
};
typedef struct raster_struct raster;

//-------------------  variables  -------------------------------------

extern uchar db_status; // snap presence byte
extern uchar db_trace_flag; // 2 - more detailed estimate (ALT-F7)

//cg_main.c
extern uchar sticks_left_to_bad[];
extern uchar letters_left_to_bad[];
extern char *results_left_to_bad[];
extern uchar prob_left_to_bad[];
extern uchar sticks_right_to_bad[];
extern uchar letters_right_to_bad[];
extern char *results_right_to_bad[];
extern uchar prob_right_to_bad[];

extern B_LINES my_bases; //базовые линии
extern int16_t blank; //ширина пробела
extern int16_t minw; //минимальная ширина буквы
extern int16_t minp; //минимальная ширина точки
extern int16_t ps3; // 1/3 высоты строчной буквы
extern int16_t sym_dist; //среднее расстояние между символами
extern int16_t sym_width; //средняя ширина символа
extern int16_t cut_width; //при ширине > cut_width можно резать

extern char snap_text[], *snap;

extern uchar trs2; // >trs2 - буква хорошая во всех отношениях
extern uchar trg; //порог для склеивания

//---------------------  macros  --------------------------------------

#define let(x)  ( (x)->flg & c_f_let )
#define bad(x)  ( (x)->flg & c_f_bad )
#define dust(x) ( (x)->flg & c_f_dust )
#define fict(x) ( (x)->flg & c_f_fict )
#define cut(x)  ( (x)->cg_flag & c_cg_cutl+c_cg_cutr )
#define just(x) ( (x)->cg_flag & c_cg_just )
#define composed(x)    ( (x)->cg_flag & c_cg_comp )
#define let_or_fict(x) ( (x)->flg & ( c_f_let | c_f_fict ) )
#define let_or_bad(x)  ( (x)->flg & ( c_f_let | c_f_bad  ) )
#define complete_recog(x) ( (x)->recsource & ( c_rs_BOX | c_rs_bitcmp ) )

#define set_dust(x) ( (x)->flg = c_f_dust )
#define set_bad(x)  ( (x)->flg = c_f_bad  )
#define set_let(x)  ( (x)->flg = c_f_let  )
#define set_just(x) ( (x)->cg_flag |= c_cg_just )
#define let_to_bad(x) ( (x)->flg = ( (x)->flg & ~c_f_let | c_f_bad ) )
#define set_noglue(x) ((x)->cg_flag |= c_cg_noglue)

#define wide(C)    ( (C)->w > ((C)->h<<1) + ((C)->h>>2) )
#define middle(C)  ( (C)->r_col -1 + (((C)->w+1)>>1) )
#define lefter(C,x)  ( middle(C) < (x) )

#define ben_cut(x)    ( (x)->var & NOT_CUT )
#define point_fit(x,p)                    \
        ( p==1 && ((x)->var & 0x5E)==0 || \
          p==2 && ((x)->var & 0x78)==0 || \
          p==3 && ((x)->var & 0x70)==0 || \
          p==4 )
#define in_set(var,set)                   \
        ( set==0 && ((var) & 0x7F)!=40 || \
          set==1 && ((var) & 0x5E)==0  || \
          set==2 && ((var) & 0x7B)==0  || \
          set==3 && ((var) & 0x77)==0  || \
          set==4 && ((var) & 0x7F)==40 )

#define may_glue(x) \
  (!( (x)->cg_flag & c_cg_noglue || (x)->flg & (c_f_solid | c_f_confirmed \
     | c_f_detouch) ))
#define debug_on ( db_status && snap_activity('a') )
#define det_trace (db_trace_flag & 8)

//------------------  functions  --------------------------------------

//cg_smcut.c
void dp_pass0(cell *LC, raster *r, struct cut_elm *cut_list,
              seg_vers **vers_list, int16_t ncut);

//cg_main.c
cell *process_word(cell *WB, cell *WE);
uchar addij(cell *C, raster *r0, struct cut_elm *cut_list,
            seg_vers **vers_list, int16_t ncut, int16_t i1, int16_t i0, char mode);
void dp_bound(struct cut_elm *cut_list, seg_vers **vers_list, int16_t pass,
              int16_t *ib, int16_t *ie);
int16_t full_recog(cell *B1, s_glue *gl0, int16_t trs, int16_t tol);
cell * create_my_cell(MN * mn, cell * ci, char bdiff, char dflag);

//cg_tools.c
cell *col_to_one(cell **clist, int16_t n);
cell *comp_to_cell(cell *C, c_comp **list, int16_t N, char bdiff, uchar dflag);
Bool glue_overlap(cell *LC, cell *E);
seg_vers *find_vers(int16_t i1, int16_t i0, seg_vers **vers_list);
uchar not_connect_sect(int16_t i1, int16_t i0, struct cut_elm *cut_list);
int16_t on_path(int16_t i, int16_t ie, struct cut_elm *cut_list);
seg_vers *store_vers(seg_vers *cur_vers, seg_vers **vers_list, int16_t i1,
                     int16_t i0, SVERS *vers, int16_t ro, int16_t width, char gvar);
void adjust_3x5(Bool prerecog);

void cg_show_rast(cell *C, raster *r, const char *msg, struct cut_elm *cut_list);
void cg_show_list(cell **cells, int16_t N, uchar *msg);
char *show_dp(puchar s, struct cut_elm *cut_list, int16_t i0);
void det_snap(cell *C, const char *txt);
void show_and_wait(const char *txt);

//cg_hcut.c
void base_lines_cut();
