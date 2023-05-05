/* City 09/06/2011
 
 $$$$$$$$$$$$$$$$$$$$
 $   City.cpp - h   $
 $$$$$$$$$$$$$$$%$$$$
 
 by W.B. Yates    
 Copyright (c) W.B. Yates. All rights reserved.
 History: Supports some of the IATA airport/city code list 	   
 

 
 */

#ifndef __CITY_H__
#include "City.h"
#endif

#include <assert.h>

	
std::ostream&
operator<<( std::ostream& ostr, const City& c )
{
	ostr << c.toString();
	return ostr;
}

std::istream&
operator>>( std::istream& istr, City& c )
{
	std::string str;
	istr >> str;
	c.setCity( str );
	return istr;
}

//
//
//

std::string
City::name( void ) const 
{
	return m_fullCityNames[m_fromISO[m_city]];	
}

bool
City::capital( void ) const
{
	return m_capital[m_fromISO[m_city]];
}

Point
City::longitude( void ) const
{
	return m_position[m_fromISO[m_city]][1];
}

Point
City::latitude( void ) const
{
	return m_position[m_fromISO[m_city]][0];
}


// this speeds up setCurrency quite a bit
const short City::m_searchPoints[27] =
{
//  A,  B,  C,  D,  E,  F,   G,   H,   I,   J,   K,   L,   M,   N,   O,   P,   Q,   R,   S,   T,   U,   V,   W,   X    Y,   Z,   Z + 1
    1, 33, 73, 97, 115, 125, 136, 158, 172, 180, 186, 199, 224, 259, 272, 281, 302, 305, 319, 361, 386, 391, 402, 407, 409, 418, 420 
};

bool
City::setCity( const std::string& str )
{	
    if (str.size() != 3)
	{
        m_city = City::XXX; // NOCITY
		return false;
	}
    
    int index = str[0] - 65; // 'A';
    
    //assert(index > -1 && index < 26);
    if (index < 0 || index > 25)
    {
        m_city = City::XXX;
        return false;
    }
    
	int low   = m_searchPoints[index]; 
	int high  = m_searchPoints[index + 1]; 
    
	while (low < high) 
	{
        int mid = low + ((high - low) >> 1);
        const char * const cty = m_cityNames[mid];
        
        int i = 1;
        
        for (; i < 3; ++i)
        {
            const char &a = str[i];
            const char &b = cty[i];
            
            if (a < b)
            {
                high = mid; 
                break;
            }
            
            if (a > b)
            {
                low = mid + 1;
                break;
            }
        }
        
        if (i > 2)
        {
            m_city = m_toISO[mid]; 
            return true;
        }
	}
    
    m_city = City::XXX; // NOCITY
	return false;
}





