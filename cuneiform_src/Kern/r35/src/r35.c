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

#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
/*#include <io.h>*/
#include <sys/stat.h>
#include <stdio.h>
#include <math.h>

#include "r35.h"
#include "mmx.h"
#include "std.h"

#include "compat_defs.h"
#include "minmax.h"

#define MTR_LEARN1

#define NAME        "hnd3rus.dat"
#define NAME_NDX    "hnd3ind.dat"
#define NAME_PRINT  "rec3r&e.dat"

#define PC_TYPE 0
#define R35_DEBUG

#ifdef PC_TYPE

static void   MMX_normalize_res(uint32_t res_comp[],uint16_t res[],int32_t numx);
#endif
static int32_t  scalar_all(int16_t Im3x5_1[],int16_t Im3x5_2[]);
static void   ALL_open_image(uint16_t Im3x5[]);
static void   ALL_close_image (void);

static int32_t (*scalar)(int16_t Im3x5_1[],int16_t Im3x5_2[])=scalar_all;
static void  (*open_image)(uint16_t Im3x5[])=ALL_open_image;
static void  (*close_image)(void)=ALL_close_image;


static void* r35_alloc(uint32_t len){return malloc(len);}
static void  r35_free(void *ptr,uint32_t len){ free(ptr);}

static void* (*my_alloc)(uint32_t len)=r35_alloc;
static void  (*my_free)(void *,uint32_t len)=r35_free;
/***********************************/
/************* COMPRESS ************/
/***********************************/
static int32_t   r35_pack(uchar raster[],
        int32_t D_X, int32_t X, int32_t Y, int32_t dx, int32_t dy,
        uint16_t Image3x5[], int32_t to_x, int32_t to_y);
static int32_t   r35_pack_gray(uchar raster[],
        int32_t D_X, int32_t X, int32_t Y, int32_t dx, int32_t dy,
        uint16_t Image3x5[], int32_t to_x, int32_t to_y);
static void r35_set_cpu(int32_t cpu);
static Bool32   R35Pack_resize(   RecRaster*  raster,
        uint16_t* Image3x5, int32_t to_x, int32_t to_y, Bool32 proport);

/***********************************/
/************** INIT ***************/
/***********************************/

typedef struct elidx
 {
 uchar  ltr;                  /* the letter                            */
 uchar  vnum;                 /* the number of the classes           */
 uint16_t numel;                /* the number of the first class     */
 } ind3x5;

typedef struct eltab
 {
 uchar  ltr;                  /* the letter                      */
 uchar  fnt;                  /* the font number               */
 uint16_t list;                 /* next of same letter         */
 uint16_t bnd;                  /* the bound of the typeface */
 uint16_t vect[15];             /* the vector of the letter */
 } elm3x5;

static Bool32 r35_init(char *, elm3x5 **, ind3x5 **);
static Bool32 r35_save(char *name,elm3x5 *tab3x5this, ind3x5 *header3x5this, int num);
static Bool32 r35_delete_elm3x5(uchar let, int32_t   num_del);
static Bool32 r35_close( elm3x5 **, ind3x5 **);
static int32_t r35_cpu=0;
static uchar r35_learn_level=0;
#ifdef MTR_LEARN
static r35_fnt=0x80;
#else
static r35_fnt=0;
#endif
static  int      num_learn=0, max_learn;
/************************************/
/************** RECOG ***************/
/************************************/

typedef struct
    {
    uchar   let;
    int32_t   prob;
    int32_t   num;
	} VERSION;

#define R35_MAX_VERS 4
static Bool32 recog_cluster(uint16_t Im3x5[], uchar let, VERSION *v);
static Bool32 recog_cluster5x3(uint16_t Im3x5[], uchar let, VERSION *v);
static void r35_recog_full(uint16_t Im3x5[],VERSION vers[], int32_t *nvers);
static void r35_recog_full_prn(uint16_t Im3x5[],VERSION vers[], int32_t *nvers);
static void r35_recog(uint16_t Im3x5[],VERSION vers[], int32_t nvers, Bool32 r5x3);


/***********************************/
/************* COMPRESS ************/
/***********************************/
static void   ALL_normalize_res(uint32_t res_comp[],uint16_t res[],int32_t numx);
static void   normalize_res_3x5(uint32_t res_comp[],uint16_t res[],int32_t numx);
static void   (*normalize_res)(uint32_t res_comp[],uint16_t res[],int32_t numx)=ALL_normalize_res;
static Bool32 delete_elm3x5(uchar let, int32_t   num_del);
static int32_t  MakeScale(int32_t Xcut[],int32_t Xval[],int32_t L,int32_t dL);

static void   ALL_compress_line(uchar *lin,int32_t nx,int32_t *buf_comp,int32_t numx,int32_t Xcut[],int32_t Xval[]);
static void   ALL_compress_gray_line(uchar *lin,int32_t nx,int32_t *buf_comp,int32_t numx,int32_t Xcut[],int32_t Xval[]);
static int32_t  ALL_SumBits(uchar *str,int32_t start, int32_t stop);
static int32_t  ALL_SumGray(uchar *str,int32_t start, int32_t stop);
static void   (*compress_line)(uchar *lin,int32_t nx,
							   int32_t *buf_comp,int32_t numx,
							   int32_t Xcut[],int32_t Xval[])=
							   ALL_compress_line;

static void   ALL_addcomp(uint32_t res_comp[],int32_t buf_comp[],int32_t numx,int32_t numy);
static void   (*addcomp)(uint32_t res_comp[],int32_t buf_comp[],
						 int32_t numx,int32_t numy)=ALL_addcomp;

/* ******************************* */
static const uint32_t mask_word32[] =
	{128,     64,   32,   16,    8,    4,    2,   1};
static const uchar mask_byte[]   =
	{128,     64,   32,   16,    8,    4,    2,   1};
static const uint32_t mask_l[]     =
	{255,    127,   63,   31,   15,    7,    3,   1};
static const uint32_t mask_r[]     =
	{  0,    128,  192,  224,  240,  248,  252,  254};

static const uint32_t bit_cnt[]  ={
0,1,1,2,1,2,2,3,    1,2,2,3,2,3,3,4,    1,2,2,3,2,3,3,4,
2,3,3,4,3,4,4,5,	1,2,2,3,2,3,3,4,	2,3,3,4,3,4,4,5,	2,3,3,4,3,4,4,5,
3,4,4,5,4,5,5,6,	1,2,2,3,2,3,3,4,	2,3,3,4,3,4,4,5,	2,3,3,4,3,4,4,5,
3,4,4,5,4,5,5,6,	2,3,3,4,3,4,4,5,	3,4,4,5,4,5,5,6,	3,4,4,5,4,5,5,6,
4,5,5,6,5,6,6,7,	1,2,2,3,2,3,3,4,	2,3,3,4,3,4,4,5,	2,3,3,4,3,4,4,5,
3,4,4,5,4,5,5,6,	2,3,3,4,3,4,4,5,	3,4,4,5,4,5,5,6,	3,4,4,5,4,5,5,6,
4,5,5,6,5,6,6,7,	2,3,3,4,3,4,4,5,	3,4,4,5,4,5,5,6,	3,4,4,5,4,5,5,6,
4,5,5,6,5,6,6,7,	3,4,4,5,4,5,5,6,	4,5,5,6,5,6,6,7,	4,5,5,6,5,6,6,7,
5,6,6,7,6,7,7,8};

/************************************/
/************** RECOG ***************/
/************************************/

static void  store_bests(VERSION *vers, VERSION *ver);
static uchar recode_prob( int32_t prob );


/***********************************/
/*************** DATA **************/
/***********************************/
static ind3x5 *header3x5,*header3x5ndx,*header3x5all,*header3x5print;
static elm3x5 *tab3x5,*tab3x5ndx,*tab3x5all,*tab3x5print;
static char   alpha[256];
static int16_t  r35_error_code=ER_R35_NO_ERROR;

