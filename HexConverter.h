/* HexConverter Template 13/10/2009.

 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 $   HexConverter.h - header   $
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 
 by W.B. Yates. Copyright 2009. All rights reserved 

 This template allows you to convert std::vector<T> to a hexadecimal string and back again
 Primarily intended for numeric types (i.e float, double) but it will work with integral and *some* non-numeric types aswell as long as they have
 copy constructors defined and do not contain pointer types, or variable length objects i.e std::vectors. 
 I use the class when I need to save floating point values exactly, i.e. with out loss of precision, in a text file.
 
 The code is Endian neutral. All Hex Strings use LITTLE Endian (see Endian.h)
 
 WARNING: unsigned char and signed char's produce different hex strings. 
 
 Example:
 
	 std::vector<double> nums(3);
	 nums[0] = 3.14159;
	 nums[1] = 10000.9123;
	 nums[2] = -3.141;	
	 std::cout << nums << std::endl;
	 HexConverter<double> h;
	 std::cout << h.writeHex(nums) << std::endl;
	 std::cout << h.readHex(h.writeHex(nums)) << std::endl;
 
     or
 
	 std::vector<Cashflow> nums(3);
	 nums[0] = Cashflow(Date(9,7,1968),-10.34,"USD");
	 nums[1] = Cashflow(Date(9,7,1969),10.44,"JPY");
	 nums[2] = Cashflow(Date(9,7,1970),-100000.34,"GBP");	
	 std::cout << nums << std::endl;
	 HexConverter<Cashflow> h;
	 std::cout << h.writeHex(nums) << std::endl;
	 std::cout << h.readHex(h.writeHex(nums)) << std::endl;
	 exit(1);
 
 */
#ifndef __HEXCONVERTER_H__
#define __HEXCONVERTER_H__

#include <string>
#include <sstream>
#include <iostream>
#include <vector>

#ifndef __ENDIAN_H__
#include "Endian.h"
#endif

#include <math.h>

template<class T>
class HexConverter 
{
public:
	
	HexConverter( void ):m_offset(0)
	{
		if (int(char(128)) < 0) // this gets rid of negative hex codes if char's are signed
			m_offset = 128;
	}
	~HexConverter( void ) { m_offset = 0; }

	std::vector<std::string>
	writeHex( const T& value ) const
	{
		return writeHex(std::vector<T>(1,value));
	}
	
	std::vector<std::string>
	writeHex( const std::vector<T>& values ) const;
	
	std::vector<T> 
	readHex( const std::vector<std::string>& vecHexText ) const;
	
private:

	int m_offset;
};

template<class T>
std::vector<std::string>
HexConverter<T>::writeHex(const std::vector<T>& values) const
{
	
	std::ostringstream oss(std::ostringstream::binary);
	
	if (Endian::byteOrder() == Endian::BIG)
	{
		for ( int i = 0; i < values.size(); ++i )
		{
			T value = values[i];
			Endian::byteReverse( value );
			oss.write((char *) &value, sizeof( values[i] ) );
		}
	}
	else
	{
		for ( int i = 0; i < values.size(); ++i )
		{
			oss.write((char *) &values[i], sizeof( values[i] ) );
		}
	}
	
	std::istringstream iss( oss.str(), std::istringstream::binary );
	char token;
	std::vector<int> tokens;
	while (1)
	{
		iss.read( ( char *) &token, sizeof( token ) ); 
		if (!iss.eof())
			tokens.push_back( (token) + m_offset );
		else break;
	}    	
	
	const char NUMS[] = "0123456789ABCDEF"; // Characters that may be used
	
	std::vector<std::string> vecRetVal;
	std::string retVal;
	for ( int i = 0; i < tokens.size(); ++i )
	{
		std::string result;
		int decimal = tokens[i];
		bool negative = false;
		
		if (decimal < 0)
		{
			decimal *= -1;
			negative = true;
		}
		
		do
		{
			result.push_back(NUMS[decimal % 16]);
			// Append the appropriate character in NUMS based on the equation decimal%base
			decimal /= 16; // Calculate new value of decimal
		}
		while (decimal != 0); // Do while used for slight optimisation
		
		
		if (result.length() == 1)
			result.push_back('0');
		
		if (negative)
			result.push_back('-');
		
		// reverse the string
		retVal.append(std::string(result.rbegin(), result.rend())); // remove dependancy on std::string constructor; expensive
		
		if ( ((i+1) % (100*sizeof(T))) == 0) // blocksize=100
		{
			vecRetVal.push_back( retVal );
			retVal.clear();
		}
	}
	if (retVal.size())
	{
		vecRetVal.push_back( retVal );
	}
	
	return vecRetVal;
}

template<class T>
std::vector<T> 
HexConverter<T>::readHex(const std::vector<std::string>& vecHexText) const
{
	enum hex { A = 10, B, C, D, E, F };
	hex h;
	std::vector<int> tokens;
	std::vector<T> retVal;
	long int j = 0;
	
	for ( int idx = 0; idx < vecHexText.size(); ++idx )
	{
		j = 0;
		tokens.clear();
		std::string text(2,'0');
		const std::string& hexText = vecHexText[idx];
		
		while  (j < hexText.size())
		{
			
			int sign = 1;
			if (hexText[j] == '-')
			{
				j++;
				sign = -1;
			}
			text[0] = hexText[j++];
			text[1] = hexText[j++];
			
			long int num = 0;
			long int pow = 1;
			pow <<= 4;
			for(int i = 0; i < 2; ++i)
			{  				
				if ( ::isdigit(text[i]) )
				{
					char c = text[i];
					num += sign * std::atoi(&c) * pow;
				}
				else if ( ::isupper(text[i]) )
				{
					switch(text[i])
					{
						case 'A': h = A; break;
						case 'B': h = B; break;
						case 'C': h = C; break;
						case 'D': h = D; break;
						case 'E': h = E;  break;
						case 'F': h = F;  break;
						default: std::cout << "readHex error; character " << text[i] << " is not hex" << std::endl; exit(1); break;
					}
					num += sign * h * pow ; 
				} 
				else 
				{
					std::cout << hexText << std::endl;
					std::cout << hexText.size() << std::endl;
					std::cout << "readHex error; character " << int(text[i]) << " is not hex" << std::endl; 
					
					exit(1);
				}
				
				pow >>= 4;
				
			}
			tokens.push_back((int) num - m_offset);
		}
		
		char token;
		std::ostringstream oss(std::ostringstream::binary);
		for (int i = 0; i < tokens.size(); ++i )
		{
			token = static_cast<char>(tokens[i]);
			oss.write( (char *) &token, sizeof(token));
		}
		std::istringstream iss( oss.str(), std::istringstream::binary );
		
		T val = T();
		if (Endian::byteOrder() == Endian::BIG)
		{
			for (int i = 0; i < (tokens.size()) / sizeof(T); ++i)
			{			
				iss.read( (char *) &val, sizeof( val ) );
				Endian::byteReverse(val);
				retVal.push_back(val);
			}
		}
		else
		{
			for (int i = 0; i < (tokens.size()) / sizeof(T); ++i)
			{			
				iss.read( (char *) &val, sizeof( val ) );
				retVal.push_back(val);
			}
		}
	}
	return retVal;
}


#endif




