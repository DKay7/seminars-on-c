#include <stdio.h>

long sum(long a, long b) { return a + b; }

extern int x;
extern int y;

int main() {
  char user_name[101] = {0};

  printf("Enter your name:\nName:\t");

  // reading the whole strig (with spaces) till newline
  scanf("%100[^\n]s", user_name);

  int result = sum(x, y);

  printf(user_name);

  printf(", The result is %d, but can you figure out which numbers I were "
         "sumed up",
         result);
}

