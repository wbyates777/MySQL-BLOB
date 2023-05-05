
/* BinaryPackage  21/10/2009.
 
 $$$$$$$$$$$$$$$%$$$$$$$$$$$$$$$$
 $   BinaryPackage.h - header   $
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 
 by W.B. Yates. Copyright 2009. All rights reserved 
 
  Convert built-in types, a few STL Types, Persistent objects and any other 
  object types we have provided code for, to a platform independant LITTLE Endian binary representation
  Note however that we *DO* assume that all platforms have the same word size.

  Between platforms the use of char and unsigned char could create problems; 
  compile using -unsigned char to avoid this
  
  WARNING: You should use this class with extreme caution  
  Always check that the object you stream out is the object you stream in before proceeding. That is
 
  BinaryPackage bp;
  MyType x(1), y(2);
  bp << x;
  bp >> y;
  if (x != y)
       std::cout << "Error" << std::endl;

 I have provided the key template stream operators which assume that arg is a derived from Persistent 
 
 template<class T>
 inline BinaryPackage&
 operator<<(const T& arg);
 
 template<class T>
 inline BinaryPackage&
 operator>>(T& arg);
 
 Note T* and const T* have been disabled to prevent accidental use with pointer types.
 
 The built in types, basic STL types are provided (via specialisation).
  
 I have provided a macro at the end of the class that will let you add built in types to BinaryPackage
 by specializing the key template functions.
 You may add your own types in similar fashion i.e. specialise the key template operators.
 
 The easiest method is to just add stream operators to/from BinaryPackage in your class; you don't have to inherit from 
 Persistent if you don't want to or can't, or specialise the members of BinaryPackage.
 
 Provided a simple assert to check for overflow of index into m_data during read i.e m_pos + len < m_data.size()
 

 Imporve char * routines strncpy perhaps
 
 Example 1:
 
 BinaryPackage test;
 
 
 
 std::vector<Cashflow> cfs, res;
 cfs.push_back( Cashflow(1, Date(1,1,1900), 7.0, Currency("ADP")) ); 
 cfs.push_back( Cashflow(2, Date(1,1,1900), 5.0, "ADP") );
 cfs.push_back( Cashflow(1999999, Date(31,12,2099), 10.0, "GBP") );
 cfs.push_back( Cashflow(1999999, Date(31,12,2099), 10.0, "ZWL") );
 
 Atom e;
 e = City(City::NYC);
 Atom f;
 short rt = -1;
 test << rt << "test" << 3L << 3.141 << cfs << e;
 
 short a; 
 unsigned long c;
 char b[6];
 double d;
 
 test >> a;
 test >> b;
 test >> c;
 test >> d;
 test >> res;
 test >> f;
 
 std::cout << a << ' ' << b << ' ' << c << ' ' << d << ' ' << res << f.toCity().toString() << std::endl; 
 exit(1);

*/

#ifndef __BINARYPACKAGE_H__
#define __BINARYPACKAGE_H__

#ifndef __PERSISTENT_H__
#include "Persistent.h"
#endif

#ifndef __ENDIAN_H__
#include "Endian.h"
#endif

#ifndef __UTILS_H__
#include "Utils.h"
#endif

#include <string>
#include <vector>
#include <map>
#include <utility>
#include <complex>
#include <assert.h>


class BinaryPackage 
{
public:
		
	BinaryPackage( void ): m_pos(0), m_data()  {}

	explicit BinaryPackage(const std::vector<char>& d ): m_pos(0), m_data(d) {}
	
	template<class T>
	explicit BinaryPackage( const T& x ): m_pos(0), m_data() { *this << x; }
	
	~BinaryPackage( void ) { m_data.clear(); m_pos = 0; } 
	
	void
	clear( void ) { m_data.clear(); m_pos = 0; } 

	const std::vector<char>& 
	data( void ) const { return m_data; }
	
	std::vector<char>& 
	data( void ) { return m_data; }
	
	int 
	size( void ) const { return int(m_data.size()); }
    
    // these are the key template operators. Later we will specialise these functions
	// for built in types and types that are not derived from Persistent i.e. Date. 
	template<class T>
	BinaryPackage&
	operator<<(const T& arg)
    {
        static_cast<const Persistent&>(arg).writeData(*this);
        return *this;
    }
    
    template<class T>
	BinaryPackage&
	operator>>(T& arg)
    {
        static_cast<Persistent&>(arg).readData(*this);
        return *this;
    }
    	
    // WARNING: ensure s is large enough to accomodate the string; always prefer std::string to char *
    // as these methods are pointers to char they cannot be specialisations as T* methods are disabled
    BinaryPackage&
    operator>>(char* s);
    
    BinaryPackage&
    operator<<(const char* s);  
    //

    BinaryPackage&
    operator<<(const std::string& s);    

    BinaryPackage&
    operator>>(std::string& s);
    
    //
    
    template<typename T>
    BinaryPackage&
    operator<<(const std::complex<T>& s);    
    
    template<typename T>
    BinaryPackage&
    operator>>(std::complex<T>& s);
    
	// STL types, Trio and Matrix	
	template<typename K, typename V>
	BinaryPackage&
	operator<<(const std::pair<K,V>& values);
	
    template<typename K, typename V>
	BinaryPackage&
	operator>>(std::pair<K,V>& values);
	


	template<class T>
	BinaryPackage&
	operator<<(const std::vector<T>& values);
	
