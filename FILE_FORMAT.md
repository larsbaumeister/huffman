## File format

The file format for the encoded files is the following:

|             | 1st - 4th Byte                          | 5th - nth Byte                                        | nth + 1 Byte - file end                 |
|-------------|-----------------------------------------|-------------------------------------------------------|-----------------------------------------|
| Data type   | unsigned int                      | Char followed by an unsigned int                | encoded binary data                     |
| Description | The number of following char int blocks | The frequency for each character in the original file | The encoded data using the huffman tree |