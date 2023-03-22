#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "library.h"

memoryBlocks init(int size)
{
    char** blocks_array = calloc(size, sizeof(char *));
    memoryBlocks mb = {size, 0, blocks_array};
    return mb;
}

void count(memoryBlocks *memoryBlocks, const char *file)
{
    char* string1 = "touch /tmp/tmp.txt | wc ";
    char* string2 = " > /tmp/tmp.txt";
    char* command = (char *) malloc(strlen(string1) + strlen(file) + strlen(string2) + 1);
    strcpy(command, string1);
    strcat(command, file);
    strcat(command, string2);
    system(command);

    char *buffer = 0;
    long length;
    FILE *file_pointer;
    if ((file_pointer = fopen("/tmp/tmp.txt", "r")) == NULL)  // check this
    {
        printf("Error! File does not exist \n");
        return;
    }

    if (file_pointer)
    {
        fseek(file_pointer, 0, SEEK_END);
        length = ftell(file_pointer);
        fseek(file_pointer, 0, SEEK_SET);
        buffer = malloc(length);
        if (buffer)
        {
            fread(buffer, 1, length, file_pointer);
        }
        fclose(file_pointer);
    }

    if (buffer)
    {
        if (memoryBlocks->number_of_blocks >= memoryBlocks->array_size)
        {
            printf("Error! There is no place left in array \n");
            return;
        }
        memoryBlocks->blocks_array[memoryBlocks->number_of_blocks] = (char*) calloc(length, sizeof(char));
        strcpy(memoryBlocks->blocks_array[memoryBlocks->number_of_blocks], buffer);

        memoryBlocks->number_of_blocks++;

        system("rm /tmp/tmp.txt");
    } else
    {
        printf("Error! Did not read from file \n");
        return;
    }
}

char* show(memoryBlocks *memoryBlocks, int index)
{
    if (index >= memoryBlocks->number_of_blocks) {
        return "This index is empty \n";
    }
    return memoryBlocks->blocks_array[index];
}

void delete_index(memoryBlocks *memoryBlocks, int index)
{
    int number_of_blocks = memoryBlocks->number_of_blocks;
    if (index >= number_of_blocks) {
        printf("Error! There is no block on this index \n");
        return;
    }
    for (int i = index; i <= number_of_blocks - 1; i++)
    {
        memoryBlocks->blocks_array[i] = memoryBlocks->blocks_array[i + 1];
    }
    free(memoryBlocks->blocks_array[number_of_blocks]);
    memoryBlocks->number_of_blocks--;
}

void destroy(memoryBlocks *memoryBlocks)
{
    for (int i = 0; i < memoryBlocks->number_of_blocks; ++i) {
        free(memoryBlocks->blocks_array[i]);
    }
    memoryBlocks->number_of_blocks = 0;
}
