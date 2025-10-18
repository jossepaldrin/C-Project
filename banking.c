#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Structures
struct Account {
    int account_number;
    char name[50];
    float balance;
    struct Account *next;
};

struct Transaction {
    int account_number;
    char type[15];     // "Deposit" or "Withdrawal"
    float amount;
    char date[20];
    struct Transaction *next;
};

// Global Pointers
struct Account *accountHead = NULL;
struct Transaction *transactionHead = NULL;
int accountCounter = 1000; // starting account number

// Utility Function: Get Current Date
void getCurrentDate(char *dateStr) {
    time_t t;
    struct tm *tm_info;
    time(&t);
    tm_info = localtime(&t);
    strftime(dateStr, 20, "%d-%m-%Y", tm_info);
}

// Create New Account
void createAccount() {
    struct Account *newAccount = (struct Account *)malloc(sizeof(struct Account));
    newAccount->account_number = ++accountCounter;
    printf("Enter customer name: ");
    scanf(" %[^\n]", newAccount->name);
    printf("Enter opening balance: ");
    scanf("%f", &newAccount->balance);
    newAccount->next = NULL;

    // Insert at end of account list
    if (accountHead == NULL) {
        accountHead = newAccount;
    } else {
        struct Account *temp = accountHead;
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = newAccount;
    }
    printf("\nAccount created successfully! Account Number: %d\n", newAccount->account_number);
}

// Display All Accounts
void displayAllAccounts() {
    struct Account *temp = accountHead;
    if (temp == NULL) {
        printf("\nNo accounts found.\n");
        return;
    }
    printf("\n----- All Accounts -----\n");
    printf("%-10s %-20s %-10s\n", "Acc.No", "Name", "Balance");
    printf("-------------------------------------------\n");
    while (temp != NULL) {
        printf("%-10d %-20s %-10.2f\n", temp->account_number, temp->name, temp->balance);
        temp = temp->next;
    }
    printf("-------------------------------------------\n");
}

// Log Transaction
void logTransaction(int acc_no, char *type, float amount) {
    struct Transaction *newTrans = (struct Transaction *)malloc(sizeof(struct Transaction));
    newTrans->account_number = acc_no;
    strcpy(newTrans->type, type);
    newTrans->amount = amount;
    getCurrentDate(newTrans->date);
    newTrans->next = NULL;
    if (transactionHead == NULL)
        transactionHead = newTrans;
    else {
        struct Transaction *temp = transactionHead;
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = newTrans;
    }
}

// Deposit Amount
void depositAmount() {
    int acc_no;
    float amount;
    printf("Enter account number: ");
    scanf("%d", &acc_no);
    struct Account *temp = accountHead;
    while (temp != NULL && temp->account_number != acc_no)
        temp = temp->next;
    if (temp == NULL) {
        printf("Account not found.\n");
        return;
    }
    printf("Enter amount to deposit: ");
    scanf("%f", &amount);
    temp->balance += amount;
    logTransaction(acc_no, "Deposit", amount);
    printf("Amount deposited successfully! Current Balance: %.2f\n", temp->balance);
}

// Withdraw Amount
void withdrawAmount() {
    int acc_no;
    float amount;
    printf("Enter account number: ");
    scanf("%d", &acc_no);
    struct Account *temp = accountHead;
    while (temp != NULL && temp->account_number != acc_no)
        temp = temp->next;
    if (temp == NULL) {
        printf("Account not found.\n");
        return;
    }
    printf("Enter amount to withdraw: ");
    scanf("%f", &amount);
    if (temp->balance < amount) {
        printf("Insufficient balance!\n");
        return;
    }
    temp->balance -= amount;
    logTransaction(acc_no, "Withdrawal", amount);
    printf("Amount withdrawn successfully! Current Balance: %.2f\n", temp->balance);
}

// Display Specific Account Details
void displayAccountDetails() {
    int acc_no;
    printf("Enter account number: ");
    scanf("%d", &acc_no);
    struct Account *temp = accountHead;
    while (temp != NULL && temp->account_number != acc_no)
        temp = temp->next;
    if (temp == NULL) {
        printf("Account not found.\n");
        return;
    }
    printf("\n--- Account Details ---\n");
    printf("Account Number : %d\n", temp->account_number);
    printf("Name           : %s\n", temp->name);
    printf("Balance        : %.2f\n", temp->balance);
}

// Display All Transactions
void displayTransactionHistory() {
    struct Transaction *temp = transactionHead;
    if (temp == NULL) {
        printf("\nNo transactions recorded yet.\n");
        return;
    }
    printf("\n--- Transaction History ---\n");
    printf("%-10s %-12s %-10s %-10s\n", "Acc.No", "Type", "Amount", "Date");
    printf("-----------------------------------------------\n");
    while (temp != NULL) {
        printf("%-10d %-12s %-10.2f %-10s\n",
               temp->account_number, temp->type, temp->amount, temp->date);
        temp = temp->next;
    }
    printf("-----------------------------------------------\n");
}

// Delete Account
void deleteAccount() {
    int acc_no;
    printf("Enter account number to delete: ");
    scanf("%d", &acc_no);
    struct Account *temp = accountHead, *prev = NULL;
    while (temp != NULL && temp->account_number != acc_no) {
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL) {
        printf("Account not found.\n");
        return;
    }
    if (prev == NULL)
        accountHead = temp->next;
    else
        prev->next = temp->next;
    free(temp);
    printf("Account deleted successfully.\n");
}

// Mini Statement (Last 5 Transactions)
void displayMiniStatement(int account_number) {
    struct Transaction *temp = transactionHead;
    struct Transaction *list[100];
    int count = 0;
    while (temp != NULL) {
        if (temp->account_number == account_number)
            list[count++] = temp;
        temp = temp->next;
    }
    if (count == 0) {
        printf("No transactions found for Account No: %d\n", account_number);
        return;
    }
    printf("\n--- Mini Statement for Account No: %d ---\n", account_number);
    printf("%-12s %-12s %-10s\n", "Type", "Amount", "Date");
    printf("----------------------------------------\n");
    int start = (count >= 5) ? count - 5 : 0;
    for (int i = start; i < count; i++) {
        printf("%-12s %-12.2f %-10s\n", list[i]->type, list[i]->amount, list[i]->date);
    }
    printf("----------------------------------------\n");
}

// Main Function
int main() {
    int choice, acc_no;
    while (1) {
        printf("\n===== SIMPLE BANKING SYSTEM =====\n");
        printf("1. Create New Account\n");
        printf("2. Display All Accounts\n");
        printf("3. Deposit Amount\n");
        printf("4. Withdraw Amount\n");
        printf("5. Display Account Details\n");
        printf("6. View Transaction History\n");
        printf("7. Delete Account\n");
        printf("8. Mini Statement (Last 5 Transactions)\n");
        printf("9. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
        case 1:
            createAccount();
            break;
        case 2:
            displayAllAccounts();
            break;
        case 3:
            depositAmount();
            break;
        case 4:
            withdrawAmount();
            break;
        case 5:
            displayAccountDetails();
            break;
        case 6:
            displayTransactionHistory();
            break;
        case 7:
            deleteAccount();
            break;
        case 8:
            printf("Enter account number: ");
            scanf("%d", &acc_no);
            displayMiniStatement(acc_no);
            break;
        case 9:
            printf("Exiting program... Thank you!\n");
            exit(0);
        default:
            printf("Invalid choice! Please try again.\n");
        }
    }
    return 0;
}
