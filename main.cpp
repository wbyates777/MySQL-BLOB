/* MySQLBlob 03/05/2023

 $$$$$$$$$$$$$$$$$
 $   MySQLBlob   $
 $$$$$$$$$$$$$$$$$

 by W.B. Yates
 Copyright (c) W.B. Yates. All rights reserved.
 History:

 An example of Binary Large Object (BLOB) database for arbitrary classes built on MySQL
 
 // VARCHAR(64) used because of MD5 hash
 
 CREATE TABLE IF NOT EXISTS object 
 ( 
      name     VARCHAR(64) NOT NULL, 
      class    VARCHAR(64) NOT NULL, 
      ptype    TINYINT,
      contents LONGBLOB, 
      added TIMESTAMP DEFAULT CURRENT_TIMESTAMP not null, 
      PRIMARY KEY (name, class)
 ) ENGINE InnoDB;
 
*/


#ifndef __DBCONNECTION_H__
#include "DBConnection.h"
#endif

#ifndef __BINARYPACKAGE_H__
#include "BinaryPackage.h"
#endif

#ifndef __TRADE_H__
#include "Trade.h"
#endif

#include <iostream>

Trade
createTrade( int portfolio, int tradeId )
{
    int flowId = 0;
    ValId tid = ValIdType::setId(portfolio, tradeId, flowId);
    Date  start(1,5,2023);
    Date  maturity = start + 180;
    Point amount  = 1;
    Cashflow price(flowId, start, 95.0, "USD", Cashflow::FIXED) ;
    
    Point coupon = 5.0;
    Date  flowDate = start;
    std::vector<Cashflow> cashflows; 
    for (int i = 0; i < 5; ++i)
    {
        cashflows.push_back( Cashflow(++flowId, flowDate += 30, coupon, "USD", Cashflow::FIXED) );
    }
    cashflows.push_back( Cashflow(flowId, maturity, 100.0, "USD", Cashflow::FIXED) );
    
    Trade trade( tid,        // unique ID
                 "Bond",     // trade type
                 price,      // price per unit of the product at which we bought or sold; always positive
                 amount,     // +ve we buy/we are long, -ve we sold/we are short 
                 start,      // start date - for a one day, one-off trade, start and maturity dates are equal
                 maturity ); // end date 
    
    trade.city("NYC");
    trade.ccys( std::vector<Currency>(1, Currency("USD")) );
    trade.sourceId( "src_123" ); 
    trade.cashflows(cashflows);

    return trade;
}

bool
saveDB(DBConnection& dbc, const Persistent& pobj, const std::string& table)
{
    std::string insStr = "insert into " + table + " values";  

    // Note MySQL has syntax to insert mutiple rows at once; more efficient
    insStr += "('";
    insStr += pobj.name();
    insStr += "','";
    insStr += pobj.className();
    insStr += "',";
    insStr += std::to_string(int(pobj.persistAs()));
    insStr += ",'";
    
    // add the persistent object data to the insert string
    if ( pobj.persistAs() == Persistent::TEXT )
    {
        std::stringstream ss;
        ss << pobj;
        insStr += ss.str();
    }
    else
    {
        BinaryPackage bp(pobj);
        insStr += dbc.toBlob( bp.data() );
    }
    
    insStr += "', CURRENT_TIMESTAMP); ";

    bool ok = dbc.sqlQuery( insStr );
    
    if (!ok)
        std::cout << dbc.getErrorMsg() << std::endl;
    
    if (ok)
        std::cout << "Object [" << pobj.name() << "] saved to database table [" << table << "]" << std::endl;
    else std::cout << "Object ["  << pobj.name() << "] could not be saved to database table [" << table << "]" << std::endl;
    
    return ok;
}

bool
loadDB( DBConnection& dbc, Persistent& pobj, const std::string& table )
{
    bool ok = false;
    
    std::string str;
    str = "select contents from " + table + " where name = '";
    str += pobj.name();
    str += "' and class = '";
    str += pobj.className();
    str += "';";
    
    if (pobj.persistAs() == Persistent::TEXT)
    {
        ResultSet resSet;
        ok = dbc.sqlQuery( str, resSet );

        if (!ok)
            std::cout  << dbc.getErrorMsg() << std::endl;
        
        if (!resSet.empty())
        {
            std::istringstream s(resSet[0][0]);
            s >> pobj;
        }
        else ok = false;
    }
    else
    {
        BinaryResultSet resSet;
        ok = dbc.sqlQuery( str, resSet );
        
        if (!ok)
            std::cout  << dbc.getErrorMsg() << std::endl;
        
        if (!resSet.empty())
        {
            BinaryPackage bp(resSet[0][0]);
            bp >> pobj;
        }
        else ok = false;
    }
    
    if (ok)
        std::cout << "Object [" << pobj.name() << "] loaded from database table [" << table << "]" << std::endl;
    else std::cout << "Object [" << pobj.name() << "] could not be found in database table [" << table << "]" << std::endl;
    
    return ok;
}


int 
main(int argc, const char * argv[]) 
{
    std::cout << "MySQL BLOB Database Demo\n" << std::endl;

    //
    // open a connection to our database
    //
    
    DBConfig dbconfig;
    dbconfig.load("MyPath/dbconfig.txt");
    
    DBConnection dbc;
    bool ok = dbc.open( dbconfig );
    
    if (!ok)
    {
        return EXIT_FAILURE;
    }
    
    //
    // create a trade and save it to the database
    //
    
    //Persistent::PersistType ptype = Persistent::TEXT;
    Persistent::PersistType ptype = Persistent::BINARY;
    int portfolio = 88888;
    int tradeId   = 13;
    std::string table = "object";
    
    {
        Trade trade1 = createTrade(portfolio, tradeId);
        
        trade1.persistAs(Persistent::TEXT);
        std::cout << std::endl << trade1 << std::endl;
        
        trade1.persistAs(ptype);
        saveDB(dbc, trade1, table);
    }
    
    //
    // load the trade from the database
    //
    
    Trade trade2;
    trade2.persistAs(ptype);
    trade2.positionId(portfolio);
    trade2.tradeId(tradeId);
    
    loadDB(dbc, trade2, table);

    trade2.persistAs(Persistent::TEXT);
    std::cout << std::endl << trade2 << std::endl;
    
    return EXIT_SUCCESS;
}