/***********************************/
/************* COMPRESS ************/
/***********************************/
int32_t MakeScale(int32_t Xcut[], int32_t Xval[], int32_t L, int32_t dL) {
    int32_t i=0;
    int32_t nu=0;
    if (dL > L)
        return 0;

    Xcut[0] = -1;
    Xval[0] = 0;
    while (i < L) {
        if ((i + 1) * dL > (nu + 1) * L) {
            nu++;
            Xcut[nu] = i, Xval[nu] = nu * L - i * dL;
        };
        i++;
    }
    Xcut[dL] = L;
    Xval[dL] = 0;
    return 1;
}

int32_t ALL_SumBits(uchar *str,int32_t start, int32_t stop)
{
int32_t l, r, i, res;

l=start>>3, r=stop>>3;
if( l==r )
	return bit_cnt[ mask_l[start&7] & str[r] & mask_r[stop&7] ];

res = bit_cnt[ str[l] & mask_l[start&7] ] +
      bit_cnt[ str[r] & mask_r[stop&7] ];

for( i=l+1;i<r;i++)
	res += bit_cnt[str[i]];

return res;
}

int32_t ALL_SumGray(uchar *str,int32_t start, int32_t stop)
{
int32_t i, res;

for( res=0, i=start;i<stop;i++)
	res += 255-str[i];

return res;
}

void    ALL_compress_gray_line( uchar *lin, int32_t nx, int32_t *buf_comp, int32_t numx,
                 int32_t Xcut[],int32_t Xval[])
{
int32_t   k,Xc=Xcut[0],Xv;
uchar   c;

memset(buf_comp,0,sizeof(int32_t)*numx);
for(k=0;k<numx-1;k++)
		{
		Xc           = Xcut[k+1];
		buf_comp[k] += ALL_SumGray(lin,Xcut[k]+1,Xc)*numx;

		if( (c = lin[Xc])!=0 )
			{
			Xv             = Xval[k+1];
			buf_comp[k]   += Xv*(255-c);
			buf_comp[k+1] += (numx-Xv)*(255-c);
			}
		}

buf_comp[k] += ALL_SumGray(lin,Xc+1,nx)*numx;
return;
}


void ALL_compress_line( uchar *lin, int32_t nx, int32_t *buf_comp, int32_t numx,
                 int32_t Xcut[],int32_t Xval[])
{
int32_t k,Xc=Xcut[0],Xv;

memset(buf_comp,0,sizeof(int32_t)*numx);
for(k=0;k<numx-1;k++)
		{
		Xc           = Xcut[k+1];
		buf_comp[k] += ALL_SumBits(lin,Xcut[k]+1,Xc)*numx;

		if( (lin[Xc>>3])&mask_byte[Xc&7] )
			{
			Xv             = Xval[k+1];
			buf_comp[k]   += Xv;
			buf_comp[k+1] += numx-Xv;
			}
		}

buf_comp[k] += ALL_SumBits(lin,Xc+1,nx)*numx;
return;
}


void ALL_addcomp(uint32_t res_comp[],int32_t buf_comp[],int32_t numx,int32_t numy) {
	int32_t i;

	for(i=0;i<numx;i++)
		res_comp[i] += numy*buf_comp[i];
	return;
}

void ALL_normalize_res(uint32_t res_comp[],uint16_t res[],int32_t numx)
{
int32_t i;
uint32_t amax=0, div;
uint32_t mod;
for(i=0;i<numx;i++)
    if( res_comp[i] )
        amax += res_comp[i]*res_comp[i];
if( !amax )
	return;
amax = sqrt(amax);
div  = 32767l/amax;
    memset(res,0,numx*sizeof(res[0]));
for(i=0;i<numx;i++)
    if( res_comp[i] )
	    res[i] = (uint16_t)(res_comp[i]*div);
if( !div )
    {
    mod  = (32767l-div*amax)*256l/amax ;
    for(i=0;i<numx;i++)
	    res[i] = (uint16_t)(res_comp[i]*div +((res_comp[i]*mod)>>8));
    }
return;
}

void normalize_res_3x5(uint32_t res_comp[],uint16_t res[],int32_t numx)
{
int32_t i;
uint32_t amax=0, div;
uint32_t mod;
for(i=0;i<numx;i++)
    if( res_comp[i] )
        amax += res_comp[i]*res_comp[i];
if( !amax )
	return;
amax = sqrt(amax);
div  = 32767l/amax;
mod  = (32767l-div*amax)*256l/amax ;
for(i=0;i<numx;i++)
	res[i] = (uint16_t)(res_comp[i]*div +((res_comp[i]*mod)>>8));

return;
}

#ifdef PC_TYPE
void MMX_normalize_res(uint32_t res_comp[],uint16_t res[],int32_t numx)
{
int32_t i,n;
uint32_t amax, div;
n = ((numx+7)/8)*8;
for(i=0;i<n;i++)
    res[i] = (uint16_t)res_comp[i];

amax = MMX_scalar_sq(res, numx);

if( !amax )
	return;

amax = sqrt(amax);
div  = 32767l/amax;

MMX_scale(res, numx, div);

return;
}
#endif

int32_t r35_pack(uchar *raster,int32_t d_x,int32_t SX,int32_t SY,int32_t dx,int32_t dy,
  uint16_t I3x5[], int32_t TO_X, int32_t TO_Y)
{
uchar      *rast ;
int32_t       k, kx, num, t, len8, len;
#define SIZE_FROM   128
#define SIZE_TO     128*63
int32_t       Ycut[SIZE_FROM],Yval[SIZE_FROM],Xcut[SIZE_FROM],
            Xval[SIZE_FROM],buf_comp[SIZE_FROM];
uint32_t      res_comp[SIZE_TO];

if( TO_X>SIZE_FROM || TO_Y>SIZE_FROM )
    return 0;

len = TO_X*TO_Y;
len8 = ((len+7)/8)*8;  // align to 8 words

if( len>SIZE_TO )
    return 0;

memset(res_comp,0,sizeof(res_comp[0])*len8);
compress_line=ALL_compress_line;
addcomp = ALL_addcomp;
rast = raster + d_x*SY + (SX>>3);
MakeScale(Ycut,Yval,dy,TO_Y);
MakeScale(Xcut,Xval,dx,TO_X);
for(t=kx=k=0;k<TO_Y;k++,kx+=TO_X)
	{
	for(num=Ycut[k]+1;num<Ycut[k+1];num++,t+=d_x)
		{     /* first lines in strip */
		compress_line( &rast[t],dx, buf_comp,TO_X,Xcut,Xval);
		addcomp(&res_comp[kx],buf_comp,TO_X,TO_Y);
		}
	if( num!=dy )
		{     /* last line in strip */
		compress_line( &rast[t],dx, buf_comp,TO_X,Xcut,Xval); t+= d_x;
		if( Yval[k+1] )
			{ /* intrsected line */
			ALL_addcomp(&res_comp[kx],buf_comp,TO_X,Yval[k+1]);
			ALL_addcomp(&res_comp[kx+TO_X],buf_comp,TO_X,TO_Y-Yval[k+1]);
			}
		else  /* alone line */
			addcomp(&res_comp[kx+TO_X],buf_comp,TO_X,TO_Y);
		}
	}
if( TO_X<10 || TO_Y<10 )
    normalize_res_3x5(res_comp,I3x5, len);
else
    normalize_res(res_comp,I3x5, len);
if(len!=len8)
    memset(&I3x5[len],0,(len8-len)*sizeof(I3x5[0]));
return 1;
}
int32_t r35_pack_gray(uchar *raster,int32_t d_x,int32_t SX,int32_t SY,int32_t dx,int32_t dy,
  uint16_t I3x5[], int32_t TO_X, int32_t TO_Y)
{
uchar      *rast ;
int32_t       k, kx, num, t, len, len8;
int32_t       Ycut[128],Yval[128],Xcut[128],Xval[128],buf_comp[128];
uint32_t      res_comp[32*32];

if( TO_X>128 || TO_Y>128 )
    return 0;

len = TO_X*TO_Y;
len8 = ((len+3)/8)*8;  // align to 8 words
if( len>32*32 )
    return 0;
memset(res_comp,0,sizeof(res_comp[0])*len8);

rast = raster + d_x*SY + SX;
MakeScale(Ycut,Yval,dy,TO_Y);
MakeScale(Xcut,Xval,dx,TO_X);


for(t=kx=k=0;k<TO_Y;k++,kx+=TO_X)
	{
	for(num=Ycut[k]+1;num<Ycut[k+1];num++,t+=d_x)
		{     /* first lines in strip */
		ALL_compress_gray_line( &rast[t],dx, buf_comp,TO_X,Xcut,Xval);
		ALL_addcomp(&res_comp[kx],buf_comp,TO_X,TO_Y);
		}
	if( num!=dy )
		{     /* last line in strip */
		ALL_compress_gray_line( &rast[t],dx, buf_comp,TO_X,Xcut,Xval); t+= d_x;
		if( Yval[k+1] )
			{ /* intrsected line */
			ALL_addcomp(&res_comp[kx],buf_comp,TO_X,Yval[k+1]);
			ALL_addcomp(&res_comp[kx+TO_X],buf_comp,TO_X,TO_Y-Yval[k+1]);
			}
		else  /* alone line */
			addcomp(&res_comp[kx+TO_X],buf_comp,TO_X,TO_Y);
		}
	}
if( TO_X<10 || TO_Y<10 )
    normalize_res_3x5(res_comp,I3x5, TO_X*TO_Y);
else
    normalize_res(res_comp,I3x5, TO_X*TO_Y);
if(len!=len8)
    memset(&I3x5[len],0,(len8-len)*sizeof(I3x5[0]));
return 1;
}



