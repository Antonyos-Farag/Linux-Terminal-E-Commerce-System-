# Sample Session

A walkthrough of a typical run (abbreviated).

```
$ ./ecommerce
select your role:
1.admin
2.seller
3.customer
2
 seller_menu
1. list my products
2.create product file
...
Enter your choice: 2
Creating product file in /home/george-emad/Desktop/ecommerce/sellers:
...
Enter your choice: 1
Listing products in /home/george-emad/Desktop/ecommerce/sellers:
Product details
Enter your choice: 0
```

Then as customer:

```
$ ./ecommerce
...
3
  customer menu
...
Enter your choice: 1   # view products -> "Product details"
Enter your choice: 2   # purchase -> copies product1.txt
Enter your choice: 3   # history -> lists purchased_product1.txt
Enter your choice: 8   # feedback -> appends feedback.txt
Enter your choice: 0
```

## Notes

- Run `make setup-dirs` first so the directories exist.
- If you pick admin option 2 (chmod 700), later runs as a different user may fail;
  reset with `chmod 755 /home/george-emad/Desktop/ecommerce/admin`.
