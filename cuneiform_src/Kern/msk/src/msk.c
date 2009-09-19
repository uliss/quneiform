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
#include <ctype.h>
/*#include <windows.h>*/
#include <sys/types.h>
#include <sys/stat.h>
/*#include <dos.h>
#include <conio.h>*/
#include <fcntl.h>
#include <float.h>
#include <stdio.h>
/*#include <io.h>*/
#include <string.h>
#include <memory.h>
/*#include <process.h>*/
#include <time.h>
#include <math.h>

#include "mskdef.h"
#include "msk.h"
#include "mmx.h"
#include "msk32fun.h"

#include "compat_defs.h"
#include "minmax.h"

int16_t err;
static uint32_t bit_cnt_msk[66000];
uint16_t Limii;
short initiated=0;
int nm, char_lst[16],number_lst[16],lst[16], ms[16];
char alphabet[256];
int is_latin;

uint16_t iobraz[16],iobraz1[16],iobraz2[16];
uint32_t obraz[32*4];

uint16_t abc_n, Hnd_abc_n[11];				//всего эт-ов
MSK_abc_ind *abc_ind, *Hnd_abc_ind[11];
MSK_abc *abc, *Hnd_abc[10];
short *ptr_ind , *Hnd_ptr_ind[11];

/***************************************************************/
/***************************************************************/
/***************************************************************/
static void MMX_setup_obraz(uint32_t *obraz, uint16_t *iobraz,
                            uint16_t *iobraz2);

uchar   *tmp_ptr;
static void all_open(int32_t *image)
{
    tmp_ptr = (uchar*)image;
    return;
}

static void all_close(void)
{
    tmp_ptr = NULL;
    return;
}

static Bool32 all_anding(int32_t *image,uint16_t *tmp,int32_t L);
static Bool32 (*anding)(int32_t *image,uint16_t *tmp,int32_t L)=all_anding;
static void all_setup_obraz(uint32_t *obraz, uint16_t *iobraz, uint16_t *iobraz2);
static void (*setup_obraz)(uint32_t *obraz, uint16_t *iobraz, uint16_t *iobraz2)=all_setup_obraz;
static void (*proc_open)(int32_t *image)=all_open;
static void (*proc_close)(void)=all_close;


int double_alphabet(int32_t i,int32_t n1)
{
    if (n1>0 && abc[i].let=='0'  &&   char_lst[n1-1]=='О')
        return 1;
    if (n1+1<nm && abc[i].let=='0'  &&  char_lst[n1-1]=='О')
        return 1;
    return 0;
}


int add_abc(short pri,uint16_t iobraz[])
{
    int i,j;
    for (i=0;i<16;i++)
        for (j=0;j<16;j++)
        {
        if (! i_16(iobraz[i],j)) continue;
        if((i>1 ) && (!i_16(iobraz[i-1],j)) && (!i_16(iobraz[i-2],j)) )
        {
            ij_16(iobraz[i-1],j,1);
            if ((i>2 && pri) && (!i_16(iobraz[i-2],j)) )
                ij_16(iobraz[i-2],j,1);
        }
        if (j>1 && (!i_16(iobraz[i],j-1))
            && (!i_16(iobraz[i],j-2)) )
            {
            ij_16(iobraz[i],j-1,1);
            if ((j>2 && pri) && (!i_16(iobraz[i],j-2)) )
                ij_16(iobraz[i],j-2,1);
        }
    }

    for (i=15;i>=0;i--)
        for (j=15;j>=0;j--)
        {
        if ( !i_16(iobraz[i],j) ) continue;
        if ( i<14   && (!i_16(iobraz[i+1],j))
            && (!i_16(iobraz[i+2],j)) )
            {
            ij_16(iobraz[i+1],j,1);
            if ( (i<13 && pri) && (!i_16(iobraz[i+2],j)) )
                ij_16(iobraz[i+2],j,1);
        }
        if ( (j<14 && pri ) && (!i_16(iobraz[i],j+1))
            && (!i_16(iobraz[i],j+2)) )
            ij_16(iobraz[i],j+1,1);
    }
    return 0;
}


