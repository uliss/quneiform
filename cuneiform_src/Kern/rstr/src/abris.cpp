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


#define ABRIS_MAIN_MODULE
#include "abris.h"
#include "lang.h"
#include "status.h"

#include "compat_defs.h"
#include "minmax.h"

static char let_mol[let_monN] = {" 0Oaeo"};
static char let_mind[80] = {
   1,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0   1 2 3 4 5 6 7 8 9 a b c d e f
// 30
   0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 40  A B C D E F G H I J K L M N O P Q R S T U V W X Y Z
// 60    			     50                    5b
   0,  3,0,0,0,4,0,0,0,0,0,0,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 } ;
// 60  a b c d e f g h i j k l m n o p q r s t u v w x y z
// 60                                70


void apply_monus(cell *cl)
{
 int16_t i, j;
 if (mon_fl == 0) return;
 for (i=0; i<let_monN; i++)
   if ((j=let_mon[i]))
      promote(0,cl,let_mol[i],j);
}


int16_t abris(s_glue *GL, cell *cl, uchar Let, int16_t prob)
{

 scl=cl;    letter = Let; cprob = prob;
 if ( Z ) {};         // to avoid compiler warning
 if (abriflag==0)  make_abris(GL,cl);
 if (abriflag==1)
  {
   switch(letter)
    {
 // o_filter:
 // ox40 - not round, but linear abris needed
 // 0x20 - horisontal scan lines (not more than 2)
 // 0x10 - vertical lines
 // 0x01 - left upper corner convex
 // 0x04 - left lower corner
 // 0x02 - right upper corner
 // 0x08 - right lower corner
     case 's': case 'S':            return s_filt(cl);
     case 'a':                      return a_filt();
     case 'B':                      return B_filt();
     case 'A':                      return A_filt();
     case 'o': case 'O': case '0':  return o_filt(0x3f);
     case 'c': case 'C':            return c_filt();
     case '<':                      return left_angle_braket_filt();
     case '>':                      return right_angle_braket_filt();
     case 'Q':                      return o_filt(0x07);
     case 'G':                      return o_filt(0x05);
     case 'D':                      return D_filt(); //return o_filt(0x3a);
     case 'W': case 'w':            return w_filt();
     case 'M': case 'N':            return M_filt();
     case 'R':                      return R_filt();
     case 'd':                      return d_filt();
     case 'x':                      return x_filt();
     case 'k':                      return k_filt();
     case ss_deaf_sound:            return ss_filt();
     case 'q':                      return q_filt();
     case 'v': case 'V':            return v_filt();
     case 'H': case 'U':            return HH_filt();
     case '6':                      return Dig_6_filt();
     case 'E':                      return E_filt();

	 // 31.08.2000 E.P.
     case (uchar)liga_CC_usual:
     case (uchar)liga_CR_usual:
		 if (liga_CC == liga_CC_usual) return CR_filt();
		 break;

     case (uchar)liga_CC_latin: // = liga_CC_turkish 31.05.2002 E.P.
     case (uchar)liga_CR_latin: // = liga_CR_turkish 31.05.2002 E.P.
		 if (liga_CC == liga_CC_latin) return CR_filt(); // Исправил 21.05.2002 E.P.
		 break;

     case (uchar)'о':
		 // Конфликтный код 17.07.2001 E.P.
		 if (is_baltic_language(language) ||
			 is_turkish_language(language)	// 21.05.2002 E.P.
			 )
			 break;
     case (uchar)'ћ':                return yu_filt();
    }
  return prob;
  }
 if (abriflag==2) return prob;   // UNABLE TO MAKE ABRIS
 return prob;

}

uchar s_filt(cell *cl)
{

// the list of penalties for different features
#define PNL_2MAX    14      // too many extremums in upper part (20)
#define PNL_NOEXT   14      // there is no extremums at left side  (20)
#define PNL_GAP     20      // there is a gap at left or right side (32)
#define PNL_1LN     20      // Just 1 line in section at specific level (32)
#define SMALL_KEG   14

 int16_t  i, mon;
 mon=0;

 cprob = o_filt(0x01);
 if ( fullh > SMALL_KEG )
   mon  += round_rb_corner();

 make_left_max();    // find 4 "extremums": lmin1, lmin2; lmax1, lmax2
                     // their coordinates are: lxmin1, lxmin2; lxmax1, lxmax2
 if ((lmin2 < BOT_MIN_LEFT_S) && (lmin1 > TOP_MIN_LEFT_S))  // italic
   goto try_r;
 if (lxmax2 < midh)     mon+=PNL_2MAX;   // 2-nd MAX (i.e. gap) in upper part
 if (linh[lxmax2] > 1)  goto try_r;      // 1 line
 if ((lmax < 3)  || (lxmax2-lxmax1 > 3)) mon+=PNL_NOEXT;   // too plain
try_r:
 make_right_max();
 if ((rmin1 < BOT_MIN_RGHT_S) && (rmin2 > TOP_MIN_RGHT_S))  // italic
   goto accept;
 if (rxmax1 > midh)
	 mon+=PNL_2MAX;         // second gap in lower part
  //AK! add
 if (rxmax1 >= 0 && rxmax1 < 128 && linh[rxmax1] > 1)
	 goto accept;     // 1 line
 if ((rmax < 3) || (rxmax2-rxmax1 > 3))
	 mon +=PNL_NOEXT;   // too plain

accept:
 //AK! add
 if (rxmax1 >= 0 && rxmax1 < 128 && r_abr[rxmax1]-rmin1 > 3)         // there is a gap in right side
   {
    //AK!
	//let_mon[let_mind['a'-'0']] -= PNL_GAP;
    //let_mon[let_mind['o'-'0']] -= PNL_GAP;
    //let_mon[let_mind['O'-'0']] -= PNL_GAP;
    //let_mon[let_mind['0'-'0']] -= PNL_GAP;
    if ( let_mind[(uchar)'a'-(uchar)'0'] < let_monN )
		let_mon[let_mind[(uchar)'a'-(uchar)'0']] -= PNL_GAP;
    if ( let_mind[(uchar)'o'-(uchar)'0'] < let_monN )
		let_mon[let_mind[(uchar)'o'-(uchar)'0']] -= PNL_GAP;
    if ( let_mind[(uchar)'O'-(uchar)'0'] < let_monN )
		let_mon[let_mind[(uchar)'O'-(uchar)'0']] -= PNL_GAP;
    if ( let_mind[(uchar)'0'-(uchar)'0'] < let_monN )
        let_mon[let_mind[(uchar)'0'-(uchar)'0']] -= PNL_GAP;
    mon_fl=1;
   }
 if (mon)       // s to be monused
  {
   if (ljmp > 1)
    {         // gap at left - try to monus 'e','o' and so on
     //AK!
	 //let_mon[let_mind['e'-'0']] -= PNL_GAP;
     //let_mon[let_mind['o'-'0']] -= PNL_GAP;
     //let_mon[let_mind['O'-'0']] -= PNL_GAP;
     //let_mon[let_mind['0'-'0']] -= PNL_GAP;
     if ( let_mind[(uchar)'e'-(uchar)'0'] < let_monN )
        let_mon[let_mind[(uchar)'e'-(uchar)'0']] -= PNL_GAP;
     if ( let_mind[(uchar)'o'-(uchar)'0'] < let_monN )
        let_mon[let_mind[(uchar)'o'-(uchar)'0']] -= PNL_GAP;
     if ( let_mind[(uchar)'O'-(uchar)'0'] < let_monN )
        let_mon[let_mind[(uchar)'O'-(uchar)'0']] -= PNL_GAP;
     if ( let_mind[(uchar)'0'-(uchar)'0'] < let_monN )
        let_mon[let_mind[(uchar)'0'-(uchar)'0']] -= PNL_GAP;
     mon_fl=1;
    }
   if (3*rxmax1 <= cl->h)
    {       // there is a gap at 1/3 of height at right side
     //AK!
	 //let_mon[let_mind['e'-'0']] -= PNL_GAP;
     if ( let_mind[(uchar)'e'-(uchar)'0'] < let_monN )
         let_mon[let_mind[(uchar)'e'-(uchar)'0']] -= PNL_GAP;
     mon_fl=1;
    }
   if (lnmid < 2)      // one line (black interval) at middle level
    {
     //AK!
	 //let_mon[let_mind['o'-'0']] -= PNL_1LN;
     //let_mon[let_mind['O'-'0']] -= PNL_1LN;
     //let_mon[let_mind['0'-'0']] -= PNL_1LN;
     if ( let_mind[(uchar)'o'-(uchar)'0'] < let_monN )
         let_mon[let_mind[(uchar)'o'-(uchar)'0']] -= PNL_1LN;
     if ( let_mind[(uchar)'O'-(uchar)'0'] < let_monN )
         let_mon[let_mind[(uchar)'O'-(uchar)'0']] -= PNL_1LN;
     if ( let_mind[(uchar)'0'-(uchar)'0'] < let_monN )
         let_mon[let_mind[(uchar)'0'-(uchar)'0']] -= PNL_1LN;
     mon_fl=1;
    }
  }
 if ( cl->w > 12 ) goto SF_Bidx;
 //AK! add
 if ( rxmax2 > -1 && rxmax2 < 127 && rmax - r_abr[rxmax2+1] >= (fullw/3) )
    mon += 24;
 //AK! add
 if ( lxmax1 > 0 && lxmax1 < 129 && lmax - l_abr[lxmax1-1] >= (fullw/3) )
    mon += 24;
SF_Bidx:
 if (mon && (cl->h < 20)) mon -= 10;   // weak penalty for small
 // check bint_index ( ratio total number of black intervals of
 // component to it's full height; it is computed by 1/64 );
 wint = ( total_num_black_int << 6 ) / fullh;
 i = wint - Let_bint_index(letter);
 if ( i > 0 )
    { mon +=  i * PNL_BINT_INDX; scl->reasno = c_rn_bint_idx; }
 return new_prob( mon );
}

