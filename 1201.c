#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
  int var = 3;
  int f = fork();
  if(!f){
    var *= var;
    printf("child: %d\n", var);
  }else{
    int status;
    wait(&status);
    printf("parent: %d\n", var);
  }
  return 0;
}
