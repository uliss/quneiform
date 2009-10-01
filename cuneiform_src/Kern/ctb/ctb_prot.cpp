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

/***************************************************************************/
/* CTB_PROT : read & write LEMAN protocol file                             */
/***************************************************************************/

#define PROTOCOL_BUFFER_SIZE	2048*4
#define OUT_BUFFER_SIZE		    8*2048
#define SHOW1

#include <stdio.h>
#include <string.h>

#include "ctb.h"
#include "cfcompat.h"

static int my_fgets(void);
static void str_to_bmp(char *buf, unsigned char *rast, int h, int ww,
		int w_last);
static int get_raster_from_prot(void);

static unsigned char raster[OUT_BUFFER_SIZE], data[CTB_DATA_SIZE];
static int h, w, www;
static unsigned char let;
static char buffer[PROTOCOL_BUFFER_SIZE];
static char sdata[PROTOCOL_BUFFER_SIZE];
static FILE *f = NULL;

Bool32 CTB_prot_open(char *name) {
	f = fopen(name, "rt");
	return (f != NULL);
}

void CTB_prot_close(void) {
	fclose(f);
	f = NULL;
}

Bool32 CTB_prot_read(unsigned char *PROT_raster, int *PROT_w, int *PROT_h,
		unsigned char *PROT_let, unsigned char *PROT_data) {
	if (!get_raster_from_prot())
		return 0;
	if (w > BASE_MAX_W)
		return 0;
	if (h > BASE_MAX_H)
		return 0;

	memcpy(PROT_raster, raster, ((w + 7) / 8) * h);
	memcpy(&PROT_data[4], data, 26/*CTB_DATA_SIZE*/);
	*PROT_w = w;
	*PROT_h = h;
	*PROT_let = let;
	return 1;
}

// used alignment of width
Bool32 CTB_prot_gray_read(unsigned char *PROT_raster, unsigned char *PROT_data) {
	int h, w, wb, i, ii, j, jj, dig;
	unsigned char let;
	char tmp[3] = { 0 };

	memset(&PROT_data[0], 0, CTB_DATA_SIZE);
	if (fgets(buffer, sizeof(buffer), f) == NULL)
		return FALSE;
	if (sscanf(buffer, "%c %d %d", &let, &w, &h) != 3)
		return FALSE;
	PROT_data[1] = w;
	PROT_data[2] = h;
	PROT_data[3] = let;

	if (w > BASE_MAX_W)
		return FALSE;
	if (h > BASE_MAX_H)
		return FALSE;
	wb = ((w + 7) / 8) * 8;
	if (wb * h > PROTOCOL_BUFFER_SIZE / 2)
		return FALSE;

	memset(&PROT_raster[0], 0, wb * h);
	for (ii = i = 0; i < h; i++, ii += wb) {
		if (fgets(buffer, sizeof(buffer), f) == NULL)
			return FALSE;
		for (jj = j = 0; j < wb; j++, jj += 2) {
			tmp[0] = buffer[jj];
			tmp[1] = buffer[jj + 1];
			if (sscanf(tmp, "%x", &dig) != 1)
				return FALSE;
			PROT_raster[ii + j] = (unsigned char) dig;
		}
	}
	PROT_data[1] = wb;

	return TRUE;
}

Bool32 CTB_prot_gray_write(char *file_name, unsigned char *PROT_raster,
		unsigned char let, int w, int h, int align) {
	int wb, wb8, i, ii, j;
	char tmp[3] = { 0 };
	char prot[256];
	FILE *ev = NULL;

	switch (align) {
	case 1:
		wb = w;
	case 2:
		wb = ((w + 1) / 2) * 2;
	case 4:
		wb = ((w + 3) / 4) * 4;
	case 8:
		wb = ((w + 7) / 8) * 8;
		break;
	default:
		return FALSE;
	}
	wb8 = ((w + 7) / 8) * 8;
	strcpy(prot, file_name);
	strlwr(prot);
	if (!strstr(prot, ".gra"))
		strcat(prot, ".gra");
	ev = fopen(prot, "at");
	if (ev == NULL)
		return FALSE; // can't open

	if (w > BASE_MAX_W || h > BASE_MAX_H || wb * h > PROTOCOL_BUFFER_SIZE / 2) {
		fclose(ev);
		return 0;
	}
	fprintf(ev, "%c %d %d\n", let, w, h);

	memset(&buffer[0], '0', wb8 * 2);
	buffer[wb8 * 2] = 0;
	for (ii = i = 0; i < h; i++, ii += wb) {
		buffer[0] = 0;
		for (j = 0; j < wb; j++) {
			sprintf(tmp, "%02x", PROT_raster[ii + j]);
			buffer[j * 2] = tmp[0];
			buffer[j * 2 + 1] = tmp[1];
		}
		fprintf(ev, "%s\n", buffer);
	}
	fclose(ev);
	return TRUE;
}

