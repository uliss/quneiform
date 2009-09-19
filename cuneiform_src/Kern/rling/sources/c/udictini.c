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

/*

 ЛННННННННННННННННННННННННННННННННННННННННННННННННННННННННННННННННННННННННННННН»
 є        This module is a set  user dictionary initialisation utilities       є
 ИНННННННННННННННННННННННННННННННННННННННННННННННННННННННННННННННННННННННННННННј
 */
#include "spelmode.h"
/////// #include <dos.h>
#if defined (WATCOM)
#include "spelwatc.h"
#elif defined (BC_FOR_WIN)
#include "bcwtypes.h"
typedef long signed int int32_t;
#elif defined(TURBO_C)
#include "tc_types.h"
#else
#error No tool specified
#endif

#include "speldefs.h"
#include "spelfunc.h"
#include "udicfunc.h"

#include "tigeremulate.h"

#include "compat_defs.h"

uint32_t LoadUserDict(char *DictName, char *pool, uint32_t pool_size,
		voc_state *user_dict) {
	int32_t size;
	pool_size = pool_size;
	if (_IsUserDict(DictName) != UD_PERMITTED)
		return 0;

#ifdef TURBO_C
	if(TEST_PRPH(pool))
	pool=ALI_PR(pool);
#endif

	user_dict -> vocseg = (uchar *) SET_VOC_ROOT(pool);

	{
		int16_t Fh;
		char nm[128];
		strcpy(nm, DictName);
		Fh = TGOPEN(VC_STREAM, nm, (int16_t)(O_RDONLY | O_BINARY), S_IREAD);
		if (Fh == -1)
			return 0;
		if (TGFILELTH(Fh) > MAX_VOC_SIZE) {
			TGCLOSE(Fh);
			return 0;
		}
		size = TGREAD(Fh, V_POINT(user_dict -> vocseg, 0), TGFILELTH(Fh));
		TGCLOSE(Fh);
	}
	if (size <= 0)
		return 0;
	else {
		voc_open(user_dict);
		user_dict -> vocfree = (uint16_t) size;
	}
	return MAX_VOC_SIZE;
}

uint32_t InitializeNewUserDict(char *pool, uint32_t pool_size, voc_state *user_dict) {
#ifdef TURBO_C
	if(TEST_PRPH(pool))
	pool=ALI_PR(pool);
#endif

	if (pool_size < MAX_VOC_SIZE)
		return 0;

	user_dict -> vocseg = SET_VOC_ROOT(pool);
	voc_init(user_dict);

	return MAX_VOC_SIZE;
}

void ResetUserDict(voc_state * user_dict) {
	user_dict ->lev = -1;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool CloseUserDictionary(uchar * DictName, voc_state *user_dict) {
	if (user_dict -> state & VOC_CHANGED) {
		char w[80];
		int32_t size;
		int16_t h;

		strcpy(w, DictName);
		h = TGOPEN(VC_STREAM, w, O_CREAT, S_IREAD);

		if (h == -1) {
			/* MsgBox("failed to open"); */
			return FALSE;
		}

		size = TGWRITE(h, V_POINT(user_dict ->vocseg, 0), user_dict ->vocfree);
		TGCLOSE(h);

		if (size != (int32_t) user_dict->vocfree) {
			/* MsgBox("wrong size"); */
			return FALSE;
		}

		user_dict -> state &= (~VOC_CHANGED);
	}
	return TRUE;
}
