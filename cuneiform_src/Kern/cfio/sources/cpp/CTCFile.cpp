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

//////////////////////////////////////////////////////////////////////////////////
//                                                                              //
//                                                                              //
//                    Cognitive Technologies Ltd.                               //
//                                                                              //
//                    Data Storage Container                                    //
//                                                                              //
//                                                                              //
//                                                                              //
//                                                                              //
//                    started at 25 may 1998                                    //
//                                                                              //
//////////////////////////////////////////////////////////////////////////////////
#include "CTCClasses.h"
//////////////////////////////////////////////////////////////////////////////////
//
CTCFileHeader::CTCFileHeader():CTCGlobalHeader()
{
	pFile = NULL;
}
//////////////////////////////////////////////////////////////////////////////////
//
CTCFileHeader::~CTCFileHeader()
{
	if ( GetFile() )
	{
		delete GetFile();
	}
}
//////////////////////////////////////////////////////////////////////////////////
//
CTCFileHeader::CTCFileHeader(CTCGlobalFile * pNewFile, 
							 Word32 Flag, 
							 Handle Storage):CTCGlobalHeader(NULL,NULL,0)
{
	pFile = pNewFile;
	SetHandle(AcceptFile(pNewFile));
	SetFlag(Flag);
	SetHeaderSize( sizeof( class CTCFileHeader ) );
	AttachToStorage(Storage);
	BreakName();
}
//////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTCFileHeader::DetachFromStorage()
{
	return ((hStorage = NULL) == NULL);
}
//////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTCFileHeader::AttachToStorage(Handle Storage)
{
	if ( TRUE /*!IsFlag(CFIO_FILE_LOCKED)*/ )
	{
		hStorage = Storage;
		return TRUE;
	}
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTCFileHeader::LockToStorage(void)
{
	if ( CanWrite() && GetAttaching() )
	{
		return AddFlag(CFIO_FILE_LOCKED);
	}
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTCFileHeader::UnlockFromStorage(void)
{
	if ( !CanWrite() && GetAttaching() )
	{
		return RemoveFlag(CFIO_FILE_LOCKED);
	}
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////////////
//
CTCFileList::CTCFileList()
{
	mfFirstItem.SetNext(&mfLastItem);
	mfFirstItem.SetSize(0);
	mfLastItem.SetSize(0); 
	mfFirstItem.SetHandle(FICTIV_Handle);
	mfLastItem.SetHandle(FICTIV_Handle);

	pList = NULL; 
	wFileCounter = 0; 
	wSpaceCounter = 0;
}
//////////////////////////////////////////////////////////////////////////////////
//
CTCFileList::~CTCFileList()
{

}
//////////////////////////////////////////////////////////////////////////////////
//
Handle CTCFileList::AddItem(CTCGlobalFile * pNewFile, Word32 wNewFlag, Handle Storage)
{
	CTCFileHeader * Current, * NewBlock = NULL;
	Handle          NewHandle = pNewFile->GetFileHandle();

	if( !NewHandle )
		return NULL;
	
	for ( Current = pFirst(); Current->GetNext() != pLast(); Current = Current->GetNext() ) 
		if ( Current->GetHandle() == pNewFile->GetFileHandle() )
		{
			return NULL;
		}
	
	NewBlock = new CTCFileHeader(pNewFile, wNewFlag, Storage);
	NewBlock->SetNext(Current->GetNext());
	Current->SetNext(NewBlock); 
	IncreaseFileCounter();

	return NewHandle;
}
//////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTCFileList::DeleteItem(Handle File, Word32 Flag)
{
	CTCFileHeader * Current, * Last, * EraseBlock;
	Word32 IsOK    = 0;

	for ( Last = Current = pFirst(); Current != pLast(); Current = Current->GetNext() )
	{
		if ( Current->GetHandle() == File )
		{
			EraseBlock = Current;
			DecreaseFileCounter();
			Last->SetNext(Current->GetNext());
			delete EraseBlock;
			IsOK++;
			Current = Last;
		}
		else
		{
			Last = Current;
		}
	}
		return ( IsOK == 1 );
}
//////////////////////////////////////////////////////////////////////////////////
//
CTCGlobalFile * CTCFileList::GetItem(Handle File)
{
	CTCFileHeader * pCurrent = GetItemHeader(File);
	CTCGlobalFile * pFounded = NULL;
	
	if ( pCurrent )
	{
		pFounded = pCurrent->GetFile();
	}

	return pFounded;
}
//////////////////////////////////////////////////////////////////////////////////
//
CTCFileHeader * CTCFileList::GetItemHeader(Handle File)
{
	CTCFileHeader * pCurrent;

	for ( pCurrent = pFirst(); pCurrent != pLast(); pCurrent = pCurrent->GetNext())
	{
		if ( pCurrent->GetHandle() == File  )
		{
			return pCurrent;
		}
	}

	return NULL;
}
//////////////////////////////////////////////////////////////////////////////////
//
Handle CTCFileList::GetAttachedFileHeader(Handle Storage, CTCFileHeader * File)
{
	CTCFileHeader * pCurrent, * pStart;

	if ( File == NULL )
	{
		pStart = pFirst();
	}
	else
	{
		pStart = File;
		
		if ( !pStart )
			return NULL;
	}
	
	for ( pCurrent = pStart; pCurrent != pLast(); pCurrent = pCurrent->GetNext())
	{
		if ( pCurrent->GetAttaching() == Storage  )
		{
			return pCurrent->GetHandle();
		}
	}
	return NULL;
}
//////////////////////////////////////////////////////////////////////////////////
//
Handle CTCFileList::FindFile(PChar8 lpFileName)
{
	CTCGlobalFile * pFile;
	CTCFileHeader * pCurrent;
	Word32          wComp;
	Word32          wiComp;

	for ( pCurrent = pFirst(); pCurrent != pLast(); pCurrent = pCurrent->GetNext())
	{
		pFile = pCurrent->GetFile();

		if ( pFile )
		{
			wComp = strcmp (pFile->GetFileName(), lpFileName );
			wiComp = _stricmp (pFile->GetFileName(), lpFileName );
			
			if ( wiComp == 0)
				return pCurrent->GetHandle();
		}
	}
	return NULL;
}
//////////////////////////////////////////////////////////////////////////////////
//end of file


