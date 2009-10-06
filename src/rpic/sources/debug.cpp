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

# include "iolib.h"
//# include "layout.h"
//# include "extract.h"
//# include "kernel.h"
//# include "status.h"

# include "dpuma.h"
# include "debug.h"
//# include "pumadef.h"

//////////////////////////////////
Handle hDebugPictures = NULL;
Handle hRectanglePictures = NULL;
Handle hPolyPictures = NULL;
Handle hShowFirstAttempt = NULL;
Handle hShowAllGreat = NULL;
Handle hShowAllCancelledGreat = NULL;
Handle hShowCheckLetters = NULL;
Handle hShowBigLetters = NULL;
Handle hShowPossiblePics = NULL;
//Handle hDebugStrings = NULL;
//////////////////////////////////
void InitDebug()
{
	//layout = 0;

	LDPUMA_Registry(&hDebugPictures,"Выделение картинок...",NULL);
    //LDPUMA_RegVariable(hDebugLayout,"Установить уровень отладки.",&layout,"unsigned char");
	LDPUMA_RegistryHelp(hDebugPictures,"Модуль выделения картинок",FALSE);

    LDPUMA_Registry(&hShowPossiblePics,"Показать неуверенные картинки 1",hDebugPictures);
	LDPUMA_RegistryHelp(hShowPossiblePics,"Показывает неуверенные картинки, выделенные \
после первого прохода.",FALSE);

	LDPUMA_Registry(&hShowBigLetters,"Показать буквицы",hDebugPictures);
	LDPUMA_RegistryHelp(hShowBigLetters,"Показывает буквицы.",FALSE);


	LDPUMA_Registry(&hRectanglePictures,"Отменить поиск прямоугольных картинок",hDebugPictures);
	LDPUMA_RegistryHelp(hRectanglePictures,"Отменить поиск прямоугольных картинок \
(непрямоугольные ищутся отдельно).",FALSE);

	LDPUMA_Registry(&hPolyPictures,"Отменить поиск непрямоугольных картинок",hDebugPictures);
	LDPUMA_RegistryHelp(hPolyPictures,"Отменить поиск непрямоугольных картинок. \
(прямоугольные ищутся отдельно).",FALSE);

	LDPUMA_Registry(&hShowAllGreat,"Показать все бльшие компоненты 1",hDebugPictures);
	LDPUMA_RegistryHelp(hShowAllGreat,"Показывает все бльшие компоненты, \
из которых на первом проходе будут собираться картинки.",FALSE);

	LDPUMA_Registry(&hShowAllCancelledGreat,"Показать все забракованные бльшие компоненты 1",hDebugPictures);
	LDPUMA_RegistryHelp(hShowAllCancelledGreat,"Показывает все забракованные бльшие компоненты, \
из которых на первом проходе будут собираться картинки.",FALSE);

	LDPUMA_Registry(&hShowCheckLetters,"Показать проверку на заголовки 1",hDebugPictures);
	LDPUMA_RegistryHelp(hShowCheckLetters,"Показывает процесс проверки на заголовки",FALSE);

	LDPUMA_Registry(&hShowFirstAttempt,"Показать картинки 1",hDebugPictures);
	LDPUMA_RegistryHelp(hShowFirstAttempt,"Показывает картинки, выделенные \
после первого прохода.",FALSE);

	/*
#define REGVAR(a)	LDPUMA_RegVariable(hDebugLayout,#a,&a,"short int")

# ifdef LT_DEBUG
    LDPUMA_RegVariable(hDebugLayout,"LT_DebugGraphicsLevel",&LT_DebugGraphicsLevel,"int");

	REGVAR(bDebugTimeFlag);
	REGVAR(bDebugOptionCalibratePictureRemovingCriteria);
	REGVAR(bDebugOptionCalibrateDD_RemovingCriteria);
	REGVAR(bDebugOptionCalibrateLinearRemovingCriteria);
# endif
	REGVAR(bOptionForceOneColumn);
	REGVAR(bOptionBusinessCardsLayout);
	REGVAR(bOptionPointSizeAnalysis);
	REGVAR(bOptionSmartBreaking);
	REGVAR(bOptionInitialBreakingByPageMatrix);
	REGVAR(bOptionBlocksRemovingByPageMatrix);
	REGVAR(bOptionBlocksRemovingByDustDistribution);
	REGVAR(bOptionSpecialHorizontalCutting);
	REGVAR(bOptionWhiteStripsBreaking);
# ifdef SE_DEBUG
    REGVAR(SE_DebugGraphicsLevel);
# endif
# ifdef MA_DEBUG
    REGVAR(MA_DebugLevel);
# endif

#undef REGVAR
  */

}
/////////////////////////////////////////
void DoneDebug()
{
	/*
#define UNREGVAR(a)	LDPUMA_UnregVariable(&a)

# ifdef LT_DEBUG
    LDPUMA_UnregVariable(&LT_DebugGraphicsLevel);

	UNREGVAR(bDebugTimeFlag);
	UNREGVAR(bDebugOptionCalibratePictureRemovingCriteria);
	UNREGVAR(bDebugOptionCalibrateDD_RemovingCriteria);
	UNREGVAR(bDebugOptionCalibrateLinearRemovingCriteria);
# endif
	UNREGVAR(bOptionForceOneColumn);
	UNREGVAR(bOptionBusinessCardsLayout);
	UNREGVAR(bOptionPointSizeAnalysis);
	UNREGVAR(bOptionSmartBreaking);
	UNREGVAR(bOptionInitialBreakingByPageMatrix);
	UNREGVAR(bOptionBlocksRemovingByPageMatrix);
	UNREGVAR(bOptionBlocksRemovingByDustDistribution);
	UNREGVAR(bOptionSpecialHorizontalCutting);
	UNREGVAR(bOptionWhiteStripsBreaking);
# ifdef SE_DEBUG
    UNREGVAR(SE_DebugGraphicsLevel);
# endif
# ifdef MA_DEBUG
    UNREGVAR(MA_DebugLevel);
# endif

#undef UNREGVAR
  */
}
////////////////////////////////////////
