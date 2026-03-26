//**************************** Pthreads_Task **********************************
//  Copyright (c) 2026 Trenser Technology Solutions
//  All Rights Reserved 
//*****************************************************************************
//
// File      : PosixHandler.c
// Summary   : POSIX handling functions
// Note      : None
// Author    : Shani
// Date      : Mar 12, 2026
//
//***************************************************************************** 

//******************************* Include Files *******************************
#include "PosixHandler.h"

//******************************* Local Types ********************************* 
 
//***************************** Local Constants ******************************* 
#define INPUT_QUEUE_NAME    "/q_InputToFormat"
#define FORMAT_QUEUE_NAME   "/q_FormatToLlog"
#define QUEUE_PERMISSIONS   0640
#define MAX_MSG_QUEUE       10

#define NUM_THREADS (sizeof(sstThreadConfigTable) / sizeof(sstThreadConfigTable[0]))
#define NUM_QUEUES (sizeof(sstQueueLookupTable) / sizeof(sstQueueLookupTable[0]))
#define NUM_SEMAPHORES (sizeof(sstSemCreateTable) / sizeof(sstSemCreateTable[0]))

//***************************** Local Variables ******************************* 
pthread_t gulUserInputThread    = 0; 
pthread_t gulFormatInputThread  = 0;
pthread_t gulLogThread          = 0;
mqd_t glInputToFormatQueue      = -1; 
mqd_t glFormatToLogQueue        = -1;
sem_t gSemInputQueueReady       = {0};
sem_t gSemFormatQueueReady      = {0};

//****************************** Local Functions ****************************** 
// Defines the execution entry point, ID, and identity for each thread.
static const THREAD_CONFIG sstThreadConfigTable[] = 
{
    {&gulUserInputThread,   ThreadHandlerUserInput,    USER_INPUT_THREAD_NAME, },
    {&gulFormatInputThread, ThreadHandlerFormatInput,  DATA_FORMAT_THREAD_NAME,},
    {&gulLogThread,         ThreadHandlerLogData,      DATA_LOG_THREAD_NAME,   }
};

//Maps communication paths between threads
static const QUEUE_CONFIG sstQueueLookupTable[] = 
{
    {USER_INPUT_THREAD_NAME,    INPUT_QUEUE_NAME,   &glInputToFormatQueue },
    {DATA_FORMAT_THREAD_NAME,   FORMAT_QUEUE_NAME,  &glFormatToLogQueue   }
};

//Initializes the signaling locks that synchronize data flow
static const SEM_CREATE sstSemCreateTable[] = 
{
    {&gSemInputQueueReady,  0, INPUT_SEM_NAME  },
    {&gSemFormatQueueReady, 0, FORMAT_SEM_NAME }
};

//******************************* PosixHandlerCreateThreads ********************
//Purpose   : Starts all system threads
//Inputs    : None
//Outputs   : None
//Return    : true if all threads are successfully created, false if any failed.
//Notes     : None
//******************************************************************************
bool PosixHandlerCreateThreads (void) 
{
    bool blStatus = true;

    for (uint8_t ucIndex = 0; ucIndex < NUM_THREADS; ucIndex++)
    {
        if (false == PthreadCoreCreateThread(&sstThreadConfigTable[ucIndex]))
        {
            blStatus = false;
        }
    }

    return blStatus;
}

//*********************** PosixHandlerSynchronizeThreads ***********************
//Purpose   : Synchronizes the threads
//Inputs    : None
//Outputs   : None
//Return    : true if all threads synchronized successfully , false if any 
//            thread failed to join.
//Notes     : None
//******************************************************************************
bool PosixHandlerSynchronizeThreads(void)
{
    bool blStatus = true;

    for (uint8_t ucIndex = 0; ucIndex < NUM_THREADS; ucIndex++)
    {
        if (false == PthreadCoreSynchronizeThreads(&sstThreadConfigTable[ucIndex]))
        {
            blStatus = false;
        }
    }

    return blStatus;
}

