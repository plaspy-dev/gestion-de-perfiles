 // Microsoft Systems Journal -- January 2000
 // If this code works, it was written by Paul DiLascia.
 // If not, I don't know who wrote it.
 // Compiles with Visual C++ 6.0, runs on Windows 98 and probably Windows NT too.
 //
#ifndef _HTML_CTRL_
#define _HTML_CTRL_

# include <afxhtml.h>


class __COMPONENTES_EXPORT__ CHTMLCtrl : public CHtmlView 
{
public:
	
	CHTMLCtrl();
	~CHTMLCtrl();

	// Generated message map functions
	//{{AFX_MSG(CHTMLCtrl)
	// overrides to bypass MFC doc/view frame dependencies
	afx_msg void OnDestroy();
	afx_msg int  OnMouseActivate(CWnd* pDesktopWnd,UINT nHitTest,UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CHTMLCtrl)

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHTMLCtrl)
public:
	virtual void OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel);
	virtual void PostNcDestroy() {};
	virtual void OnAppCmd(LPCTSTR lpszWhere);
	virtual void OnDocumentComplete(LPCTSTR lpszURL);
	//}}AFX_VIRTUAL
	enum NotificationType {					// Note: SimpleBrowser does NOT support the
		//       common notifications (NM_CLICK, etc.)
		BeforeNavigate2,					// set *LRESULT=TRUE to cancel navigation
		DocumentComplete,				
		DownloadBegin,
		ProgressChange,
		DownloadComplete,
		NavigateComplete2,
		StatusTextChange,
		TitleChange

	};

	//----------------------------------------------
	// INIT. MICROSOFT BUG CORRECTION
	//----------------------------------------------
	CString GetFullName() const;
	CString GetType() const;
	CString GetLocationName() const;
	CString GetLocationURL() const;

	void Navigate(LPCTSTR lpszURL, DWORD dwFlags = 0 ,  
								LPCTSTR lpszTargetFrameName = NULL ,  
								LPCTSTR lpszHeaders = NULL, LPVOID lpvPostData = NULL,  
								DWORD dwPostDataLen = 0);

	//Add the following functions. This action prevents the leak because the 
	//CHtmlView versions of LoadFromResource() call CHtmlView::Navigate().
	//These need to be defined because CHtmlView::Navigate() is not virtual.
	BOOL LoadFromResource(LPCTSTR lpszResource);
	BOOL LoadFromResource(UINT nRes);

	//----------------------------------------------
	// END MICROSOFT BUG CORRECTION
	//----------------------------------------------

	
	// Methods...
	BOOL CreateFromStatic(UINT nID, CWnd* pParent);

	bool SetNewHTMLContent(CString csHTML, bool bReformat = false);

	bool ParseHTMLString(CString &csCadenaATractar);

private:

	// Functions:
	bool SetHTMLInDocument();


	// Members.
	CString m_csHTMLContent;

	bool	m_bHTMLDocumentLoadedOK;
	bool	m_bNavigate;
};

class Notification {					// all notifications pass this structure

public:

	Notification(HWND hwnd,UINT ID,CHTMLCtrl::NotificationType type);

	NMHDR			hdr;				// hdr.hwndFrom = SimpleBrowser's HWND
	// hdr.idFrom   = SimpleBrowser's control ID
	// hdr.code     = <NavigationType>

	CString			URL;				// BeforeNavigate2
	// DocumentComplete
	// NavigateComplete2

	CString			frame;				// BeforeNavigate2
	void			*post_data;			// BeforeNavigate2
	int				post_data_size;
	CString			headers;			// BeforeNavigate2

	int				progress;			// ProgressChange
	int				progress_max;		// ProgressChange

	CString			text;				// StatusTextChange
	// TitleChange

};

#endif

