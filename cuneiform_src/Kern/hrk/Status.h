/*
 Copyright (c) 1993-2008, Cognitive Technologies
 All rights reserved.

 пЮГПЕЬЮЕРЯЪ ОНБРНПМНЕ ПЮЯОПНЯРПЮМЕМХЕ Х ХЯОНКЭГНБЮМХЕ ЙЮЙ Б БХДЕ ХЯУНДМНЦН ЙНДЮ,
 РЮЙ Х Б ДБНХВМНИ ТНПЛЕ, Я ХГЛЕМЕМХЪЛХ ХКХ АЕГ, ОПХ ЯНАКЧДЕМХХ ЯКЕДСЧЫХУ СЯКНБХИ:

 * оПХ ОНБРНПМНЛ ПЮЯОПНЯРПЮМЕМХХ ХЯУНДМНЦН ЙНДЮ ДНКФМШ НЯРЮБЮРЭЯЪ СЙЮГЮММНЕ
 БШЬЕ СБЕДНЛКЕМХЕ НА ЮБРНПЯЙНЛ ОПЮБЕ, ЩРНР ЯОХЯНЙ СЯКНБХИ Х ОНЯКЕДСЧЫХИ
 НРЙЮГ НР ЦЮПЮМРХИ.
 * оПХ ОНБРНПМНЛ ПЮЯОПНЯРПЮМЕМХХ ДБНХВМНЦН ЙНДЮ Б ДНЙСЛЕМРЮЖХХ Х/ХКХ Б
 ДПСЦХУ ЛЮРЕПХЮКЮУ, ОНЯРЮБКЪЕЛШУ ОПХ ПЮЯОПНЯРПЮМЕМХХ, ДНКФМШ ЯНУПЮМЪРЭЯЪ
 СЙЮГЮММЮЪ БШЬЕ ХМТНПЛЮЖХЪ НА ЮБРНПЯЙНЛ ОПЮБЕ, ЩРНР ЯОХЯНЙ СЯКНБХИ Х
 ОНЯКЕДСЧЫХИ НРЙЮГ НР ЦЮПЮМРХИ.
 * мХ МЮГБЮМХЕ Cognitive Technologies, МХ ХЛЕМЮ ЕЕ ЯНРПСДМХЙНБ МЕ ЛНЦСР
 АШРЭ ХЯОНКЭГНБЮМШ Б ЙЮВЕЯРБЕ ЯПЕДЯРБЮ ОНДДЕПФЙХ Х/ХКХ ОПНДБХФЕМХЪ
 ОПНДСЙРНБ, НЯМНБЮММШУ МЮ ЩРНЛ он, АЕГ ОПЕДБЮПХРЕКЭМНЦН ОХЯЭЛЕММНЦН
 ПЮГПЕЬЕМХЪ.

 щрю опнцпюллю опеднярюбкемю бкюдекэжюлх юбрнпяйху опюб х/хкх дпсцхлх кхжюлх "йюй
 нмю еярэ" аег йюйнцн-кхан бхдю цюпюмрхи, бшпюфеммшу ъбмн хкх ондпюгслебюелшу,
 бйкчвюъ цюпюмрхх йнллепвеяйни жеммнярх х опхцндмнярх дкъ йнмйпермни жекх, мн ме
 нцпюмхвхбюъяэ хлх. мх бкюдекеж юбрнпяйху опюб х мх ндмн дпсцне кхжн, йнрнпне
 лнфер хглемърэ х/хкх онбрнпмн пюяопнярпюмърэ опнцпюллс, мх б йнел яксвюе ме
 мея╗р нрберярбеммнярх, бйкчвюъ кчаше наыхе, яксвюимше, яоежхюкэмше хкх
 онякеднбюбьхе сашрйх, ябъгюммше я хяонкэгнбюмхел хкх онмеяеммше бякедярбхе
 мебнглнфмнярх хяонкэгнбюмхъ опнцпюллш (бйкчвюъ онрепх дюммшу, хкх дюммше,
 ярюбьхе мецндмшлх, хкх сашрйх х/хкх онрепх днунднб, онмеяеммше хг-гю деиярбхи
 рперэху кхж х/хкх нрйюгю опнцпюллш пюанрюрэ янблеярмн я дпсцхлх опнцпюллюлх,
 мн ме нцпюмхвхбюъяэ щрхлх яксвюълх), мн ме нцпюмхвхбюъяэ хлх, дюфе еякх рюйни
 бкюдекеж хкх дпсцне кхжн ашкх хгбеыемш н бнглнфмнярх рюйху сашрйнб х онрепэ.

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
 шшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшш
 шш                                                              шш
 шш     Copyright (C) 1990 Cognitive Technology Corporation.     шш
 шш     All rights reserved. This program is proprietary and     шш
 шш     a trade secret of Cognitive Technology Corporation.      шш
 шш                                                              шш
 шшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшш
 */
