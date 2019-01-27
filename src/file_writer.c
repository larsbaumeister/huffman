//
// Created by lars on 23.01.19.
//

#include "file_writer.h"

void file_writer_write_char(FILE_WRITER *writer, unsigned char c)
{
    if(writer->buffer_pos >= OUT_BUFFER_SIZE)
    {
        fwrite(writer->buffer, sizeof(char), writer->buffer_pos, writer->file);
        writer->buffer_pos = 0;
        writer->bit_buffer_pos = 0;
    }
    writer->buffer[(writer->buffer_pos)++] = c;
}

void file_writer_write_bit(FILE_WRITER *writer, BIT b)
{
    if(writer->buffer_pos >= OUT_BUFFER_SIZE)
    {
        //write the full buffer to the file
        fwrite(writer->buffer, sizeof(char), writer->buffer_pos, writer->file);
        writer->buffer_pos = 0;
        writer->bit_buffer_pos = 0;
    }

    writer->buffer[writer->buffer_pos] = PUT_BIT(writer->buffer[writer->buffer_pos], b, (writer->bit_buffer_pos)++);

    if(writer->bit_buffer_pos == 8)
    {
        (writer->buffer_pos)++;
        writer->bit_buffer_pos = 0;
    }
}

void file_writer_write_int(FILE_WRITER *writer, unsigned int i)
{
    file_writer_write_char(writer, i >> (3 * 8) & 0xff);
    file_writer_write_char(writer, i >> (2 * 8) & 0xff);
    file_writer_write_char(writer, i >> (1 * 8) & 0xff);
    file_writer_write_char(writer, i & 0xff);
}

void file_writer_write_uint64(FILE_WRITER *writer, uint64_t l)
{
    file_writer_write_char(writer, l >> (7 * 8) & 0xff);
    file_writer_write_char(writer, l >> (6 * 8) & 0xff);
    file_writer_write_char(writer, l >> (5 * 8) & 0xff);
    file_writer_write_char(writer, l >> (4 * 8) & 0xff);
    file_writer_write_char(writer, l >> (3 * 8) & 0xff);
    file_writer_write_char(writer, l >> (2 * 8) & 0xff);
    file_writer_write_char(writer, l >> (1 * 8) & 0xff);
    file_writer_write_char(writer, l & 0xff);
}

void file_writer_open(FILE_WRITER *writer, char *file_name)
{
    writer->file = fopen(file_name, "wb");
    if (writer->file == 0)
    {
        perror(file_name);
        exit(HF_EXIT_IO_ERROR);
    }
    writer->buffer_pos = 0;
    writer->bit_buffer_pos = 0;
}

void file_writer_close(FILE_WRITER *writer)
{
    while(writer->bit_buffer_pos != 0)
    {
        file_writer_write_bit(writer, ZERO);
    }

    fwrite(writer->buffer, sizeof(char), writer->buffer_pos, writer->file);

    fclose(writer->file);
}
