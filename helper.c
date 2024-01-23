#include <stdlib.h>

int ispoweroftwo(unsigned short int num)
{
    return (num != 0) && (num & (num - 1)) == 0;
}

unsigned short int* chararrtointarr(unsigned char* arr, int numBytes)
{
    unsigned short int* intarr = (unsigned short int*)calloc((numBytes + 2 - 1) / 2, sizeof(short));
    unsigned short int sFlag = 0;
    unsigned short int temp;

    for (int i = 0; i < numBytes; i++)
    {
        temp = arr[i];
        temp = temp << (8 * sFlag);
        intarr[i / 2] |= temp;
        sFlag ^= 1;   
    }

    free(arr);
    return intarr;
}

unsigned char* intarrtochararr(unsigned short int* arr, int numBytes)
{
    unsigned char* chararr = (unsigned char*)calloc(numBytes, sizeof(short));
    unsigned short int sFlag = 1;
    unsigned short int temp;

    for (int i = 0; i < numBytes; i++)
    {
        temp = arr[i / 2];
        temp = temp >> (8 * sFlag);
        chararr[i] |= temp;
        sFlag ^= 1;   
    }

    free(arr);
    return chararr;

}