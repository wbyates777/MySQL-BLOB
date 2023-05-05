/* Object Persistent 21/10/06

		$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
		$   Persistent.h - header   $
		$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

   by W.B. Yates. Copyright 2006. All rights reserved 
   History: Abstract base class for objects we wish to load/save to files or a database. Also provides streaming via TCPIP sockets.
   Depending on the functions that you override you can use a binary or textual format for the above operations;
 
 
 CREATE TABLE IF NOT EXISTS object 
 ( 
      name     VARCHAR(32) NOT NULL, 
      class    VARCHAR(32) NOT NULL, 
      ptype    TINYINT,
      contents LONGBLOB, 
      added TIMESTAMP DEFAULT CURRENT_TIMESTAMP not null, 
      PRIMARY KEY (name, class)
 ) ENGINE InnoDB;
 
*/
 
#ifndef __PERSISTENT_H__
#define __PERSISTENT_H__


#include <string>
#include <iostream>


class BinaryPackage;


class Persistent 
{
public:
	enum PersistType { BINARY, TEXT };
	
	Persistent(): m_persistAs(Persistent::BINARY), m_name()  {}
	Persistent( const PersistType pt ): m_persistAs(pt), m_name()  {}
	explicit Persistent( const std::string& n, const PersistType pt = Persistent::BINARY ): m_persistAs(pt), m_name(n) {}
	
    virtual ~Persistent();

	virtual void 
    name( const std::string& n );
	
	const std::string& 
	name( void ) const { return m_name; }
	
	std::string& 
	name( void ) { return m_name; }

	 // give this object a unique name if I can't think of one myself, based on MD5 hash function
	virtual void
	createName( void ) const;
	
	virtual std::string
	className( void ) const  = 0;
	
	// get/set the storage/transmission type for this object. Ensure you override the appropriate writeData/readData methods
	virtual void
	persistAs( const PersistType p ) const { m_persistAs = int(p); }
	
	virtual PersistType
	persistAs( void ) const { return PersistType(m_persistAs); }

	// the stream operators themselves
	friend std::istream& 
	operator>>( std::istream& str, Persistent& pobj );

	friend std::ostream& 
	operator<<( std::ostream& str, const Persistent& pobj );	

    friend class BinaryPackage;	
	
protected:

    // these functions are not pure virtual so that they can be used in the definetions of the stream operators >> and <<
	// also they are not a single pair of template functions because template functions cannot be virtual
    virtual void 
	readData( std::istream& ) { std::cout << "Persistent error. You must override readData for " << className() << std::endl; exit(1); }
    
    virtual void 
	writeData( std::ostream& ) const { std::cout << "Persistent error. You must override writeData for " << className() << std::endl; exit(1); }

    virtual void 
	readData( BinaryPackage& ) { std::cout << "Persistent error. You must override binary readData for " << className() << std::endl; exit(1); }
    
    virtual void 
	writeData( BinaryPackage& ) const { std::cout << "Persistent error. You must override binary writeData for " << className() << std::endl; exit(1); }

	mutable int m_persistAs;
	mutable std::string m_name;
};

std::istream& 
operator>>(std::istream& str,  Persistent& pobj);

std::ostream& 
operator<<(std::ostream& str,  const Persistent& pobj);







#endif // __PERSISTENT_H__



