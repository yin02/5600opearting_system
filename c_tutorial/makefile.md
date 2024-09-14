## make file
```py
# 变量定义
CC = gcc
CFLAGS = -Wall -g

# CC：定义了编译器，通常是 gcc 或者 clang。

# CFLAGS：定义了编译时的选项，比如 -Wall 用于启用所有警告，-g 用于生成调试信息。

# 目标规则
all: myprogram
#all: myprogram：这是默认目标，make 会首先执行这个目标，它依赖于 myprogram，即编译生成程序。

# 生成 myprogram 的规则
myprogram: main.o utils.o
    $(CC) $(CFLAGS) -o myprogram main.o utils.o
# myprogram: main.o utils.o：这个目标表示，生成 myprogram 需要 main.o 和 utils.o，然后会通过 gcc 命令链接生成最终的可执行文件。
# -o：这个选项用于指定输出文件的名称。它告诉编译器或链接器生成一个特定名称的可执行文件或目标文件。output
# 例如，在这条命令中，它会将链接后的输出文件命名为 myprogram。
#.o：扩展名 .o 表示的是目标文件（object file）。这些文件是由源代码文件（如 .c 文件）编译生成的，它们是编译过程中的中间文件，后续会被链接成最终的可执行文件。

# main.o 和 utils.o：这是单独编译 main.c 和 utils.c 的规则，生成对应的 .o 对象文件。

# 生成 main.o 的规则
main.o: main.c
    $(CC) $(CFLAGS) -c main.c

# -c：这个选项用于告诉编译器只编译源文件，而不链接！！。这意味着它会将源文件（如 main.c）编译成目标文件（即 .o 文件），而不会生成可执行文件。
# 例如，main.c 会被编译为 main.o。

# 生成 utils.o 的规则
utils.o: utils.c
    $(CC) $(CFLAGS) -c utils.c

# 清理生成的中间文件
clean:
    rm -f *.o myprogram
#clean：用于删除编译生成的中间文件和可执行文件，保持目录干净
#rm：这是 Linux/Unix 系统中的一个命令，表示删除文件或目录。
# -f：这是 rm 命令的选项，表示“强制删除”（force）。即使文件不存在也不会提示错误信息，并且在没有确认的情况下强制删除文件。
# *.o：表示删除当前目录下所有以 .o 结尾的文件，这些通常是编译过程中生成的目标文件（object files）。
#* 所有
# myprogram：表示删除名为 myprogram 的可执行文件

```
```
gcc -o myprogram myprogram.o
```
-o 可以翻译为**“输出为”或“指定输出文件为”**

的意思是：
使用 gcc 编译器将目标文件 myprogram.o 链接生成一个名为 myprogram 的可执行文件。


The ‘make’ command is run from the terminal (aka a shell).

```
% make target
```
这边的target只是随便打得
When ‘make’ is run, it looks for a file named Makefile in the current folder. Note the capitalization of the file – it is important.

Let’s do the classic “Hello World” program as a makefile. Below is the contents of a really simple makefile (called Makefile).
```
hello:
    echo "Hello, World"
```

Important: Rules must be indented by a TAB and not spaces. So, hit the `TAB` key in your keyboard and do not type spaces with the spacebar on your keyboard. This is one of the most common mistakes that beginners run into.
Once you have the makefile, “execute it” by running the make program.

```
make
```
可以的，试了一下，就是直接makefile，这个名字![alt text](image-4.png)

Makefiles are build files for projects and they contains all command to compile, link, copy, or move files and run scripts. Below is a simple example.

```py
all: multithread
# all: target
#all 目标：这是默认目标，执行 make 时会首先执行这个目标。它依赖于 multithread，因此会先检查并构建 multithread。
multithread: multithread.c# "multithread" 是编译生成的目标，依赖于 "multithread.c" 源文件。
# target: depend on multithread.c
    gcc -o multithread multithread.c
    # 编译命令，使用 gcc 编译器将 "multithread.c" 源文件编译成名为 "multithread" 的可执行文件。object
# "-o multithread" 指定了输出的可执行文件名为 "multithread"。
```

So, in the above example, all depends on multithread to be up-to-date, which, in turn, depends on multithread.c. So, if multithread.c changes, then it forces the rules to be applied and any commands below that rule are run. In the above example, that would be the compilation and linking with gcc.


