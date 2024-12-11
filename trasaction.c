#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define total_transactions 100

struct transaction {
    char date[11];
    float amount;
    char type[20];
    char detail[50];
    char description[200];
};

float Total_savings = 0.0;

// Function to set initial savings
void set_initial_savings(const char *history) {
    printf("Enter the initial savings amount: ");
    float initial_savings;
    scanf("%f", &initial_savings);

    Total_savings = initial_savings;

    // Save the initial savings as a transaction in the file
    FILE *fp = fopen(history, "a");
    if (fp == NULL) {
        printf("Error: Unable to open the file.\n");
        return;
    }
    fprintf(fp, "2024-11-23,Income,%.2f,Initial Setup,Initial savings\n", initial_savings);
    fclose(fp);

    printf("Initial savings of %.2f set and saved successfully!\n", initial_savings);
}

// Function to load savings from the file
void load_savings(const char *history) {
    FILE *fp = fopen(history, "r");
    if (fp == NULL) {
        printf("No previous transactions found. Starting fresh.\n");
        return;
    }

    Total_savings = 0.0; // Reset total savings
    char line[300];
    while (fgets(line, sizeof(line), fp) != NULL) {
        line[strcspn(line, "\n")] = 0; // Remove newline character

        char *date = strtok(line, ",");
        char *type = strtok(NULL, ",");
        char *amountStr = strtok(NULL, ",");
        char *detail = strtok(NULL, ",");
        char *description = strtok(NULL, "\n");

        if (!date || !type || !amountStr || !detail || !description) {
            printf("Skipping malformed line: %s\n", line);
            continue;
        }

        float amount = atof(amountStr);

        // Update Total_savings based on transaction type
        if (strcmp(type, "Income") == 0) {
            Total_savings += amount;
        } else if (strcmp(type, "Expense") == 0) {
            Total_savings -= amount;
        }

        printf("Processed transaction: Date=%s, Type=%s, Amount=%.2f\n", date, type, amount);
    }
    fclose(fp);
    printf("Loaded Total Savings: %.2f\n", Total_savings);
}

// Function to add transactions
void add_transactions(const char *history) {
    struct transaction t;
    printf("Enter date (YYYY-MM-DD): ");
    scanf("%10s", t.date);
    printf("Enter the type (Income/Expense): ");
    scanf("%19s", t.type);
    printf("Enter amount: ");
    scanf("%f", &t.amount);
    if (t.amount <= 0) {
        printf("Error: Amount must be greater than 0.\n");
        return;
    }
    printf("Enter detail: ");
    scanf("%49s", t.detail);
    printf("Enter description: ");
    getchar(); // Consume newline
    fgets(t.description, sizeof(t.description), stdin);
    t.description[strcspn(t.description, "\n")] = 0;

    // Validate transaction type
    if (strcmp(t.type, "Income") != 0 && strcmp(t.type, "Expense") != 0) {
        printf("Invalid transaction type. Use 'Income' or 'Expense'.\n");
        return;
    }

    // Update Total Savings
    if (strcmp(t.type, "Income") == 0) {
        Total_savings += t.amount;
    } else {
        Total_savings -= t.amount;
    }

    FILE *fp = fopen(history, "a");
    if (fp == NULL) {
        printf("Error: Unable to open the file.\n");
        return;
    }
    fprintf(fp, "%s,%s,%.2f,%s,%s\n", t.date, t.type, t.amount, t.detail, t.description);
    fclose(fp);
    printf("Transaction added successfully!\n");
    printf("Updated Total Savings: %.2f\n", Total_savings);
}

// Function to display transactions
void display_transactions(const char *history) {
    struct transaction transactions[total_transactions];
    int count = 0;

    FILE *fp = fopen(history, "r");
    if (fp == NULL) {
        printf("Error: Unable to open the file.\n");
        return;
    }

    char line[300];
    while (fgets(line, sizeof(line), fp) != NULL) {
        line[strcspn(line, "\n")] = 0; // Remove newline character
        if (count >= total_transactions) {
            printf("Maximum number of transactions reached.\n");
            break;
        }

        char *date = strtok(line, ",");
        char *type = strtok(NULL, ",");
        char *amountStr = strtok(NULL, ",");
        char *detail = strtok(NULL, ",");
        char *description = strtok(NULL, "\n");

        if (!date || !type || !amountStr || !detail || !description) {
            printf("Skipping malformed line: %s\n", line);
            continue;
        }

        strcpy(transactions[count].date, date);
        strcpy(transactions[count].type, type);
        transactions[count].amount = atof(amountStr);
        strcpy(transactions[count].detail, detail);
        strcpy(transactions[count].description, description);

        count++;
    }
    fclose(fp);

    if (count == 0) {
        printf("No transactions found.\n");
        return;
    }

    printf("\nExisting Transactions:\n");
    for (int i = 0; i < count; i++) {
        printf("Date: %s\n", transactions[i].date);
        printf("Income/Expense: %s\n", transactions[i].type);
        printf("Amount: %.2f\n", transactions[i].amount);
        printf("Detail: %s\n", transactions[i].detail);
        printf("Description: %s\n", transactions[i].description);
        printf("----------------------\n");
    }
    printf("Total Remaining Balance: %.2f\n", Total_savings);
}

int main() {
    const char *filename = "history.csv";
    int choice;

    // Load existing savings
    load_savings(filename);

    do {
        printf("\nOptions:\n");
        printf("1. Add a Transaction\n");
        printf("2. Display Transactions\n");
        printf("3. Set Initial Savings\n");
        printf("4. Exit\n");
        printf("Select your option: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
            case 1:
                add_transactions(filename);
                break;
            case 2:
                display_transactions(filename);
                break;
            case 3:
                set_initial_savings(filename);
                break;
            case 4:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid option. Please try again.\n");
        }
    } while (choice != 4);

    return 0;
}
