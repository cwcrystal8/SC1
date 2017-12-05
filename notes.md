# 12.4.17 - Memes
**Do Now:** Why is the aim Memes?  
*They are like memory shared between people.*

Generally, `shmat` after the fork so that pid numbers don't get wonky.

Onto continuing shared memory operations!!

### 4: `shmdt` (shared memory detach)
Detach a variable from a shared memory segment.  
Returns 0 upon success or -1 upon failure.

`shmdt (pointer)`
- **pointer:** the address used to access the segment (will NOT be changed)  

### 5: `shmctl` (shared memory control)
Perform operations on the shared memory segment.  
Each shared memory segment has metadata that can be stored in a struct (`struct shmid_ds`).  
Some of that data stored: last access, size, pid of creator, pid of last modification. 

`shmctl (descriptor, command, buffer)`
- **descriptor:** return value of `shmget`
- **commands:** 
  - `IPC_RMID` removes a shared memory segment
  - `IPC_STAT` opulate the **buffer** (`struct shmid_ds *`) with segment metadata
  - `IPC_SET` set some of the segment metadata from **buffer**
  
### `ipcs` (ipc stat)
Terminal command hat lists the current shared memory segments. 

---
# 12.1.17 - Sharing is caring!
**Do Now:** Writing a c program with a variable, fork, adjust the value in the child, and print it out in both parent and child.
```c
int f;
double var = 5.83;
f = fork();
if(!f){
    var *= 10;
    printf("[child]: %d\n", var);
}
else{
    int status;
    wait(&status);
    printf("[parent]: %d\n", var);
}
```
Parent | Child
--- | ---
5.83 | 58.3

## Shared Memory - `<sys/shm.h>`, `<sys/ipc.h>`, `<sys/types.h>`
- a segment of heap memory that can be accessed by multiple processes
- shared memory is accessed via a key that is known by any process that needs to access it
- does not get released when a program exits (only when rebooting)

### 5 Shared Memory Operations:
1. Create the segment (happens only *once*)
2. Access the segment (happens once per process)
3. Attach the segment to a variable (once per process)
4. Detach the segment from a variable (once per process)
5. Remove the segment (only once)

### 1 & 2: `shmget`
Create or access a shared memory segment.  
Returns a shared memory descriptor (similar in concept to a file descriptor), or -1 if it fails (errno).

`shmget(key, size, flags)`
- **key:** unique integer identifier for the shared memory segment (like a file name)
- **size:** how much memory to request
- **flags:** includes permissions (same as for files, 3-digit octals) for the segment, combined with bitwise or
    - `IPC_CREAT` create the segment; if segment is new, will set value to all 0s
    - `IPC_EXCL` fail if the segment already exists and `IPC_CREAT` is on
    
### 3: `shmat`
Attach a shared memory segment to a variable.  
Returns a pointer to the segment, or -1 (errno).

`shmat(descriptor, address, flags)`
- **descriptor:** return value of `shmget`
- **address:** if 0, the OS will provide the appropriate address
- **flags:** usually 0, but one useful one
    - `SHM_RDONLY` makes the memory read only

Modifying the Do Now:
```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>

#define KEY 24601

int main () {
    int f;
    int sd = shmget(KEY, sizeof(double), 0600 | IPC_CREAT);

    f = fork();
    if (!f) {
        double *p = shmat(sd, 0, 0);
        *p = 198746.9;
        printf("[child]: %d\n", *p);
    }
    else {
        int status;
        
        double *p;
        *p = shmat(sd, 0, 0);
        
        wait(&status);
        printf("[parent]: %d\n", *p);
    }
}
```

---
# 11.28.17 - C, the ultimate hipster, using # decades before it was cool.

### \#
Used to provide preprocessor instructions.  
These directives are handled by gcc first.

**`#include <LIBRARY> | "<LIBRARY>"`**
- link libraries to your code
- when you use `include`, the compiler finds the file and replaces include with what is inside (i.e. replace the include with all of the headers inside a header file)

**`#define <NAME> <VALUE>`**
- replace all occurences of NAME with VALUE
- `#define TRUE 1`

For example, for the pipe assignment you could put the following in a header file and include it in the actual file:
```c
#define READ 0
#define WRITE 1
```

**macros:**
- `#define SQUARE(x) x * x`  
- SQUARE(x) is the NAME, x * x is the VALUE
...  
`int y = SQUARE(9);` ==> `int y = 9 * 9;`

