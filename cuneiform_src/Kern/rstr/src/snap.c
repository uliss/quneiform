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


////////////////////////////////////////////////////
// EMBRION of SNAP-container
////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nt_types.h"
#include "func.h"
#include "struct.h"
#include "status.h"
#include "dpuma.h"
#include "cstr.h"
#include "ctb.h"
#include "std.h"
#include "resource.h"
#include "cuthdr.h"
#include "p2libr.h"
#include "tuner.h"
#include "fon.h"
#include "minmax.h"

static void snap_sticks(cell *,char *);


// from module PASSE
BOOL _spell(PCHAR s,BYTE lang);
BOOL _spell_agressive(PCHAR s,BYTE lang);
BOOL short_spell(BYTE *wrd,BYTE language );
BOOL short_spell_re(BYTE *wrd,BYTE language );

BYTE mwInput[80];
extern INT      line_number;
extern INT      show_cut_points;
extern BYTE     db_pass      ;
extern BYTE     db_trace_flag, language;
extern  struct  cut_elm *my_cut_points;
extern  INT             show_cut_points;
extern  INT             flag_cut_point;
extern  INT     flag_ShiftF9;
extern  void    Cells2CSTR(CSTR_line lin,CSTR_line lino,cell *cur,Bool32 enable_scaled);
extern  void    CopyAttr2CSTR(CSTR_rast_attr *attr, cell *c);
extern  int     cell2UniVers(UniVersions *ver, cell *c);
// from EVN32.DLL
extern LONG EVNGetRepresent( RecRaster   *rRaster,
    Word8 *evn, Word8 *evn_rot, Int32 font);
extern LONG  EVNRecogCharPRN(   RecRaster  * rRaster,
      RecVersions* EVNres       );
// from RCM.C
extern  void    ErrorExit(int Code);
extern  BYTE    decode_ASCII_to_[256][4];
extern  CHAR    StopPoint;
extern  BYTE    CodePages[];
//extern  INT     gbCol1,gbCol2;

static  BOOL    exit_enable         = FALSE;
static  BOOL    cut_enable          = FALSE;
static  BOOL    snap_disable        = FALSE;
static  BOOL    snap_continue       = FALSE;
BOOL    snap_page_disable   = FALSE;
static  BYTE    db_skip_client;
static  CSTR_line   snap_line;
static  cell        currcell;//,*db_stopcell;
static  Point32 cutpoints_show[128];
static  Word32  numpoints_show;
static  cell *  cutpoints_cell=NULL;
static  Int32   select_line=0, num_select_lines=0;
cell   *stopcell=NULL;
static  int stop_number=0, curr_pass=-1;

static  BYTE *snap_pass[]={
"Cut","Glue","Context","Line","Punct","Space","Lineout","(TM)(R)","Italic","Bold","Serif","Solid","Pass2","Digital","BLCut","\0"
// a   b      c         d      e       f       g          h        i        j      k       l       m       n         o
};
static  BYTE *snap_fict[]={
"Fict0","Fict1","Fict2","Fict3","Fict4","Fict5","Fict6","Fict7","Fict8","Fict9","Fict10","Fict11","Fict12","Fict13","Fict14","\0"
// a    b       c       d       e       f       g       h       i       j       k        l        m         n        o
};
static char *snap_pass_help[]={
/* a */"разрезание (и склеивание для русского)",
/* b */"склеивание ( для нерусского)",
/* c */"контекст (+выпрямление)",
/* d */"определение базовых линий и диффы",
/* e */"пунктуация",
/* f */"пробелы",
/* g */"показ финальной строки",
/* h */"распознавание ®©",
/* i */"распознавание курсивности",
/* j */"распознавание жирности",
/* k */"распознавание серифности",
/* l */"словарная валидация",
/* m */"второй проход",
/* n */"цифровые строки",
/* o */"запретить обрезание грязи по БЛ",
""
};
static char *snap_fict_help[100]={0};
static  int     snap_clients=sizeof(snap_pass)/sizeof(snap_pass[0])-1;
Handle hSnapMain,hSnapMainFict;
Handle hSnapPass[sizeof(snap_pass)/sizeof(snap_pass[0])];
Handle hSnapFict[100];
Handle hSnapSerifTrace;
Handle hSnapLEO;
Handle hSnapSmartCut;
//IGOR
static  BYTE *snap_bl[]={
"STAT Base Lines","PUMA & STAT Base Lines", "PUMA & STAT Base Lines & Differences","\0"
// a   b
};
static char *snap_bl_help[]={
/* a */"Статистический метод определения базовых линий\x0D\x0AОстанов и прорисовка базовых линий осуществляется в режиме установки точки",
/* b */"PUMA + cтатистический метод.\x0D\x0AСтатистический метод работает фиктивно.\x0D\x0AОстанов и прорисовка базовых линий осуществляется в режиме установки точки",
/* c */"PUMA + cтатистический метод.\x0D\x0AСтатистический метод работает фиктивно.\x0D\x0AПоказать различия на всей странице.",
""
};
Handle hSnapLineBL[sizeof(snap_bl)/sizeof(snap_bl[0])];
//IGOR
//Handle hUseCLine;
extern Handle hSnapBLcut;
static  BOOL    internal_skip[sizeof(snap_pass)/sizeof(snap_pass[0])];

void reset_snap(void)
{
LDPUMA_SetConsoleProperty(0,0,
        0,0,0,  0,      0,
        CodePages[3],   NULL);
return;
}
static int Lang_Console(char *text, Word8 lang)
{
char    buf[1024];
//LDPUMA_SetConsoleProperty(Bool32 bold,Bool32 italic,
//      Bool32 strikeout,Bool32 underline,Int32 height, Int32 offset,   Word32 textcolor,
//      int charset,    char * name )
LDPUMA_SetConsoleProperty(0,0,
        0,0,0,  0,      0,
        CodePages[lang],        NULL);//"Courier New" ) ;
strcpy(buf,text);
strcat(buf,"\n");
return LDPUMA_Console(buf);
}

