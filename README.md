# Todo CLI

A lightweight, dependency-free command-line todo list manager written in C. Tasks persist between sessions, are automatically sorted by due date, and are displayed in a clean, aligned table.

## Features

- Add tasks with a name and due date
- Mark tasks as done (removes them from the list)
- Automatic sorting by due date (soonest first)
- Live countdown showing time remaining until each task is due
- Tasks persist across runs, saved to a file in your home directory
- No external dependencies — just the C standard library

## Requirements

- A C compiler (`gcc` or `clang`)
- `make`

### Platform notes

| Platform | Toolchain |
|---|---|
| macOS | Xcode Command Line Tools (`xcode-select --install`) |
| Linux | `gcc` and `make` (usually preinstalled, or `sudo apt install build-essential`) |
| Windows | [MSYS2](https://www.msys2.org/) with the MinGW-w64 toolchain (`pacman -S mingw-w64-x86_64-gcc make`) |

Windows builds should be done through MSYS2/MinGW rather than raw MSVC — the project relies on a POSIX string-handling function (`strtok_r`) that MinGW's runtime provides but MSVC does not.

## Building

Clone the repo and run:

```bash
make
```

This compiles all `.c` files in the project directory into an executable named `main` (or `main.exe` on Windows, via MinGW).

To clean up the build:

```bash
make clean
```

## Running

```bash
./main
```

On Windows (from an MSYS2/MinGW shell):

```bash
./main.exe
```

## Usage

Once running, you'll see your task list along with a `$:` prompt. Available commands:

| Command | Description |
|---|---|
| `add -n (task name) -d (mm-dd-yyyy)` | Add a new task with a name and due date |
| `mark -i (task id)` | Mark a task as done, removing it from the list |
| `-help` | List all available commands |
| `exit` | Save tasks and quit |

### Examples

```
$: add -n Walk the dog -d 09-15-2026
$: add -n Finish report -d 09-01-2026
$: mark -i 0
$: exit
```

## Data storage

Tasks are saved automatically when you exit, and reloaded automatically on startup. The save file lives in your home directory:

- **macOS/Linux:** `~/.todo_tasks`
- **Windows:** `%USERPROFILE%\.todo_tasks`

If the file doesn't exist yet (first run), the app simply starts with an empty task list — no setup required.

## Project structure

```
main.c        — entry point; loads tasks, runs the main input loop
command.c/.h  — command parsing and dispatch (add, mark, -help, exit), save/load logic
task.c/.h     — individual Task struct: creation, date parsing, formatting, deletion
tasklist.c/.h — TaskList struct: dynamic array of tasks, sorting, add/remove
Makefile      — build configuration
```