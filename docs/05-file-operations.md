# File Operations Walkthrough

## Directory layout expected

```
/home/george-emad/Desktop/ecommerce/
├── admin/
│   ├── logs.txt
│   ├── new_directory/        (created/deleted by admin option 3)
│   └── sellers_link -> ../sellers   (symlink, admin option 4)
├── sellers/
│   ├── product1.txt
│   ├── product1_copy.txt     (created by seller option 3)
│   └── moved_product1.txt    (created by seller option 4)
└── customers/
    ├── purchased_product1.txt
    ├── customer_notes.txt
    └── feedback.txt
```

## Operation-by-operation

### Listing (`ls -la`)
Shows permissions, owner, size, and modification time. Used by admin (option 1) and
customer history (option 3).

### Changing permissions (`chmod 700`)
`700` = owner rwx, group ---, others ---. Locks the admin directory to its owner only.

### Creating/removing directories (`mkdir -p` / `rmdir`)
`mkdir -p` creates parents as needed and doesn't fail if the dir exists. `rmdir` only
removes **empty** directories.

### Symbolic links (`ln -s target linkname`)
Creates `sellers_link` pointing at `sellers`, letting admin browse seller files through
the link.

### Copying (`cp src dst`)
Used to "publish" a product and to "purchase" (copy product file into customers).

### Moving (`mv src dst`)
Renames/moves a file; used to simulate moving a product.

### Viewing (`cat` / `head -n 10` / `tail -n 10`)
- `cat` – whole file.
- `head -n 10` – first 10 lines.
- `tail -n 10` – last 10 lines.

### Finding (`find path -name 'product1.txt'`, `find path -type f`)
Locates files by name or type.

### Searching content (`grep -r 'pattern' path`)
Recursively searches file contents.

### Appending (`echo '...' >> file`)
`>>` appends; `>` would overwrite. Used for logs, notes, feedback.

### Aliases (`alias name='command'` in `~/.bashrc`)
Persists a shortcut across shells. Activated with `source ~/.bashrc`.

## Redirection & globs recap

- `>` overwrite, `>>` append.
- `*` glob matches any filename (e.g. `product*.txt`).
- `~` expands to the home directory.
