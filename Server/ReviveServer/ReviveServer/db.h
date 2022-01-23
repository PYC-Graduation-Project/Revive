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
	void SaveData(int c_id);
	void GetLoginData(Player*, char* name);

private:
	
	void HandleDiagnosticRecord(SQLHANDLE hHandle, SQLSMALLINT hType, RETCODE RetCode);
private:
	SQLHENV henv;
	SQLHDBC hdbc;
	SQLHSTMT hstmt;

	SQLWCHAR m_id[MAX_NAME_SIZE + 1];
	SQLWCHAR m_password[MAX_NAME_SIZE + 1];

	SQLLEN cb_id, cb_password;

	SQLRETURN retcode;
	std::atomic_bool is_active=false;
	
};


