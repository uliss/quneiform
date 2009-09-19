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


#ifndef CUTSTR

#define CUTSTR 1

#define MAX_CELLS_IN_LIST    8

struct v_v
 {int16_t v1,v2,v3;};
typedef struct v_v v_val;

struct save_vers
 {int16_t nvers; version vers[VERS_IN_CELL]; char flg; char pos; uchar source;
  uchar broken_II; uchar cg_flag_fine;uchar accent_leader;
  uchar bottom_accent;   // 16.10.97 E.P.
  uchar history;      // Nick  18.6.99
  int16_t  clink;        // Nick  18.6.99
  uint16_t clu_attr;     // Nick  18.6.99
  int16_t  nClust;
  version pr_vers;
  };
typedef struct save_vers SVERS;

struct cut_elm
 {
 char x;                            // position
 char dh;                           // height to be cut
 char h;                            // end of segment
 char var;                          // method
 v_val lv;                          // two-component estimate of left side
 char px;                           // best previous point
 v_val rv;                          // two-component estimate of right sude
 char duflm, gvarm, duflr, gvarr;   // glue variant, dust flags
 SVERS  versm;                      // best cell versions
 SVERS  versr;                      // right part versions
 };

typedef struct cut_position
 { int16_t row1, row2, cuth; char flag; } cut_pos;
struct glue_struct
 {
 c_comp *complist[MAX_CELLS_IN_LIST];
 cell *celist[MAX_CELLS_IN_LIST];
 cell *maxlist[MAX_CELLS_IN_LIST];
 cell *nclist[MAX_CELLS_IN_LIST];
 void *kitlist[MAX_CELLS_IN_LIST];
 uchar ncell, ngluc, arg, maxnc;
 uchar flarg, fres, var;
 int16_t  Dist, dist, bdiff, maxval;
 MN   *mn;
 int16_t  row, col, height, width;
 int16_t c_r, c_c, c_w, c_h, c_mc, c_mr, c_dens;
 };
typedef struct glue_struct s_glue;

struct all_c
 {
 int16_t   Nb, Nd, minb, mind, minf;
 uchar  flag;
 #define dust_left 1
 #define dust_right 2
 #define cl_disorder 4
 cell *minlet, *maxlet, *mincl, *maxcl;
 cell *bcells[MAX_CELLS_IN_LIST],
      *dcells[MAX_CELLS_IN_LIST];
 };
typedef struct all_c all_cells;

struct st_answer
 {
 char ltr;
 char fnt;
 uint16_t iprob;
 char n_rsn; // number of letter from list of same letters
 };
typedef struct st_answer t_answer;

struct info_elm
 {
 char topline, botline;
 char lnum;
 char topf, toph, botf, both;
 char sumb;
 };

struct own_cut
 {
 char x;     // position
 char dh;    // height to be cut
 char h;     // end of segment
 char var;   // method
 };

struct extrem_elem
 {
 char type;      // +- -- max/min
 char cat;       // category
 char beg, end;  // area limits
 char val;       // etrem value
 };

struct linehdr
 {
 int16_t  lth;
 uint16_t  dy;
 uint16_t  y0;
 char flag;
 };

struct segment
 {
 uchar lth;
 uchar end;
 };

struct short_line_header
 {
 uchar top;      	// top string of line (relative)
 uchar bot;	        // bottom string of line (relative)
 uchar fb;	        // beginning of first segment of line
 uchar fe;	        // end of first segment of line
 uchar lb;	        // beginning of last segment of line
 uchar le;	        // end of last segment of line
 uchar next;	        // number of next line in (shortened) lines' list
 int16_t  segm_addr;        // offset of first segment in line
 };

struct vertex_header
 {
 uchar top;	// top string of component (relative)
 uchar bot;	// bottom string of component (relative)
 int16_t  beg;	// offset of first segment of line in c_comp
 int16_t  end;	// offset of last + 1 segment of line in c_comp
 uchar out;	// outgoing valency
 uchar in;	// incoming valency
 };

struct local_min
 {
 int16_t  value;    // value of minimum
 int16_t  ambeg;    // x-coordinate of beginning
 int16_t  amend;    // end of local minimum area
 };

struct func_elem
 {			// description of argument (x)
 char fvalue[3];	// values of lth, lower, upper
 char extrem[6];	// number of minimum area for this
			//   particular x for three functions;
			//   -1 means out of minima areas
 char line;		// number of line in graph containing this x
 };

struct graph_comp
 {
 char top;
 char bot;
 char left;
 char right;
 };

struct less_vertex_elem
 {			// graph without one vertex
 char n_connect;	// number of connected components
 };

typedef struct own_cut mas_cpnt;
#endif
