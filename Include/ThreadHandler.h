//************************** Pthreads_Task  ***********************************
// Copyright (c) 2026 Trenser Technology Solutions
// All Rights Reserved
//*****************************************************************************
//
// File     : ThreadHandler.h
// Summary  : 
// Note     : 
// Author   : Shani 
// Date     : Mar 12, 2026 
//
//*****************************************************************************
#ifndef THREADHANDLER_H
#define THREADHANDLER_H

//******************************* Include Files ******************************* 
#include "Common.h"
#include "PossixHandler.h"

//******************************* Global Types ******************************** 
 
//***************************** Global Constants ****************************** 

//***************************** Global Variables ****************************** 

//**************************** Forward Declarations *************************** 
bool ExecuteThreads(void);
bool CreateMessageQueue (void);
void PrintDisplayTable(void);
void* InputThreadHandler(void* pvarguments);
void* FormatThreadHandler(void* pvarguments);
void AppendDataToLog(MESSAGE* pstIncoming);
void PrintLogFileHeader(void);
void* LogThreadHandler(void* pArguments);

//*********************** Inline Method Implementations *********************** 

#endif  /*THREADHANDLER_H*/

// EOF 