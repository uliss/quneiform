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

#include "lst3_win.h"
#include "aldebug.h"

#ifdef WIN32
#include "snptools.h"
#endif

#ifdef alDebug
extern short FlagGraphic1, Graphic1Color;
#endif

using namespace CIF;

KNOT *inc_lst(KNOT **beg, KNOT **beg_free) {
    KNOT *ptr = *beg_free, *ptr1 = *beg;

    if (ptr != NULL) { /*в списке есть память*/
        if ((*beg_free = (*beg_free)->next) != NULL)
            (*beg_free)->back = NULL;

        ptr->next = ptr1;
        ptr->back = NULL;
        *beg = ptr;

        if (ptr1 != NULL)
            ptr1->back = ptr; /*Inc не первый эл-т в List*/
    }

    return ptr;
}
/*inc_after_lst - добавить в середину списка после элемента ptr
 (имеет смысл для упорядочен. списков)*/
KNOT *inc_after_lst(KNOT *ptr, KNOT **beg, KNOT **beg_free)
/*==========1-вставить не после хвоста, 2-после хвоста*/
{
    KNOT *beg_free_old = *beg_free, *next;

    if (ptr == NULL)
        Error(1, "inc_after_lst");

    if (*beg_free == NULL)
        return NULL; /*в списке свобод. нет памяти */

    if (*beg == NULL)
        return inc_lst(beg, beg_free); /*сам список пуст*/

    if ((*beg_free = (*beg_free)->next) != NULL)
        (*beg_free)->back = NULL;

    next = ptr->next;
    ptr->next = beg_free_old;
    beg_free_old->back = ptr;

    if (next != NULL) { /*вставить не после конца списка*/
        beg_free_old->next = next;
        next->back = beg_free_old;
    }

    else { /*вставить после конца списка*/
        beg_free_old->next = NULL;
    }

    return beg_free_old;
}

void del_lst(KNOT *ptr, KNOT **beg, KNOT **beg_free)
/*=====??что будет,если удаляем (1)голову,(2)хвост,(3)в списке всего 1эл-т*/
{
    KNOT *back = ptr->back, *next = ptr->next;

    if (back != NULL && next != NULL) { /*промежут.эл-т списка*/
        back->next = next;
        next->back = back;
    }/*не голова List и не хвост*/

    else if (next != NULL) { /*не хвост  List, но голова*/
        *beg = next;
        next->back = NULL;
    }

    else if (back != NULL) /*не голова List, но хвост*/
        back->next = NULL;

    else
        /*и хвост и голова List*/
        *beg = NULL;

    ptr->back = NULL;

    if (*beg_free != NULL) { /*List Free не пуст,устанавливаем связи со старой головой*/
        ptr->next = *beg_free;
        (*beg_free)->back = ptr;
    }

    else
        ptr->next = NULL;

    *beg_free = ptr;
}

int init_lst(KNOT ***knot, int *k_bloc, int max_knot, KNOT **beg_free, int size_item) {
    int i, j, size_bloc[MAX_BLOC], kb = -1, fl, k_item;
    KNOT *ptr, *ptr1, *ptr2, *kn[MAX_BLOC];

    //char *err="init_lst";
    /* ===переписывание указателей в выходные массивы ===*/
    if ((fl = alloc_seg(kn, &kb, max_knot, size_item, size_bloc)))
        return fl - 10;

    if (*k_bloc == -1) { /*первый  захват памяти для данного списка*/
        if ((*knot = (KNOT**) malloc((MAX_BLOC + 2) * sizeof(KNOT*))) == NULL)
            return -3;
    }

    do0(i, 0, kb)
        (*knot)[i + (*k_bloc + 1)] = kn[i];

    /*===собственно инициализация ссылок===*/
    if (*k_bloc == -1) { /*первый  захват памяти для данного списка*/
        *beg_free = kn[0];
        kn[0][0].back = NULL;
    }

    else { /*подцепляем новый кусок к хвосту существующего списка*/
        if (*beg_free == NULL) { /*список свобод. памяти исчерпан*/
            *beg_free = kn[0];
            kn[0][0].back = NULL;
        }

        else {
            ptr = *beg_free;

            while (ptr->next != NULL)
                ptr = ptr->next; /*идем до хвоста*/

            kn[0][0].back = ptr;
            ptr->next = kn[0];
        }
    }

    *k_bloc += kb + 1;
    k_item = -1;
    do0(i, 0, kb) {
        ptr = kn[i];
        do0(j, 0, size_bloc[i]) {
            ptr1 = (KNOT*) ((char*) ptr + j * size_item); /*ptr[j]*/
            ptr2 = (KNOT*) ((char*) ptr1 + size_item); /*ptr[j+1]*/

            if (++k_item == max_knot) {
                ptr1->next = NULL;
                goto endc;
            }

            else {
                if (j < size_bloc[i]) { /*ссылки внутри одного блока*/
                    ptr1->next = ptr2;
                    ptr2->back = ptr1;
                }

                else { /*ссылки между блоками*/
                    if (i < kb) {
                        ptr1->next = kn[i + 1];
                        kn[i + 1]->back = ptr1;
                    }

                    else
                        return -4;
                }
            }
        }
    }
    endc: return 0;
}

