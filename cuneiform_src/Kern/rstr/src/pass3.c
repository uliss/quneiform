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

//////////////////////////////////////////////////////////////////////
//                                                                  //
//                      Third pass                                  //
//                                                                  //
//////////////////////////////////////////////////////////////////////
#include <stdlib.h>
 #include <sys/stat.h>
/* #include <io.h> */
 
 #include <stdlib.h>
 #include <setjmp.h>
 #include <assert.h>
 #include <stdio.h>
 #include <fcntl.h>
/* #include <direct.h>*/
 #include <string.h>

 #include "globus.h"
 #include "imgaccs.h"
 #include "resource.h"
#include "func.h"
#include "alphabet.h"
#include "status.h"
#include "ligas.h"
#include "linear.h"
#include "linutil.h"
#include "p2libr.h"
#include "tm.h"
#include "lang.h"
#include "ccom.h"
#include "leo.h"
#include "std.h"
#include "rsadd.h"
#include "cuthdr.h"
#include "dpuma.h"
#include "match_wd.h"
#include "specprj.h"

#include "compat_defs.h"

static void accept_Cell(cell *ce,CSTR_rast_attr*co, CCOM_comp *cmp, Bool sca);

// P2_COUR.C
Bool32 RecogLEO(RecRaster *Rs,uchar Language,UniVersions *Vs);
// ERECTION.C
extern int16_t    erection_inc;
// DIFFRV.C
void final_descriminate(cell *b, cell *e);
// EVN.DLL
extern uchar*   EVNMakeLine( RecRaster   *rRaster,int32_t parm);
// RCM.C
extern  STRLN   page_lines[];
extern  int32_t   num_of_lines;
extern Bool32 rstr_kit_realloc(void);
extern Bool32 trees_load(void);
extern void ErrorExit(int Code);
extern Bool32 trees_load_fict(void);
extern puchar string_curr;
extern uchar line_scale,line_alphabet, line_minus, line_handfragment,line_pointsusp;
extern Bool line_readyBL,line_BL;
extern uchar line_tabcell;
extern uchar db_trace_flag;
extern int16_t nIncline;
// PASSe.C functions
extern void   set_spell_solid(void);
extern uchar   english_word_recognize(void);
extern uchar   small_english_str(void) ;
extern void   russian_english_context(void);
// from Acc_tabs.c
void    correct_let_tables(void);
Bool    is_russian_language(uchar lang);

extern int32_t Flag_Courier;
extern int16_t line_number;
extern FILE *dbg_f;
extern uchar prop_in_trouble;
extern uchar let_to_dust;
extern Bool bCancelled;
extern int16_t bs_got;
extern int16_t flag_cut_point;
extern uchar language;
extern uchar CodePages[];
extern uchar decode_ASCII_to_[256][4];
extern char alphabet[256];
int16_t    page_nIncline;
uchar   pass2;
int16_t    del_squares();
Bool   ProgressSetPhase (char *text, uint16_t wPhaseSize);
Bool   ProgressSetPercentage (uint16_t wPercentage);
Bool   ProgressHandleMessages (void);
Bool   check_cancel(uint16_t t);
Bool   snap_skip();
int16_t    tot_strings(void);
Bool   kernel_reload_vocs(void);
void   delete_spaces(void);

uchar   no_linear_crit=1;
uchar   save_sp;

static  void pass3_special_recode(CSTR_line ln);
void pass3_table_points_suspension(CSTR_line ln);
void pass3_table_points_suspension2(CSTR_line ln);
static  Bool recog_minus(void);
static  int16_t p2_GetPs(void);
static  int16_t p2_GetPs_up(void);
static  void cstr_rerecog_leo(CSTR_line ln);
static  void add_vers_underlined(void);
static  Bool32  cstr_scaled_in_res(CSTR_line ln);
static  Bool32  cstr_kill_scaled(CSTR_line ln);
static  void import_lines_features(void);
static  void courier_let(CSTR_line ln);
static  Bool del_spaces_before_carry(CSTR_line ln);
static  void pass3_Ps(CSTR_line lin);
static  void delete_far_dust(CSTR_line lin);
static  void CSTR_refresh_flags(CSTR_line ln);
static  void set_cells_language(uchar lang);
static  void pass_start();
static  void convert_ligas(void);
static  void postrecog();
static  void set_normal_unrecog(void);
static  Bool test_short_english_blines(void);
void Cells2CSTR(CSTR_line lin,CSTR_line lino,cell *cur,Bool32 enable_scaled);
void    make_all_cuts( void);
void    make_all_glues(void);
void    all_cell_levcut(int16_t arg);
void    letters_ini(CSTR_line lin, Bool enable_scaling);
void clear_cells(void);
void clear_cells_pass2(void);
void clear_dust_alternates(void);

static  Bool    font_ready=FALSE;
static  CSTR_line   lne, lneout, lnout, lnsave;
static int16_t  pass3_num_killed_line()
{
int16_t n;
cell    *c;
for(n=0,c=cell_f()->nextl;c!=cell_l();c=c->nextl)
    {
    if( c->flg&c_f_bad )
        n++;
    }
return n;
}

static void  pass3_empty(CSTR_line lin,CSTR_line lino)
{
CSTR_attr               attrlin={0};
CSTR_rast       rst=CSTR_GetNext(CSTR_GetFirstRaster(lin));

if( lin )
    CSTR_GetLineAttr(lin, &attrlin);
CSTR_ClearLine(lino,(int16_t)(-16000+1),(int16_t)(0x7fff-1));

attrlin.col=0;
attrlin.row=0;
attrlin.wid=0;
attrlin.hei=0;

attrlin.r_col=0;
attrlin.r_row=0;
attrlin.r_wid=0;
attrlin.r_hei=0;

attrlin.l_col=0;
attrlin.l_row=0;
attrlin.l_wid=0;
attrlin.l_hei=0;
attrlin.incline             =nIncline;
attrlin.language                =language;
attrlin.erection                =erection_inc;
if( language==LANG_RUSSIAN && multy_language )
    attrlin.language            =LANG_RUSENG;
if( language==LANG_RUSSIAN && langUkr )
    attrlin.language            =LANG_UKRAINIAN;
if( language==LANG_RUSSIAN && langSer )
    attrlin.language            =LANG_SERBIAN;
if( language==LANG_RUSSIAN && langBul )
    attrlin.language            =LANG_BULGAR;
strcpy((char*)attrlin.VersionName,"EmptyLine");
//attrlin.Flags|=CSTR_STR_EMPTY;
CSTR_SetLineAttr(lino, &attrlin);
CSTR_SetLineAttr(lin,  &attrlin);
if( db_status && snap_activity('g') )
        {
    CSTR_line  l[]={lino};
    snap_show_text("end recog - empty line");
    snap_monitor_ori(&l[0],1);
        }
return;
}

static void ReRecognizeComponents()
{
cell          *cl;

cl=cell_f();
while(1){
        cl=cl->nextl;
        if(!cl || !cl->next) break;
        short_recog_cell(cl);
         }
}

str * first_string_language(void);
str * next_string_language(void);

void proc_bI(int16_t pass);
void proc_ii(void);


void save_rest_bases(int16_t mode, int16_t line_crit);
void save_rest_incline(int16_t mode);

void proc_Ukr( void ); // see module UKR.C

void cuts_glues(void);

void cstr2txt(char *buf,CSTR_line ln,CSTR_line lout)
{
char        s[2]={'\0','\0'};
CSTR_rast       c=CSTR_GetFirstRaster(p2_active?(lout?lout:ln):ln);
RecVersions     v;
if( !lout )
    return;
sprintf(buf,"line number start %d ", line_number);
for(c=CSTR_GetNext(c);c;c=CSTR_GetNext(c))
    {
    CSTR_GetCollection(c,&v);
    if( v.lnAltCnt )
        s[0]=v.Alt[0].Code;
    else
        s[0]=(unsigned char)176;
    strcat(buf, s );
    if( strlen(buf)>=252 )
        break;
    }
return;
}

void cell2txt(char *buf)
{
char s[2]={'\0','\0'};
cell    *c;
sprintf(buf,"line number stop %d ", line_number);
for(c=cell_f()->next;c!=cell_l();c=c->next)
    {
    s[0]=c->vers[0].let;
    strcat(buf, s );
    if( strlen(buf)>=252 )
        break;
    }
return;
}

// for special project
void kill_dusts_spaces(void)
{
cell *c;
B_LINES my_bases;
get_b_lines(NULL,&my_bases);
c=cell_f()->next;
if( c->nvers &&
   // memchr("\xD7\xD8",c->vers[0].let,2)
	(c->vers[0].let == left_quocket || // макры 10.09.2000 E.P.
	 c->vers[0].let == right_quocket)
   )

    del_cell(c);
for(c=cell_f()->next;c!=cell_l();c=c->next)
    {
    if( c->flg&(c_f_punct|c_f_space)||
        c->row>my_bases.b2+2 && c->row<my_bases.b3+2 &&
        c->row+c->h>my_bases.b3+1   && c->nvers==0
        )
        c=del_cell(c);
    }
return;
}

Bool unique_upper(uchar c)
{
Bool    ret;
switch(language)
    {
    case    LANG_ENGLISH:
        ret=(strchr("96543ABDEFGHKLMNQR",c)!=NULL);
        break;
    case    LANG_RUSSIAN:
        ret=(strchr("АБЕ954",c)!=NULL);
        break;
    default:
        ret=FALSE;
        break;
    }
return ret;
}

Bool    test_bl_comps(CSTR_line ln)
{
B_LINES         my_bases;
CSTR_rast       rst=CSTR_GetFirstRaster(ln);
CSTR_rast_attr  attr;
RecVersions     vers;
int16_t             pen;

get_b_lines(NULL,&my_bases);
for(pen=0,rst=CSTR_GetNext(rst);rst;rst=CSTR_GetNext(rst))
    {
    CSTR_GetAttr(rst,&attr);
    CSTR_GetCollection(rst,&vers);
    if( abs(attr.row-my_bases.b2)>2 &&  // conflict
        vers.lnAltCnt &&                // +
        vers.Alt[0].Method==13 &&       // neural
        is_upper(vers.Alt[0].Code) &&   // Capital
        unique_upper(vers.Alt[0].Code)&&// unigue
        vers.Alt[0].Prob>200 )          // validate
        {
        pen++;
        }
    }
return (pen>0);
}

