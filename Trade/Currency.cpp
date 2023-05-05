/* Currency 10/09/09

		$$$$$$$$$$$$$$$$$$$$$$$$$$$
		$   Currency.cpp - code   $
		$$$$$$$$$$$$$$$$$$$$$$$$$$$

   by W.B. Yates    
   Copyright (c) W.B. Yates
   History: Supports most of the ISO 4217 Currency Code List
 
 
 
  
*/

#ifndef __CURRENCY_H__
#include "Currency.h"
#endif

#include <assert.h>

std::ostream&
operator<<( std::ostream& ostr, const Currency& c )
{
	ostr << c.toString();
	return ostr;
}

std::istream&
operator>>( std::istream& istr, Currency& c )
{
	std::string str;
	istr >> str;
	c.setCurrency( str );
	return istr;
}



// default base currency
Currency Currency::m_baseCurrency = Currency::USD;

std::string
Currency::name( void ) const 
{
	return m_denomNames[m_fromISO[m_ccy]];
}


// the binary chop algorithm complexity is log2(N)−1 for the expected number of 
// probes in an average successful search, and the worst case is log2(N)
// the search points array means that N is now the size of a (smaller) alphabetic 
// partiton at the cost of a single probe.
// also note that the algorithm work efficiently in the presence of singleton currencies such as OMR and QAR
const short Currency::m_searchPoints[27] =
{
//  A, B,  C,  D,  E,  F,  G,  H,  I,  J,  K,   L,   M,   N,   O,   P,   Q,   R,   S,   T,   U,   V,   W,   X    Y,   Z,  Z + 1 
    1, 17, 37, 54, 60, 69, 73, 83, 88, 96, 99, 108, 117, 135, 142, 143, 152, 153, 159, 178, 191, 200, 204, 205, 220, 223, 229
};

// this speeds up setCurrency quite a bit as E leads to EUR, G leads to GBP, J leads to JPY, and U to USD 
// in practice these are heavily used currencies
const short Currency::m_midPoints[26] =
{
//  A,  B,  C,  D,  E,  F,  G,  H,  I,  J,   K,   L,   M,   N,   O,   P,   Q,   R,   S,   T,   U,   V,   W,   X    Y,   Z
    8, 27, 46, 57, 68, 71, 73, 85, 91, 98, 103, 112, 125, 138, 142, 147, 152, 155, 168, 184, 194, 202, 204, 212, 221, 225  
};



bool
Currency::setCurrency( const std::string& str )
// see http://en.wikipedia.org/wiki/Binary_search_algorithm
{	
    // assert(str.size() == 3);
    if (str.size() != 3)
    {
        m_ccy = Currency::XXX; // NOCURRENCY
        return false;
    }
	
    int index = str[0] - 65; // 65 is the ascii code for 'A';
    
    // assert(index > -1 && index < 26);
    if (index < 0 || index > 25)
    {
        m_ccy = Currency::XXX;
        return false;
    }
    
	int low   = m_searchPoints[index]; 
	int high  = m_searchPoints[index + 1]; 
    int mid   = m_midPoints[index];
    
	while (low < high)  
	{
        const char * const ccy = m_ccyNames[mid];

        int i = 1;
        
        for (; i < 3; ++i)
        {
            const char &a = str[i];
            const char &b = ccy[i];
 
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
            m_ccy = m_toISO[mid]; 
			return true;
        }
        
        mid = low + ((high - low) >> 1);
	} 

    m_ccy = Currency::XXX; // NOCURRENCY
    return false;
}

// tables generated automatically