/**************************************************/
/**   determination of symbol iobraz             **/
/**       list - indexes of chars in ABC1        **/
/**       meas - corresponding measures.         **/
/**************************************************/
#define CYKL2(IND) {						\
t   = *((int *)(etalons+IND));          \
      ii += bit_cnt_msk[ *(image+IND)      & t ]; \
            t   = ~t ;	                            \
                  ii += bit_cnt_msk[ *(image+16+IND)   & t ]; \
                        if (ii>=L )	                            \
                        return ii;					        \
                    }
#define CYKL20(IND) {						\
t   = *((int *)etalons);				\
      ii  = bit_cnt_msk[ *image      & t ];		\
            t   = ~t ;								\
                  ii += bit_cnt_msk[ *(image+16) & t ];		\
                    }

Bool32 all_anding(int32_t *image,uint16_t *etalons,int32_t L)
{
    int32_t t, ii = 0;

    CYKL20(0)  // 0
            CYKL2(1)
            CYKL2(2)
            CYKL2(3)
            CYKL2(4)
            CYKL2(5)
            CYKL2(6)
            CYKL2(7)
            CYKL2(8)
            CYKL2(9)
            CYKL2(10)
            CYKL2(11)
            CYKL2(12)
            CYKL2(13)
            CYKL2(14)
            CYKL2(15)  // 15

            return ii;
}


void MMX_setup_obraz(uint32_t *obraz, uint16_t *iobraz, uint16_t *iobraz2)
{
    uint16_t iobraz3[16];
    int i;

    for (i=0;i<16;i++)
        iobraz3[i] = (~iobraz2[i])&0xFFFF;

    memcpy(obraz,iobraz,sizeof(uint16_t)*16);
    memcpy(obraz+8,iobraz3,sizeof(uint16_t)*16);
    return;
}


void all_setup_obraz(uint32_t *obraz, uint16_t *iobraz, uint16_t *iobraz2)
{
    int i,j;

    for (j=i=0;i<16;i++,j++)
    {
        obraz[j]    = (iobraz [i])&0xFFFF;  // il
        obraz[j+16] = (~iobraz2[i])&0xFFFF;  // il0
    }
}


