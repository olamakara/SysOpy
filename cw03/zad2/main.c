#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("Wrong number of arguments\n");
        return 0;
    }
    printf("%s: ", argv[0]);
    fflush(stdout);
    execl("/bin/ls", "ls", argv[1], NULL);

    return 0;
}
