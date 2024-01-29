#include <stdlib.h>
#include <stdio.h>
#include <time.h>


int flipbitinbyteatindex(long int index, unsigned char* data)
{
    int bitIndex = rand() % 8;
    data[index] ^= (1 << bitIndex);
    return 0;
}

int flipnbits(int n, long int* indexs, unsigned char* data)
{
    for (int i = 0; i < n; i++)
    {
        flipbitinbyteatindex(indexs[i], data);
    }

    free(indexs);
    return 0;
}

long int* randombits(int numbits, long int finSize)
{
    long int* bits = (long int*)calloc(numbits, sizeof(long int));
    for (int i = 0; i < numbits; i++)
    {
        bits[i] = rand() % finSize;
    }
    return bits;
}

int scramblenbitsfromfile(int numbits, const char* filename)
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

    long int* bitsToFlip = randombits(numbits, finSize);
    // initialize rand
    // srand((unsigned int)time(NULL));
    flipnbits(numbits, bitsToFlip, finData);

    FILE* fout = fopen(filename, "wb");
    if (fout == NULL)
    {
        printf("file '%s' does not exist", filename);
        return -1;
    }
    fwrite(finData, 1, finSize, fout);
    
    fclose(fout);
    free(finData);

    return 0;
}