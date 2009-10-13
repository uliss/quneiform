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

#include "lnslang.h"

#include "fararray.h"
#include "lnscheck.h"
#include "skew1024.h"
#include "point.h"

using namespace CIF;

#define PPSArray TArray

#define CONSOLE /* This can also be printf. */

typedef struct {
	int first;
	int last;
	int prev;
	int next;
} ADJ;
typedef struct {
	Point16 A;
	Point16 B;
	int C;
} XSEGM;
typedef struct {
	Point16 LT;
	Point16 RT;
	Point16 LB;
	Point16 RB;
	int leftLine;
	int rightLine;
	int topLine;
	int bottomLine;
} BOX;
typedef struct {
	int status;
	int num;
	int top;
	int bot;
} EDGEBOX;
typedef struct {
	int flag;
	int part;
	int coord;
} UsedTag;

LinesTotalInfo* Lti;

int h_count;
int v_count;
int skew;
int h1_count;
int v1_count;
PPSArray<XSEGM> h_lns;
PPSArray<XSEGM> v_lns;
static PPSArray<Point16> h1_lns;
static PPSArray<Point16> v1_lns;
static PPSArray<Point16> hlink;
static PPSArray<Point16> vlink;
static PPSArray<XSEGM> YMapping; // Ax-type of mapping object (mo), Ay-number of mo, Bx-y-coord of mo
static PPSArray<int> YMapLnsNum;// number of a h_line having y-rank in accordance with YLnsOrder
static PPSArray<int> YLnsOrder; // y-rank of a h_line among h_lines (ahly-rank)
static PPSArray<int> YpARank; // ahly-rank of a h_line previous to p.A of v_line
static PPSArray<int> YpBRank; // ahly-rank of a h_line previous to p.B of v_line
static PPSArray<XSEGM> XMapping;
static PPSArray<int> XMapLnsNum;
static PPSArray<int> XLnsOrder;
static PPSArray<int> XpARank;
static PPSArray<int> XpBRank;
PPSArray<Bool> HMarkedNoise;
PPSArray<Bool> VMarkedNoise;
static PPSArray<Point16> StripLine;
PPSArray<Point16> Hlev;
PPSArray<Point16> Vlev;
PPSArray<ADJ> ADJA;
int D; // the distance to neglect
int DD; // small distance (used by HBound, VBound)
int MD;

int HLength(XSEGM* S) {
	return (((S->B).x()) - ((S->A).x()));
}

int VLength(XSEGM* S) {
	return (((S->B).y()) - ((S->A).y()));
}

int HLengthCompare(const void* S1, const void* S2) {
	return (HLength((XSEGM*) S2) - HLength((XSEGM*) S1));
}

int VLengthCompare(const void* S1, const void* S2) {
	return (VLength((XSEGM*) S2) - VLength((XSEGM*) S1));
}

int MapCoord(XSEGM* S) {
	return ((S->B).x());
}

int MappingCompare(const void* S1, const void* S2) {
	return (MapCoord((XSEGM*) S1) - MapCoord((XSEGM*) S2));
}

int StrLCoord(Point16* S) {
	return (S->y());
}

int StrLCompare(const void* S1, const void* S2) {
	return (StrLCoord((Point16*) S1) - StrLCoord((Point16*) S2));
}

void Rotate(int sk) {
	long skew = -sk;
	for (int i = 0; i < h_count; i++) {
		h_lns[i].A.deskew(skew);
		h_lns[i].B.deskew(skew);
	}

	for (int j = 0; j < v_count; j++) {
		v_lns[j].A.deskew(skew);
		v_lns[j].B.deskew(skew);
	}
}

void CorrectDirection() {
	for (int i = 0; i < h_count; i++) {
		LineInfo& L = ((Lti->Hor).Lns)[i];
		L.TmpUsage = 0;
		if ((h_lns[i].A).x() > (h_lns[i].B).x()) {
			L.TmpUsage = 1;
			Point16 U = h_lns[i].B;
			h_lns[i].B = h_lns[i].A;
			h_lns[i].A = U;
		};
	};
	for (int j = 0; j < v_count; j++) {
		LineInfo& L = ((Lti->Ver).Lns)[j];
		L.TmpUsage = 0;
		if ((v_lns[j].A).y() > (v_lns[j].B).y()) {
			L.TmpUsage = 1;
			Point16 U = v_lns[j].B;
			v_lns[j].B = v_lns[j].A;
			v_lns[j].A = U;
		};
	};
}

void LengthSort() {
	::qsort(&(h_lns[0]), h_count, sizeof(h_lns[0]), HLengthCompare);
	::qsort(&(v_lns[0]), v_count, sizeof(v_lns[0]), VLengthCompare);
}

