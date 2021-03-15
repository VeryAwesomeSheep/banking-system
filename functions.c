/*
In a lot of scanf's there could be a anti overflow safety measure but I wanted to be able to prompt users when their inputs are too long.
I could have used for ex. "%49s" but then I couldn't make that prompt. 
Well.. actually I could allocate +1 memory for inputs and limit the input to that and then propmt the user.

input[51]
scanf("%50s")
if (strlen(input) > 49)

TODO: ^
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <io.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "functions.h"
#include "structs.h"

client *Client;

void menuNotLoggedIn() { // Menu for not logged in users
	mkdir("accounts"); // Creates folder for accounts if doesn't exists
	srand(time(NULL));
	int menuChoice;

	do {
		cls();
		printf("ONION BANK\n\n");
		printf("MENU\n1. Create account\n2. Log in\n0. Exit\n");
		scanf("%d", &menuChoice);
		cls();

		switch (menuChoice)
		{
		case 0:
			end();
			break;
		case 1:
			createAccount();
			break;
		case 2:
			logIn();
			break;
		default:
			while ((menuChoice = getchar()) != '\n' && menuChoice != EOF) { }
			printWrongChoice();
			getch();
			break;
		}
	} while (menuChoice != 0);
}

void menuLoggedIn(char *accountNumberLoggedIn) { // Menu for not logged in users
	int menuChoice;
	Client = (client*)malloc(sizeof(client)); // Allocating dynamic memory for clinet's data
	
	strcpy(Client->firstName, readLine(accountNumberLoggedIn, 1)); // Reading client's data from file
	strcpy(Client->lastName, readLine(accountNumberLoggedIn, 2));
	strcpy(Client->pesel, readLine(accountNumberLoggedIn, 3));
	strcpy(Client->nationality, readLine(accountNumberLoggedIn, 4));
	strcpy(Client->idNumber, readLine(accountNumberLoggedIn, 5));
	strcpy(Client->address, readLine(accountNumberLoggedIn, 6));
	strcpy(Client->phoneNumber, readLine(accountNumberLoggedIn, 7));
	strcpy(Client->accountNumber, readLine(accountNumberLoggedIn, 8));
	strcpy(Client->password, readLine(accountNumberLoggedIn, 9));
	strcpy(Client->card.cardNumber, readLine(accountNumberLoggedIn, 10));
	strcpy(Client->card.cardPin, readLine(accountNumberLoggedIn, 11));
	Client->balance = strtof(readLine(accountNumberLoggedIn, 12), NULL);

	do {
		cls();
		printf("ONION BANK\n\n");
		printf("Welcome %s %s!\n\n", Client->firstName, Client->lastName);
		printf("MENU\n1. Balance\n2. Money transfer\n3. Cash deposit\n4. Credit card\n5. History\n6. Your data\n7. Close account\n0. Log out\n", Client->firstName, Client->lastName);
		scanf("%d", &menuChoice);
		cls();

		switch (menuChoice)
		{
		case 0:
			free(Client);
			Client = NULL;
			return;
			break;
		case 1:
			displayBalance(Client->accountNumber, Client->balance);
			break;
		case 2:
			transferMoney(Client->accountNumber, Client->balance);
			updateData(accountNumberLoggedIn);
			break;
		case 3:
			cashDeposit(Client->accountNumber);
			updateData(accountNumberLoggedIn);
			break;
		case 4:
			creditCard(Client->accountNumber, Client->card.cardNumber, Client->card.cardPin);
			updateData(accountNumberLoggedIn);
			break;
		case 5:
			displayHistory(Client->accountNumber);
			break;
		case 6:
			manageData(Client->firstName, Client->lastName, Client->pesel, Client->nationality, Client->idNumber, Client->address, Client->phoneNumber, Client->password);
			break;
		case 7:
			deleteAccount(Client->accountNumber, Client->balance);
			break;
		default:
			while ((menuChoice = getchar()) != '\n' && menuChoice != EOF) { }
			printWrongChoice();
			getch();
			break;
		}
	} while (menuChoice != 0);
}

void logIn() { // Log in interface
	char accountNumber[27], password[50], passwordInput[50];

	printf("Account number: ");
	scanf("%s", &accountNumber);
	printf("Password: ");
	scanf("%s", &passwordInput);
	strcpy(password, readLine(accountNumber, 9)); // Check if input matches password
	if (strcmp(password, passwordInput) == 0) {
		menuLoggedIn(accountNumber);
	}
	else {
		printf("\nWrong account number or password.\n\nPress enter to continue.");
		getch();
		return;
	}
}

void createAccount() { // Account creation interface
	FILE *fCreateAccount;
	char temp;
	char fName[41];
	char firstName[51], lastName[50], pesel[12], nationality[50], idNumber[10], address[100], phoneNumber[10], password[50];
	char accountNumber[27] = "", generatedDigit[2], generatedNumber[17] = "";
    static char prefix[11] = "4611402004";

	cls();
	printf("ONION BANK\n\n");
	printf("We are happy that you are interested in our services. Now we will go through the process of creating your account.\n");
	printf("You can stop at any moment by typing \"q\".\n\nPress enter to continue.");
	getch();

	do {
		cls();
		printf("Enter your first name: ");
		scanf("%s", &firstName);
		if (stopOperation(firstName) == 1) return; // If user inputs "q" then process stops
		if (49 < strlen(firstName)) { // If user enters more characters than he should he gets prompted to do it again
			cls();
			printf("First name cannot be longer than 49 characters! Ex. \"John\".\n\nPress enter to continue.\n");
			getch();
		}
	} while (49 < strlen(firstName));
	
	do {
		cls();
		printf("Enter your last name: ");
		scanf("%s", &lastName);
		if (stopOperation(lastName) == 1) return;
		if (49 < strlen(lastName)) {
			cls();
			printf("Last name cannot be longer than 49 characters! Ex. \"Doe\".\n\nPress enter to continue.\n");
			getch();
		}
	} while (49 < strlen(lastName));

	do {
		cls();
		printf("Enter your PESEL number: ");
		scanf("%s", &pesel);
		if (stopOperation(pesel) == 1) return;
		if (11 < strlen(pesel) || 11 > strlen(pesel)) {
			cls();
			printf("PESEL must be 11 digits long! Ex. \"12345678910\".\n\nPress enter to continue.\n");
			getch();
		}
	} while (11 < strlen(pesel) || 11 > strlen(pesel));

	do {
		cls();
		printf("Enter your nationality: ");
		scanf("%s", &nationality);
		if (stopOperation(nationality) == 1) return;
		if (49 < strlen(nationality)) {
			cls();
			printf("Nationality cannot be longer than 49 characters! Ex. \"polish\".\n\nPress enter to continue.\n");
			getch();
		}
	} while (49 < strlen(nationality));

	do {
		cls();
		printf("Enter your ID number: ");
		scanf("%s", &idNumber);
		if (stopOperation(idNumber) == 1) return;
		if (9 < strlen(idNumber) || 9 > strlen(idNumber)) {
			cls();
			printf("ID number must be 9 characters long! Ex. \"ABC123456\".\n\nPress enter to continue.\n");
			getch();
		}
	} while (9 < strlen(idNumber) || 9 > strlen(idNumber));

	scanf("%c", &temp);
	do {
		cls();
		printf("Enter your address: ");
		scanf("%[^\n]%*c", &address); // scanf format to accept spaces
		if (stopOperation(address) == 1) return;
		if (99 < strlen(address)) {
			cls();
			printf("Address cannot be longer than 99 characters! Ex. \"Polna 5/3, 11-111 Warszawa\".\n\nPress enter to continue.\n");
			getch();
		}
	} while (99 < strlen(address));

	do {
		cls();
		printf("Enter your phone number: ");
		scanf("%s", &phoneNumber);
		if (stopOperation(phoneNumber) == 1) return;
		if (9 < strlen(phoneNumber) || 9 > strlen(phoneNumber)) {
			cls();
			printf("Phone number must be 9 digits long! Ex. \"111555333\".\n\nPress enter to continue.\n");
			getch();
		}
	} while (9 < strlen(phoneNumber) || 9 > strlen(phoneNumber));

	do {
		cls();
		printf("Enter your password: ");
		scanf("%s", &password);
		if (stopOperation(password) == 1) return;
		if (49 < strlen(password)) {
			cls();
			printf("Password cannot be longer than 49 characters!\n\nPress enter to continue.\n");
			getch();
		}
	} while (49 < strlen(password));

	for (int i = 0; i < 16; i++) { // Account number generation
        int j = rand() % 10;
        sprintf(generatedDigit, "%d", j);
        strcat(generatedNumber, generatedDigit);
    }
    strcat(accountNumber, prefix);
    strcat(accountNumber, generatedNumber);

	sprintf(fName, "accounts/%s.txt", accountNumber); // Account file path generation
	fCreateAccount = fopen(fName, "w");
	if (fCreateAccount == NULL) {
		printf("\nFailed to open file.");
		getch();
		return;
	}

	fprintf(fCreateAccount, "%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n----------------\n----\n0.00\n\n", firstName, lastName, pesel, nationality, idNumber, address, phoneNumber, accountNumber, password); // Putting data to file
	fclose(fCreateAccount);
	cls();
	printf("Congratulations!\n\nYour account has been successfully created.\nYour account number: %s.\n\nTo log in use this account number and your password.\n\nPress enter to continue.", accountNumber);
	getch();
}

void displayBalance(char *accountNumber, float balance) { // Displays clients account balance
	printf("Account: %s\nBalance: %.2f PLN\n\nPress enter to come back.", accountNumber, balance);
	getch();
}

void transferMoney(char *accountNumberSender, float balance) { // Money transfer between accounts
	int menuChoice;
	char temp;
	char accountNumberRecepient[27], recordSender[200], recordRecepient[200], transferTitle[100] = "";
	char fAccountNumberSender[41], fAccountNumberRecepient[41];
	float amount;
	
	do {
		printf("To abort enter \"q\".\n\nEnter recepient's account number: ");
		scanf("%26s", &accountNumberRecepient);
		if (stopOperation(accountNumberRecepient) == 1) return; // If user inputs "q" then process stops
		if (strcmp(accountNumberSender, accountNumberRecepient) == 0) { // Check if user tries to send money to himself
			cls();
			printf("You cannot address a transfer to yourself.\n\nPress enter to continue.\n");
			getch();
		}
		if (accountExistsCheck(accountNumberRecepient) == 0) { // Check if recepient's account exists
			cls();
			printf("Account with such number doesn't exist.\n\nPress enter to continue.\n");
			getch();
		}
	} while (strcmp(accountNumberSender, accountNumberRecepient) == 0 || accountExistsCheck(accountNumberRecepient) == 0);

	do {
		cls();
		printf("To abort enter \"0\".\n\nEnter transfer amount: ");
		scanf("%f", &amount);
		if (amount == 0) return; // If user inputs "0" then process stops
		if (amount < 0) { // Check if user tries to send negative amount
			cls();
			printf("Amount must be higher than 0.00 PLN.\n\nPress enter to continue.\n");
			getch();
		}
		if (amount > balance) { // Check if user has enough money
			cls();
			printf("You can't make that operation. Entered amount is higher than your balance.\n\nPress enter to continue.\n");
			getch();
		} 
	} while (amount < 0 || amount > balance);

	scanf("%c", &temp);
	do {
		cls();
		printf("To abort enter \"q\".\n\nEnter transfer title: ");
		scanf("%[^\n]%*c", &transferTitle); // scanf format to accept spaces
		if (stopOperation(transferTitle) == 1) return;
		if (99 < strlen(transferTitle)) {
			cls();
			printf("Transfer title cannot be longer than 99 characters!\n\nPress enter to continue.\n");
			getch();
		}
	} while (99 < strlen(transferTitle));

	do { // Confirmation of transfer
		cls();
		printf("Recipient account number: %s\nTransfer amount: %.2f\nTransfer title: %s\n\n1. Confirm\n2. Abort\n", accountNumberRecepient, amount, transferTitle);
		scanf("%d", &menuChoice);

		switch (menuChoice)
		{
		case 1:
			sprintf(fAccountNumberSender, "accounts/%s.txt", accountNumberSender);
			sprintf(fAccountNumberRecepient, "accounts/%s.txt", accountNumberRecepient);

			FILE *fSender = fopen(fAccountNumberSender, "a"); // Opening account files
			FILE *fRecepient = fopen(fAccountNumberRecepient, "a");

			time_t rawtime; // Creating current date info
			struct tm * timeinfo;
			time (&rawtime);
			timeinfo = localtime (&rawtime);

			sprintf(recordSender, "Date: %sAmount: %.2f PLN    Account number: %s    Title: \"%s\"\n", asctime(timeinfo), amount * -1, accountNumberRecepient, transferTitle); // Creating records
			sprintf(recordRecepient, "Date: %sAmount: +%.2f PLN    Account number: %s    Title: \"%s\"\n", asctime(timeinfo), amount, accountNumberRecepient, transferTitle);

			fprintf(fSender, "%s\n", recordSender); // Putting records to files
			fprintf(fRecepient, "%s\n", recordRecepient);

			fclose(fSender);
			fclose(fRecepient);

			updateBalance(accountNumberSender, amount * -1); // Updating account balances
			updateBalance(accountNumberRecepient, amount);

			cls();
			printf("Transfer completed.\n\nPress enter to continue.\n");
			getch();
			return;
			break;
		case 2:
			return;
			break;
		default:
			while ((menuChoice = getchar()) != '\n' && menuChoice != EOF) { }
			printWrongChoice();
			getch();
			break;
		}
	} while (menuChoice != 2);
}

void cashDeposit(char *accountNumber) { // Cash deposit (in this case it's a money printer)
	int menuChoice;
	char fAccountNumber[41], record[200];
	float amount;

	printf("Enter amount to deposit: ");
	scanf("%f", &amount);
	if (amount == 0) return; // If user inputs "0" then process stops

	do { // Deposit confirmation
		cls();
		printf("Amount to deposit: %.2f\n\n1. Confirm\n2. Abort\n", amount);
		scanf("%d", &menuChoice);

		switch (menuChoice)
		{
		case 1:
			sprintf(fAccountNumber, "accounts/%s.txt", accountNumber);

			FILE *f = fopen(fAccountNumber, "a");

			time_t rawtime;
			struct tm * timeinfo;
			time (&rawtime);
			timeinfo = localtime (&rawtime);

			sprintf(record, "Date: %sAmount: %.2f PLN    Cash deposit\n", asctime(timeinfo), amount);

			fprintf(f, "%s\n", record);
			
			fclose(f);

			updateBalance(accountNumber, amount);

			cls();
			printf("Cash deposit completed.\n\nPress enter to continue.\n");
			getch();
			return;
			break;
		case 2:
			return;
			break;
		default:
			while ((menuChoice = getchar()) != '\n' && menuChoice != EOF) { }
			printWrongChoice();
			getch();
			break;
		}
	} while (menuChoice != 2);
}

void creditCard(char *accountNumber, char *cardNumber, char *cardPin) { // Displaying credit card data and if it doesn't exists creating one
	int menuChoice;
	char generatedCard[17] = "", generatedCardDigit[2], generatedCardNumber[11] = "", generatedPin[5] = "", generatedPinDigit[2]; 
	static char cardPrefix[7] = "405671";

	if (strcmp(cardNumber, "----------------") == 0 && strcmp(cardPin, "----") == 0) { // Check if credit card exists
		do {
			printf("You don't have a credit card.\n\nWould you like to generate one?\n1. Yes\n2. No\n"); // Ask if user wants to create credit card
			scanf("%d", &menuChoice);

			switch (menuChoice)
			{
			case 1:
				for (int h = 0; h < 10; h++) { // Generating credit card number
					int k = rand() % 10;
					sprintf(generatedCardDigit, "%d", k);
					strcat(generatedCardNumber, generatedCardDigit);
				}
				strcat(generatedCard, cardPrefix);
				strcat(generatedCard, generatedCardNumber);

				for (int u = 0; u < 4; u++) { // Generating credit card pin
					int p = rand() % 10;
					sprintf(generatedPinDigit, "%d", p);
					strcat(generatedPin, generatedPinDigit);
				}

				updateFile(accountNumber, 10, generatedCard); // Adding credit card info to account file
				updateFile(accountNumber, 11, generatedPin);
				
				cls();
				printf("Details of your new credit card:\nNumber: %sPIN: %s\n\nPress enter to continue.\n", generatedCard, generatedPin);
				getch();
				return;
				break;
			case 2:
				return;
				break;
			default:
				while ((menuChoice = getchar()) != '\n' && menuChoice != EOF) { }
				printWrongChoice();
				getch();
				break;
			}
		} while (menuChoice != 2);
	} else {
		printf("Your credit card data:\nNumber: %s\nPin: %s\n\nPress enter to come back.", cardNumber, cardPin); // If card exists, printing out the details
		getch();
		return;
	}
}

void displayHistory(char *accountNumber) { // Displaying operations history
	char fAccountNumber[41], lineContent[100];
	int counter = 1;
	
	sprintf(fAccountNumber, "accounts/%s.txt", accountNumber);
	FILE *f = fopen(fAccountNumber, "r");

	if (f == NULL) {
		goto jump;
	}

	while (counter < 13) { // Going to line 13 where history starts
		fgets(lineContent, 100, f);
		counter++;
	}

	printf("History of your transactions:\n");
	while (fgets(lineContent, 100, f) != NULL) { // Printing history
		printf("%s", lineContent);
	}
	jump:
	fclose(f);

	printf("\n\nPress enter to come back.\n");
	getch();
}

void manageData(char *firstName, char *lastName, char *pesel, char *nationality, char *idNumber, char *address, char *phoneNumber, char *password) { // Managing user data
	int menuChoice;
	char newFirstName[50], newLastName[50], newPesel[12], newNationality[50], newIdNumber[10], newAddress[100], newPhoneNhmber[10], newPassword[50];
	char temp;

	do {
		cls();
		printf("Your data: \nFirst name: %s\nLast name: %s\nPESEL: %s\nNationality: %s\nID number: %s\nAddress: %s\nPhone: %s\nPassword: %s\n\n", firstName, lastName, pesel, nationality, idNumber, address, phoneNumber, password); // Printing current user data
		printf("Would you like to change anything? Please select option or abort.\n"); // Ask if user want's to change anything
		printf("1. First name\n2. Last name\n3. PESEL\n4. Nationality\n5. ID number\n6. Address\n7. Phone\n8. Password\n0. Abort\n");
		scanf("%d", &menuChoice);

		switch (menuChoice)
		{
		case 0:
			return;
			break;
		case 1:
			do {
				cls();
				printf("To abort enter \"q\".\n\nEnter your new first name: ");
				scanf("%s", &newFirstName); // Ask for new name
				if (stopOperation(newFirstName) == 1) break; // If user inputs "q" then process stops
				if (49 < strlen(newFirstName)) { // If user enters more characters than he should he gets prompted to do it again
					cls();
					printf("First name cannot be longer than 49 characters! Ex. \"John\".\n\nPress enter to continue.\n");
					getch();
				}
			} while (49 < strlen(newFirstName));
			updateFile(Client->accountNumber, 1, newFirstName); // Updating data in account file
			updateData(Client->accountNumber);
			break;
		case 2:
			do {
				cls();
				printf("To abort enter \"q\".\n\nEnter your new last name: ");
				scanf("%s", &newLastName);
				if (stopOperation(newLastName) == 1) break;
				if (49 < strlen(newLastName)) {
					cls();
					printf("Last name cannot be longer than 49 characters! Ex. \"John\".\n\nPress enter to continue.\n");
					getch();
				}
			} while (49 < strlen(newLastName));
			updateFile(Client->accountNumber, 2, newLastName);
			updateData(Client->accountNumber);
			break;
		case 3:
			do {
				cls();
				printf("To abort enter \"q\".\n\nEnter your PESEL number: ");
				scanf("%s", &newPesel);
				if (stopOperation(newPesel) == 1) return;
				if (11 < strlen(newPesel) || 11 > strlen(newPesel)) {
					cls();
					printf("PESEL must be 11 digits long! Ex. \"12345678910\".\n\nPress enter to continue.\n");
					getch();
				}
			} while (11 < strlen(newPesel) || 11 > strlen(newPesel));
			updateFile(Client->accountNumber, 3, newPesel);
			updateData(Client->accountNumber);
			break;
		case 4:
			do {
				cls();
				printf("To abort enter \"q\".\n\nEnter your new nationality: ");
				scanf("%s", &newNationality);
				if (stopOperation(newNationality) == 1) break;
				if (49 < strlen(newNationality)) {
					cls();
					printf("Nationality cannot be longer than 49 characters! Ex. \"John\".\n\nPress enter to continue.\n");
					getch();
				}
			} while (49 < strlen(newNationality));
			updateFile(Client->accountNumber, 4, newNationality);
			updateData(Client->accountNumber);
			break;
		case 5:
			do {
				cls();
				printf("To abort enter \"q\".\n\nEnter your new ID number: ");
				scanf("%s", &newIdNumber);
				if (stopOperation(newIdNumber) == 1) return;
				if (9 < strlen(newIdNumber) || 9 > strlen(newIdNumber)) {
					cls();
					printf("ID number must be 9 characters long! Ex. \"ABC123456\".\n\nPress enter to continue.\n");
					getch();
				}
			} while (9 < strlen(newIdNumber) || 9 > strlen(newIdNumber));
			updateFile(Client->accountNumber, 5, newIdNumber);
			updateData(Client->accountNumber);
			break;
		case 6:
			scanf("%c", &temp);
			do {
				cls();
				printf("To abort enter \"q\".Enter your new address: ");
				scanf("%[^\n]%*c", &newAddress);
				if (stopOperation(newAddress) == 1) return;
				if (99 < strlen(newAddress)) {
					cls();
					printf("Address cannot be longer than 99 characters! Ex. \"Polna 5/3, 11-111 Warszawa\".\n\nPress enter to continue.\n");
					getch();
				}
			} while (99 < strlen(newAddress));
			updateFile(Client->accountNumber, 6, newAddress);
			updateData(Client->accountNumber);
			break;
		case 7:
			do {
				cls();
				printf("To abort enter \"q\".\n\nEnter your new phone number: ");
				scanf("%s", &newPhoneNhmber);
				if (stopOperation(newPhoneNhmber) == 1) return;
				if (9 < strlen(newPhoneNhmber) || 9 > strlen(newPhoneNhmber)) {
					cls();
					printf("Phone number must be 9 characters long! Ex. \"111555333\".\n\nPress enter to continue.\n");
					getch();
				}
			} while (9 < strlen(newPhoneNhmber) || 9 > strlen(newPhoneNhmber));
			updateFile(Client->accountNumber, 7, newPhoneNhmber);
			updateData(Client->accountNumber);
			break;
		case 8:
			do {
				cls();
				printf("To abort enter \"q\".\n\nEnter your new password: ");
				scanf("%s", &newPassword);
				if (stopOperation(newPassword) == 1) break;
				if (49 < strlen(newPassword)) {
					cls();
					printf("Password cannot be longer than 49 characters! Ex. \"John\".\n\nPress enter to continue.\n");
					getch();
				}
			} while (49 < strlen(newPassword));
			updateFile(Client->accountNumber, 9, newPassword);
			updateData(Client->accountNumber);
			break;
		default:
			while ((menuChoice = getchar()) != '\n' && menuChoice != EOF) { }
			printWrongChoice();
			getch();
			break;
		}
	} while (menuChoice != 0);
}

void deleteAccount(char *accountNumber, float balance) { // Deleting user account
	int menuChoice;
	char f[41];

	printf("You are about to close your account.\nAre you sure you want to proceed?\n1. Yes\n2. No\n"); // Ask if user wants to delete account
	scanf("%d", &menuChoice);

	switch (menuChoice)
	{
	case 1:
		if (balance > 0.00) { // Check if balance is greater than 0, if it is then account cannot be deleted
			cls();
			printf("We cannot close your account, please withdraw all your money first.\n\nPress enter to continue.\n");
			getch();
			return;
		} else {
			sprintf(f, "accounts/%s.txt", accountNumber); // Deleting account file
			int del = remove(f);
			if (!del) {
				cls();
				printf("Your account has been closed.\n\nPress enter to continue.\n");
				getch();
			} else {
				cls();
				printf("There was an error during account removal.\n\nPress enter to continue.\n");
				getch();
			}
			menuNotLoggedIn();
		}
		break;
	case 2:
		return;
		break;
	default:
		while ((menuChoice = getchar()) != '\n' && menuChoice != EOF) { }
		printWrongChoice();
		getch();
		break;
	}
}

/* =============================================================== */
void updateData(char *accountNumber) { // Updating data of currently logged in user
	strcpy(Client->firstName, readLine(accountNumber, 1));
	strcpy(Client->lastName, readLine(accountNumber, 2));
	strcpy(Client->pesel, readLine(accountNumber, 3));
	strcpy(Client->nationality, readLine(accountNumber, 4));
	strcpy(Client->idNumber, readLine(accountNumber, 5));
	strcpy(Client->address, readLine(accountNumber, 6));
	strcpy(Client->phoneNumber, readLine(accountNumber, 7));
	strcpy(Client->accountNumber, readLine(accountNumber, 8));
	strcpy(Client->password, readLine(accountNumber, 9));
	strcpy(Client->card.cardNumber, readLine(accountNumber, 10));
	strcpy(Client->card.cardPin, readLine(accountNumber, 11));
	Client->balance = strtof(readLine(accountNumber, 12), NULL);
}

