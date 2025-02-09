#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
//structures of accounts
typedef struct
{
    int month;
    int year;
}
Date;

typedef struct
{
    char accountNumber[20];
    char name[50];
    char mobile[20];
    char email[50];
    double balance;
    Date dateOpened;
}
Account;

//prototype of function
void user_menu();
void main_menu();
void login();
Account * load(int * );
void save();
void withdraw();
void deposit();
void transfer();
void report();
void modify();
int isValidName(char * );
int isEmailUnique(char * );
int isMobileNumberUnique(char * );
int isAccountNumberUnique(char * );
int isValidMobileNumber(char * );
int isValidEmail(char * );
int isInteger(char * );
void addAccount();
Date getCurrentDate();
char * months_in_letters(int);
void sortByName(Account *, int);
void SortByDate(Account *, int);
void SortByBalance(Account *, int);
void Print_Sort();
void delete_account();
char* toLowerCase(char* input);
void QUERY();
void ADVANCED_SEARCH();

int main()
{
    int x;

    main_menu(); //call main menu

    scanf("%d", & x); //selection of login or quite
    fflush(stdin);

    switch (x)
    {
    case 1:
        login();
        break;

    case 2:
        exit(0); //quit
        break;
    default:
        printf("\t\t\t\t\t\tInvalid choice. Please try again.\n");
        main();
    }

}
void menu()
{
    int choice;

    user_menu(); //call user menu
    printf("\t\t\t\t\t\tEnter your choice:");
    scanf("%d", & choice);
    fflush(stdin);

    //call functions
    switch (choice)
    {
    case 1:
        addAccount();
        break;
    case 2:
        delete_account();
        break;
    case 3:
        modify();
        break;
    case 4:
        QUERY();
        break;
    case 5:
        ADVANCED_SEARCH();
        break;
    case 6:
        withdraw();
        break;
    case 7:
        deposit();
        break;
    case 8:
        transfer();
        break;
    case 9:
        report();
        break;
    case 10:
        Print_Sort();
        break;
    case 11:
        exit(0); //quit
        break;
    default:
        printf("Invalid choice. Please try again.\n");
        menu();
    }

}
void user_menu()
{
    printf("\n\t\t\t\t\t\t1-ADD\n");
    printf("\t\t\t\t\t\t2-DELETE\n");
    printf("\t\t\t\t\t\t3-MODIFY\n");
    printf("\t\t\t\t\t\t4-SEARCH\n");
    printf("\t\t\t\t\t\t5-ADVANCED SEARCH\n");
    printf("\t\t\t\t\t\t6-WITHDRAW\n");
    printf("\t\t\t\t\t\t7-DEPOSIT\n");
    printf("\t\t\t\t\t\t8-TRANSFER\n");
    printf("\t\t\t\t\t\t9-REPORT\n");
    printf("\t\t\t\t\t\t10-PRINT(SORT)\n");
    printf("\t\t\t\t\t\t11-QUIT\n");
}
void main_menu()
{
    printf("\n\n\t\t\t\t\t\t[1] LOGIN\n");
    printf("\t\t\t\t\t\t[2] QUIT\n");
    printf("\n\t\t\t\t\t\tEnter your choice: ");
}
void login()
{
    char username[50];
    char password[7];
    int isValid = 0;

    //enter username and password
    printf("\n\t\t\t\t\t\tEnter Username: ");
    fflush(stdin);
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0'; // remove the newline character

    printf("\t\t\t\t\t\tEnter Password: ");
    fflush(stdin);
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0'; // remove the newline character

    //read from file
    FILE * acc;
    if ((acc = fopen("users.txt", "r")) == NULL)
    {
        printf("\t\t\t\t\t\tFile does not exist.");
        exit(0);
    }

    char line[100];
    while (fgets(line, sizeof(line), acc) != NULL) //read line from the file
    {

        char storedUsername[50];
        char storedPassword[7];

        // Parse the line using sscanf it returns the number of fields
        if (sscanf(line, "%49s %6s", storedUsername, storedPassword) != 2)
        {
            printf("\t\t\t\t\t\tInvalid file format.\n");
            exit(0);
        }

        // Validate the username and password
        if (strcmp(username, storedUsername) == 0 && strcmp(password, storedPassword) == 0)
            isValid = 1;

    }
    fclose(acc);

    if (isValid)
        menu();
    else
    {
        printf("\t\t\t\t\t\tInvalid Username or Password");
        login();
    }

}
Account * load(int * num_acc)
{
    * num_acc = 0;
    Account * acc;
    int i;
    //read file
    FILE * data;
    if ((data = fopen("accounts.txt", "r")) == NULL)
    {
        printf("File does not exist.");
        exit(0);
    }
    char line1[200]; // Assuming a maximum line length of 200 characters
    // Count the number of lines in the file
    while (fgets(line1, sizeof(line1), data) != NULL)
    {
        ( * num_acc) ++;
    }
    rewind(data); //returns the file position indicator to the beginning of the file
    // Dynamically allocate memory for the accounts based on the number of lines
    acc = (Account * ) malloc( * num_acc * sizeof(Account)); //casting for dynamic allocation
    if (acc == NULL)
    {
        printf("Memory allocation error.");
        exit(0);
    }
    char line2[200]; // Assuming a maximum line length of 200 characters

    for (i = 0; i < * num_acc; i++) //loop to put the file into struct
    {
        if (fgets(line2, sizeof(line2), data) != NULL)
        {
            // Parse the line using sscanf
            if (sscanf(line2, "%10[^,],%49[^,],%49[^,],%lf,%11[^,],%d-%d",
                       acc[i].accountNumber,
                       acc[i].name,
                       acc[i].email, &
                       acc[i].balance,
                       acc[i].mobile, &
                       acc[i].dateOpened.month, &
                       acc[i].dateOpened.year) != 7)
            {
                printf("Error reading line:\n%s", line2);
            }
        }
    }
    fclose(data);
    return acc; // Return the struct of accounts
}
void save(Account * acc2, int num_acc) //account after changes
{
    int i;
    FILE * data; //open file
    if ((data = fopen("accounts.txt", "w")) == NULL)
    {
        printf("can't open the file");
        exit(1);
    }
    //loop to print the change in account
    for (i = 0; i < num_acc; i++)
    {
        //print account into the file
        fprintf(data, "%s,%s,%s,%.2lf,%s,%d-%d",
                acc2[i].accountNumber,
                acc2[i].name,
                acc2[i].email,
                acc2[i].balance,
                acc2[i].mobile,
                acc2[i].dateOpened.month,
                acc2[i].dateOpened.year);
        // print new line if it's not the last account
        if (i < num_acc - 1)
            fprintf(data, "\n");
    }
    fclose(data);
}
void withdraw()
{
    char account_number[11];
    double amount;
    Account * accounts;
    int numAccounts = 0;
    printf("Enter account number: ");
    fflush(stdin);
    fgets(account_number, sizeof(account_number), stdin);
    account_number[strcspn(account_number, "\n")] = '\0';
    accounts = load( & numAccounts);
    int i;
    int found = 0;

    // Search for the account
    for (i = 0; i < numAccounts; i++)
    {
        if (strcmp(account_number, accounts[i].accountNumber) == 0)
        {
            printf("VALID\n");
            found = 1;
            break;
        }
    }
    // If account not found
    if (!found)
    {
        printf("INVALID\n");
        withdraw();
    }
    else
    {
        printf("Enter withdrawn amount: ");
        scanf("%lf", & amount);
        // Check withdrawal conditions
        if (amount > 10000 || (amount > accounts[i].balance) || amount <= 0)
            printf("The transaction is not successful\n");
        else
        {
            int save_process;
            do
            {
                printf("Do you want to save?\n");
                printf("1.Yes\n");
                printf("2.No\n");
                printf("Enter your choice: ");
                scanf("%d", & save_process);
                if (save_process != 1 && save_process != 2)
                    printf("Invalid choice. ");
            }
            while (save_process != 1 && save_process != 2);
            if (save_process == 1)
            {
                accounts[i].balance -= amount;
                save(accounts, numAccounts);
                printf("the transaction is successful\n");
                printf("the remaining balance= %.2lf$\n", accounts[i].balance);
                // Update transactions in the account file
                FILE * p;
                sprintf(account_number, "%s.txt", accounts[i].accountNumber);
                p = fopen(account_number, "a");

                if (p != NULL)
                {
                    fprintf(p, "Transaction is withdrawn by amount %.2lf$\n", amount);
                    fclose(p);
                }
            }
        }
    }
    free(accounts);
    menu();
}
void deposit()
{
    char account_number[11];
    double amount;
    Account * accounts;
    int numAccounts = 0;
    printf("enter account number: ");
    fflush(stdin);
    fgets(account_number, sizeof(account_number), stdin);
    account_number[strcspn(account_number, "\n")] = '\0';
    accounts = load( & numAccounts);
    int i;
    int found = 0;

    // Search for the account
    for (i = 0; i < numAccounts; i++)
    {
        if (strcmp(account_number, accounts[i].accountNumber) == 0)
        {
            printf("VALID\n");
            found = 1;
            break;
        }
    }
    // If account not found
    if (!found)
    {
        printf("INVALID\n");
        deposit();
    }
    else
    {
        printf("Enter amount to deposit: ");
        scanf("%lf", & amount);
        // Check deposit conditions
        if (amount > 10000 || amount <= 0)
            printf("The transaction is not successful\n");
        else
        {
            int save_process;
            do
            {
                printf("Do you want to save?\n");
                printf("1.Yes\n");
                printf("2.No\n");
                printf("enter your choice: ");
                scanf("%d", & save_process);
                if (save_process != 1 && save_process != 2)
                    printf("Invalid choice. ");
            }
            while (save_process != 1 && save_process != 2);
            if (save_process == 1)
            {
                accounts[i].balance += amount;
                save(accounts, numAccounts);
                printf("the transaction is successful\n");
                printf("the new balance= %lf$\n", accounts[i].balance);
                FILE * p;
                // Update transactions in the account file
                sprintf(account_number, "%s.txt", accounts[i].accountNumber);
                p = fopen(account_number, "a");

                if (p != NULL)
                {
                    fprintf(p, "transaction is deposited by amount %.2lf$\n", amount);
                    fclose(p);
                }
            }
        }
    }
    free(accounts);
    menu();
}
void transfer()
{
    char account_number_1[11], account_number_2[12];
    double amount;
    Account * accounts;
    int numAccounts = 0;
    printf("Enter sender account number: ");
    fflush(stdin);
    fgets(account_number_1, sizeof(account_number_1), stdin);
    account_number_1[strcspn(account_number_1, "\n")] = '\0';
    accounts = load( & numAccounts);
    int i, j;
    int found_1 = 0, found_2 = 0;

    // Search for the sender account
    for (i = 0; i < numAccounts; i++)
    {
        if (strcmp(account_number_1, accounts[i].accountNumber) == 0)
        {
            printf("Sender account number is VALID\n");
            found_1 = 1;
            break;
        }
    }
    // If sender account not found
    if (!found_1)
    {
        printf("Sender account number is INVALID\n");
        transfer();
    }
    // Do while if user entered same sender account number
    do
    {
        printf("Enter receiver account number: ");
        fflush(stdin);
        fgets(account_number_2, sizeof(account_number_2), stdin);
        account_number_2[strcspn(account_number_2, "\n")] = '\0';

    }
    while (strcmp(account_number_2, account_number_1) == 0);

    // Search for the receiver account
    for (j = 0; j < numAccounts; j++)
    {
        if (strcmp(account_number_2, accounts[j].accountNumber) == 0)
        {
            printf("Receiver account number is VALID\n");
            found_2 = 1;
            break;
        }
    }
    // If receiver account not found
    if (!found_2)
    {
        printf("Receiver account number is INVALID\n");
        transfer();
    }
    if (found_1 && found_2)
    {
        printf("Enter amount to be transfered: ");
        scanf("%lf", & amount);
        // Check transfer conditions
        if (amount > accounts[i].balance || amount <= 0)
        {
            printf("The transaction is not successful\n");
        }
        else
        {
            int save_process;
            do
            {
                printf("Do you want to save?\n");
                printf("1.Yes\n");
                printf("2.No\n");
                printf("Enter your choice: ");
                scanf("%d", & save_process);
                if (save_process != 1 && save_process != 2)
                    printf("Invalid choice. ");
            }
            while (save_process != 1 && save_process != 2);
            if (save_process == 1)
            {
                accounts[i].balance -= amount;
                accounts[j].balance += amount;
                save(accounts, numAccounts);
                printf("The sender's remaining balance= %.2lf$\n", accounts[i].balance);
                printf("The receiver's new balance= %.2lf$\n", accounts[j].balance);
                // Update transactions for sender's account file
                FILE * p1;
                sprintf(account_number_1, "%s.txt", accounts[i].accountNumber);
                p1 = fopen(account_number_1, "a");

                if (p1 != NULL)
                {
                    fprintf(p1, "Transaction is transfered to %s by amount %.2lf$\n", accounts[j].accountNumber, amount);
                    fclose(p1);
                }
            }
            // Update transactions for receiver's account file
            FILE * p2;
            sprintf(account_number_2, "%s.txt", accounts[j].accountNumber);
            p2 = fopen(account_number_2, "a");

            if (p2 != NULL)
            {
                fprintf(p2, "Transaction is transfered from %s by amount %.2lf$\n", accounts[i].accountNumber, amount);
                fclose(p2);
            }
        }
    }
    free(accounts);
    menu();
}
void report()
{
    char account_number[12];
    char line[100];
    char file_name[16];
    int j = 0, found = 0;
    char line_print[100][100];
    printf("Enter number of account you want: ");
    fflush(stdin);
    fgets(account_number, sizeof(account_number), stdin);
    account_number[strcspn(account_number, "\n")] = '\0'; // remove the newline character

    found = isAccountNumberUnique(account_number);

    if (!found)
    {
        printf("Account not found.\n");
        report();
    }
    else
    {

        sprintf(file_name, "%s.txt", account_number);
        FILE * f = fopen(file_name, "r");
        if (f != NULL)
        {
            while (fgets(line, sizeof(line), f) != NULL)
            {
                sprintf(line_print[j], "%s", line);
                j++;
            }
            int i = j;

            do
            {
                printf("%s", line_print[--j]);

            }
            while (i != j + 5 && j != 0);
        }
        else
            printf("No transaction has been done on this account\n");
    }
    menu();
}
void modify()
{
    Account * account;
    int numAccounts = 0, i;
    int found = 1;
    account = load( & numAccounts);

    char modifiedAccountNumber[11];
    printf("Enter account number to modify: ");
    fflush(stdin);
    fgets(modifiedAccountNumber, sizeof(modifiedAccountNumber), stdin);
    modifiedAccountNumber[strcspn(modifiedAccountNumber, "\n")] = '\0'; // remove the newline character

    found = isAccountNumberUnique(modifiedAccountNumber);

    char name[50];
    char mobile[20];
    char email[50];

    if (!found)
    {
        printf("Account not found.\n");
        modify();
    }
    else
    {
        for (i = 0; i < numAccounts; i++)
        {
            if (strcmp(account[i].accountNumber, modifiedAccountNumber) == 0)
            {
                printf("Enter modified details\n");

                // Name input
                do
                {
                    printf("Enter new name:");
                    fflush(stdin);
                    fgets(name, sizeof(name), stdin);
                    name[strcspn(name, "\n")] = '\0';
                }
                while (!isValidName(name));

                do
                {
                    printf("Enter new mobile number (11 digits starting with '01'): ");
                    fflush(stdin);
                    fgets(mobile, sizeof(mobile), stdin);
                    mobile[strcspn(mobile, "\n")] = '\0';
                }
                while (!isValidMobileNumber(mobile) || !isMobileNumberUnique(mobile));

                // Email input with validation loop
                do
                {
                    printf("Enter new email address (must contain '@' and end with '.com'): ");
                    fflush(stdin);
                    fgets(email, sizeof(email), stdin);
                    fflush(stdin);
                    email[strcspn(email, "\n")] = '\0';

                }
                while (!isValidEmail(email) || !isEmailUnique(email));

                break;
            }
        }

        // Update account details
        strcpy(account[i].name, name);
        strcpy(account[i].mobile, mobile);
        strcpy(account[i].email, email);

        // Save modified details
        save(account, numAccounts);

        printf("Account details modified successfully.\n");
    }
    free(account);
    menu();
}
Date getCurrentDate()
{
    time_t t; //variable of type time_t
    struct tm * current_time; //pointer to a struct tm which is in the time.h header in it (year,month,days,sec,...)

    time( & t); //returns seconds since 1 January 1970
    current_time = localtime( & t); //convert seconds to normal time in day years months

    Date currentDate;
    currentDate.month = current_time -> tm_mon + 1; // we add 1 because it starts from 0 (January=0, February=1)
    currentDate.year = current_time -> tm_year + 1900; //we add 1900 because it counts (2000=100,2004,=104,2023=123

    return currentDate;
}
int isAccountNumberUnique(char * accountNumber)
{
    Account * account;
    int numAccounts = 0, i;
    int found = 0;
    account = load( & numAccounts);

    for (i = 0; i < numAccounts; i++)
    {
        if (strcmp(account[i].accountNumber, accountNumber) == 0)
        {
            found = 1;
            break;
        }
    }
    free(account);
    if (found)
        return 1;
    return 0;

}