static int Snap_Console(char *text)
{
return Lang_Console(text,language);
}

static int RUS_Console(char *text)
{
return Lang_Console(text,3);
}

// перекодировка
static BOOL ASCII2ANSI(char *ascii,char *ansi)
{
*ansi='\0';
for(;*ascii ;ascii++)
    {
    strcat(ansi, decode_ASCII_to_[(BYTE)*ascii] );
    }
return TRUE;
}

// из контейнера в cell
static  cell * snap_CSTR2cell(CSTR_rast rst)
{
cell *c;
CSTR_rast_attr attr;
int scale;
if( !rst )
    return NULL;
CSTR_GetAttr(rst,&attr);
for(c=cell_f();c!=cell_l();c=c->next)
    {
    scale = c->env ? c->env->scale : 0;
    if( attr.col==(c->col<<scale) &&
        attr.row==(c->row<<scale) &&
        attr.w  ==(c->w  <<scale) &&
        attr.h  ==(c->h  <<scale))
        {
        return c;
        }
    }
return NULL;
}

// из cell-a в контейнер
static   CSTR_rast snap_cell2CSTR(cell *c)
{
CSTR_rast       rst=CSTR_GetFirstRaster(snap_line);
CSTR_rast_attr  attr;
int scale;
if( !c )
    return (CSTR_rast)NULL;
scale = c->env ? c->env->scale : 0;
for(rst=CSTR_GetNext(rst);rst;rst=CSTR_GetNext(rst))
    {
    CSTR_GetAttr(rst,&attr);
    if( attr.col==(c->col<<scale) &&
        attr.row==(c->row<<scale) &&
        attr.w  ==(c->w  <<scale) &&
        attr.h  ==(c->h  <<scale))
        {
        return rst;
        }
    }
return (CSTR_rast)NULL;
}

// позиция cell-a
static Word32 snap_cell2pos(cell *cl)
{
Word32 pos;
cell    *c,*e;
for(pos=0,c=cell_f()->next,e=cell_l();c!=e;c=c->next,pos++)
    {
    if( c==cl )
        return pos;
    }

return pos;
}

// общая инициализаци
BOOL snap_init(void)
{
int i;
static  init=FALSE;
if( init )
    return TRUE;
init=TRUE;
LDPUMA_Init(0,NULL);
//LDPUMA_Registry (&hUseCLine,"Работа с контейнером линий", NULL);

LDPUMA_Registry(&hSnapMain,"Распознавание строк...",NULL);
for(i=0;snap_pass[i][0];i++)
    {
    LDPUMA_Registry(&hSnapPass[i],snap_pass[i],hSnapMain);
        LDPUMA_RegistryHelp(hSnapPass[i],snap_pass_help[i],FALSE);
    }
LDPUMA_Registry(&hSnapMainFict,"Fictive points...",hSnapMain);
for(i=0;i<5;i++)
    {
    LDPUMA_Registry(&hSnapFict[i],snap_fict[i],hSnapMainFict);
        LDPUMA_RegistryHelp(hSnapFict[i],snap_fict_help[i],FALSE);
    }
LDPUMA_Registry(&hSnapBLcut,"Запретить разрезание по базам",hSnapMain);
LDPUMA_Registry(&hSnapLEO,"Запретить ЛЕО",hSnapMain);
LDPUMA_Registry(&hSnapSerifTrace,"Serif Dispaly",hSnapPass['k'-'a']);
LDPUMA_RegistryHelp(hSnapSerifTrace,"Draw rectangles",FALSE);
//IGOR
for(i=0;snap_bl[i][0];i++)
    {
    LDPUMA_Registry(&hSnapLineBL[i],snap_bl[i],hSnapPass['d' - 'a']);
        LDPUMA_RegistryHelp(hSnapLineBL[i],snap_bl_help[i],FALSE);
    }
//IGOR
LDPUMA_RegVariable(hSnapMain,"Трассировка",&db_trace_flag,"char");
#ifdef MATCH_WORD
LDPUMA_RegVariable(hSnapMain,"Наложение слова",mwInput,"char *");
#endif
LDPUMA_Registry(&hSnapSmartCut,"Запретить кластерное разрезание/склеивание на 2-ом проходе",hSnapMain);
LDPUMA_RegistryHelp(hSnapSmartCut,"Запретить кластерное разрезание/склеивание на 2-ом проходе",FALSE);
snap_page_disable = snap_disable = db_skip_client = FALSE;
memset(internal_skip,TRUE,snap_clients*sizeof(BOOL));
exit_enable=FALSE;
db_trace_flag=0;
db_skip_client=0;
cutpoints_cell=NULL;
return TRUE;
}

BOOL snap_init_variables(void)
{
snap_page_disable = snap_disable = db_skip_client = FALSE;
exit_enable=FALSE;
db_trace_flag=0;
db_skip_client=0;
cutpoints_cell=NULL;
return TRUE;
}

BOOL snap_fictive_skip(INT i)
{
return (BOOL)LDPUMA_SkipEx(hSnapFict[i],FALSE,TRUE,1);
}