uchar D_filt()
{
 int16_t  pen;
 uchar dev, wfrom, wto, lcv;
 uchar h_max, h_min, h_xmax, h_xmin;

    pen = 0;
    cprob = o_filt(0x3a);
    if ( ncells > 1 ) goto DF_Ret;

    dev = MAX( fullh / 20, 1 );

    make_inter_white_histo();

    wfrom = midh - (midh>>2);
    wto =   midh + (midh>>2);
    get_max_min( histo, wfrom, wto, &h_max, &h_min, &h_xmax, &h_xmin );
    if ( h_max - h_min > dev + 1 )
        pen += 60;
    lcv = constancy_vally_lth( r_abr, wfrom, wto, dev );
    if ( ( wto - wfrom ) - lcv > dev + 1 )
        pen += 60;
DF_Ret:
    return new_prob( pen );
}

uchar B_filt()
{
 int16_t  pen;
 uchar fl, dev, wfrom, lth;

    pen = 0;
    if ( ( scl->nvers == 1 ) || ( fullh <= 22 ) )  goto BF_Ret;
    dev = MAX( fullh / 10, 1 );
    get_max_min ( r_abr, (uchar)(midh>>1), (uchar)(fullh - (midh>>1)), &rmax, &rmin1,
                        &rxmax1, &rxmin11 );
    fl = monotonous_decrease( r_abr, 1, 6, &rmin2, 0 );
    if ( fl == SMOOTH_FUNC )
        pen += 60;
    if ( fl == NOT_FOUND_MON_DECR )
    {
        if ( rmax - rmin1 < dev )
            pen += 80;         // PNL_FOR_FLAT_RIGHT_SIDE;

        fl = monotonous_decrease( r_abr, (uchar)(midh-2), (uchar)(midh+3), &wfrom, dev );
        if ( fl == SMOOTH_FUNC )
            pen += 40;
    }
    if  ( ncells > 1 )  goto BF_Ret;
    wfrom = 0;
    lth   = (midh>>1);
    //AK!
	if (wfrom >= 0 && wfrom < 128 )
	    make_histo( &linh[wfrom], lth );
    if ( histo[1] == 0 )
        pen += 20;
    wfrom += lth;
    lth = (uchar)midh;
    //AK!
	if (wfrom >= 0 && wfrom < 128 )
       make_histo( &linh[wfrom], lth );
    if ( histo[1] == 0 )
        pen += 20;
    wfrom += lth;
    lth    = fullh-wfrom;
    //AK!
	if (wfrom >= 0 && wfrom < 128 )
       make_histo( &linh[wfrom], lth );
    if ( histo[1] == 0 )
        pen += 20;
BF_Ret:
    return new_prob( pen );
}

#define MAX_PEN_FOR_ONE_LEG  100
uchar A_filt()
{
 int16_t  pen, i;

    pen = 0;
    for ( i = fullh - 2; i > fullh - (midh>>1) - 1; i-- )
    {
      //AK! add
	  if ( i >= 0 && i < 128 && linh[i] < 2 )
	     pen ++;
      else
	     break;
    }
    if( midh>3 )
    pen = MAX_PEN_FOR_ONE_LEG * pen / ( (midh>>1) - 1 );
    return new_prob( pen );
}

uchar c_filt()
{
 int16_t  pen, top_bot_delta, ww1, ww2;
 uchar fl, wi, wi0, wi1, wi2, i;

 pen = 0;

 cprob = o_filt(0x05);
 // 02-09-94 06:50pm, LD-update   to debug
 //get_max_min( sumh, 0, fullh - 1, &fl, &wi, &wi1, &wi2 );
 //top_bot_delta = top_end_bottom_weights_delta();
 // 02-09-94 06:51pm, LD-update 
 fl = monotonous_decrease( r_abr, 0, (uchar)(midh+1), &wi, 0 );
 if ( fl == FOUND_MON_DECR )
    pen += 80;
 fl = monotonous_increase( r_abr, (uchar)midh, (uchar)fullh, &wi, 0 );
 if ( fl == FOUND_MON_INCR )
    pen += 80;
 if ( letter == 'c' )    // for lower case only
 {
    uchar wfrom, wto, beg2, end2;
    wfrom = (midh - (midh>>1));
    wto = (midh + (midh>>1));
    fl = find_2int_zone( 1, (uchar)(midh + 1), &beg2, &end2 );
    if ( fl == FOUND_TWO_INT_ZONE )
    {
        fl = monotonous_increase( histo, beg2, (uchar)(end2 + 1), &wi, 0 );
        if ( fl != NOT_FOUND_MON_INCR )
            goto    CF_BotAndTop;
        get_max_min( histo, beg2, (uchar)(end2 + 1), &fl, &wi, &wi1, &wi2 );
        if ( fl - wi <= 1 )
            goto CF_BotAndTop;
        //AK! add
        if ( end2 < 128 && end2 >=0 && histo[end2] <= wi )
            { pen += 100; goto CF_Ret; }
        //AK! add
        if ( end2 < 128 && end2 >=0 && ( 3*histo[end2]) < sumh[end2] )
            { pen += 100; goto CF_Ret; }
        //AK! add
        if ( end2 < 128 && end2 >0 && ( histo[end2-1] == wi ) &&
             ( (sumh[end2-1] - sumh[end2]) > (midw>>1) ) )
            { pen += 86; goto CF_Ret; }
        //AK! add
		if ( end2 < 128 && end2 >=0 && ( histo[end2] > midw ) ||
             ( ( histo[end2] == fl ) && ( midh - end2 > 1 ) ) )
            goto CF_BotAndTop;   // 'c' with hooked nose case
        //AK! add
		if ( end2 < 128 && end2 >0 &&
			( histo[end2] > histo[end2-1] ) && ( r_abr[end2] > r_abr[end2-1] ) )
            goto CF_BotAndTop;    // 'c' with hooked nose case

        wi = end2;
        wi2 = end2 - beg2 + 1;
        fl = find_2int_zone( (uchar)midh, (uchar)fullh, &beg2, &end2 );
        wi1 = end2 - beg2 + 1;
        if ( ( fl == FOUND_TWO_INT_ZONE ) && ( wi2 > 3 ) )
        {
            if ( wi1 < 2 )  // when lower tail is very small -> broken 'e' case
                pen += 86;
            if ( ( wi1 == 2 ) && ( beg2 - wi <= midh ) )
                pen += 86;  // when lower tail is small and
                            // nearly to upper nose -> broken 'e' case
            goto CF_Ret;
        }
        if ( fl == NOT_FOUND_TWO_INT_ZONE )
            pen += 86;
        goto CF_BotAndTop;
    }
    else        // 2int zone is not found
    {
      // avoiding stick
      get_max_min( sumh, 1, wto, &fl, &wi, &wi1, &wi2 );
      if ( fl - wi < ((fullw+3)>>2) )  // there is no extended area -> stick
        pen += 44;
    }

    fl = find_1int_zone( wfrom, wto, &beg2, &end2 );
    if ( fl == FOUND_ONE_INT_ZONE )
    {
        get_max_min( sumh, beg2, (uchar)(end2 + 1), &fl, &wi, &wi1, &wi2 );
        // wi - minimum of sumh in 1-interval zone
        if ( midw <= wi )      // bold 'c' case
            goto CF_BotAndTop;
        //AK! add
		if ( wi2 >=0 && wi2 < 128 && midh >= 0 && midh < 128 )
		   {
			for ( i = MAX( midh, wi2 ); i >= beg2 && i >= 0; i -- )
				if ( abs(wid[i] - wid[wi2]) > (fullh/3) )
				   break;
			wfrom = i;
			for ( i = (uchar)midh; i < end2 && i < 128; i ++ )
				if ( abs(wid[i] - wid[wi2]) > (fullh/3) )
					break;
            wto = i + 1;
		   }
        top_bot_delta = top_end_bottom_weights_delta();

        if ( ( wfrom < fullh - wto ) || ( wto - wfrom  > midh ) )
            goto CF_BotAndTop;
        if ( (wfrom == fullh - wto) && ( midh - wfrom > (fullh+9)/10 )  )
        {
            if ( top_bot_delta > (wi<<2) )
                pen += 126;
            goto CF_BotAndTop;
        }
        if ( top_bot_delta < 0 )
            goto CF_BotAndTop;
        if ( top_bot_delta > (wi * 3) )
                pen += 126;
        if (top_bot_delta == (wi * 3))
        {
            if ( midh - wfrom > (fullh+9)/10 )
                goto CF_BotAndTop;
            else
              if ( wfrom > fullh - wto )
                pen += 120;
        }
        /******
        if ( ( abs(r_abr[midh] - r_abr[beg2]) <= 1 ) &&
             ( abs(r_abr[midh] - r_abr[end2]) <= 1 ) )  // bold 'c' case
            goto CF_BotAndTop;
        *******/
        //AK! add
		if ( wi < 128 && wi >= 0 &&
			 ( fl - wi  >= ((fullw+2)/3) ) && ( wid[wi1] == sumh[wi1] ) )
            pen += 86;
        goto CF_BotAndTop;
    }
    if ( fl == NOT_FOUND_ONE_INT_ZONE )
        pen += 120;
 }
CF_BotAndTop:
 if ( ( letter == 'c' ) && ( fullh > 12 ) && ( fullw >= 8 ) && ( ncells == 1 ) )
 {
    // testing italic case
    wi = ( (fullh + 9) / 10 );
    wi0 = fullh - MAX( 1, wi );
    //AK! add
	if ( wi0 >= 0 && wi0 < 128 && wi >= 0 && wi < 128 )
	{
	   ww1 = r_abr[wi0] - r_abr[wi];
       ww2 = l_abr[wi] - l_abr[wi0];
	}
    if ( ( ww1 >= fullw/3 ) && ( ww2 >= fullw/3 ) ) // italic case
    {
      wi = 0;
      wi0 = fullw - 1;
    }
    else
    {
      wi = ( (fullw + 9) / 10 );
      wi0 = fullw - MAX( 1, wi );
    }
    wi1 = constancy_vally_lth (bot, wi, wi0, 1);
    wi2 = constancy_vally_lth (top, wi, wi0, 1);
    wi = abs(wi2 - wi1);
    if ( ( wi1 <= (midw>>1) ) || ( wi2 <= (midw>>1) ) )
        pen += 40;
    if ( wi > ( midw>>1 ) )
        pen += CONST_VALLY_PNL;
    wi1 = MIN( fullw - 3, midw + (midw>>1) + 1 );
    for ( fl = 0, wi = (uchar)midw; wi <= wi1; wi++ )
    {
        if ( wi < 127 && wi >= 0 &&
			 (abs(top[wi] - top[wi+1]) < midh) &&
             (abs(top[wi] - top[wi+1]) > (midh>>2) ) )
          fl ++;
        if ( wi < 127 && wi >= 0 &&
			 (abs(bot[wi] - bot[wi+1]) < midh) &&
             (abs(bot[wi] - bot[wi+1]) > (midh>>2) ) )
          fl ++;
    }
    if ( fl )
        pen += fl * 30;
 }
CF_Ret:
 return new_prob( pen );
}