int isMobileNumberUnique(char * mobile)
{
    Account * account;
    int numAccounts = 0, i;
    int found = 0;
    account = load( & numAccounts);

    for (i = 0; i < numAccounts; i++)
    {
        if (strcmp(account[i].mobile, mobile) == 0)
        {
            found = 1;
            break;
        }
    }
    free(account);
    if (found)
        return 0;
    return 1;

}

int isEmailUnique(char * email)
{
    Account * account;
    int numAccounts = 0, i;
    int found = 0;
    account = load( & numAccounts);

    for (i = 0; i < numAccounts; i++)
    {
        if (strcmp(account[i].email, email) == 0)
        {
            found = 1;
            break;
        }
    }
    free(account);
    if (found)
        return 0;
    return 1;

}

int isValidMobileNumber(char * mobile)
{
    int i;
    // Check if the mobile number is 11 digits starting with '01'
    if (strlen(mobile) == 11 && strncmp(mobile, "01", 2) == 0)
    {
        // Check if all characters in the remaining part of the mobile number are digits
        for (i = 2; i < 11; i++)
        {
            if (!isdigit(mobile[i]))
            {
                return 0; // Invalid if any non-digit character is found
            }
        }

        return 1; // Valid
    }

    return 0; // Invalid
}

int isValidEmail(char * email)
{
    // Check if the email contains '@' and ends with '.com'
    if (strstr(email, "@") && strcmp(email + strlen(email) - 4, ".com") == 0  && !(strchr(email,' ') ) )
        return 1; // Valid

    return 0; // Invalid
}

