/* City 09/06/2011
 
 $$$$$$$$$$$$$$$$$$$$$$$
 $   City.h - header   $
 $$$$$$$$$$$$$$$$$$$$$$$
 
 by W.B. Yates    
 Copyright (c) W.B. Yates. All rights reserved. 
 History: Supports some of IATA airport code list. Useful for describing a city associated with a Market Identification Codes (MIC).
 Each IATA code corresponds to the city served by that airport. For the special cases where no airport
 serves a city we have invented a code terminated in a '0'. 
 
 A city code less than 247 indicates a capital city.
 
 Note that when a city with no airport is a capital city the City code is the country 2 code followed by a 0.
 

 
 +-------------------+-----------+--------------+
 | city              | city3code | country3code |
 +-------------------+-----------+--------------+
 | Andorra la Vella  | AD0       | AND          |
 | Aylesbury         | AY0       | GBR          |
 | Ebene             | EB0       | MUS          |
 | Ecatepec          | EP0       | MEX          |
 | Espoo             | ES0       | FIN          |
 | Foster City       | FC0       | USA          |
 | King Edward Point | GS0       | SGS          |
 | Greenwich         | GW0       | USA          |
 | The Hague         | HG0       | NLD          |
 | Horsens           | HS0       | DNK          |
 | Vaduz             | LI0       | LIE          |
 | Leuven            | LV0       | BEL          |
 | Makati City       | MC0       | PHL          |
 | Mill Valley       | MV0       | USA          |
 | Pasig City        | PC0       | PHL          |
 | Adamstown         | PN0       | PCN          |
 | Nablus            | PS0       | PSE          |
 | s-Hertogenbosch   | SH0       | NLD          |
 | Shimonoseki       | SK0       | JPN          |
 | San Marino City   | SM0       | SMR          |
 | Stamford          | ST0       | USA          |
 | Schwerin          | SW0       | DEU          |
 | Port aux Francais | TF0       | ATF          |
 | Nukunonu          | TK0       | TKL          |
 | Vatican City      | VA0       | VAT          |
 | Vienna            | VN0       | USA          |
 | Varazdin          | VZ0       | HRV          |
 +-------------------+-----------+--------------+

 
 
 
 Such codes are employed to prevent conflict with genuine IATA codes
 
 https://www.iata.org/en/publications/directories/code-search/

 https://en.wikipedia.org/wiki/IATA_airport_code
 
 Example
 
 City x;
 x.setCity( "NYC" );
 std::cout << x << std::endl;
 std::cout << x.name() << std::endl;
 std::cout << short(x) << std::endl;
 std::cout << x.capital() << std::endl;
 std::cout << "latitude = " << x.latitude() << std::endl;
 std::cout << "longitude = " << x.longitude() << std::endl;
 
 City y;
 y.setCity( "LON" );
 std::cout << y << std::endl;
 std::cout << y.name() << std::endl;
 std::cout << short(y) << std::endl;
 std::cout << y.capital() << std::endl;
 std::cout << "latitude = " << y.latitude() << std::endl;
 std::cout << "longitude = " << y.longitude() << std::endl;
 
 for (int i = 0; i < City::NUMCITY; i++)
 {	
    std::cout << City::cityIndex(i).toString() + " " + City::cityIndex(i).name() << std::endl;
 }
 exit(1);
 
 */


#ifndef __CITY_H__
#define __CITY_H__


typedef double Point;

#include <string>
#include <iostream>


class City
{
public:
	
