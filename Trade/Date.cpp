/* A date class 04/03/19
 
 $$$$$$$$$$$$$$$$$$$$$$$
 $   Date.cpp - code   $
 $$$$$$$$$$$$$$$$$$$$$$$
 
 by W.B. Yates
 Copyright (c) 2019 W.B. Yates. All rights reserved.
 History:

 
 A Date class that provides dates from 01/01/-4713 to 31/12/9999. 
 
 Operations on dates between 01/01/1900 and to 31/12/2149 use lookup tables and are most efficient.
 
 The internal serial number 1 represents the (Gregorian) date 01/01/1900. That is 'Gregorian now' - gregorian(1,1,1900) + 1
 This class can deal with the Microsoft PC EXCEL leap year error, that is, that the year 1900 is supposed (incorrectly) to be a leap year. 
 For example the internal serial 91311 is 31-Dec-2149 in the (correct) STANDARD calendar, and the internal serial 91312 is 31-Dec-2149 in the EXCEL calendar.  

 The code in comments below can be used to generate the STANDARD/EXCEL Gregorian calendar lookup tables used in this class.

 Copyright Issues:
 
 Julian number algorithms are taken from Numerical Recipes Software; see http://www.numerical.recipes   
 
 Copyright (C) 1998 by Numerical Recipes Software.  Permission is
 granted for unlimited copying and redistribution, provided that this
 copyright notice is displayed, together with the two following links
 
 http://www.numerical.recipes
 
 http://www.nr.com/index.html
 
 Gregorian number algorithm are taken from the book
 
 "Calendrical Calculations" by Nachum Dershowitz and Edward M. Reingold,
 Software---Practice & Experience, vol. 20, no. 9 (September, 1990),pp. 899--928.
 
 This code is in the public domain, but any use of it should acknowledge its source.
 See http://www.cs.sunysb.edu/~algorith/implement/reingold/distrib/calendar.C
 
 When extending the table always check your max serial date fits into an int, 
 i.e assert(SERIAL_MAX < INT_MAX); assert(SERIAL_MIN > INT_MIN); somewhere

	const int TABLE_MIN = 1900;
	const int TABLE_MAX = 2149;
	 
 void
 Date::printTables( void )
 {	
     
     for (int year = TABLE_MIN; year < TABLE_MAX + 2; ++year)
     {
         // for EXCEL tables add 2 instead of 1
         std::cout << int(gregorian(1, 1, year) - GREGOR_BASE) + 1  << ", ";
         if (((year + 1) % 10) == 0) 
            std::cout << std::endl;
     }
 
     std::cout << std::endl;
     
     bool leap = false;
     for (int year = TABLE_MIN; year < TABLE_MAX + 2; ++year)
     {
         if (year % 400 == 0) 
            leap = true;          
         else if ((year % 4 == 0) && (year % 100 != 0)) 
            leap = true; 
         else leap = false; 
         
         std::cout << leap << ", ";
         if (((year + 1) % 10) == 0) 
            std::cout << std::endl;
     }
 }
*/

#ifndef __DATE_H__
#include "Date.h"
#endif


#include <ctime>
#include <fstream>
#include <assert.h>

std::istream& 
operator>>(std::istream& strm, Date& date)
{
	std::string buff;
	strm >> buff;
	date.setDate(buff);
	return strm;
}

// We default to the standard calendar
Date::Calendar Date::m_calAdjustment = Date::STANDARD; 
const int*	Date::m_pYearStart       = Date::m_yearStart;
const char*	Date::m_pLeapYear        = Date::m_leapYear;

const int TABLE_MIN = 1900;
const int TABLE_MAX = 2149;

const int YEAR_MIN = -4713;
const int YEAR_MAX = 9999;

// if numbers above change...
//const long GREGOR_BASE =  Date::gregorian(1,1,TABLE_MIN);                    // = 693596;
//const long SERIAL_MIN  =  Date::gregorian(1,1,YEAR_MIN) - GREGOR_BASE + 1;   // = -2415346; has Julian number 0
//const long SERIAL_MAX  =  Date::gregorian(31,12,YEAR_MAX) - GREGOR_BASE + 1; // = 2958464; has Julian number 5373484
//const long SERIAL_BCE  =  Date::gregorian(31,12,-1) - GREGOR_BASE + 1;       // = -693960;
//const long SERIAL_ACE  =  Date::gregorian(1,1,1) - GREGOR_BASE + 1;          // = -693594; 

// faster on initialise as no calls to gregorian function
const long GREGOR_BASE = 693596;   // Date::gregorian(1,1,TABLE_MIN)
const long SERIAL_MIN  = -2415346; // Date::gregorian(1,1,YEAR_MIN) - GREGOR_BASE + 1; has Julian number 0
const long SERIAL_MAX  = 2958464;  // Date::gregorian(31,12,YEAR_MAX) - GREGOR_BASE + 1; has Julian number 5373484
const long SERIAL_BCE  = -693960;  // Date::gregorian(31,12,-1) - GREGOR_BASE + 1; 
const long SERIAL_ACE  = -693594;  // Date::gregorian(1,1,1) - GREGOR_BASE + 1;

