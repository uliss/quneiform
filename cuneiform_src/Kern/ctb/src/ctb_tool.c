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

//************************************************************************
// CTB_TOOL : read, write and test tool for CTB-format file
//************************************************************************
//           for version 3,4,5,6,7
//************************************************************************

#define MAXPATH 256

#include<stdio.h>
#include<string.h>
#include<sys/stat.h>
/*#include<direct.h>*/
#include<stdlib.h>
#include<ctype.h>
/*#include<io.h>*/

#include "ctb.h"

//********************************************************************
//*************** static data : **************************************
//********************************************************************
static Word8 save_pack[256*128+3+CTB_DATA_SIZE]; // global bit map buffer
static Word8 zero_data[CTB_DATA_SIZE]={0};  // zero attributes              //
static char *error_strings[]={
"Error free!",
"Can't open .CTB-file",
"Seek error",
"Signature of CTB-file is bad",
"Header of CTB-file is bad",
"Not enaugh memory!",
"Can't open .IND-file",
"Read error",
"False NDX !",
"Not opened descriptor",
"Zero address of attributes",
"Can't used killed raster",
"Unknown method of packing",
"Write error",
"Align Error",
"Can't saved to CTB with different version"
};
static Word32 mask_r[]={  255,    128,  192,  224,  240,  248,  252,  254, 255};
//                        0      1      2    3      4    5     6     7     8
//********************************************************************
//********************* static function : ****************************
//********************************************************************
static Int32  CTB_volume_true(char *file_name );
static Bool32   CTB_type(Int16 wid, Int16 hei, Int16 dpb);
static void     xor_lines(Word8 *bin,Int32 wb,Int32 len);
static void     xor_lines_rest(Word8 *bin,Int32 wb,Int32 len,Word8 mask);
static void     xor_one_line(Word8 *bin,Word8 *bin1,Int32 wb);
//********************************************************************
//************ global functions : ************************************
//********************************************************************
char *  ctb_last_punct(char *word);
Bool32  CTB_files_init(char *file_name,Word8 *data,Int16 maxX,Int16 maxY,
                    Int16 dpb,Word8 signums, Word8 attr_size);
//********************************************************************
//************ global data : *****************************************
//********************************************************************
Int32   ctb_err_code = CTB_ERR_NONE;   // error code
char * ctb_tmp_dir=NULL;
char local_grey_ctb[256] = "page6666";
char local_ctb_name[256] = "ct666666";
//********************************************************************
//***********  EXPORT functions from CTB_pack ************************
//********************************************************************
Int16 decLine(Word8 *inB, Int16 inLen, Word8 *outBuffer);
Int16 encLine(Word8 *inBuff, Int16 inLen,Word8 *save_p, Int16 outLen);
Int16 encput( Word8 byt, Word8 cnt,Word8 *save);
//********************************************************************
//***********  EXPORT functions from CTB_cnvm ************************
//********************************************************************
Bool32  conv_bytes_to_bits( Int16 colors, Word8 *text,Word8 *bin, Int16 len);
Bool32  conv_bits_to_bytes( Int16 colors, Word8 *text,Word8 *bin, Int16 len);

//********************************************************************
//**************** open/close CTB-files ******************************
//********************************************************************
#ifdef _MSC_VER
#define snprintf(a, b, c, d, e) _snprintf(a, b, c, d, e)
#endif
CTB_FUNC(void)   CTB_done(void )
{
if(ctb_tmp_dir) {
char tmpname[MAXPATH]={0};
snprintf(tmpname,sizeof(tmpname),"%s/%s",ctb_tmp_dir,local_ctb_name);
CTB_unlink(tmpname);
snprintf(tmpname,sizeof(tmpname),"%s/%s",ctb_tmp_dir,local_grey_ctb);
CTB_unlink(tmpname);
rmdir(ctb_tmp_dir);
free(ctb_tmp_dir);
}
return;
}

#ifdef WIN32
extern char* mkdtemp(char*);

static char* get_tmp_pattern() {
  const int len = GetTempPath(0, 0);
  const char pattrn[] = "cuneiform-XXXXXX";
  char* tmp = malloc(len + sizeof(pattrn) + 2);
  const int len2 = GetTempPath(len, tmp);
  tmp[len2] = '\\';
  strncpy(tmp + len2 + 1, pattrn, sizeof(pattrn) + 1);
  return tmp;
}
static void free_tmp_pattern(char* tmp) {
  free(tmp);
}
#else
static char* get_tmp_pattern() {
  return "/tmp/cuneiform-XXXXXXX";
}
static void free_tmp_pattern(char* dummy) {
}
#endif

CTB_FUNC(Int32) CTB_gettmpdirname(void) {
    char* tmp = get_tmp_pattern();
    ctb_tmp_dir = malloc(strlen(tmp) + 1);
    strncpy(ctb_tmp_dir, tmp, strlen(tmp) + 1);
    ctb_tmp_dir = mkdtemp(ctb_tmp_dir);
    free_tmp_pattern(tmp);
    if(!ctb_tmp_dir)
        return 1;
    else
        return 0;
}

CTB_FUNC(Int32) CTB_get_error(void)
{
return ctb_err_code ;
}

CTB_FUNC(char *) CTB_get_error_name(char *str)
{
strcpy(str, error_strings[ctb_err_code-1] );
return str;
}

