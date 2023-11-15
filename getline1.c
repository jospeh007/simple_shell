#include "shell.h"

/**
 * bufferCommandInput - buffers chained commands
 * @commandInfo: parameter structure
 * @buffer: address of buffer
 * @bufferLength: address of buffer length variable
 *
 * Return: bytes read
 */
ssize_t bufferCommandInput(CommandInfo *commandInfo, char **buffer, size_t *bufferLength)
{
    ssize_t bytesRead = 0;
    size_t lengthRead = 0;

    if (!*bufferLength)
    {
        free(*buffer);
        *buffer = NULL;
        signal(SIGINT, blockCtrlC);

        #if USE_GETLINE
        bytesRead = getline(buffer, &lengthRead, stdin);
        #else
        bytesRead = customGetline(commandInfo, buffer, &lengthRead);
        #endif

        if (bytesRead > 0)
        {
            if ((*buffer)[bytesRead - 1] == '\n')
            {
                (*buffer)[bytesRead - 1] = '\0'; 
                bytesRead--;
            }
            commandInfo->lineCountFlag = 1;
            removeComments(*buffer);
            buildHistoryList(commandInfo, *buffer, commandInfo->historyCount++);
            *bufferLength = bytesRead;

            if (_strchr(*buffer, ';')) 
            {
                commandInfo->commandBuffer = buffer;
            }
        }
    }

    return bytesRead;
}

/**
 * getInput - gets a line without the newline character
 * @commandInfo: parameter structure
 *
 * Return: bytes read
 */
ssize_t getInput(CommandInfo *commandInfo)
{
    static char *commandBuffer; 
    static size_t currentPosition, iterator, bufferLength;
    ssize_t bytesRead = 0;
    char **bufferPtr = &(commandInfo->argument), *position;

    _putchar(BUF_FLUSH);
    bytesRead = bufferCommandInput(commandInfo, &commandBuffer, &bufferLength);

    if (bytesRead == -1) 
        return -1;

    if (bufferLength) 
    {
        iterator = currentPosition; 
        position = commandBuffer + currentPosition; 

        checkCommandChain(commandInfo, commandBuffer, &iterator, currentPosition, bufferLength);
        
        while (iterator < bufferLength)
        {
            if (isCommandChain(commandInfo, commandBuffer, &iterator))
                break;
            iterator++;
        }

        currentPosition = iterator + 1; 
        
        if (currentPosition >= bufferLength) 
        {
            currentPosition = bufferLength = 0; 
            commandInfo->commandBufferType = CMD_NORMAL;
        }

        *bufferPtr = position; 
        return _strlen(position); 
    }

    *bufferPtr = commandBuffer; 
    return bytesRead; 
}

/**
 * readBuffer - reads a buffer
 * @commandInfo: parameter structure
 * @buffer: buffer
 * @iterator: size
 *
 * Return: bytesRead
 */
ssize_t readBuffer(CommandInfo *commandInfo, char *buffer, size_t *iterator)
{
    ssize_t bytesRead = 0;

    if (*iterator)
        return 0;

    bytesRead = read(commandInfo->readFd, buffer, READ_BUF_SIZE);

    if (bytesRead >= 0)
        *iterator = bytesRead;

    return bytesRead;
}

/**
 * customGetline - gets the next line of input from STDIN
 * @commandInfo: parameter structure
 * @pointer: address of pointer to buffer, preallocated or NULL
 * @length: size of preallocated pointer buffer if not NULL
 *
 * Return: bytesRead
 */
int customGetline(CommandInfo *commandInfo, char **pointer, size_t *length)
{
    static char buffer[READ_BUF_SIZE];
    static size_t currentPosition, bufferLength;
    size_t k;
    ssize_t bytesRead = 0, totalBytesRead = 0;
    char *p = NULL, *newP = NULL, *c;

    p = *pointer;
    
    if (p && length)
        bytesRead = *length;

    if (currentPosition == bufferLength)
        currentPosition = bufferLength = 0;

    bytesRead = readBuffer(commandInfo, buffer, &bufferLength);

    if (bytesRead == -1 || (bytesRead == 0 && bufferLength == 0))
        return -1;

    c = customStrchr(buffer + currentPosition, '\n');
    k = c ? 1 + (unsigned int)(c - buffer) : bufferLength;
    
    newP = customRealloc(p, bytesRead, bytesRead ? bytesRead + k : k + 1);

    if (!newP) 
        return p ? (free(p), -1) : -1;

    if (bytesRead)
        customStrncat(newP, buffer + currentPosition, k - currentPosition);
    else
        customStrncpy(newP, buffer + currentPosition, k - currentPosition + 1);

    bytesRead += k - currentPosition;
    currentPosition = k;
    p = newP;

    if (length)
        *length = bytesRead;

    *pointer = p;
    return bytesRead;
}

/**
 * blockCtrlC - blocks ctrl-C
 * @signalNumber: the signal number
 *
 * Return: void
 */
void blockCtrlC(__attribute__((unused))int signalNumber)
{
    _puts("\n");
    _puts("$ ");
    _putchar(BUF_FLUSH);
}

