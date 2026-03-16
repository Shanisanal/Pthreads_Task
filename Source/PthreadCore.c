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
#define SUCCESS_RETURN         0

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