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