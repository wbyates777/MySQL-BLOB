/* Endian 16/11/2012
 
 $$$$$$$$$$$$$$$$$$$$$$$$$
 $   Endian.h - header   $
 $$$$$$$$$$$$$$$$$$$$$$$$$
 
 by W.B. Yates    
 Copyright (c) W.B. Yates. All rights reserved.
 History: 
 
 I have compared my Endians with the htonl, htons, ntohs, ntohl functions and everything checks. The code is "Endian Neutral".
 
 For 32 bit architectures you must define the compile time option _32BIT_ i.e. gcc -D_32BIT_ 
 This is neccessary because on a 32 bit build, long has 4 bytes whereas in a 64 bit build it has 8 bytes.

  For more details see
 
 https://en.wikipedia.org/wiki/Endianness
 
 
 This test assumes we are on a LITTLE Endian machine
 
 short i = 13, j = 13;
 std::cout << "sizeof(short) = " << sizeof(short) << std::endl;
 Endian::byteReverse(i);
 if (htons(i) == j)
	std::cout << "short test good" << std::endl;
 else std::cout << "short test bad" << std::endl;
 

 exit(1); 

	uint64_t htonll(uint64_t value) // assumes 64 bit architecture
	{
		// The answer is 42
		static const int num = 42;
		
		// Check the endianness
		if (*reinterpret_cast<const char*>(&num) == num)
		{
			const uint32_t high_part = htonl(static_cast<uint32_t>(value >> 32));
			const uint32_t low_part = htonl(static_cast<uint32_t>(value & 0xFFFFFFFFLL));
			
			return (static_cast<uint64_t>(low_part) << 32) | high_part;
		} 
		else
		{
			return value;
		}
	}
 
*/

#ifndef __ENDIAN_H__
#define __ENDIAN_H__


#include <assert.h>

class Endian
{
public:
	
	enum EndianType { UNDEFINED, LITTLE, BIG };
    
    Endian( void )=default;
    ~Endian( void )=default;
	
	static EndianType
	byteOrder( void ) { return m_byteOrder; }

	// byte manipulation
	static void byteReverse( short& s )          { swapTwoBytes(reinterpret_cast<unsigned short&>(s)); } 	
	static void byteReverse( int& s )            { swapFourBytes(reinterpret_cast<unsigned int&>(s)); }	
	static void byteReverse( long& s )           { swapEightBytes(reinterpret_cast<unsigned long&>(s)); }
	static void byteReverse( unsigned short& s ) { swapTwoBytes(s); } 	
	static void byteReverse( unsigned int& s )   { swapFourBytes(s); }	
	static void byteReverse( unsigned long& s )  { swapEightBytes(s); }	
	static void byteReverse( float& x )          { swapFourBytes(reinterpret_cast<unsigned int&>(x)); }	
	static void byteReverse( double& x )         { swapEightBytes(reinterpret_cast<unsigned long&>(x)); }
	
	template<typename T>
	static void 
	byteReverse( T& )  { /* do nothing */  }

	
	
private:

	static void 
	swapTwoBytes( unsigned short& data )
	{
		data = ( (((data) >> 8) & 0x00FF) | (((data) << 8) & 0xFF00) );
	}

	static void 
	swapFourBytes( unsigned int& data )
	{
		data = ( (((data) >> 24) & 0x000000FF) | (((data) >> 8) & 0x0000FF00) | 
				(((data) << 8) & 0x00FF0000) | (((data) << 24) & 0xFF000000) );
	}
	
	static void 
	swapEightBytes( unsigned long& data )
	{
#ifdef _32BIT_
		assert(sizeof(void*) == 4);
		data = ( (((data) >> 24) & 0x000000FF) | (((data) >> 8) & 0x0000FF00) | 
				(((data) << 8) & 0x00FF0000) | (((data) << 24) & 0xFF000000) ); 
#else
		assert(sizeof(void*) == 8);
		data = ( (((data) >> 56) & 0x00000000000000FFLL) | (((data) >> 40) & 0x000000000000FF00LL) | 
				(((data) >> 24) & 0x0000000000FF0000LL) | (((data) >> 8) & 0x00000000FF000000LL) | 
				(((data) << 8) & 0x000000FF00000000LL) | (((data) << 24) & 0x0000FF0000000000LL) | 
				(((data) << 40) & 0x00FF000000000000LL) | (((data) << 56) & 0xFF00000000000000LL) );
#endif
	}
	
	
	
	// determine if underlying machine is little-endian or big-endian 
	// written to work for any w = 8, 16, 32, 64.
	static EndianType 
	systemByteOrder( void ); 
	
	static EndianType m_byteOrder;
};


#endif

///	


