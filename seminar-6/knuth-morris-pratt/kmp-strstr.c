#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * thanks to Vladislav Oleynik for this implementation with such a great comments
 * original:
 * https://github.com/VladikNeVladik/algorithms-seminars/blob/master/examples/10_string_matching/search.c
 * */
void longest_prefix_suffix(const char *pattern, int pattern_len, int *lps) {
    int length = 0;
    lps[0] = 0; // lps[0] is unused by us, so nullify it
    lps[1] = 0; // lps[1] is always 0

    size_t lps_candidate = 0; // potential value of lps function

    for (size_t prefix_len = 2; prefix_len <= pattern_len; ++prefix_len) {

        // search for smaller prefix that matches current suffix
        while (lps_candidate > 0 && pattern[lps_candidate] != pattern[prefix_len - 1]) {
            // Using lps feature that:
            // if substring of length N has prefix-suffix of length M (M < N)
            //      and substring of length M has prefix-suffix of length K
            //      then substring of length N has prefix-suffix of length K
            //
            // Example of feature:
            // abacaba
            // aba aba
            // a a a a
            // a     a
            //
            lps_candidate = lps[lps_candidate];
        }

        if (pattern[lps_candidate] == pattern[prefix_len - 1]) {
            // in case of new symbol matching suffix, prefix can be enlarged
            lps_candidate += 1;
        }

        lps[prefix_len] = lps_candidate;
    }
}

const char *strstr_knuth_morris_pratt(const char *str, const char *pattern) {
    int str_len = strlen(str);
    int pattern_len = strlen(pattern);
    int lps[pattern_len + 1];
    //int *lps = (int *)calloc(pattern_len + 1, sizeof(*lps));

    longest_prefix_suffix(pattern, pattern_len, lps);

    // for (int idx = 1; idx < pattern_len + 1; ++idx)
    //     printf("lps[%d] = %d\n", idx, lps[idx]);

    int pattern_idx = 0;
    const char *result = NULL;

    for (int str_idx = 0; str_idx < str_len; ++str_idx) {
        while (pattern_idx > 0 && pattern[pattern_idx] != str[str_idx])
            pattern_idx = lps[pattern_idx];

        // if symbols mathed, move pointer
        if (pattern[pattern_idx] == str[str_idx])
            pattern_idx++;

        // if pattern_idx mathed len of pattern, then we found an entry!
        if (pattern_idx == pattern_len) {
            result = &str[str_idx - pattern_len + 1];
            break;
        }
    }

    //free(lps);
    return result;
}
