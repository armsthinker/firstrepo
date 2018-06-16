#ifndef MYSQLMANAGER_H
#define MYSQLMANAGER_H
#include <iostream>
#include <winsock.h> 
#include <mysql.h>    
#include <string> 
#include <vector>
using namespace std;

class MySQLManager{
public:
	MySQLManager( );    
	MySQLManager(const char* hosts,		const char* userName, 
				 const char* password,	const char* dbName,int port);
	bool  Open( const char* hosts,		const char* userName, 
				const char* password,	const char* dbName,int port);
	virtual ~MySQLManager();

	bool Query(string query, bool bShowError = true);

	int  GetResultCount( );
	int  AddSelectResults(vector< vector<string> > &vdb);

	void Close( );
private:
	MYSQL *		m_pConn;
	bool		m_blostdbconnection;
	bool		m_bconnected;
	int			m_pingTimer;		//do a query every 4 hours to avoid being disconnected

	void		Kill();
	int			ShowError( );

};

//===========================
#endif

/*
*  usage:
*  	
	MySQLManager m_Database;
	if(!m_Database.Open("127.0.0.1", "root","pwd","sixa", 3306))
	{
		return FALSE;
	}
*
*
*/