/***********************************/
/************** INIT ***************/
/***********************************/

void r35_set_cpu(int32_t cpu)
{
#ifdef PC_TYPE
switch(cpu)
	{
	case	5860:
	case	6860:
		scalar        = MMX_scalar16;
		open_image    = MMX_open_image;
		close_image   = MMX_close_image;
        normalize_res = MMX_normalize_res;
        //normalize_res = ALL_normalize_res;
		break;
	default:
		scalar        = scalar_all;
		open_image    = ALL_open_image;
		close_image   = ALL_close_image;
        normalize_res = ALL_normalize_res;
		break;
	}
#endif
return;
}

Bool32 r35_init(char *name,elm3x5 **tab3x5this, ind3x5 **header3x5this)
{
int32_t f=open_data_file(name, O_BINARY|O_RDONLY); /* , S_IREAD */
uint32_t num;

r35_error_code = ER_R35_NO_ERROR;
if( f==-1 )
  {
  r35_error_code = ER_R35_OPEN;
  return 0; /* no table */
  }

*header3x5this  = my_alloc(256*sizeof(ind3x5) );
if( *header3x5this==NULL )
	{
	close(f);
  r35_error_code = ER_R35_MEMORY;
	return 0; /* not enough memory */
	}

memset(*header3x5this,0,256*sizeof(ind3x5));
if( read(f,*header3x5this,1024)!=1024 )
  {
  close(f);
  r35_error_code = ER_R35_READ;
  return 0;
  }

num = (filelength(f)-1024)/sizeof(elm3x5);
*tab3x5this  = my_alloc(num*sizeof(elm3x5) );

if( *tab3x5this==NULL )
	{
	close(f);
    r35_error_code = ER_R35_MEMORY;
	return 0; /* not enough memory */
	}
memset(*tab3x5this,0,num*sizeof(elm3x5));
if( read( f, *tab3x5this, num*sizeof(elm3x5))!=(int)(num*sizeof(elm3x5)) )
  {
  close(f);
  r35_error_code = ER_R35_READ;
  return 0;
  }

close(f);

return 1;
}

Bool32 r35_save(char *name,elm3x5 *tab3x5this, ind3x5 *header3x5this, int num)
{
int32_t f,i;

// remove(name);
f=open_data_file(name,O_BINARY|O_WRONLY|O_CREAT|O_TRUNC ); /* S_IWRITE */
r35_error_code = ER_R35_NO_ERROR;
if( f==-1 )
  {
  r35_error_code = ER_R35_OPEN;
  return 0;
  }

if( write(f,header3x5this,1024)!=1024 )
  {
  close(f);
  r35_error_code = ER_R35_WRITE;
  return 0;
  }

for(i=0;i<num;i++)
//    if( tab3x5this[i].ltr )
        {
        if( write( f, &tab3x5this[i], sizeof(elm3x5))!=(int)(sizeof(elm3x5)) )
          {
          close(f);
          r35_error_code = ER_R35_WRITE;
          return 0;
          }
        }
/*
if( write( f, tab3x5this, num*sizeof(elm3x5))!=(int)(num*sizeof(elm3x5)) )
  {
  close(f);
  r35_error_code = ER_R35_WRITE;
  return 0;
  }
*/
close(f);

return 1;
}

int32_t r35_close(elm3x5 **tab,ind3x5 **header)
{
if( *header )
	my_free(*header,0);
if( *tab )
	my_free(*tab,0);
*tab    = NULL;
*header = NULL;
return 1;
}

/************************************/
/************** RECOG ***************/
/************************************/

int32_t scalar_all(int16_t Im3x5_1[], int16_t Im3x5_2[]) {
    uint32_t s = 0;
    int i;
    for (i = 0; i < 3 * 5; i++)
        s += Im3x5_1[i] * Im3x5_2[i];
    return s;
}

void   ALL_open_image(uint16_t Im3x5[])
{
return;
}

void   ALL_close_image (void)
{
return;
}

Bool32 r35_delete_elm3x5(uchar let, int32_t   num_del)
{
elm3x5 *curr,  *fin;
int i,j, no_next, no_kill;

if( !header3x5[ let ].numel )
    return FALSE;
num_learn   =   _msize(tab3x5)/sizeof(elm3x5);

no_kill = num_del;
no_next = tab3x5[no_kill].list;
tab3x5[no_kill].ltr=0;
curr        =   &tab3x5[0];
for(i=0;i<num_learn;i++,curr++)
    {
    if( curr->list==num_del )
        curr->list = no_next ;
    }
for(i=0;i<256;i++)
    if( header3x5[ i ].ltr )
        { // for all classes
        for(j=0,fin=&tab3x5 [ header3x5[i].numel ];;fin=&tab3x5 [ fin->list ])
            {
            j++;
            if( !fin->list )
                break;
            }

        if( j )
            header3x5[ i ].vnum   = j;
        else
            {
            header3x5[ i ].vnum   = 0;
            header3x5[ i ].numel  = 0;
            header3x5[ i ].ltr    = 0;
            }


        }

return TRUE;
}

Bool32 recog_cluster(uint16_t Im3x5[], uchar let, VERSION *v)
{
int jm = 0, rm = 0, res = 0, jold = 0;
elm3x5 *curr = 0;

curr = &tab3x5 [ (jm=header3x5[ let ].numel) ];

for(rm=0; ; curr = &tab3x5[ curr->list ])
		{
        if( !(curr->bnd&0xFF00) && !((curr->fnt&0x80) && !r35_fnt) )
            {
		    res = scalar(Im3x5,curr->vect);
		    if( res>rm )
			    {
			    rm = res;
                #ifdef    R35_DEBUG
			    jm = jold;
                #endif
			    }
            }
		if( !curr->list )
			break;
        jold = curr->list;
		}

v->let  = let;
v->prob = rm;
v->num  = jm;
return (rm>0);
}

Bool32 recog_cluster5x3(uint16_t Im3x5[], uchar let, VERSION *v)
{
int jold, jm, rm, res;
elm3x5 *curr;

curr = &tab3x5 [ (jm=header3x5[ let ].numel) ];

for(rm=0; ; curr = &tab3x5[ curr->list ])
		{
        if( (curr->bnd&0xFF00) )
            {
		    res = scalar(Im3x5,curr->vect);
		    if( res>rm )
			    {
			    rm = res;
                #ifdef    R35_DEBUG
			    jm = jold;
                #endif
			    }
            }
		if( !curr->list )
			break;
        jold = curr->list;
		}

v->let  = let;
v->prob = rm;
v->num  = jm;
return (rm>0);
}


void r35_recog(uint16_t Im3x5[],
          VERSION vers[], int32_t nvers, Bool32 r5x3)
{
int32_t i;
VERSION  v;

open_image(Im3x5);
for(i=0;i<nvers;i++)
  {
  if( alpha[(uchar)vers[i].let] && header3x5[vers[i].let].vnum )
	{
    if( r5x3 )
        recog_cluster5x3(Im3x5,(uchar)vers[i].let,&v);
    else
        recog_cluster(Im3x5,(uchar)vers[i].let,&v);
	vers[i].prob = recode_prob( v.prob );
	vers[i].num  = v.num;
	}
   }
close_image();
return;
}