// if you add cities make sure you add the names in the correct alphabetic order position
// or else the binary chop search in setCity(std::string) won't work
const char * const City::m_cityNames[NUMCITY] = { "NOCITY", 
    "ABJ", "ABV", "ABZ", "ACC", "AD0", "ADB", "ADD", "AEP", "AKL", "ALA", 
    "ALG", "ALY", "AMD", "AMM", "AMS", "ANC", "ANR", "ANU", "APW", "ARH", 
    "ASB", "ASI", "ASM", "ASU", "ATH", "ATL", "ATQ", "AUA", "AUH", "AUS", 
    "AXA", "AY0", "BAH", "BBR", "BBU", "BCN", "BCV", "BEG", "BER", "BEY", 
    "BFN", "BFS", "BGF", "BGI", "BGO", "BGW", "BHX", "BIO", "BJL", "BJM", 
    "BJS", "BKK", "BKO", "BLR", "BLZ", "BMA", "BNJ", "BNX", "BOG", "BOM", 
    "BON", "BOS", "BRE", "BRN", "BRS", "BRU", "BSB", "BSL", "BTS", "BUD", 
    "BWN", "BZV", "CAI", "CAY", "CBG", "CBR", "CCK", "CCS", "CCU", "CGK", 
    "CGO", "CGP", "CHC", "CHI", "CKY", "CLT", "CMB", "CMN", "CNQ", "COO", 
    "COR", "CPH", "CPT", "CUR", "CWB", "CWL", "DAC", "DAM", "DAR", "DCA", 
    "DCF", "DEL", "DFW", "DIL", "DKR", "DLA", "DLC", "DND", "DNK", "DOH", 
    "DUB", "DXB", "DYU", "DZA", "EB0", "EDI", "EIS", "EP0", "ES0", "ESB", 
    "ESR", "EUN", "EVN", "EWR", "FAE", "FC0", "FDF", "FIH", "FNA", "FNJ", 
    "FRA", "FRU", "FSP", "FUK", "FUN", "GBE", "GCI", "GCM", "GDT", "GEO", 
    "GIB", "GLA", "GND", "GOH", "GOI", "GOJ", "GOT", "GRU", "GRX", "GRZ", 
    "GS0", "GUA", "GUM", "GVA", "GW0", "GYD", "GYE", "HAH", "HAJ", "HAM", 
    "HAN", "HAV", "HEL", "HG0", "HIJ", "HIR", "HKG", "HNL", "HRE", "HRK", 
    "HS0", "IAH", "IDR", "IEV", "INU", "IOM", "ISB", "IST", "IUE", "JED", 
    "JER", "JIB", "JNB", "JSR", "JUB", "KBL", "KGL", "KHI", "KIJ", "KIN", 
    "KIV", "KLA", "KRK", "KRT", "KTM", "KUF", "KUL", "KWI", "LAD", "LAX", 
    "LBA", "LBU", "LBV", "LCG", "LDY", "LED", "LEJ", "LFW", "LHE", "LI0", 
    "LIM", "LIS", "LJU", "LLW", "LON", "LOS", "LPB", "LPL", "LUN", "LUX", 
    "LV0", "LYR", "LYS", "MAA", "MAD", "MAJ", "MAN", "MC0", "MCM", "MCT", 
    "MDZ", "MEL", "MEX", "MFM", "MGA", "MGF", "MGQ", "MHQ", "MIA", "MKC", 
    "MKE", "MLA", "MLE", "MLW", "MMX", "MNI", "MNL", "MOW", "MPM", "MRS", 
    "MRU", "MSP", "MSQ", "MSU", "MUC", "MV0", "MVD", "MXP", "NAP", "NAS", 
    "NBO", "NDJ", "NGO", "NIC", "NIM", "NKC", "NKW", "NLK", "NOU", "NTL", 
    "NYC", "ODS", "OKD", "OPO", "ORK", "OSA", "OSL", "OUA", "OVB", "OXB", 
    "PAC", "PAP", "PAR", "PBH", "PBM", "PC0", "PCT", "PHL", "PHX", "PN0", 
    "PNH", "PNI", "POM", "POS", "PPG", "PPT", "PRG", "PRN", "PRY", "PS0", 
    "PSY", "QDU", "QMN", "QND", "RAI", "RAR", "RBA", "RGN", "RIO", "RIX", 
    "RKV", "RMA", "ROR", "ROS", "ROV", "RTM", "RUH", "RUN", "SAH", "SAL", 
    "SAP", "SBH", "SBZ", "SDQ", "SEL", "SEZ", "SFG", "SFN", "SFO", "SGN", 
    "SH0", "SHA", "SIN", "SJI", "SJJ", "SJU", "SK0", "SKB", "SKG", "SKP", 
    "SLU", "SM0", "SNN", "SOF", "SPN", "SSG", "ST0", "STI", "STR", "STT", 
    "SUB", "SUV", "SVD", "SVX", "SW0", "SWS", "SXB", "SXM", "SYD", "SZX", 
    "TAS", "TBS", "TBU", "TF0", "TGD", "TGU", "THR", "TIA", "TIP", "TK0", 
    "TLL", "TLS", "TLV", "TMP", "TMS", "TNR", "TOS", "TRD", "TRN", "TRW", 
    "TSA", "TSE", "TUC", "TUN", "TYO", "UIO", "UKB", "UKY", "ULN", "UTC", 
    "VA0", "VIE", "VLI", "VLN", "VN0", "VNO", "VOG", "VPS", "VTE", "VVO", 
    "VZ0", "WAW", "WDH", "WLG", "WLS", "WUX", "XCH", "XXX", "YAO", "YHM", 
    "YMQ", "YOW", "YTZ", "YVR", "YWG", "YYC", "YYZ", "ZAG", "ZRH", 
};


