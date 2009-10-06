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


#include "context.h"
#include "linutil.h"
#include "func.h"
#include "tuner.h"
#include "status.h"
#include "linear.h"

int16_t final_crit_russian(cell * c);
int16_t russian_context_process(cell * c);

static int16_t help_spelling_checker(cell * c);

int16_t russian_context_process(cell * c)
{
int16_t return_code, nv;

  return_code = NO_ACTION;
if (n_ltr == 1 )             /** single letter case: **/
  {
  nv = get_nvers(c, (uchar)'Ž');
  if( nv>0 && memchr("0",c->vers[0].let,1) &&
      ((c->prev->flg&c_f_let)||
       (c->next->flg&c_f_let)||
       (c->next->nvers>0&&memchr(".,",c->next->vers[0].let,2)) ) )
     {
     vers_to_first_place ( c,get_nvers(c,(uchar)'Ž') ); // make it first
     sort_vers(c);
     return_code = CONTINUE_ACTION;
     }

  nv = get_nvers(c, (uchar)'3');
  if( nv>0 && memchr("§‡",c->vers[0].let,2) )
     {
     vers_to_first_place ( c,get_nvers(c,(uchar)'3') ); // make it first
     sort_vers(c);
     return_code = CONTINUE_ACTION;
     }

   nv = get_nvers(c, (uchar)'¨');
   if(((c->font|c->font_new) & c_fp_it) || (c->cg_flag & c_cg_comp))
    if(  nv > 0 && get_nvers(c,(uchar)'ï')!=0 ) // Oleg : 12-07-1994 : 'Ÿ'in first
    {
    if(c->vers[nv].prob > 120 )
     {
     promote( 0,c,(uchar)'¨',30 );
     sort_vers(c);
     return_code = CONTINUE_ACTION;
     }
    }
    else if( memchr("­¯",c->vers[0].let,2) )
         {  //return return_code;
          promote(0,c,(uchar)'¨',0);   //  add vers
    vers_to_first_place ( c,get_nvers(c,(uchar)'¨') ); // make it first
          sort_vers(c);
          return_code = CONTINUE_ACTION;
	 }
  }

  else
  {   // more then 1 letter in word
  cell * wc;
  int16_t i;
  int16_t num_dig;
  uchar let;
#ifdef INTERSEPTOR
  for( wc=c,num_dig=i=0; i < n_ltr;wc=wc->nextl,i++ ) // scan to word end
   if( memchr("1234567890+-/Ž¡‡’‚.,",wc->vers[0].let,20) )
    num_dig++;
  if( num_dig==n_ltr )
    { // russian word
    for( wc=c,num_dig=i=0; i < n_ltr;wc=wc->nextl,i++ ) // scan to word end
      if( memchr("Ž‡¡‚",wc->vers[0].let,4) )
             {  // add russian versions
             switch( wc->vers[0].let )
                {
                case '‚': let=(uchar)'8'; break;
                case 'Ž': let=(uchar)'0'; break;
                case '‡': let=(uchar)'3'; break;
                case '¡': let=(uchar)'6'; break;
                }
             nv = get_nvers(wc, let);
             if( nv==-1 )
                {
                promote(0,wc,let,0);   //  add vers
                vers_to_first_place ( wc,get_nvers(wc,let) ); // make it first
                sort_vers(wc);
                }
             else
                {
                vers_to_first_place ( wc,nv ); // make it first
                sort_vers(c);
                }
             }
    }
#endif
   for( wc=c,num_dig=i=0; i < n_ltr;wc=wc->nextl,i++ ) // scan to word end
   if( memchr("036Ž‡¡",wc->vers[0].let,6) )
    num_dig++;
  if( num_dig!=n_ltr )
    { // russian word
    for( wc=c,num_dig=i=0; i < n_ltr;wc=wc->nextl,i++ ) // scan to word end
      if( memchr("036",wc->vers[0].let,3) )
             {  // add russian versions
             switch( wc->vers[0].let )
                {
                case '0': let=(uchar)'Ž'; break;
                case '3': let=(uchar)'‡'; break;
                case '6': let=(uchar)'¡'; break;
                }
             nv = get_nvers(wc, let);
             if( nv==-1 )
                {
                promote(0,wc,let,0);   //  add vers
                promote(0,wc,let,-48); // spell checker may correct this word
                sort_vers(wc);
                }
             }
    }
  if( n_ltr > 3 )
   for( wc=c,i=0; i < n_ltr;wc=wc->nextl,i++ ) // scan to word end
   {
    if( wc->nvers==1 && wc->vers[0].let==(uchar)'á' &&
        wc->vers[0].prob < 254 && !(wc->cg_flag & c_cg_cut) &&
        wc->recsource & c_rs_ev
      )
      {
      promote(0,wc,(uchar)'¥',0);   //  add vers
      promote(0,wc,(uchar)'¥',-48); // spell checker may correct this word
      sort_vers(wc);
			return_code = NO_ACTION;	// Oleg : 12-07-1994 : go complex word
      }
         if(wc->vers[0].let==(uchar)'ê')
         {
		  // Ìàëûé òâåðäûé çíàê â Áîëãàðñêîì îñíîâíàÿ áóêâà.  08.09.2000 E.P.
          if( !langBul &&
			  i == n_ltr-1 && wc->vers[0].prob > 150 ) // last letter in word
            {  wc->vers[0].let=(uchar)'ì';
        /*  else  wc->vers[0].prob=MAX(0,wc->vers[0].prob-20); // decrease prob
          sort_vers(wc);*/
					return_code = NO_ACTION; // Oleg : 12-07-1994 : go complex word
            }
         }
   }
  }

 return return_code;
}

