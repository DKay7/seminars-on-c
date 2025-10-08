#include "string-functions.h"
#include <stdio.h>
#include <stdlib.h>

#define SOFT_ASSERT(cond, msg, ret)                                            \
  do {                                                                         \
    if (!(cond)) {                                                             \
      perror((msg));                                                           \
      return (ret);                                                            \
    }                                                                          \
  } while (0)

int main(void) {
  char *src = NULL;
  char *dest = NULL;

  SOFT_ASSERT(scanf("%m[^\n]s", &src) == 1, "Input error", EXIT_FAILURE);

  size_t src_len = my_strlen(src);

  dest = (char *)calloc(src_len, sizeof(*dest));
  SOFT_ASSERT(dest != NULL, "calloc failure :(", EXIT_FAILURE);

  my_strcpy(dest, src);

  printf("src: `%s`\ndest: `%s`\n", src, dest);

  // we HAVE TO call free(src) since it was malloc-ed inside scanf
  // (since we ask: %ms)
  free(src);

  // also, do not forget about free dest
  free(dest);

  return EXIT_SUCCESS;
}
