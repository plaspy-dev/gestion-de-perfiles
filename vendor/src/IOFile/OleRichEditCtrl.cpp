// OleRichEditCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "OleRichEditCtrl.h"
# include "ImageDataObject.h"
# include <Componentes/Componentes.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COleRichEditCtrl

COleRichEditCtrl::COleRichEditCtrl()
{
	m_bCallbackSet = FALSE;
}

COleRichEditCtrl::~COleRichEditCtrl()
{
	// IExRichEditOleCallback class is a reference-counted class  
	// which deletes itself and for which delete should not be called

	// delete m_pIRichEditOleCallback;
}


BEGIN_MESSAGE_MAP(COleRichEditCtrl, CRichEditCtrl)
	//{{AFX_MSG_MAP(COleRichEditCtrl)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


int COleRichEditCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
 	if (CRichEditCtrl::OnCreate(lpCreateStruct) == -1)
 		return -1;
 	
	// m_pIRichEditOleCallback should have been created in PreSubclassWindow

 	ASSERT( m_pIRichEditOleCallback != NULL );	

	// set the IExRichEditOleCallback pointer if it wasn't set 
	// successfully in PreSubclassWindow

	if ( !m_bCallbackSet )
	{
		m_bCallbackSet = SetOLECallback( m_pIRichEditOleCallback );
	}
 	
 	return 0;
}

void COleRichEditCtrl::PreSubclassWindow() 
{
	// base class first
	CRichEditCtrl::PreSubclassWindow();	

	m_pIRichEditOleCallback = NULL;
	m_pIRichEditOleCallback = new IExRichEditOleCallback;
	ASSERT( m_pIRichEditOleCallback != NULL );

	m_bCallbackSet = SetOLECallback( m_pIRichEditOleCallback );
}

void COleRichEditCtrl::ResetVariables(bool clean/*=false*/) {

	if(clean) {
		m_saVariables.RemoveAll();
		m_saVariablesB.RemoveAll();
		m_saValores.RemoveAll();
		m_saValoresB.RemoveAll();
	} else {
		for( int i=0; i<m_saValores.GetCount(); i++)
			m_saValores.SetAt(i, "");
		for( int i=0; i<m_saValoresB.GetCount(); i++)
			m_saValoresB.SetAt(i, NULL);
	}
}

void COleRichEditCtrl::SetVariable(CString variable, CString valor) {
	
	for(int i=0;i<m_saVariables.GetCount(); i++) {
		if(m_saVariables[i]==variable) {
			m_saValores.SetAt(i, valor);
			return;
		}
	}
	m_saVariables.Add(variable);
	m_saValores.Add(valor);
}

void COleRichEditCtrl::SetBitmap(CString variable, HBITMAP bmp) {
	
	for(int i=0;i<m_saVariablesB.GetCount(); i++) {
		if(m_saVariablesB[i]==variable) {
			HBITMAP hBmp= (HBITMAP)m_saValoresB.GetAt(i);
			::DeleteObject(hBmp);
			m_saValoresB.SetAt(i, bmp);
			return;
		}
	}
	m_saVariablesB.Add(variable);
	m_saValoresB.Add(bmp);
}

long COleRichEditCtrl::SaveFile(CString file) {

	CFile archivo;
	DeleteFile(file);
	if(!archivo.Open(file, CFile::modeCreate | CFile::modeWrite))
		return 0;
	EDITSTREAM es;
	es.pfnCallback= writeFunction;
	es.dwError=0;
	es.dwCookie= (DWORD) &archivo;
	long result= StreamOut(SF_RTF, es);
	archivo.Close();
	return result;
}

int COleRichEditCtrl::SelectText(CString texto) {

	char palabra[MAX_PATH];
	FINDTEXTEX ft;
	ft.chrgText.cpMax=-1;
	ft.chrgText.cpMin=0;
	ft.chrg.cpMin = 0;
	ft.chrg.cpMax = -1;
	strcpy(palabra, texto);
	ft.lpstrText = (LPSTR)palabra;
	long n = FindText(FR_DOWN, &ft);
	if (n != -1) {
		SetSel(ft.chrgText);
	}
	return n;
}

long COleRichEditCtrl::LoadFromFile(CString file) {

	if ( !m_bCallbackSet )
	{
		ASSERT(false);
		m_bCallbackSet = SetOLECallback( m_pIRichEditOleCallback );
	}

	CFile archivo;
	if(!archivo.Open(file, CFile::modeRead))
		return 0;

	EDITSTREAM es;
	es.pfnCallback = readFunction;
	es.dwError = 0;
	es.dwCookie = (DWORD) &archivo;

	long result= StreamIn( SF_RTF, es );
	archivo.Close();

	ASSERT(m_saValores.GetCount()==m_saVariables.GetCount());
	ASSERT(m_saValoresB.GetCount()==m_saVariablesB.GetCount());

	char palabra[MAX_PATH];

	for( int i=0; i<m_saVariables.GetCount(); i++) {
		if(SelectText(m_saVariables[i])!=-1) 
			ReplaceSel(m_saValores[i]);
	}
	for(int i=0; i<m_saVariablesB.GetCount(); i++) {
		if(SelectText(m_saVariablesB[i])!=-1) {
			ReplaceSel("");
			HBITMAP bmp= (HBITMAP)m_saValoresB[i];
			if(bmp) 
				CImageDataObject::InsertBitmap(GetIRichEditOle(), bmp);
		}
	}
	return result;
}

