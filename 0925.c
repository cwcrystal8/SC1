#include <stdio.h>
#include <stdlib.h>

int hi(){
  printf("hi\n");
}

int main(){
  hi();
}

/* 
9.25.17
HOW TO WRITE FUNCTIONING CODE

srand(time(NULL)) --> returns number of seconds since 1/1/1970
*/

// HOMEWORK STUFF -------------------
int arr1[10];
int arr2[10];

//randomize arr1

int *ap = arr1;
int *ap2 = arr2 + 9;

// this works because the last value is 0 (false)
while(*ap){
  *ap2 = *ap;
  ap2--;
  ap++;
}
*ap2 = *ap;
//END HW STUFF ---------------------------------------

/*
ARRAYS AND POINTERS

Array variables are immutable pointers
Pointers can be assigned to array variables
aka arrays are immutable, pointers are not
   int ray[5];
   int *rp = ray;

ray[3] is the same thing as *(ray+3)
   ray[3] = *(ray + 3);
   3[ray] = *(3 + ray); -->> THIS ACTUALLY WORKS??? but you should not do it
   bracket notation just means you add the 2 and de-reference

a[i] notation is shorthand for *(a+i)
   a[i] <==> *(a+i)
   i[a] <==> *(i+a)

p++ --> return p, then add 1
++p --> add 1, then return p


WRITING FUNCTIONS 
function headers:
   <return type> <name> (<parameters>)
All functions are pass by value.
Function arguments are new variables that contain a copy of the data passed to them.
   double a = 2.3;
   foo(a);
   ...
   void foo(double x)
   ...
when calling foo, a new variable will be created (ie. x for a), so when modifying the variable the original will not be changed 
when passing an array, you pass only the pointer
*/


