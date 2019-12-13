//-----------------------------------------------------------------------------
//
//  The pdbcal package
//  Copyright (C) PHENIX collaboration, 1999
//
//  Root LinkDef file for class PHTimeStamp
//
//  Purpose: This class needs a hand-made linkdef file
//
//  Author: messer
//-----------------------------------------------------------------------------
#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class PHTimeStamp+;

#pragma link C++ function operator +  (const PHTimeStamp &, time_t);
#pragma link C++ function operator -  (const PHTimeStamp &, time_t);
#pragma link C++ function operator -  (const PHTimeStamp &, const PHTimeStamp &);
#pragma link C++ function operator << (ostream &, const PHTimeStamp &);
#pragma link C++ function operator >> (istream &, const PHTimeStamp &);

#endif /* __CINT__ */
