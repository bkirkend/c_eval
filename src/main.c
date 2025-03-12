#include "../header/eval.h"
#include <stdio.h>

int main(int argc, char **argv){
  int a = 4;
  int b = 5;
  if(argc != 2){
    printf("Invalid usage, expected ./[exe] [string]\n");
    return 1;
  }
  // print_tokens(argv[1]);
  printf("%d\n", eval(argv[1]));
  return 0;
}
