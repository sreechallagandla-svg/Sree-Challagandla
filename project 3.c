#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>

#define MAX_ACCOUNTS 100
#define MAX_TRANSACTIONS 50
#define CASH_ACCOUNT_MIN_DEPOSIT 1500.0
#define MARGIN_ACCOUNT_MIN_DEPOSIT 5000.0
#define RETIREMENT_ACCOUNT_MIN_DEPOSIT 10000.0
#define TRADE_COMMISSION_RATE 0.0015
#define MARGIN_LEVERAGE 2.0
#define RETIREMENT_LEVERAGE 0.5

enum TransactionType {
    TRANSACTION_DEPOSIT = 1,
    TRANSACTION_WITHDRAWAL = 2,
    TRANSACTION_TRADE = 3,
    TRANSACTION_FEE = 4,
    TRANSACTION_DIVIDEND = 5
};

enum AccountType {
    ACCOUNT_TYPE_CASH = 1,
    ACCOUNT_TYPE_MARGIN = 2,
    ACCOUNT_TYPE_RETIREMENT = 3
};

typedef struct {
    int type;
    double amount;
    char timestamp[32];
    char description[128];
} Transaction;

typedef struct {
    int id;
    char name[32];
    char email[48];
    unsigned long passwordHash;
    int accountType;
    double cashBalance;
    double buyingPower;
    double marginUsed;
    bool isLocked;
    char createdAt[24];
    char lastLogin[24];
    int transactionCount;
    Transaction transactions[MAX_TRANSACTIONS];
} Account;

static Account accounts[MAX_ACCOUNTS];
static int accountCount = 0;
static int nextAccountId = 1001;

static unsigned long hashPassword(const char *password) {
    unsigned long hash = 2166136261u;
    while (*password != '\0') {
        hash ^= (unsigned char)*password;
        hash *= 16777619u;
        password++;
    }
    return hash;
}

static void trimNewline(char *text) {
    size_t len = strlen(text);
    while (len > 0 && (text[len - 1] == '\n' || text[len - 1] == '\r')) {
        text[len - 1] = '\0';
        len--;
    }
}

static void clearInputBuffer(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
    }
}

static void getCurrentTimestamp(char *buffer, size_t bufferSize) {
    time_t now = time(NULL);
    struct tm *timestamp = localtime(&now);

    if (timestamp == NULL || bufferSize == 0) {
        if (bufferSize > 0) {
            buffer[0] = '\0';
        }
        return;
    }

    strftime(buffer, bufferSize, "%Y-%m-%d %H:%M:%S", timestamp);
}

static void logAudit(const char *message) {
    FILE *logFile = fopen("audit.log", "a");
    if (!logFile) {
        return;
    }

    char timeBuffer[32];
    getCurrentTimestamp(timeBuffer, sizeof(timeBuffer));
    fprintf(logFile, "%s | %s\n", timeBuffer, message);
    fclose(logFile);
}

static Account *findAccountById(int id) {
    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].id == id) {
            return &accounts[i];
        }
    }
    return NULL;
}

static const char *transactionTypeToString(int type) {
    switch (type) {
        case TRANSACTION_DEPOSIT: return "Deposit";
        case TRANSACTION_WITHDRAWAL: return "Withdrawal";
        case TRANSACTION_TRADE: return "Trade";
        case TRANSACTION_FEE: return "Fee";
        case TRANSACTION_DIVIDEND: return "Dividend";
        default: return "Adjustment";
    }
}

static const char *accountTypeToString(int type) {
    switch (type) {
        case ACCOUNT_TYPE_CASH: return "Cash";
        case ACCOUNT_TYPE_MARGIN: return "Margin";
        case ACCOUNT_TYPE_RETIREMENT: return "Retirement";
        default: return "Unknown";
    }
}

