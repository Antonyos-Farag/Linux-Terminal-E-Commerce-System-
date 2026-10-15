# Linux Terminal E-Commerce System

An **educational** C project that wraps everyday Linux shell commands in an
interactive role-based menu (Admin / Seller / Customer). It is a guided tour
of:

- C control flow (`do…while`, `switch`, `if/else`)
- Input handling with `scanf` and `getchar`
- The `system()` function and shell commands
- File operations, permissions, links, search, and redirection

Two programs ship in the same source layout:

- `./ecommerce` — the **original** program (preserved verbatim).
- `./ecommerce-cool` — the **cool edition**: same teaching goals, but with
  ANSI colors, an ASCII-art banner, a boot sequence, spinners, a printed
  receipt, confetti, and a runtime-configurable base path.
  See [docs/13-cool-ui.md](docs/13-cool-ui.md).

## Quick start

```bash
make            # builds both ./ecommerce and ./ecommerce-cool
make run        # run the original (needs `make setup-dirs` first)
make run-cool   # run the cool edition (auto-creates its data tree)
```

## Documentation

Start here: [`docs/01-overview.md`](docs/01-overview.md)

| Chapter | Topic |
|---|---|
| 01 | Overview |
| 02 | C basics used |
| 03 | Menus and control flow |
| 04 | `system()` and the shell |
| 05 | File operations |
| 06 | Permissions and security |
| 07 | Admin menu |
| 08 | Seller menu |
| 09 | Customer menu |
| 10 | Build and run |
| 11 | Exercises |
| 12 | Glossary |
| 13 | The cool edition (`ecommerce-cool`) |

## Source

- [`src/ecommerce.c`](src/ecommerce.c) – the original program (preserved verbatim).
- [`src/ecommerce_cool.c`](src/ecommerce_cool.c) – the cool edition (themed terminal UI).
- [`examples/sample_session.md`](examples/sample_session.md) – sample run (original).
- [`examples/sample_session_cool.md`](examples/sample_session_cool.md) – sample run (cool edition).
- [`tests/test_compile.sh`](tests/test_compile.sh) – compile + smoke test for both programs.

## Build targets

```
make            build both programs
make run        build + run the original
make run-cool   build + run the cool edition
make setup-dirs create demo data for the original
make clean      remove artifacts
```

## Notes

- The original program's base path is hard-coded to
  `/home/george-emad/Desktop/ecommerce`. Adjust it (or run `make setup-dirs`)
  for your machine. See docs/10.
- The cool edition picks its base path at runtime: `$ECOMMERCE_HOME`,
  otherwise `$HOME/ecommerce`, otherwise `/tmp/ecommerce` — and creates the
  demo tree on first run. See docs/13.
- Educational only: not production-ready (see docs/06).
