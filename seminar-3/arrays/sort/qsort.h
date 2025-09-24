// Copyright github: Dkay7

/**
 * @file
 * @brief Данный файл содержит в себе прототипы функций для сортировки романа в
 * стихах "Евгений Онегин" и трагедии "Гамлет".
 *
 * @note Если файл с произведением для сортировки содержит кириллические
 * символы, то он обязательно должен быть в кодироке iso88595. В противном
 * случае, не гарантируется надлежащее сравнение символов файла. Если же файл
 * содержит только символы латиницы, он может быть в любой 8-битной кодировке.
 */

#include <stdio.h>

/**
 * @brief Функция qsort сортирует переданный массив.
 *
 * @param [in] array Указатель на массив указателей для сортировки.
 * @param [in] n_memb Количество элементов в массиве.
 * @param [in] el_size Размер одного элемента в байтах.
 * @param [in] start Левая граница сортируемого участка (индекс элемента
 * массива).
 * @param [in] end Правая граница сортируемого участка (индекс элемента
 * массива).
 * @param [in] comparator Указатель на функцию для сравнения элементов массива.
 */
void qsort(void *array, size_t n_memb, size_t el_size,
           int (*comparator)(const void *, const void *));

// flexxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

/**
 * @brief Функция paritation разделяет массив вокруг опорного элемента.
 * @details Данная функция используется внутри функции @link qsort @endlink,
 *          как часть алгоритма быстрой сортировки.
 *
 * @param [in, out] array Указатель на массив указателей для сортировки.
 * @param [in] n_memb Количество элементов в массиве.
 * @param [in] el_size Размер одного элемента в байтах.
 * @param [in, out] left Указатель на левую границу сортируемого участка (индекс
 * элемента массива).
 * @param [in, out] right Указатель на правую границу сортируемого участка
 * (индекс элемента массива).
 * @param [in] pivot_index Индекс опорного элемента.
 * @param [in] comparator Указатель на функцию для сравнения элементов массива.
 *
 * @return Возвращает индекс последнего элемента левой части массива.
 */
unsigned paritation(char *array, size_t n_memb, size_t el_size,
                    unsigned pivot_index,
                    int (*comparator)(const void *, const void *));

// flexxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

/**
 * @brief Функция swap меняет местами два элемента.
 *
 * @param [in,out] a Указатель на первый элемент.
 * @param [in,out] b Указатель на второй элемент.
 * @param [in] el_size Размер одного элемента в байтах.
 */
void swap(void *a, void *b, size_t el_size);

// flexxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

/**
 * @brief Функция choose_pivot выбирает в массиве опорный элемент.
 *
 * @param [in] n_memb Количество элементов в массиве.
 * @return Индекс опорного элемента в массиве
 */
unsigned choose_pivot(size_t n_memb);

/**
 * @brief Функция compare_letters используется для лексиграфического
 *        сравнения двух букв
 *
 * @param [in] a Первая буква.
 * @param [in] b Вторая буква.
 * @return -1, если a < b,\n
 *          0, если a == b,\n
 *          1, если a > b\n
 */
int compare_letters(char a, char b);
