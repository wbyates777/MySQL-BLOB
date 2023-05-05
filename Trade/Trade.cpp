/* Trade 21/04/07

        $$$$$$$$$$$$$$$$$$$$$$$$
        $   Trade.cpp - code   $
        $$$$$$$$$$$$$$$$$$$$$$$$

   by W.B. Yates    
   Copyright (c) W.B. Yates. All rights reserved 
   History: Basic trade class; inherit from here in order to add a new trade types  
          
*/

#ifndef __TRADE_H__
#include "Trade.h"
#endif


#ifndef __TRADEBP_H__
#include "TradeBP.h"
#endif


Trade::~Trade( void )
{
    m_tradeId = 0;
    m_amount = 0.0;
    m_cpy1 = 0;
    m_cpy2 = 0;
    m_status = CANCELLED;
    m_city = City::NOCITY;
    m_start.clear();
    m_maturity.clear();
    m_price.clear();
    m_sourceId.clear();
    m_tradeType.clear();
    m_ccys.clear();
    m_cashflows.clear();
}

void
Trade::clear( void )
{
    m_tradeId = 0;
    m_amount = 0.0;
    m_cpy1 = 0;
    m_cpy2 = 0;
    m_status = CANCELLED;
    m_city = City::NOCITY;
    m_start.clear();
    m_maturity.clear();
    m_price.clear();
    m_sourceId.clear();
    m_tradeType.clear();
    m_ccys.clear();
    m_cashflows.clear();
}



void 
Trade::writeData(std::ostream& ostr) const
{
	ostr << m_tradeId << ' ';
	ostr << m_amount << ' '; 
	ostr << m_cpy1 << ' ';
	ostr << m_cpy2 << ' ';
	ostr << m_status << ' ';
	ostr << m_city << ' ';
	ostr << m_start << ' ';
	ostr << m_maturity << ' ';
	ostr << m_price << ' '; 
	ostr << m_tradeType << ' ';
	ostr << m_sourceId << ' ';	
	ostr << m_ccys << ' ';
    ostr << m_cashflows << ' ';
}

void 
Trade::readData(std::istream& istr)
{
	istr >> m_tradeId; 
	istr >> m_amount;
	istr >> m_cpy1;
	istr >> m_cpy2;
	istr >> m_status;
	istr >> m_city;
	istr >> m_start;
	istr >> m_maturity;	
	istr >> m_price; 
	istr >> m_tradeType;
	istr >> m_sourceId;
	istr >> m_ccys;
    istr >> m_cashflows;
}

void 
Trade::writeData(BinaryPackage& ostr) const
{
	ostr << m_tradeId;
	ostr << m_amount; 
	ostr << m_cpy1;
	ostr << m_cpy2;
	ostr << m_status;
	ostr << m_city;
	ostr << m_start;
	ostr << m_maturity;
	ostr << m_price; 
	ostr << m_tradeType;
	ostr << m_sourceId;	
	ostr << m_ccys;
    ostr << m_cashflows;
}

void 
Trade::readData(BinaryPackage& istr)
{
	istr >> m_tradeId; 
	istr >> m_amount;
	istr >> m_cpy1;
	istr >> m_cpy2;
	istr >> m_status;
	istr >> m_city;
	istr >> m_start;
	istr >> m_maturity;	
	istr >> m_price; 
	istr >> m_tradeType;
	istr >> m_sourceId;
	istr >> m_ccys;
    istr >> m_cashflows;
}



