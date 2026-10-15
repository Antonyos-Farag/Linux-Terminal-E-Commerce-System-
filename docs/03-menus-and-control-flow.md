# Menus and Control Flow

## Top-level dispatch (`main`)

```c
int main(){
    int role;
    printf("select your role:\n");
    printf("1.admin\n");
    printf("2.seller\n");
    printf("3.customer\n");
    if (scanf("%d", &role) != 1) { ... role = 3; }
    while (getchar() != '\n') { }
    switch(role){
        case 1: admin_menu();   break;
        case 2: seller_menu();  break;
        case 3: customer_menu(); break;
        default: printf("invalid role\n");
    }
    return 0;
}
```

- Reads the role once.
- On bad input, defaults to customer (role 3) — a deliberate (if questionable) fallback.
- Dispatches to exactly one menu function, then the program ends when that menu returns.

## Menu loop shape

Every menu follows the same skeleton:

```c
void xxx_menu(){
    int choice;
    do {
        // print options
        // read choice (with validation + buffer drain)
        switch (choice) {
            case 1: ... break;
            ...
            case 0: return;   // exit this menu
            default: printf("Invalid choice\n");
        }
    } while (choice != 0);
}
```

Key points:

- `do…while` guarantees the menu prints at least once.
- `case 0: return;` exits the function immediately (and thus the program, since `main`
  has nothing after the call).
- The loop condition `choice != 0` is technically redundant with the `return`, but makes
  intent clear.

## Sub-menus

Some options open a second, smaller menu read inline:

- Admin option 3 (create/delete directory) reads `dir_choice`.
- Admin option 9 (view file) reads `view_choice`.
- Seller option 5 reads `view_choice`; option 6 reads `find_choice`.
- Customer option 6 reads `view_choice`; option 7 reads `search_choice`.

These sub-reads reuse the same validation + drain pattern.

## Why `break` matters

Each `case` ends with `break;` to prevent **fall-through** into the next case. Without it,
selecting option 1 would also run option 2, 3, … — a classic C bug. (Note: in this code
some `case` blocks declare variables mid-block, which is legal in C99+ but requires
braces in older standards; here it compiles fine with `-std=c11`.)

## Flow diagram (admin)

```
main -> admin_menu
         ├─ 1 list files        -> system("ls -la ...")
         ├─ 2 chmod 700 admin   -> system("chmod ...")
         ├─ 3 mkdir/rmdir       -> system("mkdir"/"rmdir")
         ├─ 4 symlink           -> system("ln -s ...")
         ├─ 5 copy              -> system("cp ...")
         ├─ 6 append log        -> system("echo ... >> logs.txt")
         ├─ 7 alias             -> system("echo alias ... >> ~/.bashrc")
         ├─ 8 find              -> system("find ...")
         ├─ 9 cat/head/tail     -> system("cat/head/tail ...")
         └─ 0 return (exit)
```