// static member initialisation 
Date Date::m_today   = Date::systemDate();
Date Date::m_maxDate = Date(31,12,YEAR_MAX);

const char * const Date::m_dayNames[7] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };

const char * const Date::m_monthNames[13] = { 
	"December", "January", "February", "March",
	"April", "May", "June", "July", "August",
	"September", "October", "November", "December" 
};
	

const int Date::m_yearStart[] = {
	1, 366, 731, 1096, 1461, 1827, 2192, 2557, 2922, 3288, 
	3653, 4018, 4383, 4749, 5114, 5479, 5844, 6210, 6575, 6940, 
	7305, 7671, 8036, 8401, 8766, 9132, 9497, 9862, 10227, 10593, 
	10958, 11323, 11688, 12054, 12419, 12784, 13149, 13515, 13880, 14245, 
	14610, 14976, 15341, 15706, 16071, 16437, 16802, 17167, 17532, 17898, 
	18263, 18628, 18993, 19359, 19724, 20089, 20454, 20820, 21185, 21550, 
	21915, 22281, 22646, 23011, 23376, 23742, 24107, 24472, 24837, 25203, 
	25568, 25933, 26298, 26664, 27029, 27394, 27759, 28125, 28490, 28855, 
	29220, 29586, 29951, 30316, 30681, 31047, 31412, 31777, 32142, 32508, 
	32873, 33238, 33603, 33969, 34334, 34699, 35064, 35430, 35795, 36160, 
	36525, 36891, 37256, 37621, 37986, 38352, 38717, 39082, 39447, 39813, 
	40178, 40543, 40908, 41274, 41639, 42004, 42369, 42735, 43100, 43465, 
	43830, 44196, 44561, 44926, 45291, 45657, 46022, 46387, 46752, 47118, 
	47483, 47848, 48213, 48579, 48944, 49309, 49674, 50040, 50405, 50770, 
	51135, 51501, 51866, 52231, 52596, 52962, 53327, 53692, 54057, 54423, 
	54788, 55153, 55518, 55884, 56249, 56614, 56979, 57345, 57710, 58075, 
	58440, 58806, 59171, 59536, 59901, 60267, 60632, 60997, 61362, 61728, 
	62093, 62458, 62823, 63189, 63554, 63919, 64284, 64650, 65015, 65380, 
	65745, 66111, 66476, 66841, 67206, 67572, 67937, 68302, 68667, 69033, 
	69398, 69763, 70128, 70494, 70859, 71224, 71589, 71955, 72320, 72685, 
	73050, 73415, 73780, 74145, 74510, 74876, 75241, 75606, 75971, 76337, 
	76702, 77067, 77432, 77798, 78163, 78528, 78893, 79259, 79624, 79989, 
	80354, 80720, 81085, 81450, 81815, 82181, 82546, 82911, 83276, 83642, 
	84007, 84372, 84737, 85103, 85468, 85833, 86198, 86564, 86929, 87294, 
	87659, 88025, 88390, 88755, 89120, 89486, 89851, 90216, 90581, 90947, 
	91312 
}; 


const int Date::m_xlYearStart[] = {
	1, 367, 732, 1097, 1462, 1828, 2193, 2558, 2923, 3289, 
	3654, 4019, 4384, 4750, 5115, 5480, 5845, 6211, 6576, 6941, 
	7306, 7672, 8037, 8402, 8767, 9133, 9498, 9863, 10228, 10594, 
	10959, 11324, 11689, 12055, 12420, 12785, 13150, 13516, 13881, 14246, 
	14611, 14977, 15342, 15707, 16072, 16438, 16803, 17168, 17533, 17899, 
	18264, 18629, 18994, 19360, 19725, 20090, 20455, 20821, 21186, 21551, 
	21916, 22282, 22647, 23012, 23377, 23743, 24108, 24473, 24838, 25204, 
	25569, 25934, 26299, 26665, 27030, 27395, 27760, 28126, 28491, 28856, 
	29221, 29587, 29952, 30317, 30682, 31048, 31413, 31778, 32143, 32509, 
	32874, 33239, 33604, 33970, 34335, 34700, 35065, 35431, 35796, 36161, 
	36526, 36892, 37257, 37622, 37987, 38353, 38718, 39083, 39448, 39814, 
	40179, 40544, 40909, 41275, 41640, 42005, 42370, 42736, 43101, 43466, 
	43831, 44197, 44562, 44927, 45292, 45658, 46023, 46388, 46753, 47119, 
	47484, 47849, 48214, 48580, 48945, 49310, 49675, 50041, 50406, 50771, 
	51136, 51502, 51867, 52232, 52597, 52963, 53328, 53693, 54058, 54424, 
	54789, 55154, 55519, 55885, 56250, 56615, 56980, 57346, 57711, 58076, 
	58441, 58807, 59172, 59537, 59902, 60268, 60633, 60998, 61363, 61729, 
	62094, 62459, 62824, 63190, 63555, 63920, 64285, 64651, 65016, 65381, 
	65746, 66112, 66477, 66842, 67207, 67573, 67938, 68303, 68668, 69034, 
	69399, 69764, 70129, 70495, 70860, 71225, 71590, 71956, 72321, 72686, 
	73051, 73416, 73781, 74146, 74511, 74877, 75242, 75607, 75972, 76338, 
	76703, 77068, 77433, 77799, 78164, 78529, 78894, 79260, 79625, 79990, 
	80355, 80721, 81086, 81451, 81816, 82182, 82547, 82912, 83277, 83643, 
	84008, 84373, 84738, 85104, 85469, 85834, 86199, 86565, 86930, 87295, 
	87660, 88026, 88391, 88756, 89121, 89487, 89852, 90217, 90582, 90948, 
	91313 
};