// IATA city name (which corresponds to the IATA airport code for that city)
const char * const City::m_fullCityNames[NUMCITY] = { "No City",
    "Abidjan", "Abuja", "Aberdeen", "Accra", "Andorra la Vella", "Izmir", "Addis Ababa", "Buenos Aires", "Auckland", "Almaty", 
    "Algiers", "Alexandria", "Ahmedabad", "Amman", "Amsterdam", "Anchorage", "Antwerp", "Saint Johns", "Apia", "Arkhangelsk", 
    "Ashgabat", "Jamestown", "Asmara", "Asuncion", "Athens", "Atlanta", "Amritsar", "Oranjestad", "Abu Dhabi", "Austin", 
    "The Valley", "Aylesbury", "Manama", "Basse Terre", "Bucharest", "Barcelona", "Belmopan", "Belgrade", "Berlin", "Beirut", 
    "Bloemfontein", "Belfast", "Bangui", "Bridgetown", "Bergen", "Baghdad", "Birmingham", "Bilbao", "Banjul", "Bujumbura", 
    "Bejing", "Bangkok", "Bamako", "Bangalore", "Blantyre", "Stockholm", "Bonn", "Banja Luka", "Bogota", "Mumbai", 
    "Kralendijk", "Boston", "Bremen", "Bern", "Bristol", "Brussels", "Brasilia", "Basel", "Bratislava", "Budapest", 
    "Bandar Seri Begawan", "Brazzaville", "Cairo", "Cayenne", "Cambridge", "Canberra", "West Island", "Caracas", "Calcutta", "Jakarta", 
    "Zhengzhou", "Chittagong", "Christchurch", "Chicago", "Conakry", "Charlotte", "Colombo", "Casablanca", "Corrientes", "Porto Novo", 
    "Cordoba", "Copenhagen", "Cape Town", "Willemstad", "Curitiba", "Cardiff", "Dhaka", "Damascus", "Dar es Salaam", "Washington", 
    "Roseau", "New Delhi", "Dallas", "Dili", "Dakar", "Douala", "Dalian", "Dundee", "Dnipropetrovsk", "Doha", 
    "Dublin", "Dubai", "Dushanbe", "Mamoudzou", "Ebene", "Edinburgh", "Road Town", "Ecatepec", "Espoo", "Ankara", 
    "El Salvador", "El Aaiun", "Yerevan", "Jersey City", "Torshavn", "Foster City", "Fort de France", "Kinshasa", "Freetown", "Pyongyang", 
    "Frankfurt", "Bishkek", "Saint Pierre", "Fukuoka", "Funafuti", "Gaborone", "Saint Peter Port", "George Town", "Cockburn Town", "Georgetown", 
    "Gibraltar", "Glasgow", "Saint Georges", "Nuuk", "Goa", "Nizhny Novgorod", "Gothenburg", "Sao Paulo", "Jaen", "Graz", 
    "King Edward Point", "Guatemala City", "Hagatna", "Geneva", "Greenwich", "Baku", "Guayaquil", "Moroni", "Hannover", "Hamburg", 
    "Hanoi", "Havana", "Helsinki", "The Hague", "Hiroshima", "Honiara", "Hong Kong", "Honolulu", "Harare", "Kharkov", 
    "Horsens", "Houston", "Indore", "Kiev", "Yaren", "Douglas", "Islamabad", "Istanbul", "Alofi", "Jeddah", 
    "Saint Helier", "Djibouti City", "Johannesburg", "Jerusalem", "Juba", "Kabul", "Kigali", "Karachi", "Niigata", "Kingston", 
    "Chisinau", "Kampala", "Krakow", "Khartoum", "Kathmandu", "Samara", "Kuala Lumpur", "Kuwait City", "Luanda", "Los Angeles", 
    "Leeds", "Labuan", "Libreville", "La Coruna", "Londonderry", "Saint Petersburg", "Leipzig", "Lome", "Lahore", "Vaduz", 
    "Lima", "Lisbon", "Ljubljana", "Lilongwe", "London", "Lagos", "La Paz", "Liverpool", "Lusaka", "Luxembourg", 
    "Leuven", "Longyearbyen", "Lyon", "Madras", "Madrid", "Majuro", "Manchester", "Makati City", "Monaco", "Muscat", 
    "Mendoza", "Melbourne", "Mexico City", "Macao", "Managua", "Maringa", "Mogadishu", "Mariehamn", "Miami", "Kansas City", 
    "Milwaukee", "Valletta", "Male", "Monrovia", "Malmo", "Brades", "Manila", "Moscow", "Maputo", "Marseille", 
    "Port Louis", "Minneapolis", "Minsk", "Maseru", "Munich", "Mill Valley", "Montevideo", "Milan", "Naples", "Nassau", 
    "Nairobi", "N Djamena", "Nagoya", "Nicosia", "Niamey", "Nouakchott", "Diego Garcia", "Kingston", "Noumea", "Newcastle", 
    "New York", "Odessa", "Sapporo", "Porto", "Cork", "Osaka", "Oslo", "Ouagadougou", "Novosibirsk", "Bissau", 
    "Panama City", "Port au Prince", "Paris", "Thimphu", "Paramaribo", "Pasig City", "Princeton", "Philadelphia", "Phoenix", "Adamstown", 
    "Phnom Penh", "Palikir", "Port Moresby", "Port of Spain", "Pago Pago", "Papeete", "Prague", "Pristina", "Pretoria", "Nablus", 
    "Stanley", "Duesseldorf", "Mbabane", "Novi Sad", "Praia", "Avarua", "Rabat", "Yangon", "Rio de Janeiro", "Riga", 
    "Reykjavik", "Rome", "Ngerulmud", "Rosario", "Rostov", "Rotterdam", "Riyadh", "Saint Denis", "Sanaa", "San Salvador", 
    "San Pedro Sula", "Gustavia", "Sibiu", "Santo Domingo", "Seoul", "Victoria", "Marigot", "Santa Fe", "San Francisco", "Ho Chi Minh City", 
    "s-Hertogenbosch", "Shanghai", "Singapore", "San Jose", "Sarajevo", "San Juan", "Shimonoseki", "Basseterre", "Thessaloniki", "Skopje", 
    "Castries", "San Marino City", "Limerick", "Sofia", "Capital Hill", "Malabo", "Stamford", "Santiago", "Stuttgart", "Charlotte Amalie", 
    "Surabaya", "Suva", "Kingstown", "Yekaterinburg", "Schwerin", "Swansea", "Strasbourg", "Philipsburg", "Sydney", "Shenzhen", 
    "Tashkent", "Tbilisi", "Nukualofa", "Port aux Francais", "Podgorica", "Tegucigalpa", "Tehran", "Tirana", "Tripoli", "Nukunonu", 
    "Tallinn", "Toulouse", "Tel Aviv", "Tampere", "Sao Tome", "Antananarivo", "Tromso", "Trondheim", "Turin", "Tarawa", 
    "Taipei", "Astana", "Tucuman", "Tunis", "Tokyo", "Quito", "Kobe", "Kyoto", "Ulan Bator", "Utrecht", 
    "Vatican City", "Vienna", "Port Vila", "Valencia", "Vienna", "Vilnius", "Volgograd", "Valparaiso", "Vientiane", "Vladivostok", 
    "Varazdin", "Warsaw", "Windhoek", "Wellington", "Mata Utu", "Wuxi", "Flying Fish Cove", "No City", "Yaounde", "Hamilton", 
    "Montreal", "Ottawa", "Toronto", "Vancouver", "Winnipeg", "Calgary", "Mississauga", "Zagreb", "Zurich", 
};


