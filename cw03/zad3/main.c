#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_NUMBER_OF_BYTES 225

int recursion(char *path, char *char_chain)
{
    struct stat path_stat;
    if (stat(path, &path_stat) == 0)
    {
        if (S_ISDIR(path_stat.st_mode))
        {
            struct dirent *dir_ptr;
            DIR *directory = opendir(path);
            if (directory == NULL)
            {
                perror("Could not read the directory\n");
                exit(1);
            }
            while ((dir_ptr = readdir(directory)) != NULL)
            {
                char new_path[strlen(path) + strlen("/") + strlen(dir_ptr->d_name) + 1];
                strcpy(new_path, path);
                strcat(strcat(new_path, "/"), dir_ptr->d_name);
                if (strcmp(dir_ptr->d_name, ".") != 0 && strcmp(dir_ptr->d_name, "..") != 0 && strlen(new_path) <= PATH_MAX)
                {
                    if (fork() == 0)
                    {
                        recursion(new_path, char_chain);
                        exit(0);
                    }
                    else
                    {
                        wait(0);
                    }
                }
            }
            closedir(directory);
        } else
        {
            FILE *file = fopen(path, "r");
            if (file == NULL)
            {
                perror("Could not read the file\n");
                exit(1);
            }
            int len = strlen(char_chain);
            char *buffer = (char*) malloc(len + 1);
            fread(buffer, 1, len, file);
            if (strcmp(char_chain, buffer) == 0)
            {
                printf("path: %s, id: %lu\n", path, path_stat.st_ino);
            }
            fclose(file);
        }
    } else
    {
        perror("Could not get the path stats\n");
    }

    return 0;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Wrong number of arguments: %d\n", argc);
        return 0;
    }
    char *dir_path = argv[1];
    char *char_chain = argv[2];
    if (strlen(char_chain) > MAX_NUMBER_OF_BYTES)
    {
        printf("Too many characters in the second argument");
    }

    recursion(dir_path, char_chain);

    return 0;
}
