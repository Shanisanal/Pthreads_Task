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
#define MAX_MSG_QUEUE       10
#define MSG_QUEUE_ERR       -1

#define INPUT_QUEUE_NAME    "/q_input_to_format"
#define LOG_QUEUE_NAME      "/q_format_to_log"
#define QUEUE_PERMISSIONS   0600

#define LOG_TXT_FILE_NAME   "DataLog.txt"
#define FILE_WRITE_MODE     "w"
#define FILE_APPEND_MODE    "a"

//***************************** Local Variables ******************************* 
mqd_t qInputToFormat; 
mqd_t qFormatToLog;

//****************************** Local Functions ****************************** 

bool ExecuteThreads(void)
{
    bool blStatus = false;
    int lThreadCreateReturn = -1;

    blStatus = CreateMessageQueue();

    if(blStatus == true)
    {
        lThreadCreateReturn = CreateThreads();

        if (lThreadCreateReturn != THREAD_SUCCESS)
        {
            blStatus = false;
        }
    }

    return blStatus;
}

//***************************** CreateMessageQueue *****************************
//Purpose   : Initializes and opens POSIX message queues for thread communication
//Inputs    : None
//Outputs   : None
//Return    : true if both queues are successfully opened, false otherwise
//Notes     : Closes first queue if the second fails.
//******************************************************************************
bool CreateMessageQueue (void)
{
    mq_unlink(INPUT_QUEUE_NAME);
    mq_unlink(LOG_QUEUE_NAME);
    struct mq_attr stMessageAttr = {0};
    bool blQueueStatus = true;

    stMessageAttr.mq_maxmsg = MAX_MSG_QUEUE;
    stMessageAttr.mq_msgsize = sizeof(MESSAGE*);

    /*Open/Create the Input to Format Queue */
    qInputToFormat = mq_open(INPUT_QUEUE_NAME, O_CREAT | O_RDWR, 
                                    QUEUE_PERMISSIONS, &stMessageAttr);
    
    if (MSG_QUEUE_ERR == qInputToFormat) 
    {
        printf("Failed to create InputToFormat Queue\n\r");
        blQueueStatus =  false;
    }

    if(blQueueStatus == true)
    {
        qFormatToLog = mq_open(LOG_QUEUE_NAME, O_CREAT | O_RDWR, 
                                     QUEUE_PERMISSIONS, &stMessageAttr);
        
        if(MSG_QUEUE_ERR == qFormatToLog)
        {
            printf("Failed to create FormatToLog Queue\n\r");
            mq_close(qInputToFormat);
            blQueueStatus = false;
        }
    }

    return blQueueStatus;
}
//*************************** ValidateUserInput ********************************
//Purpose   : Validates user input string .
//Inputs    : pucBuffer    - Pointer to the raw input string buffer.
//            plInputValue - Pointer to store the successfully converted value.
//Outputs   : plInputValue - Updated with the converted long value if valid.
//Return    : true if input is a valid integer false otherwise.
//Notes     : None
//******************************************************************************
bool ValidateUserInput(uint8_t* pucBuffer, long* plInputValue)
{
    bool blStatus = true;
    char* pcEndPtr = NULL;

    if(pucBuffer == NULL || plInputValue == NULL)
    {
        blStatus = false;
    }

    if(blStatus == true)
    {
        *plInputValue = strtol((char*)pucBuffer,&pcEndPtr, DECIMAL_BASE);

        if(pcEndPtr == (char*)pucBuffer)
        {
            blStatus = false;
        }
        else if (*plInputValue < MIN_INPUT_VAL || *plInputValue > MAX_INPUT_VAL) 
        {
            blStatus = false;
        }
    }
    
    return blStatus;
}
//*************************** PostInputToQueue *********************************
//Purpose   : Allocates a MESSAGE struct, populates data, and sends it to queue
//Inputs    : lValidInput - The validated integer from user input
//Outputs   : None
//Return    : void
//Notes     : memory is freed by the receiving thread in formatThread.
//******************************************************************************
void PostInputToQueue(long lValidInput)
{
    MESSAGE* pstMessageToFormat = (MESSAGE*)malloc(sizeof(MESSAGE));
    if(pstMessageToFormat == NULL)
    {
        printf("Memory allocation failed \n");
    }
    else
    {
        pstMessageToFormat->unInputData = (uint16_t)lValidInput;
        pstMessageToFormat->fLogValue = log10((double)pstMessageToFormat->unInputData);
        pstMessageToFormat->lTimeStamp = time(NULL);

        if (mq_send(qInputToFormat, (const char*)&pstMessageToFormat, 
                        sizeof(MESSAGE*), PRIORITY_NORMAL) == MSG_QUEUE_ERR)
        {
            printf("mq_send to Format failed\n");
            free(pstMessageToFormat);
            pstMessageToFormat = NULL;
        }
    }
}

