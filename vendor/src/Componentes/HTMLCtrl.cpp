// Microsoft Systems Journal -- January 2000
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
// Compiles with Visual C++ 6.0, runs on Windows 98 and probably Windows NT too.
//
#include "StdAfx.h"
#include "HTMLCtrl.h"

#include "comdef.h"
#include "msHTML.h"

#pragma warning(disable:4146) // Si no s'avoleix això, el compilador detecta un warning... d'operador, imagino que no és greu, però d'aquesta manera el warning s'avoleix per auqest arxiu de implementació.
#pragma warning(disable:4192)
#pragma warning(disable:4049)

#import <msHTML.tlb> no_auto_exclude

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CHTMLCtrl, CHtmlView)

BEGIN_MESSAGE_MAP(CHTMLCtrl, CHtmlView)
	//{{AFX_MSG_MAP(CHTMLCtrl)
  ON_WM_DESTROY()
  ON_WM_MOUSEACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//////////////////
// Construction...
CHTMLCtrl::CHTMLCtrl()
{
	this->m_csHTMLContent = "";
	this->m_bHTMLDocumentLoadedOK = false;
	m_bNavigate=false;
}

//////////////////
// Destruction...
CHTMLCtrl::~CHTMLCtrl()
{
	this->m_csHTMLContent.ReleaseBuffer();
}

//////////////////
// Create control in same position as an existing static control with
// the same ID (could be any kind of control, really)
//
BOOL CHTMLCtrl::CreateFromStatic(UINT nID, CWnd* pParent)
{
	BOOL BRetorn = FALSE;

  CStatic wndStatic;
	CRect rc;

	DWORD dwStyle = 0;

  if (!wndStatic.SubclassDlgItem(nID, pParent)) return FALSE;

	dwStyle = wndStatic.GetStyle();

  // Get static control rect, convert to parent's client coords.
  wndStatic.GetWindowRect(&rc);
  pParent->ScreenToClient(&rc);

	wndStatic.DestroyWindow();

	// create HTML control (CHtmlView)
  BRetorn = this->Create(NULL,											// class name
												 NULL,											// title
												 dwStyle,										// style
												 rc,                        // rectangle
												 pParent,                   // parent
												 nID,                       // control ID
												 NULL);                     // frame/doc context not used

	// Parar la navegació de la pàgina web per defecte.
	this->Navigate(_T("About:Blank"),NULL,NULL,NULL,NULL);

	return BRetorn;
}

////////////////
// Override to avoid CView stuff that assumes a frame.
//
void CHTMLCtrl::OnDestroy()
{
	IHTMLDocument2Ptr pHTMLDoc2 = NULL;

	pHTMLDoc2 = this->GetHtmlDocument();

	if (pHTMLDoc2)
	{
		pHTMLDoc2->Release();
	}

  // This is probably unecessary since ~CHtmlView does it, but
  // safer to mimic CHtmlView::OnDestroy.
	if (m_pBrowserApp)
	{
		// m_pBrowserApp->Release();
		m_pBrowserApp = NULL;
  }

  CWnd::OnDestroy(); // bypass CView doc/frame stuff
}

////////////////
// Override to avoid CView stuff that assumes a frame.
//
int CHTMLCtrl::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT msg)
{
  // bypass CView doc/frame stuff
  return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, msg);
}

//////////////////
// Override navigation handler to pass to "app:" links to virtual handler.
// Cancels the navigation in the browser, since app: is a pseudo-protocol.
//
void CHTMLCtrl::OnBeforeNavigate2( LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel )
{
  const char APP_PROTOCOL[] = "app:";

  int len = _tcslen(APP_PROTOCOL);

  if (_tcsnicmp(lpszURL, APP_PROTOCOL, len) == 0)
	{
		m_bNavigate=false;
		OnAppCmd(lpszURL + len);
		*pbCancel = TRUE;
  } else {
	  CString url= lpszURL;
	  if(url!="about:Blank")
		m_bNavigate=true;
	  CHtmlView::OnBeforeNavigate2(lpszURL, nFlags, lpszTargetFrameName, baPostedData, lpszHeaders, pbCancel);
	  // Navigate2(lpszURL, nFlags, baPostedData, lpszTargetFrameName, lpszHeaders);
  }
}

void CHTMLCtrl::OnAppCmd(LPCTSTR lpszWhere)
{
  // default: do nothing
}

