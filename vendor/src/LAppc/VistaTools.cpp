# include "stdafx.h"
#include "VistaTools.h"
#include <comdef.h>
#include <taskschd.h>

# pragma comment(lib, "taskschd.lib")
# pragma comment(lib, "comsupp.lib")
# pragma comment(lib, "credui.lib")

CVistaTools::CVistaTools(void)
{
}

CVistaTools::~CVistaTools(void)
{
}

BOOL 
MyShellExec(HWND hwnd, 
			LPCTSTR pszVerb, 
			LPCTSTR pszPath, 
			LPCTSTR pszParameters = NULL,
			LPCTSTR pszDirectory = NULL )
{
	SHELLEXECUTEINFO shex;

	memset( &shex, 0, sizeof( shex) );

	shex.cbSize			= sizeof( SHELLEXECUTEINFO ); 
	shex.fMask			= 0; 
	shex.hwnd			= hwnd;
	shex.lpVerb			= pszVerb; 
	shex.lpFile			= pszPath; 
	shex.lpParameters	= pszParameters; 
	shex.lpDirectory	= pszDirectory; 
	shex.nShow			= SW_NORMAL; 

	return ::ShellExecuteEx( &shex );
}

HRESULT CVistaTools::GetElevationType( __out TOKEN_ELEVATION_TYPE * ptet )
{
	ASSERT( ptet );

	HRESULT hResult = E_FAIL; // assume an error occured
	HANDLE hToken	= NULL;

	if ( !::OpenProcessToken( 
		::GetCurrentProcess(), 
		TOKEN_QUERY, 
		&hToken ) )
	{
		ASSERT( FALSE );
		return hResult;
	}

	DWORD dwReturnLength = 0;

	if ( !::GetTokenInformation(
		hToken,
		TokenElevationType,
		ptet,
		sizeof( *ptet ),
		&dwReturnLength ) )
	{
		ASSERT( FALSE );
	}
	else
	{
		ASSERT( dwReturnLength == sizeof( *ptet ) );
		hResult = S_OK;
	}

	::CloseHandle( hToken );

	return hResult;
}

HRESULT CVistaTools::IsElevated( __out_opt BOOL * pbElevated ) //= NULL )
{
	HRESULT hResult = E_FAIL; // assume an error occured
	HANDLE hToken	= NULL;

	if ( !::OpenProcessToken( 
		::GetCurrentProcess(), 
		TOKEN_QUERY, 
		&hToken ) )
	{
		ASSERT( FALSE );
		return hResult;
	}

	TOKEN_ELEVATION te = { 0 };
	DWORD dwReturnLength = 0;

	if ( !::GetTokenInformation(
		hToken,
		TokenElevation,
		&te,
		sizeof( te ),
		&dwReturnLength ) )
	{
		ASSERT( FALSE );
	}
	else
	{
		ASSERT( dwReturnLength == sizeof( te ) );

		hResult = te.TokenIsElevated ? S_OK : S_FALSE; 

		if ( pbElevated)
			*pbElevated = (te.TokenIsElevated != 0);
	}

	::CloseHandle( hToken );

	return hResult;
}

//////////////////////////////////////////////////////////////////////
// RunAsStdUser is based on the MSDN sample "Registration Trigger Example" 
//
// First, some helper classes, to make the interfaces release themselves on exit.
// Sure, we could use the templates for that, but let's take it easy here. 

class ITaskServiceHelper
{
public:
	ITaskServiceHelper()		{	p = NULL;	}
	~ITaskServiceHelper()		{	if ( p ) p->Release();	}

	ITaskService * p;
};

class ITaskFolderHelper
{
public:
	ITaskFolderHelper()			{	p = NULL;	}
	~ITaskFolderHelper()		{	if ( p ) p->Release();	}

	ITaskFolder * p;
};

class ITaskDefinitionHelper
{
public:
	ITaskDefinitionHelper()		{	p = NULL;	}
	~ITaskDefinitionHelper()	{	if ( p ) p->Release();	}

	ITaskDefinition * p;
};

class IRegistrationInfoHelper
{
public:
	IRegistrationInfoHelper()	{	p = NULL;	}
	~IRegistrationInfoHelper()	{	if ( p ) p->Release();	}

