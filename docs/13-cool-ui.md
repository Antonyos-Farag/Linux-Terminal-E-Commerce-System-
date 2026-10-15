# The Cool Edition (`ecommerce-cool`)

The original `src/ecommerce.c` is preserved verbatim. The **cool edition**
(`src/ecommerce_cool.c`, built as `./ecommerce-cool`) keeps the same teaching
goals — `do…while`, `switch`, `if/else`, `scanf` + `getchar`, `system()` and
shell commands, file operations, permissions, links, and search — and adds a
themed terminal UI around them.

```bash
make            # builds both ./ecommerce and ./ecommerce-cool
make run-cool   # run the cool edition
```

## What's different

| Area | Original | Cool edition |
|---|---|---|
| Look | plain `printf` menus | ANSI colors, box-drawing menus, ASCII-art banner |
| Motion | none | boot progress bars, action spinners, confetti, typewriter |
| Base path | hard-coded `/home/george-emad/Desktop/ecommerce` | chosen at runtime (see below), data tree auto-created |
| Purchase | one `cp` | order ID, stamped order file, printed receipt, order status file |
| Feedback | fixed "5 stars" line | you type a 1–5 rating (stars shown) + optional comment |
| Order status | hard-coded "Shipped" | read from a real file the purchase writes |
| `chmod 700` | one-way (locks you out) | sub-menu with **755 restore** |
| Alias | always appends to `~/.bashrc` (duplicates, stale `/tmp` path) | duplicate-guarded, points at the real base path |
| Invalid input | re-prompts | re-prompts with a hint, and **exits cleanly on EOF** (safe with piped input) |
| Menus | `0` quits the program | `0` returns to role select; role `0` quits |

## Where the data lives

The base path is resolved once at startup:

1. `BASE_PATH` if you compiled with `-DBASE_PATH=/some/path`
2. `$ECOMMERCE_HOME` environment variable
3. `$HOME/ecommerce` (the default)
4. `/tmp/ecommerce` (fallback)

On first run the program creates the tree itself (`admin/`, `sellers/`,
`customers/`, a sample product, an empty log), so `make setup-dirs` is only
needed for the *original* program.

```bash
ECOMMERCE_HOME=/tmp/mystore ./ecommerce-cool     # run from a custom location
```

## Terminal detection

- Colors, the boot sequence, spinners, confetti and screen clears are only
  enabled when **both** stdin and stdout are a TTY and `TERM` is set
  (and not `dumb`). Run it piped (`./ecommerce-cool < input > out.log`) and
  you get a clean, escape-free log — the sample session was recorded that way.
- 256-color gradients (banner, confetti) are used when the terminal reports
  support (`COLORTERM` or a `*256color*`/`xterm`-style `TERM`); otherwise a
  7-color cycle is used.

## Shell details worth noticing (educational)

- Every shell command is built with `snprintf` from the runtime base path —
  watch how format strings and `%s` arguments are composed.
- `system()` exit codes are checked with `WIFEXITED` / `WEXITSTATUS`
  (`<sys/wait.h>`) to print ✓ / ✗.
- Log entries use the shell's `$(date)` substitution; the order-header
  stamping uses a `{ …; } > file && mv` pipeline so the file is rewritten
  atomically.
- The alias command guards with `grep -qxF … || printf … >> ~/.bashrc` so
  repeated runs never duplicate the line.
- The feedback comment is sanitized (single quotes → backticks, capped at
  40 chars) before it is spliced into a shell command — a small lesson in
  why user input must be handled carefully.

## Try this session

```text
$ make run-cool
3          # customer
1          # view products  -> product card
2          # purchase       -> receipt + confetti
4          # order status   -> "Shipped (arrives in 3-5 business days)"
8          # feedback       -> rating 4, comment "Great build!"
0          # back to role select
2          # seller
1          # list products
0
1          # admin
1          # list files
6          # update logs (timestamped entry)
0
0          # quit
```

A recorded plain-mode session: [`examples/sample_session_cool.md`](../examples/sample_session_cool.md)
