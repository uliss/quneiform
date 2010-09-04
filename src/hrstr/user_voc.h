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

static const char *english_voc[] = { "at", "on", "pro", "more", "an", "mr", "many", "him", "ran",
        "any", "or", "open", "one", "once", "be", "can", "of", "we", "the", "him", "are", "it",
        "dos", "ocr", "ibm", "group", "by", "is", "in", "if", "very", "cdrom", "fax", "cd", "rom",
        "new", "old", "svga", "vga", "computer", "multimedia", "hp", "ascii", "soft", "copy",
        "sorry", "as", "system", "isn", "samsung", "infoseek", "rtf", "dbf", "odbc", "api", "sdk",
        "csv", "xls", "html", "pci", "pentium", "ide", "vesa", "cisco", "scsi", "eisa", "cashe",
        "\0" // end of word list
        };
static const char *russian_voc[] = {
// cp866
        "\xaa\xa0\xaa", // как
        "\xa5\xa3\xae", // его
        "\xa7\xa0", // за
        "\xE2\xA0\xAA", // так
        "\xad\xa0", // на
        "\xad\xa5", // не
        "\xa5\xa5", // ее
        "\xad\xae", // но
        "\xae\xad", // он
        "\xae\xad\xa0", // она
        "\xa2\xae\xe2", // вот
        "\xa8", // и
        "\xe1\xae", // со
        "\xa2\xe1\xa5", // все
        "\xa2", // в
        "\xA2\xE1\xA5\xA9", // всей
        "\xa2\xae", // во
        "\xaf\xae\xab\xa5", // поле
        "\xaf\xae", // по
        "\xad\xe3", // ну
        "\xe7\xa0\xe1", // час
        "\xae", // о
        "\xE7\xA5\xA3\xAE", // чего
        "\xa0", // а
        "\xE1\xA5\xA1\xA5", // себе
        "\xa6\xa5", // же
        "\xaf\xe0\xae\xe1\xe2\xae", // просто
        "\xaa\xae\xa3\xae", // кого
        "\xaf\xe0\xae", // про
        "\xe7\xe2\xae", // что
        "\xE2\xE0\xAE\xA5", // трое
        "\xe1\xe8\xa0", // сша
        "\xae\xe2", // от
        "\xac\xa3\xe6", // мгц
        "\xa3\xa1\xa0\xa9\xe2", // гбайт
        "\xae\xad\xa8", // они
        "\xaa\xe2\xae", // кто
        "\0" // end of word list
        };
static const char *english_voc_all[] = { "for", "ole", "win", "\0" };

static const char *russian_voc_all[] = {
// cp1251
        "\xe8\xeb\xe8", // или
        "\xeb\xe5\xf2", // лет
        "\xf1\xe8\xeb", // сил
        "\xed\xe8\xec", // ним
        "\xe8\xe7\xee", // изо
        "\xf1\xf2\xf0", // стр
        "\xed\xe5\xb8", // неё
        "\xe5\xb8", //  её
        "\xed\xb8\xec", // нём
        "\xf8\xb8\xeb", //шёл
        "\xe1\xee\xe3", //бог
        "\0" };

static const char *russian_voc_short[] = {
// cp1251
        "\xf2\xfb\xf1", // тыс
        "\xf1\xee\xf7", // соч
        "\xf1\xf2\xf0", // стр
        "\xf0\xf3\xf1", //рус
        "\0" };
