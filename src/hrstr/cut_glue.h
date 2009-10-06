/*
Copyright (c) 1993-2008, Cognitive Technologies
All rights reserved.

Ðàçðåøàåòñÿ ïîâòîðíîå ðàñïðîñòðàíåíèå è èñïîëüçîâàíèå êàê â âèäå èñõîäíîãî êîäà,
òàê è â äâîè÷íîé ôîðìå, ñ èçìåíåíèÿìè èëè áåç, ïðè ñîáëþäåíèè ñëåäóþùèõ óñëîâèé:

      * Ïðè ïîâòîðíîì ðàñïðîñòðàíåíèè èñõîäíîãî êîäà äîëæíû îñòàâàòüñÿ óêàçàííîå
        âûøå óâåäîìëåíèå îá àâòîðñêîì ïðàâå, ýòîò ñïèñîê óñëîâèé è ïîñëåäóþùèé
        îòêàç îò ãàðàíòèé.
      * Ïðè ïîâòîðíîì ðàñïðîñòðàíåíèè äâîè÷íîãî êîäà â äîêóìåíòàöèè è/èëè â
        äðóãèõ ìàòåðèàëàõ, ïîñòàâëÿåìûõ ïðè ðàñïðîñòðàíåíèè, äîëæíû ñîõðàíÿòüñÿ
        óêàçàííàÿ âûøå èíôîðìàöèÿ îá àâòîðñêîì ïðàâå, ýòîò ñïèñîê óñëîâèé è
        ïîñëåäóþùèé îòêàç îò ãàðàíòèé.
      * Íè íàçâàíèå Cognitive Technologies, íè èìåíà åå ñîòðóäíèêîâ íå ìîãóò
        áûòü èñïîëüçîâàíû â êà÷åñòâå ñðåäñòâà ïîääåðæêè è/èëè ïðîäâèæåíèÿ
        ïðîäóêòîâ, îñíîâàííûõ íà ýòîì ÏÎ, áåç ïðåäâàðèòåëüíîãî ïèñüìåííîãî
        ðàçðåøåíèÿ.

ÝÒÀ ÏÐÎÃÐÀÌÌÀ ÏÐÅÄÎÑÒÀÂËÅÍÀ ÂËÀÄÅËÜÖÀÌÈ ÀÂÒÎÐÑÊÈÕ ÏÐÀÂ È/ÈËÈ ÄÐÓÃÈÌÈ ËÈÖÀÌÈ "ÊÀÊ
ÎÍÀ ÅÑÒÜ" ÁÅÇ ÊÀÊÎÃÎ-ËÈÁÎ ÂÈÄÀ ÃÀÐÀÍÒÈÉ, ÂÛÐÀÆÅÍÍÛÕ ßÂÍÎ ÈËÈ ÏÎÄÐÀÇÓÌÅÂÀÅÌÛÕ,
ÂÊËÞ×Àß ÃÀÐÀÍÒÈÈ ÊÎÌÌÅÐ×ÅÑÊÎÉ ÖÅÍÍÎÑÒÈ È ÏÐÈÃÎÄÍÎÑÒÈ ÄËß ÊÎÍÊÐÅÒÍÎÉ ÖÅËÈ, ÍÎ ÍÅ
ÎÃÐÀÍÈ×ÈÂÀßÑÜ ÈÌÈ. ÍÈ ÂËÀÄÅËÅÖ ÀÂÒÎÐÑÊÈÕ ÏÐÀÂ È ÍÈ ÎÄÍÎ ÄÐÓÃÎÅ ËÈÖÎ, ÊÎÒÎÐÎÅ
ÌÎÆÅÒ ÈÇÌÅÍßÒÜ È/ÈËÈ ÏÎÂÒÎÐÍÎ ÐÀÑÏÐÎÑÒÐÀÍßÒÜ ÏÐÎÃÐÀÌÌÓ, ÍÈ Â ÊÎÅÌ ÑËÓ×ÀÅ ÍÅ
ÍÅÑ¨Ò ÎÒÂÅÒÑÒÂÅÍÍÎÑÒÈ, ÂÊËÞ×Àß ËÞÁÛÅ ÎÁÙÈÅ, ÑËÓ×ÀÉÍÛÅ, ÑÏÅÖÈÀËÜÍÛÅ ÈËÈ
ÏÎÑËÅÄÎÂÀÂØÈÅ ÓÁÛÒÊÈ, ÑÂßÇÀÍÍÛÅ Ñ ÈÑÏÎËÜÇÎÂÀÍÈÅÌ ÈËÈ ÏÎÍÅÑÅÍÍÛÅ ÂÑËÅÄÑÒÂÈÅ
ÍÅÂÎÇÌÎÆÍÎÑÒÈ ÈÑÏÎËÜÇÎÂÀÍÈß ÏÐÎÃÐÀÌÌÛ (ÂÊËÞ×Àß ÏÎÒÅÐÈ ÄÀÍÍÛÕ, ÈËÈ ÄÀÍÍÛÅ,
ÑÒÀÂØÈÅ ÍÅÃÎÄÍÛÌÈ, ÈËÈ ÓÁÛÒÊÈ È/ÈËÈ ÏÎÒÅÐÈ ÄÎÕÎÄÎÂ, ÏÎÍÅÑÅÍÍÛÅ ÈÇ-ÇÀ ÄÅÉÑÒÂÈÉ
ÒÐÅÒÜÈÕ ËÈÖ È/ÈËÈ ÎÒÊÀÇÀ ÏÐÎÃÐÀÌÌÛ ÐÀÁÎÒÀÒÜ ÑÎÂÌÅÑÒÍÎ Ñ ÄÐÓÃÈÌÈ ÏÐÎÃÐÀÌÌÀÌÈ,
ÍÎ ÍÅ ÎÃÐÀÍÈ×ÈÂÀßÑÜ ÝÒÈÌÈ ÑËÓ×ÀßÌÈ), ÍÎ ÍÅ ÎÃÐÀÍÈ×ÈÂÀßÑÜ ÈÌÈ, ÄÀÆÅ ÅÑËÈ ÒÀÊÎÉ
ÂËÀÄÅËÅÖ ÈËÈ ÄÐÓÃÎÅ ËÈÖÎ ÁÛËÈ ÈÇÂÅÙÅÍÛ Î ÂÎÇÌÎÆÍÎÑÒÈ ÒÀÊÈÕ ÓÁÛÒÊÎÂ È ÏÎÒÅÐÜ.

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

//#define  MY_DEBUG

#define  MAXINT   32767     //¬ ªá. æ¥«®¥ ç¨á«®
#define  MININT  -32767     //¬¨­.  æ¥«®¥ ç¨á«®
#define  MAX_COL  10000     //¬ ªá. §­ ç¥­¨¥ áâ®«¡æ 
#define  MAX_CUTS  63       //¬ ªá. ç¨á«® á¥ç¥­¨©
#define  MAX_SECT  MAX_CUTS //¬ ªá. ç¨á«® cell'®¢ ¯®á«¥ à §à¥§ ­¨ï
#define  RASTER_WIDTH   128 //à §¬¥àë à áâà 
#define  RASTER_HEIGHT   64
#define  MAX_RO   254  //¬ ªá. ¢¥à®ïâ­®áâì
#define  MAX_SEG_VERS  256

#define  NOT_CUT  128  //ä« £: § ¯à¥â¨âì à §à¥§

#define  united     1  //ª®¬¯®­¥­âë á¥ªæ¨¨ ®¡ê¥¤¨­ï«¨áì
#define  glued      2  //ª®¬¯®­¥­âë á¥ªæ¨¨ áª«¥¨¢ «¨áì
#define  up_dust    4  //¨á¯®«ì§®¢ ­ë ¢¥àå­¨¥ dust'ë
#define  low_dust   8  //­¨¦­¨¥
#define  all  up_dust+low_dust   //¢á¥ ª®¬¯®­¥­âë ¨á¯®«ì§®¢ ­ë
#define  gl_sticks 16  //áª«¥¨¢ îâáï ¯ «ª¨

//---------------------  types   --------------------------------------

struct seg_vers_struct   //¢¥àá¨¨ á¥£¬¥­â 
{
  struct seg_vers_struct *next; //á«¥¤ãîé ï ¢¥àá¨ï
  int16_t  px;      //«¥¢®¥ á¥ç¥­¨¥ á¥£¬¥­â 
  char gvar;   //¢ à¨ ­â áª«¥©ª¨
  int16_t  ro;      //¬¥à 
  int16_t width;    //è¨à¨­ 
  SVERS vers;  //¢¥àá¨¨
};
typedef struct seg_vers_struct seg_vers;

struct dp_vers_struct         //ªãç  ¢¥àá¨© dp
{
  seg_vers pool[5*MAX_SEG_VERS];  //¢¥àá¨¨
  seg_vers *node[MAX_SEG_VERS];   //ãª § â¥«ì ­  ¢¥àá¨¨ ¤«ï ª ¦¤®£® á¥ç¥­¨ï
};

struct raster_struct   //à áâà cell' 
{
  int16_t w;       //è¨à¨­ 
  int16_t h;       //¢ëá®â 
  int16_t top;     //áâà®ª  «¥¢®£® ¢¥àå­¥£® ã£« 
  int16_t left;    //áâ®«¡¥æ  -""-
  uchar pict[RASTER_WIDTH*RASTER_HEIGHT/8];  //à áâà
};
typedef struct raster_struct raster;

//-------------------  variables  -------------------------------------

 extern uchar db_status;  // snap presence byte
 extern uchar db_trace_flag;  // 2 - more detailed estimate (ALT-F7)

//cg_main.c
 extern uchar sticks_left_to_bad[];
 extern uchar letters_left_to_bad[];
 extern char *results_left_to_bad[];
 extern uchar prob_left_to_bad[];
 extern uchar sticks_right_to_bad[];
 extern uchar letters_right_to_bad[];
 extern char *results_right_to_bad[];
 extern uchar prob_right_to_bad[];

 extern B_LINES my_bases; //¡ §®¢ë¥ «¨­¨¨
 extern int16_t        blank; //è¨à¨­  ¯à®¡¥« 
 extern int16_t minw;         //¬¨­¨¬ «ì­ ï è¨à¨­  ¡ãª¢ë
 extern int16_t minp;         //¬¨­¨¬ «ì­ ï è¨à¨­  â®çª¨
 extern int16_t ps3;          // 1/3 ¢ëá®âë áâà®ç­®© ¡ãª¢ë
 extern int16_t sym_dist;     //áà¥¤­¥¥ à ááâ®ï­¨¥ ¬¥¦¤ã á¨¬¢®« ¬¨
 extern int16_t sym_width;    //áà¥¤­ïï è¨à¨­  á¨¬¢®« 
 extern int16_t cut_width;    //¯à¨ è¨à¨­¥ > cut_width ¬®¦­® à¥§ âì

 extern char snap_text[],*snap;

 extern uchar trs2;             // >trs2 - ¡ãª¢  å®à®è ï ¢® ¢á¥å ®â­®è¥­¨ïå
 extern uchar trg;              //¯®à®£ ¤«ï áª«¥¨¢ ­¨ï

//---------------------  macros  --------------------------------------

#define let(x)  ( (x)->flg & c_f_let )
#define bad(x)  ( (x)->flg & c_f_bad )
#define dust(x) ( (x)->flg & c_f_dust )
#define fict(x) ( (x)->flg & c_f_fict )
#define cut(x)  ( (x)->cg_flag & c_cg_cutl+c_cg_cutr )
#define just(x) ( (x)->cg_flag & c_cg_just )
#define composed(x)    ( (x)->cg_flag & c_cg_comp )
#define let_or_fict(x) ( (x)->flg & ( c_f_let | c_f_fict ) )
#define let_or_bad(x)  ( (x)->flg & ( c_f_let | c_f_bad  ) )
#define complete_recog(x) ( (x)->recsource & ( c_rs_BOX | c_rs_bitcmp ) )

#define set_dust(x) ( (x)->flg = c_f_dust )
#define set_bad(x)  ( (x)->flg = c_f_bad  )
#define set_let(x)  ( (x)->flg = c_f_let  )
#define set_just(x) ( (x)->cg_flag |= c_cg_just )
#define let_to_bad(x) ( (x)->flg = ( (x)->flg & ~c_f_let | c_f_bad ) )
#define set_noglue(x) ((x)->cg_flag |= c_cg_noglue)

#define wide(C)    ( (C)->w > ((C)->h<<1) + ((C)->h>>2) )
#define middle(C)  ( (C)->r_col -1 + (((C)->w+1)>>1) )
#define lefter(C,x)  ( middle(C) < (x) )

#define ben_cut(x)    ( (x)->var & NOT_CUT )
#define point_fit(x,p)                    \
        ( p==1 && ((x)->var & 0x5E)==0 || \
          p==2 && ((x)->var & 0x78)==0 || \
          p==3 && ((x)->var & 0x70)==0 || \
          p==4 )
#define in_set(var,set)                   \
        ( set==0 && ((var) & 0x7F)!=40 || \
          set==1 && ((var) & 0x5E)==0  || \
          set==2 && ((var) & 0x7B)==0  || \
          set==3 && ((var) & 0x77)==0  || \
          set==4 && ((var) & 0x7F)==40 )
/*
#define point_fit(x,p)                       \
        ( p==1 && ((x)->var & 8) == 0     || \
          p==2 && ((x)->var & 0x7F) != 40 || \
          p==3 )
#define in_set(var,set)                           \
        ( set==0 && ((var &  8)==0 || var==40) || \
          set==1 &&  (var &  8)==0 ||             \
          set==2 &&  (var & 40)==8 ||             \
          set==3 &&  (var & 0x7F)==40 )
*/

