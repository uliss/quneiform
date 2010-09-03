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

enum alph_t {
    ALPH_ALL = 0x00,
    ALPH_RUS = 0x01,
    ALPH_DIG = 0x02,
    ALPH_ROM = 0x04,
    ALPH_R_E = 0x08,
    ALPH_ENG = 0x10,
    ALPH_RALL = 0x20
};
//
// 1 - Capital Russian letters
// 2 - Digital
// 4 - Roma digit
//
static const uchar typ_of_alpha[256] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                         0, 0,
                                         0,
                                         //                                                         last 0f
                                         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                         0,
                                         //                                                         last 1f
                                         0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                         1,
                                         //     ! " # $ % & ' ( ) * + , - . /           last = 2f
                                         2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0,
                                         0,
                                         //   0 1 2 3 4 5 6 7 8 9 : ; < = > ? @         last = 40
                                         8, 8, 8, 8, 8, 8, 8, 8, 4, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 4, 8, 4,
                                         8, 8, 0, 0, 0, 0, 0, 0,
                                         //   A B C D E F G H I J K L M N O P Q R S T U V W X Y Z [ \ ] ^ _ `                                 last: 60
                                         8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
                                         8, 8, 0, 0, 0, 0, 0,
                                         //   a b c d e f g h i j k l m n o p q r s t u v w x y z
                                         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                         //   А Б В Г Д Е Ж З И Й К Л М Н О П
                                         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                         //   Р С Т У Ф Х Ц Ч Ш Щ Ъ Ы Ь Э Ю Я
                                         32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
                                         //   а  б  в  г  д  е  ж  з  и  й  к  л  м  н  о  п
                                         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                         //
                                         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                         //
                                         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                         //
                                         32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
                                         //   р  с  т  у  ф  х  ц  ч  ш  щ  ъ  ы  ь  э  ю  я
                                         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
//
                                       };

