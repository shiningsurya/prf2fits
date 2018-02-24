#ifndef _THIRD_H
#include "third.hpp"
#endif

#include <cmath>
#include <cstdio>

using namespace std;

class ManageMJD {
		private:
				string date_time;
				double mjd, imjd, fmjd, fday;
				char datetime[24];
				long year, month, day, hours, minutes, seconds;
		public:
				ManageMJD(string dt){
						/*
						 *Given date time string 
						  Date time string is expected to be in UTC format. 
						 *sets up all those four variables
						 */
						date_time = dt;
				}
				ManageMJD(double im, double fm){
						/*
						 * Given integral and fractional part of MJD, it sets up 
						 * date time string in UTC format
						 */
						imjd = im;
						fmjd = fm/86400.0L;
						mjd = imjd + fmjd; // this ensures we get accurate mjd
						// starting to get ugly
					    double jd = mjd + 2400000.5L; // not jack daniels 
						/*
						 * Following 
						 * https://gist.github.com/jiffyclub/1294443
						 * http://mathforum.org/library/drmath/view/51907.html
						 */
						//jd += 0.5;
						double F,fi;
						long A,B,C,D,E;
						long I, alpha;
						F = modf(jd, &fi); // extracts integral and fractional part
						I = (long) fi;
						//
						alpha = trunc((I - 1867216.25L)/36524.25L);
						if(I > 2299160L) 
								A = I + 1L + alpha - trunc(alpha/4.L);
						else 
								A = I;
						B = A + 1524L;
						C = trunc((B - 122.1L)/365.25L);
						D = trunc(365.25L * C);
						E = trunc((B-D)/30.6001L);
						//
						double dday = round(B - D + F - trunc(30.6001L*E));
						day = (long)dday;		
						if(E < 13.5L)
								month = E - 1L;
						else
								month = E - 13L;
						if(month > 2.5L)
								year = C - 4716L;
						else
								year = C - 4715L;
						// Taken year month and day. 
						// Now take time 
						// Assuming Fractional part is less than one.
						// Using fmjd. NB: it is less than one. 
						double dhour, dmin, dsec;
						double hour, min, sec, micro;
						// Now, fmjd is the fractional part. 
						hour = fmjd * 24. ;
						dhour = modf(hour,&hour);
						//
						min = dhour * 60.;
						dmin = modf(min, &min);
						//
						sec = dmin * 60.;
						dsec = modf(sec, &sec);
						//
						hours = (long)hour;
						minutes	 = (long)min;
						seconds = (long)sec;
				}
				string getDateTime(){
						// Now write a string. 
						// https://www.w3.org/TR/NOTE-datetime 
						// Format for UTC time 
						// This length should be 24 characters long
						sprintf(datetime, "%04ld-%02ld-%02ldT%02ld:%02ld:%02.3ld",year,month,day,hours,minutes,seconds);
						string ret(datetime);
						return ret;
				}
};