//*************************** PrintDisplayTable ********************************
//Purpose   : Displays the formatted table header for data visualization
//Inputs    : None
//Outputs   : None
//Return    : Void
//Notes     : Uses fixed-width formatting to ensure column alignment
//******************************************************************************
void PrintDisplayTable(void)
{
    printf("\n%s\n", "========================================================");
    printf("%-10s | %-15s | %-25s\n", "RAW VALUE", "LOG10 SCALE", "TIMESTAMP");
    printf("%s\n", "----------------------------------------------------------");

}
//******************************* InputThreadHandler****************************
//Purpose   : Captures user input, validates range [1, 65535], and sends to queue
//Inputs    : pvarguments - standard thread argument (unused)
//Outputs   : None
//Return    : NULL
//Notes     : None
//******************************************************************************
void* InputThreadHandler(void* pArguments)
{
    (void)pArguments; 
    uint8_t ucBuffer[BUFFER_SIZE] = {0};
    long lInputValue = 0;
    bool blReturnStatus = false;
    printf("[%s] Started. Enter numbers between 0 and 65536\n", USER_INPUT_THREAD_NAME);

    while (1)
    {
        if (fgets((char*)ucBuffer, sizeof(ucBuffer), stdin) != NULL)
        {
            ucBuffer[strcspn((char*)ucBuffer, "\n")] = 0;

            blReturnStatus = ValidateUserInput(ucBuffer,&lInputValue);
            
            if(true == blReturnStatus)
            {
                printf("Valid input: %ld. Sending to pipeline...\n", lInputValue);
                PostInputToQueue(lInputValue);
            }
            else
            {
                printf("Invalid Input ,Enter numbers between 0 and 65536\n");
            }
        }
    }
    return NULL;
}

