# Customer Menu (detailed)

Banner: `  customer menu `. Options 1–8 plus 0.

## 1. View products
`cat .../sellers/product*.txt` – browse all seller products.

## 2. Purchase products
`cp .../sellers/product1.txt .../customers/purchased_product1.txt` – simulates buying by
copying the product file into the customer area.

## 3. View purchase history
`ls -la .../customers` – lists purchased files.

## 4. View order status
Prints a hard-coded `Order Status: Shipped`. (No real tracking; placeholder.)

## 5. Add notes or preferences
`echo 'Customer preference: Express shipping' >> .../customers/customer_notes.txt`.

## 6. View order details
Sub-menu: cat / head -n 10 / tail -n 10 on `customers/purchased_product1.txt`.

## 7. Search orders
Sub-menu:
- 1 → `find .../customers -type f`
- 2 → `grep -r 'purchased' .../customers`

## 8. Feedback
`echo 'Customer feedback: 5 stars - Great product!' >> .../customers/feedback.txt`.

## 0. Exit
