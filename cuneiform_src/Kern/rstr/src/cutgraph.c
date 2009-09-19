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
/*								    */
/*                         Cutgraph program                         */
/*								    */
/********************************************************************/
#include <string.h>
#include "struct.h"
#include "cuthdr.h"
#include "func.h"

static int16_t init_list(void);
static void order_list(void);
static void init_graph(void);
static int16_t merge_split(void);
static void intersect(void);
static void make_half(struct short_line_header *);
static void place_in_list(void);
static int16_t wr_vertex(void);
static void add_edge(void);
static int16_t next_edge(void);
// static void three_excl_connect  (void);
// static int16_t concomp          (int16_t);

int16_t make_graph()
//
//	This procedure constructs the graph of (shortened) Ed_lines
//	of given component.
//
//	ON ENTER: Line presentation of turned raster.
//	ON EXIT : Shortened lines and graph of their connectivity.
//
{

	Z = &string;
	if (!init_list())
		return -1; // make initial list of lines
	max_line = n_lines;
	order_list(); // order of initial list and making one side links in it
	first_line = 0;
	init_graph(); // initialization of graph
	n_verts = 0;
	while (n_lines) {
		if (!merge_split())
			return -1;
		// find merging or splitting line and
		//   make separation of first line in list */
		n_verts++;
		first_line = Ed_lines[first_line].next; // to next line in list
		n_lines = max_line - n_verts;
	}
	return n_verts;
}

static int16_t init_list()
//
//	This procedure constructs initial list of lines.
//
{
	lnhead *lp;
	/*
	 struct ln_head
	 {
	 int16_t lth;	   // length of one line representation
	 int16_t h;		   // height of line
	 int16_t row;	   // relative row of line start
	 uchar flg;	   // flags of free beg and free end
	 #define l_fbeg		0x20
	 #define l_fend		0x80
	 };
	 */

	sl_ptr = Ed_lines;
	lp = (lnhead *) t_line_ptr;

	for (n_lines = 0; lp->lth != 0; n_lines++, sl_ptr++) {
		if (n_lines >= MAX_LINES)
			return 0;
		sl_ptr->top = (uchar) lp->row;
		sl_ptr->bot = sl_ptr->top + lp->h;
		sl_ptr->segm_addr = ((pchar) lp) - ((pchar) t_line_ptr) + sizeof(*lp);
		segm_ptr = (struct segment *) (lp + 1); // skip header of line
		sl_ptr->fb = segm_ptr->end - segm_ptr->lth;
		sl_ptr->fe = segm_ptr->end;
		segm_ptr += lp->h - 1; // segm_ptr points to the last segment in line
		sl_ptr->lb = segm_ptr->end - segm_ptr->lth;
		sl_ptr->le = segm_ptr->end;
		lp = (lnhead *) (((pchar) lp) + lp->lth); // next line
	}
	return 1;
}

static void order_list()
//
//	This procedure orders list of lines
//	and transfers it to the one link list.
//
{
	struct short_line_header *slp;
	struct short_line_header wline;
	int16_t i, j, k;

	for (i = 1; i < n_lines; i++) {
		j = i;
		k = j - 1;
		while ((j > 0) && ((Ed_lines[j].top < Ed_lines[k].top)
				|| ((Ed_lines[j].top == Ed_lines[k].top) && (Ed_lines[j].fb
						< Ed_lines[k].fb)))) {
			memcpy(&wline, &Ed_lines[j], sizeof(wline));
			memcpy(&Ed_lines[j], &Ed_lines[k], sizeof(wline));
			memcpy(&Ed_lines[k], &wline, sizeof(wline));
			j--;
			k--;
		}
	}
	slp = Ed_lines;
	for (i = 0; i < n_lines - 1; i++, slp++)
		slp->next = i + 1;
	slp->next = 0;
}

static void init_graph()
//
//	This procedure initializes graph of (shortened) lines.
//
{

	memset(adj_matrix, 0, sizeof(adj_matrix));
	memset(verts, 0, sizeof(verts));
}