CTB_FUNC(Bool32)  CTB_open(char *filename, CTB_handle *hnd, char *attr)
{
char lin[MAXPATH],file_name[MAXPATH],*p;
H_CTB_file HCTB;
ctb_err_code = CTB_ERR_NONE;
strcpy(file_name,filename);
p=ctb_last_punct(file_name);
strlwr(attr);
memset(hnd,0,sizeof(CTB_handle));
if( p ) *p='\0';
SPRINTF(lin,"%s/%s.CTB",ctb_tmp_dir,file_name);

hnd->bas = fopen(lin,*attr=='r' ? R_B : R_B_PLUS);
if( hnd->bas==BAD_FOPEN )
        {
        ctb_err_code = CTB_ERR_OPEN_CTB;
        return 0;    //  can't open CTBile //
        }
hnd->attr = *attr=='r' ? 'r' : 'w';

if( fseek(hnd->bas,0,SEEK_SET))
        {
        ctb_err_code = CTB_ERR_SEEK;
        return 0;    // can't seek //
        }
fread(&HCTB,sizeof( HCTB ),1, hnd->bas);  // read header of CTBfile//
if( HCTB.sign[0]!='C' || HCTB.sign[1]!='T' )
        {
        ctb_err_code = CTB_ERR_SIGN;
        return 0;    // bad signature //
        }
if( HCTB.dot_per_byte==0 )
        {
        ctb_err_code = CTB_ERR_HEAD;
        return 0;
        }
if( HCTB.version<3 || HCTB.version>7 )
        {
        ctb_err_code = CTB_ERR_VERS;
        return 0;
        }
hnd->len=(Int32)(((long)HCTB.size_x*HCTB.size_y)/HCTB.dot_per_byte);
                      // store attributes    //
hnd->num     = HCTB.volume>0 ? HCTB.volume : CTB_volume_true(file_name);
hnd->type           = (Int16)CTB_type(HCTB.size_x,HCTB.size_y,HCTB.dot_per_byte);
hnd->width          = HCTB.size_x;
hnd->height         = HCTB.size_y;
hnd->colors         = 1<<(8/HCTB.dot_per_byte);
hnd->dpb            = HCTB.dot_per_byte;
hnd->signums        = HCTB.signums;
hnd->version        = HCTB.version;
hnd->need_compress  = HCTB.need_compress&0x01;
hnd->is_sort        = ((HCTB.need_compress&0x02)!=0);
switch( hnd->version )
    {
    case    3:

        hnd->attr_size      = 16;
        break;
    case    4:
    case    5:
        hnd->attr_size      = 32;
        break;
    case    6:
    case    7:
        hnd->attr_size      = HCTB.attr_size;
        break;
    }
if( fseek(hnd->bas,0,SEEK_END) )  // ???? //
        {
        ctb_err_code = CTB_ERR_SEEK;
        return 0;
        }

SPRINTF(lin,"%s/%s.IND",ctb_tmp_dir,file_name);
hnd->ndx = fopen(lin,*attr=='r' ? R_B : R_B_PLUS);
if( hnd->ndx==BAD_FOPEN )
        {
        ctb_err_code = CTB_ERR_OPEN_NDX;
        return 0;            // can't open IndexFile //
        }
if( hnd->signums&CTB_PLANE )
  hnd->colors = -hnd->colors;  // special colors //
return (hnd->type!=CTB_UNKNOWN);
}

CTB_FUNC(void) CTB_close( CTB_handle *hnd)
{
H_CTB_file HCTB;

if( hnd->bas!=BAD_FOPEN )
  {
  if( fseek(hnd->bas,0,SEEK_SET) )
    return ;
  fread(&HCTB,sizeof( HCTB ),1, hnd->bas);   // read header of CTBile //
  HCTB.volume = hnd->num;
  if(hnd->need_compress)
    HCTB.need_compress |=0x01 ;
  if(hnd->is_sort)
    HCTB.need_compress |=0x02 ;
  else
    HCTB.need_compress &=(0xFF^0x02) ;
  if( HCTB.dot_per_byte<=0 )
    HCTB.dot_per_byte = (Word8)((long)hnd->len/((long)HCTB.size_x*HCTB.size_y)) ;
  if( fseek(hnd->bas,0,SEEK_SET) )
    return ;
  if( hnd->attr=='w' )                         // if enable change CTB  //
    fwrite(&HCTB,sizeof( HCTB ),1, hnd->bas);  // save corrected header //
  fclose(hnd->bas);
  hnd->bas=BAD_FOPEN;
  }


if( hnd->ndx!=BAD_FOPEN )
  {
  fclose(hnd->ndx);
  hnd->ndx=BAD_FOPEN;
  }


return;
}

