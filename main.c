#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "encoder.h"
#include "reader.h"
#include "fixer.h"
#include "scramble.h"

enum OPERATION {ENCODE = 1, READ = 2, FIX = 3, SCRAMBLE = 4, INVALID = -1};

int checkencode(int numArgs, char* args[])
{
    if (numArgs != 4)
    {
        return INVALID;
    }
    return ENCODE;
}

int checkread(int numArgs, char* args[])
{
    if (numArgs != 3)
    {
        return INVALID;
    }
    return READ;
}

int checkfix(int numArgs, char* args[])
{
    if (numArgs != 3)
    {
        return INVALID;
    }
    return FIX;
}

int checkscramble(int numArgs, char* args[])
{
    if (numArgs != 4)
    {
        return INVALID;
    }
    return SCRAMBLE;
}

int checkargs(int numArgs, char* args[])
{
    int op = atoi(args[1]);

    if (op == ENCODE)
    {
        return checkencode(numArgs, args);
    } else if (op == READ)
    {
        return checkread(numArgs, args);
    } else if (op == FIX)
    {
        return checkfix(numArgs, args);
    } else if (op == SCRAMBLE)
    {
        return checkscramble(numArgs, args);
    }

    return INVALID;
}

int main(int argc, char* argv[])
{
    int operationCode = checkargs(argc, argv);
    int exitCode = -1;

    if (operationCode == INVALID)
    {
        printf("Invalid instruction\n");
    } else if (operationCode == ENCODE)
    {
        exitCode = encodefilecontents(argv[2], argv[3]);
    } else if (operationCode == READ)
    {
        exitCode = readEHCfromfile(argv[2]);
    } else if (operationCode == FIX)
    {
        exitCode = readandfixfile(argv[2]);
    } else if (operationCode == SCRAMBLE)
    {
        exitCode = scramblenbitsfromfile(atoi(argv[2]), argv[3]);
    }

    printf("EXIT CODE: %i", exitCode);
    return exitCode;
}