	// The value of the enum elements are my arbitrary numeric code for each city. Note XXX, NOCITY, MAXCITY, NUMCITY and
	// any city codes that terminates in a '0' are not IATA codes
    enum CityCode {
        NOCITY = 0,
        ABJ = 43, ABV = 161, ABZ = 307, ACC = 81, AD0 = 7, ADB = 385, ADD = 70, AEP = 9, AKL = 363, ALA = 349, 
        ALG = 63, ALY = 295, AMD = 326, AMM = 112, AMS = 164, ANC = 390, ANR = 258, ANU = 13, APW = 241, ARH = 372, 
        ASB = 219, ASI = 194, ASM = 66, ASU = 181, ATH = 88, ATL = 391, ATQ = 327, AUA = 1, AUH = 8, AUS = 392, 
        AXA = 4, AY0 = 308, BAH = 24, BBR = 84, BBU = 186, BCN = 296, BCV = 29, BEG = 202, BER = 58, BEY = 123, 
        BFN = 417, BFS = 309, BGF = 37, BGI = 33, BGO = 360, BGW = 106, BHX = 310, BIO = 297, BJL = 85, BJM = 17, 
        BJS = 42, BKK = 216, BKO = 143, BLR = 328, BLZ = 353, BMA = 208, BNJ = 283, BNX = 261, BOG = 48, BOM = 329, 
        BON = 20, BOS = 393, BRE = 284, BRN = 40, BRS = 311, BRU = 18, BSB = 32, BSL = 272, BTS = 206, BUD = 99, 
        BWN = 34, BZV = 46, CAI = 65, CAY = 92, CBG = 312, CBR = 14, CCK = 39, CCS = 235, CCU = 330, CGK = 100, 
        CGO = 277, CGP = 260, CHC = 364, CHI = 394, CKY = 83, CLT = 395, CMB = 128, CMN = 350, CNQ = 248, COO = 19, 
        COR = 249, CPH = 61, CPT = 418, CUR = 53, CWB = 262, CWL = 313, DAC = 22, DAM = 212, DAR = 227, DCA = 231, 
        DCF = 60, DEL = 102, DFW = 396, DIL = 220, DKR = 191, DLA = 282, DLC = 278, DND = 314, DNK = 387, DOH = 184, 
        DUB = 104, DXB = 247, DYU = 217, DZA = 156, EB0 = 352, EDI = 315, EIS = 236, EP0 = 351, ES0 = 301, ESB = 224, 
        ESR = 275, EUN = 67, EVN = 10, EWR = 397, FAE = 75, FC0 = 398, FDF = 152, FIH = 45, FNA = 197, FNJ = 179, 
        FRA = 285, FRU = 116, FSP = 201, FUK = 340, FUN = 225, GBE = 36, GCI = 80, GCM = 55, GDT = 213, GEO = 94, 
        GIB = 82, GLA = 316, GND = 89, GOH = 90, GOI = 331, GOJ = 373, GOT = 383, GRU = 263, GRX = 298, GRZ = 257, 
        GS0 = 193, GUA = 91, GUM = 93, GVA = 273, GW0 = 399, GYD = 16, GYE = 294, HAH = 49, HAJ = 286, HAM = 287, 
        HAN = 238, HAV = 52, HEL = 71, HG0 = 356, HIJ = 341, HIR = 196, HKG = 95, HNL = 400, HRE = 246, HRK = 388, 
        HS0 = 293, IAH = 401, IDR = 332, IEV = 229, INU = 167, IOM = 101, ISB = 170, IST = 386, IUE = 163, JED = 381, 
        JER = 111, JIB = 59, JNB = 419, JSR = 336, JUB = 203, KBL = 2, KGL = 188, KHI = 365, KIJ = 342, KIN = 110, 
        KIV = 137, KLA = 228, KRK = 369, KRT = 190, KTM = 166, KUF = 374, KUL = 155, KWI = 121, LAD = 3, LAX = 402, 
        LBA = 317, LBU = 354, LBV = 77, LCG = 299, LDY = 318, LED = 375, LEJ = 288, LFW = 215, LHE = 366, LI0 = 127, 
        LIM = 173, LIS = 180, LJU = 207, LLW = 154, LON = 78, LOS = 355, LPB = 31, LPL = 319, LUN = 245, LUX = 131, 
        LV0 = 259, LYR = 195, LYS = 303, MAA = 333, MAD = 68, MAJ = 141, MAN = 320, MC0 = 367, MCM = 136, MCT = 169, 
        MDZ = 250, MEL = 254, MEX = 140, MFM = 133, MGA = 162, MGF = 264, MGQ = 200, MHQ = 5, MIA = 403, MKC = 404, 
        MKE = 405, MLA = 144, MLE = 139, MLW = 124, MMX = 384, MNI = 151, MNL = 174, MOW = 187, MPM = 149, MRS = 304, 
        MRU = 153, MSP = 406, MSQ = 28, MSU = 129, MUC = 289, MV0 = 407, MVD = 230, MXP = 337, NAP = 338, NAS = 25, 
        NBO = 115, NDJ = 214, NGO = 343, NIC = 56, NIM = 159, NKC = 150, NKW = 103, NLK = 160, NOU = 158, NTL = 255, 
        NYC = 408, ODS = 389, OKD = 344, OPO = 370, ORK = 334, OSA = 345, OSL = 165, OUA = 21, OVB = 376, OXB = 86, 
        PAC = 171, PAP = 98, PAR = 74, PBH = 35, PBM = 205, PC0 = 368, PCT = 409, PHL = 410, PHX = 411, PN0 = 172, 
        PNH = 117, PNI = 76, POM = 176, POS = 222, PPG = 11, PPT = 183, PRG = 57, PRN = 416, PRY = 244, PS0 = 182, 
        PSY = 73, QDU = 290, QMN = 209, QND = 382, RAI = 50, RAR = 47, RBA = 135, RGN = 145, RIO = 265, RIX = 132, 
        RKV = 107, RMA = 109, ROR = 175, ROS = 251, ROV = 377, RTM = 357, RUH = 189, RUN = 185, SAH = 243, SAL = 198, 
        SAP = 323, SBH = 27, SBZ = 371, SDQ = 62, SEL = 120, SEZ = 211, SFG = 134, SFN = 252, SFO = 412, SGN = 415, 
        SH0 = 358, SHA = 279, SIN = 192, SJI = 51, SJJ = 26, SJU = 178, SK0 = 346, SKB = 119, SKG = 322, SKP = 142, 
        SLU = 126, SM0 = 199, SNN = 335, SOF = 23, SPN = 148, SSG = 87, ST0 = 413, STI = 41, STR = 291, STT = 237, 
        SUB = 325, SUV = 72, SVD = 234, SVX = 378, SW0 = 292, SWS = 321, SXB = 305, SXM = 210, SYD = 256, SZX = 280, 
        TAS = 232, TBS = 79, TBU = 221, TF0 = 12, TGD = 146, TGU = 96, THR = 105, TIA = 6, TIP = 125, TK0 = 218, 
        TLL = 69, TLS = 306, TLV = 108, TMP = 302, TMS = 204, TNR = 138, TOS = 361, TRD = 362, TRN = 339, TRW = 118, 
        TSA = 226, TSE = 114, TUC = 253, TUN = 223, TYO = 113, UIO = 64, UKB = 347, UKY = 348, ULN = 147, UTC = 359, 
        VA0 = 233, VIE = 15, VLI = 239, VLN = 300, VN0 = 414, VNO = 130, VOG = 379, VPS = 276, VTE = 122, VVO = 380, 
        VZ0 = 324, WAW = 177, WDH = 157, WLG = 168, WLS = 240, WUX = 281, XCH = 54, XXX = 242, YAO = 44, YHM = 30, 
        YMQ = 266, YOW = 38, YTZ = 267, YVR = 268, YWG = 269, YYC = 270, YYZ = 271, ZAG = 97, ZRH = 274, 
        MAXCITY = 420, NUMCITY = 420 
    };


	
	
	
	City( void ): m_city(NOCITY) {}
	