int isValidName(char * name)
{
    // Check if the name is empty
    if ( * name == '\0')
    {
        return 0; // Invalid name
    }

    // Loop through each character in the name
    while ( * name != '\0')
    {
        // Check if the character is an alphabetic character or a space
        if (!isalpha( * name) && * name != ' ')
        {
            return 0; // Invalid name
        }

        name++; // Move to the next character
    }

    return 1; // Valid name
}

int isInteger(char * str)
{
    // Check if each character in the string is a digit
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (!isdigit(str[i]))
        {
            return 0; // Not an integer
        }
    }
    return 1; // Integer
}
void addAccount()
{
    FILE * file = fopen("accounts.txt", "a");
    if (file == NULL)
    {
        printf("File doesn't exist.\n");
        exit(1);
    }

    Account newAccount;

    do
    {
        printf("Enter Account Number (10 digits): ");
        fflush(stdin);
        fgets(newAccount.accountNumber, sizeof(newAccount.accountNumber), stdin);
        newAccount.accountNumber[strcspn(newAccount.accountNumber, "\n")] = '\0';

        if (strlen(newAccount.accountNumber) != 10 || isAccountNumberUnique(newAccount.accountNumber) || !isInteger(newAccount.accountNumber))
        {
            printf("Invalid or duplicate account number. Please enter a unique 10-digit account number.\n");
        }
    }
    while (strlen(newAccount.accountNumber) != 10 || isAccountNumberUnique(newAccount.accountNumber) || !isInteger(newAccount.accountNumber));

    do
    {

        printf("Enter Name: ");
        fflush(stdin);
        fgets(newAccount.name, sizeof(newAccount.name), stdin);
        newAccount.name[strcspn(newAccount.name, "\n")] = '\0';
        if (!isValidName(newAccount.name))
        {
            printf("Invalid name enter letters only\n");
        }

    }
    while (!isValidName(newAccount.name));

    do
    {
        printf("Enter Unique Mobile (11 digits starting with '01'): ");
        fflush(stdin);
        fgets(newAccount.mobile, sizeof(newAccount.mobile), stdin);
        newAccount.mobile[strcspn(newAccount.mobile, "\n")] = '\0';
    }
    while (!isValidMobileNumber(newAccount.mobile) || !isMobileNumberUnique(newAccount.mobile)); //but the calling function in new variable

    do
    {
        printf("Enter Unique Email (must contain '@' and end with '.com'): ");
        fflush(stdin);
        fgets(newAccount.email, sizeof(newAccount.email), stdin);
        newAccount.email[strcspn(newAccount.email, "\n")] = '\0';
    }
    while (!isValidEmail(newAccount.email) || !isEmailUnique(newAccount.email)); //but the calling function in new variable

    do
    {

        printf("Enter Balance: ");
        scanf("%lf", & newAccount.balance);
        fflush(stdin);

    }
    while (newAccount.balance < 0);

    newAccount.dateOpened = getCurrentDate();

    fprintf(file, "\n%s,%s,%s,%.2lf,%s,%d-%d", newAccount.accountNumber,
            newAccount.name, newAccount.email,
            newAccount.balance,
            newAccount.mobile,
            newAccount.dateOpened.month,
            newAccount.dateOpened.year);

    fclose(file);

    printf("Account added successfully.\n");
    menu();
}
char * months_in_letters(int month_number)
{
    char * months[] =
    {
        "January",
        "February",
        "March",
        "April",
        "May",
        "June",
        "July",
        "August",
        "September",
        "October",
        "November",
        "December"
    };
    return months[month_number - 1];
}

