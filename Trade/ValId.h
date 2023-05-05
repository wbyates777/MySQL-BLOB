/* ValIdType 15/02/2013
 
 $$$$$$$$$$$$$$$$$$$$$$$$
 $   ValId.h - header   $
 $$$$$$$$$$$$$$$$$$$$$$$$
 
 by W.B. Yates    
 Copyright (c) W.B. Yates. All rights reserved.
 
 History: 
	 An id number for a Position, Trade or Cashflow which maintains a natural order.
	 When sorted trades within each position are properly ordered, and
	 flows for each trade are continguous and properly ordered 	 
	 i.e  t1(88880,0,0) < t2(88888,123,0) < t3(88888,123,1) < t4(88888,124,0)
 
 
	Example:
 
	ValId fid = ValIdType::setId(88888,1234,100);
	ValId fid2 = ValIdType::setId(88888,1234,0);
	
	long tmp = fid;
	fid = tmp;
	
	ValIdType::flowId(fid,0);
 
	std::cout << fid << std::endl;
	std::cout << ValIdType::toString(fid) << std::endl;
	std::cout << ValIdType::fromString(ValIdType::toString(fid)) << std::endl;
	std::cout << ValIdType::positionId(fid) << std::endl;
	std::cout << ValIdType::tradeId(fid) << std::endl;
	std::cout << ValIdType::flowId(fid) << std::endl;
 
	if (fid2 <= fid)
		std::cout << "comparison works" << std::endl;
	else std::cout << "comparison FAILURE" << std::endl;
 */


#ifndef __VALID_H__
#define __VALID_H__


#include <string>
#include <vector>

typedef unsigned long ValId;

typedef unsigned int PositionId;
typedef unsigned int TradeId;
typedef unsigned int FlowId;

typedef unsigned int CPartyId;


class ValIdType
{
public:
		
	// 3 identifier types; position, trade, and flow

    static ValId 
    setId( const PositionId pid, const TradeId tid, const FlowId fid ) 
    {  
        return ((ValId(pid) << POSSHIFT) | (ValId(tid) << TRDSHIFT) | (ValId(fid)));
    }
    
    static PositionId // 0-1048575 possible positions
    positionId( const ValId& valId )  
    { 
        return PositionId(POSMASK & (valId >> POSSHIFT)); 
    }
    
    static void 
    positionId( ValId& valId, const PositionId pid ) 
    {  
        valId = ((ValId(pid) << POSSHIFT) | (ValId(tradeId(valId)) << TRDSHIFT) | ValId(flowId(valId)));		
    }
    
    static TradeId // 0-268435455 possible trades (per position, although we shall assume they are unique)
    tradeId( const ValId& valId )  
    { 
        return TradeId(TRDMASK & (valId >> TRDSHIFT)); 
    }
    
    static void 
    tradeId( ValId& valId, const TradeId tid ) 
    {  
        valId = ((ValId(positionId(valId)) << POSSHIFT) | (ValId(tid) << TRDSHIFT) | ValId(flowId(valId)));		
    }
    
    static FlowId // 0-65535 possible flows (per trade)
    flowId( const ValId& valId )  
    { 
        return FlowId(FLWMASK & valId); 
    }
    
    static void 
    flowId( ValId& valId, const FlowId fid ) 
    {  
        valId = ((ValId(positionId(valId)) << POSSHIFT) | (ValId(tradeId(valId)) << TRDSHIFT) | ValId(fid));		
    }
        
		
	static std::string
	toString( const ValId& valId );
	
	static ValId
	fromString( const std::string& strId );
	
private:

	// constructors disabled;
	ValIdType( void )=delete;
    ValIdType(const ValIdType&)=delete;
    ValIdType& operator=(const ValIdType&)=delete;
	
	static const unsigned int POSBITS;
	static const unsigned int TRDBITS;
	static const unsigned int FLWBITS; 
	
	static const unsigned int POSSHIFT;
	static const unsigned int TRDSHIFT;
	
	static const unsigned long POSMASK;
	static const unsigned long TRDMASK;
	static const unsigned long FLWMASK;	
};




#endif

///