CTB_FUNC(Bool32)  CTB_files_test(char *filename,Int16 maxX, Int16 maxY, Int16 dpb)
{
H_CTB_file H_CTB={{'C','T'},0,128,96,0,0,0,0,0,{0}},HCTB;
char s[MAXPATH],file_name[MAXPATH],*p;
FFILE fp;

strcpy(file_name,filename);
p=ctb_last_punct(file_name);
ctb_err_code = CTB_ERR_NONE;

HCTB = H_CTB;
if( p ) *p='\0';
SPRINTF(s,"%s.CTB",file_name);
fp=fopen(s,R_B);
if( fp==BAD_FOPEN )
        {
        ctb_err_code = CTB_ERR_OPEN_CTB;
        return FALSE;
        }
if( fread(&HCTB,sizeof(HCTB),1,fp)!=RET_FREAD(sizeof(HCTB),1) )
        { // can't read header CTBile //
        ctb_err_code = CTB_ERR_READ;
        fclose(fp);
        return FALSE;
        }
fclose(fp); // exist CTB file //

SPRINTF(s,"%s/%s.IND",ctb_tmp_dir,file_name);
fp=fopen(s,R_B);
if( fp==BAD_FOPEN )
        {
        ctb_err_code = CTB_ERR_OPEN_NDX;
        return FALSE;
        }
fclose(fp); // exist IND file //

if( HCTB.sign[0]=='C' && HCTB.sign[1]=='T' &&
  HCTB.size_x==maxX && HCTB.size_y==maxY &&
  HCTB.dot_per_byte==dpb )
  return CTB_type(HCTB.size_x,HCTB.size_y,HCTB.dot_per_byte);

ctb_err_code = CTB_ERR_HEAD;
return FALSE;
}

// for LEM PRO 256x128x2 ONLY
CTB_FUNC(Bool32)  CTB_create(char *file_name,Word8 *data)
{
Word8   attr_size=32+2; // version 7

if(!ctb_tmp_dir) {
    if(CTB_gettmpdirname()){
        ctb_err_code = CTB_ERR_OPEN_CTB;
        return 0;    // error on creating temporary directory
    }
}

if( data && data[0] )
    attr_size=data[0];
return  CTB_files_init(file_name,data,256,128,8,CTB_NON_SIGNUMS,attr_size);
}

CTB_FUNC(Bool32)  CTB_create_gray(char *file_name,Word8 *data)
{
Word8   attr_size=32+2; // version 7
if( data && data[0] )
    attr_size=data[0];
return  CTB_files_init(file_name,data,256,128,1,CTB_GRAY_SCALE,attr_size);
}


//********************************************************************
//**************** subroutines to read data from *********************
//********************************************************************
CTB_FUNC(Int32)  CTB_read( CTB_handle *hnd,Int32 num , Word8 *save_bin, Word8 *data)
{
FFILE   fp;
Int32   l_seek,f_seek,l, len, w,h, sign, wb, datalen;
Bool32  gray=(hnd->signums&CTB_GRAY_SCALE),
        plane=(hnd->signums&CTB_PLANE);

ctb_err_code = CTB_ERR_NONE;
if( num<0 )
        {
        ctb_err_code = CTB_ERR_NDX;
        return 0; // non correct index //
        }

if( hnd==NULL )
        {
        ctb_err_code = CTB_ERR_NOT_OPEN;
        return 0;       // file is not opened //
        }

if( data==NULL )
        {
        ctb_err_code = CTB_ERR_ZERO_DATA;
        return 0;       // for CTB ONLY need setup Width & Heigth //
        }

switch( hnd->version )
    {
    case    3:

        datalen=16;
        break;
    case    4:
    case    5:
        datalen=32;
        break;
    case    6:
    case    7:
        datalen=hnd->attr_size;
        break;
    }
if( data )
  memcpy(data,zero_data,datalen);

fp=hnd->ndx;
if( fseek(fp,num*8,SEEK_SET) )
        {
        ctb_err_code = CTB_ERR_SEEK;
        return 0;
        }
if( fread(&f_seek,4,1,fp)!=RET_FREAD(4,1) )
        {
        ctb_err_code = CTB_ERR_READ;
        return 0;
        }
if( fread(&l_seek,4,1,fp)!=RET_FREAD(4,1) )
        {
        ctb_err_code = CTB_ERR_READ;
        return 0;
        }
sign=1;
if( l_seek<0 )
    {
    l_seek = -l_seek;
    sign=2;
    }
if( f_seek<0 )
        {
        ctb_err_code = CTB_ERR_KILLED;
        return 0;       // reading element is deleted                           //
        }
// read index   //

fp=hnd->bas;
if( fseek(fp,f_seek,SEEK_SET) )
        {
        ctb_err_code = CTB_ERR_SEEK;
        return 0;
        }
l = (size_t)(l_seek+datalen);
if( l_seek>hnd->len )
    {
    ctb_err_code = CTB_ERR_UNKNOWN_PACK;
        return 0; // unknown packing type //
    }

if( fread(save_pack,l,1,fp)!=RET_FREAD(l,1) )
        {
        ctb_err_code = CTB_ERR_READ;
        return 0;
        }
save_pack[l]=save_pack[l+1]=0;
// read data and picture        //
if( data )
    {
    memcpy(data,save_pack,datalen);
    if( hnd->version<6 )
        data[0]=CTB_OEM_CHARSET;
    }
w = data[1];
h = data[2];
if( !w || !h )
    {
        ctb_err_code = CTB_ERR_READ;
        return 0;
        }

wb =  gray ? (((Int32)w+7)/8)*8 : (((Int32)w+7)/8);
len = wb*(Int32)h;
if( hnd->type==CTB_256_128_2 && (w>255 || h>127 || len>REC_MAX_RASTER_SIZE) )
    {
        ctb_err_code = CTB_ERR_READ;
        return FALSE;
        }
if( hnd->version>=6 )
    {
    switch( save_pack[datalen+0] )
            {
            case        CTB_COMP_NONE:  // no compression
                    memcpy(save_bin,&save_pack[datalen+1],len);
                    break;
            case        CTB_COMP_PCX:   // decode picture by PCX-compressing
                    decLine(&save_pack[datalen+1],(Int16)l_seek,save_bin);
                    break;
            default:
                    ctb_err_code = CTB_ERR_UNKNOWN_PACK;
                    return 0; // unknown packing type //
            }
    }
else
    {
    switch( save_pack[0] )
            {
            case        CTB_COMP_NONE:  // no compression
                    memcpy(save_bin,&save_pack[datalen],len);
                    break;
            case        CTB_COMP_PCX:   // decode picture by PCX-compressing
                    decLine(&save_pack[datalen],(Int16)l_seek,save_bin);
                    break;
            default:
                    ctb_err_code = CTB_ERR_UNKNOWN_PACK;
                    return 0; // unknown packing type //
            }
    }
{
Word8 mask=0;
if( hnd->type==CTB_256_128_2   )
    {
    mask = (Word8)mask_r[w&7];
    }
xor_lines_rest(save_bin,!plane?wb:wb/8,len,mask);
}
return sign;
}

