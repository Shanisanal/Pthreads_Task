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
#define INPUT_SEM_NAME      "/SemInputToFormat"
#define FORMAT_SEM_NAME     "/SemFormatToLlog"

//***************************** Global Variables ****************************** 
extern pthread_t gulUserInputThread ;
extern pthread_t gulFormatInputThread ;
extern pthread_t gulLogThread ;
extern mqd_t glInputToFormatQueue ; 
extern mqd_t glFormatToLogQueue ;
extern sem_t gSemInputQueueReady;
extern sem_t gSemFormatQueueReady;

//**************************** Forward Declarations *************************** 
bool PosixHandlerCreateThreads (void);
bool PosixHandlerSynchronizeThreads(void);
bool PosixHandlerCreatQueue (const char* pcThreadName);
bool PosixHandlerInitSemaphores(void);
bool PosixHandlerPostSemaphore(const char* pcSemName);
bool PosixHandlerWaitSemaphore(const char* pcSemName);

//*********************** Inline Method Implementations *********************** 

#endif

// EOF 