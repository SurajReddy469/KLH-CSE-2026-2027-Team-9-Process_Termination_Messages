# Process Termination Messages

**Operating Systems and Systems Programming (25CS2104E)**  
**2026–27, Term-I | Section 4 | Team 9**

## Team Members

- **2520030386** — T. Jashwanth Ram
- **2520030433** — J. Sai Shashank
- **2520030472** — A. Arun Tej

**Supervisor:** Ms. Soumya Enukonda

## Abstract

Process Termination Messages is a Linux-based Operating Systems and Systems Programming project that demonstrates how a parent process can create, execute, monitor, and identify the termination of a child process. The implementation uses C with Linux/POSIX mechanisms such as `fork()`, `execvp()`, `waitpid()`, process identifiers, and termination-status macros. The parent waits for the child, analyses the returned termination status, and displays a meaningful termination message containing the child PID and status.

## Objectives

1. Demonstrate child-process creation using `fork()`.
2. Launch a selected Linux command from the child using `execvp()`.
3. Monitor child completion using `waitpid()`.
4. Identify normal and signal-based termination.
5. Display the child PID and termination information.
6. Demonstrate core Linux/POSIX process-management concepts through a command-line program.

## Linux/POSIX APIs Used

- `fork()`
- `execvp()`
- `waitpid()`
- `exit()`
- `getpid()`
- `WIFEXITED()`
- `WEXITSTATUS()`
- `WIFSIGNALED()`
- `WTERMSIG()`

## Project Structure

```text
Process_Termination_Messages/
├── src/
│   ├── main.c
│   ├── termination_test.c
│   └── Makefile
├── docs/
│   ├── Project_Problem_Statement.pdf
│   ├── Project_Workflow.pdf
│   ├── README.md
├── data/
│   └── README.md
├── results/
│   ├── linux_normal.txt
│   ├── linux_nonzero.txt
│   ├── linux_abnormal.txt
│   ├── chrome_normal.txt
│   └── chrome_abnormal.txt
├── reports/
│   ├── Project_Report.pdf
│   └── Project_Report.md
└── Readme.md
```

## Setup and Execution

Open Ubuntu/WSL and enter the `src` directory:

```bash
cd ~/Process_Termination_Messages/src
```

Compile both programs:

```bash
make clean
make
```

Run the main program:

```bash
./process_monitor
```

### Linux normal termination

Choose **1** and enter:

```text
sleep 10
```

The parent creates the child, waits using `waitpid()`, and reports normal termination and exit status 0.

### Linux non-zero exit status

Choose **1** and enter:

```text
./termination_test error
```

The child terminates normally with exit status 5, allowing the program to demonstrate that normal termination can still have a non-zero exit status.

### Linux signal-based abnormal termination

Choose **1** and enter:

```text
./termination_test signal
```

The test program raises `SIGTERM`. The parent detects this using `WIFSIGNALED()` and reports the signal number using `WTERMSIG()`.

## Chrome Demonstration

The project also contains an additional WSL/Windows Chrome process-monitoring demonstration.

- **Option 2:** Chrome normal termination — launch Chrome and close it normally.
- **Option 3:** Chrome abnormal termination — launch Chrome and press ENTER; the program uses Windows `taskkill /F` to force termination.

Chrome is a Windows process, not a Linux child process. Therefore the Chrome demonstration does **not** claim a POSIX `waitpid()` exit status. The Linux mode is the core POSIX implementation of the project.