uchar a_filt()
{
 int16_t i, pen;
 uchar  prev, fl, fl_tail;

 pen = 0;
 cprob = o_filt(0x04); fl_tail = 0;

   // not more than 2 lines in every 3-horizontal scan line in the top half
   //AK! add
   for (i=1; i < 127 && i < midh; i++)
   {
     if ( ( linh[i-1] > 2 ) && ( linh[i] > 2 ) && ( linh[i+1] > 2 ) )
     {
        pen += 80;
        goto AF_TopAbrisTest;
     }
   }
   //AK! add
   if ( fullh < 130 )
   for (i=fullh-2, prev=1, fl=0; i>=midh && i >= 1; i--)
   {
      if (linh[i]==1)
        { fl=1; goto AF_Cont; }
      if (( linh[i] >= 3) && (fl || ( prev > 1)) )
      {
         fl = 0;
         if (linh[i-1] < 3)
          pen += 10;
         else
         {
           fl_tail = r_tail((uchar)i);
           if ( fl_tail )
             break;
           else
           {
               if ( bold_flag != LIGHT_COMP )
                   { pen += 80; break; }
           }
         }
       }
AF_Cont:
      prev = linh[i];
   }

AF_TopAbrisTest:
 // Test Top Abris to be monotonous decreasing (for cells aren't composed only)
 if ( ncells > 1 ) goto AF_BintIndexTest;
 fl = monotonous_decrease( top, 0, (uchar)midw, &prev, 1 );
 if ( fl == NOT_FOUND_MON_DECR )
    pen += 80;

AF_BintIndexTest:
 // check bint_index ( ratio total number of black intervals of
 // component to it's full height; it is computed by 1/64 );
 wint = ( total_num_black_int << 6 ) / fullh;
 i = wint - Let_bint_index(letter);
 if ( i > 0 )
    { pen +=  i * PNL_BINT_INDX; scl->reasno = c_rn_bint_idx; }

 if ( fl_tail )
 {
    uchar    wb;
    get_max_min( bot, (uchar)MAX( 1, (fullw/10) ), (uchar)((fullw - (midw>>1))),
                                        &fl, &prev, &wb, &wb );
    if ( fl - prev > midh )
        pen += 80;
    return new_prob(pen);
 }
 else
 {
    return a_rb_corner(new_prob( pen ));
 }
}


uchar M_filt()
{
 int16_t pen;
 int16_t i1, i2, dev, gap;
 uchar flit, i3, fax_corr, mode_linh;

 pen=0; flit=0; fax_corr = 0;
 if ( fax1x2 ) fax_corr = 1;
//AK! add
 if ( fax_corr < 127 && fax_corr >= -1 &&
	  linh[1+fax_corr] <2)  pen += 40;
 if ( fullh-fax_corr < 130 && fullh-fax_corr >= 2 &&
	  linh[fullh-2-fax_corr] < 2)  pen += 40;

 // test right side and left side against rounding ( avoiding "OO" )

 i2 = midh;
 i1 = (fullh >> 3) + fax_corr;
 i3 = fullh - i1;
 dev = MAX( get_size () / 10, 1 );
 mode_linh = (uchar)make_histo( &linh[1], (uchar)(fullh-1) );
     // this function returns mode value of the array ( 1st param.)
     // and generates histo_max_value variable
     // the mode value of the black intervals numbers has to be <= 3
//AK! add
 if ( i1 < 128 && i2 < 128 && i3 < 128 &&
      i1 >= 0  && i2 >= 0  && i3 >= 0  )
     {
	  if ( ( ( r_abr[i1] - r_abr[i2] ) > dev ) &&
           ( ( r_abr[i3] - r_abr[i2] ) > dev ) )
         pen += ROUND_SIDE_PNL;

      if ( ( ( l_abr[i1] - l_abr[i2] ) > dev ) &&
           ( ( l_abr[i3] - l_abr[i2] ) > dev )  )
         pen += ROUND_SIDE_PNL;
     }

 // test right side and left side against funnel like image ( avoiding "W" )

 i1 = fullh >> 2;
 i2 = fullh - i1;
//AK! add
 if ( i1 >= 0 && i2 >= 0 && i1 < 128 && i2 < 128 &&
	  ( wid[i1] - wid[i2] ) > dev + 1 )
 {
    if ( ncells == 1 )
        pen += FUNNEL_PNL;
    else
    {
        pen += FUNNEL_PNL / 2;
        if ( ( histo[4] == 0 ) && ( letter == 'M' ) )
            pen += 30;
    }
 }

 // test right and left sides against gap ( avoiding any garbage from BOX )
 // this testing is working only in single cell case
   if ( ncells == 1 )
   {
        i1 = fullh >> 2;  i2 = fullh - i1 - fax_corr;

        gap = gap_in_side( (uchar)i1, (uchar)i2, RIGHT_S, dev, &flit );
        if ( gap > 0 ) pen += gap * GAP_IN_SIDE_PNL;

        gap = gap_in_side( (uchar)i1, (uchar)i2, LEFT_S, dev, &i3 );
        if ( gap > 0 )  pen += gap * GAP_IN_SIDE_PNL;
   }
 // check bottom and right abris to avoid some conglomerats like "NI"
  if ( flit )  goto MF_LetterN_Testing;
  if ( (scl->nvers == 1 ) && ( letter == 'M') )  goto MF_Rbcorner;
  i1 = (midw >> 1);  i2 = midw + i1;
  i3 = constancy_vally_lth (bot, (uchar)i1, (uchar)i2, 1);
  if ( i3 > i1 )
   pen += ( i3 - i1 ) * CONST_VALLY_PNL;
  i3 = constancy_vally_lth (top, (uchar)i1, (uchar)i2, 1);
  if ( i3 > i1 )
   pen += ( i3 - i1 ) * CONST_VALLY_PNL;

MF_LetterN_Testing:

  if ( letter == 'N' )
  // check the number of black intervals to be <= 3 ( avoiding garbege )
  {
     // the mode value of the black intervals numbers has to be <= 3
    if ( mode_linh > 3 )
        pen += 80;
    goto MF_Ret;
  }
MF_Rbcorner:
 // test right bottom corner against rounding ( avoiding any garbage from BOX )
 if ( flit) goto MF_Ret;
 pen += rb_corner();

MF_Ret:
 if ( ( ncells > 1 ) && ( bold_flag == BOLD_COMP ) )
    pen += 80;
 return new_prob( pen );
}


uchar R_filt()
{
 int16_t pen;
 uchar fl, dev, wfrom;

  pen=0;  dev = MAX( fullh / 10, 1 );

 if ( scl->nvers == 1 ) goto RF_Ret;
 get_max_min ( r_abr, (uchar)(midh>>1), (uchar)(fullh - (midh>>1)), &rmax, &rmin1,
               &rxmax1, &rxmin11 );
 fl = monotonous_decrease( r_abr, 1, 6, &rmin2, 0 );
 if ( ( rmax - rmin1 < dev ) && ( fl == NOT_FOUND_MON_DECR ) )
  pen += 80;         // PNL_FOR_FLAT_RIGHT_SIDE;

  wfrom = (midh>>1);
  //AK! add
  if (wfrom < 128 && wfrom >= 0 )
      make_histo( &linh[wfrom], (uchar)midh );
  if ( ( histo[3] > 1 ) && ( histo[1] == 0 ) )
    pen += 60;

/*******************
 // check bint_index ( ratio total number of black intervals of
 // component to it's full height; it is computed by 1/64 );
 wint = ( total_num_black_int << 6 ) / fullh;
 dev = wint - Let_bint_index(letter);
 if ( dev > 0 )
    { pen +=  dev * PNL_BINT_INDX; scl->reasno = c_rn_bint_idx; }
***********************/
RF_Ret:
 return new_prob( pen );

}