// The standard convention. Note, these are the correct values (see EXCEL below)
const char Date::m_leapYear[] = {
	0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 
	0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 
	1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 
	0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 
	1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 
	0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 
	1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 
	0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 
	1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 
	0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 
	1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 
	0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 
	1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 
	0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 
	1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 
	0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 
	1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 
	0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 
	1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 
	0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 
	0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 
	0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 
	1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 
	0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 
	1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 
	0 	
}; 


// The EXCEL convention. Note EXCEL mistakenly considers 1900 to be a leap year
const char Date::m_xlLeapYear[] = {
	1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 
	0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 
	1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 
	0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 
	1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 
	0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 
	1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 
	0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 
	1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 
	0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 
	1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 
	0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 
	1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 
	0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 
	1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 
	0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 
	1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 
	0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 
	1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 
	0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 
	0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 
	0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 
	1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 
	0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 
	1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 
	0 	
};


const char Date::m_daysPerMonth[12] = {
	31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};


const char Date::m_leapDaysPerMonth[12] = {
	31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};


bool 
Date::setDate( const int d, const int m, const int y )
{
    m_day    = 0;
    m_month  = 0;
    m_year   = 0;
    m_serial = 0;
    
	if ( Date::valid( d, m, y ) )
    {
	    m_day	 = char(d);
	    m_month	 = char(m);
	    m_year	 = short(y);
		        
        m_serial = ((m_year >= TABLE_MIN) && (m_year <= TABLE_MAX)) ? (m_pYearStart[m_year - TABLE_MIN]) : (yearAsSerial(m_year));

        // add the months
	    for (int i = 1; i < m_month; ++i)         
		    m_serial += Date::daysInMonth(i, m_year);

        //add the days
	    m_serial += m_day - 1;
    }
	
    return valid();
}

int
Date::rollSerial( const int serial ) const
{
    if ((serial > SERIAL_BCE) && (serial < SERIAL_ACE)) // is this a year zero serial number?
        return (serial < m_serial) ? (-365 + serial) : (365 + serial);

    return serial;
}

bool 
Date::setDate( const int serial )
// sets the Date to serial s and determines the day, months, and year
{    
    
    if ((serial >= SERIAL_MIN) && (serial <= SERIAL_MAX))
    {
	int yearStart;
	int dInYear;
	int daysCounted;
	int day;
	int month;
	int year;
	    
	daysCounted = 0;
	month       = 0;
		
        m_serial = rollSerial( serial );
        
	year = int(double(m_serial) / 365.2425) - (m_serial < 0);
        
        yearStart = yearAsSerial(year + TABLE_MIN);
        
	if (m_serial < yearStart)
	    yearStart = yearAsSerial(--year + TABLE_MIN);
		
	dInYear = m_serial - yearStart;         
	    year += TABLE_MIN;                              
		
	if (Date::isLeapYear(year))                      
        {
            // days must be between 0 and 365
            do
            {
		daysCounted += m_leapDaysPerMonth[month];
		++month;
	    }
	    while ((daysCounted <= dInYear) && (month < 12));
		    
            daysCounted -= m_leapDaysPerMonth[month-1];
	}
	else
        { 
            // days must be between 0 and 364
            do
            {
		daysCounted += m_daysPerMonth[month];
		++month;
            }
	    while ((daysCounted <= dInYear) && (month < 12));
		    
            daysCounted -= m_daysPerMonth[month-1];
	}
	day = dInYear - daysCounted + 1;
        
        assert(valid(day, month, year));
        
        m_day   = char(day);
        m_month = char(month);
        m_year  = short(year);
    }
    else
    {
        // we do this here because serialRoll depends on m_serial
        m_day    = 0;
        m_month  = 0;
        m_year   = 0;
        m_serial = 0;
    }

	
    return valid();
}