// convert the alphabetical index for a 3 letter currency to the numeric code  
const short City::m_toISO[NUMCITY] = { NOCITY,
    ABJ, ABV, ABZ, ACC, AD0, ADB, ADD, AEP, AKL, ALA, 
    ALG, ALY, AMD, AMM, AMS, ANC, ANR, ANU, APW, ARH, 
    ASB, ASI, ASM, ASU, ATH, ATL, ATQ, AUA, AUH, AUS, 
    AXA, AY0, BAH, BBR, BBU, BCN, BCV, BEG, BER, BEY, 
    BFN, BFS, BGF, BGI, BGO, BGW, BHX, BIO, BJL, BJM, 
    BJS, BKK, BKO, BLR, BLZ, BMA, BNJ, BNX, BOG, BOM, 
    BON, BOS, BRE, BRN, BRS, BRU, BSB, BSL, BTS, BUD, 
    BWN, BZV, CAI, CAY, CBG, CBR, CCK, CCS, CCU, CGK, 
    CGO, CGP, CHC, CHI, CKY, CLT, CMB, CMN, CNQ, COO, 
    COR, CPH, CPT, CUR, CWB, CWL, DAC, DAM, DAR, DCA, 
    DCF, DEL, DFW, DIL, DKR, DLA, DLC, DND, DNK, DOH, 
    DUB, DXB, DYU, DZA, EB0, EDI, EIS, EP0, ES0, ESB, 
    ESR, EUN, EVN, EWR, FAE, FC0, FDF, FIH, FNA, FNJ, 
    FRA, FRU, FSP, FUK, FUN, GBE, GCI, GCM, GDT, GEO, 
    GIB, GLA, GND, GOH, GOI, GOJ, GOT, GRU, GRX, GRZ, 
    GS0, GUA, GUM, GVA, GW0, GYD, GYE, HAH, HAJ, HAM, 
    HAN, HAV, HEL, HG0, HIJ, HIR, HKG, HNL, HRE, HRK, 
    HS0, IAH, IDR, IEV, INU, IOM, ISB, IST, IUE, JED, 
    JER, JIB, JNB, JSR, JUB, KBL, KGL, KHI, KIJ, KIN, 
    KIV, KLA, KRK, KRT, KTM, KUF, KUL, KWI, LAD, LAX, 
    LBA, LBU, LBV, LCG, LDY, LED, LEJ, LFW, LHE, LI0, 
    LIM, LIS, LJU, LLW, LON, LOS, LPB, LPL, LUN, LUX, 
    LV0, LYR, LYS, MAA, MAD, MAJ, MAN, MC0, MCM, MCT, 
    MDZ, MEL, MEX, MFM, MGA, MGF, MGQ, MHQ, MIA, MKC, 
    MKE, MLA, MLE, MLW, MMX, MNI, MNL, MOW, MPM, MRS, 
    MRU, MSP, MSQ, MSU, MUC, MV0, MVD, MXP, NAP, NAS, 
    NBO, NDJ, NGO, NIC, NIM, NKC, NKW, NLK, NOU, NTL, 
    NYC, ODS, OKD, OPO, ORK, OSA, OSL, OUA, OVB, OXB, 
    PAC, PAP, PAR, PBH, PBM, PC0, PCT, PHL, PHX, PN0, 
    PNH, PNI, POM, POS, PPG, PPT, PRG, PRN, PRY, PS0, 
    PSY, QDU, QMN, QND, RAI, RAR, RBA, RGN, RIO, RIX, 
    RKV, RMA, ROR, ROS, ROV, RTM, RUH, RUN, SAH, SAL, 
    SAP, SBH, SBZ, SDQ, SEL, SEZ, SFG, SFN, SFO, SGN, 
    SH0, SHA, SIN, SJI, SJJ, SJU, SK0, SKB, SKG, SKP, 
    SLU, SM0, SNN, SOF, SPN, SSG, ST0, STI, STR, STT, 
    SUB, SUV, SVD, SVX, SW0, SWS, SXB, SXM, SYD, SZX, 
    TAS, TBS, TBU, TF0, TGD, TGU, THR, TIA, TIP, TK0, 
    TLL, TLS, TLV, TMP, TMS, TNR, TOS, TRD, TRN, TRW, 
    TSA, TSE, TUC, TUN, TYO, UIO, UKB, UKY, ULN, UTC, 
    VA0, VIE, VLI, VLN, VN0, VNO, VOG, VPS, VTE, VVO, 
    VZ0, WAW, WDH, WLG, WLS, WUX, XCH, XXX, YAO, YHM, 
    YMQ, YOW, YTZ, YVR, YWG, YYC, YYZ, ZAG, ZRH, 
};


