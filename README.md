## Huffman encoding/decoding implementation in C

**This project was created for learning purposes and is not intended to be used in production.**

How to build:
1. clone this git repo
2. cd into the cloned repo
3. generate a makefile with `cmake -DCMAKE_BUILD_TYPE=Release .`
4. compile with `make`
5. you should now have the huffman executable in the project directory

How to use:
- to compress a file use `huffman -c <your-file>`
- to decompress a file use `huffman -d <your-file>`
- if you want to set the output file add `-o <your-out-file>` to the commands

