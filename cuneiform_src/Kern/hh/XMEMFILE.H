/*
Copyright (c) 1993-2008, Cognitive Technologies
All rights reserved.

Ðàçðåøàåòñÿ ïîâòîðíîå ðàñïðîñòðàíåíèå è èñïîëüçîâàíèå êàê â âèäå èñõîäíîãî êîäà,
òàê è â äâîè÷íîé ôîðìå, ñ èçìåíåíèÿìè èëè áåç, ïðè ñîáëþäåíèè ñëåäóþùèõ óñëîâèé:

      * Ïðè ïîâòîðíîì ðàñïðîñòðàíåíèè èñõîäíîãî êîäà äîëæíû îñòàâàòüñÿ óêàçàííîå
        âûøå óâåäîìëåíèå îá àâòîðñêîì ïðàâå, ýòîò ñïèñîê óñëîâèé è ïîñëåäóþùèé 
        îòêàç îò ãàðàíòèé. 
      * Ïðè ïîâòîðíîì ðàñïðîñòðàíåíèè äâîè÷íîãî êîäà â äîêóìåíòàöèè è/èëè â 
        äðóãèõ ìàòåðèàëàõ, ïîñòàâëÿåìûõ ïðè ðàñïðîñòðàíåíèè, äîëæíû ñîõðàíÿòüñÿ
        óêàçàííàÿ âûøå èíôîðìàöèÿ îá àâòîðñêîì ïðàâå, ýòîò ñïèñîê óñëîâèé è
        ïîñëåäóþùèé îòêàç îò ãàðàíòèé.  
      * Íè íàçâàíèå Cognitive Technologies, íè èìåíà åå ñîòðóäíèêîâ íå ìîãóò 
        áûòü èñïîëüçîâàíû â êà÷åñòâå ñðåäñòâà ïîääåðæêè è/èëè ïðîäâèæåíèÿ 
        ïðîäóêòîâ, îñíîâàííûõ íà ýòîì ÏÎ, áåç ïðåäâàðèòåëüíîãî ïèñüìåííîãî
        ðàçðåøåíèÿ. 

ÝÒÀ ÏÐÎÃÐÀÌÌÀ ÏÐÅÄÎÑÒÀÂËÅÍÀ ÂËÀÄÅËÜÖÀÌÈ ÀÂÒÎÐÑÊÈÕ ÏÐÀÂ È/ÈËÈ ÄÐÓÃÈÌÈ ËÈÖÀÌÈ "ÊÀÊ
ÎÍÀ ÅÑÒÜ" ÁÅÇ ÊÀÊÎÃÎ-ËÈÁÎ ÂÈÄÀ ÃÀÐÀÍÒÈÉ, ÂÛÐÀÆÅÍÍÛÕ ßÂÍÎ ÈËÈ ÏÎÄÐÀÇÓÌÅÂÀÅÌÛÕ, 
ÂÊËÞ×Àß ÃÀÐÀÍÒÈÈ ÊÎÌÌÅÐ×ÅÑÊÎÉ ÖÅÍÍÎÑÒÈ È ÏÐÈÃÎÄÍÎÑÒÈ ÄËß ÊÎÍÊÐÅÒÍÎÉ ÖÅËÈ, ÍÎ ÍÅ
ÎÃÐÀÍÈ×ÈÂÀßÑÜ ÈÌÈ. ÍÈ ÂËÀÄÅËÅÖ ÀÂÒÎÐÑÊÈÕ ÏÐÀÂ È ÍÈ ÎÄÍÎ ÄÐÓÃÎÅ ËÈÖÎ, ÊÎÒÎÐÎÅ 
ÌÎÆÅÒ ÈÇÌÅÍßÒÜ È/ÈËÈ ÏÎÂÒÎÐÍÎ ÐÀÑÏÐÎÑÒÐÀÍßÒÜ ÏÐÎÃÐÀÌÌÓ, ÍÈ Â ÊÎÅÌ ÑËÓ×ÀÅ ÍÅ 
ÍÅÑ¨Ò ÎÒÂÅÒÑÒÂÅÍÍÎÑÒÈ, ÂÊËÞ×Àß ËÞÁÛÅ ÎÁÙÈÅ, ÑËÓ×ÀÉÍÛÅ, ÑÏÅÖÈÀËÜÍÛÅ ÈËÈ 
ÏÎÑËÅÄÎÂÀÂØÈÅ ÓÁÛÒÊÈ, ÑÂßÇÀÍÍÛÅ Ñ ÈÑÏÎËÜÇÎÂÀÍÈÅÌ ÈËÈ ÏÎÍÅÑÅÍÍÛÅ ÂÑËÅÄÑÒÂÈÅ 
ÍÅÂÎÇÌÎÆÍÎÑÒÈ ÈÑÏÎËÜÇÎÂÀÍÈß ÏÐÎÃÐÀÌÌÛ (ÂÊËÞ×Àß ÏÎÒÅÐÈ ÄÀÍÍÛÕ, ÈËÈ ÄÀÍÍÛÅ, 
ÑÒÀÂØÈÅ ÍÅÃÎÄÍÛÌÈ, ÈËÈ ÓÁÛÒÊÈ È/ÈËÈ ÏÎÒÅÐÈ ÄÎÕÎÄÎÂ, ÏÎÍÅÑÅÍÍÛÅ ÈÇ-ÇÀ ÄÅÉÑÒÂÈÉ 
ÒÐÅÒÜÈÕ ËÈÖ È/ÈËÈ ÎÒÊÀÇÀ ÏÐÎÃÐÀÌÌÛ ÐÀÁÎÒÀÒÜ ÑÎÂÌÅÑÒÍÎ Ñ ÄÐÓÃÈÌÈ ÏÐÎÃÐÀÌÌÀÌÈ, 
ÍÎ ÍÅ ÎÃÐÀÍÈ×ÈÂÀßÑÜ ÝÒÈÌÈ ÑËÓ×ÀßÌÈ), ÍÎ ÍÅ ÎÃÐÀÍÈ×ÈÂÀßÑÜ ÈÌÈ, ÄÀÆÅ ÅÑËÈ ÒÀÊÎÉ 
ÂËÀÄÅËÅÖ ÈËÈ ÄÐÓÃÎÅ ËÈÖÎ ÁÛËÈ ÈÇÂÅÙÅÍÛ Î ÂÎÇÌÎÆÍÎÑÒÈ ÒÀÊÈÕ ÓÁÛÒÊÎÂ È ÏÎÒÅÐÜ.

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

#pragma message(__FILE__"(1)  :")
/////////////////////////////////////////////////////////////////
// class XMemFile - utility to put/get misc data into binary pool,
// read/write it to disk
//
// 28.05.97 22:32, Postnikov
/////////////////////////////////////////////////////////////////
#ifndef __XMEMFILE_H
   #define __XMEMFILE_H

   #ifndef __XPOOL_H
   #  include "xpool.h"
   #endif

   #ifndef __SWABYTES_H
   #  include "swabytes.h"
   #endif

class XMemFile : public XPool
{
   Int32    nFileLength;  // count of bytes in use (put into)
   Word8*   pCur;        // current ptr

public:
   XMemFile( Int32 init_size = 0 )
      : XPool(init_size)
      { pCur=(Word8*)Data; nFileLength = 0; };

   Int32    Tell()
      { return pCur - (Word8*)Data; };

   Int32    FileLength()
      { return nFileLength; };

   void    RestoreFileLength(Int32 len) // Oleg : knot for second recongition
      { nFileLength=len; }; 

   Bool     Seek( Int32 nOffset )
      {  if ((nOffset > nFileLength)||(nOffset < 0))
            RET_FALSE;
         pCur= ((Word8*)Data)+nOffset;
         return TRUE;
      };

   void     SeekToStart( void ) {  pCur= ((Word8*)Data); };
   void     SeekToFinish( void ) {  pCur= ((Word8*)Data)+nFileLength; };

   void     Reset() { nFileLength = 0; SeekToStart(); };

   Bool     Get( void* data, Int32 size )
      {
         if (size < 0 || size+Tell() > nFileLength)
            RET_FALSE;
         if (size==0)
            return TRUE;
         stdMemcpy(data, pCur, size);
         pCur+=size;
         return TRUE;
      };

   Bool     Writeln( const char* str ) 
      { 
         return Put(str, strlen(str)+1); 
      };
   char*    Readln(void) // be sure that there is zt string!
      {  
	 int nRest = nFileLength - Tell();
         if (nRest <=0)
            return NULL;
	 char* pNext = (char*)memchr(pCur, 0, nRest);
	 if (pNext==NULL)
            return NULL;
         char* ret = (char*)pCur;
         pCur = (Word8*)pNext; pCur++;
         return ret;
      };
   
   Bool     Reserve( Int32 size )
      {
         if (size < 0)
            RET_FALSE; // too strange...
         Int32 tell = Tell();
         if (size+tell <= Volume)
            return TRUE; // there is enough place

         Int32 new_size = maxi(size+tell, Volume*2);
         if (!Realloc( new_size ))
         {  
            if (   size+tell < Volume*2  && 
                  !Realloc(size+tell ) 
               )
            RET_FALSE;  // can't realloc
         }
         Seek(tell);
         return TRUE;
      }

   Bool     Put( const void* data, Int32 size )
      {
         if (size==0)
            return TRUE;
         if (!Reserve(size)) // prepeare place to put
            RET_FALSE;
         // put
         stdMemcpy(pCur, (void*)data, size);
         pCur+=size;
         nFileLength += size;
         return TRUE;
      };

   // TODO: optimize!
   Bool     Get( Int32& t ) { return Get( &t, sizeof(Int32) ); }
   Bool     Put( Int32& t ) { return Put( &t, sizeof(Int32) ); }
   Bool     Get( Int16& t ) { return Get( &t, sizeof(Int16) ); }
   Bool     Put( Int16& t ) { return Put( &t, sizeof(Int16) ); }
   Bool     Get( Int8& t ) { return Get( &t, sizeof(Int8) ); }
   Bool     Put( Int8& t ) { return Put( &t, sizeof(Int8) ); }
   Bool     Get( Word32& t ) { return Get( &t, sizeof(Word32) ); }
   Bool     Put( Word32& t ) { return Put( &t, sizeof(Word32) ); }
   Bool     Get( Word16& t ) { return Get( &t, sizeof(Word16) ); }
   Bool     Put( Word16& t ) { return Put( &t, sizeof(Word16) ); }
   Bool     Get( Word8& t ) { return Get( &t, sizeof(Word8) ); }
   Bool     Put( Word8& t ) { return Put( &t, sizeof(Word8) ); }
   Bool     Get( Rect16& t ) { return Get( &t, sizeof(Rect16) ); }
   Bool     Put( Rect16& t ) { return Put( &t, sizeof(Rect16) ); }
   Bool     Get( Point16& t ) { return Get( &t, sizeof(Point16) ); }
   Bool     Put( Point16& t ) { return Put( &t, sizeof(Point16) ); }
   Bool     Get( Rect32& t ) { return Get( &t, sizeof(Rect32) ); }
   Bool     Put( Rect32& t ) { return Put( &t, sizeof(Rect32) ); }
   Bool     Get( Point32& t ) { return Get( &t, sizeof(Point32) ); }
   Bool     Put( Point32& t ) { return Put( &t, sizeof(Point32) ); }
   // end optimize
/*
   template <class T>
   Bool     Get( T& t ) { return Get( &t, sizeof(T) ); }

   template <class T>
   Bool     Put( T& t ) { return Put( &t, sizeof(T) ); }
*/
   Bool     GetArray( void* p_start, Int32& nCount, Int32 cbElemSize )
      {  if (!Get( nCount ))
            RET_FALSE;
         if (!Get( p_start, nCount*cbElemSize ))
            RET_FALSE;
         return TRUE;
      };

   Bool     PutArray( void* p_start, Int32 nCount, Int32 cbElemSize )
      {  if (!Put( nCount ))
            RET_FALSE;
         if (!Put( p_start, nCount*cbElemSize ))
            RET_FALSE;
         return TRUE;
      };

   void     Destroy() { XPool::Destroy(); pCur = NULL; nFileLength = 0; };
   /////////////////////////////////////////////////////
   // Disk IO
   Err16    Read( XFile & xf, Bool32 swap_bytes = FALSE )
      {
         RETIFERR( XPool::Read(xf, swap_bytes) );
         nFileLength = XPool::GetVolume();
         SeekToStart();
         return ER_NONE;
      };

   Bool     Write( XFile & xf ) const
      {
         if(!XPool::Write(xf, nFileLength))   RET_FALSE;
         return TRUE;
      }

   /////////////////////////////////////////////////////
   // Exchange
   Bool CopyFrom( XMemFile& src )
   {
      Destroy();
      if (!XPool::CopyFrom( *(XPool*)&src ))
         return FALSE;

      nFileLength = src.FileLength();
      Seek( src.Tell() );
      return TRUE;
   }
};

#endif // __XMEMFILE_H

