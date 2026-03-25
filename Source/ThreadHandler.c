//**************************** Pthreads_Task **********************************
//  Copyright (c) 2026 Trenser Technology Solutions
//  All Rights Reserved 
//*****************************************************************************
//
// File      : ThreadHandler.c
// Summary   : Implementation of the primary thread routines (Input, Format, Log).
// Note      : None
// Author    : Shani
// Date      : Mar 12, 2026
//
//***************************************************************************** 

//******************************* Include Files *******************************
#include "ThreadHandler.h"

//******************************* Local Types ********************************* 
 
//***************************** Local Constants ******************************* 
#define BUFFER_SIZE         256
#define TIME_BUFFER_SIZE    26
#define DECIMAL_BASE        10
#define MIN_INPUT_VAL       1
#define MAX_INPUT_VAL       65535
#define LOG_TXT_FILE_NAME   "DataLog.txt"
#define FILE_WRITE_MODE     "w"
#define FILE_APPEND_MODE    "a"
#define HDR_RAW_VALUE       "RAW VALUE"
#define HDR_LOG_SCALE       "LOG10 SCALE"
#define HDR_TIMESTAMP       "TIMESTAMP"

//***************************** Local Variables ******************************* 

//****************************** Local Functions ****************************** 
static bool ThreadHandlerSendInput(long lValidInput);
static bool ThreadHandlerAppendDataToLog(MESSAGE* pstIncoming);
static bool ThreadHandlerFormatMsgSend(MESSAGE* pstMessage);
static bool ThreadHandlerFormatInit(void);

//***************************** ThreadHandlerRun ******************************
//Purpose   : Initializes and starts the system threads
//Inputs    : None
//Outputs   : None
//Return    : true if the thread system started and synchronized successfully, 
//            false otherwise.
//Notes     : None.
//******************************************************************************
bool ThreadHandlerRun(void)
{
    bool blStatus = false;

    blStatus = PosixHandlerInitSemaphores();

    if (true == blStatus)
    {
        if (true == PosixHandlerCreateThreads())
        {
            if (false == PosixHandlerSynchronizeThreads())
            {
                printf("ERROR: Failed to sync system threads.\n");
                blStatus = false;
            }
        }
        else
        {
            printf("ERROR: Failed to create system threads.\n");
            blStatus = false;
        }
    }
 
    return blStatus;
}

//************************* ThreadHandlerValidateInput *************************
//Purpose   : Validates user input string .
//Inputs    : pucBuffer    - Pointer to the raw input string buffer.
//            plInputValue - Pointer to store the successfully converted value.
//Outputs   : plInputValue - Updated with the converted long value if valid.
//Return    : true if input is a valid integer false if not valid.
//Notes     : None
//******************************************************************************
bool ThreadHandlerValidateInput(uint8_t* pucBuffer, long* plInputValue) 
{
    bool blStatus = true;
    char* pcEndPtr = NULL;

    do 
    {
        if (pucBuffer == NULL || plInputValue == NULL)
        {
            printf("ERROR: Null pointer detected.\n\r");
            blStatus = false;
            break; 
        }

        *plInputValue = strtol((char*)pucBuffer, &pcEndPtr, DECIMAL_BASE);

        if (pcEndPtr == (char*)pucBuffer)
        {
            printf("ERROR: Invalid input '%s' - Not a numeric value.\n\r", pucBuffer);
            printf("Enter a number (0-65536):\n\r");
            blStatus = false;
            break;
        }

        if (*pcEndPtr != '\0' && *pcEndPtr != '\n' && *pcEndPtr != '\r')
        {
            printf("ERROR: Invalid input '%s' - Contains non-numeric characters.\n\r", 
                   pucBuffer);
            printf("Enter a number (0-65536):\n\r");
            blStatus = false;
            break;
        }

        if (*plInputValue < MIN_INPUT_VAL || *plInputValue > MAX_INPUT_VAL) 
        {
            printf("ERROR: Value %ld is out of range.\n\r", *plInputValue);
            printf("Enter a number (0-65536):\n\r");
            blStatus = false;
            break;
        }

    } while (0);
    
    return blStatus;
}
//*************************** ThreadHandlerSendInput ***************************
//Purpose   : Sends the data to queue
//Inputs    : lValidInput - The validated integer from user input
//Outputs   : None
//Return    : true if message queue is send successfully.
//            false if message queue send is failed.
//Notes     : memory is freed by the receiving thread in formatThread.
//******************************************************************************
static bool ThreadHandlerSendInput(long lValidInput)
{
    bool blStatus = true;
    MESSAGE* pstMessageToFormat = (MESSAGE*)malloc(sizeof(MESSAGE));

    if(pstMessageToFormat == NULL)
    {
        blStatus = false;
        printf("Memory allocation failed \n");
    }
    else
    {
        pstMessageToFormat->unInputData = (uint16_t)lValidInput;
        pstMessageToFormat->fLogValue = log10((double)pstMessageToFormat->unInputData);
        pstMessageToFormat->lTimeStamp = time(NULL);
        
        if (false == PthreadCoreMessageSend(glInputToFormatQueue, &pstMessageToFormat, 
                                      sizeof(MESSAGE*), PRIORITY_NORMAL))
        {
            printf("mq_send to Format failed\n");
            blStatus = false;
            free(pstMessageToFormat);
            pstMessageToFormat = NULL;
        }
    }

    return blStatus;
}