static bool isStrongPassword(const char *password) {
    size_t length = strlen(password);
    bool hasUpper = false;
    bool hasLower = false;
    bool hasDigit = false;
    bool hasSymbol = false;

    if (length < 8) {
        return false;
    }

    for (size_t i = 0; i < length; i++) {
        unsigned char ch = (unsigned char)password[i];
        if (isupper(ch)) {
            hasUpper = true;
        } else if (islower(ch)) {
            hasLower = true;
        } else if (isdigit(ch)) {
            hasDigit = true;
        } else if (!isspace(ch)) {
            hasSymbol = true;
        }
    }

    return hasUpper && hasLower && hasDigit && hasSymbol;
}

static void recordTransaction(Account *account, int type, const char *description, double amount) {
    if (!account || account->transactionCount >= MAX_TRANSACTIONS) {
        return;
    }

    Transaction *transaction = &account->transactions[account->transactionCount];
    transaction->type = type;
    transaction->amount = amount;
    transaction->description[0] = '\0';
    snprintf(transaction->description, sizeof(transaction->description), "%s", description);

    getCurrentTimestamp(transaction->timestamp, sizeof(transaction->timestamp));
    account->transactionCount++;
}

static double calculateBuyingPower(const Account *account) {
    switch (account->accountType) {
        case ACCOUNT_TYPE_CASH:
            return account->cashBalance;
        case ACCOUNT_TYPE_MARGIN:
            return account->cashBalance * MARGIN_LEVERAGE;
        case ACCOUNT_TYPE_RETIREMENT:
            return account->cashBalance * RETIREMENT_LEVERAGE;
        default:
            return account->cashBalance;
    }
}

static void refreshAccountMetrics(Account *account) {
    account->buyingPower = calculateBuyingPower(account);
    if (account->marginUsed < 0.0) {
        account->marginUsed = 0.0;
    }
    if (account->accountType == ACCOUNT_TYPE_MARGIN && account->marginUsed > account->cashBalance) {
        account->isLocked = true;
    }
}

static void printAccountSummary(const Account *account) {
    double availableFunds = account->cashBalance;
    if (account->accountType == ACCOUNT_TYPE_MARGIN) {
        availableFunds = account->buyingPower - account->marginUsed;
    }

    printf("\nXM Trading Account Summary\n");
    printf("ID: %d\n", account->id);
    printf("Client: %s\n", account->name);
    printf("Email: %s\n", account->email);
    printf("Account Type: %s\n", accountTypeToString(account->accountType));
    printf("Status: %s\n", account->isLocked ? "Locked" : "Active");
    printf("Cash Balance: $%.2f\n", account->cashBalance);
    printf("Buying Power: $%.2f\n", account->buyingPower);
    printf("Margin Used: $%.2f\n", account->marginUsed);
    printf("Available Funds: $%.2f\n", availableFunds);
    printf("Last Login: %s\n", account->lastLogin[0] ? account->lastLogin : "Never");
    printf("Transactions: %d\n", account->transactionCount);
}

