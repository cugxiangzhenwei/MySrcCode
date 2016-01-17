#ifndef FIND_PRO_H
#define FIND_PRO_H

// Created: 12/29/2000  (RK)
// Last modified: 6/16/2003  (RK)
// Please report any problems or bugs to kochhar@physiology.wisc.edu
// The latest version of this routine can be found at:
//     http://www.neurophys.wisc.edu/ravi/software/killproc/
// Check whether the process "szToFind" is currently running in memory
// This works for Win/95/98/ME and also Win/NT/2000/XP
// The process name is case-insensitive, i.e. "notepad.exe" and "NOTEPAD.EXE"
// will both work (for szToFind)
// Return codes are as follows:
//   0   = Process was not found
//   1   = Process was found
//   605 = Unable to search for process
//   606 = Unable to identify system type
//   607 = Unsupported OS
//   632 = Process name is invalid
// Change history:
//  3/10/2002   - Fixed memory leak in some cases (hSnapShot and
//                and hSnapShotm were not being closed sometimes)
//  6/13/2003   - Removed iFound (was not being used, as pointed out
//                by John Emmas)
/************************************************************************/
/* 用法，在使用此函数之前做如下声明
* extern int FindProcByName(const char *szToFind);
* 使用如下语句隐式链接lib #pragma comment(lib,"F:/study/20130704/FindProc/Release/FindProcDLL.lib")
* 或在lib路径中指定FindProcDLL.lib的路径，即可调用该函数
* 如下irev = FindProcByName("RsTools.exe");
/************************************************************************/
extern "C" __declspec(dllexport)  int FindProcByName(const char *szToFind);

#endif //FIND_PRO_H