	IRegistrationInfo * p;
};

class IPrincipalHelper
{
public:
	IPrincipalHelper()			{	p = NULL;	}
	~IPrincipalHelper()			{	if ( p ) p->Release();	}

	IPrincipal * p;
};

class ITaskSettingsHelper
{
public:
	ITaskSettingsHelper()		{	p = NULL;	}
	~ITaskSettingsHelper()		{	if ( p ) p->Release();	}

	ITaskSettings * p;
};

class ITriggerCollectionHelper
{
public:
	ITriggerCollectionHelper()	{	p = NULL;	}
	~ITriggerCollectionHelper()	{	if ( p ) p->Release();	}

	ITriggerCollection * p;
};

class ITriggerHelper
{
public:
	ITriggerHelper()			{	p = NULL;	}
	~ITriggerHelper()			{	if ( p ) p->Release();	}

	ITrigger * p;
};

class IRegistrationTriggerHelper
{
public:
	IRegistrationTriggerHelper(){	p = NULL;	}
	~IRegistrationTriggerHelper(){	if ( p ) p->Release();	}

	IRegistrationTrigger * p;
};

class IActionCollectionHelper
{
public:
	IActionCollectionHelper()	{	p = NULL;	}
	~IActionCollectionHelper()	{	if ( p ) p->Release();	}

	IActionCollection * p;
};

class IActionHelper
{
public:
	IActionHelper()				{	p = NULL;	}
	~IActionHelper()			{	if ( p ) p->Release();	}

	IAction * p;
};

class IExecActionHelper
{
public:
	IExecActionHelper()			{	p = NULL;	}
	~IExecActionHelper()		{	if ( p ) p->Release();	}

	IExecAction * p;
};

class IRegisteredTaskHelper
{
public:
	IRegisteredTaskHelper()		{	p = NULL;	}
	~IRegisteredTaskHelper()	{	if ( p ) p->Release();	}

	IRegisteredTask * p;
};

///////////////////////////////////////////////////////////////////
// DO is a handy macro that makes the code a bit more readable:
// it checks the result of action and returns if tehre is an error.
// Sure, we could also use throw/catch, etc., but lets keep things simple here.

#define DO( action )		\
	if( FAILED( action ) )	\
{						\
	ASSERT( FALSE );	\
	return FALSE;		\
}

/////////////////////////////////////////////
// now the function itself:

