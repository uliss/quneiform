// Machine generated IDispatch wrapper class(es) created with ClassWizard

#include "stdafx.h"
#include "tiger.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// IRecognition properties

/////////////////////////////////////////////////////////////////////////////
// IRecognition operations

short IRecognition::GetLanguage()
{
	short result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void IRecognition::SetLanguage(short nNewValue)
{
	static uchar parms[] =
		VTS_I2;
	InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IRecognition::GetSpeller()
{
	long result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IRecognition::SetSpeller(long nNewValue)
{
	static uchar parms[] =
		VTS_I4;
	InvokeHelper(0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IRecognition::GetOneColumn()
{
	long result;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IRecognition::SetOneColumn(long nNewValue)
{
	static uchar parms[] =
		VTS_I4;
	InvokeHelper(0x3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IRecognition::GetFax100()
{
	long result;
	InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IRecognition::SetFax100(long nNewValue)
{
	static uchar parms[] =
		VTS_I4;
	InvokeHelper(0x4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IRecognition::GetDotMatrix()
{
	long result;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IRecognition::SetDotMatrix(long nNewValue)
{
	static uchar parms[] =
		VTS_I4;
	InvokeHelper(0x5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString IRecognition::GetUserDictName()
{
	CString result;
	InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IRecognition::SetUserDictName(const char * lpszNewValue)
{
	static uchar parms[] =
		VTS_BSTR;
	InvokeHelper(0x6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

long IRecognition::GetBold()
{
	long result;
	InvokeHelper(0x7, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IRecognition::SetBold(long nNewValue)
{
	static uchar parms[] =
		VTS_I4;
	InvokeHelper(0x7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IRecognition::GetItalic()
{
	long result;
	InvokeHelper(0x8, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IRecognition::SetItalic(long nNewValue)
{
	static uchar parms[] =
		VTS_I4;
	InvokeHelper(0x8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IRecognition::GetSize()
{
	long result;
	InvokeHelper(0x9, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IRecognition::SetSize(long nNewValue)
{
	static uchar parms[] =
		VTS_I4;
	InvokeHelper(0x9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IRecognition::GetFormat()
{
	long result;
	InvokeHelper(0xa, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IRecognition::SetFormat(long nNewValue)
{
	static uchar parms[] =
		VTS_I4;
	InvokeHelper(0xa, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString IRecognition::GetSerifName()
{
	CString result;
	InvokeHelper(0xb, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IRecognition::SetSerifName(const char * lpszNewValue)
{
	static uchar parms[] =
		VTS_BSTR;
	InvokeHelper(0xb, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IRecognition::GetSansSerifName()
{
	CString result;
	InvokeHelper(0xc, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IRecognition::SetSansSerifName(const char * lpszNewValue)
{
	static uchar parms[] =
		VTS_BSTR;
	InvokeHelper(0xc, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IRecognition::GetCourierName()
{
	CString result;
	InvokeHelper(0xd, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IRecognition::SetCourierName(const char * lpszNewValue)
{
	static uchar parms[] =
		VTS_BSTR;
	InvokeHelper(0xd, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

long IRecognition::GetProgressStep()
{
	long result;
	InvokeHelper(0xe, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IRecognition::SetProgressStep(long nNewValue)
{
	static uchar parms[] =
		VTS_I4;
	InvokeHelper(0xe, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IRecognition::GetProgressStop()
{
	long result;
	InvokeHelper(0xf, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IRecognition::SetProgressStop(long nNewValue)
{
	static uchar parms[] =
		VTS_I4;
	InvokeHelper(0xf, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IRecognition::GetProgressStart()
{
	long result;
	InvokeHelper(0x10, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IRecognition::SetProgressStart(long nNewValue)
{
	static uchar parms[] =
		VTS_I4;
	InvokeHelper(0x10, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IRecognition::GetReturnCode()
{
	long result;
	InvokeHelper(0x11, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

CString IRecognition::GetReturnString(long rc)
{
	CString result;
	static uchar parms[] =
		VTS_I4;
	InvokeHelper(0x12, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		rc);
	return result;
}

long IRecognition::Load()
{
	long result;
	InvokeHelper(0x13, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

void IRecognition::Unload()
{
	InvokeHelper(0x14, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

long IRecognition::RecogDIBtoFile(long hDIB, const char * lpFileName, short shFormat, short shCode)
{
	long result;
	static uchar parms[] =
		VTS_I4 VTS_BSTR VTS_I2 VTS_I2;
	InvokeHelper(0x15, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		hDIB, lpFileName, shFormat, shCode);
	return result;
}

short IRecognition::EnumLanguage(short PrevLang)
{
	short result;
	static uchar parms[] =
		VTS_I2;
	InvokeHelper(0x16, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		PrevLang);
	return result;
}

long IRecognition::RecogDIBtoMemory(long hDIB, long* lphMem, short Format, short Code)
{
	long result;
	static uchar parms[] =
		VTS_I4 VTS_PI4 VTS_I2 VTS_I2;
	InvokeHelper(0x17, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		hDIB, lphMem, Format, Code);
	return result;
}

long IRecognition::RecogClipboard()
{
	long result;
	InvokeHelper(0x18, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long IRecognition::XOpen(long hDIB, const char * FileName)
{
	long result;
	static uchar parms[] =
		VTS_I4 VTS_BSTR;
	InvokeHelper(0x19, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		hDIB, FileName);
	return result;
}

long IRecognition::XClose()
{
	long result;
	InvokeHelper(0x1a, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long IRecognition::XRecognition(long bThread, long context)
{
	long result;
	static uchar parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x1b, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		bThread, context);
	return result;
}

long IRecognition::XSave(const char * FileName, long Format, long Code)
{
	long result;
	static uchar parms[] =
		VTS_BSTR VTS_I4 VTS_I4;
	InvokeHelper(0x1c, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		FileName, Format, Code);
	return result;
}

long IRecognition::XPageAnalysis()
{
	long result;
	InvokeHelper(0x1d, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

CString IRecognition::GetLanguageString(long Code)
{
	CString result;
	static uchar parms[] =
		VTS_I4;
	InvokeHelper(0x1e, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		Code);
	return result;
}

long IRecognition::EnumFormats(long prev)
{
	long result;
	static uchar parms[] =
		VTS_I4;
	InvokeHelper(0x1f, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		prev);
	return result;
}

CString IRecognition::GetFormatString(long Code)
{
	CString result;
	static uchar parms[] =
		VTS_I4;
	InvokeHelper(0x20, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		Code);
	return result;
}

long IRecognition::EnumCodes(long Format, long prev)
{
	long result;
	static uchar parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x21, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		Format, prev);
	return result;
}

CString IRecognition::GetCodeString(long Code)
{
	CString result;
	static uchar parms[] =
		VTS_I4;
	InvokeHelper(0x22, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		Code);
	return result;
}

long IRecognition::GetModePicture()
{
	long result;
	InvokeHelper(0x23, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IRecognition::SetModePicture(long nNewValue)
{
	static uchar parms[] =
		VTS_I4;
	InvokeHelper(0x23, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IRecognition::GetModeTable()
{
	long result;
	InvokeHelper(0x24, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IRecognition::SetModeTable(long nNewValue)
{
	static uchar parms[] =
		VTS_I4;
	InvokeHelper(0x24, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IRecognition::GetModeFormat()
{
	long result;
	InvokeHelper(0x25, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IRecognition::SetModeFormat(long nNewValue)
{
	static uchar parms[] =
		VTS_I4;
	InvokeHelper(0x25, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString IRecognition::GetVersion()
{
	CString result;
	InvokeHelper(0x26, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

short IRecognition::GetUnrecogChar()
{
	short result;
	InvokeHelper(0x27, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void IRecognition::SetUnrecogChar(short nNewValue)
{
	static uchar parms[] =
		VTS_I2;
	InvokeHelper(0x27, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IRecognition::EnumFormatMode(long nPrev)
{
	long result;
	static uchar parms[] =
		VTS_I4;
	InvokeHelper(0x28, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nPrev);
	return result;
}

long IRecognition::EnumTableMode(long nPrev)
{
	long result;
	static uchar parms[] =
		VTS_I4;
	InvokeHelper(0x29, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nPrev);
	return result;
}

long IRecognition::EnumPictureMode(long nPrev)
{
	long result;
	static uchar parms[] =
		VTS_I4;
	InvokeHelper(0x2a, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nPrev);
	return result;
}

CString IRecognition::GetFormatModeString(long nMode)
{
	CString result;
	static uchar parms[] =
		VTS_I4;
	InvokeHelper(0x2b, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		nMode);
	return result;
}

CString IRecognition::GetPictureModeString(long nMode)
{
	CString result;
	static uchar parms[] =
		VTS_I4;
	InvokeHelper(0x2c, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		nMode);
	return result;
}

CString IRecognition::GetTableModeString(long nMode)
{
	CString result;
	static uchar parms[] =
		VTS_I4;
	InvokeHelper(0x2d, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		nMode);
	return result;
}

long IRecognition::XGetRotateDIB(long* phDIB, long* x0, long* y0)
{
	long result;
	static uchar parms[] =
		VTS_PI4 VTS_PI4 VTS_PI4;
	InvokeHelper(0x2e, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		phDIB, x0, y0);
	return result;
}

long IRecognition::GetAutoRotate()
{
	long result;
	InvokeHelper(0x2f, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IRecognition::SetAutoRotate(long nNewValue)
{
	static uchar parms[] =
		VTS_I4;
	InvokeHelper(0x2f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

void IRecognition::SetPageSize(long width, long height)
{
	static uchar parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x30, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 width, height);
}

void IRecognition::RenameImageFile(const char * name)
{
	static uchar parms[] =
		VTS_BSTR;
	InvokeHelper(0x31, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 name);
}

long IRecognition::XSetTemplate(long nLeft, long nTop, long nRight, long nBottom)
{
	long result;
	static uchar parms[] =
		VTS_I4 VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x32, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nLeft, nTop, nRight, nBottom);
	return result;
}

long IRecognition::GetCurrentEdPage()
{
	long result;
	InvokeHelper(0x33, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IRecognition::SetCurrentEdPage(long nNewValue)
{
	static uchar parms[] =
		VTS_I4;
	InvokeHelper(0x33, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IRecognition::GetPreserveLineBreaks()
{
	long result;
	InvokeHelper(0x34, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IRecognition::SetPreserveLineBreaks(long nNewValue)
{
	static uchar parms[] =
		VTS_I4;
	InvokeHelper(0x34, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IRecognition::Save(long hEdPage, const char * FileName, long Format, long Code, long bAppend)
{
	long result;
	static uchar parms[] =
		VTS_I4 VTS_BSTR VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x35, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		hEdPage, FileName, Format, Code, bAppend);
	return result;
}

long IRecognition::SaveToBSTR(long hEdPage, long Format, long Code, BSTR* lpbstr)
{
	long result;
	static uchar parms[] =
		VTS_I4 VTS_I4 VTS_I4 VTS_PBSTR;
	InvokeHelper(0x36, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		hEdPage, Format, Code, lpbstr);
	return result;
}

void IRecognition::DeleteEdPage(long hEdPage)
{
	static uchar parms[] =
		VTS_I4;
	InvokeHelper(0x37, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 hEdPage);
}