// показ распознавания эвент
static void snap_recogEVN(CSTR_rast r,cell * cl)
 {
 INT    i;
 RecRaster  rRaster;
 char       evn[256], evnr[256],evnw[256], evnrw[256], buf[256];

 RecVersions rv;
 if (!(cl->flg & (c_f_bad | c_f_let)))
  return;
 if (tenv(cl))
  {
  CSTR_GetImage(r,(Word8*)(&rRaster),CSTR_TYPE_IMAGE_RS);

  if( EVNGetRepresent( &rRaster, evn, evnr, 1)==-1  )
    RUS_Console("Не могу получить ЛП");
  else
    {
    ASCII2ANSI(evn,evnw);
    ASCII2ANSI(evnr,evnrw);
    sprintf(evn, "EVN : %s",evnw);
    sprintf(evnr,"ROT : %s",evnrw);
    Snap_Console(evn);
    Snap_Console(evnr);
    }
  EVNRecogCharPRN(   &rRaster,   &rv );
  for(i=0;i<rv.lnAltCnt;i++)
    {
    if( !pidx_skip(cl->h, cl->w,rv.Alt[i].Code) )
        {
        sprintf(buf,"PROP KILL : %s,pidx=%d",
            decode_ASCII_to_[(BYTE)rv.Alt[i].Code],
            prop_index(cl->h,cl->w));
        RUS_Console(buf);
        }
    }
  }
 return;
 }

// показ дискриминатора пропорций
static void show_prop(cell *c)
{
int i;
char    buf[256];
for(i=0;i<c->nvers;i++)
    {
    if( !pidx_skip(c->h, c->w,c->vers[i].let) )
        {
        sprintf(buf,"PROP KILL : %s",decode_ASCII_to_[(BYTE)c->vers[i].let]);
        RUS_Console(buf);
        }
    }
return;
}


// показ разных кластеров 3х5
static void snap_recog3X5(cell *b)
 {
 s_glue GL={0};
 cell   sb=*b;
 BYTE save_db_pass=db_pass;

 db_pass='a';
 internal_skip[0]=FALSE;
 GL.arg=GABOXt;
 crepat(b,&GL,0,0);
 crecell(b,&GL,3);
 if (b->nvers)
    {
    RUS_Console("BOX : ");
    dmiBOX(b,&GL,1);
    }

 GL.arg=0;
 set_bad_cell(b);
 RUS_Console("BOXL : ");
 dmiBOX(b,&GL,1);

 GL.arg=GABOXl;   // no LINEAR !!
 set_bad_cell(b);
 RUS_Console("BOXF : ");
 dmiBOX(b,&GL,1);
internal_skip[0]=TRUE;
*b=sb;
db_pass=save_db_pass;
return;
}

// показ батонов Фараджева
static void snap_sticks(cell *c, char *str)
 {
 INT i,n;
 STICK *res,*r;

 n=sticks_in_letter(c,0,&res);
 if( str )
     {
     if (n>=0)
        for (*str=0,r=res,i=0; i<n; i++,r++)
            sprintf(str+strlen(str),"x=%d,y=%d,l=%d,w=%d,incl=%d,bot=%d,top=%d\n",
                                      r->x,r->y,r->l,r->w,r->incl,r->bot,r->top);
     else
          {
          sprintf(str,"can't find sticks:\n");
          if (n==-1) sprintf(str+STRLEN(str),"base lines not counted");
          if (n==-2) sprintf(str+STRLEN(str),"no envelope");
          if (n==-3) sprintf(str+STRLEN(str),"too many intervals");
          if (n==-4) sprintf(str+STRLEN(str),"too many extremums");
          if (n==-5) sprintf(str+STRLEN(str),"too many sticks");
          }
    }
 return;
 }

static void store_spell(cell *c)
{
Word8       wrd[80],buf[160],*w=wrd;
FILE       *fp=fopen("frh.txt","at");
int         num_ln=0;
cell       *b, *e;

for(;c->flg&(c_f_let|c_f_bad);c=c->prev);
b=c->next;
for(c=c->next;c->flg&(c_f_let|c_f_bad);c=c->next);
e=c;
for(c=b;c!=e;c=c->next)
    {
    *w++ = c->vers[0].let;
    }
*w=0;

if( fp )
    {
    Int32       i;
    //if( num_ln<2 && num_ln!=-1)
    //    fprintf(fp,"New Line\n");
    fprintf(fp,"-- line %d \n", num_ln);
    for(c=b;c!=e;c=c->next)
        {
        fprintf(fp,"{%d,%d,%d,%d} ",c->row,c->col,c->row+c->h,c->col+c->w);
        if( c->nvers)
            {
            for(i=0;i<c->nvers;i++)
                fprintf(fp,"%c(%d) ",stdAsciiToAnsi(c->vers[i].let),c->vers[i].prob);
            }
        else
            {
            fprintf(fp,"~(0) ");
            }
        fprintf(fp,"\n");
        }
    fprintf(fp,"-- ");
    }
fclose(fp);
sprintf(buf,"word <%s> is saved as frh",wrd);
snap_show_text(buf);

return;
}

static void show_spell(cell *c)
{
Word8   wrd[80],buf[160],*w=wrd, lang;
Bool32  nonrec=FALSE;


for(;c->flg&(c_f_let|c_f_bad);c=c->prev);

for(c=c->next,lang=c->language;c->flg&(c_f_let|c_f_bad);c=c->next)
    {
    if( c->flg&c_f_bad )
        nonrec=TRUE;
    *w++ = c->vers[0].let;
    }
*w=0;
strcpy(buf,wrd);

strcat(buf," static : ");
if( _spell(wrd,lang)        )
    strcat(buf,"y");
else
    strcat(buf," n");

strcat(buf," small : ");
if( short_spell(wrd,lang)   )
    strcat(buf,"y ");
else
    strcat(buf," n");

strcat(buf," small_re : ");
if( short_spell_re(wrd,lang)   )
    strcat(buf,"y ");
else
    strcat(buf," n");

strcat(buf," new agressive : ");
if( _spell_agressive(wrd,lang) )
    strcat(buf,"y ");
else
    strcat(buf," n");
snap_show_text(buf);
return;
}

