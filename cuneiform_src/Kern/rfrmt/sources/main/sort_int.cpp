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

#ifdef _MSC_VER
#include<crtdbg.h>
#endif

//----Unrecursive sorting----
//#include <time.h>
#include "lst3_win.h"

#include <string.h>
#include <stdlib.h>
//#include <math.h>

#include "wind32.h"

static  int  w ;
static  int  (*c)(char *, char *);

#define MAX_KEY_SIZE 100
static  char  flip_buffer[MAX_KEY_SIZE+4] ; /*¬ ЄбЁ¬ «. а §¬Ґа. н«-в  ¤ ­­ле*/
#define MAX__DEPTH 300 /* Maximal depth of recursion */
static char* base_arr[MAX__DEPTH] ; /* base buffer */
static int num_arr[MAX__DEPTH] ; /* num buffer */
static char **base_ptr, **base_start_ptr, **base_end_ptr ;
static int* num_ptr ;

//flip - ®Ў¬Ґ­ Ї ал н«-в®ў ¬Ґбв ¬Ё
static  void  flip(char*, char *) ;
static  void  quick_sort( char *, int ) ;
static void  flip(char *a,char *b)
{  memcpy( flip_buffer, a, w ) ;
   memcpy( a, b, w ) ;
   memcpy( b, flip_buffer, w ) ;
}
/*====*/
//void  u4sort(void *base, int  num, int  width, COMP_FUN (*compare()))
void  u4sort(void *base2, int  num, int  width, int  (*compare)())
{
   w      =  width;
   c      =  (int (*)(char *, char *)) compare;
   quick_sort( (char *) base2, num ) ;
}

#define LOWORD(l) ((WORD)(l))
int compare(TYPE *a,TYPE *b) { return (*a >= *b ? 1:-1);}
int comp_left(FRAME **a,FRAME **b) { return ((*a)->left >= (*b)->left ? 1:-1);}
int comp_vert(FRAME **a,FRAME **b) { return ((*a)->up >= (*b)->up ? 1:-1);}
int comp_leftDesc(FRAME **a,FRAME **b) {return (-(*a)->left >= -(*b)->left ? 1:-1);}
int comp_vertDesc(FRAME **a,FRAME **b) {return (-(*a)->up >= -(*b)->up ? 1:-1);}
int comp1(TYPE *a,TYPE *b) { return (*a >= *b ? 1:-1); }
int compF(float *a,float *b) { return (*a >= *b ? 1:-1); }
int comp_long(DWORD *a,DWORD *b) {return (LOWORD(*a) >= LOWORD(*b) ? 1:-1);}
int comp1_long(DWORD *a,DWORD *b) {return ( *((uint*)a) >= *((uint*)b) ? 1:-1);}
/*  *((uint*)a+1) - нв® бв аиҐҐ б«®ў®  */
/* ЎЁ­ а­л© Ї®ЁбЄ ў ®вб®авЁа®ў ­­®¬ ¬ ббЁўҐ    */
int search_int(int *x,int n,int a)
{ int left,right,middle;
  if(a < x[0]) return 0; if(a > x[n]) return n+1;
  left=0; right=n;
  while(right-left > 1)
   { if(  a < x[ (middle=(right+left)>>1) ]  ) right=middle; else left=middle;}
  return left+1;
}

static void quick_sort(char *base1,int num1)
{ int num, num_smaller ;
  char *base, *smallQ, *largeQ;
  base_start_ptr = base_arr ;
//  base_end_ptr = base_start_ptr + MAX__DEPTH + 1 ;
		base_end_ptr = base_start_ptr + MAX__DEPTH;  //071299
  base_ptr = base_start_ptr + 1 ;
  num_ptr = (int*) num_arr ;
  num_ptr++ ;
  *base_ptr = base1 ;
  *num_ptr = num1 ;
  do
  {
   base = *base_ptr ;
   num = *num_ptr ;
   if ( num < 3 )
   {
      if ( num == 2 )
         if ( (*c)(base, base+w ) > 0 )
            flip( base, base+w ) ;
      base_ptr-- ;
      num_ptr-- ;
   }
   else
   {
   largeQ =  base+  w*(num-1) ;
   smallQ =  base+  w*(num>>1) ;

   /* base should point to a middle value fewest recursions */
   if ( (*c)( smallQ, largeQ )  > 0 )   flip( smallQ, largeQ ) ;
   if ( (*c)( smallQ, base ) > 0 )
      flip( smallQ, base ) ;
   else
      if ( (*c)( base, largeQ ) > 0 )
         flip( base, largeQ ) ;

   if ( num == 3 )
   {
      flip( base, smallQ ) ;
      base_ptr-- ;
      num_ptr-- ;
   }
   else
   {

   smallQ =  base + w ;

   while ( smallQ < largeQ )
   {
      if ( (*c)( smallQ, base ) < 0 )
      {
         smallQ += w ;
         continue ;
      }

      do
      {
         if ( (*c)(base, largeQ) > 0 )
         {
            flip( smallQ, largeQ ) ;
            largeQ -= w ;
            smallQ += w ;
            break ;
         }
         largeQ -= w ;
      } while( smallQ < largeQ ) ;
   }

   if ( (*c)(smallQ,base) < 0 )   flip(smallQ,base) ;
   num_smaller =  (int) ((smallQ-base) / w) ;
   *num_ptr = num_smaller ;
   base_ptr++ ;
 //
	  if (base_ptr > base_end_ptr)
   {
      printf("\n Sorting Buffer Overflow") ;
     return ;
   } ;
   num_ptr++ ;
   *base_ptr = smallQ ;   //10000 bytes ml
   *num_ptr = num - num_smaller ;
  }
  }
  } while( base_ptr != base_start_ptr ) ;

   return ;
}
