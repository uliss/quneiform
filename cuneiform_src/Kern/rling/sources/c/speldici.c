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

/**************************************************************************/
/*                         -- Updation history. --                        */
/**************************************************************************/
/* 17-02-93 03:36pm, Mike - File has been commented once more.            */
/* 04-01-93 09:49pm, Mike - New dictionary file format.                   */
/**************************************************************************/
//
// 08-13-93 06:33pm, Mike.
// Comments:  Updating for multi-lingual version of CuneiForm ( release
//            28-June-93 ). Release has been received from Joe by FEDERAL
//            mail.
//
// 1. Function < uchar * load_stat_dict ( char  *point ) > has
//    been rewritten to use new strems technology.
//    Parameter CountryCode in the load_stat_dict has been removed.
//    The global variable uchar language is used now to check current
//    language settings. All IO operations have been changed to use
//    data streams from IOLIB.H.
// 2. Functions < void user_voc_init (void) > and < static void
//    unload_user_dicts(void) > have been added after Joe.
// 3. Function < void load_user_dicts ( char * list_name, char  *
//    point) > has been rewritten after Joe.
// 4. Function < int32_t read_all_voc( int16_t seqn, char *name, char  *p ) >
//    has been removed with new streams technology.
//
// 08-14-93 06:01pm, Mike
//
// 5. Function < uchar * load_stat_dict ( char  *point ) > has
//    been changed to load ALL tables at once. Speller's standalone module
//    MAIN.C need to be changed !!!
//
// Process indicator:
//  1. Preliminary updation. 08-13-93 06:40pm, Mike
//
/**************************************************************************/

/**************************************************************************/
/***********    Definitions section.    ***********************************/
/**************************************************************************/
#include <string.h>
#include <stdio.h>

#include "spelmode.h"

#include "recdefs.h"
#include "lang.h"

#include "speldefs.h"
#include "spelfunc.h"
#include "udicfunc.h"
#include "tigeremulate.h"

#ifndef MAXPATH
#define MAXPATH 128
#endif

/**************************************************************************/
/***********      Export section.       ***********************************/
/**************************************************************************/
/* -- Code -- */

// 08-13-93 08:13pm, Mike
// Parameter CountryCode has been removed.
// 12-09-93 ATAL
// This function have tu return proper pointer, never NULL !
void * (*my_alloc)(uint32_t len);
void (*my_free)(void *);
void ErrorExit(int Code);
//uint32_t  LoadUserDict( char*, char*, uint32_t, voc_state*);

uchar * load_stat_dict(char *point);
/*---------- Updated : 04-01-93 09:46pm, Mike ------
 Function loads static dictionary file into far memory location
 <point> The side effect is initialization of decoder table
 by call of function dectable_init(). New static dictionary
 format is used : all tables in the one file.
 Returns far pointer to the next available memory block.
 --------------------------------------------------*/

void init_stat_dict(struct dict_state * dict);
/*-----------------17-02-93 03:21pm-----------------
 Function copies static dictionary control structure from far memory
 <load_dict> into NEAR control structure <dict>.
 --------------------------------------------------*/

// 08-13-93 08:55pm, Mike
// Return type has been changed from <uchar  *> to <void>
void load_user_dicts(char * list_name, char * point);
/*-----------------17-02-93 03:30pm-----------------
 Function loads user's dictionaries into memory by
 list of vocs.
 --------------------------------------------------*/
void unload_user_dicts(void);

// 08-13-93 06:35pm, Mike after Joe...
extern int32_t read_all_vtab(int16_t seqn, char *p);

/* -- Data -- */

struct dict_state * load_dict = NULL; //Allex 09.07.98
/*-----------------17-02-93 02:49pm-----------------
 Global work pointer to static dictionary control structure.
 --------------------------------------------------*/

user_voc voc_array[MAX_VOC_NUMBER];
int16_t real_voc_no = 0;

// 08-13-93 06:35pm, Mike after Joe...
extern int16_t vocs_NOK;

/**************************************************************************/
/***********      Import section.       ***********************************/
/**************************************************************************/
/* -- Code -- */

int16_t cond_open(int16_t seqn, puchar name, uint16_t b1, uint16_t b2);
puchar seq_nam(int16_t seqn);
pchar full_name(puchar w, puchar n);

/* -- Data -- */

extern uchar alphabet[][ABCSIZE];

extern char tiger_dir[40];

/**************************************************************************/
/***********      Locals section.     *************************************/
/**************************************************************************/
/* -- Code -- */

// 08-13-93 06:32pm, Mike
//  int32_t  read_all_voc(int16_t seqn, char *name, char  *p);
//    /*-----------------17-02-93 02:27pm-----------------
//     Function reads an dictionary file with name <name>
//     or with number <seqn> into far memory location <p>.
//     --------------------------------------------------*/