uchar HH_filt()
{
 int16_t  pen;
 int16_t  dev, gap;
 uchar flit, i3, i1, i2;

 pen=0; flit = 0;


 // test right side and left side against rounding ( avoiding "OO" )

 i2 = (uchar)midh;
 i1 = fullh >> 3;
 i3 = fullh - i1;
 dev = MAX( fullh / 10, 1 );
//AK! add
 if ( i1 < 128 && i2 < 128 && i3 < 128 &&
      i1 >= 0  && i2 >= 0  && i3 >= 0  )
    {
	 if ( ( ( r_abr[i1] - r_abr[i2] ) > dev ) &&
		  ( ( r_abr[i3] - r_abr[i2] ) > dev ) )
	  pen += ROUND_SIDE_PNL;

	 if ( ( ( l_abr[i1] - l_abr[i2] ) > dev ) &&
		  ( ( l_abr[i3] - l_abr[i2] ) > dev )  )
	  pen += ROUND_SIDE_PNL;
    }
 // test right side against gap ( avoiding any garbege from BOX )

    if ( ncells == 1 )
    {
        i1 = fullh >> 2;  i2 = fullh - i1;
        gap = gap_in_side( i1, i2, RIGHT_S, dev, &flit );
        if ( gap > 0 ) pen += gap * GAP_IN_SIDE_PNL;
    }

 // test left side against gap ( avoiding any garbege from BOX )

    if ( ncells == 1 )
    {
        i1 = fullh >> 2;  i2 = fullh - i1;
        gap = gap_in_side( i1, i2, LEFT_S, dev, &i3 );
        if ( gap > 0 )
            pen += gap * GAP_IN_SIDE_PNL;
    }

// This checking works only for sanserif letter "U" !!!
// check right bottom corner avoiding anything beside arounding
 if ( letter == 'U' )
  {
    i2 = test_bottom_corner( r_abr,  &gap );
    if ( i2 == BC_SERIF )
    {
        i3 = monotonous_increase( l_abr, 1, (uchar)(3+dev), &i1, 0 );
        if ( i3 != FOUND_MON_INCR )
            pen += gap;
    }
    if ( i2 == BC_CONCAVE )
    {
        i3 = monotonous_increase( l_abr, (uchar)dev, (uchar)(dev+4), &i1, 0 );
        if ( i3 != FOUND_MON_INCR )
            pen += gap;
    }
    //AK! add
	if ( midh >=0 && midh < 128 && fullh >= 0 && fullh < 128 &&
		 ( flit == 0 ) && ( l_abr[midh] - l_abr[fullh-2] > 1 ) )
     pen += 60;             // that distances must be invariable
  }
// check the number of black intervals to be < 3 ( avoiding "FI" conglomerat )
 make_histo( linh, (uchar)fullh );          // generates histo_max_value variable
 if ( (histo_max_value > 2) &&       // 3 intervals and more are forbidden
      ( (histo[3] > histo[2]) || (histo[3] > fullh/3) )  )
   pen += 80;

 return new_prob( pen );
}


#define PNL_JMP_RS 20
uchar w_filt()
{
 int16_t pen, i, i3, j1, j2, db;
 uchar sb, nb, flat_side, dps, jmp, fl_ev_vers, dp1, dp2, fl, pnl_rs;
 int16_t    arm1, arm2;
 pen=0;

   if ( ( ( scl->nvers == 1 ) ||
          (  ( scl->nvers == 2 ) &&
             ( ( (scl->vers[0].let == 'w') && (scl->vers[1].let == 'W') ) ||
               ( (scl->vers[0].let == 'W') && (scl->vers[1].let == 'w') )
             )
          )
        ) && ( scl->recsource & c_rs_ev )
       )  fl_ev_vers = 1;
   else   fl_ev_vers = 0;
 // Checking up top central node position ( avoiding 'vv' conglomerat )
 get_max_min( bot, (uchar)(midw>>1), (uchar)(fullw - (midw>>1)), &bmax, &bmin1,
                                            &bxmax1, &bxmin11 );
 if ( ( ncells == 1 ) && ( bmax > midh ) && ( MIN( bxmax1, midw ) > 3 ) )
 {
    i3 = 0; dp1 = MAX( bxmax1, midw );
    i3 = constancy_vally_lth ( bot, (uchar)(dp1 - 3), (uchar)(dp1 + 3), 0 );
    if ( ( i3 > MAX( 1, (fullw/10) ) ) || ( fullh - bmax < 3 ) )
        pen += 36;
 }
 if ( (fl_ev_vers ) && ( num_lines_of_comp >= 4 ) )
    goto    WF_Ret;

 if (ncells==1)
  {dp1=80; dp2=10; pnl_rs = 20;}
 else
  {dp1=16; dp2=2; pnl_rs = 10;}

 for (i3=0, sb=sumh[i3]; i3<4; i3++)
   {
     if (linh[i3] > 2)
		 break;
     if ((sumh[i3] - sb) > 2)
		 break;
     sb=sumh[i3];
   }

 db = midh + ( fullh >> 3 ) ;              // middle + 1/8 of full height
 // prepare 'shift' depending on the point size
 dps = 2;                           // regular point size
 if ( fullh < 20 )  dps = 1;        // small point size
 if ( fullh > 38 )  dps = 3;        // big point size
//AK! add
 for ( i= MAX(dps, 0); i<db && i < 128; i++)               // all strings of 2 or less segments
  if ( linh[i] >= 3) goto acc1;
 if (linh[0] <3) pen += dp1;
 else
  { if (linh[1] < 3)
     { if (fullh > 18)	pen += dp1;  else pen += (dp1/2); }
  }
acc1:
 //AK! add
  for (i=MAX(fullh-1, 0), j1=100; i>fullh-5 && i < 128; i--)
      if (j1 > linh[i])
         j1=linh[i];
 if (j1 > 2)
	 pen+=dp1;

 j1=l_abr[1];
 //AK! add
 if ( fullh >= 2 && fullh < 130 )
     j2=l_abr[fullh-2];
 //AK! add
 if ( midh >= 0 && midh < 128 )
     i=l_abr[midh];
 if ((j1-i > 2) && (j2-i > 2))
	 pen+=dp1;
 j1=r_abr[1];
 //AK! add
 if ( fullh >= 2 && fullh < 130 )
 j2=r_abr[fullh-2];
 //AK! add
 if ( midh >= 0 && midh < 128 )
 i=r_abr[midh];
 if ((j1-i > 2) && (j2-i > 2)) pen+=dp1;
 flat_side = 0;
 //AK! add
 if ( midh >= 3 && midh < 131 )
 j1 = r_abr[midh-3];
 //AK! add
 if ( fullh >= 3 && midh < 131 )
 j2 = r_abr[fullh-3];
  //AK! add
 if ( midh=dps >= 0 && midh+dps < 128 )
 i=r_abr[midh+dps];
 if ( j1 == j2 )
	 flat_side += 1;
 if ( j1 == i )
	 flat_side += 1;
 if ( j2 == i )
	 flat_side += 1;
 pen += pnl_rs * flat_side;       // too flat right side is forbidden

 // test bottom right corner against monotonious increasing
 //AK! add
 for ( fl = 0, jmp = 0, i = MAX(fullh-2, 0); i > midh && i < 127; i-- )
  {
     if ( r_abr[i] < r_abr[i-1] )
      if ( ( r_abr[i-1] - r_abr[i] ) > 1 )
       fl += 1;      // decreasing is forbidden at bottom right side
     if ( abs( r_abr[i] - r_abr[i-1] ) > 3 )
       jmp += 1;      // jumps are forbidden at bottom right side
  }
 pen += pnl_rs * fl;  // decreasing at right side penalty

 if ( jmp & 0xfe )    // number of jumps is odd
  pen += jmp * PNL_JMP_RS;
//AK! add
 if ( midh < 128 && midh >= 0)
 for (i=midh, sb=l_abr[midh], fl=0; i>=i3 && i >= 0; i--)
    {
	 nb=l_abr[i];
     db=nb-sb;
     if (db <= 0)
       {
		if (fl==0)
		   {
			j1=i;
			sb=nb;
	       }
	   }
      else
       {
        if ((db >= 2) && (3*nb <= fullw))
           {
            if (fl)
               {
                if ((fullh < 18) || (i > 0))
					goto penl;  // second jump
                break;
               }
              else
               {
				if (i==i3)
					break;  // may be serif and 1 pixel at top
 penl:
				pen+=dp1;
				break;
               }
           }
          else
		  {
		   fl=1;
		   continue;
		  }
       }
    } /* end of for_operator */
 //AK! add
 if ( midh < 128 && midh >= 0)
 for (i=midh, sb=l_abr[midh], fl=0; i<fullh && i < 128; i++)
  { nb=l_abr[i];
    db=nb-sb;
    if (db >= 0)
      { if (fl==0) { j2=i; sb=nb; }
        if ((db > 4) && (i < (fullh-2))) { pen+=dp1; break; }
      }
    else
      { fl=1; if (db <= -2) { pen+=dp1; break; } }
   }

 //AK! add
 if ( j2 < 128 && j2 >= 0 && j1 < 128 && j1 >=0)
    db=l_abr[j2]-l_abr[j1];

 if ((db < 3) && (db*6 < (j2-j1))) pen+=dp2;

 //AK! add
 if ( midh < 128 && midh >= 0)
 for (i=midh, sb=r_abr[midh], fl=0; i>=i3; i--)
  { nb=r_abr[i];
    db=nb-sb;
    if (db <= 0)
      { if (fl==0)  { j1=i; sb=nb; }}
    else
      {	if ((db >= 2) && (3*nb <= fullw))
	 {
	  if (fl)         // second jump
	    { if ((fullh < 18) || (i > 0))  goto penr;
	      break;
	    }
	  else
	    { if (i==i3) break;
	      penr: pen+=dp1; break;
	    }
	  }
	 else { fl=1; continue; }
       }
   }

 //AK! add
 if ( midh < 128 && midh >= 0)
 for (i=midh, sb=r_abr[midh], fl=0; i<fullh; i++)
  { nb=r_abr[i];
    db=nb-sb;
    if (db >= 0)
      {
        if (fl==0)                       { j2=i; sb=nb; }
        if ((db > 4) && (i < (fullh-2))) { pen+=dp1; break; }
      }
    else
      { fl=1; if (db <= -2) { pen+=dp1; break; } }
   }

 //AK! add
 if ( j2 < 128 && j2 >= 0 && j1 < 128 && j1 >=0)
     db=r_abr[j2]-r_abr[j1];

 if ((db < 3) && (db*6 < (j2-j1))) pen+=dp2;

 if (pen)   // try to save italic, verify top and apply weak penalty
  {
   if ( !fl_ev_vers )  goto retpen;

   for (i=0, sb=0; i<midw && i < 128; i++)
     if (sb < top[i]) { sb=top[i]; j1=i; }
   if (sb < midh) goto retpen;

   //AK! add
   if (fullw < 129 )
   for (i=fullw-1, sb=0; i>midw && i >=0; i--)
     if (sb < top[i]) { sb=top[i]; j2=i; }
   if (sb < midh) goto retpen;

   //AK! add
   if (j1 >= 0 )
   for (i=j1, sb=127; i<j2 && i < 128; i++)
     if (sb > top[i])  sb=top[i];
   if ( (sb < midh) && ( flat_side == 0) ) pen = 20;
     // high enough hill between two deep gaps and right side is not flat

  }
retpen:
 for (i=0, arm1=127; i<(midw-2) && i < 128; i++)
     if (sb > top[i]) sb=top[i];
 //AK! add
 if (midw >= -2 )
 for (i=midw+2, arm2=127; i<fullw && i < 128; i++)
     if (nb > top[i]) nb=top[i];
 arm1=arm1-arm2;
 if ((arm1 > 3) || (arm1 < -3)) pen+=50;   // different arms

 if ( ( ncells > 1 ) && ( bold_flag == BOLD_COMP ) )
    pen += 80;

WF_Ret:
 return new_prob( pen );
}


