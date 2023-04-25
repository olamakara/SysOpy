#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>
#include <time.h>
#include <sys/stat.h>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Wrong number of arguments\n");
        return 1;
    }
    double a = 0, b = 1;  // przedzial funkcji
    double square_width = strtod(argv[1], NULL);  // 0.0ab0
    int number_of_processes = atoi(argv[2]);
    printf("%0.12f, %d\n", square_width, number_of_processes);
    double final_result = 0;

    struct timespec start_time, end_time;
    clock_gettime(CLOCK_REALTIME, &start_time);

    FILE* fd;
    mkfifo("/tmp/fifo", 0666);

    for (int i = 0; i < number_of_processes; i++) {
        int pid = fork();
        if (pid == 0) {
            double tmp_a = a + (b - a) / number_of_processes * i;
            double tmp_b = a + (b - a) / number_of_processes * (i + 1);
            char exec_a[1000], exec_b[1000];
            sprintf(exec_a, "%0.12f", tmp_a);
            sprintf(exec_b, "%0.12f", tmp_b);
            execl("count", "count", exec_a, exec_b, argv[1], NULL);
            exit(1);
        } else if (pid == -1) {
            perror("Fork error\n");
            exit(1);
        }
    }
    fd = fopen("/tmp/fifo", "r");

    for (int i = 0; i < number_of_processes; i++) {
        wait(NULL);
        double result = 0;
        fread(&result, sizeof (double), 1, fd);
        final_result += result;
    }

    fclose(fd);
    clock_gettime(CLOCK_REALTIME, &end_time);
    long time = end_time.tv_sec - start_time.tv_sec;
    printf("Execution time: %lu s\n", time);

    printf("Result: %0.12f\n", final_result);
    return 0;
}