static int16_t merge_split()
//
//	This procedure finds a line which merges with first line
//	or goes out from the first line. Then the first line separates
//	in two parts: before merging point and after it. Fist part
//	is a new vertex in the graph. Second one goes to its place
//	in the list in accordance with ordering.
//
{
	int16_t i;

	meet_flag = 0;
	wr_vertex_flag = 0;
	sl_ptr = fl_ptr = &Ed_lines[first_line];
	for (i = 1; i < MAX_LINES; i++) {
		meet_flag = 0;
		if (sl_ptr->next == 0)
			break;
		sl_ptr = &Ed_lines[sl_ptr->next];
		if (sl_ptr->top > fl_ptr->bot)
			break; // all possible splittings or mergings were passed
		intersect();
		if (meet_flag & (SPLIT | MERGE1)) {
			make_half(fl_ptr); // dividing of first line in two parts
			place_in_list(); // place lower part of first line in list
			max_line++;
			if (max_line >= MAX_LINES)
				return 0;
			wr_vertex_flag = 0;
			add_edge();
		}
		if (meet_flag & MERGE2) {
			make_half(sl_ptr); // dividing of second line in two parts
			place_in_list(); // place lower part of second line in list
			max_line++;
			add_edge();
		}
		if (meet_flag & CONT)
			add_edge();
		n_lines = max_line - n_verts;
	}
	return wr_vertex(); //Paul 02-02-2000
	/*
	 wr_vertex();		         // add vertex to graph
	 return 1;
	 */
}

static void intersect()
//
//	This procedure checks intersection between two lines.
//	ON ENTRY: fl_ptr - pointer to first line
//		  sl_ptr - pointer to second line
//    ON EXIT:
//		  segm_ptr - ptr to last segment in upper part of first
//			     or second line (which is finishes first
//		  meet_flag
//
//
{
	struct segment *sg;

	if (fl_ptr->top < sl_ptr->top) // second line goes from first ?
	{
		sg = ((struct segment *) (((pchar) t_line_ptr) + fl_ptr->segm_addr));
		sg += sl_ptr->top - fl_ptr->top - 1;/* possible splitting segment */
		if (!((sl_ptr->fb > sg->end) || (sl_ptr->fe < sg->end - sg->lth))) {
			if (fl_ptr->bot > sl_ptr->top) // proper splitting ?
				meet_flag = SPLIT; // splitting
			else
				meet_flag = CONT; // continuation
			segm_ptr = ++sg;
			goto inter_end;
		}
	}
	if (fl_ptr->bot > sl_ptr->bot) // second line goes into first ?
	{
		sg = ((struct segment *) (((pchar) t_line_ptr) + fl_ptr->segm_addr));
		sg += sl_ptr->bot - fl_ptr->top; // possible merging segment
		if (!((sl_ptr->lb > sg->end) || (sl_ptr->le < sg->end - sg->lth))) {
			meet_flag |= MERGE1; // merging
			segm_ptr = sg;
			goto inter_end;
		}
	}
	if ((fl_ptr->bot < sl_ptr->bot) && (fl_ptr->bot > sl_ptr->top)) { // first line goes into second ?
		sg = ((struct segment *) (((pchar) t_line_ptr) + sl_ptr->segm_addr));
		sg += fl_ptr->bot - sl_ptr->top; // possible merging segment
		if (!((fl_ptr->lb > sg->end) || (fl_ptr->le < sg->end - sg->lth))) {
			meet_flag |= MERGE2; // merging
			segm_ptr = sg;
		}
	}
	inter_end: ;
}

static void make_half(struct short_line_header *fslp)
//
//	This procedure splits first or second line.
//	fslp     - pointer to first or second line.
//	segm_ptr - pointer to first segment in lower
//		   part of first or second line.
//
{
	struct short_line_header *nlp;

	nlp = &Ed_lines[max_line];
	memcpy(nlp, fslp, sizeof(*nlp));
	nlp->segm_addr = ((pchar) segm_ptr) - ((pchar) t_line_ptr);
	fslp->bot = nlp->top = fslp->top + (nlp->segm_addr - fslp->segm_addr)
			/ sizeof(*segm_ptr);
	fslp->lb = (segm_ptr - 1)->end - (segm_ptr - 1)->lth;
	fslp->le = (segm_ptr - 1)->end;
	nlp->fb = segm_ptr->end - segm_ptr->lth;
	nlp->fe = segm_ptr->end;
}

