/*
 * This code contais errors
 * Bad desicions
 * and some bad practise
 *
 * Your task is to find ALL mistakes you can
 * and fix them
 */

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SOFT_ASSERT(cond, arr, code, msg)                                                \
    do {                                                                                 \
        if (!(cond)) {                                                                   \
            fprintf(stderr, "[Warning] %s:%d: %s\n", __FILE__, __LINE__, (msg));         \
            if ((arr) != NULL)                                                           \
                (arr)->error = (code);                                                   \
            return NULL;                                                                 \
        }                                                                                \
    } while (0)

// clang-format off
typedef enum { 
    ERR_OK = 0,
    ERR_MEMORY = 1,
    ERR_SYNTAX = 2,
    ERR_DIV_ZERO = 3 
} ErrorCode;

typedef enum {
    TOK_LITERAL = 0, // variable name
    TOK_CONST = 1,   // double constant
    TOK_OPERATOR = 2 // operator +, -, *, /
} TokenType;

// clang-format on

typedef union {
    const char *name;
    double number;
    char op; // TODO: Make enum to support multi-symbols operators
} TokenData;

typedef struct {
    TokenType type;
    TokenData data;
} Token;

typedef struct {
    size_t count;
    ErrorCode error;
    Token tokens[];
} TokenArray;

TokenArray *create_tokens(size_t n) {
    size_t size_bytes = sizeof(TokenArray) + n * sizeof(Token);

    // num bytes is quite tricky so we use malloc + memset
    TokenArray *arr = malloc(size_bytes);
    SOFT_ASSERT(arr != NULL, arr, ERR_MEMORY, "Malloc wailed");
    memset(arr, 0, size_bytes);

    arr->count = n;
    arr->error = ERR_OK;
    return arr;
}

void destroy_tokens(TokenArray *arr) {
    // poisoning
    arr->count = 0;
    memset(arr->tokens, 0xDEAD, sizeof(Token) * arr->count);
    free(arr);
}

void dump_tokens(const TokenArray *arr) {
    printf("TokenArray:\n");

    for (size_t i = 0; i < arr->count; ++i) {
        const Token *t = &arr->tokens[i];
        printf("[%zu] ", i);

        switch (t->type) {
        case TOK_LITERAL:
            printf("LITERAL: %s", t->data.name);
            break;
        case TOK_CONST:
            printf("CONST: %.3f", t->data.number);
            break;
        case TOK_OPERATOR:
            printf("OPERATOR: %c", t->data.op);
            break;
        }

        printf("\n");
    }

    if (arr->error != ERR_OK)
        printf("[Error] Code: %d\n", arr->error);
}

bool double_eq_zero(double num) {
    const double EPS = 1e-9;

    return fabs(num) < EPS;
}

double evaluate(const TokenArray *arr) {
    double acc = 0.0;
    char current_op = '+';

    for (size_t i = 0; i < arr->count; ++i) {
        const Token *t = &arr->tokens[i];

        switch (t->type) {
        case TOK_CONST:
            switch (current_op) {
            case '+':
                acc += t->data.number;
                break;
            case '-':
                acc -= t->data.number;
                break;
            case '*':
                acc *= t->data.number;
                break;
            case '/':
                if (fabs(t->data.number) < 1e-9) {
                    fprintf(stderr, "Zero division");
                    return NAN;
                }
                acc /= t->data.number;
                break;
            }
            break;

        case TOK_OPERATOR:
            current_op = t->data.op;
            break;

        default:
            fprintf(stderr, "Syntax error at %lu", i);
            return NAN;
        }
    }

    return acc;
}

int main(void) {
    TokenArray *expr = create_tokens(7);
    if (!expr)
        return EXIT_FAILURE;

    TokenData data;
    data.number = 49.0;

    printf("%c", data.op);

    expr->tokens[0] = (Token){.type = TOK_CONST, .data.number = 10.0};
    expr->tokens[1] = (Token){.type = TOK_OPERATOR, .data.op = '+'};
    expr->tokens[2] = (Token){.type = TOK_CONST, .data.number = 2.0};
    expr->tokens[3] = (Token){.type = TOK_OPERATOR, .data.op = '*'};
    expr->tokens[4] = (Token){.type = TOK_CONST, .data.number = 3.0};
    expr->tokens[5] = (Token){.type = TOK_OPERATOR, .data.op = '-'};
    expr->tokens[6] = (Token){.type = TOK_CONST, .data.number = 1.0};

    dump_tokens(expr);

    double result = evaluate(expr);
    printf("Result: %.3f\n", result);

    destroy_tokens(expr);
    return 0;
}
