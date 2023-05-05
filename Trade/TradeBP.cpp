/* BinaryPackage  24/03/2015.
 
 $$$$$$$$$$$$$$$$$$%$$$$$$$
 $   TradeBP.cpp - code   $
 $$$$$$$$$$$$$$$$$$$$$$$$$$
 
 by W.B. Yates. Copyright 24/03/2015. All rights reserved 
 
 The specialised template operators required by the Trade class.
 
 */

#ifndef __TRADEBP_H__
#include "TradeBP.h"
#endif


template<>
BinaryPackage& 
BinaryPackage::operator>>(City& c)
{
    short iso = 0;
    *this >> iso;
    c.setCity( City::CityCode(iso) );
    return *this;
}  

template<>
BinaryPackage& 
BinaryPackage::operator<<(const City& c)
{
    *this << short(c);
    return *this;
}

template<>
BinaryPackage& 
BinaryPackage::operator>>(Currency& c)
{
    short iso = 0;
    *this >> iso;
    c.setCurrency( Currency::CurrencyCode(iso) );
    return *this;
}  

template<>
BinaryPackage& 
BinaryPackage::operator<<(const Currency& c)
{
    *this << short(c);
    return *this;
}

template<>
BinaryPackage& 
BinaryPackage::operator>>(Date& d)
{
    int serial = 0;
    *this >> serial;
    d.setDate(serial);
    return *this;
}  

template<>
BinaryPackage& 
BinaryPackage::operator<<(const Date& d)
{
    *this << d.serial();
    return *this;
} 

template<>
BinaryPackage& 
BinaryPackage::operator>>(Cashflow& cf)
{
    ValId id = 0; // NB depends on ValId and Cashflow
    Date date;
    Value value = 0.0;
    Currency ccy;
    short flowType = 0;
    
    *this >> id >> date >> value >> ccy >> flowType;
    cf.setCashflow( id, date, value, ccy, Cashflow::FlowType(flowType) );
    return *this;
}  

template<>
BinaryPackage& 
BinaryPackage::operator<<(const Cashflow& cf)
{
    *this << cf.valId() << cf.date() << cf.value() << cf.ccy() << short(cf.flowType());
    return *this;
}

