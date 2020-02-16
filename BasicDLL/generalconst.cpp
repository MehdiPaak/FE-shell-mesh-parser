//=========================
// General Constants
//
// By: Mehdi Paak
//=========================

#include "stdafx.h"

//#define BASICDLL_EXPORTS, in preprocessor definition
#include "generalconst.h"




clIOError::clIOError(enmIOError i_enmIOErrorType, const char *i_pacFileName) throw()
{
  //Check input.
  assert(i_enmIOErrorType > -1 && i_pacFileName && "Wrong input to clIOError constructor");

  m_enmIOErrorType = i_enmIOErrorType;
  memset(m_acFileName,'\0', sizeof(m_acFileName));
  strcpy(m_acFileName, i_pacFileName);
}


const char* clIOError::what()  const throw() 
{
  switch(m_enmIOErrorType)
  {
  case eErrorOpeningFile:
    return "Error opening file";

  case eErrorReadingFile:
    return "Error reading file";

  default:
    return "IO exception\n";
  }
}