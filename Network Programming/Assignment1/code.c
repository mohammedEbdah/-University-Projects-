#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Function prototypes
void displayMenu();
int customAddition(int a, int b);
int customModulus(int a, int b);
int customComplement(int a);
double customPower(int base, int exponent);
int getUserInput(const char *message);
void getTwoIntegers(int *a, int *b, const char *message);

int main() {
    int choice, num1, num2, base, exponent;

    do {
        displayMenu();
        choice = getUserInput("Enter your choice: ");

        // Perform operation based on user's choice
        if (choice == 1) {
            getTwoIntegers(&num1, &num2, "Enter the two 16-bit integers: ");
            printf("Result: %d\n", customAddition(num1, num2));
        } else if (choice == 2) {
            getTwoIntegers(&num1, &num2, "Enter the two 16-bit integers: ");
            if (num2 == 0) {
                printf("Error: Division by zero\n");
            } else {
                printf("Result: %d\n", customModulus(num1, num2));
            }
        } else if (choice == 3) {
            num1 = getUserInput("Enter the 16-bit integer: ");
            printf("Result: %d\n", customComplement(num1));
        } else if (choice == 4) {
            base = getUserInput("Enter the base: ");
            exponent = getUserInput("Enter the exponent: ");
            printf("Result: %.0f\n", customPower(base, exponent));
        } else if (choice == 5) {
            printf("Exiting...\n");
        } else {
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 5);

    return 0;
}

// Display the menu options
void displayMenu() {
    printf("\nMenu:\n");
    printf("1. Add\n");
    printf("2. Modulus\n");
    printf("3. Complement\n");
    printf("4. Power\n");
    printf("5. Exit\n");
}

// Perform addition operation
int customAddition(int a, int b) {
    return a + b;
}

// Perform modulus operation
int customModulus(int a, int b) {
    return a % b;
}

// Perform one's complement operation
int customComplement(int a) {
    return ~a;
}

// Perform power operation
double customPower(int base, int exponent) {
    return pow(base, exponent);
}

// Get integer input from user with input validation
int getUserInput(const char *message) {
    int input;
    printf("%s", message);
    
    // Loop until valid integer input is provided
    for (;;) {
        if (scanf("%d", &input) == 1 && getchar() == '\n') {
            break;
        }
        printf("Invalid input. Please enter a valid integer: ");
        while (getchar() != '\n'); // Clear input buffer
    }
    return input;
}

// Get two integers from user with input validation
void getTwoIntegers(int *a, int *b, const char *message) {
    printf("%s", message);
    
    // Loop until valid 16-bit integer input is provided for both integers
    for (;;) {
        *a = getUserInput("Enter the first integer: ");
        if (*a >= -32768 && *a <= 32767) {
            break;
        }
        printf("Invalid input. Please enter a valid 16-bit integer (-32768 to 32767): ");
    }
    for (;;) {
        *b = getUserInput("Enter the second integer: ");
        if (*b >= -32768 && *b <= 32767) {
            break;
        }
        printf("Invalid input. Please enter a valid 16-bit integer (-32768 to 32767): ");
    }
}
