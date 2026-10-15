#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#define BASE_PATH "/home/george-emad/Desktop/ecommerce"

void admin_menu(){
    int choice;
    do{
        printf("  admin menu \n");
        printf("1.list files\n");
        printf("2.change permissions\n");
        printf("3.create/delete directories\n");
        printf("4.Create symbolic link files\n");
        printf("5.copy files\n");
        printf("6.update logs\n");
        printf("7.Create aliasses\n");
        printf("8.Find files\n");
        printf("9.View file content (cat/head/tail)\n");
        printf("0.exit\n");
        printf("Enter your choice:\n");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Please enter a number.\n");
            while (getchar() != '\n') { /* clear input */ }
            continue;
        }
        while (getchar() != '\n') { /* clear input */ }

        switch (choice) {
            case 1: /*list files*/
                printf("Listing files in /home/george-emad/Desktop/ecommerce:\n");
                system("ls -la /home/george-emad/Desktop/ecommerce");
                break;
            case 2: /*change permissions*/
                printf("Changing permissions of /home/george-emad/Desktop/ecommerce/admin to 700\n");
                system("chmod 700 /home/george-emad/Desktop/ecommerce/admin");
                break;
            case 3: /*create/delete directories*/
                printf("Creating/deleting directories in /home/george-emad/Desktop/ecommerce:\n");
                printf("1. Create directory\n2. Delete directory\n");
                int dir_choice;
                if (scanf("%d", &dir_choice) != 1) {
                    printf("Invalid input!\n");
                    while (getchar() != '\n') { /* clear input */ }
                    break;
                }
                while (getchar() != '\n') { /* clear input */ }
                if (dir_choice == 1) {
                    system("mkdir -p /home/george-emad/Desktop/ecommerce/admin/new_directory");
                    printf("Directory created: /home/george-emad/Desktop/ecommerce/admin/new_directory\n");
                } else if (dir_choice == 2) {
                    system("rmdir /home/george-emad/Desktop/ecommerce/admin/new_directory");
                    printf("Directory deleted: /home/george-emad/Desktop/ecommerce/admin/new_directory\n");
                } else {
                    printf("Invalid choice\n");
                }
                break;
            case 4: /*Create symbolic link files*/
                printf("Creating symbolic link in /home/george-emad/Desktop/ecommerce/admin:\n");
                system("ln -s /home/george-emad/Desktop/ecommerce/sellers /home/george-emad/Desktop/ecommerce/admin/sellers_link");
                printf("Symbolic link created: /home/george-emad/Desktop/ecommerce/admin/sellers_link -> /home/george-emad/Desktop/ecommerce/sellers\n");
                break;
            case 5: /*copy files*/
                printf("Copying file from /home/george-emad/Desktop/ecommerce/sellers to /home/george-emad/Desktop/ecommerce/admin:\n");
                system("cp /home/george-emad/Desktop/ecommerce/sellers/product1.txt /home/george-emad/Desktop/ecommerce/admin/");
                printf("File copied to /home/george-emad/Desktop/ecommerce/admin/\n");
                break;
            case 6: /*update logs*/
                printf("Updating logs in /home/george-emad/Desktop/ecommerce/admin/logs.txt:\n");
                system("echo 'Log entry at $(date)' >> /home/george-emad/Desktop/ecommerce/admin/logs.txt");
                printf("Logs updated: /home/george-emad/Desktop/ecommerce/admin/logs.txt\n");
                break;
            case 7: /*Create aliasses*/
                printf("Creating alias for listing files:\n");
                system("echo \"alias listfiles='ls -la /tmp/ecommerce'\" >> ~/.bashrc");
                printf("Alias created: listfiles (run: source ~/.bashrc)\n");
                break;
            case 8: /*Find files*/
                printf("Finding files in /home/george-emad/Desktop/ecommerce:\n");
                system("find /home/george-emad/Desktop/ecommerce -name 'product1.txt'");
                break;
            case 9: /*View file content*/
                printf("View file content options:\n");
                printf("1. View entire file (cat)\n2. View first 10 lines (head)\n3. View last 10 lines (tail)\n");
                int view_choice;
                if (scanf("%d", &view_choice) != 1) {
                    printf("Invalid input!\n");
                    while (getchar() != '\n') { /* clear input */ }
                    break;
                }
                while (getchar() != '\n') { /* clear input */ }
                if (view_choice == 1) {
                    system("cat /home/george-emad/Desktop/ecommerce/admin/logs.txt");
                } else if (view_choice == 2) {
                    system("head -n 10 /home/george-emad/Desktop/ecommerce/admin/logs.txt");
                } else if (view_choice == 3) {
                    system("tail -n 10 /home/george-emad/Desktop/ecommerce/admin/logs.txt");
                } else {
                    printf("Invalid choice\n");
                }
                break;
            case 0:
                return;
            default:
                printf("Invalid choice\n");
        }
    }while(choice != 0);
}