// convert the alphabetical index for a 3 letter currency to the numeric code  
const short Currency::m_toISO[NUMCURRENCY] = {
	NOCURRENCY,  // 10 codes per row
	ADP, AED, AFA, AFN, ALL, AMD, ANG, AOA, AON, AOR, 
	ARS, ATS, AUD, AWG, AZM, AZN, BAM, BBD, BDT, BEC, 
	BEF, BEL, BGL, BGN, BHD, BIF, BMD, BND, BOB, BOV, 
	BRL, BSD, BTN, BWP, BYR, BZD, CAD, CDF, CHE, CHF, 
	CHW, CLF, CLP, CNY, COP, COU, CRC, CSK, CUC, CUP, 
	CVE, CYP, CZK, DDM, DEM, DJF, DKK, DOP, DZD, ECS, 
	EEK, EGP, ERN, ESA, ESB, ESP, ETB, EUR, FIM, FJD, 
	FKP, FRF, GBP, GEL, GHS, GIP, GMD, GNF, GRD, GTQ, 
	GWP, GYD, HKD, HNL, HRK, HTG, HUF, IDR, IEP, ILS, 
	INR, IQD, IRR, ISK, ITL, JMD, JOD, JPY, KES, KGS, 
	KHR, KMF, KPW, KRW, KWD, KYD, KZT, LAK, LBP, LKR, 
	LRD, LSL, LTL, LUF, LVL, LYD, MAD, MDL, MGA, MGF, 
	MKD, MMK, MNT, MOP, MRO, MTL, MUR, MVR, MWK, MXN, 
	MXV, MYR, MZM, MZN, NAD, NGN, NIO, NLG, NOK, NPR, 
	NZD, OMR, PAB, PEN, PGK, PHP, PKR, PLN, PLZ, PTE, 
	PYG, QAR, ROL, RON, RSD, RUB, RUR, RWF, SAR, SBD, 
	SCR, SDD, SDG, SEK, SGD, SHP, SIT, SKK, SLL, SOS, 
	SRD, SRG, SSP, STD, SVC, SYP, SZL, THB, TJR, TJS, 
	TMM, TMT, TND, TOP, TPE, TRL, TRY, TTD, TWD, TZS, 
	UAH, UAK, UGX, USD, USN, USS, UYI, UYU, UZS, VEB, 
	VEF, VND, VUV, WST, XAF, XAG, XAU, XBA, XBB, XBC, 
	XBD, XCD, XDR, XOF, XPD, XPF, XPT, XTS, XXX, YDD, 
	YER, YUM, ZAL, ZAR, ZMK, ZRN, ZWD, ZWL
};