void pass3(CSTR_line ln,CSTR_line lout)
{
    int16_t     h;
    int16_t     lsq;
    uchar    Lang;
    int16_t     Step=0,no_crit;
    int16_t     eng=0;
    Bool    snap_active=FALSE,line_scale0=FALSE,stop_pass2=FALSE;
    CSTR_attr   lattr;

    pass_start();
    snap_is_active();
    font_ready=FALSE;

    if( !erection_language(language) )
        erection_enable    = 0;
    if(p2_active)
        { // some init
        extern Bool pass4_in;
        pass4_in = TRUE;
        p2_init();  //init data for second pass
        pass2=0; // safe init
        }

    if( snap_is_active() || snap_is_marked(ln) )
        {
        char        buf[256];
        cstr2txt(buf,ln,lout);

        snap_show_text(buf);
        snap_active=TRUE;
        if( snap_is_marked(ln) )
            {
            snap_show_text("Marked line");
            snap_monitor();
            }
        }

        set_tab_incl();
        db_trace_flag &= 0xf8;   // reset detailed snap
        kit_init();
        Step         = 0;
        no_crit      = 0;
        setup_string();
        if(p2_active)
            {
            snap_newline();
            bs_got=0;
            db_trace_flag &= 0xf8;  // reset detailed snap
            if( language==LANG_RUSSIAN && multy_language )
                {
                lneout = CSTR_GetLineHandle(line_number, CSTR_LINVERS_ENGOUT);
                if( lneout )
                    CSTR_PackLine(lneout);
                }
// разбирательство c курсивом и прямым шрифтом с использованием кластеров
            cstr_erection_pass2(lout);
            if( language==LANG_RUSSIAN )
                multi_bas=0;
            goto got_line;          // skip bilingual block
            }

            pass2 = (  multy_language );

            Lang=language;

            if( pass2 )
                {
                lnout = lout;
                lne = CSTR_NewLine(line_number, CSTR_LINVERS_ENG, -1);
                if( !lne )
                    {
                    ErrorExit(RSTR_ERR_CSTR);
                    return ;
                    }
                lneout = CSTR_NewLine(line_number, CSTR_LINVERS_ENGOUT, -1);
                if( !lneout )
                    {
                    ErrorExit(RSTR_ERR_CSTR);
                    return ;
                    }
                CSTR_CopyLine (lne,ln);
                }

for (Step=0; Step<pass2+1; Step++ )
    {
        if(p2_active && Step > 0)
            break;

        if( pass2 )
            {
            if(!Step)
                language = LANG_RUSSIAN;
            else
                language = LANG_ENGLISH;
            trees_load_fict();
            }
        bs_got=0;
        db_trace_flag &= 0xf8;   // reset detailed snap

        setup_dump ();

        if(pass2)
            {
            if( !Step )
                snap_newline();
            }
        else
            snap_newline();

// коррекция наклона строки
        skew_corr(!Step?ln:lne,0);

        snap_newpass('d');

// выбор символов из исходной строки в рабочий массив
        letters_ini(!Step?ln:lne, 1);

        if( !Step )
            line_scale0=line_scale;
// goto ready BL

// если базовые линии были найдены (агрегатированная строка из таблицы)
//		- берем готовые базовые линии, добавление мелочи из исходной строки,
//		- отсечение альтернатив по базовым линиям. Проверка корректности.
        if( line_readyBL && !line_scale )
            {
            int16_t old_nIncline=nIncline, bado, badn;
            lin_str=!Step?ln:lne;
            p2_setBasLines(lin_str);
            if( old_nIncline!=nIncline )
            {
                clear_cells();
                letters_ini(lin_str,1 );
            }
            bring_dust(0);
            if (snap_activity('d'))
                {
                snap_show_text("Bring dust begin (ready BL)");
                snap_monitor();
                }
            prop(0);
            all_caps = 0;
            bado = pass3_num_killed_line();
            all_cell_levcut(1);    // apply penalties
            badn = pass3_num_killed_line();
            if( bado<2 && badn>0 &&
				// уходим, только если убили лишнего
				badn > bado + 1 // Nick 16.06.2001
				)
                {
                CSTR_GetLineAttr(lout,&lattr);
                if( lattr.Flags & CSTR_STR_ReadyBL )
                    lattr.Flags -=CSTR_STR_ReadyBL;
                //CSTR_GetLineAttr(lout,&lattr);
				CSTR_SetLineAttr(lout,&lattr); // Nick 13.02.01
                clear_cells();
                letters_ini(!Step?ln:lne, 1);
                goto no_ready_BL;
                }
            if( recog_minus() )
                goto ready_result;
            bs_got=1;
            goto ready_BL;
            }

no_ready_BL:;
        if(!p2_active&&!Step)
            avdens();
        if ((cell_f()->nextl)->next==NULL)
        {
            if( language==LANG_ENGLISH && Step )
                { eng=0;  break;  }
            pass3_empty(ln,lout);
            goto cont;
        }

        if( !spec_camera && pass2 && language==LANG_ENGLISH)
            ReRecognizeComponents();

// удалить "черные квадраты"
        lsq = del_squares();

        if (lsq && snap_activity('d'))
            {
            snap_show_text("Squares deleted");
            snap_monitor();
            }

        if ((cell_f()->nextl)->next==NULL)
            {
             if( language==LANG_ENGLISH && Step )
                { eng=0;  break;  }
             pass3_empty(ln,lout);
             goto cont;
            }

// ищутся допустимые пропорции букв в строке
         prop(0);

        if( spec_camera )
            no_crit      = 0;
        else
            no_crit=small_english_str();

        if( !Step || Step && !no_crit )
            {
            if (snap_activity('d'))
                {
                snap_show_text("Basedraft begin");
                snap_monitor();
                }
// поиск БЛ, добавление мелочи
            basedraft(!Step?ln:lne);
            }
        else
            {
            if (snap_activity('d'))
                {
                snap_show_text("Bring dust begin (short line)");
                snap_monitor();
                }
            bring_dust(0);
            }
        if( spec_camera && test_bl_comps(!Step?ln:lne) )
            {
            no_linpen=1; // NO CUT
            }

// характеристики по позиции на БЛ
        cell_by_base();
        import_lines_features();
        save_rest_incline(Step);
        if( !Step || Step && !no_crit )
            {
            if (snap_activity('d'))
                {
                snap_show_text("Linpos begin");
                snap_monitor();
                }
// уточнение БЛ, анализ многобазовости, отсечение альтернатив по БЛ
            linpos(!Step?ln:lne);
            }
        else
            {
            if (snap_activity('d'))
                {
                snap_show_text("cells levcut begin (short line)");
                snap_monitor();
                }
            all_cell_levcut(1);    // apply penalties
            }

ready_BL:;
        if( spec_camera )
            no_crit      = 0;
        else if( !no_crit )
            no_crit = test_short_english_blines();
        if( !Step )
            save_rest_bases(0,0);    // save base lines on 1 Step in Rus/Eng cycle

        if( Step && no_crit )    // bad base lines on the eng line in Rus/Eng cycle
            {
            save_rest_bases(1,1);    // restore base lines on 2 Step in Rus/Eng cycle
            bs_got=1;
            }

        if ((cell_f()->nextl)->next == NULL)
            {
            if (snap_activity('d'))
                {
                snap_show_text("Skiped as dust");
                snap_monitor();
                }

            if( Step )
                {
                clear_cells();
                eng=0;
                goto merge_lines;
                }
            else
                {
                 if( language==LANG_ENGLISH && Step )
                    { eng=0;  break;  }
                 pass3_empty(ln,lout);
                 goto cont;
                }
            }

// перепроверка пропорций (по условию)
        if (prop_in_trouble && let_to_dust)
            prop(1);

//проверка-поиск стандартной высоты, ширины
        checkpitch();
        clear_dust_alternates();
        if( !Step && !p2_active)
            {
            if(  line_scale )
                {
                lnsave = CSTR_NewLine(line_number, CSTR_LINVERS_SAVE, -1);
                if( !lnsave )
                    {
                    ErrorExit(RSTR_ERR_CSTR);
                    return ;
                    }
                Cells2CSTR(ln,lnsave,NULL,1);
                }
            else
                delete_far_dust(ln);
            CSTR_refresh_flags(ln);
            }

// проверка букв на наклонность, выпрямление с бритьем, перераспознвание
        if( erection_enable &&   erection_language(language))
            {
            erection_incline_words(0);
            if( erection_cond_language(language) )
                erection_delete();
            }

        snap_newpass('d');

        if (snap_activity('d'))
            {
            snap_show_text("Diffs beg");
            snap_monitor();
            }

// проверка - уточнение распознавания сходных букв. Это цикл дискриминаторов.
        criteries();

        if (snap_activity('d'))
            {
            snap_show_text("Diffs end");
            snap_monitor();
            }

// распознать звезду,процент. Распознаём отдельно, так как единообразно не удаётся
        if ( !( language == LANG_DIG || NO_Punct ) )
            {
            star();
            perc();
            }

// распознать 'Й'. Русская буква с шапкой распознаётся уникальным алгоритмом,
//		а не через accent(), как русская буква 'Ё'
        if( language == LANG_RUSSIAN && !langUkr && !langSer ) //&& !langBul) Almi&Oleg
            proc_ii();//paste '©'


        // чистим версии
        if ( language == LANG_DIG || NO_Punct )
            AKClearVers();

///////// Common code block
got_line:
        if(!p2_active && Step == 0)
            {
            set_cells_language(language);
            }
        else if(p2_active && Step == 0)
            {
            int32_t ret;
            Bool32 wasSnap=FALSE;
            if (snap_activity('m'))
                {
                char ttt[256]="Before p2_proc() ";
                p2_FillTxt (lout, ttt+strlen(ttt),TRUE);
                snap_show_text(ttt);
                wasSnap=TRUE;
                }
#ifdef _USE_PASS2_
            // make all postrecognition
			// распознавание на втором проходе
// Библиотека Pass2.dll c использованием Fon.dll, Leo.dll
// Символы, распознанные на первом проходе, проверяются при помощи кластерного
//  наложения. В случае отсутствия кластеров возможно
//			распознавания по ЛЕО. Если нет подтверждения - символы перераспознаются по кластерам.
//			Слова могут перерезаться при помощи кластерного наложения,
//			могут происходить склейки из пыли.
//  В случае двуязычия производится окончательный выбор одного из вариантов языка для каждого слова.

                {
                int32_t i;
                P2GLOBALS p2globals;
                extern Handle hSnapSmartCut;
                uchar    slang=language;
                // set bas lines - for cut/glue & after p2_...
                p2_setBasLines(ln);
                // fill values - language,...
                p2_GetP2Globals(&p2globals);
	            p2globals.enable_smart_cut = ( !hSnapSmartCut|| LDPUMA_SkipEx(hSnapSmartCut,TRUE,FALSE,1));
                CSTR_GetLineAttr(lout,&lattr);
                line_tabcell= lattr.tab_number;
                if( (i=p2_proc(ln,lout,&p2globals)) < 0 )
                    {
                    if (snap_activity('m'))
                        {
                        char ttt[256]="";
                        sprintf(ttt,"Error %d in p2_proc()",i);
                        snap_show_text(ttt);
                        snap_monitor();
                        return;
                        }
                    }
                language=slang;
                p2_SetP2Globals(&p2globals); // 17.02.00
                }

            if (wasSnap || snap_activity('m'))
                {
                char ttt[256]="After p2_proc() ";
                p2_FillTxt (lout, ttt+strlen(ttt),TRUE);
                p2_SetShowWords(TRUE);
                snap_show_text(ttt);
                p2_SetShowWords(FALSE);
                }
#endif
            // Oleg : 17-02-2000 : extract line attribute
            CSTR_GetLineAttr(lout,&lattr);
            line_tabcell= lattr.tab_number;

			// need FLAG !!!! Nick 06.04.2002
			if( lattr.Flags & CSTR_STR_SCALED )
				line_scale= lattr.scale;
			else
				line_scale = 0;

            if( lattr.Flags & CSTR_STR_Digital )
                line_alphabet=ALPHA_DIGITAL_TRUE;
            else if( lattr.Flags & CSTR_STR_DigitalFuzzy )
                line_alphabet=ALPHA_DIGITAL;
            if( lattr.Flags & CSTR_STR_ReadyBL )
                line_readyBL=TRUE;
            if( lattr.Flags & CSTR_STR_HandFragment )
                line_handfragment=TRUE;
            if( lattr.Flags & CSTR_STR_PointSuspension )
                line_pointsusp=TRUE;

            // переведем в cell
            clear_cells_pass2();
            ret = p2_Cstr2Cell( lout,NULL,NULL,TRUE, CSTR_f_space );
            // сколько получилось ?
            if(ret <= 0)
                {
                if( language==LANG_ENGLISH && Step )
                    { eng=0;  break;  }
                pass3_empty(ln,lout);
                goto end;
                }
//  Выбор строчных-прописных букв, распознанных ЛЕО (ЛЕО не делает различия между большими и маленькими).
//	Уточнение регистра всех букв с использованием БЛ и кластеров.
            { // p2_tools.c
            int p2_checkLeoCase(void);
            p2_checkLeoCase();
            }

            if (wasSnap || snap_activity('m'))
                {
                snap_monitor();
                }

                if(language == LANG_RUSSIAN && !langUkr && !langSer && !langBul)
                        proc_bI(1);//glue all 'л'
        if( p2_active && cell_f()->next==cell_l() )
            {
            if( language==LANG_ENGLISH && Step )
                { eng=0;  break;  }
            pass3_empty(ln,lout);
            goto end;
            }
        }
        if(!p2_active)
            {
            // Oleg : 11-01-1999 for trash bad lines
            B_LINES my_bases;
            get_b_lines(NULL,&my_bases);
            if( my_bases.b1>my_bases.b2 ||
                my_bases.b2>my_bases.b3 ||
                my_bases.b3>my_bases.b4  )
                {
                  if( language==LANG_ENGLISH && Step )
                    { eng=0;  break;  }
                  pass3_empty(ln,lout);
                  goto end;
                }

// разрезание-склейка тем или иным методом (зависит от языка и установок)
            if( !cuts_glues_methode )
                    {
                    make_all_cuts();
                    }
            else
                    {
                    if( language == LANG_RUSSIAN )
                            {
                            cuts_glues();   // new Paul cut & glue
                            number();
                            }
                    else
                            {
                            uchar sv = cuts_point_methode;

                            cuts_point_methode = 0 ;       // leman def cut points
                            make_all_cuts();               // leman cut process
                            cuts_point_methode = sv;

                            }
                    }
// распознавание Ы. Это, действительно уникальная буква, состоящая из двух компонент, стоящих рядом.
//			Аналога в латинице нет.
// распознавание особых украинских букв, а также сербских, болгарских и других, производимых из алфавита кириллицы
            if( language == LANG_RUSSIAN && !langUkr && !langSer && !langBul)
                    proc_bI(0);                        //paste cutted '|'
            if( language == LANG_RUSSIAN && langUkr )
                    proc_Ukr();                        //UKRAINIAN "iI & .."
            if( language == LANG_RUSSIAN && !langSer ) //&& !langBul)Almi&Oleg
                    proc_ii();                         //paste '©'

            if( !cuts_glues_methode ||
                    cuts_glues_methode && language != LANG_RUSSIAN )
                    make_all_glues();

            if(language == LANG_RUSSIAN && !langUkr && !langSer && !langBul)
                    proc_bI(1);                       //glue all 'л'
            }

// распознавание особых символов TM
        if (!pass2 || !Step)
            TM_control();
        if(p2_active)
            delete_spaces();

// проверка букв на наклонность, выпрямление с бритьем, перераспознвание после выпрямления
        if( !p2_active && erection_enable &&   erection_language(language))
            erection_incline_words(1);

        if( erection_cond_language(language) )
            erection_restore();

// проверка курсива, серифа
        font_let();

        if( erection_cond_language(language) )
            erection_delete();

        // чистим версии
        if ( language == LANG_DIG || NO_Punct)
            AKClearVers();

// проверка сходных английских версий (DO FP ...)
        if( !p2_active )
            differ();

// проверка il1. Явно устаревшая функция
        if( !p2_active )
            if( !(language==LANG_RUSSIAN && langSer) )
                il1();

// распознавание особых символов BULLET (прямоугольник, эллипс, треугольник)
        bullet();

        if (!delgarb())
            {
            if( Step )
                {
                clear_cells();
                eng=0;
                goto merge_lines;
                }
            else
                {
                if( language==LANG_ENGLISH && Step )
                    { eng=0;  break;  }
                if( !pass2 ) // Nick 03.05.2001 - только если один язык
				 {
                  pass3_empty(ln,lout);
                  goto end;
				 }
                }
            }

        snap_newpass('d');

        if (snap_activity('d'))
            {
            snap_show_text("Basefin begin");
            snap_monitor();
            }

        if (!delgarb())
            {
            if( Step )
                {
                clear_cells();
                eng=0;
                goto merge_lines;
                }
             else
                {
                if( language==LANG_ENGLISH && Step )
                    { eng=0;  break;  }
                if( !pass2 ) // Nick 03.05.2001 - только если один язык
				 {
                  pass3_empty(ln,lout);
                  goto end;
				 }
                }
            }

// финальное уточнение БЛ
        if( !Step || Step && !no_crit )
            if(!p2_active && !line_readyBL ) // ???
                basefin(!Step?ln:lne);  // Nick 12.02.001
			// was basefin(Step?ln:lne);  // Nick 12.02.001

        if( (language==LANG_RUSSIAN && langSer) )
            {
// распознавание особых сербских букв. Эвристики, корректирующие недостатки 3х5
            serbian_J2j();
            il1();
            }

//проверка-уточнение стандартной высоты, ширины
        checkpitch();
        h=(int16_t)get_size();
        if( !erection_cond_language(language) )
            erection_delete();

// распознавание-расстановка пунктуации
        if( !(language == LANG_DIG || NO_Punct ) )
            punct();

        clear_punct_twin_flags();

        if( !delgarb() )
            {
            if( Step )
                {
                clear_cells();
                eng=0;
                goto merge_lines;
                }
            else
                {
                if( language==LANG_ENGLISH && Step )
                    { eng=0;  break;  }
                if( !pass2 ) // Nick 03.05.2001 - только если один язык
				 {
                  pass3_empty(ln,lout);
                  goto end;
				 }
                }
            }

//проверка-уточнение стандартной высоты, ширины
        checkpitch();
        dump_to_string ();
        recover_dusts();
        hide_dusts(1);

// расстановка пробелов
        space_size(h);

        if(pass2 &&  !Step )
            save_sp = (uchar)get_space_size();

        if(pass2 &&  Step  )
            set_space_size(save_sp);
#ifndef DISABLE_SPACE
        space_cell();
#endif

// проверка - удаление пунктуации
        clean_punct();
        hide_dusts(0);

// проверка - замена << на шеврон
        shevron(cell_f()->nextl);
        clean_punct_temp();

// замена альтернатив по контексту
        context();
        if( p2_active )
            {
            set_spell_solid();
            }
        else
            {
// финальная перепроверка сходных букв, подчеркнутых символов (vy)
            final_crit();
            if( line_alphabet==ALPHA_DIGITAL  )
                {
                stop_pass2 = ( pass2 && !Step ) ;
                if( !digital_last_context() )
                    stop_pass2 = 0;
                }
             else
                stop_pass2=0;

// проверка по словарю, расстановка меток словарности
            set_cells_language(language);
            set_spell_solid();

// проверка русско-английских вариантов. Дискриминаторы двух языков
            if( Step )
                final_descriminate(cell_f()->next,cell_l());
            if( pass2 )
                {
                if( db_status && snap_activity('g') )
                    {
                    if( !Step )
                        {
                        snap_show_text("RUS LINE");
                        snap_monitor();
                        }
                    else
                        {
                        snap_show_text("ENG LINE");
                        snap_monitor();
                        }
                    }

                recover_dusts();
                clear_punct_twin_flags();

// проверка - может ли строка быть с английскими словами
                if( !Step )
                    {
                    eng=english_word_recognize();
                    if( !eng )  // step1 : select english word
                        {
                        hide_dusts(0);
                        break;
                        }
                     }

                hide_dusts(0);
                }
            }

merge_lines:;
    if( !p2_active )
        set_cells_language(language);
    if( spec_camera )
        kill_dusts_spaces();

// перевод в строку CSTR
    if( pass2 )
        {
        if( !Step )
            Cells2CSTR(ln,lout,NULL,1);
        else
            Cells2CSTR(lne,lneout,NULL,1);
        clear_cells();
        }
    if( stop_pass2 )
        break; // digital string can't be english
    } // end of Step (RUSSIAN & ENGLISH)

if( pass2 )
    {
// склейка русской и английской строк - удаление слабых вариантов,
// выбор основного варианта с возможным оставлением второго варианта для второго //прохода

#ifdef _USE_RSADD_
    if(  eng )
        {
        Bool32 res;
        CSTR_line  l[]={lout,lneout};
        if( db_status && snap_activity('g') )
            {
            snap_show_text("RUS & ENG LINES before english adding");
            snap_monitor_ori(l,2);
            }

        language = LANG_RUSSIAN;
        trees_load_fict();

        RSADD_SetRSTR( db_status,spec_camera,
                        (Handle)snap_monitor_ori,
                        (Handle)snap_activity,
                        (Handle)snap_show_text);
        res=RSADD_take(lout,lneout);

        p2_Cstr2Cell( lout,NULL,NULL,TRUE,CSTR_f_dust);
        if( res && db_status && snap_activity('g') )
            {
            snap_show_text("RUS-ENG LINE after english adding");
            snap_monitor();
            }
        CSTR_ClearLine(lout,(int16_t)(-16000+1),(int16_t)(0x7fff-1));
        }
    else
#endif
        {
        language = LANG_RUSSIAN;
        trees_load_fict();
        p2_Cstr2Cell( lout,NULL,NULL,TRUE,CSTR_f_dust);
        CSTR_ClearLine(lout,(int16_t)(-16000+1),(int16_t)(0x7fff-1));
        }
    CSTR_DeleteLine(lne);
    erection_restore();
    font_let();
    erection_delete();
    }

//распознавание особых символов - '®','©'
if( !( language == LANG_DIG || NO_Punct ) )
    trade_marks ();

if( pass2 )
    language = LANG_RUSSIAN;

// русско-английский контекст
if( pass2 && !p2_active )
    russian_english_context();

if(!p2_active)
    {
    if( pass2 )
        {
        if( !no_crit )
            save_rest_bases(1,0);
        save_rest_bases(2,0);
        }
    }
clean_line();
if( p2_active)
    {
    int16_t p2Ps= p2_GetPs();
    if(p2Ps )
        {
        if( line_readyBL && abs(p2Ps-(bbs3-bbs2))>1 ||
            abs(p2Ps-(bbs3-bbs2))>3 )
            bbs2=bbs3-p2Ps;
        }
    else if( line_tabcell &&
            (line_alphabet==ALPHA_DIGITAL_TRUE ||line_alphabet==ALPHA_DIGITAL) &&
             abs(Ps*3/2-(bbs3-bbs1))>2 && (p2Ps= p2_GetPs_up()) )
            bbs2=bbs3-p2Ps;
    }

//уточнение в расстановке пробелов
space_valid_words();

// установка характеристик шрифта - курсив,серифность,
//                                 подчеркнутый,узкий
font_str();
serif_let();
font_narrow();
font_ready=TRUE;
set_normal_unrecog(); // OLEG : all unrecog letters can be set to 176=°

// добавление версий для подчеркнутого
add_vers_underlined();
clear_twin_flags();
snap_newpass('g');

// перевод лигатур
if( p2_active || language!=LANG_RUSSIAN&&language!=LANG_ENGLISH )
    convert_ligas();

ready_result:;
// пишем в ту же строку - старое содержимое удалим
if( p2_active )  // p2_active ?
    {
    void p2_CellsToCSTR(CSTR_line lino);

    CSTR_rast curRast=CSTR_GetFirstRaster(lout),nextRast;
    CSTR_DeleteLoops(lout); // удаляем петли
    for(curRast=CSTR_GetNext(curRast);curRast;curRast=nextRast)
        {
        nextRast=CSTR_GetNext(curRast);
        CSTR_DelRaster(curRast);
        }
    if( language==LANG_RUSSIAN && multy_language )
        {
        russian_english_context();

        lneout = CSTR_GetLineHandle(line_number, CSTR_LINVERS_ENGOUT);
        if( lneout )
            CSTR_ClearLine(lneout,(int16_t)(-16000+1),(int16_t)(0x7fff-1));
        }
    p2_CellsToCSTR( lout );
    }
else
    {
    Cells2CSTR(ln,lout,NULL,1);

// Если шрифт - узкий,шкалированная строка, перераспознаем по ЛЕО
    if( line_scale && language==LANG_RUSSIAN &&
        cell_f()->nextl && (cell_f()->nextl->font&c_fp_narrow) )
        cstr_rerecog_leo(lout);

    if( language==LANG_RUSSIAN && multy_language )
        {
        lneout = CSTR_GetLineHandle(line_number, CSTR_LINVERS_ENGOUT);
        if( lneout )
            CSTR_PackLine(lneout);
        }
    }

// установка жирности, накопление статистики по жирности
{
 int p2_SetBoldLine(CSTR_line line);
 p2_SetBoldLine(lout);
}

if( line_scale0 && !p2_active )
    {
    if( cstr_scaled_in_res(lout) )
        {
        CSTR_ClearLine(ln,(int16_t)(-16000+1),(int16_t)(0x7fff-1));
        CSTR_CopyLine (ln,lnsave);
        }
    else
        {
        cstr_kill_scaled(ln);
        }
    }
end:
skew_end();

// проверка моноширинности
if( p2_active && Flag_Courier )
    courier_let(lout);
if( p2_active )
    {
    extern void p2_add_probs(CSTR_line ln);
    p2_add_probs(lout);
    }

if( db_status && snap_activity('g') || snap_is_marked(ln) )
    {
    CSTR_line  l[]={lout,ln};
    snap_show_text("end recog");
    snap_monitor_ori(&l[0],2-1*0);
    }
if( snap_active )
    {
    char buf[256];
    cell2txt(buf);
    snap_show_text(buf);
    }
cont:;

// удаление мусора из выходной строки
#ifdef _USE_RSADD_
if( !p2_active )
    RSADD_kill_dusts(lout);
if( eng )
    RSADD_kill_dusts(lneout);
#endif
if(flag_cut_point)
    {
    flag_cut_point=0;
    db_status=1;
    }

// перекодировка особых случаев
pass3_special_recode(lout);
// проверка многоточий - замена в них запятых на точки
if( !p2_active )
    pass3_table_points_suspension(lout);
else
    {
    CSTR_attr   attr;
    CSTR_GetLineAttr(lout,&attr);
    if( attr.Flags & CSTR_STR_PointSuspension )
        pass3_table_points_suspension2(lout);
    }
// чистим версии
if ( language == LANG_DIG || NO_Punct )
    AKClearVers();
clear_cells();
del_spaces_before_carry(lout);
font_ready=FALSE;
if( line_alphabet==ALPHA_DIGITAL_TRUE )
    {
    trees_load_fict();
    clear_digital_mode();
    enable_table_recog=FALSE;
    }
return;
}