void CHTMLCtrl::OnDocumentComplete(LPCTSTR lpszURL)
{
	CHtmlView::OnDocumentComplete(lpszURL);
	if(!m_bNavigate)
		this->SetHTMLInDocument();
	m_bHTMLDocumentLoadedOK = true;	
	// esto es para obtener el foco.
	/*GetNextWindow(GW_CHILD)->GetNextWindow(GW_CHILD)->GetNextWindow(GW_CHILD)->PostMessage(WM_LBUTTONDOWN, 0, MAKEWORD(1,1));
	GetNextWindow(GW_CHILD)->GetNextWindow(GW_CHILD)->GetNextWindow(GW_CHILD)->PostMessage(WM_LBUTTONUP, 0, MAKEWORD(1,1));*/
	CWnd *parent = GetParent();

	if (parent != NULL) {

		Notification	notification(m_hWnd,GetDlgCtrlID(),DocumentComplete);

		notification.URL = lpszURL;

		LRESULT result = parent->SendMessage(WM_NOTIFY,
			notification.hdr.idFrom,
			(LPARAM)&notification);

	}
}

bool CHTMLCtrl::ParseHTMLString(CString &csCadenaATractar)
{
	csCadenaATractar.Replace("à","&agrave;");
	csCadenaATractar.Replace("á","&aacute;");
	csCadenaATractar.Replace("è","&egrave;");
	csCadenaATractar.Replace("é","&eacute;");
	csCadenaATractar.Replace("ì","&igrave;");
	csCadenaATractar.Replace("í","&iacute;");
	csCadenaATractar.Replace("ò","&ograve;");
	csCadenaATractar.Replace("ó","&oacute;");
	csCadenaATractar.Replace("ù","&ugrave;");
	csCadenaATractar.Replace("ú","&uacute;");

	return true;
}

bool CHTMLCtrl::SetNewHTMLContent(CString csHTML, bool bReformat /*=false*/)
{
	//------------------------------------------------
	// Reassign the string.
	//------------------------------------------------
	this->m_csHTMLContent = csHTML;

	//------------------------------------------------
	// Reformat the string if needed.
	//------------------------------------------------
	if (bReformat)	ParseHTMLString(this->m_csHTMLContent);

	this->m_csHTMLContent.ReleaseBuffer();

	//------------------------------------------------
	// If the document has been loaded OK then the HTML
	// content gets modified now.
	//------------------------------------------------
	if (this->m_bHTMLDocumentLoadedOK) return this->SetHTMLInDocument();

	return true;
}

bool CHTMLCtrl::SetHTMLInDocument()
{
	SAFEARRAY *pSafeArray = NULL;

	VARIANT *pElement = NULL;

	IHTMLDocument2Ptr pHTMLDoc2 = NULL;
	LPDISPATCH lpDispatch = NULL;

	HRESULT hr;

	bool bRetorn = false;

	// Processat del document HTML, assignació de punters
	// i escriptura del contingut de la cadena de text.
	lpDispatch = this->GetHtmlDocument();

	if (lpDispatch != NULL)
	{
		hr = lpDispatch->QueryInterface(IID_IHTMLDocument2, (LPVOID*)&pHTMLDoc2);

		if (!SUCCEEDED(hr))
		{
			bRetorn = false;
		}
		else
		{
			pSafeArray = ::SafeArrayCreateVector(VT_VARIANT, 0, 1);

			if (pSafeArray)
			{
				hr = ::SafeArrayAccessData(pSafeArray,(LPVOID*) &pElement);

				if (!SUCCEEDED(hr))
				{
					bRetorn = false;
				}
				else
				{
					pElement->vt				= VT_BSTR;
					pElement->bstrVal		= this->m_csHTMLContent.AllocSysString();

					hr = SafeArrayUnaccessData(pSafeArray);

					if (!SUCCEEDED(hr))
					{
						bRetorn = false;
					}
					else
					{
						pHTMLDoc2->clear();
						pHTMLDoc2->close();

						hr = pHTMLDoc2->write(pSafeArray);

						if (!SUCCEEDED(hr))
						{
							bRetorn = false;
						}
					}
					SysFreeString(pElement->bstrVal);
				}
				if (pSafeArray) SafeArrayDestroy(pSafeArray);
			}
			if (pHTMLDoc2)
			{
				pHTMLDoc2->close();
				pHTMLDoc2.Release();
			}
		}

		lpDispatch->Release();
	}

	return bRetorn;
}

