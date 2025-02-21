// atm.c
#include "atm.h"

void readCardInfo(ATMCard *card) {
    printf("Enter holder name: ");
    scanf("%s", card->holderName);
    printf("Enter card number (6 digits): ");
    scanf("%d", &card->cardNumber);
    printf("Enter expiration month: ");
    scanf("%d", &card->expMonth);
    printf("Enter expiration year: ");
    scanf("%d", &card->expYear);
    printf("Enter initial balance: ");
    scanf("%lf", &card->balance);
    printf("Enter PIN (3 digits): ");
    scanf("%d", &card->pin);
}

void displayMenu() {
    printf("\n---- ATM Menu ----\n");
    printf("1. Check balance\n");
    printf("2. Withdrawal\n");
    printf("3. Deposit\n");
    printf("4. Change PIN number\n");
    printf("5. Edit name\n");
    printf("6. Renew card\n");
    printf("7. Print card information\n");
    printf("8. Exit\n");
}

void checkBalance(ATMCard *card) {
    printf("Current balance: %.2f\n", card->balance);
}

void withdrawal(ATMCard *card) {
    double amount;
    printf("Enter amount to withdraw: ");
    scanf("%lf", &amount);
    if (amount > card->balance) {
        printf("Insufficient balance!\n");
    } else {
        card->balance -= amount;
        printf("Withdrawal successful! New balance: %.2f\n", card->balance);
    }
}

void deposit(ATMCard *card) {
    double amount;
    printf("Enter amount to deposit: ");
    scanf("%lf", &amount);
    card->balance += amount;
    printf("Deposit successful! New balance: %.2f\n", card->balance);
}

void changePin(ATMCard *card) {
    int newPin;
    printf("Enter new PIN (3 digits): ");
    scanf("%d", &newPin);
    card->pin = newPin;
    printf("PIN changed successfully!\n");
}

void editName(ATMCard *card) {
    printf("Enter new holder name: ");
    scanf("%s", card->holderName);
    printf("Name changed successfully!\n");
}

void renewCard(ATMCard *card) {
    printf("Enter new expiration month: ");
    scanf("%d", &card->expMonth);
    printf("Enter new expiration year: ");
    scanf("%d", &card->expYear);
    printf("Card renewed successfully!\n");
}

void printCardInfo(ATMCard *card) {
    printf("\n---- Card Information ----\n");
    printf("Holder Name: %s\n", card->holderName);
    printf("Card Number: %06d\n", card->cardNumber);
    printf("Expiration Date: %02d/%d\n", card->expMonth, card->expYear);
    printf("Balance: %.2f\n", card->balance);
    printf("PIN: %03d\n", card->pin);
}
