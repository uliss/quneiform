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

#include "resource.h"
#include "mymem.h"
#include "cpage.h"
#include "backup.h"
#include "tableclass.h"

//###########################################
CPAGE_FUNC(Handle)  CPAGE_ExTableCreate (Handle hPage,Int32 Skew2048, Word32 nVer,Int32 * lpVCor, Word32 nHor,Int32 * lpHCor)
{
	PROLOG;
  SetReturnCode_cpage(IDS_ERR_NO);
	Handle rc = NULL;
	Bool32 res = FALSE;

	TableClass tc;

	if(tc.Create(Skew2048,nVer,lpVCor,nHor,lpHCor))
		rc = tc.Store(hPage);

	rc = rc ? TableClass::Attach(hPage,rc):NULL;
	EPILOG;
	return rc;
}
//###########################################
CPAGE_FUNC(void)  CPAGE_ExTableDelete (Handle hTable)
{
	PROLOG;
  SetReturnCode_cpage(IDS_ERR_NO);
	TableClass * tc = (TableClass *)hTable;
	if(tc)
		tc->Remove();
	EPILOG;
}
//###########################################
CPAGE_FUNC(Handle)  CPAGE_ExTableGetFirst		(Handle hPage)
{
	PROLOG;
	Handle rc = NULL;
	Handle hBlock = NULL;
  SetReturnCode_cpage(IDS_ERR_NO);
	Handle Type = CPAGE_GetInternalType("TableClass");
	if(!CPAGE_UpdateBlocks( hPage, Type ))
	{
		rc = NULL;
		goto lOut;
	}
	hBlock = CPAGE_GetBlockFirst(hPage, Type);

	rc = hBlock ? TableClass::Attach(hPage,hBlock):NULL;
lOut:EPILOG;
	return rc;
}
//###########################################
CPAGE_FUNC(Handle)  CPAGE_ExTableGetNext(Handle hTable)
{
	PROLOG;
  SetReturnCode_cpage(IDS_ERR_NO);
	TableClass * tc = (TableClass *)hTable;
	if(tc)
		tc = tc->GetNext();
	EPILOG;
return tc;
}
//###########################################
CPAGE_FUNC(Bool32)  CPAGE_ExGeTableGetNumberCells	(Handle hTable,Int32 * lpNumber)
{
	PROLOG;
  Bool32 rc = FALSE;
  SetReturnCode_cpage(IDS_ERR_NO);
	TableClass * tc = (TableClass *)hTable;
	if(tc)
	{
		*lpNumber  = tc->GetNumberGeCell();
		rc = TRUE;
	}
	EPILOG;
  return rc;
}
//###########################################
CPAGE_FUNC(Bool32)  CPAGE_ExPhTableGetNumberCells	(Handle hTable,Int32 * lpNumber)
{
	PROLOG;
	Bool32 rc = FALSE;
  SetReturnCode_cpage(IDS_ERR_NO);
	TableClass * tc = (TableClass *)hTable;

  if(tc)
  {
	*lpNumber  = tc->GetNumberPhCell();
	rc = TRUE;
  }
  EPILOG;
  return rc;
}
//###########################################
CPAGE_FUNC(Bool32)  CPAGE_ExTableGetNumberRow		(Handle hTable,Int32 * lpNumber)
{
	PROLOG;
	Bool32 rc = FALSE;
  SetReturnCode_cpage(IDS_ERR_NO);
	TableClass * tc = (TableClass *)hTable;
  if(tc)
  {
	*lpNumber  = tc->GetNumberRow();
	rc = TRUE;
  }
  EPILOG;
  return rc;
}
//###########################################
CPAGE_FUNC(Bool32)  CPAGE_ExTableGetNumberColumn		(Handle hTable,Int32 * lpNumber)
{
	PROLOG;
	Bool32 rc = FALSE;
  SetReturnCode_cpage(IDS_ERR_NO);
	TableClass * tc = (TableClass *)hTable;
  if(tc)
  {
	*lpNumber  = tc->GetNumberColumn();
	rc = TRUE;
  }
  EPILOG;
  return rc;
}
//###########################################
CPAGE_FUNC(Bool32)  CPAGE_ExGeTableGetSizeCell		(Handle hTable,Point32 point,Rect32 * lpRect)
{
	PROLOG;
	Bool32 rc = FALSE;
  SetReturnCode_cpage(IDS_ERR_NO);
	Rect32      rect = {0};

	_ASSERT(lpRect);
	TableClass * tc = (TableClass *)hTable;
  if(tc)
  {
	  rect.left		= tc->GetVLine(point.x);
	  rect.right	= tc->GetVLine(point.x + 1);
	  rect.top		= tc->GetVLine(point.y);
	  rect.bottom	= tc->GetVLine(point.y + 1);
	  *lpRect		= rect;
	rc = TRUE;
  }
  EPILOG;
  return rc;
}
//###########################################
CPAGE_FUNC(Bool32)  CPAGE_ExPhTableGetSizeCell		(Handle hTable,Point32 point,Rect32 * lpRect)
{
	PROLOG;
	Bool32 rc = FALSE;
  SetReturnCode_cpage(IDS_ERR_NO);
  TableClass * tc = (TableClass *)hTable;
  if(tc)
  {
  // ÍÀÄÎ ÑÄÅËÀÒÜ !!!!
  }
  EPILOG;
  return rc;
}
//###########################################
CPAGE_FUNC(Bool32)  CPAGE_ExTableGetNumberBlock	(Handle hTable,Point32 point,Int32 * lpNumber)
{
	PROLOG;
	Bool32 rc = FALSE;
  SetReturnCode_cpage(IDS_ERR_NO);

  _ASSERT(lpNumber);

  TableClass * tc = (TableClass *)hTable;
  if(tc)
  {
	  Point32 phPoint = tc->GetCell(point);
	  *lpNumber  = tc->GetCell(phPoint);

	rc = TRUE;
  }
  EPILOG;
  return rc;
}
//###########################################

