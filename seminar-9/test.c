
#include <stdio.h>

typedef union {
    const char *name;
    int number;
    char op; // TODO: Make enum to support multi-symbols operators
} TokenData;

int main() {
    TokenData data[4];

    data[0].number = 51;
    data[0].op = 'a';

    data[1].op = '+';
    data[2].name = "variaable_1";
    data[3].number = 5;


    printf("`%c`\n", data[0].op);


}
