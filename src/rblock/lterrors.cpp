/*
 Copyright (c) 1993-2008, Cognitive Technologies
 All rights reserved.

 ����������� ��������� ��������������� � ������������� ��� � ���� ��������� ����,
 ��� � � �������� �����, � ����������� ��� ���, ��� ���������� ��������� �������:

 * ��� ��������� ��������������� ��������� ���� ������ ���������� ���������
 ���� ����������� �� ��������� �����, ���� ������ ������� � �����������
 ����� �� ��������.
 * ��� ��������� ��������������� ��������� ���� � ������������ �/��� �
 ������ ����������, ������������ ��� ���������������, ������ �����������
 ��������� ���� ���������� �� ��������� �����, ���� ������ ������� �
 ����������� ����� �� ��������.
 * �� �������� Cognitive Technologies, �� ����� �� ����������� �� �����
 ���� ������������ � �������� �������� ��������� �/��� �����������
 ���������, ���������� �� ���� ��, ��� ���������������� �����������
 ����������.

 ��� ��������� ������������� ����������� ��������� ���� �/��� ������� ������ "���
 ��� ����" ��� ������-���� ���� ��������, ���������� ���� ��� ���������������,
 ������� �������� ������������ �������� � ����������� ��� ���������� ����, �� ��
 ������������� ���. �� �������� ��������� ���� � �� ���� ������ ����, �������
 ����� �������� �/��� �������� �������������� ���������, �� � ���� ������ ��
 ��Ѩ� ���������������, ������� ����� �����, ���������, ����������� ���
 ������������� ������, ��������� � �������������� ��� ���������� ����������
 ������������� ������������� ��������� (������� ������ ������, ��� ������,
 ������� ���������, ��� ������ �/��� ������ �������, ���������� ��-�� ��������
 ������� ��� �/��� ������ ��������� �������� ��������� � ������� �����������,
 �� �� ������������� ����� ��������), �� �� ������������� ���, ���� ���� �����
 �������� ��� ������ ���� ���� �������� � ����������� ����� ������� � ������.

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

/****************************************************************************
 *                                                                          *
 *                                                                          *
 *                   P A G E   L A Y O U T                                  *
 *                                                                          *
 *              S T R I N G S   E X T R A C T I O N                         *
 *                                                                          *
 *              Written in 1991 by Yuri Panchul                             *
 *              Update  in 1994 by Pit  Khlebutin                           *
 *                                                                          *
 *              LTERRORS.C - common purposes data and routines              *
 *                                                                          *
 ***************************************************************************/
#define __LTERRORS__

#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

# include "resource.h"
# include "newfunc.h"

# include "lterrors.h"
# include "layout.h"
# include "extract.h"

# include "glalloc.h"
# include "dpuma.h"

extern jmp_buf fatal_error_exit;

void FreeAllData(void) {
    SE_FreeAllData();
    LT_FreeAllData();

# ifdef MA_DEBUG
    AllocationsAccountingClose();
# endif
}

void ErrorNoEnoughMemory(const char * message) {
    LDPUMA_Console("No enough memory - press any key\nMESSAGE:%s", message);
    //    LT_Getch ();
    FreeAllData();
    SetReturnCode_rblock(IDS_ERR_NO_MEMORY);
    longjmp(fatal_error_exit, -1);
    //    exit (-1);
}

void ErrorInternal(const char * s) {
    LDPUMA_Console("Internal error: %s - press any key\n", s);
    //    LT_Getch ();
    FreeAllData();
    SetReturnCode_rblock(IDS_ERR_INTERNAL);
    longjmp(fatal_error_exit, -1);
    //    exit (-1);
}

# include "msgerr.h"

# define ERROR_NO_ENOUGH_MEMORY  6
# define ERROR_INCLINE           7
# define ERROR_EMPTY             8
# define ERROR_FILE              8
# define ERROR_INTERNAL          8

#define sizeof(a) ((long)sizeof(a))

#ifdef DebugFile
//  03-24-94 06:30pm,Pit �� ����祭�� DebugFile:
//      1. �뢮����� �⫠��筠� ���ଠ�� � 䠩� FileError
extern char StringError[]; // �६����� ��ப� ��� ᮮ�饭��
extern FILE * FileError; // 䠩� ��� ᮮ�饭��
char * TemporaleString[80]; // �६����� ��ப� ��� ᮮ�饭��
#endif

struct MemAllocate
{
#define MEMFREE         0x00   // ������ ᢮�����
#define MEMMALLOC       0x01   // ������ �⢥���� �� malloc
#define MEMEND          0x04   // ��� ��᫥����� �����
    char id; // ����⥫� ����� �.���
    long size; // ࠧ��� �����
};
typedef struct MemAllocate Memory;