- `#define MINE(x,y) x < y ? x : y`

**conditional statement:**
Useful when using header files and there are multiple functions with the same name.
```c
#ifndef <IDENTIFIER> //if not defined
#define <IDENTIFIER>
<CODE>
#endif
```
If the identifier has been defined, ignore all the code up until the endif statement. 

For example:
```c
#ifndef PARSE_H // parse.h would break
#define PARSE_H
int count_tokens ...
...
#endif
```

---
# 11.27.17 - Redirection; how does it... SQUIRREL

### File Redirection
- Changing the usual input/output behavior of a program.

### Command line redirection:
- **`>`** redirects stdout to a file (creates a new file if the file doesn't exist)
    - overwrites the contents of the file
    - `<COMMAND> > <FILE>` i.e. `ls > file_list`
- **`>>`** redirects stdout to a file by appending
- **`2>`** redirects stderr to a file
    - overwrites the file (2>> appends)
- **`&>`** redirect stdout and stderr (&>> appends)
- **`<`** redirects stdin from a file
    - the file goes into the command
- **`|` (pipe)** redirect stdout from one command to stdin of the next
    - `ls | wc`

`$ ps > ps_file` writes whatever ps gives into a file; a good way to write logs  
`$ cat` with no file name opens stdin, and prints it to stdout  

`$ cat > new_file` the worlds simplest, and most useless text editor
- control-d to send the end of file character, and close

`$ cat new_file > newer_file` takes new_file and send it to newer_file

| # | std |  
| --- | --- |  
| 0 | stdin |  
| 1 | stdout |  
| 2 | stderr |

### Redirection in c programs
**dup - `<unistd.h>`**  
- duplicates an existing entry in the file table
- returns a new file descriptor for the duplicate entry
- `dup(fd)` returns the new file descriptor

**dup2 - `<unistd.h>`**
- `dup2(fd1, fd2)` redirects fd2 to fd1
- duplicates the behavior for fd1 at fd2
- you will lose any reference to the original fd2; that file wll be closed

`dup2(3, 1)` this gets rid of stdout  

| # | std |  
| --- | --- |  
| 0 | stdin |  
| 1 | foo.txt |  
| 2 | stderr |  
| 3 | foo.txt |   
| 4 | bar.txt |  

```c
int x = dup(1); // makes 5
dup2(3, 1) // modifies 1 to foo.txt
```
| # | std |  
| --- | --- |  
| 0 | stdin |  
| 1 | foo.txt |  
| 2 | stderr |  
| 3 | foo.txt |  
| 4 | bar.txt |  
| 5 | stdout |  

`dup2(x, 1)` puts stdout back into 1

---
# 11.21.17 - A pipe by any other name...

### named pipes 
- also known as FIFOS (first-in, first-out)
- same as unnamed pipes except FIFOs have a name that can be used to identify them via different programs
- Like unnamed pipes, FIFOs are unidirectional

### `mkfifo`
- shell command to make a FIFO
- `$ mkfifo <pipe name>`

Pipes show up as 0 bytes (`ls -l`) because they do not take up any disk space (only memory).

You can write into the same pipe from different processes, which may lead to conflicts. the pipe will not close until all connected processes have closed.  
If you read a pipe from two different processes, weird things happen (you may get alternating messages); this does not return an error and both processes will just continue running, so **don't do it**.  
If you delete a pipe while it's in use, it will still be in use (it becomes an unnamed pipe)


### mkfifo - `<sys/types.h> <sys/stat.h>`
- `mkfifo(<name>, <permissions>)`
- c function to create a FIFO
- reutrns 0 on success and -1 on failure
- once created, the FIFO acts like a regular file, and we can use open, read, write and close on it
- FIFOs will block on open (not read) until both ends of the pipe have a connection

### other things - the 2nd marking period project???
- putting together a shell
- you can put multiple commands on the same line in bash with a semicolon
- i.e. `$ echo HELLO ; ls`
- 2 commands you can't run by exec --> `cd` and `exit`

main  
|  
|----> exec  
|  
v  
continues running

---
# 11.17.17 - Ceci n'est pas une pipe

### pipe  
- a conduit between 2 separate processes on the same computer
- pipes have 2 ends; a read from end and a write to end
- unidirectional (a single pipe must be either read or write only in a process); they only go one way
- act just like files; open, close, read, write - same functions are used, and they go into the file table as well
- you can transfer any data you like through a pipe using read/write
- unnamed pipes have no external identifier (woah!!!) 

### pipe - `<unistd.h>`
- create an unnamed pipe
- returns 0 if the pipe was created, -1 if not
- opens both ends of the pipe as files (will show in the file table)

`pipe(int descriptors[2])`  
- descriptors: array that will contain the descriptors for each end of the pipe

```c
int main(){
    int fds[2];
    pipe(fds); // creates a pipe
    
    int f;
    f = fork(); // now both parent and child have an array
    
    // if child
    if(!f){
        close(fds[READ]); // for child to write to the parent 
        char s[10] = "hello!";
        sleep(2); // parent will wait for the child to run, even without using wait
        write(fds[WRITE], s, sizeof(s));
    }
    //if parent
    else{
        close(fds[WRITE]);
        char s[10];
        read(fds[READ], s, sizeof(s)); // read will block until there is something to read in the pipe
        printf("parent received: [%s]\n", s);
    }
    
    return 0;
}
```

---
# 11.15.17 - Playing Favorites
```c
void print_bytes(void *p, int size){
    int i = 0;
    unsigned char *cp = (unsigned char *)p;
    for(; i < size; i++){
        printf("byte %u: %d\n", i, *cp);
        cp++;
    }
}
int main(){
    int x = 302;
    print_bytes(&x, sizeof(x));
}
```
generally we think of ints as a single place in memory that stores an int, but ints actually have 4 bytes, so if you declare `int x = 32`, that sets x to [ 0 | 0 | 0 | 32 ]

actually, Mr. DW lied, and that is not always the case. 
- i.e. 302 is [ 46 | 1 | 0 | 0 ]  
- or at least that's how it works on his computer 

**endian-ness** of a number:
- big-endian - most significant digits first
- little-endian - least significant digits first

46 | 1 | 0 | 0
--- | --- | --- | ---
00101110 | 00000001 | 00000000 | 00000000

**`WEXITSTATUS(status)`** a macro where things are replaced; hence the CAPs
- looks at the bytes of status
- returns the process has returned when it exited

### wait - `<unistd.h>`
- waits for a specific child  
`waitpid(pid, status, options)` 
- **pid:** the pid of the specific child to look for; if -1, will wait for any child
- **options:** can set other behavior for wait; if 0, will work normally

---
# 11.14.17 - Wait for it
```c
int f -2;
f = fork();
if(f){
    sleep(1);
}
fork();
printf("post-fork I'm %d\tf: %d\tparent: %d\n", getpid(), f, getppid());
```
**orphan processes:** If a parent process has ended, `getppid()` returns 1.

While a child process is a normal executeable process with its own memory space, a thread is code that runs on its own but shares its memory with another process (only running as long as the parent is running). 

### wait - `<unistd.h>`
Stops a parent process from running until any child has provided status information to the parent via a signal.  
Usually the child has exited.

Returns the pid of the child that exited, or -1 (errno).

`wait(int *status)`
- the parameter (status) is used to store information about how the process exited 

---
# 11.13.17 - What the fork?
Some things about HW: don't be stupid and read the instructions :(  

## **Managing Sub-Processes**

### fork() - `<unistd.h>`  
Creates a separate process based on the current one.  
The new process is called a child, the original is the parent.

The child process is a duplicate of the parent process.  
All parts of the parent process are copied, including stack and heap memory, and the file table.

Returns 0 to the child and the child's pid to the parent or -1 (errno).

```c
printf("pre-fork\n");
printf("pid: %d\n", getpid());

fork();

printf("post-fork\n");
printf("pid: %d\n", getpid());
```
This prints post-fork twice, because the child process will copy the parent process at the point of the fork, thus running the same code.

There is no guarantee to the order of the processes after forking.

_**DO NOT USE FORK BOMBS:**_  
```
while(1){
    fork();
}
```

---
# 11.9.17 - Time to make an executive decision
**Do Now:** `execlp` --> `int execlp(char *file, char *arg, .../*. (char *) NULL */)`

## the exec family - `<unistd.h>`
There are a number of c functions that can be used to run other programs from within.  
Replaces the current process with the new program.

### `execlp(char *file, char *arg0, ..., (char *)NULL)`
- file should be the same as arg[0], so you basically put in the command twice
- since there can be an arbitrary number of arguments you must end with NULL
- i.e. `execlp("ls", "ls", "-l", NULL);`

### `execvp`
- an easier version of execlp so you don't have to individually put in each arg; instead you just pass an array of strings
```c
char *args[5];
args[0] = "ls";
args[1] = "-a";
args[2] = NULL;

execvp(args[0], args);
```
- **YOU MUST REMEMBER TO PUT A NULL AT THE END OF THE ARGS WITHIN THE ARRAY**

---
# 11.8.17 - Sending mixed signals

##### Self Explanatory functions
`getpid()` returns the PID of the current process number  
`sleep(<TIME>)` does this need to be explained??

### Signals
Limited way of sending information to a process.

`kill`: command line utility to send a signal to a process
- `$ kill <PID>` sends signal 15 (SIGTERM) to PID; a misnomer - does not always lead to a program quitting
- `$ kill -<SIGNAL> <PID>` sends SIGNAL to PID

`killall [-<SIGNAL>] <PROCESS>`
- sends SIGTERM (or SIGNAL if provided) to all processes with PROCESS as the name

basically; `kill` is for PID, `killall` is for name

### Signal handling in c programs `<signal.h>`

**kill**  
- `kill(<PID>, <SIGNAL>)` 
- returns 0 on succes or -1 (errno) on failure

**signalhandler**  
- to intercept signals in a c program you must create a signal handling function
- some signals (i.e. SIGKILL) cannot be caught
- `static void sighandler(int signo)`
- must be static, must be void, must take a single int parameter
- static: the function can only be called from within the file it is defined

**signal**
- after you create the function, you attach the signals to it using the signal function
- `signal

```c
static void sighandler(int signo){
  if(signo == SIGINT){
    printf("");
  }
  if(signo == SIGSTOP){
    printf("");
  }
}

int main(){
  signal(SIGINT,
}

---
# 11.6.17 - Are your processes running? Then you should go out and catch them!

### fgets - `<stdio.h>`
Read in from a file stream and store it in a string.  
`fgets(<DESTINATION>, <BYTES>, <FILE POINTER>)`  
`fgets(s, n, f)` reads at most `n-1` characters from file stream `f` and stores it in `s`  

#### File pointer
- File * type, more complex than a file descriptor
- stdin is a FILE * variable; can be used with fgets

Stops at newline, end of file, or the byte limit.  
If applicable, keeps the newline character as part of the string, appends NULL after.  
`fgets(s, 256, stdin)` reads a line of text from standard in

```c 
printf("enter data: ");
fgets(s, sizeof(s), stdin);

printf("s: %s", s);
```

### sscanf - `<stdio.h>`  
Scan a string and extract values based on a format string.  
`sscanf(<SOURCE STRING>, <FORMAT STRING>, <VAR 1>, <VAR 2>...)`  

## Processes
Every running program is a process. A process can create subprocesses, but these are no different from regular processes.  
A processor can handle 1 process per cycle (per core).  
"Multitasking" appears to happen because the processor switches between all the active processes quickly.  

### pid 
Every process has a unique identifier call the pid.  
pid 1 is the init process (if you shut down pid 1, you shut down your computer).  
Each entry in the /proc directory is a current pid.  
`ps` shows all the processes running that YOU own and are attached to terminal session.  
`ps -a` shows everything?  
`ps -ax` processes that are not attached to a terminal session - actually shows everything

---
# 11.3.17 - Input? fgets about it!

```c
char * get_perm_string(mode_t mode){
     mode_t perms[3];
     int i;
     char *perm_string = (char *)malloc(10);
     perm_string[9] = 0;

     perms[0] = (mode & 0b111000000) >> 6;
     perms[1] = (mode & 0b111000) >> 3;
     perms[2] = (mode & 0b111);

     for(i = 0; i < 3; i++){
     	   if(perms[i] & 0b100) perm_string[0 + i*3] = '';
     }
}
```

### Command Line Arguments
- `int main(int argc, char *argv[])`
- program name is considered the first command line argument
- `argc`: the number of command line arguments
- `argv`: array of actual arguments

### scanf - `<stdio.h` (not really great, but ok)
- `scanf(<FORMAT STRING>, <VAR 1>, <VAR 2> ...)`
- reads in data from stdin using the format string to determine types and puts the dat ain eachv variable
- variables must be pointers
```c
int x;
float f;
double d;
scanf("%d %f %lf, &x, &f, &d);
```

---
# 11.1.17 - Where do compsci priests keep their files? In directory!

### Directories
- A *nix directory is a file containing the names of the files within the directory along with basic information, like file type.
- Moving files into/out of a directory means changing the directory file, not actually moving any data (with the exception of drives; i.e. a flash drive).
- All linux box directories are the same size.

### opendir - `<dirent.h>`
- Open a directory file (so that you can read it later).
- This will not change the current working diretory (cwd), it only allows your program to read the contents of the directory file.
- `opendir(<PATH>)`
- Returns a pointer to a directory **stream** (DIR *).
- You cannot create or write to a directory with opendir, only read.

### closedir - `<dirent.h>`
- closes the directory stream and frees the pointer associated with it.
- `closedir(<DIRECTORY STREAM *>)`

### readdir - `<dirent.h>`
- `readdir(<DIRECTORY STREAM *>)`
- returns a pointer to the next entry in a directory stream, or NULL if all entries have already been returned.

#### struct dirent - `<sys/types.h>`
- directory struct that contains the information stored in a directory file.
Some of the data members:
- `d_name`: file name
- `d_type`: file type as an int

For example:
```c
DIR * d;
d = opendir("somedir");

struct dirent *entry;
entry = readdir(d);

closedir(d);
```
You do not have to worry about malloc and free (this is done with opendir and closedir).
File names are limited to 256 characters. 

---
# 10.30.17 - Seek and ye shall find

### lseek - `<unistd.h>`
- set the current position in an open file
- `lseek(<FILE DESCRIPTOR>, <OFFSET>, <WHENCE>)`

**OFFSET**: number of bytes to move the position by. Can be negative.

**WHENCE**: where to measure the offset from
- `SEEK_SET`: offset is evaluated from the beginning of the file
- `SEEK_CUR`: offset is relative to the current position in the file
- `SEEK_END`: offset is evaluated from the end of the file

Returns the number of bytes the current position is from the beginning of the file, or -1 (errno).

For the dev-random assignment, instead of closing and reopening to read, use lseek to set place to the beginning of the file again.

### stat - `<sys/stat.h>`
- get information about a file (metadata)
- `stat(<PATH>, <STAT BUFFER>)`
- does not require a file descriptor (you can stat a file without opening it)

```c
struct stat sb;
stat("foo", &sb);
```

**STAT BUFFER**
- Must be a pointer to a struct stat
- All the file information gets put into the stat buffer.
Some of the fields in struct stat:
- `st_size`: file size in bytes
- `st_uid, st_gid`: user ID, group ID
- `st_mode`: file permissions
- `st_atime, st_mtime`: last access, last modification
    - these are time_t variables. We can use functions in time.h to make sense of them.
    - `ctime(<TIME>)` returns time as a string (TIME is type time_t)
    
---
# 10.26.17 - Read your writes!

### umask - `<sys/stat.h>`
- Set the file creation permission mask
- By default, created files are not given the exact permissions provided in the mode argument to open. Some permissions are automatically shut off.
Umask is applied in the following way:
`new_permissions = ~mask & mode`
The default mask is 0002.

mode   = 666 -> 110 110 110  
umask  = 022 -> 000 010 010  

~umask =     -> 111 101 101  
& mode =     -> 110 110 110  
- - - - - - - - - - - - - -  
       = 644 -> 110 100 100

**`umask(<MASK>)`**
- You can define the umask using a 3 digit octal.
- `umask(0000);` allows everything

### read - `<unistd.h>`
- read in data from a file
- `read(<FILE DESCRIPTOR>, <BUFFER>, <AMOUNT>)`; read AMOUNT bytes from file
- `read(fd, buff, n)`
- Read n bytes from the fd's file and put that data into buff
- Returns the number of bytes actually read. Returns -1 and sets errno if unsuccessful.
- BUFFER must be a pointer

### write - `<unistd.h>`
- write data to a file
- `write(<FILE DESCRIPTOR>, <BUFFER>, <AMOUNT>)`
- `write(fd, buff, n)`
- Write n byes from buff into fd's file

---
# 10.25.17 - Opening up a world of possibilities

### open - `<fcntl.h>`
- Add a file to the file table and returns it file descriptor.
- If open fails, -1 is returned, extra error information can be found in errno, an int variable that can be found in `<errno.h>`. Using strerror (in string.h) on errno will treturn a string description of the error.
**`open(<PATH>, <FLAGS>, <MODE>)`**
#### mode
- Only used when creating a file. Set the new file's permissions using a 3 digit octal number.

#### flags
- Determine what you plan to do with the file.
Use the following constants:
- O_RDONLY, O_WRONLY, O_ROWR, O_APPEND, O_TRUNC
- O_CREAT: must be used if the file does not exist
- O_EXCL: when combined with O_CREAT, will return an error if the file exists

Each flag is a number; to combine flags we use bitwise OR
- O_WRONLY = 1           00000001
- O_APPEND = 8           00001000
- O_WRONLY | O_APPEND =  00001001

---
# 10.24.17 - File this under useful information

### file permissions
- 3 types: read, write, execute
Permissions can be represented as 3-digit binary or 1-digit octal numbers.
- 100 <==> 4 => read only
- 111 <==> 7 => read, write, execute

There are 3 permission "areas":
- user, group, others (everyone else)
- each area can have its own permissions
- 0644 => user: read + write, group: read:, other: read

Command: `ls -l` to show file permissions (plus some extra stuff)
- `user-group-other` --> i.e. `-rwxr-xr-x`
To change file permissions: `chmod`
- i.e. `chmod -x <filename>`, `chmod 644 <filename>` (rw-r-r), `chmod 600 <filename>` (-rw-------)
- 644 is probably the standard permissions given by the system

### File table
- A list of all the files being used by a program while it is running.
- Contains basic information like the file's location and size.
- The file table has a limited size, which is a power of 2 (commonly 256). `getdtablesize()` will return this value. This is a hard limit (256 files opened at the same time is a little much).
- Each file is given an integer index, starting at 0. This is referred to as the file descriptor.

There are 3 files always open in the table:
- 0 or STDIN_FILENO: stdin
- 1 or STDOUT_FILENO: stdout
- 2 or STDERR_FILENO: stderr

---
# 10.23.17 - A bit of wisdom

### base formatting characters
- **%o**: octal integer
- **%x**: hexadecimal integer

You can define native integers in bases 2, 8 and 16 by using the following prefixes.
- **0b**: binary
- **0**: octal
- **0x**: hexadecimal
```c
int b = 0b1011; // prints 11
int o = 0122;   // prints 82
int x = 0xa3;   // prints 163
```
THIS DOES NOT CHANGE THE VALUE  
`0b1011 == 013 == 0xB == 11`

## Bitwise Operators
Evaluated on each individual bit of a value.

### >> right shift
- i.e. `x >> 1`
- move all bits to the right, add 0s in the front
```c
unsigned char cb = 0b11001010; // prints 202
cb = cb >> 1;                  // prints 101
cb = cb >> 1;                  // prints 50
cb = cb << 1;                  // prints 100
```

### << left shift
- i.e. `x << 2`
- move all bits to the left, add 0s in the back

Left shift and right shift will not overflow end bits into adjacent memory.

### other bitwise operators

**~negation**
- flip every bit
- i.e. ~x

**| or**
- perform or for each pair of bits in (a,b)
- i.e. a | b

**& and**
- perform and for each pair of bits in (a, b)
- i.e. a & b
- 10110111 & 01011100 = 00010100

**^ xor**
- perform xor for each pair of bits in (a, b)
- i.e. a ^ b
- one or the other must be true; not both
- 10110111 ^ 01011100 = 11101011

xor swap:
1. a = a ^ b;
2. b = a ^ b;
3. a = a ^ b;

---
# 10.19.17 - Get Dem Bugs

Valgrind - memory issues

## GDB - GNU Debugger
- more general, like an all-purpose debugger
- an interactive shell to run your program through
```
gcc -c broken.c
gdb ./a.out

(gdb) run

// this shows the line in which the error occured, plus some buffer space
(gdb) list

// you can set break points
(gdb) break <line number>
(gdb) break <function name>

// print variable values and arrays
(gdb) print <variable>
(gdb) print <array>

// continue to next break point
(gdb) continue

// to end session
(gdb) quit
```

### Commands from the gdb shell
- run: runs the program until it ends / crashes
- list: show the lines of code run around a crash
- print <VAR>: print a variable
- backtrace: show th current stack
- break <line number>: creates a breakpoint at the given line

Running a program in pieces:
- continue: run the program until the next breakpoint
- next: run the next line of the program only
- step: run the next line of the program; if that is a function call, run the next line of that function

---
# 10.18.17 - Back to the Grind

### valgrind
- tool for debugging memory issues in C programs
- You must compile with -g in order to use valgrind (and similar tools)
Usage:
```
gcc -g foo.c
valgrind --leak-check=yes ./a.out
```
This may create a separate debugging file (depending on your OS), which you should probably avoid adding to github. 

---
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
