# 10.12.17 - malloc & free: The Dynamic Duo!

## Dynamic Memory Allocation

### `malloc(size_t x)` - Memory Allocation
- allocates x bytes of memory (from the heap)
- returns the address at the beginning of the allocation
- returns a void *, always typecast to the correct pointer type.

```c
int *p;
p = (int *)malloc(5 * sizeof(int));
```

### `free`

Releases dynamically allocated memory.

Takes one parameter, a pointer to the beginning of a dynamically allocated block of memory.

**Every call to malloc/calloc should have a corresponding call to free.**

```c
int *p;
p = (int *)malloc(20);
free(p);
```
When calling free, the original pointer does not need to be used. If there are 3 pointers pointing to the same address (must be the beginning of the allocation), any one of the pointers will work.

### `calloc(size_t n, size_t x)` - Clear Allocation
- allocates n * x bytes of memory
- ensures that each bit is set to 0
- works like malloc in all other ways

```c
int *p;
p = (int*)calloc(5, sizeof(int));
```

### `realloc(void *p, size_t x)` - Re-Allocation
- changes the amount of memory allocated to a given block
- `p` must be a pointer to the beginning of an allocated block of memory, but it does not have to be the original pointer.
- if `x` is smaller than the original size of the allocation, the extra bytes will be released
- if `x` is larger and there is no room after the original allocation, it will copy over the memory and return a different pointer (frees the old one)

```c
int *p;
p = (int *)malloc(20);
p = (int *)realloc(p, 40);
```

---
# 10.10.17 - If you don't pay attention you'll get into a heap of trouble!

### struct
We use the `.` operator to access a value inside a struct.
```c
struct foo {
       int a;
       char x;
       
       struct bar { double z; short pants; }
       struct foo *next;
};
foo s;
s.a = 10;
s.x = '@';
```
`struct` can include structs within, as well as struct pointers.
This is basically a node, which allows us to make things like linked lists!!!

You can make pointers to functions. If you put one inside a struct, it looks a lot like a method.

`.` binds before `*`
```c
struct foo *p;
p = &s;
(*p).x;
```
`*p.x` is the same thing as `*(p.x)`;

To access data from a struct pointer you can also use `->`.
`p->x` is the same thing as `(*p).x`

You can typedef structs, but be careful because this will hide the fact that you are working with a struct.

## Stack v. Heap Memory

Computer programs seaprate memory usage into two parts: stack and heap.

Every program can have its own stack and heap.

### Stack Memory
- Stores all normally declared variables (including pointers and structs), arrays and function calls.
- Function are pushed onto the stack in the order they are called, and popped off when completed.
- When a function is popped off the stack, the stack memory associated with it is released.

---
# 10.11.17 - Yesterday continued.

```c
struct node {
    int data;
    struct node *next;
};

struct node *insert_front(struct node *front, int d){
    struct node new;

    new.data = d;
    new.next = front;

    return &new; // see note below
}

int main(){
    struct node *list = NULL;
    list = insert_front(list, 12);
    return 0;
}
```
* This will not work because the node is created within a function, and will "disappear" from memory once the function gets popped off the stack.

### Heap Memory
- Stores dynaminally allocated memory.
- **Data will remain in the heap until it is released** (or the program terminates).
- Can be accessed through pointers.
- Can be accessed across many functions.

---
# 10.6.17 - Finding your type.

### typedef
Allows you to provide a new name for an existing data type.
`typedef <real type> <new name>;`
```c
typedef unsigned long size_t;
size_t x = 139; //x is really an unsigned long
```
**IMPORTANT** - The use of typedef is another in a long line of religious wars in programming (see emacs v. vim, tabs v. spaces, etc.)

### struct
Creates a new type that is a collection of values.
`struct { int a; char x; } s;` makes `s` a variable of that type (that being `struct {int a; char x;}`)
- this makes the size of `s` 8 (theoretically it would be a 5 byte size)

`struct foo { int a; char x; };`
Here, foo is a prototype for this kind of struct, to be used later: `struct foo s;`
```c
struct foo{
       int a;
       char x;
};
```

---
# 10.3.17 - **Make it so**

### SEPARATE COMPILATION
You can combine multiple c files into a c program by including them all in one gcc command.
* **i.e.** gcc test.c string.c foo.c woohoo.c
This creates a single executeable.

You cannot have duplicate function or global variable names acrosss these files. This will result in an error (not a warning). 
* **i.e.** multiple main functions

There must be a main function in order to create an executeable program.

Including .h files: #include "<filename>"

