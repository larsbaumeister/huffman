//
// Created by lars on 23.01.19.
//

#include "file_writer.h"

void file_writer_write_char(FILE_WRITER *writer, unsigned char c)
{
    file_writer_write_bit(writer, (c & 128) > 0);
    file_writer_write_bit(writer, (c & 64) > 0);
    file_writer_write_bit(writer, (c & 32) > 0);
    file_writer_write_bit(writer, (c & 16) > 0);
    file_writer_write_bit(writer, (c & 8) > 0);
    file_writer_write_bit(writer, (c & 4) > 0);
    file_writer_write_bit(writer, (c & 2) > 0);
    file_writer_write_bit(writer, (c & 1) > 0);
}

void file_writer_write_bit(FILE_WRITER *writer, BIT b)
{
    if((writer->buffer_bit_pos / 8) >= OUT_BUFFER_SIZE)
    {
        fwrite(writer->buffer, sizeof(char), OUT_BUFFER_SIZE, writer->file);
        writer->buffer_bit_pos = 0;
    }

    unsigned int pos = writer->buffer_bit_pos / 8;
    unsigned int bit_pos = writer->buffer_bit_pos % 8;
    writer->buffer[pos] = PUT_BIT(writer->buffer[pos], b, bit_pos);

    writer->buffer_bit_pos++;
}

void file_writer_open(FILE_WRITER *writer, char *file_name)
{
    writer->file = fopen(file_name, "wb");
    if (writer->file == 0)
    {
        perror(file_name);
        exit(HF_EXIT_IO_ERROR);
    }
    writer->buffer_bit_pos = 0;
}

void file_writer_close(FILE_WRITER *writer)
{
    // fill the last byte with zeros
    while((writer->buffer_bit_pos % 8) != 0)
    {
        file_writer_write_bit(writer, ZERO);
    }

    unsigned int size = ((writer->buffer_bit_pos) / 8);
    fwrite(writer->buffer, sizeof(char), size, writer->file);

    fclose(writer->file);
}