int alloc_seg(KNOT **kn, int *kb, int max_kn, uint size_item, int *size_bloc) {
    int i, k_knot;
    uint size;
    *kb = -1;
    /*===попытка захвата кусочной памяти под max_knot узлов*/
    k_knot = (int) (SIZE_SEG / size_item); //max_kn=max_knot;

    while (max_kn > -2) {
        uint k = MAX(MIN(k_knot, max_kn + 2), 1);

        if ((kn[++(*kb)] = (KNOT*) malloc(k * size_item)) != NULL) { /*смогли взять*/
            max_kn -= k;
            size_bloc[*kb] = k - 1;
        }

        else {
            --(*kb);
            size = determine_free_memory(k * size_item);

            if (size < (uint) MIN_KNOT * size_item) { /*памяти явно не хватает*/
                do0(i, 0, *kb)
                    free(kn[i]);
                return -3;
            }

            k_knot = size / size_item;
        }

        if ((*kb) > MAX_BLOC - 2) { /*очень много мелких кусочков памяти*/
            do0(i, 0, *kb)
                free((char*) kn[i]);
            return -4;
        }
    }

    return 0;
}
/*================опр-ние макс.доступного непрерыв.куска(max=65500)*/
uint determine_free_memory(uint size1) {
    char *ptr;
    uint size = size1;

    while ((ptr = (char*) malloc(size)) == NULL)
        if ((size = ((size >> 1) + (size >> 2))) < 100) {
            size = 0;
            break;
        }

    if (size)
        free(ptr);

    return size;
}
/*===========освобождение памяти списка*/
void free_lst(KNOT **knot, int k_bloc) {
    int i;

    if (k_bloc < 0 || knot == NULL)
        return;

    doi(i, k_bloc, 0)
        free((char*) knot[i]);
    free((char*) knot);
}
#ifndef CLUST1

typedef FRAME PRMTR;

#ifdef OLD_COOR
typedef struct tagOLD_FRAME
{
        unsigned long start_pos, end_pos;
        int left, up, right, down;//Standard FRAME
        int oldX, oldY;
} OLD_FRAME;
#else
typedef FRAME OLD_FRAME;
#endif

#define KBUF 512
#define KHIS 50

Rect16 RectCut;

#define TestExactReg(f) \
       (DIST_V(f.left,f.right,RectCut.left,RectCut.right)  > 0 && \
          DIST_V(f.up,  f.down, RectCut.top, RectCut.bottom) > 0)

#endif
//*****стек-массив*****

//Return: 0 - OK, NOT_ALLOC - нет места для стека
//==конструктор
int NewStack(int size, STACK *St) {
    if ((St->arr = (KNOTT**) malloc(size * sizeof(PTR))) == NULL)
        return NOT_ALLOC;

    St->size = size;
    St->pos = 0;
    return 0;
}
//==деструктор
void DelStack(STACK *St) {
    if (St->arr != NULL)
        free(St->arr);
}
//Parameters:ptr - заносимый объект
//Return: 0 - OK, NOT_ALLOC - переполнение стека
//==занести в стек
int PushStack(KNOTT *ptr, STACK *St) {
    if (St->pos >= St->size)
        return NOT_ALLOC;

    St->arr[St->pos++] = ptr;
    return 0;
}
//Return: вытолкнутый указатель, если стек непуст или NULL, если стек пуст
//==Вытолкать из стека
KNOTT *PopStack(STACK *St) {
    return (St->pos > 0) ? St->arr[--St->pos] : NULL;
}
//==Очистить стек без освобождения памяти
void ClearStack(STACK *St) {
    St->pos = 0;
}
//==Проверка переполнения стека: return=FALSE => стек еще не переполнен
int OverflowStack(STACK *St) {
    return St->pos > St->size;
}

//*****Операции с TREE2*********
//Прохождение по кусту реализуется как прох-е по дереву от корня,
//если предварит. у корня куста занулить все ссылки, кроме down

