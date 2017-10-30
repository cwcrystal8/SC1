#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

int main(){
  umask(0);
  printf("table size: %d\n", getdtablesize());
  
  int fd = open("testing.txt", O_CREAT | O_WRONLY, 0666);
  // supposed to be rw-rw-rw, but rw-rw-r without umask

  char s[100] = "this is in a file!\n";
  write(fd, s, strlen(s));
  
  int ia[4] = {45, 97, 100};
  //write(fd, ia, sizeof(ia));

  printf("fd: %d\n", fd);

  close(fd);
}