CTB_FUNC(Int32)  CTB_volume(CTB_handle *hnd )
{
return hnd->num;
}

CTB_FUNC(Int32)  CTB_volume_all(char *filename )
{
struct stat sts;
char lin[MAXPATH],file_name[MAXPATH],*p;

strcpy(file_name,filename);
p=ctb_last_punct(file_name);
if( p )
    *p='\0';
SPRINTF(lin,"%s/%s.IND",ctb_tmp_dir,file_name);
if( STAT(lin,&sts)==-1 )
        return 0;

return (Int32)(sts.st_size/8);
}



CTB_FUNC(Bool32)  CTB_read_global_data(CTB_handle *hnd, Word8 *data)
{
Int32   gdatalen;
ctb_err_code = CTB_ERR_NONE;
if( data==NULL )
        {
        ctb_err_code = CTB_ERR_ZERO_DATA;
        return FALSE; // can't store to it //
        }
switch( hnd->version )
    {
    case    3:

        gdatalen=16;
        break;
    case    4:
    case    5:
        gdatalen=32;
        break;
    case    6:
    case    7:
        gdatalen=CTB_DATA_SIZE;
        break;
    }
memcpy(data,zero_data,gdatalen);
if( hnd==NULL )
        {
        ctb_err_code = CTB_ERR_NOT_OPEN;
        return FALSE;   // file is not opened //
        }

if( fseek(hnd->bas,sizeof(H_CTB_file)-gdatalen,SEEK_SET) )
        {
        ctb_err_code = CTB_ERR_SEEK;
        return FALSE;
        }
if( fread(data,gdatalen,1,hnd->bas)!=RET_FREAD(gdatalen,1) )
        {
        ctb_err_code = CTB_ERR_READ;
        return FALSE;
        }
switch( hnd->version )
    {
    case    3:
    case    4:
    case    5:
        data[0]=0;
        break;
    }
return( TRUE );
}


CTB_FUNC(Bool32)  CTB_read_data(CTB_handle *hnd, Int32 num, Word8 *data)
{
Int32 f_seek, datalen;
FFILE fp;

ctb_err_code = CTB_ERR_NONE;
if( data==NULL )
        {
        ctb_err_code = CTB_ERR_ZERO_DATA;
        return FALSE; // can't store to it //
        }
switch( hnd->version )
    {
    case    3:

        datalen=16;
        break;
    case    4:
    case    5:
        datalen=32;
        break;
    case    6:
    case    7:
        datalen=hnd->attr_size;
        break;
    }
memcpy(data,zero_data,datalen);
if( hnd==NULL )
        {
        ctb_err_code = CTB_ERR_NOT_OPEN;
        return FALSE;   // file is not opened //
        }
fp = hnd->ndx;
if( fseek(fp,num*8,SEEK_SET) )
        {
        ctb_err_code = CTB_ERR_SEEK;
        return FALSE;
        }
if( fread(&f_seek,4,1,fp)!=RET_FREAD(4,1) )
        {
        ctb_err_code = CTB_ERR_READ;
        return FALSE;
        }
if( f_seek<0 )
        {
        ctb_err_code = CTB_ERR_SEEK;
        return FALSE; // deleting image //
        }
// read index   //

fp = hnd->bas;
if( fseek(fp,f_seek,SEEK_SET))
        {
        ctb_err_code = CTB_ERR_SEEK;
        return FALSE;
        }

if( fread(data,datalen,1,fp)!=RET_FREAD(datalen,1) )
        {
        ctb_err_code = CTB_ERR_READ;
        return FALSE;
        }
if( hnd->version<6 )
    {
    data[0]=CTB_OEM_CHARSET;
    }
return TRUE;
}

