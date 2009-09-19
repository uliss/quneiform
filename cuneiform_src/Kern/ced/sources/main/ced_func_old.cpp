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
#include "stdafx.h"

#include "ced_struct.h"
#include "cedint.h"

//define variables and stubs
FNRDProc RDProced=0;//points to unstructured data processor
#define DEC_FUN(a,b,c)  FN##b b; a My##b c {} a MyRawData##b c {RDProced((uchar*)pt,sizeof(*pt));}
//points to structured data processor; functions map structured data into unstructured one

DEC_FUN(void, CED_BitmapRef,(const bit_map_ref* pt))
DEC_FUN(void, CED_TextRef,(const text_ref* pt))
DEC_FUN(void, CED_FontKegl,(const font_kegl *pt))
DEC_FUN(void, CED_Kegl,(const kegl* pt))
DEC_FUN(void, CED_Shift,(const shift* pt))
DEC_FUN(void, CED_RetrieveLevel,(const retrieve_level* pt))
DEC_FUN(void, CED_Underline,(const underline* pt))
DEC_FUN(void, CED_DensPrint,(const dens_print* pt))
DEC_FUN(void, CED_Tabul,(const tabul* pt))
DEC_FUN(void, CED_TablTabul,(const tabl_tabul* pt))
DEC_FUN(void, CED_SheetDiskDescr,(const sheet_disk_descr* pt))
DEC_FUN(void, CED_FragmDiskDescr,(const fragm_disk_descr* pt))
DEC_FUN(void, CED_FragmDisk,(const fragm_disk* pt))
DEC_FUN(void, CED_StepBack,(const step_back* pt))
DEC_FUN(void, CED_LineBeg,(const line_beg* pt))
DEC_FUN(void, CED_Position,(const position* pt))
DEC_FUN(void, CED_EdTagLanguage,(const EdTagLanguage* pt))
DEC_FUN(void, CED_TableConformSizes,(const table_conform_sizes* pt))
DEC_FUN(void, CED_GroupWords,(const group_words* pt))
DEC_FUN(void, CED_GroupSymbols,(const group_symbols* pt))
DEC_FUN(void, CED_Border,(const border* pt))
DEC_FUN(void, CED_TableHeader,(const table_header* pt))
DEC_FUN(void, CED_ListOfFragments,(const list_of_fragments* pt))
DEC_FUN(void, CED_Aksant,(const aksant* pt))

#undef DEC_FUN

#define DEC_FUN(a,b,c)  FN##b b; a My##b c {} a MyRawData##b c {RDProced((uchar*)pt,alternatives*2);}
//the same for symbols
    DEC_FUN(void, CED_Letter,(const letter* pt,const uint32_t alternatives))
#undef DEC_FUN
#define DEC_FUN(a,b,c)  FN##b b; a My##b c {} a MyRawData##b c {RDProced((uchar*)pt,pt->length);}
//the same for special codes
    DEC_FUN(void, CED_Extention,(const edExtention* pt,const void* ptExt))
    DEC_FUN(void, CED_ExtentionNew,(const edExtentionNew* pt,const void* ptExt))
#undef DEC_FUN

struct lin
{
	CEDChar * beg,* end;
	int number,frameNum,x,ident;
	Bool32 ParaBegin;
	CEDLine * line;
};

static CEDPage * mainPage;
static edBox		refBox;
static int		font,kegl,lang;
static uchar	level;		//level in a structure, where we put ExtData
					//0-CEDPage,1-Section,2-Para,3-Line,4-Char
static lin* array;//array for rearrangement of rows
static int arPosition,arLen;//position and length in array
static char * verInfo;
static text_ref * TRarray;//put all text_ref-s here
static int TRPosition,TRLen;//position and length in array


static void ExtDataProc(uchar* _ptr, uint32_t lth);
void FormattedSDD(const sheet_disk_descr* pt);
void FormattedFDD(const fragm_disk_descr* pt);
void FormattedTR(const text_ref* pt);
void FormattedFD(const fragm_disk* pt);
void FormattedLB(const line_beg* pt);
void FormattedL(const letter* pt,const uint32_t alternatives);
void FormattedBMR(const bit_map_ref * pt);
void FormattedFK(const font_kegl * pt);
void FormattedE(const edExtention* pt,const void* ptExt);
void FormattedLang(const EdTagLanguage* pt);

