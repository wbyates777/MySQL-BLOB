/* BinaryPackage  21/10/2009.
 
 $$$$$$$$$$$$$$$%$$$$$$$$$$$$$$$$
 $   BinaryPackage.cpp - code   $
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 
 by W.B. Yates. Copyright 2009. All rights reserved 
 
 */

#ifndef __BINARYPACKAGE_H__
#include "BinaryPackage.h"
#endif

BinaryPackage&
BinaryPackage::operator<<(const char* s)
{
    assert(s);
    size_t len = std::strlen(s);
    *this << len;
    for (int i = 0; i < len; ++i)
        m_data.push_back(s[i]);
    return *this;
}

BinaryPackage&
BinaryPackage::operator>>(char* s)
{	
    assert(s);
    int len = 0;		
    *this >> len;
    assert(unsigned(m_pos + len) <= m_data.size());	
    for (int i = 0; i < len; ++i)
        s[i] = m_data[m_pos + i];
    s[len] = '\0';
    m_pos += len;
    return *this;
}

BinaryPackage&
BinaryPackage::operator<<(const std::string& s)
{
    *this << int(s.size());	
    m_data.insert(m_data.end(), s.begin(), s.end());
    return *this;
}


BinaryPackage&
BinaryPackage::operator>>(std::string& s)
{
    int len = 0;
    *this >> len;
    assert(unsigned(m_pos + len) <= m_data.size());
    s.assign( &m_data[m_pos], len );
    m_pos += len;
    return *this;
}

// binary version of the stream operators
std::istream& 
operator>>(std::istream& str, BinaryPackage& bp)
{
	int len;
	str.read( (char *) &len, sizeof( len ) );
	bp.data().resize(len);
	str.read( (char *) &bp.data()[0], sizeof( char ) * len );
	return str;
}

std::ostream& 
operator<<(std::ostream& str, const BinaryPackage& bp)
{
	int len = (int) bp.data().size();
	str.write( (char *) &len, sizeof( len ) );
	str.write( (char *) &bp.data()[0], sizeof( char ) * len );
	return str;
}


//