//********************************************************************
// ************** subroutines to write data to a file ***************** //
//********************************************************************
CTB_FUNC(Bool32)  CTB_write_mark( CTB_handle *hnd, Int32 num,Word8 *bin, Word8 *data , Bool32  mark)
{
Int16   sp,n=(Int16)hnd->len;
FFILE   fp;
Int32   pos, datalen, wb;
Int16   len;
Word8   w,h;
Bool32  gray=(hnd->signums&CTB_GRAY_SCALE),
        plane=(hnd->signums&CTB_PLANE);

ctb_err_code = CTB_ERR_NONE;
if( hnd==NULL )
        {
        ctb_err_code = CTB_ERR_NOT_OPEN;
        return FALSE;   // file is not opened //
        }
if( data==NULL )
        {
        ctb_err_code = CTB_ERR_ZERO_DATA;
        return 0;       // for CTB ONLY need setup Width & Heigth //
        }

w = data[1];
h = data[2];
if( !w || !h )
    {
        ctb_err_code = CTB_ERR_WRITE;
        return FALSE;
        }

wb =  gray ? (((Int32)w+7)/8)*8 : (((Int32)w+7)/8);

n = len = (Int16)(wb*(Int32)h);
if( hnd->type==CTB_256_128_2 && (w>255 || h>127 || len>REC_MAX_RASTER_SIZE) )
    {
        ctb_err_code = CTB_ERR_WRITE;
        return FALSE;
        }

xor_lines(bin,!plane?wb:wb/8,len);
sp = encLine(bin, n,&save_pack[1],len);

if( sp>=n )
        {    // copy bit map to out buffer if PCX can't decrease size //
        sp = n;
    memcpy(&save_pack[1],bin,sp);
    save_pack[0]=CTB_COMP_NONE;
    sp++;
        }
else
    {
    save_pack[0]=CTB_COMP_PCX;
    sp++;
    }

fp=hnd->bas;
if(fseek(fp,0,SEEK_END) )
        {
        ctb_err_code = CTB_ERR_SEEK;
        return FALSE;
        }
pos=ftell(fp);
switch( hnd->version )
    {
    case    3:

        datalen=16;
        break;
    case    4:
    case    5:
        datalen=32;
        break;
    case    6:
    case    7:
        datalen=hnd->attr_size;
        break;
    }
if( fwrite(data?data:zero_data,datalen,1,fp)!=RET_FWRITE(datalen,1) )
        {
        ctb_err_code = CTB_ERR_WRITE;
        return FALSE;
        }
if( fwrite(save_pack,sp,1,fp)!=RET_FWRITE(sp,1) )
        {
        ctb_err_code = CTB_ERR_WRITE;
        return FALSE;
        }

fp=hnd->ndx;
if( num>-1 )
        {
        if(     num<hnd->num )
                {       // rewrite index //
                if( fseek(fp,num*8,SEEK_SET) )
                        {
                        ctb_err_code = CTB_ERR_SEEK;
                        return FALSE;
                        }
                hnd->need_compress=1;
                }
        else
                {
                ctb_err_code = CTB_ERR_NDX;
                return FALSE;
                }
        }
else
                { // adding index  //
                if( !hnd->need_compress )
                        {
                        if( fseek(fp,0,SEEK_END) )
                                {
                                ctb_err_code = CTB_ERR_SEEK;
                                return FALSE;
                                }
                        }
                else
                        {
                        if( fseek(fp,(long)hnd->num*8, SEEK_SET) )
                                {
                                ctb_err_code = CTB_ERR_SEEK;
                                return FALSE;
                                }
                        }
                hnd->num++;
                }

if( fwrite(&pos,sizeof(pos),1,fp)!=RET_FWRITE(sizeof(pos),1) )
        {
        ctb_err_code = CTB_ERR_WRITE;
        return FALSE;
        }
pos = mark?-sp:sp;
if( fwrite(&pos,sizeof(pos),1,fp)!=RET_FWRITE(sizeof(pos),1) )
        {
        ctb_err_code = CTB_ERR_WRITE;
        return FALSE;
        }
hnd->is_sort = 0;
return TRUE;
}

CTB_FUNC(Bool32)  CTB_write( CTB_handle *hnd, Int32 num,Word8 *bin, Word8 *data )
{
Bool32 ret = CTB_write_mark(hnd,num,bin,data,FALSE);
if( ret )
    hnd->is_sort = 0;
return ret;
}

CTB_FUNC(Bool32)  CTB_kill(CTB_handle *hnd, Int32 num)
{
long f_seek;
FFILE fp;

ctb_err_code = CTB_ERR_NONE;
if( hnd==NULL )
    {
    ctb_err_code = CTB_ERR_NOT_OPEN;
        return FALSE;   // file is not opened //
    }
fp = hnd->ndx;
if( fseek(fp,num*8,SEEK_SET) )
    {
    ctb_err_code = CTB_ERR_SEEK;
        return FALSE;
    }
if( fread(&f_seek,4,1,fp)!=RET_FREAD(4,1) )
    {
    ctb_err_code = CTB_ERR_READ;
        return FALSE;
    }
if( f_seek<0 )
    {
    return TRUE;  // already killed //
    }

f_seek = - f_seek;
if( fseek(fp,num*8,SEEK_SET) )
    {
    ctb_err_code = CTB_ERR_SEEK;
        return FALSE;
    }
if( fwrite(&f_seek,4,1,fp)!=RET_FWRITE(4,1) )
    {
    ctb_err_code = CTB_ERR_WRITE;
        return FALSE;
    }

hnd->num--;
hnd->need_compress=1;
return TRUE;
}


