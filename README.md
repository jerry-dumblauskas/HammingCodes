# C implementation of Extended Hamming Codes
This repo is a C implementation of Extended Hamming Codes. I was inspired to make this by the two part series by 3blue1brown as well as the chessboard puzzle videos by both 3blue1brown and Standup Maths (videos linked at bottom).

## Using The Executable
The compiled executable can be run in the command line and has four modes.<br/>
The first command line argument is a single integer that determines the mode.<br/>
Each mode then accepts the following appropriate command line arguments.<br/>
_ENCODE = 1;<br/> READ = 2;<br/> FIX = 3;<br/> SCRAMBLE = 4;_<br/>

#### Encode
```bash
./app.exe 1 filenamein filenameout
```
Reads binary data in ```filenamein``` and writes encoded data to ```filenameout```.

#### Read
```bash
./app.exe 2 filenamein
```
Reads encoded data in ```filenamein``` and prints decoded value to console.

#### Fix
```bash
./app.exe 3 fixfilename
```
Reads encoded data in ```fixfilename```, prints each block's status, fixes fixable blocks, and writes corrected data back into ```fixfilename```

#### Scramble
```bash
./app.exe 4 numbitstoscramble scramblefilename
```
Reads encoded data in ```scramblefilename```, flips ```numbitstoscramble``` random bits, and writes scrambled data back into ```scramblefilename```.

## Decision Making Insights
#### Bit Implementation
If you read through the code, you'll quickly realize that almost all of the operations use unsigned integers to store bit data. Frankly, it's makes the code a decent bit more complicated. I could have just stored the data as integer arrays, but doing that felt like it would defeat the purpose of Hamming Codes. Hamming Codes are a way make error correction possible with less redundancy. Using integers would result in 16 to 32 times extra space being used (short int and int compared to individual bits). So, while not necessary, I felt it appropriate to at least slightly optimize for space since that's the purpose of the codes anyway.

#### 16 bit blocks
Hamming Codes can be implemented with any block size that is a power of two. The bigger the block size, the smaller percentage of the block is needed for reduntancy (number of reduntant bits = log<sub>2</sub>(block_size) + 1). However, a bigger block also means the Hamming Code can correct less data, as it can only identify and fix one error per block. I chose a 16 bit block for one main reason; I wanted relatively small blocks so I could test the functions easier. I didn't want to have to solve a 32, 64, or 256 bit Hamming Code by hand anytime something went wrong in my algorithm. I had actually intended to start it small and then make the program able to accept a variable block size as an input parameter, but due to an issue I ran into later, I didn't end up implementing that.

#### Why C?
Because I wanted to. I like the idea of low level languages, and this seemed like a good (semi-simple) project to help me solidify my foundations and get some hands-on experience. Especially regarding pointers, which I understood conceptually but haven't had a whole lot of practice implementing. 

## Learnings
#### Reading/Writing Binary Files
I may be silly for this, but I thought if you made a binary file and typed 1s and 0s, it would encode that binary value. Imagine my surprise when figuring out what had gone wrong, when looking at '10101010' was not one byte (eight bits), but eight bytes. As far as I'm currently aware, there is no way to 'write' binary with a keyboard. So, many many a minute were spent figuring out that I had to write binary files with code.

#### Pointers
I've _learned_ about pointers in C before, but, to be honest, I've never been super strong with them. I generally understood that a pointer is a memory address, and to get or store the value at pointer it needs to be dereferenced; However, I haven't implemented something that needed anything beyond extremely basic pointers to integers and such. After this project I have a much better understanding of how pointers work and how to actually implement their uses.

#### Hamming Codes
This is the obvious goal of this project. I wanted to be sure that I truly understood how Hamming Codes worked. I also saw the "one-line implementation" from the second 3blue1brown video, and I was a bit disatisfied. Yes, it was a one-line implementation of the core concept of Hamming Codes, and it did a great job of explaining and demonstrating the concept, but I was left wanting more as far as an "implementation" goes. I wanted to see how encoding, decoding, and fixing such data in a more programmatic way would work. I accomplished this, at least in my eyes, because I'm able to see, through my program, how this can be used practically, well, as practically as encoding and decoding text files can be. I'm confident in saying I now have a much better grasp over the fundamental theory of Hamming Codes after working on it myself.

#### Data Types and Allocating Memory
My initial struggle with this project was figuring out how I was going to store each block. I initially wanted to make a custom type, which could accept some size variable, similar to short vs long integers. This would be part of the process for implementing variable sized blocks mentioned above. The main issue with this is the lack of granularity of control over how I can request memory in C. I'm storing values in bits, so I'd like to be able to request some arbitrary number of bits. That, as far as I know, isn't really possible. The functions for memory allocation in C can only allocate in byte sized chunks. At the time, I ended up simply implementing 16 bit blocks due to convenience. 

By the time I did more research and found out about how the allocation worked, I'd already locked in a decent bit of implementation on 16 bit unsigned short integers. If there's one thing I'd like to change/improve in this project it's adding the variable block size feature. I think it could be accomplished using character arrays. The only issue, and my original reason for not implementing, is that I couldn't find a succinct way to allocate arbitrary sizes of memory in specifically block sized chunks. That was very confusing wording, but I can't think of a better way to phrase it so I'll use an example. 

