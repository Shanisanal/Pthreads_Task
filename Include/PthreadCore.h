//************************** Pthreads_Task  ***********************************
// Copyright (c) 2026 Trenser Technology Solutions
// All Rights Reserved
//*****************************************************************************
//
// File     : PthreadCore.h
// Summary  : Function declarations of wrapper functions in PthreadCore.c
// Note     : None
// Author   : Shani 
// Date     : Mar 16, 2026 
//
//*****************************************************************************
#ifndef PTHREADCORE_H
#define PTHREADCORE_H

//******************************* Include Files ******************************* 
#include <stdbool.h>
#include <stdio.h>

#include "Common.h"

//******************************* Global Types ******************************** 
 
//***************************** Global Constants ****************************** 

//***************************** Global Variables ****************************** 

//**************************** Forward Declarations *************************** 
bool POSSIXHandlerCreateThread(pthread_t* pulThread, void *(*pRoutine)(void*), 
                               void* pArguments, const char* pcThreadName);
bool POSSIXSynchronizeThreads(pthread_t ulThread, const char* pcThreadName);

//*********************** Inline Method Implementations *********************** 

#endif  /*PTHREADCORE_H */

// EOF 