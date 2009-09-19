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

#define OLEG_DIS 1   //пропорциональный критерий в Leo

//FIELD_INFO - База данных по текущему полю. Определяется в Rcm.cpp.
//Для использования в других файлах должна быть объявлена как
//           extern "C" FIELD_INFO FieldInfo;

typedef uchar ALPHA_TABLE[256];

typedef struct tagFIELD_INFO
{
  Bool LINret;          //флаг: базовые линии определены LINDefineLines
  Bool HandRec;         //рукописное распознавание
  uint32_t Style;
#define FIS_DONT_GLUE      1  //не клеить
#define FIS_DONT_CUT       2  //не резать
#define FIS_PRINTED        4  //печатное поле
#define FIS_NDX            8  //индексное поле
#define FIS_OBLIGATORY    16  //поле не может быть пустым
#define FIS_DIGIT         32  //цифровой алфавит
#define FIS_PASPORT       64  //поле "паспорт"
#define FIS_PUNCT        128  //допускается пунктуация
#define FIS_MATRIX       256  //матричный (DOS) принтер
#define FIS_DIRTY        512  //грязное поле
#define FIS_FUSE        1024  //рассыпанное поле
#define FIS_MIX         2048  //может быть написано от руки или напечатано
#define FIS_SPACED      4096  //символы разделены пробелами
  ALPHA_TABLE  AlphaTable;    //алфавит
  int32_t LetWidth;             //оценка для ширины буквы
  int32_t MinLetWidth;
  int32_t MaxLetWidth;
  int32_t BigLetHeight;         //оценка для высоты большой буквы
  int32_t SmallLetHeight;       // то же для маленькой
  int32_t MaxWidthHeight;        //максимальное отношение 64*w/h

//из FrhFieldSetup
  Rect16   compRect;         // calculated rect in ideal coords
  int32_t    nMinLetWidth;
  int32_t    nMaxLetWidth;
  int32_t    nMinLetHeight; // letters excluding '-','.',...
  int32_t    nMaxLetHeight;
  int32_t    nMinChars;     // minimum count of chars (including dots, ...)
  int32_t    nMaxChars;     // maximum count of chars, 0 - not restricted;
  int32_t    xFlags;
} FIELD_INFO;

#define not_glue        (FieldInfo.Style & FIS_DONT_GLUE)
#define not_cut         (FieldInfo.Style & FIS_DONT_CUT)
#define printField      (FieldInfo.Style & FIS_PRINTED)
#define bIndexField     (FieldInfo.Style & FIS_NDX)
#define not_empty_field (FieldInfo.Style & FIS_OBLIGATORY)

#define digitField    (FieldInfo.Style & FIS_DIGIT)
#define pasportField  (FieldInfo.Style & FIS_PASPORT)
#define PunctPresence (FieldInfo.Style & FIS_PUNCT)
#define smatrix        (FieldInfo.Style & FIS_MATRIX)
#define dirty_field   (FieldInfo.Style & FIS_DIRTY)
#define fuse_field    (FieldInfo.Style & FIS_FUSE)

#define HandRecog   (FieldInfo.HandRec)
#define Presence(x) (FieldInfo.AlphaTable[x])
#define linOK       (FieldInfo.LINret)
