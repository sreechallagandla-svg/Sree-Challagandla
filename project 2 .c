
#include <stdio.h>
#include <limits.h>
#include <string.h>

#define ACCOUNT_COUNT 2
#define MAX_HISTORY 5

struct Transaction {
    char description[50];
    int amount;
};

struct Account {
    int number;
    int pin;
    char name[30];
    char type[20];
    char phone[12];
    char email[30];
    int balance;
    struct Transaction history[MAX_HISTORY];
    int historyCount;
};

void clearInputBuffer(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
    }
}

int readNumber(const char *prompt, int *number) {
    printf("%s", prompt);
    if (scanf("%d", number) != 1) {
        clearInputBuffer();
        printf("Please enter a valid whole number.\n");
        return 0;
    }
    clearInputBuffer();
    return 1;
}

int findAccount(const struct Account accounts[], int accountNumber) {
    for (int i = 0; i < ACCOUNT_COUNT; i++) {
        if (accounts[i].number == accountNumber) {
            return i;
        }
    }
    return -1;
}

void addTransaction(struct Account *account, const char *description, int amount) {
    if (account->historyCount >= MAX_HISTORY) {
        for (int i = 1; i < MAX_HISTORY; i++) {
            account->history[i - 1] = account->history[i];
        }
        account->historyCount = MAX_HISTORY - 1;
    }

    snprintf(account->history[account->historyCount].description,
             sizeof(account->history[account->historyCount].description),
             "%s", description);
    account->history[account->historyCount].amount = amount;
    account->historyCount++;
}

void showAccounts(const struct Account accounts[]) {
    printf("\nAvailable accounts:\n");
    for (int i = 0; i < ACCOUNT_COUNT; i++) {
        printf("%d - %s (%s)\n", accounts[i].number, accounts[i].name, accounts[i].type);
    }
}

void showRecentTransactions(const struct Account *account) {
    printf("\nRecent activity for %s:\n", account->name);
    if (account->historyCount == 0) {
        printf("No recent transactions.\n");
        return;
    }

    for (int i = 0; i < account->historyCount; i++) {
        printf("- %s: Rs.%d\n", account->history[i].description, account->history[i].amount);
    }
}

void showAccountDetails(const struct Account *account) {
    printf("\nAccount details\n");
    printf("Name: %s\n", account->name);
    printf("Account no.: %d\n", account->number);
    printf("Account type: %s\n", account->type);
    printf("Phone: %s\n", account->phone);
    printf("Email: %s\n", account->email);
    printf("Balance: Rs.%d\n", account->balance);
}

int authenticateAccount(struct Account accounts[], int *activeAccount) {
    int accountNumber;
    int pin;

    if (!readNumber("Enter account number: ", &accountNumber)) {
        return 0;
    }

    *activeAccount = findAccount(accounts, accountNumber);
    if (*activeAccount == -1) {
        printf("Account not found. Please use a valid demo account.\n");
        return 0;
    }

    printf("Enter 4-digit PIN: ");
    if (scanf("%d", &pin) != 1) {
        clearInputBuffer();
        printf("PIN must be a valid 4-digit number.\n");
        return 0;
    }
    clearInputBuffer();

    if (accounts[*activeAccount].pin != pin) {
        printf("Incorrect PIN. Access denied.\n");
        *activeAccount = -1;
        return 0;
    }

    printf("Authentication successful.\n");
    return 1;
}

