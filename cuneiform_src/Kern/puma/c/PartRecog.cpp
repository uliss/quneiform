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

//#define _USE_REF_  // Nick 23.05.2001

#ifdef _DEBUG
#include <windows.h>
#endif
#include <stdio.h>
#include <string.h>

#include "puma.h"
#include "mpuma.h"
#include "specprj.h"
#include "ligas.h"		// 12.06.2002 E.P.

//#include "sjtl.h"

char    global_buf[64000];  // OLEG fot Consistent
int32_t   global_buf_len=0;   // OLEG fot Consistent

#ifdef _USE_REF_    // Nick 23.05.2001
 #include "ref.h"
#endif

#ifdef _DEBUG
#pragma warning(disable:4273)
Bool32 PUMA_Save(Handle hEdPage, const char * lpOutFileName, int32_t lnFormat, int32_t lnCode, Bool32 bAppend );
#endif

static Bool32 rblockProgressStep(uint32_t perc)
{
	return ProgressStep(2,NULL,perc);
}
static void   rblockProgressFinish( void )
{
	ProgressStep(2,NULL,100);
}
static Bool32 rselstrProgressStep(uint32_t perc)
{
	return ProgressStep(2,NULL,perc);
}
static void   rselstrProgressFinish( void )
{
	ProgressStep(2,NULL,100);
}

////////////////////////////////////////
static Bool32 MakeStrings(Handle hccom,Handle hcpage)
{
	Bool32 rc = TRUE;
	if(LDPUMA_Skip(hDebugStrings))
    {
	 RSELSTR_SetImportData(RSELSTR_FNRSELSTR_ProgressStep, (void*) rselstrProgressStep);
	 RSELSTR_SetImportData(RSELSTR_FNRSELSTR_ProgressFinish, (void*) rselstrProgressFinish);

 	 if(!ProgressStep(1,GetResourceString(IDS_MAKESTRING),5))
		rc = FALSE;

	 if(rc && !RSELSTR_ExtractTextStrings(hccom,hcpage))
	 {
		SetReturnCode_puma(RSELSTR_GetReturnCode());
		rc=FALSE;
	 }
	}
	else
	{
	 RBLOCK_SetImportData(RBLOCK_FNRBLOCK_ProgressStep, (void*) rblockProgressStep);
	 RBLOCK_SetImportData(RBLOCK_FNRBLOCK_ProgressFinish, (void*) rblockProgressFinish);

 	 if(!ProgressStep(1,GetResourceString(IDS_MAKESTRING),5))
		rc = FALSE;

	 if(rc && !RBLOCK_ExtractTextStrings(hccom,hcpage))
	 {
		SetReturnCode_puma(RBLOCK_GetReturnCode());
		rc = FALSE;
	 }
	}
//	RSELSTR_GetObjects(hccom,hcpage);
	return rc;
}
//////////////////////////////////////////
static Bool32 RecognizeSetup(int language)
{
	Bool32 rc = TRUE;
	// рапознавание строк
    PAGEINFO info = {0};
    GetPageInfo(hCPAGE,&info);

//    FrhPageSetup setup={0};
//    FrhFieldSetup fsetup={0};
    RSTR_Options opt={0};

//    opt.setup = &fsetup;
    opt.pageSkew2048 = info.Incline2048;//0
//    setup.nResolutionY = info.DPIY;//300;
    int32_t nResolutionY = info.DPIY;//300;
//    fsetup.nMaxLetWidth = 127;
//    fsetup.nMaxLetHeight = 64;
//    fsetup.wFlags = FFS_PRINTED;

    opt.language = language;
    global_buf_len=0;   // OLEG fot Consistent
//    if(!RSTR_NewPage(&setup,hCPAGE))
    if(!RSTR_NewPage(nResolutionY, hCPAGE))
	{
		SetReturnCode_puma(RSTR_GetReturnCode());
		rc= FALSE;
	}

	if(rc && !RSTR_SetOptions (&opt))
	{
		SetReturnCode_puma(RSTR_GetReturnCode());
		rc= FALSE;
	}
	else
	{
		// Настройка параметров
		uchar w8 = (uchar)gnLanguage;
		RSTR_SetImportData(RSTR_Word8_Language,&w8);

		uint16_t w16 = (uint16_t)info.DPIY;//300;
		RSTR_SetImportData(RSTR_Word16_Resolution,&w16);

		w8 = (uchar)gbFax100;
		RSTR_SetImportData(RSTR_Word8_Fax1x2,&w8);

		w8 = (uchar)gbDotMatrix;
		RSTR_SetImportData(RSTR_Word8_Matrix,&w8);

		w8 = 0;
		RSTR_SetImportData(RSTR_Word8_P2_active,&w8);

        w8 = 1;
        if(!LDPUMA_Skip(hDebugCancelStringsPass2))
            RSTR_SetImportData(RSTR_Word8_P2_disable,&w8);

		w8 = (uchar)gbSpeller;
		RSTR_SetImportData(RSTR_Word8_Spell_check,&w8);

		RSTR_SetImportData(RSTR_pchar_user_dict_name,gpUserDictName);

		// Передать язык в словарный контроль. 12.06.2002 E.P.
		// причем всегда 08.02.2008 DVP
		{
			uchar w8 = (uchar)language;
			RPSTR_SetImportData(RPSTR_FNIMP_LANGUAGE, &w8);
			RCORRKEGL_SetImportData(RCORRKEGL_FNIMP_LANGUAGE, &w8);
		}

	}
	return rc;
}

