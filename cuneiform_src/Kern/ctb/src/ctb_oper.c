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

//************************************************************************//
//* CTB_oper.c : set of commands uses closed CTB-files********************//
//************************************************************************//

#define MAXPATH 256
#include<string.h>
#include<sys/stat.h>
#include "ctb.h"

/// extern fuxntions and data
extern char * ctb_last_punct(char *word);
extern Bool32 CTB_files_init(char *file_name,uchar *data,int16_t maxX,int16_t maxY,
                    int16_t dpb,uchar signums,uchar attr_size);
extern int32_t ctb_err_code ;   // error code                   //

CTB_FUNC(int32_t)  CTB_compress(char *filename)
{
char *p,tmp_file[MAXPATH],file_name[MAXPATH];
CTB_handle hi,ho;
int32_t i,n,compress;
uchar dst[CTB_DATA_SIZE],buffer[256*128+2+CTB_DATA_SIZE];

p=ctb_last_punct(file_name);
strcpy(file_name,filename);

ctb_err_code = CTB_ERR_NONE;
if( p )	*p='\0';
STRCPY(tmp_file, file_name);
p = STRRCHR(tmp_file,'\\');
if( p )
	{
	*(p+1)='\0';
	STRCAT(tmp_file,"$$$$$$$$");
	}
else
	STRCPY(tmp_file, "$$$$$$$$");

if( !CTB_open(file_name, &hi,"w") )
	return FALSE;

CTB_read_global_data(&hi,dst);
CTB_files_init(tmp_file,dst,hi.width,hi.height, hi.dpb,hi.signums,hi.attr_size);

n = CTB_volume(&hi);
compress = hi.need_compress;

if( compress )
	{
  if( !CTB_open(tmp_file,&ho,"w") )
		return 0;
	for(i=0;i<n;i++)
        {
        switch( CTB_read(&hi,i,buffer,dst) )
            {
            case    1 :
                CTB_write_mark(&ho,-1,buffer,dst,FALSE);
                break;
            case    2:
                CTB_write_mark(&ho,-1,buffer,dst,TRUE);
                break;
            default:
                break;
            }
        }
	ho.need_compress=0;
    CTB_close(&ho);
	}

hi.need_compress=0;
CTB_close(&hi);
if( !compress )
    {   // delete tmp file //
    STRCAT(tmp_file,".CTB");
	UNLINK(tmp_file);
	*ctb_last_punct(tmp_file)=0;
    STRCAT(tmp_file,".IND");
	UNLINK(tmp_file);
	return 0;
	}

STRCAT(file_name,".CTB");
STRCAT(tmp_file,".CTB");
UNLINK(file_name);
RENAME(tmp_file,file_name);
*ctb_last_punct(tmp_file)=0;
*ctb_last_punct(file_name)=0;
STRCAT(file_name,".IND");
STRCAT(tmp_file,".IND");
UNLINK(file_name);
RENAME(tmp_file,file_name);

return n;
}

CTB_FUNC(int32_t)  CTB_rename(char *new_name, char *old_name)
{
char newname[MAXPATH], oldname[MAXPATH];

if( *(new_name+1)!=':' && *(old_name+1)!=':' || // without disk names //
		toupper(*new_name)==toupper(*old_name) &&
		*(new_name+1)==':' && *(old_name+1)==':')   // one disk           //
	{
	char *p = ctb_last_punct(old_name);
	if (p)
	 *p = 0;
	p = ctb_last_punct(new_name);
	if (p)
	 *p = 0;

  SPRINTF(newname,"%s.CTB",new_name);
  SPRINTF(oldname,"%s.CTB",old_name);
  if (!access(newname,0))
         unlink(newname);
  RENAME( oldname, newname);       // two files in one disk  //
  SPRINTF(newname,"%s.IND",new_name);
  SPRINTF(oldname,"%s.IND",old_name);
  if (!access(newname,0))
         unlink(newname);
  RENAME( oldname, newname);       // two files in one disk  //
    return 1;
  }

CTB_move(new_name, old_name); // move from disk to disk //
return 2;
}


CTB_FUNC(int32_t)  CTB_move(char *new_name, char *old_name)
{
int16_t n;

n = (int16_t)CTB_copy(new_name, old_name);
CTB_unlink(old_name);
return n;
}

CTB_FUNC(void) CTB_unlink(char *name)
{
char str[MAXPATH],*p=ctb_last_punct(name);

if( p )	*p='\0';
STRCPY(str,name);
STRCAT(str,".CTB");
UNLINK(str);

STRCPY(str,name);
STRCAT(str,".IND");
UNLINK(str);
return ;
}

CTB_FUNC(int32_t)  CTB_copy(char *new_name, char *old_name)
{
char *p;
CTB_handle hi,ho;
int16_t i,n;
uchar dst[CTB_DATA_SIZE], buffer[256*128+2+CTB_DATA_SIZE];
//size_t size;

ctb_err_code = CTB_ERR_NONE;
p=ctb_last_punct(old_name);
if( p )	*p='\0';
p=ctb_last_punct(new_name);
if( p )	*p='\0';

if( !CTB_open(old_name, &hi,"w") )
	return 0;
CTB_read_global_data(&hi,dst);
CTB_files_init(new_name,dst,hi.width,hi.height,
					hi.dpb,hi.signums,hi.attr_size);

n = (int16_t)CTB_volume(&hi);

if( !CTB_open(new_name,&ho,"w") )
	return 0;

for(i=0;i<n;i++)
    {
    switch( CTB_read(&hi,i,buffer,dst) )
        {
        case    1 :
            CTB_write_mark(&ho,-1,buffer,dst,FALSE);
            break;
        case    2:
            CTB_write_mark(&ho,-1,buffer,dst,TRUE);
            break;
        default:
            break;
        }
    }
CTB_close(&ho);
CTB_close(&hi);

return n;
}