void StripLines();
void RecreateFrames();

CEDPage * Formattedload_96(char * file,Bool32 readFromFile, uint32_t bufLen)
{
	return 0;
	CED_SheetDiskDescr=FormattedSDD;
	CED_TextRef=FormattedTR;
	CED_FragmDiskDescr=FormattedFDD;
	CED_FragmDisk=FormattedFD;
	CED_LineBeg=FormattedLB;
	CED_Letter=FormattedL;
	CED_BitmapRef=FormattedBMR;
	CED_FontKegl=FormattedFK;
	CED_Extention=FormattedE;
	CED_EdTagLanguage=FormattedLang;

	mainPage=new(CEDPage);
	level=0;//ExtData goes to CEDPage
	refBox.x=refBox.y=refBox.h=refBox.w=0;
	font=kegl=lang=-1;
	arPosition=0;
	arLen=1000;
	array=new lin[1000];
	TRarray=new text_ref[1000];
	TRPosition=0;
	TRLen=1000;
	verInfo=0;
	CED_ReadED(file,readFromFile,bufLen);
 	if (verInfo&&memcmp(verInfo,"\nVer 96.2 (C) Cognitive Technologies Ltd.\n",40)==0)
	{
		StripLines();
		RecreateFrames();
		delete[] verInfo;
	}
	else
		return 0;
	delete[]array;
	delete[]TRarray;
	return mainPage;
}


void FormattedLang(const EdTagLanguage* pt)
{
	lang=pt->language;
}


void FormattedSDD(const sheet_disk_descr* pt)
{
	level=0;
	mainPage->dpi.cx=mainPage->dpi.cy=pt->resolution;
	mainPage->pageNumber=pt->sheet_numb;
	mainPage->turn=pt->incline;
	mainPage->NumberOfParagraphs=pt->quant_fragm;
	mainPage->InsertSection();
}

void FormattedFDD(const fragm_disk_descr* pt)
{
	edBox bx;
	bx.x=pt->col;
	bx.y=pt->row;
	bx.w=pt->w_width;
	bx.h=pt->height;
	CEDSection * sect=mainPage->GetCurSection();
	if (sect->numberOfColumns==0)
		sect->CreateColumn();
	sect->CreateFrame(sect->columnsCur,bx);
}