bool
Date::setDate( const std::string& dateString, const std::string& format ) 
{
    int day = 0, month = 0, year = 0;
    
    // check for common conventions
	if (format == "YYYY-MM-DD") // default arg; ISO standard date convention
	{
		std::sscanf(dateString.c_str(), "%4d-%2d-%2d", &year, &month, &day);
		return setDate(day, month, year);
	}
	if (format == "DD-MM-YYYY") // European convention
	{		
		std::sscanf(dateString.c_str(), "%2d-%2d-%4d", &day, &month, &year);
		return setDate(day, month, year);
	} 
	if (format == "MM-DD-YYYY") // USA convention
	{		
		std::sscanf(dateString.c_str(), "%2d-%2d-%4d", &month, &day, &year);
		return setDate(day, month, year);
	}
    
	// step through the sequence; expensive compared to a common format
	const int START  = 0;
	const int DAY2   = 1;
	const int MONTH2 = 3;
	const int MONTH3 = 4;
	const int YEAR2  = 5;
	const int YEAR3  = 6;
	const int YEAR4  = 7;
	const int DELIM  = 8;
	const int FAIL   = 9;
	
	int idx = 0;
	int state = START;
	
	const char *datestr = dateString.c_str();
	
   	bool finished = false;
	while( !finished )
	{
		switch( state )
		{
                        case START:
                                if (format[idx] == 'D')
                                {
                                        ++idx;
                                        state = DAY2;
                                         break;
                                }
                                if (format[idx] == 'M')
                                {
                                        ++idx;
                                        state = MONTH2;
                                        break;
                                }
                                if (format[idx] == 'Y') 
                                {
                                        ++idx;
                                        state = YEAR2;
                                        break;
                                }
                                state = FAIL;
                                break;
			case DAY2:
                                if (format[idx] == 'D')
				{		
                                        ++idx;
                                        int digits;
                                        if (std::sscanf( datestr, "%2d%n", &day, &digits ))
                                        {
                                                state = DELIM;
                                                datestr += digits;
                                                break;
                                        }
				} 
                                state = FAIL;
				break;
			case MONTH2:
				if (format[idx] == 'M')
				{
					++idx;
					state = MONTH3;
                                         break;
				}
                                state = FAIL;
				break;
			case MONTH3:
				if (format[idx] == 'M')
				{
                                        ++idx;
					char buff[4];
					if (std::sscanf( datestr, "%3s",  buff ))
					{
                                                buff[0] = char(std::toupper(buff[0])); // This should match our month names
                                                buff[1] = char(std::tolower(buff[1]));
                                                buff[2] = char(std::tolower(buff[2]));
                                                buff[3] = '\0';
                        
                                                for(month = 1; month <= 12; ++month)
                                                {
                                                        if(!std::strncmp(buff, m_monthNames[month], 3))
                                                        {
                                                                break;
                                                        }
                                                }
                        
                                                if (month <= 12)
                                                {
                                                    state = DELIM;
                                                    datestr += 3;
                                                    break;
                                                }
                                         }
				}
                                else
                                {
                                    int digits;
                                    if (std::sscanf( datestr, "%2d%n", &month, &digits ))
                                    {
                                        state = DELIM;
                                        datestr += digits;
                                        break;
                                    }
                                }
                                state = FAIL;
                                break;
			case YEAR2:
				if (format[idx] == 'Y')
				{
					++idx;
					state = YEAR3;
                                        break;
				}
                                state = FAIL;
				break;
			case YEAR3:
				if (format[idx] == 'Y')
				{
					++idx;
					state = YEAR4;
                                        break;
				}
                                if (std::sscanf(datestr, "%2d", &year))
                                {
                                    if( (year > 0) && (year < 70) )
                                        year += 2000;
                                    else if( year < 100 )
                                        year += TABLE_MIN;

                                    state = DELIM;
                                    datestr += 2;
                                    break;
                                }
                                state = FAIL;
				break;
			case YEAR4:
				if (format[idx] == 'Y')
				{
				        ++idx;
                                        if (std::sscanf( datestr, "%4d",  &year ))
                                        {
                                                datestr += 4;
                                                state = DELIM;
                                                break;
                                        }
				}
                                state = FAIL;
				break;
			case DELIM:
                                if (format[idx] == '\0')
                                {
                                    finished = true;
                                    break;
                                }
                                if (format[idx] == *datestr)
                                {
                                    ++datestr;
                                    ++idx;
                                    state = START;
                                    break;
                                }
                                state = FAIL;
				break;
			case FAIL:
				std::cout << "Date: Bad format " << format << " for " << dateString << std::endl;
				return false;
				break;
			default:
				std::cout << "Date: Format error " << format << " for " << dateString << std::endl;
				return false;
				break;
				
		}
		
	}
    
	return setDate(day,month,year);
}