// convert the numeric code into the alphabetical index for a 3 letter currency code
const short City::m_fromISO[MAXCITY] = { 
    0, 28, 186, 199, 31, 238, 368, 5, 29, 8, 
    123, 295, 364, 18, 76, 392, 156, 50, 66, 90, 
    61, 278, 97, 344, 33, 260, 335, 322, 253, 37, 
    410, 217, 67, 44, 71, 284, 136, 43, 412, 77, 
    64, 348, 51, 1, 409, 128, 72, 306, 59, 158, 
    305, 334, 162, 94, 407, 138, 264, 297, 39, 182, 
    101, 92, 324, 11, 386, 73, 23, 122, 225, 371, 
    7, 163, 352, 301, 283, 125, 292, 203, 215, 362, 
    137, 4, 141, 85, 34, 49, 280, 346, 25, 143, 
    144, 152, 74, 153, 140, 167, 366, 418, 282, 70, 
    80, 176, 102, 267, 111, 367, 46, 311, 373, 312, 
    190, 181, 14, 385, 382, 261, 132, 291, 380, 338, 
    325, 198, 399, 40, 244, 369, 341, 210, 87, 254, 
    396, 220, 310, 234, 327, 307, 229, 191, 376, 243, 
    233, 226, 340, 53, 242, 308, 365, 389, 345, 249, 
    266, 246, 127, 251, 214, 197, 114, 403, 269, 265, 
    268, 2, 235, 179, 15, 277, 195, 175, 404, 230, 
    177, 281, 290, 211, 247, 313, 293, 402, 336, 130, 
    212, 24, 300, 296, 110, 318, 35, 248, 187, 317, 
    194, 105, 333, 151, 22, 222, 166, 129, 320, 342, 
    237, 133, 38, 185, 375, 285, 69, 213, 56, 303, 
    358, 326, 98, 139, 262, 208, 52, 113, 370, 21, 
    104, 363, 294, 384, 120, 135, 381, 99, 192, 174, 
    257, 100, 361, 391, 353, 78, 117, 350, 161, 393, 
    405, 19, 408, 319, 299, 219, 169, 112, 89, 91, 
    231, 314, 328, 383, 232, 270, 359, 150, 17, 221, 
    82, 58, 95, 148, 236, 309, 411, 413, 414, 415, 
    416, 417, 68, 154, 419, 121, 398, 81, 107, 332, 
    360, 406, 106, 57, 63, 131, 159, 160, 207, 255, 
    302, 349, 355, 171, 157, 12, 36, 48, 149, 204, 
    394, 119, 374, 223, 250, 357, 372, 3, 32, 42, 
    47, 65, 75, 96, 108, 116, 142, 201, 205, 218, 
    227, 356, 339, 321, 401, 351, 13, 27, 54, 60, 
    79, 145, 173, 224, 275, 343, 184, 258, 259, 379, 
    134, 165, 189, 263, 273, 276, 337, 387, 388, 10, 
    88, 118, 115, 55, 202, 216, 164, 316, 331, 390, 
    45, 377, 378, 9, 83, 188, 209, 228, 286, 193, 
    274, 323, 20, 146, 196, 206, 279, 315, 354, 397, 
    400, 180, 304, 147, 245, 6, 178, 109, 170, 272, 
    16, 26, 30, 62, 84, 86, 103, 124, 126, 155, 
    168, 172, 200, 239, 240, 241, 252, 256, 271, 287, 
    288, 289, 329, 347, 395, 330, 298, 41, 93, 183,     
};

