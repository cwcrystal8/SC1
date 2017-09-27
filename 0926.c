/*
9.26.17
LETS HEAD TO FUNCTION TOWN
*/

void print_array(int a[], int length){
  int i;
  for(i = 0; i < length; i++){
    printf("\telement %d: %d\n", i, a[i]);
  }
}

// since function order matters, you can create a function header
void print_array(int a[], int length);

// you can also create a separate file of headers (stdio.h is a collection of headers)
// i.e. create headers.h
#include "headers.h"
// this allows for portability

/*
You must declare a function before you use it. This can be done i a few ways:
1. Write the entire function at the top of your code, making sure to keep the order correct.
2. Write the function headers at the top of your code, and then provide the full definition later.
3. Put all the function headers in a separate file (ending in .h)
*/