static void place_in_list()
//
//	This program places lower part of first or second line
//	(Ed_lines[max_line]) in the appropriate place in the list.
//
//
{
	struct short_line_header *nlp, *blp, *blpold;

	if (fl_ptr->next != 0) {
		nlp = &Ed_lines[max_line];
		blpold = fl_ptr;
		blp = &Ed_lines[fl_ptr->next];
		while (1) {
			if ((blp->top > nlp->top) || ((blp->top == nlp->top) && (blp->fb
					> nlp->fb))) {
				nlp->next = blpold->next;
				blpold->next = (uchar) max_line;
				break;
			}
			blpold = blp;
			if (blp->next == 0) {
				blp->next = (uchar) max_line;
				nlp->next = 0;
				break;
			}
			blp = &Ed_lines[blp->next];
		}
	} else
		fl_ptr->next = (uchar) max_line;
}

static int16_t wr_vertex()
//
//	This procedure adds first_line as a new vertex to the graph.
//
{
	struct vertex_header *vp;

	if (!wr_vertex_flag) {
		if (fl_ptr - Ed_lines >= MAX_LINES) //Paul 02-02-2000
			return 0;
		vp = &verts[fl_ptr - Ed_lines];
		vp->top = fl_ptr->top;
		vp->bot = fl_ptr->bot;
		vp->beg = fl_ptr->segm_addr;
		vp->end = vp->beg + (vp->bot - vp->top) * sizeof(struct segment);
		wr_vertex_flag = 1;
		meet_flag = 0;
		sl_ptr = fl_ptr;
	}
	return 1; //Paul 02-02-2000
}

static void add_edge()
//
//	This procedure adds edges to current vertex.
//
{
	struct vertex_header *vp;
	int nv;

	wr_vertex();
	nv = fl_ptr - Ed_lines;
	vp = &verts[nv];
	if (meet_flag & (SPLIT | MERGE1 | MERGE2)) {
		if (adj_matrix[nv][max_line - 1] == 0) {
			adj_matrix[nv][max_line - 1] = 1;
			vp->out++;
			adj_matrix[max_line - 1][nv] = -1;
			verts[max_line - 1].in++;
		}
	}
	if (meet_flag & CONT) {
		if (adj_matrix[nv][sl_ptr - Ed_lines] == 0) {
			adj_matrix[nv][sl_ptr - Ed_lines] = 1;
			vp->out++;
			adj_matrix[sl_ptr - Ed_lines][nv] = -1;
			verts[sl_ptr - Ed_lines].in++;
		}
	}
}

void find_path()
//
//  This procedure find lowermost path from left border to right one.
//  If it fails, it means so-called "S"-case that will be treated specially.
//
{
	int16_t n, l, l0, v;
	uchar svpath[MAX_LINES];
	int16_t svpathl, svpatot;

	Z = &string;
	svpatot = -1;
	S_flag = 0;
	memset(path, 0, sizeof(path)); // first line is the beginning

	path_lth = 1;
	l0 = verts[0].bot;
	svpath[0] = 0;
	svpathl = 1;

	while (1) {
		if ((n = next_edge()) > 0) {
			l = verts[n].bot;
			path[path_lth++] = (uchar) n;
			if ((l - l0) > svpatot) // save longest path
			{
				svpatot = l - l0;
				for (v = 0; v < path_lth; v++)
					svpath[v] = path[v];
				svpathl = path_lth;
			}
			if (l == t_height)
				break;
			continue;
		}

		/*
		 if (verts[path[path_lth-1]].in > 1)            // "S"-case
		 flag=1;
		 */
		path[path_lth--] = 0;
		if (path_lth >= 1) // ****** >= 0 ??
			continue; // backtracking
		S_flag = 1;
		break;
	}
	//  if (S_flag)
	{
		for (l = 0; l < svpathl; l++)
			path[l] = svpath[l];
		path_lth = svpathl;
	}
}

