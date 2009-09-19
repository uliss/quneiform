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

#include <string.h>
#include <ctype.h>
#include "stdafx.h"

#include "ced_struct.h"
#include "cedint.h"
#include "resource.h"
#include "compat_defs.h"

#define MAX_LEN 500
#define MAX_RTF_COLORS     200
#define TextDefBkColor	RGB(255,255,255)

#define far

Bool32	CEDPage::FormattedWriteRtf(const char * fileName, Bool merge)
{
	Bool ret;
    struct StrRtfOut far *rtf=NULL;
    struct StrRtfColor far *color=NULL;
//    long WriteBegRow,WriteEndRow;
//    LPBYTE ptr,fmt;
    int  j;
    CEDSection* sect;

//	Bool inFrm=FALSE;

//	Bool result;
//    uint AllocType;
//    OFSTRUCT OpenBuf;

//       WriteBegRow=0;
//       WriteBegCol=0;
//       WriteEndRow=TotalLines-1;
//       WriteEndCol=LineLen(WriteEndRow);     // last uchar not included

    // allocate space for the Strrtf info structure
    if (NULL==(rtf=new StrRtfOut))
	{
		SetReturnCode_ced(IDS_ERR_NO_MEMORY);
//       PrintError(w,MSG_OUT_OF_MEM,"RtfWrite");
		return FALSE;
    }

    memset(rtf,0,sizeof(struct StrRtfOut));  // initialize with zeros

	//read old rtf
	HANDLE oldRtfHndl;
	if (merge)
	{
		rtf->oldFileLen=MemFromFile((pchar)fileName,&oldRtfHndl);
		if (rtf->oldFileLen==0)
		{
			SetReturnCode_ced(CFIO_GetReturnCode());
			return 0;
		}
		rtf->oldFile=(char*)Lock(oldRtfHndl);
		if ( !rtf->oldFile )
		{
			SetReturnCode_ced(CFIO_GetReturnCode());
			Unlock(oldRtfHndl);
			Free(oldRtfHndl);
			return 0;
		}
		rtf->hFile=0;
	}
	else
	{
		//open new rtf
		rtf->hFile=Open(0, (pchar)fileName,OSF_CREATE|OSF_BINARY);

		if (!rtf->hFile)
		{
			SetReturnCode_ced(CFIO_GetReturnCode());
			return FALSE;
		}
    }
	// initialize global variables
    rtf->RtfInHdrFtr=0;
    rtf->RtfInTable=FALSE;
	rtf->page=this;

	rtf->PrevChar.fontHeight=24;
	rtf->PrevChar.fontNum=-1;
    rtf->PrevChar.foregroundColor=-1;
    rtf->PrevChar.backgroundColor=-1;

	rtf->table=new int[rtf->page->fontsUsed];
	memset(rtf->table,-1,sizeof(int)*rtf->page->fontsUsed);

	//    rtf->output=output;                         // output type
    /*if (rtf->output==RTF_FILE)*/
/*    else {
       rtf->BufLen=0;                // calculate estimated text size
       for (l=WriteBegRow;l<=WriteEndRow;l++) rtf->BufLen=rtf->BufLen+LineLen(l)+2;
       rtf->BufLen=(rtf->BufLen*3/2);// add estimated space for the formatting info
       if (rtf->BufLen<=0) rtf->BufLen=1;

       if (output==RTF_BUF) AllocType=GMEM_MOVEABLE;
       else                 AllocType=GMEM_MOVEABLE|GMEM_DDESHARE;   // clipboard data needs to be shareable
       if ( NULL==(rtf->hBuf=GlobalAlloc(AllocType,rtf->BufLen))
         || NULL==(rtf->buf=(uchar *)GlobalLock(rtf->hBuf)) ){
          PrintError(w,MSG_OUT_OF_MEM,"RtfWrite(buf)");
          return FALSE;
       }
       rtf->BufIndex=0;              // next character position in the buffer
    }
*/
    // allocate color table
    if (NULL==(color=rtf->color=new StrRtfColor[MAX_RTF_COLORS]))
	{
       return 0;//PrintError(w,MSG_OUT_OF_MEM,"RtfRead(color)");
    }
    memset(color,0,sizeof(struct StrRtfColor)*MAX_RTF_COLORS);  // initialize with zeros


	if (!merge)
	{
    // write the rtf header
    if (!BeginRtfGroup(rtf)) goto WRITE_END;
    if (!WriteRtfControl(rtf,"rtf",PARAM_INT,1)) goto WRITE_END;
    if (!WriteRtfControl(rtf,"ansi",PARAM_NONE,0)) goto WRITE_END;
    if (!WriteRtfControl(rtf,"deff",PARAM_INT,0)) goto WRITE_END;
    if (!WriteRtfControl(rtf,"deflang",PARAM_INT,1024)) goto WRITE_END;
    //fill in font table with default values
	for (int i=0;i<rtf->page->fontsUsed;i++)
		rtf->table[i]=rtf->page->fontTable[i].fontNumber;
	rtf->maxFntNum=-1;
	if (!WriteRtfFont(rtf,TRUE)) goto WRITE_END; //write the font table
    if (!WriteRtfColor(rtf)) goto WRITE_END; //write the color table
//    if (!WriteRtfStylesheet(w,rtf)) goto WRITE_END; //write the color table
    if (!WriteRtfMargin(rtf)) goto WRITE_END; //write default margin and paper information
	}
	else
	{
		if (!WriteRtfMergedHeader(rtf,fileName))
		{
			if (rtf->hFile) goto WRITE_END; //write default margin and paper information

			if (rtf->table)   delete[] rtf->table;          // free rtf control area
		    if (rtf)   delete rtf;          // free rtf control area
			return FALSE;
		}
	}
    sect=GetSection(0);
	rtf->PrevPfmt=DEFAULT_PFMT;
/*    if (cid(WriteBegRow)>0) {
       PrevPfmt=DEFAULT_PFMT;    // force writing of cell data
       PrevCell=PrevFID=0;
       if (!CellCharIncluded(w,WriteBegRow,WriteBegCol,WriteEndRow,WriteEndCol))
          rtf->flags|=ROFLAG_IGNORE_TABLE;
    }
    else if (ParaCharIncluded(w,WriteBegRow,WriteBegCol,WriteEndRow,WriteEndCol)) {
       PrevPfmt=DEFAULT_PFMT;    // force writing of frame data
       PrevCell=PrevFID=0;
    }
*/
    // write the text lines
	for (;sect>0;sect=sect->next)
	{
		if (!WriteRtfSection(rtf,sect)) goto WRITE_END; //write section properties
		//	int sectNum=0;
		for (int colNum=0;colNum<sect->numberOfColumns;colNum++)
		{
			CEDParagraph* col=sect->GetColumn(colNum);

			Bool firstParaGraph=TRUE;//process first element in a row - for correct paragraph convertation
			CEDParagraph* para=col->GetFirstObject();
			while (para)
			{
				if (para->type&FICTIVE)
				{
					if (para->type==TAB_BEGIN)
					{
						CEDParagraph * prevRow=0;
						for (int tblRowNum=0;tblRowNum<((EDTABDESCR*)(para)->descriptor)->numOfRows;tblRowNum++)
						{
							CEDParagraph * tblRow=para->GetRow(tblRowNum);
							WriteRtfRow(rtf,tblRow, prevRow);
							prevRow=tblRow;
							for (int tblCellNum=0;tblCellNum<((EDROWDESCR*)(tblRow)->descriptor)->numOfCells;tblCellNum++)
							{
								CEDParagraph* hPara=tblRow->GetCell(tblCellNum);
								CEDParagraph* para1=hPara->GetFirstObject();
								rtf->reset=TRUE;
								if (!WriteRtfControl(rtf,"pard",PARAM_NONE,0)) goto WRITE_END;
								while (para1)
								{
									if(!WriteRtfPara(rtf,para1,para1->GetNextObject()?TRUE:FALSE)) goto WRITE_END;
									para1=para1->GetNextObject();
								}
								if(!WriteRtfControl(rtf,"cell",PARAM_NONE,0)) goto WRITE_END;
							}
							if (!WriteRtfControl(rtf,"row",PARAM_NONE,0)) goto WRITE_END; // write para break
						}
						rtf->RtfInTable=FALSE;
						para=para->GetNextObject();
						rtf->reset=TRUE;
						if (!WriteRtfControl(rtf,"pard",PARAM_NONE,0)) goto WRITE_END;
					}
					else
						if (para->type==FRAME_BEGIN)
						{
							rtf->RtfInFrame=TRUE;
							if (!WriteRtfControl(rtf,"pard",PARAM_NONE,0)) goto WRITE_END;
							rtf->reset=TRUE;
							rtf->frm=para;
							para=rtf->frm->GetFirstObject();
							//if there is only one picture in a frame --- dont write frame's width and height
							rtf->wrtFrmSz=TRUE;
							//if there is only one paragraph in a frame and this paragraph is real.
							if (para&&(!para->GetNextObject())&&(!(para->type&FICTIVE)))
							{
								CEDLine * l=para->lines;
								//if there is only one line in a paragraph
								if (l&&((!l->next)||
									l->next->parentNumber!=l->parentNumber))
								{
									CEDChar * c=l->chars;
									//the same for symbol
									if (c&&((!c->next)||
										c->next->parentNumber!=c->parentNumber)
										&&CED_IsPicture(c))
										rtf->wrtFrmSz=FALSE;
								}
							}
							//if there is only one picture in a frame --- dont write frame's width and height
							//if there is only one paragraph in a frame and this paragraph is a table
							if (para&&(!para->GetNextObject())&&(para->type==TAB_BEGIN))
								rtf->wrtFrmSz=FALSE;
						}
				}
				else
				{
					int shading=para->shading;
					//if we found painted paragraph, which is first one in a non-first line
					//then we fill in fake paragraph in order not to paint column break in word
					if (colNum!=0&&firstParaGraph&&rtf->PrevPfmt&&shading!=rtf->PrevPfmt->shading)
					{
						if (!WriteRtfControl(rtf,"pard",PARAM_NONE,0)) goto WRITE_END;
						if (!WriteRtfControl(rtf,"fs",PARAM_INT,2)) goto WRITE_END;
						rtf->PrevChar.fontHeight=2;
						if (!WriteRtfControl(rtf,"par",PARAM_NONE,0)) goto WRITE_END;
						rtf->reset=TRUE;
					}
					if(!WriteRtfPara(rtf,para, TRUE)) goto WRITE_END;
					para=para->GetNextObject();
				}
				if (!para&&rtf->RtfInFrame )
				{
					para=rtf->frm->GetNextObject();
					rtf->RtfInFrame =FALSE;
					rtf->reset=TRUE;
					if (!WriteRtfControl(rtf,"pard",PARAM_NONE,0)) goto WRITE_END;
				}
				firstParaGraph=FALSE;
			}
			if (colNum!=sect->numberOfColumns-1)
				if (!WriteRtfControl(rtf,"column",PARAM_NONE,0)) goto WRITE_END;
		}
       // write section break if any
		if (!WriteRtfControl(rtf,"pard",PARAM_NONE,0)) goto WRITE_END;  // end any open table
		if (sect->next)
			if (!WriteRtfControl(rtf,"sect",PARAM_NONE,0)) goto WRITE_END; // end the previous section

      continue;
	}

    // terminate table in not terminated properly
    if (rtf->RtfInTable) {
       if (!WriteRtfControl(rtf,"sstblend",PARAM_NONE,0)) goto WRITE_END; // instructs the reader to ignore the following cell/row symbols
       if (!WriteRtfControl(rtf,"cell",PARAM_NONE,0)) goto WRITE_END; // write para break
       if (!WriteRtfControl(rtf,"row",PARAM_NONE,0)) goto WRITE_END; // write para break
    }

    // end all open groups
    for (j=rtf->GroupLevel;j>0;j--) if (!EndRtfGroup(rtf)) goto WRITE_END;

	ret=TRUE;
	SetReturnCode_ced(IDS_ERR_NO);
	goto FINAL;
WRITE_END:
	SetReturnCode_ced(CFIO_GetReturnCode());
	ret=FALSE;
FINAL:
    // flush text buffer
    FlushRtfLine(rtf);             // flush the rtf line to the output

	Close(rtf->hFile,CSF_SAVEDISK);

    if (rtf->table)   delete[] rtf->table;          // free rtf control area
    if (merge&&rtf->oldFile)   Free(rtf->oldFile);          // free old rtf
    if (rtf)   delete rtf;          // free rtf control area
    if (color) delete[] color;        // free the rtf color table

//    if (output==RTF_FILE || output==RTF_BUF) TerArg.modified=PageModifyCount=0;       // reset modification flag

    return ret;
}

char* GetLiteralBorderType(int type);