int det_sym_sort(short ptr_char, uint16_t pri, uint16_t abc_n1,
                 uint16_t iobraz[], uint16_t iobraz1[],uint16_t list[],
                 uint16_t meas0[], uint16_t proc)
{
    int32_t	i,l,n1,m1;
    uint32_t ii,j,jj, meas[3],L, k = 0, k1,let;
    int j1;

    memcpy(iobraz2,iobraz1,16*sizeof(short));
    add_abc(0,iobraz2);
    setup_obraz(obraz,iobraz,iobraz2);
    nm=0; list[0]=abc_n1; meas[0]=999; l=0; L = LIMI;
    proc_open(obraz);
    jj=ptr_char;
    for(j=ptr_ind[jj];j!=-1;j=ptr_ind[++jj])
    {
        let = abc_ind[j].let;			// letter

        if (!alphabet[let])	continue;	// absent letter
        for(i=abc_ind[j].start;i!=-1;i=abc[i].next)
        { 	if( (ii = anding(obraz,abc[i].rst,L))<L )
            {	// store new result
                for (n1=0;n1<nm;n1++)
                    if (ii<(uint32_t)ms[n1])  break;
                if (nm)
                {	if (double_alphabet(i,n1))
                    continue;
                    for (m1=nm;m1>n1;m1--)
                    {	if (m1<MAX_NM)
                        {	lst[m1]=lst[m1-1];
                            char_lst[m1]=char_lst[m1-1];
                            number_lst[m1]=number_lst[m1-1];
                            ms[m1]=ms[m1-1];
                        }
                    }
                }
                ms[n1]=ii;char_lst[n1]=let;lst[n1]=i;number_lst[n1]=i;
                if (nm<MAX_NM)  nm++;
                L=MIN(L,(uint32_t)(ms[0]+4));
                for (m1=nm-1;m1>0;m1--)
                    if ((uint32_t)ms[m1]>=L)  nm--;
                if (n1>=nm) continue;
                k1=-1;
                for (m1=0;m1<nm;m1++)
                {	if ((uint32_t)char_lst[m1]!=let) continue;
                    if (k1==-1)
                    {	k1=m1;k=1;
                        continue;
                    }
                    k++;
                    if ( (k1>1 && k>1) || (k>2))
                    {	if (m1+1<nm)
                        {	k=sizeof(int)*(nm-m1-1);
                            memmove(lst+m1,lst+m1+1,k);
                            memmove(char_lst+m1,char_lst+m1+1,k);
                            memmove(number_lst+m1,number_lst+m1+1,k);
                            memmove(ms+m1,ms+m1+1,k);
                            k=3;
                        }
                        nm--; m1--;
                    }
                }
                if (nm==MAX_NM)  // nm>=MAX_NM, decrize limit L
                    L = ms[MAX_NM-1];
                if (n1>0)	continue;
                if (ii<meas[0])
                {	meas[0]=ii;	char_lst[0]=(int)let;     //(int32_t)let;
                    number_lst[0]=i; lst[0]= list[0]=(uint16_t)i;	l=1;
                }
                if (!ii)	goto end_cykl;
            } // end of storing result
        } // end of letter 'let'
    } // end of etalons

    end_cykl:;

        if( l && nm>2 && ms[0]+4>=ms[1] && meas[0]>0)
        {	for (j1=2;j1<nm;j1++)
            {	if ( char_lst[0]==char_lst[j1] ) 	break;
                if (char_lst[1]==char_lst[j1])
                {	if (j1+1<nm && char_lst[0]==char_lst[j1+1]
                            &&  ms[j1]>=ms[j1+1] )
                    break;
                    i=char_lst[0];
                    char_lst[0]=char_lst[1];
                    char_lst[1]=i;
                    i=number_lst[0];
                    number_lst[0]=number_lst[1];
                    number_lst[1]=i;
                    break;
                }
            }
        }

        for (i=0,j=0; i<nm;i++)
        {	if (!j)
            {	j++;
                continue;
            }
            for (k=0;k<j;k++)
                if (char_lst[k]==char_lst[i]) goto nni;
            char_lst[j]=char_lst[i];
            lst[j]=lst[i];
            ms[j]=ms[i];
            j++;
            nni:;
        }
        nm=j;

        meas0[0] = (uint16_t)meas[0];
        proc_close();
        return(l);
    }



MSK_FUNC(void)    MSKSetPlatform(int32_t cpu)
{
    switch( cpu )
    {
                        case    5860:
                        case    6860:
        anding       = MMX_anding;
        setup_obraz  = MMX_setup_obraz;
        proc_open    = MMX_open;
        proc_close   = MMX_close;
        break;
                        default:
        anding       = all_anding;
        setup_obraz  = all_setup_obraz;
        proc_open    = all_open;
        proc_close   = all_close;
        break;
    }
}
/**************************************************************/
/**************************************************************/
/**************************************************************/



MSK_FUNC(Bool32)  MSKInit( MemFunc* mem, const char *NameFile)
{
    unsigned int i,j,k;
    Limii=LIMI;
    iobraz[0]=0;

    if ( tch_in( NameFile ) == -1 ) {
        initiated--;
        fprintf(stderr, "MSKInit: Error with file \"%s\"\n", NameFile);
        return FALSE;
    }

    //	memset(alphabet,0,256*sizeof(alphabet[0]));

    for(i=0;i<256;i++) {
        bit_cnt_msk[i]=0;
        for (k=0;k<16;k++)
            if (i_16(i,k))
                bit_cnt_msk[i]++;
    }
    for(k=256,i=1;i<256;i++,k+=256)
        for(j=0;j<256;j++)
            bit_cnt_msk[k+j] = bit_cnt_msk[i]+bit_cnt_msk[j];

    MMX_ind_setup_table(bit_cnt_msk);
    return initiated;
}



MSK_FUNC(Bool32)  MSKSetAlphabet( char char_tbl_put_to[] )
{
    is_latin = char_tbl_put_to[(short)('I')];
    memcpy(alphabet,char_tbl_put_to,256);
    return TRUE;
}



