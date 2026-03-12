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

//******************************* Global Types ******************************** 
 
//***************************** Global Constants ****************************** 

//***************************** Global Variables ****************************** 
pthread_t ulUserInputThread = 0;
pthread_t ulFormatInputThread = 0;
pthread_t ulLogThread = 0;

//**************************** Forward Declarations *************************** 

//*********************** Inline Method Implementations *********************** 

#endif

// EOF 