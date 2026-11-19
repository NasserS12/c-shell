# C Shell

A small Unix shell written from scratch in C, built step by step to understand how shells, processes, and memory really work under the hood.

## Features

- **Run programs** using `fork`, `execvp`, and `waitpid`
- **Built-in commands:** `cd`, `pwd`, `exit`, `export`, `history`, `alias`
- **I/O redirection:** `>` (overwrite), `>>` (append), `<` (input)
- **Pipes:** `cmd1 | cmd2`
- **Signal handling:** `Ctrl+C` stops the running command, not the shell
- **Command history** stored in a linked list
- **Aliases** stored in a hash table (with chaining for collisions)
- **Memory safe:** tested with Valgrind, zero leaks

## Build and run

```bash
make
./shell
```

To remove build files:

```bash
make clean
```

## Examples

```
$ ls -l > files.txt
$ cat < files.txt
$ ls | grep .c
$ export NAME=Nasser
$ alias ll=ls -la
$ ll /tmp
$ history
$ exit
```

## Project structure

| File | Responsibility |
|------|----------------|
| `shell.c` | Main loop: reads input and decides what runs |
| `parse.c` | Splits the line into words and finds redirections and pipes |
| `exec.c` | Runs commands and pipes in child processes |
| `builtins.c` | Built-in commands that must run inside the shell |
| `history.c` | Command history as a linked list |
| `alias.c` | Aliases as a hash table |

## What I learned

- How processes are created and replaced (`fork`, `exec`, `wait`), and what zombie processes are
- File descriptors, and how `dup2` makes redirection and pipes possible
- Why `cd` must be a built-in
- Signals and why handlers must be async-signal-safe
- Stack vs heap, `malloc` / `free`, and finding leaks with Valgrind
- Linked lists and hash tables, built by hand
- Splitting a program into multiple files, header files, the linker, and Makefiles

## Limitations

- Only one pipe per command
- Pipes and redirection cannot be combined in the same command
- Built-in commands do not support redirection
- No quoting support (`"..."` or `'...'`)