void seller_menu(){
    int choice;
    do{
        printf(" seller_menu \n");
        printf("1. list my products\n");
        printf("2.create product file\n");
        printf("3.copy product\n");
        printf("4.move product\n");
        printf("5.view product (cat/head/tail)\n");
        printf("6.find product by name or content\n");
        printf("7.use aliasses\n");
        printf("0.exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Please enter a number.\n");
            while (getchar() != '\n') { /* clear input */ }
            continue;
        }
        while (getchar() != '\n') { /* clear input */ }
        switch (choice) {
            case 1: /*list my products*/
                printf("Listing products in /home/george-emad/Desktop/ecommerce/sellers:\n");
                system("cat /home/george-emad/Desktop/ecommerce/sellers/product*.txt");
                break;
            case 2: /*create product file*/
                printf("Creating product file in /home/george-emad/Desktop/ecommerce/sellers:\n");
                system("echo 'Product details' > /home/george-emad/Desktop/ecommerce/sellers/product1.txt");
                break;
            case 3: /*copy product*/
                printf("Copying product file in /home/george-emad/Desktop/ecommerce/sellers:\n");
                system("cp /home/george-emad/Desktop/ecommerce/sellers/product1.txt /home/george-emad/Desktop/ecommerce/sellers/product1_copy.txt");
                break;
            case 4: /*move product*/
                printf("Moving product file in /home/george-emad/Desktop/ecommerce/sellers:\n");
                system("mv /home/george-emad/Desktop/ecommerce/sellers/product1_copy.txt /home/george-emad/Desktop/ecommerce/sellers/moved_product1.txt");
                break;
            case 5: /*view product with options*/
                printf("View product details:\n");
                printf("1. View entire file (cat)\n2. View first 10 lines (head)\n3. View last 10 lines (tail)\n");
                int view_choice;
                if (scanf("%d", &view_choice) != 1) {
                    printf("Invalid input!\n");
                    while (getchar() != '\n') { /* clear input */ }
                    break;
                }
                while (getchar() != '\n') { /* clear input */ }
                if (view_choice == 1) {
                    system("cat /home/george-emad/Desktop/ecommerce/sellers/product1.txt");
                } else if (view_choice == 2) {
                    system("head -n 10 /home/george-emad/Desktop/ecommerce/sellers/product1.txt");
                } else if (view_choice == 3) {
                    system("tail -n 10 /home/george-emad/Desktop/ecommerce/sellers/product1.txt");
                } else {
                    printf("Invalid choice\n");
                }
                break;
            case 6: /*find product*/
                printf("Find products:\n");
                printf("1. Find by name\n2. Search file content\n");
                int find_choice;
                if (scanf("%d", &find_choice) != 1) {
                    printf("Invalid input!\n");
                    while (getchar() != '\n') { /* clear input */ }
                    break;
                }
                while (getchar() != '\n') { /* clear input */ }
                if (find_choice == 1) {
                    printf("Finding products by name in /home/george-emad/Desktop/ecommerce/sellers:\n");
                    system("find /home/george-emad/Desktop/ecommerce/sellers -type f");
                } else if (find_choice == 2) {
                    printf("Searching product content in /home/george-emad/Desktop/ecommerce/sellers:\n");
                    system("grep -r 'Product' /home/george-emad/Desktop/ecommerce/sellers");
                } else {
                    printf("Invalid choice\n");
                }
                break;
            case 7: /*use aliasses*/
                printf("Using alias to list files:\n");
                system("bash -lc \"source ~/.bashrc && listfiles\"");
                break;
            case 0:
                return;
            default:
                printf("Invalid choice\n");}
    }while(choice != 0);
}

