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

/*
  ЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫ
  ЫЫ                                                              ЫЫ
  ЫЫ     Copyright (C) 1990 Cognitive Technology Corporation.     ЫЫ
  ЫЫ     All rights reserved. This program is proprietary and     ЫЫ
  ЫЫ     a trade secret of Cognitive Technology Corporation.      ЫЫ
  ЫЫ                                                              ЫЫ
  ЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫ
*/
////////////////////////////////////////////////////////////////////
#ifndef _STATUS_H_	// 01.09.2000 E.P.
#define _STATUS_H_

#include "nt_types.h"
#include "tuner.h"

////////////////////////////////////////////////////////////////////
//AK for Snap.dll
#ifndef _SNAP_
//   #include "face_lng.h"
#endif
////////////////////////////////////////////////////////////////////
#ifdef  CREATE_STATUS
        #define Edef
        #define VAL(a)  = a
#else
        #define Edef extern
        #define VAL(a)
#endif
//--------------- System information ---------------------
Edef CHAR dos_tiger_dir[160]  VAL("..\\bin\\"); // system directory

//------------------ Name fields -------------------------
Edef  CHAR group_name[5]      VAL("PAGE");  // group of files name
Edef  INT  page_number        VAL(1);       // current page number
Edef  CHAR group_dir[30]      VAL("");      // directory for pages group

//---------- Source and target identification ------------
#define S_Scaner  1                   // Scaner
#define S_Tiff    2                   // Tiff file
#define S_Ed      4                   // Edit file
Edef  BYTE source_type VAL(S_Tiff);   // source type
#define T_Tiff    2                   // Tiff file
#define T_Rec     4                   // Recognize file
#define T_Ed       8                  // Edit file
Edef  BYTE target_type        VAL(T_Rec);     // target type
#define T_Pack    1                   // Target should be packed
Edef  BYTE target_pack        VAL(0); // target packing flag
Edef  BYTE spell_check        VAL(0); // using spell checker for test result
Edef  BYTE layout             VAL(0); // using layout processing

//----------------- Scaner descriptions -------------------
Edef BYTE brightness           VAL(2);
Edef INT intensity             VAL(30);
Edef INT scan_col              VAL(0);
Edef INT scan_row              VAL(0);
Edef INT scan_height           VAL(275);
Edef INT scan_width            VAL(208);

//------------------ Image attributes ---------------------
Edef INT  image_type           VAL(-1); // image file type (0 - scanner)
Edef INT  image_handle         VAL(-1); // image file
Edef INT  image_blth           VAL(0);  // pixels per line
Edef INT  image_height         VAL(0);  // lines in file number
Edef INT  image_lth            VAL(0);  // bytes per line
Edef BYTE image_black          VAL(0);  // mask for black pixels adding
Edef BYTE image_white          VAL(0);  // mask for wite pixels adding
Edef BYTE matrix               VAL(0);  // dot-matrix

//------------------ Colors asignement --------------------
Edef BYTE attr_hor_menu        VAL(0x30); // attr for horizontal menu
Edef BYTE attr_h_hor_menu      VAL(0x1b); // attr for its hightlighted items
Edef BYTE attr_l_hor_menu      VAL(0x3c); // attr for key letter

Edef BYTE attr_vert_menu       VAL(0x30); // attr for vertical menu
Edef BYTE attr_h_vert_menu     VAL(0x5b); // attr for its hightlighted items
Edef BYTE attr_b_vert_menu     VAL(0x17); // attr for border
Edef BYTE attr_l_vert_menu     VAL(0x3c); // attr for key letter

Edef BYTE attr_value           VAL(0x1b); // attr for value reading window
Edef BYTE attr_h_value         VAL(0x1b); // attr for value hightlighting
Edef BYTE attr_b_value         VAL(0x1b); // attr for window border

Edef BYTE attr_line_help       VAL(0x30); // attr for line help
Edef BYTE attr_line_help_head  VAL(0x34); // attr for line help

Edef BYTE attr_window          VAL(0x70); // attr for working window

