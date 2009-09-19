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

#include"compat_defs.h"

void MyDelete(void* vElem, int size_type);
void MyDeleteAll(void* vRoot, void* vTail, int size_type);
void* MyNew(int size_type);

template<class TYPE> class LIST {
public:

	TYPE* m_root;
	TYPE* m_tail;
	int m_count;

	LIST() {
		m_root = NULL;
		m_tail = NULL;
		m_count = 0;
	}

	~LIST() {
	}

	TYPE* GetRoot() {
		return m_root;
	}

	TYPE* GetTail() {
		return m_tail;
	}

	TYPE* GetNext(TYPE* now);
	TYPE* Add();
	void Attach(TYPE* att);
	Bool Detach(TYPE* det);
	TYPE* RealAdd();
	void Del(TYPE* now);
	void RealDelAll();
	Bool IfExistElem(TYPE* now);
	void FastDel() {
		TYPE* temp;
		while (m_root) {
			temp = m_root;
			m_root = m_root->next;
			MyDelete(temp, sizeof(TYPE));
		}
		m_count = 0;
		m_tail = NULL;
	}
};

template<class TYPE> TYPE* LIST<TYPE>::GetNext(TYPE* now) {
	if (!now)
		return NULL;
	return now->next;
}

template<class TYPE> TYPE* LIST<TYPE>::Add() {
	TYPE* temp = NULL;
	temp = (TYPE*) MyNew(sizeof(TYPE));
	if (!temp)
		return NULL;
	if (!m_root)
		m_root = m_tail = temp;
	else {
		m_tail->next = temp;
		m_tail = temp;
	}
	m_count++;
	temp->next = NULL;
	return temp;
}

template<class TYPE> TYPE* LIST<TYPE>::RealAdd() {
	TYPE* temp = NULL;
	temp = new TYPE;
	if (!temp)
		return NULL;
	if (!m_root)
		m_root = m_tail = temp;
	else {
		m_tail->next = temp;
		m_tail = temp;
	}
	m_count++;
	temp->next = NULL;
	return temp;
}

template<class TYPE> void LIST<TYPE>::Del(TYPE* now) {
	if (!now)
		return;
	if (!m_root)
		return;
	if (now == m_root) {
		if (m_root == m_tail) {
			MyDelete(m_root, sizeof(TYPE));
			m_root = m_tail = NULL;
		} else {
			m_root = m_root->next;
			MyDelete(now, sizeof(TYPE));
		}
		m_count--;
		return;
	}
	TYPE* temp = m_root;
	while (temp->next && temp->next != now)
		temp = temp->next;
	if (!temp->next)
		return;
	temp->next = now->next;
	if (m_tail == now)
		m_tail = temp;
	MyDelete(now, sizeof(TYPE));
	m_count--;
}

template<class TYPE> void LIST<TYPE>::RealDelAll() {
	TYPE* temp;
	while (m_root) {
		temp = m_root;
		m_root = m_root->next;
		delete temp;
	}
	m_count = 0;
	m_tail = NULL;
}

template<class TYPE> Bool LIST<TYPE>::IfExistElem(TYPE* now) {
	if (!now)
		return FALSE;
	if (!m_root)
		return FALSE;
	TYPE* temp = m_root;
	while (temp) {
		if (temp == now)
			return TRUE;
		temp = temp->next;
	}
	return FALSE;
}

template<class TYPE> void LIST<TYPE>::Attach(TYPE* att) {
	if (!m_root)
		m_root = m_tail = att;
	else {
		m_tail->next = att;
		m_tail = att;
	}
	m_count++;
	att->next = NULL;
}

template<class TYPE> Bool LIST<TYPE>::Detach(TYPE* det) {
	if (!m_root)
		return FALSE;

	TYPE* now = m_root;
	TYPE* prev = NULL;

	while (now) {
		if (now == det) {
			if (prev)
				prev->next = now->next;
			else
				m_root = now->next;

			if (!now->next)
				m_tail = prev;

			m_count--;

			return TRUE;
		}

		prev = now;
		now = now->next;
	}

	return FALSE;
}

