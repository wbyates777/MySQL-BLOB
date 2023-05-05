/* Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
rights reserved.
License to copy and use this software is granted provided that it
is identified as the "RSA Data Security, Inc. MD5 Message-Digest
Algorithm" in all material mentioning or referencing this software
or this function.
License is also granted to make and use derivative works provided
that such works are identified as "derived from the RSA Data
Security, Inc. MD5 Message-Digest Algorithm" in all material
mentioning or referencing the derived work.
RSA Data Security, Inc. makes no representations concerning either
the merchantability of this software or the suitability of this
software for any particular purpose. It is provided "as is"
without express or implied warranty of any kind.
These notices must be retained in any copies of any part of this
documentation and/or software.

*/

#ifndef __MD5HASH_H__
#define __MD5HASH_H__


#include <string>



class MD5Hash  
{
public:
    
	MD5Hash( void )
	{
		PADDING[0] = 0x80;
		std::memset(PADDING+1, 0, 63);
	}

	std::string 
    hash(const std::string& msg);

private:

	unsigned char PADDING[64]; 

};

 
#endif 
