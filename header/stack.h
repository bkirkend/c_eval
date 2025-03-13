#ifndef STACK_H
#define STACK_H

#include <stdbool.h>

#define INIT_CAP 20

typedef enum {
  NUMBER,
  BINARY_OPERATOR,
  UNARY_OPERATOR,
  OPEN_PAR,
  CLOSE_PAR,
  EOF_TOKEN,
  INVALID_TOKEN
} token_type;

typedef struct {
  token_type type;
  union {
    char op;
    int num;
  };
} token;

typedef struct {
    int cap;
    int size;
    token *items;
} stack;

stack* stack_init(void);
void stack_free(stack *s);
void stack_push(stack *s, token c);
token stack_pop(stack *s);
token stack_top(stack *s);
bool stack_empty(stack *s);
void stack_print(stack *s);
void stack_reverse(stack *s);

#endif
