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

#ifndef __CFIO_H__
#define __CFIO_H__
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Functions
#include "globus.h"
///////////////////////////////////////////////////////////////////////////////////////////////
#ifdef __CFIO__
#define CFIO_FUNC  FUN_EXPO
#else
#define CFIO_FUNC  FUN_IMPO
#endif
///////////////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
///////////////////////////////////////////////////////////////////////////////////////////////
#define CFIO_MAX_PATH          256
#define CFIO_MAX_OWNER         16
#define CFIO_MAX_COMMENT       48
///////////////////////////////////////////////////////////////////////////////////////////////
CFIO_FUNC(Bool32) CFIO_Init(uint16_t wHeightCode,Handle hStorage);
CFIO_FUNC(Bool32) CFIO_Done();
CFIO_FUNC(uint32_t) CFIO_GetReturnCode();
CFIO_FUNC(char *) CFIO_GetReturnString(uint32_t dwError);
CFIO_FUNC(Bool32) CFIO_GetExportData(uint32_t dwType, void * pData);
CFIO_FUNC(Bool32) CFIO_SetImportData(uint32_t dwType, void * pData);
///////////////////////////////////////////////////////////////////////////////////////////////
enum Parameters {
	CFIO_PCHAR_TEMPORARY_FOLDER = 1,
	CFIO_PCHAR_STORAGE_FOLDER,
	CFIO_PCHAR_FILE_FOLDER,
	CFIO_FNOpenStorage,
	CFIO_FNCloseStorage,
	CFIO_FNDeleteStorage,
	CFIO_FNWriteFileToStorage,
	CFIO_FNReadFileFromStorage,
	CFIO_FNOpenFreeFile,
	CFIO_FNCloseFreeFile,
	CFIO_FNWriteToFile,
	CFIO_FNReadFromFile,
	CFIO_FNSeekFilePointer,
	CFIO_FNTellFilePointer,
	CFIO_FNFlushFile,
	CFIO_FNAllocMemory,
	CFIO_FNDAllocMemory,
	CFIO_FNReAllocMemory,
	CFIO_FNFreeMemory,
	CFIO_FNLockMemory,
	CFIO_FNUnlockMemory,
	CFIO_FNWriteMemoryToFile,
	CFIO_FNReadMemoryFromFile,
	CFIO_FNWriteMemoryToStorage,
	CFIO_FNReadMemoryFromStorage
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Folders
enum CFIOFolders {
	CFIO_TEMP_FOLDER = 1, CFIO_FILE_FOLDER, CFIO_STORAGE_FOLDER
};
/////////////////////////////////////////////////////////////////////////////////////////////////////Storages
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
#define DEC_FUN(a,b,c) typedef a (*FNCFIO##b)c; CFIO_FUNC(a) CFIO_##b c
//////////////////////////////////////////////////////////////////////////////////////////
//Open storage
#define   OS_CREATE               0x01
#define   OS_OPEN                 0x02
DEC_FUN(Handle, OpenStorage, (pchar, uint32_t) )
;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Close Storage
#define   CS_WITHOUT_SAVE         0x01                                 // Only close
#define   CS_DELETE               0x02                                 // Delete storage
#define   CS_FILE_DELETE          0x04                                 // Delete all attached files from disk
#define   CS_SAVE                 0x08                                 // Save storage at current state
#define   CS_FILE_SAVE            0x10                                 // Save all attached files
#define   CS_ALL                  0x20                                 // Close all open storages
DEC_FUN(Bool32, CloseStorage, (Handle, uint32_t))
;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Delete storage from disk (don't need to be opened)
DEC_FUN(Bool32, DeleteStorage, (pchar))
;
////////////////////////////////////////////////////////////////////////////////////////////////////////Files
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Write file to storage
DEC_FUN(uint32_t, WriteFileToStorage, (Handle , Handle, pchar ))
;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Read file from storage
DEC_FUN(Handle, ReadFileFromStorage, (Handle , pchar ))
;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Open file
#define   OSF_CREATE               0x01
#define   OSF_OPEN                 0x02
#define   OSF_READ                 0x04
#define   OSF_WRITE                0x08
#define   OSF_BINARY               0x10
#define   OSF_IN_MEMORY            0x20
#define   OSF_TEMPORARY            0x40
DEC_FUN(Handle, OpenFreeFile, (Handle, pchar, uint32_t))
;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Close file
#define   CSF_SAVEDISK             0x01
#define   CSF_SAVESTORAGE          0x02
#define   CSF_DELETE               0x04
#define   CSF_WRITE                0x08
DEC_FUN(Bool32, CloseFreeFile, (Handle, uint32_t))
;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Write data to file
DEC_FUN(uint32_t, WriteToFile, (Handle, pchar, uint32_t))
;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Read data from file
DEC_FUN(uint32_t, ReadFromFile, (Handle, pchar, uint32_t))
;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Seek pointer
#define   FS_END                   0x01
#define   FS_BEGIN                 0x02
#define   FS_CUR                   0x04
DEC_FUN(uint32_t, SeekFilePointer, (Handle, uint32_t, uint32_t))
;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Tell pointer
DEC_FUN(uint32_t, TellFilePointer, (Handle))
;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Flash data from buffer
DEC_FUN(Bool32, FlushFile, (Handle))
;
///////////////////////////////////////////////////////////////////////////////////////////////////////Memory
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Alloc memory
#define   MAF_GPTR                   0x0001
#define   MAF_GNHD                   0x0002
#define   MAF_GALL_GMEM_FIXED        0x0004
#define   MAF_GALL_GMEM_MOVEABLE     0x0008
#define   MAF_GALL_GPTR              0x0010
#define   MAF_GALL_GHND              0x0020
#define   MAF_GALL_GMEM_DDESHARE     0x0040
#define   MAF_GALL_GMEM_DISCARDABLE  0x0080
#define   MAF_GALL_GMEM_LOWER        0x0100
#define   MAF_GALL_GMEM_NOCOMPACT    0x0200
#define   MAF_GALL_GMEM_NODISCARD    0x0400
#define   MAF_GALL_GMEM_NOT_BANKED   0x0800
#define   MAF_GALL_GMEM_NOTIFY       0x1000
#define   MAF_GALL_GMEM_SHARE        0x2000
#define   MAF_GALL_GMEM_ZEROINIT     0x4000
#define   MAF_GALL_GMEM_RESERVED     0x8000
DEC_FUN(Handle, AllocMemory, (uint32_t, uint32_t))
;
DEC_FUN(Handle, DAllocMemory, (uint32_t, uint32_t, char*, char*))
;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ReAlloc memory
#define   MRF_NEW_MEMORY                  0x0000
#define   MRF_GALL_GMEM_DISCARDABLEGPTR   0x0001
#define   MRF_GALL_GMEM_MOVEABLE          0x0002
#define   MRF_GALL_GMEM_NOCOMPACT         0x0004
#define   MRF_GALL_GMEM_ZEROINIT          0x0008
//typedef   Handle (*FNReAlloc)(Handle, uint32_t, uint32_t);
DEC_FUN(Handle, ReAllocMemory, (Handle, uint32_t, uint32_t))
;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Free memory
DEC_FUN(Bool32, FreeMemory, (Handle))
;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Lock memory
DEC_FUN(Handle, LockMemory, (Handle))
;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Unlock memory
DEC_FUN(Bool32, UnlockMemory, (Handle))
;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Write from memory to disk
DEC_FUN(uint32_t, WriteMemoryToFile, (Handle, pchar))
;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Read data from disk to memory
DEC_FUN(uint32_t, ReadMemoryFromFile, (pchar, Handle *))
;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Write data from memory to storage
DEC_FUN(uint32_t, WriteMemoryToStorage, (Handle, Handle, pchar))
;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Read data from storage to memory
DEC_FUN(uint32_t, ReadMemoryFromStorage, (Handle, pchar, Handle *))
;/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#undef DEC_FUN
/////////////////////////////////////////////////////////////////////////////////////////////////////////////


#ifdef __cplusplus
}
#endif

#endif                                                                                           //__CFIO_H__
