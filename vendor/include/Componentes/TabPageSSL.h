//***************************************************************************
//
// AUTHOR:  JUHU
//
// DESCRIPTION: Forms para TabCtrlSSL
//
// ORIGINAL: 
//
//***************************************************************************
# pragma once

class __COMPONENTES_EXPORT__ CTabPageSSL : public CDialog
{
// Construction
public:
	//***********************************************************************
	// Name:		CTabPageSSL
	// Description:	Constructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************	
	CTabPageSSL ();
	//***********************************************************************
	// Name:		CTabPageSSL
	// Description:	Constructor.
	// Parameters:	UINT nIDTemplate, CWnd* pParent = NULL.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************	
	CTabPageSSL (UINT nIDTemplate, CWnd* pParent = NULL);
	//***********************************************************************
	// Name:		CTabPageSSL
	// Description:	Destructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************	
	~CTabPageSSL ();

// Attributes
protected:
	// Routing flags
	bool m_bRouteCommand;
	bool m_bRouteCmdMsg;
	bool m_bRouteNotify;

// Operations
public:
	//***********************************************************************
	// Name:		EnableRouteCommand
	// Description:	Enable/disable command routing to the parent.
	// Parameters:	bool bRoute = true.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************	
	void EnableRouteCommand(bool bRoute = true) { m_bRouteCommand = bRoute; };
	//***********************************************************************
	// Name:		IsRouteCommand
	// Description:	Enable/disable command routing to the parent?.
	// Parameters:	None.
	// Return:		bool.	
	// Notes:		return m_bRouteCommand.
	//***********************************************************************	
	bool IsRouteCommand() { return m_bRouteCommand; };
	//***********************************************************************
	// Name:		EnableRouteCmdMsg
	// Description:	Enable CmdMsg routing to the parent.
	// Parameters:	bool bRoute = true.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************	
	void EnableRouteCmdMsg(bool bRoute = true) { m_bRouteCmdMsg = bRoute; };
	//***********************************************************************
	// Name:		IsRouteCmdMsg
	// Description:	Enable CmdMsg routing to the parent?.
	// Parameters:	None.
	// Return:		bool.	
	// Notes:		return m_bRouteCmdMsg.
	//***********************************************************************	
	bool IsRouteCmdMsg() { return m_bRouteCmdMsg; };
	//***********************************************************************
	// Name:		EnableRouteNotify
	// Description:	Enable/Disable Notify routing to the parent.
	// Parameters:	bool bRoute = true.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************	
	void EnableRouteNotify(bool bRoute = true) { m_bRouteNotify = bRoute; };
	//***********************************************************************
	// Name:		IsRouteNotify
	// Description:	Enable/Disable Notify routing to the parent?.
	// Parameters:	None.
	// Return:		bool.	
	// Notes:		return m_bRouteNotify.
	//***********************************************************************	
	bool IsRouteNotify() { return m_bRouteNotify; };

protected:
// Message Handlers
	virtual BOOL OnCommand (WPARAM wParam, LPARAM lParam);
	virtual BOOL OnNotify (WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual void OnOK (void);
	virtual void OnCancel (void);
	virtual BOOL OnCmdMsg (UINT nID, int nCode, void* pExtra,
		AFX_CMDHANDLERINFO* pHandlerInfo);

};
