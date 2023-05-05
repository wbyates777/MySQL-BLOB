/* Cashflow 12/05/2009

		$$$$$$$$$$$$$$$$$$$$$$$$$$$
		$   Cashflow.h - header   $
		$$$$$$$$$$%$$$$$$$$$$$$$$$$

   by W.B. Yates    
   Copyright (c) W.B. Yates. All rights reserved 
   History: Every Trade is a collection of Cashflows. 
   We have provided basic arithmetic operations on Cashflows i.e *, /, +, - etc

  Example
 
	 Cashflow yyy(1, Date(1,1,1900), 10.0, Currency("AED"));
	 Cashflow zab(2, Date(31,12,2099), 5.0, "GBP");
	 yyy = 5 + zab + yyy ;
	 yyy += 1;
	 
	 std::cout << sizeof(yyy) << std::endl;
	 std::cout << yyy << std::endl;
	 std::cout << zab << std::endl;
	 exit(1);
*/

#ifndef __CASHFLOW_H__
#define __CASHFLOW_H__

#ifndef __DATE_H__
#include "Date.h"
#endif

#ifndef __CURRENCY_H__
#include "Currency.h"
#endif

#ifndef __VALID_H__
#include "ValId.h"
#endif


typedef double Value;
constexpr Value NEAR_ZERO =   1.0E-8;
inline bool nearZero( Value p ) { return ((p < NEAR_ZERO) && (p > -NEAR_ZERO)); }

class Cashflow
{
public:
	enum FlowType { FIXED, FLOAT, FEE, COST, MAXTYPE }; // Do I need DIVIDEND here?
	
	Cashflow( void ): m_flowId(0), m_date(Date::today()), m_value(0.0), m_ccy(Currency::baseCurrency()), m_flowType(FIXED) {}

	explicit Cashflow( const Value v ): m_flowId(0), m_date(Date::today()), m_value(v), m_ccy(Currency::baseCurrency()), m_flowType(FIXED) {}
    
    Cashflow( const Date& d, const Value v ): m_flowId(0), m_date(d), m_value(v), m_ccy(Currency::baseCurrency()), m_flowType(FIXED) {}
    
	Cashflow( const Date& d, const Value v, const Currency& c ): m_flowId(0), m_date(d), m_value(v), m_ccy(c), m_flowType(FIXED) {}
	
	Cashflow( const ValId i, const Date& d, const Value v, const Currency& c, FlowType ft = FIXED ): 
			  m_flowId(i), m_date(d), m_value(v), m_ccy(c), m_flowType(short(ft)) {}
	
	
	~Cashflow( void ) { m_flowId = 0; m_date.clear(); m_value = 0.0; m_ccy = Currency::NOCURRENCY; m_flowType = FIXED; }
	
    void
    clear( void ) { m_flowId = 0; m_date.clear(); m_value = 0.0; m_ccy = Currency::NOCURRENCY; m_flowType = FIXED; }
    
	void
	setCashflow( const ValId cid, const Date& d, const Value v, const Currency& c, FlowType ft = FIXED )
	{
		m_flowId = cid;
		m_date   = d;
		m_value  = v; // we receive positive and pay negative values
		m_ccy    = c;
		m_flowType = short(ft);
	}
		
	ValId
	valId( void ) const { return m_flowId; }
	
	void 
	valId( const ValId fid ) { m_flowId = fid; }
	
	PositionId 
	positionId( void ) const { return ValIdType::positionId(m_flowId); }
	
	void 
	positionId( const PositionId pid ) { ValIdType::positionId(m_flowId, pid); }
	
	TradeId 
	tradeId( void ) const { return ValIdType::tradeId(m_flowId); }
	
	void 
	tradeId( const TradeId tid ) { ValIdType::tradeId(m_flowId, tid); }
	
	FlowId 
	flowId( void ) const { return ValIdType::flowId(m_flowId); }
	
	void 
	flowId( const FlowId fid ) { ValIdType::flowId(m_flowId, fid); }
	
	void 
	flowId( const PositionId pid, const TradeId tid, const FlowId fid ) { m_flowId = ValIdType::setId(pid, tid, fid); }
	
	const Date& date( void ) const { return m_date; }
	void date( const Date& d )  { m_date = d; }
	
	Value value( void ) const { return m_value; }
	Value& value( void ) { return m_value; }
	void value( const Value& v )  { m_value = v; }
	
	const Currency& ccy( void ) const { return m_ccy; }	
	void ccy( const Currency& c )  { m_ccy = c; }
	
	FlowType flowType( void ) const { return FlowType(m_flowType); }	
	void flowType( const FlowType c )  { m_flowType = short(c); }

	std::string 
	flowTypeString( void ) const { return m_flowTypeName[m_flowType]; }
	
	void
	flowTypeString( const std::string& flt );
	

 	   	
	// Arithmetic operators act only on values; note there are no currency or date checks
    Cashflow& operator+=(const Value& rhs) { m_value += rhs; return *this; }	
	Cashflow& operator-=(const Value& rhs) { m_value -= rhs; return *this; }
	Cashflow& operator*=(const Value& rhs) { m_value *= rhs; return *this; }	
	Cashflow& operator/=(const Value& rhs) { m_value /= rhs; return *this; } 
	Cashflow& operator=(const Value& rhs) { m_value = rhs; return *this; }
    
    const Cashflow operator-(void) const { return Cashflow(m_flowId, m_date, -m_value, m_ccy, FlowType(m_flowType)); }		
	const Cashflow operator+(const Value& rhs) const { return Cashflow(*this) += rhs; }	
	const Cashflow operator-(const Value& rhs) const { return Cashflow(*this) -= rhs; }
	const Cashflow operator*(const Value& rhs) const { return Cashflow(*this) *= rhs; }	
	const Cashflow operator/(const Value& rhs) const { return Cashflow(*this) /= rhs; }    
	
	operator Value( void ) const { return m_value; } 
	
    // relational operators
    bool 
	operator==(const Cashflow& cf) const
	{
		// we check for value equality and flowId because the same flowId can have different values under different scenarios
        // we check the other fields to ensure consistency
		return (m_ccy == cf.m_ccy && m_date == cf.m_date && m_flowType == cf.m_flowType
                && m_flowId == cf.m_flowId && nearZero(m_value - cf.m_value));
	}
    
	friend std::istream&
	operator>>(std::istream& istr, Cashflow& cf);
	
	friend std::ostream&
	operator<<(std::ostream& ostr, const Cashflow& cf);
	
private:
	
	ValId    m_flowId;
	Date     m_date;	
	Value    m_value; // we receive positive and pay negative values
	Currency m_ccy;
	short    m_flowType;
	
	static const char * const m_flowTypeName[];

};



std::ostream&
operator<<(std::ostream& ostr, const Cashflow& cf);

std::istream&
operator>>(std::istream& istr, Cashflow& cf);

#endif





