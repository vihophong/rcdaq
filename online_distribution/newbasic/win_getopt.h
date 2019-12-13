///////////////////////////////////////////////////////////////////////////////
//
//  FILE: getopt.h
//              
//      Header for the GetOption function
//
//  COMMENTS:
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
// function prototypes
#ifdef __cplusplus
extern "C"
{
#endif

int  __declspec(dllexport) GetOption (int argc, char** argv, char* pszValidOpts, char** ppszParam);
#ifdef __cplusplus
}
#endif
