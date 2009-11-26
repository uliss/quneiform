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


/********************************************************************/
/*                                                                  */
/*                proportional indexes for narrow letters           */
/*                                                                  */
/********************************************************************/

int16_t    prop_narrow[256] = {
    /*   0   */0,
    /*   1  */0,
    /*   2  */0,
    /*   3  */0,
    /*   4  */0,
    /*   5  */0,
    /*   6  */0,
    /*   7  */0,
    /*   8  */0,
    /*   9   */0,
    /*  10   */0,
    /*  11  */0,
    /*  12   */0,
    /*  13   */0,
    /*  14  */0,
    /*  15  */0,
    /*  16  */0,
    /*  17  */0,
    /*  18  */0,
    /*  19  */0,
    /*  20  */0,
    /*  21  */0,
    /*  22  */0,
    /*  23  */0,
    /*  24  */0,
    /*  25  */0,
    /*  26  */0,
    /*  27  */0,
    /*  28  */0,
    /*  29  */0,
    /*  30  */0,
    /*  31  */0,
    /*  32   */0,
    /*  33 ! */0,
    /*  34 " */0,
    /*  35 # */0,
    /*  36 $ */0,
    /*  37 % */0,
    /*  38 & */0,
    /*  39 ' */0,
    /*  40 ( */0,
    /*  41 ) */0,
    /*  42 * */0,
    /*  43 + */0,
    /*  44 , */0,
    /*  45 - */0,
    /*  46 . */0,
    /*  47 / */0,
    /*  48 0 */38,
    /*  49 1 */0,
    /*  50 2 */34,
    /*  51 3 */34,
    /*  52 4 */34,
    /*  53 5 */34,
    /*  54 6 */34,
    /*  55 7 */34,
    /*  56 8 */34,
    /*  57 9 */34,
    /*  58 : */0,
    /*  59 ; */0,
    /*  60 < */0,
    /*  61 = */0,
    /*  62 > */0,
    /*  63 ? */0,
    /*  64 @ */0,
    /*  65 A */40,
    /*  66 B */40,
    /*  67 C */40,
    /*  68 D */40,
    /*  69 E */40,
    /*  70 F */40,
    /*  71 G */40,
    /*  72 H */43,
    /*  73 I */0,
    /*  74 J */0,
    /*  75 K */40,
    /*  76 L */40,
    /*  77 M */0,
    /*  78 N */40,
    /*  79 O */0,
    /*  80 P */40,
    /*  81 Q */40,
    /*  82 R */40,
    /*  83 S */41,
    /*  84 T */40,
    /*  85 U */0,
    /*  86 V */40,
    /*  87 W */0,
    /*  88 X */40,
    /*  89 Y */40,
    /*  90 Z */36,
    /*  91 [ */0,
    /*  92 \ */0,
    /*  93 ] */0,
    /*  94 ^ */0,
    /*  95 _ */0,
    /*  96 ` */0,
    /*  97 a */40,
    /*  98 b */40,
    /*  99 c */40,
    /* 100 d */40,
    /* 101 e */40,
    /* 102 f */0,
    /* 103 g */36,
    /* 104 h */36,
    /* 105 i */0,
    /* 106 j */0,
    /* 107 k */30,
    /* 108 l */0,
    /* 109 m */0,
    /* 110 n */40,
    /* 111 o */40,
    /* 112 p */36,
    /* 113 q */0,
    /* 114 r */0,
    /* 115 s */40,
    /* 116 t */0,
    /* 117 u */40,
    /* 118 v */40,
    /* 119 w */0,
    /* 120 x */40,
    /* 121 y */40,
    /* 122 z */40,
    /* 123 { */0,
    /* 124 | */0,
    /* 125 } */0,
    /* 126 ~ */0,
    /* 127  */0,
    /* 128 Ђ */44,
    /* 129 Ѓ */40,
    /* 130 ‚ */40,
    /* 131 ѓ */40,
    /* 132 „ */34,
    /* 133 … */40,
    /* 134 † */0,
    /* 135 ‡ */32,
    /* 136 € */40,
    /* 137 ‰ */0,
    /* 138 Љ */36,
    /* 139 ‹ */40,
    /* 140 Њ */0,
    /* 141 Ќ */40,
    /* 142 Ћ */40,
    /* 143 Џ */40,
    /* 144 ђ */40,
    /* 145 ‘ */40,
    /* 146 ’ */40,
    /* 147 “ */36,
    /* 148 ” */40,
    /* 149 • */40,
    /* 150 – */40,
    /* 151 — */40,
    /* 152 � */0,
    /* 153 ™ */0,
    /* 154 љ */40,
    /* 155 › */0,
    /* 156 њ */40,
    /* 157 ќ */40,
    /* 158 ћ */0,
    /* 159 џ */40,
    /* 160   */40,
    /* 161 Ў */36,
    /* 162 ў */40,
    /* 163 Ј */32,
    /* 164 ¤ */40,
    /* 165 Ґ */40,
    /* 166 ¦ */0,
    /* 167 § */35,
    /* 168 Ё */45,
    /* 169 © */0,
    /* 170 Є */40,
    /* 171 « */40,
    /* 172 ¬ */0,
    /* 173 ­ */40,
    /* 174 ® */40,
    /* 175 Ї */40,
    /* 176 ° */0,
    /* 177 ± */0,
    /* 178 І */0,
    /* 179 і */0,
    /* 180 ґ */0,
    /* 181 µ */0,
    /* 182 ¶ */0,
    /* 183 · */0,
    /* 184 ё */0,
    /* 185 № */0,
    /* 186 є */0,
    /* 187 » */0,
    /* 188 ј */0,
    /* 189 Ѕ */0,
    /* 190 ѕ */0,
    /* 191 ї */0,
    /* 192 А */0,
    /* 193 Б */0,
    /* 194 В */0,
    /* 195 Г */0,
    /* 196 Д */0,
    /* 197 Е */0,
    /* 198 Ж */0,
    /* 199 З */0,
    /* 200 И */0,
    /* 201 Й */0,
    /* 202 К */0,
    /* 203 Л */0,
    /* 204 М */0,
    /* 205 Н */0,
    /* 206 О */0,
    /* 207 П */0,
    /* 208 Р */0,
    /* 209 С */0,
    /* 210 Т */0,
    /* 211 У */0,
    /* 212 Ф */0,
    /* 213 Х */0,
    /* 214 Ц */0,
    /* 215 Ч */0,
    /* 216 Ш */0,
    /* 217 Щ */0,
    /* 218 Ъ */0,
    /* 219 Ы */0,
    /* 220 Ь */0,
    /* 221 Э */0,
    /* 222 Ю */0,
    /* 223 Я */0,
    /* 224 а */35,
    /* 225 б */36,
    /* 226 в */40,
    /* 227 г */38,
    /* 228 д */40,
    /* 229 е */40,
    /* 230 ж */40,
    /* 231 з */40,
    /* 232 и */0,
    /* 233 й */0,
    /* 234 к */40,
    /* 235 л */0,
    /* 236 м */40,
    /* 237 н */36,
    /* 238 о */0,
    /* 239 п */40,
    /* 240 р */0,
    /* 241 с */0,
    /* 242 т */0,
    /* 243 у */0,
    /* 244 ф */0,
    /* 245 х */0,
    /* 246 ц */0,
    /* 247 ч */0,
    /* 248 ш */0,
    /* 249 щ */0,
    /* 250 ъ */0,
    /* 251 ы */0,
    /* 252 ь */0,
    /* 253 э */48,
    /* 254 ю */0,
    /* 255 я */0
};
