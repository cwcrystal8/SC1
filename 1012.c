#include <stdio.h>
#include <stdlib.h>

/*
void *calloc(size_t nmemb, size_t size);
allocate memory for an array of nmemb elements of size bytes each; returns pointer to allocated memory. memory is set to zero. if any given parameter is 0, then NULL is returned 

void *realloc(void *ptr, size_t size);
changes size of memory block (*ptr) to size bytes; content will remain unchanged to the minimum between old and new; added memory is not initialized 
if ptr is NULL, call is equivalent to malloc(size)
if size = 0 (and ptr isnt null), equivalent to free(ptr)
*/
int main(){

  //calloc
  int i;
  int *a = calloc(10, 4);
  printf("%p\n", a);
  for(i = 0; i < 11; i++){
    printf("a[%d] = %d\n", i, a[i]);
  }
  a[3] = 746;
  printf("a[3] = %d\n", a[3]);

  //realloc
  printf("sizeof(a) = %lu\n", sizeof(a));
  int *b = realloc(a, 40);
  printf("b[0] = %d\n", b[0]);
}
