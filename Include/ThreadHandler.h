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
#include "PosixHandler.h"

//******************************* Global Types ******************************** 
 
//***************************** Global Constants ****************************** 

//***************************** Global Variables ****************************** 

//**************************** Forward Declarations *************************** 
bool ThreadHandlerRun(void);
void* ThreadHandlerUserInput(void* pvarguments);
void* ThreadHandlerFormatInput(void* pvarguments);
void* ThreadHandlerLogData(void* pArguments);

//*********************** Inline Method Implementations *********************** 

#endif  /*THREADHANDLER_H*/

// EOF 