// convert the numeric code into the alphabetical index for a 3 letter currency code
const short Currency::m_fromISO[MAXCURRENCY] = { // 10 values per row
	0, 0, 0, 0, 3, 0, 0, 0, 5, 0, 
	0, 0, 59, 0, 0, 0, 0, 0, 0, 0, 
	1, 0, 0, 0, 9, 0, 0, 0, 0, 0, 
	0, 15, 11, 0, 0, 0, 13, 0, 0, 0, 
	12, 0, 0, 0, 32, 0, 0, 0, 25, 0, 
	19, 6, 18, 0, 0, 0, 21, 0, 0, 0, 
	27, 0, 0, 0, 33, 0, 0, 0, 29, 0, 
	0, 0, 34, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 36, 0, 0, 0, 0, 0, 
	160, 0, 0, 0, 0, 0, 28, 0, 0, 0, 
	23, 0, 0, 0, 122, 0, 0, 0, 26, 0, 
	0, 0, 0, 0, 0, 0, 101, 0, 0, 0, 
	0, 0, 0, 0, 37, 0, 0, 0, 0, 0, 
	0, 0, 51, 0, 0, 0, 106, 0, 0, 0, 
	0, 0, 0, 0, 110, 0, 0, 0, 0, 0, 
	0, 0, 43, 0, 0, 0, 44, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	45, 0, 0, 0, 102, 0, 0, 0, 0, 0, 
	226, 0, 0, 0, 0, 0, 0, 0, 47, 0, 
	0, 85, 50, 0, 0, 0, 52, 0, 0, 0, 
	48, 0, 0, 53, 0, 0, 0, 0, 57, 0, 
	0, 0, 0, 0, 58, 0, 0, 0, 60, 0, 
	0, 0, 175, 0, 0, 0, 0, 0, 0, 0, 
	67, 0, 63, 61, 0, 0, 0, 0, 71, 0, 
	0, 0, 70, 0, 0, 0, 69, 0, 0, 0, 
	72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 56, 0, 0, 0, 0, 0, 0, 0, 
	77, 0, 0, 0, 0, 0, 55, 0, 54, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 76, 0, 0, 0, 0, 0, 0, 0, 
	79, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	80, 0, 0, 0, 78, 0, 0, 0, 82, 0, 
	0, 0, 86, 0, 0, 0, 0, 0, 0, 0, 
	84, 0, 0, 0, 83, 0, 0, 0, 87, 0, 
	0, 0, 94, 0, 0, 0, 91, 0, 0, 0, 
	88, 0, 0, 0, 93, 0, 0, 0, 92, 0, 
	0, 0, 89, 0, 0, 0, 90, 0, 0, 0, 
	95, 0, 0, 0, 0, 0, 0, 0, 96, 0, 
	0, 0, 98, 0, 0, 0, 0, 0, 107, 0, 
	97, 0, 0, 0, 99, 0, 0, 0, 103, 0, 
	104, 0, 0, 0, 105, 0, 0, 100, 108, 0, 
	0, 0, 109, 0, 0, 0, 112, 0, 115, 0, 
	111, 0, 0, 0, 116, 0, 0, 0, 0, 0, 
	113, 0, 114, 0, 0, 0, 124, 0, 0, 0, 
	120, 0, 0, 0, 129, 0, 0, 0, 132, 0, 
	0, 0, 128, 0, 0, 0, 0, 0, 0, 0, 
	126, 0, 0, 0, 0, 0, 0, 0, 125, 0, 
	127, 0, 0, 0, 130, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 123, 0, 118, 0, 
	0, 0, 0, 0, 117, 0, 0, 0, 133, 0, 
	0, 0, 142, 0, 0, 0, 135, 0, 0, 0, 
	0, 0, 0, 0, 140, 0, 0, 0, 138, 0, 
	0, 0, 7, 14, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 203, 0, 
	0, 0, 0, 0, 141, 0, 0, 0, 137, 0, 
	0, 0, 0, 0, 0, 0, 136, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 139, 0, 
	0, 0, 0, 0, 0, 0, 147, 0, 0, 0, 
	143, 0, 0, 0, 0, 0, 0, 0, 145, 0, 
	151, 0, 0, 0, 144, 0, 0, 0, 146, 0, 
	0, 0, 0, 0, 0, 0, 149, 0, 0, 0, 
	150, 0, 0, 0, 81, 0, 185, 0, 0, 0, 
	0, 0, 0, 0, 152, 0, 0, 0, 0, 0, 
	0, 0, 153, 156, 0, 0, 158, 0, 0, 0, 
	0, 0, 0, 0, 166, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 174, 0, 
	0, 0, 159, 0, 0, 0, 0, 0, 0, 0, 
	161, 0, 0, 0, 169, 0, 0, 0, 0, 0, 
	0, 0, 165, 168, 202, 167, 170, 0, 0, 0, 
	224, 0, 0, 0, 0, 0, 227, 0, 0, 0, 
	220, 0, 0, 0, 66, 0, 0, 0, 173, 0, 
	0, 0, 0, 0, 0, 0, 162, 0, 0, 0, 
	172, 0, 0, 0, 0, 0, 0, 0, 177, 0, 
	0, 0, 164, 0, 0, 0, 40, 0, 0, 0, 
	176, 0, 179, 0, 178, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 184, 0, 0, 0, 
	188, 0, 0, 0, 2, 0, 0, 0, 183, 0, 
	0, 0, 186, 0, 0, 181, 0, 0, 0, 0, 
	193, 0, 0, 0, 192, 0, 0, 121, 0, 0, 
	157, 0, 0, 0, 0, 0, 0, 0, 62, 0, 
	0, 0, 0, 0, 0, 0, 73, 0, 0, 0, 
	0, 0, 0, 0, 190, 0, 0, 0, 0, 0, 
	194, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 198, 0, 
	199, 0, 200, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 204, 0, 0, 0, 221, 0, 0, 0, 
	0, 222, 0, 0, 225, 0, 0, 0, 0, 0, 
	0, 189, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 49, 228, 0, 182, 0, 75, 201, 163, 0, 
	197, 155, 0, 134, 16, 0, 154, 39, 41, 187, 
	205, 212, 214, 216, 0, 208, 209, 210, 211, 207, 
	213, 206, 217, 218, 215, 0, 0, 0, 171, 119, 
	46, 4, 180, 8, 35, 24, 38, 17, 68, 131, 
	191, 74, 10, 0, 30, 148, 31, 0, 0, 0, 
	42, 223, 22, 20, 0, 65, 64, 195, 196, 219
};

