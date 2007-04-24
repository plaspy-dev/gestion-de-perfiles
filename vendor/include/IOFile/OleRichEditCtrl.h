#pragma once



#include <richole.h>


/////////////////////////////////////////////////////////////////////////////
// COleRichEditCtrl window

class __IOFILE_EXPORT__ COleRichEditCtrl : public CRichEditCtrl
{
// Construction
public:
	COleRichEditCtrl();
	virtual ~COleRichEditCtrl();

	long StreamInFromResource(int iRes, LPCTSTR sType);
	long LoadFromFile(CString file);
	long SaveFile(CString file);
	void Print(CDC* pDC, CPrintInfo* pInfo);
	void SetVariable(CString variable, CString valor);
	void SetBitmap(CString variable, HBITMAP bmp);
	void ResetVariables(bool clean=false);
protected:
	CStringArray	m_saVariables;
	CStringArray	m_saValores;
	CStringArray	m_saVariablesB;
	CPtrArray		m_saValoresB;
	static DWORD CALLBACK readFunction(DWORD dwCookie,
		 LPBYTE lpBuf,			// the buffer to fill
		 LONG nCount,			// number of bytes to read
		 LONG* nRead);			// number of bytes actually read

	static DWORD CALLBACK writeFunction(DWORD dwCookie,
		 LPBYTE lpBuf,			// the buffer to fill
		 LONG nCount,			// number of bytes to read
		 LONG* nRead);			// number of bytes actually read

	interface IExRichEditOleCallback;	// forward declaration (see below in this header file)

	IExRichEditOleCallback* m_pIRichEditOleCallback;
	BOOL m_bCallbackSet;
	
	
	interface IExRichEditOleCallback : public IRichEditOleCallback
	{
	public:
		IExRichEditOleCallback();
		virtual ~IExRichEditOleCallback();
		int m_iNumStorages;
		IStorage* pStorage;
		DWORD m_dwRef;

		virtual HRESULT STDMETHODCALLTYPE GetNewStorage(LPSTORAGE* lplpstg);
		virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void ** ppvObject);
		virtual ULONG STDMETHODCALLTYPE AddRef();
		virtual ULONG STDMETHODCALLTYPE Release();
		virtual HRESULT STDMETHODCALLTYPE GetInPlaceContext(LPOLEINPLACEFRAME FAR *lplpFrame,
			LPOLEINPLACEUIWINDOW FAR *lplpDoc, LPOLEINPLACEFRAMEINFO lpFrameInfo);
 		virtual HRESULT STDMETHODCALLTYPE ShowContainerUI(BOOL fShow);
 		virtual HRESULT STDMETHODCALLTYPE QueryInsertObject(LPCLSID lpclsid, LPSTORAGE lpstg, LONG cp);
 		virtual HRESULT STDMETHODCALLTYPE DeleteObject(LPOLEOBJECT lpoleobj);
 		virtual HRESULT STDMETHODCALLTYPE QueryAcceptData(LPDATAOBJECT lpdataobj, CLIPFORMAT FAR *lpcfFormat,
			DWORD reco, BOOL fReally, HGLOBAL hMetaPict);
 		virtual HRESULT STDMETHODCALLTYPE ContextSensitiveHelp(BOOL fEnterMode);
 		virtual HRESULT STDMETHODCALLTYPE GetClipboardData(CHARRANGE FAR *lpchrg, DWORD reco, LPDATAOBJECT FAR *lplpdataobj);
 		virtual HRESULT STDMETHODCALLTYPE GetDragDropEffect(BOOL fDrag, DWORD grfKeyState, LPDWORD pdwEffect);
 		virtual HRESULT STDMETHODCALLTYPE GetContextMenu(WORD seltyp, LPOLEOBJECT lpoleobj, CHARRANGE FAR *lpchrg,
			HMENU FAR *lphmenu);
	};
 

public:
	int SelectText(CString texto);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COleRichEditCtrl)
	protected:
	virtual void PreSubclassWindow();
	
	//}}AFX_VIRTUAL

// Implementation
public:


	// Generated message map functions
protected:
	//{{AFX_MSG(COleRichEditCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

