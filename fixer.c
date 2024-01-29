#include <stdio.h>
#include <stdlib.h>
#include "helper.h"

/*
data: bit data in the form of an int
*/
unsigned short int finderr(unsigned short int data)
{
    unsigned short int errorbit = 0;
    unsigned short int mask = 1;
    int numactivebits = 0;
    for (int i = 0; i < 16; i++)
    {
        // printf("bit %i: %i\n", i + 1, (data >> i & mask));
        if (((data >> i) & mask) == 1)
        {
            numactivebits += 1;
            errorbit ^= 15 - i;
        }
    }
    if (errorbit != 0 && numactivebits % 2 == 0)
    {
        return 16;
    }
    
    return errorbit;
}

int fixdata(int data, int location)
{
    int corrected = data ^ (1 << (15 - location));
    return corrected;
}

unsigned short int* errorvalarr(unsigned short int* blocks, int numBlocks)
{
    unsigned short int* errorValues = calloc(numBlocks, sizeof(short int));

    for (int i = 0; i < numBlocks; i++)
    {
        errorValues[i] = finderr(blocks[i]);
    }

    return errorValues;
}

unsigned short int* fixmultiplevals(unsigned short int* blocks, unsigned short int* errorvals, int numBlocks)
{
    unsigned short int* correctedData = calloc(numBlocks, sizeof(unsigned short int));

    for (int i = 0; i < numBlocks; i++)
    {
        if (errorvals[i] == 16)
        {
            correctedData[i] = blocks[i];
            printf("Block %i unrecoverable\n", i);
        } else if (errorvals[i] == 0)
        {
            correctedData[i] = blocks[i];
            printf("Block %i no errors\n", i);
        } else
        {
            correctedData[i] = fixdata(blocks[i], errorvals[i]);
            printf("Block %i corrected\n", i);
        }
    }

    free(blocks);
    free(errorvals);
    return correctedData;
}

/*
read bin file
fix data
write bin file
*/
int readandfixfile(const char* filename)
{
    FILE* fin = fopen(filename, "rb");
    if (fin == NULL)
    {
        printf("file '%s' does not exist", filename);
        return -1;
    }

    // get size of input file
    fseek(fin, 0L, SEEK_END);
    long int finSize = ftell(fin);
    // return to file beginning
    rewind(fin);
    // read contents into correctly sized short array
    unsigned char* finData = (unsigned char*)calloc(finSize, sizeof(char));
    for (int i = 0; i < finSize; i++)
    {
        finData[i] = fgetc(fin);
    }
    fclose(fin);

    unsigned short int numBlocks = (finSize + 2 - 1) / 2;
    unsigned short int* intData = chararrtointarr(finData, finSize);
    unsigned short int* errorvalues = errorvalarr(intData, numBlocks);
    unsigned short int* fixedData = fixmultiplevals(intData, errorvalues, numBlocks);
    unsigned char* writedata = intarrtochararr(fixedData, finSize);

    
    FILE* fout = fopen(filename, "wb");
    if (fout == NULL)
    {
        printf("file '%s' does not exist", filename);
        return -1;
    }
    fwrite(writedata, 1, finSize, fout);
    
    fclose(fout);
    
    free(writedata);
    return 0;

}

/*
int main()
{
    int res = readandfixfile("hello.bin");
    printf("END: %i", res);
    return res;
}
*/
