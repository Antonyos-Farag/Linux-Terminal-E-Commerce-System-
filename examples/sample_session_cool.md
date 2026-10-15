# Sample Session — Cool Edition

A typical run of `./ecommerce-cool`, recorded in plain mode (piped input,
no TTY — so no colors or animations). On a real terminal you also get the
boot progress bars, the rainbow ASCII banner, spinners while commands run,
and confetti after a purchase.

```
$ make run-cool
terminal commerce v2.0 (cool edition)
base: /home/user/ecommerce
  ~/ecommerce  ·  role: —  ·  Thu Oct 15 15:07:59

  ╭──────────────────────────────────────────────╮
  │  TERMINAL COMMERCE          choose your role │
  ╰──────────────────────────────────────────────╯

   [1]  👑  Admin                 back office & file ops
   [2]  🏪  Seller                manage the products
   [3]  🛒  Customer              browse & checkout
   [0]   exit the store

  ➜ choose 0..3: 3
```

As customer — browse, buy, give feedback:

```
  ╭──────────────────────────────────────────────╮
  │  CUSTOMER STORE            browse & checkout │
  ╰──────────────────────────────────────────────╯

   [1]  🛍️  View products         browse the catalog
   [2]  🧾  Purchase product      checkout + receipt
   ...
  ➜ choose 0..8: 1            # view products
  ─ product catalog ─
Product: Retro Mech Keyboard
Price: $89.99
Stock: 42
Condition: New
  ✓  showing the catalog

  ➜ choose 0..8: 2            # purchase
  ✓  packing your order
  ✓  stamping the order header

            TERMINAL COMMERCE
             official receipt
  ──────────────────────────────────────
  TC-474F  ·  Thu Oct 15 15:07:59 2026
  ──────────────────────────────────────
  Retro Mech Keyboard
  x1  ............................$89.99
  ──────────────────────────────────────
  TOTAL  .........................$89.99
  ──────────────────────────────────────
         **** thank you ****

  ➜ choose 0..8: 4            # order status
Order TC-474F — Retro Mech Keyboard
Status: Shipped (arrives in 3-5 business days)
  ✓  checking order status

  ➜ choose 0..8: 8            # feedback
   rate your experience: 1-5
  ➜ choose 0..5: 4
   ★★★★☆
  ➜ one-line comment (optional): Great build!
Customer feedback: 4 stars - Great build!
  ✓  recording your feedback

  ➜ choose 0..8: 0            # back to role select
```

As seller and admin:

```
  ➜ choose 0..3: 2            # seller
  ➜ choose 0..7: 1            # list products
Product: Retro Mech Keyboard
Price: $89.99
Stock: 42
Condition: New
  ✓  listing your products

  ➜ choose 0..7: 0
  ➜ choose 0..3: 1            # admin
  ➜ choose 0..9: 1            # list files
total 4
drwxr-xr-x 5 user user   60 Oct 15 15:07 .
drwx------ 5 user user 4096 Oct 15 15:07 ..
drwxr-xr-x 2 user user   60 Oct 15 15:07 admin
drwxr-xr-x 2 user user  128 Oct 15 15:07 customers
drwxr-xr-x 2 user user   60 Oct 15 15:07 sellers
  ✓  listing files

  ➜ choose 0..9: 6            # update logs
[Thu Oct 15 15:07:59 UTC 2026] admin: state snapshot recorded
  ✓  appending to admin/logs.txt

  ➜ choose 0..9: 0
  ➜ choose 0..3: 0

  👋 thanks for shopping — see you in the shell!
```

## Notes

- The data tree (`$HOME/ecommerce`) is created automatically on first run.
  Override with `ECOMMERCE_HOME=... ./ecommerce-cool`.
- Menu `0` returns to the role select screen; role `0` quits (in the
  original, menu `0` quit the program).
- Repeated admin option 2 → 1 (`chmod 700`) is now recoverable with
  option 2 → 2 (`chmod 755`).