目标文件（Object File）：

目标文件通常是 .o 或 .obj 结尾的文件。它是通过编译器将源代码（如 .c 文件）翻译为机器代码后生成的中间文件。
目标文件还未完成链接，不能独立运行。它通常包含单个或多个模块中的机器指令和数据。
目标文件是编译阶段的结果，需要链接器将其与其他目标文件或库进行链接，才能生成可执行文件。
可执行文件（Executable File）：

可执行文件是通过编译器和链接器将所有目标文件、库文件组合在一起后生成的，最终可以直接运行的文件。
在Unix/Linux系统上，可执行文件通常没有特定扩展名；而在Windows系统上，通常是 .exe 文件。
当你运行 gcc -o myprogram myprogram.o，就会生成一个名为 myprogram 的可执行文件，可以直接通过命令行执行 ./myprogram。

编译过程：
编译（Compile）：源文件（如 multithread.c）被编译器编译为目标文件（如 multithread.o）。
链接（Linking）：多个目标文件或库被链接器结合在一起，最终生成可执行文件（如 multithread）。
因此，目标文件是编译的中间产物，而可执行文件是最终可以执行的程序。


To initiate the full build, call make all or simply make as all is the first target.

## Makefile Structure and Syntax
A Makefile consists of a set of rules. A rule generally looks like this:
```
target: prerequisites
    command
    command
    command
```

### Target ‘clean’
Most makefiles includes a rule for a target named clean that is invoked with make clean. It’s purpose is to clean up your development environment remove the output of other targets. Note that is should not be the first target as it would then be the default target; the default target should be all.
```c
clean:
    rm -f multithread  # 强制删除名为 "multithread" 的文件，无论它是否存在，不提示错误信息
    touch multithread.c  # 更新 "multithread.c" 文件的时间戳，如果文件不存在，则创建空文件
```
rm -f multithread：强制删除名为 multithread 的可执行文件，-f 选项确保不会提示错误，即使文件不存在。
touch multithread.c：更新 multithread.c 文件的时间戳，如果该文件不存在，touch 命令将创建一个新的空文件。

源文件：myprogram.c 是包含源代码的文件。
目标文件：myprogram.o 是编译器将 myprogram.c 转换成的中间文件，包含机器码，但无法直接运行。
可执行文件：myprogram 是通过链接器将目标文件和其他库链接生成的最终可执行文件。



```
target: dependencies
    [commands]
```

- `Target`: The name of the target you want to build.
- `Dependencies`: The files or targets that the current target depends on. These represent the `prerequisites` for `building the target`.
- `Commands`: The shell commands to be executed when make builds the target. These commands are indented with tabs.

```py
# Define the target and its dependencies
my_program: main.o

# Compile the program
main.o: main.c
    gcc -o main.o -c main.c

# Clean up generated files
clean:
    rm -f my_program main.o
```

In this Makefile:

my_program is the target, and it depends on main.o.
main.o is another target, which depends on main.c.
The gcc command is used to compile main.c into an object file (main.o).
The clean target is used to remove the generated files when you run make clean.

## How `make` Works
When you run make in the command line without specifying a target, it looks for a special target called the `default target`. By convention, this is often named `all`. If you have an all target in your Makefile, make will execute the commands associated with that target.
```
all: my_program
```
When you run make without arguments, it will execute the `all` target, which depends on my_program. Thus, it will build `my_program` and its dependencies.


# 3.1 Targets

Targets in a Makefile represent the artifacts or actions that you want to create or perform. They are the focal points of your build process and can include:

- Executable programs.
- Object files (compiled source files).
- Documentation.
- Distribution packages.
- Custom tasks like cleaning or testing.

Each target corresponds to a specific task or output that `make` should generate. Targets can be created by defining their names in your Makefile.

## 3.1.1 Example Targets

In a C/C++ project, you might define targets like:

- **`my_program`**: To build the final executable.
- **`main.o`, `util.o`**: To compile individual source files into object files.
- **`clean`**: To remove generated files and clean up the project.

# 3.2 Rules

Rules in a Makefile define how to create a target from its dependencies. A rule consists of three main parts:

1. **Target**: The name of the target that you want to build.
2. **Dependencies**: The files or targets that the current target depends on. These represent the prerequisites for building the target.
3. **Commands**: The shell commands to be executed when `make` builds the target from its dependencies. These commands are typically indented with tabs.