BOOL CVistaTools::RunAsStdUser(__in		HWND	hwnd, __in	LPCTSTR pszPath, __in_opt	LPCTSTR pszParameters /*= NULL*/, __in_opt	LPCTSTR pszDirectory ) //	= NULL );
{
	ASSERT( pszPath );

	if ( S_FALSE == IsElevated() ) 
	{
		// if the current process is not elevated, we can use ShellExecuteAs directly! 

		return MyShellExec( hwnd, 
			NULL, 
			pszPath, 
			pszParameters, 
			pszDirectory );
	}

	// If process is elevated, we ask Task Scheduler to start a task for us.
	// I wish Microsoft would have provided an easier way for starting a non-elevated task,
	// like another verb for ShellExecuteEx, but they chose not to. 
	// I hope they had good reasons for that.

	HRESULT hr = 0;

	//  Choose a name for the task.

	LPCWSTR pszTaskName = L"RunAsStdUser Task";

	//  Create an instance of the Task Service. 

	ITaskServiceHelper iService;

	DO( hr = CoCreateInstance( CLSID_TaskScheduler,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_ITaskService,
		(void**)&iService.p ) )

		//  Connect to the task service.
		DO( iService.p->Connect( _variant_t(), _variant_t(), _variant_t(), _variant_t()) )

		//  Get the pointer to the root task folder.  This folder will hold the
		//  new task that is registered.

		ITaskFolderHelper iRootFolder;

	DO( iService.p->GetFolder( _bstr_t( L"\\") , &iRootFolder.p ) )

		//  If the same task exists, remove it.
		iRootFolder.p->DeleteTask( _bstr_t( pszTaskName), 0  ); // ignore error message, if any

	//  Create the task builder object to create the task.

	ITaskDefinitionHelper iTask;

	DO( iService.p->NewTask( 0, &iTask.p ) )

		//  Get the registration info for setting the identification.

		IRegistrationInfoHelper iRegInfo;

	DO( iTask.p->get_RegistrationInfo( &iRegInfo.p ) )

		DO( iRegInfo.p->put_Author( L"RunAsStdUser" ) )

		//  Create the principal for the task

		IPrincipalHelper iPrincipal;

	DO( iTask.p->get_Principal( &iPrincipal.p ) )

		//  Set up principal information: 
		DO( iPrincipal.p->put_Id( _bstr_t(L"RunAsStdUser_Principal") ) )

		DO( iPrincipal.p->put_LogonType( TASK_LOGON_INTERACTIVE_TOKEN ) )

		//  Run the task with the least privileges (LUA) 
		DO( iPrincipal.p->put_RunLevel( TASK_RUNLEVEL_LUA ) )

		//  Create the settings for the task

		ITaskSettingsHelper iSettings;

	DO( iTask.p->get_Settings( &iSettings.p ) )

		//  Set setting values for the task.
		DO( iSettings.p->put_StartWhenAvailable(VARIANT_BOOL(true)) )

		//  Get the trigger collection to insert the registration trigger.

		ITriggerCollectionHelper iTriggerCollection;

	DO( iTask.p->get_Triggers( &iTriggerCollection.p ) )

		//  Add the registration trigger to the task.

		ITriggerHelper iTrigger;

	DO( iTriggerCollection.p->Create( TASK_TRIGGER_REGISTRATION, &iTrigger.p ) )

		IRegistrationTriggerHelper iRegistrationTrigger;

	DO( iTrigger.p->QueryInterface( 
		IID_IRegistrationTrigger, (void**) &iRegistrationTrigger.p ) )

		DO( iRegistrationTrigger.p->put_Id( _bstr_t( L"RunAsStdUser_Trigger" ) ) )

		//  Define the delay for the registration trigger.
		DO( iRegistrationTrigger.p->put_Delay( L"PT0S" ) ) // 0 second delay of execution; 

		//  Add an Action to the task. This task will execute notepad.exe.     

		IActionCollectionHelper iActionCollection;

	//  Get the task action collection pointer.
	DO( iTask.p->get_Actions( &iActionCollection.p ) )

		//  Create the action, specifying that it is an executable action.
		IActionHelper iAction;

	DO( iActionCollection.p->Create( TASK_ACTION_EXEC, &iAction.p ) )

		IExecActionHelper iExecAction;

	//  QI for the executable task pointer.
	DO( iAction.p->QueryInterface( 
		IID_IExecAction, (void**) &iExecAction.p ) )

		//  Set the path of the executable to notepad.exe.
		DO( iExecAction.p->put_Path( _bstr_t( pszPath ) ) )

		if( pszParameters )
		{
			DO( iExecAction.p->put_Arguments( _bstr_t( pszParameters ) ) )
		}

		if( pszDirectory )
		{
			DO( iExecAction.p->put_WorkingDirectory( _bstr_t( pszDirectory ) ) )
		}

		//  Save the task in the root folder.

		IRegisteredTaskHelper iRegisteredTask;

		DO( iRootFolder.p->RegisterTaskDefinition(
			_bstr_t( pszTaskName ),
			iTask.p,
			TASK_CREATE_OR_UPDATE, 
			_variant_t(), 
			_variant_t(), 
			TASK_LOGON_INTERACTIVE_TOKEN,
			_variant_t(L""),
			&iRegisteredTask.p) )

			return TRUE;
}

////////////////////////////////
// RunElevated simply calls ShellExecuteEx with the verb "runas" to start the elevated process.
// I wish there was a just as easy way to start a non-elevated process, as well.

BOOL CVistaTools::RunElevated(__in	HWND hwnd, __in	LPCTSTR pszPath, __in_opt LPCTSTR pszParameters /*= NULL*/, __in_opt	LPCTSTR pszDirectory ) //	= NULL );
{
	return MyShellExec( hwnd, _T("runas"), pszPath, pszParameters, pszDirectory );
}