CTB_FUNC(Bool32)  CTB_swap(CTB_handle *hnd,
                                                   Int32 num1,Int32 num2)
{
long f_seek1, l_seek1;
long f_seek2, l_seek2;
FFILE fp;

ctb_err_code = CTB_ERR_NONE;
if( hnd==NULL )
    {
    ctb_err_code = CTB_ERR_NOT_OPEN;
        return FALSE;   // file is not opened //
    }
if( num1<0 || num1>=hnd->num || num2<0 || num2>=hnd->num )
    {
    ctb_err_code = CTB_ERR_NDX;
        return FALSE;
    }

fp = hnd->ndx;

if( fseek(fp,num1*8,SEEK_SET) )
    {
    ctb_err_code = CTB_ERR_SEEK;
        return FALSE;
    }
if( fread(&f_seek1,4,1,fp)!=RET_FREAD(4,1) )
    {
    ctb_err_code = CTB_ERR_READ;
        return FALSE;
    }
if( f_seek1<0 )
    {
    ctb_err_code = CTB_ERR_KILLED;
        return FALSE; // deleted image //
    }

if( fread(&l_seek1,4,1,fp)!=RET_FREAD(4,1) )
    {
    ctb_err_code = CTB_ERR_READ;
        return FALSE;
    }

if( fseek(fp,num2*8,SEEK_SET) )
    {
    ctb_err_code = CTB_ERR_SEEK;
        return FALSE;
    }
if( fread(&f_seek2,4,1,fp)!=RET_FREAD(4,1) )
    {
    ctb_err_code = CTB_ERR_READ;
        return FALSE;
    }
if( f_seek2<0 )
    {
    ctb_err_code = CTB_ERR_KILLED;
        return FALSE; // deleted image //
    }
if( fread(&l_seek2,4,1,fp)!=RET_FREAD(4,1) )
    {
    ctb_err_code = CTB_ERR_READ;
        return FALSE;
    }

if( fseek(fp,num1*8,SEEK_SET) )
    {
    ctb_err_code = CTB_ERR_SEEK;
        return FALSE;
    }
if( fwrite(&f_seek2,4,1,fp)!=RET_FWRITE(4,1) )
    {
    ctb_err_code = CTB_ERR_WRITE;
        return FALSE;
    }
if( fwrite(&l_seek2,4,1,fp)!=RET_FWRITE(4,1) )
    {
    ctb_err_code = CTB_ERR_WRITE;
        return FALSE;
    }

if( fseek(fp,num2*8,SEEK_SET) )
    {
    ctb_err_code = CTB_ERR_SEEK;
        return FALSE;
    }
if( fwrite(&f_seek1,4,1,fp)!=RET_FWRITE(4,1) )
    {
    ctb_err_code = CTB_ERR_WRITE;
        return FALSE;
    }
if( fwrite(&l_seek1,4,1,fp)!=RET_FWRITE(4,1) )
    {
    ctb_err_code = CTB_ERR_WRITE;
        return FALSE;
    }
hnd->need_compress=1;
hnd->is_sort = 0;
return TRUE;
}

CTB_FUNC(Bool32)  CTB_mark(CTB_handle *hnd, Int32 num)
{
long f_seek, l_seek;
FFILE fp;

ctb_err_code = CTB_ERR_NONE;
if( hnd==NULL )
    {
    ctb_err_code = CTB_ERR_NOT_OPEN;
        return FALSE;   // file is not opened //
    }
if( num<0 || num>=hnd->num )
    {
    ctb_err_code = CTB_ERR_NDX;
        return FALSE;
    }
fp = hnd->ndx;
if( fseek(fp,num*8,SEEK_SET) )
    {
    ctb_err_code = CTB_ERR_SEEK;
        return FALSE;
    }
if( fread(&f_seek,4,1,fp)!=RET_FREAD(4,1) )
    {
    ctb_err_code = CTB_ERR_READ;
        return FALSE;
    }
if( f_seek<0 )
    {
    ctb_err_code = CTB_ERR_KILLED;
        return FALSE; // deleted image //
    }
if( fread(&l_seek,4,1,fp)!=RET_FREAD(4,1) )
    {
    ctb_err_code = CTB_ERR_READ;
        return FALSE;
    }

l_seek = - l_seek;
if( fseek(fp,num*8,SEEK_SET) )
    {
    ctb_err_code = CTB_ERR_SEEK;
        return FALSE;
    }
if( fwrite(&f_seek,4,1,fp)!=RET_FWRITE(4,1) )
    {
    ctb_err_code = CTB_ERR_WRITE;
        return FALSE;
    }
if( fwrite(&l_seek,4,1,fp)!=RET_FWRITE(4,1) )
    {
    ctb_err_code = CTB_ERR_WRITE;
        return FALSE;
    }

return TRUE;
}

// delete image num //
CTB_FUNC(Bool32)  CTB_delete( CTB_handle *hnd,Int32 num )
{
Word8 buffer[8];
Int16 i,n=hnd->num - 1;

ctb_err_code = CTB_ERR_NONE;
if( hnd==NULL )
    {
    ctb_err_code = CTB_ERR_NOT_OPEN;
        return FALSE;   // file is not opened //
    }
if( num<0 || num>n )
    {
    ctb_err_code = CTB_ERR_NDX;
        return FALSE;
    }

for(i=(Int16)num;i<n;i++)
        {
        if( fseek(hnd->ndx,(i+1)*8,SEEK_SET) )
        {
        ctb_err_code = CTB_ERR_SEEK;
                return FALSE;
        }
        if( fread(buffer,8,1,hnd->ndx)!=RET_FREAD(8,1) )
        {
        ctb_err_code = CTB_ERR_READ;
                return FALSE;
        }
        if( fseek(hnd->ndx,i*8,SEEK_SET) )
        {
        ctb_err_code = CTB_ERR_SEEK;
                return FALSE;
        }
        if( fwrite(buffer,8,1,hnd->ndx)!=RET_FWRITE(8,1) )
        {
        ctb_err_code = CTB_ERR_WRITE;
                return FALSE;
        }
        }
CTB_kill(hnd,n);

return TRUE;
}

