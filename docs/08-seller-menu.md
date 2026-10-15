# Seller Menu (detailed)

Banner: ` seller_menu `. Options 1–7 plus 0.

## 1. List my products
`cat .../sellers/product*.txt` – prints contents of all product files (glob).

## 2. Create product file
`echo 'Product details' > .../sellers/product1.txt` – creates/overwrites product1.txt.

## 3. Copy product
`cp .../sellers/product1.txt .../sellers/product1_copy.txt`.

## 4. Move product
`mv .../sellers/product1_copy.txt .../sellers/moved_product1.txt`. Renames the copy.

## 5. View product
Sub-menu: cat / head -n 10 / tail -n 10 on `sellers/product1.txt`.

## 6. Find product by name or content
Sub-menu:
- 1 → `find .../sellers -type f` (list all files)
- 2 → `grep -r 'Product' .../sellers` (search contents)

## 7. Use aliases
`bash -lc "source ~/.bashrc && listfiles"` – spawns a login shell, sources bashrc, and
runs the alias. Shows how aliases are loaded.

## 0. Exit