void InitMapping() {
	int r, s, d;
	for (int i = 0; i < h_count; i++) {
		YMapping[i].A.set(0, i);
		YMapping[i].B.rx() = h_lns[i].A.y();
	};
	for (int j = h_count; j < (h_count + v_count); j++) {
		d = j - h_count;
		r = h_count + (d << 1);
		YMapping[r].A.set(1, d);
		YMapping[r].B.rx() = v_lns[d].A.y();
		YMapping[r + 1].A.set(2, d);
		YMapping[r + 1].B.rx() = v_lns[d].B.y();
	};

	for (int l = 0; l < v_count; l++) {
		XMapping[l].A.set(0, l);
		XMapping[l].B.rx() = v_lns[l].A.x();
	};
	for (int m = v_count; m < (v_count + h_count); m++) {
		d = m - v_count;
		s = v_count + (d << 1);
		XMapping[s].A.set(1, d);
		XMapping[s].B.rx() = h_lns[d].A.x();
		XMapping[s + 1].A.set(2, d);
		XMapping[s + 1].B.rx() = h_lns[d].B.x();
	};
}

void SortMapping() {
	::qsort(&(YMapping[0]), h_count + (v_count << 1), sizeof(YMapping[0]),
			MappingCompare);
	::qsort(&(XMapping[0]), v_count + (h_count << 1), sizeof(XMapping[0]),
			MappingCompare);
}

void InitMappingInverse() {
	int l;
	int m = 0;

	for (int rk = 0; rk < h_count + (v_count << 1); rk++) {
		l = YMapping[rk].A.y();
		switch (YMapping[rk].A.x()) {
		case 0: {
			YLnsOrder[l] = m;
			YMapLnsNum[m] = l;
			m++;
		}
			break;

		case 1:
			YpARank[l] = m - 1;
			break;

		case 2:
			YpBRank[l] = m - 1;
		}
	}

	m = 0;
	for (int j = 0; j < v_count + (h_count << 1); j++) {
		l = XMapping[j].A.y();
		switch (XMapping[j].A.x()) {
		case 0: {
			XLnsOrder[l] = m;
			XMapLnsNum[m] = l;
			m++;
		}
			break;

		case 1:
			XpARank[l] = m - 1;
			break;

		case 2:
			XpBRank[l] = m - 1;
		}
	}
}

Bool IsHCloseCovering(Point16 S, int n) // expanding S1 covers S2
{ // where |S1|>|S2|
	int S1Ax = h_lns[S.x()].A.x();
	int S1Bx = h_lns[S.y()].B.x();
	int S2Ax = h_lns[n].A.x();
	int S2Bx = h_lns[n].B.x();
	if ((S1Ax - D < S2Ax && S2Ax < S1Bx + D) || (S1Ax - D < S2Bx && S2Bx < S1Bx
			+ D) || (S2Ax - D < S1Bx && S1Bx < S2Bx + D))
		return FALSE;

	int xlength = ((S1Bx - S1Ax)); //>>2)+((S1Bx-S1Ax)>>3);
	if ((S1Bx + xlength < S2Ax) || (S1Ax - xlength > S2Bx))
		return FALSE;

	if ((S2Bx - S2Ax) > xlength + D)
		return FALSE;

	int S1Ay = h_lns[S.x()].A.y();
	int S1By = h_lns[S.y()].B.y();
	int S2Ay = h_lns[n].A.y();
	int S2By = h_lns[n].B.y();

	if (S1By < S1Ay) {
		if ((((S2Ay < S1By + D) && (S2By < S1By + D) && (S2Ax > S1Bx))
				|| ((S2By > S1Ay - D) && (S2Ay > S1Ay - D) && (S2Bx < S1Ax))))
			return TRUE;
	} else {
		if ((((S2By < S1Ay + D) && (S2Ay < S1Ay + D) && (S2Bx < S1Ax))
				|| ((S2Ay > S1By - D) && (S2By > S1By - D) && (S2Ax > S1Bx))))
			return TRUE;
	}
	return FALSE;
}

Bool IsVCloseCovering(Point16 S, int n) // expanding S1 covers S2
{ // where |S1|>|S2|
	int S1Ay = v_lns[S.x()].A.y();
	int S1By = v_lns[S.y()].B.y();
	int S2Ay = v_lns[n].A.y();
	int S2By = v_lns[n].B.y();

	if ((S1Ay - D < S2Ay && S2Ay < S1By + D) || (S1Ay - D < S2By && S2By < S1By
			+ D) || (S2Ay - D < S1By && S1By < S2By + D))
		return FALSE;

	int ylength = ((S1By - S1Ay)); //>>2)+((S1By-S1Ay)>>3);
	if ((S1By + ylength < S2Ay) || (S1Ay - ylength > S2By))
		return FALSE;

	if ((S2By - S2Ay) > ylength + D)
		return FALSE;

	int S1Ax = v_lns[S.x()].A.x();
	int S1Bx = v_lns[S.y()].B.x();
	int S2Ax = v_lns[n].A.x();
	int S2Bx = v_lns[n].B.x();

	if (S1Bx < S1Ax) {
		if ((((S2Ax < S1Bx + D) && (S2Bx < S1Bx + D) && (S2Ay > S1By))
				|| ((S2Bx > S1Ax - D) && (S2Ax > S1Ax - D) && (S2By < S1Ay))))
			return TRUE;
	} else {
		if ((((S2Bx < S1Ax + D) && (S2Ax < S1Ax + D) && (S2By < S1Ay))
				|| ((S2Ax > S1Bx - D) && (S2Bx > S1Bx - D) && (S2Ay > S1By))))
			return TRUE;
	}
	return FALSE;
}