//*********************** ThreadHandlerLogHeaderDisplay ************************
//Purpose   : Displays the formatted table header for data visualization
//Inputs    : None
//Outputs   : None
//Return    : Void
//Notes     : None
//******************************************************************************
static void ThreadHandlerLogHeaderDisplay(void)
{
    printf("%s\n", "========================================================");
    printf("%-10s | %-15s | %-25s\n", HDR_RAW_VALUE, HDR_LOG_SCALE, HDR_TIMESTAMP);
    printf("%s\n", "----------------------------------------------------------");
}

//******************************* ThreadHandlerUserInput************************
//Purpose   : Captures user input, validates range , and sends to queue
//Inputs    : pArguments - standard thread argument
//Outputs   : None
//Return    : NULL
//Notes     : None
//******************************************************************************
void* ThreadHandlerUserInput(void* pArguments)
{
    (void)pArguments; 
    uint8_t ucBuffer[BUFFER_SIZE] = {0};
    long lInputValue = 0;

    if (false == PosixHandlerCreatQueue(USER_INPUT_THREAD_NAME))
    {
        printf("[%s] ERROR: Message Queue initialization failed.\n", USER_INPUT_THREAD_NAME);
    }
    else 
    {
        if (false == PosixHandlerPostSemaphore(INPUT_SEM_NAME))
        {
            printf("[%s] ERROR: Failed to signal Format Thread!\n", USER_INPUT_THREAD_NAME);
        }
    }

    printf("[%s] Started .Enter numbers between 0 and 65536 \n", USER_INPUT_THREAD_NAME);

    while (1)
    {
        if (fgets((char*)ucBuffer, sizeof(ucBuffer), stdin) != NULL)
        {
            ucBuffer[strcspn((char*)ucBuffer, "\n")] = 0;
            
            if(true == ThreadHandlerValidateInput(ucBuffer,&lInputValue))
            {
                printf("\rValid input: %ld. \n", lInputValue);

                if(false == ThreadHandlerSendInput(lInputValue))
                {
                    printf("ERROR: Failed to push data into Queue.\n\r");
                }
            }
        }
    }
    return NULL;
}

//*************************** ThreadHandlerFormatMsgSend ***********************
//Purpose   : Forwards the processed message structure from the Format Thread
//            to the Log Thread through message queue.
//Inputs    : pstMessage - Pointer to the MESSAGE structure .
//Outputs   : None
//Return    : true if the message was sent successfully; false otherwise.
//Notes     : None
//******************************************************************************
static bool ThreadHandlerFormatMsgSend(MESSAGE* pstMessage)
{
    bool blStatus = true;

    do 
    {
        if (NULL == pstMessage)
        {
            blStatus = false;
            break; 
        }

        if (false == PthreadCoreMessageSend(glFormatToLogQueue, &pstMessage, 
                                            sizeof(MESSAGE*), PRIORITY_NORMAL))
        {
            printf("mq_send to Log failed\n");
            free(pstMessage);
            blStatus = false;
            break;
        }

    } while (0);

    return blStatus;
}