//Параметры: Curr - текущ.узел, St - вспомогат. стек (размер = max глубине TREE)
//После каждого вызова NextKnot проверять переполнение стека либо внутри ф-ции
//==перейти к следующему узлу дерева (приоритет обхода - вниз, потом вправо)
KNOTT *NextKnot(KNOTT *Curr, STACK *St) {
    KNOTT *Next = Curr->next;

    if (Curr->down) { //Есть дочерние узлы, идем вниз
        if (Next)
            PushStack(Next, St);

#ifdef alDebug

        if (FlagGraphic1) ++Graphic1Color;

#endif
        return Curr->down;
    }

    else { //Если есть правый сосед, идем к нему, иначе выбираем из стека
        if (!Next) {
            Next = PopStack(St);
#ifdef alDebug

            if (FlagGraphic1) --Graphic1Color;

#endif
        }

        return (Next);
    }
}
//---Общий сценарий работы с эмулятором:---
//1.InitSubAlloc(&SubAlloc)
//2.Ptr=SubMalloc(uint16_t size,&SubAlloc)
//3.DeleteSubAlloc(&SubAlloc)
//---Особенности: ---
//1.При нехватке памяти в первоначальном массиве сегментов он наращивается
//  новыми сегментами
//2.Отдельного освобождения памяти, занятой операциями SubMalloc, не нужен,
//  делается общий DeleteSubAlloc
//3.Общий размер сегментир.адрес.простр-во м.б. > 64Kb
//4.Одновременно м.б. несколько сегментир.адрес.простр-в.
//5.Очень полезно при  большом наборе маленьких областей,т.к. не расходуются
//WIndows-дескрипторы и не тратится время на весьма дорогостоящие операции
//malloc/free и GlobalLock/GlobalAlloc/LocalLock/LocalAlloc/GlobalFree и т.п.
//Первоначальный захват сегментир.адрес.простр-ва Sub размера Size байт
//Return: 0 - OK, NOT_ALLOC - нет памяти
int InitSubAlloc(long Size, SUB_ALLOC *Sub) {
    int NumPtr = (int) (Size / SIZE_SEGL), in;
    long k;

    if ((long) NumPtr * SIZE_SEGL < Size)
        ++NumPtr;

    Sub->Ptr = (char**) malloc(NumPtr * sizeof(PTR)); //ml
    Sub->SizePtr = (long*) malloc(NumPtr * sizeof(long)); //ml
    Sub->NumPtr = NumPtr;
    Sub->CurrPtr = 0;
    Sub->CurrPos = 0;
    Sub->Size = Size;
    in = -1;

    while (Size > 0) {
        k = MIN(SIZE_SEGL, Size);

        if ((Sub->Ptr[++in] = (char*) malloc((uint) k)) == NULL)
            return NOT_ALLOC;

        Sub->SizePtr[in] = k;
        Size -= k;
    }

    return 0;
}
//Захватить память в SubMalloc-адрес.пространстве s, при необх-ти - с
//захватом дополнит. сегмента
char *Submalloc(uint size, SUB_ALLOC *s) {
    long pos;
    const char *err = "Submalloc";
#ifdef DEBUG_MEM

    if (size == 0 || size > SIZE_SEGL) Error(2, err);

#endif

    if (s->CurrPtr >= s->NumPtr || s->SizePtr[s->CurrPtr] - s->CurrPos < (long) size) {
        s->CurrPos = 0;//переход к след.сегменту

        while (++s->CurrPtr < s->NumPtr && s->SizePtr[s->CurrPtr] < (long) size)
            ;

        if (s->CurrPtr >= s->NumPtr) { //Исчерпаны все сегменты,заводим новый
            s->Ptr = (char**) realloc(s->Ptr, (s->NumPtr + 1) * sizeof(PTR));
            s->SizePtr = (long*) realloc(s->SizePtr, (s->NumPtr + 1) * sizeof(long));

            if ((s->Ptr[s->NumPtr] = (char*) malloc((uint) SIZE_SEGL)) == NULL)
                return NULL;

            s->SizePtr[s->NumPtr] = SIZE_SEGL;
            s->Size += SIZE_SEGL;
            s->CurrPtr = s->NumPtr++;
        }
    }

    if ((pos = s->CurrPos) < 0 || pos + (long) size > s->SizePtr[s->CurrPtr])
        Error(1, err);

    s->CurrPos += size;
    return &s->Ptr[s->CurrPtr][pos];
}
//==Освободить память SubAllocator-a
void DeleteSubAlloc(SUB_ALLOC *s) {
    for (int i = 0; i < s->NumPtr; ++i)
        free(s->Ptr[i]);

    free(s->Ptr);
    free(s->SizePtr);
}