uchar o_filt(int16_t f)
{
 int16_t pen, fl, i, i1, i2, i3, i4, j1, j2, j3, j4;
 uchar *lp1, pv, cv;
 i1=fullh-1; i2=i1-1; i3=i2-1; i4=i3-1;
 j1=fullw-1; j2=j1-1; j3=j2-1; j4=j3-1;
 pen=0;
 if (f & 16)
  {
  // NO VERTICAL LINES PROVIDED
  }
 if (f & 32)
  {
   for (i=2, lp1=&linh[2], pv=1, fl=0; i<=midh; i++,lp1++,pv=cv)
    { cv=*lp1; if (cv==1) fl=1;
      if ((cv >= 3) && fl && (pv > 1))
	{ if (*(lp1+1) < 3) pen += 10; else { pen += 80; break; }}
    }
     // not more than 2 lines in every vertical scan line
   //AK! add
   if (fullh >= -2 && fullh < 128 )
   for (i=fullh-2, lp1=&linh[i], pv=1, fl=0; i>=midh && i>=0; i--,lp1--,pv=cv)
    { cv=*lp1; if (cv==1) fl=1;
      if ((cv >= 3) && fl && (pv > 1))
	{ if (*(lp1-1) < 3) pen += 10; else { pen += 80; break; }}
    }
     // not more than 2 lines in every horizontal scan line
  }
 if ( pen > 0 ) cv = (uchar)pen;
 else           cv = 0;
 if (fullh < 18) goto small_;
 if (f & 1)
  {
   fl=0;
   if (l_abr[0] < l_abr[1]) goto dv1;
   if (l_abr[0] == l_abr[1]) fl=1;
   if (l_abr[1] < l_abr[2]) goto dv1;
   if (l_abr[1] == l_abr[2]) fl=1;
   if (l_abr[2] < l_abr[3]) goto dv1;
   if (l_abr[2] == l_abr[3]) fl=1;
   if ((fl) && (l_abr[0] == l_abr[3])) goto dv1;
   fl=0;
   if (top[0] < top[1]) goto dv1;
   if (top[0] == top[1]) fl=1;
   if (top[1] < top[2]) goto dv1;
   if (top[1] == top[2]) fl=1;
   if (top[2] < top[3]) goto dv1;
   if (top[2] == top[3]) fl=1;
   if ((fl) && (top[0] == top[3])) goto dv1;
  }
 if ((f & 0x40)) pen+=20;
 goto ac2;
 dv1: if (!(f & 0x40)) pen+=20;
 ac2:
 if (f & 2)
  {
   fl=0;
   if (r_abr[0] < r_abr[1]) goto dv2;
   if (r_abr[0] == r_abr[1]) fl=1;
   if (r_abr[1] < r_abr[2]) goto dv2;
   if (r_abr[1] == r_abr[2]) fl=1;
   if (r_abr[2] < r_abr[3]) goto dv2;
   if (r_abr[2] == r_abr[3]) fl=1;
   if ((fl) && (r_abr[0] == r_abr[3])) goto dv2;
   fl=0;
   //AK! add
   if ( j1 >= 0  && j2 >=  0 && j3 >=  0 &&
	    j1 < 128 && j2 < 128 && j3 < 128 )
      {
	   if (top[j1] < top[j2]) goto dv2;
	   if (top[j1] == top[j2]) fl=1;
	   if (top[j2] < top[j3]) goto dv2;
	   if (top[j2] == top[j3]) fl=1;
	   if (top[j3] < top[j4]) goto dv2;
	   if (top[j3] == top[j4]) fl=1;
	   if ((fl) && (top[j1]  == top[j3])) goto dv2;
      }
  }
 if ((f & 0x40)) pen+=20;
 goto ac3;
 dv2: if (!(f & 0x40)) pen+=20;
 ac3:
 if (f & 4)
  {
   fl=0;
   //AK! add
   if ( i1 >= 0  && i2 >=  0 && i3 >=  0 &&
	    i1 < 128 && i2 < 128 && i3 < 128 )
     {
	   if (l_abr[i1] < l_abr[i2]) goto dv3;
	   if (l_abr[i1] == l_abr[i2]) fl=1;
	   if (l_abr[i2] < l_abr[i3]) goto dv3;
	   if (l_abr[i2] == l_abr[i3]) fl=1;
	   if (l_abr[i3] < l_abr[i4]) goto dv3;
	   if (l_abr[i3] == l_abr[i4]) fl=1;
	   if ((fl) && (l_abr[i1]  == l_abr[i3])) goto dv3;
     }
   fl=0;
   if (bot[0] < bot[1]) goto dv3;
   if (bot[0] == bot[1]) fl=1;
   if (bot[1] < bot[2]) goto dv3;
   if (bot[1] == bot[2]) fl=1;
   if (bot[2] < bot[3]) goto dv3;
   if (bot[2] == bot[3]) fl=1;
   if ((fl) && (bot[0] == bot[3])) goto dv3;
  }
 if ((f & 0x40)) pen+=20;
 goto ac4;
 dv3: if (!(f & 0x40)) pen+=20;
 ac4:
 if (f & 8)
  {
   fl=0;
      //AK! add
   if ( i1 >= 0  && i2 >=  0 && i3 >=  0 &&
	    i1 < 128 && i2 < 128 && i3 < 128 )
     {
	   if (r_abr[i1] < r_abr[i2]) goto dv4;
	   if (r_abr[i1] == r_abr[i2]) fl=1;
	   if (r_abr[i2] < r_abr[i3]) goto dv4;
	   if (r_abr[i2] == r_abr[i3]) fl=1;
	   if (r_abr[i3] < r_abr[i4]) goto dv4;
	   if (r_abr[i3] == r_abr[i4]) fl=1;
   }
   if ((fl) && (r_abr[i1]  == r_abr[i3])) goto dv4;
   fl=0;
      //AK! add
   if ( j1 >= 0  && j2 >=  0 && j3 >=  0 &&
	    j1 < 128 && j2 < 128 && j3 < 128 )
   {
	   if (bot[j1] < bot[j2]) goto dv4;
	   if (bot[j1] == bot[j2]) fl=1;
	   if (bot[j2] < bot[j3]) goto dv4;
	   if (bot[j2] == bot[j3]) fl=1;
	   if (bot[j3] < bot[j4]) goto dv4;
	   if (bot[j3] == bot[j4]) fl=1;
	   if ((fl) && (bot[j1]  == bot[j3])) goto dv4;
   }
  }
 if ((f & 0x40)) pen+=20;
 goto ac5;
 dv4: if (!(f & 0x40)) pen+=20;
 ac5:
 goto getout;
small_:
 if (f & 1)
  {
   fl=0;
   if (l_abr[0] < l_abr[1]) goto d2v1;
   if (l_abr[0] == l_abr[1]) fl=1;
   if (l_abr[1] < l_abr[2]) goto d2v1;
   if (l_abr[1] == l_abr[2]) fl=1;
   if (l_abr[2] < l_abr[3]) goto d2v1;
   if (l_abr[2] == l_abr[3]) fl=1;
   if ((fl) && (l_abr[0] == l_abr[3])) goto d2v1;
   fl=0;
   if (top[0] < top[1]) goto d2v1;
   if (top[0] == top[1]) fl=1;
   if (top[1] < top[2]) goto d2v1;
   if (top[1] == top[2]) fl=1;
   if (top[2] < top[3]) goto d2v1;
   if (top[2] == top[3]) fl=1;
   if ((fl) && (top[0] == top[3])) goto d2v1;
  }
 if ((f & 0x40)) pen+=20;
 goto a2c2;
 d2v1: if (!(f & 0x40)) pen+=20;
 a2c2:
 if (f & 2)
  {
   fl=0;
   if (r_abr[0] < r_abr[1]) goto d2v2;
   if (r_abr[0] == r_abr[1]) fl=1;
   if (r_abr[1] < r_abr[2]) goto d2v2;
   if (r_abr[1] == r_abr[2]) fl=1;
   if (r_abr[2] < r_abr[3]) goto d2v2;
   if (r_abr[2] == r_abr[3]) fl=1;
   if ((fl) && (r_abr[0] == r_abr[3])) goto d2v2;
   fl=0;
      //AK! add
   if ( j1 >= 0  && j2 >=  0 && j3 >=  0 &&
	    j1 < 128 && j2 < 128 && j3 < 128 )
   {
	   if (top[j1] < top[j2]) goto d2v2;
	   if (top[j1] == top[j2]) fl=1;
	   if (top[j2] < top[j3]) goto d2v2;
	   if (top[j2] == top[j3]) fl=1;
	   if (top[j3] < top[j4]) goto d2v2;
	   if (top[j3] == top[j4]) fl=1;
	   if ((fl) && (top[j1]  == top[j3])) goto d2v2;
   }
  }
 if ((f & 0x40)) pen+=20;
 goto a2c3;
 d2v2: if (!(f & 0x40)) pen+=20;
 a2c3:
 if (f & 4)
  {
   fl=0;
   //AK! add
   if ( i1 >= 0  && i2 >=  0 && i3 >=  0 &&
	    i1 < 128 && i2 < 128 && i3 < 128 )
   {
	   if (l_abr[i1] < l_abr[i2]) goto d2v3;
	   if (l_abr[i1] == l_abr[i2]) fl=1;
	   if (l_abr[i2] < l_abr[i3]) goto d2v3;
	   if (l_abr[i2] == l_abr[i3]) fl=1;
	   if (l_abr[i3] < l_abr[i4]) goto d2v3;
	   if (l_abr[i3] == l_abr[i4]) fl=1;
	   if ((fl) && (l_abr[i1]  == l_abr[i3])) goto d2v3;
   }
   fl=0;
   if (bot[0] < bot[1]) goto d2v3;
   if (bot[0] == bot[1]) fl=1;
   if (bot[1] < bot[2]) goto d2v3;
   if (bot[1] == bot[2]) fl=1;
   if (bot[2] < bot[3]) goto d2v3;
   if (bot[2] == bot[3]) fl=1;
   if ((fl) && (bot[0] == bot[3])) goto d2v3;
  }
 if ((f & 0x40)) pen+=20;
 goto a2c4;
 d2v3: if (!(f & 0x40)) pen+=20;
 a2c4:
 if (f & 8)
  {
   fl=0;
      //AK! add
   if ( i1 >= 0  && i2 >=  0 && i3 >=  0 &&
	    i1 < 128 && i2 < 128 && i3 < 128 )
   {
	   if (r_abr[i1] < r_abr[i2]) goto d2v4;
	   if (r_abr[i1] == r_abr[i2]) fl=1;
	   if (r_abr[i2] < r_abr[i3]) goto d2v4;
	   if (r_abr[i2] == r_abr[i3]) fl=1;
	   if (r_abr[i3] < r_abr[i4]) goto d2v4;
	   if (r_abr[i3] == r_abr[i4]) fl=1;
	   if ((fl) && (r_abr[i1]  == r_abr[i3])) goto d2v4;
   }
   fl=0;
      //AK! add
   if ( j1 >= 0  && j2 >=  0 && j3 >=  0 &&
	    j1 < 128 && j2 < 128 && j3 < 128 )
   {
	   if (bot[j1] < bot[j2]) goto d2v4;
	   if (bot[j1] == bot[j2]) fl=1;
	   if (bot[j2] < bot[j3]) goto d2v4;
	   if (bot[j2] == bot[j3]) fl=1;
	   if (bot[j3] < bot[j4]) goto d2v4;
	   if (bot[j3] == bot[j4]) fl=1;
	   if ((fl) && (bot[j1]  == bot[j3])) goto d2v4;
   }
  }
 if ((f & 0x40)) pen+=20;
 goto a2c5;
 d2v4: if (!(f & 0x40)) pen+=20;
 a2c5:
 goto getout;
getout:
 if ( letter == 'o' )
 {
  // if BOX confirmed this letter than horisontal scan penalty
  // has to be eliminated.
  if ( (scl->recsource & c_rs_BOX) && ( cprob > 220 ) )
    pen -= cv;
  // check bint_index ( ratio total number of black intervals of
  // component to it's full height; it is computed by 1/64 );
  wint = ( total_num_black_int << 6 ) / fullh;
  i = wint - Let_bint_index(letter);
  if ( i > 0 )
    { pen +=  i * PNL_BINT_INDX; scl->reasno = c_rn_bint_idx; }
 }
 return new_prob( pen );
}