void InitMarkedNoise() {
	for (int i = 0; i < h_count; i++) {
		HMarkedNoise[i] = TRUE;
	}

	for (int j = 0; j < v_count; j++) {
		VMarkedNoise[j] = TRUE;
	}
}

Bool HExp(int& counter, int nl) {
	int StripCount = 0;
	int r, num;
	Point16 P, SS;
	XSEGM S;
	int NHL = h_count - 1;

	int order = YLnsOrder[nl];
	int ycoord = h_lns[nl].A.y();
	int dy = h_lns[nl].B.y() - ycoord;

	r = order;
	while (r > 0) {
		r--;
		num = YMapLnsNum[r];
		S = h_lns[num];

		if (((dy < 0) && ((S.A).y() < ycoord + dy + dy - D)) || ((!(dy < 0))
				&& ((S.A).y() < ycoord - dy - dy - D)))
			break;

		StripLine[StripCount].rx() = num;
		StripLine[StripCount].ry() = (S.A).x();
		StripCount++;
	}

	r = order;
	while (r < NHL) {
		r++;
		num = YMapLnsNum[r];
		S = h_lns[num];

		if (((dy < 0) && ((S.A).y() > ycoord - dy - dy + D)) || ((!(dy < 0))
				&& ((S.A).y() > ycoord + dy + dy + D)))
			break;

		StripLine[StripCount].rx() = num;
		StripLine[StripCount].ry() = (S.A).x();
		StripCount++;
	}

	StripLine[StripCount].rx() = nl;
	StripLine[StripCount].ry() = h_lns[nl].A.x();
	StripCount++;

	if (StripCount > 1) {
		::qsort(&(StripLine[0]), StripCount, sizeof(StripLine[0]), StrLCompare);

		for (int i = 0; i < StripCount; i++) {
			if (StripLine[i].x() == nl) {
				order = i;
				break;
			}
		}

		h1_lns[counter].set(nl, nl);
		SS.set(nl, nl);
		r = order;
		while (r > 0) {
			r--;
			num = StripLine[r].x();
			if (IsHCloseCovering(SS, num)) {
				SS.rx() = num;
				HMarkedNoise[num] = FALSE;
				if ((hlink[num]).x() == 0) {
					P.set(1, num);
					hlink[StripLine[r + 1].x()] = P;
					h1_lns[counter].rx() = num;
				}
			}
		}
		r = order;
		while (r < StripCount - 1) {
			r++;
			num = StripLine[r].x();
			if (IsHCloseCovering(SS, num)) {
				SS.ry() = num;
				HMarkedNoise[num] = FALSE;
				if (hlink[num].x() == 0) {
					P.set(1, StripLine[r - 1].x());
					hlink[num] = P;
					h1_lns[counter].ry() = num;
				}
			}
		}

		if (SS.x() != SS.y()) {
			counter++;
			return TRUE;
		} else
			return FALSE;
	} else
		return FALSE;
}

