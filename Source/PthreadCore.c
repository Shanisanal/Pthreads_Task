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

//************************* POSSIXHandlerCreateThread.**************************
//Purpose   : Wrapper to create a POSSIX thread
//Inputs    : pulThread - pointer to the thread ID variable
//            routine - address of thread function
//            pvarguments - arguments to pass to the routine
//            pcThreadName - name of the thread
//Outputs   : None
//Return    : true - Thread created, false - thread creation failed
//Notes     : None
//*****************************************************************************
bool POSSIXHandlerCreateThread(pthread_t* pulThread, void *(*pRoutine)(void*), 
                               void* pArguments, const char* pcThreadName)
{
    bool blReturn = true;

    if((pulThread == NULL) || (pRoutine == NULL))
    {
        blReturn = false;
    }

    if(blReturn == true)
    {
        if(pthread_create(pulThread, NULL, pRoutine, pArguments) == SUCCESS_RETURN)
        {
            printf(" %s Created Successfully  \r\n",pcThreadName);
        }
        else
        {
            blReturn = false;
            printf("ERROR: %s Creation failed\r\n",pcThreadName);
        }
    }
    else
    {
        printf("ERROR: Invalid parameters in %s creation\r\n",pcThreadName);
    }

    return blReturn;
}
//************************* POSSIXSynchronizeThreads **************************
//Purpose    : Wrapper to synchronize a POSIX thread
//Inputs     : pulThread - pointer to the thread ID variable
//             pcThreadName - name of the thread for logging purposes
//Outputs    : None
//Return     : 0 - Thread synchronized successfully, 
//Notes      : This function blocks the caller until the target thread terminates.
//*****************************************************************************
bool POSSIXSynchronizeThreads(pthread_t ulThread, const char* pcThreadName)
{
    bool blReturn = false;
    int lReturnStatus = -1;

    lReturnStatus = pthread_join(ulThread, NULL);

    if(SUCCESS_RETURN == lReturnStatus)
    {
        blReturn = true;
    }
    else
    {
        printf("ERROR: %s Synchronized failed\r\n",pcThreadName);
    }
    
    return blReturn;
}


//***************************** FindMessageQueue *******************************
//Purpose   : Checks if a POSIX message queue exists in the kernel.
//Inputs    : pcQueuename - The string name of the queue to search for
//Outputs   : None
//Return    : true if the queue is found, false otherwise
//Notes     : Attempts to open in O_RDONLY mode and closes the handle immediately.
//******************************************************************************
bool FindMessageQueue(const char* pcQueuename)
{
    bool blStatus = true;
    mqd_t lTempQueue = 0;

    lTempQueue = mq_open(pcQueuename,O_RDONLY);

    if(lTempQueue != MSG_QUEUE_ERR)
    {
        mq_close(lTempQueue);
    }
    else
    {
        blStatus = false;
        printf(" %s does not exist.\r\n", pcQueuename);
    }

    return blStatus;

}

//***************************** RemoveMessageQueue *****************************
//Purpose   : Deletes a message queue from the system if it exists
//Inputs    : pcQueuename - The string name of the queue
//Outputs   : None
//Return    : true if queue was found and removed, false if it didn't exist
//Notes     : None
//******************************************************************************
bool RemoveMessageQueue(const char* pcQueuename)
{
    bool blStatus = true;
    int lReturnStatus = 0;

    blStatus = FindMessageQueue(pcQueuename) ;

    if(true == blStatus)
    {
        lReturnStatus = mq_unlink(pcQueuename);

        if(lReturnStatus != MSG_QUEUE_ERR)
        {
            printf(" %s removed successfully.\r\n", pcQueuename);
        }
        else
        {
            printf("ERROR: Failed to unlink %s.\r\n", pcQueuename);
            blStatus = false;
        }
    }

    return blStatus;
}