int  // i.e. Sun = 1 to Sat = 7 
Date::dayNumber( void ) const 
// see https://en.wikipedia.org/wiki/Julian_day#Finding_day_of_week_given_Julian_day_number
{
    return (m_serial > 0) ? gregorianDayNumber(m_serial) : julianDayNumber(julian());
}


std::string 
Date::dayName( void ) const { return m_dayNames[dayNumber()-1]; }


std::string 
Date::monthName( void ) const
{
	return m_monthNames[month()];
}


const Date 
Date::operator+( const int days ) const
{
	return Date(*this) += days;
}


const Date 
Date::operator-( const int days ) const
{
	return Date(*this) -= days;
}


int 
Date::operator-( const Date& date ) const
// the number of calendar days between two dates
{
	return m_serial - date.m_serial;
}


Date& 
Date::operator+=( const int days )
{
	setDate(m_serial + days);
	return *this;
}


Date& 
Date::operator-=( const int days )
{
	setDate(m_serial - days);
	return *this;
}


Date& 
Date::operator++( void )
// prefix increment
{
	setDate(m_serial + 1);
	return *this;
}


Date& 
Date::operator--( void )
// prefix decrement
{
	setDate(m_serial - 1);
	return *this;
}
 

Date 
Date::operator++( const int )
// postfix increment
{
	Date d = *this;
	setDate(m_serial + 1);
	return d;
}


Date 
Date::operator--( const int )
// postfix decrement
{
	Date d = *this;
	setDate(m_serial - 1);
	return d;
}


bool
Date::addWeekDays(const int days)
// Note day number is calculated from the julian
{
    int serial = m_serial;
    int increment = 1;
    int workdays = days; 
    
    if (days < 0)
    {
        increment = -1;
        workdays = -days; 
    }

    if ((m_serial + days) > 0)
    {
        for ( int i = 0; i < workdays; ++i )
        {
            serial += increment;
            
            int dow = gregorianDayNumber(serial); 
            while (dow == Saturday || dow == Sunday)
            {
                serial += increment;
                dow = gregorianDayNumber(serial);
            }
        }           
    }
    else
    {
        long jd = julian(); 
        
        for ( int i = 0; i < workdays; ++i )
        {
            jd += increment;
            serial += increment;

            int dow = julianDayNumber(jd);
            while (dow == Saturday || dow == Sunday)
            {
                jd += increment;
                serial += increment;
                dow = julianDayNumber(jd);
            }
        }
    }
    return setDate(serial);
}

 
bool 
Date::addMonths( const int months )
{  

    int d = 0, m = 0, y = 0;
    
    div_t res;
    
    if (m_year < 0)
    {
        int inMonths = (12 * (-m_year)) + m_month + months - 1;
        res = div(inMonths, 12);
        y = m_year + (res.quot + m_year);       
    }
    else
    {
        int inMonths = (12 * (m_year)) + m_month + months - 1;
        res = div(inMonths, 12);
        y = res.quot;       
    }	     
    
    m = res.rem + 1;

    if (y == 0)
    {
        if (months < 0)
            --y;
        else ++y;
    }
 
    int dim = Date::daysInMonth(m, y);	
    d = m_day;
    
    if (d > dim)
    {
        d = dim;
    }
    
    return setDate(d, m, y);
}


bool 
Date::addYears( const int years )
{
    int nYear = m_year + years;

    if (nYear == 0)
    {
        if (years < 0)
            --nYear;
        else ++nYear;
    }
    
    return setDate( std::min( Date::daysInMonth(m_month, m_year + years), (int) m_day ), m_month, nYear );
}


int
Date::daysInMonth( void ) const
{
	return Date::daysInMonth(m_month, m_year);
}


bool 
Date::isLeapYear( void ) const
{
	return Date::isLeapYear(m_year);
}


bool 
Date::isWeekend( void ) const
{
    int dow = dayNumber();
	
    return (dow == Saturday || dow == Sunday);
}


bool 
Date::isMonthEnd( void ) const
{
    return (m_day == lastDayOfMonth());
}


int 
Date::lastDayOfMonth( void ) const
{
    if (m_month == February)
    {
        if (isLeapYear())
        {
            return 29;
        }
        else
        {
            return 28;
        }
    }
    else if (m_month == April || m_month == June || m_month == September || m_month == November)
    {
        return 30;
    }
    else
    {
        return 31;
    }
}


bool               
Date::next( const Day d )
{    
    int serial = m_serial;

    if (m_serial > 0)
    {
        ++serial;
        int dow = gregorianDayNumber(serial);
        while (dow != d)
        {
            ++serial;
            dow = gregorianDayNumber(serial);
        }
    }
    else
    {
        long jd = julian(); 
        
        ++jd;
        ++serial;
        int dow = julianDayNumber(jd);
        while (dow != d)
        {
            ++jd;
            ++serial;
            dow = julianDayNumber(jd);
        }
    }
    return setDate(serial);
}


