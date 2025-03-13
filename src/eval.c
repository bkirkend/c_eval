#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../header/stack.h"
#include "../header/eval.h"

static inline int add(int a, int b){
  return a + b;
}

static inline int multiply(int a, int b){
  return a * b;
}

static inline int subtract(int a, int b){
  return a - b;
}

static inline int divide(int a, int b){
  return a / b;
}

static inline int mod(int a, int b){
  return a % b;
}

static inline int power(int a, int b) {
    int result = 1;
    while (b > 0) {
        //multiplies normally if b is odd
        if (b & 1) {
            result *= a;
        }
        //squares base and halves exponent otherwise (even)
        a *= a;
        b >>= 1; 
    }
    return result;
}

static inline int factorial(int a){
  if (a == 0) return 1;

  int value = 1;
  for(int i = a; i > 1; i--){
    value *= i;
  }
  return value;
}

static inline int inverse(int a){
  return -a;
}

static int get_precedence(char op){
  switch(op){
    case '+':
    case '-':
      return 1;
    case '*':
    case '/':
    case '%':
      return 2;
    case '^':
      return 3;
    default:
      return 0;
  } 
}

bin_operator get_bin_operator(char c){
  switch (c) {
    case '+':
      return add;
    case '-':
      return subtract;
    case '*':
      return multiply;
    case '/':
      return divide;
    case '^':
      return power;
    case '%':
      return mod;

    default:
      return NULL;
  }
}

unary_operator get_unary_operator(char c){
  switch(c) {
    case '-':
      return inverse;
    case '!':
      return factorial;

    default:
      return NULL;
  }
}


static bool is_digit(char c){
  return '0' <= c && c <= '9';
}

static bool is_op(char c){
  switch (c){
    case '+':
    case '-':
    case '*':
    case '/':
    case '^':
    case '!':
      return true;
    default:
      return false;
  }
}

lexer* init_lexer(char *eval_str){
  lexer *l = malloc(sizeof(lexer));
  if(!l){
    printf("failed to allocate lexer\n");
    return NULL;
  }
  l->idx = 0;
  l->expr = eval_str;
  token t = {.type = INVALID_TOKEN};
  l->prev = t;
  return l;
}

token next_token(lexer *l){
  //ignore whitespace
  while(l->expr[l->idx] == ' ') l->idx++;
  char c = l->expr[l->idx];
  
  token t = {.type = INVALID_TOKEN, .op = c};
  // check if operator
  if (is_op(c)){
    l->idx++;

    if (c == '!'){ //postfix unary op
      if(l->prev.type == NUMBER || l->prev.type == CLOSE_PAR){
        t.type = UNARY_OPERATOR;
      } else {
        printf("Invalid factorial usage\n");
        exit(1);
      }
    } else if (l->prev.type != NUMBER){ //prefix unary op
      t.type = UNARY_OPERATOR;
    } else { //binary op
      t.type = BINARY_OPERATOR;
    }
    l->prev = t;
    return t;
  }

  // digits - can be multi-character
  if(is_digit(c)){
    int value = c - '0';
    while(is_digit(l->expr[++l->idx])){
      value *= 10;
      int next_int = l->expr[l->idx] - '0';
      value += next_int;
    }
    token t = {.type = NUMBER, .num = value};
    l->prev = t;
    return t;
  }

  //opening parenthesis
  if(c == '('){
    l->idx++;
    token t = {.type = OPEN_PAR};
    l->prev = t;
    return t;
  }

  // closing parenthesis
  if(c == ')'){
    l->idx++;
    token t = {.type = CLOSE_PAR};
    l->prev = t;
    return t;
  }

  // EOF
  if(c == '\0' || c == '\n' || c == '\r'){
    token t = {.type = EOF_TOKEN};
    l->prev = t;
    return t;
  }

  l->idx++;
  l->prev = t;
  return t;
}

stack *infix_to_reverse_polish(char *expr){
  lexer *l = init_lexer(expr);
  stack *num_stack = stack_init();
  stack *op_stack = stack_init();

  token t;
  while((t = next_token(l)).type != EOF_TOKEN){
    switch(t.type){
      case BINARY_OPERATOR:
        while(!stack_empty(op_stack) && get_precedence(stack_top(op_stack).op) >= get_precedence(t.op)){
          stack_push(num_stack, stack_pop(op_stack));
        }
        stack_push(op_stack, t);
        break;
      case UNARY_OPERATOR:
        if(t.op == '!'){ //factorial postfix op
          stack_push(num_stack, t); 
        } else { //negation prefix op
          stack_push(op_stack, t);
        }
        break;
      case NUMBER:
        stack_push(num_stack, t);
        break;
      case OPEN_PAR:
        stack_push(op_stack, t);
        break;
      case CLOSE_PAR:
        {
          if (stack_empty(op_stack)) {
            printf("Mismatched parentheses\n");
            stack_free(num_stack);
            stack_free(op_stack);
            free(l);
            return NULL;
          }

          token tmp_token = {.type = INVALID_TOKEN};
          while(!stack_empty(op_stack) && (tmp_token = stack_pop(op_stack)).type != OPEN_PAR){
            stack_push(num_stack, tmp_token);
          }

          if(tmp_token.type == INVALID_TOKEN){
            printf("Mismatched parentheses\n");
            stack_free(num_stack);
            stack_free(op_stack);
            free(l);
            return NULL;
          }

          break;
        }
      default:
        printf("Invalid token\n");
    }
  }

  //push op stack onto num stack
  while(!stack_empty(op_stack)){
    stack_push(num_stack, stack_pop(op_stack));
  }

  //cleanup lexer and op stack - not num stack
  free(l);
  stack_free(op_stack);

  return num_stack;
}

int evaluate_reverse_polish(stack *pfix_stack){
  stack_reverse(pfix_stack);
  stack *eval_stack = stack_init();

  while(!stack_empty(pfix_stack)){
    token t = stack_pop(pfix_stack);

    if(t.type == BINARY_OPERATOR){
      int a = stack_pop(eval_stack).num; 
      int b = stack_pop(eval_stack).num;
      token ab = {.type = NUMBER, .num = get_bin_operator(t.op)(b, a)};
      stack_push(eval_stack, ab);

    } else if(t.type == UNARY_OPERATOR){
      int a = stack_pop(eval_stack).num;
      token a_prime = {.type = NUMBER, .num = get_unary_operator(t.op)(a)};
      stack_push(eval_stack, a_prime);

    } else if(t.type == NUMBER){
      stack_push(eval_stack, t);
    }
  }
  int ret_value = stack_pop(eval_stack).num;
  stack_free(eval_stack);
  stack_free(pfix_stack);
  return ret_value;
}

int eval(char *expr){
  stack *s = infix_to_reverse_polish(expr); 
  return evaluate_reverse_polish(s);
}

void print_tokens(char *expr){
  lexer *l = init_lexer(expr);
  stack *num_stack = stack_init();
  stack *op_stack = stack_init();
  
  token t;
  while((t = next_token(l)).type != EOF_TOKEN){
    switch(t.type){
      case BINARY_OPERATOR:
      case UNARY_OPERATOR:
        printf("operator: %c\n", t.op);
        break;
      case NUMBER: 
        printf("digit: %d\n", t.num);
        break;
      case OPEN_PAR:
        printf("opening parenthesis: (\n");
        break;
      case CLOSE_PAR:
        printf("closing parenthesis: )\n");
        break;
      default:
        printf("invalid token\n");
    }
  }

  free(l);
  stack_free(num_stack);
  stack_free(op_stack);

}
