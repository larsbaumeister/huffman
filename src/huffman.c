//
// Created by lars on 23.01.19.
//

#include <stdlib.h>
#include <stdbool.h>
#include "huffman.h"
#include "bheap.h"
#include "file_reader.h"
#include "common.h"
#include "btree.h"
#include "file_writer.h"

#define SIZE_OF_BYTE 256

int compare_tree(BTREE* t1, BTREE* t2)
{
    BTREE_NODE* n1 = btree_get_root(t1);
    BTREE_NODE* n2 = btree_get_root(t2);

    FREQUENCY* f1 = (FREQUENCY*)btreenode_get_data(n1);
    FREQUENCY* f2 = (FREQUENCY*)btreenode_get_data(n2);

    return frequency_compare(f1, f2);
}

void print_tree(BTREE* tree)
{
    BTREE_NODE* node = btree_get_root(tree);
    FREQUENCY* f = btreenode_get_data(node);
    if(f != NULL)
        frequency_print(f);
}

void fill_code_table(unsigned char code_table[SIZE_OF_BYTE][64], BTREE_NODE* node)
{
    static unsigned char cache[64];
    static unsigned int cache_idx;

    if(btreenode_is_leaf(node))
    {
        cache[cache_idx] = '\\';
        cache_idx++;

        FREQUENCY* freq = btreenode_get_data(node);
        memcpy(code_table[freq->letter], cache, cache_idx);
        cache_idx--;
    }
    else
    {
        BTREE_NODE* left_node = btreenode_get_left(node);
        BTREE_NODE* right_node = btreenode_get_right(node);

        cache[cache_idx] = 0;
        if(left_node != NULL)
        {
            cache_idx++;
            fill_code_table(code_table, left_node);
            cache_idx--;
        }

        cache[cache_idx] = 1;
        if(right_node != NULL)
        {
            cache_idx++;
            fill_code_table(code_table, right_node);
            cache_idx--;
        }
    }
}

unsigned char decode_next_char(BTREE_NODE* node, FILE_READER* reader)
{
    FREQUENCY* freq = btreenode_get_data(node);

    if(btreenode_is_leaf(node))
    {
        return freq->letter;
    }
    else
    {
        if(file_reader_has_next_bit(reader))
        {
            BIT b = file_reader_read_bit(reader);
            if(b == ONE)
            {
                return decode_next_char(btreenode_get_right(node), reader);
            }
            else
            {
                return decode_next_char(btreenode_get_left(node), reader);
            }
        }
        return 0;
    }
}

unsigned int calc_number_of_out_bits(BTREE_NODE* code_tree)
{
    unsigned int num_bits = 0;
    BTREE_NODE* left = btreenode_get_left(code_tree);
    if(left != NULL)
    {
        num_bits += calc_number_of_out_bits(left) + 1;
    }

    BTREE_NODE* right = btreenode_get_right(code_tree);
    if(right != NULL)
    {
        num_bits += calc_number_of_out_bits(right) + 1;
    }
    return num_bits;
}

void write_tree_structure(BTREE_NODE* node, FILE_WRITER* writer)
{
    if(btreenode_is_leaf(node))
    {
        FREQUENCY* freq = btreenode_get_data(node);
        file_writer_write_bit(writer, ONE);
        file_writer_write_char(writer, freq->letter);
    }
    else
    {
        BTREE_NODE* left = btreenode_get_left(node);
        BTREE_NODE* right = btreenode_get_right(node);

        file_writer_write_bit(writer, ZERO);
        write_tree_structure(left, writer);
        write_tree_structure(right, writer);
    }
}

BTREE_NODE* read_tree_structure(FILE_READER* reader)
{
    FREQUENCY* freq = hf_malloc(sizeof(FREQUENCY));
    BTREE_NODE* node = btreenode_new(freq);
    if(file_reader_read_bit(reader) == ONE)
    {
        btreenode_set_left(node, NULL);
        btreenode_set_right(node, NULL);
        freq->letter = file_reader_read_char(reader);
    }
    else
    {
        btreenode_set_left(node, read_tree_structure(reader));
        btreenode_set_right(node, read_tree_structure(reader));
        freq->letter = 0;
    }
    return node;
}