## 3.2.1 Example Rule Structure
Here’s the structure of a rule:
```
target: dependencies
    [commands]
```

## 3.2.2 Example Rule
Consider a simple rule to compile a C source file into an object file:
```c
main.o: main.c
    gcc -o main.o -c main.c
```
In this rule:
 - main.o is the target we want to build.
 - main.c is the dependency (source file) that main.o depends on.
- The command gcc -o main.o -c main.c is executed to build main.o from main.c.
## 3.3 Practical Usage
Makefiles become powerful when you have multiple rules and targets, and they help you automate complex build processes. Here are some practical tips:

### 3.3.1 Building All Targets
By convention, you can create an all target that depends on all the main targets in your project. When you run make, it builds the all target, which in turn builds everything else. For example:
```
all: my_program

my_program: main.o util.o
    gcc -o my_program main.o util.o

main.o: main.c
    gcc -o main.o -c main.c

util.o: util.c
    gcc -o util.o -c util.c
```
3.3.2 Cleaning Up
A clean target is handy for removing generated files and cleaning up your project. For example:
```
clean:
    rm -f my_program main.o util.o
```

3.3.3 Default Target
You can specify a default target by including it at the beginning of your Makefile. For example:
```
.DEFAULT_GOAL := all
```
This sets the all target as the default, so running make without any arguments will build all.

### 3.3.4 Phony Targets
`Phony targets` are targets that don’t represent actual files. They are used for tasks like `cleaning, testing`, or documentation generation. To declare a target as phony, use the `.PHONY` special target. For example:

```
.PHONY: clean test docs

clean:
    rm -f my_program main.o util.o

test:
    ./run_tests.sh

docs:
    doxygen
In this example, `clean`, `test`, and `docs` are phony targets.
```

### 3.4 Summary
In summary, Makefiles are composed of rules and targets. Rules specify how to build targets from their dependencies, and targets represent the artifacts or actions you want to create or perform. Understanding the structure and usage of rules and targets is essential for effectively automating tasks and managing dependencies in your software projects.

Certainly, let’s expand on the fourth section, “Variables in Makefiles,” to provide a more detailed explanation of variables and their practical usage in Makefiles.


