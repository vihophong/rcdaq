//-----------------------------------------------------------------------------
//  $Header: /afs/rhic.bnl.gov/phenix/PHENIX_CVS/online_distribution/newbasic/PHBigTimeStamp.C,v 1.3 2003/09/10 20:37:01 purschke Exp $
//
//  The PHOOL's Software
//  Copyright (C) PHENIX collaboration, 1999
//
//  Implementation of class PHBigTimeStamp
//
//  Author: Matthias Messer
//-----------------------------------------------------------------------------
#include "PHBigTimeStamp.h" 

PHBigTimeStamp::PHBigTimeStamp() 
{
   runNumber   = 0;
   eventNumber = 0;
}

PHBigTimeStamp::PHBigTimeStamp(int year, int month, int day, int hour, int minute, int second, int fraction, int run, int event)
   : PHTimeStamp(year, month, day, hour, minute, second, fraction)
{
   runNumber   = run;
   eventNumber = event;
}

PHBigTimeStamp::PHBigTimeStamp(PHTimeStamp & timeStamp, int run, int event)
   : PHTimeStamp(timeStamp)
{
   runNumber   = run;
   eventNumber = event;   
}

PHBigTimeStamp::~PHBigTimeStamp() 
{
}

int PHBigTimeStamp::isInEventRange(PHBigTimeStamp & t1, PHBigTimeStamp & t2)
{
   if (runNumber <  t1.getRunNumber() || runNumber   > t2.getRunNumber()  ) return (1==0);
   if (runNumber == t1.getRunNumber() && eventNumber < t1.getEventNumber()) return (1==0);
   if (runNumber == t2.getRunNumber() && eventNumber > t2.getEventNumber()) return (1==0);

   return (1==1);
}

//
// Non member functions and operators
//

OSTREAM & operator << ( OSTREAM & s, PHBigTimeStamp & t)
{
   return s << PHTimeStamp(t) << ", run: " << t.getRunNumber() << ", event: " << t.getEventNumber();
}

