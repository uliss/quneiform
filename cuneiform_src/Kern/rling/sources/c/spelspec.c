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

// 08-14-93 03:08pm, Mike
// This file has been modified to use the new stream technology.
//

  #include "spelmode.h"

  #if defined(TURBO_C)
    #include "tc_types.h"
  #elif defined(WATCOM)
    #include "spelwatc.h"
  #elif defined(THINK_C)
    #include "spelthin.h"
  #else
    #error   NO TOOOL SPECIFIED
  #endif

  #include "speldefs.h"
  #include "spelfunc.h"
  #include "tigeremulate.h"
//  #include "iolib.h"


#define SP_ABC_NO    12

typedef struct spec_abc {
		ArtVH           *    hd;
		uchar    *  *  pref;
		uchar            *  body;
		uchar    *  * postf;
} SABC;

SABC          SpABC[SP_ABC_NO];
ArtFH  *        SpABCroot;

/* ------------------------------------------------------------------ */

extern int32_t read_all_vtab(int16_t,char  *);

uchar  * load_specABC (uchar  *point, int16_t Country)
{
  uchar  *a;
  uchar  *  *b;
  long  size;
  int i,j;

  SpABCroot = (ArtFH  *)point;

// 08-14-93 03:03pm, Mike
// Reading *.art file ( art-dictionary ).
  size = read_all_vtab( 8, (char *)SpABCroot );
  if (size == -1L ) {
    #ifdef SYSPR_ERROR
      PRINTF("Unable to open %s \n", w);
    #endif
    return NULL;
  }

  for ( i = strlen(ARTFILE_ID) ; i < size ; i++ ) {
    *(point +i) ^= XOR_MASK;
  }

  if ( SpABCroot->voc_no >= SP_ABC_NO ) {
    #ifdef SYSPR_ERROR
      PRINTF("Too many specABC: %d\n", SpABCroot -> voc_no);
    #endif
    return (NULL);
  }

  a = point + sizeof(ArtFH);
  b = (uchar * *)(point + size);
  for ( i = 0; i < SpABCroot -> voc_no; i++) {
    SpABC[i].hd = (ArtVH *)a;
    a += sizeof (ArtVH);
    SpABC[i].pref = b;
    for ( j = 0; j < SpABC[i].hd->pref_no; j++) {
      *b = a;
      while( *(a++) );
      b++;
    }
    *(b++) = NULL;
    SpABC[i].body = a;
    while( *(a++) );
    SpABC[i].postf = b;
    for ( j = 0 ; j < SpABC[i].hd->post_no ; j++ ) {
      *b = a;
      while( *(a++) );
      b++;
    }
    *(b++) = NULL;
  }

 Country=0;
 return (uchar *)b;
}


/* ------------------------------------------------------------------ */

int16_t check_art_dict (char word[], int16_t * wordlth, int16_t * vockind)
{

      char                      no;
      char                    pref;
      char                   postf;
      char  CapWord[MAX_WORD_SIZE];
 register pchar             body_b;
      pchar                 body_e;
 register uchar             *p;
  word[*wordlth+1]=0;

  for ( p =word,body_b = CapWord; *p  ; p++,body_b++)
                         *body_b = _2cap (*p);
  *body_b = 0;

 /*for ( no= (strcmp ( CapWord, word) != 0 ) ? 2 : 0; no<SP_ABC_NO; no++ )*/

 /** Do not forget about Latyn **/

 /** The first (0) triple is always main dictionary prefs - postfs */

 for (no= 1 ; no <SpABCroot ->voc_no; no++)

    for(pref=0; (p=SpABC[no].pref[pref]) != 0 ;pref++)
    {
      for( body_b=CapWord ; *p && *body_b ; p++,body_b++)
      if( *body_b != *p) break;
      if( (!*p) && (*body_b) )
      {
        for(postf=0; (p=SpABC[no].postf[postf]) != 0 ;postf++)
        { char lth;
          for(body_e=CapWord+(*wordlth), lth=*wordlth+1;
                *p && lth ; p++,body_e--,lth--)
          {
            if(*body_e != *p ) break;
          }
    /*     if((!*p) && lth)                   Lepik -- 08/06/92 04:30pm  */
    /*                                                                   */
    /* This restriction prohibits 1st and 2nd.                             */

    /* The following permits 'TH'                                        */

          if (!*p)
          {
            for( ; body_b <= body_e; body_b++)
            { for(p=SpABC[no].body; *p ; p++)
                if(*body_b==*p) break;
              if (!(*p)) break;
            }
            if( body_b > body_e)
              {  *vockind=no+4;
                return SpABC[no].hd ->relabty;
              }
          }
        }
      }
    }
  return 0;
}

/* ------------------------------------------------------------------ */

int16_t test_apppostrof (uchar word[],SWORD *wrd, int16_t *l, int16_t *r)
 {
  int16_t       i, pref, apf, postf;
  uchar       *p,  *pp;
  char   CapWord[MAX_WORD_SIZE];

  for ( p =word, pp = CapWord ; (*p) != 0  ; p++, pp++)
                         *pp = _2cap (*p);
  *pp = 0;

  *l = 0;
  *r = wrd -> lth;

  for ( apf  = 0; apf < wrd -> lth; apf++)
        if (CapWord[apf] == 0x27)             break;
    //  if (wrd ->pos[apf] ->type_sp & T_APF)             break;
  if (apf == wrd -> lth)
				             	                        return FALSE;

  for(pref =0; (p=SpABC[0].pref[pref]) != 0 ; pref++)
    {
	  for (i = 0; *(p+i) && CapWord[i]; i++)
		 if ( *(p+i) != CapWord[i]) break;
	                                                 /* vowel */
	  if ( (!*(p+i)) &&  (i == apf) &&(i))
		if (IsVowel(CapWord[i+1]))
		  {
		   *l =i + 1;
		   break;
		  }
	}

  for ( i  = wrd -> lth -1; i >apf; i--)
        if (CapWord[i] == 0x27)             break;
      //if (wrd ->pos[i] ->type_sp & T_APF)                  break;

  if ( (i == apf) && *l)                             return TRUE;

  apf = i;

  for(postf =0; (p=SpABC[0].postf[postf]) != 0 ; postf++)
   {
	  for (i = 0; *(p+i) && (wrd -> lth -i-1); i++)
		 if ( *(p+i) != CapWord[wrd ->lth -i -1])          break;
	  if ( (!*(p+i)) &&  (wrd -> lth -i -1 == apf) && (i !=0))
		  {
		   *r =apf;
		   break;
		  }

   }
 return  (*r != wrd -> lth) || (*l);
 }
