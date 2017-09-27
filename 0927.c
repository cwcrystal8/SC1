/*
9.27.17
CTRINGS
*/

#include <stdio.h>
#include <stdlib.h>

int main(){
  char *s = "hello";
  char s1[] = "hello";
  printf("s points to: %p\n", s);
  printf("s1 points to: %p\n", s1);
  printf("address of \"hello\": %p\n", &"hello");

  char s2[5];
  // %s prints out character array as a string
  s2[0] = 'h';
  s2[1] = 'i';
  s2[2] = 0;
  s2[3] = 'z';
  printf("s2: %s\n", s2);
  
  return 0;
}

/*
STRINGS - character arrays (a string of characters)
There is no string data type --> just char.

s1
[]   points to  --> 
['h'] ['e'] ['l'] ['l'] ['o'] [0]

s points to --> ['h'] ['e'] ['l'] ['l'] ['o'] [0]


By convention, strings end with a null character (either '', 0, or '\0' - NOT '0' because it has a value). 

When you make a literal string using "", an immutable string is created in memory. These strings are automatically null terminated. 

All references to the same literal string refer to the same immutable string in memory.
s is a string literal, so when you refer to &"hello" you refer to same exact address as s. 

YOU CANNOT MODIFY LITERAL STRINGS. 

DECLARING STRINGS
char s[256]; --> normal array declaration, allocates 256 bytes

char s[256] = "Imagine";
   Allocates 256 bytes. 
   Creates the immutable string "Imagine" and then copies it (including the terminating null) into the array. 

char s[] = "Tuesday";
   Allocates 8 bytes.
   Creates the immutable string "Tuesday" and then copies it """ ^^^^
*/