uchar left_angle_braket_filt()
{
int16_t     pen;
uchar    tresh;

    pen = 0;
    tresh = (uchar)prop_index( fullh, fullw );
    if ( ( tresh > 76 ) && ( tresh < 19 ) )
    {
        pen += 160;
        goto LAB_Ret;
    }
    tresh = fullh/25;

{
    uchar mid_bound1, mid_bound2;
    mid_bound1 = mid_bound2 = (uchar)midh;
    if ( (fullh & 1) == 0 )         // if height is even, then mid_bounds are 2
        mid_bound1 -= 1;
    pen += test_against_convex( l_abr, tresh, mid_bound1 );
    pen += test_against_convex( l_abr, mid_bound2,
                                    (uchar)MIN( fullh-1, fullh-tresh ) );
}

{
    int16_t lcv;
    tresh = MIN( 2, fullh/20 );
    lcv = constancy_vally_lth( r_abr, (uchar)(midh - (midh>>1)),
                                        (uchar)(midh + (midh>>1)), tresh );
    if ( lcv > midh - (midh>>2) )
        pen += 48;
}

    if ( fullw > 8 )
    {
        make_histo( wid, (uchar)fullh );
        if ( fullw - histo_max_value <= ( midw>>1 ) )
            pen += 60;
    }
LAB_Ret:
    return  new_prob(pen);
}


uchar right_angle_braket_filt()
{
int16_t     pen;
uchar    tresh;

    pen = 0;
    tresh = (uchar)prop_index( fullh, fullw );
    if ( ( tresh > 76 ) && ( tresh < 19 ) )
    {
        pen += 160;
        goto RAB_Ret;
    }

    tresh = fullh/25;

{
    uchar mid_bound1, mid_bound2;
    mid_bound1 = mid_bound2 = (uchar)midh;
    if ( (fullh & 1) == 0 )         // if height is even, then mid_bounds are 2
        mid_bound1 -= 1;
    pen += test_against_convex( r_abr, tresh, mid_bound1 );
    pen += test_against_convex( r_abr, mid_bound2, (uchar)MIN( fullh-1, fullh-tresh ) );
}
{
    int16_t lcv;
    tresh = MIN( 2, fullh/20 );
    lcv = constancy_vally_lth( l_abr, (uchar)(midh - (midh>>1)),
                                        (uchar)(midh + (midh>>1)), tresh );
    if ( lcv > midh - (midh>>2) )
        pen += 48;
}
    if ( fullw > 8 )
    {
        make_histo( wid, (uchar)fullh );
        if ( fullw - histo_max_value <= ( midw>>1 ) )
            pen += 60;
    }
RAB_Ret:
    return  new_prob(pen);
}

uchar ss_filt()
{
uchar wi, wi1, wi2;
int16_t pen, degree;

    pen = 0;
    wi = test_bottom_corner( l_abr, &degree );
    if ( wi == BC_CONVEX )
        pen += degree;

    if ( ( bases.b3 - bases.b2 > 2 ) &&
            ( scl->row + fullh - bases.b3 >= 2 ) )
    /* hanging down tail case */
    {
        wi1 = bases.b3 - scl->row + 1;
        wi =  MAX( ((fullh + 9)/10), 4 );
        wi2 = MIN( fullh-1, wi1 + wi );
        for ( wi = wi1; wi < wi2; wi++ )
        {
			//AK! add
            if ( wi < 128 && wi >= 0 && linh[wi] > 1 )
                pen += 20;
        }
        /* prepare zone's margines for top abris testing */
        wi1 = midw - 1;
        wi2 = fullw - 2;
    }
    else
    /* the letter 'ss' stays in the third base line */
    {
        wi = test_bottom_corner( r_abr, &degree );
        if ( ( wi == BC_CONCAVE ) || ( wi == BC_SERIF ) )
            pen += degree;

        /* prepare zone's margines for top abris testing */
        wi1 = midw - (midw>>1);
        wi2 = midw + (midw>>1);
    }

    get_max_min( top, wi1, wi2, &tmax, &tmin1, &txmax1, &txmin11 );
    if ( tmax - tmin1 > midh )
        pen += 120;
    return new_prob( pen );
}