////////////////////////////////////////////////////////////////////
#ifndef _STATUS_H_	// 01.09.2000 E.P.
#define _STATUS_H_

#include "tuner.h"

////////////////////////////////////////////////////////////////////
#ifdef  CREATE_STATUS
#define Edef
#define VAL(a)  = a
#else
#define Edef extern
#define VAL(a)
#endif

//------------------ Name fields -------------------------
Edef char group_name[5] VAL("PAGE");
// group of files name
Edef int page_number VAL(1);
// current page number
Edef char group_dir[30] VAL("");
// directory for pages group

//---------- Source and target identification ------------
#define S_Scaner  1                   // Scaner
#define S_Tiff    2                   // Tiff file
#define S_Ed      4                   // Edit file
Edef unsigned char source_type VAL(S_Tiff);
// source type
#define T_Tiff    2                   // Tiff file
#define T_Rec     4                   // Recognize file
#define T_Ed       8                  // Edit file
Edef unsigned char target_type VAL(T_Rec);
// target type
#define T_Pack    1                   // Target should be packed
Edef unsigned char target_pack VAL(0);
// target packing flag
Edef unsigned char spell_check VAL(0);
// using spell checker for test result
Edef unsigned char layout VAL(0);
// using layout processing

//----------------- Scaner descriptions -------------------
Edef unsigned char brightness VAL(2);
Edef int intensity VAL(30);
Edef int scan_col VAL(0);
Edef int scan_row VAL(0);
Edef int scan_height VAL(275);
Edef int scan_width VAL(208);
//------------------ Image attributes ---------------------
Edef int image_type VAL(-1);
// image file type (0 - scanner)
Edef int image_handle VAL(-1);
// image file
Edef int image_blth VAL(0);
// pixels per line
Edef int image_height VAL(0);
// lines in file number
Edef int image_lth VAL(0);
// bytes per line
Edef unsigned char image_black VAL(0);
// mask for black pixels adding
Edef unsigned char image_white VAL(0);
// mask for wite pixels adding
Edef unsigned char matrix VAL(0);
// dot-matrix

//------------------ Colors asignement --------------------
Edef unsigned char attr_hor_menu VAL(0x30);
// attr for horizontal menu
Edef unsigned char attr_h_hor_menu VAL(0x1b);
// attr for its hightlighted items
Edef unsigned char attr_l_hor_menu VAL(0x3c);
// attr for key letter

Edef unsigned char attr_vert_menu VAL(0x30);
// attr for vertical menu
Edef unsigned char attr_h_vert_menu VAL(0x5b);
// attr for its hightlighted items
Edef unsigned char attr_b_vert_menu VAL(0x17);
// attr for border
Edef unsigned char attr_l_vert_menu VAL(0x3c);
// attr for key letter

Edef unsigned char attr_value VAL(0x1b);
// attr for value reading window
Edef unsigned char attr_h_value VAL(0x1b);
// attr for value hightlighting
Edef unsigned char attr_b_value VAL(0x1b);
// attr for window border

Edef unsigned char attr_line_help VAL(0x30);
// attr for line help
Edef unsigned char attr_line_help_head VAL(0x34);
// attr for line help