char * lout_memory = NULL; // ���� ��砫� ����

#define SizeBuffer      (2048*1024L)// ࠧ��� �� ��� ࠡ��� � ��ப���
static long SizeAllocate = 0; // �᫮ ࠧ��饭��� ����
static long SizeFree = 0; // �᫮ �᢮��������� ����
static long SizeMemory = 0; // ࠧ��� ����⮩ ���

#ifdef DebugFile
void PrintBlocks()
{
    Memory * Block;
    for(Block=(Memory *)lout_memory;Block->id!=MEMEND;
            Block=(Memory *)((char *)Block+Block->size+sizeof(Memory)))
    fprintf(FileError,"\n%i %li",Block->id,Block->size);
}
#endif

int SetupMemoryLayout() {
#ifdef ONE_BLOCK

    Memory * Block;

    if(lout_memory==NULL) {
        //        if((lout_memory=malloc(SizeBuffer))==NULL) return 1;
        if((lout_memory=TigerAllocateMemory(SizeBuffer))==NULL) return 1;
    }
    Block=(Memory *)lout_memory;
    Block->id=MEMEND;
    Block->size=SizeBuffer-sizeof(Memory); // �ᥣ� ����� � ����稨
    SizeAllocate=0; // �᫮ ࠧ��饭��� ����
    SizeFree =0; // �᫮ �᢮��������� ����

#endif

    return 0;
}

void ClearLayoutMemory() {
    if (lout_memory)
    //	{ free(lout_memory); lout_memory=NULL;}
    {
        TigerFreeMemory(lout_memory);
        lout_memory = NULL;
    }
}

static void JoinEmptyBlocks() {
    //long     sizeblock;
    Memory * Block;
    Memory * NextBlock;

    if (lout_memory == NULL)
        ErrorInternal("Malloc:��� �� ���!");
    // ���㯭���� ������ ������
    for (Block = (Memory *) lout_memory; Block->id != MEMEND; Block = (Memory *) ((char *) Block
            + Block->size + sizeof(Memory))) {

        NextBlock = (Memory *) ((char *) Block + Block->size + sizeof(Memory));
        toNextBlock: if (Block->id == MEMFREE && (NextBlock->id == MEMFREE || NextBlock->id
                == MEMEND)) {
            Block->id = NextBlock->id;
            Block->size += NextBlock->size + sizeof(Memory);
            if (Block->id == MEMEND) {
                SizeMemory = (long) ((char *) Block - lout_memory);
                break;
            }
            NextBlock = (Memory *) ((char *) NextBlock + NextBlock->size + sizeof(Memory));
            goto toNextBlock;
        }
    }

}

void * DebugMalloc(size_t size) {
    void * memvoid;
    int idblock;
    long sizeblock;
    Memory * Block;
    Memory * NextBlock;

    if (lout_memory == NULL)
        ErrorInternal("Malloc:��� �� ���!");
    if (size == 0)
        return NULL;

    JoinEmptyBlocks();

    // ���� ᢮������� ����
    for (Block = (Memory *) lout_memory; (char *) Block < (lout_memory + SizeBuffer); Block
            = (Memory *) ((char *) Block + sizeof(Memory) + Block->size)) {
        idblock = Block->id;
        sizeblock = Block->size;

        NextBlock = Block;

        if (idblock == MEMEND || idblock == MEMFREE) {
            if (sizeblock >= size + sizeof(Memory)) {
                // ������塞 �������� ᢮����� ����
                Block->id = MEMMALLOC;
                Block->size = size;
                memvoid = (void *) ((char *) Block + sizeof(Memory));
                // ᮧ���� ���� ᢮����� ����
                Block = (Memory *) ((char *) Block + size + sizeof(Memory));
                Block->id = idblock;
                Block->size = sizeblock - (size + sizeof(Memory));
                if (idblock == MEMEND)
                    SizeMemory = (long) ((char *) Block - lout_memory);
                goto YES;
            }
            else {
                if (idblock == MEMEND)
                    return NULL; //  ErrorNoEnoughMemory("��� ������ ���௠��...");
            }
        } // if ...
    }// for ...

    ErrorNoEnoughMemory("���� �� ���᪥ ᢮������� ����...");
    return NULL;

    YES:
#ifdef DebugFile
    SizeAllocate+=size;
    if(FileError) {
        fprintf(FileError,"\nMalloc \t= %8li\tRunning \t= %li\tAll %li",
                (long)size,
                SizeAllocate-SizeFree,
                SizeMemory);
    }
#endif
    return memvoid;
}
// ���� 㦥 ࠧ��饭���� �����
Memory * FindMem(void * blk) {
    Memory * Block;
    for (Block = (Memory *) lout_memory; Block->id != MEMEND; Block = (Memory *) ((char *) Block
            + Block->size + sizeof(Memory))) {
#ifdef DebugFile
        if((char *)Block>(lout_memory+SizeBuffer))
        ErrorInternal("\n���� ����� �� ���᪥ �����...");
#endif
        if (blk == ((char *) Block + sizeof(Memory))) {
#ifdef DebugFile
            if(Block->id==MEMFREE)
            fprintf(FileError,"\n������ �᢮�������� ����...");
#endif
            return Block;
        } // if(blk...
    }// for(...
    return NULL;
}