void sortByName(Account s[], int size)
{

    int i, j;
    Account temp;
    for (i = 0; i < size - 1; i++)
    {
        for (j = 0; j < size - 1; j++)
        {
            if (strcmp(s[j].name, s[j + 1].name) > 0)
            {
                temp = s[j + 1];
                s[j + 1] = s[j];
                s[j] = temp;
            }

        }
    }
}

void SortByDate(Account s[], int size)
{

    int i, j;
    Account temp;
    for (i = 0; i < size - 1; i++)
    {
        for (j = 0; j < size - i - 1; j++)
        {
            if (s[j].dateOpened.year > s[j + 1].dateOpened.year)
            {
                temp = s[j + 1];
                s[j + 1] = s[j];
                s[j] = temp;
            }
            if (s[j].dateOpened.year == s[j + 1].dateOpened.year)
            {
                if (s[j].dateOpened.month > s[j + 1].dateOpened.month)
                {
                    temp = s[j + 1];
                    s[j + 1] = s[j];
                    s[j] = temp;
                }
            }

        }
    }
}
void SortByBalance(Account s[], int size)
{
    int i, j;
    Account temp;
    for (i = 0; i < size-1; i++)
    {
        for (j = 0; j < size - i - 1; j++)
        {
            if (s[j].balance > s[j + 1].balance)
            {
                temp = s[j + 1];
                s[j + 1] = s[j];
                s[j] = temp;
            }
        }
    }

}

