// Machine generated IDispatch wrapper class(es) created with ClassWizard
/////////////////////////////////////////////////////////////////////////////
// _IRecognitionEvents wrapper class

/////////////////////////////////////////////////////////////////////////////
// IRecognition wrapper class

class IRecognition : public COleDispatchDriver
{
public:
	IRecognition() {}		// Calls COleDispatchDriver default constructor
	IRecognition(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IRecognition(const IRecognition& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	short GetLanguage();
	void SetLanguage(short nNewValue);
	long GetSpeller();
	void SetSpeller(long nNewValue);
	long GetOneColumn();
	void SetOneColumn(long nNewValue);
	long GetFax100();
	void SetFax100(long nNewValue);
	long GetDotMatrix();
	void SetDotMatrix(long nNewValue);
	CString GetUserDictName();
	void SetUserDictName(const char * lpszNewValue);
	long GetBold();
	void SetBold(long nNewValue);
	long GetItalic();
	void SetItalic(long nNewValue);
	long GetSize();
	void SetSize(long nNewValue);
	long GetFormat();
	void SetFormat(long nNewValue);
	CString GetSerifName();
	void SetSerifName(const char * lpszNewValue);
	CString GetSansSerifName();
	void SetSansSerifName(const char * lpszNewValue);
	CString GetCourierName();
	void SetCourierName(const char * lpszNewValue);
	long GetProgressStep();
	void SetProgressStep(long nNewValue);
	long GetProgressStop();
	void SetProgressStop(long nNewValue);
	long GetProgressStart();
	void SetProgressStart(long nNewValue);
	long GetReturnCode();
	CString GetReturnString(long rc);
	long Load();
	void Unload();
	long RecogDIBtoFile(long hDIB, const char * lpFileName, short shFormat, short shCode);
	short EnumLanguage(short PrevLang);
	long RecogDIBtoMemory(long hDIB, long* lphMem, short Format, short Code);
	long RecogClipboard();
	long XOpen(long hDIB, const char * FileName);
	long XClose();
	long XRecognition(long bThread, long context);
	long XSave(const char * FileName, long Format, long Code);
	long XPageAnalysis();
	CString GetLanguageString(long Code);
	long EnumFormats(long prev);
	CString GetFormatString(long Code);
	long EnumCodes(long Format, long prev);
	CString GetCodeString(long Code);
	long GetModePicture();
	void SetModePicture(long nNewValue);
	long GetModeTable();
	void SetModeTable(long nNewValue);
	long GetModeFormat();
	void SetModeFormat(long nNewValue);
	CString GetVersion();
	short GetUnrecogChar();
	void SetUnrecogChar(short nNewValue);
	long EnumFormatMode(long nPrev);
	long EnumTableMode(long nPrev);
	long EnumPictureMode(long nPrev);
	CString GetFormatModeString(long nMode);
	CString GetPictureModeString(long nMode);
	CString GetTableModeString(long nMode);
	long XGetRotateDIB(long* phDIB, long* x0, long* y0);
	long GetAutoRotate();
	void SetAutoRotate(long nNewValue);
	void SetPageSize(long width, long height);
	void RenameImageFile(const char * name);
	long XSetTemplate(long nLeft, long nTop, long nRight, long nBottom);
	long GetCurrentEdPage();
	void SetCurrentEdPage(long nNewValue);
	long GetPreserveLineBreaks();
	void SetPreserveLineBreaks(long nNewValue);
	long Save(long hEdPage, const char * FileName, long Format, long Code, long bAppend);
	long SaveToBSTR(long hEdPage, long Format, long Code, BSTR* lpbstr);
	void DeleteEdPage(long hEdPage);
};
