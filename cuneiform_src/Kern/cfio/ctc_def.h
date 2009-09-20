/*
 * ctc_def.h
 *
 *  Created on: 20.09.2009
 *      Author: uliss
 */

#ifndef CTC_DEF_H_
#define CTC_DEF_H_

#define                CFIO_ALLOC(a,b)                  GlobalAlloc(a,b)
#define                CFIO_REALLOC(a,b,c)              GlobalRealloc(a,b,c)
#define                CFIO_FREE(a)                     GlobalFree(a)
#define                CFIO_LOCK(a)                     GlobalLock(a)
#define                CFIO_UNLOCK(a)                   GlobalUnlock(a)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// base operations with files and storages as files
// storageheader and fileheader translate HandleFILE to owns Handle
//     global flags
#define                CFIO_GF_CREATE              0x00000001
#define                CFIO_GF_OPEN                0x00000002
#define                CFIO_GF_WRITE               0x00000004
#define                CFIO_GF_READ                0x00000008
#define                CFIO_GF_APPEND              0x00000010
#define                CFIO_GF_BINARY              0x00000020
#define                CFIO_GF_COMMIT              0x00000040
#define                CFIO_GF_FILE                0x00000080
#define                CFIO_GF_STORAGE             0x00000100
#define                CFIO_GF_IN_MEMORY           0x00000200
#define                CFIO_GF_TEMPORARY           0x00000400
#define                CFIO_GF_LOCKED              0x00000800
#define                CFIO_GF_SEEK_CURR           0x00001000
#define                CFIO_GF_SEEK_BEG            0x00002000
#define                CFIO_GF_SEEK_END            0x00004000
#define                CFIO_GF_MEMORY_FILE_CLUSTER 0x00008000
#define                CFIO_GF_COMPLITE            0x00010000
//     local funcs

#define                CFIO_OPEN(a,b)                   fopen(a,b)
#define                CFIO_CLOSE(a)                    fclose((FILE *)a)
#define                CFIO_WRITE(a,b,c,d)              fwrite((void *)a,b,c,(FILE *)d)
#define                CFIO_READ(a,b,c,d)               fread((void *)a,b,c,(FILE *)d)
#define                CFIO_SEEK(a,b,c)                 fseek((FILE *)a,b,c)
#define                CFIO_TELL(a)                     ftell((FILE *)a)
#define                CFIO_FLUSH(a)                    fflush((FILE *)a)
#define                CFIO_REMOVE(a)                   remove(a)
#define                CFIO_STRCAT(a,b)                 strcat(a,b)
#define                CFIO_STRCPY(a,b)                 strcpy(a,b)
#define                CFIO_STRLEN(a)                   strlen(a)
#define                CFIO_GETFOLDERSITEMS             split_path
#define                CFIO_MAKEPATH                    make_path
#define                CFIO_MAKEFULLPATH                _fullpath
#define                CFIO_MAKEFOLDER(a)               CreateDirectory(a,NULL)      //_mkdir(a) //
#define                CFIO_DELETEFOLDER                RemoveDirectory         //_rmdir          //
//                                                 CreateFolder
//     local flags
#define                CFIO_GF_CWRITE              "w"
#define                CFIO_GF_CREAD               "r"
#define                CFIO_GF_CAPPEND             "a"
#define                CFIO_GF_CWRITEREAD          "w+"
#define                CFIO_GF_CREADWRITE          "r+"
#define                CFIO_GF_CAPPENDEOF          "a+"
#define                CFIO_GF_CTEXT               "t"
#define                CFIO_GF_CBINARY             "b"
#define                CFIO_GF_CCOMMIT             "c"
#define                CFIO_GF_CNOCOMMIT           "n"


#define                CFIO_FILE_CREATE                    CFIO_GF_CREATE
#define                CFIO_FILE_OPEN                      CFIO_GF_OPEN
#define                CFIO_FILE_WRITE                     CFIO_GF_WRITE
#define                CFIO_FILE_READ                      CFIO_GF_READ
#define                CFIO_FILE_APPEND                    CFIO_GF_APPEND
#define                CFIO_FILE_BINARY                    CFIO_GF_BINARY
#define                CFIO_FILE_COMMIT                    CFIO_GF_COMMIT
#define                CFIO_FILE_EXIST                     CFIO_GF_FILE
#define                CFIO_FILE_ATTACHED                  CFIO_GF_STORAGE
#define                CFIO_FILE_IN_MEMORY                 CFIO_GF_IN_MEMORY
#define                CFIO_FILE_TEMPORARY                 CFIO_GF_TEMPORARY
#define                CFIO_FILE_LOCKED                    CFIO_GF_LOCKED
#define                CFIO_FILE_SEEK_CUR                  CFIO_GF_SEEK_CURR
#define                CFIO_FILE_SEEK_BEG                  CFIO_GF_SEEK_BEG
#define                CFIO_FILE_SEEK_END                  CFIO_GF_SEEK_END

#define    FICTIV_BLOC             0xffffffff
#define    FICTIV_Handle           (void *)FICTIV_BLOC

namespace CIF {
namespace CTC {

}
}

#endif /* CTC_DEF_H_ */