Int32 snap_AddRecRaster(char *fname, RecRaster *r,Word8 let, Word8 font)
{
#ifdef _USE_CTB_
CTB_handle  hnd;
Int32       num;
Word8       raster[4096],data[CTB_DATA_SIZE]={0};
Int32       wb;
Bool32      ret;
B_LINES     my_bases;

get_b_lines(NULL,&my_bases);
if( !CTB_open( fname, &hnd, "w") )
    {
    if( 1 )
        CTB_create(fname,NULL);
    else
        CTB_create_gray(fname,NULL);
    if( !CTB_open( fname, &hnd, "w") )
        {
        return FALSE;
        }
    }
data[1] = (Word8)r->lnPixWidth;
data[2] = (Word8)r->lnPixHeight;
data[3] = let;
data[4] = 0; // typeface
if( font & CSTR_fp_it )
    data[5] |= CTB_PRINT_ITALIC;
if( font & CSTR_fp_bold )
    data[5] |= CTB_PRINT_BOLD;
if( font & CSTR_fp_ser )
    data[5] |= CTB_PRINT_SERIFIC;
memcpy(&data[6],&my_bases,sizeof(my_bases));
if( hnd.signums==(CTB_GRAY_SCALE|CTB_PLANE) )
    {
    wb = r->lnPixWidth;
    memcpy( raster, r->Raster, wb*r->lnPixHeight);
    }
else
    {
    wb = ((r->lnPixWidth+63)/64)*8;
    memcpy( raster, r->Raster, wb*r->lnPixHeight);
    CTB_align1_lines(raster, r->lnPixWidth, r->lnPixHeight);
    }
ret = CTB_write( &hnd, -1, raster,  data);
num = hnd.num;
CTB_close( &hnd);
return ret?num:-1;
#else
return -1;
#endif
}

// действия по изменению растра
#define END_RUN_CELL(s)                                     \
    {                                                       \
    CSTR_DelRaster(r);                                      \
    if( (r=CSTR_NewRaster (snap_line ,c->r_col, c->r_row,c->w))!=0 ) \
        {                                                   \
        CopyAttr2CSTR(&attr, c);                            \
        CSTR_SetAttr (r, &attr);                            \
        CSTR_SetAttr (r, &attr);                            \
        cell2UniVers(&uvs, c);                              \
        CSTR_StoreCollectionUni (r, &uvs);                  \
        if( c->env )                                        \
          CSTR_StoreComp (r, (Word8*)((Word8*)c->env+c->env->lines),1,c->env->scale); \
        RUS_Console(s);                                    \
        }                                                   \
    LDPUMA_CSTR_Update();                                   \
    }

#ifdef _USE_DFON_
  int (*DFon_SendRaster)(RecRaster* rr, Handle pPaintWnd, Word8 let);
  void (*DFon_Terminated)(Word32 wParam);
#endif