//***************************** PosixHandlerCreatQueue *************************
//Purpose   : Initializes and opens POSIX message queues 
//Inputs    : pcThreadName - The name of the thread requesting queue creation.
//Outputs   : None
//Return    : true if both queues are successfully opened, false otherwise
//Notes     : Closes first queue if the second fails.
//******************************************************************************
bool PosixHandlerCreatQueue (const char* pcThreadName)
{
    struct mq_attr stMessageAttr = {0};
    bool blQueueStatus = true;
    stMessageAttr.mq_maxmsg = MAX_MSG_QUEUE;
    stMessageAttr.mq_msgsize = sizeof(MESSAGE*);

    do 
    {
        if (NULL == pcThreadName)
        {
            blQueueStatus = false;
            break; 
        }

        for (uint8_t ucIndex = 0; ucIndex < NUM_QUEUES; ucIndex++)
        {
            if (0 == strcmp(pcThreadName, sstQueueLookupTable[ucIndex].pcTargetThread))
            {
                *(sstQueueLookupTable[ucIndex].plQueueDescriptor) = 
                    PthreadCoreOpenQueue( sstQueueLookupTable[ucIndex].pcQueueName, 
                            O_CREAT|O_RDWR, QUEUE_PERMISSIONS, &stMessageAttr);

                if (MSG_QUEUE_ERR == (*(sstQueueLookupTable[ucIndex].plQueueDescriptor)))
                {
                    printf("ERROR: Failed to create queue\n");
                    blQueueStatus = false;
                }
                
                break; 
            }
        }

    } while (0);

    return blQueueStatus;
}

//*************************** PosixHandlerInitSemaphores ***********************
//Purpose   : Initializes semaphore with init value 0.
//Inputs    : None 
//Outputs   : None
//Return    : true if the semaphore is successfully initialized, 
//            false if semaphore initialization failed
//Notes     : None
//******************************************************************************
bool PosixHandlerInitSemaphores(void) 
{
    bool blStatus = true; //todo

    for (uint8_t ucIndex = 0; ucIndex < NUM_SEMAPHORES; ucIndex++)
    {
        if (false == PthreadCoreSemInit(&sstSemCreateTable[ucIndex]))
        {
            blStatus = false;
            break;
        }
    }

    return blStatus;
}

//************************* PosixHandlerPostSemaphore **************************
//Purpose   : Post semaphore to wake up waiting threads.
//Inputs    : pcSemName - The string name of the semaphore to post.
//Outputs   : None
//Return    : true if the semaphore was posted successfully , false otherwise.
//Notes     : None.
//******************************************************************************
bool PosixHandlerPostSemaphore(const char* pcSemName)
{
    bool blStatus = true;
    uint8_t ucIndex = 0;

    do 
    {
        if (NULL == pcSemName)
        {
            blStatus = false;
            break; 
        }

        for (ucIndex = 0; ucIndex < NUM_SEMAPHORES; ucIndex++)
        {
            if (0 == strcmp(pcSemName, sstSemCreateTable[ucIndex].pcSemName))
            {
                blStatus = PthreadCoreSemPost(sstSemCreateTable[ucIndex].pSemId);
                break;
            }
        }

        if (false == blStatus)
        {
            printf("ERROR: Failed to POST semaphore '%s'\n", pcSemName);
        }

    } while (0);

    return blStatus;
}

//************************** PosixHandlerWaitSemaphore *************************
//Purpose   : Wait on the semaphore.
//Inputs    : pcSemName - The string name of the semaphore to wait for.
//Outputs   : None
//Return    : true if the semaphore wait was successful ,false if the semaphore wait failed.
//Notes     : None
//******************************************************************************
bool PosixHandlerWaitSemaphore(const char* pcSemName)
{
    bool blStatus = false;
    uint8_t ucIndex = 0;

    do 
    {
        if (NULL == pcSemName)
        {
            break; 
        }

        for (ucIndex = 0; ucIndex < NUM_SEMAPHORES; ucIndex++)
        {
            if (0 == strcmp(pcSemName, sstSemCreateTable[ucIndex].pcSemName))
            {
                blStatus = PthreadCoreSemWait(sstSemCreateTable[ucIndex].pSemId);
                break; 
            }
        }

        if (false == blStatus)
        {
            printf("ERROR: failed to WAIT on semaphore '%s'\n", pcSemName);
        }

    } while (0);

    return blStatus;
}