void updateFile(char *accountNumber, int fileLine, char *newData) { // Updating data in account file
	FILE *f;
	FILE *fTemp;
	char buffer[100], fAccountNumber[41];
	int count = 0;

	sprintf(fAccountNumber, "accounts/%s.txt", accountNumber);
	sprintf(newData, "%s\n", newData);

	f = fopen(fAccountNumber, "r");
	fTemp = fopen("replace.tmp", "w"); // Creating temp file
	if (f == NULL || fTemp == NULL) {
		return;
	}
	
	while ((fgets(buffer, 100, f)) != NULL) { // Going to desired line
		count++;
		if (count == fileLine) { // Check if loop is at desired line
			fputs(newData, fTemp); // Replacing data
		} else {
			fputs(buffer, fTemp); // Copying data to temp file
		}
	}

	fclose(f);
	fclose(fTemp);
	remove(fAccountNumber); // Deleteing old original file
	rename("replace.tmp", fAccountNumber); // Renaming temp file to real account file
}

void updateBalance(char *accountNumber, float amount) { // Updating balace
	char newBalance[20];

	sprintf(newBalance, "%.2f", strtof(readLine(accountNumber, 12), NULL) + amount);
	updateFile(accountNumber, 12, newBalance);
}

int accountExistsCheck(char *accountNumber) { // Check if account exists
	FILE *f;
	char fAccountNumber[41];
	sprintf(fAccountNumber, "accounts/%s.txt", accountNumber);

	if ((f = fopen(fAccountNumber, "r"))) {
		fclose(f);
		return 1;
	} else {
		return 0;
	}
}

char* readLine(char *accountNumber, int fileLine) { // Reading line from account file
	char fAccountNumber[41];
	int counter = 1;
	char lineContent[100];
	char *lineContentPtr = lineContent;
	sprintf(fAccountNumber, "accounts/%s.txt", accountNumber);
	FILE *f = fopen(fAccountNumber, "r");

	if (f == NULL) {
		goto jump;
	}

	while (counter < fileLine) {
		fgets(lineContent, 100, f);
		counter++;
	}
	fgets(lineContent, 100, f);
	lineContent[strcspn(lineContent, "\n")] = 0;
	jump:
	fclose(f);
	return lineContentPtr;
}

int stopOperation(char *string) { // Stopping operation if user enters "q"
	if (strcmp(string, "q") == 0) {
		return 1;
	}
	else return 0;
}

void printWrongChoice() { // Default message for switch case
	cls();
	printf("Wrong choice!\n\nPress enter to continue.\n");
}

void cls() { // Clearing the console
	system("cls");
}

void end() { // Ending the program
	exit(0);
}