void COleRichEditCtrl::Print(CDC* pDC, CPrintInfo* pInfo)
{
	
	// This code basically taken from MS KB article Q129860 
	
	FORMATRANGE fr;
	  int	  nHorizRes = pDC->GetDeviceCaps(HORZRES);
	  int	  nVertRes = pDC->GetDeviceCaps(VERTRES);
      int     nLogPixelsX = pDC->GetDeviceCaps(LOGPIXELSX);
	  int     nLogPixelsY = pDC->GetDeviceCaps(LOGPIXELSY);
	  LONG	      lTextLength;   // Length of document.
	  LONG	      lTextPrinted;  // Amount of document printed.

	 // Ensure the printer DC is in MM_TEXT mode.
	  pDC->SetMapMode ( MM_TEXT );

	 // Rendering to the same DC we are measuring.
	  ZeroMemory(&fr, sizeof(fr));
	  fr.hdc = fr.hdcTarget = *pDC;

	 // Set up the page.
	  fr.rcPage.left     = fr.rcPage.top = 0;
	  fr.rcPage.right    = (nHorizRes/nLogPixelsX) * 1440;
	  fr.rcPage.bottom   = (nVertRes/nLogPixelsY) * 1440;

	 // Set up 0" margins all around.
	  fr.rc.left   = fr.rcPage.left ;//+ 1440;  // 1440 TWIPS = 1 inch.
	  fr.rc.top    = fr.rcPage.top ;//+ 1440;
	  fr.rc.right  = fr.rcPage.right ;//- 1440;
	  fr.rc.bottom = fr.rcPage.bottom ;//- 1440;

	 // Default the range of text to print as the entire document.
	  fr.chrg.cpMin = 0;
	  fr.chrg.cpMax = -1;
      FormatRange(&fr,true);      
	 // Set up the print job (standard printing stuff here).
		DOCINFO di;
	  ZeroMemory(&di, sizeof(di));
	  di.cbSize = sizeof(DOCINFO);

	  di.lpszDocName = "Recepción";

	    // Do not print to file.
	     di.lpszOutput = NULL;
	     

	 // Start the document.
		 pDC->StartDoc(&di);

	 // Find out real size of document in characters.
	  lTextLength = GetTextLength();//SendMessage ( /*hRTFWnd,*/ WM_GETTEXTLENGTH, 0, 0 );
	int old=-1;
	lTextPrinted=0;
	 do
	     {
		if(old==lTextPrinted)
			break;
		old= lTextPrinted;
	     // Start the page.
		pDC->StartPage();

	    // Print as much text as can fit on a page. The return value is
	     // the index of the first character on the next page. Using TRUE
	     // for the wParam parameter causes the text to be printed.

	    lTextPrinted =FormatRange(&fr,true);
		
		DisplayBand(&fr.rc );
	    
	    // Print last page.
		pDC->EndPage();

	    // If there is more text to print, adjust the range of characters
	     // to start printing at the first character of the next page.
		if (lTextPrinted < lTextLength)
		{
		fr.chrg.cpMin = lTextPrinted;
		fr.chrg.cpMax = -1;
		}
	     }
	  while (lTextPrinted < lTextLength);

	 // Tell the control to release cached information.
	  FormatRange(NULL,false);
		  //SendMessage( EM_FORMATRANGE, 0, (LPARAM)NULL);

	  pDC->EndDoc();

	  ///////
}

long COleRichEditCtrl::StreamInFromResource(int iRes, LPCTSTR sType)
{
	HINSTANCE hInst = AfxGetInstanceHandle();
	HRSRC hRsrc = ::FindResource(hInst,
		MAKEINTRESOURCE(iRes), sType);
	
	DWORD len = SizeofResource(hInst, hRsrc); 
	BYTE* lpRsrc = (BYTE*)LoadResource(hInst, hRsrc); 
	ASSERT(lpRsrc); 
 
	CMemFile mfile;
	mfile.Attach(lpRsrc, len); 

	EDITSTREAM es;
	es.pfnCallback = readFunction;
	es.dwError = 0;
	es.dwCookie = (DWORD) &mfile;

	return StreamIn( SF_RTF, es );
}


/* static */
DWORD CALLBACK COleRichEditCtrl::readFunction(DWORD dwCookie,
		 LPBYTE lpBuf,			// the buffer to fill
		 LONG nCount,			// number of bytes to read
		 LONG* nRead)			// number of bytes actually read
{
	CFile* fp = (CFile *)dwCookie;
	*nRead = fp->Read(lpBuf,nCount);
	return 0;
}