void store_bests(VERSION *vers, VERSION *ver)
{
int32_t i, j, prob=ver->prob;

for(j=0;j<R35_MAX_VERS;j++)
	if( prob>vers[j].prob )
		break;

if( j==R35_MAX_VERS )
	return;

for(i=R35_MAX_VERS-1;i>j;i--)
    vers[i] = vers[i-1];

vers[j] = *ver;
return;
}

uchar recode_prob( int32_t prob )
{
int32_t start[]={32767, 32113, 31455, 30475, 29200,0,0}, i, res;
uint32_t stop[]={255,   230,   190,   110,   50,   0}  ;

prob >>= 15;
if( prob>=32767 )
	return 255;
//return (uchar)(prob>>7);
for(i=0;i<sizeof(stop)/sizeof(stop[0]);i++)
	{
	res = prob-start[i] ;
	if( res>=0 )
		break;
	}
res = (res*(stop[i-1]-stop[i]))/(start[i-1]-start[i])+stop[i];
return (uchar)res;
}



void r35_recog_full(uint16_t Im3x5[],
          VERSION v[], int32_t *nvers)
{
int32_t i;
VERSION vers[R35_MAX_VERS], ver;

memset(vers,0,sizeof(VERSION)*R35_MAX_VERS);
open_image(Im3x5);

for(i=0;i<256;i++)
    {
	if( alpha[i] && header3x5[i].vnum  )
	    {
        if( recog_cluster(Im3x5,(uchar)i,&ver) )
		    store_bests(vers, &ver);
	    }
    }
close_image ();
for(i=0;i<R35_MAX_VERS&&vers[i].prob>0;i++)
  ;
*nvers = i;
for(i--;i>=0;i--)
	{
	v[i].let  = vers[i].let;
	v[i].prob = recode_prob(vers[i].prob);
	v[i].num  = vers[i].num;
	}
return;
}

void r35_recog_full_prn(uint16_t Im3x5[],
          VERSION v[], int32_t *nvers)
{
int32_t i;
VERSION vers[R35_MAX_VERS], ver;
Bool32  used_cl[256]={0};

memset(vers,0,sizeof(VERSION)*R35_MAX_VERS);
open_image(Im3x5);

for(i=0;i<256;i++)
    {
	if( alpha[i] && header3x5[i].vnum && !used_cl[stdLeoTypefaceChar(header3x5[i].ltr)] )
	    {
        if( recog_cluster(Im3x5,(uchar)i,&ver) )
		    store_bests(vers, &ver);
        used_cl[header3x5[i].ltr]=1;
	    }
    }
close_image ();
for(i=0;i<R35_MAX_VERS&&vers[i].prob>0;i++)
  ;
*nvers = i;
for(i--;i>=0;i--)
	{
	v[i].let  = vers[i].let;
	v[i].prob = recode_prob(vers[i].prob);
	v[i].num  = vers[i].num;
	}
return;
}

/*************/
/* FUNCTIONS */
/*************/
R35_FUNC(void)  R35SetPlatform( int32_t cpu )
{
r35_set_cpu(cpu);
r35_cpu = cpu;
return ;
}

// INIT & DONE
R35_FUNC(Bool32)  R35InitNDX( MemFunc* mem )
{
Bool32  ret=TRUE;
if( mem!=NULL )
	{
	my_alloc  = mem->alloc;
	my_free   = mem->free;
	}
else
	{
	my_alloc  = r35_alloc;
	my_free   = r35_free;
	}

ret = r35_init(NAME_NDX,   &tab3x5ndx,  &header3x5ndx);
return ret;
}

R35_FUNC(Bool32)  R35InitHND( MemFunc* mem )
{
Bool32  ret=TRUE;
if( mem!=NULL )
	{
	my_alloc  = mem->alloc;
	my_free   = mem->free;
	}
else
	{
	my_alloc  = r35_alloc;
	my_free   = r35_free;
	}

ret = r35_init(NAME,       &tab3x5all,  &header3x5all);
return ret;
}

R35_FUNC(Bool32)  R35InitPRN( MemFunc* mem )
{
Bool32  ret=TRUE;
if( mem!=NULL )
	{
	my_alloc  = mem->alloc;
	my_free   = mem->free;
	}
else
	{
	my_alloc  = r35_alloc;
	my_free   = r35_free;
	}

ret = r35_init(NAME_PRINT, &tab3x5print,&header3x5print);
return ret;
}

R35_FUNC(Bool32)  R35Init( MemFunc* mem )
{
return R35InitNDX( mem ) & R35InitHND( mem ) & R35InitPRN( mem );
}

R35_FUNC(void)  R35Done(void)
{
if( header3x5all )
    r35_close(&tab3x5all,  &header3x5all);
if( header3x5ndx )
    r35_close(&tab3x5ndx,  &header3x5ndx);
if( header3x5print )
    r35_close(&tab3x5print,&header3x5print);
tab3x5=NULL;
header3x5=NULL;
return ;
}


R35_FUNC(int16_t) R35GetErr(void)
{
return r35_error_code;
}

R35_FUNC(Bool32)  R35SetAlphabet(
                        char     char_tbl_put_to[] // char table[0-255]
                   )
{
memcpy(alpha,char_tbl_put_to,256);
return TRUE;
}

// PACKING & BINARIZING
R35_FUNC(Bool32)   R35Binarize(RecRaster *rRaster,
							   uint16_t *CompImage,int32_t dx, int32_t dy)
{
int i,j,ii,iii,j1, dbx=(dx+63)/64*8;


/*
{
lev=1;
unsigned long lev;
int skip_x = 0, skip_y = 0;
unsigned long n;
for(n=lev=i=ii=0;i<dy;i++,ii+=dx)
	for(j=0;j<dx;j++)
		if( CompImage[ii+j] )
			{
			lev += CompImage[ii+j];
			n++;
			}
if( n )
	lev /= n;
lev-=lev/2;
skip_y=-1;
for(i=ii=0;i<dy;i++,ii+=dx)
	{
	for(j=0;j<dx;j++)
		if( CompImage[ii+j]>=lev )
			{
			skip_y=i;
			break;
			}
	if( skip_y!=-1 )
		break;
	}
if( skip_y==-1 )
	skip_y=dy;
skip_x=dx;
for(i=ii=0;i<dy;i++,ii+=dx)
	{
	for(j=0;j<dx;j++)
		if( CompImage[ii+j]>=lev)
		{
		if( skip_x>j )
			skip_x=j;  // min horizontal destination
        break;
		}
    if( skip_x==0 )
        break;
	}
memset(rRaster->Raster,0,dbx*dy);

rRaster->lnPixWidth=dx-skip_x;
rRaster->lnPixHeight=dy-skip_y;
rRaster->lnRasterBufSize = REC_MAX_RASTER_SIZE;
for(i=skip_y,ii=skip_y*dx,iii=0;i<dy;i++,ii+=dx,iii+=dbx)
	for(j1=0,j=skip_x;j<dx;j++,j1++)
		{
		if( CompImage[ii+j]>=lev )
			rRaster->Raster[iii+(j1>>3)] |= mask_byte[j1&7];
		}
}
*/
rRaster->lnPixWidth      = dx;
rRaster->lnPixHeight     = dy;
rRaster->lnRasterBufSize = REC_MAX_RASTER_SIZE;  ;
if( (r35_cpu==5860||r35_cpu==6860) && dx==16 && dy==16 )
    MMX_binarize_16x16(CompImage,rRaster->Raster);
else
{
memset(rRaster->Raster,0,dbx*dy);

rRaster->lnRasterBufSize = REC_MAX_RASTER_SIZE;
for(i=ii=iii=0;i<dy;i++,ii+=dx,iii+=dbx)
	for(j1=j=0;j<dx;j++,j1++)
		{
		if( CompImage[ii+j] )
			rRaster->Raster[iii+(j1>>3)] |= mask_byte[j&7];
		}
}
return TRUE;
}

