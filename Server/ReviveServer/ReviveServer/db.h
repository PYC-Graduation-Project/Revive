#pragma once
#include <sqlext.h> 
#include<atomic>
#include"define.h"

class DB
{
public:
	DB() ;
	~DB();

	bool Init();
	LOGINFAIL_TYPE SaveData(char*,char*);
	LOGINFAIL_TYPE CheckLoginData( char* name, char* password);
	bool CompWcMc(wchar_t*, char*);
private:
	
	void HandleDiagnosticRecord(SQLHANDLE hHandle, SQLSMALLINT hType, RETCODE RetCode);
private:
	SQLHENV henv;
	SQLHDBC hdbc;
	SQLHSTMT hstmt;

	SQLWCHAR m_id[MAX_NAME_SIZE + 1];
	SQLWCHAR m_password[MAX_PASSWORD_SIZE + 1];

	SQLLEN cb_id, cb_password;

	SQLRETURN retcode;
	std::atomic_bool is_active=false;
	
};


