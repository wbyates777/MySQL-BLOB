/* ValIdType 15/02/2013
 
 $$$$$$$$$$$$$$$$$$$$$$$$
 $   ValId.cpp - code   $
 $$$$$$$$$$$$$$$$$$$$$$$$
 
 by W.B. Yates    
 Copyright (c) W.B. Yates
 History: 
 
 */

#ifndef __VALID_H__
#include "ValId.h"
#endif

#ifndef __HEXCONVERTER_H__
#include "HexConverter.h"
#endif

// assumes 64 bit architecture; note 20 + 28 + 16 = 64
const unsigned int ValIdType::POSBITS     = 20;
const unsigned int ValIdType::TRDBITS     = 28;
const unsigned int ValIdType::FLWBITS     = 16; 

// the bitwise left shift needed to move an int/short bit pattern into the relevant
// position in the long
const unsigned int ValIdType::POSSHIFT = FLWBITS + TRDBITS;
const unsigned int ValIdType::TRDSHIFT = FLWBITS;

// each mask has the form: 1111...11100...0
// when using bitwise OR with a mask and a ValId the returned value has the property
// that the 1's of the mask preserve the ValIds existing values, while the portion of 
// the mask with 0's sets that portion of the ValIdType allowing us to set these values ourseleves
const unsigned long ValIdType::POSMASK     = (1L << POSBITS) - 1;
const unsigned long ValIdType::TRDMASK     = (1L << TRDBITS) - 1;
const unsigned long ValIdType::FLWMASK     = (1L << FLWBITS) - 1;

std::string
ValIdType::toString( const ValId& valId )  
{
	std::vector<ValId> hid(1,valId);
	HexConverter<ValId> h;
	return h.writeHex(hid)[0];
}

ValId
ValIdType::fromString( const std::string& strId )  
{
	std::vector<std::string> hid(1,strId);
	HexConverter<ValId> h;
	return h.readHex(hid)[0];
}

//