uchar k_filt()
{
uchar wi, wi1, fl;
int16_t pen, degree, numi_mode_val;

   pen = 0;
   wi = test_bottom_corner( l_abr, &degree );
   if ( wi == BC_CONVEX )
       pen += degree;
   wi = bases.b2 - scl->row;
   //AK! add
   if ( wi < 128 && wi >= 0 )
	numi_mode_val = make_histo( &linh[wi], (uchar)(fullh - wi - 1) );
   if ( ( histo[1] > 1 ) || ( histo_max_value >= 3 ) )
   {
       get_max_min( top, (uchar)((midw>>1)), (uchar)(midw + 1), &tmax,
                                        &tmin1, &txmax1, &txmin11 );
       if ( tmax > ( wi + ((bases.b3 - bases.b2)>>1) )  )
       //if ( tmax - ( wi + ((bases.b3 - bases.b2)>>1) ) > (fullh/10) )
          pen += 60;
       goto KF_Ret;
   }
   if ( (numi_mode_val == 2) && ( histo_max_value == 2 ) && ( histo[1] == 0 ) )
   {
      wi += 2;
      get_max_min( r_abr, wi, (uchar)(fullh - 1), &rmax, &rmin1, &rxmax1, &rxmin11 );
      fl = monotonous_increase( r_abr, wi, rxmax1, &wi1, 0 );
      if ( fl == NOT_FOUND_MON_INCR )
          pen += 40;
      fl = monotonous_decrease( r_abr, rxmax1, (uchar)(fullh - (fullh>>3)), &wi1, 0 );
      if ( fl == NOT_FOUND_MON_DECR )
          pen += 40;
      for ( wi1 = 0; wi1 < fullh && wi1 < 128; wi1++ )
        histo[wi1] = wid[wi1] - sumh[wi1];
      wi1 = ( fullh>>3 );
      get_max_min( histo, wi, (uchar)(fullh - wi1), &bmax, &bmin1, &bxmax1, &bxmin11 );
      if ( bmax - bmin1 <= 1 )
        pen += 80;
   }
KF_Ret:
   return new_prob( pen );
}

#define PNL_X_NOSIM 40
#define PNL_X_FLAT_SIDE 40
uchar x_filt()
{
int16_t wi, tresh;
int16_t pnl;

   pnl = 0;

  make_left_max();
  make_right_max();
// Check left and right sides to be not flat ( avoiding double 'o' (H15/5) )
  if ( ljmp == 0 )
   pnl += PNL_X_FLAT_SIDE;
  if ( rjmp == 0 )
   pnl += PNL_X_FLAT_SIDE;
// Check italic case ( italic letters have not simmetrical covers )
    make_histo( linh, (uchar)fullh );         // generates histo_max_value variable
    if ( (histo_max_value >= 3) &&     // 3 int-s and more vote for italic 'x'
         ( (histo[3] > 1) || (histo[4] > 1) )  )
        goto    XF_Ret;
// Check left and right extremums to be simmetrical ( avoiding 'z' )
  tresh = MAX( get_size() / 5, 1 );
  wi = abs (lxmax1 - rxmax1);
  if ( wi > tresh )
   pnl += ( wi - tresh ) * PNL_X_NOSIM;
XF_Ret:
 return new_prob( pnl );
}

uchar d_filt()
{
int16_t twoint, lgap, i;
uchar h14, h23;
uchar b_max, b_min, b_x_max, b_x_min, b_end;
int16_t pnl;

  twoint = 0;   pnl = 0;

// check two black intervals at upper quarter ( avoiding "doubl l", "ft" etc. )
 pnl += two_int_over_b2(scl,&twoint);
 if (twoint < 0)    // 2 intervals not found
   { pnl = 250; goto  DF_Ret; }
 h14 = ( (fullh - 2) >> 2) - 1;
 h23 = ( fullh << 1 ) / 3 ;

// check too large black intervals at lower third ( avoiding "al" );
//AK! add
  if ( h23 >= -1 && h23 < 127 &&
	   h14 >= 1  && h14 < 129 &&
	   sumh[h23 + 1] > ( (sumh[h14-1] + 1) << 1 ))
   pnl += 10;

// check the upper gap  ( avoiding stick tuching anything at left side)

 if ( db_pass == 'b' ) goto DF_around;   // if step is glue - no checking
 make_top_max();
 lgap = tmax - twoint;
 if ( lgap < 0 ) goto DF_around;
 if ( lgap > (fullh-twoint)/3 )  // there is a long chink near right stick
  {
   if ( fax1x2 )
    {
     int16_t j,k;
	 for ( j=0, k=0, i=MAX(twoint,0); ((i < fullh-2) && k<5 && i < 127); k++,i++ )
      {
        if ( ( linh[i] >= 3 ) && ( linh[i+1] >= 3 ) )
         j++;
      }
     if ( j >= 2 )
      pnl += 80;
    }
   else    // not fax case
    {
      if ( ( txmax1 > (midw>>1) ) && ( txmax1 < midw ) )
       pnl += 60;
    }
  }
DF_around:
// Check arounding of the left abris
 for ( i=MAX(twoint,0); ( i < (fullh-2) && i < 127 ); i++ )
  {
   if ( l_abr[i] - l_abr[i+1] > 3 )
    pnl += 40;
   if ( l_abr[i] - l_abr[i+1] < -3 )
    pnl += 40;
  }
 // check bottom abris to be unbroken ( avoiding "cl" glued pair )
  i = MAX ( fullw/10, 2 );         // admissable tresh
  //AK! add
  if (fullh >=3 && fullh < 131 )
      b_end = fullw - r_abr[fullh-3];  // avoiding italic
  get_max_min( bot, (uchar)(midw>>1), b_end,
               &b_max, &b_min, &b_x_max, &b_x_min );
  lgap = b_max - b_min;
  if ( ( lgap > i ) && ( (fullh - b_max) - twoint < h14 ) )
   {
    uchar p,q;
    for ( q = 0, p = MIN(b_x_max, 127); ( q<10 && p>(midw>>1) && p >= 0); p-- )
     {
      if ( bot[p] - b_min > i )
       q++;
     }
    q = q - (i - 1);
    if ( q > 0 ) pnl += q * GAP_IN_SIDE_PNL;
   }

 // check bint_index ( ratio total number of black intervals of
 // component to it's full height; it is computed by 1/64 );
 wint = ( total_num_black_int << 6 ) / fullh;
 i = wint - Let_bint_index(letter);
 if ( i > 0 )
    { pnl +=  i * PNL_BINT_INDX; scl->reasno = c_rn_bint_idx; }
 DF_Ret:
 return new_prob( pnl );
}

uchar v_filt()
{
int16_t   pen;
uchar  wi1, wi2;

    pen = 0;  wi1 = wi2 = (fullh/6);

    return (uchar)cprob;
    //return new_prob( pen );
}

uchar q_filt()
{
int16_t pnl, wi;
uchar  i, extr_min, extr_min_pos, dif, wpos;

 pnl = 0;

// Check right abris at bottom against rounding (avoiding "g" )
 dif = test_bottom_corner( r_abr,  &wi );
 if ( dif == BC_CONVEX )
  pnl += wi;
   //AK! add
   if (fullh >=3 && fullh < 131 )
      wi = fullw - r_abr[fullh-3];  // avoiding italic
   for (extr_min_pos = 1, extr_min = 255, i = MIN(wi,127); (i > midw && i >= 0 ); i--)
    {
      if ( bot[i] < extr_min )
       { extr_min = bot[i];  extr_min_pos = i; }
    }

   wpos = fullw - wi - 1;
   extr_min_pos = MIN ( extr_min_pos, wpos );

   dif = MAX( get_size()/10, 1 );
   for ( i = MIN(extr_min_pos,127); i > 0; i-- )
    {
      if ( (bot[i] - extr_min) > dif + 1 )
       break;
    }

   if ( ( extr_min_pos - i ) > (midw - 1) )
    pnl += 80;

 return new_prob( pnl );
}

