#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include "huffman.h"
#include "common.h"
#include "file_writer.h"
#include "file_reader.h"
#include <unistd.h>
#include <time.h>

bool hf_debug;

void parse_arguments(int argc, char** argv, bool* compress, bool* decompress, bool* verbose, bool* help, int* level, char** outfile, char**infile)
{
    bool infile_was_set = false;
    for (int i = 1; i < argc; i++)
    {
        char* argument = argv[i];

        if(strcmp(argument, "-d") == 0)
            *decompress = true;
        else if(strcmp(argument, "-c") == 0)
            *compress = true;
        else if(strcmp(argument, "-v") == 0)
            *verbose = true;
        else if(strcmp(argument, "-h") == 0)
            *help = true;
        else if(string_starts_with(argument, "-o"))
            *outfile = argv[++i];
        else if(string_starts_with(argument, "-l"))
        {
            if(strlen(argument) > 2)
            {
                *level = atoi(argument + 2);
                if(*level < 1 || *level > 7)
                {
                    ERROR_LOG("level has to be in range of 1 to 7\n");
                    exit(HF_EXIT_ARGUMENT_ERROR);
                }
            }
            else
            {
                ERROR_LOG("level argument was not set\n");
                exit(HF_EXIT_ARGUMENT_ERROR);
            }
        }
        else {
            if(!infile_was_set) {
                *infile = argument;
                infile_was_set = true;
            }
            else
            {
                ERROR_LOG("invalid arguments\n");
                exit(HF_EXIT_ARGUMENT_ERROR);
            }
        }
    }

}

void print_help(void)
{
    printf("Usage:\n"
           "huffman [options] <input file>\n"
           "Options:\n"
           "-h\tshow help page\n"
           "-l<level>\tcompression level [1 - 12] default is 2\n"
           "-v\tverbose mode\n"
           "-c\tcompress\n"
           "-d\tdecompress\n"
           "-o<outfile>\tout file\n");
}

int main(int argc, char** argv)
{
    hf_debug = false;
    bool do_compression = false;
    bool do_decompression = false;
    bool show_help = false;
    int level = 2;
    char* outfile = NULL;
    char* infile = NULL;

    parse_arguments(argc, argv, &do_compression, &do_decompression, &hf_debug, &show_help, &level, &outfile, &infile);

    if(do_compression == do_decompression)
    {
        print_help();
        return HF_EXIT_ARGUMENT_ERROR;
    }

    if(show_help || infile == NULL)
    {
        print_help();
        return HF_EXIT_ARGUMENT_ERROR;
    }

    char* default_outfile = NULL;
    if(outfile == NULL)
    {
        char* extension = do_compression ? ".hc" : ".hd";
        int infile_length = strlen(infile);
        default_outfile = hf_malloc(infile_length + 3);

        memcpy(default_outfile, infile, infile_length);
        memcpy(default_outfile + infile_length, extension, 4);
        outfile = default_outfile;
    }

    // check if in and out file are not the same
    if(strcmp(infile, outfile) == 0) {
        fprintf(stderr, "infile and outfile are the same");
        exit(HF_EXIT_ARGUMENT_ERROR);
    }

    if(do_compression)
    {
        encode(infile, outfile);
        printf("encoding finished\n");
    }
    else if(do_decompression)
    {
        decode(infile, outfile);
        printf("decoding finished\n");
    }

    if (default_outfile != NULL)
        free(outfile);

    return HF_EXIT_SUCCESS;
}

