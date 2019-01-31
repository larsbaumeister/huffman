//
// Created by lars on 23.01.19.
//

#ifndef HUFFMAN_FILE_WRITER_H
#define HUFFMAN_FILE_WRITER_H

#include "common.h"
#include <stdbool.h>

#define OUT_BUFFER_SIZE 16384

typedef struct
{
    unsigned char buffer[OUT_BUFFER_SIZE];
    unsigned int buffer_bit_pos;
    FILE* file;
}
FILE_WRITER;

/**
 * Appends a char to <code>out_buffer</code>
 * @param c the char to append
 */
extern void file_writer_write_char(FILE_WRITER* writer, unsigned char c);

/**
 * Appends a bit to <code>out_buffer</code>
 * @param c the bit to append
 */
extern void file_writer_write_bit(FILE_WRITER* writer, BIT b);


/**
 * Opens a file in write mode.<br>
 * Needs to be called before any of the write methods
 * @param writer
 * @param file_name
 */
extern void file_writer_open(FILE_WRITER* writer, char *file_name);

/**
 * Closes an open file and writes missing buffers to the file<br>
 * Should be called when the writing is finished.
 * @param writer
 */
extern void file_writer_close(FILE_WRITER* writer);

#endif //HUFFMAN_FILE_WRITER_H
