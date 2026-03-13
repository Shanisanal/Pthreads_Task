//************************** Pthreads_Task  ***********************************
// Copyright (c) 2026 Trenser Technology Solutions
// All Rights Reserved
//*****************************************************************************
//
// File     : Common.h
// Summary  : 
// Note     :  
// Author   : Shani 
// Date     : Mar 12, 2026 
//
//*****************************************************************************
#ifndef COMMON_H
#define COMMON_H

//******************************* Include Files ******************************* 
#include <pthread.h>
#include <mqueue.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#include "PossixHandler.h"

//******************************* Global Types ******************************** 
 
//***************************** Global Constants ****************************** 
#define USER_INPUT_THREAD_NAME      "INPUT_DATA_THREAD"
#define DATA_FORMAT_THREAD_NAME     "DATA_FORMAT_THREAD"
#define DATA_LOG_THREAD_NAME        "DATE_LOG_THREAD"

//***************************** Global Variables ****************************** 
extern pthread_t gulUserInputThread ;
extern pthread_t gulFormatInputThread ;
extern pthread_t gulLogThread ;

//**************************** Forward Declarations *************************** 

//*********************** Inline Method Implementations *********************** 

#endif

// EOF 