// обработчик  сообщений
Word32 myMonitorProc(Handle wnd,Handle hwnd,Word32 message,Word32 wParam,Word32 lParam)
{
int             ret=0;
CSTR_rast       r=(CSTR_rast)0;
cell    *       c=NULL;
UniVersions     uvs;
RecRaster       rRaster;
CSTR_rast_attr  attr;
static Bool32   shift=0;
BYTE            str[1200];
static CSTR_rast s_r = (CSTR_rast)0;
unsigned int    i,ii;
static BOOL     no_process=TRUE;
Word32          pos;
char            buf[256];

pos=LDPUMA_CSTR_GetPosition((int *)&r);
if( r )
    c = snap_CSTR2cell(r); // for any call
if(no_process && cutpoints_cell)
    {
    if( show_cut_points || cutpoints_cell==c)
        {

        no_process=FALSE;
        if( r!=s_r || show_cut_points)
        {
        s_r = r;
        for(i=0;i<numpoints_show;i+=2)
            {
            LDPUMA_ShowCutPoint(2, &cutpoints_show[i] );
            }
        }
        no_process=TRUE;
        show_cut_points=0;
        }
     else
         {
         if( r!=s_r )
            s_r = r;
         }
     }

switch(message)
        {
    // MOUSE CLICKS
    case WM_LBUTTONDBLCLK:
                { // select start cut point
                        Point32 p = { (Word16)(lParam), (Word16)(lParam>>16)};  // vertical position of cursor  }
                        p = LDPUMA_GetRasterPixel(wnd,p);
            for(ii=1,i=0;i<numpoints_show;i+=2,ii++)
                {
                if( abs(my_cut_points[ii].x-p.x)<2 )
                    break;
                }
            if( i<numpoints_show )
                { // setup start cut point
                            flag_cut_point = 1;
                StopPoint = my_cut_points[ii].x;
                sprintf(buf,"Точка разрезания (%d)",ii-1);
                RUS_Console(buf);
                db_status=0;
                ret=-1;
                }
                }
                break;

    case WM_RBUTTONDBLCLK:
                { // show one cut point
                Point32 p = { (Word16)(lParam), (Word16)(lParam>>16)};  // vertical position of cursor  }
                p = LDPUMA_GetRasterPixel(wnd,p);
                for(ii=1,i=0;i<numpoints_show;i+=2,ii++)
                    {
                    if( abs(my_cut_points[ii].x-p.x)<2 )
                        break;
                    }
                if( i<numpoints_show )
                    {
                                sprintf(buf,"Точка разрезания (%d) x=%i,dh=%i,h=%i,var=%i",ii-1,
                        my_cut_points[ii].x,my_cut_points[ii].dh,
                        my_cut_points[ii].h,my_cut_points[ii].var);
                    RUS_Console(buf);
                    }
                }
                break;
    case WM_DDE_TERMINATE:
#ifdef _USE_DFON_
                 {
                 DFon_Terminated(wParam);
                 }
#endif
                 break;

    // ALT + KEY
    case WM_SYSKEYDOWN:
        switch((int)wParam)
        {
        case    'F':
            if( c&&c->flg&(c_f_bad|c_f_let) )
                store_spell(c);
            else
                RUS_Console("Не буква");
            ret=1;
            break;
        case    'S':
            if( c&&c->flg&(c_f_bad|c_f_let) )
                show_spell(c);
            else
                RUS_Console("Не буква");
            ret=1;
            break;
        case    'X':    // ALT + X
            exit_enable=TRUE;
            ret=-1;
            break;
        case    'N':    // ALT + N
            if( c->nextl && !(c->nextl->flg&c_f_fict))
                {
                currcell = *c->nextl;
                LDPUMA_CSTR_SetPosition(snap_cell2pos(c->nextl));
                ret=0;
                }
            else
                RUS_Console("Нет следующего");
            break;
        case    'P':    // ALT + P
            if( c->prevl && !(c->prevl->flg&c_f_fict))
                {
                currcell = *c->prevl;
                LDPUMA_CSTR_SetPosition(snap_cell2pos(c->prevl));
                ret=0;
                }
            else
                RUS_Console("Нет предыдущего");
            break;
        case    VK_F2:  // ALT + F2
            db_skip_client=1;
            ret=-1;
            break;
        case    VK_F8:  // ALT + F8
            if( r )
                {
                Word8 let;
                CSTR_GetImage(r,(Word8*)(&rRaster),CSTR_TYPE_IMAGE_RS);
                let = (Word8)(c->nvers? c->vers[0].let:'~');
                if( decode_ASCII_to_[let][1]==0 )
                    let = decode_ASCII_to_[let][0];
                CSTR_GetAttr(r,&attr);
                snap_AddRecRaster("SQPROT", &rRaster,let,attr.font);
                RUS_Console("Отписано в SQPROT.CTB");
                }
            break;
        default:
            ret=0;
            break;
        }
        break;

    // KEYS
        case WM_KEYDOWN:

                switch((int)wParam)
                {
        case    VK_ESCAPE:
            snap_page_disable = TRUE;
            RUS_Console("распознай до конца");
            ret=-1;
            break;
        case    VK_DELETE:
            del_cell(c);
            //ret=-1;
            break;
        case VK_SHIFT          :
            shift=1;
            break;

        case VK_F1:
            if( shift )
                {
                shift=0;
                snap_sticks(c,str);
                Snap_Console(str);
                RUS_Console("анализ батонов");
                }
            else
                {   // F1 : help
                RUS_Console("ESC - до конца без снэпа");

                RUS_Console("F1  - справка");
                RUS_Console("F2  - новая строка");
                RUS_Console("F3  - встать здесь");
                RUS_Console("F4  - просмотр кластеров на 2-м проходе");
                RUS_Console("F5  - просмотр склейки/разрезания кластерами на 2-м проходе");
                RUS_Console("F7  - в отладчик");
                RUS_Console("F9  - отладочное распознавание (в т.ч. на 2-м проходе)");
                RUS_Console("F11 - задать трассировку");
                RUS_Console("F12 - вывести адреса структур");

                RUS_Console("Shift+F1  - анализ батонов");
                RUS_Console("Shift+F2  - анализ стика");
                RUS_Console("Shift+F3  - обрить");
                RUS_Console("Shift+F4  - анализ слов на 2-м проходе");
                RUS_Console("Shift+F5  - выпрямить и обрить");
                RUS_Console("Shift+F6  - снять выпярмление");
                RUS_Console("Shift+F7  - выпрямить на брея");
                RUS_Console("Shift+F9  - на cледующую склейку");
                RUS_Console("Shift+F11 - снять трассировку");

                RUS_Console("Alt+F2 - следующий этап");
                RUS_Console("Alt+F8 - запомнить образ в CTBase");
                RUS_Console("Alt+F  - FRH для агрессивного");
                RUS_Console("Alt+N  - следующий символ");
                RUS_Console("Alt+P  - предыдущий символ");
                RUS_Console("Alt+S  - словари");
                RUS_Console("Alt+X  - прервать");

                RUS_Console("2(3,4...)- показать 2-ю(3,4...) строку");
                RUS_Console("Home/End - начало/конец строки");
                RUS_Console("+/- - увеличить/уменьшить шрифт");

                            LDPUMA_RasterText("F1 - подсказка. см. консоль.");
                }
                        break;
        case VK_F2:
            if( shift )
                {   // SHIFT + F2 : stick study
                shift=0;
                if( !stick_center_study(c,NULL,0) )
                    {
                    END_RUN_CELL("анализ стика");
                    }
                }
            else
                {   // F2 : goto next line
                snap_disable = TRUE;
                ret=-1;
                }
            break;
        case    VK_F3:
            if( shift )
                {   // SHIFT + F3 : shaving
                shift=0;
                pimpleshave(c, 1, 1);
                END_RUN_CELL("удаление прыщей");
                }
            else
                {   // F3 : stop here
#ifdef _USE_PASS2_
                if(p2_active) // Nick
                    {
                    if(c)
                        p2_SetStopBound(c->col);
                    else if(r)
                    {
                        CSTR_rast_attr at1;
                        CSTR_GetAttr(r,&at1);
                        p2_SetStopBound(at1.col);
                    }
                    stopcell=NULL;
                    }
                else
#endif
                    stopcell = c;
                ret=-1;
                RUS_Console("точка остановки");
                }
            break;
        case VK_F4:
            {   // start clustres viewer
            if( p2_active )
                {
#ifdef  _USE_DFON_
                if(shift)
                    {
                    shift=0;
                    p2_SetShowWords(TRUE);
                    // as F2 : goto next line
                    snap_disable = TRUE;
                    ret=-1;
                    break;
                    }

                CSTR_GetImage(r,(Word8*)(&rRaster),CSTR_TYPE_IMAGE_RS);
                // надо повернуть ?
                p2_rotate(&rRaster);
                DFon_SendRaster( &rRaster, hwnd, (Word8)(c&&c->nvers?c->vers[0].let:'~'));
#else
                RUS_Console("verrions without DFON.DLL");
#endif
                }
            else
                RUS_Console("дождитесь 2-го прохода");
            }
            break;
        case VK_F5:
            if( shift )
                {   // SHIFT + F5 : erection
                shift=0;
                if( !(c->pos_inc&erect_rot) )
                    {
                    erector(c, 0, 1, 0,1);
                    END_RUN_CELL("выпрямление с бритьём");
                    }
                else
                    RUS_Console("уже выпрямлено");
                }
            else
                {   // F5
#ifdef _USE_PASS2_
                FONInitSnap(hwnd);
#endif
                }
            break;
        case VK_F6:
            if( shift )
                {   // SHIFT + F6 : erection
                shift=0;
                if( (c->pos_inc&erect_rot) )
                    {
                    erect_cell_value(c, (INT)(-c->stick_inc), 0, FALSE);
                    c->stick_inc=NO_INCLINE;
                    c->pos_inc=erect_no;
                    END_RUN_CELL("уничтожение выпрямления");
                    }
                else
                    RUS_Console("ещё не выпрямлено");
                }
            else
                {   // F6
                }
            break;
        case VK_F7:
            if( shift )
                {   // SHIFT + F7 : erection
                shift=0;
                if( !(c->pos_inc&erect_rot) )
                    {
                    erector(c, 0, 0, 0,1);
                    END_RUN_CELL("выпрямление без бритья");
                    }
                else
                    RUS_Console("уже выпрямлено");
                }
            else
                {   // F7 : debug
#if defined (_MSC_VER)
                __asm  int 03h;
#else
                abort();
#endif
                }
            break;
        case    VK_F9:
            if( shift )
                {
                shift=0;
                db_status=0;
                flag_ShiftF9=1;
                RUS_Console("на следующую склейку");
                ret=-1;
                }
            else
                {
#ifdef _USE_PASS2_
                if( p2_active )
                    {
                    RecVersions vers;                                                                                                                                   char rere[256],rerew[256];
                    int i;

                    CSTR_GetImage(r,(Word8*)(&rRaster),CSTR_TYPE_IMAGE_RS);
                    p2_recog(&rRaster,&vers,NULL,FALSE);
                    rere[0]=0;
                    for(i=0;i<vers.lnAltCnt;i++)
                        sprintf(rere+strlen(rere),"%c(%d)",vers.Alt[i].Code,vers.Alt[i].Prob);
                    ASCII2ANSI(rere,rerew);
                    sprintf(buf,"pass2 : %s",rerew);
                    RUS_Console(buf);
                    }
                 else
#endif
                 if(c)
                    {
                    snap_recogEVN(r,c);
                    //show_prop(c);
                    snap_recog3X5(c);
                    }
                 RUS_Console("распознавание");
                 }
            break;
        case    VK_F10:
            if( shift )
                {
                shift=0;
                }
            else
                {
                snap_page_disable = TRUE;
                RUS_Console("распознай до конца");
                ret=-1;
                }
            break;

        case    VK_F11:
            if( !shift )
                {   // F11 : set tracing
                db_trace_flag=6;
                RUS_Console("установлена трассировка");
                }
            else
                {   // F11 : clear tracing
                db_trace_flag=0;
                RUS_Console("снятa трассировкa");
                }
            break;
        case VK_F12:    // F12 : show positions
                        sprintf(buf,"Позиция %i из %i",pos,LDPUMA_CSTR_GetLength());
            RUS_Console(buf);
            sprintf(buf,"растр CSTR=0x%x(cell=0x%x,env=0x%x,prevl=0x%x,nextl=0x%x,prev=0x%x,next=0x%x)",
                r, c,c?c->env:0,c?c->prevl:0,c?c->nextl:0,c?c->prev:0,c?c->next:0);
            RUS_Console(buf);
                        LDPUMA_RasterText("F12 - позиция. см. консоль.");
                        break;
        case    '1':
            select_line=0;
            snap_continue=TRUE;
            ret=-1;
            break;
        case    '2':
            if( num_select_lines>1 )
                select_line=1;
            snap_continue=TRUE;
            ret=-1;
            break;
        case    '3':
            if( num_select_lines>2 )
                select_line=2;
            snap_continue=TRUE;
            ret=-1;
            break;
        case    '4':
            if( num_select_lines>3 )
                select_line=3;
            snap_continue=TRUE;
            ret=-1;
            break;
        case    '5':
            if( num_select_lines>4 )
                select_line=4;
            snap_continue=TRUE;
            ret=-1;
            break;
                }
                break;
        }

return (Word32)(ret);
}

