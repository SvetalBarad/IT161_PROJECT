

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_USERS 100
#define MAX_STOCKS 5

typedef struct {
char username[50];
char password[50];
float walletBalance;
int stocksOwned[MAX_STOCKS];
} User;

typedef struct {
char stockName[50];
float stockPrice;
} Stock;


User users[MAX_USERS];
int userCount = 0;
int loggedInUser = -1;
Stock stocks[MAX_STOCKS];


void initializeStocks();
void mainMenu();
void login();
void registerUser();
void viewStocks();
void tradeStocks();
void viewPortfolio();
void saveData();
void loadData();


int main() {
    loadData(); 
    initializeStocks(); 
    mainMenu();
    return 0;
}


void initializeStocks() {
    strcpy(stocks[0].stockName, "TechCorp");
    strcpy(stocks[1].stockName, "HealthInc");
    strcpy(stocks[2].stockName, "EduWorld");
    strcpy(stocks[3].stockName, "AgriPlus");
    strcpy(stocks[4].stockName, "FinanceNow");

    srand(time(NULL));
    for (int i = 0; i < MAX_STOCKS; i++) {
        stocks[i].stockPrice = (rand() % 1000) + 50; 
    }
}


void mainMenu() {
    int choice;
    while (1) {
        printf("\n====================================\n");
        printf("    Virtual Stock Trading System    \n");
        printf("====================================\n");
        printf("1. Login\n");
        printf("2. Register\n");
        printf("3. View Stock Market\n");
        printf("4. Trade Stocks (Buy/Sell)\n");
        printf("5. View Portfolio\n");
        printf("6. Exit\n");
        printf("====================================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                login();
                break;
            case 2:
                registerUser();
                break;
            case 3:
                viewStocks();
                break;
            case 4:
                tradeStocks();
                break;
            case 5:
                viewPortfolio();
                break;
            case 6:
                saveData();
                printf("Exiting the program. Thank you!\n");
                exit(0);
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
}


void login() {
    char username[50], password[50];
    printf("\n--- Login ---\n");
    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            loggedInUser = i;
            printf("Login successful! Welcome, %s.\n", username);
            return;
        }
    }
    printf("Invalid username or password. Please try again.\n");
}


void registerUser() {
    if (userCount >= MAX_USERS) {
        printf("User limit reached. Cannot register more users.\n");
        return;
    }

    char username[50], password[50];
    printf("\n--- Register ---\n");
    printf("Choose a username: ");
    scanf("%s", username);

    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0) {
            printf("Username already exists. Please choose another.\n");
            return;
        }
    }

    printf("Choose a password: ");
    scanf("%s", password);

    strcpy(users[userCount].username, username);
    strcpy(users[userCount].password, password);
    users[userCount].walletBalance = 10000.0; 
    memset(users[userCount].stocksOwned, 0, sizeof(users[userCount].stocksOwned));
    userCount++;
    printf("Registration successful! You can now log in.\n");
}


void viewStocks() {
    printf("\n--- Stock Market ---\n");
    for (int i = 0; i < MAX_STOCKS; i++) {
        printf("%d. %s - $%.2f\n", i + 1, stocks[i].stockName, stocks[i].stockPrice);
    }
}


void tradeStocks() {
    if (loggedInUser == -1) {
        printf("Please login to trade stocks.\n");
        return;
    }

    int choice, quantity;
    printf("\n--- Trade Stocks ---\n");
    viewStocks();
    printf("Enter the stock number to trade (1-%d): ", MAX_STOCKS);
    scanf("%d", &choice);

    if (choice < 1 || choice > MAX_STOCKS) {
        printf("Invalid stock selection.\n");
        return;
    }

    choice--; 
    printf("Enter quantity (positive to buy, negative to sell): ");
    scanf("%d", &quantity);

    float totalCost = stocks[choice].stockPrice * abs(quantity);
    if (quantity > 0) {
        if (users[loggedInUser].walletBalance >= totalCost) {
            users[loggedInUser].walletBalance -= totalCost;
            users[loggedInUser].stocksOwned[choice] += quantity;
            printf("Successfully bought %d shares of %s.\n", quantity, stocks[choice].stockName);
        } else {
            printf("Insufficient balance.\n");
        }
    } else {
        quantity = -quantity; 
        if (users[loggedInUser].stocksOwned[choice] >= quantity) {
            users[loggedInUser].walletBalance += stocks[choice].stockPrice * quantity;
            users[loggedInUser].stocksOwned[choice] -= quantity;
            printf("Successfully sold %d shares of %s.\n", quantity, stocks[choice].stockName);
        } else {
            printf("You don't own enough shares to sell.\n");
        }
    }
}


void viewPortfolio() {
    if (loggedInUser == -1) {
        printf("Please login to view your portfolio.\n");
        return;
    }

    printf("\n--- Portfolio ---\n");
    printf("Wallet Balance: $%.2f\n", users[loggedInUser].walletBalance);
    printf("Stocks Owned:\n");
    for (int i = 0; i < MAX_STOCKS; i++) {
        if (users[loggedInUser].stocksOwned[i] > 0) {
            printf("%s: %d shares\n", stocks[i].stockName, users[loggedInUser].stocksOwned[i]);
        }
    }
}


void saveData() {
    FILE *file = fopen("users.dat", "wb");
    if (!file) {
        printf("Error saving data.\n");
        return;
    }
    fwrite(&userCount, sizeof(int), 1, file);
    fwrite(users, sizeof(User), userCount, file);
    fclose(file);
}


void loadData() {
    FILE *file = fopen("users.dat", "rb");
    if (!file) {
        return; 
    }
    fread(&userCount, sizeof(int), 1, file);
    fread(users, sizeof(User), userCount, file);
    fclose(file);
}   
