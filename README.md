# Welcome to the first project for CSC256-04

# Goals:

1. Learn more about git by building a simpler version, called gatorgit
2. Write a substantial C program

**Let me remind you of the general expectations for this project:** 
- Code must be neat, with proper and consistent indentation and well documented. 
- Keep line to around 80 characters per line, and not line greater than 100 characters.
- Comments must describe the logic and reason for the code and not repeat the code.  
- Variables must have meaningful names and be in a consistent format (I do not care if you use camelCase or under_scores in variables but be consistent.
- You must use `make` to compile the program.
- In addition, each file must have the standard header as defined below.
- All source files  must be in the main branch of the github.

This is a GROUP assignment.  You can (and should) work in groups to research how to do the assignment, but each person should code their own version and make their own submission.

# What is gatorgit?

git is a great tool for managing source code and other files. However, even great tools can be used for evil; what if someone uses it to create git commits with hideous messages. So in this homework, you will be developing your own version of git, which will put an end to such behavior by requiring every commit message to contain the words "GOLDEN GATOR!". ;-)

At its core, gatorgit is essentially a simpler version of git. gatorgit can track individual files in the current working directory (no subdirectories!). It maintains a .gatorgit/ subdirectory containing information about your repository. For each commit that the user makes, a directory is created inside the .gatorgit/ directory (.gatorgit/<ID>, where <ID> is the ID of the commit). The .gatorgit/ directory additionally contains two files: .index (a list of all currently tracked files, one per line, no duplicates) and .prev (contains the ID of the last commit, or 0..0 if there is no commit yet). Each .gatorgit/<ID> directory contains a copy of each tracked file (as well as the .index file) at the time of the commit, a .msg file that contains the commit message (one line) and a .prev file that contains the commit ID of the previous commit.

# Key differences between gatorgit and git:
1. The only supported commands are init, add, rm, commit and status. For each of them, only the most basic command line options are supported.
gatorgit does not track diffs between files. Instead, each time you make a commit, it simply copies all files that are being tracked into the .gatorgit/<ID> directory (where <ID> is the commit ID). 
2. Commit IDs are not based on cryptographic hash functions, but instead are a fixed sequence of 40-character strings that only contain 'c', 's', '2', '5' and '6' (why we chose those characters is left as an exercise to the reader). Any commits with a commit message that does not contain "GOLDEN GATOR!" (with exactly this capitalization and spelling) will be rejected with an error message.
```
Example Commit IDs:

First commit ID: cccccccccccccccccccccccccccccccccccccccc
Second commit ID: cccccccccccccccccccccccccccccccccccccccs
Third commit ID: cccccccccccccccccccccccccccccccccccccccc2

... and so on.
```
4. No user, date or other additional information is tracked by gatorgit. It does not allow to track subdirectories.
5. The rm command only causes gatorgit to stop tracking a file, but does not delete it from the file system.


# Files:
gatorgit.c - This is the file that you will fill in with your implementation of gatorgit.
gatorgit.h - Do not edit - This file contains declarations of various constructs in gatorgit.c along with convenient #defines. See the "Important Numbers" section below.
main.c - Do not edit - Contains the main for gatorgit (which parses command line options and calls into the functions defined in gatorgit.c).
Makefile - Do not edit - This tells the program make how to build your code when you run the make command. This is a convenient alternative to having to repeatedly type long commands involving gcc.
util.h - Do not edit - Contains helper functions that you may wish to use when completing the assignment.

# You should only modify gatorgit.c

# Important Numbers: (see gatorgit.h)

In lecture, you learned about using #define to define constants as a single source of truth. You should use the appropriate constants from gatorgit.h whenever you are using any of the following numbers:

1. Commit ID lengths are limited to 40 characters (not including the null terminator).
2. Filenames are limited to 512 characters (including null terminator).
3. Commit messages are limited to 512 characters in length (including null terminator).

Preliminaries:
For this homework, you will be using some C functionality that you may not be familiar with. We will now highlight some of these features:

C library functions:

