#include "../header/eval.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv){
  char buffer[128];
  printf("========== EVAL TERM ==========\n");
  printf("Type [help] for commands: \n");
  for(;;){
    printf(">> ");

    fgets(buffer, 128, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';

    if (!strcmp(buffer, "help")){
      printf("[exit] - exit\n");
    } else if(!strcmp(buffer, "exit")){
      break;
    } else {
      printf("%d\n", eval(buffer));
    }
  }
}