MSK_FUNC(Bool32)  MSKRecogChar(
        int HndTab,
        RecRaster  *recraster, // raster description
        RecVersions *Indres           )
{
    uchar*    p1616=NULL;
    short i;
    short hor;		// ширина в пикселах по горизонтали (кратно 64).
    short ver;		// количество строк во всем изображении.
    short lett_coo[4];	// (x1,y1,x2,y2) - координаты выделенного
    //  символа(замкнутой кривой), включа
    //  касательные.
    uint16_t  *buff_image;
    int ii;
    uchar io_char;

    if( MSKSetHndTab(HndTab) == -1 )
        return FALSE;

    err=0;
    hor=(short)(recraster->lnPixWidth);
    ver=(short)(recraster->lnPixHeight);
    lett_coo[0]=lett_coo[1]=(short)(0);
    lett_coo[2]=hor-1;
    lett_coo[3]=ver-1;
    hor=((hor+63)>>6)<<6;
    buff_image=(uint16_t  *)(recraster->Raster);

    nm=0;
    err=(int16_t) recindex (0, (uchar)(0), hor, ver, lett_coo, (uchar)(0),
                          buff_image, p1616, &io_char, TRUE );

    //Indres->lnAltCnt=nm;
    // Oleg :  example : russian X from latin collection
    for (ii=i=0;i<nm;i++)
        if( alphabet[ char_lst[i] ] )
        {
        Indres->Alt[ii].Code    = char_lst[i];
        Indres->Alt[ii].Prob    = ((LIMI-ms[i])*255)/LIMI;
        Indres->Alt[ii].Method  = REC_METHOD_MSK;
        ii++;
    }
    Indres->lnAltCnt=ii;

    return TRUE;
}



MSK_FUNC(Bool32)  MSKRecogCharExp(
        int HndTab,
        RecRaster  *recraster, // raster description
        RecVersions *Indres           )
{
    uchar*    p1616=NULL, io_char;
    short i, hor, ver, lett_coo[4];
    uint16_t  *buff_image;
    int let;

    if( MSKSetHndTab(HndTab) == -1 )
        return FALSE;

    err=0;
    hor=(short)(recraster->lnPixWidth);
    ver=(short)(recraster->lnPixHeight);
    lett_coo[0]=lett_coo[1]=(short)(0);
    lett_coo[2]=hor-1;
    lett_coo[3]=ver-1;
    hor=((hor+63)>>6)<<6;
    buff_image=(Bool16  *)(recraster->Raster);

    for(i=0;i<Indres->lnAltCnt;i++)
    {
        let=ptr_ind[257]=Indres->Alt[i].Code;
        ptr_ind[258]=-1; nm=0;

        if( abc_ind[let].vol == 0  ||  !alphabet[(uchar)let] )
            Indres->Alt[i].Prob = 1;
        else{
            err=(int16_t) recindex (257, (uchar)(0) ,hor, ver, lett_coo,
                                  (uchar)(0),buff_image, p1616, &io_char, TRUE );
            if( err!=-1 )
                Indres->Alt[i].Prob = ((LIMI-ms[0])*255)/LIMI;
            else
                Indres->Alt[i].Prob = 1;
        }

        Indres->Alt[i].Method  = REC_METHOD_MSK;
    }
    return TRUE;
}

MSK_FUNC(Bool32)  MSKRecogCharExpPuma(
        int HndTab,
        RecRaster  *recraster, // raster description
        RecVersions *Indres,
        uint32_t    Flags           )
{
    uchar*    p1616=NULL, io_char;
    short i, hor, ver, lett_coo[4];
    uint16_t  *buff_image;
    int let;

    if( MSKSetHndTab(HndTab) == -1 )
        return FALSE;

    err=0;
    hor=(short)(recraster->lnPixWidth);
    ver=(short)(recraster->lnPixHeight);
    lett_coo[0]=lett_coo[1]=(short)(0);
    lett_coo[2]=hor-1;
    lett_coo[3]=ver-1;
    hor=((hor+63)>>6)<<6;
    buff_image=(Bool16  *)(recraster->Raster);

    for(i=0;i<Indres->lnAltCnt;i++)
    {
        let=ptr_ind[257]=Indres->Alt[i].Code;
        ptr_ind[258]=-1; nm=0;

        if( abc_ind[let].vol == 0  ||  !alphabet[(uchar)let] )
            Indres->Alt[i].Prob = 1;
        else{
            err=(int16_t) recindex (257, (uchar)(0) ,hor, ver, lett_coo,
                                  (uchar)(0),buff_image, p1616, &io_char, (uchar)((Flags&1)?FALSE:TRUE) );
            if( err!=-1 )
                Indres->Alt[i].Prob = ((LIMI-ms[0])*255)/LIMI;
            else
                Indres->Alt[i].Prob = 1;
        }

        Indres->Alt[i].Method  = REC_METHOD_MSK;
    }
    return TRUE;
}