bool
Date::prev( const Day d )
{
    int serial = m_serial;
 
    if ((m_serial - 7) > 0)
    {
        --serial;
        int dow = gregorianDayNumber(serial);
        while (dow != d)
        {
            --serial;
            dow = gregorianDayNumber(serial);
        }
    }
    else
    {
        long jd = julian(); 
        
        --jd;
        --serial;
        int dow = julianDayNumber(jd);
        while (dow != d)   
        {
            --jd;
            --serial;
            dow = julianDayNumber(jd);
        }
    }
    return setDate(serial);
}


bool 
Date::nextWeekDay( void ) 
{
    int serial = m_serial;
 
    if (m_serial > 0)
    {
        ++serial;
        int dow = gregorianDayNumber(serial); // Note day number is calculated from the serial
        while (dow == Saturday || dow == Sunday)
        {
            ++serial;
            dow = gregorianDayNumber(serial); 
        }
    }
    else
    {
        long jd = julian(); 
        
        ++jd;
        ++serial;
        int dow = julianDayNumber(jd); // Note day number is calculated from the julian
        while (dow == Saturday || dow == Sunday)
        {
            ++jd;
            ++serial;
            dow = julianDayNumber(jd); 
        }
    }
    return setDate(serial);
}


bool 
Date::prevWeekDay( void )
{    
    int serial = m_serial;
    
    if ((m_serial - 7) > 0)
    {
        --serial;
        int dow = gregorianDayNumber(serial); // Note day number is calculated from the serial
        while (dow == Saturday || dow == Sunday)   
        {
            --serial;
            dow = gregorianDayNumber(serial); 
        }
    }
    else
    {
        long jd = julian(); 
        
        --jd;
        --serial;
        int dow = julianDayNumber(jd); // Note day number is calculated from the julian
        while (dow == Saturday || dow == Sunday)   
        {
            --jd;
            --serial;
            dow = julianDayNumber(jd); 
        }
    }
    return setDate(serial);
}

bool
Date::setToCurrentDate( void )
{
	*this = Date::today();
	return true;
}


bool
Date::setToSystemDate( void )
{
	*this = Date::systemDate();
	return true;
}


bool 
Date::setDate( const Date& date )
{
         assert(date.valid());
	
        m_day    = date.m_day;
        m_month  = date.m_month;
        m_year   = date.m_year;
        m_serial = date.m_serial;
	
        return valid();
}


bool
Date::julian( const long jn )
// Copyright Issues:
// Julian number algorithms are taken from Numerical Recipes Software; see http://www.nr.com/julian.html   
// Copyright (C) 1998 by Numerical Recipes Software.  Permission is
// granted for unlimited copying and redistribution, provided that this
// copyright notice is displayed, together with the two following links

// http://www.nr.com/news.html

// http://www.nr.com/index.html
// 
// all dates from 1 Jan 4713BC to 31 Dec 9999AD.
// there is no year 0AD only 1BC followed by 1AD
// see https://en.wikipedia.org/wiki/Julian_day
{   
    m_day    = 0;
    m_month  = 0;
    m_year   = 0;
    m_serial = 0;
    
    if (jn >= 0) // this code does not work with negative Julian numbers
    {
        long a, b, c, d, e, alpha;
        
        if ( jn < 2299161L )
        {
            a = jn;
        }
        else
        {
            alpha = (long) ( (jn - 1867216.25) / 36524.25 );
            a = jn + 1 + alpha - (alpha / 4);
        }
        
        b = a + 1524;
        c = long( ( b - 122.1 ) / 365.25 );
        d = long( 365.25 * c );
        e = long( ( b - d ) / 30.6001 );
        
        int day   = int(b - d - (long) ( 30.6001 * e ));
        int month = int(( e < 13.5 ) ? (e - 1) : (e - 13));
        int year  = int(( month > 2.5 ) ? ( c - 4716L ) : (c - 4715L));
        
        if ( year <= 0 )
            --year;
        
        return setDate(day, month, year); 
    }
    
    return false; 
}


bool 
Date::gregorian( const long gn )
// Copyright Issues:
// Gregorian number algorithms are taken from Calendrical Calculations

// "Calendrical Calculations" by Nachum Dershowitz and Edward M. Reingold,
// Software---Practice & Experience, vol. 20, no. 9 (September, 1990),pp. 899--928.

// This code is in the public domain, but any use of it should acknowledge its source.
// See http://www.cs.sunysb.edu/~algorith/implement/reingold/distrib/calendar.C