static int16_t parce_voc_list_record(char * w, char * nm, int16_t *type);

/**************************************************************************/
/***********      Code section.      **************************************/
/**************************************************************************/

uchar * load_stat_dict(char *point)
/*---------- Updated : 04-01-93 09:46pm, Mike ------
 Function loads static dictionary file into far memory location
 <point> The side effect is initialization of decoder table
 by call of function dectable_init(). New static dictionary
 format is used : all tables in the one file.
 Returns far pointer to the next available memory block.
 --------------------------------------------------*/
{
	int32_t size;
	PTDictState dict;
	PTDictHeaderMask dictHdr;
	char nearBuf[65];
	uint32_t treeLength, tailsLength;
	uint32_t rulesLength, hushLength;

	if (!InitializeAlphabet(language)) {
		ErrorExit(RLING_ERROR_CANT_OPEN_TABLE);
	}

	/* -- Initialize control pointers. -- */
	load_dict = dict = (PTDictState) point;
	point += sizeof(TDictState);

	/* -- Read whole static dictionary file. -- */
	size = read_all_vtab(7, point);
	if (size == -1) {
#ifdef SYSPR_ERROR
		PRINTF("Unable to open TREE.VOC \n");
#endif
		ErrorExit(RLING_ERROR_CANT_OPEN_TABLE);
		//return (uchar  *)dict;
	} else {
		dict->size = size;
		dictHdr = (PTDictHeaderMask) point;
	}

	/* -- Check correctness of file header. -- */
	memcpy(nearBuf, dictHdr->sign, sizeof(dictHdr->sign));
	if (memcmp(nearBuf, STAT_DICT_SIGN, sizeof(STAT_DICT_SIGN))) {
		return (uchar *) dict;
	}

	/* -- Check CPU type for current data. -- */
#ifdef PC_TYPE
	if ( dictHdr->cpuType[0] != 'I' ) {
		return (uchar *)dict;
	}
#endif
#ifdef __MAC__
	if ( dictHdr->cpuType[0] != 'M' ) {
		return (uchar *)dict;
	}
#endif

	/* -- Get data section sizes. -- */
	treeLength = strtoul(dictHdr->treeLength, NULL, 10);
	tailsLength = strtoul(dictHdr->tailsLength, NULL, 10);
	rulesLength = strtoul(dictHdr->rulesLength, NULL, 10);
	hushLength = strtoul(dictHdr->hushLength, NULL, 10);

	/* -- Get alphabet size. -- */
	size = strtoul(dictHdr->abcSize, NULL, 10);
	if (size > 64) {
		return (uchar *) dict;
	} else {
		dict->abcSize = (uint16_t) size;
	}

	/*  -- Load decode tables. -- NEED TO THINK ABOUT !!!
	 --
	 --  memcpy( nearBuf, dictHdr->abcUpper, sizeof(dictHdr->abcUpper));
	 --  memcpy( alphabet[0], nearBuf, sizeof(dictHdr->abcUpper) );
	 --  memcpy( nearBuf, dictHdr->abcLower, sizeof(dictHdr->abcLower));
	 --  memcpy( alphabet[1], nearBuf, sizeof(dictHdr->abcLower) );
	 --
	 */

	/* -- Initialize decoder tables. */
	dectable_init();

	/* -- Set pointers for access procedure. -- */
	dict->root = (uchar *) dictHdr + sizeof(TDictHeaderMask);
	dict->tailset_root = (uchar *) dict->root + treeLength;
	dict->vartable = (PTTailVar)((uchar *) dict->tailset_root + tailsLength);
	dict->table = (PTShiftType)((uchar *) dict->vartable + rulesLength);

	/* -- Check size corectness. -- */
	size = treeLength + tailsLength + rulesLength + hushLength
			+ sizeof(TDictHeaderMask);

	if ((int32_t) dict->size != size) {
		return (uchar *) dict;
	}

	// 08-14-93 05:50pm, Mike
	//  Bolvano ( after Lepik )!!!!
	//  If you want to use .ART and so on you need to load these files!!!!!

	//  else {
	//    return dict->root + dict->size;
	//  }

	// Here is the new version of dictionary loading.

	point = dict->root + dict->size; // temp: for return value counting.

	if ((point = load_specABC(point, language)) == NULL) {
#ifdef SYSPR
		PRINTF( "\n SPELLER: Unable to open Special voc...\n");
#endif
		return (uchar *) dict;
	}

	if (!loadArtBase(language)) {
#ifdef SYSPR
		PRINTF( "\n SPELLER: Problems to load special"
				" replacement table...\n"
		);
#endif
		return (uchar *) dict;
	}

	return point;
}

