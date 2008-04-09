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

/////////////////////////////////////////////////////////
// class XPool: memory pool with autodestruction, file io
// To be used as base for classes owned dynamic memory blocks
//
// 1997, Postnikov
/////////////////////////////////////////////////////////
#ifndef __XPOOL_H
#define __XPOOL_H

#ifndef __STD_H
#include "std.h"
#endif

#ifndef __XFILE_H
#include "xfile.h"
#endif

#ifndef __XMACROS_H
#include "xmacros.h"
#endif

#ifndef __SWABYTES_H
#include "swabytes.h"
#endif


class XPool
{
protected:
   void HUGE *    	Data;
   Int32      		Volume;

public:
   XPool(const XPool& x)
   {
      Data = NULL;
      Volume = x.GetVolume();
      if (Volume && Create(x.GetVolume()))
      {
         if (x.GetData())
            memcpy(Data, x.GetData(), x.GetVolume());         
      }
   };

   Bool CopyFrom( XPool& src )
   {
      Destroy();
      Volume = src.GetVolume();
      if (Volume && !Create(src.GetVolume()))
         return FALSE;
      if (src.GetData())
         memcpy(Data, src.GetData(), src.GetVolume());         
      return TRUE;
   }

	Bool     operator !() const {  return ((Volume > 0) != (Data != NULL)); };
   void*    GetData(void) const    { return Data; };
   Int32    GetVolume(void) const  { return Volume; };

   XPool(void) : Volume(0), Data(NULL)     	{  };
   XPool(Int32 size) :Volume(0),Data(NULL)   	{  Create(size); };
   ~XPool()                                	{  Destroy(); };

   Bool     Create(Int32 size)
      {  
         if (size != Volume || (Data==NULL))
         {
            Destroy();
            if (size > 0)
               Data=stdMalloc(size);
            Volume = size;
         }
         return ((Volume > 0) == (Data != NULL)); // !!*this
      }

   void     Destroy(void)
      {  if (Data!=NULL)
            stdFree(Data);
         Data=NULL; Volume=0;
      }

   Bool     Realloc( Int32 new_size )
      {
         void* p=stdRealloc( Data, new_size, Volume );
         if (new_size && p==NULL)   // failure
            RETFALSE;
         Data=p;
         Volume=new_size;
         return TRUE;
      };

   //XPool&   operator =  ( XPool  const & mp );  // memcpy
   //XPool&   operator += ( XPool const & mp );   // concatenation
   //operator PByte() const  {  return (PByte)Data; };
   //operator PVoid() const  {  return (PVoid)Data; };
   //PByte    operator + (Indx i) const { return ((PByte)Data)+i; };

   void*    VPtr( Int32 i=0 ) const    { return (void*)(((Word8*)Data)+i);};
   Word8*   BPtr( Int32 i=0 ) const    { return        (((Word8*)Data)+i);};
   char*    CPtr( Int32 i=0 ) const    { return (char*)(((Word8*)Data)+i);};

   Err16 Read( XFile& bf, Bool32 swap_bytes = FALSE );
   Bool  Write( XFile& bf, Int32 size=-1 ) const;

   //BINFILE_READ_FUNCS_PROTOTYPES

   //Bool     Write( RBinFile bf, Int32 size = -1 ) const;
   void     MemSet( Word8 pattern )
   {
      if (Data)
         memset(Data, pattern, Volume );
   }
/*
   void     CopyFrom(const void* src, Int32 size = -1 )
      {  assert(src);
         if (size == -1)
            size = Volume;
		assert(size>=0);
		if (((Word32)size) > Volume)
            size = Volume;
         if (size) memcpy(Data, src, (Indx)size);
      };
   void     CopyTo(void* dst, Int32 size = -1 )
      {  assert(dst);
         if (size == -1)
            size = Volume;
		assert(size >=0);
		 if (((Word32)size) > Volume)
            size = Volume;
			if (size) memcpy(dst, Data, (Indx)size);
		};
*/
};

inline Err16 XPool::Read( XFile& bf, Bool32 swap_bytes )
   {
      if (!bf)
         { assert(0); return ER_CANTREAD; };
      Int32 new_vol=0;
      if (!TagRead( bf, new_vol ))
         { assert(0); return ER_CANTREAD; };
      if (swap_bytes)
         SwapInt32(new_vol);
      if (!Create(new_vol))
         { assert(0); return ER_NOMEMORY; };
      if ((Data!=NULL)&& (!bf.Read( Data, Volume )))
         {  assert(0); return ER_CANTREAD; };
      return ER_NONE;
   };

inline Bool XPool::Write( XFile& bf, Int32 size ) const
   {
      if (size==-1) size = Volume;
      assert(size <= Volume);
      if (!bf)
         RETFALSE;
      if (!TagWrite( bf, size ))
         RETFALSE;
      if (size==0) return TRUE;
      return bf.Write(Data, size);
   };

#endif   // __XPOOL_H