	template<class T>
	BinaryPackage&
	operator>>(std::vector<T>& s);
	

	
    template<class K, class V>
	BinaryPackage&
	operator<<(const std::map<K,V>& values);
	
    template<class K, class V>
	BinaryPackage&
	operator>>(std::map<K,V>& values);		
	// end of STL types	


private:

	// disabled to prevent accidental use with pointer types	
	template<typename T>
	BinaryPackage&
	operator<<(const T* s);	
	
	template<typename T>
	BinaryPackage&
	operator<<(T* s);	
    
	template<typename T>
	BinaryPackage&
	operator>>(T* s);
	// end of disabled methods 
			
	template<typename T>
	void 
	writeToBinaryPackage(const T& x);
	
	template<typename T>
	void 
	readFromBinaryPackage(T& s);
	
    int m_pos;	
	std::vector<char> m_data;
};	

// str should be set for binary input (see std::ios::binary)
std::istream& 
operator>>(std::istream& str, BinaryPackage& pobj); 

// str should be set for binary output (see std::ios::binary)
std::ostream& 
operator<<(std::ostream& str, const BinaryPackage& pobj);


template<typename T>
void 
BinaryPackage::writeToBinaryPackage(const T& x)  
{
    if (Endian::byteOrder() == Endian::BIG)
    {
        T s = x;
        Endian::byteReverse(s);
        int len = sizeof(s);	
        std::vector<char> buff(len);
        std::memcpy(&buff[0], (char*) &s, len);
        m_data.insert(m_data.end(), buff.begin(), buff.end()); 
    }
    else
    {
        int len = sizeof(x);	
        std::vector<char> buff(len);
        std::memcpy(&buff[0], (char*) &x, len);
        m_data.insert(m_data.end(), buff.begin(), buff.end()); 
    }
}

template<typename T>
void 
BinaryPackage::readFromBinaryPackage(T& s) 
{
    size_t len = sizeof(s);
    assert(unsigned(m_pos + len) <= m_data.size());		 	
    std::memcpy(&s, (char*) &m_data[m_pos], len);
    m_pos += len;
    
    if (Endian::byteOrder() == Endian::BIG)
    {
        Endian::byteReverse(s);
    }
}

// Add the built-in types using the following macro and template specialisation
// specialisations declared and defined in header must be inline
// if not inline put declaration in header and definition in BinaryPackage.cpp file
#define ADD_TYPES(X) \
template<> \
inline BinaryPackage& \
BinaryPackage::operator<<(const X & values) \
{ \
	writeToBinaryPackage(values);\
	return *this;\
} \
template<> \
inline BinaryPackage& \
BinaryPackage::operator>>(X & values) \
{ \
	readFromBinaryPackage(values);\
	return *this;\
} \

ADD_TYPES(bool)
ADD_TYPES(short)
ADD_TYPES(unsigned short)
ADD_TYPES(int)
ADD_TYPES(unsigned int)
ADD_TYPES(long)
ADD_TYPES(unsigned long)
ADD_TYPES(float)
ADD_TYPES(double)



template<>
inline BinaryPackage&
BinaryPackage::operator<<(const char& values)
{
	m_data.push_back( values );
	return *this;
}

template<>
inline BinaryPackage&
BinaryPackage::operator>>(char& values)
{
	assert(unsigned(m_pos) < m_data.size());
	values = m_data[m_pos];
	++m_pos;
	return *this;
}

// end of specialisations

template<typename T>
BinaryPackage&
BinaryPackage::operator<<(const std::complex<T>& s)
{
    *this << s.real() << s.imag();
    return *this;
}

template<typename T>
BinaryPackage&
BinaryPackage::operator>>(std::complex<T>& s)
{
    T real = T(), imag = T();
    *this >> real >> imag;
    s = std::complex<T>(real, imag);
    return *this;
}


template<typename K, typename V>
BinaryPackage&
BinaryPackage::operator<<(const std::pair<K,V>& values)
{	
	*this << values.first << values.second;		
	return *this;
}

template<typename K, typename V>
BinaryPackage&
BinaryPackage::operator>>(std::pair<K,V>& values)
{		
	*this >> values.first >> values.second;
	return *this;
}


template<class T>
BinaryPackage&
BinaryPackage::operator<<(const std::vector<T>& values)
{
	*this << int(values.size());
	for (auto i = values.begin(); i != values.end(); ++i)
    {
        *this << *i;		
    }
    return *this;
}

template<class T>
BinaryPackage&
BinaryPackage::operator>>(std::vector<T>& values)
{		
	int len = 0;
	*this >> len;
	values.resize(len);
	for (auto i = values.begin(); i != values.end(); ++i)
    {
        *this >> *i;
    }
    return *this;
} 



template<class K, class V>
BinaryPackage&
BinaryPackage::operator<<(const std::map<K,V>& values)
{
	*this << int(values.size());
	for (auto idx = values.begin(); idx != values.end(); ++idx)
	{
		*this << idx->first << idx->second;
	}			
	return *this;
}

template<class K, class V>
BinaryPackage&
BinaryPackage::operator>>(std::map<K,V>& values)
{		
	values.clear();
	
	int len = 0;
	*this >> len;		

    K key;
    V item;
	auto lastPos = values.begin();
    for (int i = 0; i < len; ++i)
	{
		*this >> key >> item;
		lastPos = values.insert(lastPos, typename std::map<K,V>::value_type( key, item ));
	}
	
	return *this;
}



#endif




