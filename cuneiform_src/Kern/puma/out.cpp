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

#include "mpuma.h"
#include <string.h>

bool ConverROUT(const std::string& filename, puma_format_t Format,
		puma_code_t Code, bool Append) {
	char szName[260];
	strcpy(szName, filename.c_str());
	char * str = strrchr(szName, '.');
	if (str)
		*(str) = '\0';

	if (!ROUT_SetImportData(ROUT_BOOL_PreserveLineBreaks,
			(void*) gnPreserveLineBreaks) || !ROUT_SetImportData(
			ROUT_PCHAR_PageName, szName) || !ROUT_SetImportData(
			ROUT_HANDLE_PageHandle, ghEdPage) || !ROUT_SetImportData(
			ROUT_LONG_Format, (void*) Format) || !ROUT_SetImportData(
			ROUT_LONG_Code, (void*) Code) || !ROUT_SetImportData(
			ROUT_PCHAR_BAD_CHAR, &gnUnrecogChar)) {
//		SetReturnCode_puma(ROUT_GetReturnCode());
		return false;
	}
	// Количество объектов
	long countObjects = ROUT_CountObjects();
	if (countObjects == -1) {
//		SetReturnCode_puma(ROUT_GetReturnCode());
		return false;
	}

	for (long objIndex = 1; objIndex <= countObjects; objIndex++) {
		std::string path(filename);

		if (countObjects != 1) {
			path = ROUT_GetDefaultObjectName(objIndex);
			if (!path.empty()) {
//				SetReturnCode_puma(ROUT_GetReturnCode());
				return false;
			}
		}

		if (!ROUT_SaveObject(objIndex, path.c_str(), Append)) {
//			SetReturnCode_puma(ROUT_GetReturnCode());
			return false;
		}
	} // Цикл по объектам на странице

	return true;
}

uint32_t ConverROUTtoMemory(Handle hEd, int32_t lnFormat, int32_t lnCode,
		Byte * lpMem, uint32_t size) {
	uint32_t rc = 0;
	if (!ROUT_SetImportData(ROUT_BOOL_PreserveLineBreaks,
			(void*) gnPreserveLineBreaks) || !ROUT_SetImportData(
			ROUT_HANDLE_PageHandle, hEd) || !ROUT_SetImportData(
			ROUT_LONG_Format, (void*) lnFormat) || !ROUT_SetImportData(
			ROUT_LONG_Code, (void*) lnCode) || !ROUT_SetImportData(
			ROUT_PCHAR_BAD_CHAR, &gnUnrecogChar)) {
//		SetReturnCode_puma(ROUT_GetReturnCode());
		return rc;
	}

	// Количество объектов
	long countObjects = ROUT_CountObjects();
	if (countObjects == -1) {
//		SetReturnCode_puma(ROUT_GetReturnCode());
		return rc;
	}
	// Просмотрим размер памяти
	long nSize = 0;
	for (long objIndex = 1; objIndex <= countObjects; objIndex++) {
		long nCurSize = ROUT_GetObjectSize(objIndex);
		nSize += nCurSize;
		if (nSize <= (long) size) {
			if (!ROUT_GetObject(objIndex, lpMem + (nSize - nCurSize), &nCurSize)) {
//				SetReturnCode_puma(ROUT_GetReturnCode());
				return rc;
			}
		}
	} // Цикл по объектам на странице
	rc = nSize;
	return rc;
}
