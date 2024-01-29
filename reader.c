#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "helper.h"

/*
reads 16 extended hamming code block
stores data bits into an unsigned
short int (left aligned)
*/
unsigned short int readblock(unsigned short int block)
{
    unsigned short int data = 0;
    unsigned short int bitpos = 0;
    unsigned short int mask = 1 << 15;

    for (int i = 0; i < 11; i++)
    {
        if (ispoweroftwo(bitpos) || bitpos == 0)
        {
            bitpos++;
            i--;
        } else
        {
            // store bitposth bit into ith position
            data |= (((block & (mask >> bitpos)) << bitpos) >> i);
            bitpos++;
        }
    }
    return data;
}

unsigned short int* readblocks(unsigned short int* blocks, int numBlocks)
{
    unsigned short int* alignedBlocks = (unsigned short int*)calloc(numBlocks, sizeof(unsigned short int));

    for (int i = 0; i < numBlocks; i++)
    {
        alignedBlocks[i] = readblock(blocks[i]);
    }

    free(blocks);
    return alignedBlocks;
}

/*
condense data down to remove trailing 
unused space inside of the shorts
*/
unsigned short int* condenseblocks(unsigned short int* alignedBlocks, int numBlocks)
{
    // total blocks in zipped
    // state will be * 11/16 (rounded up)
    // Ceil(A/B) = (A + B - 1) / B
    int numZippedBlocks = ((numBlocks * 11) + 16 - 1) / 16;
    unsigned short int* zippedData = (unsigned short int*)calloc(numZippedBlocks, sizeof(unsigned short int));

    // mask for setting sig bit
    unsigned short int mask = 1 << 15;

    // write every 16 bits to an unsigned short
    // read every 11 bits from each block

    int blockNumber;
    int zipNumber;

    int inBlockIndex;
    int inZipIndex;

    // number of bits is 11
    // sig bits from each block
    int numBits = numBlocks * 11;

    for (int i = 0; i < numBits; i++)
    {
        blockNumber = i / 11;
        zipNumber = i / 16;

        inBlockIndex = i % 11;
        inZipIndex = i % 16;

        // get block bit
        // store in zip bit
        zippedData[zipNumber] |= (((alignedBlocks[blockNumber] & (mask >> inBlockIndex)) << inBlockIndex) >> inZipIndex);

    }

    free(alignedBlocks);
    return zippedData;
    
}

unsigned short int* readEHC(unsigned short int* blocks, int numBlocks)
{

    unsigned short int* rawdata = readblocks(blocks, numBlocks);
    unsigned short int* treatedData = condenseblocks(rawdata, numBlocks);
    return treatedData;
}

/*
read a bin file
decode the EHC
print to console
*/
int readEHCfromfile(const char* filenamein)
{
    FILE* fin = fopen(filenamein, "rb");
    if (fin == NULL)
    {
        printf("file '%s' does not exist", filenamein);
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

    int numBytes = ((((finSize + 2 - 1) / 2) * 11) + 8 - 1) / 8;
    unsigned short int* blocks = chararrtointarr(finData, finSize);
    unsigned short int* data = readEHC(blocks, (finSize + 2 - 1) / 2);
    unsigned char* text = intarrtochararr(data, numBytes);


    for (int i = 0; i < numBytes; i++)
    {
        printf("%c", text[i]);
    }
    printf("\n");
    free(text);
    return 0;
}

/*
int main()
{
    int res = readEHCfromfile("hello.bin");
    return res;
}
*/

