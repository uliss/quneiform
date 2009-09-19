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

// ============================================================================
// Written by Shahverdiev  Alik
// This file cration date: 10.18.98
//
// frmtfile.h
//
// ============================================================================

#ifndef __frmtfile_h__
#define __frmtfile_h__
#include <stdio.h>
#include "cstrdefs.h"

Bool CreateInternalFileForFormatter(FILE *fpInternalFileName);
Bool CheckComingLine(CSTR_line* Comingline);
Bool CheckLineForFilling(CSTR_line* Comingline);

//Bool CheckRect(InternalRect* Inner);

/////////////////////////////////////////////////////////////////////////////

class CFragment;
class CFString;
class CWord;
class CChar;

class CRtfString;

struct PageElementCount {
	uint16_t Frags;
	uint16_t Strings;
	uint16_t Words;
	uint16_t Chars;
};

typedef struct tagInternalRect {
	int16_t left;
	int16_t top;
	int16_t right;
	int16_t bottom;
} InternalRect;

void VCopyRect(InternalRect* InnerR, RECT* OuterR);
Bool CheckRect(InternalRect* Inner);

/////////////////////////////////////////////////////////////////////////////
// Page class

class CFPage //: public CObject
{
public:
	CFPage();
	~CFPage();

	CFragment* GetFirstFrag();
	CFragment* GetNextFrag();
	void ProcessingComingLine(CSTR_line* line);
	void CreateArray_For_TextFragments();
	Bool CheckComingLine_For_TextFragments(CSTR_line* line);
	void AddString(CSTR_line* line);
	Bool Write();

	/*CDWordArray*/
	std::vector<uint32_t> FragmentsArray;
	PageElementCount Count;
	uint16_t m_wDpi;
	std::vector<CFragment*>/*CObArray*/m_arFrags;
	int m_nIndex;
	int m_nCurFragNumber;
	int m_nPrevFragNumber;
};

/////////////////////////////////////////////////////////////////////////////
// Fragment class

class CFragment//:public CObject
{
public:
	CFragment();
	~CFragment();

	CFString* GetFirstString();
	CFString* GetNextString();
	void AddString(CSTR_line* line, PageElementCount* Count);
	Bool Write();

	uint16_t m_wStringsCount;
	/*CObArray*/
	std::vector<CFString*> m_arStrings;
	uint16_t m_wIndex;
	RECT m_rectFrag;
	uint16_t m_wType;
	uint32_t m_wUserNumber;
	uint32_t m_Flags;
};

/////////////////////////////////////////////////////////////////////////////
// String class

class CFString//:public CObject
{
public:
	CFString();
	~CFString();

	CWord* GetFirstWord();
	CWord* GetNextWord();
	void ExtractWordsFromString(CSTR_line* line, PageElementCount* Count);
	void ExtractNextWord(CSTR_line* line);
	Bool Write();

	uint16_t m_wWordsCount;
	/*CObArray*/
	std::vector<CWord*> m_arWords;
	uint16_t m_wIndex;
	CFragment* m_Frag;
	RECT m_rectString;
	uint16_t m_wType;
	RECT m_rectBaseLine;
	uint32_t S_Flags; //NEGA_STR vmk 10-06-2001
};

/////////////////////////////////////////////////////////////////////////////
// Word class

class CWord//:public CObject
{
public:
	CWord();
	~CWord();

	CChar* GetFirstChar();
	CChar* GetNextChar();
	void AddLetter2Word(CSTR_rast* rast, PageElementCount* Count,
			Bool* FlagCapDrop);
	Bool Write();

	/*CObArray*/
	std::vector<CChar*> m_arChars;
	uint16_t m_wCharsCount;
	uint16_t m_wIndex;
	CFString* m_String;
	RECT m_rectWord;
	uint16_t m_wFontNumber;
	uint16_t m_wFontPointSize;
};

/////////////////////////////////////////////////////////////////////////////
// Char class

class CChar//:public CObject
{
public:
	CChar();
	~CChar();
	void AddingLetter(CSTR_rast* rast, int index, Bool* FlagCapDrop);
	Bool Write();

	RECT m_rectChar;
	RECT m_RealRectChar;
	struct {
		uchar m_bChar;
		uchar m_bProbability;
	} m_chrVersions[REC_MAX_VERS];//!!! Art
	uchar m_blanguage;
	uchar m_bFlg_spell;
	uchar m_bFlg_spell_nocarrying;
	uchar m_bFlg_cup_drop;
	uint16_t m_wCountAlt;
	uint16_t m_wFontNumber;
	uint16_t m_wIndex;
};

/////////////////////////////////////////////////////////////////////////////

#endif // __frmtfile_h__
