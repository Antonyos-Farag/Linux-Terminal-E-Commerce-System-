# Exercises

Beginner → advanced tasks to deepen understanding.

## Beginner

1. Change the menu banner text and rebuild.
2. Add a new admin option "10. Disk usage" running `du -sh .../ecommerce`.
3. Make `BASE_PATH` actually used: replace every literal path with the macro.
4. Add range validation: reject choices outside 0–9.

## Intermediate

5. Replace `system("mkdir ...")` with the POSIX `mkdir()` call; check its return value.
6. Read a filename from the user (safely, with `fgets`) and view it; validate it stays
   inside `BASE_PATH` to prevent path traversal.
7. Make the alias append idempotent (check before appending).
8. Add a "remove symlink" option using `unlink`.

## Advanced

9. Refactor menus to use a table of function pointers instead of a giant switch.
10. Replace all `system()` calls with POSIX APIs and proper error reporting (`perror`).
11. Add persistence: store products as small structs serialized to a file.
12. Add unit tests for input parsing (extract `read_int` into a testable function).

## Security challenge

13. Demonstrate command injection by (in a *private* copy) building a `system()`
    string from `scanf("%s")` input. Then fix it using `execvp` + argument arrays.
    Never run the vulnerable version on a shared machine.
