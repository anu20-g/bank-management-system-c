/*
 * Bank Management System
 * Team 1
 * Concepts: Structures, Functions, File Handling
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Account {
    int accNo;
    char name[50];
    double balance;
};

void createAccount() {
    FILE *fp = fopen("accounts.dat", "ab");
    if (!fp) { printf("File error!\n"); return; }
    struct Account a;
    printf("\nEnter Account Number: ");
    scanf("%d", &a.accNo);
    getchar();
    printf("Enter Account Holder Name: ");
    fgets(a.name, sizeof(a.name), stdin);
    a.name[strcspn(a.name, "\n")] = 0;
    printf("Enter Initial Deposit: $");
    scanf("%lf", &a.balance);
    fwrite(&a, sizeof(struct Account), 1, fp);
    fclose(fp);
    printf(">> Account created successfully!\n");
}

void showAllAccounts() {
    FILE *fp = fopen("accounts.dat", "rb");
    if (!fp) { printf("No accounts on record.\n"); return; }
    struct Account a;
    printf("\n================ BANK ACCOUNTS LIST ================\n");
    printf("%-12s %-25s %-12s\n", "Account No", "Holder Name", "Balance");
    printf("----------------------------------------------------\n");
    while (fread(&a, sizeof(struct Account), 1, fp)) {
        printf("%-12d %-25s $%-12.2f\n", a.accNo, a.name, a.balance);
    }
    printf("====================================================\n");
    fclose(fp);
}

void depositWithdraw(int isDeposit) {
    FILE *fp = fopen("accounts.dat", "rb+");
    if (!fp) { printf("Database error.\n"); return; }
    int targetAcc, found = 0;
    double amt;
    struct Account a;
    printf("Enter Account Number: ");
    scanf("%d", &targetAcc);
    while (fread(&a, sizeof(struct Account), 1, fp)) {
        if (a.accNo == targetAcc) {
            found = 1;
            printf("Current Balance: $%.2f\n", a.balance);
            if (isDeposit) {
                printf("Enter amount to deposit: $");
                scanf("%lf", &amt);
                a.balance += amt;
            } else {
                printf("Enter amount to withdraw: $");
                scanf("%lf", &amt);
                if (amt > a.balance) {
                    printf("Error: Insufficient balance!\n");
                    fclose(fp);
                    return;
                }
                a.balance -= amt;
            }
            fseek(fp, -((long)sizeof(struct Account)), SEEK_CUR);
            fwrite(&a, sizeof(struct Account), 1, fp);
            printf(">> Transaction complete. New Balance: $%.2f\n", a.balance);
            break;
        }
    }
    if (!found) printf("Account not found.\n");
    fclose(fp);
}

int main() {
    int ch;
    while (1) {
        printf("\n====== BANK CORE SYSTEM ======\n");
        printf("1. Open New Account\n2. Deposit Funds\n3. Withdraw Funds\n4. View All Accounts\n5. Exit\n");
        printf("Choice: ");
        if (scanf("%d", &ch) != 1 || ch == 5) break;
        switch (ch) {
            case 1: createAccount(); break;
            case 2: depositWithdraw(1); break;
            case 3: depositWithdraw(0); break;
            case 4: showAllAccounts(); break;
            default: printf("Invalid option.\n");
        }
    }
    return 0;
}
