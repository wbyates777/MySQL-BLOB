/* Trade 21/04/07

		$$$$$$$$$$$$$$$$$$$$$$$$
		$   Trade.h - header   $
		$$$$$$$$$$$$$$$$$$$$$$$$

   by W.B. Yates    
   Copyright (c) W.B. Yates. All rights reserved 
   History: Basic trade class; inherit from here in order to add a new trade types  
   	   
*/

#ifndef __TRADE_H__
#define __TRADE_H__



#ifndef __CASHFLOW_H__
#include "Cashflow.h"
#endif

#ifndef __PERSISTENT_H__
#include "Persistent.h"
#endif

#ifndef __CITY_H__
#include "City.h"
#endif

#include <exception>
#include <assert.h>
#include <unistd.h>


class Trade : public Persistent 
{
public:
	
	enum TradeStatus { CANCELLED, ACTIVE, TEMPORARY };
	
	Trade( void ):  Persistent(Persistent::BINARY),
					m_tradeId(0),
					m_amount(0.0),
					m_cpy1(0),
					m_cpy2(0),
					m_status(ACTIVE),
					m_start(Date::today()), 
					m_maturity(Date::today()),
					m_price(), 
					m_tradeType("UNKNOWN"), 
					m_sourceId("NONE"),
					m_ccys(),
                    m_cashflows()
	{}
	
	Trade( const ValId       tId,     // unique ID
	       const std::string&     tType,   // trade type
		   const Cashflow&   ptc,     // price per unit of the product at which we bought or sold; always positive
	       const Value&      amt,     // +ve we buy/we are long, -ve we sold/we are short 
	       const Date&       begin,   // start date. For a one day, one-off trade, start and maturity dates are equal
	       const Date&       end ): Persistent(std::to_string(tId), Persistent::BINARY),
									m_tradeId(tId),
									m_amount(amt),
									m_cpy1(0),
									m_cpy2(0),
									m_status(ACTIVE),
									m_start(begin), 
									m_maturity(end),
									m_price(ptc), 
									m_tradeType(tType), 
									m_sourceId("NONE"),
									m_ccys(),
                                    m_cashflows()
	{ 
		assert(m_price >= 0); 
		assert(m_start <=  m_maturity);
	}
	
	virtual ~Trade( void );
    
    virtual void
    clear( void );
	
    
	//
	// identification numbers
	//
	ValId 
	valId( void ) const { return m_tradeId; }
	
	void 
	valId( const ValId tid ) { m_tradeId = tid;  name(std::to_string(m_tradeId)); }
	
	PositionId 
	positionId( void ) const { return ValIdType::positionId(m_tradeId); }
	
	void 
    positionId( const PositionId pid ) { ValIdType::positionId(m_tradeId, pid); name(std::to_string(m_tradeId)); }
	
	TradeId 
	tradeId( void ) const { return ValIdType::tradeId(m_tradeId); }
	
	void 
	tradeId( const TradeId tid ) { ValIdType::tradeId(m_tradeId, tid); name(std::to_string(m_tradeId)); }
	
	void 
	tradeId( const PositionId pid, const TradeId tid ) { m_tradeId = ValIdType::setId(pid, tid, 0); name(std::to_string(tid)); }
	
	
	//
	// Trade data accessors
	//
	
	
    // trade status i.e. ACTIVE, CANCELLED, TEMPORARY, Maturity can be infered from Dates
	TradeStatus status( void ) const { return TradeStatus(m_status); }
	void status( const TradeStatus s ) { m_status = short(s); }
	
	// city this transaction is domiciled or eqivalently was conducted
	City city( void ) const { return m_city; }	
    void city( const City c ) { m_city = c; }
    
	// +ve we bought/we are long, -ve we sold/we are short. It's double not int, e.g. I've bought/sold 2.50 GBP
	Value amount( void ) const { return m_amount; }
	void amount( const Value amt ) { m_amount = amt; }
	
	// on what date does the transaction begin
	const Date& start( void ) const { return m_start; }
	void start( const Date& d ) { m_start = d; }
	
	// on what date does the transaction end
	const Date& maturity( void ) const { return m_maturity; }
	void maturity( const Date& d ) { m_maturity = d; }
	
	// price per amount of the product at which we bought or sold; always positive
	const Cashflow& price( void ) const { assert(m_price >= 0.0); return m_price; }
	void price( const Cashflow& cf ) { m_price = cf; assert(m_price >= 0.0); }
	
	// the type or class of a trade i.e 'vanilla FX swap', see http://en.wikipedia.org/wiki/ISO_10962
	const std::string& tradeType( void ) const { return m_tradeType; }
	void tradeType( const std::string& tt ) { m_tradeType = tt; }
	
	// the (legal) entities involved in this deal
	CPartyId cpy1( void ) const { return m_cpy1; }
	void cpy1( const CPartyId c1 ) { m_cpy1 = c1; }		

	// the (legal) entities involved in this deal
	CPartyId cpy2( void ) const { return m_cpy2; }
	void cpy2( const CPartyId c2 ) { m_cpy2 = c2; }

	// what trade id does this trade have in the system it came from
	const std::string& sourceId( void ) const { return m_sourceId; }
	void sourceId( const std::string& sid ) { m_sourceId = sid; }
	
	// currencies employed by this trade; 
	const std::vector<Currency>& ccys( void ) const { return m_ccys; }
	void ccys( const std::vector<Currency>& c ) { m_ccys = c; }
    
    // assume m_ccy[0] is returned by the valuation method
    Currency
    ccy( void ) const { return (!m_ccys.empty()) ? m_ccys[0] : Currency::NOCURRENCY; } 
	
    
    const std::vector<Cashflow>& 
    cashflows( void ) const { return m_cashflows; } 
    void cashflows( const std::vector<Cashflow>& cf ) { m_cashflows = cf; }
    
	//
	// misc
	//
	
	bool 
	operator<(const Trade& t) const { return (m_start < t.m_start); }
	
	bool 
	operator==(const Trade& t) const { return (m_tradeId == t.m_tradeId); }
	
protected: 
				
    void 
	readData(std::istream& str) override;
    
    void 
	writeData(std::ostream& str) const override;
	
    void 
	readData(BinaryPackage& str) override;
    
    void 
	writeData(BinaryPackage& str) const override;

    std::string
    className( void ) const  override { return "Trade"; }
    
	ValId    m_tradeId;        // unique trade id
	Value    m_amount;         // +ve we bought/we are long, -ve we sold/we are short. It's float not int, e.g. I've bought/sold 2.50 GBP	
	CPartyId m_cpy1;		   // the (legal) entities involved in this deal;
	CPartyId m_cpy2;		   // note trade could be internal (to the instituition), external or internal/external.
	short    m_status;         // CANCELLED, ACTIVE, TEMPORARY	
    City     m_city;           // city where this trade wa done
	Date     m_start;          // on what date does the transaction begin
	Date     m_maturity;       // on what date does the transaction end.	
	Cashflow m_price;          // price per amount of the product at which we bought or sold; always positive
	std::string   m_sourceId;  // what trade id does this trade have in the system it came from
	std::string   m_tradeType; // the type or class of a trade i.e 'vanilla FX swap', see http://en.wikipedia.org/wiki/ISO_10962
	std::vector<Currency> m_ccys;  // currencies employed by this trade; assume m_ccys[0] is used for reporting
    std::vector<Cashflow> m_cashflows;
};




#endif

