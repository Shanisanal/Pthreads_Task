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
bool PthreadCoreCreateThread(const THREAD_CONFIG* pstThreadConfig);
bool PthreadCoreSynchronizeThreads(const THREAD_CONFIG* pstThreadConfig);
mqd_t PthreadCoreOpenQueue(const char* pcQueueName, int lFlags, 
                            const uint32_t ulPermission, struct mq_attr* pstAttr);
bool PthreadCoreMessageSend(mqd_t lQueueDescriptor, const void* pMessage, 
                      uint32_t ulSize, uint32_t ulPriority);
bool PthreadCoreMessageReceive(mqd_t lQueueDescriptor,void* pMessage, 
                        uint32_t ulSize, uint32_t* pulPriority);
bool PthreadCoreSemInit(const SEM_CREATE* pstSem);
bool PthreadCoreSemPost(sem_t* pSemId);
bool PthreadCoreSemWait(sem_t* pSemId);

//*********************** Inline Method Implementations *********************** 

#endif  /*PTHREADCORE_H */

// EOF 