Bool WriteRtfPara(struct StrRtfOut far *rtf,CEDParagraph* p, Bool brk)
{
	CEDLine * lastLin=0;
	CEDLine* l;
	CEDChar* chr;
	CEDChar lastChar;
	int j,FirstCol,LastCol,len;
	char ptr[MAX_LEN];

	/*       // write header control if any
       if (LineLen(l)==1 && tabw(l) && tabw(l)->type&INFO_HDR) {
          if (!RtfInHdrFtr) {
             if (l==0 || !(PfmtId[pfmt(l-1)].flags&PAGE_HDR)) {
                if (!BeginRtfGroup(w,rtf)) goto WRITE_END;
                if (!WriteRtfControl(w,rtf,"header",PARAM_NONE,0)) goto WRITE_END; // end the previous section
                RtfInHdrFtr=PAGE_HDR;
             }
          }
          else {
             if (!EndRtfGroup(w,rtf)) goto WRITE_END;
             RtfInHdrFtr=0;
             PrevFont=PrevPfmt=-1;    // force write of attribute
          }
          continue;
       }

       // write footer control if any
       if (LineLen(l)==1 && tabw(l) && tabw(l)->type&INFO_FTR) {
          if (!RtfInHdrFtr) {
             if (l==0 || !(PfmtId[pfmt(l-1)].flags&PAGE_FTR)) {
                if (!BeginRtfGroup(w,rtf)) goto WRITE_END;
                if (!WriteRtfControl(w,rtf,"footer",PARAM_NONE,0)) goto WRITE_END; // end the previous section
                RtfInHdrFtr=PAGE_FTR;
             }
          }
          else {
             if (!EndRtfGroup(w,rtf)) goto WRITE_END;
             RtfInHdrFtr=0;
             PrevFont=PrevPfmt=-1;    // force write of attribute
          }
          continue;
       }
*/
       // write paragraph formats
//       if ((int)pfmt(l)!=PrevPfmt || cid(l)!=PrevCell || fid(l)!=PrevFID) {
         // terminate any super/subscript groups
//         if (rtf->flags&ROFLAG_IN_SUPSCR) {
//            if (!EndRtfGroup(rtf)) return FALSE;      // end the group
//            rtf->flags&=~ROFLAG_IN_SUPSCR;  // out of superscript group
//         }
//         if (rtf->flags&ROFLAG_IN_SUBSCR) {
//            if (!EndRtfGroup(rtf)) return FALSE;      // end the group
//            rtf->flags&=~ROFLAG_IN_SUBSCR;  // out of subscript group
//         }

		if (rtf->RtfInFrame&&rtf->reset)
			if (!WriteFrmPos(rtf,rtf->frm,rtf->wrtFrmSz)) return FALSE;
		if (!WriteRtfParaFmt(rtf,p,rtf->PrevPfmt/*,cid(l),PrevCell,fid(l),PrevFID*/)) return FALSE;
		rtf->PrevPfmt=p;
//         PrevCell=cid(l);
//         PrevFID=fid(l);

         // reset font as fonts could have changed in a higher group
//         if (!WriteRtfControl(w,rtf,"plain",PARAM_NONE,0)) goto WRITE_END;
//         if (!WriteRtfControl(w,rtf,"fs",PARAM_INT,2*TerFont[0].PointSize)) goto WRITE_END;  // write because pointsize can be intepretted differently on 'plain'
//         PrevFont=-1;
//       }

       // Write character formats and para break
		 int parent=p->lines?p->lines->parentNumber:0;
		for (l=p->lines;l&&l->parentNumber==parent;l=l->next)
		{
			lastLin=l;
			// determine the column range to write
			FirstCol=0;
			LastCol=CED_GetCountChar(l);                // last uchar not included
			if (FirstCol>=LastCol) goto LINE_END;  // nothing to write

//       ptr=pText(l);                   // lock the text
//       fmt=OpenCfmt(w,l);
//       CurFont=fmt[0];
//	   if(CurFont!=PrevFont) // Piter CharSet
//	   {
//               if (!WriteRtfCharFmt(rtf,l->GetChar(0))) return FALSE;
//               PrevFont=CurFont;
//	   }
       len=0;      // length of the segment
       for (j=0;j<=LastCol;j++)
	   {
		   chr=l->GetChar(j);//this construction is correct
			//!!Warning  - cycle is longer then a line (length of line +1), so be careful
		   //this one is also correct
		   CEDChar * prvCh=&(rtf->PrevChar);//chr?chr->prev:0;
		   //in the next if we primarily need last call check
		   // since it is the symbol outside of a line
          if ( j==LastCol || len==MAX_LEN||
			  (prvCh&&(prvCh->fontHeight!=chr->fontHeight||
			  prvCh->fontAttribs!=chr->fontAttribs||
			  prvCh->fontNum!=chr->fontNum))
/*            || ptr[j]==ParaChar || ptr[j]==CellChar || ptr[j]==LINE_CHAR
            || ptr[j]==ROW_CHAR*/ || CED_IsPicture(chr)/*TerFont[fmt[j]].style&PICT*/)
		  {
             if (!WriteRtfText(rtf,ptr,len)) return FALSE; // write text
             if (j<LastCol&&prvCh&&(!CED_IsPicture(chr))&&(prvCh->fontHeight!=chr->fontHeight||
			  prvCh->fontAttribs!=chr->fontAttribs||
			  prvCh->fontNum!=chr->fontNum))
			 {          // write font change
               if (!WriteRtfCharFmt(rtf,chr)) return FALSE;
//               PrevFont=CurFont;
             }
             if (j==LastCol) break;           // end of line
             len=0;                           // reset line segment length
          }
//          CurFont=fmt[j];

          // write picture and parabreak
		   if(chr&&CED_IsPicture(chr))
			{
				for (int pict=0;pict<rtf->page->picsUsed;pict++)
				{
					if (rtf->page->picsTable[pict].pictNumber==chr->fontNum-ED_PICT_BASE)
					{
						if (rtf->page->picsTable[pict].type==1)
						{
							if (!WriteRtfDIB(rtf, pict)) return FALSE;
						}
						else if (rtf->page->picsTable[pict].type==2)
							if (!WriteRtfMetafile(rtf, pict)) return FALSE;

						break;
					}
				}
				continue;
			}
/*          if (TerFont[CurFont].style&PICT) {
             result=TRUE;
             if (output==RTF_FRAME_BUF) {     // write objects as ids
                result=WriteRtfControl(w,rtf,"subpictid",PARAM_INT,CurFont);
             }
             else if (TerFont[CurFont].ObjectType==OBJ_NONE || TerFont[CurFont].ObjectType==OBJ_EMBED_TEMP) {
                if (TerFont[CurFont].LinkFile && !(TerFlags&TFLAG_IGNORE_PICT_LINK))
                   result=WriteRtfLinkedPicture(w,rtf,CurFont);
                else {
                   if      (TerFont[CurFont].PictType==PICT_METAFILE) result=WriteRtfMetafile(w,rtf,CurFont);
                   else if (TerFont[CurFont].PictType==PICT_DIBITMAP) result=WriteRtfDIB(w,rtf,CurFont);
                   else if (TerFont[CurFont].PictType==PICT_CONTROL)  result=WriteRtfCtl(w,rtf,CurFont);
                }
             }
             else {
                result=WriteRtfObject(w,rtf,CurFont);
             }
             if (!result) goto WRITE_END;
          }
*/
		if(!CED_IsPicture(chr))
		{
			ptr[len]=chr->alternatives[0].alternative;
			len++;
		}

/*          else if (ptr[j]==CellChar || ptr[j]==LINE_CHAR) { // write cell or line break
             if (CurFont!=PrevFont) {        // write the font change
                if (!WriteRtfCharFmt(w,rtf,CurFont,PrevFont)) goto WRITE_END; // write the font change
                PrevFont=CurFont;
             }
             if (ptr[j]==CellChar  && !WriteRtfControl(w,rtf,"cell",PARAM_NONE,0)) goto WRITE_END;
             if (ptr[j]==LINE_CHAR && !WriteRtfControl(w,rtf,"line",PARAM_NONE,0)) goto WRITE_END;
          }
          else if (ptr[j]==ROW_CHAR) {       // write row break
             if (CurFont!=PrevFont) {        // write the font change
                if (!WriteRtfCharFmt(w,rtf,CurFont,PrevFont)) goto WRITE_END; // write the font change
                PrevFont=CurFont;
             }
             if (!WriteRtfControl(w,rtf,"intbl",PARAM_NONE,0)) goto WRITE_END; // write para break
             if (!WriteRtfControl(w,rtf,"row",PARAM_NONE,0)) goto WRITE_END; // write para break
             RtfInTable=FALSE;
          }
          else len++;                         // do not write para char
 */    }
//       CloseCfmt(w,l);

       LINE_END:
       // Write EOL in non-wordwrap mode
//       if (!TerArg.WordWrap && l<WriteEndRow && !WriteRtfControl(w,rtf,"par",PARAM_NONE,0)) goto WRITE_END; // write para break
       //If line is not last one in paragraph
		if (l->next&&l->next->parentNumber==parent&&l->hardBreak)
		   if (!WriteRtfControl(rtf,"line",PARAM_NONE,0)) return FALSE;
		}


//         int ParaFID=PrevFID;
//         if (CurFont!=PrevFont) {        // write the font change
		if (brk)
		{
			if (lastLin&&lastLin->defChrFontHeight>0)
			{
				memcpy(&lastChar,&(rtf->PrevChar),sizeof(CEDChar));
				lastChar.fontHeight=lastLin->defChrFontHeight;
//			int ttt=lastLin->GetChar(lastLin->GetCountChar()-1)->fontHeight;
//			lastLin->GetChar(lastLin->GetCountChar()-1)->fontHeight=lastLin->defChrFontHeight;
				if (!WriteRtfCharFmt(rtf,&lastChar)) return FALSE; // write the font change
//				memset (&(rtf->PrevChar),0,sizeof(CEDChar));
				rtf->PrevChar.fontHeight=lastLin->defChrFontHeight;
//				PrevFont=CurFont;
//			lastLin->GetChar(lastLin->GetCountChar()-1)->fontHeight=ttt;
			}
//         }
//         if (ParaFID==0 || !(ParaFrame[ParaFID].flags&(PARA_FRAME_LINE|PARA_FRAME_RECT))) {
			if (!WriteRtfControl(rtf,"par",PARAM_NONE,0)) return FALSE; // write para break
//         }
		}
	return TRUE;
    }

/*****************************************************************************
    BeginRtfGroup:
    Write the rtf 'begin group' character to output
******************************************************************************/
Bool BeginRtfGroup(struct StrRtfOut far *rtf)
{
    rtf->SpacePending=FALSE;          // delimit the last control by '{' character
    rtf->GroupLevel++;
    return PutRtfChar(rtf,'{');
}

/*****************************************************************************
    PutRtfChar:
    Write a character to rtf output
******************************************************************************/
Bool PutRtfChar(struct StrRtfOut far *rtf,uchar CurChar)
{
    Bool IgnoreSlash=rtf->flags&ROFLAG_IGNORE_SLASH;

    if (rtf->SpacePending) {
       rtf->text[rtf->TextLen]=' ';       // append the pending space
       rtf->TextLen++;
       rtf->SpacePending=FALSE;
    }

    // check for line break
    if (!rtf->WritingControl && (rtf->TextLen+1)>MAX_WIDTH/3) if (!FlushRtfLine(rtf)) return FALSE;  // try not be break at a control word
    if (CurChar=='\\' && !IgnoreSlash && (rtf->TextLen+1)>MAX_WIDTH/2)  if (!FlushRtfLine(rtf)) return FALSE;  // flush the current line
    if ((rtf->TextLen+1)>MAX_WIDTH)  if (!FlushRtfLine(rtf)) return FALSE;  // flush the current line

    rtf->text[rtf->TextLen]=CurChar;      // store current character
    rtf->TextLen++;

    return TRUE;
}


/*****************************************************************************
    WriteRtfControl:
    Write the rtf control text to rtf output device.  The control text should
    be NULL terminated.  The next argument provide control parameter type.
******************************************************************************/
Bool WriteRtfControl(struct StrRtfOut far *rtf, const char* control, int type, double val)
{
    char string[20];

    rtf->SpacePending=FALSE;      // no need to write the space after the last control

    rtf->WritingControl=TRUE;     // beginning of a control word

    //write the control prefix
    if (!PutRtfChar(rtf,'\\')) return FALSE; // write prefix

    //write the control text
    if (!WriteRtfText(rtf,control,lstrlen(control))) return FALSE;

    //write the numeric parameter
    if (type==PARAM_INT) {         // write in integer format
       wsprintf(string,"%ld",(long)val);
       if (!WriteRtfText(rtf,string,lstrlen(string))) return FALSE;
    }
    else if (type==PARAM_DBL) {    // write in double format
       wsprintf(string,"%f",val);
       if (!WriteRtfText(rtf,string,lstrlen(string))) return FALSE;
    }

    // write the space delimiter
    rtf->SpacePending=TRUE;      // this space will be written out only if next word is not a control

    rtf->WritingControl=FALSE;   // end of a control word

    return TRUE;

}

/*****************************************************************************
    FlustRtfLine:
    Write the current line to the RTF file or output buffer
******************************************************************************/
Bool FlushRtfLine(struct StrRtfOut far *rtf)
{
    if (rtf->TextLen==0) return TRUE;    // nothing to flush

    // add cr/lf to the line
    rtf->text[rtf->TextLen]=0xd;
    rtf->TextLen++;
    rtf->text[rtf->TextLen]=0xa;
    rtf->TextLen++;


    if (rtf->hFile) {    // write to file
       if (HFILE_ERROR==(HFILE)Write(rtf->hFile,(char*)rtf->text,rtf->TextLen)) {
          return 0;//PrintError(w,MSG_ERR_FILE_WRITE,"FlushRtfLine");
       }
    }
/*    else {                   // write to buffer
       if (rtf->BufIndex+rtf->TextLen>rtf->BufLen) { // expand the buffer
          rtf->BufLen=rtf->BufLen + (rtf->BufLen/4); // expand 25 percent
          if (rtf->BufLen<(rtf->BufIndex+rtf->TextLen)) rtf->BufLen=rtf->BufIndex+rtf->TextLen;

          GlobalUnlock(rtf->hBuf);
          if ( NULL==(rtf->hBuf=GlobalReAlloc(rtf->hBuf,rtf->BufLen+1, 0))
            || NULL==(rtf->buf=(uchar *)GlobalLock(rtf->hBuf)) ){
             return PrintError(w,MSG_OUT_OF_MEM,"FlushRtfLine");
          }
       }
       HugeMove(rtf->text,&(rtf->buf[rtf->BufIndex]),rtf->TextLen);
       rtf->BufIndex=rtf->BufIndex+rtf->TextLen;
    }
*/
    rtf->TextLen=0;                                 // reset buffer
    rtf->SpacePending=FALSE;                        // line already delimited

    return TRUE;
}

/*****************************************************************************
    WriteRtfFont:
    Write the font table to the rtf output device.
******************************************************************************/
Bool WriteRtfFont(struct StrRtfOut far *rtf, Bool head)
{
//    int i;
	char /*name[32],*/family[32];
//    uchar FontFamily,DefFamily;

	if (head)
	{
		if (!BeginRtfGroup(rtf)) return FALSE;      // begin font group
		if (!WriteRtfControl(rtf,"fonttbl",PARAM_NONE,0)) return FALSE;
	}
	char* ch=0;
//	char tmp[100];
	fontDiscr fond;
	CEDPage* page=rtf->page;

	for (int q=0;q<page->fontsUsed;q++)
	{
		if (rtf->table[q]<=rtf->maxFntNum) continue;
		page->GetFont(q,&(fond.fontNumber),&(fond.fontPitchAndFamily),
			&(fond.fontCharset),&ch);
		//write font info
		if (!BeginRtfGroup(rtf)) return FALSE;// begin current font
		if (!WriteRtfControl(rtf,"f",PARAM_INT,rtf->table[q])) return FALSE;  // write font number
		rtf->WritingControl=TRUE;                   // disable line break
		rtf->SpacePending=FALSE;      // no need to write the space after the last control

		switch(fond.fontPitchAndFamily&0xf0)
		{
		case FF_DECORATIVE:
			lstrcpy(family,"decor");
			break;
		case FF_DONTCARE:
			lstrcpy(family,"nil");
			break;
		case FF_MODERN:
			lstrcpy(family,"modern");
			break;
		case FF_ROMAN:
			lstrcpy(family,"roman");
			break;
		case FF_SCRIPT:
			lstrcpy(family,"script");
			break;
		case FF_SWISS:
			lstrcpy(family,"swiss");
			break;
		}
		if (!PutRtfChar(rtf,'\\')) return FALSE;  // write family control prefix
		if (!PutRtfChar(rtf,'f'))  return FALSE;  // write family control
		if (!WriteRtfText(rtf,family,lstrlen(family))) return FALSE; // write font family

		switch(fond.fontPitchAndFamily&0xf)
		{
		case DEFAULT_PITCH:
			lstrcpy(family,"prq0");
			break;
		case FIXED_PITCH:
			lstrcpy(family,"prq1");
			break;
		case VARIABLE_PITCH:
			lstrcpy(family,"prq2");
			break;
		}
		if (!PutRtfChar(rtf,'\\')) return FALSE;  // write family control prefix
		if (!PutRtfChar(rtf,'f'))  return FALSE;  // write family control
		if (!WriteRtfText(rtf,family,lstrlen(family))) return FALSE; // write font family

		if (!WriteRtfControl(rtf,"fcharset",PARAM_INT,fond.fontCharset)) return FALSE;
		//write font ame
	  rtf->SpacePending=FALSE;//in order to paste 1 blank, not 2
      rtf->WritingControl=TRUE;                   // disable line break
      if (!WriteRtfText(rtf," ",1)) return FALSE;// write family delimiter

      if (!WriteRtfText(rtf,ch,lstrlen(ch))) return FALSE; // write font typeface

	  switch(fond.fontCharset)
		{
		case 204:// Cyrillic
			if (memcmp(ch+lstrlen(ch)-strlen(" Cyr")," Cyr",strlen(" Cyr"))!=0)
			if (!WriteRtfText(rtf," Cyr",4)) return FALSE;
			  break;
		case 238:// Central Europe
			if (memcmp(ch+lstrlen(ch)-strlen(" CE")," CE",strlen(" CE"))!=0)
		      if (!WriteRtfText(rtf," CE",3)) return FALSE;
			  break;
		case 0:// Default
		default:	;
		}

		if (!WriteRtfText(rtf,";",1)) return FALSE;// write typeface delimiter

      if (!EndRtfGroup(rtf)) return FALSE;      // end current font

      rtf->WritingControl=FALSE;                  // enable line breaks

	}
	if (head)
		if (!EndRtfGroup(rtf)) return FALSE;        // end font group

    return TRUE;
}
/*****************************************************************************
    WriteRtfMargin:
    Write default document margins
******************************************************************************/
Bool WriteRtfMargin(struct StrRtfOut far *rtf)
{
//    float PaperHeight,PaperWidth;
	CEDPage * page=rtf->page;

	if (page->pageSizeInTwips.cx>=0)
		if (!WriteRtfControl(rtf,"paperw",PARAM_INT,page->pageSizeInTwips.cx)) return FALSE;
	if (page->pageSizeInTwips.cy>=0)
		if (!WriteRtfControl(rtf,"paperh",PARAM_INT,page->pageSizeInTwips.cy)) return FALSE;

	if (page->pageBordersInTwips.left>=0)
		if (!WriteRtfControl(rtf,"margl",PARAM_INT,page->pageBordersInTwips.left)) return FALSE;
	if (page->pageBordersInTwips.right>=0)
	    if (!WriteRtfControl(rtf,"margr",PARAM_INT,page->pageBordersInTwips.right)) return FALSE;
	if (page->pageBordersInTwips.top>=0)
	    if (!WriteRtfControl(rtf,"margt",PARAM_INT,page->pageBordersInTwips.top)) return FALSE;
	if (page->pageBordersInTwips.bottom>=0)
	    if (!WriteRtfControl(rtf,"margb",PARAM_INT,page->pageBordersInTwips.bottom)) return FALSE;

    return TRUE;
}

