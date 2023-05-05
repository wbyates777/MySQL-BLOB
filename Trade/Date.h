/* A date class 04/03/19

		$$$$$$$$$$$$$$$$$$$$$$$
		$   Date.h - header   $
		$$$$$$$$$$$$$$$$$$$$$$$

   by W.B. Yates
   Copyright (c) 2019 W.B. Yates. All rights reserved.
   History:

   A Gregorian Date class that provides dates from 01/01/-4713 to 31/12/9999. 

   see http://en.wikipedia.org/wiki/Gregorian_calendar
   
   Operations on dates between 01/01/1900 and to 31/12/2149 use lookup tables and are most efficient.
   
   The internal serial number 1 represents the (Gregorian) date 01/01/1900. For each date the serial is gregorian(date) - gregorian(1,1,1900) + 1
   This class can deal with the Microsoft PC EXCEL leap year error, that is, that the year 1900 is supposed (incorrectly) to be a leap year. 
   For example the internal serial 91311 is 31-Dec-2149 in the (correct) STANDARD calendar, and the internal serial 91312 is 31-Dec-2149 in the EXCEL calendar.   
 
   Extending the Date lookup tables range is straight forward (see Date.cpp for details).      

   Some simple date formats are supported. Format string can employ DD, MM, MMM, YY, or YYYY and a single char delimiter i.e '-', or '/' 
   The format DD/MMM/YY reads 09/Jul/68 or 9/JUL/68 and writes 09/Jul/68
   YYYY-DD-MM reads 1968-09-7 and writes 1968-09-07
   delimiters must match i.e. DD/MMM/YY is not the same as DD-MMM-YY; the exception is when reading using the ISO format. Here delimiters are ignored
 
   Support for positive Julian numbers is provided.
 
 
   Copyright Issues:
 
   (i) Julian number algorithms are taken from Numerical Recipes Software; see http://www.nr.com/julian.html   
 
     Copyright (C) 1998 by Numerical Recipes Software.  Permission is
     granted for unlimited copying and redistribution, provided that this
     copyright notice is displayed, together with the two following links
 
     http://www.nr.com/news.html
 
     http://www.nr.com/index.html
 
  (ii) Gregorian number algorithms are taken from Calendrical Calculations
 
    "Calendrical Calculations" by Nachum Dershowitz and Edward M. Reingold,
    Software---Practice & Experience, vol. 20, no. 9 (September, 1990),pp. 899--928.
 
    This code is in the public domain, but any use of it should acknowledge its source.
    See http://www.cs.sunysb.edu/~algorith/implement/reingold/distrib/calendar.C
 
 
 Example
 
 Date::setCalendar( Date::EXCEL );
 Date st(31,12,2149);	 
 cout << st.toString("DDD-DD-MM-YYYY") << endl;
 cout << st.serial() << endl;
 exit(1);
 
 or
 
 Date st(23,8,2029), st2, st3(1,1,1900);
 st2.julian(st.julian());
 st.setDate(st2.serial());
 cout << st2 << endl;
 cout << st  << endl;
 
 cout << 1 + st.julian() - st3.julian()  << endl;
 cout << st2.serial() << endl;
 exit(1);
 
*/ 
 

#ifndef __DATE_H__
#define __DATE_H__

#include <iostream>
#include <string>


class Date 
{
public:

    // default to STANDARD; Note the value is used in dayNumber calculations, do not alter
    enum Calendar { STANDARD = 0, EXCEL = 1  }; 

    
	enum Day { Sun = 1, Sunday = 1, Mon = 2, Monday = 2, Tue = 3, Tuesday = 3,
		Wed = 4, Wednesday = 4, Thu = 5, Thursday = 5, Fri = 6, Friday = 6, Sat = 7, Saturday = 7 };
	
	enum Month { Jan = 1, January = 1, Feb = 2, February = 2, Mar = 3, March = 3, Apr = 4, April = 4, 
		May = 5, Jun = 6, June = 6, Jul = 7, July = 7, Aug = 8, August = 8, Sep = 9, September = 9, 
		Oct = 10, October = 10, Nov = 11, November = 11, Dec = 12, December = 12 };	


    Date(void): m_day(0), m_month(0), m_year(0), m_serial(0) {}	                      

	Date(const int d, const int m, const int y): m_day(0), m_month(0), m_year(0), m_serial(0)
	{
		setDate(d, m, y);
	}
    