R35_FUNC(Bool32)   R35Binarize8(RecRaster *rRaster,
							   uchar *CompImage,int32_t dx, int32_t dy)
{
int i,j,ii,iii,j1, dbx=(dx+63)/64*8, dbxin=(dx+7)/8*8;

unsigned long lev;
int skip_x = 0, skip_y = 0, skip_x_right=0, skip_y_top;
unsigned long n;
for(n=lev=i=ii=0;i<dy;i++,ii+=dbxin)
	for(j=0;j<dx;j++)
		if( CompImage[ii+j] )
			{
			lev += CompImage[ii+j];
			n++;
			}
if( n )
	lev /= n;
lev-=lev/2;

skip_y=-1;
for(i=ii=0;i<dy;i++,ii+=dbxin)
	{
	for(j=0;j<dx;j++)
		if( CompImage[ii+j]<lev )
			{
			skip_y=i;
			break;
			}
	if( skip_y!=-1 )
		break;
	}
if( skip_y==-1 )
	skip_y=dy;

skip_y_top=-1;
for(i=0,ii=(dy-1)*dbxin;i<dy;i++,ii-=dbxin)
	{
	for(j=0;j<dx;j++)
		if( CompImage[ii+j]<lev )
			{
			skip_y_top=dy-1-i;
			break;
			}
	if( skip_y_top!=-1 )
		break;
	}
if( skip_y_top==-1 )
	skip_y_top=0;
skip_x=dx;
skip_x_right = dx;
for(i=ii=0;i<dy;i++,ii+=dbxin)
	{
	for(j=0;j<dx;j++)
		if( CompImage[ii+j]<lev)
		{
		if( skip_x>j )
			skip_x=j;  // min horizontal destination
        if( skip_x_right>dx-1-j )
            skip_x_right=dx-1-j;
		}
	}
//skip_x=skip_y=0;
memset(rRaster->Raster,0,dbx*dy);


rRaster->lnRasterBufSize = REC_MAX_RASTER_SIZE;
for(i=skip_y,ii=skip_y*dbxin,iii=0;i<dy;i++,ii+=dbxin,iii+=dbx)
	for(j1=0,j=skip_x;j<dx;j++,j1++)
		{
		if( CompImage[ii+j]<lev )
			rRaster->Raster[iii+(j1>>3)] |= mask_byte[j1&7];
		}
rRaster->lnPixWidth=dx-skip_x-skip_x_right;
rRaster->lnPixHeight=/*dy*/skip_y_top-skip_y;
/*
rRaster->lnPixWidth  = dx;
rRaster->lnPixHeight = dy;
{
memset(rRaster->Raster,0,dbx*dy);

rRaster->lnRasterBufSize = REC_MAX_RASTER_SIZE;
for(i=ii=iii=0;i<dy;i++,ii+=dx,iii+=dbx)
	for(j1=j=0;j<dx;j++,j1++)
		{
		if( CompImage[ii+j] )
			rRaster->Raster[iii+(j1>>3)] |= mask_byte[j&7];
		}
*/

return TRUE;
}

uchar   double_raster[REC_MAX_RASTER_SIZE];
uchar   quadro_raster[REC_MAX_RASTER_SIZE];
uchar  dquadro_raster[REC_MAX_RASTER_SIZE];
uchar   tab16x256[16]={0x00,0x03,0x0C,0x0F,
                       0x30,0x33,0x3C,0x3F,
                       0xC0,0xC3,0xCC,0xCF,
                       0xF0,0xF3,0xFC,0xFF };

Bool32   R35Pack_resize(
        RecRaster*  raster,
        uint16_t* Image3x5,
		int32_t to_x, int32_t to_y, Bool32 proport)
{
int32_t   w, h, wb, i,ii,iii, swb, w0,h0, ret;
uchar   *rast;
uint16_t   Im16x16[32*32];

w0 = w    = raster->lnPixWidth;
h0 = h    = raster->lnPixHeight;

rast = raster->Raster;
wb   = ((w+63)/64)*8;

if( h<to_y )
    {
    if( h*4>=to_y )
        {
        for(iii=ii=i=0;i<h;i++,ii+=wb,iii+=4*wb)
            {
            memcpy(&double_raster[iii],         &rast[ii],wb);
            memcpy(&double_raster[iii+wb],      &rast[ii],wb);
            memcpy(&double_raster[iii+wb*2],    &rast[ii],wb);
            memcpy(&double_raster[iii+wb*3],    &rast[ii],wb);
            }
        h = h*4;
        rast = double_raster;
        }
    else if( h*2>=to_y )
        {
        for(iii=ii=i=0;i<h;i++,ii+=wb,iii+=2*wb)
            {
            memcpy(&double_raster[iii],   &rast[ii],wb);
            memcpy(&double_raster[iii+wb],&rast[ii],wb);
            }
        h = h*2;
        rast = double_raster;
        }
    }

if( w<to_x && w*4>=to_x )
    {
    int wbn = (((w*2)+63)/64)*8, k,kk;
    for(iii=ii=i=0;i<h;i++,ii+=wb,iii+=wbn)
        {
        for(kk=k=0;k<wb;k++,kk+=2)
            {
            quadro_raster[iii+kk]   = tab16x256[ rast[ii+k]>>4 ];
            quadro_raster[iii+kk+1] = tab16x256[ rast[ii+k]&15 ];
            }
        }
    w = w*2;
    rast = quadro_raster;
    wb   = wbn;
    }

if( w<to_x && w*2>=to_x )
    {
    int wbn = (((w*2)+63)/64)*8, k,kk;
    for(iii=ii=i=0;i<h;i++,ii+=wb,iii+=wbn)
        {
        for(kk=k=0;k<wb;k++,kk+=2)
            {
            dquadro_raster[iii+kk]   = tab16x256[ rast[ii+k]>>4 ];
            dquadro_raster[iii+kk+1] = tab16x256[ rast[ii+k]&15 ];
            }
        }
    w = w*2;
    rast = dquadro_raster;
    wb   = wbn;
    }


r35_error_code = ER_R35_NO_ERROR;

if( w<to_x || h<to_y )
	{
	r35_error_code = ER_R35_TOO_SMALL;
	return FALSE;
	}
if( proport==0 )
    ret = r35_pack(rast, wb, 0,0, w,h, Image3x5,to_x, to_y  );
else if( proport==1 )
    { // save original proportions

    if( w0>h0 )
        {
        memset(Image3x5,0,to_x*to_y*sizeof(Image3x5[0]));
        to_y = MAX((h0*to_y)/w0,1);
        ret = r35_pack(rast, wb, 0,0, w,h, Image3x5,to_x, to_y  );
        }
    else if( w0<h0 )
        {
        memset(Image3x5,0,to_x*to_y*sizeof(Image3x5[0]));
        swb   = to_x;
        to_x = MAX((w0*to_x)/h0,1);
        ret = r35_pack(rast, wb, 0,0, w,h, Im16x16,to_x, to_y  );
        wb   = to_x*sizeof(Image3x5[0]);
        for(iii=ii=i=0;i<to_y;i++,ii+=to_x,iii+=swb)
            memcpy(&Image3x5[iii],&Im16x16[ii],wb);
        }
    else
        ret = r35_pack(rast, wb, 0,0, w,h, Image3x5,to_x, to_y  );

    }


return (ret>0);
}