static const uchar up_of_alpha[256] = {
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    //                                                         last 0f
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    //                                                         last 1f
    0,
    0,
    0,
    '#',
    0,
    '%',
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    '/',
    //     ! "  #  $  %  & ' ( ) * + , - . /           last = 2f
    (uchar) '0',
    (uchar) '1',
    (uchar) '2',
    (uchar) '3',
    (uchar) '4',
    (uchar) '5',
    (uchar) '6',
    (uchar) '7',
    (uchar) '8',
    (uchar) '9',
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    //    0   1   2   3   4   5   6   7   8   9 : ; <  =  >  ?  @         last = 40
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
    'O', 'P', 'Q', 'R',
    'S',
    'T',
    'U',
    'V',
    'W',
    'X',
    'Y',
    'Z',
    0,
    0,
    0,
    0,
    0,
    0,
    //   A B C D E F G H I J K L M N O P Q R S T U V W X Y Z [  \  ] ^ _ `                                 last: 60
    'a', 'b', 'C', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
    'O', 'P', 'q', 'r', 'S',
    't',
    'u',
    'V',
    'W',
    'X',
    'Y',
    'Z',
    0,
    (uchar) '|',
    0,
    0,
    0,
    //   a b c d e f g h i j k l m n o p q r s t u v w x y z {  |  } ~
    (uchar) '\x80' /* А */, (uchar) '\x81' /* Б */, (uchar) '\x82' /* В */, (uchar) '\x83' /* Г */, (uchar) '\x84' /* Д */,
    (uchar) '\x85' /* Е */,
    (uchar) '\x86' /* Ж */,
    (uchar) '\x87' /* З */,
    (uchar) '\x88' /* И */,
    (uchar) '\x89' /* Й */,
    (uchar) '\x8a' /* К */,
    (uchar) '\x8b' /* Л */,
    (uchar) '\x8c' /* М */,
    (uchar) '\x8d' /* Н */,
    (uchar) '\x8e' /* О */,
    (uchar) '\x8f' /* П */,
    //    А   Б   В   Г   Д   Е   Ж   З   И   Й   К   Л   М   Н   О   П
    (uchar) '\x90' /* Р */, (uchar) '\x91' /* С */, (uchar) '\x92' /* Т */, (uchar) '\x93' /* У */, (uchar) '\x94' /* Ф */,
    (uchar) '\x95' /* Х */, (uchar) '\x96' /* Ц */, (uchar) '\x97' /* Ч */, (uchar) '\x98' /* Ш */,
    (uchar) '\x99' /* Щ */,
    (uchar) '\x9a' /* Ъ */,
    (uchar) '\x9b' /* Ы */,
    (uchar) '\x9c' /* Ь */,
    (uchar) '\x9d' /* Э */,
    (uchar) '\x9e' /* Ю */,
    (uchar) '\x9f' /* Я */,
    //    Р   С   Т   У   Ф   Х   Ц   Ч   Ш   Щ   Ъ   Ы   Ь   Э   Ю   Я
    (uchar) '\xa0' /* а */, (uchar) '\xa1' /* б */, (uchar) '\x82' /* В */, (uchar) '\x83' /* Г */, (uchar) '\x84' /* Д */,
    (uchar) '\xa5' /* е */, (uchar) '\x86' /* Ж */, (uchar) '\x87' /* З */, (uchar) '\x88' /* И */, (uchar) '\x89' /* Й */,
    (uchar) '\x8a' /* К */, (uchar) '\x8b' /* Л */, (uchar) '\x8c' /* М */,
    (uchar) '\x8d' /* Н */,
    (uchar) '\x8e' /* О */,
    (uchar) '\x8f' /* П */,
    //    а   б   в   г   д   е   ж   з   и   й   к   л   м   н   о   п
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0,
    0,
    //
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0,
    0,
    //
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0,
    0,
    //
    (uchar) '\x90' /* Р */, (uchar) '\x91' /* С */, (uchar) '\x92' /* Т */, (uchar) '\x93' /* У */, (uchar) '\x94' /* Ф */,
    (uchar) '\x95' /* Х */, (uchar) '\x96' /* Ц */, (uchar) '\x97' /* Ч */, (uchar) '\x98' /* Ш */, (uchar) '\x99' /* Щ */,
    (uchar) '\x9a' /* Ъ */, (uchar) '\x9b' /* Ы */, (uchar) '\x9c' /* Ь */, (uchar) '\x9d' /* Э */, (uchar) '\x9e' /* Ю */,
    (uchar) '\x9f' /* Я */,
    //    р   с   т   у   ф   х   ц   ч   ш   щ   ъ   ы   ь   э   ю   я
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
//
};

static const uchar leo_accepted[256] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                         0, 0,
                                         0,
                                         //                                                         last 0f
                                         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                         0,
                                         //                                                         last 1f
                                         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                         0,
                                         //     !  " #  $  % & ' (   ) *  + , - . /           last = 2f
                                         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
                                         0,
                                         //   0 1 2 3 4 5 6 7 8 9 : ; <  =  >  ?  @         last = 40
                                         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                         1, 1, 0, 0, 0, 0, 0, 0,
                                         //   A B C D E F G H I J  K L M N O P  Q  R S T U V  W  X Y Z [  \  ] ^ _ `                                 last: 60
                                         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                         1, 1, 0, 0, 0, 0, 0,
                                         //   a b c d e f  g  h i j k l m n o p  q r s t  u v w x y z
                                         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                         //   А Б В Г Д  Е Ж З И Й К Л М Н О П
                                         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                         //   Р С Т У Ф Х Ц  Ч Ш Щ  Ъ Ы Ь Э Ю Я
                                         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                         //   б в г д  е ж з и й  к л м н о п
                                         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                         //
                                         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                         //
                                         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                         //
                                         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                         //   р с т у ф х ц  ч ш щ  ъ ы ь э ю я
                                         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
//
                                       };