// sets the Date to gregorian gn and determines the day, months, and year
// see https://en.wikipedia.org/wiki/Gregorian_calendar
{
    m_day    = 0;
    m_month  = 0;
    m_year   = 0;
    m_serial = 0;
    
    if (gn > 0) // this code does not work with negative Gregorian numbers
    {
	    long yearStart;
	    int dInYear;
	    int daysCounted;
            int day;
	    int month;
	    int year;
	    
	    daysCounted = 0;
	    month       = 0;
		
	    year = int(double(gn) / 365.2425) + 1;
		
	    yearStart = gregorian(1, 1, year);
	    
	    if (gn < yearStart)
            {
		    yearStart = gregorian(1, 1, --year);
	    }
		
            dInYear = int(gn - yearStart);                                      
		
	    if (Date::isLeapYear( year ))                      
            {
                // days must be between 0 and 365
                do
                {
                        daysCounted += m_leapDaysPerMonth[month];
                        ++month;
		 }
		 while ((daysCounted <= dInYear) && (month < 12));
		    
            daysCounted -= m_leapDaysPerMonth[month-1];
	    }
	    else
            { 
            // days must be between 0 and 364
                do
                {
                        daysCounted += m_daysPerMonth[month];
                        ++month;
		}
		while ((daysCounted <= dInYear) && (month < 12));
		    
                daysCounted -= m_daysPerMonth[month-1];
	    }
	    
        day = dInYear - daysCounted + 1;				
        
        return setDate(day, month, year);
    }
	
    return false;
}


std::string
Date::toString( const std::string& format ) const
// we copy delims from the format string
// "%02d" a 2 digit number; pad with 0

// we should rewrite this using strstream and read and write; safer and more c++
{
	const int START  = 0;
	const int DAY2   = 1;
	const int DAY3   = 2;
	const int MONTH2 = 3;
	const int MONTH3 = 4;
	const int YEAR2  = 5;
	const int YEAR3  = 6;
	const int YEAR4  = 7;
	const int DELIM  = 8;
	const int FAIL   = 9;
	
	int idx = 0;
	int state = START;
	
	char result[32];
	char *datestr = result;
	
    bool finished = false;
	while( !finished )
	{
		switch( state )
		{
			case START:
				if (format[idx] == 'D')
				{
					state = DAY2;
					++idx;
                                        break;
				}
				if (format[idx] == 'M')
				{
					state = MONTH2;
					++idx;
                                        break;
				}
				if (format[idx] == 'Y') 
				{
					state = YEAR2;
					++idx;
                                         break;
				}
				state = FAIL;
				break;
			case DAY2:
				if (format[idx] == 'D')
				{
					++idx;
					state = DAY3;
                                        break;
				}
				state = FAIL;
				break;
			case DAY3:
				if (format[idx] == 'D')
				{
                                         ++idx;
					std::snprintf( datestr, 32, "%3s",  dayName().c_str() );
                                        datestr += 3;
				}
				else
                                {
                                    std::snprintf( datestr, 32, "%02d", day() );
                                    datestr += 2;
                                }
				state = DELIM;
				break;
			case MONTH2:
				if (format[idx] == 'M')
				{
					++idx;
					state = MONTH3;
                                        break;
				}
				state = FAIL;
				break;
			case MONTH3:
				if (format[idx] == 'M')
				{
					++idx;
					std::snprintf( datestr, 32, "%3s",  monthName().c_str() );
                                        datestr += 3;
				}
				else
				{
					std::snprintf( datestr, 32, "%02d", month() );
					datestr += 2;
				}
				state = DELIM;
				break;
			case YEAR2:
				if (format[idx] == 'Y')
				{
					++idx;
					state = YEAR3;
                                        break;
				}
				state = FAIL;
				break;
			case YEAR3:
				if (format[idx] == 'Y')
				{
					++idx;
					state = YEAR4;
				}
				else
				{
                                        int y = year() - ((year() / 100) * 100);
					std::snprintf( datestr, 32, "%02d", y);
                                        datestr += 2;
					state = DELIM;
				}
				break;
			case YEAR4:
				if (format[idx] == 'Y')
				{
					++idx;
                                        int width = 4 + (m_year < 0);
                                        std::snprintf( datestr, 32, "%0*d", width, year() );
                                        datestr += width;
					state = DELIM;
                                        break;
				}
				state = FAIL;
				break;
			case DELIM:
			    if (format[idx] == '\0')
				{
                                    *datestr = '\0';
				    finished = true;
				}
				else
				{
					*datestr++ = format[idx++];
					state = START;
				}
				break;
			case FAIL:
				result[idx] = '\0';
				std::cout << "Date: Bad format " << format << std::endl;
				return result;
				break;
			default:
				result[idx] = '\0';
				std::cout << "Date: Format error " << format << std::endl;
				return result;
				break;
		}
	}
	return result;
}


void 
Date::clear( void )
{
    m_day    = 0;
    m_month  = 0;
    m_year   = 0;
    m_serial = 0;
}

//
// static Date members
//