// режим мониторинга строки и ожидания клавиши
static Int32 snap_monitor_calls=0;
BOOL snap_monitor(void)
{
Handle hnd;
        int i,scale;
    CSTR_rast rst, snap_rst=(CSTR_rast)NULL;
    CSTR_rast_attr attr;
if( snap_monitor_calls )
    return FALSE;
if( snap_disable || snap_page_disable || db_skip_client )
    return FALSE;
#ifdef _USE_PASS2
if(p2_stopPlace())      // Nick
    return FALSE;
#endif
if( stopcell && stopcell->col>currcell.col )
    return FALSE;
else
    stopcell = NULL;
if( !db_pass )
    return FALSE;
num_select_lines = 0;
hnd = (curr_pass==-1||curr_pass>=snap_clients) ? hSnapMain : hSnapPass[curr_pass];
if( LDPUMA_SkipEx(hnd,TRUE,TRUE,1) )
    return FALSE;
snap_line  = CSTR_NewLine(999999, 0, -1);
if( !snap_line )
    return FALSE;
Cells2CSTR((CSTR_line)NULL,snap_line,(cell *)(currcell.complist),1);
rst=CSTR_GetFirstRaster(snap_line);
scale = currcell.env ? currcell.env->scale : 0;
stop_number=0;
for(i=0,rst=CSTR_GetNext(rst);rst;rst=CSTR_GetNext(rst),i++)
    {
    CSTR_GetAttr(rst,&attr);

    if( (attr.col==(currcell.col<<scale) &&
        attr.row==(currcell.row<<scale) &&
        attr.w  ==(currcell.w  <<scale) &&
        attr.h  ==(currcell.h  <<scale))

    || (attr.flg_new & CSTR_fn_stop)  )        //Paul 12-11-99
        {
        snap_rst = rst;
        stop_number=i;
        break;
        }
    }
if( stop_number>i )
    stop_number=0;

//
snap_monitor_calls++;
stop_number = LDPUMA_CSTR_Monitor(hnd,snap_line,stop_number,myMonitorProc);
snap_monitor_calls--;
//
CSTR_DeleteLine(snap_line);
if( exit_enable==TRUE )
    {
    exit_enable=FALSE;
    ErrorExit(RSTR_ERR_USERBREAK);
    }
return TRUE;
}