	// format string can employ DD, MM, MMM, YY, or YYYY and single char delimiter i.e '-', '/' etc
	// DD/MMM/YY reads 9/Jul/68  or 9/JUL/68
	// YYYY-DD-MM reads 1968-09-7
	// delimiters must match i.e. DD/MMM/YY is not the same as DD-MMM-YY	
	Date(const std::string& dateStr, const std::string& format): m_day(0), m_month(0), m_year(0), m_serial(0)
	{
		setDate(dateStr, format);
	}
	
	explicit Date( const std::string& dateStr ): m_day(0), m_month(0), m_year(0), m_serial(0)
	{
		setDate(dateStr);
	}
	
	explicit Date( const int serial ):  m_day(0), m_month(0), m_year(0), m_serial(0)
	{
		setDate(serial);
	}
	
	~Date(void) { m_day = 0; m_month = 0; m_year = 0; m_serial = 0; }	

    // set date functions return true on success
	bool 		        
	setDate( const Date& d );
	
	bool       
	setDate( const int day, const int month, const int year );
    
    // get/set methods for serial numbers, 1/1/1900 = 1, 2/1/1900 = 2, ... ,
	bool        
	setDate( const int serial );
    
	// format string can employ DD, MM, MMM, YY, or YYYY and single char delimiter i.e '-', '/' etc
	// DD/MMM/YY reads 9/Jul/68 
	// YYYY-DD-MM reads 1968-09-7
	// delimiters must match i.e. DD/MMM/YY is not the same as DD-MMM-YY
	// this method is slow
	bool
	setDate( const std::string& dateStr, const std::string& format );
    
	bool
	setDate( const std::string& dateStr )
	// assumes ISO convention, that is YYYY-DD-MM
    // will match on any char delimiter i.e. YYYY/DD/MM or YYYY-DD-MM
	{
		int d = 0, m = 0, y = 0;
            	std::sscanf(dateStr.c_str(), "%d%*c%d%*c%d", &y, &m, &d);
		return setDate(d, m, y);
	}
    
    	void        
	clear( void );
		
	int // i.e. 1 to 31                
	day( void ) const { return m_day; }                 
	
	int // i.e. 1 to 12                 
	month( void ) const { return m_month; }              
	
	int // i.e. -4713 to 9999                
	year( void ) const { return m_year; }                

    int  // i.e. Sun = 1 to Sat = 7 
    dayNumber( void ) const; 

    std::string 
    dayName( void ) const;
	
	std::string               
	monthName( void ) const;  

	// Date arithmetic; d is measured in CALENDAR days
	const Date  operator+( const int d ) const;
	const Date  operator-( const int d ) const;
	int         operator-( const Date& d ) const;
	Date&	    operator+=( const int d );
	Date&	    operator-=( const int d );
	Date&       operator++( void );        // prefix
	Date&       operator--( void );        // prefix
	Date        operator++( const int d ); // postfix
	Date        operator--( const int d ); // postfix
	
	bool operator<( const Date& d ) const { return (m_serial < d.m_serial); }
	bool operator>( const Date& d ) const { return (m_serial > d.m_serial); }
	bool operator<=( const Date& d ) const { return (m_serial <= d.m_serial); }
	bool operator>=( const Date& d ) const { return (m_serial >= d.m_serial); }
	bool operator==( const Date& d ) const { return (m_serial == d.m_serial); }
	bool operator!=( const Date& d ) const { return (m_serial != d.m_serial); }
	
	
	void                
	addDays( const int days ) { *this += days; }	
	
	bool                
	addWeekDays( const int days );
	
	bool                
	addMonths( const int months );
	
	bool                
	addYears( const int years );                                        
	
	int                 
	daysInMonth( void ) const;                                          
	
	bool                
	isLeapYear( void ) const;                                                 
	
	bool                
	isWeekend( void ) const;                                                  
	
	bool                
	isMonthEnd( void ) const;                                           
	
	int                 
	lastDayOfMonth( void ) const;
	
	bool                
	next( const Day d ); // roll forward to next i.e. d = Day::Monday
	
	bool                
	prev( const Day d );
	
	bool                
	nextWeekDay( void );                                                   

	bool                
	prevWeekDay( void );

	
	// set this date to the system date
	bool                 
	setToSystemDate( void );
	
	// set this date to Date::today()
	bool                
	setToCurrentDate( void );

	 
	int                
	serial( void ) const { return m_serial; } 
              	
	// get/set methods for Julian and Gregorian numbers
	long               
	julian( void ) const { return julian(m_day, m_month, m_year); } 
	
	bool  // we only handle positive Julian numbers             
	julian( const long j );

