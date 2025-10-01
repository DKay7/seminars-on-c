
#include <math.h>
#include <stdio.h>

void PrintHelloMessage(void);
double FindMaxNum(double, double);

int main(int argc, const char **argv) {

  PrintHelloMessage();

  double lhs = NAN, rhs = NAN;

  int num_readed = scanf("%lf %lf ", &lhs, &rhs);

  if (num_readed != 2) {
    fprintf(stderr, "Error while reading data from stdin");
    return -1;
  }

  double max = FindMaxNum(lhs, rhs);

  printf("Maximum of given nums is %lf\n", max);
}

void PrintHelloMessage(void) {
  printf("Hello! This program determines the largest of two given nums. Enjoy "
         ";)\n");
}

double FindMaxNum(double lhs, double rhs) { return lhs > rhs ? lhs : rhs; }
