#ifndef EVAL_H
#define EVAL_H

#include "stack.h"

typedef struct {
  int idx;
  char *expr;
} lexer;

typedef int (*operator)(int, int);


operator get_operator(char c);
lexer* init_lexer(char *eval_str);
token next_token(lexer *l);
stack *infix_to_reverse_polish(char *expr);
int evaluate_reverse_polish(stack *pfix_stack);
int eval(char *expr);
void print_tokens(char *expr);

#endif
