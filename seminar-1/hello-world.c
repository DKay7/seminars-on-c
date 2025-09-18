// RUN: clang hello-world.c -o hello-world

#include <stdio.h>

int printf(const char *, ...);

int main() {

  printf("Hello World!\n");

  return 0; // Not neccessary to return from main
  
}
