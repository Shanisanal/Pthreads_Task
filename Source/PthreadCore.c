//**************************** Pthreads_Task **********************************
//  Copyright (c) 2026 Trenser Technology Solutions
//  All Rights Reserved 
//*****************************************************************************
//
// File      : PthreadCore.c
// Summary   : A centralized utility file for wrapping standard pthread calls
// Note      : None
// Author    : Shani
// Date      : Mar 16, 2026
//
//***************************************************************************** 

//******************************* Include Files *******************************
#include "PthreadCore.h"

//******************************* Local Types ********************************* 
 
//***************************** Local Constants ******************************* 

//***************************** Local Variables ******************************* 

//****************************** Local Functions ****************************** 

//************************* PthreadCoreCreateThread.***************************
//Purpose   : Create POSSIX thread
//Inputs    : pstThreadConfig - pointer to the thread config structure
//Outputs   : None
//Return    : true - Thread created, false - thread creation failed
//Notes     : None
//*****************************************************************************
bool PthreadCoreCreateThread(const THREAD_CONFIG* pstThreadConfig)
{
    bool blReturn = true;

    do 
    {
        if (NULL == pstThreadConfig)
        {
            blReturn = false;
            printf("ERROR: Thread creation failed - Null pointer.\r\n");
            break; 
        }

        if (pthread_create(pstThreadConfig->pulThreadId, NULL, 
                        pstThreadConfig->ThreadHandler, NULL) != SUCCESS_RETURN)
        {
            printf("ERROR: %s Creation failed\r\n", pstThreadConfig->pcThreadName);
            blReturn = false;
            break;
        }
        else
        {
            printf("%s Created Successfully\r\n", pstThreadConfig->pcThreadName);
        }

    } while (0);

    return blReturn;
}

//************************* PthreadCoreSynchronizeThreads **********************
//Purpose    : Synchronizes a POSIX thread
//Inputs     : pulThread - pointer to the thread ID variable
//             pcThreadName - name of the thread for logging purposes
//Outputs    : None
//Return     : true if thread synchronized successfully, false if 
//             synchronization failed
//Notes      : None
//*****************************************************************************
bool PthreadCoreSynchronizeThreads(const THREAD_CONFIG* pstThreadConfig)
{
    bool blReturn = true;

    do 
    {
        if (NULL == pstThreadConfig)
        {
            printf("ERROR: Synchronization failed - Null configuration pointer.\r\n");
            blReturn = false;
            break; 
        }

        if(pthread_join(*(pstThreadConfig->pulThreadId), NULL) != SUCCESS_RETURN)
        {
            printf("ERROR: %s Synchronization (Join) failed with status \r\n",
                    pstThreadConfig->pcThreadName);
            blReturn = false;
            break;
        }

    } while (0);
    
    return blReturn;
}

//************************* PthreadCoreOpenQueue ******************************
//Purpose   : Opens or creates a POSIX message queue
//Inputs    : pcQueueName  - The string name of the queue
//            lFlags       - Access modes
//            ulPermission - Number representing octal permissions 
//            pstAttr      - Pointer to mq_attr structure for queue configuration
//Outputs   : None
//Return    : Valid queue descriptor on success, or MSG_QUEUE_ERR on failure
//Notes     : None.
//*****************************************************************************
mqd_t PthreadCoreOpenQueue(const char* pcQueueName, int lFlags, 
                            const uint32_t ulPermission, struct mq_attr* pstAttr)
{
    mqd_t lQueueDescriptor = MSG_QUEUE_ERR;

    do 
    {
        if (NULL == pcQueueName || NULL == pstAttr)
        {
            printf("ERROR: mq_open failed - NULL Arguments.\r\n");
            break; 
        }

        lQueueDescriptor = mq_open(pcQueueName, lFlags, ulPermission, pstAttr);

        if (MSG_QUEUE_ERR == lQueueDescriptor)
        {
            printf("ERROR: mq_open failed for queue: %s\r\n", pcQueueName);
        }

    } while (0);

    return lQueueDescriptor;
}

