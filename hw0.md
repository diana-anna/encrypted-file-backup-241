Homework 0
----------

### Watch the videos and write up your answers to the following questions

**Important!**

The virtual machine-in-your-browser and the videos you need for HW0 are here:

<http://cs-education.github.io/sys/>

Questions? Comments? Use Piazza: <https://piazza.com/illinois/spring2019/cs241>

The in-browser virtual machine runs entirely in Javascript and is fastest in Chrome. Note the VM and any code you write is reset when you reload the page, **so copy your code to a separate document.** The post-video challenges are not part of homework 0 but you learn the most by doing rather than just passively watching - so we suggest you have some fun with each end-of-video challenge.

HW0 questions are below. Please use this document to write the answers. This will be hand graded.

### Chapter 1

In which our intrepid hero battles standard out, standard error, file descriptors and writing to files

1.  **Hello, World! (system call style)** Write a program that uses `write()` to print out “Hi! My name is &lt;Your Name&gt;”.

```c
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
	write(1, "Hello, World!",13);
	return 0;
}
```

2.  **Hello, Standard Error Stream!** Write a function to print out a triangle of height `n` to standard error. Your function should have the signature `void write_triangle(int n)` and should use `write()`. The triangle should look like this, for n = 3:

```
*
**
***
```

```c
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void write_triangle(int n) {
	int i = 1;
	while (i <= n) {
		int j;
		for ( j = 0; j < i; j++) {
			write(2, "*", 1);
		}
		write(2, "\n", 1);
		i++;
	}
}

int main() {
	write_triangle(3);
	return 0;
}
```

3.  **Writing to files** Take your program from “Hello, World!” modify it write to a file called `hello_world.txt`. Make sure to to use correct flags and a correct mode for `open()` (`man 2 open` is your friend).

```c
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
	mode_t mode = S_IWUSR | S_IRUSR;
	int fd = open("hello_world.txt", O_CREAT | O_TRUNC | O_RDWR, mode);
	write(fd, "Hello, World!", 13);
	close(fd);
	return 0;
}

```

5. **Not everything is a system call** Take your program from “Writing to files” and replace `write()` with `printf()`. *Make sure to print to the file instead of standard out!*

```c
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
	mode_t mode = S_IWUSR | S_IRUSR;
	close(STDOUT_FILENO);
	int fd = open("hello_world.txt", O_CREAT | O_TRUNC | O_RDWR, mode);
	printf("Hello, World!\n");
	close(fd);
	return 0;
}
```

6.  What are some differences between `write()` and `printf()`?

```c
write() is more primitive. It is in the kernel and writes in bytes. printf() is more user-friendly and provides a layer of abstraction where the user can input data of any format. printf() is a function from a library that calls write() and makes displaying input easier to understand.
```

### Chapter 2

Sizing up C types and their limits, `int` and `char` arrays, and incrementing pointers

1.  How many bits are there in a byte?

```c
8 bits in a byte.
```

2.  How many bytes are there in a `char`?

```c
1 byte in a char.
```

3.  How many bytes the following are on your machine? 

* `int`: 4
* `double`: 8 
* `float`: 4
* `long`: 4
* `long long`: 8

4.  On a machine with 8 byte integers, the declaration for the variable `data` is `int data[8]`. If the address of data is `0x7fbd9d40`, then what is the address of `data+2`?

```c
0x7fbd9d50
```

5.  What is `data[3]` equivalent to in C? Hint: what does C convert `data[3]` to before dereferencing the address? Remember, the type of a string constant `abc` is an array.

```c
data[3] is equivalent to data + 3.
```

6.  Why does this segfault?

```c
char *ptr = "hello";
*ptr = 'J';
```

The assembly code and instructions for the CPU have read-only access and cannot write to constant variables.

7.  What does `sizeof("Hello\0World")` return?

```c
12 since there are 11 characters and a final \0.
```

8.  What does `strlen("Hello\0World")` return?

```c
5 since it stops counting characters as soon as it reaches the first \0.
```

9.  Give an example of X such that `sizeof(X)` is 3.

```c
char *X = “Hi”;
```

10. Give an example of Y such that `sizeof(Y)` might be 4 or 8 depending on the machine.

```c
int Y = 5;
```

### Chapter 3

Program arguments, environment variables, and working with character arrays (strings)

1.  What are two ways to find the length of `argv`?
argc, sizeof(argv[1]).

2.  What does `argv[0]` represent?
the program execution ./program

3.  Where are the pointers to environment variables stored (on the stack, the heap, somewhere else)?
stack

4.  On a machine where pointers are 8 bytes, and with the following code:

    ``` c
    char *ptr = "Hello";
    char array[] = "Hello";
    ```

    What are the values of `sizeof(ptr)` and `sizeof(array)`? Why?

```c
sizeof(ptr) would return 8, since ptr is a char pointer and pointers are 8 bytes.
sizeof(array) would return 6, since the array contains 5 chars (1 byte each) and one terminating symbol.
```

5.  What data structure manages the lifetime of automatic variables?
Automatic variables are managed by the stack since they are declared inside of the function and the function reserves its own stack frame.

### Chapter 4

Heap and stack memory, and working with structs

1.  If I want to use data after the lifetime of the function it was created in ends, where should I put it? How do I put it there?
To use data after a function dies, you can either declare it as static, or use malloc to allocate memory and then free to free the memory when you are done using the data stored.

2.  What are the differences between heap and stack memory?
Stack memory is where variables are automatically stored only until they go out of scope (the function they’re in ends). Heap memory is managed by the user, who is in charge of allocating and freeing space for variables. Heap memory is only managed via pointers, so it is slower than the stack. Also, stack memory restricts the sizes of variables while the heap does not.