void DebugFree(void * blk) {
    Memory * Block;

    if (lout_memory == NULL)
        ErrorInternal("Free:��� �� ���!");
    if (blk == NULL)
        ErrorInternal("����⪠ �᢮������ ������ �� �� ࠧ��饭�� !");
    if ((Block = FindMem(blk)) == NULL) {
        //ErrorInternal("�᢮��������� ������ �� ������� !");
        return;
    }
    //�᢮�������� �����
    Block->id = MEMFREE;
    SizeFree += Block->size;
#ifdef DebugFile
    if(FileError) {
        fprintf(FileError,"\nFree \t= %8li\tRunning \t= %li\tAll %li",
                (long)Block->size,
                SizeAllocate-SizeFree,
                SizeMemory);
    }
#endif
}

void * DebugRealloc(void * old_blk, size_t size) {
    void * new_blk;
    Memory * Block;

    if (lout_memory == NULL)
        ErrorInternal("Realloc:��� �� ���!");
    if (size == 0)
        return NULL;
    if (old_blk == NULL) {
        new_blk = DebugMalloc(size);
        return new_blk;
    }

    if ((Block = FindMem(old_blk)) == NULL)
        ErrorInternal("\n��室�� ���� ��� REALLOC �� ������!");

    if (Block->size <= size + sizeof(Memory)) {
        // �ॡ���� ���� ������ ࠧ��஢
        new_blk = DebugMalloc(size);

        if (new_blk != NULL && old_blk != NULL)
            memcpy(new_blk, old_blk, (Block->size <= size ? Block->size : size));

        if (old_blk != NULL)
            DebugFree(old_blk);
    }
    else {
        // �ॡ���� ���� ������ ࠧ��஢
        Memory * NewBlock = (Memory *) ((char *) Block + size + sizeof(Memory));
        NewBlock->id = MEMFREE;
        NewBlock->size = Block->size - (size + sizeof(Memory));

        SizeFree += NewBlock->size;

        Block->size = size;
        new_blk = old_blk;
#ifdef DebugFile
        if(FileError) {
            fprintf(FileError,"\nRealloc \t= %8li\tRunning \t= %li\tAll %li",
                    (long)Block->size,
                    SizeAllocate-SizeFree,
                    SizeMemory);
        }
#endif

    }

    return new_blk;
}

void ErrorNoEnoughMemory(puchar message) {

    message = message;
    FreeAllData();
#ifdef DebugFile

    fprintf(FileError,"\nMESSAGE:%s",message);
    fprintf(FileError,"\n%s\n%s",TemporaleString,StringError);

    fclose(FileError);
#endif
    error_exit(ERR_comp, ERROR_NO_ENOUGH_MEMORY);
}

void ErrorInternal(char * s) {

    s = s;
    FreeAllData();
#ifdef DebugFile

    fprintf(FileError,"\n%s",s);

    fclose(FileError);
#endif
    error_exit(ERR_comp, ERROR_INTERNAL);
}

void ErrorEmptyPage(void) {
    FreeAllData();
#ifdef DebugFile

    fprintf(FileError,"\nPage is empty");

    fclose(FileError);
#endif
    error_exit(ERR_comp, ERROR_EMPTY);
}

void ErrorFile(void) {
    FreeAllData();
    error_exit(ERR_comp, ERROR_FILE);
}

long GetMaxSizeFreeMem() {
    int idblock;
    long sizeblock;
    long MaxSizeBlock;
    Memory * Block;

    if (lout_memory == NULL)
        ErrorInternal("Malloc:��� �� ���!");

    JoinEmptyBlocks();

    MaxSizeBlock = 0L;

    // ���� ᢮������� ����
    for (Block = (Memory *) lout_memory; (char *) Block < (lout_memory + SizeBuffer); Block
            = (Memory *) ((char *) Block + sizeof(Memory) + Block->size)) {
        idblock = Block->id;
        sizeblock = Block->size;

        if ((idblock == MEMEND || idblock == MEMFREE) && sizeblock > MaxSizeBlock)
            MaxSizeBlock = sizeblock;

    }// for ...

    return MaxSizeBlock;
}
//#endif
