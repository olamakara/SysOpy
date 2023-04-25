#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>
#include <time.h>

double f(double x);

double integral(double a, double b, double square_width);

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

//    FILE *report = fopen("report", "a");
//    if (report == NULL) {
//        printf("Failed to open file.\n");
//        return 1;
//    }
//    char text[strlen(argv[1]) + strlen(argv[2]) + 13];
//    strcat(text, "Arguments: ");
//    strcat(text, argv[1]);
//    strcat(text, ", ");
//    strcat(text, argv[2]);
//    printf("%s", text);
//    fwrite(text, 1, strlen(text), report);

    struct timespec start_time, end_time;
    clock_gettime(CLOCK_REALTIME, &start_time);

    for (int i = 0; i < number_of_processes; i++) {
        int fd[2];
        if (pipe(fd) == -1) {
            perror("Pipe error\n");
            exit(1);
        }
        int pid = fork();
        if (pid == 0) {
            close(fd[0]);
            double tmp_a = a + (b - a) / number_of_processes * i;
            double tmp_b = a + (b - a) / number_of_processes * (i + 1);
            double result = integral(tmp_a, tmp_b, square_width);
            char buff[sizeof(long double)];
            sprintf(buff, "%0.12f", result);
            write(fd[1], buff, sizeof(long double));
            close(fd[1]);
            exit(1);
        } else if (pid == -1) {
            perror("Fork error\n");
            exit(1);
        } else {
            close(fd[1]);
            wait(NULL);
            char buff[sizeof (long double)];
            read(fd[0], buff, sizeof (long double));
            final_result += strtod(buff, NULL);
            close(fd[0]);
        }
    }
    clock_gettime(CLOCK_REALTIME, &end_time);
    long time = end_time.tv_sec - start_time.tv_sec;
    printf("Execution time: %lu s\n", time);

    printf("Result: %0.12f\n", final_result);
//    fclose(report);
    return 0;
}

double f(double x) {
    return 4 / (x * x + 1);  // mozna pow(x, 2)
}

double integral(double a, double b, double square_width) {
    double result = 0;
    for (double i = a + square_width / 2; i < b; i += square_width) {
        result += f(i) * square_width;
    }
    return result;
}
