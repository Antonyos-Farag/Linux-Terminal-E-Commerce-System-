# Admin Menu (detailed)

Printed banner: `  admin menu `. Options 1–9 plus 0 to exit.

## 1. List files
Runs `ls -la /home/.../ecommerce`. Shows everything including hidden files and
permissions. Good first step to "see" the store.

## 2. Change permissions
Runs `chmod 700 .../admin`. Demonstrates restricting a directory. After this, only the
owner can read/enter it.

## 3. Create/delete directories
Sub-menu:
- 1 → `mkdir -p .../admin/new_directory`
- 2 → `rmdir .../admin/new_directory`

Teaches the difference between creating (with parents) and removing (empty only).

## 4. Create symbolic link
`ln -s .../sellers .../admin/sellers_link`. Admin gains a shortcut view into seller
data without copying.

## 5. Copy files
`cp .../sellers/product1.txt .../admin/`. Demonstrates file duplication across
directories.

## 6. Update logs
`echo 'Log entry at $(date)' >> .../admin/logs.txt`. Appends a timestamped line. Note
`$(date)` is expanded by the shell at run time.

## 7. Create aliases
Appends `alias listfiles='ls -la /tmp/ecommerce'` to `~/.bashrc`. Persistent shortcut;
needs `source ~/.bashrc` to use in current shell.

## 8. Find files
`find .../ecommerce -name 'product1.txt'`. Locates a file by exact name.

## 9. View file content
Sub-menu: cat / head -n 10 / tail -n 10 on `admin/logs.txt`. Teaches the three common
viewing tools and the `-n` count option.

## 0. Exit
`return;` leaves `admin_menu`, ending the program.