CPAGE_FUNC(Bool32)  CPAGE_ExGeTableGetPhysical		(Handle hTable,Point32 point,Point32 * lpPoint)
{
	PROLOG;
	Bool32 rc = FALSE;
  SetReturnCode_cpage(IDS_ERR_NO);

  _ASSERT(lpPoint);

  TableClass * tc = (TableClass *)hTable;
  if(tc)
  {
	*lpPoint  = tc->GetCell(point);
	rc = TRUE;
  }
  EPILOG;
  return rc;
}
//###########################################
CPAGE_FUNC(Bool32)  CPAGE_ExPhTableGetNumberGeometry	(Handle hTable,Point32 point,Int32 * lpNumber)
{
	PROLOG;
	Bool32 rc = FALSE;
  SetReturnCode_cpage(IDS_ERR_NO);

  _ASSERT(lpNumber);

  TableClass * tc = (TableClass *)hTable;
  if(tc)
  {

	  Point32 PhCoord = tc->GetCell(point);
	  *lpNumber  = tc->GetCell(PhCoord).GetGeCount();

	rc = TRUE;
  }
  EPILOG;
  return rc;
}
//###########################################
CPAGE_FUNC(Bool32)  CPAGE_ExPhTableGetGeometry		(Handle hTable,Point32 point,Int32 count,Point32 * lpPoint)
{
	PROLOG;
	Bool32 rc = FALSE;
  SetReturnCode_cpage(IDS_ERR_NO);

  _ASSERT(lpPoint);

  TableClass * tc = (TableClass *)hTable;
  if(tc)
  {
	  // ÍÀÄÎ ÑÄÅËÀÒÜ !
  }
  EPILOG;
  return rc;
}
//###########################################
CPAGE_FUNC(Bool32)  CPAGE_ExPhTableSetNumberBlock	(Handle hTable,Point32 point,Int32 number)
{
	PROLOG;
	Bool32 rc = FALSE;
  SetReturnCode_cpage(IDS_ERR_NO);

  TableClass * tc = (TableClass *)hTable;
  if(tc)
  {
	  Point32 PhCoord = tc->GetCell(point);
	  tc->GetCell(PhCoord)= number ;
  }
  EPILOG;
  return rc;
}
//###########################################
CPAGE_FUNC(Bool32)  CPAGE_ExTableIsPhysicCell	(Handle hTable,Point32 point,Bool32 * lpIsPhysic)
{
	PROLOG;
	Bool32 rc = FALSE;
  SetReturnCode_cpage(IDS_ERR_NO);

	_ASSERT(lpIsPhysic);

  TableClass * tc = (TableClass *)hTable;
  if(tc)
  {
	  *lpIsPhysic = tc->GetCell(point).IsPhysic();
	rc = TRUE;
  }
  EPILOG;
  return rc;
}
//###########################################
CPAGE_FUNC(Bool32)  CPAGE_ExTableSize(Handle hTable,Rect32 * lpRect)
{
	PROLOG;
	Bool32 rc = FALSE;
  SetReturnCode_cpage(IDS_ERR_NO);

	_ASSERT(lpRect);

  TableClass * tc = (TableClass *)hTable;
  if(tc)
  {

	lpRect->left = tc->GetVLine(0);
	lpRect->right= tc->GetVLine(tc->GetNumberColumn());
	lpRect->top  = tc->GetHLine(0);
	lpRect->bottom= tc->GetHLine(tc->GetNumberRow());

	rc = TRUE;
  }
  EPILOG;
  return rc;
}
//###########################################
CPAGE_FUNC(Bool32)  CPAGE_ExTableGetSkew(Handle hTable,Int32 * lpNumerator,Int32 * lpDenominator)
{
	PROLOG;
	Bool32 rc = FALSE;
  SetReturnCode_cpage(IDS_ERR_NO);

	_ASSERT(lpNumerator);
	_ASSERT(lpDenominator);

  TableClass * tc = (TableClass *)hTable;
  if(tc)
  {
	*lpNumerator   = tc->GetSkew2048();
	*lpDenominator = 2048;
	rc = TRUE;
  }
  EPILOG;
  return rc;
}