You may wish to familiarize yourself with the following C library functions: fprintf, sprintf, fopen (and fclose, fwrite, etc.), strcmp, strlen, strtok, and fgets. You can find documentation of the C library here ([GNU C Manual](https://www.gnu.org/software/libc/manual/pdf/libc.pdf)). Make sure not to stray away.

When you look at the existing code in gatorgit.c, you will see examples of how these functions can be used to achieve the desired functionality. We recommend trying to understand the provided functions first, before starting to implement your own.

Handling I/O (more than just printf):
Unix machines use a concept called "streams" to handle arbitrary I/O. We will need two of these output streams in this homework. The first is stdout, which is where your output goes when you call printf. We will use stdout to output all output indicating a "successful" action. The other output stream is stderr, which is where we will output all error messages. By default, both of these streams, stdout and stderr, are printed to your screen when you run a program.

Outputting to either stdout or stderr can be done similarly to using printf. The only change is that you use the fprintf function, and the first argument you pass in must be either "stderr" or "stdout" (without quotes).

```
[inside your C code]
fprintf(stdout, "%d\n", 3); // prints the number 3 to stdout, along with a newline
fprintf(stderr, "%d\n", 4); // prints the number 4 to stderr, along with a newline
```

If you want to know what messages went to stdout or stderr, you can forward them to a file instead of the terminal, by appending 2>log_err and/or 1>log_out to your command (e.g., ./my_program 2>log_err). This will forward everything from stderr to the file log_err (and equivalently for stdout if you add 1>log_out).

Included helper functions:
To make life easier for you, we provide helper functions for common operations that you will encounter while implementing gatorgit. You fill find these in utils.h. Here is a brief overview of each of these functions:

```
void fs_mkdir(const char* dirname): Create a new directory of name dirname
void fs_rm(const char* filename): Delete the file filename
void fs_mv(const char* src, const char* dst): Move the file src to dst, potentially overwriting it
void fs_cp(const char* src, const char* dst): Copy the file src to dst, potentially overwriting it
void write_string_to_file(const char* filename, const char* str): Create or overwrite the file filename and write str into it, including the NULL-character
void read_string_from_file(const char* filename, char* str, int size): Open the file filename and read its content into the location pointed to by str; limit the amount to read to at most size bytes, including the NULL character
```

The last two functions should only be used together. Specifically, don't try to use read_string_from_file to read multi-line files, but only for single strings that you previously wrote into a file using write_string_to_file.

While these functions perform some basic checks to prevent you from accidentally overwriting important files, be careful whenever you call any function that modifies the file system. There is always a risk of unintentionally deleting or overwriting files, especially when working on your own machine!

# Setup:
For this assignment, you can have your own development environment for use Github Codespaces. Codespaces is a paid service, but you can access it through the Github Student Developer Pack for free.

1. If you haven't already, sign up for the [Github Student Developer Pack](https://education.github.com/pack).
2. Navigate to the Github repository designated for this assignment and locate the green "Code" button.
3. From the dropdown menu, opt for "Open with Codespaces."
4. After the Codespace initializes, a terminal window will appear at the bottom of your screen. You can utilize this terminal to execute commands and compile your code.

The first thing you should do is to compile your code. You can do this by running the following command in the terminal:

```
$ make gatorgit
$ ./gatorgit init
```


Required functionality:
While the version of gatorgit that we've given to you compiles, you can't do much except call gatorgit init to create a new repository, and call gatorgit add <file> to start tracking a file. Everything else you need to implement yourself!

We recommend that you implement the gatorgit commands in this order, as this makes testing easier:

1. gatorgit status
2. gatorgit rm
3. gatorgit commit

For each of these, you need to implement one of the functions below (but feel free to define new helper functions to make things easier). We give you an outline of each function's job, as well as the errors you need to be able to detect, and the output you need to produce.



Testing your code:

Unlike CS classes you may have taken in the past, we will not provide you with a full autograder for the assignment. Instead, you should devise a methodology to test your code to ensure that it performs as you intend it to. 

But... why?
When you write production code in the "real" world (and upper division classes), much of the time you will not be provided with any test cases to validate your code against (not even a sanity check). The ability to write good test cases is just as important a skill for a programmer as the ability to write functioning code.

The test cases you write for this homework won't be submitted or graded.

Automated basic tests
To make life a bit easier for you, we are providing you with one way to test your implementation. The first one is an automated testing tool that will run your implementation against a series of basic tests to determine whether its output is sensible. Note that this is just a small subset of the tests that the actual autograder will be running. Even if your program passes all these tests, it may still fail on some of the test cases in the autograder. You therefore shouldn't rely on this tool for your testing but consider it a sanity check.

To run this tests, go into the main source directory and type *make test-gatorgit*. You will see output similar to the following:
```
$ make test-gatorgit
Running test cases...

  [  OK  ] INIT TEST
  [  OK  ] ADD TEST
  [ FAIL ] STATUS TEST
  [ FAIL ] RM TEST
  [ FAIL ] COMMIT TEST
```
Manual testing

You can then run commands such as:
```
$ ./gatorgit init
$ touch test.txt
$ gatorgit add test.txt
```

# Steps to implement gatorgit

Step 1: The status command
Functionality:
The status command in gatorgit should read the file .gatorgit/.index and print a line for each tracked file. The exact format is described below. Unlike git status, gatorgit status should not print anything about untracked files.

Output to stdout:
```
$ gatorgit status
Tracked files:

  <file1>
  [...]
  <fileN>

<N> files total
```
For each file in the above output, <file*> should be replaced with the filename of that file.

Return value and output to stderr:
This function should always return 0 (indicating success) and should never output to stderr.


Step 2: The rm command
Hint: You may want to have a look at the provided implementation of gatorgit add before implementing this command.

Functionality:
The rm command in gatorgit takes in a single argument, which specifies the file to remove from the index (which is stored in the file .gatorgit/.index). If the filename passed in is not currently being tracked, you should print an error as indicated below. Note that this behavior is different from git in that it doesn't delete the file from your file system.

Output to stdout:
None.

Return value and output to stderr:
If the filename specified in the provided argument exists in the index, the function should return 0 and produce no output on stderr. If the filename specified does not exist in the index, the function should return 1 and output the following to stderr:
```
$ gatorgit rm FILE_THAT_IS_NOT_TRACKED.txt
ERROR: File <filename> not tracked
```
Step 3: The commit command
Functionality:
The commit command involves a couple of steps:

First, check whether the commit string contains "GOLDEN GATOR!". If not, display an error message. Read the ID of the previous last commit from .gatorgit/.prev. Generate the next ID (newid) in such a way that: All characters of the id are either 'c','s','2', '5' or '6'. Generating 100 IDs in a row will generate 100 IDs that are all unique (Hint: you can do this in such a way that you go through all possible IDs before you repeat yourself. Some of the ideas from the number representation class may help you!)
Generate a new directory .gatorgit/<newid> and copy .gatorgit/.index, .gatorgit/.prev and all tracked files into the directory.
Store the commit message (<msg>) into .gatorgit/<newid>/.msg
Write the new ID into .gatorgit/.prev.

IMPORTANT RULE THAT WILL AFFECT YOUR GRADE IF YOU DON'T READ IT!

Now that we have your attention: when implementing the code that checks whether the commit message includes GOLDEN GATOR!, you are not allowed to use any library functions, including any of the str* ones you may have seen before.
Output to stdout:
None.

Return value and output to stderr:
If the commit message does not contain the exact string "GOLDEN GATOR!", then you must output the following to stderr and return 1:

```
$ gatorgit commit -m "G-O- -G-A-T-O-R-!"
ERROR: Message must contain "GOLDEN GATOR!"
If the commit message does contain the string "GOLDEN GATOR!", then the function should produce no output and return 0.
```

# Submission
There are two steps required to submit hw1. Failure to perform both steps will result in loss of credit:

1. First, you must submit using using a zip file -  create a zip file and submit it on Canvas.
                                          

2. Additionally, you must submit hw1 to your GitHub repository. To do so, follow these instructions:

```
$ git add -u                      # should add all modified files in hw1 directory (must include gatorgit.c)
$ git commit -m "FP 1 submission"
$ git tag -f "hw1"                # The tag MUST be "hw1". Failure to do so will result in loss of credit.
$ git push origin master --tags
```



| Grading Rubric | Grade Points | 
|---------------|--------------|
| Documented Code | 10 | 
| Passes STATUS TEST | 10 | 
| Passes RM TEST | 10 | 
| Passes COMMIT TEST | 10 | 
| Runs to Completion | 10 |