Bool32   R35PackGray_resize(
        RecRaster*  raster,          // raster description
        uint16_t* Image3x5,
		int32_t to_x, int32_t to_y, Bool32 proport)      // image 3x5
{
int32_t   w, h, wb, i,ii,iii, swb, w0,h0;
uchar   *rast;
uint16_t   Im16x16[16*16];

w0 = w    = raster->lnPixWidth;
h0 = h    = raster->lnPixHeight;

rast = raster->Raster;
wb   = ((w+7)/8)*8;

if( h<to_y && h*2>=to_y )
    {
    for(iii=ii=i=0;i<h;i++,ii+=wb,iii+=2*wb)
        {
        memcpy(&double_raster[iii],   &rast[ii],wb);
        memcpy(&double_raster[iii+wb],&rast[ii],wb);
        }
    h = h*2;
    rast = double_raster;
    }

if( w<to_x && w*4>=to_x )
    {
    int wbn = (((w*2)+7)/8)*8, k,kk;
    for(iii=ii=i=0;i<h;i++,ii+=wb,iii+=wbn)
        {
        for(kk=k=0;k<wb;k++,kk+=2)
            {
            quadro_raster[iii+kk]   = rast[ii+k];
            quadro_raster[iii+kk+1] = rast[ii+k];
            }
        }
    w = w*2;
    rast = quadro_raster;
    wb   = wbn;
    }
if( w<to_x && w*2>=to_x )
    {
    int wbn = (((w*2)+7)/8)*8, k,kk;
    for(iii=ii=i=0;i<h;i++,ii+=wb,iii+=wbn)
        {
        for(kk=k=0;k<wb;k++,kk+=2)
            {
            dquadro_raster[iii+kk]   = rast[ii+k];
            dquadro_raster[iii+kk+1] = rast[ii+k];
            }
        }
    w = w*2;
    rast = dquadro_raster;
    wb   = wbn;
    }


r35_error_code = ER_R35_NO_ERROR;

if( w<to_x || h<to_y )
	{
	r35_error_code = ER_R35_TOO_SMALL;
	return FALSE;
	}
if( !proport )
    r35_pack_gray(rast, wb, 0,0, w,h, Image3x5,to_x, to_y  );
else
    { // save original proportions

    if( w0>h0 )
        {
        memset(Image3x5,0,to_x*to_y*sizeof(Image3x5[0]));
        to_y = MAX((h0*to_y)/w0,1);
        r35_pack_gray(rast, wb, 0,0, w,h, Image3x5,to_x, to_y  );
        }
    else if( w0<h0 )
        {
        memset(Image3x5,0,to_x*to_y*sizeof(Image3x5[0]));
        swb   = to_x;
        to_x = MAX((w0*to_x)/h0,1);
        r35_pack_gray(rast, wb, 0,0, w,h, Im16x16,to_x, to_y  );
        wb   = to_x*sizeof(Image3x5[0]);
        for(iii=ii=i=0;i<to_y;i++,ii+=to_x,iii+=swb)
            memcpy(&Image3x5[iii],&Im16x16[ii],wb);
        }
    else
        r35_pack_gray(rast, wb, 0,0, w,h, Image3x5,to_x, to_y  );

    }

return TRUE;
}

R35_FUNC(Bool32)   R35Pack(
        RecRaster*  raster,          // raster description
        uint16_t* Image3x5,
		int32_t to_x, int32_t to_y)      // image 3x5
{
return (raster->lnRasterBufSize&1) ?
    R35PackGray_resize(   raster, Image3x5, to_x, to_y,0):
    R35Pack_resize(   raster, Image3x5, to_x, to_y,0);
}

R35_FUNC(Bool32)   R35PackProport(
        RecRaster*  raster,          // raster description
        uint16_t* Image3x5,
		int32_t to_x, int32_t to_y)      // image 3x5
{


return R35Pack_resize(   raster, Image3x5, to_x, to_y,1);
}

R35_FUNC(Bool32)   R35PackProportHor16(
        RecRaster*  raster,
        uint16_t* Image3x5,
		int32_t *to_x, int32_t *to_y)
{
int w = raster->lnPixWidth, h = raster->lnPixHeight;
*to_y = 16;
*to_x = (*to_y * w ) /  h;
return R35Pack_resize(   raster, Image3x5, *to_x, *to_y, 0);
}

// RECOGNIZING
R35_FUNC(Bool32)  R35RecogCharIm3x5(
			uint16_t* Im3x5,
			RecVersions* res  )
{
VERSION vers[16];
int32_t   nvers, i;

r35_error_code = ER_R35_NO_ERROR;
if( !header3x5all || !header3x5all )
  {
  res->lnAltCnt = 0;
  r35_error_code = ER_R35_NO_LOAD;
  return FALSE;
  }
memset(vers,0,sizeof(vers));
header3x5 =	header3x5all;
tab3x5    =	tab3x5all;
r35_recog_full(Im3x5,          vers, &nvers);
if( !nvers )
  {
  res->lnAltCnt = 0;
  r35_error_code = ER_R35_NO_RECOG;
  return FALSE;
  }

res->lnAltCnt = nvers;
for(i=0;i<nvers&&i<REC_MAX_VERS;i++)
	{
  res->Alt[i].Code     = vers[i].let;
  res->Alt[i].Info     = (uchar)vers[i].num;
  res->Alt[i].Prob     = (uchar)vers[i].prob;
  res->Alt[i].Method   = REC_METHOD_3X5;
  }
return TRUE;
}


R35_FUNC(Bool32)  R35RecogNdxIm3x5(
			uint16_t* Im3x5,       // image 3x5
			RecVersions* res  )  // acuracy
{
VERSION vers[16];
int32_t   nvers, i;

r35_error_code = ER_R35_NO_ERROR;
if( !header3x5ndx || !header3x5ndx )
  {
  res->lnAltCnt = 0;
  r35_error_code = ER_R35_NO_LOAD;
  return FALSE;
  }
memset(vers,0,sizeof(vers));
header3x5 =	header3x5ndx;
tab3x5    =	tab3x5ndx;
r35_recog_full(Im3x5,          vers, &nvers);
if( !nvers )
  {
  res->lnAltCnt = 0;
  r35_error_code = ER_R35_NO_RECOG;
  return FALSE;
  }

res->lnAltCnt = nvers;
for(i=0;i<nvers&&i<REC_MAX_VERS;i++)
	{
  res->Alt[i].Code     = vers[i].let;
  res->Alt[i].Info     = (uchar)vers[i].num;
  res->Alt[i].Prob     = (uchar)vers[i].prob;
  res->Alt[i].Method   = REC_METHOD_3X5;
  }
return TRUE;
}

R35_FUNC(Bool32)  R35RecogPrintCharIm3x5(
			uint16_t* Im3x5,       // image 3x5
			RecVersions* res  )  // acuracy
{
VERSION vers[16];
int32_t   nvers, i;

r35_error_code = ER_R35_NO_ERROR;
if( !header3x5print || !header3x5print )
  {
  res->lnAltCnt = 0;
  r35_error_code = ER_R35_NO_LOAD;
  return FALSE;
  }
memset(vers,0,sizeof(vers));
header3x5 =	header3x5print;
tab3x5    =	tab3x5print;
r35_recog_full_prn(Im3x5,          vers, &nvers);
if( !nvers )
  {
  res->lnAltCnt = 0;
  r35_error_code = ER_R35_NO_RECOG;
  return FALSE;
  }

res->lnAltCnt = nvers;
for(i=0;i<nvers&&i<REC_MAX_VERS;i++)
	{
  res->Alt[i].Code     = vers[i].let;
  res->Alt[i].Info     = (uint16_t)vers[i].num;
  res->Alt[i].Prob     = (uchar)vers[i].prob;
  res->Alt[i].Method   = REC_METHOD_3X5;
  }
return TRUE;
}


// RECONGIZE EXPERT
R35_FUNC(Bool32)  R35RecogCharIm3x5_expert(
			uint16_t* Im3x5,       // image 3x5
			RecVersions* res  )  // acuracy
{
VERSION vers[16];
int32_t   nvers, i;

r35_error_code = ER_R35_NO_ERROR;
if(  res->lnAltCnt == 0 )
  {
  r35_error_code = ER_R35_NO_RECOG;
  return FALSE;
  }
if( !header3x5all || !header3x5all )
  {
  r35_error_code = ER_R35_NO_LOAD;
  return FALSE;
  }

memset(vers,0,sizeof(vers));
nvers = MIN(res->lnAltCnt,sizeof(vers)/sizeof(vers[0]) );
for(i=0;i<nvers;i++)
	{
  vers[i].let=res->Alt[i].Code;
  vers[i].prob=0;
  }
header3x5 =	header3x5all;
tab3x5    =	tab3x5all;
r35_recog(Im3x5,   vers, nvers, 0);

for(i=0;i<nvers&&i<REC_MAX_VERS;i++)
	{
  res->Alt[i].Code     = vers[i].let;
  res->Alt[i].Info     = (uchar)vers[i].num;
  res->Alt[i].Prob     = (uchar)vers[i].prob;
  res->Alt[i].Method   = REC_METHOD_3X5;
  }
return TRUE;
}

