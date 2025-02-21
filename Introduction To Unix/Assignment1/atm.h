// atm.h
#ifndef ATM_H
#define ATM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the ATM card struct
typedef struct {
    char holderName[100];
    int cardNumber;
    int expMonth;
    int expYear;
    double balance;
    int pin;
} ATMCard;

// Function prototypes
void readCardInfo(ATMCard *card);
void displayMenu();
void checkBalance(ATMCard *card);
void withdrawal(ATMCard *card);
void deposit(ATMCard *card);
void changePin(ATMCard *card);
void editName(ATMCard *card);
void renewCard(ATMCard *card);
void printCardInfo(ATMCard *card);

#endif
