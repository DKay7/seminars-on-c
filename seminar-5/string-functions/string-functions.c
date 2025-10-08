#include "string-functions.h"
#include <stddef.h>
#include <stdio.h>

size_t my_strlen(const char *str) {
  const char *str_ptr = str;
  while (*str_ptr != 0)
    ++str_ptr;

  return (size_t)(str_ptr - str); // differens of two pointers IS VALID and has
                                  // a type ptrdiff_t
}

// does NOT check if dest has enough space
// just like the one from stdlib
char *my_strcpy(char *dest, const char *src) {

  // copy pointer to prevent changing dest since we must
  // return pointer to dest
  char *dest_ptr = dest;

  while (*src != '\0') { // same as *str != 0
    *dest_ptr = *src;
    dest_ptr++;
    src++;
  }

  return dest;
}