/*****************************************************************************
    WriteRtfSection:
    Write the section properties for a given section
******************************************************************************/
Bool WriteRtfSection(struct StrRtfOut far *rtf, CEDSection* sect)
{
    if (!WriteRtfControl(rtf,"sectd",PARAM_NONE,0)) return FALSE;  // set to default

	// write the landscape and paper dimensions
    if (sect->orientation==ED_ORIENT_LANDSCAPE)
	{
       if (!WriteRtfControl(rtf,"lndscpsxn",PARAM_NONE,0)) return FALSE;
	}

	if (sect->width>=0)
		if (!WriteRtfControl(rtf,"pgwsxn",PARAM_INT,sect->width)) return FALSE;
	if (sect->height>=0)
		if (!WriteRtfControl(rtf,"pghsxn",PARAM_INT,sect->height)) return FALSE;

    // write margins
    if(sect->borders.left>=0)
		if (!WriteRtfControl(rtf,"marglsxn",PARAM_INT,sect->borders.left)) return FALSE;
    if(sect->borders.right>=0)
		if (!WriteRtfControl(rtf,"margrsxn",PARAM_INT,sect->borders.right)) return FALSE;
    if(sect->borders.top>=0)
		if (!WriteRtfControl(rtf,"margtsxn",PARAM_INT,sect->borders.top)) return FALSE;
    if(sect->borders.bottom>=0)
		if (!WriteRtfControl(rtf,"margbsxn",PARAM_INT,sect->borders.bottom)) return FALSE;

    if(sect->headerY>=0)
		if (!WriteRtfControl(rtf,"headery",PARAM_INT,sect->headerY)) return FALSE;
    if(sect->footerY>=0)
		if (!WriteRtfControl(rtf,"footery",PARAM_INT,sect->footerY)) return FALSE;

    // write break
    if (sect->sectionBreak==ED_SECT_CONTINUE) if (!WriteRtfControl(rtf,"sbknone",PARAM_NONE,0)) return FALSE;
    if (sect->sectionBreak==ED_SECT_NEW_PAGE) if (!WriteRtfControl(rtf,"sbkpage",PARAM_NONE,0)) return FALSE;

	if (sect->lineBetCol)
		if (!WriteRtfControl(rtf,"linebetcol",PARAM_NONE,0)) return FALSE;
    // write columns
    if (sect->numSnakeCols>1)
	{
	int i;
		if (!WriteRtfControl(rtf,"cols",PARAM_INT,sect->numSnakeCols)) return FALSE;
		if (!WriteRtfControl(rtf,"colsx",PARAM_INT,MAX(sect->colInterval,0))) return FALSE;
		//dont write column info if first width is -1, i.e. user set different-width property
		if (sect->colInfo&&sect->colInfo[0].width>=0)
			for (i=0;i<sect->numSnakeCols;i++)
			{
				if (!WriteRtfControl(rtf,"colno",PARAM_INT,i+1)) return FALSE;
				if (!WriteRtfControl(rtf,"colw",PARAM_INT,sect->colInfo[i].width)) return FALSE;
				if (!WriteRtfControl(rtf,"colsr",PARAM_INT,sect->colInfo[i].space)) return FALSE;
			}
    }
    return TRUE;
}
/*****************************************************************************
    WriteRtfCharFmt:
    Write the character formatting info when the attributes change from
    the previsous font selection
******************************************************************************/
Bool WriteRtfCharFmt(struct StrRtfOut far *rtf,CEDChar* curChar)
{
//    uchar CurTypeFace[32],PrevTypeFace[32];
    uchar CurFamily,PrevFamily;
    uint CurStyle,PrevStyle;
//    int  CurCharSID,PrevCharSID;
    int  /*i,*/CurPointSize,PrevPointSize/*,CurFieldId,PrevFieldId*/;
    Bool result;
    COLORREF PrevTextColor,CurTextColor;
    COLORREF PrevTextBkColor,CurTextBkColor;
//	uchar CurCharSet,PrevCharSet;

	CEDChar* prevChar=&(rtf->PrevChar);//curChar->prev;
    if (curChar==0) return TRUE;
//    if (curChar->fontNum>=rtf->page->fontsUsed || (prevChar&&prevChar->fontNum>=rtf->page->fontsUsed)) return TRUE;

    // extract value for comparison
    if (prevChar>0) {
//       lstrcpy(PrevTypeFace,TerFont[PrevFont].TypeFace);
       PrevFamily=rtf->table[rtf->page->GetFontByNum(prevChar->fontNum)];
       PrevStyle=prevChar->fontAttribs;
       PrevTextColor=prevChar->foregroundColor;
       PrevTextBkColor=prevChar->backgroundColor;
//       PrevFieldId=TerFont[PrevFont].FieldId;
//       PrevCharSID=TerFont[PrevFont].CharStyId;
       PrevPointSize=prevChar->fontHeight;  // store as twice the point size
//	   PrevCharSet = 	TerFont[PrevFont].CharSet; // Piter CharSet
    }
    else {
//       lstrcpy(PrevTypeFace,"");
       PrevFamily=-1;
       PrevStyle=0;
       PrevTextColor=-1;
       PrevTextBkColor=-1;
       PrevPointSize=0;
//       PrevFieldId=0;
//       PrevCharSID=1;          // default style id
//	   PrevCharSet = 0; // Piter CharSet

       WriteRtfControl(rtf,"plain",PARAM_NONE,0);  // turnoff formatting
    }
//    lstrcpy(CurTypeFace,TerFont[CurFont].TypeFace);
    CurFamily=rtf->table[rtf->page->GetFontByNum(curChar->fontNum)];
    CurStyle=curChar->fontAttribs;
    CurTextColor=curChar->foregroundColor;
    CurTextBkColor=curChar->backgroundColor;
//    CurFieldId=TerFont[CurFont].FieldId;
//    CurCharSID=TerFont[CurFont].CharStyId;
    CurPointSize=curChar->fontHeight;  // store as twice the point size
//	CurCharSet=TerFont[CurFont].CharSet; // Piter CharSet

    // end superscript/subscript groups
    if ((CurStyle&ED_SUPSCR)!=(PrevStyle&ED_SUPSCR) && !(CurStyle&ED_SUPSCR)) {
       if (!EndRtfGroup(rtf)) return FALSE;      // end the group
       rtf->flags&=~ROFLAG_IN_SUPSCR;  // out of superscript group
    }
    if ((CurStyle&ED_SUBSCR)!=(PrevStyle&ED_SUBSCR) && !(CurStyle&ED_SUBSCR)) {
       if (!EndRtfGroup(rtf)) return FALSE;      // end the group
       rtf->flags&=~ROFLAG_IN_SUBSCR;  // out of subscript group
    }

    // temporarily halt superscript/subscript groups
    if (rtf->flags&ROFLAG_IN_SUPSCR) if (!EndRtfGroup(rtf)) return FALSE;      // end the group
    if (rtf->flags&ROFLAG_IN_SUBSCR) if (!EndRtfGroup(rtf)) return FALSE;      // end the group

    // Check for the end of a previous field group
//    if (PrevFieldId>0 && PrevFieldId!=CurFieldId) {  // close the previous field result
//       if (!EndRtfGroup(w,rtf)) return FALSE;        // end current field result
//       if (!EndRtfGroup(w,rtf)) return FALSE;        // end current field group
//
//       WriteRtfControl(w,rtf,"plain",PARAM_NONE,0);  // turnoff formatting
//       lstrcpy(PrevTypeFace,"");                     // force writing of font information
//       PrevFamily=FF_DONTCARE;
//       PrevStyle=0;
//       PrevTextColor=0;
//       PrevTextBkColor=TextDefBkColor;
//       PrevPointSize=0;
//       PrevFieldId=0;
//       PrevCharSID=1;                    // default style id
//	   PrevCharSet = 0; // Piter CharSet
//    }

    // Check for the end of a previous equation (boxed string)
//    if (PrevStyle&CHAR_BOX && !(CurStyle&CHAR_BOX)) {  // close the previous field result
//       WriteRtfText(w,rtf,")",1);                    // close the equation
//       if (!EndRtfGroup(w,rtf)) return FALSE;        // close the fldinst group
//       if (!BeginRtfGroup(w,rtf)) return FALSE;      // begin group for fldrslt
//       if (!WriteRtfControl(w,rtf,"fldrslt",PARAM_NONE,0)) return FALSE;   // field display
//       if (!EndRtfGroup(w,rtf)) return FALSE;        // close fldrslt group
//       if (!EndRtfGroup(w,rtf)) return FALSE;        // close field group
//
//       WriteRtfControl(w,rtf,"plain",PARAM_NONE,0);  // turnoff formatting
//       lstrcpy(PrevTypeFace,"");                     // force writing of font information
//       PrevFamily=FF_DONTCARE;
//       PrevStyle=0;
//       PrevTextColor=0;
//       PrevTextBkColor=TextDefBkColor;
//       PrevPointSize=0;
//       PrevFieldId=0;
//       PrevCharSID=1;                    // default style id
//	   PrevCharSet = 0; // Piter CharSet
//    }

    // check for the end of previous footnote
//    if (PrevStyle&FNOTETEXT && CurStyle&FNOTEREST) if (!EndRtfGroup(w,rtf)) return FALSE;
//    if (PrevStyle&FNOTEREST && !(CurStyle&(FNOTETEXT|FNOTEREST))) if (!EndRtfGroup(w,rtf)) return FALSE;
//    if (PrevStyle&FNOTE && !(CurStyle&(FNOTE|FNOTETEXT|FNOTEREST))) if (!EndRtfGroup(w,rtf)) return FALSE;
//   if (PrevStyle&FNOTETEXT && !(CurStyle&(FNOTETEXT|FNOTEREST))) {
//       if (!EndRtfGroup(w,rtf)) return FALSE;
//       if (!EndRtfGroup(w,rtf)) return FALSE;
//    }

    // check for the beginning a footnote style
//    if (!(PrevStyle&FNOTE) && CurStyle&FNOTE) if (!BeginRtfGroup(w,rtf)) return FALSE;      // begin footnote reference group
//    if (PrevStyle&FNOTE && CurStyle&FNOTETEXT) {
//       if (!BeginRtfGroup(w,rtf)) return FALSE;      // begin footnote reference group
//       if (!WriteRtfControl(w,rtf,"footnote",PARAM_NONE,0)) return FALSE;
//    }


    // check for default properties
    if ( CurFamily==0
      && CurPointSize==24
      && (CurTextColor==0||CurTextColor==-1)
      && (CurTextBkColor==TextDefBkColor||CurTextBkColor==-1)
      && CurStyle/*&(~((uint)(FNOTEALL)))*/==0
//      && CurFieldId==0
//      && CurCharSID==1
/*	  && CurCharSet==0*/)
	  { // Piter CharSet
         if (!WriteRtfControl(rtf,"plain",PARAM_NONE,0)) return FALSE;
         return WriteRtfControl(rtf,"fs",PARAM_INT,CurPointSize);  // other RTF reader may interpret different default pointsize on 'plain', so write here
	}

    //write the character style id
//    if (CurCharSID!=PrevCharSID) {
//      if (!WriteRtfControl(w,rtf,"cs",PARAM_INT,CurCharSID)) return FALSE;
//    }

    //write typeface/family
    if (CurFamily!=PrevFamily) {
      if (!WriteRtfControl(rtf,"f",PARAM_INT,CurFamily)) return FALSE;  // write font number
    }

    //write foreground color
    if (CurTextColor!=PrevTextColor/* && CurTextColor != RGB( 0, 128, 0) && CurTextBkColor != RGB( 192, 192, 192 )*/ ) {
        int i;
       for (i=0;i<rtf->TotalColors;i++) if (rtf->color[i].color==CurTextColor) break;
       if (i==rtf->TotalColors) i=0;
       if (!WriteRtfControl(rtf,"cf",PARAM_INT,i)) return FALSE;        // write the color index
    }

    //write background color
    if (CurTextBkColor!=PrevTextBkColor/* && CurTextColor != RGB( 0, 128, 0) && CurTextBkColor != RGB( 192, 192, 192 )*/ ) {
       int i;
       for (i=0;i<rtf->TotalColors;i++) if (rtf->color[i].color==CurTextBkColor) break;
       if (i==rtf->TotalColors) i=0;
       if (!WriteRtfControl(rtf,"highlight",PARAM_INT,i)) return FALSE;        // write the color index
    }

    //write point size
    if (CurPointSize!=PrevPointSize) if (!WriteRtfControl(rtf,"fs",PARAM_INT,CurPointSize)) return FALSE;   // write point size

    //write style
//    if (CurStyle==PrevStyle) goto SKIP_STYLES;  // no change to style

    if ((CurStyle&ED_BOLD)!=(PrevStyle&ED_BOLD)) { // process bold
       if (CurStyle&ED_BOLD) result=WriteRtfControl(rtf,"b",PARAM_NONE,0); // turn on
       else               result=WriteRtfControl(rtf,"b",PARAM_INT,0);  // turn off
       if (!result) return FALSE;
    }
    if ((CurStyle&ED_ULINE)!=(PrevStyle&ED_ULINE)) { // process underline
       if (CurStyle&ED_ULINE) result=WriteRtfControl(rtf,"ul",PARAM_NONE,0); // turn on
       else                result=WriteRtfControl(rtf,"ul",PARAM_INT,0);  // turn off
       if (!result) return FALSE;
    }
    if ((CurStyle&ED_ULINED)!=(PrevStyle&ED_ULINED)) { // process double underline
       if (CurStyle&ED_ULINED) result=WriteRtfControl(rtf,"uldb",PARAM_NONE,0); // turn on
       else                 result=WriteRtfControl(rtf,"uldb",PARAM_INT,0);  // turn off
       if (!result) return FALSE;
    }
    if ((CurStyle&ED_ITALIC)!=(PrevStyle&ED_ITALIC)) { // process italic
       if (CurStyle&ED_ITALIC) result=WriteRtfControl(rtf,"i",PARAM_NONE,0); // turn on
       else                 result=WriteRtfControl(rtf,"i",PARAM_INT,0);  // turn off
       if (!result) return FALSE;
    }
    if ((CurStyle&ED_HIDDEN)!=(PrevStyle&ED_HIDDEN)) { // process hidden text
       if (CurStyle&ED_HIDDEN) result=WriteRtfControl(rtf,"v",PARAM_NONE,0); // turn on
       else                 result=WriteRtfControl(rtf,"v",PARAM_INT,0);  // turn off
       if (!result) return FALSE;
    }
    if ((CurStyle&ED_STRIKE)!=(PrevStyle&ED_STRIKE)) { // process strike
       if (CurStyle&ED_STRIKE) result=WriteRtfControl(rtf,"strike",PARAM_NONE,0); // turn on
       else                 result=WriteRtfControl(rtf,"strike",PARAM_INT,0);  // turn off
       if (!result) return FALSE;
    }
    if ((CurStyle&ED_PROTECT)!=(PrevStyle&ED_PROTECT)) { // process protect
       if (CurStyle&ED_PROTECT) result=WriteRtfControl(rtf,"protect",PARAM_NONE,0); // turn on
       else                  result=WriteRtfControl(rtf,"protect",PARAM_INT,0);  // turn off
       if (!result) return FALSE;
    }

/*    if (CurStyle&CHAR_BOX && !(PrevStyle&CHAR_BOX)) {
       if (!BeginRtfGroup(w,rtf)) return FALSE;      // begin field group
       if (!WriteRtfControl(w,rtf,"field",PARAM_NONE,0)) return FALSE;
       if (!BeginRtfGroup(w,rtf)) return FALSE;      // begin instruction group
       if (!WriteRtfControl(w,rtf,"fldinst",PARAM_NONE,0)) return FALSE;

       if (!WriteRtfText(w,rtf,"EQ \\X(",6)) return FALSE;
    }
*/
//    SKIP_STYLES:

    // Check for the beginning of new field group
/*    if (CurFieldId>0 && CurFieldId!=PrevFieldId) {
       if (!BeginRtfGroup(w,rtf)) return FALSE;      // begin field group
       if (!WriteRtfControl(w,rtf,"field",PARAM_NONE,0)) return FALSE;
       if (!BeginRtfGroup(w,rtf)) return FALSE;      // begin instruction group
       if (!WriteRtfControl(w,rtf,"fldinst",PARAM_NONE,0)) return FALSE;

       if (CurFieldId==FIELD_ID_PAGE) if (!WriteRtfText(w,rtf,"PAGE",4)) return FALSE;
       // !!! Stas begins
       if (CurFieldId==FIELD_ID_DATE) if (!WriteRtfText(w,rtf,"DATE",4)) return FALSE;
       if (CurFieldId==FIELD_ID_TIME) if (!WriteRtfText(w,rtf,"TIME",4)) return FALSE;
       if (CurFieldId==FIELD_ID_FILE) if (!WriteRtfText(w,rtf,"FILENAME",4)) return FALSE;
       // !!! Stas ends
       if (!EndRtfGroup(w,rtf)) return FALSE;        // end the instruction group
       if (!BeginRtfGroup(w,rtf)) return FALSE;      // begin the result group
       if (!WriteRtfControl(w,rtf,"fldrslt",PARAM_NONE,0)) return FALSE;
    }
*/
    // begin superscript/subscript groups
/*    if ((CurStyle&SUPSCR)!=(PrevStyle&SUPSCR) && CurStyle&SUPSCR) rtf->flags|=ROFLAG_IN_SUPSCR;
    if (rtf->flags&ROFLAG_IN_SUPSCR) {
       if (!BeginRtfGroup(w,rtf)) return FALSE;      // begin the group
       if (!WriteRtfControl(w,rtf,"super",PARAM_NONE,0)) return TRUE; // turn on
    }
    if ((CurStyle&SUBSCR)!=(PrevStyle&SUBSCR) && CurStyle&SUBSCR) rtf->flags|=ROFLAG_IN_SUBSCR;
    if (rtf->flags&ROFLAG_IN_SUBSCR) {
       if (!BeginRtfGroup(w,rtf)) return FALSE;      // begin the group
       if (!WriteRtfControl(w,rtf,"sub",PARAM_NONE,0)) return TRUE; // turn on
    }
*/
	memcpy(&rtf->PrevChar,curChar,sizeof(CEDChar));
    return TRUE;
}

