*This project has been created as part of the 42 curriculum by sevdsahi.*

# get_next_line

## Description

**get_next_line** is a C function that reads and returns one line at a time from a file descriptor. Each successive call continues where the previous one stopped, so a whole file (or standard input) can be read line by line without loading it entirely into memory.

Key characteristics:

- Works with any `BUFFER_SIZE` defined at compile time (`-D BUFFER_SIZE=n`).
- Works on regular files as well as standard input.
- The returned line includes the terminating `\n`, except when the end of the file is reached without a trailing newline.
- Returns `NULL` when there is nothing left to read or when an error occurs.

The main learning goals of the project are the use of **static variables** to keep state between calls and rigorous **manual memory management** (no leaks, no double frees).

## Instructions

### Files

| File | Description |
|------|-------------|
| `get_next_line.c` | Core function and its static helpers |
| `get_next_line_utils.c` | Utility functions (`gnl_strlen`, `gnl_strchr`, `gnl_strjoin`, `gnl_substr`) |
| `get_next_line.h` | Prototypes and default `BUFFER_SIZE` definition |

### Compilation

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c main.c
```

`BUFFER_SIZE` can be changed to test different behaviors (e.g. `-D BUFFER_SIZE=1` or `-D BUFFER_SIZE=9999`). If the flag is omitted, the default value defined in `get_next_line.h` (42) is used.

### Usage

Include the header and call the function in a loop:

```c
#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int main(void)
{
    int     fd;
    char    *line;

    fd = open("example.txt", O_RDONLY);
    if (fd < 0)
        return (1);
    while ((line = get_next_line(fd)) != NULL)
    {
        printf("%s", line);
        free(line);
    }
    close(fd);
    return (0);
}
```

### Return value

- The line that was read (including `\n` if present), which must be freed by the caller.
- `NULL` on end of file or on error.

## Algorithm

### Approach: persistent static stash

`read()` fetches fixed-size chunks (`BUFFER_SIZE` bytes) that almost never align with line boundaries. A single `static char *stash` keeps the leftover data between calls, so nothing that was read is lost. Each call of `get_next_line` goes through three steps:

1. **`read_into_stash`**: allocates a temporary buffer of `BUFFER_SIZE + 1` bytes and calls `read()` repeatedly, appending every chunk to the stash with `gnl_strjoin`. It stops as soon as the stash contains a `\n` or `read()` returns `0` (EOF). If `read()` fails (`-1`) or an allocation fails, all allocated memory (buffer and stash) is freed and `NULL` is returned.
2. **`extract_line`**: finds the first `\n` in the stash (or the end of the string) and returns a new string containing everything from the start up to and including that newline, using `gnl_substr`. If the stash is empty, it returns `NULL`, which signals EOF to the caller.
3. **`update_stash`**: removes the returned line from the stash and keeps only the remainder for the next call. If there is no newline or nothing remains, the stash is freed and set to `NULL`.

### Justification

- **Static variable**: it acts as a memory that survives between calls, allowing the extra characters read past a newline to be reused later, without any global variable.
- **Reads only what is needed**: the loop stops as soon as a newline is in the stash, so the file is never fully loaded into memory.
- **Any `BUFFER_SIZE`**: because leftovers are always preserved, the function behaves the same for `BUFFER_SIZE=1` and for very large values.
- **Memory safety**: `gnl_strjoin` frees the old stash after joining it with the new chunk, and every error path frees what was allocated, which avoids memory leaks.
- **Separation of concerns**: each helper does one job, which keeps the code short, readable, Norm-friendly and easy to debug.
- **Edge cases**: files without a trailing newline, empty files and invalid file descriptors (`fd < 0`) or `BUFFER_SIZE <= 0` are handled.

## Resources

- `man 2 read`, `man 3 malloc`, `man 3 free`: behavior of the system calls and memory functions.
- [Static variables in C – GeeksforGeeks](https://www.geeksforgeeks.org/static-variables-in-c/)
- [File descriptors – Wikipedia](https://en.wikipedia.org/wiki/File_descriptor)
- *The C Programming Language* – Brian W. Kernighan & Dennis M. Ritchie
- Peer discussions with other 42 students about how static variables persist between calls.

### AI usage



For this project, AI was solely used as a writing assistant for the documentation. The actual C code, logic, and algorithmic structure were developed from scratch without any AI assistance.