void FormattedE(const edExtention* pt,const void* ptExt)
{
	switch(pt->Ecode)
	{
	case 0:
		verInfo=new char[pt->length-sizeof(edExtention)];
		memcpy(verInfo,ptExt,pt->length-sizeof(edExtention));
		break;
	}
}
void FormattedTR(const text_ref* pt)
{
	if (TRPosition==TRLen)
	{
		text_ref * qq=new text_ref[TRLen+1000];
		memcpy(qq,TRarray,TRLen*sizeof(text_ref));
		delete[] TRarray;
		TRarray=qq;
		TRLen+=1000;
	}
	TRarray[TRPosition].object=pt->object;
	TRarray[TRPosition].type=pt->type;
	TRPosition++;

	switch(pt->type)
	{
/*	case SSR_FRAG_TYPE:
		level=2;
		{
			if(pt->object==TP_MCOL_BEG||pt->object==TP_MCOL_END)
			{
				if(mainPage->GetCurSection()&&
					mainPage->GetCurSection()->numberOfColumns==1&&
					mainPage->GetCurSection()->GetColumn(0)->GetFirstObject()==0)
					break;
				(mainPage->InsertSection())->CreateColumn(0,0);
				break;
			}
			if (pt->object==TP_NEW_COL)
			{
				if (mainPage->GetCurSection())
					mainPage->GetCurSection()->CreateColumn(0,0);
				break;
			}
			if(mainPage->GetSection(0)==0)
				(mainPage->InsertSection())->CreateColumn(0,0);;
			mainPage->GetCurSection()->InsertParagraph();
			if (pt->object&(TP_LEFT_ALLIGN|TP_RIGHT_ALLIGN|TP_CENTER))
				mainPage->GetCurSection()->GetCurParagraph()->alignment=pt->object;
			break;

		}
	case SSR_FRAG_PNUM:
		level=2;
//		mainPage->GetCurSection()->numberOfColumns=pt->object-1;
		break;
	case SSR_FRAG_N:
		level=2;
		mainPage->GetCurSection()->GetCurParagraph()->userNumber=pt->object;
		break;
	case SSR_FRAG_X:
		level=2;
		mainPage->GetCurSection()->GetCurParagraph()->layout.x=pt->object;
		break;
	case SSR_FRAG_W:
		level=2;
		mainPage->GetCurSection()->GetCurParagraph()->layout.w=pt->object;
		break;
	case SSR_FRAG_Y:
		level=2;
		mainPage->GetCurSection()->GetCurParagraph()->layout.y=pt->object;
		break;
	case SSR_FRAG_H:
		level=2;
		mainPage->GetCurSection()->GetCurParagraph()->layout.h=pt->object;
		break;

	case SSR_LINE_INDENT:
		mainPage->GetCurSection()->GetCurParagraph()->ident.top=pt->object;
		break;

	case SSR_FRAG_END:
		mainPage->SetCurSection(0);
		mainPage->GetCurSection()->SetCurParagraph(0);
*/
	case SSR_LINE_NUMBER:
		if (arPosition==arLen)
		{
			lin * qq=new lin[arLen+1000];
			memcpy(qq,array,arLen*sizeof(lin));
			delete[] array;
			array=qq;
			arLen+=1000;
		}

		CEDLine *line=mainPage->GetCurSection()->GetCurParagraph()->GetCurLine();
		array[arPosition].beg=0;
		if (line->curChar)
			array[arPosition].beg=line->curChar;
		else
		{
			if(line->internalNumber!=0)
			{
				CEDLine *ww=line->prev;
				while(ww&&!ww->chars)
						ww=ww->prev;
				if (ww)
				{
					CEDChar *qq=ww->chars;
					while (qq->next)
						qq=qq->next;
					array[arPosition].beg=qq;
				}
			}
		}
		array[arPosition].number=pt->object;
		int i;
		for (i=TRPosition-1;i>=0&&TRarray[i].type!=SSR_LINE_FN;i--);
		if(i>=0&&TRarray[i].type==SSR_LINE_FN)
			array[arPosition].frameNum=TRarray[i].object;

		for (i=TRPosition-1;i>=0&&TRarray[i].type!=SSR_LINE_FN&&TRarray[i].type!=SSR_LINE_X;i--);
		if(i>=0&&TRarray[i].type==SSR_LINE_X)
			array[arPosition].x=/*(signed short int)*/TRarray[i].object;
		else
			array[arPosition].x=0;

		for (i=TRPosition-1;i>=0&&TRarray[i].type!=SSR_LINE_FN&&TRarray[i].type!=SSR_LINE_PARAGRAPH;i--);
		if(i>=0&&TRarray[i].type==SSR_LINE_PARAGRAPH)
		{
			array[arPosition].ParaBegin=TRarray[i].object;
			for (i=TRPosition-1;i>=0&&TRarray[i].type!=SSR_LINE_FN&&TRarray[i].type!=SSR_LINE_INDENT;i--);
			if(i>=0&&TRarray[i].type==SSR_LINE_INDENT)
				array[arPosition].ident=TRarray[i].object;
			else
				array[arPosition].ident=0;
		}
		else
			array[arPosition].ParaBegin=FALSE;

		arPosition++;
		break;
	}
}

void FormattedFD(const fragm_disk* pt)
{

}

void FormattedLB(const line_beg* pt)
{
	if(!mainPage->GetCurSection())
		mainPage->InsertSection()->CreateColumn();//In case of wrong 'ed', such that symbols are before the definition of fragment
	mainPage->GetCurSection()->GetCurParagraph()->InsertLine();
}

void FormattedL(const letter* pt,const uint32_t alternatives)
{
	if(!mainPage->GetCurSection())
		mainPage->InsertSection()->CreateColumn();//In case of wrong 'ed', such that symbols are before the definition of fragment
	CEDLine * lin=mainPage->GetCurSection()->GetCurParagraph()->GetCurLine();
	if (!lin)
		lin=mainPage->GetCurSection()->GetCurParagraph()->InsertLine();//In case of wrong 'ed', such that symbols are before the definition of line
	CEDChar *chr=lin->InsertChar();
//	char * lpData = (char *)malloc(alternatives*sizeof(letter));
	letterEx * lpData = new letterEx[alternatives];
	memcpy(lpData,(void*)pt,alternatives*sizeof(letterEx));
	chr->alternatives=/*(letter*)*/lpData;
	chr->numOfAltern=alternatives;
	memcpy(&(chr->layout),&refBox,sizeof(edBox));
	chr->fontHeight=kegl;
	chr->fontAttribs=font;
//	chr->fontLang=lang;
}

