//
// Created by lars on 23.01.19.
//

#include <stdlib.h>
#include "file_reader.h"

bool file_reader_has_next_char(FILE_READER *reader)
{
    return reader->file_size > reader->bytes_read;
}

unsigned char file_reader_read_char(FILE_READER *reader)
{
    if(reader->buffer_pos >= reader->buffer_size)
    {
        unsigned int bytes_read = (unsigned int) fread(reader->buffer, sizeof(char), IN_BUFFER_SIZE, reader->file);
        reader->buffer_size = bytes_read;

        reader->bit_buffer_pos = 8;
        reader->buffer_pos = 0;
    }
    (reader->bytes_read)++;

    return reader->buffer[(reader->buffer_pos)++];
}

bool file_reader_has_next_bit(FILE_READER *reader)
{
    return file_reader_has_next_char(reader) || reader->bit_buffer_pos < 8;
}

BIT file_reader_read_bit(FILE_READER *reader)
{
    if(reader->bit_buffer_pos == 8)
    {
        reader->bit_buffer = file_reader_read_char(reader);
        reader->bit_buffer_pos = 0;
    }

    return GET_BIT(reader->bit_buffer, (reader->bit_buffer_pos)++);
}

unsigned int file_reader_read_int(FILE_READER *reader)
{
    unsigned int i = 0;

    if(file_reader_has_next_char(reader))
        i = file_reader_read_char(reader) << (3 * 8);
    if(file_reader_has_next_char(reader))
        i = i | file_reader_read_char(reader) << (2 * 8);
    if(file_reader_has_next_char(reader))
        i = i | file_reader_read_char(reader) << (1 * 8);
    if(file_reader_has_next_char(reader))
        i = i | file_reader_read_char(reader);

    return i;
}

uint64_t file_reader_read_uint64(FILE_READER* reader)
{
    uint64_t i = 0;

    if(file_reader_has_next_char(reader))
        i = i | (uint64_t) file_reader_read_char(reader) << (7* 8);
    if(file_reader_has_next_char(reader))
        i = i | (uint64_t) file_reader_read_char(reader) << (6 * 8);
    if(file_reader_has_next_char(reader))
        i = i | (uint64_t) file_reader_read_char(reader) << (5 * 8);
    if(file_reader_has_next_char(reader))
        i = i | (uint64_t) file_reader_read_char(reader) << (4 * 8);
    if(file_reader_has_next_char(reader))
        i = i | (uint64_t) file_reader_read_char(reader) << (3 * 8);
    if(file_reader_has_next_char(reader))
        i = i | (uint64_t) file_reader_read_char(reader) << (2 * 8);
    if(file_reader_has_next_char(reader))
        i = i | (uint64_t) file_reader_read_char(reader) << (1 * 8);
    if(file_reader_has_next_char(reader))
        i = i | file_reader_read_char(reader);

    return i;
}

void file_reader_open(FILE_READER *reader, char *file_name)
{
    reader->file = fopen(file_name, "rb");
    if (reader->file == 0)
    {
        perror(file_name);
        exit(HF_EXIT_IO_ERROR);
    }
    reader->bit_buffer_pos = 8;
    reader->buffer_pos = 0;
    reader->bytes_read = 0;

    fseek(reader->file, 0L, SEEK_END);
    reader->file_size = (unsigned long) ftell(reader->file);
    rewind(reader->file);

    reader->buffer_size = (unsigned int) fread(reader->buffer, sizeof(char), IN_BUFFER_SIZE, reader->file);
}

void file_reader_close(FILE_READER *reader)
{
    fclose(reader->file);
}