MSK_FUNC(void)  MSKDone(void)
{
    int i;
    if( initiated >= 0)
    {
        for(i=1; i<=initiated; i++)
        {
            free(Hnd_abc[i]);
            free(Hnd_abc_ind[i]);
            free(Hnd_ptr_ind[i]);
            Hnd_abc_n[i]=0;
        }
        initiated=0;
    }
}



MSK_FUNC(Bool32)  MSKSetHndTab(int hndTab)
{
    if(hndTab > initiated  ||  hndTab >= 11)
        return -1;
    abc_n=Hnd_abc_n[hndTab];
    abc=Hnd_abc[hndTab];
    abc_ind=Hnd_abc_ind[hndTab];
    ptr_ind=Hnd_ptr_ind[hndTab];
    return hndTab;

}



MSK_FUNC(Bool32)  MSKRecogNDX(
        RecRaster  *recraster, // raster description
        RecVersions *Indres           )
{
    short hor,ver,lett_coo[4],i,err;
    uint16_t  *buff_image;

    err=0;
    hor=(short)(recraster->lnPixWidth);
    ver=(short)(recraster->lnPixHeight);
    lett_coo[0]=lett_coo[1]=(short)(0);
    lett_coo[2]=hor-1;
    lett_coo[3]=ver-1;
    hor=((hor+63)>>6)<<6;
    buff_image=(Bool16  *)(recraster->Raster);

    nm=0;
    err=(int16_t) recindexNDX(hor,ver,lett_coo,(uchar)(0),
                            buff_image);
    if (err!=0)
        return FALSE;

    Indres->lnAltCnt=nm;
    for (i=0;i<nm;i++)
    {
        Indres->Alt[i].Prob=255;
        //		if (lst[i]!=-1)
        Indres->Alt[i].Code    = abn1[lst[i]];
        Indres->Alt[i].CodeExt = 0;
        Indres->Alt[i].Method  = REC_METHOD_MSK;
    }
    Indres->Alt[i].Code=0;

    return TRUE;
}

/******************************************************************/
/******************************************************************/
/******************************************************************/
///
///  sors fun
/////////////
int tch_in(const char *NameFile)
{
    uint16_t num_mask,tmp;
    int hnd,i,j;
    char NameTab[32];

    initiated++;
    Hnd_abc_n[initiated]=0;
    if((hnd = open_data_file(NameFile, O_RDONLY|O_BINARY)) > 0) {
        read(hnd,NameTab,32*sizeof(char));
        read(hnd,(uint16_t *)&Hnd_abc_n[initiated],sizeof(short));
        read(hnd,(uint16_t *)&num_mask,sizeof(short));
        read(hnd,(uint16_t *)&tmp,sizeof(short));
        read(hnd,(uint16_t *)&tmp,sizeof(short));
    }
    else {
        fprintf(stderr, "tch_in: can't open file: \"%s\"\n", NameFile);
        return -1;
    }

    if(!Hnd_abc_n[initiated])
        return -1;

    if( (Hnd_abc[initiated]=(MSK_abc *)
        calloc(Hnd_abc_n[initiated],sizeof(MSK_abc))) == NULL)
        {	close(hnd);
        return -1;
    }
    if((Hnd_abc_ind[initiated]=(MSK_abc_ind *)
        calloc(256,sizeof(MSK_abc_ind))) == NULL)
        {	close(hnd);
        return -1;
    }
    if((Hnd_ptr_ind[initiated]=(short *)
        calloc(260,sizeof(short))) == NULL)
        {	close(hnd);
        return -1;
    }


    if( read(hnd, Hnd_abc_ind[initiated],256*sizeof(MSK_abc_ind))== -1)
    {	close ( hnd );
        return -1;
    }
    memset(Hnd_ptr_ind[initiated],-1,260*sizeof(short));
    j=0;
    for(i=0; i<256; i++)
    {	if(Hnd_abc_ind[initiated][i].vol != 0)
        {	Hnd_ptr_ind[initiated][j]=i;  j++; }
    }

    for ( i = 0; i < Hnd_abc_n[initiated]; i++ )
    {	if( read( hnd, &Hnd_abc[initiated][i],sizeof(MSK_abc)) == -1)
        {	close ( hnd );
            return -1;
        }
    }

    close(hnd);
    return MSKSetHndTab(initiated);
}


