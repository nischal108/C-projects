#include <stdio.h>
#include <string.h>

#define MAX_BANK_ACCOUNTS 100
#define NAME_LENGTH 50
#define PASSWORD_LENGTH 10

typedef struct {
    char account_Holder_Name[NAME_LENGTH];
    char account_Password[PASSWORD_LENGTH];
    int accountNumber;
    double balance;
} bankAccount;

typedef struct {
    bankAccount accounts[MAX_BANK_ACCOUNTS];
    int num_Accounts;
} bank;

bank NepalRastraBank; // Global instance of the bank named NepalRastraBank

// Function prototypes
int searchForAccount(int customer_account_number);
void customerPortal(bankAccount customerAccount, int i);
void CustomerTasks(int acNumber);
void withdraw(bankAccount customerAccount);
void transfer(bank *bankInstance, const bankAccount senderAccount);
int createAccount(bank *bankInstance);
int deleteAccount(bank *bankInstance, int accountNumber);
void displayAllAccounts(const bank *bankInstance);
void adminTasks();
void deposit();

// Function to search for account and return its index if found
int searchForAccount(int customer_account_number) {
    for (int i = 0; i < NepalRastraBank.num_Accounts; i++) {
        if (NepalRastraBank.accounts[i].accountNumber == customer_account_number) {
            return i; // Found the account, return its index
        }
    }
    return -1; // Account not found
}