/*****************************************************************************
    EndRtfGroup:
    Write the rtf 'end group' character to output
******************************************************************************/
Bool EndRtfGroup(struct StrRtfOut far *rtf)
{
    rtf->SpacePending=FALSE;          // delimit the last control by '}' character
    rtf->GroupLevel--;
    return PutRtfChar(rtf,'}');
}

/*****************************************************************************
    WriteRtfParaFmt:
    Write the paragraph formatting info when the attributes change from
    the previsous paragraph selection
******************************************************************************/
Bool WriteRtfParaFmt(struct StrRtfOut far *rtf,CEDParagraph* NewPfmt,CEDParagraph* PrevPfmt/*, int NewCell, int PrevCell, int NewFID, int PrevFID*/)
{
    int CurLeftIndent,PrevLeftIndent=0;
    int CurRightIndent,PrevRightIndent=0;
    int CurFirstIndent,PrevFirstIndent=0;
//    int CurTabId,PrevTabId=0;
//    uint CurFlags,PrevFlags=0,NoValueFlags;
//    int CurCellId,PrevCellId=0;
//    int CurRowId,PrevRowId=0;
    int CurShading,PrevShading=0;
//    int CurParaFID,PrevParaFID=0;
    int CurSpaceBefore,PrevSpaceBefore=0;
    int CurSpaceAfter,PrevSpaceAfter=0;
    int CurSpaceBetween,PrevSpaceBetween=0;
    int CurSpaceBetweenMult,PrevSpaceBetweenMult=0;
//    int CurParaSID,PrevParaSID=0;
	int CurParaKeep,PrevParaKeep=0;
	int CurAlignment,PrevAlignment=0;
	int CurLbt,PrevLbt=0;
	int CurRbt,PrevRbt=0;
	int CurTbt,PrevTbt=0;
	int CurBbt,PrevBbt=0;
	int CurRbw,PrevRbw=0;
	int CurTbw,PrevTbw=0;
	int CurBbw,PrevBbw=0;
	int CurLbw,PrevLbw=0;
	int CurBrdrBtw,PrevBrdrBtw=0;

    Bool result;

    if (!NewPfmt) return TRUE;

    // extract value for comparison
    if (PrevPfmt) {
       PrevLeftIndent=PrevPfmt->indent.left;
       PrevRightIndent=PrevPfmt->indent.right;
       PrevFirstIndent=PrevPfmt->indent.top;
//       PrevFlags=ResetUintFlag(PfmtId[PrevPfmt].flags,PAGE_HDR_FTR);
//       PrevTabId=PfmtId[PrevPfmt].TabId;
//      PrevCellId=PrevCell;
//       PrevRowId=cell[PrevCellId].row;
       PrevShading=PrevPfmt->shading;
//       PrevParaFID=PrevFID;
//       PrevParaSID=PfmtId[PrevPfmt].StyId;
       PrevSpaceBefore=PrevPfmt->interval.cx;
       PrevSpaceAfter=PrevPfmt->interval.cy;
       PrevSpaceBetween=PrevPfmt->spaceBetweenLines;
	   PrevSpaceBetweenMult=PrevPfmt->spcBtwLnsMult;
		PrevAlignment=PrevPfmt->alignment;
		PrevParaKeep=PrevPfmt->keep;
		PrevLbt=PrevPfmt->leftBrdrType;
		PrevRbt=PrevPfmt->rightBrdrType;
		PrevTbt=PrevPfmt->topBrdrType;
		PrevBbt=PrevPfmt->bottomBrdrWidth;
		PrevLbw=PrevPfmt->leftBrdrWidth;
		PrevRbw=PrevPfmt->rightBrdrWidth;
		PrevTbw=PrevPfmt->topBrdrWidth;
		PrevBbw=PrevPfmt->bottomBrdrWidth;
		PrevBrdrBtw=PrevPfmt->brdrBtw;
    }
    CurLeftIndent=NewPfmt->indent.left;
    CurRightIndent=NewPfmt->indent.right;
    CurFirstIndent=NewPfmt->indent.top;
//    CurFlags=ResetUintFlag(PfmtId[NewPfmt].flags,PAGE_HDR_FTR);
//    CurTabId=PfmtId[NewPfmt].TabId;
//    CurCellId=NewCell;
//    CurRowId=cell[CurCellId].row;
    CurShading=NewPfmt->shading;
//    CurParaFID=NewFID;
//    CurParaSID=PfmtId[NewPfmt].StyId;
    CurSpaceBefore=NewPfmt->interval.cx;
    CurSpaceAfter=NewPfmt->interval.cy;
    CurSpaceBetween=NewPfmt->spaceBetweenLines;
	CurSpaceBetweenMult=NewPfmt->spcBtwLnsMult;
	CurAlignment=NewPfmt->alignment;
	CurParaKeep=NewPfmt->keep;
	CurLbt=NewPfmt->leftBrdrType;
	CurRbt=NewPfmt->rightBrdrType;
	CurTbt=NewPfmt->topBrdrType;
	CurBbt=NewPfmt->bottomBrdrWidth;
	CurLbw=NewPfmt->leftBrdrWidth;
	CurRbw=NewPfmt->rightBrdrWidth;
	CurTbw=NewPfmt->topBrdrWidth;
	CurBbw=NewPfmt->bottomBrdrWidth;
	CurBrdrBtw=NewPfmt->brdrBtw;
    // terminate the previous para frame text box
//    if (PrevParaFID>0 && PrevParaFID!=CurParaFID && ParaFrame[PrevParaFID].flags&PARA_FRAME_TEXT_BOX) {
//       if (!WritePfObjectTail(w,rtf,PrevFID)) return FALSE;
//    }

    // check for default para
    if ( CurLeftIndent==0 && CurRightIndent==0 && CurFirstIndent==0
      /*&& CurFlags==0 && CurTabId==0 && CurCellId==0 */
      && CurShading==0 /*&& CurParaFID==0 && PrevParaFID==0 */
      && CurSpaceBefore==0 && CurSpaceAfter==0 && CurSpaceBetween==0
      /*&& CurParaSID==0*/&&!rtf->RtfInFrame&&!rtf->RtfInTable&&CurAlignment==0 ) {
       return WriteRtfControl(rtf,"pard",PARAM_NONE,0);
    }

    // do we need to reset the paragraph?
//    NoValueFlags=PARA_KEEP|PARA_KEEP_NEXT|PARA_BOX_ATTRIB|BULLET;  // para flags which do not have turn on/off values

    if (  rtf->reset)
	{  // reset para id

//do this by hands before the call
//       WriteRtfControl(rtf,"pard",PARAM_NONE,0);   // reset para information as tab stops are not incremental
       if (rtf->RtfInTable)
          WriteRtfControl(rtf,"intbl",PARAM_NONE,0);   // indicate that we are in a table now

       // reset previous variable to force write any changes
       PrevLeftIndent=PrevRightIndent=PrevFirstIndent/*=PrevTabId=PrevParaFID*/=0;
       PrevSpaceBefore=PrevSpaceAfter=PrevSpaceBetween=PrevSpaceBetweenMult=0;
	   PrevAlignment=TP_LEFT_ALLIGN;
//       PrevFlags=0;
//       PrevParaSID=0;
    }


//    // check if style id needs to be written
//    if (CurParaSID!=PrevParaSID || reset) {
//       if (!WriteRtfControl(w,rtf,"s",PARAM_INT,CurParaSID)) return FALSE;
//    }


    // check if tab id needs to be written
//    if (CurTabId!=PrevTabId || reset) {
//       // write tab position
//       if (!WriteRtfTab(w,rtf,CurTabId)) return FALSE;

       // force writing of other parameters
//       PrevFirstIndent=PrevLeftIndent=PrevRightIndent=0;
//       PrevSpaceBefore=PrevSpaceAfter=PrevSpaceBetween=0;
//       PrevFlags=0;
//    }

    // write any paragraph bullets
//    if (CurFlags&BULLET && (reset || !(PrevFlags&BULLET))) {
//       if (!WriteRtfBullet(w,rtf)) return FALSE;
//    }

    // check if para border specification need to be written
    if (rtf->reset ||CurLbt!=PrevLbt ||CurRbt!=PrevRbt ||CurTbt!=PrevTbt
		||CurBbt!=PrevBbt ||CurLbw!=PrevLbw ||CurRbw!=PrevRbw
		||CurTbw!=PrevTbw ||CurBbw!=PrevBbw)
	{

       if (!WriteRtfParaBorder(rtf,NewPfmt)) return FALSE;

       // force writing of other parameters
       PrevFirstIndent=PrevLeftIndent=PrevRightIndent=PrevSpaceBetweenMult=0;
//       PrevFlags=0;
    }

    // check if para frame specification need to be written
/*    if ((reset || CurParaFID!=PrevParaFID) && CurParaFID>0) {
       if (ParaFrame[CurParaFID].flags&PARA_FRAME_OBJECT) {
          // write object frame header
          if (!BeginRtfGroup(w,rtf)) return FALSE;   // begin 'do' group
          if (!WriteRtfControl(w,rtf,"do",PARAM_NONE,0)) return FALSE;
          if (!(ParaFrame[CurParaFID].flags&PARA_FRAME_HPAGE) && !WriteRtfControl(w,rtf,"dobxmargin",PARAM_NONE,0)) return FALSE;
          if ((ParaFrame[CurParaFID].flags&PARA_FRAME_HPAGE) && !WriteRtfControl(w,rtf,"dobxpage",PARAM_NONE,0)) return FALSE;
          if (!(ParaFrame[CurParaFID].flags&PARA_FRAME_VPAGE) && !WriteRtfControl(w,rtf,"dobypara",PARAM_NONE,0)) return FALSE;
          if ((ParaFrame[CurParaFID].flags&PARA_FRAME_VPAGE) && !WriteRtfControl(w,rtf,"dobypage",PARAM_NONE,0)) return FALSE;
          if (!WriteRtfControl(w,rtf,"dodhgt",PARAM_INT,12288+ParaFrame[CurParaFID].y)) return FALSE;     // z order

          // write the frame object type
          if (ParaFrame[CurParaFID].flags&PARA_FRAME_TEXT_BOX) {
             if (!WriteRtfControl(w,rtf,"dptxbx",PARAM_NONE,0)) return FALSE;  // text box
             if (!WriteRtfControl(w,rtf,"dptxbxmar",PARAM_INT,ParaFrame[CurParaFID].margin)) return FALSE;  // text box
             if (!BeginRtfGroup(w,rtf)) return FALSE;   // begin the text group
             if (!WriteRtfControl(w,rtf,"dptxbxtext",PARAM_NONE,0)) return FALSE;  // text box
          }
          else if (ParaFrame[CurParaFID].flags&PARA_FRAME_LINE) {
             if (!WriteRtfControl(w,rtf,"dpline",PARAM_NONE,0)) return FALSE;  // text box
             WritePfObjectTail(w,rtf,NewFID);
          }
          else if (ParaFrame[CurParaFID].flags&PARA_FRAME_RECT) {
             if (!WriteRtfControl(w,rtf,"dprect",PARAM_NONE,0)) return FALSE;  // text box
             WritePfObjectTail(w,rtf,NewFID);
          }
       }
       else {             // write text frame header
          // write the additional para border information
          int margin=ParaFrame[CurParaFID].margin;
          if (margin<0) margin=0;
          if (!WriteRtfControl(w,rtf,"brsp",PARAM_INT,margin)) return FALSE;

          // write the frame information
          if      ((ParaFrame[CurParaFID].flags&PARA_FRAME_VPAGE) && !WriteRtfControl(w,rtf,"pvpg",PARAM_NONE,0)) return FALSE;
          else if ((ParaFrame[CurParaFID].flags&PARA_FRAME_VMARG) && !WriteRtfControl(w,rtf,"pvmrg",PARAM_NONE,0)) return FALSE;
          else if (!(ParaFrame[CurParaFID].flags&(PARA_FRAME_VPAGE|PARA_FRAME_VMARG)) && !WriteRtfControl(w,rtf,"pvpara",PARAM_NONE,0)) return FALSE;

          if (!(ParaFrame[CurParaFID].flags&PARA_FRAME_HPAGE) && !WriteRtfControl(w,rtf,"phmrg",PARAM_NONE,0)) return FALSE;
          if ((ParaFrame[CurParaFID].flags&PARA_FRAME_HPAGE) && !WriteRtfControl(w,rtf,"phpg",PARAM_NONE,0)) return FALSE;
          if (!WriteRtfControl(w,rtf,"posx",PARAM_INT,ParaFrame[CurParaFID].x)) return FALSE;
          if (!WriteRtfControl(w,rtf,"posy",PARAM_INT,ParaFrame[CurParaFID].ParaY)) return FALSE;
          if (!WriteRtfControl(w,rtf,"absh",PARAM_INT,ParaFrame[CurParaFID].MinHeight)) return FALSE;
          if (!WriteRtfControl(w,rtf,"absw",PARAM_INT,ParaFrame[CurParaFID].width)) return FALSE;
          if (!WriteRtfControl(w,rtf,"dxfrtext",PARAM_INT,ParaFrame[CurParaFID].DistFromText)) return FALSE;
          if (!WriteRtfControl(w,rtf,"dfrmtxtx",PARAM_INT,ParaFrame[CurParaFID].DistFromText)) return FALSE;
          if (!WriteRtfControl(w,rtf,"dfrmtxty",PARAM_INT,0)) return FALSE;
       }
    }
*/
    // compare and write indents
    if (CurFirstIndent!=PrevFirstIndent&&CurFirstIndent>=0) {
       if (!WriteRtfControl(rtf,"fi",PARAM_INT,CurFirstIndent)) return FALSE;
    }
    if (CurLeftIndent!=PrevLeftIndent&&CurLeftIndent>=0) {
       if (!WriteRtfControl(rtf,"li",PARAM_INT,CurLeftIndent)) return FALSE;
    }
    if (CurRightIndent!=PrevRightIndent&&CurRightIndent>=0) {
       if (!WriteRtfControl(rtf,"ri",PARAM_INT,CurRightIndent)) return FALSE;
    }
    if (CurSpaceBefore!=PrevSpaceBefore&&CurSpaceBefore>=0) {
       if (!WriteRtfControl(rtf,"sb",PARAM_INT,CurSpaceBefore)) return FALSE;
    }
    if (CurSpaceAfter!=PrevSpaceAfter&&CurSpaceAfter>=0) {
       if (!WriteRtfControl(rtf,"sa",PARAM_INT,CurSpaceAfter)) return FALSE;
    }
    if (CurSpaceBetween!=PrevSpaceBetween&&CurSpaceBetween>=0) {
       if (!WriteRtfControl(rtf,"sl",PARAM_INT,CurSpaceBetween)) return FALSE;
    }

    // compare and write justification
    if (CurAlignment!=PrevAlignment)
	{
	if (CurAlignment==TP_RIGHT_ALLIGN){
          result=WriteRtfControl(rtf,"qr",PARAM_NONE,0);
          if (!result) return FALSE;
    }
    else if (CurAlignment==TP_CENTER){
          result=WriteRtfControl(rtf,"qc",PARAM_NONE,0);
          if (!result) return FALSE;
    }
    else if (CurAlignment==(TP_LEFT_ALLIGN|TP_RIGHT_ALLIGN) ){
          result=WriteRtfControl(rtf,"qj",PARAM_NONE,0);
          if (!result) return FALSE;
    }
    else if (CurAlignment==TP_LEFT_ALLIGN){
          result=WriteRtfControl(rtf,"ql",PARAM_NONE,0);
          if (!result) return FALSE;
    }
	}
    // compare and write double spacing
    if (PrevSpaceBetweenMult!=CurSpaceBetweenMult/* && CurSpaceBetween==PrevSpaceBetween*/) {
       if (CurSpaceBetweenMult)   {
//           result=WriteRtfControl(w,rtf,"sl",PARAM_INT,480);
           result=WriteRtfControl(rtf,"slmult",PARAM_INT,1);
       }
       else
		   result=WriteRtfControl(rtf,"sl",PARAM_INT,0);
       if (!result) return FALSE;
    }

    // compare and write para keep together/next controls
    if ((CurParaKeep&ED_PARA_KEEP) && !(PrevParaKeep&ED_PARA_KEEP)) {
        result=WriteRtfControl(rtf,"keep",PARAM_NONE,0);
    }
    if ((CurParaKeep&ED_PARA_KEEP_NEXT) && !(PrevParaKeep&ED_PARA_KEEP_NEXT)) {
        result=WriteRtfControl(rtf,"keepn",PARAM_NONE,0);
    }


    // compare and write shading
    if (CurShading!=PrevShading&&CurShading>=0) {
       if (!WriteRtfControl(rtf,"shading",PARAM_INT,CurShading)) return FALSE;
    }

	rtf->reset=FALSE;
    return TRUE;
}