static Bool32 RecognizeStringsPass1(void)
{
	Bool32 rc = TRUE;
	// распознавание строк
	if(rc)
	{
		int count = CSTR_GetMaxNumber();
		int i;

		LDPUMA_StartLoop( hDebugRecognition, count );
		if(!ProgressStep(2,GetResourceString(IDS_PRG_RECOG),0))
			rc = FALSE;

		for(i=1;rc && i<=count;i++)
		{
			CSTR_line lin_out,lin_in;
			if(!ProgressStep(2,NULL,i*100/count))
				rc = FALSE;

			LDPUMA_LoopNext( hDebugRecognition );
			if(!LDPUMA_Skip(hDebugRecognition))
			{
				LDPUMA_Console("Остановились перед %i строкой.\n",i);
				LDPUMA_Console("Нажмите любую клавишу...\n");
				LDPUMA_WaitUserInput(hDebugRecognition,NULL);
			}

			lin_out = CSTR_NewLine(i, CSTR_LINVERS_MAINOUT, -1); // OLEG
			if(lin_out == (CSTR_line)NULL)
			{
				SetReturnCode_puma(CSTR_GetReturnCode());
				rc = FALSE;
				break;
			}

			lin_in  = CSTR_GetLineHandle (i,CSTR_LINVERS_MAIN);
			if(lin_in == (CSTR_line)NULL)
			{
				SetReturnCode_puma(CSTR_GetReturnCode());
				rc = FALSE;
				break;
			}

			if (!RSTR_Recog(lin_in, lin_out) ) // Recognition
			{
				SetReturnCode_puma(RSTR_GetReturnCode());
                rc = FALSE;
				break;
			}
			/*
			else
			{
			if( !LDPUMA_Skip(hDebugCancelStringsPass2)&&
			(!LDPUMA_Skip(hDebugCancelPostSpeller)||
			!gbSpeller) )
			{
			PrintResult(i,lin_out);
			}
			}
			*/

#ifdef _USE_REF_    // Nick 23.05.2001
			{
             int32_t numFind = 0;
			 REF_findEtaz(lin_in, lin_out, &numFind);
               // есть формулы ?
			 if( numFind > 0 )
			 {
				 // новое число строк
				 count = CSTR_GetMaxNumber();

				 CSTR_DeleteLine( lin_out );

				 // изменить номер текущей строки
				 RSTR_ChangeLineNumber(-1);
				 // перераспознать строку
				 i--;
			 }
			}
#endif
		}
		LDPUMA_DestroyRasterWnd();
	}

#ifdef _USE_REF_    // Nick 23.05.2001
	// попробуем найти куски формул
	REF_unionEtaz();
#endif

	if(rc)
	{
		if(!ProgressStep(2,GetResourceString(IDS_PRG_RECOG2),100))
			rc = FALSE;
		rc = RSTR_EndPage(hCPAGE);
		if(!rc)
			SetReturnCode_puma(RSTR_GetReturnCode());
	}

	return rc;
}
//////////////////////////////////////////
static Bool32 RecognizeStringsPass2()
{
	Bool32 rc = TRUE;
	// рапознавание строк

	uchar w8 = 1;
	RSTR_SetImportData(RSTR_Word8_P2_active,&w8);

	int count = CSTR_GetMaxNumber();
	int i;

	LDPUMA_StartLoop( hDebugRecognition, count );
	if(!ProgressStep(2,GetResourceString(IDS_PRG_RECOG2),0))
		rc = FALSE;

	for(i=1;rc && i<=count;i++)
	{
		CSTR_line lin_out,lin_in;
		if(!ProgressStep(2,NULL,i*100/count))
			rc = FALSE;

		LDPUMA_LoopNext( hDebugRecognition );
		if(!LDPUMA_Skip(hDebugRecognition))
		{
			LDPUMA_Console("Остановились перед %i строкой.\n",i);
			LDPUMA_Console("Нажмите любую клавишу...\n");
			LDPUMA_WaitUserInput(hDebugRecognition,NULL);
		}

		lin_out = CSTR_GetLineHandle(i, CSTR_LINVERS_MAINOUT);
		if(lin_out == (CSTR_line)NULL)
		{
        continue;
//			SetReturnCode_puma(CSTR_GetReturnCode());
//			rc = FALSE;
//			break;
		}

		lin_in  = CSTR_GetLineHandle (i,CSTR_LINVERS_MAIN);
		if(lin_in == (CSTR_line)NULL)
		{
        continue;
//			SetReturnCode_puma(CSTR_GetReturnCode());
//			rc = FALSE;
//			break;
		}

		if (!RSTR_Recog(lin_in, lin_out) ) // Recognition
		{
			SetReturnCode_puma(RSTR_GetReturnCode());
			break;
		}
		/*
		else
		{
        if( !LDPUMA_Skip(hDebugCancelPostSpeller)||
		!gbSpeller )
		{
		PrintResult(i,lin_out);
		}
        }
		*/

	}
	LDPUMA_DestroyRasterWnd();
	if(rc)
	{
		rc = RSTR_EndPage(hCPAGE);
		if(!rc)
			SetReturnCode_puma(RSTR_GetReturnCode());
	}

	return rc;
}
///////////////////////////////////////////////
/*
static Bool StoreColorStrings()
{
Bool            rc = TRUE;
int             count = CSTR_GetMaxNumber();
int             i;


	for(i=1;rc && i<=count;i++)
	{
	CSTR_line lin_in;

	  if(!ProgressStep(2,NULL,i*100/count))
	  rc = FALSE;

		lin_in  = CSTR_GetLineHandle (i,CSTR_LINVERS_MAINOUT);
		if(lin_in == (CSTR_line)NULL)
		{
		SetReturnCode_puma(CSTR_GetReturnCode());
		rc = FALSE;
		break;
		}
        CSTR_rast       rst=CSTR_GetFirstRaster(lin_in);
        CSTR_rast_attr  attr;
        Point32         point,size;
        uint32_t          lpColor[10];
        uchar           lpMask[4096];

		  for(rst=CSTR_GetNext(rst);rst;rst=CSTR_GetNext(rst))
		  {
		  CSTR_GetAttr(rst,&attr);
		  point.x = attr.col;
		  point.y = attr.row;
		  size.x  = attr.w;
		  size.y  = attr.h;
		  if( attr.flg & (CSTR_f_let|CSTR_f_bad|CSTR_f_punct) )
		  {
		  CSTR_GetImage(rst,lpMask,CSTR_TYPE_IMAGE_RS1);
		  *lpColor=3;
		  if( !RCOLOR_GetColorMask( point, lpMask, size, lpColor) )
		  {
		  SetReturnCode_puma(RCOLOR_GetReturnCode());
		  rc = FALSE;
		  return rc;
		  }
		  attr.ColorLtr   = *((uchar*)lpColor);
		  attr.ColorBack  = *(((uchar*)lpColor)+1);
		  CSTR_SetAttr(rst,&attr);
		  }
		  }

			}

			  return rc;
}*/
double portion_of_rus_letters(CSTR_line lin_ruseng)
{
    if( !lin_ruseng )
        return 0;
    CSTR_rast_attr  attr;
    CSTR_rast       rast=CSTR_GetFirstRaster(lin_ruseng);
    UniVersions     uv;
    int     nRus=0, nAll=0;
    for(rast = CSTR_GetNext(rast);rast;rast=CSTR_GetNext(rast))
    {
        CSTR_GetAttr(rast,&attr);
        CSTR_GetCollectionUni(rast,&uv);
        if( attr.flg&(CSTR_f_let) )
        {
            if( attr.language==LANG_RUSSIAN && uv.lnAltCnt && uv.Alt[0].Prob>100 &&
                !strchr("0123456789/%",uv.Alt[0].Code[0]) )
                nRus++;
            nAll++;
        }
    }
    if( !nAll )
        return 0;

    return (double)nRus/(double)nAll;
}
///////////////////////////////////////////////
Bool32 Recognize()
{
	Bool32 rc = TRUE;
	CSTR_DeleteAll();

	if(hCPAGE)
		CPAGE_UpdateBlocks(hCPAGE,/*TYPE_DESK*/TYPE_CPAGE_TABLE);
	//
	// Сохраним опсиание Layout в файл.
	//
	if(!LDPUMA_Skip(hDebugLayoutToFile))
	{
//Paul 29-12-2000
#ifdef NOSAVEBACKUP
        CPAGE_ClearBackUp(hCPAGE);
#endif
//
		if(!CPAGE_SavePage(hCPAGE,(char*)szLayoutFileName))
		{
			SetReturnCode_puma(CPAGE_GetReturnCode());
			return FALSE;
		}
		else
			LDPUMA_Console("Layout сохранен в файле '%s'\n",szLayoutFileName);
	}
	//
	// Прочитаем опсиание Layout из файла.
	//
	if(!LDPUMA_Skip(hDebugLayoutFromFile))
	{
		hCPAGE = CPAGE_RestorePage(TRUE,(char*)szLayoutFileName);
		if(hCPAGE==NULL)
		{
			SetReturnCode_puma(CPAGE_GetReturnCode());
			rc= FALSE;
		}
		else
		{
			CPAGE_SetCurrentPage(CPAGE_GetNumberPage(hCPAGE));
			LDPUMA_Console("Layout восстановлен из файла '%s'\n",szLayoutFileName);
		}
	}


	if(rc && IsUpdate(FLG_UPDATE_CCOM) )
	{
		if(LDPUMA_Skip(hDebugCancelComponent))
		{
			PRGTIME prev = StorePRGTIME(0, 5);
			PAGEINFO info = {0};
			if(GetPageInfo(hCPAGE,&info))
			{
				rc = ExtractComponents( FALSE , NULL, (puchar)info.szImageName);
			}
			else
			{
				SetReturnCode_puma(CPAGE_GetReturnCode());
				rc = FALSE;
			}
			RestorePRGTIME(prev);
		}
		else
			LDPUMA_Console("Пропущен этап выделения компонент.\n");
	}
	//
	// Получим описатель страницы
	//
	hCPAGE = CPAGE_GetHandlePage(CPAGE_GetCurrentPage());
	//
	// Выделим строки
	//

	if(rc)
	{
		if(LDPUMA_Skip(hDebugCancelStrings))
		{
			PRGTIME prev = StorePRGTIME(5, 10);
			rc = MakeStrings(hCCOM,hCPAGE);
			RestorePRGTIME( prev );
            //
            //  удалим общий контейнер
            //
            //CCOM_DeleteContainer(REXCGetContainer());hCCOM=NULL;

			//
			// распознаем строки
			//

			if(rc)
			{
				CSTR_SortFragm( 0 );
				rc = RecognizeSetup(gnLanguage);
				LDPUMA_DestroyRasterWnd();

				if(rc && LDPUMA_Skip(hDebugCancelRecognition))
				{
					PRGTIME prev = StorePRGTIME(10, 60);

                    if(1)
                    {
                        CSTR_SortFragm( 0 );
                        CSTR_line   ln;
                        CSTR_attr   attr;
                        int32_t       nf=CSTR_GetMaxFragment(0),i;
                        Handle      hBlock = CPAGE_GetBlockFirst(hCPAGE,TYPE_TEXT);
                        if( hBlock)
                        {
                            int32_t       *flagfrag;
                            flagfrag=(int32_t*)myAlloc(nf*sizeof(int32_t));

                            if( flagfrag )
                            {
                                memset(flagfrag,0,nf*sizeof(int32_t));
                                for (i=0; hBlock && i<nf ; i++)
                                {
                                    //nunfrag[i]=CPAGE_GetBlockInterNum(hCPAGE,hBlock);
                                    flagfrag[i]=CPAGE_GetBlockFlags(hCPAGE,hBlock);
                                    hBlock = CPAGE_GetBlockNext (hCPAGE,hBlock,TYPE_TEXT);
                                }
                                for(i=1;i<=nf;i++)
                                {
                                    ln=CSTR_FirstLineFragm( i, 0);
                                    if( ln )
                                    {
                                        CSTR_GetLineAttr(ln,&attr);
                                        if( flagfrag[attr.fragment-1]&CPAGE_BLOCK_USER )
                                        {
                                            attr.Flags |= CSTR_STR_HandFragment;
                                            CSTR_SetLineAttr(ln,&attr);
                                        }
                                        do
                                        {
                                            ln = CSTR_NextLineFragm(ln);
                                            if( ln )
                                            {
                                                CSTR_GetLineAttr(ln,&attr);
                                                if( flagfrag[attr.fragment-1]&CPAGE_BLOCK_USER )
                                                {
                                                    attr.Flags |= CSTR_STR_HandFragment;
                                                    CSTR_SetLineAttr(ln,&attr);
                                                }
                                            }
                                        }while( ln );
                                    }
                                }
                                myFree(flagfrag);
                            }
                        }

                    }
                    if(!LDPUMA_Skip(hDebugEnableSaveCstr1))
						{
							char    szCstrFileName[260];
							int     len=strlen( szInputFileName );
							strcpy( szCstrFileName, szInputFileName);
							szCstrFileName[len-4]=0;
							strcat( szCstrFileName, "_1.cst");
							CSTR_SaveCont(szCstrFileName);
						}
					rc = RecognizeStringsPass1();
					RestorePRGTIME( prev );
                    if(!LDPUMA_Skip(hDebugEnableSaveCstr2))
						{
							char    szCstrFileName[260];
							int     len=strlen( szInputFileName );
							strcpy( szCstrFileName, szInputFileName);
							szCstrFileName[len-4]=0;
							strcat( szCstrFileName, "_2.cst");
							CSTR_SaveCont(szCstrFileName);
						}
					if(rc)
					{
                        ///////////////////////////////
                        // OLEG : 01-05-18 : for GiP //
                        ///////////////////////////////
                        if(SPEC_PRJ_GIP==gnSpecialProject&&gnLanguage==LANG_RUSENG)
                        {
                            int         i,j,n;
                            double      s;
                            CSTR_line   lin_ruseng;

                            n = CSTR_GetMaxNumber();
                            for(s=0.0, i=1;i<=n;i++)
                            {
                                lin_ruseng = CSTR_GetLineHandle(i, 1);
                                s += portion_of_rus_letters(lin_ruseng);
                            }
                            if( n )
                                s /= (double)n;
                            if( n && s<0.4 )
                            {

                                for(i=0;i<=n;i++)
                                {
                                    for(j=1;j<10;j++)
                                    {
                                        lin_ruseng = CSTR_GetLineHandle(i, j);
                                        if( lin_ruseng  )
                                            CSTR_DeleteLine(lin_ruseng);
                                    }
                                }
                                gnLanguage = LANG_ENGLISH;
                                rc = RecognizeSetup(gnLanguage );
                                PRGTIME prev = StorePRGTIME(10, 60);
                                rc = RecognizeStringsPass1();
                                RestorePRGTIME( prev );
                            }
                        }
						if(RSTR_NeedPass2())
						{
							PRGTIME prev = StorePRGTIME(60, 85);

							if(LDPUMA_Skip(hDebugCancelStringsPass2))
								rc = RecognizeStringsPass2();
							else
								LDPUMA_Console("Пропущен этап второго прохода распознавания.\n");

							RestorePRGTIME( prev );
						}
						else
							LDPUMA_Console("RSTR считает, что второй проход не нужен.\n");
						if(!LDPUMA_Skip(hDebugEnableSaveCstr3))
						{
							char    szCstrFileName[260];
							int     len=strlen( szInputFileName );
							strcpy( szCstrFileName, szInputFileName);
							szCstrFileName[len-4]=0;
							strcat( szCstrFileName, "_3.cst");
							CSTR_SaveCont(szCstrFileName);
						}
                        //
						// Дораспознаем по словарю
						//
                        CSTR_SortFragm( 1 );
                        RPSTR_CollectCapDrops(1);
                        if(rc)
                            rc = ProgressStep(2,GetResourceString(IDS_CORRECTSPELL),85);
						if(rc && LDPUMA_Skip(hDebugCancelPostSpeller) &&
							gbSpeller )
						{
							if( !RPSTR_CorrectSpell(1))
							{
								SetReturnCode_puma(RPSTR_GetReturnCode());
								rc= FALSE;
							}
						}
						//
						// Скорректируем результат распознавани
						//
                        CSTR_SortFragm( 1 );
                        if(rc)
                            rc = ProgressStep(2,GetResourceString(IDS_CORRECTRESULT),90);
						if(rc && LDPUMA_Skip(hDebugCancelPostRecognition))
						{
//							if( !RPSTR_SetImportData(RPSTR_Bool32_Fax100,&gbFax100) ||
//                              !RPSTR_CorrectKegl(1))
							if( !RCORRKEGL_SetImportData(RCORRKEGL_Bool32_Fax100,&gbFax100) ||
                                !RCORRKEGL_CorrectKegl(1))
							{
								SetReturnCode_puma(RPSTR_GetReturnCode());
								rc= FALSE;
							}
						}
                        CSTR_SortFragm( 1 );
                        RPSTR_CorrectIncline(1);
//                        RPSTR_CollectCapDrops(1);
//#ifdef _DEBUG
                        if( rc && !LDPUMA_Skip(hDebugEnableSaveJtl) && szInputFileName[0] )
						{
                            if( My_SJTL_save(szInputFileName)==-1 )
							{
                                My_SJTL_open("one.frm", "one.jtl");
                                My_SJTL_save(szInputFileName);
							}
						}
//#endif
                        if(!LDPUMA_Skip(hDebugEnableSaveCstr4))
						{
							char    szCstrFileName[260];
							int     len=strlen( szInputFileName );
							strcpy( szCstrFileName, szInputFileName);
							szCstrFileName[len-4]=0;
							strcat( szCstrFileName, "_4.cst");
							CSTR_SaveCont(szCstrFileName);
						}
						//
						// Печать результатов в консоль
						//
						LDPUMA_DestroyRasterWnd();

                        int count = CSTR_GetMaxNumber();
						if( rc &&
							LDPUMA_Skip(hDebugCancelConsoleOutputText) &&
							LDPUMA_IsActive())
						{
							for(int i=1;i<=count;i++)
                                {
								PrintResult(i,CSTR_GetLineHandle(i, CSTR_LINVERS_MAINOUT),hCPAGE);
                                }
						}
                        /*
                        //
                        // Печать строк во временный файл
                        //
                        Handle tmpfile=LDPUMA_FOpen(NULL,"at");
                        char    str[1024];
                        if( tmpfile )
                            {
                            for(int i=1;i<=count;i++)
                                    {
                                    CSTR_LineToTxt(CSTR_GetLineHandle(i, 1),str);
                                    LDPUMA_FPuts(tmpfile,str);
                                    }
                            LDPUMA_FClose(tmpfile);
                            }
                            */
						//
                        // OLEG fot Consistent
						if( SPEC_PRJ_CONS==gnSpecialProject )
						{
							char    *   buf = &global_buf[0], buf_str[1024];
							char	*	pb = buf;
							global_buf_len=0;
							CSTR_line   buf_line;
							count = CSTR_GetMaxNumber();

							for(int i=1;i<=count;i++)
							{
								buf_line = CSTR_GetLineHandle(i, 1);
								CSTR_LineToTxt_Coord(buf_line,buf_str,1023);
								strcpy(pb, buf_str);
								int len = strlen(pb);
								pb += len + 1;
								global_buf_len+=len+1;
							}
							*pb = 0;
							global_buf_len++;

							// OLEG fot Consistent
						}
						// Отформатируем результат
						//

						//Нормализуем вертикальные строки

						RPSTR_NormalizeVertStr();

						if(!ProgressStep(2,GetResourceString(IDS_FORMAT),95))
							rc = FALSE;
						if(rc)
						{
							if(LDPUMA_Skip(hDebugCancelFormatted))
							{
								SetOptionsToFRMT();
								if(ghEdPage)
								{
									CED_DeletePage(ghEdPage);
									ghEdPage = NULL;
								}
								if(!RFRMT_Formatter(szInputFileName,&ghEdPage))
								{
									SetReturnCode_puma(RFRMT_GetReturnCode());
									rc = FALSE;
								}
								else
								{
									if(!LDPUMA_Skip(hDebugEnablePrintFormatted))
									{
										char szFileName[260];
										strcpy(szFileName,szInputFileName);
										strcat(szFileName,"_tmp_.rtf");
										SetOptionsToFRMT();
										rc = RFRMT_SaveRtf((char*)szFileName,8);
										strcpy(szFileName,szInputFileName);
										strcat(szFileName,"_tmp_.fed");
										PUMA_Save(ghEdPage, szFileName, 0, 0, FALSE);
									}
								}
							}
							else
								LDPUMA_Console("Пропущен этап форматирования.\n");
						}
					}
                                }
                                else
									LDPUMA_Console("Пропущен этап распознавания.\n");
                        }
                }
                else
					LDPUMA_Console("Пропущен этап выделения строк.\n");
					}/*
					 if(rc)
					 {
					 if(LDPUMA_Skip(hDebugCancelStringsColor) &&
					 LDPUMA_Skip(hDebugCancelBinarize))
					 {
					 PRGTIME prev = StorePRGTIME(90, 100);
					 rc = StoreColorStrings();
					 RestorePRGTIME( prev );
					 }
					 else
					 LDPUMA_Console("Пропущен этап определения цвета букв и фона.\n");
					 }
					 */
					 LDPUMA_Skip(hDebugCancelFictive);
					 if(!ProgressStep(2,NULL,100))
						 rc = FALSE;
					 return rc;
}