Edef unsigned char attr_window VAL(0x70);
// attr for working window
// Edef  int Help_handle       VAL(-1);   // handle of help file
Edef int Help_w_row VAL(3);
Edef int Help_w_col VAL(10);
// size and place for help window
Edef int Help_w_width VAL(18);
Edef int Help_w_height VAL(60);
Edef unsigned char attr_Help_window VAL(0x30);
// attr for window and text
Edef unsigned char attr_Help_border VAL(0x31);
// attr for border
Edef unsigned char attr_Help_field VAL(0x17);
// attr for noncurrent fields
Edef unsigned char attr_Help_current VAL(0x5f);
// attr for current field
Edef unsigned char attr_err VAL(0x4a);
// attr for message window
Edef unsigned char attr_b_err VAL(0x4a);
// attr for message window border

Edef unsigned char db_ed_type VAL(1);
// fragmentation of Ed
Edef unsigned char db_pidx_crit VAL(1);
Edef unsigned char db_box VAL(1);
Edef unsigned char db_status VAL(0);
Edef int db_stop_ln VAL(1);
Edef unsigned char db_pos_letters VAL(0);
Edef unsigned char db_convert_rtf VAL(1);
Edef unsigned char db_collect_events VAL(0);
Edef unsigned char db_skip_flag VAL(0);
Edef unsigned char no_formating VAL(0);
Edef unsigned char fax1x2 VAL(0);
Edef unsigned char force_one_column VAL(0);
Edef unsigned char bcr VAL(0);
Edef unsigned char bcr_layout VAL(0);
Edef unsigned char cut_page_left VAL(0);
Edef unsigned char cut_page_right VAL(0);
/////////////////////////////////////////////////////////////////////////////////////
//#if MORDA==LANG_ENGLISH
//Edef unsigned char language             VAL(0);
//#else
//Edef unsigned char language             VAL(3);
//#endif
////////////////////////////////////////////////////////////////////////////////////
Edef unsigned char language VAL(3);
Edef unsigned char langSer VAL(0);
Edef unsigned char langUkr VAL(0);
Edef unsigned char langBul VAL(0);
// BULGAR 31.08.2000 E.P.
Edef unsigned char multi_page_mode VAL(0);
Edef unsigned char multy_language VAL(0);
Edef unsigned char pictures_recog VAL(0);
Edef char windows_tmp_dir[256] VAL("");
// system directory
Edef unsigned char enable_table_recog VAL(0);
Edef char user_dict_name[256] VAL("");
Edef unsigned char cuts_glues_methode VAL(1);
Edef unsigned char erection_enable VAL(2);
Edef unsigned char cuts_point_methode VAL(1);
///////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _SR_NO_ED_FILE_
Edef unsigned char p2_active VAL(0); // disabled if no ed file option
#else
Edef unsigned char p2_active VAL(2);
#endif
///////////////////////////////////////////////////////////////////////////////////////////////
Edef unsigned char image_invers VAL(0);
// Windows KIRR Gr4 == negative DOS
Edef unsigned char two_cells VAL(0);
Edef unsigned char space_correction VAL(0);
Edef unsigned char mmx VAL(0);
Edef unsigned char spec_camera VAL(0);
Edef unsigned char no_linpen VAL(0);
Edef unsigned char p2_disable VAL(0);
Edef unsigned char db_end_parm VAL(0);
Edef char windows_dir[256] VAL("");
// windows directory for protect
Edef char mashine_name[32] VAL("");
// network mashine name
///////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _SR_NO_ED_FILE_
Edef char DisableEdFile VAL(0);
#endif
///////////////////////////////////////////////////////////////////////////////////////////////
Edef unsigned char ED_File_Emulate VAL(1);
Edef unsigned char NO_Punct VAL(0);
Edef unsigned char db_special_project VAL(0);
///////////////////////////////////////////////////////////////////////////////////////////////
#ifdef Ecom
Edef Ecom Q;
#else
extern unsigned char Q[];
#endif
///////////////////////////////////////////////////////////////////////////////////////////////
#endif	// _STATUS_H_
