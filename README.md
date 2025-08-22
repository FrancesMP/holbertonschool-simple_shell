# Simple Shell Project

A Minimalist command-line interpreter written in C that mimics basic UNIX shell behavior.
Developed as part of the Holberton School curriculum.


![Logo](https://bashlogo.com/img/logo/jpg/full_colored_light.jpg)
## Functionality

- Displays a prompt (`$`)
- Executes system commands (e.g., `ls`, `pwd`, `echo`, etc.)
- Handles command-line arguments
- Searches for executables in the system PATH
- Handles execution errors
- Signal handling (e.g., Ctrl + C)
- Built-ins:
   - `exit` — exits the shell
   - `cd` — changes directory
   - `env` — prints the environment variables
- Supports interactive mode (line-by-line input) and non-interactive mode (batch execution from a script or pipe)

## Flowchart

The following diagram illustrates the workflow of the shell:  
## Compilation

To run tests, run the following command

```bash
  gcc -Wall -Werror -Wextra -pedantic *.c -o hsh

```

## Usage

```dknd
./hsh
$ ls
AUTHORS  README.md  main.c  shell.h  ...
$ pwd
/home/username/project
$ exit
```
##  Non Interactive Mode

````
echo "ls -l" | ./hsh

````
or 

````
./hsh < script.sh
 ````

## Man Page

````
man ./man_1_simple_shell.1
````

## Files

- main.c – entry point, main shell loop
- shell_functions.c – parsing, executing, utilities
- shell.h – header file with structures and prototypes
- man_1_simple_shell.1 – man page for the shell
- AUTHORS – contributors list
- README.md – project documentation
## Example
````
$ echo Hello World
Hello World
$ /bin/ls
AUTHORS  README.md  hsh  main.c  shell.h  shell_functions.c  man_1_simple_shell.1
````

## Authors

- [@FrancesMP](https://github.com/FrancesMP)
- [@SedraR78](https://github.com/SedraR78)
