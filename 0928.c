/*
9.28.17
A STRING OF FUNCTIONS
*/

#include <stdio.h>
#include <stdlib.h>

int slength(char *a);

int slength(char *a){
  int i = 0;
  while(a[i]){
    i++;
  }
  return i;
}

int main(){
  char s1[] = "hello";
  char s2[] = "hi";
  printf("hello: %d\n", slength(s1));
  printf("hi: %d\n", slength(s2));

  char *s3 = "goodbye";
  printf("goodbye: %d\n", slength(s3));
}

/*
Array variable pointers are IMMUTABLE. You can modify the array itself, but not what the pointer is pointing to.

You can only assign char arrays with = at declaration.
   char s[] = "zero";  --> ok
   s = "seven";        --> not ok, because s is an array pointer and that cannot be modified

Char pointers can be assigned using = at any time. 
   char *s = "zero";   --> ok
   s = "seven";        --> ok

What a variable is initially created as (a regular vs. array pointer) is important to keep in mind.
Use arrays when you want to modify a string.

 */