static void createAccount(void) {
    if (accountCount >= MAX_ACCOUNTS) {
        puts("Maximum account limit reached. No further accounts can be opened.");
        return;
    }

    Account newAccount;
    char password[64];
    char email[48];
    char auditMessage[256];
    int typeChoice;
    double initialDeposit;
    double minimumDeposit;

    printf("Enter account holder name: ");
    if (!fgets(newAccount.name, sizeof(newAccount.name), stdin)) {
        puts("Failed to read account holder name.");
        return;
    }
    trimNewline(newAccount.name);

    if (newAccount.name[0] == '\0') {
        puts("Account holder name cannot be empty.");
        return;
    }

    printf("Enter email address: ");
    if (!fgets(email, sizeof(email), stdin)) {
        puts("Failed to read email address.");
        return;
    }
    trimNewline(email);

    if (strchr(email, '@') == NULL) {
        puts("Please enter a valid email address.");
        return;
    }

    printf("Create password (8+ chars, upper, lower, digit, symbol): ");
    if (!fgets(password, sizeof(password), stdin)) {
        puts("Failed to read password.");
        return;
    }
    trimNewline(password);

    if (!isStrongPassword(password)) {
        puts("Password does not meet the broker security policy.");
        return;
    }

    puts("Select account type:");
    puts("1. Cash Account");
    puts("2. Margin Account");
    puts("3. Retirement Account");
    printf("Choice: ");
    if (scanf("%d", &typeChoice) != 1) {
        clearInputBuffer();
        puts("Invalid account type selection.");
        return;
    }
    clearInputBuffer();

    if (typeChoice != ACCOUNT_TYPE_CASH && typeChoice != ACCOUNT_TYPE_MARGIN && typeChoice != ACCOUNT_TYPE_RETIREMENT) {
        puts("Account type is not valid.");
        return;
    }

    switch (typeChoice) {
        case ACCOUNT_TYPE_CASH:
            minimumDeposit = CASH_ACCOUNT_MIN_DEPOSIT;
            break;
        case ACCOUNT_TYPE_MARGIN:
            minimumDeposit = MARGIN_ACCOUNT_MIN_DEPOSIT;
            break;
        case ACCOUNT_TYPE_RETIREMENT:
            minimumDeposit = RETIREMENT_ACCOUNT_MIN_DEPOSIT;
            break;
        default:
            minimumDeposit = 0.0;
            break;
    }

    printf("Enter initial deposit: $");
    if (scanf("%lf", &initialDeposit) != 1) {
        clearInputBuffer();
        puts("Invalid initial deposit amount.");
        return;
    }
    clearInputBuffer();

    if (initialDeposit < minimumDeposit) {
        printf("Initial deposit must be at least $%.2f for a %s account.\n", minimumDeposit, accountTypeToString(typeChoice));
        return;
    }

    newAccount.id = nextAccountId++;
    newAccount.accountType = typeChoice;
    newAccount.passwordHash = hashPassword(password);
    newAccount.cashBalance = initialDeposit;
    newAccount.marginUsed = 0.0;
    newAccount.isLocked = false;
    newAccount.transactionCount = 0;

    snprintf(newAccount.email, sizeof(newAccount.email), "%s", email);
    getCurrentTimestamp(newAccount.createdAt, sizeof(newAccount.createdAt));
    newAccount.lastLogin[0] = '\0';
    refreshAccountMetrics(&newAccount);

    accounts[accountCount++] = newAccount;
    recordTransaction(&accounts[accountCount - 1], TRANSACTION_DEPOSIT, "Opening deposit", initialDeposit);

    snprintf(auditMessage, sizeof(auditMessage), "Account opened: ID %d | Name %s | Type %s | Deposit $%.2f", newAccount.id, newAccount.name, accountTypeToString(newAccount.accountType), initialDeposit);
    logAudit(auditMessage);

    printf("Account created successfully. Your trading account ID is %d.\n", newAccount.id);
    printf("Initial deposit recorded: $%.2f. Estimated buying power: $%.2f.\n", initialDeposit, newAccount.buyingPower);
}

static void viewAccounts(void) {
    if (accountCount == 0) {
        puts("No trading accounts are available in the system.");
        return;
    }

    puts("\nXM Trading Accounts");
    puts("ID    Client                Type       Cash Balance    Status");
    for (int i = 0; i < accountCount; i++) {
        printf("%-5d %-21s %-10s $%-14.2f %-8s\n",
               accounts[i].id,
               accounts[i].name,
               accountTypeToString(accounts[i].accountType),
               accounts[i].cashBalance,
               accounts[i].isLocked ? "Locked" : "Active");
    }
}

static void showTransactionHistory(const Account *account) {
    if (account->transactionCount == 0) {
        puts("No transactions recorded yet for this account.");
        return;
    }

    puts("\nTransaction History");
    puts("Date/Time                Type        Amount       Notes");
    for (int i = 0; i < account->transactionCount; i++) {
        const Transaction *transaction = &account->transactions[i];
        printf("%-22s %-11s $%-11.2f %s\n",
               transaction->timestamp,
               transactionTypeToString(transaction->type),
               transaction->amount,
               transaction->description);
    }
}