/* ------------------------------------------------------------------ */
// 08-13-93 05:37pm, Mike
// Not needed with IOLIB.H
//
//int32_t read_all_voc( int16_t seqn, char *name, char  *p )
//    /*-----------------17-02-93 02:27pm-----------------
//     Function reads an dictionary file with name <name>
//     or with number <seqn> into far memory location <p>.
//     --------------------------------------------------*/
//{
//  int32_t l;
//  uchar w[MAXPATH];
//
//  full_name( w, (puchar)name );
//  l = read_all_file( (char *)w, p );
//  if ( l <= 0 ) {
//    full_name( w, (puchar)seq_nam( seqn ));
//    l =  read_all_file( (char *)w, p );
//  }
//
//  return l;
//}
//
/* ------------------------------------------------------------------ */

void init_stat_dict(struct dict_state * dict)
/*-----------------17-02-93 03:21pm-----------------
 Function copies static dictionary control structure from far memory
 <load_dict> into NEAR control structure <dict>.
 --------------------------------------------------*/
{
	memcpy(dict, load_dict, sizeof(struct dict_state));
}

/* ------------------------------------------------------------------ */
// 08-13-93 05:54pm, Mike
// From Joe...

void user_voc_init(void) {
	real_voc_no = 0;
}

/* ------------------------------------------------------------------ */
// 08-13-93 05:54pm, Mike
// From Joe...

void unload_user_dicts(void) {
	int i;
	for (i = 0; i < real_voc_no; i++) {
		my_free(voc_array[i].voc.vocseg/*,0*/);
	}
	real_voc_no = 0;
}

/* ------------------------------------------------------------------ */

#define VOCMEMSIZE 0x10000L     /* 64K */
// old version : read list of vocs from disk file USER.LST

void load_user_dicts_kzl(char * list_name, char * point)
/*-----------------17-02-93 03:30pm-----------------
 Function loads user's dictionaries into memory by
 list of vocs.
 --------------------------------------------------*/
{
	char w[MAXPATH], nm[MAXPATH];
	FILE * lst;
	int16_t type;
	int16_t errorNo = 0;

	unload_user_dicts();

	lst = fopen(list_name, "rt");
	if (lst < 0) {
		return;
	}

	while (fgets(w, MAXPATH, lst) != NULL) {

		if (real_voc_no == MAX_VOC_NUMBER) {
			errorNo = VOC_TOOLARGELIST;
			break;
		}

		if (!parce_voc_list_record(w, nm, &type)) {
			continue;
		}

		if ((point = my_alloc(VOCMEMSIZE)) == NULL) {
			errorNo = VOC_NOTLOADED;
			break;
		}

		if (LoadUserDict(nm, point, VOCMEMSIZE, &(voc_array[real_voc_no].voc))
				== 0L) {
			my_free(point/*, 0*/);
			errorNo = VOC_NOTLOADED;
			break;
		}

		real_voc_no++;
	}

	fclose(lst);
	if (errorNo != 0) {
		ErrorExit( /*ERR_voc,*/errorNo);
	}

	return;
}
//////////////////////////////////////////////////////////////////////////////
// list_of_name if concat many vocs name, cutting '\0', last limit is "\0\0"
/*-----------------17-02-93 03:30pm-----------------
 Function loads user's dictionaries into memory using
 list of names of vocabularies(in list_of_names).
 --------------------------------------------------*/
void load_user_dicts(char * list_of_names, char * point) {
	int16_t type;
	int16_t errorNo = 0;
	char nm[MAXPATH];

	unload_user_dicts();

	while ((*list_of_names) != 0) {
		if (real_voc_no == MAX_VOC_NUMBER) {
			errorNo = RLING_ERROR_TOO_MANY_USER_DICTONARY;
			break;
		}

		if (!parce_voc_list_record(list_of_names, nm, &type)) {
			continue;
		}

		if ((point = my_alloc(VOCMEMSIZE)) == NULL) {
			errorNo = RLING_ERROR_CANT_OPEN_USER_DICTONARY;
			break;
		}

		if (LoadUserDict(nm, point, VOCMEMSIZE, &(voc_array[real_voc_no].voc))
				== 0L) {
			my_free(point /*,0*/);
			errorNo = RLING_ERROR_CANT_OPEN_USER_DICTONARY;
			break;
		}

		list_of_names += (strlen(list_of_names) + 1);
		real_voc_no++;
	}

	if (errorNo != 0) {
		unload_user_dicts();
		ErrorExit(errorNo);
	}

	return;
}
/* ------------------------------------------------------------------ */

int16_t parce_voc_list_record(char * w, char * nm, int16_t *type) {
	*type = 0;
	while ((*w) && (*w == ' '))
		w++;
	if ((!*w) || (*w == '\n'))
		return 0;
	while ((*w != ' ') && (*w != 0) && (*w != '\n'))
		*(nm++) = *(w++);
	*nm = 0;
	return 1;
}

/* ------------------------------------------------------------------ */