/*****************************************************************************
    WriteRtfText:
    Write the rtf text to output.  This routine scans the text stream. If
    any special character are found, they are preceede with a '\' character.
******************************************************************************/
Bool WriteRtfText(struct StrRtfOut far *rtf, const char* text, int TextLen)
{
    int i;
    uchar CurChar;
    Bool IgnoreSlash=rtf->flags&ROFLAG_IGNORE_SLASH;

    if (TextLen==0) return TRUE;

    for (i=0;i<TextLen;i++) {
        CurChar=text[i];
        if ((CurChar=='\\' && !IgnoreSlash) || CurChar=='{' || CurChar=='}' /*|| CurChar==NBSPACE_CHAR*/)
		{
            if (!FlushRtfLine(rtf)) return FALSE;    // ensure that prefix and character are in one line
            if (!PutRtfChar(rtf,'\\')) return FALSE; // write prefix
        }
/*        if (CurChar==TAB) {
            if (!WriteRtfControl(rtf,"tab",PARAM_NONE,0)) return FALSE;
        }
        else if (CurChar==NBSPACE_CHAR) {
            if (!PutRtfChar(w,rtf,'~')) return FALSE; // write non-break char suffix
        }
        else if (CurChar==PAGE_CHAR) {
            if (!WriteRtfControl(rtf,"page",PARAM_NONE,0)) return FALSE;
        }
        else*/ if (!PutRtfChar(rtf,CurChar)) return FALSE;  // write character
    }
    return TRUE;
}
/*****************************************************************************
    WriteRtfRow:
    Write the table row information to the file
******************************************************************************/
Bool WriteRtfRow(struct StrRtfOut far *rtf, CEDParagraph* NewCell,CEDParagraph * prevRow)
{
    Bool result;
    CEDParagraph*  CellId,*PrevCellId;
	int c;

	EDROWDESCR* td=(EDROWDESCR*)NewCell->descriptor;
	EDROWDESCR* prvtd=prevRow?(EDROWDESCR*)prevRow->descriptor:0;

//   	int left,heig,lb,rb,tb,bb,gp,pos;
//	CED_GetTableRowParams(tblRow,&left,&heig,&lb,0,&rb,0,&tb,0,&bb,0,&gp,&pos,0);
//	fprintf(stream,"\nRow: left=%d,heig=%d,lb=%d,rb=%d,tb=%d,bb=%d,gp=%d,pos=%d",left,heig,lb,rb,tb,bb,gp,pos);

//	CurCellId=NewCell;
//    CurRowId=cell[CurCellId].row;

    // check if row formatting is identical to the previous row
//    PrevRowId=TableRow[CurRowId].PrevRow;
    if ( prvtd==0
      || prvtd->gaph!=td->gaph
      || prvtd->left!=td->left
      || prvtd->numOfCells!=td->numOfCells
      || prvtd->position!=td->position
      || prvtd->rowHeight!=td->rowHeight
      || prvtd->leftBrdrType!=td->leftBrdrType
      || prvtd->leftBrdrWidth!=td->leftBrdrWidth
      || prvtd->rightBrdrType!=td->rightBrdrType
      || prvtd->rightBrdrWidth!=td->rightBrdrWidth
      || prvtd->topBrdrType!=td->topBrdrType
      || prvtd->topBrdrWidth!=td->topBrdrWidth
      || prvtd->bottomBrdrType!=td->bottomBrdrType
      || prvtd->bottomBrdrWidth!=td->bottomBrdrWidth
	)
	  goto WRITE_ROW;

	CellId=NewCell->GetCell(0);        // now check for cell formatting
	PrevCellId=prevRow->GetCell(0);

    for (c=0;CellId;c++)
	{
		if (!PrevCellId) goto WRITE_ROW;  // prev row had less number of cells
		EDCELLDESCR* cd=(EDCELLDESCR*)CellId->descriptor;
		EDCELLDESCR* prvcd=(EDCELLDESCR*)PrevCellId->descriptor;

		if ( cd->merging!=prvcd->merging
         || prvcd->cellX!=cd->cellX
         || prvcd->leftBrdrType!=cd->leftBrdrType
         || prvcd->leftBrdrWidth!=cd->leftBrdrWidth
         || prvcd->bottomBrdrType!=cd->bottomBrdrType
         || prvcd->bottomBrdrWidth!=cd->bottomBrdrWidth
         || prvcd->rightBrdrType!=cd->rightBrdrType
         || prvcd->rightBrdrWidth!=cd->rightBrdrWidth
         || prvcd->topBrdrType!=cd->topBrdrType
         || prvcd->vertTextAlign!=cd->vertTextAlign
         || prvcd->flag!=cd->flag
		 ) goto WRITE_ROW;

		c++;
		CellId=NewCell->GetCell(c);        // now check for cell formatting
		PrevCellId=prevRow->GetCell(c);
    }
    if (PrevCellId) goto WRITE_ROW;   // prev row had additional cells

    rtf->RtfInTable=TRUE;   // to ensure that table is terminated properly.
    return TRUE;       // the current row has identical properties as the previous row


    WRITE_ROW:

    if (!WriteRtfControl(rtf,"trowd",PARAM_NONE,0)) return FALSE;

    // write table data
    result=TRUE;
    if      (td->position==TP_CENTER)        result=WriteRtfControl(rtf,"trqc",PARAM_NONE,0);
    else if (td->position==TP_RIGHT_ALLIGN) result=WriteRtfControl(rtf,"trqr",PARAM_NONE,0);
    if (!result) return result;

    if (td->gaph>=0)
		if (!WriteRtfControl(rtf,"trgaph",PARAM_INT,td->gaph)) return FALSE;
    if (td->left>=0)
		if (!WriteRtfControl(rtf,"trleft",PARAM_INT,td->left)) return FALSE;

    if (td->rowHeight!=0)
		if (!WriteRtfControl(rtf,"trrh",PARAM_INT,td->rowHeight)) return FALSE;

    if (td->header && !WriteRtfControl(rtf,"trhdr",PARAM_NONE,0)) return FALSE;

    // write border
//dont write borders, since they overlap with information about cell boundaries. even in a quite strange manner.
//(if we put all boundaries near every line, they will appear near table only
/*    if (td->topBrdrType>0) {
       if (!WriteRtfControl(rtf,"trbrdrt",PARAM_NONE,0)) return FALSE;
	   if(td->topBrdrType==1)
		if (!WriteRtfControl(rtf,"brdrs",PARAM_NONE,0)) return FALSE;
       if (td->topBrdrWidth>=0)
	   {
		if (!WriteRtfControl(rtf,"brdrw",PARAM_INT,td->topBrdrWidth)) return FALSE;
	   }
	   else
		if (!WriteRtfControl(rtf,"brdrw",PARAM_INT,4)) return FALSE;
    }
    if (td->bottomBrdrType>0) {
       if (!WriteRtfControl(rtf,"trbrdrb",PARAM_NONE,0)) return FALSE;
	   if(td->bottomBrdrType==1)
		if (!WriteRtfControl(rtf,"brdrs",PARAM_NONE,0)) return FALSE;
       if (td->bottomBrdrWidth>=0)
	   {
		if (!WriteRtfControl(rtf,"brdrw",PARAM_INT,td->bottomBrdrWidth)) return FALSE;
	   }
	   else
		if (!WriteRtfControl(rtf,"brdrw",PARAM_INT,4)) return FALSE;
    }
    if (td->leftBrdrType>0) {
       if (!WriteRtfControl(rtf,"trbrdrl",PARAM_NONE,0)) return FALSE;
	   if(td->leftBrdrType==1)
		if (!WriteRtfControl(rtf,"brdrs",PARAM_NONE,0)) return FALSE;
       if (td->leftBrdrWidth>=0)
	   {
		if (!WriteRtfControl(rtf,"brdrw",PARAM_INT,td->leftBrdrWidth)) return FALSE;
	   }
	   else
		if (!WriteRtfControl(rtf,"brdrw",PARAM_INT,4)) return FALSE;
    }
    if (td->rightBrdrType>0) {
       if (!WriteRtfControl(rtf,"trbrdrr",PARAM_NONE,0)) return FALSE;
	   if(td->rightBrdrType==1)
		if (!WriteRtfControl(rtf,"brdrs",PARAM_NONE,0)) return FALSE;
       if (td->rightBrdrWidth>=0)
	   {
		if (!WriteRtfControl(rtf,"brdrw",PARAM_INT,td->rightBrdrWidth)) return FALSE;
	   }
	   else
		if (!WriteRtfControl(rtf,"brdrw",PARAM_INT,4)) return FALSE;
    }
*/
    // write the cell information
    for (int cl=0;cl<td->numOfCells;cl++)
	{
		CellId=NewCell->GetCell(cl);
        WriteRtfCell(rtf,CellId);
    }

    rtf->RtfInTable=TRUE;   // to ensure that table is terminated properly.

    return TRUE;
}