void 
Date::setCalendar( const Calendar cal )
// static method sets static pointers to the appropriate calendar
{
	switch(cal)
        { 
		case STANDARD:
			m_pYearStart     = Date::m_yearStart;
			m_pLeapYear      = Date::m_leapYear;
			m_calAdjustment  = STANDARD;
			break;
		case EXCEL:
			m_pYearStart     = Date::m_xlYearStart;
			m_pLeapYear      = Date::m_xlLeapYear;
			m_calAdjustment  = EXCEL;
			break;
		default:
			m_pYearStart     = Date::m_yearStart;
			m_pLeapYear      = Date::m_leapYear;
			m_calAdjustment  = STANDARD;
			break;			
	}
	// update serial numbers
	setToday( Date( today().day(), today().month(), today().year() ) );
	m_maxDate.setDate(31,12,YEAR_MAX);
}


void
Date::setToday( void )
// static method; sets Date::today()
{
	std::ifstream fp("today.txt");
    
	if (fp)
	{
		fp >> m_today;
		fp.close();
	}
	else
	{
		m_today.setToSystemDate();
	}
}


Date
Date::systemDate( void )
// static method; returns current system date
// will not work if system date is before 1/1/1970 
{
	time_t ltime;
	struct tm ts;	
	
    time( &ltime );
    
	localtime_r( &ltime, &ts );	                         
	
	int y = ts.tm_year + TABLE_MIN;
	int d = ts.tm_mday;
	int m = ts.tm_mon + 1;
	
	return Date(d, m, y);
}


int 
Date::daysInMonth( const int month, const int year )
// static method
{		
    if (Date::isLeapYear(year)) 
		return m_leapDaysPerMonth[month - 1]; 
	return m_daysPerMonth[month - 1];
}


bool 
Date::isLeapYear( const int year )
// static method
{
    if ((year >= TABLE_MIN) && (year <= TABLE_MAX))
    {
		return (m_pLeapYear[year - TABLE_MIN]);
    }
	else
    {
        if (year % 400 == 0) 
            return true; 
        
        if ((year % 4 == 0) && (year % 100 != 0)) 
            return true; 
        
        return false; 
	}
}


int 
Date::dayDifference( const Date& start, const Date& end )
// static method
{
    return end.day() - start.day();
}


int 
Date::weekDayDifference( const Date& from, const Date& to )
// static method; the number of week days between two dates
{
    return to.weekDaySerial() - from.weekDaySerial();
}


int 
Date::monthDifference( const Date& start, const Date& end )
// static method
{
    return end.month() - start.month();
}


int 
Date::yearDifference( const Date& start, const Date& end )
// static method
{
    return end.year() - start.year();
}


int 
Date::yearAsSerial( const int year )
// static method; the serial of the 1st day of the year
// the gregororian day number - gregorian day number for (1,1,1900)
{
	if ((year >= TABLE_MIN) && (year <= TABLE_MAX))
	{
        return m_pYearStart[year - TABLE_MIN];
    }
    else if (year >= TABLE_MIN)
    {
        return int(gregorian(1, 1, year) - GREGOR_BASE + 1 + m_calAdjustment);
	}
    else if ((year >= YEAR_MIN) && (year <= YEAR_MAX))
    {  
        return int(gregorian(1, 1, year) - GREGOR_BASE) + 1;        
    }
    
	return 1;     
}


int
Date::lastDayOfMonth( int month, int year )
{
    // last day of the month in the Gregorian calendar

    switch (month)
    {
        case 2:
            return (isLeapYear(year)) ? 29 : 28;
            break;
        case 4:
        case 6:
        case 9:
        case 11: 
            return 30;
            break;
        default: 
            return 31;
            break;
    }
}


long
Date::gregorian( int day, int month, int year )
// caclulate the Gregorian number for this date
{
    assert(valid(day, month, year));
    
    // days in prior months this year
    for (int m = month - 1;  m > 0; --m) 
    {
        day = day + lastDayOfMonth( m, year );
    }
    
    --year;
    
    return (day + (365 * year) + (year / 4) - (year / 100) + (year / 400));   
}


long 
Date::julian( int day, int month, int year ) 
// calculate the Julian number for this date
{
    assert(valid(day, month, year));
    
    int a = 0;
    int b = 0;
    
	if (year < 0)
    {
        ++year;
    }
    
	if (month <= 2)
    {
        --year;
        month += 13;
    }
    else
    {
        ++month;
    }
    
	// deal with Gregorian calendar
    if (((year * 10000.0) + (month * 100.0) + day) >= 15821015.0)
    {
        a = (int) (year * 0.01);
        b = 2 - a + a / 4;
    }

	//return long(floor(365.25 * year)) + long(30.6001 * month) +  day + 1720995L + b;
    return long((365.25 * year)) - (year < 0) + long(30.6001 * month) +  day + 1720995L + b;
}






////