	~City( void )  { m_city = NOCITY; }
	
	// non-explicit constructors intentional here
	City( CityCode i ): m_city(i) {} // i.e. i = City::LON
	City( const std::string& s ): m_city(NOCITY) { setCity(s); }
	City( const char *s ): m_city(NOCITY) { if (s) setCity(s); } 
    
	// My numeric code for this city i.e. City::LON = 78
	operator short( void ) const { return m_city; }
	
	// The IATA 3 letter code for this city i.e. "LON"
    std::string
    toString( void ) const { return m_cityNames[m_fromISO[m_city]]; }
	
	std::string
	name( void ) const; // i.e "London" 
	
	bool
	setCity( const std::string& s ); // i.e. s = "LON"
	
	void
	setCity( const CityCode s ) { m_city = s; } // i.e. s = City::LON

	Point
	longitude( void ) const;
	
	Point
	latitude( void ) const;	
	
	bool
	capital( void ) const;
	
	static City
	cityIndex(const int i)  { return CityCode(m_toISO[i]); }
    
    static int
	cityIndex(const City c) { return CityCode(m_fromISO[c]); }
    
    bool                
	valid( void ) const { return m_city != NOCITY; }
	
private:
	
	short m_city; // we use short here as it simplifies streaming 
	
	static const char * const  m_cityNames[NUMCITY];
	static const char * const  m_fullCityNames[NUMCITY];
	static const short         m_toISO[NUMCITY];
	static const short         m_fromISO[MAXCITY];
	static const unsigned char m_capital[NUMCITY];
	static const Point         m_position[NUMCITY][2];    
    static const short         m_searchPoints[27];    
};


std::ostream&
operator<<(std::ostream& ostr, const City& c);

std::istream&
operator>>(std::istream& istr, City& c);


#endif
