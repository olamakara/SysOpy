#ifndef ZAD1_LIBRARY_H
#define ZAD1_LIBRARY_H

typedef struct
{
    int array_size;
    int number_of_blocks;
    char** blocks_array;
} memoryBlocks;

memoryBlocks init(int size);
void count(memoryBlocks *memoryBlocks, const char *file);
char* show(memoryBlocks *memoryBlocks, int index);
void delete_index(memoryBlocks *memoryBlocks, int index);
void destroy(memoryBlocks *memoryBlocks);

#endif //ZAD1_LIBRARY_H