My new proposed solution would use character arrays. If I used 16 bit Extended Hamming Code blocks, I would need ```(NumberTotalBits / 8)``` characters, and every two characters would constitute one block. If I then wanted 128 bit Extended Hamming Code blocks, I would still need ```(NumberTotalBits / 8)```, but every 16 characters would constitute one block. What I would _like_ is a way to have some type which would take up the entire size of the block, so instead of having to do some extra logic ensuring that every ```(blockSizeInBits / 8)``` characters constitutes one block, I could just reliably know that each time I go to the next element I'm accessing a new block.

## File Details
### **encoder.c**
#### Main Function: encodefilecontents(const char* filenamein, const char* filenameout);
This file's main function takes a file name as input and a file name as output.<br/>
It reads the input file and writes the extended hamming code into the output file through five main steps.

#### Data Reading:
Read each byte of a file into a char array and convert that to unsigned short int array.

#### Align Bits:
Each extended hamming code block has 16 bits. Of these 16 bits, 5 are reduntant, and 11 are significant.<br/>
The aligning of bits ensures that each unsigned short int (a 16 bit block) has only 11 bits of data.<br/>
(aligned to least significant bit)

#### Map Significant Bits:
Each significant bit is placed into the next position which isn't 0 or a power of two.

#### Flip Parity Bits:
Check the parity of each section of the block. Flip the corresponding bit if necessary.<br/>
Do a final check to flip the extended parity bit. (0th index)

#### Write Encoded Data:
Write the newly paratized data back into _filenameout_.

### **reader.c**
#### Main Function: int readEHCfromfile(const char* filenamein);
This files main function takes one filename as input.<br/>
It reads that file, assuming it is in EHC, and extracts the valuable bits before printing them.
This file runs through the following four main steps.

#### Data Reading:
Read each byte of a file into a char array and convert that to unsigned short int array.

#### Significant Bit Extraction:
Take every bit except those located at an index of 0, or power of two.

#### Condense Data:
Immediately upon being extracted, the data is not condensed very well.<br/>
The next step is to ensure all bits in the structure are significant by zipping them together.

#### Print Plain Text:
Convert unsigned int array back to char array and print to console.

### **fixer.c**
#### Main Function: int readEHCfromfile(const char* filenamein);
This file's main function is to read a binary file in Extended Hamming Code, fix any detected errors, and then write the fixed block back into its position. This is achieved in the following five steps.

#### Data Reading:
Read each byte of a file into a char array and convert that to unsigned short int array.

#### Detect Errors:
Send each block into a function which uses an xor to return the location error if an error exists.<br/>
Add to a list of locations for all blocks.

#### Fix Data:
Send error data to a function which will flip the bit at the location of the error, if an error exists.

#### Write Data:
Convert unsigned int array back to char array and print to console.

### **scramble.c**
#### Main Function: int scramblenbitsfromfile(int numbits, const char* filenamein);
This file's main function reads a binary file, flips a number of random bits,<br/>
and rewrites the results to the same file.

#### Data Reading:
Read each byte of a file into a char array and convert that to unsigned short int array.

#### Pick Random Bits:
Do the following numbits times.<br/>
Generate a random number i from 0 to fileSize - 1.<br/>
Generate a random number k from 0 to 7.<br/>

#### Flip bits
Flip the kth bit of the ith byte from the file data using xor bit operations.

#### Write Data:
Convert unsigned int array back to char array and print to console.

### **helper.c**
#### int ispoweroftwo(unsigned short int num);
Return 1 if num is power of two, 0 if not.

#### unsigned short int* chararrtointarr(unsigned char* arr, int numBytes);
Given a char pointer and number of bytes, store the bit data into 16 bit unsigned int pointer.

#### unsigned char* intarrtochararr(unsigned short int* arr, int numBytes);
Given a 16 bit unsigned int pointer and the number of bytes, reassign to a char pointer.

### **m.sh**
```bash
gcc main.c encoder.c reader.c fixer.c scramble.c helper.c -o app.exe -Wall
```

This is the gcc command to build the executable. It will be placed in the current directory with the name _app.exe_. If you run ```./m.sh``` it will run the above command for you. It _should_ compile for whatever machine you're running it on.

## 3blue1brown Hamming Codes parts 1 & 2
<a href="http://www.youtube.com/watch?feature=player_embedded&v=X8jsijhllIA
" target="_blank"><img src="http://img.youtube.com/vi/X8jsijhllIA/0.jpg" 
alt="3blue1brown Hamming Codes part 1" width="160" height="120" border="3" /></a>
<a href="http://www.youtube.com/watch?feature=player_embedded&v=b3NxrZOu_CE
" target="_blank"><img src="http://img.youtube.com/vi/b3NxrZOu_CE/0.jpg" 
alt="3blue1brown Hamming Codes part 2" width="160" height="120" border="3" /></a>

## 3blue1brown & Standup Maths Chessboard Puzzle
<a href="http://www.youtube.com/watch?feature=player_embedded&v=wTJI_WuZSwE
" target="_blank"><img src="http://img.youtube.com/vi/wTJI_WuZSwE/0.jpg" 
alt="3blue1brown Chessboard Puzzle" width="160" height="120" border="3" /></a>
<a href="http://www.youtube.com/watch?feature=player_embedded&v=as7Gkm7Y7h4
" target="_blank"><img src="http://img.youtube.com/vi/as7Gkm7Y7h4/0.jpg" 
alt="Standup Maths Chessboard Puzzle" width="160" height="120" border="3" /></a>