int16_t final_crit_russian(cell * c)
{
int16_t cnt=0;

cnt += help_spelling_checker(c);
return cnt > 0;
}

static uchar predessor[]="®î¥áãí";

int16_t help_spelling_checker(cell * c)
{
if( !(c->flg&(c_f_let|c_f_bad)) )
  return 0;
if(c->vers[0].let != (uchar)'£')
  return 0;

if( memchr(predessor,c->prevl->vers[0].let,sizeof(predessor)) &&
    (c->cg_flag & c_cg_cutl) )
   {
    if(c->vers[0].prob > 120 )
     { version * v0;
     for (v0=c->vers; v0->let != 0; v0++)
      if( v0->let ==(uchar)'â' ) return 0;     // already exist - OK
       promote(0,c,(uchar)'â',0);   //  add vers
       promote(0,c,(uchar)'â',-48); // promotion
       c->vers[c->nvers].let=c->vers[c->nvers].prob=0;
       return 1;
     }
   }
 return 0;
}

#define MAX_LEN_WORD  48

static Bool case_convert(cell *b, cell *e);
static uchar wave_up_abris(cell *b, cell *e);
static void make_convert(cell *b, cell *e, uchar r);

static uchar capital_rus[]="‰–Šƒ˜™‡•š”›‚Ž‹„†Ÿ—‘Œˆ’œž";
static uchar   small_rus[]="©æª­£èé§åêäë¢¯®«¤¦íïçá¬¨âìî";
//
// TRY CONVERT CASE FOR ANY CELL
//
void correct_case(void)
{
cell *c, *e;
uchar buf[MAX_LEN_WORD+40],word_len;
uchar wrd[MAX_LEN_WORD+40];
Bool r;


if( db_status && snap_activity('c') )
 {
 snap_show_text("CASE : Begin case context");
 snap_monitor();
 }

c=cell_f();
while(1)
  {
  if( (c  = next_word(c))==NULL )break;
  if( db_status && snap_activity('c') )
    strcpy(buf,"CASE : Word : ");
  e = end_word(c,wrd,&word_len,"");
  if( db_status && snap_activity('c') )
    strcpy(&buf[13],wrd);

  r = case_convert(c,e);

  if( r && db_status && snap_activity('c') )
      {
      snap_newcell(c);
      snap_show_text(buf);
      snap_monitor();
      }
  if( e==NULL )break;
  c = e;
  }

if( db_status && snap_activity('c') )
 {
 snap_show_text("CASE : End case context");
 snap_monitor();
 }

return;
}


void make_convert(cell *b, cell *e, uchar r)
{
cell *c;
puchar p;

if( r==1 )
  {  // to lower
  for(c=b->next;c!=e;c=c->next)
    {
    if( (c->flg&c_f_let) && c->nvers>0 )
    if( (p=memchr(capital_rus,c->vers[0].let,sizeof(capital_rus)))!=NULL )
      c->vers[0].let = small_rus[p-capital_rus];
    }
  }
else
  {    // to upper
  if( b->nvers>0 && (b->flg&c_f_let) &&
      is_lower(b->vers[0].let) &&
      !memchr(small_rus,b->vers[0].let,sizeof(small_rus)) )
    return;
  for(c=(is_lower(b->vers[0].let))?b:b->next;c!=e;c=c->next)
    {
    if( (c->flg&c_f_let) && c->nvers>0 )
    if( (p=memchr(small_rus,c->vers[0].let,sizeof(small_rus)))!=NULL )
      c->vers[0].let = capital_rus[p-small_rus];
    }
  }
return;
}

uchar wave_up_abris(cell *b, cell *e)
{
cell *c;
int16_t  up, dn, n, up1, up2;

if( Nb1==-1 || Nb2==-1 || Nb2==128 || Nb1==128 )
  return 0;
for(up1=up2=n=up=dn=0,c=b;c!=e;c=c->next,n++)
  {
  if( (c->flg&c_f_let) && c->nvers>0 )
    {
    if(  memchr(small_rus,c->vers[0].let,sizeof(small_rus)) )
      dn++;
    if(  memchr(capital_rus,c->vers[0].let,sizeof(capital_rus)) )
      up++;
    if( abs( minrow+c->bdiff+bbs1 - c->row )<3 ) up1++;
    if( abs( minrow+c->bdiff+bbs2 - c->row )<3 ) up2++;
    }
  }
if( n<6 || up==0 || dn==0 || up1 && up2 )
  return 0;

return up2 ? 1 : 2 ;
}

Bool case_convert(cell *b, cell *e)
{
uchar r;

if( b->next==e || (b->next->flg&c_f_fict) && b->next->next==e )
  return FALSE;        // one or two letters in word

if( (r=wave_up_abris(b->next,e))==0 )
  return FALSE;        // non convertable word

make_convert(b,e, r);

return TRUE;
}