Bool VExp(int& counter, int nl) {
	int StripCount = 0;
	int r, num;
	Point16 P, SS;
	XSEGM S;
	int NVL = v_count - 1;

	int order = XLnsOrder[nl];
	int xcoord = v_lns[nl].A.x();
	int dx = v_lns[nl].B.x() - xcoord;

	r = order;
	while (r > 0) {
		r--;
		num = XMapLnsNum[r];
		S = v_lns[num];

		if (((dx < 0) && ((S.A).x() < xcoord + dx + dx - D)) || ((!(dx < 0))
				&& ((S.A).x() < xcoord - dx - dx - D)))
			break;

		StripLine[StripCount].set(num, S.A.y());
		StripCount++;
	}

	r = order;
	while (r < NVL) {
		r++;
		num = XMapLnsNum[r];
		S = v_lns[num];

		if (((dx < 0) && ((S.A).x() > xcoord - dx - dx + D)) || ((!(dx < 0))
				&& ((S.A).x() > xcoord + dx + dx + D)))
			break;

		StripLine[StripCount].set(num, S.A.y());
		StripCount++;
	}

	StripLine[StripCount].set(nl, v_lns[nl].A.y());
	StripCount++;

	if (StripCount > 1) {
		::qsort(&(StripLine[0]), StripCount, sizeof(StripLine[0]), StrLCompare);

		for (int i = 0; i < StripCount; i++) {
			if (StripLine[i].x() == nl) {
				order = i;
				break;
			}
		}

		v1_lns[counter].set(nl, nl);
		SS.set(nl, nl);

		r = order;
		while (r > 0) {
			r--;
			num = StripLine[r].x();
			if (IsVCloseCovering(SS, num)) {
				SS.rx() = num;
				VMarkedNoise[num] = FALSE;
				if (vlink[num].x() == 0) {
					P.set(1, num);
					vlink[StripLine[r + 1].x()] = P;
					v1_lns[counter].rx() = num;
				}
			}
		}
		r = order;
		while (r < StripCount - 1) {
			r++;
			num = StripLine[r].x();
			if (IsVCloseCovering(SS, num)) {
				SS.ry() = num;
				VMarkedNoise[num] = FALSE;
				if (vlink[num].x() == 0) {
					P.set(1, StripLine[r - 1].x());
					vlink[num] = P;
					v1_lns[counter].ry() = num;
				}
			}
		}
		if (SS.x() != SS.y()) {
			counter++;
			return TRUE;
		} else
			return FALSE;
	} else
		return FALSE;
}

Bool HBound(int& counter, int i) {
	int num, dnum, numAy, numBy;
	XSEGM S;
	int NVL = v_count - 1;
	int Ax = h_lns[i].A.x();
	int Ay = h_lns[i].A.y();
	int Bx = h_lns[i].B.x();
	int By = h_lns[i].B.y();
	int dx = MIN(((Bx - Ax) >> 3), DD); //+((Bx-Ax)>>3);

	int L1 = Ax - dx - D;
	int L2 = Ax + D;

	int rank = XpARank[i];
	int r = rank + 1;
	while (r > 0) {
		r--;
		num = XMapLnsNum[r];
		S = v_lns[num];

		if (S.A.x() < L1)
			break;

		numAy = S.A.y();
		numBy = S.B.y();
		dnum = MIN(((numBy - numAy) >> 3), DD);
		if ((abs(Ay - numAy) < dnum + D) || (abs(Ay - numBy) < dnum + D)) {
			if (VMarkedNoise[num]) {
				VMarkedNoise[num] = FALSE;
				v1_lns[counter].set(num, num);
				counter++;
			}
			return TRUE;
		}
	}

	r = rank;
	while (r < NVL) {
		r++;
		num = XMapLnsNum[r];
		S = v_lns[num];

		if (S.A.x() > L2)
			break;

		numAy = S.A.y();
		numBy = S.B.y();
		dnum = MIN(((numBy - numAy) >> 3), DD);
		if ((abs(Ay - numAy) < dnum + D) || (abs(Ay - numBy) < dnum + D)) {
			if (VMarkedNoise[num]) {
				VMarkedNoise[num] = FALSE;
				v1_lns[counter].set(num, num);
				counter++;
			}
			return TRUE;
		}
	}

	int R1 = Bx - D;
	int R2 = Bx + dx + D;

	rank = XpBRank[i];
	r = rank + 1;
	while (r > 0) {
		r--;
		num = XMapLnsNum[r];
		S = v_lns[num];

		if (S.A.x() < R1)
			break;

		numAy = S.A.y();
		numBy = S.B.y();
		dnum = MIN(((numBy - numAy) >> 3), DD);
		if ((abs(By - numAy) < dnum + D) || (abs(By - numBy) < dnum + D)) {
			if (VMarkedNoise[num]) {
				VMarkedNoise[num] = FALSE;
				v1_lns[counter].set(num, num);
				counter++;
			}
			return TRUE;
		}
	}

	r = rank;
	while (r < NVL) {
		r++;
		num = XMapLnsNum[r];
		S = v_lns[num];

		if (S.A.x() > R2)
			break;

		numAy = S.A.y();
		numBy = S.B.y();
		dnum = MIN(((numBy - numAy) >> 3), DD);
		if ((abs(By - numAy) < dnum + D) || (abs(By - numBy) < dnum + D)) {
			if (VMarkedNoise[num]) {
				VMarkedNoise[num] = FALSE;
				v1_lns[counter].set(num, num);
				counter++;
			}
			return TRUE;
		}
	}

	return FALSE;
}

