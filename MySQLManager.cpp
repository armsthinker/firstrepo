#include "MySQLManager.h"


MySQLManager::MySQLManager( )
{
	m_pConn		= NULL;
	m_pingTimer = 0;
	m_blostdbconnection = false;
	m_bconnected		= false;
}


MySQLManager::MySQLManager(const char* hosts,		const char* userName,
						   const char* password,	const char* dbName,int port )
{
	//Kill();
	m_pConn = mysql_init(NULL);
	if( !m_pConn ){ m_bconnected = false; ShowError(); return;}
	if (!mysql_real_connect(m_pConn, hosts, userName, password, dbName, port, NULL, 0))
	{
		m_bconnected = false;
	}else{
		m_bconnected = true;
	}

}


bool MySQLManager::Open(const char* hosts,		const char* userName, 
						const char* password,	const char* dbName,int port)
{
	
	m_pConn = mysql_init(NULL);
	
	if( !m_pConn ){ m_bconnected =false; ShowError( ); return false;}
	if (!mysql_real_connect(m_pConn, hosts, userName, password, dbName, port, NULL, 0))
	{
		m_bconnected =false;
		ShowError( );
	}else{
		mysql_set_character_set( m_pConn, "gb2312");
		m_bconnected = true;
	}	
	return m_bconnected;
}


MySQLManager::~MySQLManager( )
{
	Kill();
}


bool MySQLManager::Query(string query, bool bShowError /* = true */)
{
	if (mysql_query(m_pConn, query.c_str()))
	{
		if (bShowError){
			int error = ShowError();
			if (error == 2006) {//CR_SERVER_GONE_ERROR
				m_blostdbconnection = true;//mysql service probably died, requiring a restart
			}
		}
		return false;
	}
	return true;
}


int MySQLManager::GetResultCount( )
{
	MYSQL_RES *result = NULL;
	result = mysql_store_result(m_pConn);
	if (!result){ return 0;}
	int num_rows   = mysql_num_rows  ( result );

	mysql_free_result(result);
	return num_rows;
}

//
int  MySQLManager::AddSelectResults(vector< vector<string> > &vdb)
{
	MYSQL_RES *result = NULL;
	result = mysql_store_result(m_pConn);
	if (!result){ return 0;}

	int num_fields = mysql_num_fields( result );
	int num_rows   = mysql_num_rows  ( result );
	MYSQL_ROW		row;
	MYSQL_FIELD*	field;

	vector<string>				fieldNames;
	vector<enum_field_types>	fieldType;  //mysql_com.h  line 342
	vector<int>					maxLength;
	//int curRow = (int)vdb.size();
	//vdb.resize(curRow+num_rows);

	while ((row = mysql_fetch_row(result))){
		vector<string> vrow;
		for(int i = 0; i <num_fields; i++)
		{
			if (i == 0) 
			{
				while(field = mysql_fetch_field(result)) 
				{
					fieldNames.push_back(field->name);
					fieldType.push_back(field->type);
					maxLength.push_back(field->max_length);
				}
			}
			vrow.push_back( row[i]==NULL?"":row[i] ); //¥¶¿ÌNULL
		}
		vdb.push_back(vrow);
	}

	mysql_free_result(result);
	return num_rows;
}


void MySQLManager::Close()
{
	if (m_pConn)
	{
		mysql_close(m_pConn);
		m_pConn = NULL;
		m_bconnected = false;
	}
}


void MySQLManager::Kill()
{
	if (m_pConn)
	{
		mysql_close(m_pConn);
		m_pConn = NULL;
		m_bconnected = false;
		
	}
}


int MySQLManager::ShowError()
{
	int error = mysql_errno(m_pConn);
	return error;
}