Bool32 CTB_prot_write(char *file_name, unsigned char *raster, int lnPixWidth,
		int lnPixHeight, unsigned char code, int align, unsigned char *data) {
	short int byteLen, byteMmx, height, j, i;
	FILE *ev = NULL;
	char digit[] = "0123456789abcdef";
	unsigned char tmp;
	char prot[256];

	if (lnPixWidth > BASE_MAX_W)
		return 0; // too wide
	if (lnPixHeight > BASE_MAX_H)
		return 0; // too high

	strcpy(prot, file_name);
	strlwr(prot);
	if (!strstr(prot, ".pro"))
		strcat(prot, ".pro");
	ev = fopen(prot, "at");
	if (ev == NULL)
		return 0; // can't open

	byteLen = (lnPixWidth + 7) / 8;
	switch (align) {
	case 8:
		byteMmx = ((lnPixWidth + 63) / 64) * 8;
		break;
	case 1:
	default:
		byteMmx = (lnPixWidth + 7) / 8;
		break;
	}
	height = (short int) lnPixHeight;
	if (data) {
		for (i = j = 0; j < 16; j++, i += 2) {
			tmp = data[j];
			tmp = tmp >> 4;
			prot[i + 0] = digit[tmp];
			tmp = data[j];
			tmp = tmp & 15;
			prot[i + 1] = digit[tmp];
		}
		prot[32] = 0;
	} else
		prot[0] = 0;

	if (fprintf(ev, "-2 '%c' 1 \n00 %d\n-1 %d %d\n", code,/*prot,*/code,
			byteLen, height) == 0)
		return 0; // can't write

	for (i = 0; i < height; i++, raster += byteMmx) {
		for (j = 0; j < byteLen; j++) {
			tmp = raster[j];
			tmp = tmp >> 4;
			fputc(digit[tmp], ev);
			tmp = raster[j];
			tmp = tmp & 15;
			fputc(digit[tmp], ev);
		}
	}

	fputc('\n', ev);
	fclose(ev);

	return 1;
}

int my_fgets(void) {
	if (fgets(buffer, sizeof(buffer), f) == NULL)
		return 0;
	if (buffer[strlen(buffer) - 1] == '\n')
		buffer[strlen(buffer) - 1] = 0;
	return 1;
}

void str_to_bmp(char *buf, unsigned char *raster, int h, int ww, int w_last) {
	char c[3];
	int i, j;

	for (i = 0; i < h; i++) {
		for (j = 0; j < ww; j += 2) {
			c[0] = buf[i * ww + j];
			c[1] = buf[i * ww + j + 1];
			c[2] = 0;
			sscanf(c, "%x", raster++);
		}
		raster += w_last;
	}
	return;
}

int column(unsigned char *raster, unsigned char mask, int wb, int h) {
	int j;
	for (j = 0; j < h; j++, raster += wb)
		if ((*raster) & mask)
			return 1;
	return 0;
}

int width(unsigned char *raster, int wb, int wid, int h) {
	unsigned char mask[] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };
	int i;
	raster += wb;
	for (i = 7; i >= 0; i--)
		if (column(raster, mask[i], wid, h))
			return wb * 8 + i + 1;
	return 0;
}

int get_raster_from_prot() {
	int l;

	int ww, w_, w_last;

	memset(data, 0, CTB_DATA_SIZE);
	w = h = 0;
	if (!my_fgets())
		return 0;
	l = sscanf(buffer, "%s %s %s %s", sdata, sdata, sdata, sdata);
	if (l == 4 && strlen(sdata) == 52)
		str_to_bmp(sdata, data, 1, 52, 52);
	if (!my_fgets())
		return 0;
	sscanf(buffer, "%d %d", &l, &let);
	if (!my_fgets())
		return 0;
	sscanf(buffer, "%d %d %d", &l, &w_, &h);
	w_ <<= 3;
	if ((w_ / 4) * h > sizeof(buffer))
		return 0;
	if (!my_fgets())
		return 0;
	ww = strlen(buffer) / h;
	w = ((w_ + 7) / 8) * 8;
	www = (w + 7) / 8;
	w_last = www - ww / 2;
	memset(raster, 0, h * (w / 8));
	str_to_bmp(buffer, raster, h, ww, w_last);

	w = width(raster, (w_ >> 3) - 1, www, h);

	return 1;
}
