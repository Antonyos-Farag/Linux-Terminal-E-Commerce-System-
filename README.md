# Linux Terminal E-Commerce System

An **educational** C project that wraps everyday Linux shell commands in an interactive
role-based menu (Admin / Seller / Customer). It is a guided tour of:

- C control flow (`do…while`, `switch`, `if/else`)
- Input handling with `scanf` and `getchar`
- The `system()` function and shell commands
- File operations, permissions, links, search, and redirection

## Quick start

```bash
make setup-dirs   # create demo data tree
make              # build ./ecommerce
make run          # build and run
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

## Source

- [`src/ecommerce.c`](src/ecommerce.c) – the original program (preserved verbatim).
- [`examples/sample_session.md`](examples/sample_session.md) – sample run.
- [`tests/test_compile.sh`](tests/test_compile.sh) – compile smoke test.

## Build targets

```
make            build
make run        build + run
make setup-dirs create demo data
make clean      remove artifacts
```

## Notes

- The base path is hard-coded to `/home/george-emad/Desktop/ecommerce`. Adjust it (or run
  `make setup-dirs`) for your machine. See docs/10.
- Educational only: not production-ready (see docs/06).