//----------------------------------------------
// INIT MICROSOFT BUG CORRECTION.
//----------------------------------------------
CString CHTMLCtrl::GetFullName() const
{
	ASSERT(m_pBrowserApp != NULL);

	BSTR bstr;
	m_pBrowserApp->get_FullName(&bstr);
	CString retVal(bstr);

	SysFreeString(bstr); // Added this line to prevent leak.
	return retVal;
}

CString CHTMLCtrl::GetType() const
{
	ASSERT(m_pBrowserApp != NULL);

	BSTR bstr;
	m_pBrowserApp->get_Type(&bstr);

	CString retVal(bstr);

	SysFreeString(bstr); // Added this line to prevent leak.
	return retVal;
}

CString CHTMLCtrl::GetLocationName() const
{
	ASSERT(m_pBrowserApp != NULL);

	BSTR bstr;
	m_pBrowserApp->get_LocationName(&bstr);
	CString retVal(bstr);

	SysFreeString(bstr); // Added this line to prevent leak.
	return retVal;
}

CString CHTMLCtrl::GetLocationURL() const
{
	ASSERT(m_pBrowserApp != NULL);

	BSTR bstr;
	m_pBrowserApp->get_LocationURL(&bstr);
	CString retVal(bstr);

	SysFreeString(bstr); // Added this line to prevent leak.
	return retVal;
}

void CHTMLCtrl::Navigate(LPCTSTR lpszURL, DWORD dwFlags /* = 0 */,
														LPCTSTR lpszTargetFrameName /* = NULL */ ,
														LPCTSTR lpszHeaders /* = NULL */, LPVOID lpvPostData /* = NULL */,
														DWORD dwPostDataLen /* = 0 */)
{
	CString strURL(lpszURL);
	BSTR bstrURL = strURL.AllocSysString();

	COleSafeArray vPostData;
	if (lpvPostData != NULL)
	{
		if (dwPostDataLen == 0) dwPostDataLen = lstrlen((LPCTSTR) lpvPostData);
		vPostData.CreateOneDim(VT_UI1, dwPostDataLen, lpvPostData);
	}

	m_pBrowserApp->Navigate(bstrURL,
													COleVariant((long) dwFlags, VT_I4),
													COleVariant(lpszTargetFrameName, VT_BSTR),
													vPostData,
													COleVariant(lpszHeaders, VT_BSTR));

	SysFreeString(bstrURL); // Added this line to prevent leak.
}

BOOL CHTMLCtrl::LoadFromResource(LPCTSTR lpszResource)
{
	HINSTANCE hInstance = AfxGetResourceHandle();
	ASSERT(hInstance != NULL);

	CString strResourceURL;
	BOOL bRetVal = TRUE;
	LPTSTR lpszModule = new TCHAR[_MAX_PATH];

	if (GetModuleFileName(hInstance, lpszModule, _MAX_PATH))
	{
		 strResourceURL.Format(_T("res://%s/%s"), lpszModule, lpszResource);
		 Navigate(strResourceURL, 0, 0, 0);
	}
	else
	{
		bRetVal = FALSE;
	}

	delete [] lpszModule;
	return bRetVal;
}

BOOL CHTMLCtrl::LoadFromResource(UINT nRes)
{
	HINSTANCE hInstance = AfxGetResourceHandle();
	ASSERT(hInstance != NULL);

	CString strResourceURL;
	BOOL bRetVal = TRUE;
	LPTSTR lpszModule = new TCHAR[_MAX_PATH];

	if (GetModuleFileName(hInstance, lpszModule, _MAX_PATH))
	{
		 strResourceURL.Format(_T("res://%s/%d"), lpszModule, nRes);
		 Navigate(strResourceURL, 0, 0, 0);
	}
	else
	{
		bRetVal = FALSE;
	}

	delete [] lpszModule;
	return bRetVal;
}

//----------------------------------------------
// END MICROSOFT BUG CORRECTION.
//----------------------------------------------

Notification::Notification(HWND hwnd,UINT ID,CHTMLCtrl::NotificationType type)
{
	hdr.hwndFrom	= hwnd;
	hdr.idFrom		= ID;
	hdr.code		= type;
	URL				= _T("");
	frame			= _T("");
	post_data		= NULL;
	post_data_size	= 0;
	headers			= _T("");
	progress		= 0;
	progress_max	= 0;
	text			= _T("");
}