/*****************************************************************************
    WriteRtfCell:
    Write the table cell information to the file
******************************************************************************/
Bool WriteRtfCell(struct StrRtfOut far *rtf, CEDParagraph* NewCell)
{
	EDCELLDESCR* cd=(EDCELLDESCR*)NewCell->descriptor;

	//write merging
	if (cd->merging==ED_CELL_MRGF && !WriteRtfControl(rtf,"clvmgf",PARAM_NONE,0)) return FALSE;
	else if (cd->merging==ED_CELL_MRG && !WriteRtfControl(rtf,"clvmrg",PARAM_NONE,0)) return FALSE;

	//write vert alignment
	if (cd->vertTextAlign==1&&!WriteRtfControl(rtf,"clvertalc",PARAM_NONE,0)) return FALSE;
	else if (cd->vertTextAlign==2&&!WriteRtfControl(rtf,"clvertalb",PARAM_NONE,0)) return FALSE;

    // write border
	if (cd->topBrdrType) {
		if (!WriteRtfControl(rtf,"clbrdrt",PARAM_NONE,0)) return FALSE;
		if (!WriteRtfControl(rtf,GetLiteralBorderType(cd->topBrdrType),PARAM_NONE,0)) return FALSE;
		if (cd->topBrdrWidth>=0)
		{
			if (!WriteRtfControl(rtf,"brdrw",PARAM_INT,cd->topBrdrWidth)) return FALSE;
		}
	    else
			if (!WriteRtfControl(rtf,"brdrw",PARAM_INT,4)) return FALSE;
    }
	if (cd->bottomBrdrType>0) {
       if (!WriteRtfControl(rtf,"clbrdrb",PARAM_NONE,0)) return FALSE;
		if (!WriteRtfControl(rtf,GetLiteralBorderType(cd->bottomBrdrType),PARAM_NONE,0)) return FALSE;
		if (cd->bottomBrdrWidth>=0)
		{
			if (!WriteRtfControl(rtf,"brdrw",PARAM_INT,cd->bottomBrdrWidth)) return FALSE;
		}
		else
			if (!WriteRtfControl(rtf,"brdrw",PARAM_INT,4)) return FALSE;
    }
	if (cd->leftBrdrType>0) {
       if (!WriteRtfControl(rtf,"clbrdrl",PARAM_NONE,0)) return FALSE;
		if (!WriteRtfControl(rtf,GetLiteralBorderType(cd->leftBrdrType),PARAM_NONE,0)) return FALSE;
		if (cd->leftBrdrWidth>=0)
		{
			if (!WriteRtfControl(rtf,"brdrw",PARAM_INT,cd->leftBrdrWidth)) return FALSE;
		}
		else
			if (!WriteRtfControl(rtf,"brdrw",PARAM_INT,4)) return FALSE;
    }
	if (cd->rightBrdrType>0) {
       if (!WriteRtfControl(rtf,"clbrdrr",PARAM_NONE,0)) return FALSE;
		if (!WriteRtfControl(rtf,GetLiteralBorderType(cd->rightBrdrType),PARAM_NONE,0)) return FALSE;
		if (cd->rightBrdrWidth>=0)
		{
			if (!WriteRtfControl(rtf,"brdrw",PARAM_INT,cd->rightBrdrWidth)) return FALSE;
		}
		else
			if (!WriteRtfControl(rtf,"brdrw",PARAM_INT,4)) return FALSE;
    }

    // write shading percentage
    if (cd->shading>=0 && !WriteRtfControl(rtf,"clshdng",PARAM_INT,cd->shading)) return FALSE;

	//write direction of text
	if (cd->flag&ED_TDIR_UP&& !WriteRtfControl(rtf,"cltxbtlr",PARAM_NONE,0)) return FALSE;
	if (cd->flag&ED_TDIR_DOWN&& !WriteRtfControl(rtf,"cltxtbrl",PARAM_NONE,0)) return FALSE;

    // write ending x position
    if (!WriteRtfControl(rtf,"cellx",PARAM_INT,cd->cellX)) return FALSE;

    return TRUE;
}

/*****************************************************************************
    WriteFrmPos:
    Write the frame position information to the file
******************************************************************************/
Bool WriteFrmPos(struct StrRtfOut far *rtf, CEDParagraph* frm,Bool writeWidth)
{
	EDFRAMEDESCR* fd=(EDFRAMEDESCR*)(frm->descriptor);
	EDBOX bx=fd->rec;
			  // write the additional para border information
	if (fd->borderSpace>=0)
		if (!WriteRtfControl(rtf,"brsp",PARAM_INT,fd->borderSpace)) return FALSE;

  // write the frame information
	if      ((fd->position&0xf)==VPOS_PAGE && !WriteRtfControl(rtf,"pvpg",PARAM_NONE,0)) return FALSE;
	else if ((fd->position&0xf)==VPOS_MRG && !WriteRtfControl(rtf,"pvmrg",PARAM_NONE,0)) return FALSE;
	else if ((fd->position&0xf)==VPOS_PARA && !WriteRtfControl(rtf,"pvpara",PARAM_NONE,0)) return FALSE;

	if ((fd->position&0xf0)==HPOS_MRG && !WriteRtfControl(rtf,"phmrg",PARAM_NONE,0)) return FALSE;
	else if ((fd->position&0xf0)==HPOS_PAGE && !WriteRtfControl(rtf,"phpg",PARAM_NONE,0)) return FALSE;
	else if ((fd->position&0xf0)==HPOS_COL && !WriteRtfControl(rtf,"phcol",PARAM_NONE,0)) return FALSE;

	if (bx.x!=0)
		if (!WriteRtfControl(rtf,"posx",PARAM_INT,bx.x)) return FALSE;
	if (bx.y!=0)
		if (!WriteRtfControl(rtf,"posy",PARAM_INT,bx.y)) return FALSE;

	if (writeWidth)
	{
		if (bx.h!=0)
			if (!WriteRtfControl(rtf,"absh",PARAM_INT,bx.h)) return FALSE;
		if (bx.w!=0)
			if (!WriteRtfControl(rtf,"absw",PARAM_INT,bx.w)) return FALSE;
	}
	if (fd->dxfrtextx>=0)
	{
		if (!WriteRtfControl(rtf,"dxfrtext",PARAM_INT,fd->dxfrtextx)) return FALSE;
		if (!WriteRtfControl(rtf,"dfrmtxtx",PARAM_INT,fd->dxfrtextx)) return FALSE;
	}
	if (fd->dxfrtexty>=0)
		if (!WriteRtfControl(rtf,"dfrmtxty",PARAM_INT,fd->dxfrtexty)) return FALSE;
	if (fd->flag&ED_DROPCAP)
	{
		if (!WriteRtfControl(rtf,"dropcapli",PARAM_INT,3)) return FALSE;
		if (!WriteRtfControl(rtf,"dropcapt",PARAM_INT,1)) return FALSE;
	}
	return TRUE;
}

/*****************************************************************************
    WriteRtfDIB:
    Write the device independent bitmap group and picture data for the
    specified picture. When the DIB_TO_METAFILE global constant is defined
    (see ter_def.h), the function imbeds the DIB into a metafile, and
    then writes the metafile to the rtf file.
******************************************************************************/
Bool WriteRtfDIB(struct StrRtfOut far *rtf,int pict)
{
   Bool result=TRUE;

   // Write the actual DIB to the rtf file

   long height,width,width_bytes;
   uchar *pMem;
   LPBITMAPINFO pInfo;

   // get picture height/width
   height=rtf->page->picsTable[pict].pictGoal.cy;             // picture height in pointsize
   width=rtf->page->picsTable[pict].pictGoal.cx;               // picture width in pointsize
   pInfo=(LPBITMAPINFO)rtf->page->picsTable[pict].data;

   width_bytes = ((width * pInfo->bmiHeader.biBitCount + 31) / 32) * 4;

    if (!BeginRtfGroup(rtf)) return FALSE;     // begin current picture
   if (!WriteRtfControl(rtf,"pict",PARAM_NONE,0)) return FALSE;     // write picture group control
   if (!WriteRtfControl(rtf,"dibitmap",PARAM_INT,0)) return FALSE;  // write picture format

   if (!WriteRtfControl(rtf,"wbmbitspixel",PARAM_INT,pInfo->bmiHeader.biBitCount)) return FALSE;  // bit count
 if (!WriteRtfControl(rtf,"wbmplanes",PARAM_INT,1)) return FALSE;  // Number of bitmap color planes (must equal 1).
 if (!WriteRtfControl(rtf,"wbmwidthbytes",PARAM_INT,width_bytes)) return FALSE;  // write picture format


   // write picture height/width in HIMETRIC
   if (!WriteRtfControl(rtf,"picw",PARAM_INT,rtf->page->picsTable[pict].pictSize.cx)) return FALSE;  // write picture format
   if (!WriteRtfControl(rtf,"pich",PARAM_INT,rtf->page->picsTable[pict].pictSize.cy)) return FALSE;  // write picture format

   // write picture height/width in twips
   if (!WriteRtfControl(rtf,"picwgoal",PARAM_INT,(int)width )) return FALSE;  // write picture format
   if (!WriteRtfControl(rtf,"pichgoal",PARAM_INT,(int)height )) return FALSE;  // write picture format

   // write picture alignment
   if (!WriteRtfControl(rtf,"sspicalign",PARAM_INT,(int)(rtf->page->picsTable[pict].pictAlign) )) return FALSE;  // write picture format

  pMem=(uchar *)rtf->page->picsTable[pict].data;
   // write the picture information
   for (uint32_t l(0); l < rtf->page->picsTable[pict].len; ++l) {
      if (!(result=PutRtfHexChar(rtf,pMem[l]))) break;
   }

   if (!EndRtfGroup(rtf)) return FALSE;       // end current picture
   return result;

}
/*****************************************************************************
    PutRtfHexChar:
    Write a character to rtf output in hex format
******************************************************************************/
Bool PutRtfHexChar(struct StrRtfOut far *rtf,uchar CurChar)
{
    uchar HiChar,LoChar,HexChar;
    int SaveFlag;

    HiChar=(char)((CurChar&0xF0)>>4);
    LoChar=(char)(CurChar&0x0F);

    // write hi char
    if (HiChar<=9) HexChar=(char)('0'+HiChar);
    else           HexChar=(char)('a'+HiChar-10);

    if (!PutRtfChar(rtf,HexChar)) return FALSE;

    // write lo char
    if (LoChar<=9) HexChar=(char)('0'+LoChar);
    else           HexChar=(char)('a'+LoChar-10);

    SaveFlag=rtf->WritingControl;
    rtf->WritingControl=TRUE;      // disable line break

    if (!PutRtfChar(rtf,HexChar)) return FALSE;

    rtf->WritingControl=SaveFlag;  // restore the flag status

    return TRUE;
}

/*****************************************************************************
    WriteRtfMetafile:
    Write the metafile group and picture data for the specified picture.
******************************************************************************/
Bool WriteRtfMetafile(struct StrRtfOut far *rtf,int pict)
{
   long l;//,bmHeight,bmWidth;
//   HGLOBAL hMem;
   uchar *pMem;
   Bool result=TRUE;

   // get picture height/width
//   bmHeight=ScrToTwipsY(TerFont[pict].bmHeight);  // picture height in twips
//   bmWidth=ScrToTwipsX(TerFont[pict].bmWidth);    // picture width in twips

   if (!BeginRtfGroup(rtf)) return FALSE;     // begin current picture
   if (!WriteRtfControl(rtf,"pict",PARAM_NONE,0)) return FALSE;      // write picture group control
   if (!WriteRtfControl(rtf,"wmetafile",PARAM_INT,8)) return FALSE;  // write picture format

   // write picture height/width in HIMETRIC
//   if (!WriteRtfControl(rtf,"picw",PARAM_INT,((float)rtf->page->picsTable[pict].pictSize.cx)*8.3))/*((bmWidth*2500)/1440) ))*/ return FALSE;  // write picture format
//   if (!WriteRtfControl(rtf,"pich",PARAM_INT,((float)rtf->page->picsTable[pict].pictSize.cy)*8.3))/*((bmHeight*2500)/1440) ))*/ return FALSE;  // write picture format
//this seems to be more or less correct
   if (!WriteRtfControl(rtf,"picw",PARAM_INT,((double)(rtf->page->picsTable[pict].pictGoal.cx))*1.7641 )) return FALSE;  // write picture format
   if (!WriteRtfControl(rtf,"pich",PARAM_INT,((double)(rtf->page->picsTable[pict].pictGoal.cy))*1.7641 )) return FALSE;  // write picture format

   // write picture width
//   if (bmWidth>0) {
      if (!WriteRtfControl(rtf,"picwgoal",PARAM_INT,rtf->page->picsTable[pict].pictGoal.cx)) return FALSE;  // write picture format
      //if (!WriteRtfControl(w,rtf,"picwgoal",PARAM_INT,(int)(bmWidth) )) return FALSE;  // write picture format
      //if (!WriteRtfControl(w,rtf,"picscalex",PARAM_INT,(int)(((long)TerFont[pict].PictWidth*100*20)/bmWidth))) return FALSE;
//   }
/*   else {
      if (!WriteRtfControl(rtf,"picwgoal",PARAM_INT,(int)(TerFont[pict].PictWidth*20) )) return FALSE;  // write picture format
      if (!WriteRtfControl(rtf,"picscalex",PARAM_INT,100)) return FALSE;  // write picture format
   }
*/
   // write picture height
//   if (bmHeight>0) {
      if (!WriteRtfControl(rtf,"pichgoal",PARAM_INT,rtf->page->picsTable[pict].pictGoal.cy)) return FALSE;  // write picture format
      //if (!WriteRtfControl(w,rtf,"pichgoal",PARAM_INT,(int)(bmHeight) )) return FALSE;  // write picture format
      //if (!WriteRtfControl(w,rtf,"picscaley",PARAM_INT,(int)(((long)TerFont[pict].PictHeight*100*20)/bmHeight))) return FALSE;
//   }
/*   else {
      if (!WriteRtfControl(rtf,"pichgoal",PARAM_INT,(int)(TerFont[pict].PictHeight*20))) return FALSE;  // write picture format
      if (!WriteRtfControl(rtf,"picscaley",PARAM_INT,100)) return FALSE;
   }
*/

   // write picture alignment
   if (!WriteRtfControl(rtf,"sspicalign",PARAM_INT,rtf->page->picsTable[pict].pictAlign )) return FALSE;  // write picture format


   // retrieve picture handle
	pMem=(uchar *)rtf->page->picsTable[pict].data;

	for (l=0;l<(long)rtf->page->picsTable[pict].len;l++) {
      result=PutRtfHexChar(rtf,pMem[l]);
      if (!result) break;
   }

   // recreate the metafile, this calls unlocks and releases the buffer
//   GlobalUnlock(hMem);
//   if ( NULL==(TerFont[pict].hMeta=TerSetMetaFileBits(hMem)) ){
//      return PrintError(w,MSG_ERR_META_RECREATE,"WriteRtfMetafile");
//   }

   if (!EndRtfGroup(rtf)) return FALSE;       // end current picture
   return result;
}

