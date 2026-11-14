// Copyright github: Dkay7

/**
 * @file
 * @brief Sorting function declarations for literary texts.
 * @note Cyrillic input must use ISO-8859-5 for character comparison.
 * Latin-only input may use any compatible 8-bit encoding.
 */

#include <stdio.h>

/**
 * @brief Sort an array using the supplied comparator.
 * @param array Array to sort.
 * @param n_memb Number of elements.
 * @param el_size Element size in bytes.
 * @param comparator Element comparison function.
 */
void qsort(void *array, size_t n_memb, size_t el_size,
           int (*comparator)(const void *, const void *));

// flexxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

/**
 * @brief Partition an array around a pivot as part of quicksort.
 * @param array Array to partition.
 * @param n_memb Number of elements.
 * @param el_size Element size in bytes.
 * @param pivot_index Pivot element index.
 * @param comparator Element comparison function.
 * @return Index of the last element in the left partition.
 */
unsigned paritation(char *array, size_t n_memb, size_t el_size,
                    unsigned pivot_index,
                    int (*comparator)(const void *, const void *));

// flexxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

/**
 * @brief Swap two elements.
 * @param a First element.
 * @param b Second element.
 * @param el_size Element size in bytes.
 */
void swap(void *a, void *b, size_t el_size);

// flexxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

/**
 * @brief Choose a pivot index.
 * @param n_memb Number of elements.
 * @return Pivot element index.
 */
unsigned choose_pivot(size_t n_memb);

/**
 * @brief Compare two letters lexicographically.
 * @param a First letter.
 * @param b Second letter.
 * @return -1 if a < b, 0 if a == b, or 1 if a > b.
 */
int compare_letters(char a, char b);