// insert kadr (bin,data) after (num-1) image, before num image //
CTB_FUNC(Bool32)  CTB_insert( CTB_handle *hnd,Int32 num,Word8 *bin,     Word8 *data )
{
Word8 buffer[8];
Int32 i,n=hnd->num - 1;

ctb_err_code = CTB_ERR_NONE;
if( hnd==NULL )
    {
    ctb_err_code = CTB_ERR_NOT_OPEN;
        return FALSE;   // file is not opened //
    }
if( num<=n )
        {
        for(i=n;i>=num;i--)
                {
                if( fseek(hnd->ndx,i*8,SEEK_SET) )
                        {
            ctb_err_code = CTB_ERR_SEEK;
            return FALSE;
            }
                if( fread(buffer,8,1,hnd->ndx)!=RET_FREAD(8,1) )
                        {
            ctb_err_code = CTB_ERR_READ;
            return FALSE;
            }
                if( fseek(hnd->ndx,(i+1)*8,SEEK_SET) )
                        {
            ctb_err_code = CTB_ERR_SEEK;
            return FALSE;
            }
                if( fwrite(buffer,8,1,hnd->ndx)!=RET_FWRITE(8,1) )
            {
            ctb_err_code = CTB_ERR_WRITE;
                        return FALSE;
            }
                }
  if( !CTB_write( hnd,num,bin,data) )
                return FALSE; // save image //
        hnd->num++;
        }
else
  if( !CTB_write( hnd,-1,bin,data) )
                return FALSE; // save image //
hnd->is_sort = 0;
return TRUE;
}

CTB_FUNC(Bool32)  CTB_write_data(CTB_handle *hnd, Int32 num, Word8 *data)
{
Int32 f_seek, datalen;
FFILE fp;

if( hnd->version<CTB_VERSION )
    {
    ctb_err_code = CTB_ERR_DIF_VERSION;
    return FALSE; // can't save it //
    }

ctb_err_code = CTB_ERR_NONE;
if( data==NULL )
    {
    ctb_err_code = CTB_ERR_ZERO_DATA;
    return FALSE; // can't save it //
    }
if( hnd==NULL )
    {
    ctb_err_code = CTB_ERR_NOT_OPEN;
    return FALSE;   // file is not opened //
    }
fp = hnd->ndx;
if( fseek(fp,num*8,SEEK_SET) )
    {
    ctb_err_code = CTB_ERR_SEEK;
    return FALSE;
    }
if( fread(&f_seek,4,1,fp)!=RET_FREAD(4,1) )
    {
    ctb_err_code = CTB_ERR_READ;
    return FALSE;
    }

if( f_seek<0 )
    {
    ctb_err_code = CTB_ERR_KILLED;
    return FALSE; // deleting image //
    }
// read index   //

fp = hnd->bas;
if( fseek(fp,f_seek,SEEK_SET))
    {
    ctb_err_code = CTB_ERR_SEEK;
    return FALSE;
    }
switch( hnd->version )
    {
    case    3:
        datalen=16;
        break;
    case    4:
    case    5:
        datalen=32;
        break;
    case    6:
    case    7:
        datalen=hnd->attr_size;
        break;
    }
if( fwrite(data,datalen,1,fp)!=RET_FWRITE(datalen,1) )
    {
    ctb_err_code = CTB_ERR_WRITE;
    return FALSE;
    }
hnd->is_sort = 0;
return TRUE;
}

CTB_FUNC(Bool32)  CTB_write_global_data(CTB_handle *hnd,Word8 *data)
{
FFILE fp;
Int32 gdatalen;

ctb_err_code = CTB_ERR_NONE;
if( data==NULL )
    {
    ctb_err_code = CTB_ERR_ZERO_DATA;
    return FALSE; // can't save it //
    }
if( hnd==NULL )
        {
    ctb_err_code = CTB_ERR_NOT_OPEN;
    return FALSE;       // file is not opened //
    }
fp = hnd->bas;
if( fseek(fp,16,SEEK_SET) )
        {
    ctb_err_code = CTB_ERR_SEEK;
    return FALSE;
    }
switch( hnd->version )
    {
    case    3:

        gdatalen=16;
        break;
    case    4:
    case    5:
        gdatalen=32;
        break;
    case    6:
    case    7:
        gdatalen=CTB_DATA_SIZE;
        break;
    }
if( fwrite(data,gdatalen,1,fp)!=RET_FWRITE(gdatalen,1) )
        {
    ctb_err_code = CTB_ERR_WRITE;
    return FALSE;
    }

return TRUE;
}


//********************************************************************
//***************** static functions : *******************************
//********************************************************************
static void xor_lines(Word8 *bin,Int32 wb,Int32 len)
{
int i,ii,h=len/wb;
for(ii=(h-1)*wb,i=1;i<h;i++,ii-=wb)
    xor_one_line(&bin[ii],&bin[ii-wb],wb);
return;
}

static void xor_lines_rest(Word8 *bin,Int32 wb,Int32 len,Word8 mask)
{
int i,ii,h=len/wb;
if( mask )
    {
    bin[wb-1]&=mask;
    for(ii=wb,i=1;i<h;i++,ii+=wb)
        {
        xor_one_line(&bin[ii],&bin[ii-wb],wb);
        bin[ii+wb-1]&=mask;
        }

    }
else
    {
    for(ii=wb,i=1;i<h;i++,ii+=wb)
        xor_one_line(&bin[ii],&bin[ii-wb],wb);
    }

return;
}