BOOL snap_monitor_ori(CSTR_line *snap_line, Int32 num_lines)
{
Handle hnd;
        int i,scale;
    CSTR_rast rst, snap_rst=(CSTR_rast)NULL;
    CSTR_rast_attr attr;
if( snap_disable || snap_page_disable || db_skip_client )
    return FALSE;
#ifdef _USE_PASS2
if(p2_stopPlace())      // Nick
    return FALSE;
#endif
if( stopcell && stopcell->col>currcell.col )
    return FALSE;
else
    stopcell = NULL;
if( !db_pass )
    return FALSE;
hnd = (curr_pass==-1||curr_pass>=snap_clients) ? hSnapMain : hSnapPass[curr_pass];
num_select_lines = num_lines;
select_line      = 0;
rst=CSTR_GetFirstRaster(snap_line[select_line]);
scale = currcell.env ? currcell.env->scale : 0;
for(i=0,rst=CSTR_GetNext(rst);rst;rst=CSTR_GetNext(rst),i++)
    {
    CSTR_GetAttr(rst,&attr);
    if( attr.col==(currcell.col<<scale) &&
        attr.row==(currcell.row<<scale) &&
        attr.w  ==(currcell.w  <<scale) &&
        attr.h  ==(currcell.h  <<scale))
        {
        snap_rst = rst;
                stop_number=i;
                break;
        }
    }
if( stop_number>=i )
    stop_number=0;
//

new_snap:snap_continue=FALSE;
stop_number = LDPUMA_CSTR_Monitor(hnd,snap_line[select_line],stop_number,myMonitorProc/*Ori*/);
if( snap_continue )
    {
    stop_number=0;
    goto new_snap;
    }
//
num_select_lines = 0;
if( exit_enable==TRUE )
    {
    exit_enable=FALSE;
    ErrorExit(RSTR_ERR_USERBREAK);
    }
return TRUE;
}


// вывести текст
BOOL snap_show_text(BYTE *txt)
{
char text[256],texto[80*40];
if( snap_disable
#ifdef _USE_PASS2
&& !p2_GetShowWords()
#endif
    || snap_page_disable || db_skip_client )
    return FALSE;
#ifdef _USE_PASS2
if(p2_stopPlace())      // Nick
    return FALSE;
#endif
if( stopcell && stopcell->col>currcell.col )
    return FALSE;

if( !(*txt) )
    {
    LDPUMA_RasterText("");
    RUS_Console("Empty message");
    return TRUE;
    }
sprintf(texto,"line %d,%s, pass %d, language %d, lines %d\n",
    line_number,
    (curr_pass>=snap_clients||curr_pass<0)?"":snap_pass[curr_pass],
    p2_active+1, language,
    num_select_lines?num_select_lines:1);
while(1)
{

text[0]='\0';
for(;*txt && *txt!='\n';txt++)
    {
    if( strlen(text)+strlen(decode_ASCII_to_[(BYTE)*txt])>256 )
           break;
    strcat(text, decode_ASCII_to_[(BYTE)*txt] );
    strcat(texto, decode_ASCII_to_[(BYTE)*txt] );
    }
if( !text[0] )
    break;
Snap_Console(text);
if( *txt=='\n' )
    {
    strcat(texto,"\n");
    *txt++;
    }
}
LDPUMA_RasterText(texto);
return TRUE;
}

BOOL    snap_is_marked(CSTR_line ln)
{
Point16     p;
CSTR_attr   r;
Handle wnd = LDPUMA_GetWindowHandle("Main");

if( !wnd )
    return FALSE;
CSTR_GetLineAttr(ln,&r);
if( !LDPUMA_GetUserPoint(wnd,&p) )
    return FALSE;
return !( (p.x < r.r_col)||(p.x > r.r_col+r.r_wid )||
          (p.y < r.r_row) ||(p.y > r.r_row+r.r_hei)
          );
}

// активен ли клиент
BOOL snap_activity(BYTE a)
{
Bool32 ret;
if( snap_disable || snap_page_disable || db_skip_client || (a-'a')>=snap_clients)
    return FALSE;
curr_pass=a-'a';
if( !internal_skip[curr_pass] )
    return TRUE;
if( LDPUMA_SkipEx(hSnapMain,TRUE,TRUE,1))
    return FALSE;
ret = LDPUMA_SkipEx(hSnapPass[a-'a'],FALSE,TRUE,1);

return(!ret);
}

// активен ли хотя бы один клиент
BOOL snap_is_active(void)
{
int i;
snap_disable = FALSE;
if( LDPUMA_SkipEx(hSnapMain,TRUE,TRUE,1))
    return FALSE;
for(i=0;i<snap_clients;i++)
    {
    if( !LDPUMA_SkipEx(hSnapPass[i],FALSE/*TRUE*/,TRUE,1) )
        {
        db_status=1;
        return TRUE;
        }
    }
db_status=0;
return FALSE;
}
// встать на клетке
BOOL snap_newcell (cell *c)
{
if( snap_disable || snap_page_disable || db_skip_client )
    return FALSE;
currcell = *c;
currcell.complist = (c_comp *)c;
return TRUE;
}

