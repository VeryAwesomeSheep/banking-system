#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct creditCard
{
	char cardNumber[17];
	char cardPin[5];
};

typedef struct client
{
	char firstName[50];
	char lastName[50];
	char pesel[12];
	char nationality[50];
	char idNumber[10];
	char address[100];
	char phoneNumber[10];
	char password[50];
	char accountNumber[27];
	float balance;
	struct creditCard card;
} client;

#endif