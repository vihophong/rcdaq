//-----------------------------------------------------------------------------
//  $Header: /afs/rhic.bnl.gov/phenix/PHENIX_CVS/online_distribution/newbasic/PHBigTimeStamp.h,v 1.3 2001/07/06 19:19:39 purschke Exp $
//
//  The PHOOL's Software
//  Copyright (C) PHENIX collaboration, 1999
//
//  Declaration of class PHBigTimeStamp
//
//  Author: Matthias Messer
//-----------------------------------------------------------------------------
#ifndef PHBIGTIMESTAMP_H
#define PHBIGTIMESTAMP_H

#include "PHTimeStamp.h"

#ifndef __CINT__
class WINDOWSEXPORT PHBigTimeStamp : public PHTimeStamp { 
#else
class  PHBigTimeStamp : public PHTimeStamp { 
#endif

public: 
   PHBigTimeStamp(); 
   PHBigTimeStamp(int, int, int, int, int, int, int, int, int); 
   PHBigTimeStamp(PHTimeStamp &, int, int); 
   ~PHBigTimeStamp(); 

public: 
   int getRunNumber()   const { return runNumber; }
   int getEventNumber() const { return eventNumber; }

   void setRunNumber(int val)   { runNumber = val; }
   void setEventNumber(int val) { eventNumber = val; }

   int isInEventRange(PHBigTimeStamp &, PHBigTimeStamp &);
   
protected: 
   int runNumber;
   int eventNumber;
}; 

OSTREAM & operator << ( OSTREAM &, PHBigTimeStamp &);

#endif /* PHBIGTIMESTAMP_H */ 