void encode(char *in_file, char* out_file)
{
    DEBUG_LOG("Encoding started")

    FILE_READER reader;
    file_reader_open(&reader, in_file);

    //count the occurrences of every character
    //the character is used as the index of the array, so we don't have to search
    unsigned int char_counts[SIZE_OF_BYTE] = {0};
    while(file_reader_has_next_char(&reader))
    {
        unsigned char c = file_reader_read_char(&reader);
        char_counts[c]++;
    }
    file_reader_close(&reader);


    DEBUG_LOG("Building the heap")
    BINARY_HEAP heap;
    heap_init(&heap, (HEAP_ELEM_COMP)compare_tree, (HEAP_ELEM_PRINT)print_tree);

    unsigned int number_of_letters = 0;
    //now convert this strange format to a array of frequencies
    for(unsigned int i = 0; i < SIZE_OF_BYTE; i++)
    {
        if(char_counts[i] > 0)
        {
            number_of_letters++;

            FREQUENCY* freq = hf_malloc(sizeof(FREQUENCY));
            freq->letter = (unsigned char) i;
            freq->count = char_counts[i];


            BTREE* tree = btree_new(freq, (DESTROY_DATA_FCT)frequency_destroy, (PRINT_DATA_FCT)frequency_print);
            heap_insert(&heap, tree);
        }
    }

    // build the huffman tree from the prev generated heap
    BTREE* left_tree = NULL;
    BTREE* right_tree = NULL;

    DEBUG_LOG("Building the code tree")
    //check if the heap is not empty, otherwise the program ends in an infinite loop
    if(heap_peek(&heap, (void**) &left_tree))
    {
        do
        {
            // extract two elements for the next iteration
            heap_extract_min(&heap, (void**) &left_tree);
            heap_extract_min(&heap, (void**) &right_tree);

            // if two elements were found, merge them together
            if(left_tree != NULL && right_tree != NULL)
            {
                FREQUENCY* left_freq = btreenode_get_data(btree_get_root(left_tree));
                FREQUENCY* right_freq = btreenode_get_data(btree_get_root(right_tree));

                FREQUENCY* merged_freq = hf_malloc(sizeof(FREQUENCY));
                merged_freq->letter = 0;
                merged_freq->count = left_freq->count + right_freq->count;
                BTREE* merged_tree = btree_merge(left_tree, right_tree, merged_freq);
                heap_insert(&heap, merged_tree);
            }
        }
        while(!XOR(left_tree == NULL, right_tree == NULL));
        // the head of the final tree is in left_tree after this loop

    }
    heap_destroy(&heap);

    //after merging the trees, the final tree is in left_tree
    BTREE* code_tree = left_tree;
    if(code_tree == NULL)
    {
        // special case, file is completely empty
        FILE_WRITER writer;
        file_writer_open(&writer, out_file);

        file_writer_write_char(&writer, 1); // bits used in last byte of the encoded data
        file_writer_write_bit(&writer, 1); // one tree node is required, even if there is no data
        file_writer_write_char(&writer, 0); // set the tree node value to something, doesn't matter what

        file_writer_close(&writer);
    }
    else
    {
        // normal case, file has some content
        BTREE_NODE* root = btree_get_root(code_tree);

        DEBUG_LOG("Building the code table")
        unsigned char code_table[SIZE_OF_BYTE][64];
        fill_code_table(code_table, root);


        FILE_WRITER writer;
        file_writer_open(&writer, out_file);

        // calculate the number of bits used in the output
        unsigned long num_bits = btree_number_of_elements(code_tree);
        num_bits += 8 * number_of_letters;
        for(unsigned int i = 0; i < SIZE_OF_BYTE; i++)
        {
            if(char_counts[i] > 0)
            {
                unsigned char* code = code_table[i];
                unsigned char code_length = 0;
                while(*code != '\\')
                {
                    code_length++;
                    code++;
                }
                num_bits += code_length * char_counts[i];
            }
        }

        DEBUG_LOG("Encoding the file")
        unsigned char bits_used_in_last_byte = (unsigned char) (num_bits % 8);
        // write the number of bits, used in the last char
        file_writer_write_char(&writer, bits_used_in_last_byte == 0 ? 8 : bits_used_in_last_byte);


        write_tree_structure(root, &writer);

        //start encoding the file
        file_reader_open(&reader, in_file);
        while(file_reader_has_next_char(&reader))
        {
            unsigned char c = file_reader_read_char(&reader);
            unsigned char* code = code_table[c];

            for(unsigned char* bit = code; *bit != '\\'; bit++)
            {
                file_writer_write_bit(&writer, *bit);
            }
        }
        file_writer_close(&writer);
        file_reader_close(&reader);

        btree_destroy(&code_tree, true);
    }
}

void decode(char *in_file, char *out_file)
{
    DEBUG_LOG("Decoding started")

    FILE_READER reader;
    file_reader_open(&reader, in_file);

    unsigned char number_of_remaining_bits = file_reader_read_char(&reader);

    DEBUG_LOG("Creating the code table")
    BTREE_NODE* root_node = read_tree_structure(&reader);

    DEBUG_LOG("Decoding the file")
    FILE_WRITER writer;
    file_writer_open(&writer, out_file);

    while(file_reader_has_next_char(&reader) || (reader.bits_read % 8 != 0 && ((reader.bits_read % 8)) < number_of_remaining_bits) )
    {
        unsigned char decoded_char = decode_next_char(root_node, &reader);
        file_writer_write_char(&writer, decoded_char);
    }

    file_writer_close(&writer);
    file_reader_close(&reader);

    btreenode_destroy(&root_node, (DESTROY_DATA_FCT) frequency_destroy);
}