[variables in makeFile](http://artificium.us/lessons/01.programming/l-1-901-make/l-1-901.html)还是要看看的

### Recursive Variables
Recursive variables are defined using `:=`. They are evaluated only when used, which means changes to their values do not affect previous usages.
```
VAR := initial_value
VAR := new_value  # Does not affect previous usages of VAR
```
### 4.3.3 Conditional Variables
Conditional variables are defined using ?=. They are set only if the variable is not already defined. This is useful for providing default values.

```
VAR ?= default_value  
# VAR is set to default_value if VAR is not already defined
```

## 4.4 Practical Usage
### 4.4.1 Compiler and Flags
Variables are useful for defining lists of files, especially when there are many source files:

```
CC = gcc
CFLAGS = -Wall -O2

my_program: main.c
    $(CC) $(CFLAGS) -o my_program main.c
```
### 4.4.2 File Lists
Variables are useful for defining lists of files, especially when there are many source files:


```
SOURCES = main.c util.c
OBJECTS = $(SOURCES:.c=.o)

my_program: $(OBJECTS)
    $(CC) $(CFLAGS) -o my_program $(OBJECTS)
```
Here, OBJECTS is generated from `SOURCES`, converting `.c `to `.o`

### 4.4.3 Conditional Compilation
Variables enable conditional compilation based on build configurations or target platforms:

```c
# Build a debug version when DEBUG is defined
ifdef DEBUG
    CFLAGS += -g
endif

my_program: main.c
    $(CC) $(CFLAGS) -o my_program main.c
```

4.4.4 Versioning
```
VERSION = 1.0
PROJECT_NAME = my_project

my_program: main.c
    $(CC) $(CFLAGS) -o $(PROJECT_NAME)_$(VERSION) main.c
```

## 5. Automatic Variables in Makefiles
### 5.1.1 $@ - The Target
```
my_program: main.o util.o
    $(CC) $(CFLAGS) -o $@ $^
```
Here, `$@` is replaced with `my_program`, and `$^` with `main.o` `util.o`.

#### 5.1.2 $^ - All Dependencies
$^ represents all the dependencies of the rule being executed. This is handy when you want to include all prerequisites in a command.

5.1.3 $< - The First Dependency
$< stands for the first dependency in the list of prerequisites. It’s often used when there’s only one dependency and you want to reference it directly.

Example:
```py
main.o: main.c
    $(CC) $(CFLAGS) -o $@ $<
# $@ represents the main.o $< representing main.c
```
Here, $< is replaced with main.c.

#### 5.1.4 $? - Out-of-Date Dependencies
$? represents all the dependencies that are out-of-date concerning the target. It’s useful when you want to process only the files that have changed since the last build.

#### 5.2.1 Building Multiple Targets
Automatic variables are especially useful when building multiple targets from a single rule:
```py
objects = main.o util.o

all: my_program

my_program: $(objects)
    $(CC) $(CFLAGS) -o $@ $^
# "$^" 表示所有的依赖文件（即对象文件）
$(objects): %.o: %.c
#从最左边往右看%.o: %.c 是一个模式规则，它表示如何将 .c 文件编译成 .o 文件，这边百分号表示任意
    $(CC) $(CFLAGS) -c -o $@ $<
```
In this example, $@ and $^ are used to specify the target and dependencies for the my_program rule, as well as for the pattern rule that compiles .c files into .o files.

-c：将 .c 文件编译成 .o 文件（只编译，不链接）。
-o $@：指定输出文件为 $@，这里 $@ 代表目标文件（例如 main.o）。
$<：代表第一个依赖文件，即源文件（例如 main.c

因此，这条命令的完整含义是：将源文件（$<，例如 main.c）编译为目标文件（$@，例如 main.o），并且不进行链接
将每个 .c 文件编译为同名的 .o 文件，编译时使用编译器和编译选项 $(CC) 和 $(CFLAGS)，并生成 .o 文件，而不进行链接

```py
DEBUG ?= 0
#：如果变量 DEBUG 没有被定义（例如在命令行中没有被传递），那么它会被设置为 0

ifeq ($(DEBUG), 1)
#如果在命令行中运行 make DEBUG=1，则 DEBUG 被设置为 1。
    CFLAGS += -g
#-g 标志的作用是告诉编译器在编译过程中生成调试信息
endif
#因此，如果 DEBUG=1，编译时会生成调试信息。如果 DEBUG=0，则不会生成调试信息。

my_program: main.c
    $(CC) $(CFLAGS) -o $@ $<
```



如果没有传递 DEBUG，它默认为 0。
1. ifeq ($(DEBUG), 1)
这部分是一个条件判断语句，使用 ifeq 来判断 DEBUG 变量的值是否为 1。它的含义是：如果 DEBUG 等于 1，那么执行下面的代码块。

1. CFLAGS += -g
这条语句表示：如果 DEBUG 被设置为 1，那么会在 CFLAGS（编译选项）中追加 -g 标志。-g 标志的作用是告诉编译器在编译过程中生成调试信息，这样可以在调试器（如 GDB）中使用这些信息来调试程序。



4. my_program: main.c
这条语句定义了一个名为 my_program 的目标，它依赖于 main.c 文件。也就是说，Makefile 需要 main.c 文件来生成 my_program。

5. $(CC) $(CFLAGS) -o $@ $<
这条语句是用来编译并生成可执行文件 my_program 的命令，具体解释如下：

$(CC)：编译器（通常是 gcc）。
$(CFLAGS)：编译选项。如果 DEBUG=1，那么 CFLAGS 会包含 -g 选项。
-o $@：指定输出文件，$@ 代表目标文件（即 my_program）。
$<：表示第一个依赖文件（即 main.c）。
#### 5.2.3 Out-of-Date Dependencies
Automatic variables like $? are useful when you want to process only the dependencies that have changed since the last build. For example, in a custom build script:
```py
generate_resources: resource_generator
    ./resource_generator -o $@ $?
#执行 resource_generator 程序，并将生成的输出保存到 generate_resources 中，同时使用所有自从目标上次更新后变化过的依赖文件作为输入。

resource_generator: resource_generator.c
    $(CC) $(CFLAGS) -o $@ $<
#这一行会编译 resource_generator.c 文件并生成一个名为 resource_generator 的可执行文
```