// if you add currency names, countries, denominations make sure you add the names in the position that maintains 
// a correct alphabetic order or else the binary chop search in setCurrency(std::string) won't work
const char * const Currency::m_ccyNames[NUMCURRENCY] = { 
    "NOCURRENCY", // 10 codes per row
	"ADP", "AED", "AFA", "AFN", "ALL", "AMD", "ANG", "AOA", "AON", "AOR", 
	"ARS", "ATS", "AUD", "AWG", "AZM", "AZN", "BAM", "BBD", "BDT", "BEC", 
	"BEF", "BEL", "BGL", "BGN", "BHD", "BIF", "BMD", "BND", "BOB", "BOV", 
	"BRL", "BSD", "BTN", "BWP", "BYR", "BZD", "CAD", "CDF", "CHE", "CHF", 
	"CHW", "CLF", "CLP", "CNY", "COP", "COU", "CRC", "CSK", "CUC", "CUP", 
	"CVE", "CYP", "CZK", "DDM", "DEM", "DJF", "DKK", "DOP", "DZD", "ECS", 
	"EEK", "EGP", "ERN", "ESA", "ESB", "ESP", "ETB", "EUR", "FIM", "FJD", 
	"FKP", "FRF", "GBP", "GEL", "GHS", "GIP", "GMD", "GNF", "GRD", "GTQ", 
	"GWP", "GYD", "HKD", "HNL", "HRK", "HTG", "HUF", "IDR", "IEP", "ILS", 
	"INR", "IQD", "IRR", "ISK", "ITL", "JMD", "JOD", "JPY", "KES", "KGS", 
	"KHR", "KMF", "KPW", "KRW", "KWD", "KYD", "KZT", "LAK", "LBP", "LKR", 
	"LRD", "LSL", "LTL", "LUF", "LVL", "LYD", "MAD", "MDL", "MGA", "MGF", 
	"MKD", "MMK", "MNT", "MOP", "MRO", "MTL", "MUR", "MVR", "MWK", "MXN", 
	"MXV", "MYR", "MZM", "MZN", "NAD", "NGN", "NIO", "NLG", "NOK", "NPR", 
	"NZD", "OMR", "PAB", "PEN", "PGK", "PHP", "PKR", "PLN", "PLZ", "PTE", 
	"PYG", "QAR", "ROL", "RON", "RSD", "RUB", "RUR", "RWF", "SAR", "SBD", 
	"SCR", "SDD", "SDG", "SEK", "SGD", "SHP", "SIT", "SKK", "SLL", "SOS", 
	"SRD", "SRG", "SSP", "STD", "SVC", "SYP", "SZL", "THB", "TJR", "TJS", 
	"TMM", "TMT", "TND", "TOP", "TPE", "TRL", "TRY", "TTD", "TWD", "TZS", 
	"UAH", "UAK", "UGX", "USD", "USN", "USS", "UYI", "UYU", "UZS", "VEB", 
	"VEF", "VND", "VUV", "WST", "XAF", "XAG", "XAU", "XBA", "XBB", "XBC", 
	"XBD", "XCD", "XDR", "XOF", "XPD", "XPF", "XPT", "XTS", "XXX", "YDD", 
	"YER", "YUM", "ZAL", "ZAR", "ZMK", "ZRN", "ZWD", "ZWL"
};