int ReadRtfFontTable(struct StrRtfOut far *rtf, int * maxFontNum);
int ReadRtfColorTable(struct StrRtfOut far *rtf);
Bool WriteRtfMergedHeader(struct StrRtfOut far *rtf, const char * name)
{
	int oldIndex;
	Bool head;
	char * ptr=rtf->oldFile;
	int len=rtf->oldFileLen;
	int i;
	for (i=0;i<len-(int)strlen("\\fonttbl");i++)
		if (memcmp(ptr+i,"\\fonttbl",strlen("\\fonttbl"))==0)
			if (i==0||ptr[i-1]!='\\')
				break;
	if (i==len-(int)strlen("\\fonttbl"))
	{
		SetReturnCode_ced(IDS_ERR_NOT_RTF);
		return FALSE;
	}
	i+=strlen("\\fonttbl");
	rtf->TextIndex=i;
	int m;
	if (ReadRtfFontTable(rtf,&m))
	{
		SetReturnCode_ced(IDS_ERR_NOT_RTF);
		return FALSE;
	}
//	if (m=-1) m=0;
	rtf->maxFntNum=m;
	m++;
	for (i=0;i<rtf->page->fontsUsed;i++)
	{
		if (rtf->table[i]==-1)
		{
			rtf->table[i]=m;
			m++;
		}
	}
    Bool ret=TRUE;
	//remove old file - it is similar to rtf
	rtf->hFile=Open(0, (pchar)name,OSF_CREATE|OSF_BINARY);
	if (!rtf->hFile) goto END_HDR;

	if (HFILE_ERROR==(HFILE)Write(rtf->hFile,(char*)rtf->oldFile,rtf->TextIndex-1)) goto END_HDR;
	if (!WriteRtfFont(rtf,FALSE)) goto END_HDR;
    if (!FlushRtfLine(rtf)) goto END_HDR;    // flush the rtf line to the output

	oldIndex=rtf->TextIndex;

	for (i=rtf->TextIndex-1;i<len-(int)strlen("\\colortbl");i++)
		if (memcmp(ptr+i,"\\colortbl",strlen("\\colortbl"))==0)
			if (i==0||ptr[i-1]!='\\')
				break;
	if (i!=len-(int)strlen("\\colortbl"))
	{
		i+=strlen("\\colortbl");
		while (ptr[i]==' ')
			i++;
		rtf->TextIndex=i;
		if (ReadRtfColorTable(rtf))
		{
			SetReturnCode_ced(IDS_ERR_NOT_RTF);
			return FALSE;
		}
		head=FALSE;
	}
	else
	{
		rtf->TextIndex++;
		head=TRUE;
	}
    if (HFILE_ERROR==(HFILE)Write(rtf->hFile,((char*)rtf->oldFile)+oldIndex-1,rtf->TextIndex-oldIndex)) goto END_HDR;
	if (!WriteRtfColor(rtf,head)) goto END_HDR;
    if (!FlushRtfLine(rtf)) goto END_HDR;    // flush the rtf line to the output

    if (HFILE_ERROR==(HFILE)Write(rtf->hFile,((char*)rtf->oldFile)+rtf->TextIndex-1,rtf->oldFileLen-(rtf->TextIndex+2))) goto END_HDR;

	// end the previous section
	if (!WriteRtfControl(rtf,"sect",PARAM_NONE,0)) goto END_HDR;
	if (!WriteRtfControl(rtf,"plain",PARAM_NONE,0)) goto END_HDR;
	if (!WriteRtfControl(rtf,"fs",PARAM_INT,24)) goto END_HDR;
	//in order to draw "}" at the end
	rtf->GroupLevel=1;
	rtf->SpacePending=FALSE;          // delimit the last control by '{' character

	goto OK;
END_HDR:
	SetReturnCode_ced(CFIO_GetReturnCode());
	ret=FALSE;
OK:
	return ret;
}

Bool GetRtfWord(struct StrRtfOut far *rtf);
int nameCmp(char* s1,char* s2);
void StrTrim(char* string);
/******************************************************************************
    ReadRtfFontTable:
    Read the RTF font table.  This function return 0 when sucessful, otherwise
    it returns an error code (RTF_FILE_INCOMPLETE or RTF_SYNTAX_ERROR)
******************************************************************************/
int ReadRtfFontTable(struct StrRtfOut far *rtf,int * maxFontNum)//PTERWND w,struct StrRtf far *rtf)
{
    struct StrRtfFont font;
    int    len,FontId,CurFont,ControlGroupLevel=0,CurGroupLevel,WordLen;
    Bool   ExtractingName,NameWordRead;
	int maxNum=-1;
//    font=rtf->font;
    ControlGroupLevel=rtf->GroupLevel=2;
	memset(&font,0,sizeof(font));
	font.FontId=-1;
    while (TRUE) {
       // get font number
       if (!GetRtfWord(rtf)) return RTF_FILE_INCOMPLETE;

       if (rtf->GroupBegin) continue;
       if (rtf->GroupEnd) {
          if (rtf->GroupLevel<ControlGroupLevel)
		  {
				if(	maxNum<font.FontId) maxNum=font.FontId;
				for (int i=0;i<rtf->page->fontsUsed;i++)
				{
					if (rtf->page->fontTable[i].fontCharset==font.CharSet
						&&nameCmp(rtf->page->fontTable[i].fontName,font.name)==0)
					rtf->table[i]=font.FontId;
				}
				break;    // font group ended
		  }
          else                                   continue; // look for next font
       }

       if (rtf->GroupLevel==ControlGroupLevel) continue;   // ignore anything between the fonts

       if (font.FontId!=-1)
	   {
			if(	maxNum<font.FontId) maxNum=font.FontId;
			for (int i=0;i<rtf->page->fontsUsed;i++)
			{
				if (nameCmp(rtf->page->fontTable[i].fontName,font.name)==0&&rtf->page->fontTable[i].fontCharset==font.CharSet)
				rtf->table[i]=font.FontId;
			}
	   }
	   if (lstrcmpi(rtf->CurWord,"f")!=0 || !rtf->IsControlWord) return RTF_SYNTAX_ERROR;
       FontId=(int)(rtf->IntParam);

       // get the rtf font table slot
/*       if (FontId<MAX_RTF_SEQ_FONTS)*/ CurFont=FontId;  // lower numbers are stored sequentially
/*       else {
          if (MaxRtfFonts<MAX_RTF_SEQ_FONTS) CurFont=MAX_RTF_SEQ_FONTS;
          else for (CurFont=MAX_RTF_SEQ_FONTS;CurFont<MaxRtfFonts;CurFont++) if (!(font[CurFont].InUse)) break;
       }
*/
       if (CurFont<0/* || CurFont>=MAX_RTF_FONTS*/) return RTF_SYNTAX_ERROR;

/*       if (CurFont>=MaxRtfFonts) {    // allocate
          int NewMaxRtfFonts=CurFont+50;
          if (NewMaxRtfFonts>=MAX_RTF_FONTS) NewMaxRtfFonts=MAX_RTF_FONTS-1;
          if (NewMaxRtfFonts<(CurFont+1))    NewMaxRtfFonts=CurFont+1;

          if (NULL==(rtf->font=(struct StrRtfFont far *)OurRealloc(rtf->font,sizeof(struct StrRtfFont)*NewMaxRtfFonts))) {
             PrintError(w,MSG_OUT_OF_MEM,"ReadRtfFontTable");
             return RTF_SYNTAX_ERROR;
          }
          FarMemSet(&(rtf->font[MaxRtfFonts]),0,sizeof(struct StrRtfFont)*(NewMaxRtfFonts-MaxRtfFonts));  // initialize with zeros
          MaxRtfFonts=NewMaxRtfFonts;
       }
*/
       // initialize the rtf font table slot
//       font[CurFont].InUse=TRUE;
       font.FontId=FontId;
       if (lstrlen(font.family)>0) font.family[0]=0;  // erase previous specifiation if any
       if (lstrlen(font.name)>0)   font.name[0]=0;  // erase previous specifiation if any

       // get font family
       NameWordRead=FALSE;
       if (!GetRtfWord(rtf)) return RTF_FILE_INCOMPLETE;
       if (rtf->IsControlWord) {  // font family found
          if (rtf->WordLen<=1) return RTF_SYNTAX_ERROR;
          if (rtf->CurWord[0]!='f' && rtf->CurWord[0]!='F') return RTF_SYNTAX_ERROR;
          lstrcpy(font.family,&(rtf->CurWord[1])); //exclude first 'f' character
          StrTrim(font.family);
       }
       else NameWordRead=TRUE;  // first word of the font name read

       // get font name
       CurGroupLevel=rtf->GroupLevel;
       ExtractingName=TRUE;

       while (TRUE) {           // get each name framgment
         if (!NameWordRead && !GetRtfWord(rtf)) return RTF_FILE_INCOMPLETE;
         NameWordRead=FALSE;    // reset for the next read

         if (rtf->GroupEnd && rtf->GroupLevel<CurGroupLevel) {  // font name group ending without ';'
             StrTrim(font.name);
             break;
         }

         // stas !!! it can be PANSONE!!!
	 if (rtf->WordLen==0)    continue;
	 if ( lstrcmpi(rtf->CurWord,"panose") == 0) {
	    if ( ! GetRtfWord(rtf) ) return RTF_FILE_INCOMPLETE;
	    continue;
	 }
	 // Piter A
	 if ( lstrcmpi(rtf->CurWord,"fcharset") == 0) {
		 font.CharSet = rtf->IntParam;
	    continue;
	 }

         if (rtf->IsControlWord) {
            if (lstrcmpi(rtf->CurWord,"falt")==0) {
               font.name[0]=0;  // alternate font name supplied
               ExtractingName=TRUE;      // retextract the name
            }
            continue;          // ignore any other control word
         }

         if (ExtractingName) lstrcat(font.name,rtf->CurWord);
         WordLen=lstrlen(rtf->CurWord);

         if (WordLen>0 && rtf->CurWord[WordLen-1]==';') {
            len=lstrlen(font.name);
            if (ExtractingName && len>0) {
                font.name[len-1]=0; // exclude the semicolon
                StrTrim(font.name);
            }
            if (rtf->GroupLevel==CurGroupLevel) break; // complete name derived
            else ExtractingName=FALSE;     // name extracted
         }
       }
    }
	if (maxFontNum)
		*maxFontNum=maxNum;
    return 0;                 // successful
}

//compare fonts names
int nameCmp(char* s1,char* s2)
{
	if (strcmp(s1,s2)==0) return 0;
	if (strcmp(s1+strlen(s1)-4," Cyr")==0) s1[strlen(s1)-4]=0;
	if (strcmp(s2+strlen(s2)-4," Cyr")==0) s2[strlen(s2)-4]=0;
	if (strcmp(s1+strlen(s1)-3," CE")==0) s1[strlen(s1)-3]=0;
	if (strcmp(s2+strlen(s2)-3," CE")==0) s2[strlen(s2)-3]=0;
	if (strcmp(s1,s2)==0) return 0;
	return 1;
}

Bool GetRtfChar(struct StrRtfOut far *rtf);
Bool PushRtfChar(struct StrRtfOut far *rtf);
/*****************************************************************************
    GetRtfWord:
    Reads the next word from the input file or the input buffer.
******************************************************************************/
Bool GetRtfWord(struct StrRtfOut far *rtf)
{
    uchar CurChar,TempChar;
    uchar line[MAX_WIDTH+1];
	char TempString[MAX_WIDTH+1];
    int  len,i,j,k,temp,GroupLevel;
    Bool SpecialChar,NumParamFound,NumFound;
//    Bool IgnoreSlash=rtf->flags&RFLAG_IGNORE_SLASH;

    NEXT_WORD:

    if (rtf->eof) return FALSE; // end of file

    rtf->GroupBegin=rtf->GroupEnd=FALSE;
    rtf->IsControlWord=FALSE;
    rtf->CurWord[0]=0;
    rtf->WordLen=0;

    // Extract a word delimited by {, }, \ or a space.  If space
    // is the delimiter, it is included in the extracted word.  All other
    // delimiters are pushed back on the character stream.
    len=0;
    while (GetRtfChar(rtf)) {
       CurChar=rtf->CurChar;

       // check for delimiters before accepting the character
       if (len>0 && line[len-1]!='\\') {
          if (  CurChar=='{'
             || CurChar=='}'
             || CurChar=='\\' ){
             PushRtfChar(rtf);
             break;
          }
       }

       line[len]=CurChar;         // build string
       len++;
       // check for these delimiters after accepting the characters
       if (len==1) {              // first character
          if (CurChar=='{' || CurChar=='}') break;
       }
       else if (len==2 && line[len-2]=='\\' && line[len-1]=='\\') break;
       else if (CurChar==' ' ) break;

       if (len==MAX_WIDTH) break; // break this long word
    }
    if (rtf->eof) return FALSE;   // end of stream
    line[len]=0;

    // process group begin and end
    if (len==1 && line[0]=='{') { // new group

       if (rtf->GroupLevel>=MAX_RTF_GROUPS) return FALSE;//PrintError(w,MSG_OUT_OF_GROUP,"GetRtfWord");

       rtf->GroupBegin=TRUE;
       rtf->GroupLevel++;         // increase the group level
       return TRUE;
    }

    if (len==1 && line[0]=='}') {   // end group
       GroupLevel=rtf->GroupLevel;

       rtf->GroupEnd=TRUE;

       rtf->GroupLevel--;
       if (rtf->GroupLevel<=0) return FALSE;   // end of rtf data
       else                    return TRUE;
    }

    // Translate special characters
    SpecialChar=FALSE;
    j=0;
    for (i=0;i<len;i++) {
       CurChar=line[i];

       if (SpecialChar) {
          switch (CurChar) {
            case '\\':
            case '{':
            case '}':
              rtf->CurWord[j]=CurChar;
              j++;
              break;

            case '\'':      // hexadecimal value follows
              if (i+2<len) {
                 TempChar=(char)toupper(line[i+1]);      // convert to decimal
                 if (TempChar>='A') temp=10+TempChar-'A';
                 else               temp=TempChar-'0';
                 temp=temp<<4;
                 rtf->CurWord[j]=(char)temp;
                 TempChar=(char)toupper(line[i+2]);      // second byte
                 if (TempChar>='A') temp=10+TempChar-'A';
                 else               temp=TempChar-'0';
                 rtf->CurWord[j]+=(char)temp;

                 i=i+2;
                 j++;
              }
              break;

            case '|':       // special characters to be ignored, Formula character
              break;

            case '-':       // optional hyphen
            case '_':       // non-breaking hyphen
              rtf->CurWord[j]='-';
              j++;
              break;

            default:        // control word found
              if (rtf->IsControlWord) { // a control word already active
//                 wsprintf(NULL,line,"Cannot understand this RTF syntax!",MB_OK);
                 rtf->eof=TRUE;
                 return FALSE;
              }
              rtf->IsControlWord=TRUE;
              NumParamFound=FALSE;  // will be set to TRUE when a numeric parameter is found
              if (CurChar==10 || CurChar==13) {  // treat as paragraph
                 rtf->CurWord[j]='p';j++;
                 rtf->CurWord[j]='a';j++;
                 rtf->CurWord[j]='r';j++;
                 break;
              }
              else {
                rtf->CurWord[j]=CurChar;
                j++;
              }
          }
          SpecialChar=FALSE;//Turnoff special character flag
       }
       else {               // if not under special character
          if (CurChar=='\\' ) SpecialChar=TRUE;  // special character may follow
          else {            // ordinary character
             // check for the end of a control word
             if (rtf->IsControlWord) {
                // Handle cr/lf within control word
                if (CurChar==13) continue; // wait for CR
                if (CurChar==10) {
                      for (k=len-1;k>i;k--) {
                         rtf->CurChar=line[k];
                         PushRtfChar(rtf);
                      }
                      break;
                   }

                // check if current character is a number
                if (CurChar=='.' || CurChar=='-' || (CurChar>='0' && CurChar<='9'))
                      NumFound=TRUE;
                else  NumFound=FALSE;

                // terminate this control word, if the next one starts
                if (!NumFound && !isalpha(CurChar) && CurChar!=' ') {// control word ends without a numeric parameter
                   // Push remaining characters including the current character
                   for (k=len-1;k>=i;k--) {
                      rtf->CurChar=line[k];
                      PushRtfChar(rtf);
                   }
                   break;
                }
                else if (NumParamFound && !NumFound) {
                   // Push back every character after the last numeric character
                   for (k=len-1;k>=i;k--) {
                      rtf->CurChar=line[k];
                      PushRtfChar(rtf);
                   }
                   if (CurChar==' ') GetRtfChar(rtf);  // space is considered part of the control character
                   else {                   // next control word starts
                     rtf->CurChar='\\';     // begin next control word
                     PushRtfChar(rtf);
                   }
                   break;                   // break outof main loop
                }
                // check if numeric parameter for a control word is found
                if (NumFound) NumParamFound=TRUE;
             }

             // incorporate current character into current word
             if (CurChar != 10 &&  CurChar !=13) {
               rtf->CurWord[j]=CurChar;  // ordinary character
               j++;
             }
          }
       }
    }
    rtf->WordLen=len=j;          //current word length
    rtf->CurWord[len]=0;

    if (!(rtf->IsControlWord)) {
        if (rtf->WordLen>0) return TRUE;     // an oridnary word extracted
        else                goto NEXT_WORD;  // suppress zero length words
    }

    // Separate numeric parameter for the control word
    StrTrim(rtf->CurWord);
    for (i=0;i<len;i++) {
       CurChar=rtf->CurWord[i];
       if (CurChar=='.' || CurChar=='-' || (CurChar>='0' && CurChar<='9')) break;
    }

    if (i==0) {
        // wsprintf(NULL,rtf->CurWord,"Cannot understand this RTF control word!",MB_OK);
        rtf->eof=TRUE;
        return FALSE;
    }

    for (j=i;j<len&&j<MAX_WIDTH/3+i;j++) rtf->param[j-i]=rtf->CurWord[j];  // extrac param
    rtf->param[j-i<MAX_WIDTH/3-1?j-i:MAX_WIDTH/3-1]=0;
    rtf->CurWord[i]=0;
    rtf->WordLen=i;

    // convert to double numeric
    lstrcpy(TempString,rtf->param);              // copy to data segment
    if (lstrlen(TempString)>0) {
       rtf->IntParam=atoi(TempString);           // number in integer format
//       rtf->DoubleParam=strtod(TempString,NULL); // number in double format
    }
    else {
       rtf->IntParam=1;                         // a TRUE value
//       rtf->DoubleParam=1;
    }

    // increment number of control words in the group
//    (rtf->group[rtf->GroupLevel].ControlCount)++;

    return TRUE;
}
void lTrim(char* string);
void rTrim(char* string);
/*****************************************************************************
    StrTrim:
    Remove spaces from the left and right of a NULL terminated string.
******************************************************************************/
void StrTrim(char* string)
{
    rTrim(string);lTrim(string);
}
/******************************************************************************
    rTrim:
    Remove spaces on the right of a string.
*****************************************************************************/
void rTrim(char* string)
{
    int i,TempLen;

    TempLen=lstrlen(string);
    for (i=TempLen-1;i>=0 && string[i] == ' ';i--);
    string[i+1] = '\0';
}