static int16_t next_edge()
//
//	This procedure gives number of next outcoming edge
//	from vertex path[path_lth-1].
//
{
	char *mp;
	int16_t n;

	n = path[path_lth] + 1; // 1 + older son of path[path_lth-1]
	mp = adj_matrix[path[path_lth - 1]] + n;
	for (; n < n_verts; n++, mp++)
		if (*mp == 1)
			return n;
	return 0;
}

struct conn {
	puchar adjs; // current point in the edges list
	puchar adje; // end of the edges list
	uint32_t vert; // number of vertex (identifier)
	int16_t covers; // crossing vertexes starts counter
	uint16_t ncp; // components counter
};

void excl_connect()
//
//	This procedure finds connected components of the graph
//	after deletion of each (one by one) vertex of the path.
//
{
	struct conn stk[MAX_LINES + 1], *stp, *ref;
	struct conn * mark[MAX_LINES];
	puchar edge, edge_end;
	uint32_t vn, nverts = n_verts;

	memset(mark, 0, sizeof(mark)); // clear usage vertexes list
	stp = stk + 1; // leave one elemenet above the top
	vn = 0; // start with vertex number 0
	stp->ncp = 0; // vertex 0 has zero as start counter
	// of components

	after_down: mark[vn] = stp; // mark using of the vertex
	stp->vert = vn; // save vertex number
	stp->covers = 0; // clear counters
	edge = adj_matrix[vn]; // start of edge list
	stp->adje = edge_end = edge + nverts; // end of edge list

	edge_search: while ((edge != edge_end) && (*edge == 0))
		edge++; // skip empty edges
	if (edge == edge_end)
		goto up_stack; // edges finished
	vn = edge - edge_end + nverts; // number of adjusten vertex
	stp->adjs = edge + 1; // save start for next search
	if ((ref = mark[vn]) != 0)
		goto up_edge; // edge go upward

	stp->ncp++; // increase number of possible components
	(++stp)->ncp = 1; // number of possible components after down
	goto after_down;

	up_edge: if (ref != stk) // if edge to not 'leave' vertex
	{
		(stp - 1)->covers++; // increase number of starts in previous stack vertex
		ref->covers--; // increase number of ends in end of edge
	}
	edge++; // start search from next vertex
	goto edge_search;

	up_stack: less_vertices[stp->vert].n_connect = (char) stp->ncp; // set a component number
	// for the vertex

	if (!stp->vert)
		return; // return if top of the stack
	mark[stp->vert] = stk; // mark 'leave' vertex
	stp--; // step up
	if (stp->covers) // if last edge covered
	{
		(stp - 1)->covers += stp->covers; // move covers counters to parent
		stp->covers = 0; // and clear it for next edge
		stp->ncp--; // no component under edge
	}
	edge = stp->adjs;
	edge_end = stp->adje;
	goto edge_search;
}