Bool VBound(int& counter, int i) {
	int num, dnum, numAx, numBx;
	XSEGM S;
	int NHL = h_count - 1;
	int Ax = v_lns[i].A.x();
	int Ay = v_lns[i].A.y();
	int Bx = v_lns[i].B.x();
	int By = v_lns[i].B.y();
	int dy = MIN(((By - Ay) >> 3), DD);

	int L1 = Ay - dy - D;
	int L2 = Ay + D;

	int rank = YpARank[i];
	int r = rank + 1;
	while (r > 0) {
		r--;
		num = YMapLnsNum[r];
		S = h_lns[num];

		if (S.A.y() < L1)
			break;

		numAx = S.A.x();
		numBx = S.B.x();
		dnum = MIN(((numBx - numAx) >> 3), DD);
		if ((abs(Ax - numAx) < dnum + D) || (abs(Ax - numBx) < dnum + D)) {
			if (HMarkedNoise[num]) {
				HMarkedNoise[num] = FALSE;
				h1_lns[counter].set(num, num);
				counter++;
			}
			return TRUE;
		}
	}

	r = rank;
	while (r < NHL) {
		r++;
		num = YMapLnsNum[r];
		S = h_lns[num];

		if (S.A.y() > L2)
			break;

		numAx = S.A.x();
		numBx = S.B.x();
		dnum = MIN(((numBx - numAx) >> 3), DD);
		if ((abs(Ax - numAx) < dnum + D) || (abs(Ax - numBx) < dnum + D)) {
			if (HMarkedNoise[num]) {
				HMarkedNoise[num] = FALSE;
				h1_lns[counter].set(num, num);
				counter++;
			}
			return TRUE;
		}
	}

	int R1 = By - D;
	int R2 = By + dy + D;
	rank = YpBRank[i];
	r = rank + 1;
	while (r > 0) {
		r--;
		num = YMapLnsNum[r];
		S = h_lns[num];

		if (S.A.y() < R1)
			break;

		numAx = S.A.x();
		numBx = S.B.x();
		dnum = MIN(((numBx - numAx) >> 3), DD);
		if ((abs(Bx - numAx) < dnum + D) || (abs(Bx - numBx) < dnum + D)) {
			if (HMarkedNoise[num]) {
				HMarkedNoise[num] = FALSE;
				h1_lns[counter].set(num, num);
				counter++;
			}
			return TRUE;
		}
	}

	r = rank;
	while (r < NHL) {
		r++;
		num = YMapLnsNum[r];
		S = h_lns[num];

		if (S.A.y() > R2)
			break;

		numAx = S.A.x();
		numBx = S.B.x();
		dnum = MIN(((numBx - numAx) >> 3), DD);
		if ((abs(Bx - numAx) < dnum + D) || (abs(Bx - numBx) < dnum + D)) {
			if (HMarkedNoise[num]) {
				HMarkedNoise[num] = FALSE;
				h1_lns[counter].set(num, num);
				counter++;
			}
			return TRUE;
		}
	}

	return FALSE;
}

void NoiseSelect() {
	h1_count = 0;
	v1_count = 0;

	for (int i = 0; i < h_count; i++) {
		if (HMarkedNoise[i]) {
			if (HExp(h1_count, i))
				HMarkedNoise[i] = FALSE;
			else {
				if (HLength(&h_lns[i]) > 100) {
					HMarkedNoise[i] = FALSE;
					h1_lns[h1_count].set(i, i);
					h1_count++;
				}
			}
		}
	}

	for (int j = 0; j < v_count; j++) {
		if (VMarkedNoise[j]) {
			if (VExp(v1_count, j))
				VMarkedNoise[j] = FALSE;
			else {
				if (VLength(&v_lns[j]) > 50) {
					VMarkedNoise[j] = FALSE;
					v1_lns[v1_count].set(j, j);
					v1_count++;
				}
			}
		}
	}

	for (int l = 0; l < h_count; l++) {
		if (!HMarkedNoise[l])
			continue;
		if (HBound(v1_count, l)) {
			HMarkedNoise[l] = FALSE;
			h1_lns[h1_count].set(l, l);
			h1_count++;
		}
	}

	for (int j = 0; j < v_count; j++) {
		if (!VMarkedNoise[j])
			continue;
		if (VBound(h1_count, j)) {
			VMarkedNoise[j] = FALSE;
			v1_lns[v1_count].set(j, j);
			v1_count++;
		}
	}
}

//-***************** calculate the skew ******************

int SkewCalc() {
	int dx;
	if (h_count > 0) {
		long Dx = h_lns[0].B.x() - h_lns[0].A.x();
		long Dy = h_lns[0].B.y() - h_lns[0].A.y();
		if (abs(Dy) < 2)
			dx = (Dx >> 1);
		else
			dx = Dx / (abs(Dy));
		for (int n = 1; n < (h_count >> 1); n++) {
			if (HLength(&(h_lns[n])) < dx)
				break;
			Dx = Dx + h_lns[n].B.x() - h_lns[n].A.x();
			Dy = Dy + h_lns[n].B.y() - h_lns[n].A.y();
		};

		if (Dx == 0) {
			skew = 0;
		} else
			skew = (Dy > 0) ? ((Dy << 10) + (Dx >> 1)) / Dx : ((Dy << 10) - (Dx
					>> 1)) / Dx;
	} else
		return 0;

	CONSOLE
	("skew:  %d", skew);
	return skew;
}

