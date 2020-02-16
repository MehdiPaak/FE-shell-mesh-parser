/********************************************************************
Performance measurment utility functions

By: Mehdi Paak

********************************************************************/

#pragma once

#ifdef UTILITYDLL_EXPORTS
#define UTIL_LIBRARY_API __declspec(dllexport) 
#else
#define UTIL_LIBRARY_API __declspec(dllimport) 
#endif

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// TimeUtil class.
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class UTIL_LIBRARY_API TimeUtil
{

public:
  
  //---------------------------
  // Measure time from i_dTime.
  // RETURN:
  //  Time difference in seconds.
  //---------------------------
  static double TimeDiff(double i_dTime);

  static void PrintCurrentDateTime();

  static double CPUTimeDiff(const double &i_dCPUTimeStart);

  static double WallTimeDiff(const double &i_dWallTimeStart);

};