#include <stdio.h>
#include <stdlib.h>
#include "../header/stack.h"

static void grow_stack(stack *s);

stack* stack_init(void){
  stack *s = malloc(sizeof(stack));
  if (!s){
    return NULL;
  }
  s->size = 0;
  s->cap = INIT_CAP;
  s->items = malloc(s->cap * sizeof(token));
  if (!s->items){
    free(s);
    return NULL;
  }
  return s;
}

void stack_free(stack *s){
  free(s->items);
  free(s);
}

void stack_push(stack *s, token t){
  if(s->size >= s->cap){
    grow_stack(s);
  }
  s->items[s->size++] = t;
}

token stack_pop(stack *s){
  if (!s->size){
    printf("Error: Pop from empty stack\n");
  }
  return s->items[--s->size];
}

static void grow_stack(stack *s){
  int new_cap = s->cap << 1;
  token *new_items = realloc(s->items, s->cap * sizeof(token));
  if (!new_items){
    printf("Error: Stack reallocation\n");
  }
  s->cap = new_cap;
  s->items = new_items;
}

token stack_top(stack *s){
  if (s->size <= 0){
    printf("Error: Peeking empty stack\n");
  }
  return s->items[s->size - 1];
}

bool stack_empty(stack *s){
  return s->size <= 0;
}

void stack_print(stack *s){
  printf("Stack Print: \n");
  for(int i = s->size - 1; i >= 0; i--){
    token t = s->items[i];
    switch(t.type){
      case OPERATOR:
        printf("%c\n", t.op);
        break;
      case NUMBER:
        printf("%d\n", t.num);
        break;
      case OPEN_PAR:
        printf("(\n");
        break;
      case CLOSE_PAR:
        printf(")\n");
        break;
      default:
        break;
    }
  }
}

static void swap_tokens(token *t1, token *t2){
  token tmp = *t1;
  *t1 = *t2;
  *t2 = tmp;
}

void stack_reverse(stack *s){
  for(int i = 0; i < s->size / 2; i++){
    swap_tokens(&s->items[i], &s->items[s->size - 1 - i]); 
  } 
}

