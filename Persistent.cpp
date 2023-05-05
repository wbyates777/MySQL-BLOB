/* Object Persistent 21/10/06

		$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
		$   Persistent.cpp - code   $
		$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

 by W.B. Yates. Copyright 2006. All rights reserved 
 History: Base class for objects we wish to load/save to files or a database. Also allows streaming via TCPIP sockets.
 Depending on the functions that you over-ride you can use a binary or textual format for the above operations;
*/



#ifndef __PERSISTENT_H__
#include "Persistent.h"
#endif

#ifndef __MD5HASH_H__
#include "MD5Hash.h"
#endif

#ifndef __BINARYPACKAGE_H__
#include "BinaryPackage.h"
#endif


///
std::ostream& 
operator<<(std::ostream& str,  const Persistent& pobj)
{
	if (pobj.persistAs() == Persistent::TEXT)
	{
		pobj.writeData(str);
	}
	else 
	{
		BinaryPackage bp;
		pobj.writeData(bp);
		str << bp;
	}
							
	return str;
} 

std::istream& 
operator>>(std::istream& str,  Persistent& pobj)
{
	if (pobj.persistAs() == Persistent::TEXT)
	{
		pobj.readData(str);
	}
	else 
	{
		BinaryPackage bp;
		str >> bp;
		pobj.readData(bp);
	}

	return str;
}

//
Persistent::~Persistent( void )
{
	m_persistAs = Persistent::BINARY;
}

void 
Persistent::name( const std::string& n )
{
	m_name = n;
}


void
Persistent::createName( void ) const
{
	MD5Hash h;
	std::ostringstream s;
	writeData(s); // we always use the text stream when determining a name	
    m_name = h.hash( s.str() );
}


