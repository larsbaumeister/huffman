cmake_minimum_required(VERSION 3.12)
project(huffman C)

set(CMAKE_C_STANDARD 99)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -D_DEBUG")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")

set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3")

add_executable(huffman
                src/main.c
                src/huffman.c
                src/huffman.h
                src/file_reader.c
                src/file_reader.h
                src/file_writer.c
                src/file_writer.h
                src/bheap.h
                src/bheap.c
                src/btree.h
                src/btree.c
                src/btreenode.h
                src/btreenode.c
                src/common.h)
