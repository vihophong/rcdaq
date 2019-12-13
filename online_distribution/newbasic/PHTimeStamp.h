//  The PHOOL's Software
//  Copyright (C) PHENIX collaboration, 1999
//
//  Purpose: PHENIX - wide time stamp class
//
//  Declaration of class PHTimeStamp
//
//  Author: Matthias Messer
//-----------------------------------------------------------------------------
#ifndef __PHTIMESTAMP_H__
#define __PHTIMESTAMP_H__
#include <event_io.h>

#include <ctime>
#include "TObject.h"


#ifndef __CINT__

#ifdef WINDOWS
#include <sys/types.h>
#include "stdafx.h"
#define WINDOWSEXPORT __declspec(dllexport)
#else  // ndef WINDOWS
#define WINDOWSEXPORT
#endif // WINDOWS


#ifdef WIN32
typedef ULONGLONG phtime_t;
#else
typedef unsigned long long phtime_t;
#endif // WIN32

#else  // ndef __CINT__
#define WINDOWSEXPORT
#endif // __CINT__

typedef unsigned long long phtime_t;

#ifndef __CINT__
class  WINDOWSEXPORT PHTimeStamp : public TObject{ 
#else
  class  PHTimeStamp : public TObject{ 
#endif

  public:
    enum { PHFarFuture = 2147483647 };
   
    PHTimeStamp();

    PHTimeStamp(int, int, int, int, int, int, int = 0);
    PHTimeStamp(time_t);
#ifndef __CINT__
    //   PHTimeStamp(phtime_t);
    void setBinTics(const phtime_t t);
   
#endif
    virtual ~PHTimeStamp() {}

  public: 
    void set(int, int, int, int, int, int, int = 0);

    void set(const char *);

    void setToSystemTime();
    void setToFarFuture() { setTics(PHFarFuture); }
   
#ifndef __CINT__
    phtime_t getBinaryTime() const { return binaryTime; }
#endif   
    time_t getTics() const;
    void   setTics(time_t);

    int isInRange(const PHTimeStamp &, const PHTimeStamp &);
    void print();
   
    int operator == (const PHTimeStamp &) const;
    int operator != (const PHTimeStamp &) const;
    int operator >  (const PHTimeStamp &) const;
    int operator >= (const PHTimeStamp &) const;
    int operator <  (const PHTimeStamp &) const;
    int operator <= (const PHTimeStamp &) const;

    PHTimeStamp & operator =  (const PHTimeStamp &);
    PHTimeStamp   operator += (time_t);
    PHTimeStamp   operator -= (time_t);


    char * formatTimeString() const;
    void print() const;
  
  private:
#ifndef __CINT__
    phtime_t ticsToBinaryTime(time_t) const;
    time_t   binaryTimeToTics(phtime_t) const;
#endif
  
  protected: 
    //#ifndef __CINT__
    phtime_t binaryTime;
    //#endif
    ClassDef(PHTimeStamp,1)
  }; 

  PHTimeStamp operator + (const PHTimeStamp &, time_t);
  PHTimeStamp operator - (const PHTimeStamp &, time_t);
  time_t      operator - (const PHTimeStamp &, const PHTimeStamp &);

  std::ostream &   operator << (std::ostream &, const PHTimeStamp &);
  std::istream &   operator >> (std::istream &, PHTimeStamp &);
   
#endif /* __PHTIMESTAMP_H__ */ 
