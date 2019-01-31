//
// Created by lars on 23.01.19.
//

#include <stdlib.h>
#include "file_reader.h"

bool file_reader_has_next_char(FILE_READER *reader)
{
    return reader->file_size * 8 - reader->bits_read >= 8;
}

unsigned char file_reader_read_char(FILE_READER *reader)
{
    unsigned char c = 0;
    c = PUT_BIT(c, file_reader_read_bit(reader), 0);
    c = PUT_BIT(c, file_reader_read_bit(reader), 1);
    c = PUT_BIT(c, file_reader_read_bit(reader), 2);
    c = PUT_BIT(c, file_reader_read_bit(reader), 3);
    c = PUT_BIT(c, file_reader_read_bit(reader), 4);
    c = PUT_BIT(c, file_reader_read_bit(reader), 5);
    c = PUT_BIT(c, file_reader_read_bit(reader), 6);
    c = PUT_BIT(c, file_reader_read_bit(reader), 7);
    return c;
}


bool file_reader_has_next_bit(FILE_READER *reader)
{
    unsigned long file_size_in_bits = reader->file_size * 8;
    return file_size_in_bits >= reader->bits_read;
}

BIT file_reader_read_bit(FILE_READER *reader)
{

    unsigned int buffer_pos = reader->buffer_bit_pos / 8;
    if(buffer_pos >= reader->buffer_size)
    {
        // the buffer is full, replace it
        reader->buffer_size = (unsigned int) fread(reader->buffer, sizeof(char), IN_BUFFER_SIZE, reader->file);
        reader->buffer_bit_pos = 0;
        buffer_pos = 0;
    }

    unsigned int bit_pos = reader->buffer_bit_pos % 8;
    reader->buffer_bit_pos++;
    reader->bits_read++;

    BIT b = GET_BIT(reader->buffer[buffer_pos], bit_pos);
    return b;
}


void file_reader_open(FILE_READER *reader, char *file_name)
{
    reader->file = fopen(file_name, "rb");
    if (reader->file == 0)
    {
        perror(file_name);
        exit(HF_EXIT_IO_ERROR);
    }
    reader->buffer_bit_pos = 0;
    reader->bits_read = 0;

    fseek(reader->file, 0L, SEEK_END);
    reader->file_size = (unsigned long) ftell(reader->file);
    rewind(reader->file);

    reader->buffer_size = (unsigned int) fread(reader->buffer, sizeof(char), IN_BUFFER_SIZE, reader->file);
}

void file_reader_close(FILE_READER *reader)
{
    fclose(reader->file);
}

