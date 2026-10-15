# `system()` and the Shell

## What `system()` does

```c
int system(const char *command);
```

- Forks a child process running `/bin/sh -c "command"`.
- Waits for it to complete.
- Returns the termination status (shell exit code, shifted).

In this program every filesystem action is delegated to the shell this way.

## Commands used

| Menu | Option | Shell command |
|------|--------|---------------|
| admin | 1 list files | `ls -la /home/.../ecommerce` |
| admin | 2 chmod | `chmod 700 .../admin` |
| admin | 3 create dir | `mkdir -p .../admin/new_directory` |
| admin | 3 delete dir | `rmdir .../admin/new_directory` |
| admin | 4 symlink | `ln -s .../sellers .../admin/sellers_link` |
| admin | 5 copy | `cp .../sellers/product1.txt .../admin/` |
| admin | 6 log | `echo 'Log entry at $(date)' >> .../admin/logs.txt` |
| admin | 7 alias | `echo "alias listfiles='ls -la /tmp/ecommerce'" >> ~/.bashrc` |
| admin | 8 find | `find .../ecommerce -name 'product1.txt'` |
| admin | 9 cat/head/tail | `cat/head/tail -n 10 .../admin/logs.txt` |
| seller | 1 list | `cat .../sellers/product*.txt` |
| seller | 2 create | `echo 'Product details' > .../sellers/product1.txt` |
| seller | 3 copy | `cp .../sellers/product1.txt .../sellers/product1_copy.txt` |
| seller | 4 move | `mv .../sellers/product1_copy.txt .../sellers/moved_product1.txt` |
| seller | 5 view | `cat/head/tail .../sellers/product1.txt` |
| seller | 6 find by name | `find .../sellers -type f` |
| seller | 6 search content | `grep -r 'Product' .../sellers` |
| seller | 7 alias | `bash -lc "source ~/.bashrc && listfiles"` |
| customer | 1 view products | `cat .../sellers/product*.txt` |
| customer | 2 purchase | `cp .../sellers/product1.txt .../customers/purchased_product1.txt` |
| customer | 3 history | `ls -la .../customers` |
| customer | 5 notes | `echo 'Customer preference: Express shipping' >> .../customers/customer_notes.txt` |
| customer | 6 view order | `cat/head/tail .../customers/purchased_product1.txt` |
| customer | 7 find by name | `find .../customers -type f` |
| customer | 7 search content | `grep -r 'purchased' .../customers` |
| customer | 8 feedback | `echo 'Customer feedback: 5 stars - Great product!' >> .../customers/feedback.txt` |

## Why this is educational

Each option maps 1-to-1 to a Linux command, so the program doubles as a **guided tour of
core Linux file utilities**: `ls`, `chmod`, `mkdir`/`rmdir`, `ln`, `cp`, `mv`, `cat`,
`head`, `tail`, `find`, `grep`, `echo`, plus shell features (redirection `>>`, globs
`*`, aliases, `~/.bashrc`).

## Caveats of `system()`

- **Security**: never pass untrusted/user input into `system()` (command injection). This
  program avoids that by using fixed strings, which is safe but inflexible.
- **Portability**: assumes a POSIX shell and GNU coreutils.
- **Performance**: each call forks a shell; fine for interactive menus.
- **Error handling**: return value is ignored here; failures are silent.
- **Environment**: aliases written to `~/.bashrc` only take effect in new shells (the
  program hints `source ~/.bashrc`).

## Better alternatives (for real projects)

Use POSIX APIs directly: `opendir`/`readdir`, `stat`, `chmod`, `mkdir`, `rmdir`,
`symlink`, `rename`, `unlink`, `open`/`read`/`write`. These avoid shell overhead and
injection risk. See [11-exercises.md](11-exercises.md).