uchar Dig_6_filt()
{
  int16_t   pen;
  uchar  i, sumb, max_sum, min_sum, max_x_sum, min_x_sum;

  pen = 0;
  get_max_min( r_abr, 0, (uchar)(midh + 1), &rmax, &rmin1, &rxmax1, &rxmin11 );
  for ( sumb = 0, i = MAX(rxmax1,0); (i < midh && i < 128 ); i++ )
  {
    if ( linh[i] != 1 )
      break;
    else
      sumb = i;
  }
  if ( sumb == 0 ) goto D6F_Ret;
  if ( (rxmax1*10) > (fullh * 3) )       // accent size > 3/10 of height
    goto D6F_Ret;                        // it means that symb. is 6 for shure

  get_max_min( sumh, 1, (uchar)(sumb + 1), &max_sum, &min_sum, &max_x_sum, &min_x_sum );
//AK! add
  if  ( rxmax1 >= 0 && rxmax1 < 128 && min_x_sum >= 0 && min_x_sum < 128 &&
	    sumb >=0 && sumb < 128 &&
	    (( sumh[rxmax1] == min_sum ) || ( r_abr[min_x_sum] == rmax ) ) &&
        ( l_abr[rxmax1] > midw - (fullh/10) ) &&
        ( 2 * min_sum <= max_sum ) && (3 * min_sum <= sumh[sumb] ) )
  {
    pen += 80;
    goto D6F_Ret;
  }
  // avoiding symbols "”" with glued dot
  get_max_min( sumh, 1, rxmax1, &max_sum, &min_sum, &max_x_sum, &min_x_sum );
//AK! add
  if ( rxmax1 >= -1 && rxmax1 < 127 && min_x_sum >= 0 && min_x_sum < 128 &&
	   ( max_sum - min_sum > (fullh>>3) ) &&
       ( abs(min_x_sum - rxmax1) <= fullh/10 ) &&
       ( ( l_abr[min_x_sum] > l_abr[rxmax1+1] ) &&
         ( l_abr[min_x_sum] > l_abr[max_x_sum] ) ) )
    pen += 60;

D6F_Ret:
  return new_prob( pen );
}

uchar E_filt()
{
uchar tbc;
int16_t pen;

    pen = 0;
    tbc = monotonous_decrease( l_abr, (uchar)(midh - (midh>>1)), (uchar)(midh + (midh>>1)),
                                    &wbyte, (uchar)MAX( 1, fullh/20 ) );
    if ( !( tbc == SMOOTH_FUNC ) || ( tbc == FOUND_MON_DECR ) )
        pen += 60;

    tbc = test_bottom_corner( l_abr,  &wint );
    if ( tbc == BC_CONVEX )
    {
      if ( ! ( scl->cg_flag & c_cg_cut ) )
        pen += wint;
    }
    return new_prob( pen );
}

int16_t h_filt(cell *c, int16_t prob, s_glue *GL, uchar h_or_b_Let)
{
 lnhead *Lp1, *Lp2;
 interval *int1;
 c_comp *cp1;
 cell *a;

 uchar intbeg1, intend1, intlth1,  intbeg2, intend2, intlth2, fax_corr;
 uchar h14, h23;
 int16_t  Lc1, lc1, pnl, nc,
      hbeg, hend, rowd, cold;

 cprob=prob;
 nc = 0; pnl = 0;
 // ALLD 940331 ... fullh = c->h;
 letter = h_or_b_Let, fax_corr = 0;
 if ( fax1x2 ) fax_corr = 2;

 if (abriflag==0)  make_abris(GL,c);
 if (abriflag==0)  return prob;

 while ((nc < MAX_CELLS_IN_LIST && nc >= 0 && (a=GL->celist[nc++]) != NULL ))
 {
  if (a->env == NULL) return prob;
  rowd=a->row - c->row;  // cell_to_box_relative row
  cold=a->col - c->col;  // cell_to_box_relative col
  cp1 = a->env;
  Lc1=cp1->nl;      // number of lines in component
  Lp1=(lnhead *) ( (char *)cp1 + cp1->lines + 2); // beginning of first line
  Lp2=Lp1;
  for (lc1=0; lc1 < Lc1; lc1++)
     {
      if ((Lp1->flg & l_fbeg)==0) goto nextl;  // need free_beg line
      hbeg = Lp1->row + rowd;
      hend = hbeg + Lp1->h;
      if ((hend > c->h/2) || (Lp1->h < 4)) goto nextl;
      int1=(interval *)(Lp1+1);		// ptr to first interval
      intend1=int1->e+cold;
      intlth1=int1->l;
      intbeg1=intend1-intlth1;
      if (intbeg1 < c->w/2) goto nextl;
      int1+=(Lp1->h-1);
      intend2=int1->e+cold;
      intlth2=int1->l;
      intbeg2=intend2-intlth2;
      if (hbeg >= (c->h/4)) goto nextl;       // starts low enough
      if ((intlth2-intlth1) <= (Lp1->h+2)/3) goto pen70;  // doesn't extend to bottom
      if (intbeg2 < intbeg1)
	if (intend2 <= intend1) goto nextl;
  pen70:
      pnl += 70;	// PENALIZE 'h' or 'b'

    nextl:
      Lp2=Lp1=(lnhead *) ((char *)Lp2+Lp2->lth);   // next line
     }                                            // for all lines
  }                                               // while all cells


// check two black intervals at upper quoter ( avoiding "double l" )
  pnl += two_int_over_b2(c,&wint);
// check the "width" of the component to be invariable at middle zone
  intbeg1 = wint + 1;       // right here two intervals take place
  intend1 = wint + 5;
  rowd = 2;
  if ( fullh > 40 ) rowd = 3;
  if ( fullh < 20 ) rowd = 1;
//AK! add
  if ( intbeg1 < 128 && intbeg1 >= 0 && intend1 < 128 && intend1 >=0 &&
	   ( fullh + fax_corr < 20  ) &&
       abs( (wid[intbeg1] - sumh[intbeg1])-(wid[intend1] - sumh[intend1]) ) > rowd )
   pnl += 40;
  get_max_min ( wid, intbeg1, intend1, &intlth1, &intlth2, &intbeg2, &intend2);
// parametrs:   func, from,   to,       fmax,     fmin,     x_fmax,   x_fmin )
   //AK! add
  if ( wint < 128 && wint >= 0 && wid[wint] - intlth2 > rowd )
  pnl += 40;

 h14 = ( (fullh - 2) >> 2 ) - 1;
 h23 = ( fullh << 1 ) / 3 ;
// check too large black intervals at lower third ( avoiding "la" );
// this checking is admissible only for non-glued components

  if ( db_pass == 'b' ) goto UpperGap;   // if step is glue - no checking
  //AK! add
  if ( h23 >= -1 && h23 < 127 &&
	   h14 >= 1  && h14 < 129 &&
	   sumh[h23 + 1] > ( (sumh[h14-1] + 1) << 1 ) )
   pnl += 10;

// check the upper gap  ( avoiding stick tuching anything at right side)
UpperGap:

 make_top_max();
 if ( ( tmax > h23 )  &&  ( txmax1 < ( c->w >> 1 ) ) )
   pnl += 20;
 // check bint_index ( ratio total number of black intervals of
 // component to it's full height; it is computed by 1/64 );
 wint = ( total_num_black_int << 6 ) / fullh;
 nc = wint - Let_bint_index('h');
 if ( nc > 0 )
    { pnl +=  nc * PNL_BINT_INDX; c->reasno = c_rn_bint_idx; }
// For letter 'b' ONLY! Check bottom right corner to be without serif
 if ( letter == 'b' )
  {
    Lc1 = test_bottom_corner( r_abr,  &lc1 );
    if ( ( Lc1 == BC_SERIF ) || ( Lc1 == BC_CONCAVE ) )
     pnl += lc1;
  }
 return new_prob( pnl );
}


uchar CR_filt()
{
 int16_t  pen;
 int16_t  dev, gap;
 uchar flit, i3, i1, i2;

 pen=0; flit = 0;

 i2 = (uchar)midh;  i1 = fullh >> 3;  i3 = fullh - i1;
 dev = MAX( fullh / 10, 1 );

 // test right side against gap ( avoiding any garbege from BOX )
    if ( ncells == 1 )
    {
        i1 = fullw >> 2;  i2 = fullw - i1;
        gap = gap_in_side( i1, i2, TOP_S, dev, &flit );
        if ( gap > 0 ) pen += gap * GAP_IN_SIDE_PNL;
    }

 // test left side against gap ( avoiding any garbege from BOX )
    if ( ncells == 1 )
    {
        i1 = fullw >> 2;  i2 = fullw - i1;

        gap = gap_in_side( i1, i2, BOTTOM_S, dev, &i3 );
        if ( gap > 0 )
            pen += gap * GAP_IN_SIDE_PNL;
    }

 // test right side against gap ( avoiding any garbege from BOX )
    if ( ncells == 1 )
    {
        i1 = fullh >> 2;  i2 = fullh - i1;
        gap = gap_in_side( i1, i2, RIGHT_S, dev, &flit );
        if ( gap > 0 ) pen += gap * GAP_IN_SIDE_PNL;
    }

 // test left side against gap ( avoiding any garbege from BOX )
    if ( ncells == 1 )
    {
        i1 = fullh >> 2;  i2 = fullh - i1;

        gap = gap_in_side( i1, i2, LEFT_S, dev, &i3 );
        if ( gap > 0 )
            pen += gap * GAP_IN_SIDE_PNL;
    }

 return new_prob( pen );
}


uchar yu_filt()
{
 int16_t  pen;
 int16_t  dev, gap;
 uchar flit, i3, i1, i2;

 pen=0; flit = 0;

 i2 = (uchar)midh;  i1 = fullh >> 3;  i3 = fullh - i1;
 dev = MAX( fullh / 10, 1 );

    if ( ncells == 1 )
    {
        i1 = fullw >> 2;  i2 = fullw - i1;

        gap = gap_in_side( i1, i2, BOTTOM_S, dev, &i3 );
        gap -= (fullh*8/3);
        if ( gap > 0 )
            pen += gap * (GAP_IN_SIDE_PNL/4);
        if( pen > 50 )
          pen = 50;
    }

 return new_prob( pen );
}