//*************************** ThreadHandlerFormatInit **************************
//Purpose   : Initializes the synchronization and message queue for the Format
//            thread. 
//Inputs    : None
//Outputs   : None
//Return    : true if synchronization and queue creation succeed;
//            false if synchronization or queue creation failed.
//Notes     : None
//******************************************************************************
static bool ThreadHandlerFormatInit(void)
{
    bool blStatus = true;

    do 
    {
        if (false == PosixHandlerWaitSemaphore(INPUT_SEM_NAME))
        {
            printf("ERROR - Failed to receive signal from input thread.\n");
            blStatus = false;
            break;
        }

        if (false == PosixHandlerCreatQueue(DATA_FORMAT_THREAD_NAME))
        {
            printf("[%s] ERROR: Queue initialization failed.\n", DATA_FORMAT_THREAD_NAME);
            blStatus = false;
            break;
        }

        if (false == PosixHandlerPostSemaphore(FORMAT_SEM_NAME))
        {
            printf("[%s] ERROR: Failed to signal Log Thread.\n", DATA_FORMAT_THREAD_NAME);
            blStatus = false;
            break;
        }

        printf("[%s]: Initialization success. Signaling Log Thread.\n", DATA_FORMAT_THREAD_NAME);

    } while (0);

    return blStatus;
}

//*************************** ThreadHandlerFormatInput *************************
//Purpose   : Receives data from Input Queue, displays it in a table format.
//Inputs    : pArguments - standard thread argument
//Outputs   : None
//Return    : NULL
//Notes     : frees the memory allocated by the Input Thread.
//******************************************************************************
void* ThreadHandlerFormatInput(void* pArguments)
{
    (void)pArguments;
    MESSAGE* pstIncoming = NULL;
    uint32_t ulPriority = 0;
    char cTimeBuffer[TIME_BUFFER_SIZE] = {0}; 
    struct tm stTimeInfo = {0};
    time_t lActualTime = 0;

    if(false == ThreadHandlerFormatInit())
    {
        printf("[%s] ERROR: Initialization failed.\n", DATA_FORMAT_THREAD_NAME);
    }
 
    printf("[%s] Started.\n", DATA_FORMAT_THREAD_NAME);

    while (1)
    {
        if(true == PthreadCoreMessageReceive(glInputToFormatQueue, &pstIncoming, 
                                          sizeof(MESSAGE*), &ulPriority))
        {
            if (pstIncoming != NULL)
            {
                ThreadHandlerLogHeaderDisplay();
                lActualTime = (time_t)pstIncoming->lTimeStamp;
                localtime_r(&lActualTime, &stTimeInfo);
                strftime(cTimeBuffer, sizeof(cTimeBuffer), "%Y-%m-%d %H:%M:%S", 
                                                                &stTimeInfo);
                printf("%-10u | %-15.4f | %-25s\n", pstIncoming->unInputData, 
                        pstIncoming->fLogValue, cTimeBuffer);
                printf("\nEnter a number (0-65536):\n\r ");

                if (false == ThreadHandlerFormatMsgSend(pstIncoming))
                {
                    printf("Message sending to Log failed\n");
                }
            }
        }
        else
        {
            printf("mq_receive failed in FormatThread\n");
        }
    }
    return NULL;
}

