/********************************************************************
Performance measurment utility functions

By: Mehdi Paak

********************************************************************/

//--- General header ---
#include "stdafx.h"
#include "generalconst.h"

//--- PerformanceUtil header ---
//#define UTILITYDLL_EXPORTS, in preprocessor definition
#include "PerformanceUtil.h"

//--- Time header ---
#include<ctime>
#include<sys/timeb.h>
#include <chrono> // C++11. 


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// TimeUtil class.
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------
// Measure Wall time from i_dTime.
// RETURN:
//  Time difference in seconds.
//---------------------------
double TimeUtil::TimeDiff(double i_dTime)
{

  timeb TimeNow;
  ftime(&TimeNow);
  double Dt = (double)(TimeNow.time) + (double)(TimeNow.millitm)/1000. - i_dTime;
  return Dt;
}

//---------------------------
// Prints Current date and time.
//---------------------------
void TimeUtil::PrintCurrentDateTime()
{
  time_t rawtime;
  struct tm * timeinfo;
  char buffer[120];

  time (&rawtime);
  timeinfo = localtime(&rawtime);

  strftime(buffer,sizeof(buffer),"Date: %d-%m-%Y  Time: %I:%M:%S",timeinfo);

  fprintf(
    OUTPUT_STREAM,
    "--------------------------------------\n"
    "%s\n"
    "--------------------------------------\n",
    buffer);
}

//---------------------------
// Return CPU time in seconds.
// 
// Description:
//  From chrono library in C++ 11, measures CPU time.
//  Caution: might not measure system calls CPU time.
//---------------------------

double TimeUtil::CPUTimeDiff(const double &i_dCPUTimeStart)
{
  std::clock_t CPUTimeNow = std::clock();
  double dCPUTimeDiff = (CPUTimeNow) / (double)CLOCKS_PER_SEC - i_dCPUTimeStart;

  return dCPUTimeDiff;
}

//---------------------------
// return Wall time in seconds.
// 
// Description:
//  From chrono library in C++ 11, measures Wall time.
//---------------------------
/*auto wcts = std::chrono::system_clock::now();
do_some_fancy_stuff();
std::chrono::duration<double> wctduration = (std::chrono::system_clock::now() - wcts);
std::cout << "Finished in " << wctduration.count() << " seconds [Wall Clock]" << std::endl; */
