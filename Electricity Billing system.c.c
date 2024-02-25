#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Customer {
    char c_Name[20], c_Months[25];
    int c_id;
    int SerialNo;
};

struct Bill {
    int b_id;
    int cid;
    char name[20], months[25];
    int serialNo;
    int unit;
    float total_charge, amount;
    float extra_charge, charge, netamount;
};

struct Customer* customer_head = NULL;
struct Bill* bill_head = NULL;

int Customer_ID() {
    int a;
    FILE* fp;
    fp = fopen("cus.txt", "r");
    if (fp == NULL)
        a = 1;
    else {
        fscanf(fp, "%d", &a);
        a++;
        return a;
    }
    fclose(fp);

    fp = fopen("cus.txt", "w");
    fprintf(fp, "%d", a);
    fclose(fp);
}

int Bill_ID() {
    int y;
    FILE* fp;
    fp = fopen("bi.txt", "r");
    if (fp == NULL)
        y = 1;
    else {
        fscanf(fp, "%d", &y);
        y++;
    }
    fclose(fp);

    fp = fopen("bi.txt", "w");
    fprintf(fp, "%d", y);
    fclose(fp);
    return y;
}

void BubbleSort(struct Bill arr[], int n) {
    int i, j;
    struct Bill temp;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (arr[j].b_id > arr[j + 1].b_id) {
                // Swap the bills if they are in the wrong order
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void Add_Customers() {
    FILE* ptr;
    int i;
    struct Customer c;
    ptr = fopen("cust.txt", "a");
    fflush(stdin);

    printf("\n ENTER CUSTOMER DETAIL :-  ");
    printf("\n Name:  ");
    gets(c.c_Name);
    printf("Input Serial Number: ");
    scanf("%d", &c.SerialNo);
    fflush(stdin);
    c.c_id = Customer_ID();

    printf("\n Record added successfully!!!! ");
    printf("\n Your customer id is: %d", c.c_id);

    fwrite(&c, sizeof(c), 1, ptr);
    fclose(ptr);
}

void Generate_bill() {
    int find, flag = 0;
    FILE* ptr, * ptr1;
    struct Customer c;
    struct Bill b;
    printf("\n Enter your customer id:  ");
    scanf("%d", &find);

    ptr = fopen("cust.txt", "r");
    while (!feof(ptr)) {
        fread(&c, sizeof(c), 1, ptr);
        if (c.c_id == find) {
            flag = 1;
            ptr1 = fopen("bill1.txt", "a");
            b.b_id = Bill_ID();
            strcpy(b.name, c.c_Name);
            b.cid = c.c_id;

            printf("Which month bill you want to pay?: ");
            scanf("%s", b.months);
            printf("Enter number of unit consumed in this month: ");
            scanf("%d", &b.unit);
            if (b.unit <= 20)
                b.charge = 3;
            else if (b.unit <= 30)
                b.charge = 7;
            else if (b.unit <= 50)
                b.charge = 8.5;
            else if (b.unit <= 150)
                b.charge = 10;
            else if (b.unit <= 250)
                b.charge = 11;
            else if (b.unit <= 400)
                b.charge = 12;
            else
                b.charge = 13;

            b.total_charge = b.charge * b.unit;

            if (b.total_charge > 400) {
                b.extra_charge = b.total_charge * 15 / 100.0;
                b.netamount = b.extra_charge + b.total_charge;
            } else {
                b.extra_charge = 0;
                b.netamount = b.total_charge;
            }

            if (b.netamount < 100) {
                b.netamount = 100;
            }

            fflush(stdin);
            printf("\n Bill has been generated successfully!!!!\n   ");
            printf("   Your bill id is: %d\n", b.b_id);
            fwrite(&b, sizeof(b), 1, ptr1);
            fclose(ptr1);
            break;
        }
    }
    if (flag == 0)
        printf("\n Error! No such customer with id no: %d exist.", find);
    fclose(ptr);

    // Sort bills after adding a new bill
    struct Bill bills[100]; // Assuming a maximum of 100 bills, adjust as needed
    int numberOfBills = 0;

    // Read bills from the file into an array
    ptr1 = fopen("bill1.txt", "r");
    while (fread(&bills[numberOfBills], sizeof(b), 1, ptr1)) {
        numberOfBills++;
    }
    fclose(ptr1);

    // Apply bubble sort
    BubbleSort(bills, numberOfBills);

    // Write the sorted bills back to the file
    ptr1 = fopen("bill1.txt", "w");
    for (int i = 0; i < numberOfBills; i++) {
        fwrite(&bills[i], sizeof(b), 1, ptr1);
    }
    fclose(ptr1);
}

void Display_bill() {
    int flag = 0, billid, custid;
    FILE* ptr1;
    struct Bill b;

    ptr1 = fopen("bill1.txt", "r");
    printf("Enter your customer id: ");
    scanf("%d", &custid);
    fflush(stdin);
    printf("Enter bill id. whose bill you want to display:  ");
    scanf("%d", &billid);

    while (!feof(ptr1)) {
        fflush(stdin);

        fread(&b, sizeof(b), 1, ptr1);
        if ((b.b_id == billid) && (b.cid == custid)) {
            flag++;

            printf("\n-----Electricity Bill-----\n");
            printf("Bill id        :- %d\n", b.b_id);
            printf("Customer ID NO: %d\n", b.cid);
            printf("Customer Name: %s\n", b.name);
            printf("Month: %s\n", b.months);
            printf("Unit Consumed: %d\n", b.unit);
            printf("Amount Charges @ Rs. %4.2f per unit: %8.2f\n", b.charge, b.total_charge);
            printf("Extra Amount: %8.2f\n", b.extra_charge);
            printf("Net Amount Paid By the Customer: %8.2f\n", b.netamount);
            break;
        }
    }
    if (flag == 0)
        printf("\n Error! No such customer id no.  %d  OR bill no. %d  exist.", custid, billid);
    fclose(ptr1);
}

void Delete_bill() {
    int flag = 0, billid, custid;
    FILE* ptr, * ptr1;
    struct Bill b;

    ptr = fopen("bill1.txt", "r");
    ptr1 = fopen("temp.txt", "w");
    printf("\nEnter bill id. whose bill you want to delete:  ");
    scanf("%d", &billid);
    while (!feof(ptr)) {
        fread(&b, sizeof(b), 1, ptr);
        if (b.b_id == billid) {
            printf("\n Bill with id no.  %d DELETED successfully!!!", b.b_id);
            flag = 1;
        } else
            fwrite(&b, sizeof(b), 1, ptr1);
    }

    fclose(ptr);
    fclose(ptr1);
    if (flag == 0)
        printf("\n Error! No such bill with id no.  %d  exist.", billid);
    remove("bill1.txt");
    rename("temp.txt", "bill1.txt");
}

void Search_bill(int custid, int billid) {
    FILE* ptr1;
    struct Bill b;
    int found = 0;

    ptr1 = fopen("bill1.txt", "r");
    while (fread(&b, sizeof(b), 1, ptr1)) {
        if (b.cid == custid && b.b_id == billid) {
            printf("-----Electricity Bill-----\n");
            printf("Bill ID: %d\n", b.b_id);
            printf("Customer ID: %d\n", b.cid);
            printf("Customer Name: %s\n", b.name);
            printf("Month: %s\n", b.months);
            printf("Unit Consumed: %d\n", b.unit);
            printf("Amount Charges @ Rs. %4.2f per unit: %8.2f\n", b.charge, b.total_charge);
            printf("Extra Amount: %8.2f\n", b.extra_charge);
            printf("Net Amount Paid By the Customer: %8.2f\n", b.netamount);
            found = 1;
            break;
        }
    }
    if (!found)
        printf("\n Error! No such customer id no. %d OR bill no. %d exist.", custid, billid);
    fclose(ptr1);
}

int main() {
    int choice = 1;
    while (choice != 6) {
        printf("\n **************************************  ");
        printf("\n      Electricity Bill Calculator");
        printf("\n **************************************  ");

        printf("\n  <<<<<<-MAIN MENU->>>>>>  ");
        printf("\n  1. Add new customer  ");
        printf("\n  2. Bill Generation  ");
        printf("\n  3. Display bill  ");
        printf("\n  4. Delete bill  ");
        printf("\n  5. Search Bill by Customer ID and Bill ID");
        printf("\n  6. EXIT ");
        printf("\n\n Enter your choice :   ");
        scanf("%d", &choice);
        switch (choice) {
        case 1:
            Add_Customers();
            break;
        case 2:
            Generate_bill();
            break;
        case 3:
            Display_bill();
            break;
        case 4:
            Delete_bill();
            break;
        case 5: {
            int custid, billid;
            printf("Enter customer ID: ");
            scanf("%d", &custid);
            printf("Enter bill ID: ");
            scanf("%d", &billid);
            Search_bill(custid, billid);
            break;
        }
        case 6:
            printf("Exiting.....Press any key !!!");
            break;
        default:
            printf("\n\n  Invalid Choice...!");
            printf("\n\n Press any key to continue..");
            getchar(); // Consume the newline character left in the input buffer
            break;
        }
    }

    return 0;
}