short recindex
        (short ptr_char,
         uchar get_put,
         short hor,           // input
         short ver,           // input
         short *lett_coo,      // input
         uchar buff_col,      // input
         uint16_t  *buff_image,    // input
         uchar  *p1616,
         uchar  *io_char,       // output
         uchar  enable_1
         )
{
    if ( (lett_coo[0]+8>lett_coo[2] &&  lett_coo[1]+8>lett_coo[3] )
        || lett_coo[2] >= hor || lett_coo[3] >= ver )
        return -1;
    err=new_reco(ptr_char,hor,buff_image,p1616,buff_col,get_put,
                 lett_coo[0],lett_coo[1],lett_coo[2],
                 lett_coo[3], io_char,enable_1);

    if (err==1) return 0;
    else return err?err:99;
}


short recindexNDX
        (short hor,           // input
         short ver,           // input
         short *lett_coo,      // input
         uchar buff_col,      // input
         uint16_t  *buff_image    // input
         )
{
    unsigned short i1,i2,j1,j2,j11;
    uint16_t iob[16];
    uint16_t  list[2],meas[6];
    iob[0]=0;
    if( (lett_coo[0]+8>lett_coo[2] && lett_coo[1]+8>lett_coo[3]) ||
        lett_coo[2] >= hor || lett_coo[3] >= ver )
        return -1;

    if( lett_coo[2]-lett_coo[0]>=200 || lett_coo[3]-lett_coo[1]>=300 )
    {err=5; goto ret;}

    i1=lett_coo[1];
    i2=lett_coo[3];
    j11=j1=lett_coo[0];
    j2=lett_coo[2];

    chn_mat1 (hor,buff_image,buff_col,i1,j1,i2,j2,iob,16,0,0,15,15);

    det_symn(iob,list,meas);
    err=1;
    ret:
    if (err==1) return 0;
    else return err?err:99;
}




#define MIN_WID_PERC  33
#define MAX_WID_PERC 460
int new_reco(short ptr_char, short hor,uint16_t  *bgf,
             uchar  *p1616, uchar bgf_col, uchar pri, short m1,
             short n1,short m2,short n2, uchar *ch, uchar enable_1)
{
    short rt/*,i,jj*/;
    uint16_t k ;
    short  k1;
    uint16_t  list[2],meas[6];
    int32_t l1;
    rt=0;
    if  (abc_n==0 || m2<=m1+1 )	//04.02.97
    {
        rt=-1; ch[0]=0; nm=0;
        goto ret;
    }

    l1=( (int32_t)(n2-n1+1) * (int32_t)100);
    l1/=(m2-m1+1);
    if (l1>1000) k1=1000;
    else k1=(short)l1;

    if (k1<MIN_WID_PERC)
    {
        nm=rt=1; ms[0]=0;
        lst[0]=-1; char_lst[0]=ch[0]='~';
        goto ret;
    }
    if (k1>MAX_WID_PERC && enable_1)
    {
        nm=rt=1; ms[0]=0;lst[0]=-1;
        char_lst[0]=ch[0]=is_latin?'I':'1';
        goto ret;
    }
    if ( p1616==NULL )
        chn_mat(hor, bgf, bgf_col, n1, m1, n2, m2, iobraz,16,0,0,15,15);
    else memmove(iobraz,p1616,32);
    /*
        jj=0;
        for (i=0;i<16;i++)
                jj+=(short)bit_cnt_msk[iobraz[i]];
*/
    memcpy(iobraz1,iobraz,16*sizeof(short));
    memcpy(iobraz1,iobraz,16*sizeof(short));
    add_abc(0,iobraz1);

    k=det_sym_sort(ptr_char,pri,abc_n,iobraz,iobraz1,list,meas,k1);
    if (k==0) goto bd;

    if ( ( (k>=1) && (meas[0]<Limii  ) ) )
        k=1;
    else {
        if (meas[0]==999)	k=0;
        else k=2;
    }

    rt=1; ch[0]=(uchar)abc[list[0]].let;
    bd:
    if (k!=1)
    {
        rt=-1;
        if (meas[0]==999) ch[0]=0;
    }
    ret:
    return rt;
}