/******************************************************************************
    lTrim:
    Trim initial spaces from the string.
*******************************************************************************/
void lTrim(char* string)
{
    int i,TempLen,BeginPoint;
	char TempStr[MAX_WIDTH];

    TempLen=lstrlen(string);
    for (i=0;i < TempLen && string[i] == ' ';i++);
    BeginPoint=i;
    for (i=BeginPoint;i<TempLen;i++) TempStr[i-BeginPoint]=string[i];
    TempStr[TempLen-BeginPoint] = '\0';
    lstrcpy(string,TempStr);
}
/*****************************************************************************
    GetRtfChar:
    Reads the next characters from the input file or the input buffer.
    When the InSteam is NULL, the characters are read from the buffer.
    Returns FALSE when no more characters available.
******************************************************************************/
Bool GetRtfChar(struct StrRtfOut far *rtf)
{
    // get the character from the character stack if available
    if (rtf->StackLen>0) {
       rtf->StackLen--;
       rtf->CurChar=rtf->stack[rtf->StackLen];
//       rtf->FilePos++;       // advance file position
       return TRUE;
    }

    // read from the file
    rtf->eof=FALSE;
//    if (rtf->hFile) {
       if (rtf->TextIndex<rtf->oldFileLen) {  // read from the temporary buffer
          rtf->CurChar=rtf->oldFile[rtf->TextIndex];
          (rtf->TextIndex)++;
//          rtf->FilePos++;    // advance file position
          return TRUE;
       }

       // read more characters from the file
//       rtf->TextLen=_lread(rtf->hFile,rtf->text,MAX_WIDTH);
//       if (rtf->TextLen==HFILE_ERROR) return PrintError(w,MSG_ERR_FILE_READ,"RTF input file.");

//       if (rtf->TextLen==0)
	   else
	   {
          rtf->eof=TRUE;
          return FALSE;      // end of file
       }
/*\       rtf->TextIndex=0;

       rtf->CurChar=rtf->text[rtf->TextIndex];
       (rtf->TextIndex)++;
       rtf->FilePos++;      // advance file position
       return TRUE;
    }
    else {              // read from the buffer
       if (rtf->BufLen>=0 && rtf->BufIndex>=rtf->BufLen) {
          rtf->eof=TRUE;
          return FALSE; // end of buffer
       }
       rtf->CurChar=rtf->buf[rtf->BufIndex];
       (rtf->BufIndex)++;
       rtf->FilePos++;  // advance file position
       return TRUE;
    }
*/}
/*****************************************************************************
    PushRtfChar:
    Push the lastly read character onto stack.
******************************************************************************/
Bool PushRtfChar(struct StrRtfOut far *rtf)
{
    if (rtf->StackLen>=MAX_WIDTH) return FALSE;//PrintError(w,MSG_OUT_OF_CHAR_STACK,"PushRtfChar");

    rtf->stack[rtf->StackLen]=rtf->CurChar;
    (rtf->StackLen)++;
//    rtf->FilePos--;  // decrement file position
    return TRUE;
}

/*****************************************************************************
    WriteRtfParaBorder:
    Write paragraph border controls
******************************************************************************/
Bool WriteRtfParaBorder(struct StrRtfOut far *rtf, CEDParagraph * para)
{
    if (para->leftBrdrType!=0&&
		para->leftBrdrType==para->rightBrdrType&&para->rightBrdrType==para->topBrdrType&&
		para->topBrdrType==para->bottomBrdrType
		&&para->leftBrdrWidth==para->rightBrdrWidth&&para->rightBrdrWidth==para->topBrdrWidth
		&&para->topBrdrWidth==para->bottomBrdrWidth)
	{
       if (!WriteRtfControl(rtf,"box",PARAM_NONE,0)) return FALSE;
       if (para->topBrdrWidth>=0)
	   {
		   if (!WriteRtfControl(rtf,"brdrw",PARAM_INT,para->topBrdrWidth)) return FALSE;
	   }
	   else
		   if (!WriteRtfControl(rtf,"brdrw",PARAM_INT,4)) return FALSE;

//       if (CurFlags&PARA_BOX_DOUBLE && !WriteRtfControl(w,rtf,"brdrdb",PARAM_NONE,0)) return FALSE;
       if (/*!(CurFlags&PARA_BOX_DOUBLE) &&*/ !WriteRtfControl(rtf,GetLiteralBorderType(para->leftBrdrType),PARAM_NONE,0)) return FALSE;
    }
    else {
       if (para->topBrdrType>0) {
          if (!WriteRtfControl(rtf,"brdrt",PARAM_NONE,0)) return FALSE;
       if (para->topBrdrWidth>=0)
	   {
		   if (!WriteRtfControl(rtf,"brdrw",PARAM_INT,para->topBrdrWidth)) return FALSE;
	   }
	   else
		   if (!WriteRtfControl(rtf,"brdrw",PARAM_INT,4)) return FALSE;
       if (!WriteRtfControl(rtf,GetLiteralBorderType(para->topBrdrType),PARAM_NONE,0)) return FALSE;
       }

       if (para->bottomBrdrType>0) {
          if (!WriteRtfControl(rtf,"brdrb",PARAM_NONE,0)) return FALSE;
       if (para->bottomBrdrWidth>=0)
	   {
		   if (!WriteRtfControl(rtf,"brdrw",PARAM_INT,para->topBrdrWidth)) return FALSE;
	   }
	   else
		   if (!WriteRtfControl(rtf,"brdrw",PARAM_INT,4)) return FALSE;
       if (!WriteRtfControl(rtf,GetLiteralBorderType(para->bottomBrdrType),PARAM_NONE,0)) return FALSE;
       }

	   if (para->leftBrdrType>0) {
          if (!WriteRtfControl(rtf,"brdrl",PARAM_NONE,0)) return FALSE;
       if (para->leftBrdrWidth>=0)
	   {
		   if (!WriteRtfControl(rtf,"brdrw",PARAM_INT,para->topBrdrWidth)) return FALSE;
	   }
	   else
		   if (!WriteRtfControl(rtf,"brdrw",PARAM_INT,4)) return FALSE;
       if (!WriteRtfControl(rtf,GetLiteralBorderType(para->leftBrdrType),PARAM_NONE,0)) return FALSE;
       }

	   if (para->rightBrdrType>0) {
          if (!WriteRtfControl(rtf,"brdrr",PARAM_NONE,0)) return FALSE;
       if (para->rightBrdrWidth>=0)
	   {
		   if (!WriteRtfControl(rtf,"brdrw",PARAM_INT,para->topBrdrWidth)) return FALSE;
	   }
	   else
		   if (!WriteRtfControl(rtf,"brdrw",PARAM_INT,4)) return FALSE;
       if (!WriteRtfControl(rtf,GetLiteralBorderType(para->rightBrdrType),PARAM_NONE,0)) return FALSE;
       }
    }
    return TRUE;
}

/*****************************************************************************
    WriteRtfColor:
    Fill the RTF color table and write the color table to the rtf output device.
******************************************************************************/
Bool WriteRtfColor(struct StrRtfOut far *rtf,Bool head)
{
    int i,j,TotalColors;
    uchar red,green,blue;
    struct StrRtfColor far *color;
	int oldColors=rtf->TotalColors;

    // Fill the rtf color table
    color=rtf->color;
	color[0].color=-1;    // default color
	if (head)
		TotalColors=1;
	else
	    TotalColors=rtf->TotalColors;                           // total number of colors in the table

    // scan the font table
    for (CEDChar * ch=rtf->page->GetChar(0);ch;ch=ch->next)
	{
       // fill the text foreground color
       for (j=0;j<TotalColors;j++) {                        // scan the color table
          if (ch->foregroundColor==(int)color[j].color) break;  // color found
       }
       if (j==TotalColors && TotalColors<MAX_RTF_COLORS) {
          color[TotalColors].color=ch->foregroundColor;
          TotalColors++;
       }
       // fill the text background color
       for (j=0;j<TotalColors;j++) {                         // scan the color table
          if (ch->backgroundColor==(int)color[j].color) break; // color found
       }
       if (j==TotalColors && TotalColors<MAX_RTF_COLORS) {
          color[TotalColors].color=ch->backgroundColor;
          TotalColors++;
       }
    }
    // scan the stylesheet table
/*    for (i=0;i<TotalSID;i++) {
       // fill the text foreground color
       for (j=0;j<TotalColors;j++) {                        // scan the color table
          if (StyleId[i].TextColor==color[j].color) break;  // color found
       }
       if (j==TotalColors && TotalColors<MAX_RTF_COLORS) {
          color[TotalColors].color=StyleId[i].TextColor;
          TotalColors++;
       }
       // fill the text foreground color
       for (j=0;j<TotalColors;j++) {                         // scan the color table
          if (StyleId[i].TextBkColor==color[j].color) break; // color found
       }
       if (j==TotalColors && TotalColors<MAX_RTF_COLORS) {
          color[TotalColors].color=StyleId[i].TextBkColor;
          TotalColors++;
       }
    }
*/
    rtf->TotalColors=TotalColors;                           // total number of colors in the table

    // write the color table
	if (head)
	{
		if (!BeginRtfGroup(rtf)) return FALSE;      // begin color group
		if (!WriteRtfControl(rtf,"colortbl",PARAM_NONE,0)) return FALSE;
//		if (!WriteRtfText(rtf,";",1)) return FALSE; // write the default color
	}
    for (i=oldColors;i<TotalColors;i++)
	{                  // write colors from the font table
		if (color[i].color!=-1)
		{
			red=GetRValue(color[i].color);
			green=GetGValue(color[i].color);
			blue=GetBValue(color[i].color);

			if (!WriteRtfControl(rtf,"red",PARAM_INT,red)) return FALSE;
			if (!WriteRtfControl(rtf,"green",PARAM_INT,green)) return FALSE;
			if (!WriteRtfControl(rtf,"blue",PARAM_INT,blue)) return FALSE;
		}
		if (!WriteRtfText(rtf,";",1)) return FALSE;  // write the delimiter
    }

    if (head)
		if (!EndRtfGroup(rtf)) return FALSE;        // end color group

    return TRUE;
}

/*****************************************************************************
    ReadRtfColorTable:
    Read the RTF color table.  This function return 0 when sucessful, otherwise
    it returns an error code (RTF_FILE_INCOMPLETE or RTF_SYNTAX_ERROR)
******************************************************************************/

int ReadRtfColorTable(struct StrRtfOut far *rtf)
{
    struct StrRtfColor far *color;
    int    i,CurColor=0,ControlGroupLevel=0;
    uchar   red,green,blue;

    color=rtf->color;
    for (i=0;i<MAX_RTF_COLORS;i++) color->color=-1;   // initialize colors

    ControlGroupLevel=rtf->GroupLevel=2;
//    rtf->IgnoreCrLfInControlWord=TRUE;     // ignore cr/lf in control word

    while (TRUE) {
       // get color number
       if (!GetRtfWord(rtf)) return RTF_FILE_INCOMPLETE;

       if (rtf->GroupBegin) continue;
       if (rtf->GroupEnd) {
          if (rtf->GroupLevel<ControlGroupLevel)
		  {
			  rtf->TotalColors=CurColor;
			  break;    // color group ended
		  }
          else                                   continue; // look for next color
       }

       if (rtf->IsControlWord) {                 // extract color component
          red=GetRValue(color[CurColor].color);  // break existing color components
          green=GetGValue(color[CurColor].color);
          blue=GetBValue(color[CurColor].color);

          if      (lstrcmpi(rtf->CurWord,"red")==0)   red=(uchar)(rtf->IntParam);
          else if (lstrcmpi(rtf->CurWord,"green")==0) green=(uchar)(rtf->IntParam);
          else if (lstrcmpi(rtf->CurWord,"blue")==0)  blue=(uchar)(rtf->IntParam);
          color[CurColor].color=RGB(red,green,blue);   // extracted color
       }
       else {
          if (lstrcmpi(rtf->CurWord,";")==0) CurColor++;                   // next color begins
       }
    }

//    rtf->IgnoreCrLfInControlWord=FALSE;       // reset this flag

    return 0;                 // successful
}

char brdS[]="brdrs";
char brdSh[]="brdrsh";
char brdDot[]="brdrdot";
char brdDash[]="brdrdash";
char brdDb[]="brdrdb";

char* GetLiteralBorderType(int type)
{
	switch (type)
		{
		case ED_BRDR_SINGLE:
			return brdS;
			break;
		case ED_BRDR_SHADOWED:
			return brdSh;
			break;
		case ED_BRDR_DOTTED:
			return brdDot;
			break;
		case ED_BRDR_DASHED:
			return brdDash;
			break;
		case ED_BRDR_DOUBLE:
			return brdDb;
			break;
		}
	return brdS;
}