DWORD CALLBACK COleRichEditCtrl::writeFunction(DWORD dwCookie,
		 LPBYTE lpBuf,			// the buffer to fill
		 LONG nCount,			// number of bytes to read
		 LONG* nRead)			// number of bytes actually read
{
	CFile* fp = (CFile *)dwCookie;
	fp->Write(lpBuf,nCount);
	*nRead= nCount;
	return 0;
}

/////////////////////////////////////////////////////////////////////////////

COleRichEditCtrl::IExRichEditOleCallback::IExRichEditOleCallback()
{
	pStorage = NULL;
	m_iNumStorages = 0;
	m_dwRef = 0;

	// set up OLE storage

	HRESULT hResult = ::StgCreateDocfile(NULL,
		STGM_TRANSACTED | STGM_READWRITE | STGM_SHARE_EXCLUSIVE /*| STGM_DELETEONRELEASE */|STGM_CREATE ,
		0, &pStorage );

	if ( pStorage == NULL ||
		hResult != S_OK )
	{
		AfxThrowOleException( hResult );
	}
}

COleRichEditCtrl::IExRichEditOleCallback::~IExRichEditOleCallback()
{
}

HRESULT STDMETHODCALLTYPE 
COleRichEditCtrl::IExRichEditOleCallback::GetNewStorage(LPSTORAGE* lplpstg)
{
	m_iNumStorages++;
	WCHAR tName[50];
	swprintf(tName, L"REOLEStorage%d", m_iNumStorages);

	HRESULT hResult = pStorage->CreateStorage(tName, 
		STGM_TRANSACTED | STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE ,
		0, 0, lplpstg );

	if (hResult != S_OK )
	{
		::AfxThrowOleException( hResult );
	}

	return hResult;
}

HRESULT STDMETHODCALLTYPE 
COleRichEditCtrl::IExRichEditOleCallback::QueryInterface(REFIID iid, void ** ppvObject)
{

	HRESULT hr = S_OK;
	*ppvObject = NULL;
	
	if ( iid == IID_IUnknown ||
		iid == IID_IRichEditOleCallback )
	{
		*ppvObject = this;
		AddRef();
		hr = NOERROR;
	}
	else
	{
		hr = E_NOINTERFACE;
	}

	return hr;
}



ULONG STDMETHODCALLTYPE 
COleRichEditCtrl::IExRichEditOleCallback::AddRef()
{
	return ++m_dwRef;
}



ULONG STDMETHODCALLTYPE 
COleRichEditCtrl::IExRichEditOleCallback::Release()
{
	if ( --m_dwRef == 0 )
	{
		delete this;
		return 0;
	}

	return m_dwRef;
}


HRESULT STDMETHODCALLTYPE 
COleRichEditCtrl::IExRichEditOleCallback::GetInPlaceContext(LPOLEINPLACEFRAME FAR *lplpFrame,
	LPOLEINPLACEUIWINDOW FAR *lplpDoc, LPOLEINPLACEFRAMEINFO lpFrameInfo)
{
	return S_OK;
}


HRESULT STDMETHODCALLTYPE 
COleRichEditCtrl::IExRichEditOleCallback::ShowContainerUI(BOOL fShow)
{
	return S_OK;
}



HRESULT STDMETHODCALLTYPE 
COleRichEditCtrl::IExRichEditOleCallback::QueryInsertObject(LPCLSID lpclsid, LPSTORAGE lpstg, LONG cp)
{
	return S_OK;
}


HRESULT STDMETHODCALLTYPE 
COleRichEditCtrl::IExRichEditOleCallback::DeleteObject(LPOLEOBJECT lpoleobj)
{
	return S_OK;
}



HRESULT STDMETHODCALLTYPE 
COleRichEditCtrl::IExRichEditOleCallback::QueryAcceptData(LPDATAOBJECT lpdataobj, CLIPFORMAT FAR *lpcfFormat,
	DWORD reco, BOOL fReally, HGLOBAL hMetaPict)
{
	return S_OK;
}


HRESULT STDMETHODCALLTYPE 
COleRichEditCtrl::IExRichEditOleCallback::ContextSensitiveHelp(BOOL fEnterMode)
{
	return S_OK;
}



HRESULT STDMETHODCALLTYPE 
COleRichEditCtrl::IExRichEditOleCallback::GetClipboardData(CHARRANGE FAR *lpchrg, DWORD reco, LPDATAOBJECT FAR *lplpdataobj)
{
	return S_OK;
}


HRESULT STDMETHODCALLTYPE 
COleRichEditCtrl::IExRichEditOleCallback::GetDragDropEffect(BOOL fDrag, DWORD grfKeyState, LPDWORD pdwEffect)
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE 
COleRichEditCtrl::IExRichEditOleCallback::GetContextMenu(WORD seltyp, LPOLEOBJECT lpoleobj, CHARRANGE FAR *lpchrg,
	HMENU FAR *lphmenu)
{
	return S_OK;
}