//***************************** PthreadCoreMessageSend *************************
//Purpose   : Sends a message through a POSIX queue .
//Inputs    : lQueueDescriptor - The handle for the target message queue
//            pMessage         - Pointer to the message data
//            ulMsgSize        - Size of the message being sent
//            ulPriority       - The priority level of the message
//Outputs   : None
//Return    : true if message was sent successfully, false if message sent failed
//Notes     : None
//******************************************************************************
bool PthreadCoreMessageSend(mqd_t lQueueDescriptor, const void* pMessage, 
                      uint32_t ulSize, uint32_t ulPriority)
{
    bool blStatus = true;

    do 
    {
        if (lQueueDescriptor == MSG_QUEUE_ERR || pMessage == NULL)
        {
            printf("ERROR: Invalid Queue Descriptor or NULL Message Pointer.\n\r");
            blStatus = false;
            break; 
        }

        if (MSG_QUEUE_ERR == mq_send(lQueueDescriptor, (const char*)pMessage, 
                                     ulSize, ulPriority))
        {
            printf("ERROR: Failed to send message to queue descriptor %d.\n\r", 
                    lQueueDescriptor);
            blStatus = false;
            break;
        }

    } while (0);

    return blStatus;
}

//*********************** PthreadCoreMessageReceive ****************************
//Purpose   : Receives a message from a POSIX message queue.
//Inputs    : lQueueDescriptor - The handle of the target queue.
//            pMessage         - Pointer to the buffer where data will be stored.
//            ulSize           - Size of the message buffer in bytes.
//            pulPriority      - Pointer to store the received message priority.
//Outputs   : None
//Return    : true if a message was received , false if message receive failed
//Notes     : None
//******************************************************************************
bool PthreadCoreMessageReceive(mqd_t lQueueDescriptor, void* pMessage, 
                               uint32_t ulSize, uint32_t* pulPriority)
{
    bool blReturn = true;

    do 
    {
        if (NULL == pMessage || NULL == pulPriority)
        {
            printf("ERROR: Message queue receive failed - NULL pointer detected.\n\r");
            blReturn = false;
            break; 
        }

        if(MSG_QUEUE_ERR == mq_receive(lQueueDescriptor, (char*)pMessage, ulSize, 
                                                                    pulPriority))
        {
            printf("ERROR: mq_receive failed for descriptor %d.\n\r", lQueueDescriptor);
            blReturn = false;
            break;
        }

    } while (0);

    return blReturn;
}

//*************************** PthreadCoreSemInit *******************************
//Purpose   : Initializes a POSIX semaphore 
//Inputs    : pstSem - Pointer to a SEM_CREATE structure 
//Outputs   : None
//Return    : true if the semaphore was successfully initialized , false if 
//            semaphore initialisation fails.
//Notes     : None
//******************************************************************************
bool PthreadCoreSemInit(const SEM_CREATE* pstSem)
{
    bool blReturn = true;

    do 
    {
        if (NULL == pstSem)
        {
            printf("ERROR: Semaphore initialization failed - Null pointer.\n");
            blReturn = false;
            break; 
        }

        if (sem_init(pstSem->pSemId, 0, pstSem->ulInitialVal) != THREAD_SUCCESS)
        {
            printf("ERROR: Failed to initialize semaphore: %s\n", pstSem->pcSemName);
            blReturn = false;
            break;
        }

    } while (0);

    return blReturn;
}

//*************************** PthreadCoreSemPost *******************************
//Purpose   : Posts a POSIX semaphore.
//Inputs    : pSemId - Pointer to the semaphore object to be posted.
//Outputs   : None
//Return    : true if the semaphore was successfully posted; 
//            false if semaphore post fails.
//Notes     : None.
//******************************************************************************
bool PthreadCoreSemPost(sem_t* pSemId)
{
    bool blReturn = true;

    do 
    {
        if (NULL == pSemId)
        {
            printf("ERROR: Semaphore post failed - Null pointer.\n");
            blReturn = false;
            break; 
        }

        if (sem_post(pSemId) != THREAD_SUCCESS)
        { 
            printf("ERROR: Semaphore post failed (Descriptor: %p)\n", (void*)pSemId);
            blReturn = false;
            break;
        }

    } while (0);

    return blReturn;
}

//*************************** PthreadCoreSemWait *******************************
//Purpose   : Waits a POSIX semaphore.
//Inputs    : pSemId - Pointer to the semaphore object.
//Outputs   : None
//Return    : true if the semaphore was successfully locked; 
//            false if the pointer is NULL or sem_wait fails.
//Notes     : None
//******************************************************************************
bool PthreadCoreSemWait(sem_t* pSemId)
{
    bool blReturn = true;

    do
    {
        if (NULL == pSemId)
        {
            printf("ERROR: Semaphore wait failed - Null pointer.\n");
            blReturn = false;
            break; 
        }

        if (sem_wait(pSemId) != THREAD_SUCCESS)
        { 
            printf("ERROR: Semaphore wait failed (Descriptor: %p)\n", (void*)pSemId);
            blReturn = false;
            break;
        }

    } while (0);

    return blReturn;
}