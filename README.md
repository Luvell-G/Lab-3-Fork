# Lab-3-Fork
 src/ contains all source files and challenge implementations.
 CMakeLists.txt or Makefile builds all executables.
 lab_report.pdf contains screenshots proving build & run for the required demos and challenges .
 Source code and  GitHub repo link provided.
 Include short notes in README.md listing which challenges you completed and any known issues.

 
**Course:** Operating Systems  
**Student:** Luvell Glanton  
**Environment:** Ubuntu Linux (GitHub Codespaces)

---

## Overview
This lab explores **UNIX process control and interprocess communication** using C.  
The project demonstrates how processes are created, executed, synchronized, and connected using:

- `fork()`
- `exec*()` family
- `wait()` / `waitpid()`
- Pipes
- Zombie processes
- Concurrency control (worker pool)

---

## Project Structure
Lab-3-Fork/
├── CMakeLists.txt
├── README.md
├── src/
│ ├── fork_demo.c
│ ├── exec_demo.c
│ ├── pipe_demo.c
│ ├── pipeline_demo.c
│ └── zombie_demo.c
├── challenges/
│ ├── ch1_single_fork.c
│ ├── ch2_multi_child.c
│ ├── ch3_exec_ls.c
│ ├── worker.c
│ ├── ch4_exec_worker.c
│ ├── ch5_exec_examples.c
│ ├── ch6_pipe_sum.c
│ ├── ch7_pipeline.c
│ ├── ch8_wait_nonblock.c
│ ├── ch9_zombie.c
│ └── ch10_pool.c
├── build/
└── lab_report/


---

## Build Instructions

### Option 1 — CMake (recommended)
```bash
mkdir -p build
cd build
cmake ..
cmake --build .
Run an example:

./fork_demo
Option 2 — Compile individually with gcc
gcc challenges/ch1_single_fork.c -o challenges/ch1_single_fork
./challenges/ch1_single_fork
Demo Programs (src/)
File	Description
fork_demo.c	Basic fork() with parent/child and waitpid()
exec_demo.c	Child replaces process image using execlp()
pipe_demo.c	Parent writes to pipe, child reads and executes wc -l
pipeline_demo.c	Manual pipeline using two forks (`ls
zombie_demo.c	Demonstrates zombie process and cleanup
Challenges Summary
Challenge 1 — Single fork lifecycle
./challenges/ch1_single_fork
Child prints PID/PPID, sleeps, exits with status 7. Parent waits and reports status.

Challenge 2 — Multiple children in a loop
./challenges/ch2_multi_child 5
Parent spawns N children and reports exit order.

Challenge 3 — execlp() example
./challenges/ch3_exec_ls
Child executes ls -la; parent waits.

Challenge 4 — Exec internal worker
gcc -o worker challenges/worker.c
./ch4_exec_worker
Worker prints arguments and custom environment variable MYVAR=hello.

Challenge 5 — execv vs execl
./challenges/ch5_exec_examples
Two children print identical output using different exec methods.

Challenge 6 — Parent-to-child pipe
./challenges/ch6_pipe_sum
Parent writes numbers 1–10; child prints Sum = 55.

Challenge 7 — Pipeline (ls | grep pattern)
./challenges/ch7_pipeline "\.c$"
Manual pipeline implementation using two forks and one pipe.

Challenge 8 — Non-blocking wait (WNOHANG)
./challenges/ch8_wait_nonblock
Parent polls for child completion without blocking.

Challenge 9 — Zombie process observation
./challenges/ch9_zombie
Child exits immediately; parent sleeps to create zombie, then reaps it.

Challenge 10 — Parallel worker pool
./challenges/ch10_pool 3 file1 file2 file3 file4 file5
Processes tasks with a concurrency limit using a worker pool.

Key Concepts Demonstrated
Process creation and lifecycle

Program execution replacement

Exit codes and synchronization

Interprocess communication via pipes

Zombie and orphan processes

Non-blocking waits

Concurrency limits
