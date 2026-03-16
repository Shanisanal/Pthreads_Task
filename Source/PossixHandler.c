//**************************** Pthreads_Task **********************************
//  Copyright (c) 2026 Trenser Technology Solutions
//  All Rights Reserved 
//*****************************************************************************
//
// File      : PossixHandler.c
// Summary   : POSSIX handling functions
// Note      : None
// Author    : Shani
// Date      : Mar 12, 2026
//
//***************************************************************************** 

//******************************* Include Files *******************************
#include "PossixHandler.h"

//******************************* Local Types ********************************* 
 
//***************************** Local Constants ******************************* 
#define SUCCESS_RETURN         0

//***************************** Local Variables ******************************* 
pthread_t gulUserInputThread = 0; 
pthread_t gulFormatInputThread = 0;
pthread_t gulLogThread = 0;

//****************************** Local Functions ****************************** 
//******************************* CreateThreads ********************************
//Purpose   : Manager function to create all system threads and track failures
//Inputs    : None
//Outputs   : None
//Return    : THREAD_SUCCESS (0) if all threads started, or a bitmask of failures
//Notes     : Uses a bitwise OR to accumulate errors for each failed thread
//******************************************************************************
int CreateThreads (void)
{
    int lReturn = THREAD_SUCCESS;

    if (POSSIXHandlerCreateThread(&gulUserInputThread, InputThreadHandler, NULL,
                    USER_INPUT_THREAD_NAME) == false)
    {
        lReturn |= ERR_INPUT_THREAD;
    }

    if (POSSIXHandlerCreateThread(&gulFormatInputThread, FormatThreadHandler, NULL,
                    DATA_FORMAT_THREAD_NAME) == false)
    {
        lReturn |= ERR_FORMAT_THREAD;
    }

    if (POSSIXHandlerCreateThread(&gulLogThread, LogThreadHandler , NULL,
                    DATA_LOG_THREAD_NAME) == false)
    {
        lReturn |= ERR_LOG_THREAD;
    }

    return lReturn;
}
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
//*************************** SynchronizeThreads *******************************
//Purpose   : Blocks the main process until all operational threads complete.
//Inputs    : None
//Outputs   : None
//Return    : None
//Notes     : None
//******************************************************************************
void SynchronizeThreads(void)
{
    pthread_join(gulUserInputThread, NULL);
    pthread_join(gulFormatInputThread, NULL);
    pthread_join(gulLogThread, NULL);
}