//*************************** ThreadHandlerInitLogFile *************************
//Purpose   : Initializes the log file and writes the table header
//Inputs    : None
//Outputs   : None
//Return    : true if file is ready for logging; false on file creation error.
//Notes     : None
//******************************************************************************
static bool ThreadHandlerInitLogFile(void)
{
    bool blStatus = true;
    FILE* pstLogFile = NULL;

    if (access(LOG_TXT_FILE_NAME, F_OK) != 0)
    {
        pstLogFile = fopen(LOG_TXT_FILE_NAME, FILE_WRITE_MODE);

        if (pstLogFile != NULL)
        {
            fprintf(pstLogFile, "===========================================\n");
            fprintf(pstLogFile, "%-10s | %-15s | %-25s\n", 
                                    HDR_RAW_VALUE, HDR_LOG_SCALE, HDR_TIMESTAMP);
            fprintf(pstLogFile, "-------------------------------------------\n");
            fclose(pstLogFile);
            printf("[%s] New log file created.\n", DATA_LOG_THREAD_NAME);
        }
        else
        {
            blStatus = false;
            printf("Error: Could not create log file\n\r");
        }
    }
    else
    {
        printf("[%s] Log file already exists. Appending to existing data.\n", 
                                            DATA_LOG_THREAD_NAME);
    }

    return blStatus;
}

//*************************** ThreadHandlerAppendDataToLog *********************
//Purpose   : Opens the log file in append mode and records data 
//Inputs    : pstIncoming - Pointer to the MESSAGE structure containing data
//Outputs   : None
//Return    : true if data appends successfully false if data logging fails
//Notes     : None
//******************************************************************************
static bool ThreadHandlerAppendDataToLog(MESSAGE* pstIncoming)
{
    bool blStatus = true;
    FILE* pstLogFile = NULL;
    char cTimeBuffer[TIME_BUFFER_SIZE] = {0};
    time_t lActualTime = 0;
    int lBytesWritten = 0 ;

    do 
    {
        if (pstIncoming == NULL)
        {
            blStatus = false;
            break;
        }

        pstLogFile = fopen(LOG_TXT_FILE_NAME, FILE_APPEND_MODE);
        if (NULL == pstLogFile)
        {
            printf("ERROR: Failed to open log file for appending\n\r");
            blStatus = false;
            break;
        }

        lActualTime = (time_t)pstIncoming->lTimeStamp;
        ctime_r(&lActualTime, cTimeBuffer);
        cTimeBuffer[strcspn(cTimeBuffer, "\n")] = 0;
        lBytesWritten = fprintf(pstLogFile, "%-10u | %-15.4f | %-25s\n", 
                    pstIncoming->unInputData, pstIncoming->fLogValue, cTimeBuffer);

        if (lBytesWritten <= 0)
        {
            printf("ERROR: No bytes written to log file\n\r");
            blStatus = false;
        }

        fclose(pstLogFile);

    } while (0);

    return blStatus;
}

//*************************** ThreadHandlerLogData *****************************
//Purpose   : Receives processed data and appends it to a log file.
//Inputs    : pArguments - standard thread argument 
//Outputs   : None
//Return    : NULL
//Notes     : None
//******************************************************************************
void* ThreadHandlerLogData(void* pArguments)
{
    (void)pArguments;
    MESSAGE* pstIncoming = NULL;
    uint32_t ulPriority = 0;

    if (true == PosixHandlerWaitSemaphore(FORMAT_SEM_NAME))
    {
        if (false == ThreadHandlerInitLogFile())
        {
            printf("[%s] ERROR: DataLog file initialization failed.\n", DATA_LOG_THREAD_NAME);
        }
    }
    else
    {
        printf("[%s] ERROR - Failed to receive sync signal.\n", DATA_LOG_THREAD_NAME);
    }
 
    while (1)
    {
        if(true == PthreadCoreMessageReceive(glFormatToLogQueue, &pstIncoming, 
                                            sizeof(MESSAGE*), &ulPriority))
        {
            if (pstIncoming != NULL)
            {
                if (false == ThreadHandlerAppendDataToLog(pstIncoming))
                {
                    printf("[%s] WARNING: File log failed! Data\n", 
                                                        DATA_LOG_THREAD_NAME);
                }
                free(pstIncoming);
                pstIncoming = NULL;
            }
        }
        else
        {
            printf("mq_receive failed in logThread\n");
        }
    }
    return NULL;
}
