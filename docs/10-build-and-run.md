# Build and Run

## Prerequisites

- A C compiler (`gcc` or `cc`).
- A POSIX environment with coreutils (`ls`, `chmod`, `mkdir`, …).
- The base directory tree (see below).

## Build

```bash
make            # produces ./ecommerce
make clean      # remove artifacts
```

Or manually:

```bash
gcc -Wall -Wextra -Wpedantic -std=c11 -O2 -o ecommerce src/ecommerce.c
```

## Prepare data directory

```bash
make setup-dirs
```

This creates `/home/george-emad/Desktop/ecommerce/{admin,sellers,customers}` with a
sample product and an empty log file. Override the path by editing `BASE_PATH` in the
source (or pass `-DBASE_PATH=...` after refactoring to use the macro).

## Run

```bash
make run
# or
./ecommerce
```

Then choose a role (1/2/3) and follow the menus. Enter `0` to exit a menu.

### The cool edition

`make` also builds `./ecommerce-cool`. It needs no `setup-dirs`: it creates
its own data tree under `$HOME/ecommerce` on first run (override with
`ECOMMERCE_HOME=...` or `-DBASE_PATH=...`). Run it in a real terminal to see
colors and animations; run it with output redirected and it falls back to a
clean, escape-free mode. Details: [13-cool-ui.md](13-cool-ui.md).

## Running in this sandbox

The sandbox user is not `george-emad`, so `/home/george-emad` may not exist. Either:

- Create it: `sudo mkdir -p /home/george-emad/Desktop` (if permitted), or
- Change `BASE_PATH` to a writable location such as `/tmp/ecommerce` and rebuild.

## Troubleshooting

- `scanf` loop / "Invalid input": type a number and press Enter.
- Shell errors like `No such file or directory`: run `make setup-dirs` first.
- Permission denied on admin dir: you previously ran option 2 (chmod 700); run as the
  same owner or `chmod 755` it back.