static Bool32  CTB_type(Int16 wid, Int16 hei, Int16 dpb)
{
if( wid==128 && hei==96 && dpb==8 )
  return CTB_128_96_2;
if( wid==128 && hei==64 && dpb==8 )
  return CTB_128_64_2;
if( wid==128 && hei==64 && dpb==4 )
  return CTB_128_64_4;
if( wid==128 && hei==96 && dpb==2 )
  return CTB_128_96_16;
if( wid==128 && hei==64 && dpb==2 )
  return CTB_128_64_16;
if( wid==256 && hei==96 && dpb==8 )
  return CTB_256_96_2;
if( wid==256 && hei==96 && dpb==2 )
  return CTB_256_96_16;
if( wid==384 && hei==64 && dpb==8 )
  return CTB_384_64_2;
if( wid==384 && hei==32 && dpb==8 )
  return CTB_384_32_2;
if( wid==128 && hei==96 && dpb==4 )
  return CTB_128_96_4;
if( wid==256 && hei==128 && dpb==4 )
  return CTB_256_128_4;
if( wid==256 && hei==96 && dpb==4 )
  return CTB_256_96_4;
if( wid==256 && hei==128 && dpb==8 )
  return CTB_256_128_2;
if( wid==256 && hei==128 && dpb==2 )
  return CTB_256_128_16;
if( wid==128 && hei==128 && dpb==2 )
  return CTB_128_128_16;
if( wid==256 && hei==128 && dpb==1 )
  return CTB_256_128_256;

return CTB_UNKNOWN;
}

static Int32 CTB_volume_true(char *filename )
{
struct stat sts;
char lin[MAXPATH],file_name[MAXPATH],*p;
Int32 i,n,k;
Int32 fs,fl;
FFILE fp;

strcpy(file_name,filename);
p=ctb_last_punct(file_name);
if( p ) *p='\0';
SPRINTF(lin,"%s/%s.IND",ctb_tmp_dir,file_name);
if( STAT(lin,&sts)==-1 )
        return 0;

n = (Int32)(sts.st_size/8);

fp=fopen(lin,R_B);
if( fp==BAD_FOPEN )
        return 0;
for(k=i=0;i<n;i++)
        {
        fread(&fs,4,1,fp);
        fread(&fl,4,1,fp);
        if( fs>0 ) k++;
        }
fclose(fp);

return k;
}
static void xor_one_line(Word8 *bin,Word8 *bin1,Int32 wb)
{
int i;
for(i=0;i<wb;i++)
    bin[i] ^= bin1[i];
return;
}

//********************************************************************
//************ global functions : ************************************
//********************************************************************
char * ctb_last_punct(char *word)
{
char *p = word + STRLEN(word) - 1;
Int16 i=0;

do{
  if( *p==':' || *p=='\\' )
    return NULL; // disk name or end of filename //
  if( *p=='.' )
    return p;    // exist punct && i<4           //
  p--; i++;
  } while( i<4 );
return NULL;     // i==4                         //
}

Bool32  CTB_files_init(char *filename,Word8 *data,Int16 maxX, Int16 maxY,
      Int16 dpb, Word8 signums, Word8 attr_size)
{
H_CTB_file H_CTB={{'C','T'},0,0,0,0,0,0,0,0,{0}}; // signatura only //
char s[MAXPATH],file_name[MAXPATH],*p;
FFILE fp;
Int16 l;

strcpy(file_name,filename);
p=ctb_last_punct(file_name);
ctb_err_code = CTB_ERR_NONE;
if( p ) *p='\0';
SPRINTF(s,"%s/%s.CTB",ctb_tmp_dir,file_name);
UNLINK(s);
fp = FCREAT(s,0);
if( fp==BAD_FOPEN )
        {
        ctb_err_code = CTB_ERR_OPEN_CTB;
        return FALSE;
        }
H_CTB.version      = CTB_VERSION; // version number
H_CTB.size_x       = maxX;        // frame width
H_CTB.size_y       = maxY;        // frame height
H_CTB.dot_per_byte = (Word8)dpb;  // point inbyte
H_CTB.signums      = signums;     // characteristics
H_CTB.attr_size    = attr_size;   // number of attributes
l = sizeof(H_CTB)-CTB_DATA_SIZE;
if( fwrite(&H_CTB,l,1,fp)!=RET_FWRITE(l,1) )
        {
        ctb_err_code = CTB_ERR_READ;
        fclose(fp);
        return FALSE;
        }
if( fwrite( data?data:zero_data,CTB_DATA_SIZE,1,fp)!=RET_FWRITE(CTB_DATA_SIZE,1) )
        {
        ctb_err_code = CTB_ERR_READ;
        fclose(fp);
        return FALSE;
        }
fclose(fp);

SPRINTF(s,"%s/%s.IND",ctb_tmp_dir,file_name);
fp = FCREAT(s, 0);
if( fp==BAD_FOPEN )
        {
        ctb_err_code = CTB_ERR_OPEN_NDX;
        return FALSE;
        }
fclose(fp);

return TRUE;
}

CTB_FUNC(Int32)   CTB_GetVersion(void )
{
return CTB_VERSION;
}