int RectSkew() {
	int dx, ddx, ddy;
	if (h_count > 0) {
		long Dx = h_lns[0].B.x() - h_lns[0].A.x();
		long Dy = h_lns[0].B.y() - h_lns[0].A.y();
		if (abs(Dy) < 2)
			dx = (Dx >> 1);
		else
			dx = Dx / (abs(Dy));
		Dx = 0;
		Dy = 0;
		for (int n = 0; n < (h_count >> 1); n++) {
			if (HLength(&(h_lns[n])) < dx)
				break;
			ddy = h_lns[n].B.y() - h_lns[n].A.y();
			ddx = h_lns[n].B.x() - h_lns[n].A.x();
			if ((abs(ddy) > 10) && ((ddx >> 10) > 0) && (abs(ddy / (ddx >> 10))
					> 20))
				continue;
			Dx = Dx + ddx;
			Dy = Dy + ddy;
		};

		if (Dx == 0) {
			skew = 0;
		} else
			skew = (Dy > 0) ? ((Dy << 10) + (Dx >> 1)) / Dx : ((Dy << 10) - (Dx
					>> 1)) / Dx;
	} else
		return 0;CONSOLE
	("rect skew:  %d", skew);
	return skew;
}

void Init_hlink_vlink() {
	for (int i = 0; i < h_count; i++) {
		hlink[i].rx() = 0;
		h1_lns[i].set(i, i);
		AdjacentLst& L = (((Lti->Hor).Lns)[i]).Adj;
		L.ltiNext = -1;
		L.ltiPrev = -1;
		L.ltiFirst = -1;
		L.ltiLast = -1;
	};
	for (int j = 0; j < v_count; j++) {
		vlink[j].rx() = 0;
		v1_lns[j].set(j, j);
		AdjacentLst& L = (((Lti->Ver).Lns)[j]).Adj;
		L.ltiNext = -1;
		L.ltiPrev = -1;
		L.ltiFirst = -1;
		L.ltiLast = -1;
	};
}

//-********************** main procedure ********************

void Refine() {

	CorrectDirection();

	LengthSort();

	SkewCalc();
	(Lti->Skew1024) = skew;

	if ((abs(skew) >> 5) > 0) {
		int s = skew >> 1;
		//  CONSOLE("s =  %d", s);
		Rotate(s);
		SkewCalc();
	}

	Rotate(skew);
	int s = RectSkew();
	(Lti->Skew1024) += s;
	if (abs(s) > 5)
		Rotate(s);

	InitMapping();
	SortMapping();

	InitMappingInverse();

	InitMarkedNoise();
	Init_hlink_vlink();
	NoiseSelect();

	SkewCalc();

}

//-***********************   Lti filling   ************************

void FillFlag() {
	LineInfo* ha = Lti->Hor.Lns;
	LineInfo* va = Lti->Ver.Lns;
	//	Lti->Hor.NoiseCnt = 0;
	//	Lti->Ver.NoiseCnt = 0;
	int i(0);
	for (i = 0; i < h_count; i++) {
		if (HMarkedNoise[i]) {
			ha[(h_lns[i]).C].Flags |= LI_NOISE;
			//				Lti->Hor.NoiseCnt ++;
		};
	};

	for (i = 0; i < v_count; i++) {
		if (VMarkedNoise[i]) {
			va[(v_lns[i]).C].Flags |= LI_NOISE;
			//				Lti->Ver.NoiseCnt ++;
		};
	};
}

void FillRotatedCoord() {
	int i(0);
	for (i = 0; i < h_count; i++) {
		LineInfo& L = ((Lti->Hor).Lns)[(h_lns[i].C)];
		if (L.TmpUsage == 0) {
			L.Ar = h_lns[i].A;
			L.Br = h_lns[i].B;
		} else {
			L.Ar = h_lns[i].B;
			L.Br = h_lns[i].A;
		};
	};

	for (i = 0; i < v_count; i++) {
		LineInfo& L = ((Lti->Ver).Lns)[(v_lns[i].C)];
		if (L.TmpUsage == 0) {
			L.Ar = v_lns[i].A;
			L.Br = v_lns[i].B;
		} else {
			L.Ar = v_lns[i].B;
			L.Br = v_lns[i].A;
		};
	};
}

