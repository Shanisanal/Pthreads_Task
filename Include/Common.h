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
#include <math.h>

#include "PossixHandler.h"
#include "ThreadHandler.h"

//******************************* Global Types ******************************** 
 
//***************************** Global Constants ****************************** 
#define USER_INPUT_THREAD_NAME      "INPUT_DATA_THREAD"
#define DATA_FORMAT_THREAD_NAME     "DATA_FORMAT_THREAD"
#define DATA_LOG_THREAD_NAME        "DATE_LOG_THREAD"

#define PRIORITY_NORMAL         0
#define PRIORITY_URGENT         10
#define PRIORITY_EMERGENCY      99

//***************************** Global Variables ****************************** 
extern pthread_t gulUserInputThread ;
extern pthread_t gulFormatInputThread ;
extern pthread_t gulLogThread ;

 typedef struct _MESSAGE_
 { 
    uint16_t  unInputData; 
    float     fLogValue; 
    int32_t   lTimeStamp; 
} MESSAGE; 

typedef enum 
{
    THREAD_SUCCESS      = 0,
    ERR_INPUT_THREAD    = (1 << 0), 
    ERR_FORMAT_THREAD   = (1 << 1), 
    ERR_LOG_THREAD      = (1 << 2)  
} THREAD_ERROR_MASK;



//**************************** Forward Declarations *************************** 

//*********************** Inline Method Implementations *********************** 

#endif  /*COMMON_H */

// EOF 