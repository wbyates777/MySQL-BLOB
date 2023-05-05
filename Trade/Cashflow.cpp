/* Cashflow 12/05/2009
 
 $$$$$$$$$$$$$$$$$$$$$$$$$$$
 $   Cashflow.cpp - code   $
 $$$$$$$$$$%$$$$$$$$$$$$$$$$
 
 by W.B. Yates    
 Copyright (c) W.B. Yates. All rights reserved 
 History: Every Trade is a collection of Cashflows. 
 We have provided basic arithmetic operations on Cashflows i.e *, /, +, - etc
 
*/

#ifndef __CASHFLOW_H__
#include "Cashflow.h"
#endif

/*
	Currency::add("GBP");
	Currency::add("EUR");
	Currency::add("USD");
	Currency::add("JPY");


	Currency JPY("JPY");
	Currency USD("USD");
	Currency GBP(0);
	Currency EUR("EUR");

    std::vector<Cashflow> cfs, res;
	cfs.push_back( Cashflow(Date(1,1,1950),5.0, Currency("XCV")) );
	cfs.push_back( Cashflow(Date(1,1,1950),5.0, GBP) );
	cfs.push_back( Cashflow(Date(7,1,2000),10.0, GBP) );
	cfs.push_back( Cashflow(Date(7,1,2000),10.0, GBP) );
	
	cfs.push_back( Cashflow(Date(6,1,2000),10.0, JPY) );
	cfs.push_back( Cashflow(Date(6,1,2000),10.0, JPY) );
	cfs.push_back( Cashflow(Date(6,1,2000),10.0, JPY) );
	
	cfs.push_back( Cashflow(Date(12,1,2000),10.0, USD) );
	cfs.push_back( Cashflow(Date(12,1,2000),10.0, USD) );
	cfs.push_back( Cashflow(Date(12,1,2000),10.0, USD) );
	cfs.push_back( Cashflow(Date(12,1,2000),10.0, USD) ); 
 
	res = Cashflow::aggregate( cfs );
	std::cout << res;

	int tmp; cin >> tmp;
	exit(1);
*/

const char * const Cashflow::m_flowTypeName[4] = {
	"FIXED", 
	"FLOAT",
	"FEE",
	"COST"
};

void
Cashflow::flowTypeString( const std::string& flt )
{	
    m_flowType = FIXED;
	for (int i = 0; i < MAXTYPE; ++i)
	{
		if (std::strncmp(flt.c_str(), m_flowTypeName[i], 3) == 0)
		{
			m_flowType = FlowType(i);
			break;
		}
	}
}

std::ostream&
operator<<(std::ostream& ostr, const Cashflow& cf)
{
	ostr << cf.flowId() << ' ';
	ostr << cf.m_date << ' ';
	ostr << cf.m_value << ' ';
	ostr << cf.m_ccy << ' ';
	ostr << cf.flowTypeString() << '\n';
	return ostr;
}

std::istream&
operator>>(std::istream& istr, Cashflow& cf)
{
	// check this works!!!
	istr >> cf.m_flowId;
	istr >> cf.m_date;
	istr >> cf.m_value;
	istr >> cf.m_ccy;
	std::string tmp;
	istr >> tmp;
	cf.flowTypeString(tmp);
	return istr;
}

//