Bool FillAdjacent() {
	int next = -1;
	int l, f, lC, fC, cur, curC, Init, adnum;
	int i(0);
	int s(0);
	int k(0);

	if ((h_count == 0) && (v_count == 0))
		return TRUE;

	Bool x = ADJA.create(MAX(h_count, v_count));
	if (!x)
		return FALSE;

	for (s = 0; s < h_count; s++) {
		ADJA[s].first = -1;
		ADJA[s].last = -1;
		ADJA[s].prev = -1;
		ADJA[s].next = -1;
	};

	for (i = 0; i < h1_count; i++) {
		l = h1_lns[i].y();
		f = h1_lns[i].x();
		lC = h_lns[l].C;
		fC = h_lns[f].C;

		cur = l;
		curC = lC;
		Init = hlink[cur].x();

		adnum = curC;

		//   AdjacentLst& L = (((Lti->Hor).Lns)[curC]).Adj;
		//   L.ltiLast  = lC;
		//   L.ltiFirst = fC;
		ADJA[curC].last = lC; //L.ltiLast;
		ADJA[curC].first = fC; //L.ltiFirst;
		if (Init != 0) {
			next = cur;
			cur = hlink[cur].y();
			curC = h_lns[cur].C;
			//     L.ltiPrev = curC;
			ADJA[adnum].prev = curC; //L.ltiPrev;
			adnum = curC;
		};

		while (hlink[cur].x() != 0) {
			//     L = (((Lti->Hor).Lns)[curC]).Adj;
			adnum = curC;
			//     L.ltiLast  = lC;
			//     L.ltiFirst = fC;
			ADJA[adnum].last = lC; //L.ltiLast;
			ADJA[adnum].first = fC; //L.ltiFirst;
			//     L.ltiNext  = h_lns[next].C;
			ADJA[adnum].next = h_lns[next].C; //L.ltiNext;
			next = cur;
			cur = hlink[cur].y();
			curC = h_lns[cur].C;
			//     L.ltiPrev = curC;
			ADJA[adnum].prev = curC; //L.ltiPrev;
		};

		adnum = curC;
		//   L = (((Lti->Hor).Lns)[curC]).Adj;
		//   L.ltiLast  = lC;
		//   L.ltiFirst = fC;
		ADJA[adnum].last = lC; //L.ltiLast;
		ADJA[adnum].first = fC; //L.ltiFirst;

		if (Init != 0)
			ADJA[adnum].next = /*  L.ltiNext */h_lns[next].C;
	};

	for (k = 0; k < h_count; k++) {
		AdjacentLst& L = (((Lti->Hor).Lns)[k]).Adj;
		L.ltiLast = ADJA[k].last;
		L.ltiFirst = ADJA[k].first;
		L.ltiNext = ADJA[k].next;
		L.ltiPrev = ADJA[k].prev;
	};

	for (s = 0; s < v_count; s++) {
		ADJA[s].first = -1;
		ADJA[s].last = -1;
		ADJA[s].prev = -1;
		ADJA[s].next = -1;
	};
	for (i = 0; i < v1_count; i++) {
		l = v1_lns[i].y();
		f = v1_lns[i].x();
		lC = v_lns[l].C;
		fC = v_lns[f].C;

		cur = l;
		curC = v_lns[cur].C;
		adnum = curC;
		Init = vlink[cur].x();

		//   AdjacentLst& L = (((Lti->Ver).Lns)[curC]).Adj;
		//   L.ltiLast  = lC;
		//   L.ltiFirst = fC;
		ADJA[curC].last = lC; //L.ltiLast;
		ADJA[curC].first = fC; //L.ltiFirst;
		if (Init != 0) {
			next = cur;
			cur = vlink[cur].y();
			curC = v_lns[cur].C;
			ADJA[adnum].prev = curC;
		};

		while (vlink[cur].x() != 0) {
			adnum = curC;
			ADJA[adnum].last = lC; //L.ltiLast;
			ADJA[adnum].first = fC; //L.ltiFirst;
			ADJA[adnum].next = v_lns[next].C; //L.ltiNext;
			next = cur;
			cur = vlink[cur].y();
			curC = v_lns[cur].C;
			ADJA[adnum].prev = curC;
		};

		adnum = curC;
		//   L = (((Lti->Ver).Lns)[curC]).Adj;
		//   L.ltiLast  = lC;
		//   L.ltiFirst = fC;
		ADJA[adnum].last = lC; //L.ltiLast;
		ADJA[adnum].first = fC; //L.ltiFirst;
		if (Init != 0)
			ADJA[adnum].next = /* L.ltiNext */v_lns[next].C;
	};

	for (k = 0; k < v_count; k++) {
		AdjacentLst& L = (((Lti->Ver).Lns)[k]).Adj;
		L.ltiLast = ADJA[k].last;
		L.ltiFirst = ADJA[k].first;
		L.ltiNext = ADJA[k].next;
		L.ltiPrev = ADJA[k].prev;

	};

	ADJA.flush();
	return TRUE;
}
//-********************   Library functions   *************************

