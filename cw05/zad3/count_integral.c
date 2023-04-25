#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

double f(double x);

double integral(double a, double b, double square_width);

int main(int argc, char* argv[]) {
    if (argc != 4) {
        printf("Wrong number of arguments\n");
        return 1;
    }
    double a = strtod(argv[1], NULL);
    double b = strtod(argv[2], NULL);
    double square_width = strtod(argv[3], NULL);
    double result = integral(a, b, square_width);
    FILE* fd;
    fd = fopen("/tmp/fifo", "w");
    fwrite(&result, sizeof (double), 1, fd);
    fclose(fd);
    exit(1);
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