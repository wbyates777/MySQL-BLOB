/* Endian 16/11/2012
 
 $$$$$$$$$$$$$$$$$$$$$$$$$
 $   Endian.cpp - code   $
 $$$$$$$$$$$$$$$$$$$$$$$$$
 
 by W.B. Yates    
 Copyright (c) W.B. Yates. All rights reserved.
 History: 
 
 */

#ifndef __ENDIAN_H__
#include "Endian.h"
#endif	

Endian::EndianType Endian::m_byteOrder = Endian::systemByteOrder();

Endian::EndianType 
Endian::systemByteOrder( void ) 
{   
	EndianType byteOrder;
	unsigned int x, w;
	
	w = sizeof(x) * 8;
	x = 1 | (((unsigned int)2) << (w - 8));
	
	unsigned char *cp = (unsigned char *) &x;
	
	switch(*cp)
	{ 
		case 1:
			byteOrder = LITTLE;
			break;
		case 2:
			byteOrder = BIG;
			break;
		default:
			byteOrder = UNDEFINED;
			break;			
	}
	
	return byteOrder;
}

//

