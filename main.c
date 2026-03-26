//**************************** Pthreads_Task **********************************
//  Copyright (c) 2026 Trenser Technology Solutions
//  All Rights Reserved 
//*****************************************************************************
//
// File      : main.c
// Summary   : This program implements a multi-threaded data pipeline using Pthreads.
// Note      : None
// Author    : Shani
// Date      : Mar 12, 2026
//
//***************************************************************************** 

//******************************* Include Files *******************************
#include "Common.h"
#include "PosixHandler.h"
#include "ThreadHandler.h"

//******************************* Local Types ********************************* 
 
//***************************** Local Constants ******************************* 

//***************************** Local Variables ******************************* 

//****************************** Local Functions ****************************** 

//*********************************** main *************************************
//Purpose   : Entry point of the thread application.
//Inputs    : None
//Outputs   : None
//Return    : 0 on successful completion, 1 if thread execution fails.
//Notes     : None
//******************************************************************************
int main()
{
    int lReturnCode = 0;
    bool blStatus = true;

    blStatus = ThreadHandlerRun();

    if(false == blStatus)
    {
        printf("Critical Error: Thread execution failed.\n");
        lReturnCode = 1; 
    }

    return lReturnCode; 
}