// Edef  INT Help_handle       VAL(-1);   // handle of help file
Edef  INT Help_w_row           VAL(3);
Edef  INT Help_w_col           VAL(10);   // size and place for help window
Edef  INT Help_w_width         VAL(18);
Edef  INT Help_w_height        VAL(60);
Edef  BYTE attr_Help_window    VAL(0x30); // attr for window and text
Edef  BYTE attr_Help_border    VAL(0x31); // attr for border
Edef  BYTE attr_Help_field     VAL(0x17); // attr for noncurrent fields
Edef  BYTE attr_Help_current   VAL(0x5f); // attr for current field
Edef BYTE attr_err             VAL(0x4a); // attr for message window
Edef BYTE attr_b_err           VAL(0x4a); // attr for message window border

Edef BYTE db_ed_type           VAL(1);    // fragmentation of Ed
Edef BYTE db_pidx_crit         VAL(1);
Edef BYTE db_box               VAL(1);
Edef BYTE db_status            VAL(0);
Edef INT  db_stop_ln           VAL(1);
Edef BYTE db_pos_letters       VAL(0);
Edef BYTE db_convert_rtf       VAL(1);
Edef BYTE db_collect_events    VAL(0);
Edef BYTE db_skip_flag         VAL(0);
Edef BYTE no_formating         VAL(0);
Edef BYTE fax1x2               VAL(0);
Edef BYTE force_one_column     VAL(0);
Edef BYTE bcr                  VAL(0);
Edef BYTE bcr_layout           VAL(0);
Edef BYTE cut_page_left        VAL(0);
Edef BYTE cut_page_right       VAL(0);
/////////////////////////////////////////////////////////////////////////////////////
//#if MORDA==LANG_ENGLISH
//Edef BYTE language             VAL(0);
//#else
//Edef BYTE language             VAL(3);
//#endif
////////////////////////////////////////////////////////////////////////////////////
Edef BYTE language              VAL(3);
Edef BYTE langSer               VAL(0);
Edef BYTE langUkr               VAL(0);
Edef BYTE langBul               VAL(0);	// BULGAR 31.08.2000 E.P.
Edef BYTE multi_page_mode       VAL(0);
Edef BYTE multy_language        VAL(0);
Edef BYTE pictures_recog        VAL(0);
Edef CHAR windows_tmp_dir[256]  VAL(""); // system directory
Edef BYTE enable_table_recog    VAL(0);
Edef CHAR user_dict_name[256]   VAL("");
Edef BYTE cuts_glues_methode    VAL(1);
Edef BYTE erection_enable       VAL(2);
Edef BYTE cuts_point_methode    VAL(1);
///////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _SR_NO_ED_FILE_
Edef BYTE p2_active             VAL(0); // disabled if no ed file option
#else
Edef BYTE p2_active             VAL(2);
#endif
///////////////////////////////////////////////////////////////////////////////////////////////
Edef BYTE image_invers          VAL(0);  // Windows KIRR Gr4 == negative DOS
Edef BYTE two_cells             VAL(0);
Edef BYTE space_correction      VAL(0);
Edef BYTE mmx                   VAL(0);
Edef BYTE spec_camera           VAL(0);
Edef BYTE no_linpen             VAL(0);
Edef BYTE p2_disable            VAL(0);
Edef BYTE db_end_parm VAL(0);
Edef CHAR windows_dir[256]      VAL(""); // windows directory for protect
Edef CHAR mashine_name[32]      VAL(""); // network mashine name
///////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _SR_NO_ED_FILE_
Edef CHAR DisableEdFile         VAL(0);
#endif
///////////////////////////////////////////////////////////////////////////////////////////////
Edef BYTE ED_File_Emulate       VAL(1);
Edef BYTE NO_Punct              VAL(0);
Edef BYTE db_special_project	VAL(0);
///////////////////////////////////////////////////////////////////////////////////////////////
#ifdef Ecom
Edef Ecom Q;
#else
extern BYTE Q[];
#endif
///////////////////////////////////////////////////////////////////////////////////////////////
#define OPTIONS_LTH &db_end_parm - dos_tiger_dir
#endif	// _STATUS_H_
