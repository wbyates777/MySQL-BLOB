
/* BinaryPackage  20/01/2011.
 
 $$$$$$$$$$$$$$$$$$%$$$$$$$
 $   TradeBP.h - header   $
 $$$$$$$$$$$$$$$$$$$$$$$$$$
 
 by W.B. Yates. Copyright 20/01/2011. All rights reserved 
 
 The specialised template operators required by the Trade class.
 
 
*/

#ifndef __TRADEBP_H__
#define __TRADEBP_H__

#ifndef __BINARYPACKAGE_H__
#include "BinaryPackage.h"
#endif

#ifndef __CASHFLOW_H__
#include "Cashflow.h"
#endif

#ifndef __CITY_H__
#include "City.h"
#endif

#ifndef __DATE_H__
#include "ADate.h"
#endif


template<>
BinaryPackage& 
BinaryPackage::operator>>(City& c); 

template<>
BinaryPackage& 
BinaryPackage::operator<<(const City& c);

template<>
BinaryPackage& 
BinaryPackage::operator>>(Currency& c);  

template<>
BinaryPackage& 
BinaryPackage::operator<<(const Currency& c);


template<>
BinaryPackage& 
BinaryPackage::operator>>(Date& d);  

template<>
BinaryPackage& 
BinaryPackage::operator<<(const Date& d);


template<>
BinaryPackage& 
BinaryPackage::operator>>(Cashflow& cf);

template<>
BinaryPackage& 
BinaryPackage::operator<<(const Cashflow& cf);


#endif