/*
 static void three_excl_connect()
 //
 //  This procedure finds connected components of the graph
 //  after deletion of each (one by one) vertex of the path,
 //  and gives three answers: cut the leftmost segment of
 //  line, middle segment or rigthmost segment.
 //
 {
 struct less_vertex_elem *lv_ptr;
 int16_t n,vert;
 int16_t i,j,k;
 pchar c;
 pchar ac;
 pchar s;

 lv_ptr=less_vertices;
 memset(matrix,0,sizeof(matrix));
 for (vert=0; vert < n_verts; vert++,lv_ptr++)
 {
 switch (verts[vert].bot-verts[vert].top)
 {

 case 1  :
 memcpy(matrix,adj_matrix,n_verts*MAX_LINES);
 for (n=0,c=&matrix[0][vert]; n < n_verts; n++,c+=MAX_LINES)
 if (*c) *c=0;
 memset(matrix[vert],0,MAX_LINES);
 lv_ptr->n_connect=concomp(n_verts);
 lv_ptr->l_connect=lv_ptr->r_connect=lv_ptr->n_connect;
 break;

 case 2  :
 memcpy(matrix,adj_matrix,n_verts*MAX_LINES);
 for (n=0,c=&matrix[0][vert]; n < n_verts; n++,c+=MAX_LINES)
 if (*c == 1) *c=0;
 for (n=0,c=matrix[vert]; n < n_verts; n++,c++)
 if (*c == -1) *c=0;
 lv_ptr->l_connect=concomp(n_verts);
 memcpy(matrix,adj_matrix,n_verts*MAX_LINES);
 for (n=0,c=&matrix[0][vert]; n < n_verts; n++,c+=MAX_LINES)
 if (*c == -1) *c=0;
 for (n=0,c=matrix[vert]; n < n_verts; n++,c++)
 if (*c == 1) *c=0;
 lv_ptr->r_connect=concomp(n_verts);
 lv_ptr->n_connect=lv_ptr->l_connect;
 break;

 default :
 memcpy(matrix,adj_matrix,n_verts*MAX_LINES);
 for (n=0,c=&matrix[0][vert]; n < n_verts; n++,c+=MAX_LINES)
 if (*c == 1) *c=0;
 for (n=0,c=matrix[vert]; n < n_verts; n++,c++)
 if (*c == -1) *c=0;
 lv_ptr->l_connect=concomp(n_verts);
 memcpy(matrix,adj_matrix,n_verts*MAX_LINES);
 for (n=0,c=&matrix[0][vert]; n < n_verts; n++,c+=MAX_LINES)
 if (*c == -1) *c=0;
 for (n=0,c=matrix[vert]; n < n_verts; n++,c++)
 if (*c == 1) *c=0;
 lv_ptr->r_connect=concomp(n_verts);
 for (i=0; i < n_verts; i++)
 {
 ac=adj_matrix[i];
 c=matrix[i];
 if (i != vert)
 {
 for (j=0; j < n_verts; j++,c++,ac++)
 {
 if (j != vert) *c=*ac;
 else
 switch (*ac)
 {
 case  0:
 matrix[i][n_verts]=0;
 matrix[i][n_verts+1]=0;
 break;
 case  1:
 matrix[i][n_verts]=1;
 matrix[i][n_verts+1]=0;
 matrix[i][vert]=0;
 break;
 case -1:
 matrix[i][n_verts]=0;
 matrix[i][n_verts+1]=-1;
 matrix[i][vert]=0;
 break;
 }
 }
 }
 else
 {
 s=(pchar)matrix[n_verts];
 memcpy(s,adj_matrix[vert],MAX_LINES);
 for (k=0; k < n_verts; k++,s++)
 if (*s == 1) *s=0;
 s=(pchar)matrix[n_verts+1];
 memcpy(s,adj_matrix[vert],MAX_LINES);
 for (k=0; k < n_verts; k++,s++)
 if (*s == -1) *s=0;
 memset(matrix[vert],0,MAX_LINES);
 }
 }
 lv_ptr->n_connect=concomp(n_verts+2)-1;
 break;
 }
 memset(lv_ptr->roof,0,s_width);
 for (n=0; n < n_verts; n++)
 {
 if (n == vert)
 continue;
 for (i=verts[n].top; i < verts[n].bot; i++)
 lv_ptr->roof[i]=1;
 }
 }
 }

 static int16_t concomp(int nv)
 {
 char mark[MAX_LINES];
 int16_t ncp;
 char vert_list[MAX_LINES];
 int16_t beg,end;
 pchar mc,mp;
 int16_t n;
 int16_t i;

 memset(mark,0xFF,sizeof(mark));
 mark[0]=1;
 vert_list[0]=0;
 beg=0;
 end=1;
 ncp=1;                                 // connected component number
 while (1)
 {
 while (beg < end)
 {
 mp=matrix[vert_list[beg]];
 mc=mark;
 for (n=0; n < nv; n++,mp++,mc++)
 if ((*mp != 0) && (*mc == -1))
 {
 *mc=ncp;
 vert_list[end++]=n;
 }
 beg++;
 }
 for (mc=mark,n=0; n < nv; n++,mc++)
 {
 if (*mc == -1)                          // new connected component
 {
 vert_list[beg]=n;
 end=beg+1;
 *mc=++ncp;
 break;
 }
 }
 if (n == nv)
 break;
 }
 return ncp;
 }
 */
