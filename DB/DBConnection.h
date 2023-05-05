/* DBConnection 07/01/08

		$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
		$   DBConnection.h - header   $
		$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

   by W.B. Yates 
   History: Handles DB access for MySQL

   Tested with MySQL 8.0.29 
 
*/

#ifndef __DBCONNECTION_H__
#define __DBCONNECTION_H__

struct MYSQL;
struct MYSQL_RES;


#include <string>
#include <sstream>
#include <vector>

#ifndef __DBCONFIG_H__
#include "DBConfig.h"
#endif



typedef  std::vector< std::string > DBRow;
typedef  std::vector< DBRow > ResultSet;
typedef  std::vector< std::vector< std::vector<char> > > BinaryResultSet;


class DBConnection
{
public:
	
	DBConnection( void );	
    ~DBConnection( void );

	DBConnection( const DBConfig& dbconfig );
	
	bool
	open( const DBConfig& dbconfig );
    
    bool
    isOpen(void) const  { return (m_conn != nullptr); }
	
	void
	close( void );
	
    // return false on error
	bool
	sqlQuery( const std::string& qry ); // an insert will return nothing

	bool
	sqlQuery( const std::string& qry, ResultSet& resSet ); // a select will. This function will not work with BLOB fields

	bool
	sqlQuery( const std::string& qry, BinaryResultSet& resSet ); // a select will. This function will work with BLOB fields

    long
    rowsAffected( void );
    
	std::string  
	toBlob(const std::vector<char>& msg ) const; // Convert the binary object in msg into a DB usable std::string 

	std::string
	getErrorMsg( void ) const;
	
private:


	// copy constructors disabled
    DBConnection( const DBConnection& db )=delete;
	DBConnection& operator=(const DBConnection& db)=delete;

    MYSQL*	m_conn;
	MYSQL_RES*	m_resSet;
};

#endif // __DBCONNECTION_H__