static void depositFunds(Account *account) {
    double amount;
    char auditMessage[192];

    printf("Enter amount to deposit: ");
    if (scanf("%lf", &amount) != 1) {
        clearInputBuffer();
        puts("Invalid amount entered.");
        return;
    }
    clearInputBuffer();

    if (amount <= 0.0) {
        puts("Deposit amount must be greater than zero.");
        return;
    }

    account->cashBalance += amount;
    refreshAccountMetrics(account);

    recordTransaction(account, TRANSACTION_DEPOSIT, "Bank transfer", amount);
    snprintf(auditMessage, sizeof(auditMessage), "Deposit | Account %d | Amount $%.2f", account->id, amount);
    logAudit(auditMessage);

    puts("Funds deposited successfully.");
}

static void withdrawFunds(Account *account) {
    double amount;
    char auditMessage[192];

    printf("Enter amount to withdraw: ");
    if (scanf("%lf", &amount) != 1) {
        clearInputBuffer();
        puts("Invalid amount entered.");
        return;
    }
    clearInputBuffer();

    if (amount <= 0.0) {
        puts("Withdrawal amount must be greater than zero.");
        return;
    }

    if (amount > account->cashBalance) {
        puts("Insufficient available cash for this withdrawal.");
        return;
    }

    account->cashBalance -= amount;
    refreshAccountMetrics(account);

    recordTransaction(account, TRANSACTION_WITHDRAWAL, "Cash withdrawal", amount);
    snprintf(auditMessage, sizeof(auditMessage), "Withdrawal | Account %d | Amount $%.2f", account->id, amount);
    logAudit(auditMessage);

    puts("Withdrawal completed successfully.");
}

static void executeTrade(Account *account) {
    char symbol[16];
    int quantity;
    double price;
    double notionalValue;
    double commission;
    double totalCost;
    char auditMessage[256];

    if (account->isLocked) {
        puts("This account is locked and cannot place new trades.");
        return;
    }

    printf("Enter stock symbol: ");
    if (!fgets(symbol, sizeof(symbol), stdin)) {
        puts("Failed to read stock symbol.");
        return;
    }
    trimNewline(symbol);

    if (symbol[0] == '\0') {
        puts("Stock symbol is required.");
        return;
    }

    for (size_t i = 0; symbol[i] != '\0'; i++) {
        symbol[i] = (char)toupper((unsigned char)symbol[i]);
    }

    printf("Enter quantity: ");
    if (scanf("%d", &quantity) != 1) {
        clearInputBuffer();
        puts("Invalid quantity.");
        return;
    }
    clearInputBuffer();

    if (quantity <= 0) {
        puts("Quantity must be greater than zero.");
        return;
    }

    printf("Enter execution price: ");
    if (scanf("%lf", &price) != 1) {
        clearInputBuffer();
        puts("Invalid execution price.");
        return;
    }
    clearInputBuffer();

    if (price <= 0.0) {
        puts("Execution price must be greater than zero.");
        return;
    }

    notionalValue = (double)quantity * price;
    commission = notionalValue * TRADE_COMMISSION_RATE;
    totalCost = notionalValue + commission;

    if (account->accountType == ACCOUNT_TYPE_CASH && totalCost > account->cashBalance) {
        printf("Trade rejected: insufficient cash available. Estimated total cost is $%.2f.\n", totalCost);
        return;
    }

    if (account->accountType == ACCOUNT_TYPE_MARGIN && totalCost > account->buyingPower) {
        printf("Trade rejected: order exceeds available buying power. Estimated total cost is $%.2f.\n", totalCost);
        return;
    }

    if (account->accountType == ACCOUNT_TYPE_RETIREMENT && totalCost > account->cashBalance) {
        printf("Trade rejected: retirement accounts cannot exceed available cash. Estimated total cost is $%.2f.\n", totalCost);
        return;
    }

    account->cashBalance -= totalCost;
    account->marginUsed += (account->accountType == ACCOUNT_TYPE_MARGIN) ? notionalValue * 0.5 : 0.0;
    refreshAccountMetrics(account);

    char tradeDescription[128];
    snprintf(tradeDescription, sizeof(tradeDescription), "BUY %s %d @ $%.2f (fee $%.2f)", symbol, quantity, price, commission);
    recordTransaction(account, TRANSACTION_TRADE, tradeDescription, totalCost);

    snprintf(auditMessage, sizeof(auditMessage), "Trade executed | Account %d | %s | Value $%.2f | Commission $%.2f", account->id, symbol, notionalValue, commission);
    logAudit(auditMessage);

    printf("Trade executed successfully: %s\n", tradeDescription);
}

