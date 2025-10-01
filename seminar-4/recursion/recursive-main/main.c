
#include <stdio.h>

int main(int argc, const char **argv) {

  if (argc <= 1)
    return 0;

  printf("%s\n", argv[argc - 1]);

  main(argc - 1, argv);
  return 0;
}