const unsigned char City::m_capital[NUMCITY] = { 0, // NOCITY
    1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 
    1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 
    1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 
    1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 
    0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 
    1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 
    1, 0, 0, 1, 0, 1, 1, 0, 1, 1, 
    1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 
    0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 
    0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 
    1, 1, 0, 1, 1, 0, 0, 0, 0, 1, 
    1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 
    0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 
    0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 
    1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 
    1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 
    1, 1, 1, 0, 0, 1, 1, 0, 1, 0, 
    0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 
    1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 
    1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 
    0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 
    1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 
    0, 1, 0, 0, 1, 1, 0, 0, 1, 1, 
    0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 
    0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 
    1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 
    1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 
    0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 
    1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 
    1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 
    1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 
    1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 
    0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 
    0, 0, 1, 1, 1, 1, 0, 1, 0, 1, 
    1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 
    0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
    1, 0, 1, 0, 1, 1, 0, 0, 0, 1, 
    1, 1, 0, 1, 1, 1, 0, 0, 1, 0, 
    1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 
    0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 
    0, 1, 0, 0, 0, 0, 0, 1, 0, 
};
// lat,lon
const Point City::m_position[NUMCITY][2] = { { 0.0, 0.0 }, // NOCITY
    { 5.317, -4.033 },{ 9.067, 7.483 },{ 57.1526, -2.1100 },{ 5.55, -0.2 },{ 42.5, 1.5 },{ 38.433, 27.15 },{ 9.03, 38.74 },{ -34.60333, -58.38167 },{ -36.840417, 174.7398694 },{ 43.2775, 76.89583 },
    { 36.7, 3.217 },{ 31.2, 29.916667 },{ 23.03, 72.58 },{ 31.933, 35.933 },{ 52.37306, 4.89222 },{ 61.21806, -149.8925 },{ 51.217, 4.4 },{ 17.116667, -61.85 },{ -13.833, -171.75 },{ 64.55, 40.533333 },
    { 37.933, 58.367 },{ -15.924444, -5.718056 },{ 15.333, 38.933 },{ -25.267, -57.667 },{ 37.967, 23.717 },{ 33.755, -84.39 },{ 31.64, 74.86 },{ 12.519, -70.037 },{ 24.467, 54.367 },{ 30.25, -97.75 },
    { 18.220833, -63.051667 },{ 51.8333333, -0.8333333 },{ 26.217, 50.583 },{ 15.9958, -61.7292 },{ 44.4325, 26.10389 },{ 41.41389, 2.175 },{ 17.25, -88.767 },{ 44.82056, 20.46222 },{ 52.50056, 13.39889 },{ 33.88694, 35.51306 },
    { -29.1, 26.217 },{ 54.597, -5.93 },{ 4.367, 18.583 },{ 13.106, -59.632 },{ 60.38944, 5.33 },{ 33.325, 44.422 },{ 52.4796, -1.9030 },{ 43.25694, -2.92361 },{ 13.45306, -16.5775 },{ -3.383, 29.367 },
    { 39.90639, 116.37972 },{ 13.75222, 100.49389 },{ 12.65, -8 },{ 12.967, 77.567 },{ -15.78611, 35.00583 },{ 59.32944, 18.06861 },{ 50.73389, 7.09972 },{ 44.78, 17.19 },{ 4.59806, -74.07583 },{ 18.975, 72.82583 },
    { 12.1507, -68.2767 },{ 42.35778, -71.06167 },{ 53.07583, 8.8075 },{ 46.9524056, 7.439583 },{ 51.45, -2.583 },{ 50.85, 4.35 },{ -15.8, -47.9 },{ 47.567, 7.6 },{ 48.14389, 17.10972 },{ 47.47194, 19.05028 },
    { 4.89028, 114.94222 },{ -4.267, 15.283 },{ 30.05806, 31.22889 },{ 4.9227, -52.3269 },{ 52.205, 0.119 },{ -35.30806, 149.12444 },{ -12.116667, 96.9 },{ 10.5, -66.917 },{ 22.567, 88.367 },{ -6.2, 106.8 },
    { 34.767, 113.65 },{ 22.367, 91.8 },{ -43.53, 172.62028 },{ 41.88194, -87.62778 },{ 9.50917, -13.71222 },{ 35.226944, -80.843333 },{ 6.93444, 79.84278 },{ 33.533, -7.583 },{ -27.43889, -58.7675 },{ 6.49722, 2.605 },
    { -31.416667, -64.183333 },{ 55.67611, 12.56833 },{ -33.925278, 18.423889 },{ 12.117, -68.933 },{ -25.417, -49.25 },{ 51.483, -3.183 },{ 23.7, 90.375 },{ 33.513, 36.292 },{ -6.8, 39.283 },{ 38.895111, -77.036667 },
    { 15.30139, -61.38833 },{ 28.61389, 77.20889 },{ 32.782778, -96.803889 },{ -8.55, 125.583 },{ 14.69278, -17.44667 },{ 4.05, 9.7 },{ 38.92083, 121.63917 },{ 56.464, -2.970 },{ 48.45, 34.983333 },{ 25.28667, 51.533333 },
    { 53.34778, -6.25972 },{ 25.2697, 55.3095 },{ 38.53667, 68.78 },{ -12.7806, 45.2278 },{ -20.243889, 57.49 },{ 55.95306, -3.18889 },{ 18.43139, -64.62306 },{ 19.6, -99.05 },{ 60.205556, 24.655556 },{ 39.941, 32.864 },
    { 13.667, -89.167 },{ 27.15361, -13.20333 },{ 40.183, 44.517 },{ 40.711417, -74.06476 },{ 62.01167, -6.7675 },{ 37.55139, -122.26639 },{ 14.6, -61.0833 },{ -4.325, 15.32222 },{ 8.48444, -13.23444 },{ 39.01944, 125.73806 },
    { 50.11167, 8.68583 },{ 42.87472, 74.61222 },{ 46.783333, -56.166667 },{ 33.583, 130.4 },{ -8.517, 179.217 },{ -24.65806, 25.91222 },{ 49.45, -2.533 },{ 19.3034, -81.3863 },{ 21.459, -71.139 },{ 6.8, -58.166667 },
    { 36.143, -5.353 },{ 55.858, -4.259 },{ 12.05, -61.75 },{ 64.1666667, -51.7333333 },{ 15.563, 73.818 },{ 56.333333, 44 },{ 57.7, 11.966667 },{ -23.55, -46.633 },{ 37.766667, -3.771111 },{ 47.066667, 15.433333 },
    { -54.283333, -36.5 },{ 14.652083, -90.505972 },{ 13.483, 144.75 },{ 46.2, 6.15 },{ 41.038889, -73.613611 },{ 40.39528, 49.88222 },{ -2.183, -79.883 },{ -11.75, 43.2 },{ 52.36667, 9.71667 },{ 53.56528, 10.00139 },
    { 21.033333, 105.85 },{ 23.133, -82.383 },{ 60.17083, 24.9375 },{ 52.083333, 4.316667 },{ 34.39806, 132.475806 },{ -9.467, 159.817 },{ 22.27833, 114.15889 },{ 21.30889, -157.82611 },{ -17.86389, 31.02972 },{ 49.916667, 36.316667 },
    { 55.867, 9.833 },{ 29.762778, -95.383056 },{ 22.42, 75.54 },{ 50.45, 30.52333 },{ -0.5477, 166.920867 },{ 54.14521, -4.48172 },{ 33.43, 73.04 },{ 41.01224, 28.976018 },{ -19.058873, -169.875412 },{ 21.543333, 39.172778 },
    { 49.187, -2.107 },{ 11.588, 43.145 },{ -26.20444, 28.04556 },{ 31.783, 35.217 },{ 4.85, 31.6 },{ 34.53306, 69.16611 },{ -1.94389, 30.05944 },{ 24.86, 67.01 },{ 37.916111, 139.036389 },{ 17.983333, -76.8 },
    { 47.0107, 28.8687 },{ 0.31361, 32.58111 },{ 50.061389, 19.938333 },{ 15.63306, 32.53306 },{ 27.7, 85.333 },{ 53.233333, 50.166667 },{ 3.1357, 101.688 },{ 29.36972, 47.97833 },{ -8.83833, 13.23444 },{ 34.05, -118.25 },
    { 53.79972, -1.54917 },{ 5.3203222, 115.2112278 },{ 0.3901, 9.4544 },{ 43.371491, -8.395970 },{ 54.9958, -7.3074 },{ 59.95, 30.317 },{ 51.333333, 12.383333 },{ 6.137778, 1.2125 },{ 31.54972, 74.34361 },{ 47.141, 9.521 },
    { -12.04333, -77.02833 },{ 38.7138194, -9.1393861 },{ 46.05556, 14.50833 },{ -13.983, 33.783 },{ 51.50722, -0.1275 },{ 6.45306, 3.39583 },{ -16.5, -68.133333 },{ 53.4, -2.983333 },{ -15.417, 28.283 },{ 49.6, 6.117 },
    { 50.883, 4.7 },{ 78.217, 15.55 },{ 45.759722, 4.842222 },{ 13.08389, 80.27 },{ 40.4, -3.683 },{ 7.067, 171.267 },{ 53.466667, -2.233333 },{ 14.55, 121.033 },{ 43.73278, 7.41972 },{ 23.61, 58.54 },
    { -32.883333, -68.816667 },{ -37.81361, 144.96306 },{ 19.433, -99.133 },{ 22.167, 113.55 },{ 12.13639, -86.25139 },{ -23.4, -51.916667 },{ 2.067, 45.367 },{ 60.1, 19.933 },{ 25.787676, -80.224145 },{ 39.109722, -94.588611 },
    { 43.05, -87.95 },{ 35.89778, 14.5125 },{ 4.175278, 73.508889 },{ 6.317, -10.8 },{ 55.583333, 13.033333 },{ 16.79278, -62.21056 },{ 14.583, 120.967 },{ 55.75, 37.617 },{ -25.967, 32.583 },{ 43.2964, 5.37 },
    { -20.167, 57.5 },{ 44.983, -93.267 },{ 53.9, 27.567 },{ -29.31, 27.48 },{ 48.133333, 11.566667 },{ 37.90611, -122.545 },{ -34.88361, -56.18194 },{ 45.46417, 9.19028 },{ 40.845, 14.258333 },{ 25.06, -77.345 },
    { -1.283, 36.817 },{ 12.112, 15.035 },{ 35.1814556, 136.9063861 },{ 35.167, 33.367 },{ 13.52139, 2.10528 },{ 18.1, -15.95 },{ -7.3, 72.4 },{ -29.066667, 167.966667 },{ -22.2758, 166.4580 },{ -32.916667, 151.75 },
    { 40.664167, -73.938611 },{ 46.467, 30.733 },{ 43.067, 141.35 },{ 41.162142, -8.621953 },{ 51.897222, -8.47 },{ 34.69389, 135.50222 },{ 59.94944, 10.75639 },{ 12.35722, -1.53528 },{ 55.017, 82.933 },{ 11.85, -15.567 },
    { 8.983, -79.517 },{ 18.533, -72.333 },{ 48.8567, 2.3508 },{ 27.4666667, 89.64167 },{ 5.867, -55.167 },{ 14.58722, 121.06111 },{ 40.357115, -74.670165 },{ 39.95, -75.17 },{ 33.4482, -112.0738 },{ -25.066667, -130.1 },
    { 11.55, 104.91667 },{ 6.917778, 158.185 },{ -9.417, 147.283 },{ 10.667, -61.517 },{ -14.27944, -170.70056 },{ -17.535, -149.5696 },{ 50.083, 14.417 },{ 42.666667, 21.166667 },{ -25.74611, 28.18806 },{ 32.22028, 35.27889 },
    { -51.6921, -57.8589 },{ 51.233, 6.783 },{ -26.317, 31.133 },{ 42.25, 19.85 },{ 14.92083, -23.50833 },{ -21.2, -159.767 },{ 34.033, -6.833 },{ 16.8, 96.15 },{ -22.90833, -43.19639 },{ 56.94889, 24.10639 },
    { 64.1333333, -21.9333333 },{ 41.9, 12.5 },{ 7.50056, 134.62417 },{ -32.95056, -60.70417 },{ 57.183, 39.417 },{ 51.921667, 4.481111 },{ 24.633, 46.717 },{ -20.8789, 55.4481  },{ 15.348333, 44.206389 },{ 13.69, -89.19 },
    { 15.5, -88.033333 },{ 17.897908, -62.850556 },{ 45.792784, 24.152069 },{ 18.5, -69.983333 },{ 37.5665361, 126.9779694 },{ -4.6167, 55.45 },{ 18.0731, -63.0822 },{ -31.633333, -60.7 },{ 37.7793, -122.4192 },{ 10.76944, 106.68194 },
    { 51.683333, 5.3 },{ 31.233, 121.467 },{ 1.367, 103.8 },{ 9.933333, -84.083333 },{ 43.8476, 18.3564 },{ 18.45, -66.066667 },{ 33.95, 130.933 },{ 17.3, -62.733 },{ 40.65, 22.9 },{ 42, 21.433333 },
    { 14.017, -60.983 },{ 43.9346, 12.4473 },{ 52.6652, -8.6238 },{ 42.7, 23.333 },{ 15.2, 145.751944 },{ 3.75, 8.783 },{ 41.096667, -73.552222 },{ -33.43783, -70.65033 },{ 48.77861, 9.17944 },{ 18.35, -64.95 },
    { -7.26528, 112.7425 },{ -18.1416, 178.4419 },{ 13.167, -61.233 },{ 56.833, 60.583 },{ 53.633333, 11.416667 },{ 51.616667, -3.95 },{ 48.5844, 7.7486 },{ 18.0237, -63.0458 },{ -33.859972, 151.21111 },{ 22.55, 114.1 },
    { 41.267, 69.217 },{ 41.717, 44.783 },{ -21.133333, -175.2 },{ -49.35, 70.216667 },{ 42.467, 19.267 },{ 14.0833, -87.2167 },{ 35.69611, 51.42306 },{ 41.326, 19.816 },{ 32.90222, 13.18583 },{ -9.16833, -171.80972 },
    { 59.43722, 24.74528 },{ 43.6045, 1.444 },{ 32.083, 34.8 },{ 61.5, 23.766667 },{ 0.33611, 6.68139 },{ -18.933, 47.517 },{ 69.67583, 18.91944 },{ 63.429722, 10.393333 },{ 45.066667, 7.7 },{ 1.417, 173.033 },
    { 25.033, 121.633 },{ 51.166667, 71.433333 },{ -26.816667, -65.216667 },{ 36.8, 10.183 },{ 35.70056, 139.715 },{ -0.25, -78.5833333 },{ 34.69, 135.19556 },{ 35.01167, 135.76833 },{ 47.92, 106.92 },{ 52.091, 5.122 },
    { 41.904, 12.453 },{ 48.208333, 16.373056 },{ -17.75, 168.3 },{ 39.470239, -0.376805 },{ 38.9, -77.266667 },{ 54.683, 25.283 },{ 48.7, 44.516667 },{ -33.05, -71.617 },{ 17.9666667, 102.6 },{ 43.133, 131.9 },
    { 46.312, 16.361 },{ 52.23, 21.01083 },{ -22.57, 17.08361 },{ -41.28889, 174.77722 },{ -13.283, -176.183 },{ 31.566667, 120.3 },{ -10.42167, 105.67806 },{ 0.0, 0.0 },{ 3.867, 11.517 },{ 32.293, -64.782 },
    { 45.50889, -73.55417 },{ 45.417, -75.7 },{ 43.7165889, -79.3406861 },{ 49.25, -123.1 },{ 49.89944, -97.13917 },{ 51.05, -114.067 },{ 43.6, -79.65 },{ 45.816667, 15.983333 },{ 47.367, 8.55 },
};
//