// function to enter customer portal 
void customerPortal(bankAccount customerAccount, int i){
    int choice;
    do {
        printf("\nCustomer Options:\n");
        printf("1. View Balance\n");
        printf("2. Withdraw\n");
        printf("3. Transfer Balance\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Your account balance: %.2f\n", NepalRastraBank.accounts[i].balance);
                break;
            case 2:
                withdraw(customerAccount);
                break;
            case 3:
                transfer(&NepalRastraBank, customerAccount);
                break;
            case 4:
                printf("Exiting customer tasks.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 4);
}

// Function To handle Customer tasks 
void CustomerTasks(int acNumber) {
    char entered_password[PASSWORD_LENGTH];
    int index_of_account = searchForAccount(acNumber);

    if(index_of_account != -1) {
        printf("Enter the password of your account: ");
        scanf("%s", entered_password);
        
        if (strcmp(entered_password, NepalRastraBank.accounts[index_of_account].account_Password) == 0) {
            printf("Login Successful!\n");
            customerPortal(NepalRastraBank.accounts[index_of_account], index_of_account);
        } else {
            printf("Access denied.\n");
        }
    } else {
        printf("No account with the entered account number found.\n");
    }
}

void withdraw(bankAccount customerAccount){
    double withdrawAmount;
    printf("Enter the amount to withdraw: ");
    scanf("%lf", &withdrawAmount);
    customerAccount.balance -= withdrawAmount;
    printf("You have successfully withdrawn %.2f & your new balance is %.2f\n", withdrawAmount, customerAccount.balance);
}

void transfer(bank *bankInstance, bankAccount senderAccount){
    int receiverACnumber, indexOfreceiver;
    double transferAmount; 

    printf("\nEnter the account number of the receiver: ");
    scanf("%d", &receiverACnumber);
    
    indexOfreceiver = searchForAccount(receiverACnumber);
    
    if(indexOfreceiver != -1) { 
        printf("\nEnter the amount to transfer: ");
        scanf("%lf", &transferAmount); 

        // Check if sender's balance is sufficient for transfer
        if(senderAccount.balance >= transferAmount) {
            senderAccount.balance -= transferAmount;
            bankInstance->accounts[indexOfreceiver].balance += transferAmount;
            printf("Transfer Successful!\n");
        } else {
            printf("Insufficient balance for transfer.\n");
        }
    } else {
        printf("Receiver account not found.\n");
    }
}

// Function to create a new bank account
int createAccount(bank *bankInstance) {
    if (bankInstance->num_Accounts >= MAX_BANK_ACCOUNTS) {
        printf("Cannot create more accounts. Maximum limit reached.\n");
        return 0;
    }

    bankAccount newAccount;
    printf("Enter account holder name: ");
    scanf("%s", newAccount.account_Holder_Name);

    printf("Enter account number: ");
    scanf("%d", &newAccount.accountNumber);

    // Check if account number already exists
    for (int i = 0; i < bankInstance->num_Accounts; i++) {
        if (bankInstance->accounts[i].accountNumber == newAccount.accountNumber) {
            printf("Account number already exists. Please choose a different one.\n");
            return 0;
        }
    }

    printf("Enter opening balance: ");
    scanf("%lf", &newAccount.balance);

    printf("Set account password: ");
    scanf("%s", newAccount.account_Password);
    bankInstance->accounts[bankInstance->num_Accounts++] = newAccount;
    return 1;
}

//function to delete account 
int deleteAccount(bank *bankInstance, int accountNumber) {
    char entered_password[PASSWORD_LENGTH];
    int index = searchForAccount(accountNumber);
    if (index == -1) {
        printf("Account not found.\n");
        return 0;
    }
    printf("Enter the password to delete account: ");
    scanf("%s", entered_password);
    if (strcmp(entered_password, bankInstance->accounts[index].account_Password) != 0) {
        printf("Password incorrect. Account deletion failed.\n");
        return 0; 
    }
    for (int i = index; i < bankInstance->num_Accounts - 1; i++) {
        bankInstance->accounts[i] = bankInstance->accounts[i + 1];
    }
    bankInstance->num_Accounts--;
    printf("Account deletion successful.\n");
    return 1; 
}

// function to display all accounts
void displayAllAccounts(const bank *bankInstance) {
    printf("\nAll Bank Accounts:\n");
    printf("--------------------------------------------------------------\n");
    printf("| %s | %s | %s | %s |\n", "Account #", "Holder Name", "Balance", "Password");
    printf("--------------------------------------------------------------\n");

    for (int i = 0; i < bankInstance->num_Accounts; i++) {
        printf("| %d | %s | %.2f | %s |\n", 
            bankInstance->accounts[i].accountNumber, 
            bankInstance->accounts[i].account_Holder_Name, 
            bankInstance->accounts[i].balance, 
            bankInstance->accounts[i].account_Password);
    }

    printf("--------------------------------------------------------------\n");
}

// Function to handle admin tasks
void adminTasks() {
    int choice_for_tasks;
    int search_account_number, delete_account_number;

    printf("Welcome Admin\n");
    do {
        printf("\nWhat would you like to do:\n");
        printf("1. Create an account\n");
        printf("2. Search for an account\n");
        printf("3. Delete an account\n");
        printf("4. See all accounts\n");
        printf("5. Exit\n");
        scanf("%d", &choice_for_tasks);

        switch (choice_for_tasks) {
            case 1:
                if (createAccount(&NepalRastraBank) == 1) {
                    printf("Account Creation Successful\n");
                } else {
                    printf("Failed to Create Account\n");
                }
                break;
            case 2:
                printf("Enter the account number: ");
                scanf("%d", &search_account_number);
                if (searchForAccount(search_account_number) != -1) {
                    printf("Yes, the account exists\n");
                } else {
                    printf("There is no account with such details\n");
                }
                break;
            case 3:
                printf("Enter the Account Number of the account you want to delete: ");
                scanf("%d", &delete_account_number);
                if (deleteAccount(&NepalRastraBank, delete_account_number) == 1) {
                    printf("Account deletion was successful\n");
                } else {
                    printf("Account Deletion Failed\n");
                }
                break;
            case 4:
                displayAllAccounts(&NepalRastraBank);
                break;
            case 5:
                printf("Exiting admin panel.\n");
                break;
            default:
                printf("Invalid input\n");
                break;
        }
    } while (choice_for_tasks != 5);
}

//function to deposit amount
void deposit() {
    int accountNumber;
    double depositAmount;

    printf("Enter your account number: ");
    scanf("%d", &accountNumber);

    int index = searchForAccount(accountNumber);
    if (index == -1) {
        printf("Account not found.\n");
        return;
    }

    printf("Enter the amount to deposit: ");
    scanf("%lf", &depositAmount);

    NepalRastraBank.accounts[index].balance += depositAmount;
    printf("Deposit of %.2f successfully made.\n", depositAmount);
}

// main function 
int main() {
    int choice_to_login;
    char admin_password[] = "nischal";
    int customer_account_number;

    do {
        printf("Welcome to Nepal Rastra Bank\n");
        printf("\n1. Login as admin\n2. Login as Customer\n3. Deposit\n4. Exit\n");
        scanf("%d", &choice_to_login);

        switch (choice_to_login) {
            case 1:
                {
                    char entered_password[PASSWORD_LENGTH];
                    printf("Enter the admin password: ");
                    scanf("%s", entered_password);
                    if (strcmp(entered_password, admin_password) == 0) {
                    adminTasks();
                    } else {
                        printf("Access denied!\n");
                    }
                }
                break;
            case 2:
                printf("Welcome to the Customer Portal\n");
                printf("Enter your account number: ");
                scanf("%d", &customer_account_number);
                CustomerTasks(customer_account_number);
                break;
            case 3:
                deposit();
                break;
            case 4:
                printf("Exiting program.\n");
                return 0;
            default:
                printf("Invalid input.\n");
                break;
        }
    } while (1);

    return 0;
}
