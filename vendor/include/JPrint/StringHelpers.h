# pragma once

void	MakeSaveString( CString& str );
void	UnmakeSaveString( CString& str );
int		ReplaceTag( CString& text, const CString& tag );
void	Tokenize( const CString& data, CStringArray& arr );

HANDLE	__JPRINT_EXPORT__ DDBToDIB( HBITMAP bitmap );