const char * const Currency::m_denomNames[NUMCURRENCY] = { "No Currency", // 10 names per row
	"Andorran Peseta (1:1 peg to the Spanish Peseta)", "UAE Dirham", "Afghani", "Afghani", "Lek", "Armenian Dram", "Netherlands Antillian Guilder", "Kwanza", "Angolan New Kwanza", "Angolan Kwanza Readjustado", 
	"Argentine Peso", "Austrian Schilling", "Australian Dollar", "Aruban Guilder", "Azerbaijani Manat", "Azerbaijanian Manat", "Convertible Marks", "Barbados Dollar", "Taka", "Belgian Franc (convertible)", 
	"Belgian Franc (currency union with LUF)", "Belgian Franc (financial)", "Bulgarian Lev A/99", "Bulgarian Lev", "Bahraini Dinar", "Burundi Franc", "Bermudian Dollar", "Brunei Dollar", "Boliviano", "Mvdol", 
	"Brazilian Real", "Bahamian Dollar", "Ngultrum", "Pula", "Belarussian Ruble", "Belize Dollar", "Canadian Dollar", "Franc Congolais", "WIR Euro", "Swiss Franc", 
	"WIR Franc", "Unidades de Fomento", "Chilean Peso", "Yuan Renminbi", "Colombian Peso", "Unidad de Valor Real", "Costa Rican Colon", "Czechoslovak Koruna", "Peso Convertible ", "Cuban Peso", 
	"Cape Verde Escudo", "Cyprus Pound", "Czech Koruna", "East German Mark", "Deutsche Mark", "Djibouti Franc", "Danish Krone", "Dominican Peso", "Algerian Dinar", "Ecuador Sucre", 
	"Kroon", "Egyptian Pound", "Nakfa", "Spanish Peseta (Account A)", "Spanish Peseta (Account B)", "Spanish Peseta", "Ethiopian Birr", "Euro", "Finnish Markka", "Fiji Dollar", 
	"Falkland Island Pound", "French Franc", "Pound Sterling", "Lari", "Cedi", "Gibraltar Pound", "Dalasi", "Guinea Franc", "Greek Drachma", "Quetzal", 
	"Guinea-Bissau Peso", "Guyana Dollar", "Hong Kong Dollar", "Lempira", "Croatian Kuna", "Gourde", "Forint", "Rupiah", "Irish Punt", "New Israeli Sheqel", 
	"Indian Rupee", "Iraqi Dinar", "Iranian Rial", "Iceland Krona", "Italian Lira", "Jamaican Dollar", "Jordanian Dinar", "Yen", "Kenyan Shilling", "Som", 
	"Riel", "Comoro Franc", "North Korean Won", "Won", "Kuwaiti Dinar", "Cayman Islands Dollar", "Tenge", "Kip", "Lebanese Pound", "Sri Lanka Rupee", 
	"Liberian Dollar", "Loti", "Lithuanian Litas", "Luxembourg Franc (currency union with BEF)", "Latvian Lats", "Libyan Dinar", "Moroccan Dirham", "Moldovan Leu", "Malagasy Ariary", "Malagasy Franc", 
	"Denar", "Kyat", "Tugrik", "Pataca", "Ouguiya", "Maltese Lira", "Mauritius Rupee", "Rufiyaa", "Kwacha", "Mexican Peso", 
	"Mexican Unidad de Inversion (UDI)", "Malaysian Ringgit", "Mozambican Metical", "Metical", "Namibia Dollar", "Naira", "Cordoba Oro", "Netherlands Guilder", "Norwegian Krone", "Nepalese Rupee", 
	"New Zealand Dollar", "Rial Omani", "Balboa", "Nuevo Sol", "Kina", "Philippine Peso", "Pakistan Rupee", "Zloty", "Polish Zloty A/94", "Portuguese Escudo", 
	"Guarani", "Qatari Rial", "Romanian Leu A/05", "New Leu", "Serbian Dinar", "Russian Ruble", "Russian RubleA/97", "Rwanda Franc", "Saudi Riyal", "Solomon Islands Dollar", 
	"Seychelles Rupee", "Sudanese Dinar", "Sudanese Pound", "Swedish Krona", "Singapore Dollar", "Saint Helena Pound", "Slovenian Tolar", "Slovak Koruna", "Leone", "Somali Shilling", 
	"Surinam Dollar", "Suriname Guilder", "South Sudanese Pound", "Dobra", "El Salvador Colon", "Syrian Pound", "Lilangeni", "Baht", "Tajikistan Ruble", "Somoni", 
	"Manat", "Manat", "Tunisian Dinar", "Paanga", "Portuguese Timorese Escudo", "Turkish Lira A/05", "Turkish Lira", "Trinidad and Tobago Dollar", "New Taiwan Dollar", "Tanzanian Shilling", 
	"Hryvnia", "Ukrainian Karbovanets", "Uganda Shilling", "US Dollar", "US Dollar (Next Day)", "US Dollar (Same Day)", "Uruguay Peso en Unidades Indexadas", "Peso Uruguayo", "Uzbekistan Sum", "Venezuelan Bolivar", 
	"Bolivar Fuerte", "Dong", "Vatu", "Tala", "CFA Franc BEAC", "Silver", "Gold", "EURCO", "E.M.U.-6", "E.U.A.-9", 
	"E.U.A.-17", "East Caribbean Dollar", "Special Drawing Rights", "CFA Franc BCEAO", "Palladium", "CFP Franc", "Platinum", "Test Code", "No Currency", "South Yemeni Dinar", 
	"Yemeni Rial", "Yugoslav Dinar", "South African Financial Rand (funds code) ", "Rand", "Zambian Kwacha", "Zairean New Zaire", "Zimbabwe Dollar", "Zimbabwe Dollar"
};


//
//
//	