void FormattedBMR(const bit_map_ref * pt)
{
	refBox.h=pt->height;
	refBox.w=pt->width;
	refBox.x=pt->col;
	refBox.y=pt->row;
}

void FormattedFK(const font_kegl * pt)
{
	font=pt->new_font;
	kegl=pt->new_kegl;
}

int compare( const void *arg1, const void *arg2 );

void StripLines()
{
    int i;
    CEDChar* ch;

	for (i=0;i<arPosition-1;i++)
	{
		array[i].end=array[i+1].beg;
		array[i].beg=array[i].beg!=0?array[i].beg->next:0;
	}

	for (ch=array[i].beg;ch&&ch->next;ch=ch->next);
	array[i].beg=array[i].beg!=0?array[i].beg->next:0;
	array[i].end=ch;

	//If line started before the first symbol (or even some amount of empty lines):
	for (i=0;array[i].beg==0;i++);
	if	(i)
	{
		array[i-1].beg=mainPage->GetChar(0);
		if (i!=1)
		for (int k=i-1;k<arPosition;k++)
			array[k-(i-1)]=array[k];
	}

	qsort( (void *)array, (size_t)arPosition, sizeof(lin), compare );
	for (i=0;i<arPosition-1;i++)
	{
		(array[i].end)->next=array[i+1].beg;
		array[i+1].beg->prev=array[i].end;
	}
	array[i].end->next=0;
	CEDLine * li1,*li;
	li1=li=mainPage->GetLine(0);
	while (li1)
	{
		li=li1->next;
		delete li1;
		li1=li;
	}
	mainPage->GetCurSection()->GetCurParagraph()->curLine=0;
	mainPage->GetCurSection()->GetCurParagraph()->numOfLines=0;

	for (i=0;i<arPosition;i++)
	{
	CEDLine *ll=mainPage->GetCurSection()->GetCurParagraph()->InsertLine();
	array[i].line=ll;
	ll->curChar=ll->chars=array[i].beg;
		for (CEDChar * cc=array[i].beg;cc&&cc!=array[i].end->next;cc=cc->next)
		{
			cc->parentNumber=i;
			ll->numOfChars++;
		}
	}
}
int compare( const void *arg1, const void *arg2 )
{
	if (((lin*)arg1)->frameNum==((lin*)arg2)->frameNum)
		return ((lin*)arg1)->number-((lin*)arg2)->number;
	else
		return ((lin*)arg1)->frameNum-((lin*)arg2)->frameNum;
}
void RecreateFrames()
{
	//save first line
	CEDLine *ll=mainPage->GetLine(0);
	if (ll==0)
		return;
	//remove all paragraphs
	CEDParagraph * pa1,*pa;
	pa1=pa=mainPage->GetParagraph(0);
	while (pa1)
	{
		pa=pa1->next;
		if (pa1->descriptor)
			free(pa1->descriptor);
		delete pa1;
		pa1=pa;
	}
	//delete the only existing section
	delete mainPage->sections;
	mainPage->curSect=0;
	mainPage->NumberOfSections=0;
	Bool32 inTable=FALSE;//is table processing now?
	int numOfCols=0;//number of columns in a table
	int borNum=0;//number of cells of rows in a table, multiplied by 2
	int * borders;//array of coordintes and widths (alternately)
	Bool32 firstCell;
	CEDParagraph *cell,*table,*row;
	int i;
	//cycle over all text_remark-s up to the end of file description
	for (i=0;i<TRPosition&&TRarray[i].type!=SSR_FRAG_END;i++)
	{
		if (TRarray[i].type==SSR_FRAG_TYPE&&(TRarray[i].object==TP_MCOL_BEG))
			(mainPage->InsertSection())->CreateColumn();
		if (TRarray[i].type==SSR_FRAG_TYPE&&TRarray[i].object==TP_NEW_COL&&!inTable)
			(mainPage->GetCurSection())->CreateColumn();
		if (TRarray[i].type==SSR_FRAG_TYPE&&(TRarray[i].object==TP_TAB_BEG))
		{
			inTable=TRUE;
			edBox bx;
			bx.h=bx.w=bx.x=bx.y=-1;
//			if (!mainPage->GetCurSection())
			//new table is always in a new section
				mainPage->InsertSection()->CreateColumn();
			table=mainPage->GetCurSection()->CreateTable(mainPage->GetCurSection()->columnsCur/*,
									sz,q1,q2,q3,q4,q5*/);
			row=mainPage->GetCurSection()->CreateTableRow(table,0, 0, -1, -1, -1,-1,-1, -1,-1,-1,-1,0, FALSE);
			cell=mainPage->GetCurSection()->CreateCell(row,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,bx,-1,-1);
			firstCell=TRUE;//in order to change its width
			numOfCols=-1;//this means: when we find relevant amount of columns, we memorize them
		}
		if (TRarray[i].type==SSR_FRAG_PNUM&&numOfCols==-1)
		{
			numOfCols=TRarray[i].object;
			borders=new int[numOfCols*2];
			borNum=0;
		}

		//memorize vertical lines
		if (TRarray[i].type==SSR_FRAG_COLXW&&numOfCols!=-1)
		{
			borders[borNum++]=(int16_t)TRarray[i].object;
		}
		if (TRarray[i].type==SSR_FRAG_TYPE&&TRarray[i].object==TP_NEW_ROW&&inTable)
		{
			edBox bx;
			bx.h=bx.w=bx.x=bx.y=-1;
			cell=mainPage->GetCurSection()->CreateTableRow(table,0, 0, -1, -1, -1,-1,-1, -1,-1,-1,-1,0, FALSE);
		}
		if (TRarray[i].type==SSR_FRAG_TYPE&&TRarray[i].object==TP_NEW_COL&&inTable)
		{
			edBox bx;
			bx.h=bx.w=bx.x=bx.y=-1;
			cell=mainPage->GetCurSection()->CreateCell(row,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,bx,-1,-1);
		}
		if (TRarray[i].type==SSR_FRAG_TYPE&&(TRarray[i].object==TP_MCOL_END)&&inTable)
		{
		    int k;
			//renew info in a table header
			edSize sz;
			sz.cx=numOfCols;
			sz.cy=((edTabDescr*)(table->descriptor))->numOfRows;
			int * q1=new int[sz.cx+1];
			int * q2=new int[sz.cy+1];
			int * q3=new int[sz.cy*sz.cx];
			Bool32 * q4=new Bool32[sz.cx+1];
			Bool32 * q5=new Bool32[sz.cy+1];
			for(k=0;k<sz.cy;k++)
				for(int j=0;j<sz.cx;j++)
					q3[k*sz.cx+j]=k*sz.cx+j;
			for (k=0;k<borNum;k+=2)
				q1[k/2]=borders[k];
			q1[k/2]=borders[k-2]+borders[k-1];
			delete[] borders;
//			table=mainPage->GetCurSection()->ReCreateTable(table,sz,q1,q2,q3,q4,q5);
			delete[]q1;
			delete[]q2;
			delete[]q3;
			delete[]q4;
			delete[]q5;
			numOfCols=-1;
			inTable=FALSE;
			CEDParagraph* para;
			//force paragraphs bounding a section to be invisible to function GetParagraph( in order for them to coincide with a file by paragraph number)
			for (para=table->prev;para!=((edTabDescr*)(table->descriptor))->last->next;para=para->next)
				para->internalNumber--;
			para->internalNumber-=2;
		}

		if ((TRarray[i].type==SSR_FRAG_TYPE)&&((TRarray[i].object&TP_BRACKET)==0))
		{
			edSize sz;
			edBox bx;
			EDRECT rct;
			int j;
			rct.left=rct.right=rct.top=-1;
			sz.cx=sz.cy=-1;
			bx.h=bx.w=bx.x=bx.y=-1;
			for (j=i;j<TRPosition&&TRarray[j].type!=SSR_FRAG_X;j++);
			if (TRarray[j].type==SSR_FRAG_X)
				bx.x=TRarray[j].object;
			for (j=i;j<TRPosition&&TRarray[j].type!=SSR_FRAG_Y;j++);
			if (TRarray[j].type==SSR_FRAG_Y)
				bx.y=TRarray[j].object;
			for (j=i;j<TRPosition&&TRarray[j].type!=SSR_FRAG_H;j++);
			if (TRarray[j].type==SSR_FRAG_H)
				bx.h=TRarray[j].object;
			for (j=i;j<TRPosition&&TRarray[j].type!=SSR_FRAG_W;j++);
			if (TRarray[j].type==SSR_FRAG_W)
				bx.w=TRarray[j].object;

			if (!inTable)
				mainPage->GetCurSection()->CreateParagraph(mainPage->GetCurSection()->columnsCur,-1,rct,-1,-1,sz,bx,-1,-1,-1,-1,-1);
			else
				mainPage->GetCurSection()->CreateParagraph(cell,-1,rct,-1,-1,sz,bx,-1,-1,-1,-1,-1);
		}
	}
	//create first lines in these paragraphs
	int last=-1;
	for (i=0;i<arPosition;i++)
	{
		if (last!=array[i].frameNum)
		{
			last=array[i].frameNum;
			mainPage->GetParagraph(last)->lines=ll;
		}
		ll=ll->next;
	}
	//fill in info about parent, number of lines in paragraph and create new paragraphs for lines with attribute ParaBegin
	int inc=0;
	int fn=array[0].frameNum;
	CEDParagraph * par=mainPage->GetParagraph(array[0].frameNum);
	par->indent.left=par->indent.top=(array[0].x-par->layout.x)*1440/mainPage->dpi.cx;
	for (i=0;i<arPosition;i++)
	{
		if (array[i].frameNum!=fn)
		{
			fn=array[i].frameNum;
			par=mainPage->GetParagraph(array[i].frameNum+inc);
			par->indent.left=par->indent.top=(array[i].x-par->layout.x)*1440/mainPage->dpi.cx;
		}
		if (array[i].ParaBegin)
		{
			par=mainPage->GetParagraph(array[i].frameNum+1+inc);
			mainPage->SetCurSection(par->parentNumber)->SetCurParagraph(par);
			par=mainPage->GetCurSection()->InsertParagraph(FALSE);
			par->lines=par->curLine=array[i].line;
			par->indent.top=array[i].ident*1440/mainPage->dpi.cx;
//			par->indent.left=(array[i].x-par->layout.x)*1440/mainPage->dpi;
			inc++;
		}
		array[i].line->parentNumber=array[i].frameNum+inc;
		par->numOfLines++;
	}

	//delete useless section separators
	int dec=0,paraNum,numOfSect=mainPage->GetNumberOfSections();
	if (mainPage->GetNumberOfSections()!=1)
	{
		CEDSection *sec=mainPage->SetCurSection(0);
		CEDSection *sec1;
		for (i=1;i<numOfSect;i++)
		{
			sec1=mainPage->NextSection();
			sec1->internalNumber-=dec;
			CEDParagraph *para=sec1->paragraphs;
			paraNum=sec1->internalNumber;
			if(sec->numberOfColumns==sec1->numberOfColumns)
			{
				sec->next=sec1->next;
				if(sec->next)
					sec->next->prev=sec;
				mainPage->NumberOfSections--;
				dec++;
				para=para->next;
				paraNum--;
				CEDParagraph* col=sec->paragraphs;
				CEDParagraph* col1=sec1->paragraphs;
				CEDParagraph*tmp;
				for (int j=0;j<sec->numberOfColumns;j++)
				{
					col=((EDCOLDESCR*)(col->descriptor))->next;
					if (!col)
						col=sec->columnsEnd;
					col->prev->next=col1->next;
					col1->next->prev=col->prev;
					tmp=col1;
					col1=((EDCOLDESCR*)(col1->descriptor))->next;
					if (!col1)
						col1=sec1->columnsEnd;
					free(tmp->descriptor);
					delete tmp;
					col1->prev->next=col;
					col->prev=col1->prev;
				}
				col->next=col1->next;
				if(col1->next)
					col1->next->prev=col;
				free(col1->descriptor);
				delete col1;
				delete sec1;
			}
			else
			{
				sec=mainPage->SetCurSection(sec1);
			}
			if(dec!=0)
				while (para&&para->parentNumber!=i+1)
				{
					para->parentNumber=paraNum;
					para=para->next;
				}
		}
	}
	//restore enumeration of paragraphs
	i=0;
	for (par=mainPage->GetParagraph(0);par;par=par->next)
	{
		par->internalNumber=i++;
		if (par->numOfLines==0)
			par->curLine=par->lines=0;
		else
			par->curLine=par->lines;
	}
}