#define may_glue(x) \
  (!( (x)->cg_flag & c_cg_noglue || (x)->flg & (c_f_solid | c_f_confirmed \
     | c_f_detouch) ))
#define debug_on ( db_status && snap_activity('a') )
#define det_trace (db_trace_flag & 8)

//------------------  functions  --------------------------------------

//cg_smcut.c
void dp_pass0(cell *LC, raster *r, struct cut_elm *cut_list,
             seg_vers **vers_list, int16_t ncut);

//cg_main.c
cell *process_word (cell *WB, cell *WE);
uchar addij(cell *C, raster *r0, struct cut_elm *cut_list,
           seg_vers **vers_list, int16_t ncut, int16_t i1, int16_t i0, char mode);
void dp_bound(struct cut_elm *cut_list,seg_vers **vers_list,
              int16_t pass, int16_t *ib, int16_t *ie);
int16_t full_recog (cell *B1, s_glue *gl0, int16_t trs, int16_t tol);
cell * create_my_cell(MN * mn, cell * ci, char bdiff, char dflag);

//cg_tools.c
cell *col_to_one(cell **clist, int16_t n);
cell *comp_to_cell(cell *C, c_comp **list, int16_t N, char bdiff, uchar dflag);
Bool glue_overlap(cell *LC, cell *E);
seg_vers *find_vers( int16_t i1, int16_t i0, seg_vers **vers_list );
uchar not_connect_sect(int16_t i1, int16_t i0, struct cut_elm *cut_list);
int16_t on_path(int16_t i, int16_t ie, struct cut_elm *cut_list);
seg_vers *store_vers(seg_vers *cur_vers, seg_vers **vers_list,
         int16_t i1, int16_t i0, SVERS *vers, int16_t ro, int16_t width, char gvar);
void adjust_3x5(Bool prerecog);

void cg_show_rast(cell *C, raster *r, char *msg,
               struct cut_elm *cut_list);
void cg_show_list(cell **cells, int16_t N, uchar *msg);
char *show_dp( puchar s, struct cut_elm *cut_list, int16_t i0);
void det_snap(cell *C, char *txt);
void show_and_wait(char *txt);

//cg_hcut.c
void  base_lines_cut();
