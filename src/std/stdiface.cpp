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

#include <map>
#include <string>
#include <fstream>
#include <algorithm>
#include <climits>

#include "internal.h"
#include "wmb.h"
#include "std.h"
#include "cfcompat.h"
#include "compat/filefunc.h"

using namespace std;

int32_t stdMessageBox(const char * /*szMessageText*/,
		const char * /*szMessageTitle*/, int32_t /*nFlags*/) {
	return 0;
}

static class CRptDlgMap {
#define RDM_REPEAT                  0x00000000  //show this dlg next time
#define RDM_UNREP_SESS        0x00000100  //don't show dlg in this sess
#define RDM_UNREP_PERM       0x00000200  //don't show dlg always
#define RDM_UNREP			         0x00000300  //don't show dlg always
	typedef map<string, int32_t> Tmap;
	Tmap mp;
public:
	//construction/initialization
	CRptDlgMap() {
	}
	~CRptDlgMap() {
	}
	CRptDlgMap(const char* pIniFile) {
		Initialize(pIniFile);
	}
	Bool32 Initialize(const char* pIniFile);
	Bool32 Destroy();
	//get/set show props
	int32_t GetShowProps(const char* pDlgKey) {
		return mp[string(pDlgKey)];
	}
	void SetShowProps(const char* pDlgKey, int32_t nProps) {
		mp[string(pDlgKey)] = nProps;
	}
	Bool32 Load(const char* pFileName);
	Bool32 Save(const char* pFileName);
}*pRptMap = NULL;

const char szDlgRptSecBr[] = "[Dialog Rpt]";
const char szDlgRptSec[] = "Dialog Rpt";

Bool32 CRptDlgMap::Load(const char* pFileName) {
	try {
		char szBuff[1024];
		szBuff[0] = 0;
		ifstream ifs(pFileName);
		Bool32 bRes = TRUE;
		Bool32 bInSection = FALSE;
		while (ifs.get(szBuff, sizeof(szBuff), '\n') && ifs.get()) {
			if (!strncmp(szBuff, szDlgRptSecBr, sizeof(szDlgRptSecBr))) {
				bInSection = TRUE;
				continue;
			}
			if (bInSection && szBuff[0] == '[')
				break;
			if (!bInSection || szBuff[0] == 0)
				continue;
			string curr_str(szBuff);
			string::iterator pEql = find(curr_str.begin(), curr_str.end(), '=');
			if (pEql == curr_str.end()) {
				bRes = FALSE;
				continue;
			}
			string key_str(curr_str.begin(), pEql);
			++pEql;
			mp[key_str] = atoi((const char*) *pEql);
		};
		return bRes;
	} catch (...) {
		return FALSE;
	}
}

Bool32 CRptDlgMap::Save(const char* pFileName) {
	for (Tmap::iterator it = mp.begin(); it != mp.end(); it++)
		if (it->second & RDM_UNREP_PERM) {
			stdSetProfileInt(it->second, pFileName, szDlgRptSec,
					it->first.c_str());
		}
	return TRUE;
}

static char szRptStoreFileName[PATH_MAX] = { 0 };

Bool32 InitRptMap(const char* pFileName) {
	try {
		strcpy(szRptStoreFileName, pFileName);
		pRptMap = new (CRptDlgMap);
		if (pRptMap == NULL)
			throw ;
			pRptMap->Load(szRptStoreFileName);
			return TRUE;
		}
		catch(...)
		{
			return FALSE;
		}
	}

Bool32 FreeRptMap() {
	try {
		//save map
		if (pRptMap) {
			pRptMap->Save(szRptStoreFileName);
			delete (pRptMap);
			pRptMap = NULL;
		}
		return TRUE;
	} catch (...) {
		return FALSE;
	}
}


