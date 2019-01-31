//
// Created by lars on 23.01.19.
//

#ifndef HUFFMAN_FILE_READER_H
#define HUFFMAN_FILE_READER_H

#include <stdbool.h>
#include <stdio.h>
#include "common.h"

#define IN_BUFFER_SIZE 16384

typedef struct
{
    FILE* file;

    unsigned long bits_read;
    unsigned long file_size;
    unsigned int buffer_size;

    unsigned int buffer_bit_pos;
    unsigned char buffer[IN_BUFFER_SIZE];

}
FILE_READER;

/**
 * @return Returns true if <code>read_char()</code> can be called one more time
 */
extern bool file_reader_has_next_char(FILE_READER* reader);

/**
 * Reads a char from <code>in_buffer</code>
 * @return the read char
 */
extern unsigned char file_reader_read_char(FILE_READER* reader);

/**
 * @return Returns if <code>read_bit()</code> can be called one more time
 */
extern bool file_reader_has_next_bit(FILE_READER* reader);

/**
 * Reads a bit from <code>in_buffer</code>
 * @return the read bit
 */
extern BIT file_reader_read_bit(FILE_READER* reader);

/**
 * opens a file reader
 * @param reader
 * @param file_name
 */
extern void file_reader_open(FILE_READER* reader, char* file_name);

/**
 * clases a file reader
 * @param reader
 */
extern void file_reader_close(FILE_READER* reader);


#endif //HUFFMAN_FILE_READER_H