uchar    convert_eng_liga( uchar c)
{
switch( c )
        {
		// Конфликтные коды 08.09.2000 E.P.
        case liga_i_usual:
			if (liga_i_usual == liga_i)
				c = 'i';
			break;

        case liga_i_latin:
			if (liga_i_latin == liga_i)
				c = 'i';
			break;

        case liga_j          : c = 'j';    break;

		// 31.08.2000 E.P.
        case liga_exm_usual  :
			if (liga_exm_usual == liga_exm)
				c = '!';
			break;

        case liga_exm_latin  :
			if (liga_exm_latin == liga_exm)
				c = '!';
			break;

        case liga_qm         : c = '?';    break;

        		// 31.08.2000 E.P.
        case liga_inv_exm_usual    :
			if (liga_inv_exm_usual == liga_inv_exm)
				c = '\xA2';
			break;

        case liga_inv_exm_latin    :
			if (liga_inv_exm_latin == liga_inv_exm)
				c = '\xA2';
			break;

        case liga_inv_qm     : c = '\xBF'; break;
        default:
            break;
        }
return c;
}

void convert_ligas(void)
{
cell *c, *ce;
int16_t i;

for(c=cell_f()->next,ce=cell_l();c!=ce;c=c->next )
    {
    if( c->nvers )
        for(i=0;i<c->nvers;i++)
            {
            c->vers[i].let = convert_eng_liga(c->vers[i].let);
            }
    }

return;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
void set_cells_language(uchar lang)
{ // Oleg : 06-08-95 09:48pm : set 2 language
cell *c;
if( lang==LANG_RUSSIAN )
    {
    if( langSer )
        lang=LANG_SERBIAN;
    if( langUkr )
        lang=LANG_UKRAINIAN;
	if( langBul )
        lang=LANG_BULGAR;
    }
for(c=cell_f()->next;c!=cell_l();c=c->next)
  c->language = lang;
return;
}

void save_rest_incline(int16_t mode)
{
static int16_t sIncline;
cell *c,*e;
if( !mode )
  sIncline = nIncline;  // save
else
  {
  nIncline = sIncline;  // restore
  for(c=cell_f()->next, e=cell_l();c!=e; c=c->next )
    ideal_rc(c);
  }
return;
}

void save_rest_bases(int16_t mode, int16_t line_crit)
{
static int16_t ominrow, omincol,
   obbs1,   obbs2,   obbs3,   obbs4,   obbsm,
   oNb1 ,   oNb2 ,   oNb3 ,   oNb4 ,   oNbt,
   oPs  ,   oPsf ,   omulti_bas;
if( mode==0 )
  {
  ominrow=minrow;
  omincol=mincol;
  obbs1=bbs1;
  obbs2=bbs2;
  obbs3=bbs3;
  obbs4=bbs4;
  obbsm=bbsm;
  oNb1 =Nb1;
  oNb2 =Nb2;
  oNb3 =Nb3;
  oNb4 =Nb4;
  oNbt =Nbt;
  oPs  =Ps;
  oPsf =Psf;
  omulti_bas=multi_bas;
  }
else if( mode==1 )
  {
  minrow=ominrow;
  bbs1=obbs1;
  bbs2=obbs2;
  bbs3=obbs3;
  bbs4=obbs4;
  bbsm=obbsm;
  Nb1 =oNb1;
  Nb2 =oNb2;
  Nb3 =oNb3;
  Nb4 =oNb4;
  Nbt =oNbt;
  Ps  =oPs;
  Psf =oPsf;
  multi_bas=(uchar)omulti_bas;
  if( line_crit )
    all_cell_levcut(1);    // apply penalties
  }
else if( mode==2 )
    {
    mincol=omincol;
    }
return;
}

static void pass_start()
 {
 cell *c;

 c=cell_f();
 c->prev=c->prevl=NULL;
 c->col=-16000;     // AL 940405  -1000 not enough for terrible skews
 c->w=0;
 c->vers[0].let=0;
 c->flg=c_f_fict;
 c->clink = -2;
 c->clu_attr = 0;
 c->nClust=0;
 c=cell_l();
 c->next=c->nextl=NULL;
 c->col=0x7fff;
 c->w=0;
 c->vers[0].let=0;
 c->flg=c_f_fict;
 c->clink = -2;
 c->clu_attr = 0;
 c->nClust=0;

 page_nIncline=nIncline;
 }

#define PROPMAX   25
void DeskewCell(cell *c, CCOM_comp *cmp, int16_t nIncline, int16_t shift)
{
c->row=cmp->upper-(int16_t)((int32_t)nIncline*cmp->left/2048);
c->col=cmp->left+(int16_t)((int32_t)nIncline*cmp->upper/2048);
if( shift )
    {
    c->row>>=shift;
    c->col>>=shift;
    }

return;
}

extern puchar kit_curr, kit_end;
static void accept_Cell( cell *c,CSTR_rast_attr *rst, CCOM_comp *cmp, Bool shift)
 {
 c_comp ec={0};
 uint16_t zero=0;

if( !cmp )
    return;

ec.size     = sizeof(c_comp)+cmp->size_linerep+2; // +2 - Nick
if( kit_curr+ec.size >kit_end )
    {
    ErrorExit(RSTR_ERR_NOPLACE);
    return;
    }
ec.upper    = cmp->upper>>shift;
ec.left     = cmp->left>>shift;
ec.h        = cmp->h>>shift;
ec.w        = cmp->w>>shift;
ec.rw       = ((ec.w+7)/8);
ec.type     = cmp->type;
ec.lines    = sizeof(c_comp);
ec.nl       = cmp->nl;
ec.begs     = cmp->begs;
ec.ends     = cmp->ends;
ec.large    = cmp->large;
ec.scale    = cmp->scale;

c->env      = (c_comp*)kit_curr;

memcpy(kit_curr, &ec, sizeof(c_comp));
kit_curr += sizeof(c_comp);
memcpy(kit_curr, cmp->linerep, cmp->size_linerep);
kit_curr += cmp->size_linerep;
memcpy(kit_curr, &zero, 2);
kit_curr += 2;

 c->h=cmp->h>>shift;
 c->w=cmp->w>>shift;
 c->r_row=cmp->upper>>shift;
 c->r_col=cmp->left>>shift;
 c->reasno=0;
 c->cpos=c->keg=c->font=0;
 c->bas_acc=0;
 DeskewCell(c,cmp,nIncline,shift);
 set_bad_cell(c);
 if (cmp->type == ch_great)
         c->env=0;

 return;
 }

static void compress_second(cell *c2,int16_t scale)
{
if( !c2 )
    return;
c2->w >>=scale;
c2->h >>=scale;
c2->col >>=scale;
c2->row >>=scale;
c2->r_col >>=scale;
c2->r_row >>=scale;
if( c2->env )
    {
    c2->env->left >>=scale;
    c2->env->upper >>=scale;
    c2->env->w >>=scale;
    c2->env->h >>=scale;
    c2->env->rw = (c2->env->w+7) >> 3;
    }
return;
}

void letters_ini(CSTR_line lin, Bool enable_scaling)
 {
 cell           *c1,*c2;
 CSTR_rast_attr  cur;
 CSTR_rast       curr, last, one;
 CSTR_attr       attr;
 CCOM_comp      *cmp;
 int16_t             i,j,hmax,nscale=0,nsmall=0,nall=0,ndust, nlet;
 RecVersions     evn,zer={0};
 uchar            sl=language, ssc=line_scale;


 c1=cell_f();
 CSTR_GetLineAttr(lin,&attr);
 if( attr.Flags & CSTR_STR_Digital )
    line_alphabet=ALPHA_DIGITAL_TRUE;
 if( attr.Flags & CSTR_STR_PointSuspension )
    line_pointsusp=TRUE;
 else if( attr.Flags & CSTR_STR_DigitalFuzzy )
    line_alphabet=ALPHA_DIGITAL;
 if( attr.Flags & CSTR_STR_ReadyBL )
    line_readyBL=TRUE;
 if( attr.Flags & CSTR_STR_HandFragment )
    line_handfragment=TRUE;
 if( attr.tab_number )
    line_tabcell= attr.tab_number;
 if( line_alphabet==ALPHA_DIGITAL_TRUE )
        {
        language = LANG_DIG;
        trees_load_fict();
        set_digital_mode();
        enable_table_recog=TRUE;
        }
 curr=CSTR_GetFirstRaster(lin);
 last=CSTR_GetLastRaster (lin);
 ndust=nlet=0;
 for (curr=CSTR_GetNextRaster (curr, CSTR_f_all); curr&&curr!=last; curr=CSTR_GetNextRaster (curr, CSTR_f_all))
  {
  CSTR_GetAttr(curr,&cur);
  if( cur.version != CSTR_VERSION_CODE )
    ErrorExit(RSTR_ERR_VERSION);
  cmp=CSTR_GetComp(curr);
  if( cur.flg&CSTR_f_dust )
        ndust++;
    else
        nlet++;
  if( cmp && cmp->scale )
    {
    if ( (cmp->type&(CCOM_CH_GREAT|CCOM_CH_DUST)) || cmp->scale>1)
        {
        if( cmp->h*10<cmp->w )
            {
            page_lines[num_of_lines].beg.y=cmp->upper+cmp->h/2;
            page_lines[num_of_lines].beg.x=cmp->left;
            page_lines[num_of_lines].end.y=cmp->upper+cmp->h/2;
            page_lines[num_of_lines].end.x=cmp->left+cmp->w;
            page_lines[num_of_lines].type=HOR_LN;
            page_lines[num_of_lines].width=(uchar)cmp->h;
            num_of_lines++;
            if (snap_activity('d'))
                        {
                char    buf[80];
                sprintf(buf,"Add line (%d,%d)-(%d,%d).w=%d",
                    cmp->upper+cmp->h/2,    cmp->left,
                    cmp->upper+cmp->h/2,    cmp->left+cmp->w,
                    cmp->h);
                        snap_show_text(buf);
                        snap_monitor();
                        }
            curr=CSTR_DelRaster(curr);
            curr=CSTR_GetPrevRaster(curr,255);
            continue;
            }
        if( (cmp->type&(CCOM_CH_GREAT|CCOM_CH_DUST)) || cmp->scale>2-1*0 )
            continue;
        }
    if ( cur.h>PROPMAX*cur.w)
        continue;
    if ( cmp->scale && ((cur.w>>cmp->scale) > RASTER_MAX_WIDTH ||  (cur.h>>cmp->scale) > RASTER_MAX_HEIGHT )     )
        continue;
/* еще не построены bsdust_upper и т.п. ! Nick 16.03.2001
    if( cur.flg==CSTR_f_dust )
        {
        int16_t c2bdiff=(char)(obtain_diff(cmp->left));
        if( cmp->upper+cmp->h < bsdust_upper + c2bdiff ||
           (cmp->upper+(cmp->h+1)/2>bsdust_lower+c2bdiff) &&
          !(cmp->h<bsdust_ps/2 && cmp->w>=bsdust_ps) ||
            cmp->h>(bsdust_lower-bsdust_upper)<<1 )
            continue;
        }
*/
    if( line_scale<cmp->scale )
        {
        line_scale=cmp->scale;
        hmax=cmp->h;
        nscale++;
        one=curr;
        }
    }
  }
 if( spec_camera && ndust>20 && ndust>nlet*2 )
    {
    curr=CSTR_GetFirstRaster(lin);
    last=CSTR_GetLastRaster (lin);
    for (curr=CSTR_GetNextRaster (curr, CSTR_f_all); curr&&curr!=last; )
      {
      CSTR_GetAttr(curr,&cur);
      cmp=CSTR_GetComp(curr);

      if( cur.flg&CSTR_f_dust )
        {
        curr=CSTR_DelRaster(curr);
        if( !curr )
            break;
        }
      else
        curr=CSTR_GetNextRaster (curr, CSTR_f_all);
      }
    }
 if( line_scale )
     {
     if( nscale==1 )
        {
        curr=CSTR_GetFirstRaster(lin);
        last=CSTR_GetLastRaster (lin);
        for (curr=CSTR_GetNextRaster (curr, CSTR_f_all); curr&&curr!=last; curr=CSTR_GetNextRaster (curr, CSTR_f_all))
          {
          CSTR_GetAttr(curr,&cur);
          cmp=CSTR_GetComp(curr);

          if( cmp && !cmp->scale && !(cur.flg&CSTR_f_dust) )
            {
            if( cmp->h*2<hmax )
                nsmall++;
            nall++;
            }
          }

        }
     if( nall>3 && nall==nsmall ||
        multy_language && language==LANG_ENGLISH && ssc==0 ||
        !enable_scaling )
        {
        line_scale=0;
        curr=CSTR_GetFirstRaster(lin);
        last=CSTR_GetLastRaster (lin);
        for (curr=CSTR_GetNextRaster (curr, CSTR_f_all); curr&&curr!=last; )
          {
          CSTR_GetAttr(curr,&cur);
          cmp=CSTR_GetComp(curr);

          if( cmp && cmp->scale )
            {
            curr=CSTR_DelRaster(curr);
            if( !curr )
                break;
            }
          else
            curr=CSTR_GetNextRaster (curr, CSTR_f_all);
          }
        }
     else
         {
         attr.Flags |= CSTR_STR_SCALED;
         attr.scale=line_scale;
         CSTR_SetLineAttr(lin,&attr);
         }
     }
 c1=cell_f();

 curr=CSTR_GetFirstRaster(lin);
 last=CSTR_GetLastRaster (lin);
 for (curr=CSTR_GetNextRaster (curr, CSTR_f_all); curr&&curr!=last; curr=CSTR_GetNextRaster (curr, CSTR_f_all))
  {

  CSTR_GetAttr(curr,&cur);
  cmp=CSTR_GetComp(curr);
  CSTR_GetCollection(curr,&evn);
  if( cur.flg&CSTR_f_dust  )
    continue;

  // CSTR_f_dust not set yet!!!  some very small - as dust
  // else in new_cell() could be error - if too many cells (>1650?) !
  // nick 18.05.2003
  if( cur.w < 5 && cur.h < 5 )
	  continue;

  if( !cmp )
    continue;

  if ( (cmp->type&CCOM_CH_GREAT) || cmp->scale>2-1*0)
       continue;
  if ( cur.h>PROPMAX*cur.w)
        continue;

  if (!cmp->scale && (cur.w > RASTER_MAX_WIDTH ||  cur.h > RASTER_MAX_HEIGHT )     )
        continue;
  if ( cmp->scale && ((cur.w>>cmp->scale) > RASTER_MAX_WIDTH ||
                      (cur.h>>cmp->scale) > RASTER_MAX_HEIGHT )     )
        continue;

  // Nick 03.04.2002 - test free space, чтобы не выходить по ErrorExit
  if( kit_curr+sizeof(c_comp)+cmp->size_linerep+2 >kit_end )
	  continue;

  c2=new_cell();
  c1->next=c1->nextl=c2;
  c2->stick_inc=NO_INCLINE;
  c2->save_stick_inc=NO_INCLINE;
  c2->prev=c2->prevl=c1;
  c2->font_new=0;
  c2->rus_eng_word = ruseng_no;
  c2->bdiff=0;
  c2->cg_flag=0;
  c2->dens=255;    // density not yet made by BOX
  c2->dlang_dup = cur.dlang_dup;
  c2->env=NULL;


  if(  line_scale )
  { //  scaling
  if( cmp->scale<line_scale )
    {
    if( cmp->h>(CCOM_DUST_SIZE<<cmp->scale) )
        {
        RecRaster       rs;
        CCOM_comp      *scale_comp;
        uchar          *lpool;
        int16_t           scale=line_scale-cmp->scale;

        if( !CCOM_GetScaleRaster(cmp, &rs,scale) )
            continue;

        lpool=EVNMakeLine( &rs , 2);
        if( !lpool )
            continue;

        scale_comp = CCOM_New(CSTR_GetContainer(lin) ,cmp->upper, cmp->left, cmp->w, cmp->h);
        CCOM_Store(scale_comp,0, (int16_t)((*((int16_t*)&lpool[0]))), lpool,  0, 0, 0, &zer,  NULL);
        if( scale_comp->w<(1<<scale) )
            scale_comp->w= 1<<scale;
        if( scale_comp->h<(1<<scale) )
            scale_comp->h= 1<<scale;

        accept_Cell(c2,&cur, scale_comp, scale);
        c2->env->scale=(uchar)line_scale;
        if( cmp->scale )
            compress_second(c2,1);
        }
    else
        { // convert small raster to dust type
        cur.flg=CSTR_f_dust;
        CSTR_SetAttr(curr,&cur);
        continue;
        }
    }
  else  // compressed by REXC. cmp->scale==line_scale
    accept_Cell(c2,&cur, cmp, line_scale);
  }
  else // no scaling. line_scale==0
    accept_Cell(c2,&cur, cmp, FALSE);

  if(c2->env)
          c2->env->scale=0;
  c2->flg= evn.lnAltCnt ? c_f_let : c_f_bad;
  i=0;
  if( evn.lnAltCnt )
        {
    if( evn.lnAltCnt && evn.Alt[0].Method==REC_METHOD_NCU)
        {
        uchar   *p,local_alphabet[256]={0};
        for(j=i=0;i<evn.lnAltCnt;i++)
            {

            for(p=stdLeoGetTypeface(evn.Alt[i].Code);*p;p++)
                {
                if( alphabet[*p] && !local_alphabet[*p] )
                    {
                    local_alphabet[*p]=1;
                    c2->vers[j].let =*p;
                    c2->vers[j].prob=MAX(evn.Alt[i].Prob&254,2);
                    j++;
                    if( j==VERS_IN_CELL-1 )
                        break;
                    }
                }
           if( j==VERS_IN_CELL-1 )
                        break;
            }
        c2->nvers=(int16_t)j;
        c2->recsource = c_rs_NCU;
        c2->history   = c_rs_NCU;
        c2->vers[j].let=0;
        }
    else
        {

        for(j=i=0;i<evn.lnAltCnt;i++)
            {
            if( alphabet[evn.Alt[i].Code] )
                {
                c2->vers[j].let =evn.Alt[i].Code;
                c2->vers[j].prob=(evn.Alt[i].Prob&1)?evn.Alt[i].Prob-1:evn.Alt[i].Prob;
                j++;
                }
            }
        c2->nvers=(int16_t)j;
        c2->recsource = c_rs_ev;
        c2->history   = c_rs_ev;
        c2->vers[j].let=0;
        if( j==0 )
            {
            short_recog_cell(c2);
            }
        }
    }
  else if( attr.Flags & (CSTR_STR_UPDOWN|CSTR_STR_DOWNUP|CSTR_STR_NEGATIVE) )
  {
	  short_recog_cell(c2);
  }
  else

    {
    c2->vers[0].let=0xB0;
    c2->vers[1].let=0;
    }

  v2_pidx_crit (c2);
  c1=c2;
  }
 c2=cell_l();
 c1->next=c1->nextl=c2;
 c2->prev=c2->prevl=c1;
 c2->dupstart=0/*(int32_t)CSTR_GetDup(curr)*/;
 c2->dupend  =0/*(int32_t)CSTR_GetDupEnd(curr)*/;

if( line_alphabet==ALPHA_DIGITAL_TRUE )
        language=sl;

return;
}

void dust_ini(CSTR_line lin)
 {
 cell            *c1,*c2;
 CSTR_rast_attr   cur;
 CSTR_attr        attr;
 CSTR_rast        curr, last;
 CCOM_comp       *cmp;
 RecVersions      zer={0};
 int16_t              cmpscale;

 CSTR_GetLineAttr(lin,&attr);
 for(c1=cell_f(),c2=cell_l();c1!=c2;c1=c1->next)
    {
    if( (c1->env && c1->env->large & ch_taken ) ||
		(c1->flg_new & c_fn_taken ) // Nick 13.02.2001
	  )
        c1=del_cell(c1);
    }

 if(db_special_project==SPEC_PRJ_GIP)   // OLEG : 02-05-18 : FOR GiP
 {
     int16_t   nSmall=0, nAll=0;
     curr=CSTR_GetFirstRaster(lin);
     for (curr=CSTR_GetNext(curr); curr; curr=CSTR_GetNext(curr))
     {
         CSTR_GetAttr(curr,&cur);
         if( cur.w<6 && cur.h<6 )
             nSmall++;
         nAll++;
     }
     if( nSmall>500 && (nAll-nSmall)*4<nAll )
     {
         curr=CSTR_GetFirstRaster(lin);
         for (curr=CSTR_GetNext(curr); curr; /*curr=CSTR_GetNext(curr)*/)
         {
             CSTR_GetAttr(curr,&cur);
             if( cur.w<6 && cur.h<6 )
             {
                 curr = CSTR_DelRaster(curr);

             }
             else
                 curr = CSTR_GetNext(curr);
         }
     }
 }

// dust cycle
 curr=CSTR_GetFirstRaster(lin);
 last=CSTR_GetLastRaster (lin);
 c1=cell_f();
 cmpscale = 0*(( attr.Flags & CSTR_STR_SCALED )!=0);
 for (curr=CSTR_GetNextRaster (curr, CSTR_f_all); curr&&curr!=last; curr=CSTR_GetNextRaster (curr, CSTR_f_all))
  {
  CSTR_GetAttr(curr,&cur);
  cmp=CSTR_GetComp(curr);

  if( !(cur.flg&CSTR_f_dust) )
        continue;
  if( cmp->large & ch_taken)
          continue;
  if( (cmp->type&CCOM_CH_GREAT) ||cmp->scale>2-1*0 )
      continue;
  if( (cur.w>>cmpscale) > RASTER_MAX_WIDTH ||  //???
      (cur.h>>cmpscale) > RASTER_MAX_HEIGHT )
      continue;
  if( cur.h>PROPMAX*cur.w)
          continue;

  // Nick 03.04.2002 - test free space, чтобы не выходить по ErrorExit
  if( kit_curr+sizeof(c_comp)+cmp->size_linerep+2 >kit_end )
	  continue;

  c2=new_cell();
  c2->stick_inc=NO_INCLINE;
  c2->save_stick_inc=NO_INCLINE;
  c2->font_new=0;
  c2->dlang_dup = cur.dlang_dup;
  c2->rus_eng_word = ruseng_no;
  c2->broken_II = 0;
  c2->language = 0;
  c2->pos_inc = 0;
  c2->flg=c_f_dust;
  c2->bdiff=0;
  c2->cg_flag=c2->basflg=c2->bas1=c2->bas2=c2->bas3=c2->bas3=c2->bdiff=0;
  c2->dens=255;
  if( ( attr.Flags & CSTR_STR_SCALED ) && cmp->w>1 && cmp->h>1 )
    {
    RecRaster       rs;
    CCOM_comp      *scale_comp;
    uchar          *lpool;
    int16_t           scale=line_scale-cmp->scale;

    if( !CCOM_GetScaleRaster(cmp, &rs,scale) )
        continue;

    lpool=EVNMakeLine( &rs , 2);
    if( !lpool )
        continue;

    scale_comp = CCOM_New(CSTR_GetContainer(lin) ,cmp->upper, cmp->left, cmp->w, cmp->h);
    CCOM_Store(scale_comp,0, (int16_t)((*((int16_t*)&lpool[0]))), lpool,  0, 0, 0, &zer,  NULL);
    if( scale_comp->w<(1<<scale) )
        scale_comp->w= 1<<scale;
    if( scale_comp->h<(1<<scale) )
        scale_comp->h= 1<<scale;
    accept_Cell(c2,&cur, scale_comp, scale); // compressed this
    c2->env->scale=(uchar)line_scale;
    if( cmp->scale )
            compress_second(c2,1);
    }
  else
    accept_Cell(c2,&cur, cmp,0);
  c2->env->scale=0;
  c2->env->large |= ch_taken;
  c2->reasno=0;
  c2->bdiff=(char)(obtain_diff(c2->col));
  c2->dupstart=(int32_t)CSTR_GetDup(curr);
  c2->dupend  =(int32_t)CSTR_GetDupEnd(curr);

  c2->flg_new |= c_fn_taken; // Nick 13.02.2001

  if (c2->row+c2->h < bsdust_upper + c2->bdiff ||
      (c2->row+(c2->h+1)/2>bsdust_lower+c2->bdiff) &&
             !(c2->h<bsdust_ps/2 && c2->w>=bsdust_ps) ||
      c2->h>(bsdust_lower-bsdust_upper)<<1 || (c2->env && c2->env->scale>line_scale ))
    {
    free_cell(c2);
    curr=CSTR_DelRaster(curr);
    curr=CSTR_GetPrevRaster(curr,255);
    continue;
    }
  insert_cell(c2,c1);
  c1=c2;
  }

  c1=cell_f();
  while ((c1=c1->nextl)->nextl != NULL)
	{
    ij_test(c1);  // do delete/confirm i/j
	}
return;
}

void delete_far_dust(CSTR_line lin)
{
cell            *c2;
CSTR_rast_attr   attr;
CSTR_rast        curr;
for(curr=CSTR_GetNext( CSTR_GetFirstRaster(lin) );
                curr ; curr=CSTR_GetNext(curr) )
                {
                CSTR_GetAttr(curr,&attr);
                attr.language = language;
                CSTR_SetAttr(curr,&attr);
                }
for(c2=cell_f()->next;c2!=cell_l();c2=c2->next)
    {
    if( c2->flg&c_f_dust )
        {
        if(  c2->row+c2->h < bsdust_upper + c2->bdiff ||
            (c2->row+(c2->h+1)/2>bsdust_lower+c2->bdiff) &&
           !(c2->h<bsdust_ps/2 && c2->w>=bsdust_ps) ||
             c2->h>(bsdust_lower-bsdust_upper)<<1 || (c2->env && c2->env->scale>line_scale ))
            {
            for(curr=CSTR_GetNext( CSTR_GetFirstRaster(lin) );
                curr ; curr=CSTR_GetNext(curr) )
                {
                CSTR_GetAttr(curr,&attr);
                if( attr.r_col==c2->r_col && attr.r_row==c2->r_row &&
                    attr.w==c2->w && attr.h==c2->h )
                    {
                    CSTR_DelRaster(curr);
                    break;
                    }
                }

            c2=del_cell(c2);
            }
        }
    }
}

static void postrecog()
 {
 version vers[VERS_IN_CELL];
 cell *c;
 int16_t i,j;

 for (c=cell_f()->next; c->next!=NULL; c=c->next)
  {
  memcpy(vers,c->vers,VERS_IN_CELL*sizeof(version));
  short_recog_cell(c);
  for (i=0; vers[i].let; i++)
   {
   for (j=0; j<c->nvers; j++)
    if (vers[i].let==c->vers[j].let)
     {
     c->vers[j].prob=MAX(vers[i].prob,c->vers[j].prob);
     break;
     }
   if (j==c->nvers && c->nvers<VERS_IN_CELL-1)
    {
    c->vers[j].let=vers[i].let;
    c->vers[j].prob=vers[i].prob;
    c->nvers++;
    }
   }
  sort_vers(c);
  }
 }

void   delete_spaces(void)
{ // Valdemar makes pass2
cell * c;
 for(c=cell_f()->next; c->next != NULL;c=c->next)
  if(c->flg & c_f_space) c=del_cell(c);
}


void set_normal_unrecog(void) // OLEG : all unrecog letters can be set to 176=°
{
cell *c, *e;
for(c=cell_f()->next, e=cell_l(); c!=e; c=c->next )
  {
  if( (c->flg&c_f_bad) && c->nvers<1 )
    {
    c->vers[0].let  = 176;
    c->vers[0].prob = 0;
    c->vers[1].let  = 0;
    c->vers[1].prob = 0;
    }
  if( c->nvers>0 )
    {
    int16_t i;
    for(i=0;i<c->nvers;i++)
      if( c->vers[i].let==0 || c->vers[i].prob==0)  break;
    if( i!=c->nvers )
      {
      if( i>0 )
        {
        c->nvers=i;
        c->vers[i].let=c->vers[i].prob=0;
        }
      else
        {      // i==0
        c->vers[0].let  = 176;
        c->vers[0].prob = 0;
        c->vers[1].let  = 0;
        c->vers[1].prob = 0;
        }
      }
    }
  }
return;
}

Bool test_short_english_blines(void)
{
int16_t bad;
bad = (Nb1==-1)+(Nb2==-1)+(Nb3==-1)+(Nb4==-1);
return Nb2==-1 || Nb3==-1 || bad==2 ;
}


void CopyAttr2CSTR(CSTR_rast_attr *attr, cell *c)
{
        attr->row   = c->row     ;
        attr->col   = c->col     ;
        attr->w     = c->w       ;
        attr->h     = c->h       ;
        attr->r_row = c->r_row   ;
        attr->r_col = c->r_col   ;
        attr->bdiff = c->bdiff   ;
        attr->difflg= c->difflg  ;
        attr->basflg= c->basflg  ;
        attr->bas1  = c->bas1    ;
        attr->bas2  = c->bas2    ;
        attr->bas3  = c->bas3    ;
        attr->bas4  = c->bas4    ;
        attr->cg_flag=c->cg_flag ;
        attr->cpos  = c->cpos    ;
        attr->bas_acc   = c->bas_acc;
        attr->dlang_dup = c->dlang_dup;
        attr->flg_new   = c->flg_new;
        attr->flg   = 0;
        if( c->flg &    c_f_let)
            attr->flg   |= CSTR_f_let;
        if( c->flg &    c_f_bad)
            attr->flg   |= CSTR_f_bad;
        if( c->flg &    c_f_dust)
            attr->flg   |= CSTR_f_dust;
        if( c->flg &    c_f_punct)
            attr->flg   |= CSTR_f_punct;
        if( c->flg &    c_f_space)
            attr->flg   |= CSTR_f_space;
        if( c->flg &    c_f_solid)
            attr->flg   |= CSTR_f_solid;
        if( c->flg &    c_f_confirmed)
            attr->flg   |= CSTR_f_confirmed;
        if( c->flg &    c_f_fict)
            attr->flg   |= CSTR_f_fict;

        attr->reasno = c->reasno;

        if( font_ready )
            {
            attr->keg  = c->keg;
            attr->font = c->font ;
            attr->font_new = c->font_new ;
            }

        else
            {
            attr->keg  = 0;
            attr->font = 0;
            attr->font_new = 0;
            }

        attr->dens = c->dens ;
        attr->recsource = c->recsource ;
        attr->stick_inc = c->stick_inc ;
        attr->bas_acc = c->bas_acc ;
        attr->left = c->left ;
        attr->right = c->right ;
        attr->shape = c->shape ;
        attr->rus_eng_word = c->rus_eng_word ;
        attr->broken_II = c->broken_II ;
        attr->language = c->language ;
        attr->pos_inc  = c->pos_inc;
        attr->save_stick_inc  = c->save_stick_inc;
        attr->n_baton=c->n_baton;
        if( c->n_baton!=NO_BATONS )
            memcpy(attr->save_baton,c->save_baton,c->n_baton*sizeof(STICK));

        attr->RecogHistory = CSTR_hi_none  ;
        if( c->history & c_rs_ev)
            attr->RecogHistory |= CSTR_hi_evn   ;
        if( c->history & c_rs_BOX)
            attr->RecogHistory |= CSTR_hi_3x5   ;
        if( c->history & c_rs_BOXs )
            attr->RecogHistory |= CSTR_hi_3x5s  ;
        if( c->history & c_rs_deskr )
            attr->RecogHistory |= CSTR_hi_desk  ;
        if( c->history & c_rs_bitcmp )
            attr->RecogHistory |= CSTR_hi_fon   ;
        if( c->history & c_rs_LEO )
            attr->RecogHistory |= CSTR_hi_leo   ;
        attr->clink    = c->clink;
        attr->clu_attr = c->clu_attr;
        attr->nClust   = c->nClust;

		// Nick 20.02.2001
		attr->cg_flag_fine = c->cg_flag_fine;

		// Nick 11.03.2002
		attr->accent_leader = c->accent_leader;
		attr->bottom_accent = c->bottom_accent;
  return;
}


int CopyVers2CSTR(RecVersions *ver, cell *c)
{
int16_t i,j,let, ret=0;
memset(ver,0,sizeof(RecVersions));
ver->lnAltCnt = c->nvers;
ver->lnAltMax = REC_MAX_VERS;

if( ver->lnAltCnt )
  {
  for(j=i=0;i<ver->lnAltCnt;i++)
      {
      let = (int16_t)c->vers[i].let;
      if( decode_ASCII_to_[let][1]==0 )
          {
          ver->Alt[j].Code = decode_ASCII_to_[let][0];
              ver->Alt[j].CodeExt = 0;
              ver->Alt[j].Prob = c->vers[i].prob;
          switch( c->recsource&0xef )
            {
            case c_rs_ev:
                ver->Alt[i].Method    = REC_METHOD_EVN;
                break;
            case c_rs_NCU:
                ver->Alt[i].Method    = REC_METHOD_NCU;
                break;
            case c_rs_BOX:
            case c_rs_BOXs:
                ver->Alt[i].Method    = REC_METHOD_3X5;
                break;
            case c_rs_bitcmp:
                ver->Alt[i].Method    = REC_METHOD_FON;
                break;
            case c_rs_LEO:
                ver->Alt[i].Method    = REC_METHOD_LEO;
                break;
            default:
                ver->Alt[i].Method    = 0;
                break;
            }
          j++;
          }
      else if( i==0 )
          {
          ver->Alt[j].Code = (uchar)let;
              ver->Alt[j].CodeExt = 0;
              ver->Alt[j].Prob = c->vers[i].prob;
          j++;
          ret=1;
          }
      }
  }
else
  {
  ver->Alt[0].Code = '~';
  }
if( !(c->flg & (c_f_let|c_f_bad)) )
    ver->Alt[0].Info = 0;
return ret;
}

int cell2UniVers(UniVersions *ver, cell *c)
{
int16_t i,let, ret=0;
int16_t lang=c->language;

if( lang==LANG_ENGLISH && multy_language )
    lang    = LANG_RUSENG;
memset(ver,0,sizeof(RecVersions));
ver->lnAltCnt = MIN(c->nvers, REC_MAX_VERS);
ver->lnAltCnt = MAX(ver->lnAltCnt, 0);
ver->lnAltMax = REC_MAX_VERS;

/*  comment Nick 17.02.2001
if( c->nvers>1 && c->vers[0].prob==c->vers[1].prob && c->vers[0].prob>2 )
  {
  c->vers[1].prob-=2;
  }
*/

if( ver->lnAltCnt )
  {
  for(i=0;i<ver->lnAltCnt;i++)
      {
      let = (int16_t)c->vers[i].let;
      strcpy(ver->Alt[i].Code , decode_ASCII_to_[let]);
      ver->Alt[i].Liga      = (uchar)c->vers[i].let;
          ver->Alt[i].Prob      = c->vers[i].prob;
      switch( c->recsource&0xef )
            {
            case c_rs_ev:
                ver->Alt[i].Method    = REC_METHOD_EVN;
                break;
            case c_rs_NCU:
                ver->Alt[i].Method    = REC_METHOD_NCU;
                break;
            case c_rs_BOX:
            case c_rs_BOXs:
                ver->Alt[i].Method    = REC_METHOD_3X5;
                break;
            case c_rs_bitcmp:
                ver->Alt[i].Method    = REC_METHOD_FON;
                break;
            case c_rs_LEO:
                ver->Alt[i].Method    = REC_METHOD_LEO;
                break;
            default:
                ver->Alt[i].Method    = 0;
                break;
            }
      ver->Alt[i].Charset   = CodePages[lang];
      }
  }
else
  {
  ver->Alt[0].Liga ='~';
  strcpy( ver->Alt[0].Code ,"~");
  }
if( !(c->flg & (c_f_let|c_f_bad)) )
    ver->Alt[0].Info = 0;
return ret;
}


void CSTR_kill_dusts( CSTR_line lino )
{
  cell *c;
  CSTR_rast_attr  attr;
  CSTR_rast       rst;
  RecVersions     ver={0};
ver.Alt[0].Code='~';
for (c = cell_f()->next; c != cell_l(); c = c->next)
  if(c->flg & c_f_dust)
          {
    if( (rst=CSTR_NewRaster (lino ,c->col, c->row,c->w))!=0 )
        {
        CopyAttr2CSTR(&attr, c);
        CSTR_SetDup(rst,(CSTR_rast)c->dupstart);
        CSTR_SetDupEnd(rst,(CSTR_rast)c->dupend);
        CSTR_SetAttr (rst, &attr);

        CSTR_StoreCollection (rst, &ver);

        if( c->env /*&& c->env->scale<2*/ )
          CSTR_StoreComp (rst, (uchar*)((uchar*)c->env+c->env->lines),1,0);
        }
                  c=del_cell(c);
            } //loop for dusts
return;
}


void Cells2CSTR(CSTR_line lin,CSTR_line lino,cell *cur,Bool32 enable_scaled)
{
cell*    c, cc;
CSTR_rast_attr  attr,zattr={0};
CSTR_attr               attrlin={0};
CSTR_rast       rst, old_rst=CSTR_GetFirstRaster(lino);
UniVersions     uvs;
int16_t            macol,  micol,  marow,  mirow;
int16_t           rmacol, rmicol, rmarow, rmirow;
int32_t           need_scale=0;
Bool            sfr;
if( lin )
    CSTR_GetLineAttr(lin, &attrlin);
if( line_pointsusp )
    attrlin.Flags |= CSTR_STR_PointSuspension;
if( line_minus )
    {
    attrlin.Flags |= CSTR_STR_PlusMinus+CSTR_STR_ReadyResults;
    sfr = font_ready;
    font_ready=TRUE;
    }
switch( line_alphabet )
    {
    case    ALPHA_DIGITAL:
        attrlin.Flags |= CSTR_STR_DigitalFuzzy;
        break;
    case    ALPHA_DIGITAL_TRUE:
        attrlin.Flags |= CSTR_STR_Digital;
        break;
    default:
        break;
    }
if( line_readyBL )
    attrlin.Flags |= CSTR_STR_ReadyBL;
if( line_tabcell )
    attrlin.tab_number=line_tabcell;
//if( line_tabcell )
//    attrlin.tab_column=line_tabcell;
if( line_handfragment )
    attrlin.Flags |= CSTR_STR_HandFragment;
marow=macol=-16000;
mirow=micol=0x7fff;
rmarow=rmacol=-16000;
rmirow=rmicol=0x7fff;
for (c = cell_f()->next; c != cell_l(); c = c -> next)
   {
   cc = *c;
   attr = zattr;
   rst = 0;          //Paul 12-11-99
   if( c->flg&c_f_space  )
    {
    if( (rst=CSTR_InsertRaster(old_rst))!=0 )
        {
        CopyAttr2CSTR(&attr, c);
        CSTR_SetDup(rst,(CSTR_rast)c->dupstart);
        CSTR_SetDupEnd(rst,(CSTR_rast)c->dupend);
        CSTR_SetAttr (rst, &attr);

        uvs.lnAltCnt=1;
        switch( c->vers[0].let )
            {
            case    SS_NEG_HALF_SPACE:
                uvs.Alt[0].Code[0]=0;
                uvs.Alt[0].Liga   =SS_NEG_HALF_SPACE;
                uvs.Alt[0].Prob   =75;
                break;
            case    SS_POS_HALF_SPACE:
                uvs.Alt[0].Code[0]=' ';
                uvs.Alt[0].Code[1]=0;
                uvs.Alt[0].Liga   =SS_POS_HALF_SPACE;
                uvs.Alt[0].Prob   =150;
                break;
            case    SS_SPACE         :
                uvs.Alt[0].Code[0]=' ';
                uvs.Alt[0].Code[1]=0;
                uvs.Alt[0].Liga   =SS_SPACE;
                uvs.Alt[0].Prob   =254;
                break;
            }
        CSTR_StoreCollectionUni (rst, &uvs);
        old_rst = rst;
        }
    continue;
    }
    if( (attrlin.Flags & CSTR_STR_SCALED) || line_scale && enable_scaled )
        {
        if( cc.env )
            cc.env->scale=line_scale;
        cc.col <<= line_scale;
        cc.row <<= line_scale;
        cc.r_col <<=line_scale;
        cc.r_row <<=line_scale;
        cc.w <<= line_scale;
        cc.h <<= line_scale;
        need_scale=1;
        }
//   if( (rst=CSTR_NewRaster (lino ,cc.col, cc.row,cc.w))!=0 )
   if( (rst=CSTR_InsertRaster(old_rst))!=0 )
        {
        CopyAttr2CSTR(&attr,&cc);
        CSTR_SetDup(rst,(CSTR_rast)c->dupstart);
        CSTR_SetDupEnd(rst,(CSTR_rast)c->dupend);
        cell2UniVers(&uvs, c);
        CSTR_StoreCollectionUni (rst, &uvs);
        CSTR_SetAttr (rst, &attr);

        if( c->env /*&& c->env->scale<2*/ )
          CSTR_StoreComp (rst, (uchar*)((uchar*)c->env+c->env->lines),1,c->env->scale);
        old_rst = rst;
        if( cc.col+cc.w>macol )
            macol=cc.col+cc.w;
        if( cc.col<micol )
            micol=cc.col;
        if( cc.row+cc.h>marow )
            marow=cc.row+cc.h;
        if( cc.row<mirow )
            mirow=cc.row;
        if( cc.r_col+cc.w>rmacol )
            rmacol=cc.r_col+cc.w;
        if( cc.r_col<rmicol )
            rmicol=cc.r_col;
        if( cc.r_row+cc.h>rmarow )
            rmarow=cc.r_row+cc.h;
        if( cc.r_row<rmirow )
            rmirow=cc.r_row;
        }
   //Paul 12-11-99 for snap
    if (rst && c==cur)
    {
      attr.flg_new |= CSTR_fn_stop;
      CSTR_SetAttr (rst, &attr);
    }
   //
   }
if( line_scale && enable_scaled )
      attrlin.Flags|=CSTR_STR_SCALED;
if( micol==32767 )
        micol=mincol;
if( mirow==32767 )
        mirow=minrow;
if( macol==-16000 )
        macol=maxcol;
if( marow==-16000 )
        marow=maxrow;
attrlin.col=micol;//<<need_scale;
attrlin.row=mirow;//<<need_scale;
attrlin.wid=macol-micol;
attrlin.hei=marow-mirow;//(marow-mirow)<<need_scale;

attrlin.r_col=rmicol;
attrlin.r_row=rmirow;
attrlin.r_wid=rmacol-rmicol;
attrlin.r_hei=rmarow-rmirow;

attrlin.l_col=mincol;
attrlin.l_row=minrow;
attrlin.l_wid=macol-micol;
attrlin.l_hei=maxrow-minrow;//(maxrow-minrow)<<need_scale;

attrlin.bs1=bbs1;
attrlin.bs2=bbs2;
attrlin.bs3=bbs3;
attrlin.bs4=bbs4;
attrlin.Nb1=Nb1;
attrlin.Nb2=Nb2;
attrlin.Nb3=Nb3;
attrlin.Nb4=Nb4;
attrlin.Nbt=Nbt;
attrlin.bsm=bbsm;
attrlin.Ps =Ps;
attrlin.Psf=Psf;
attrlin.incline             =nIncline;
attrlin.language                =language;
attrlin.erection                =erection_inc;
if( language==LANG_RUSSIAN && multy_language )
    attrlin.language            =LANG_RUSENG;
if( language==LANG_RUSSIAN && langUkr )
    attrlin.language            =LANG_UKRAINIAN;
if( language==LANG_RUSSIAN && langSer )
    attrlin.language            =LANG_SERBIAN;
if( language==LANG_RUSSIAN && langBul )
    attrlin.language            =LANG_BULGAR;
strcpy((char*)attrlin.VersionName,"RecogVersions");
CSTR_SetLineAttr(lino, &attrlin);
if( lin )
    CSTR_SetLineAttr(lin,  &attrlin);
if( line_minus )
     font_ready = sfr;

return;
}

void accept_cell(cell *c,c_comp *cmp)
 {
 extern int16_t nIncline;

 c->env=cmp;
 c->h=cmp->h;
 c->w=cmp->w;
 c->r_row=cmp->upper;
 c->r_col=cmp->left;
 c->reasno=cmp->reasno;
 c->cpos=c->keg=c->font=0;
 c->row=cmp->upper-(int16_t)((int32_t)nIncline*cmp->left/2048);
 c->col=cmp->left+(int16_t)((int32_t)nIncline*cmp->upper/2048);
 if ((c->nvers=cmp->nvers)>0)
  {
  memcpy(c->vers,(pchar)cmp+cmp->records,c->nvers*sizeof(version));
  c->vers[c->nvers].let=0;
  c->recsource = c_rs_ev;   // events done
  c->history   = c_rs_ev;
  }
 else set_bad_cell(c);
 if (cmp->type == ch_great) c->env=0;
 }

void clear_cells(void)
{
cell *c=cell_f()->next, *e=cell_l();
for(;c!=e;c=c->next)
    {
    c = del_cell(c);
    }
if( cell_f()->next!=cell_l() )
    cell_f()->next =cell_l();
if( cell_l()->prev!=cell_f() )
    cell_l()->prev =cell_f();
if( cell_f()->nextl!=cell_l() )
    cell_f()->nextl=cell_l();
if( cell_l()->prevl!=cell_f() )
    cell_l()->prevl=cell_f();
return;
}

void clear_cells_pass2(void)
{
if( cell_f()->next!=cell_l() )
    cell_f()->next =cell_l();
if( cell_l()->prev!=cell_f() )
    cell_l()->prev =cell_f();
if( cell_f()->nextl!=cell_l() )
    cell_f()->nextl=cell_l();
if( cell_l()->prevl!=cell_f() )
    cell_l()->prevl=cell_f();
return;
}

void clear_dust_alternates(void)
{
cell *c;
for(c=cell_f();c!=cell_l();c=c->next)
    {
    if( c->flg&c_f_dust )
        {
        c->nvers=0;
        c->vers[0].let=0xB0;
        }
    }
return;
}

void del_word_for2lang(int16_t left_limit,int16_t right_limit)
{
CSTR_ClearLine(lne,(int16_t)(left_limit<<line_scale),(int16_t)(right_limit<<line_scale));
return;
}


void CSTR_refresh_flags(CSTR_line ln)
{
cell           *c=cell_f()->next,*cc;
CSTR_rast       r=CSTR_GetNext(CSTR_GetFirstRaster(ln));
CSTR_rast_attr  a;
CCOM_comp      *com;
int16_t             m;

for(; c!=cell_l() && r ; r=CSTR_GetNext(r),c=c->next)
    {
    CSTR_GetAttr(r,&a);
    com = CSTR_GetComp(r);

    if( com->scale>1 || a.w > RASTER_MAX_WIDTH || a.h > RASTER_MAX_HEIGHT ||
        a.h>PROPMAX*a.w)
            continue;
    m = 1<<com->scale ;
    if( c->r_col*m==a.r_col && c->r_row*m==a.r_row &&
        c->w*m==a.w && c->h*m==a.h )
        cc=c; // normal order of cells
    else
        { // find by coordinate
        for( cc=cell_f()->next;cc!=cell_l(); cc=cc->next )
            {
            if( cc->r_col*m==a.r_col && cc->r_row*m==a.r_row &&
                cc->w*m==a.w && cc->h*m==a.h )
                break;
            }
        if( cc=cell_l() )
            continue; // not find
        c=cc;
        }
    if( a.flg!=cc->flg )
        {
        a.flg=cc->flg;
        CSTR_SetAttr(r,&a);
        }
    }

return;
}

Bool del_spaces_before_carry(CSTR_line ln)
{
CSTR_rast       r=CSTR_GetPrev(CSTR_GetLastRaster(ln)),l,f;
CSTR_rast_attr  a;
UniVersions     u;
uchar           carry[]="-\x5F";

CSTR_GetCollectionUni(r,&u);
if(!u.lnAltCnt || !strchr(carry,u.Alt[0].Liga)    )
    return FALSE;
do{
    r=CSTR_GetPrev(r);
    if( !r )
        break;
    CSTR_GetCollectionUni(r,&u);
    } while( u.lnAltCnt && strchr(carry,u.Alt[0].Liga)    );

if( r )
    l = CSTR_GetNext(r);
else
    return FALSE;
CSTR_GetAttr(r,&a);
while( a.flg&CSTR_f_space )
    {
    r=CSTR_GetPrev(r);
    if( !r )
        break;
    CSTR_GetAttr(r,&a);
    }

if( r )
    f=CSTR_GetNext(r);
else
    return FALSE;
for(;f!=l;)
    {
    f=CSTR_DelRaster(f);
    }
return TRUE;
}

Bool32  cstr_scaled_in_res(CSTR_line ln)
{
CSTR_rast       r=CSTR_GetNext(CSTR_GetFirstRaster(ln));
CCOM_comp   *   cmp;
return TRUE;
for(; r ; r=CSTR_GetNext(r))
    {
    cmp=CSTR_GetComp(r);
    if( cmp && cmp->scale &&
        (cmp->w>RASTER_MAX_WIDTH || cmp->h>RASTER_MAX_HEIGHT) )
        return TRUE;
    }
return FALSE;
}

Bool32  cstr_kill_scaled(CSTR_line ln)
{
CSTR_rast       r=CSTR_GetNext(CSTR_GetFirstRaster(ln)), p;
CCOM_comp   *   cmp;

for(; r ; r=CSTR_GetNext(r))
    {
    cmp=CSTR_GetComp(r);
    if( cmp && cmp->scale )
        {
        p=CSTR_GetPrevRaster(r,255);
        if( !r )
            return FALSE;
        CSTR_DelRaster(r);
        r=p;
        }
    }
return TRUE;
}

void courier_let(CSTR_line ln)
{
CSTR_rast       r=CSTR_GetNext(CSTR_GetFirstRaster(ln));
CSTR_rast_attr  a;

for(; r ; r=CSTR_GetNext(r))
    {
    CSTR_GetAttr(r,&a);
    a.font_spec = CSTR_fs_courier;
    CSTR_SetAttr(r,&a);
    }
return;
}

void import_lines_features(void)
{
cell *c,*b=cell_f()->next, *e=cell_l();
int32_t i,dy,dx;
int16_t up, uploc;

for(i=0;i<num_of_lines;i++)
    {
    up=MIN(page_lines[i].beg.y, page_lines[i].end.y) - page_lines[i].width/2;
    if( abs(up-maxrow)<10 )
        {
        for(c=b;c!=e;c=c->next)
            {
            dx=page_lines[i].end.x-page_lines[i].beg.x;
            dy=page_lines[i].end.y-page_lines[i].beg.y;
            if( c->r_col>=page_lines[i].beg.x &&
                c->r_col+c->w<=page_lines[i].end.x )
                {
                uploc = ((c->r_col-page_lines[i].beg.x)*dy)/dx+
                        page_lines[i].beg.y;
                if( abs(uploc-c->r_row-c->h)<5 )
                    c->flg_new |= c_fn_under;
                }
            }
        }
    }
return;
}

Bool pass3BL(CSTR_line ln)
{
    int16_t     lsq, loc_bdiff[6]={0},i, mlbd, bd, imlbd, bado, badn;
    Bool    snap_active=FALSE,line_scale0=0;
    cell   *c;
    uchar    str[100];
    Bool    ret;
	int16_t   minr,maxr;

    pass_start();
    snap_is_active();
    font_ready=FALSE;
    line_BL=TRUE;

    if( snap_is_active() || snap_is_marked(ln) )
        {
        char        buf[256];

        cstr2txt(buf,ln,(CSTR_line)NULL);
        snap_show_text(buf);
        snap_active=TRUE;
        if( snap_is_marked(ln) )
            {
            snap_show_text("Marked line");
            snap_monitor();
            }
        }

        set_tab_incl();
        db_trace_flag &= 0xf8;   // reset detailed snap
                                                     // need keep language for interrupt
                                                     // ( used for restore in kernel_reload_vocs )
        kit_init();
        setup_string();
        bs_got=0;
        setup_dump ();
        snap_newline();
        skew_corr(ln,0);
        snap_newpass('d');
        letters_ini(ln,0);

		minr=32000;
		maxr=0;
        for(bado=0,c=cell_f()->next;c!=cell_l();c=c->next)
            {
            if( c->flg&c_f_bad )
                bado++;
             if( c->row < minr)
				 minr = c->row;
			 if( c->row + c->h > maxr)
				 maxr = c->row + c->h ;
            }
        line_scale0=line_scale;
        if ((cell_f()->nextl)->next==NULL)
                return FALSE;

        lsq = del_squares();
        if (lsq && snap_activity('d'))
                {
                snap_show_text("Squares deleted");
                snap_monitor();
                }

        if ((cell_f()->nextl)->next==NULL)
                return FALSE;

        prop(0);

        if (snap_activity('d'))
            {
            snap_show_text("Basedraft begin");
            snap_monitor();
            }

		// Nick 17.02.2001
		// установим атрибуты - для skew_corr in basedraft
		{
			CSTR_attr linattr;
			CSTR_GetLineAttr(ln,&linattr);
			linattr.row=minr;
			linattr.hei = maxr-minr;
			CSTR_SetLineAttr(ln,&linattr);
		}
        basedraft(ln);
        cell_by_base();
        if (snap_activity('d'))
            {
            snap_show_text("Linpos begin");
            snap_monitor();
            }
        linpos(ln);
        if (snap_activity('d'))
            {
            snap_show_text("Linpos end");
            snap_monitor();
            }

    for(badn=0,c=cell_f();c!=cell_l();c=c->next)
        {
        bd=abs(c->bdiff);
        if( bd>=5 )
            loc_bdiff[5]++;
        else
            loc_bdiff[bd]++;
        if( c->flg&c_f_bad )
            badn++;
        }
    for(mlbd=loc_bdiff[0],imlbd=0,i=1;i<6;i++)
        {
        if( loc_bdiff[i]>mlbd )
            {
            mlbd=loc_bdiff[i] ;
            imlbd=i;
            }
        }
    ret = (imlbd<2&&loc_bdiff[5]*6<mlbd);
    if( bado*3<badn || bado<3 && bado*3<=badn)
        ret = FALSE;
    if (snap_activity('d'))
        {
        sprintf(str,"string agregat is %s",ret?"combined":"not combined");
        snap_show_text(str);
        snap_monitor();
        }
    CSTR_ClearLine(ln,-16000,32000);
    Cells2CSTR((CSTR_line)NULL,ln,NULL,1);
    line_BL=FALSE;

return ret;
}


Bool match_word_prepare(CSTR_line ln, uchar *alphabet, MatchWordPar *param)
{
    int16_t     lsq;
    uchar    Lang;
    int16_t     Step=0,no_crit;
    Bool    line_scale0=0;
    extern Bool pass4_in;

    line_scale=0;
    pass_start();
    snap_is_active();
    font_ready=FALSE;
    language=param->language;
    p2_active=param->p2_active;

    if( !erection_language(language) )
        erection_enable    = 0;
    pass4_in = FALSE;
    if(p2_active)
        { // some init
        pass4_in = TRUE;
        p2_init();  //init data for second pass
        pass2=0; // safe init

        }

        set_tab_incl();
        db_trace_flag &= 0xf8;   // reset detailed snap
                                                     // need keep language for interrupt
                                                     // ( used for restore in kernel_reload_vocs )
            kit_init();
        Step         = 0;
                no_crit      = 0;
        setup_string();

                Lang=language;

        bs_got=0;
        db_trace_flag &= 0xf8;   // reset detailed snap

        setup_dump ();

        skew_corr(ln,0);

    snap_newpass('n');
    letters_ini(ln, 1);
    line_scale0=line_scale;
        if(!p2_active)
                avdens();
        if ((cell_f()->nextl)->next==NULL)
                return FALSE;

        if(language==LANG_ENGLISH)
                ReRecognizeComponents();

        lsq = del_squares();

        if (lsq && snap_activity('n'))
                {
                snap_show_text("Squares deleted");
                snap_monitor();
                }

        if ((cell_f()->nextl)->next==NULL)
                return FALSE;

        prop(0);


        no_crit=small_english_str();

    if (p2_setBasLines(ln)==-1)
        return FALSE;
    if( test_bl_comps(ln) )
        {
        no_linpen=1; // NO CUT
        }
    cell_by_base();
    if (snap_activity('d'))
        {
        snap_show_text("Linpos begin");
        snap_monitor();
        }
    linpos(ln);

    if( !no_crit )
        no_crit = test_short_english_blines();

    if ((cell_f()->nextl)->next == NULL)
        {
        if (snap_activity('n'))
                {
                snap_show_text("Skiped as dust");
                snap_monitor();
                }
                        return FALSE;
        }

    if (prop_in_trouble && let_to_dust)
            prop(1);

    checkpitch();
    clear_dust_alternates();
    if( erection_enable &&
        erection_language(language))
        {
        erection_incline_words(0);
        if( erection_cond_language(language) )
            erection_delete();
        }

    snap_newpass('n');

    if (snap_activity('n'))
            {
            snap_show_text("Diffs beg");
            snap_monitor();
            }

    criteries();

    if (snap_activity('n'))
            {
            snap_show_text("Diffs end");
            snap_monitor();
            }
    if( alphabet['©'] &&
		!is_russian_turkish_conflict(language)	// 21.05.2002 E.P.
	  )
            proc_ii(); //склейка всех 'й'

///////// Common code block
            set_cells_language(language);
    if( alphabet['л'] &&
			!is_baltic_language(language)	// 17.07.2001 E.P. ==!is_russian_baltic_conflict(let)
	  )
                    proc_bI(1);// склейка всех 'ы'

    if( p2_active && cell_f()->next==cell_l() )
        {
                            return FALSE;
         }

    // Oleg : 11-01-1999 for trash bad lines
    {
    B_LINES my_bases;
    get_b_lines(NULL,&my_bases);
    if( my_bases.b1>my_bases.b2 ||
        my_bases.b2>my_bases.b3 ||
        my_bases.b3>my_bases.b4  )
        {
                            return FALSE;
        }
    }

return TRUE;
}

Bool pass1_test_alphabet(str_info *str,CSTR_line ln)
{
CSTR_attr   la;
CSTR_GetLineAttr(ln,&la);
snap_is_active();
line_number = (int16_t)la.number;
kit_init();
setup_string();
letters_ini(ln, 1);
make_simples_diff(la.language);
save_alphabet_information(str,0,ln); // 0 column - old multy-col interface
clear_cells();
line_number =0;
return TRUE;
}

Bool pass2_test_alphabet(str_info *str,CSTR_line ln)
{
CSTR_attr   la;
CSTR_GetLineAttr(ln,&la);
snap_is_active();
line_number = (int16_t)la.number;
kit_init();
setup_string();
letters_ini(ln, 1);
//make_simples_diff(la.language);
save_alphabet_information_pass2(str,0,ln); // 0 column - old multy-col interface
clear_cells();
line_number =0;
return TRUE;
}


Bool add_rus_under(cell *c)
{
Bool    ret = FALSE;
uchar    pr = (uchar)(MAX((int16_t)c->vers[0].prob-10,2));
switch( c->vers[0].let )
    {
    case    (uchar)'з':
		if(is_russian_turkish_conflict(c->vers[0].let)) // 21.05.2002 E.P.
			break;

        add_stick_vers(c,(char)'г', pr ) ;
        ret = TRUE;
        break;
    case    (uchar)'Ё':
        add_stick_vers(c,(char)'ж', pr ) ;
        add_stick_vers(c,(char)'г', (uchar)MAX((int16_t)pr-10,2) ) ;
        ret = TRUE;
        break;
    case    (uchar)'®':
 		if(is_russian_turkish_conflict(c->vers[0].let)) // 21.05.2002 E.P.
			break;
       add_stick_vers(c,(char)'а', pr ) ;
        ret = TRUE;
        break;
    default:
        break;
    }
return FALSE;
}

Bool add_eng_under(cell *c)
{
Bool    ret = FALSE;
uchar    pr = (uchar)(MAX((int16_t)c->vers[0].prob-10,2));
switch( c->vers[0].let )
    {
    case    (uchar)'v':
        add_stick_vers(c,(char)'y',pr ) ;
        ret = TRUE;
        break;
    case    (uchar)'o':
        add_stick_vers(c,(char)'p',pr ) ;
        ret = TRUE;
        break;
    case    (uchar)'a':
        add_stick_vers(c,(char)'q',pr ) ;
        add_stick_vers(c,(char)'g',pr ) ;
        ret = TRUE;
        break;
    default:
        break;
    }
return ret;
}
//  for omnifont pass only
void add_vers_underlined(void)
{
cell *  c;
uint16_t  flg;
for(c=cell_f()->nextl;c!=cell_l();c=c->nextl)
    {
    if( c->nvers && ((c->flg_new&CSTR_fn_under)||(c->font&c_fp_undrln)) )
        {
        flg = (c->flg&(CSTR_f_solid|CSTR_f_confirmed));
        if( is_russian_language(c->language) )
            add_rus_under(c);
        else
            add_eng_under(c);
        c->flg|=flg;
        }
    }
}

void cstr_rerecog_leo(CSTR_line ln)
{
CSTR_rast       rst;
CSTR_rast_attr  attr;
UniVersions     u, uo;
RecRaster       rc;
int32_t           n, i;

for(rst=CSTR_GetNextRaster(CSTR_GetFirstRaster(ln),CSTR_f_let);
    rst; rst=CSTR_GetNextRaster(rst,CSTR_f_let) )
    {
    CSTR_GetAttr(rst,&attr);
    if( attr.language==LANG_RUSSIAN )
        {
        CSTR_GetImage(rst,(uchar*)&rc, CSTR_TYPE_IMAGE_RS);
        CSTR_GetCollectionUni(rst,&uo);
        u=uo;
        RecogLEO(&rc,attr.language,&u);
        if( u.lnAltCnt &&
            u.Alt[0].Liga!='|' )
            {
            if( u.Alt[0].Prob>200 )
                CSTR_StoreCollectionUni(rst,&u);
            else if( uo.lnAltCnt &&
                     u.Alt[0].Prob>20+uo.Alt[0].Prob )
                {
                n = MIN(REC_MAX_VERS-1,uo.lnAltCnt);
                for(i=0;i<1;i++)
                    u.Alt[i+1]=uo.Alt[i];
                u.lnAltCnt = n+1;
                CSTR_StoreCollectionUni(rst,&u);
                }

            }
        }
    }
}

Bool recog_minus(void)
{
int16_t up, dn, dh, rn, con, wn, hn;
cell    *c=cell_f()->next, *cn, *clist[2];

dh=(bbs3-bbs2)/4;
if( c->next!=cell_l() )
    {
    cn=c->next;
    if( (c->flg&c_f_dust) && (cn->flg&c_f_dust) &&
        cn->next==cell_l() )
        { // two dusts
        rn = MIN(c->row,cn->row);
        hn = MAX(c->row+c->h,cn->row+cn->h)-rn;
        con = c->col;
        wn = cn->col+cn->w-con;
        up = rn-minrow;
        dn = MAX(c->row+c->h,cn->row+cn->h)-minrow;
        if( hn*12<wn*5 &&
            wn>=Ps/2 &&
            up>=bbs2+dh &&
            dn<=bbs3-dh )
            {
            clist[0]=c;
            clist[1]=cn;
            compose_cell(2,clist,c);
            c->vers[0].let='-';
            c->vers[0].prob=254;
            c->vers[1].let=0;
            c->nvers=1;
            c->flg = c_f_punct;
            c->nvers=1;
            c->keg=Ps<<line_scale;
            c->language=language;
            line_minus=1;
            return TRUE;
            }
        }
    else
        return FALSE;
    }
up=c->row-minrow;
dn=up+c->h;

if( c->h*12<c->w*5 &&
    c->w>=Ps/2  /*&& up>=bbs1 && dn<=bbs4 */)
    {
    c->vers[0].let='-';
    c->vers[0].prob=( up>=bbs2+dh && dn<=bbs3-dh)?254:150;
    c->vers[1].let=0;
    c->flg = c_f_punct;
    c->nvers=1;
    c->keg=Ps<<line_scale;
    c->language=language;
    line_minus=1;
    return TRUE;
    }
return FALSE;
}

int16_t p2_GetPs(void)
{
cell *  c;
char    lets[]="weuoaszxcvnmЄҐ­Ји§еклў Ї®«¦нпзб¬ЁвмЎо";
//			   "weuoaszxcvnmкенгшзхъываполжэячсмитьбю"

int32_t    s2,s1,n,h,s12,na,nl;

for(s1=s2=na=nl=n=0,c=cell_f()->nextl;c!=cell_l();c=c->nextl,na++)
    {
    if( c->nvers                        &&
        (((c->recsource|c->history) & c_rs_bitcmp)    ||
        c->flg&(c_f_let|c_f_confirmed))&&
        strchr(lets,c->vers[0].let)     &&
		!is_russian_baltic_conflict(c->vers[0].let) &&	// 17.07.2001 E.P.
		!is_russian_turkish_conflict(c->vers[0].let)&& // 21.05.2002 E.P.
        c->vers[0].prob>200
        )
        {
        if( c->vers[0].prob>200 )
            n+=2;
        else
            n++;
        h=c->h;
        s1+=h;
        s2+=h*h;
        }
    if( c->nvers )
        nl++;
    }
if( n>=6*2 || nl<6 && nl*2>na && n>2 )
    {
    s1 *=2; s2*=2; // n - scaled * 2
    s1/=n;
    s2/=n;
    s12=s1*s1;
    s2-=s12;
    if( s12>s2*7 ) // dispersion > CKO * 2.65
        return (int16_t)s1;
    }
return 0;
}

int16_t p2_GetPs_up(void)
{
cell *  c;
char    lets_up[]="WERTYUOPQASDFGHJKLZXCVBNM‰“Љ…Ќѓ‡•љ”›‚ЂЏђЋ‹†ќџ—‘Њ€’њЃћ012234567890";
//				  "WERTYUOPQASDFGHJKLZXCVBNMЙУКЕНГШЗХЪФЫВАПРОЛЖЭЯЧСМИТЬБЮ012234567890";

int32_t    s2,s1,n,h,s12,na,nl;

for(s1=s2=na=nl=n=0,c=cell_f()->nextl;c!=cell_l();c=c->nextl,na++)
    {
    if( c->nvers                        &&
        (((c->recsource|c->history) & c_rs_bitcmp)  ||
        c->flg&(c_f_let|c_f_confirmed) ) &&
        strchr(lets_up,c->vers[0].let)   &&
		!is_russian_turkish_conflict(c->vers[0].let)&& // 21.05.2002 E.P.
        c->vers[0].prob>200
        )
        {
        if( c->vers[0].prob>200 )
            n+=2;
        else
            n++;
        h=c->h;
        s1+=h;
        s2+=h*h;
        }
    if( c->nvers )
        nl++;
    }
if( n>1 )
    {
    s1 *=2; s2*=2; // n - scaled * 2
    s1/=n;
    s2/=n;
    s12=s1*s1;
    s2-=s12;
    if( s12>s2*7 ) // dispersion > CKO * 2.65
        return (int16_t)(s1*2/3);
    }
return 0;
}

void pass3_special_recode(CSTR_line ln)
{
CSTR_rast       rst;
CSTR_rast_attr  attr;
UniVersions     uni;

if( !(language==LANG_RUSSIAN && multy_language) )
    return;

for(rst=CSTR_GetNext(CSTR_GetFirstRaster(ln));rst;rst=CSTR_GetNext(rst))
    {
    CSTR_GetCollectionUni(rst,&uni);
    CSTR_GetAttr(rst,&attr);
    if( uni.lnAltCnt && uni.Alt[0].Liga==0x84 && attr.language==LANG_ENGLISH )
        {
        uni.Alt[0].Liga=0xd5;
        CSTR_StoreCollectionUni(rst,&uni);
        }
    }
return;
}


void pass3_table_points_suspension(CSTR_line ln)
{
CSTR_rast       rst, rsts;
//CSTR_rast_attr  attr;
UniVersions     uni;
char            dots[]=".,\xd5";
int32_t           numdots=0;

if( !line_tabcell )
    return;
for(numdots=0,rst=CSTR_GetNext(CSTR_GetFirstRaster(ln));rst;rst=CSTR_GetNext(rst))
    {
    CSTR_GetCollectionUni(rst,&uni);
    //CSTR_GetAttr(rst,&attr);
    if( uni.lnAltCnt && strchr(dots,uni.Alt[0].Liga) /*||
       !uni.lnAltCnt && attr.h<CCOM_DUST_SIZE*/ )
        {
        numdots=1;
        rsts=rst;
        for(rst=CSTR_GetNext(rst);rst;rst=CSTR_GetNext(rst) )
            {
            CSTR_GetCollectionUni(rst,&uni);
            if( !uni.lnAltCnt || !strchr(dots,uni.Alt[0].Liga)  )
                break;
            numdots++;
            }
        if( numdots>5 )
            {
            for(rst=rsts;rst;rst=CSTR_GetNext(rst) )
            {
            CSTR_GetCollectionUni(rst,&uni);
            if( uni.lnAltCnt )
                {
                switch( uni.Alt[0].Liga  )
                    {
                    case    ',':
                        uni.Alt[0].Liga='.';
                        uni.Alt[0].Code[0]='.'; uni.Alt[0].Code[1]=0;
                        CSTR_StoreCollectionUni(rst,&uni);
                        break;
                    case    '.':
                        break;
                    case    (uchar)'\xd5':
                        uni.Alt[0].Liga='.';
                        uni.Alt[0].Code[0]='.'; uni.Alt[0].Code[1]=0;
                        CSTR_StoreCollectionUni(rst,&uni);
                        break;
                    }
                }
            }
            }
        else
            rst = rsts;
        }
    }
return;
}

void pass3_table_points_suspension2(CSTR_line ln)
{
CSTR_rast       rst;
UniVersions     uni;
char            dots[]=".,\xd5\xFE\x1e\x1f\"-\x20"; //"'.,\xd5\xFE\x1e\x1f\"-\x20";

if( !line_tabcell )
    return;
for(rst=CSTR_GetPrev(CSTR_GetLastRaster(ln));rst;rst=CSTR_GetPrev(rst))
    {
    CSTR_GetCollectionUni(rst,&uni);
    if( uni.lnAltCnt && strchr(dots,uni.Alt[0].Liga)  )
        {
        switch( uni.Alt[0].Liga  )
            {
            case    (uchar)0x1e:
            case    (uchar)0x1f:
            case    ' ':
                uni.Alt[0].Liga='\x1e';
                uni.Alt[0].Code[0]=0;
                uni.Alt[0].Prob=75;
                CSTR_StoreCollectionUni(rst,&uni);
                break;
            case    (uchar)0xFE:
            case    '\"':
            case    '-':
            case    ',':
                uni.Alt[0].Liga='.';
                uni.Alt[0].Code[0]='.'; uni.Alt[0].Code[1]=0;
                CSTR_StoreCollectionUni(rst,&uni);
                break;
            case    '.':
                break;
            case    (uchar)'\xd5':
                uni.Alt[0].Liga='.';
                uni.Alt[0].Code[0]='.'; uni.Alt[0].Code[1]=0;
                CSTR_StoreCollectionUni(rst,&uni);
                break;
            }
        }
    else
        break;
    }
return;
}

//  Nick 20.02.2001  -
// чтобы без потерь использовать CopyAttr2CSTR!!!
Bool SetFontReady(Bool fready)
{
	Bool old = font_ready;

	font_ready = fready;

	return old;
}
