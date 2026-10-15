# Permissions and Security

## Permission model recap

Linux permissions are three triples (owner/group/others) of r/w/x, often shown as
octal: `700` = `rwx------`.

The admin menu sets the admin directory to `700`, restricting access to the owner.

## Issues in the original code

1. **`BASE_PATH` macro is unused.** The path string is repeated everywhere. A typo in
   one spot would silently break that feature. Fix: use the macro (or better, a variable
   / config).
2. **Hard-coded absolute paths** assume a specific user (`george-emad`) and machine.
   Portable code should take the base path from an environment variable or argument.
3. **`system()` with fixed strings is safe** (no injection) but **fragile** (depends on
   shell/coreutils). Prefer POSIX APIs for production.
4. **Input validation is minimal.** `scanf("%d")` accepts leading whitespace and partial
   numbers; non-numeric input is drained but the program continues. There's no range
   check on `choice`.
5. **No error checking** on `system()` return values; a missing directory produces shell
   errors printed to stderr but the C program keeps going.
6. **Aliases appended to `~/.bashrc`** accumulate duplicate lines on repeated runs.
7. **Symlink creation fails** if `sellers_link` already exists (no `-f`/cleanup).
8. **`rmdir` fails** if `new_directory` is non-empty.
9. **`cp` of a missing `product1.txt`** fails silently-ish.

## Security best practices

- Never build shell commands from user input. If you must, validate strictly and use
  `execvp` with an argument array instead of `system`.
- Drop privileges where possible; don't run the whole tool as root.
- Use `mkstemp` for temp files, not predictable names.
- Check return values of all syscalls/APIs.
- Avoid writing to `~/.bashrc` from a program; it's surprising and persistent.

## Exercise

See [11-exercises.md](11-exercises.md) for hardening tasks.