static void accountMenu(Account *account) {
    int choice;

    while (true) {
        puts("\n=== XM Client Portal ===");
        puts("1. View account overview");
        puts("2. Deposit funds");
        puts("3. Withdraw funds");
        puts("4. Execute trade");
        puts("5. View transaction history");
        puts("6. Logout to XM homepage");
        printf("Select an option: ");

        if (scanf("%d", &choice) != 1) {
            clearInputBuffer();
            puts("Invalid input. Please try again.");
            continue;
        }
        clearInputBuffer();

        switch (choice) {
            case 1:
                printAccountSummary(account);
                break;
            case 2:
                depositFunds(account);
                break;
            case 3:
                withdrawFunds(account);
                break;
            case 4:
                executeTrade(account);
                break;
            case 5:
                showTransactionHistory(account);
                break;
            case 6:
                puts("You have been logged out successfully.");
                getCurrentTimestamp(account->lastLogin, sizeof(account->lastLogin));
                return;
            default:
                puts("Invalid option. Please choose a valid menu item.");
                break;
        }
    }
}

static void loginAccount(void) {
    int id;
    char password[64];
    Account *account;

    if (accountCount == 0) {
        puts("No accounts are available. Create an account before attempting login.");
        return;
    }

    printf("Enter account ID: ");
    if (scanf("%d", &id) != 1) {
        clearInputBuffer();
        puts("Invalid account ID.");
        return;
    }
    clearInputBuffer();

    account = findAccountById(id);
    if (account == NULL) {
        puts("Account not found in the system.");
        return;
    }

    if (account->isLocked) {
        puts("This trading account is locked. Contact support for assistance.");
        logAudit("Blocked login attempt on locked account");
        return;
    }

    printf("Enter password: ");
    if (!fgets(password, sizeof(password), stdin)) {
        puts("Password input failed.");
        return;
    }
    trimNewline(password);

    if (hashPassword(password) == account->passwordHash) {
        getCurrentTimestamp(account->lastLogin, sizeof(account->lastLogin));
        printf("Login successful for %s.\n", account->name);
        char auditMessage[128];
        snprintf(auditMessage, sizeof(auditMessage), "Successful login | Account %d | %s", account->id, account->name);
        logAudit(auditMessage);
        accountMenu(account);
    } else {
        puts("Incorrect password.");
        char auditMessage[128];
        snprintf(auditMessage, sizeof(auditMessage), "Failed login attempt | Account %d", account->id);
        logAudit(auditMessage);
    }
}

int main(void) {
    int option;

    puts("===============================================");
    puts("XM Trading App");
    puts("Global Markets | CFDs | Forex | Shares");
    puts("===============================================");

    while (true) {
        puts("\nMain Menu");
        puts("1. Open XM account");
        puts("2. Log in to XM");
        puts("3. View all XM accounts");
        puts("4. Exit XM platform");
        printf("Select an option: ");

        if (scanf("%d", &option) != 1) {
            clearInputBuffer();
            puts("Invalid input. Please enter a number.");
            continue;
        }
        clearInputBuffer();

        switch (option) {
            case 1:
                createAccount();
                break;
            case 2:
                loginAccount();
                break;
            case 3:
                viewAccounts();
                break;
            case 4:
                puts("Closing XM Trading App. Thank you for trading with XM.");
                logAudit("XM platform shutdown");
                return 0;
            default:
                puts("Invalid option. Please choose again.");
                break;
        }
    }

    return 0;
}