R35_FUNC(Bool32)  R35RecogNdxIm3x5_expert(
			uint16_t* Im3x5,       // image 3x5
			RecVersions* res  )  // acuracy
{
VERSION vers[16];
int32_t   nvers, i;

r35_error_code = ER_R35_NO_ERROR;
if(  res->lnAltCnt == 0 )
  {
  r35_error_code = ER_R35_NO_RECOG;
  return FALSE;
  }
if( !header3x5ndx || !header3x5ndx )
  {
  r35_error_code = ER_R35_NO_LOAD;
  return FALSE;
  }
memset(vers,0,sizeof(vers));

nvers = MIN(res->lnAltCnt,sizeof(vers)/sizeof(vers[0]) );
for(i=0;i<nvers;i++)
	{
  vers[i].let=res->Alt[i].Code;
  vers[i].prob=0;
  }
header3x5 =	header3x5ndx;
tab3x5    =	tab3x5ndx;
r35_recog(Im3x5,   vers, nvers, 0);

for(i=0;i<nvers&&i<REC_MAX_VERS;i++)
	{
  res->Alt[i].Code     = vers[i].let;
  res->Alt[i].Info     = (uchar)vers[i].num;
  res->Alt[i].Prob     = (uchar)vers[i].prob;
  res->Alt[i].Method   = REC_METHOD_3X5;
  }
return TRUE;
}


R35_FUNC(Bool32)  R35RecogPrintCharIm3x5_expert(
			uint16_t* Im3x5,
			RecVersions* res, Bool32 r5x3  )
{
VERSION vers[16];
int32_t   nvers, i;

r35_error_code = ER_R35_NO_ERROR;
if(  res->lnAltCnt == 0 )
  {
  r35_error_code = ER_R35_NO_RECOG;
  return FALSE;
  }

if( !header3x5print || !header3x5print )
  {
  r35_error_code = ER_R35_NO_LOAD;
  return FALSE;
  }


memset(vers,0,sizeof(vers));

nvers = MIN(res->lnAltCnt,sizeof(vers)/sizeof(vers[0]) );
for(i=0;i<nvers;i++)
	{
  vers[i].let=res->Alt[i].Code;
  vers[i].prob=0;
  }
header3x5 =	header3x5print;
tab3x5    =	tab3x5print;
r35_recog(Im3x5,   vers, nvers, r5x3);

for(i=0;i<nvers&&i<REC_MAX_VERS;i++)
	{
  res->Alt[i].Code     = vers[i].let;
  res->Alt[i].Info     = (uchar)vers[i].num;
  res->Alt[i].Prob     = (uchar)vers[i].prob;
  res->Alt[i].Method   = REC_METHOD_3X5;
  }
return TRUE;
}

// LEARNING

// CLUSTER SECTION
typedef struct Cluster_
{
uchar   ltr;        // Name of Cluster
uint32_t   num;        // number of accepted images
uint16_t  vect[16];   // ideal image
double  vsum[16];   // sum of all images
struct  Cluster_ * next;
}Cluster;
// functions
Cluster * ClusterNew(uchar ltr);
void    ClusterAdd(Cluster *clu,uint16_t vect[]);
uchar   ClusterRecog(Cluster *clu,uint16_t vect[]);
uchar   ClusterRecogOneLet(uint16_t vect[],uchar ltr,VERSION *v);
int32_t   ClusterRecogFull(uint16_t vect[],VERSION v[],int32_t *nvers);
void    ClusterFree(void);
Bool32  ClusterTo3x5(int32_t FontType);
//
static  Cluster *First3x5=NULL, *Last3x5=NULL;
static  uchar   alpha_clust[256]={0};
Cluster * ClusterNew(uchar ltr)
{
Cluster *tmp;
tmp=calloc(1,sizeof(Cluster));
if( !tmp )
    return NULL;

if( !First3x5 )
    First3x5    = Last3x5 = tmp;
Last3x5->next   = tmp;
Last3x5         = tmp;
tmp->ltr        = ltr;
tmp->num        = 0;
tmp->next       = NULL;
alpha_clust[ltr]=1;
return tmp;
}

Cluster * ClusterNum(int32_t  num)
{
int32_t j;
Cluster * curr;
if( !First3x5 )
    return NULL;
for(j=0,curr=First3x5;curr/*->next*/&&j<num;curr=curr->next,j++);
return j==num ? curr : NULL;
}

void    ClusterAdd(Cluster *clu,uint16_t vect[])
{
int     i;
uint32_t  sq;
// adding
for(i=0;i<15;i++)
    clu->vsum[i]+=(double)vect[i];
clu->num++;
// reduce to uint16_t
for(i=0;i<15;i++)
    clu->vect[i]   = (uint16_t)(clu->vsum[i] / clu->num);
// accuracy normalize
sq=scalar_all(clu->vect,clu->vect);
sq = sqrt (sq);
for(i=0;i<15;i++)
    clu->vect[i]   = (uint16_t)((((int)(clu->vect[i]))*32767l)/sq);
return;
}

uchar   ClusterRecog(Cluster *clu,uint16_t vect[])
{
return recode_prob(scalar_all(clu->vect,vect));
}

uchar   ClusterRecogOneLet(uint16_t vect[],uchar ltr,VERSION *v)
{
Cluster *curr;
uchar   rm,prob;
int32_t   jm,j;

if( !First3x5 )
    {
    v->let  =0;
    v->prob =0;
    v->num  =-1;
    return 0;
    }

for(jm=-1,j=rm=0,curr=First3x5;curr/*->next*/;curr=curr->next,j++)
    if( curr->ltr==ltr )
        {
        prob = ClusterRecog(curr,vect);
        if( rm<prob )
            {
            rm=prob;
            jm=j;
            }
        }
v->let  =ltr;
v->prob =rm;
v->num  =jm;
return rm>0;
}

int32_t   ClusterRecogFull(uint16_t vect[],VERSION v[],int32_t *nvers)
{
VERSION vers[R35_MAX_VERS]={0}, ver;
int     i;

for(i=0;i<256;i++)
    if( alpha_clust[i] )
        {
        if( ClusterRecogOneLet(vect,(uchar)i,&ver) )
		    store_bests(vers, &ver);
        }
for(i=0;i<R35_MAX_VERS&&vers[i].prob>0;i++);
*nvers = i;
for(i--;i>=0;i--)
	{
	v[i].let  = vers[i].let;
	v[i].prob = vers[i].prob;
	v[i].num  = vers[i].num;
	}
return *nvers;
}

Bool32    ClusterTo3x5(int32_t FontType)
{
Cluster *curr;
elm3x5  *curr3x5;
uint32_t  lim = (FontType>-1)?FontType:1;

if( !First3x5 )
    return FALSE;
for(curr=First3x5; curr; curr=curr->next)
if( curr->num>lim )
  {
  if( num_learn==max_learn )
    {
    r35_error_code = ER_R35_MEMORY;
	return FALSE; /* not enough memory */
	}
  if( header3x5[ curr->ltr ].vnum )
    { // add to 3x5 class
    tab3x5[num_learn].ltr   = curr->ltr;
    tab3x5[num_learn].fnt   = r35_fnt;
    tab3x5[num_learn].list  = 0;
    tab3x5[num_learn].bnd   = 0;
    memcpy(&tab3x5[num_learn].vect[0], curr->vect, 30);
    for(curr3x5 = &tab3x5 [ header3x5[ curr->ltr ].numel ]; ; curr3x5 = &tab3x5[ curr3x5->list ])
        {
	    if( !curr3x5->list )
	        break;
	    }
      header3x5[ curr->ltr ].vnum++;
      curr3x5->list = num_learn++;
    }
  else
    { // store first class
    header3x5[ curr->ltr ].ltr   = curr->ltr;
    header3x5[ curr->ltr ].numel = num_learn;
    header3x5[ curr->ltr ].vnum  = 1;

    tab3x5[num_learn].ltr   = curr->ltr;
    tab3x5[num_learn].fnt   = r35_fnt;
    tab3x5[num_learn].list  = 0;
    tab3x5[num_learn].bnd   = 0;
    memcpy(&tab3x5[num_learn].vect[0], curr->vect, 30);

    num_learn++;
    }
  }
return TRUE;
}

