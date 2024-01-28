# C implementation of Extended Hamming Codes

This repo is a C implementation of Extended Hamming Codes. I was inspired to make this by the two part series by 3blue1brown as well as the chessboard puzzle videos by both 3blue1brown and Standup Maths.

## 3blue1brown part 1
<a href="http://www.youtube.com/watch?feature=player_embedded&v=X8jsijhllIA
" target="_blank"><img src="http://img.youtube.com/vi/X8jsijhllIA/0.jpg" 
alt="3blue1brown Hamming Codes part 1" width="240" height="180" border="3" /></a>

## 3blue1brown part 2
<a href="http://www.youtube.com/watch?feature=player_embedded&v=b3NxrZOu_CE
" target="_blank"><img src="http://img.youtube.com/vi/b3NxrZOu_CE/0.jpg" 
alt="3blue1brown Hamming Codes part 2" width="240" height="180" border="3" /></a>

## 3blue1brown Chessboard Puzzle
<a href="http://www.youtube.com/watch?feature=player_embedded&v=wTJI_WuZSwE
" target="_blank"><img src="http://img.youtube.com/vi/wTJI_WuZSwE/0.jpg" 
alt="3blue1brown Chessboard Puzzle" width="240" height="180" border="3" /></a>

## Standup Maths Chessboard Puzzle
<a href="http://www.youtube.com/watch?feature=player_embedded&v=as7Gkm7Y7h4
" target="_blank"><img src="http://img.youtube.com/vi/as7Gkm7Y7h4/0.jpg" 
alt="Standup Maths Chessboard Puzzle" width="240" height="180" border="3" /></a>

## encoder.c

#### Main Function: encodefilecontents(const char* filenamein, const char* filenameout);

This file's main function takes a file name as input and a file name as output.<br/>
It reads the input file and writes the extended hamming code into the output file through five main steps.

**Data Reading:**<br/>
Read each byte of a file into a char array and convert that to unsigned short int array.

**Align Bits:**<br/>
Each extended hamming code block has 16 bits. Of these 16 bits, 5 are reduntant, and 11 are significant.<br/>
The aligning of bits ensures that each unsigned short int (a 16 bit block) has only 11 bits of data.<br/>
(aligned to least significant bit)

**Map Significant Bits:**<br/>
Each significant bit is placed into the next position which isn't 0 or a power of two.

**Flip Parity Bits:**<br/>
Check the parity of each section of the block. Flip the corresponding bit if necessary.<br/>
Do a final check to flip the extended parity bit. (0th index)

**Write Encoded Data:**<br/>
Write the newly paratized data back into _filenameout_.

## reader.c

#### Main Function: int readEHCfromfile(const char* filenamein);

This files main function takes one filename as input.<br/>
It reads that file, assuming it is in EHC, and extracts the valuable bits before printing them.
This file runs through the following four main steps.

**Data Reading:**<br/>
Read each byte of a file into a char array and convert that to unsigned short int array.

**Significant Bit Extraction:**<br/>
Take every bit except those located at an index of 0, or power of two.

**Condense Data:**<br/>
Immediately upon being extracted, the data is not condensed very well.<br/>
The next step is to ensure all bits in the structure are significant by zipping them together.

**Print Plain Text:**<br/>
Convert unsigned int array back to char array and print to console.

## fixer.c

#### Main Function: int readEHCfromfile(const char* filenamein);

This files main function is to read a binary file in Extended Hamming Code, fix any detected errors, and then write the fixed block back into its position. This is achieved in the following five steps.

**Data Reading:**<br/>
Read each byte of a file into a char array and convert that to unsigned short int array.

**Detect Errors:**<br/>
Send each block into a function which uses an xor to return the location error if an error exists.<br/>
Add to a list of locations for all blocks.

**Fix Data:**<br/>
Send error data to a function which will flip the bit at the location of the error, if an error exists.

**Write Data:**<br/>
Convert unsigned int array back to char array and print to console.

## helper.c

__int ispoweroftwo(unsigned short int num);__<br/>
Return 1 if num is power of two, 0 if not.

__unsigned short int* chararrtointarr(unsigned char* arr, int numBytes);__<br/>
Given a char pointer and number of bytes, store the bit data into 16 bit unsigned int pointer.

__unsigned char* intarrtochararr(unsigned short int* arr, int numBytes);__<br/>
Given a 16 bit unsigned int pointer and the number of bytes, reassign to a char pointer.

## m.sh

```bash
gcc main.c encoder.c reader.c fixer.c helper.c -o app.exe -Wall
```

This is the gcc command to build the executable. It will be placed in the current directory with the name _app.exe_.