void Print_Sort()
{

    int size = 0;
    Account * s;
    int num = 0;
    s = load( & size);
    printf("Choose what type of sorting you want:\n");

    printf("1.Name\n");
    printf("2.Date\n");
    printf("3.Balance\n");
    printf("\nEnter a number from 1 to 3: ");
    scanf("%d", & num);
    fflush(stdin);
    switch (num)
    {
    case 1:
        sortByName(s, size);
        break;
    case 2:
        SortByDate(s, size);
        break;
    case 3:
        SortByBalance(s, size);
        break;
    default:
        printf("\nInvalid choice\n");
        Print_Sort();
        break;
    }
    int i;
    for (i = 0; i < size; i++)
    {
        printf("\nAccount Number : %s \n", s[i].accountNumber);
        printf("Name: %s\n", s[i].name);
        printf("E-mail : %s\n", s[i].email);
        printf("Balance: %.2lf $\n", s[i].balance);
        printf("Mobile: %s\n", s[i].mobile);
        printf("Date Opened: %s %d\n", months_in_letters(s[i].dateOpened.month), s[i].dateOpened.year);
    }
    free(s);
    menu();
}
// Function to delete an account
void delete_account()
{
    // Get the account number from the user
    Account * account;
    int numAccounts = 0, i, j;
    account = load( & numAccounts);

    char account_number[11];
    printf("Enter the account number you want to delete: ");
    fflush(stdin);
    fgets(account_number, sizeof(account_number), stdin);
    account_number[strcspn(account_number, "\n")] = '\0'; // remove the newline character

    do
    {
        if (isAccountNumberUnique(account_number) == 0)
            printf("Account not exist!");
        else
        {
            //search the accountnumber
            for (i = 0; i < numAccounts; i++)
            {
                if (strcmp(account_number, account[i].accountNumber) == 0)
                    break;
            }

            if (account[i].balance != 0)
                printf("Account balance must equal 0");
            else
            {
                // Remove the account by shifting the data
                for (j = i; j < numAccounts - 1; j++)
                {
                    account[j] = account[j + 1];

                }
                numAccounts--;

                printf("Account %s with zero balance is deleted.\n", account_number);
                save(account, numAccounts);
            }
        }
    }
    while (isAccountNumberUnique(account_number) && account[i].balance == 0);
    free(account);
    menu();
}
char* toLowerCase(char* input)
{
    // Calculate the length of the input string
    int length = strlen(input);

    // Allocate memory for the new string
    char* result = (char*)malloc((length + 1) * sizeof(char)); // the +1 is for the /0 at the end

    // Convert each character to lowercase
    for (int i = 0; i < length; i++)
    {
        result[i] = tolower(input[i]);
    }

    // Add the null terminator to the end of the new string
    result[length] = '\0';

    return result;
}
void QUERY()
{
    //loading accounts
    Account * accounts; // Pointer to an array of accounts
    int numAccounts = 0; // Actual number of accounts
    // Load data from the file and get the actual number of accounts
    accounts = load( & numAccounts);

    //asking user to supply the account number
    char searchNumber[11];
    printf("please enter the account number: ");
    //gets(searchNumber);
    fflush(stdin);
    fgets(searchNumber, sizeof(searchNumber), stdin);
    // Removing the newline character
    searchNumber[strcspn(searchNumber, "\n")] = '\0';

    //comparing
    int i;
    for (i = 0; i < numAccounts; i++)
    {
        if (strcmp(searchNumber, accounts[i].accountNumber) == 0)
        {
            break;
        }
    }
    if (i == numAccounts)
    {
        printf("Account not found.\n");
        QUERY();
    }

    printf("Account Number: %s\n", accounts[i].accountNumber);
    printf("Name: %s\n", accounts[i].name);
    printf("Email: %s\n", accounts[i].email);
    printf("Balance: %.2lf\n", accounts[i].balance);
    printf("Mobile: %s\n", accounts[i].mobile);
    printf("Date Opened: %s %d\n", months_in_letters(accounts[i].dateOpened.month), accounts[i].dateOpened.year);
    printf("\n");
    free(accounts);
    menu();
}
void ADVANCED_SEARCH()
{
    //loading accounts
    Account * accounts; // Pointer to an array of accounts
    int numAccounts = 0; // Actual number of accounts

    // Load data from the file and get the actual number of accounts
    accounts = load( & numAccounts);

    //asking user to supply the Keyword
    char keyword[50];
    printf("Please enter the keyword: ");
    fflush(stdin);
    fgets(keyword, sizeof(keyword), stdin);
    // Removing the newline character
    keyword[strcspn(keyword, "\n")] = '\0';

    //preparing to search
    int i = 0;
    int accountIndex[numAccounts];
    int foundAccounts = 0;
    //converting the keyword to lowercase
    char* keywordptr=toLowerCase(keyword);
    strcpy(keyword, keywordptr);
    free(keywordptr);

    // searching for the keyword
    char* nameptr =NULL;

    for (i = 0; i < numAccounts; i++)
    {
        if (strstr( nameptr=toLowerCase(accounts[i].name), keyword) != NULL)
        {
            accountIndex[foundAccounts] = i;
            foundAccounts++;
        }
        free(nameptr);
    }
    //in case of not found
    if (foundAccounts == 0)
    {
        printf("Account not found.\n");
        ADVANCED_SEARCH();
    }

    // printing the found accounts
    printf("\n===========================\n");
    int currentIndex;
    for (i = 0; i < foundAccounts; i++)
    {
        currentIndex = accountIndex[i];

        printf("Account Number : %s\n", accounts[currentIndex].accountNumber);
        printf("Name: %s\n", accounts[currentIndex].name);
        printf("E-mail : %s\n", accounts[currentIndex].email);
        printf("Balance: %.2lf $\n", accounts[currentIndex].balance);
        printf("Mobile: %s\n", accounts[currentIndex].mobile);
        printf("Date Opened: %s %d\n", months_in_letters(accounts[currentIndex].dateOpened.month), accounts[currentIndex].dateOpened.year);
        printf("===========================\n");
    }
    free(accounts);
    menu();
}