/**************************************************/
/**   change of inner pixel rectangular matrix   **/
/**    into another rectangular matrix.          **/
/**     i:bgf       - given pixel matrix.        **/
/**       i1,j1,i2,j2 - line&column start&final  **/
/**       coordinates of inner matrix.           **/
/**     o:ss[][co1]- output matrix.              **/
/**       k0,l0,k1,l1 - line&column start&final  **/
/**       coordinates of output matrix.          **/
/**************************************************/
int chn_mat(int16_t coo,uint16_t  *bgf,uchar bgf_col,
            int16_t i1,int16_t j1,int16_t i2,int16_t j2,uint16_t *ss,
            int16_t co1,int16_t k0,int16_t l0,int16_t k1,int16_t l1)
{
    int j,ii,ii1,jj;
    int co,n,m,i,k,iiold,jjold,pp;
    char tabi[300],tabj[300];

    for (i=k0;i<=k1;i++)
        ss[i]=0;

    ii1=0;
    for (i=0;i<=i2-i1;i++)
    {
        tabi[i]=( ii1 )  /(i2-i1)  ;
        if (i==i2-i1) tabi[i]=k1-k0;
        ii1+=(k1-k0);
    }
    ii1=0;
    for (i=0;i<=j2-j1;i++)
    {
        tabj[i]=( ii1 )  /(j2-j1)  ;
        if (i==j2-j1) tabj[i]=l1-l0;
        ii1+=(l1-l0);
    }
    co=coo/pc_16_32;
    if (i2<=i1 || j2<=j1) return(-1);
    pp=(co)*(i1)  + (j1/pc_16_32);
    iiold= -1;
    for (i=i1;i<=i2;i++)
    {
        n=0;
        m=j1%pc_16_32;
        ii=tabi[i];
        jjold=-1;
        for (j=j1;j<=j2;j++)
        {
            jj=tabj[j];
            if (i_16(bgf[pp+n],m))
            {
                ss[ii]|=ist0[jj];
                if (jjold!=-1 && (jj>jjold+1))
                    for (k=jjold+1;k<jj;k++)
                        ss[ii]|=ist0[k];
                jjold=jj;

                if (iiold!=-1 && (ii>iiold+1) )
                    for (k=iiold+1;k<ii;k++)
                        ss[k]=ss[iiold];
                iiold=ii;
            }
            else jjold=-1;

            if (m==pc_16_32-1) {m=0;n++;}
            else m++;
        }
        pp+=(co);
    }
    return(0);
}


/**************************************************/
/**   change of inner pixel rectangular matrix   **/
/**    into another rectangular matrix.          **/
/**     i:bgf       - given pixel matrix.        **/
/**       i1,j1,i2,j2 - line&column start&final  **/
/**       coordinates of inner matrix.           **/
/**     o:ss[][co1]- output matrix.              **/
/**       k0,l0,k1,l1 - line&column start&final  **/
/**       coordinates of output matrix.          **/
/**************************************************/
/***/
int chn_mat1(coo,bgfff,buff_col,i1,j1,i2,j2,ss,co1,k0,l0,k1,l1)
        short coo,i1,j1,i2,j2,co1,k0,l0,k1,l1;
