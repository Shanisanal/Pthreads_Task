//************************** Pthreads_Task  ***********************************
// Copyright (c) 2026 Trenser Technology Solutions
// All Rights Reserved
//*****************************************************************************
//
// File     : PossixHandler.h
// Summary  : Contains function declaration from PossixHandler.C
// Note     : None
// Author   : Shani 
// Date     : Mar 12, 2026 
//
//*****************************************************************************
#ifndef POSSIXHANDLER_H
#define POSSIXHANDLER_H

//******************************* Include Files ******************************* 
#include <stdbool.h>
#include <stdio.h>

#include "Common.h"
#include "ThreadHandler.h"
#include "PthreadCore.h"

//******************************* Global Types ******************************** 
 
//***************************** Global Constants ****************************** 

//***************************** Global Variables ****************************** 

//**************************** Forward Declarations *************************** 
int CreateThreads (void);
void SynchronizeThreads(void);

//*********************** Inline Method Implementations *********************** 

#endif

// EOF 