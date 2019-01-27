//
// Created by lars on 23.01.19.
//

#ifndef HUFFMAN_COMMON_H
#define HUFFMAN_COMMON_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**Gets the value of the bit in the given position*/
#define GET_BIT(byte, pos)          ((BIT)((byte) >> (7 - (pos)) & 0x01))

/**Sets value of the bit in the given position*/
#define PUT_BIT(byte, bit, pos)     ((BIT)((bit == 0)\
                                        ? (~(1 << (7 - (pos))) & (byte))\
                                        : ((1 << (7 - (pos))) | (byte))))

/**The filename of the current file*/
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define ERROR_LOG(...)      fprintf(stderr, "[ERROR in file: %s line %d]   ", __FILENAME__, __LINE__);\
                            fprintf(stderr, __VA_ARGS__);fprintf(stderr, "\n");

#define XOR(ex1, ex2) ((ex1) != (ex2))

extern bool hf_debug;
#define DEBUG_LOG(...) { if(hf_debug) {printf(__VA_ARGS__); printf("\n"); } }

typedef struct
{
    unsigned char letter;
    unsigned int count;
}
FREQUENCY;

static inline int frequency_compare(FREQUENCY* a, FREQUENCY* b)
{
    return (int) ((long)a->count - (long)b->count);
}

static inline void frequency_print(FREQUENCY* freq)
{
    printf("%c [%d]", freq->letter, freq->count);
}

static inline void frequency_destroy(FREQUENCY* freq)
{
    free(freq);
}


typedef enum
{
    ZERO, ONE
}
BIT;

typedef enum {
    HF_EXIT_SUCCESS = 0,
    HF_EXIT_UNKNOWN_ERROR = 1,
    HF_EXIT_ARGUMENT_ERROR = 2,
    HF_EXIT_IO_ERROR = 3,
    HF_EXIT_COMPRESSION_ERROR = 4,
    HF_EXIT_MEMORY_ERROR = 5
} EXIT_CODE;


static inline void* hf_malloc(size_t size)
{
    void* ptr = malloc(size);
    if(ptr == NULL)
    {
        ERROR_LOG("Memory allocation failed");
        exit(HF_EXIT_MEMORY_ERROR);
    }
    return ptr;
}

static inline void* hf_realloc(void* ptr, size_t size)
{
    void* new_ptr = realloc(ptr, size);
    if(new_ptr == NULL)
    {
        ERROR_LOG("Memory allocation failed");
        exit(HF_EXIT_MEMORY_ERROR);
    }
    return new_ptr;
}

static inline void* hf_calloc(size_t type_size, size_t num_elems)
{
    void* new_ptr = calloc(type_size, num_elems);
    if(new_ptr == NULL)
    {
        ERROR_LOG("Memory allocation failed");
        exit(HF_EXIT_MEMORY_ERROR);
    }
    return new_ptr;
}

static inline bool string_starts_with(const char *string, const char *prefix)
{
    return strncmp(string, prefix, strlen(prefix)) == 0;
}

static inline long get_file_size(FILE* file)
{
    fseek(file, 0L, SEEK_END);
    long filesize = ftell(file);
    rewind(file);
    return filesize;
}

static inline long get_file_size_by_name(char* file_name)
{
    FILE* file = fopen(file_name, "rb");
    long size = get_file_size(file);
    fclose(file);
    return size;
}

#endif //HUFFMAN_COMMON_H
