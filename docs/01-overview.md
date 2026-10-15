# Overview

This repository is an **educational walkthrough** of a small C program that simulates a
Linux-terminal based e-commerce system. The original program lives in
[`src/ecommerce.c`](../src/ecommerce.c).

## What the program does

At startup it asks the user to pick a **role**:

1. **Admin** – manages files, permissions, directories, links, copies, logs, aliases, search, viewing.
2. **Seller** – manages product files (list, create, copy, move, view, find, aliases).
3. **Customer** – browses/purchases products, views history/status, notes, feedback, search.

Each role gets its own interactive **menu loop**. The menus are driven by `scanf` reading an
integer choice, then a `switch` statement dispatching to the matching action.

## How actions are performed

Almost every action is performed by calling the C standard library function **`system()`**,
which spawns a shell (`/bin/sh`) and runs a Linux command such as `ls`, `chmod`, `mkdir`,
`ln`, `cp`, `mv`, `cat`, `head`, `tail`, `find`, `grep`, `echo`. In other words, the C
program is a thin interactive wrapper around everyday Linux shell commands.

## Base path

All data lives under a single hard-coded directory:

```c
#define BASE_PATH "/home/george-emad/Desktop/ecommerce"
```

Sub-directories used:

- `admin/` – admin area (logs, new directories, links)
- `sellers/` – product files (`product*.txt`)
- `customers/` – purchased products, notes, feedback

> Note: `BASE_PATH` is defined but the code mostly uses the literal path string. See
> [06-permissions-and-security.md](06-permissions-and-security.md) for discussion.

## Learning objectives

After studying this repo you should be able to:

- Read and navigate a multi-function C program.
- Understand `do…while` menu loops and `switch` dispatch.
- Use `scanf`/`getchar` for input handling (and its pitfalls).
- Understand what `system()` does and its security implications.
- Map each menu option to the underlying Linux command.
- Build, run, and experiment safely in a sandbox.

## Repository layout

```
.
├── README.md
├── Makefile
├── src/ecommerce.c          # original program
├── docs/                    # chapter-by-chapter explanation
├── examples/sample_session.md
├── tests/test_compile.sh
└── .gitignore
```