    long              
	gregorian( void ) const { return gregorian(m_day, m_month, m_year); } 
    
    bool  // we only handle positive Gregorian numbers 
    gregorian( const long gn );
    
	// format string can employ DD, DDD, MM, MMM, YY, YYYY and single char delimiter i.e '-', '/' etc
	// the delimiter is copied from the format string i.e. DD/MMM/YY writes 09/Jul/68 
	// and YYYY-DDD-MM writes 1968-Tue-07
	// this method is slow (three calls to printf rather than one)
	std::string              
	toString( const std::string& format ) const;
	
	std::string // use format = "YYYY-MM-DD"              
	toString( void ) const
	{
		char buff[16];
        	std::snprintf( buff, 16, "%0*d-%02d-%02d", 4 + (m_year < 0), year(), month(), day() );
		return buff;
	}
	
	//
    // static Date members
    //
	// EXCEL or STANDARD serial numbers; Julian unaffected
	// Not thread safe. Do not change Calendar mid-computation as old calendar dates will become incorrect
    //
	static void                
	setCalendar( const Calendar cal ); 
		
	// max date, today and system today methods
	static const Date&
	maxDate( void ) { return m_maxDate; }; // this implementation uses Date(31,12,9999)
	
	static const Date&
	today( void ) { return m_today; }; 
	
	static void // set Date::today() to system date
	setToday( void ); // not thread safe. 
	
	static void // not thread safe.
	setToday( const Date& d ) { m_today = d; }
	
	static Date  // get the current system date              
	systemDate( void );

	// misc methods 
	static int   
	daysInMonth( const int month, const int year );                   
	
	static bool         
	isLeapYear( const int year ); 
	
	static int       
	dayDifference( const Date& start, const Date& end );
	
	static int          
	weekDayDifference( const Date& start, const Date& end );                               

	static int       
	monthDifference( const Date& start, const Date& end );
	
	static int       
	yearDifference( const Date& start, const Date& end );
	
	static int	// look up value from table or calculate otherwise
	yearAsSerial( const int year );
	
    static int
    lastDayOfMonth( int month, int year );
    
    static long // (1, Jan, 0001) where 0001 is 1 AD is gregorian = 1
   	gregorian( int day, int month, int year );
    
    static long // (2, Jan, -4713) is julian = 1              
	julian( int day, int month, int year );
    
	// date validation
                                    
	static bool 
   	valid( const int day, const int month, const int year )
    {
        return ((day >= 1) && (day <= Date::daysInMonth(month, year)) && (month >= 1) && (month <= 12) && (year >= -4713) && (year <= 9999));
	}
    
	bool                
	valid( void ) const { return (m_day != 0); }  
    
	friend std::ostream& 
	operator<<( std::ostream& strm, const Date& date );
	
    friend std::istream& 
	operator>>( std::istream& strm, Date& date );

	//static void
    //printTables( void );
    
private:
	          
	int 
	weekDaySerial( void ) const 
	{
		int s = rollSerial(m_serial - 3);		
		return  int(s / 7) * 5 + std::min(int(s % 7), 4);
	}
    
    // the US Julian day number 
    // returns Sun = 1 to Sat = 7 
    // (see https://en.wikipedia.org/wiki/Julian_day#Finding_day_of_week_given_Julian_day_number)
    int  
    julianDayNumber( const long jd ) const { return int((1 + jd) % 7) + 1; } 

    int  // i.e. Sun = 1 to Sat = 7 
   	gregorianDayNumber( const int gd ) const  { return ((gd - m_calAdjustment) % 7) + 1; }
    
    int
    rollSerial( const int serial ) const;
    
		
	int   m_serial;
	char  m_day;
	char  m_month;
	short m_year;

	static Date              m_today;
	static Date              m_maxDate;
	static const char* const m_dayNames[];
	static const char* const m_monthNames[];
	static const int*        m_pYearStart;
	static const char*       m_pLeapYear;	
	static const int         m_yearStart[];
	static const int         m_xlYearStart[];
	static const char        m_leapYear[];
	static const char        m_xlLeapYear[];
	static const char        m_daysPerMonth[];
	static const char        m_leapDaysPerMonth[];
	static Calendar          m_calAdjustment;
};

inline std::ostream& 
operator<<( std::ostream& ostr, const Date& date )
{
	return ostr << date.m_year << '-' << int(date.m_month) << '-' << int(date.m_day) << ' ';
}

std::istream& 
operator>>( std::istream& istr, Date& date );




#endif // __DATE_H__
