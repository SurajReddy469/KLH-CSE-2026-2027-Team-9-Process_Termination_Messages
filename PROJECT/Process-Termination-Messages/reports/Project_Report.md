# PROJECT REPORT
# Process Termination Messages

## 1. Abstract

Process Termination Messages is a Linux-based Operating Systems and Systems
Programming project that demonstrates how a parent process can create, execute,
monitor, and identify the termination of a child process. The implementation
uses C programming with Linux/POSIX mechanisms such as `fork()`, `execvp()`,
`waitpid()`, process identifiers, and termination-status macros. A parent
process creates a child process; the child launches a selected Linux
application, and the parent waits for the child and displays a meaningful
termination message. The system distinguishes normal termination from
termination caused by a signal and displays relevant process information such
as PID and exit status.

## 2. Problem Statement

A process may finish normally or terminate because of an error or another
condition. Beginners often find it difficult to understand how the parent
process detects and reports the termination of a child process. This project
addresses the problem by implementing a Linux program in which a parent creates
a child, launches an application, waits for it, checks its termination status,
and displays an appropriate message.

## 3. Objectives

1. Demonstrate creation of a child process using `fork()`.
2. Launch an application from the child process using `execvp()`.
3. Monitor child-process completion using `waitpid()`.
4. Identify and display normal or signal-based process termination.
5. Display the child PID and termination status.
6. Provide a practical Linux command-line demonstration of process management.

## 4. Proposed Methodology

The parent process calls `fork()` to create a child process. The child uses
`execvp()` to replace itself with the selected Linux application. The parent
uses `waitpid()` to wait for the child and obtain its termination status. The
returned status is examined using `WIFEXITED()`, `WEXITSTATUS()`,
`WIFSIGNALED()`, and `WTERMSIG()`. Based on the result, the program prints a
clear termination message containing the child PID and termination status.

## 5. System Architecture

User
 |
 v
Process Termination Monitor
 |
 +-----------------------------+
 |                             |
fork()                       Menu
 |
 +-----------------------------+
 |                             |
Parent                        Child
 |                             |
waitpid()                    execvp()
 |                             |
 |                        Linux Application
 |                             |
 +<-----------------------------+
              |
        Termination status
              |
      +-------+-------+
      |               |
 Normal             Signal
 termination       termination
      |               |
 exit status       signal number
      +-------+-------+
              |
              v
     Termination message

## 6. OS Concepts and APIs

### fork()
Creates a child process from the parent.

### execvp()
Replaces the child process image with the selected application.

### waitpid()
Allows the parent to wait for the specific child and obtain its termination
status.

### getpid()
Obtains the process ID used in the displayed process information.

### WIFEXITED()
Checks whether the child terminated normally.

### WEXITSTATUS()
Retrieves the exit status when normal termination occurs.

### WIFSIGNALED()
Checks whether the child terminated because of a signal.

### WTERMSIG()
Retrieves the signal number responsible for signal-based termination.

## 7. Syllabus Mapping

### CO-1
The project demonstrates Linux/POSIX system programming and interaction with
operating-system process services.

### CO-2
The project directly demonstrates process abstraction, process creation,
execution, parent-child relationships, synchronization using `waitpid()`, and
process termination.

### CO-3
The project demonstrates signal-based process termination detection through
`WIFSIGNALED()` and `WTERMSIG()`.

CO-4, CO-5 and CO-6 are outside the core scope of this implementation.

## 8. Algorithm

1. Display the project menu.
2. Ask the user for an application command.
3. Call `fork()`.
4. In the child, call `execvp()` to launch the application.
5. In the parent, store the child PID.
6. Call `waitpid()` for that child.
7. Check `WIFEXITED()`.
8. If true, retrieve `WEXITSTATUS()`.
9. Otherwise check `WIFSIGNALED()`.
10. If true, retrieve `WTERMSIG()`.
11. Display the PID and meaningful termination message.
12. Return to the menu.

## 9. Test Cases

### Test Case 1: Normal termination
Input: `xclock` or another installed GUI application.

Expected result:
- Application opens.
- User closes it.
- Parent detects normal termination.
- PID and exit status are displayed.

### Test Case 2: Non-zero exit
Expected result:
- Parent detects normal termination.
- Non-zero exit status is displayed.

### Test Case 3: Signal termination
Expected result:
- Parent detects `WIFSIGNALED()`.
- Signal number is displayed.

## 10. Expected Output

```text
[Parent] Child PID: 4821
[Monitor] Application is running...
[Monitor] Waiting with waitpid()...

[Monitor] Application process ended.
[Monitor] PID: 4821
[Monitor] Termination type: Normal
[Monitor] Exit status: 0
[Monitor] Message: Application closed normally.
```

## 11. Tools

- Linux / Ubuntu
- C
- GCC
- Make / Makefile
- Git / GitHub

## 12. Team Contributions

### 2520030386 — T. Jashwanth Ram
Process creation and child-process implementation using `fork()`.

### 2520030433 — J. Sai Shashank
Termination-status handling, `waitpid()`, and termination-message logic.

### 2520030472 — A. Arun Tej
Testing, debugging, output documentation, and project report/presentation
support.

## 13. Conclusion

The project provides a practical demonstration of Linux process management.
It shows how a parent process can launch a child process, wait for its
completion, determine whether it terminated normally or because of a signal,
and communicate the result through a meaningful terminal message.
