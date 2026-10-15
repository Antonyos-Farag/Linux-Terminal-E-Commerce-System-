# C Basics Used in This Program

## Includes

```c
#include <stdio.h>    // printf, scanf, getchar, FILE I/O
#include <stdlib.h>   // system(), exit helpers
#include <string.h>   // string helpers (included, lightly used)
#include <unistd.h>   // POSIX: chdir, getcwd, etc. (included)
#include <sys/stat.h> // stat structures (included)
#include <errno.h>    // errno (included)
```

These headers are pulled in even though only a subset is strictly used. In an educational
refactor you would trim unused includes.

## The `#define` macro

```c
#define BASE_PATH "/home/george-emad/Desktop/ecommerce"
```

`BASE_PATH` is a **preprocessor macro**: before compilation, every occurrence of the token
`BASE_PATH` is replaced by the string literal. It is a compile-time constant, not a runtime
variable. (Note: the code mostly hard-codes the path instead of using this macro — a
common beginner inconsistency worth fixing.)

## Function declarations

The program defines four functions:

- `void admin_menu()` – admin interactive menu
- `void seller_menu()` – seller interactive menu
- `void customer_menu()` – customer interactive menu
- `int main()` – entry point

Each menu function returns `void` (no value) and takes no arguments.

## Variables and types

- `int choice;` – holds the user's numeric menu selection.
- `int dir_choice, view_choice, find_choice, search_choice;` – sub-menu selections.
- `int role;` – top-level role selection in `main`.

## Control flow

- `do { ... } while (condition);` – runs the body at least once, then repeats while the
  condition is true. Menus use `while (choice != 0)` so choosing `0` exits.
- `switch (choice) { case N: ... break; }` – multi-way branch on an integer.
- `if / else if / else` – used for sub-choices.

## Input handling pattern

```c
if (scanf("%d", &choice) != 1) {
    printf("Invalid input! Please enter a number.\n");
    while (getchar() != '\n') { /* clear input */ }
    continue;   // or break;
}
while (getchar() != '\n') { /* clear input */ }
```

- `scanf("%d", &choice)` reads an integer. It returns the number of successful
  conversions (1 on success, 0 on failure, EOF on end-of-input).
- On failure, the offending characters remain in the input buffer, so they are drained
  with `getchar()` until a newline.
- After a successful read, the trailing newline is also drained so the next `scanf` starts
  cleanly.

This is a classic, if fragile, way to read line-based menu input in C.

## `system()`

```c
system("ls -la /home/george-emad/Desktop/ecommerce");
```

`system()` (from `<stdlib.h>`) passes the string to the system shell for execution and
waits for it to finish. It returns the shell's exit status. It is convenient but has
security and portability implications (see [06](06-permissions-and-security.md)).

## `printf` formatting

- `\n` – newline.
- Plain strings for menus; no width/precision specifiers are used here.

## Return values

- `main` returns `0` on normal completion.
- Menu functions return early via `return;` when the user picks `0`.