// --------------------   LNSCheck functions  -------------------------

Bool LCEXPORT LC_Init(LinesTotalInfo* lti) {
	Lti = lti;

	h_count = (lti->Hor).Cnt;
	v_count = (lti->Ver).Cnt;

	if (h_count < 0 || v_count < 0)
		return FALSE;

	int hcn = MAX(h_count, 1);
	int vcn = MAX(v_count, 1);
	Bool x = YMapping.create(hcn + (vcn << 1)) && XMapping.create(vcn + (hcn
			<< 1)) && StripLine.create(MAX(hcn, vcn)) && YpARank.create(vcn)
			&& YpBRank.create(vcn) && YLnsOrder.create(hcn)
			&& YMapLnsNum.create(hcn) && XpARank.create(hcn) && XpBRank.create(
			hcn) && XLnsOrder.create(vcn) && XMapLnsNum.create(vcn)
			&& HMarkedNoise.create(hcn) && VMarkedNoise.create(vcn)
			&& h_lns.create(hcn) && v_lns.create(vcn) && h1_lns.create(hcn)
			&& v1_lns.create(vcn) && hlink.create(hcn) && vlink.create(vcn);

	if (!x)
		CONSOLE("Cannot create PPS arrays");

	return x;
}

Bool LCEXPORT LC_Done(LinesTotalInfo* /*lti*/) {
	YMapping.flush(); //      &&
	XMapping.flush(); //      &&
	StripLine.flush(); //     &&
	YpARank.flush(); //       &&
	YpBRank.flush(); //       &&
	YLnsOrder.flush(); //     &&
	YMapLnsNum.flush(); //    &&
	XpARank.flush(); //       &&
	XpBRank.flush(); //       &&
	XLnsOrder.flush(); //     &&
	XMapLnsNum.flush(); //    &&
	HMarkedNoise.flush(); //  &&
	VMarkedNoise.flush(); //  &&
	h1_lns.flush(); //        &&
	v1_lns.flush(); //        &&
	hlink.flush(); //         &&
	vlink.flush();

	return TRUE;
}

Bool LCEXPORT LC_MarkBadLines(LinesTotalInfo* lti) {
	D = (lti->LCS).NeglectedDelta;
	DD = (lti->LCS).MaxCornerGap;
	if (D == 0)
		D = 5;
	if (DD == 0)
		DD = 20;

	int i(0);
	for (i = 0; i < h_count; i++) {
		(h_lns[i]).A = (((lti->Hor).Lns)[i]).A;
		(h_lns[i]).B = (((lti->Hor).Lns)[i]).B;
		(h_lns[i]).C = i;
	};

	for (i = 0; i < v_count; i++) {
		(v_lns[i]).A = (((lti->Ver).Lns)[i]).A;
		(v_lns[i]).B = (((lti->Ver).Lns)[i]).B;
		(v_lns[i]).C = i;
	};

	Refine();

	FillFlag();
	FillRotatedCoord();
	FillAdjacent();
	return TRUE;
}

Bool LCEXPORT LC_CorrectSkew(LinesTotalInfo* lti) {
	h_count = (lti->Hor).Cnt;
	v_count = (lti->Ver).Cnt;

	if (h_count < 0 || v_count < 0)
		return FALSE;
	if (h_count == 0 && v_count == 0)
		return TRUE;

	h_lns.flush();
	v_lns.flush();

	Bool x;
	if (h_count > 0)
		x = h_lns.create(h_count);
	if (!x)
		return FALSE;
	if (v_count > 0)
		x = v_lns.create(v_count);
	if (!x)
		return FALSE;

	Lti = lti;
	int i(0);
	for (i = 0; i < h_count; i++) {
		(h_lns[i]).A = (((lti->Hor).Lns)[i]).A;
		(h_lns[i]).B = (((lti->Hor).Lns)[i]).B;
		(h_lns[i]).C = i;
	};

	for (i = 0; i < v_count; i++) {
		(v_lns[i]).A = (((lti->Ver).Lns)[i]).A;
		(v_lns[i]).B = (((lti->Ver).Lns)[i]).B;
		(v_lns[i]).C = i;
	};
	CorrectDirection();

	LengthSort();

	SkewCalc();
	(Lti->Skew1024) = skew;

	if ((abs(skew) >> 5) > 0) {
		int s = skew >> 1;
		//  CONSOLE("s =  %d", s);
		Rotate(s);
		SkewCalc();
	}

	Rotate(skew);
	int s = RectSkew();
	(Lti->Skew1024) += s;
	if (abs(s) > 5)
		Rotate(s);

	FillRotatedCoord();

	h_lns.flush();
	v_lns.flush();
	return TRUE;
}

