#ifndef FUNCTIONS_H
#define FUNCTIONS_H

// User accessible functions
void menuNotLoggedIn();
void menuLoggedIn();
void logIn();
void createAccount();
void displayBalance(char *accountNumber, float balance);
void transferMoney(char *accountNumberSender, float balance);
void cashDeposit(char *accountNumber);
void creditCard(char *accountNumber, char *cardNumber, char *cardPin);
void displayHistory(char *accountNumber);
void manageData(char *firstName, char *lastName, char *pesel, char *nationality, char *idNumber, char *address, char *phoneNumber, char *password);
void deleteAccount(char *accountNumber, float balance);

// Backend functions
void updateData(char *accountNumber);
void updateFile(char *accountNumber, int fileLine, char *newData);
void updateBalance(char *accountNumber, float amount);
int accountExistsCheck(char *accountNumber);
char* readLine(char *accountNumber, int fileLine);
char* getTime();
int stopOperation(char *string);
void printWrongChoice();
void cls();
void end();

#endif