3.  Are there other kinds of memory in a process?
There is also a text segment which contains executables, initialized data segment for static and global variables, and uninitialized data segment for variables that are declared but not initialized.

4.  Fill in the blank: “In a good C program, for every malloc, there is a \_\_\_”.
free

5.  What is one reason `malloc` can fail?
You have run out of space on the heap.

6.  What are some differences between `time()` and `ctime()`?
time() returns the time in seconds, while ctime() converts a time_t object into a char string that is easier for humans to read.

7.  What is wrong with this code snippet?

``` c
free(ptr);
free(ptr);
```
This code attempts to free a pointer after it was already freed. This will cause a segfault.

8.  What is wrong with this code snippet?

``` c
free(ptr);
printf("%s\n", ptr);
```
This code is trying to print ptr after it was freed, so technically anything could be stored in ptr now, we don’t know.

9.  How can one avoid the previous two mistakes?
Avoid 7. by making sure there is one free() for every malloc().
Avoid 8. by setting ptr = NULL after freeing it.


10. Use the following space for the next four questions

```c
// 10
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

struct Person {
	char* name;
	int age;
	struct Person **friends;
};

typedef struct Person person_t;
// 12
person_t* create(char* n, int a) {
	person_t* p = (person_t*) malloc(sizeof(person_t));
	p->name = strdup(n);
	p->age = a;
	p->friends = malloc(10 * sizeof(person_t)); // allocate for 10 friends
	return p;
}
// 13
void destroy(person_t* p) {
	free(p->name);
	free(p->friends);
	memset(p, 0, sizeof(person_t));
	free (p);
}
int main() {
// 11
person_t* agent_smith = create("Agent Smith", 128);
	person_t* sonny_moore = create("Sonny Moore", 256);
	
	agent_smith->friends[0] = sonny_moore;
	sonny_moore->friends[0] = agent_smith;
	
	destroy(agent_smith);
	destroy(sonny_moore);
	
	return 0;
}
```

* Create a `struct` that represents a `Person`. Then make a `typedef`, so that `struct Person` can be replaced with a single word. A person should contain the following information: their name (a string), their age (an integer), and a list of their friends (stored as a pointer to an array of pointers to `Person`s). 

*  Now, make two persons on the heap, “Agent Smith” and “Sonny Moore”, who are 128 and 256 years old respectively and are friends with each other. Create functions to create and destroy a Person (Person’s and their names should live on the heap).

* `create()` should take a name and age. The name should be copied onto the heap. Use malloc to reserve sufficient memory for everyone having up to ten friends. Be sure initialize all fields (why?).

* `destroy()` should free up not only the memory of the person struct, but also free all of its attributes that are stored on the heap. Destroying one person should not destroy any others.


### Chapter 5

Text input and output and parsing using `getchar`, `gets`, and `getline`.

1.  What functions can be used for getting characters from `stdin` and writing them to `stdout`?
getc(), putc()

2.  Name one issue with `gets()`.
There is no way to tell gets() that it should not accept data larger than what was specified with a buffer.

3.  Write code that parses the string “Hello 5 World” and initializes 3 variables to “Hello”, 5, and “World”.

```c
int main() {
	
	char* str = "Hello 5 World";
	char* hello[6];
	int five;
	char* world[6];
	
	int result = sscanf(str, "%s %d %s", hello, &five, world);
	
	printf("Result = %d\n hello = %s\n five = %d\n world = %s\n", result, hello, five, world);
	
	return 0;
}
```

4.  What does one need to define before including `getline()`?
#define _GNU_SOURCE
One must also define a buffer variable and a capacity.

5.  Write a C program to print out the content of a file line-by-line using `getline()`.

```c
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define _GNU_SOURCE
int main() {
	file_dest = open("file.txt", O_CREAT | O_TRUNC | O_RDWR, S_IWUSR | S_IRUSR);
	char* buffer = NULL;
	size_t capacity = 0;
	while(getline(&buffer, &capacity, file_dest) != -1) {
		printf("%s", buffer);
	}
	
	free(buffer);
	return 0;
}
```

### C Development

These are general tips for compiling and developing using a compiler and git. Some web searches will be useful here


1.  What compiler flag is used to generate a debug build?
-g

2.  You fix a problem in the Makefile and type `make` again. Explain why this may be insufficient to generate a new build.
There may already be compiled object files that you don’t want to use anymore and want deleted. Instead, call make new which cleans.

3.  Are tabs or spaces used to indent the commands after the rule in a Makefile?
tabs

4.  What does `git commit` do? What’s a `sha` in the context of git?
git commit saves changes to the local repository. SHA is a secure hash algorithm that identifies an object and specific commits.

5.  What does `git log` show you?
git log shows a history of commits.

6.  What does `git status` tell you and how would the contents of `.gitignore` change its output?
git status shows you the (staged) changes and files tracked by git in the working directory.

7.  What does `git push` do? Why is it not just sufficient to commit with `git commit -m ’fixed all bugs’ `?
git push pushes the changes made in a commit in a local repository to a remote repository. git commit only updates the local repository.

8.  What does a non-fast-forward error `git push` reject mean? What is the most common way of dealing with this?
This error occurs when git cannot make a change to the remote repository without losing commits. This can be fixed by fetching and merging the remote repository.

### Optional: Just for fun

-   Convert your a song lyrics into System Programming and C code covered in this wiki book and share on Piazza.

-   Find, in your opinion, the best and worst C code on the web and post the link to Piazza.

-   Write a short C program with a deliberate subtle C bug and post it on Piazza to see if others can spot your bug.

-   Do you have any cool/disastrous system programming bugs you’ve heard about? Feel free to share with your peers and the course staff on piazza.