//*************************** FormatThreadHandler ******************************
//Purpose   : Receives data from Input Queue, displays it in a table format.
//Inputs    : pvarguments - standard thread argument
//Outputs   : None
//Return    : NULL
//Notes     : frees the memory allocated by the Input Thread.
//******************************************************************************
void* FormatThreadHandler(void* pArguments)
{
    (void)pArguments;
    MESSAGE* pstIncoming = NULL;
    uint32_t ulPriority = 0;
    char cTimeBuffer[TIME_BUFFER_SIZE] = {0}; 
    struct tm stTimeInfo = {0};
    time_t lActualTime = 0;

    printf("[%s] Started.\n", DATA_FORMAT_THREAD_NAME);

    while (1)
    {
        if (mq_receive(qInputToFormat, (char*)&pstIncoming, 
                       sizeof(MESSAGE*), &ulPriority) != MSG_QUEUE_ERR)
        {
            if (pstIncoming != NULL)
            {
                PrintDisplayTable();
                lActualTime = (time_t)pstIncoming->lTimeStamp;
                localtime_r(&lActualTime, &stTimeInfo);;

                strftime(cTimeBuffer, sizeof(cTimeBuffer), "%Y-%m-%d %H:%M:%S", 
                                                                &stTimeInfo);
                printf("%-10u | %-15.4f | %-25s\n", pstIncoming->unInputData, 
                        pstIncoming->fLogValue, cTimeBuffer);

                if (mq_send(qFormatToLog, (const char*)&pstIncoming, 
                    sizeof(MESSAGE*), PRIORITY_NORMAL) == MSG_QUEUE_ERR)
                {
                    printf("mq_send to Log failed\n");
                    free(pstIncoming);
                    pstIncoming = NULL;
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

//*************************** PrintLogFileHeader *******************************
//Purpose   : Initializes the log file and writes the table header
//Inputs    : None
//Outputs   : None
//Return    : Void
//Notes     : Opens file in "w" mode to ensure a fresh log for each session
//******************************************************************************
void PrintLogFileHeader(void)
{
    FILE* pstLogFile = NULL;

    if (access(LOG_TXT_FILE_NAME, F_OK) != 0)
    {
        // File does NOT exist, so create it and write the header
        pstLogFile = fopen(LOG_TXT_FILE_NAME, FILE_WRITE_MODE);

        if (pstLogFile != NULL)
        {
            fprintf(pstLogFile, "===========================================\n");
            fprintf(pstLogFile, "%-10s | %-15s | %-25s\n", 
                                        "RAW VALUE", "LOG10 SCALE", "TIMESTAMP");
            fprintf(pstLogFile, "-------------------------------------------\n");
            fclose(pstLogFile);
            printf("[%s] New log file created.\n", DATA_LOG_THREAD_NAME);
        }
        else
        {
            printf("Error: Could not create log file\n\r");
        }
    }
    else
    {
        printf("[%s] Log file already exists. Appending to existing data.\n", 
                                            DATA_LOG_THREAD_NAME);
    }
}

//*************************** AppendDataToLog **********************************
//Purpose   : Opens the log file in append mode and records a single data row
//Inputs    : pstIncoming - Pointer to the MESSAGE structure containing data
//Outputs   : None
//Return    : Void
//Notes     : Handles internal time conversion and file pointer safety
//******************************************************************************
void AppendDataToLog(MESSAGE* pstIncoming)
{
    FILE* pstLogFile = NULL;
    char cTimeBuffer[TIME_BUFFER_SIZE] = {0};
    time_t lActualTime = 0;

    if (pstIncoming != NULL)
    {
        pstLogFile = fopen(LOG_TXT_FILE_NAME, FILE_APPEND_MODE);

        if (pstLogFile != NULL)
        {
            lActualTime = (time_t)pstIncoming->lTimeStamp;
            ctime_r(&lActualTime, cTimeBuffer);
            cTimeBuffer[strcspn(cTimeBuffer, "\n")] = 0;

            fprintf(pstLogFile, "%-10u | %-15.4f | %-25s\n", 
                    pstIncoming->unInputData, 
                    pstIncoming->fLogValue, 
                    cTimeBuffer);

            fclose(pstLogFile);
        }
        else
        {
            printf("Failed to open log file for appending\n\r");
        }
    }
}
//*************************** LogThreadHandler *********************************
//Purpose   : Receives processed data and appends it to a log file.
//Inputs    : pArguments - standard thread argument 
//Outputs   : None
//Return    : NULL
//Notes     : None
//******************************************************************************
void* LogThreadHandler(void* pArguments)
{
    (void)pArguments;
    MESSAGE* pstIncoming = NULL;
    uint32_t ulPriority = 0;

    printf("[%s] Started.\n", DATA_LOG_THREAD_NAME);
    PrintLogFileHeader();

    while (1)
    {
        if (mq_receive(qFormatToLog, (char*)&pstIncoming, 
                       sizeof(MESSAGE*), &ulPriority) != MSG_QUEUE_ERR)
        {
            if (pstIncoming != NULL)
            {
                AppendDataToLog(pstIncoming);
                free(pstIncoming);
                pstIncoming = NULL;
            }
        }
    }
    return NULL;
}
