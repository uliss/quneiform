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

#include "sfont.h"

#ifndef __FON_H
   #define __FON_H

   #ifndef __GLOBUS_H
   #include "globus.h"
   #endif

   #include "fondefs.h"
#ifndef __RECDEFS_H
   #include "recdefs.h"
#endif
   #include "memfunc.h"

   #ifdef __FON__
      #define FON_FUNC  FUN_EXPO
   #else
      #define FON_FUNC  FUN_IMPO
   #endif

   #ifdef __cplusplus
   extern "C" {
   #endif

	// return < 0 - error
    // FonSpecInfo *fonSpecInfo может быть NULL
	FON_FUNC(int32_t) FONRecogChar(RecRaster *recRast,RecVersions *collection,FonSpecInfo *fonSpecInfo);
	FON_FUNC(int32_t) FONRecogCharTiger(RecRaster *recRast,RecVersions *collection,FonSpecInfo *fonSpecInfo);
	// okr - не штрафовать точки на расстоянии от кластера okr
	// proc - штрафовать точки кластера > (wei*proc)/100
	FON_FUNC(int32_t) FONRecogOkr(RecRaster *recRast,RecVersions *collection,FonSpecInfo *specInfo,int32_t okr,int32_t proc);

	// больше штрафовать точки кластера
	// меньше - растра
	FON_FUNC(int32_t) FONRecogInner(RecRaster *recRast,RecVersions *collection,FonSpecInfo *specInfo);

	// получить номер кластера для символа из базы
	FON_FUNC(int32_t) FONGetNumCluster(int32_t nInCTB);

	// проверить растр с именем let
	// возврат  -2 - слишком большой символ
	//    -1 - нет кластера с таким именем
	//    >= 0 , <= 4- сколько кластеров с разными
	//     атрибутами с непустыми оценками
	//    в testInfo - результаты
	//      атрибуты кластера Italic,Bold,Serif
	FON_FUNC(int32_t) FONTestChar(RecRaster *recRast,uchar let,FonTestInfo *testInfo,int16_t nInCTB);
	FON_FUNC(int32_t) FONTestCharTiger(RecRaster *recRast,uchar let,FonTestInfo *testInfo);
    // оценка по кластеру  numWelet
	// movx,movy - смещение
	FON_FUNC(int32_t) FONCompareRasterCluster(RecRaster *recRast,int numWelet,
										int movx,int movy);
    FON_FUNC(int32_t) FONCompareOkrRasterCluster(RecRaster *recRast,int numWelet,
										int movx,int movy,int okr,int procent,
										int *dist1,int *dist2);
	FON_FUNC(int32_t) FONSetAlphabet(char *coll);
	// выбрать кластеры для колонки
	FON_FUNC(int32_t) FONSetAlphaColumn(int32_t column,Bool32 onlyDigits);

	FON_FUNC(int32_t) FONFontInfo(FontInfo *fontinfo);
	FON_FUNC(int32_t) FONSizesInfo(SizesInfo *sizeinfo,int num);

	// 3.12.98
	// return < 0 - error (no base loaded)
	//          0 - undefined
	//        > 0 - count
	FON_FUNC(int32_t) FONGetFontCount(void);
////////////////// 29 sep 98
	//
	// if input clustinfo->let == 0 - получить информацию о кластере num
	// иначе - получить информацию о кластере с именем clustinfo->let
	// и номером >= num
	FON_FUNC(int32_t) FONGetClustInfo( ClustInfo *clustinfo,
                                    int num   // in range [1..FONGetClustCount(void)]
                                   );
	FON_FUNC(int32_t) FONGetClustCount(void);   // returns count (N) => numbers [1..N] can be used as indexes

   FON_FUNC(int32_t) FONGetNearestClusters( RecRaster* pattern,  // symbol
                                          ClustAlt result[],   // result table of K nearest klusters
                                          int nClustAltLen,    // length of table
                                          int nDistLevel       // don't report about clusters with distance more than nDistLevel
                                        );
////////////////////////////////////
	//
	//  Создать по кластеру черно-белый растр
	//   [in/out] name   - нужен кластер с именем *name (если name !=NULL && *name >0)
	//                 - помещается имя выбранного кластера (если name!=NULL)
	//   [in] number - & с порядковым номером >= number
	//   [in] porog -  & использовать порог (0-100 ) (0 - default)
	//   [out]recRaster - положить в recRaster
	//
	//   Return
	//      >=0 - номер кластера, который выбран.
	//      < 0 - error :
	//       -10 - не загружена база
	//       -20 - неверный номер  (<0 или > числа кластеров)
	//       -21 - неверное имя    (<0 or > 255)
	//       -22 - кластер не найден (нет больше с нужным именем)
	//
	FON_FUNC(int32_t) FONGetClusterAsBW(int32_t *name,int32_t number,int32_t porog,RecRaster *recRast);

	 // return < 0  - error	 (-1 - no memory)
	 //		   == 0 no file or invalid file
	 //		   > 0 O.K
	 //
	FON_FUNC(int32_t)  FONInit(char *);

    FON_FUNC(void)   FONDone(void);

	// поставить кластеру (num>=1) плохую метку
	FON_FUNC(int32_t) FONSetClusterInvalid(int32_t num);

    FON_FUNC(int16_t) FONGetErr(void);
      // Err16.h defined codes or one of the next
      //

	    // Кластеризация.

	// передать очередной растр для будущей кластеризации
	// возвращает номер
	// <0 - ошибка (нет памяти)
	//  0 - растр не взят по какой-то причине (плохой...)
	FON_FUNC(int32_t) FONStoreRaster(RecRaster *r,uchar let, uchar IsPrint,
		uchar Prob, uchar Valid, int16_t line_number, uchar kegl,Rect16 *rect,uchar column);

    //
	// return < 0 - error
    // return >= 0 - сколько сохранено кластеров
    //
    //  Параметры:
    //  CTBname - имя входной базы или имя файла растров (.r для tiger)
    //  cluname - выходное имя. Если cluname==NULL - получаем из CTBname с расширением clu
    //                       (если выход .clu)
    //  accept - указатель на функцию, вызываемую для каждого символа при отнесении
    //         его к кластеру (только для tiger) - можно NULL
    //  extern_buf - внешний буфер, который можно использовать - можно NULL
	//  size       - размер буфера
    //  param - параметры кластеризации (0 default)
	//    if( (param&FONCLU_MultiFontRow) != 0 ) - в одной строке много шрифтов
    //    else                      - в одной строке один шрифт
    //    if( (param&FONCLU_SaveAsClu) != 0 ) - сохранять как файл .clu
    //    else                      - сохранять как grey CTB
	//    if( (param&FONCLU_NotFonts) != 0 ) - сохранять все кластеры
    //    else                     - создать фонт
    //    if( (param&FONCLU_SaveWeak) != 0 ) - не объединять solid и twin (tiger )
	//                               не выбрасывать слабые кластеры (CTB)
    //    else                     - объединять
    //    if( (param&FONCLU_AddClu) != 0 ) - объединять кластеры с имеющимися в cluname (tiger only)
    //    else                     - создавать заново
    //    if( (param&FONCLU_InputR) != 0 ) - входной файл - файл растров
    //    else                     - входной файл - CTB-база
    //    param&FONCLU_Threshold - порог кластеризации
    //     если порог <=0  - устанавливается равен 3 ( расстояние >=3 - разные кластеры)
	//  ShowProgress - указатель на функцию, (void Show(long percent)),
    //    которой по ходу передается процент сделанной работы. Можно NULL.

    //    default param=0
    //    accept,extern_buf, ShowProgress  may be NULL
    //  standard usage (FormReader) FONFontClusters(CTBname,cluname,NULL,NULL,0,0,NULL);
	//  standard usage (Puma) FONFontClusters(CTBname,cluname,NULL,NULL,0,0x2000,NULL);
    FON_FUNC(int32_t) FONFontClusters(char *CTBname,char *cluname,void *accept,uchar *extern_buf,int32_t size,
					 			    uint32_t param,void *ShowProgress,uchar language);
	FON_FUNC(int32_t) FONCutOffClusters(welet *ww,int16_t numWelet);
    FON_FUNC(Bool32) FONCheckItself(uchar let,int32_t num,int16_t col,int16_t row);
////////////////////
	// функции для разрезания
	FON_FUNC(int32_t) FONRecogCharBound(RecRaster *recRast,RecVersions *collection,
								  int32_t *recBounds);
	FON_FUNC(int32_t) FONRecogKley(RecRaster *recRast,RecVersions *collection);
	// snap for RecogKley  (unglue)
	FON_FUNC(int32_t) FONShowSnap(void);
	FON_FUNC(int32_t) FONInitSnap(Handle hhh);
	FON_FUNC(int32_t) FONEndSnap(void);

//////////////////
//
//  Функции сравнения - тестирования кластеров
//
// сравнить - есть ли похожие среди кластеров с именами
// name1 & name2
//  return < 0 - error
//   else 0 - 255  (255 - the same )
//
    FON_FUNC(int32_t) FONCompareClustNames(int32_t name1,int32_t name2);
//
// найти кластеры, похожие на кластер с заданным номером
// номера - c 1
// записать в collect. Info - nomer кластера
//
    FON_FUNC(int32_t) FONGetSameClust(int32_t num,RecVersions *collect);


///////////////////////
// error code
#define ER_FON_NO_ERROR  0
#define ER_FON_NO_RECOG  1
#define ER_FON_OPEN      2
#define ER_FON_READ      3
#define ER_FON_MEMORY    4

   #ifdef __cplusplus
            }
   #endif

#endif

