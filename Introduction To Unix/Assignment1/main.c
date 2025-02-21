// main.c
#include "atm.h"

int main() {
    ATMCard card;
    readCardInfo(&card);
    int choice;

    while (1) {
        displayMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                checkBalance(&card);
                break;
            case 2:
                withdrawal(&card);
                break;
            case 3:
                deposit(&card);
                break;
            case 4:
                changePin(&card);
                break;
            case 5:
                editName(&card);
                break;
            case 6:
                renewCard(&card);
                break;
            case 7:
                printCardInfo(&card);
                break;
            case 8:
                printf("Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }

    return 0;
}
