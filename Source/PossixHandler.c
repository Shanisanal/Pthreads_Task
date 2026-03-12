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
#include "Include/Common.h"

//******************************* Local Types ********************************* 
 
//***************************** Local Constants ******************************* 
#define SUCCESS_RETURN         0

//***************************** Local Variables ******************************* 
typedef enum 
{
    THREAD_SUCCESS      = 0,
    ERR_INPUT_THREAD    = (1 << 0), 
    ERR_FORMAT_THREAD   = (1 << 1), 
    ERR_LOG_THREAD      = (1 << 2)  
} THREAD_ERROR_MASK;

//****************************** Local Functions ****************************** 


int CreateThreads (void)
{
    int lReturn = THREAD_SUCCESS;





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
bool POSSIXHandlerCreateThread(pthread_t* pulThread, void *(*routine)(void*), 
                               void* pvarguments, const char* pcThreadName)
{
    bool blReturn = true;

    if((pulThread == NULL) || (routine == NULL))
    {
        blReturn = false;
    }

    if(blReturn == true)
    {
        if(pthread_create(pulThread, NULL, routine, pvarguments) == SUCCESS_RETURN)
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