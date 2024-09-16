
[180 min] practice assignment / program in c  
due sep 24 by 11:59pm points 100 submitting a file upload file types tar available until oct 3 at 11:59pm  
this practice assignment is not graded, it does not count towards your course grade. it is for diagnostics only. but you are strongly encouraged to work on it as though it is real assignment, as you will need to apply what you learn here in all subsequent coding assignments.

motivation  
in the first part of this practice assignment, you will have an opportunity to practice developing c programs that have functions.

in the second part, you will have an opportunity to practice developing c programs that take command line arguments and that are called from within shell scripts. the objective is to develop a practical understanding of operating system concepts related to command-line utilities and shell scripting. you will be tasked with creating a c program that accepts multiple command-line arguments and integrates it into a shell script.

learning outcomes  
- learn basic c programming  
- build functions with parameters  
- use the command line to compile and execute c programs  
- appreciate the use of makefiles  
- learn shell scripting  
- use command line arguments  

## part 1: c program functions  
- (3 pts) create a new source file called `learnc.c`.  
- (2 pts) add a proper header with your information, the name of the course, and the assignment title. an example header is shown below:

```c
/*
* filename.c / assignment name
*
* paws husky / cs5600 / northeastern university
* fall 2024 / sep 8, 2024
*
*/
```

- (2 pts) add a `main()` function and include all necessary header files.  
- (3 pts) create a makefile and ensure that it compiles your code. the resulting executable must be called `learnc`. ensure that the executable runs.  
- (25 pts) create the following three functions that convert between mileage/efficiency metrics:
  - `mpg2kml()` that takes miles per gallon as an argument and returns the equivalent kilometers per liter. define the signatures as needed.
  - `mpg2lphm()` that takes miles per gallon as an argument and returns the equivalent number of liters per 100 kilometers. define the signatures as needed.
  - `lph2mpg()` that takes the liters per 100 kilometers and returns miles per gallon. define the signatures.
  
- (5 pts) add some test cases to your `main()` function to test that the functions work. display the results (from `main()` and not in the functions).  
- (3 pts) add some test cases to your `main()` to ensure that your functions work even when "bad" argument values are passed. use your judgement as to what "bad" arguments might be. determine which problems with parameters you can actually detect in c.  
- (2 pts) make sure that your functions have headers (look up common standards) and that your code is professionally documented and formatted.  
- (5 pts) move the functions from the source file `learnc.c` into a separate source file called `mpg2km.c` (leaving only `main()` and the test cases in the original file). create a new file that contains the function signatures. include only the header file in the original file now only containing `main()`. this setup is most common: functions are "libraries" that are in separate source files and their signatures are in header (include) files. update your makefile so it all compiles and links.

## part 2: command line arguments  
in this part, you will create a c program that serves as a command-line utility. this utility will accept multiple command-line arguments and perform a specific task based on those arguments. you will then integrate this c program into a shell script, demonstrating how command-line utilities can be seamlessly used within the shell environment.

- (30 pts) write a c program that accepts multiple command-line arguments. your program must exit with an error code of `0` if there is no error. your c program should use appropriate error handling mechanisms and provide informative error messages. the executable should have the name "arger".

for example, the program should be callable like this:

```bash
% arger -u help is on the way
```

and display:

```bash
HELP IS ON THE WAY
```

and as another example:

```bash
% arger -cap help is on the way
```

and display:

```bash
Help Is On The Way
```

the program should perform one of the following tasks based on the arguments provided:
- task 1: if the argument `-u` is provided, the program should display the remaining text in all upper case.
- task 2: if the argument `-l` is provided, the program should display the remaining text in all lower case.
- task 3: if the argument `-cap` is provided, the program should display the remaining text with the first letter of each word capitalized and all other characters of the word in lower case.
- task 4: if the first argument is not one of the above or there are no characters afterward, then the program should display an appropriate message and exit with an error code of `-1`.

you may not use any libraries other than `stdio`.

- (20 pts) call the program from within a shell script named `dumper` (you may use any shell scripting mechanism of your choice, e.g., `csh`, `bourne`, etc.) and try passing the various arguments and that uses the error code returned by the program. the exact mechanism and what the program does is up to you and not germane to the assignment's objective.

## hints  
remember that in c/c++ characters can also be treated as their integer values; they are encoded using ascii where `'A'` is `65`, `'B'` is `66`, `'C'` is `67` and so on... lower case letters are `97` for `'a'`, `98` for `'b'`, so they are `32` apart... so it is a simple computation; all non-letter characters should be left as is.

## practice submission  
create a single `.tar` file (`username-cs5600-f24-week2.tar`) that contains your source file and the makefile:

- source and header files containing all of your code and signatures  
- makefile  

the make targets should be: `all`, `learnc`, and `arger`.
```