`gcc -c` compiles a c file into a .o file. It is not a fully functional program, but it is compiled code.
Do this to compile a .c file that does not contain a main() function (to check for syntax errors).
.o files can be linked together with .c files through gcc
* **i.e.** gcc string.o main.o
    
### MAKE
* Creates compiling instructions and sets up dependencies
* Standard name for the file is makefile

Syntax:
```
<TARGET>: <DEPENDENCIES>
TAB<RULES>
```
For example:
```
strtest: dwstring.o main.o
    gcc -o strtest dwstring.o main.o
dwstring.o: dwstring.c dwstring.h
    gcc -c dwstring.c
main.o: main.c dwstring.h
    gcc -c main.c
```
To run the makefile: `make`

Every time you run make, only the target is checked, and any of its subsequent dependencies. 
`touch` makes it seem like the file has been modified

### More common things in make files
```
clean:
    rm *.o
    rm *~
run: all
    ./strtest
```
In this case, `make run` will compile everything, and then run the program.

---
# 9.29.17

The "n" versions of all the string functions ensure that the array does not go beyond the memory allocated to it.
 - `strncpy(s, "hello", sizeOf(s));`
 - `strncat(s1, s2, sizeOf(s1) - strlen(s2));`
 
---
# 9.28.17 - A string of functions.

Array variable pointers are IMMUTABLE. You can modify the array itself, but not what the pointer is pointing to.

You can only assign char arrays with `=` at declaration.
- `char s[] = "zero";` --> ok
- `s = "seven";`       --> not ok; s is an array pointer, and cannot be modified

Char pointers can be assigned using `=` at any time.
- `char *s = "zero";`  --> ok
- `s = "seven";`       --> ok

What a variable is initially created as (regular variable vs. array pointer) is important to keep in mind. Use arrays when you want to be able to modify a string.

---
# 9.27.17 - Ctrings
**Strings** - character arrays (a string of characters...)
- There is no string data type; only char.

By convention, strings end with a null character (either '', 0, or '\0'; NOT '0' because it has a value).
When you make a literal string using "", an immutable string is created in memory. There strings are automatically null terminated.

All references to the same literal string refer to the same immutable string in memory.

YOU CANNOT MODIFY LITERAL STRINGS.

### DECLARING STRINGS
`char s[256];`
- normal array declaration
- allocates 256 bytes
`char s[256] = "imagine";`
- allocates 256 bytes
- creates the immutable string `"imagine"` and then copies it (including the terminating null) into the array
`char s[] = "tuesday";`
- allocates 8 bytes
- creates the immutable string `"tuesday"` and then copies it (including the terminating null) into the array
`char *s3 = "mankind";`
- creates the immutable string `"mankind"` and returns a pointer to that string
- Since the pointer is to an immutable piece of memory, you cannot modify strings created this way.

---
# 9.26.17 - Let's head to function town.
Since function order matters, you can avoid issues by creating a function header.
```c
void print_array(int a[], int length);
```

You can also create a separate file of headers (stdio.h is a collection of headers).
- For example, create `headers.h`, and place `#include "headers.h"` at the start of your code. This allows for portability.

You must declare a function before you use it. This can be done in a few ways:
1. Write the entire function at the top of your code, making sure to keep the order correct.
2. Write the function headers at the top of your code, and then provide the full definition later.
3. Put all the function headers in a separate file (ending in .h).

---
# 9.25.17 - How to write functioning code.

`srand(time(NULL))` returns the number of seconds since 1.1.1970

### ARRAYS AND POINTERS
Array variables are immutable pointers. Pointers can be assigned to array variables (aka, arrays are immutable, pointers are not).
```c
int ray[5];
int *rp = ray;
```
`ray[3]` is the same thing as `*(ray+3)`
```
ray[3] = *(ray+3);
3[ray] = *(3+ray);
```
Bracket notation simply means to add the two and de-reference (but DO NOT do the second option because that is dumb).

Formally put:
`a[i]` notation is shorthand for `*(a+i)`

`p++` means return p, then add 1
`++p` means add 1, then return p

### WRITING FUNCTIONS
Function headers: `<return type> <name> (<parameters>)`
All functions are pass by value. Function arguments are new variables that contain a copy of the data passed to them.
```c
doubla a = 2.3;
    foo(a);
...
void foo(double x){
    x = 59;
}
```
When calling foo, a new variable will be created (i.e. x for a), so when modifying the variable the original will not be changed.
If you want to actually modify the variable within the function, use pointers.
```c
void foo(double *x){
    *x = 59;
}
```
When passing an array, you pass only the pointer.
