#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "helper.h"

/*
Takes a short with 11 bits (aligned to least significant bit)
returns with bits in data portion of
a 16 bit extended hamming code
*/
unsigned short int rearrange(unsigned short int elevenbit)
{
    unsigned short int mask = 1;
    unsigned short int ehcPositionOffset = 15;
    unsigned short int organizedbits = 0;

    for (int i = 0; i < 11; i++)
    {
        if (ispoweroftwo(ehcPositionOffset) || ehcPositionOffset == 0)
        {
            ehcPositionOffset--;
            i--;
        } else
        {
            organizedbits |= (((elevenbit & (mask << i)) >> i) << (15 - ehcPositionOffset));
            ehcPositionOffset--;
        }
    }
    
    return organizedbits;
}

/*
takes in a power of 2 int that tells
program which parity group to look at
in the unparatized data.
returns 0 or 1, the value to set the parity bit
*/
unsigned short int paritycheck(unsigned short int group, unsigned short int unparatized)
{
    unsigned short int assignbit = 0;
    unsigned short int mask = 1 << 15;

    /*
    scan from position 1 to 15. flip assignbit every time
    a '1' is detected in the specified group
    */
    for (int i = 1; i < 16; i++)
    {
        if ((((mask >> i) & unparatized) != 0) && ((i & group) != 0))
        {
            assignbit ^= 1;
        }
    }
    return assignbit;

}

/*
take in a hamming code, and return
0 or 1 to set first bit, making it
an extended hamming code
*/
unsigned short int extendedbitcheck(unsigned short int unextended)
{
    unsigned short int assignbit = 0;
    unsigned short int mask = 1 << 15;
    for (int i = 1; i < 16; i++)
    {
        if (((mask >> i) & unextended) != 0)
        {
            assignbit ^= 1;
        }
    }
    return assignbit;
}

/*
Takes in 16 bits without parity bits correctly set
sets parity bits according to significant data and
returns the resulting final EHC block
*/
unsigned short int flipparitybits(unsigned short int unparatized)
{
    unsigned short int numchecks = 4;
    unsigned short int paratized = unparatized;

    for (int i = 0; i < numchecks; i++)
    {
        unsigned short int bitval = paritycheck(1 << i, unparatized);
        paratized |= ((bitval << 15) >> (1 << i));
    }

    unsigned short int bitval = extendedbitcheck(paratized);
    paratized |= bitval << 15;

    return paratized;
}


/*
Takes in 11 sig bits (aligned right) and 
returns 16 bits in extended hamming code.
*/
unsigned short int encode(unsigned short int data)
{
    unsigned short int unparatized = rearrange(data);
    unsigned short int paratized = flipparitybits(unparatized);
    return paratized;
}

unsigned short int* encodemultiple(unsigned short int* data, int numElements)
{
    unsigned short int* encodedData = calloc(numElements, sizeof(unsigned short int));

    for (int i = 0; i < numElements; i++)
    {
        encodedData[i] = encode(data[i]);
    }

    free(data);
    return encodedData;
}

unsigned short int* uncondensedata(unsigned char* condensedData, int numBlocks)
{
    
    unsigned short int* alignedData = (unsigned short int*)calloc(numBlocks, sizeof(unsigned short int*));

    int charArrayIndex;
    int unzipIntIndex;

    int charBitIndex;
    int unzipIntBitIndex;

    unsigned short int mask = 1 << 7;
    unsigned short int alignOffset = 5;

    // read every 8 bits of finData (char)
    // write every 11 bits into alignedData (unsigned short)
    for (int i = 0; i < numBlocks * 16; i++)
    {
        charArrayIndex = i / 8;
        unzipIntIndex = i / 11;

        charBitIndex = i % 8;
        unzipIntBitIndex = (i % 11) + alignOffset;

        // must be aligned right
        unsigned short int setMask = (((condensedData[charArrayIndex] & (mask >> charBitIndex)) << (charBitIndex + 8)) >> unzipIntBitIndex);
        alignedData[unzipIntIndex] |= setMask;
    }

    free(condensedData);
    return alignedData;

}

/*
open text file.
encode bits
write to binary file
*/
int encodefilecontents(const char* filenamein, const char* filenameout)
{
    FILE* fin = fopen(filenamein, "r");
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

    // convert number of bytes into number of uint16_t (rounded up)
    int numShorts = (finSize + 2 - 1) / 2;
    // multiply number of shorts by 16/11 (rounded up)
    int numBlocks = (numShorts * 16 + 11 - 1) / 11;

    unsigned short int* alignedData = uncondensedata(finData, numBlocks);
    unsigned short int* encoded = encodemultiple(alignedData, numBlocks);
    unsigned char* chararr = intarrtochararr(encoded, numBlocks * 2);

    FILE* fout = fopen(filenameout, "wb");
    fwrite(chararr, sizeof(unsigned char), numBlocks * 2, fout);
    
    fclose(fout);
    free(encoded);
    return 0;
}

/*
int main()
{
    const char* fin = "test.txt";
    const char* fout = "hello.bin";
    int res = encodefilecontents(fin, fout);
    printf("%i", res);
    return 0;
}
*/
