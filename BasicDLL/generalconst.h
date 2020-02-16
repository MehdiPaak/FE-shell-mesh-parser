//=========================
// General Constants
//
// By: Mehdi Paak
//=========================

#pragma once

//--- general header ---
//#include "stdafx.h"
#include <exception>

#ifdef BASICDLL_EXPORTS
#define BASIC_LIBRARY_API __declspec(dllexport) 
#else
#define BASIC_LIBRARY_API __declspec(dllimport) 
#endif


//--- I/O Related Const ---
#define MAX_CHAR_ARRAY_SIZE 1024
#define OUTPUT_STREAM stdout

//--- General Errors ---
enum enmGeneralErrorCodes
{
  eErrorNothing = 0,
  eErrorInsufficientInput,
  eErrorIO,
  eErrorMemAlloc,
  eErrorUnknown,
  eNumErrorCodes,
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Felez exception class.
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class BASIC_LIBRARY_API clFelezException: public std::exception
{

  virtual enmGeneralErrorCodes GetExceptionType() const throw()
  {
    return eErrorUnknown;
  }

  virtual const char* what() const throw() {return "UNKNOWN_EXCEPTION\n";}
};

class BASIC_LIBRARY_API clIOError: public clFelezException
{

public:

  enum enmIOError
  {
    eErrorOpeningFile,
    eErrorReadingFile
  };

  // Constructor.
  clIOError(enmIOError i_enmIOErrorType, const char *i_pacFileName) throw();

  // Get general error exception.
  enmGeneralErrorCodes GetExceptionType() const throw() {return eErrorIO;}

  // Get file name.
  const char *GetFileName() const throw() {return m_acFileName;}

  // define what function.
  virtual const char* what()  const throw();

private:
  enmIOError m_enmIOErrorType;
  char m_acFileName[MAX_CHAR_ARRAY_SIZE];
};