static void snap_align8_lines(Word8 *bin,Int32 w, Int32 h)
{
int i,ii,iii, wb=(w+7)/8, wb_new=((w+63)/64)*8;
Word8   buf[256];

memset(buf,0,wb_new);
for(iii=(h-1)*wb_new,ii=(h-1)*wb,i=0;i<h;i++,ii-=wb,iii-=wb_new)
    {
    memcpy(buf,&bin[ii],wb);
    memcpy(&bin[iii],buf,wb_new);
    }

return;
}

// вывести растр
BOOL snap_show_raster(PBYTE raster, INT height, INT width)
{
DPUMA_RecRaster rs={0};

if( snap_disable || snap_page_disable || db_skip_client )
    return FALSE;
#ifdef _USE_PASS2
if(p2_stopPlace())      // Nick
    return FALSE;
#endif
if( stopcell && stopcell->col>currcell.col )
    return FALSE;

    cut_enable=FALSE;
    rs.lnPixWidth  = width;
    rs.lnPixHeight = height;
    rs.lnRasterBufSize = REC_MAX_RASTER_SIZE;
    memcpy( rs.Raster, raster, ((width+7)/8)*height);
    snap_align8_lines(rs.Raster, rs.lnPixWidth, rs.lnPixHeight);

    LDPUMA_DrawRaster(&rs);

return TRUE;
}

// + остановиться, если между B,E
// can be call after setup stopcell (F3)
BOOL snap_stopcell(cell *B,cell *E)
{
if( snap_disable || snap_page_disable || db_skip_client || !stopcell)
    return FALSE;
return(stopcell->col>=B->col && stopcell->col<=E->col);
}

cell*snap_get_stopcell(void)
{
return stopcell;
}

// снэп из разрезания.
void snap_show_cuts(cell *C, struct cut_elm *cut_points)
{
struct cut_elm  *cpnt;

my_cut_points=cut_points;

  if( snap_disable || snap_page_disable || db_skip_client )
    return ;
  if (db_status && snap_activity('a'))
   {
    flag_cut_point=0;
    snap_newcell(C);
    cpnt = my_cut_points;
    cut_enable=TRUE;
    for(numpoints_show=0,cpnt++;cpnt->x != 127;cpnt++, numpoints_show+=2)
        {
        if( cpnt->dh==0 )
            {
            cutpoints_show[numpoints_show].y  =0;
            cutpoints_show[numpoints_show+1].y=C->h+1;
            }
        else
            {
            cutpoints_show[numpoints_show].y=C->h-cpnt->h+1;
            cutpoints_show[numpoints_show+1].y=cutpoints_show[numpoints_show].y+(cpnt->dh?cpnt->dh:C->h);
            cutpoints_show[numpoints_show+1].y=MIN(C->h,cutpoints_show[numpoints_show+1].y);
            }
        cutpoints_show[numpoints_show+1].y--;
        if( !cuts_point_methode )
            {
            cutpoints_show[numpoints_show].y  --;
            cutpoints_show[numpoints_show+1].y++;
            }
        cutpoints_show[numpoints_show].x=cutpoints_show[numpoints_show+1].x=cpnt->x;
        }
    numpoints_show-=2;
    show_cut_points=1;
    cutpoints_cell=C;
    snap_monitor();
    cutpoints_cell=NULL;
    show_cut_points=0;
   }
return ;
}

// начало нового шага
BOOL snap_newpass(BYTE pass)
{
if( snap_disable || snap_page_disable  )
    return FALSE;
cutpoints_cell=NULL;
curr_pass=pass-'a';
stop_number=0;
db_pass=pass;
db_skip_client=0;
flag_ShiftF9=0;
return TRUE;
}

// задание остановки в точках разрезани
void snap_stop_cuts(void)
{
if( snap_disable || snap_page_disable || db_skip_client )
    return ;
if(flag_cut_point)
      { flag_cut_point=0;  db_status=1; }
if(flag_ShiftF9)
    {flag_ShiftF9=0; db_status=1;}
return;
}

// флагов снэпа для перехода к следующей склейке
void snap_newcut(void)
{
if( snap_disable || snap_page_disable || db_skip_client )
    return ;
if(flag_ShiftF9)
    {flag_ShiftF9=0; db_status=1;}
return;
}

// точка остановки в разрезании
void snap_newpoint(INT i)
{
if( snap_disable || snap_page_disable || db_skip_client )
    return ;
 if(flag_cut_point)
    if((my_cut_points+i)->x == StopPoint)
      { flag_cut_point=0;  db_status=1; }
return;
}


// knot for Alik debug. Absent pass 'j'
BOOL Alik_snap_show_raster(PBYTE raster,PBYTE raster1,PBYTE buf,INT height,
                           INT width,PCHAR product,PCHAR product_two,
                           PINT penalty)

{
return FALSE;
}

// knot for EVENT snap
void snap_keep(BYTE user, PBYTE addr, WORD lth)
{
if( snap_disable || snap_page_disable || db_skip_client)
    return ;
return;
}

// knot for DOS-init
BOOL snap_newline(void)
{
if( snap_disable && db_skip_client )
    return FALSE;
return TRUE;
}

//IGOR
BOOL snap_baselines(BYTE a)
{
	if(!hSnapLineBL[a - 'a'])
		return FALSE;

	return !LDPUMA_Skip(hSnapLineBL[a - 'a']);
}

void snap_draw_line(Handle wnd, Point16 *start, Point16 *end, Int32 skew, Word32 rgb, Int16 pen, Word32 key)//IGOR
{
	LDPUMA_DrawLine(wnd,start, end, skew, rgb, pen, key);
	return;
}

void snap_del_line(Handle wnd, Word32 key)
{
	LDPUMA_DeleteLines(wnd, key);
	return;
}
//IGOR
