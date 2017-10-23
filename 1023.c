#include <stdio.h>
#include <stdlib.h>

int main(){
  int x = 33;
  printf("actual value of x: %d\n", x);
  printf("o : %o\n", x);
  printf("x : %x\n\n", x);

  for(int i = 10; i < 20; i++){
    printf("actual value: %d\n", i);
    printf("o : %o\n", i);
    printf("x : %x\n\n", i);
  }
}
