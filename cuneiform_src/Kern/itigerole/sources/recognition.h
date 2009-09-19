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

// Recognition.h : Declaration of the CRecognition

#ifndef __RECOGNITION_H_
#define __RECOGNITION_H_

#include "resource.h"       // main symbols
#include "TigerCP.h"

#define MAX_STR 260

/////////////////////////////////////////////////////////////////////////////
// CRecognition
class ATL_NO_VTABLE CRecognition :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CRecognition, &CLSID_Recognition>,
	public ISupportErrorInfo,
	public IConnectionPointContainerImpl<CRecognition>,
	public IDispatchImpl<IRecognition, &IID_IRecognition, &LIBID_TIGERLib>,
	public CProxy_IRecognitionEvents< CRecognition >
{
public:
	CRecognition();
	~CRecognition();
// Attributes
public:
	HANDLE	hThread;
	DWORD	dwReturnCode;
	Bool	bIsLoad; // Загружена ли библиотека
	Bool	bIsOpen; // Открыто ли изображение

	long	nContext; // Для временного использования. В частности,
				// для передачи данных между потоками.

	HRESULT hRetCode;
private:
// Operations
public:
	inline Bool IsLoad(){ return bIsLoad;};
	inline Bool IsOpen(){ return bIsOpen;};

	void SetError(uint nID);
	void SetError(char * str);
	int  PassException(LPEXCEPTION_POINTERS  rc);

#if defined(__TIGER__)
DECLARE_REGISTRY_RESOURCEID(IDR_RECOGNITION_TIGER)
#elif defined(__PUMA__)
	#if defined (__MERGE__)
		DECLARE_REGISTRY_RESOURCEID(IDR_RECOGNITION_PUMA_MERGE)
	#else
		DECLARE_REGISTRY_RESOURCEID(IDR_RECOGNITION_PUMA)
	#endif
#endif
DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CRecognition)
	COM_INTERFACE_ENTRY(IRecognition)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
END_COM_MAP()
BEGIN_CONNECTION_POINT_MAP(CRecognition)
CONNECTION_POINT_ENTRY(DIID__IRecognitionEvents)
END_CONNECTION_POINT_MAP()


// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IRecognition
public:
	STDMETHOD(XGetTemplate)(/*[out]*/ long * lpnLeft,/*[out]*/ long * lpnTop,/*[out]*/ long * lpnRight,/*[out]*/ long * lpnBottom,/*[out,retval]*/ Bool * rc);
	STDMETHOD(SetSpecialProject)(/*[in]*/ long nSpecPrj, /*[retval]*/ Bool * ret);
	STDMETHOD(DeleteEdPage)(/*[in]*/long hEdPage);
	STDMETHOD(SaveToBSTR)(/*[in]*/ long hEdPage,/*[in]*/ long format,/*[in]*/ long code,/*[out]*/ BSTR * lpBstr,/*[out,retval]*/ Bool * rc);
	STDMETHOD(Save)(/*[in]*/ long hEdPage,/*[in]*/ BSTR FileName,/*[in]*/ long format,/*[in]*/ long code,/*[in]*/ Bool bAppend,/*[out,retval]*/ Bool * rc);
	STDMETHOD(get_PreserveLineBreaks)(/*[out, retval]*/ Bool *pVal);
	STDMETHOD(put_PreserveLineBreaks)(/*[in]*/ Bool newVal);
	STDMETHOD(get_CurrentEdPage)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_CurrentEdPage)(/*[in]*/ long newVal);
	STDMETHOD(XSetTemplate)(/*[in]*/ long nLeft,/*[in]*/ long nTop,/*[in]*/ long nRight,/*[in]*/ long nBottom,/*[out,retval]*/ Bool * rc );
	STDMETHOD(RenameImageFile)(/*[in]*/ BSTR name);
	STDMETHOD(SetPageSize)(/*[in]*/ long width,/*[in]*/ long height);
	STDMETHOD(get_AutoRotate)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_AutoRotate)(/*[in]*/ long newVal);
	STDMETHOD(XGetRotateDIB)(/*[out]*/ long * phDIB,/*[out]*/ long * x0, /*[out]*/ long * y0,/*[out,retval]*/ Bool * rc);
	STDMETHOD(GetTableModeString)(/*[in]*/ long nMode,/*[out,retval]*/ BSTR * lpbstr);
	STDMETHOD(GetPictureModeString)(/*[in]*/  long nMode,/*[out,retval]*/ BSTR * lpbstr);
	STDMETHOD(GetFormatModeString)(/*[in]*/  long nMode,/*[out,retval]*/ BSTR * lpbstr);
	STDMETHOD(EnumPictureMode)(/*[in]*/  long nPrev,/*[out,retval]*/  long * pMode);
	STDMETHOD(EnumTableMode)(/*[in]*/  long nPrev,/*[out,retval]*/ long * pMode);
	STDMETHOD(EnumFormatMode)(/*[in]*/ long nPrev,/*[out,retval]*/ long * pMode);
	STDMETHOD(get_UnrecogChar)(/*[out, retval]*/ short *pVal);
	STDMETHOD(put_UnrecogChar)(/*[in]*/ short newVal);
	STDMETHOD(GetVersion)(/*[out,retval]*/ BSTR *  bstrVersion);
	STDMETHOD(get_ModeFormat)(long *pVal);
	STDMETHOD(put_ModeFormat)(/*[in]*/ long newVal);
	STDMETHOD(get_ModeTable)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_ModeTable)(/*[in]*/ long newVal);
	STDMETHOD(get_ModePicture)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_ModePicture)(/*[in]*/ long newVal);
	STDMETHOD(GetCodeString)( /*[in]*/ long code,/*[out, retval]*/ BSTR * str);
	STDMETHOD(EnumCodes)(/*[in]*/long format,/*[in]*/long prev,/*[out,retval]*/ long * next);
	STDMETHOD(GetFormatString)(/*[in]*/ long code,/*[out,retval]*/ BSTR * lpstr);
	STDMETHOD(EnumFormats)(/*[in]*/ long prev,/*[out,retval]*/ long * next);
	STDMETHOD(GetLanguageString)(/*[in]*/ long code,/*[out,retval]*/ BSTR * lpstr);
	STDMETHOD(XPageAnalysis)(/*[out,retval]*/ Bool * rc);
	STDMETHOD(XSave)(/*[in]*/ BSTR FileName,/*[in]*/ long format,/*[in]*/ long code,/*[out,retval]*/ Bool * rc);
	STDMETHOD(XRecognition)(/*[in]*/ Bool bThread,/*[in]*/long context,/*[out,retval]*/ Bool * rc);
	STDMETHOD(XClose)(/*[out,retval]*/ Bool * rc);
	STDMETHOD(XOpen)(/*[in]*/ long hDIB,/*[in]*/ BSTR FileName,/*[out,retval]*/ Bool * hRc);
	STDMETHOD(RecogClipboard)(/*[out,retval]*/ Bool * rc);
	STDMETHOD(RecogDIBtoMemory)(/*[in]*/ long hDIB,/*[out,retval]*/ long * lphMem, /*[in]*/ short Format,/*[in]*/ short Code,/*[out,retval]*/ Bool * rc);
	STDMETHOD(EnumLanguage)(/*[in]*/ short PrevLang,/*[out,retval]*/ short * lpNextLang);
	STDMETHOD(RecogDIBtoFile)(/*[in]*/ long hDIB, /*[in]*/ BSTR lpFileName, /*[in]*/ short shFormat,/*[in]*/ short shCode,/*[out,retval]*/Bool * rc);
	STDMETHOD(Unload)();
	STDMETHOD(Load)(/*[out, retval]*/ Bool * rc);
	STDMETHOD(GetReturnString)(/*[in]*/ long rc,/*[out]*/ BSTR * str);
	STDMETHOD(GetReturnCode)(/*[in]*/ long * rc);
	STDMETHOD(get_ProgressStart)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_ProgressStart)(/*[in]*/ long newVal);
	STDMETHOD(get_ProgressStop)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_ProgressStop)(/*[in]*/ long newVal);
	STDMETHOD(get_ProgressStep)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_ProgressStep)(/*[in]*/ long newVal);
	STDMETHOD(get_CourierName)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_CourierName)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_SansSerifName)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_SansSerifName)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_SerifName)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_SerifName)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_Format)(/*[out, retval]*/ Bool *pVal);
	STDMETHOD(put_Format)(/*[in]*/ Bool newVal);
	STDMETHOD(get_Size)(/*[out, retval]*/ Bool *pVal);
	STDMETHOD(put_Size)(/*[in]*/ Bool newVal);
	STDMETHOD(get_Italic)(/*[out, retval]*/ Bool *pVal);
	STDMETHOD(put_Italic)(/*[in]*/ Bool newVal);
	STDMETHOD(get_Bold)(/*[out, retval]*/ Bool *pVal);
	STDMETHOD(put_Bold)(/*[in]*/ Bool newVal);
	STDMETHOD(get_UserDictName)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_UserDictName)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_DotMatrix)(/*[out, retval]*/ Bool *pVal);
	STDMETHOD(put_DotMatrix)(/*[in]*/ Bool newVal);
	STDMETHOD(get_Fax100)(/*[out, retval]*/ Bool *pVal);
	STDMETHOD(put_Fax100)(/*[in]*/ Bool newVal);
	STDMETHOD(get_OneColumn)(/*[out, retval]*/ Bool *pVal);
	STDMETHOD(put_OneColumn)(/*[in]*/ Bool newVal);
	STDMETHOD(get_Speller)(/*[out, retval]*/ Bool *pVal);
	STDMETHOD(put_Speller)(/*[in]*/ Bool newVal);
	STDMETHOD(get_Language)(/*[out, retval]*/ short *pVal);
	STDMETHOD(put_Language)(/*[in]*/ short newVal);
};

#endif //__RECOGNITION_H_
