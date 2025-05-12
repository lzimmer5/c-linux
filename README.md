# C-Linux: Reimplementing Core Linux Commands in C

This project contains my from-scratch C implementations of several core Linux command-line utilities. 
The goal was to deepen understanding of low-level system interfaces, memory management, and Unix-style modular software design.

## Implemented Commands

### `curlit/` – `curl`
A simplified version of `curl` that establishes TCP connections using sockets and performs HTTP GET requests.

- Uses low-level socket programming (e.g., `socket()` and `connect()`).
- Demonstrates manual URL parsing and stream-based network I/O.

---

### `findit/` – `find`
Searches for files in a directory hierarchy matching specific conditions.

- Implements recursive directory traversal.
- Uses a custom linked list (`linked-list-2/`) to manage filter chains and matching paths.

---

### `seqit/` – `seq`
Outputs a sequence of numbers from a given start to end (with optional step).

- Mimics basic `seq` behavior.
- Uses the generic linked list from `linked-list-1/` to store and emit the sequence.

---

### `tailit/` – `tail`
Prints the last `n` lines of a file.

- Supports configurable line counts (`-n` flag).
- Uses `linked-list-1/` to buffer the most recent lines in a rolling window.

---

### `timeit/` – `time`
Measures the wall-clock time taken to run a given command.

- Uses `fork()` and `execvp()` to spawn a subprocess.
- Captures timing using `clock_gettime()` before and after execution.

---

## Data Structure Reuse

- `linked-list-1/`: General-purpose singly linked list used in `seqit/` and `tailit/`.
- `linked-list-2/`: Specialized linked list for structured filtering in `findit/`.

---

## How to Compile

Use gcc to compile each command along with its dependencies. The dependencies for each command are contained within the same folder as
the command. For the `tailit` and `seqit` commands, include the `linked-list-1` .c files. For the `findit` command, include the linked-list-2
files. For example:

```bash
cd findit
gcc -o findit findit.c filter.c list.c
./findit *.c
```