void customer_menu(){
    int choice;
    do{
        printf("  customer menu \n");
        printf("1.view products\n");
        printf("2.purchase products\n");
        printf("3.view purchase history\n");
        printf("4.view order status\n");
        printf("5.add notes or preferences to order\n");
        printf("6.view order details (cat/head/tail)\n");
        printf("7.search orders by ID or content\n");
        printf("8.feedback\n");
        printf("0.exit\n");
        printf("Enter your choice:\n");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Please enter a number.\n");
            while (getchar() != '\n') { /* clear input */ }
            continue;
        }
        while (getchar() != '\n') { /* clear input */ }
        switch(choice){
            case 1: /*view products*/
                printf("Viewing products in /home/george-emad/Desktop/ecommerce/sellers:\n");
                system("cat /home/george-emad/Desktop/ecommerce/sellers/product*.txt");
                break;
            case 2: /*purchase products*/
                printf("Purchasing product from /home/george-emad/Desktop/ecommerce/sellers:\n");
                system("cp /home/george-emad/Desktop/ecommerce/sellers/product1.txt /home/george-emad/Desktop/ecommerce/customers/purchased_product1.txt");
                break;
            case 3: /*view purchase history*/
                printf("Viewing purchase history in /home/george-emad/Desktop/ecommerce/customers:\n");
                system("ls -la /home/george-emad/Desktop/ecommerce/customers");
                break;
            case 4: /*view order status*/
                printf("Viewing order status:\n");
                printf("Order Status: Shipped\n\n\n");
                break;
            case 5: /*add notes or preferences*/
                printf("Add notes or preferences to your order:\n");
                system("echo 'Customer preference: Express shipping' >> /home/george-emad/Desktop/ecommerce/customers/customer_notes.txt");
                printf("Notes added to order!\n");
                break;
            case 6: /*view order details*/
                printf("View order details:\n");
                printf("1. View entire file (cat)\n2. View first 10 lines (head)\n3. View last 10 lines (tail)\n");
                int view_choice;
                if (scanf("%d", &view_choice) != 1) {
                    printf("Invalid input!\n");
                    while (getchar() != '\n') { /* clear input */ }
                    break;
                }
                while (getchar() != '\n') { /* clear input */ }
                if (view_choice == 1) {
                    system("cat /home/george-emad/Desktop/ecommerce/customers/purchased_product1.txt");
                } else if (view_choice == 2) {
                    system("head -n 10 /home/george-emad/Desktop/ecommerce/customers/purchased_product1.txt");
                } else if (view_choice == 3) {
                    system("tail -n 10 /home/george-emad/Desktop/ecommerce/customers/purchased_product1.txt");
                } else {
                    printf("Invalid choice\n");
                }
                break;
            case 7: /*search orders*/
                printf("Search your orders:\n");
                printf("1. Find by order ID/filename\n2. Search order content\n");
                int search_choice;
                if (scanf("%d", &search_choice) != 1) {
                    printf("Invalid input!\n");
                    while (getchar() != '\n') { /* clear input */ }
                    break;
                }
                while (getchar() != '\n') { /* clear input */ }
                if (search_choice == 1) {
                    printf("Finding your orders in /home/george-emad/Desktop/ecommerce/customers:\n");
                    system("find /home/george-emad/Desktop/ecommerce/customers -type f");
                } else if (search_choice == 2) {
                    printf("Searching order content:\n");
                    system("grep -r 'purchased' /home/george-emad/Desktop/ecommerce/customers");
                } else {
                    printf("Invalid choice\n");
                }
                break;
            case 8: /*feedback*/
                printf("Submitting feedback (rate 1-5):\n");
                printf("Adding your feedback to order history:\n");
                system("echo 'Customer feedback: 5 stars - Great product!' >> /home/george-emad/Desktop/ecommerce/customers/feedback.txt");
                printf("Thank you for your feedback!\n");
                break;
            case 0:
                return;
            default:
                printf("Invalid choice\n");
        }
    }while(choice != 0);
}

int main(){
    int role;
    printf("select your role:\n");
    printf("1.admin\n");
    printf("2.seller\n");
    printf("3.customer\n");
    if (scanf("%d", &role) != 1) {
        printf("Invalid input! Please enter a number.\n");
        while (getchar() != '\n') { /* clear input */ }
        printf("Defaulting to customer role.\n");
        role = 3;
    }
    while (getchar() != '\n') { /* clear input */ }
    switch(role){
        case 1:
            admin_menu();
            break;
        case 2:
            seller_menu();
            break;
        case 3:
            customer_menu();
            break;
        default:
            printf("invalid role\n");
    }

    return 0;
}