int main(void) {
    struct Account accounts[ACCOUNT_COUNT] = {
        {1001, 1234, "Aarav Sharma", "Savings", "9876543210", "aarav@bankdemo.com", 10000, {{"", 0}}, 0},
        {1002, 5678, "Maya Nair", "Current", "9123456780", "maya@bankdemo.com", 7500, {{"", 0}}, 0}
    };

    int activeAccount = -1;
    int accountNumber;
    int choice;
    int amount;
    int recipient;

    printf("====================================\n");
    printf("         INDUS BANK ONLINE\n");
    printf("====================================\n");
    printf("Secure digital banking demo\n");
    printf("This demo does not save account data permanently.\n\n");

    showAccounts(accounts);

    while (activeAccount == -1) {
        if (!authenticateAccount(accounts, &activeAccount)) {
            printf("Please try again.\n\n");
        }
    }

    printf("\nWelcome, %s!\n", accounts[activeAccount].name);
    addTransaction(&accounts[activeAccount], "Login", 0);

    for (;;) {
        printf("\n--- BANKING MENU ---\n");
        printf("1. Check balance\n");
        printf("2. Deposit money\n");
        printf("3. Withdraw money\n");
        printf("4. Transfer money\n");
        printf("5. View account details\n");
        printf("6. View recent transactions\n");
        printf("7. Switch account\n");
        printf("8. Exit\n");

        if (!readNumber("Choose an option: ", &choice)) {
            continue;
        }

        if (choice == 1) {
            printf("\nAccount holder: %s\n", accounts[activeAccount].name);
            printf("Account number: %d\n", accounts[activeAccount].number);
            printf("Available balance: Rs.%d\n", accounts[activeAccount].balance);
        } else if (choice == 2 || choice == 3) {
            if (!readNumber("Enter amount in rupees: ", &amount)) {
                continue;
            }

            if (amount <= 0) {
                printf("Amount must be greater than zero.\n");
            } else if (choice == 2) {
                if (amount > INT_MAX - accounts[activeAccount].balance) {
                    printf("Deposit amount is too large for this account.\n");
                } else {
                    accounts[activeAccount].balance += amount;
                    addTransaction(&accounts[activeAccount], "Deposit", amount);
                    printf("Deposit successful.\n");
                    printf("New balance: Rs.%d\n", accounts[activeAccount].balance);
                }
            } else {
                if (amount > accounts[activeAccount].balance) {
                    printf("Insufficient balance. Withdrawal cancelled.\n");
                } else {
                    accounts[activeAccount].balance -= amount;
                    addTransaction(&accounts[activeAccount], "Withdrawal", -amount);
                    printf("Withdrawal successful.\n");
                    printf("Remaining balance: Rs.%d\n", accounts[activeAccount].balance);
                }
            }
        } else if (choice == 4) {
            if (!readNumber("Enter recipient account number: ", &accountNumber)) {
                continue;
            }

            recipient = findAccount(accounts, accountNumber);
            if (recipient == -1) {
                printf("Recipient account not found.\n");
                continue;
            }
            if (recipient == activeAccount) {
                printf("You cannot transfer money to your own account.\n");
                continue;
            }

            if (!readNumber("Enter amount in rupees: ", &amount)) {
                continue;
            }

            if (amount <= 0) {
                printf("Transfer amount must be greater than zero.\n");
            } else if (amount > accounts[activeAccount].balance) {
                printf("Insufficient balance. Transfer cancelled.\n");
            } else if (amount > INT_MAX - accounts[recipient].balance) {
                printf("Recipient account cannot accept this transfer amount.\n");
            } else {
                accounts[activeAccount].balance -= amount;
                accounts[recipient].balance += amount;
                addTransaction(&accounts[activeAccount], "Transfer", -amount);
                char note[60];
                snprintf(note, sizeof(note), "Received from %d", accounts[activeAccount].number);
                addTransaction(&accounts[recipient], note, amount);

                printf("Transfer successful.\n");
                printf("Rs.%d sent to %s.\n", amount, accounts[recipient].name);
                printf("Your balance: Rs.%d\n", accounts[activeAccount].balance);
            }
        } else if (choice == 5) {
            showAccountDetails(&accounts[activeAccount]);
        } else if (choice == 6) {
            showRecentTransactions(&accounts[activeAccount]);
        } else if (choice == 7) {
            printf("Logging out from %s...\n", accounts[activeAccount].name);
            activeAccount = -1;
            while (activeAccount == -1) {
                if (!authenticateAccount(accounts, &activeAccount)) {
                    printf("Please try again.\n");
                }
            }
            printf("Switched to %s's account.\n", accounts[activeAccount].name);
        } else if (choice == 8) {
            printf("Thank you for banking with INDUS BANK. Have a great day!\n");
            break;
        } else {
            printf("Invalid option. Please choose from 1 to 8.\n");
        }
    }

    return 0;
}