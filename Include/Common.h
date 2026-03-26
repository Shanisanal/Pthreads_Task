//************************** Pthreads_Task  ***********************************
// Copyright (c) 2026 Trenser Technology Solutions
// All Rights Reserved
//*****************************************************************************
//
// File     : Common.h
// Summary  : Contains Centralized definitions, shared structures, and global 
//            declarations
// Note     : None
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
#include <unistd.h>

//******************************* Global Types ******************************** 
 
//***************************** Global Constants ****************************** 
#define USER_INPUT_THREAD_NAME      "INPUT_DATA_THREAD"
#define DATA_FORMAT_THREAD_NAME     "DATA_FORMAT_THREAD"
#define DATA_LOG_THREAD_NAME        "DATE_LOG_THREAD"

#define PRIORITY_NORMAL         0
#define SUCCESS_RETURN          0
#define MSG_QUEUE_ERR           -1

//***************************** Global Variables ****************************** 
 typedef struct _MESSAGE_
 { 
    uint16_t  unInputData; 
    float     fLogValue; 
    int32_t   lTimeStamp; 
} MESSAGE; 

typedef struct _SEM_CREATE_
{
    sem_t*      pSemId;
    uint32_t    ulInitialVal;
    const char* pcSemName;
} SEM_CREATE; 

typedef struct _QUEUE_CONFIG_
{
    const char* pcTargetThread;
    const char* pcQueueName;
    mqd_t* plQueueDescriptor; 
} QUEUE_CONFIG;

typedef struct _THREAD_CONFIG_
{
    pthread_t* pulThreadId;     
    void* (*ThreadHandler)(void*); 
    const char* pcThreadName;
} THREAD_CONFIG;

typedef enum 
{
    THREAD_SUCCESS      = 0,
    ERR_INPUT_THREAD    = (1 << 0), 
    ERR_FORMAT_THREAD   = (1 << 1), 
    ERR_LOG_THREAD      = (1 << 2)  
} THREAD_ERROR_MASK;

typedef enum 
{
    MSG_QUEUE_SUCCESS       = 0,
    ERR_INPUT_FORMAT_QUEUE  = (1 << 0), 
    ERR_FORMAT_LOG_QUEUE    = (1 << 1), 
} MESSAGE_QUEUE_ERR_MASK;

//**************************** Forward Declarations *************************** 

//*********************** Inline Method Implementations *********************** 

#endif  /*COMMON_H */

// EOF 