void    ClusterFree(void)
{
Cluster *curr,*next;int32_t num=0;
if( !First3x5 )
    return;
for(curr=First3x5; curr; curr=next)
    {
    next =  curr->next;
    free(curr);
    num++;
    }
First3x5 = Last3x5 = NULL;
return;
}

int32_t ClusterNumber(void)
{
int32_t num;
Cluster *curr;
if( !First3x5 )
    return 0;
for(num=0,curr=First3x5; curr; curr=curr->next)
    {
    num++;
    }
return num;
}
// end CLUSTER SECTION

R35_FUNC(Bool32)  R35Init_learn( int32_t num, uchar level, int32_t FontType )
{
switch(FontType)
    {
    case    0:

        if( !r35_init(NAME,       &tab3x5all,  &header3x5all) )
            {
            r35_error_code = ER_R35_MEMORY;
	        return FALSE;
	        }
        header3x5 =	header3x5all;
        tab3x5    =	tab3x5all;
        break;
    case    1:

        if( !r35_init(NAME_PRINT,       &tab3x5print,  &header3x5print) )
            {
            r35_error_code = ER_R35_MEMORY;
	        return FALSE;
	        }
        header3x5 =	header3x5print;
        tab3x5    =	tab3x5print;
        break;
    default :
        return FALSE;
    }

r35_learn_level=level;
num_learn=_msize(tab3x5)/sizeof(elm3x5);
max_learn=num_learn+num;
switch(FontType)
    {
    case    0:
		tab3x5all=realloc(tab3x5all,(max_learn)*sizeof(elm3x5));
		tab3x5   = tab3x5all;
		break;
	case    1:
		tab3x5print=realloc(tab3x5print,(max_learn)*sizeof(elm3x5));
		tab3x5   = tab3x5print;
		break;
	}
if( !tab3x5 )
    {
    r35_error_code = ER_R35_MEMORY;
	return FALSE;
	}

return TRUE;
}

R35_FUNC(void)  R35Done_learn(int32_t FontType)
{
if(FontType!=0&&FontType!=1)
        return ;

if( num_learn||ClusterNumber() && tab3x5 )
    {
    ClusterTo3x5(FontType);
    ClusterFree();
    switch(FontType)
    {
    case    0:
        r35_save(NAME,tab3x5all, header3x5all,num_learn);
        break;
    case    1:
        r35_save(NAME_PRINT,tab3x5print, header3x5print,num_learn);
        break;
    }
    R35Done();
    }
num_learn=0;
return ;
}

R35_FUNC(Bool32)  R35RecogCharIm3x5_learn(
            uchar Code,uint16_t* Im3x5/*,
			Bool32 r5x3*/, int32_t FontType  )
{
VERSION vers[R35_MAX_VERS];
int32_t   nvers;

r35_error_code = ER_R35_NO_ERROR;
if( r35_learn_level==0 )
  {
  r35_error_code = ER_R35_NO_LEVEL;
  return FALSE;
  }
if( !alpha[Code] )
  {
  r35_error_code = ER_R35_ALPHA;
  return FALSE;
  }


memset(vers,0,sizeof(vers));
switch(FontType)
    {
    case    0:
        header3x5 =	header3x5all;
        tab3x5    =	tab3x5all;
        break;
    case    1:
        header3x5 =	header3x5print;
        tab3x5    =	tab3x5print;
        break;
    default :
        return FALSE;
    }

if( !header3x5 || !header3x5 )
  {
  r35_error_code = ER_R35_NO_LOAD;
  return FALSE;
  }
vers[0].let =Code;
vers[0].prob=0;

if( header3x5[ Code ].vnum )
    { // class exist
    switch( FontType )
    {
    case    0:
        r35_recog_full(Im3x5, vers, &nvers);
        break;
    case    1:
        r35_recog_full_prn(Im3x5, vers, &nvers);
        break;
    }
    if( vers[0].let!=Code ||
        vers[0].let==Code && vers[0].prob<r35_learn_level )
      { // error or too small prob
      ClusterRecogFull(Im3x5, vers, &nvers);
      if( vers[0].let!=Code || !nvers ||
        vers[0].let==Code && vers[0].prob<r35_learn_level )
        { // error or too small prob
        ClusterAdd( ClusterNew(Code), Im3x5 );
        }
       else
        {
        ClusterAdd( ClusterNum(vers[0].num), Im3x5 );
        }
      return FALSE;
      }
    }
else
    { //  class absent
    ClusterRecogFull(Im3x5, vers, &nvers);
    if( vers[0].let!=Code || !nvers ||
        vers[0].let==Code && vers[0].prob<r35_learn_level )
        { // error or too small prob
        ClusterAdd( ClusterNew(Code), Im3x5 );
        }
    else
        {
        ClusterAdd( ClusterNum(vers[0].num), Im3x5 );
        }
    }
return TRUE;
}

R35_FUNC(Bool32)  R35RecogCharIm3x5_learn_expert(
            uchar Code,uint16_t* Im3x5)
{
VERSION vers[R35_MAX_VERS];
int32_t   nvers;

r35_error_code = ER_R35_NO_ERROR;
if( r35_learn_level==0 )
  {
  r35_error_code = ER_R35_NO_LEVEL;
  return FALSE;
  }
if( !alpha[Code] )
  {
  r35_error_code = ER_R35_ALPHA;
  return FALSE;
  }

if( !header3x5print || !header3x5print )
  {
  r35_error_code = ER_R35_NO_LOAD;
  return FALSE;
  }
memset(vers,0,sizeof(vers));
header3x5 =	header3x5print;
tab3x5    =	tab3x5print;
vers[0].let =Code;
vers[0].prob=0;

if( header3x5[ Code ].vnum )
    {
    r35_recog(Im3x5,   vers, 1, 0);
    if( vers[0].prob<r35_learn_level )
      { // error or too small prob
      nvers = ClusterRecogOneLet(Im3x5, Code, vers);
      if( !nvers || vers[0].prob<r35_learn_level )
        { // error or too small prob
        ClusterAdd( ClusterNew(Code), Im3x5 );
        }
       else
        {
        ClusterAdd( ClusterNum(vers[0].num), Im3x5 );
        }
      return FALSE;
      }
    }
else
    { // store first class
    nvers = ClusterRecogOneLet(Im3x5, Code, vers);
    if( !nvers || vers[0].prob<r35_learn_level )
        { // error or too small prob
        ClusterAdd( ClusterNew(Code), Im3x5 );
        }
    else
        {
        ClusterAdd( ClusterNum(vers[0].num), Im3x5 );
        }
    }
return TRUE;
}

R35_FUNC(void)  R35SetMTR(int fnt)
{
r35_fnt = fnt;
return;
}

R35_FUNC(Bool32) R35Delete(uchar    let, int32_t num_del)
{
return r35_delete_elm3x5(let, num_del);
}

R35_FUNC(Bool32) R35Add(uchar    let,uint16_t* Im3x5, int32_t FontType)
{
elm3x5 *curr;
uchar  fnt=0,  bnd=0;

if( num_learn==max_learn )
    {
    r35_error_code = ER_R35_MEMORY;
	return FALSE; /* not enough memory */
	}
switch(FontType)
    {
    case    0:
        header3x5 =	header3x5all;
        tab3x5    =	tab3x5all;
        break;
    case    1:
        header3x5 =	header3x5print;
        tab3x5    =	tab3x5print;
        break;
    default :
        return FALSE;
    }

tab3x5[num_learn].ltr   = let;
tab3x5[num_learn].fnt   = fnt|r35_fnt;
tab3x5[num_learn].list  = 0;
tab3x5[num_learn].bnd   = bnd;
memcpy(&tab3x5[num_learn].vect[0], Im3x5, 30);

if( header3x5[ let ].vnum )
    { // add to 3x5 class

    for(curr = &tab3x5 [ header3x5[ let ].numel ]; ; curr = &tab3x5[ curr->list ])
        {
	    if( !curr->list )
	        break;
	    }
      header3x5[ let ].vnum++;
      curr->list = num_learn;
    }
else
    { // store first class
    header3x5[ let ].ltr   = let;
    header3x5[ let ].numel = num_learn;
    header3x5[ let ].vnum  = 1;
    }

num_learn++;
return TRUE;
}
