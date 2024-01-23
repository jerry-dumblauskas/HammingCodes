# C implementation of Extended Hamming Codes

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

## helper.c

## m.sh