unsigned short *ss;
char buff_col;
uint16_t  *bgfff;
{
    int j,ii,jj,ii1,jj1,pp;
    int co,n,m,i,k,iiold,jjold;
    int tabi[300],tabj[300];

    co=coo>>4;

    if (i2<=i1 || j2<=j1) return(-1);

    for (i=k0;i<=k1;i++)
        ss[i]=0;

    ii1=0;
    for (i=0;i<=i2-i1;i++)
    {
        tabi[i]=( ii1 )  /(i2-i1)  ;
        if (i==i2-i1)
            tabi[i]=k1-k0;
        ii1+=(k1-k0);
    }
    ii1=0;
    for (i=0;i<=j2-j1;i++)
    {
        tabj[i]=( ii1 )  /(j2-j1)  ;
        if (i==j2-j1)
            tabj[i]=l1-l0;
        ii1+=(l1-l0);
    }

    pp=co*i1  + (j1>>4);
    jjold=0;
    iiold=0;
    jj1=99;
    for (i=0;i<=i2-i1;i++)
    {
        n=0;
        m=j1%16;
        ii=tabi[i];
        if (ii!=iiold)
        {
            iiold=ii;
            if (jjold>=3 && ii>1 && ii<14)
            {
                for (jj=0;jj<=jj1;jj++)
                    ss[ii-1]&=ist1[jj];
                jjold=0; jj1=14;
            }
        }
        k=0;
        for (j=0;j<=j2-j1;j++)
        {
            jj=tabj[j];
            if( bgfff[pp+n]&ist0[m] )
            {
                ss[ii]|=ist0[jj];
                if (!k)
                {
                    if (jjold<3 && ii>1 && ii<14)
                    {
                        if (jj<=4)
                        {
                            jjold=0;
                            jj1=14;
                        }
                        else
                        {
                            jjold++;
                            jj1=MIN(jj-1,jj1);
                        }
                    }
                    k=1;
                }
            }
            if (m==15)
            {
                m=0;
                n++;
            }
            else
                m++;
        }
        pp+=co;
    }
    return(0);
}


/**************************************************/
/**   determination of symbol iobraz             **/
/**       list - indexes of chars in ABC1        **/
/**       meas - corresponding measures.         **/
/**************************************************/
#define CYKL(NUM) { \
if ( loc_iobraz[NUM]&loc_i_abn1[NUM]) goto nex_i; \
    }

#define LIMIT 1
short det_symn(iobraz,list,meas)
        unsigned short iobraz[],list[],meas[];
{
    short l,n1,m1;
    unsigned int *loc_i_abn1, loc_iobraz[16];
    int i;

    for(i=0;i<16;i++)
        loc_iobraz[i] = iobraz[i];

    meas[0]=999;
    l=nm=0;

    for (i=0;i<MAX_ABC1;i++)
    {
        meas[3]=0;
        if (!alphabet[abn1[i]]) continue;

        loc_i_abn1 = i_abn1[i];
        CYKL(0);
        CYKL(1);
        CYKL(2);
        CYKL(3);
        CYKL(4);
        CYKL(5);
        CYKL(6);
        CYKL(7);

        CYKL(8);
        CYKL(9);
        CYKL(10);
        CYKL(11);
        CYKL(12);
        CYKL(13);
        CYKL(14);
        CYKL(15);


        for (n1=0;n1<nm;n1++)
            if (meas[3]<ms[n1]) break;
        if (n1>0 && abn1[i]==abn1[lst[n1-1]])
            goto nex_i;
        if (n1+1<nm && abn1[i]==abn1[lst[n1+1]])
        {
            ms[n1+1]=meas[3];
            goto nex_i;
        }
        if (nm)
            for (m1=nm;m1>n1;m1--)
                if (m1<MAX_NM1) {lst[m1]=lst[m1-1]; ms[m1]=ms[m1-1];}
        ms[n1]=meas[3]; lst[n1]=i;
        if (nm<MAX_NM1) nm++;
        if (n1>0) goto nex_i;

        l=1; lst[0]=list[0]=i; ms[0]=meas[0]=meas[3];
        if (!meas[3]) goto ret;
        nex_i:
        ;
    }
    ret:
    return(l);
}
/////////
/// end sors fun
///
/******************************************************************/
/******************************************************************/
/******************************************************************/
