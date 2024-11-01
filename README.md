# Arithmetic-coder

## SUMMARY 
Simple arithmetic ecoder/decoder for text that uses
finite precision. Unfortunatley computers only 
have a limited amount of precision and binary is
not ideal for handeling decimals so instead of 
using floats to create a incredibly small range 
that we will use a value that will be our compressed 
code what we do instead is use ints and essentially pretend 
it is a float by doing rescaling 

## FILES

| H FILES | CPP FILES | BRIEF DESCRIPTION|
|-----------------|-----------------|-----------------|
| [Bitio.h](src/Bitio.h)    | [Bitio.cpp](src/Bitio.cpp)    | Handles reading/writing bits in the binar file |
| [Constants.h](src/Constants.h)    | N/A | Stores the constants like MAX value 1/4 MAX val etc... (eventually this will need to be dynammic based on types)    |
| [Decoder.h](src/Decoder.h)    | [Decoder.cpp](src/Decoder.cpp)    | Reads meta data from binary file and then decodes the binary file |
| [Encoder.h](src/Encoder.h)    | [Encoder.cpp](src/Encoder.cpp)    | Writes meta data (symbol counts and text size) to the binary file |
| [P.h](src/P.h)    | [P.cpp](src/P.cpp)    | A container that stores the upper and lower range for each character in the file (basically a specific type of std::pair) |
| [PairDic.h](src/PairDic.h)    | [PairDic.cpp](src/PairDic.cpp)    | A dictionary that stores the character as a key and the ranges (P) as the values. Also stores the text and the size of the text |
| N/A    | [main.cpp](src/main.cpp)    | Runs the program. Currently it's where you enter the name of the text file you want to compress |

## RESOURCES
1. [Mark Nelson Data compression with Arithmetic coding](https://marknelson.us/posts/2014/10/19/data-compression-with-arithmetic-coding.html)
2. [cbloom blog Arithmetic Coding and statistical coders](https://www.cbloom.com/algs/statisti.html#A5.0)


## IMPROVMENTS
1. Allow users to enter their precision and types (use templates for dynamic typing)
2. Add asserts that will guarantee:PERCISION >= CODE_VAL_BITS + FREQ_BITS && CODE_VAL_BITS >= (FREQ_BITS + 2) 
3. For larger files will need to actually handle max symbol sizes and max number of symbols
4. Take in command line arguements for file name, size, etc...


## HOW TO RUN
1. Create a text file and change the string inputFileName in main.cpp to reflect this file name and relative path to wherever you plan on compiling the code (will take command line arguments in the future)
2. Create a directory in the root of the project 
3. Go inside that directory
4. run the following command 'cmake .. && make && ./runme'
5. (optional) rename the bin file and relative path in main.cpp currently the default is binfile (will make this a command line argument in the future)


