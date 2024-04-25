#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"
#include <conio.h> // for _getch

#define MAX_LENGTH 100

int main() {
    char pass[MAX_LENGTH];
    char user_name[MAX_LENGTH];

    sqlite3 *db;
    int rc = sqlite3_open("login.db", &db); // Open or create a SQLite database named login.db

    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db)); // Error handling if the database fails to open
        return 0;
    }

user:
    printf("Enter Username: ");
    scanf("%s", user_name);

    // Prepare the SQL statement to select the username and password for the given username
    sqlite3_stmt *stmt;
    char sql[MAX_LENGTH];
    sprintf(sql, "SELECT username, password FROM users WHERE username='%s'", user_name);

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0); // Prepare the SQL statement

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement\n"); // Error handling for SQL preparation failure
        sqlite3_close(db);
        return 0;
    }

    // Execute the SQL statement
    rc = sqlite3_step(stmt);

    if (rc == SQLITE_ROW) {
        const unsigned char *saved_user = sqlite3_column_text(stmt, 0);
        const unsigned char *saved_pass = sqlite3_column_text(stmt, 1);

        printf("Enter your Password: ");
        int i = 0;
        char ch;
        while ((ch = _getch()) != '\r' && i < MAX_LENGTH - 1) {
            pass[i++] = ch;
            printf("*");
        }
        pass[i] = '\0'; // Null-terminate the password string

        if (strcmp(user_name, (const char *)saved_user) == 0 && strcmp(pass, (const char *)saved_pass) == 0) {
            printf("\n\n******LOGIN SUCCESSFUL******\n\n\n");
        } else {
            printf("\n\n******INCORRECT PASSWORD******\nPlease Try again...\n\n");
            goto user;
        }
    } else {
        printf("User not found\n");
        printf("Do you want to sign up? (Y/N): ");
        char answer;
        scanf(" %c", &answer);
        if (answer == 'Y' || answer == 'y') {
            // Sign-up process
            printf("Enter your Password: ");
            int i = 0;
            char ch;
            while ((ch = _getch()) != '\r' && i < MAX_LENGTH - 1) {
                pass[i++] = ch;
                printf("*");
            }
            pass[i] = '\0'; // Null-terminate the password string
            sprintf(sql, "INSERT INTO users (username, password) VALUES ('%s', '%s')", user_name, pass);
            rc = sqlite3_exec(db, sql, 0, 0, 0);
            if (rc != SQLITE_OK) {
                fprintf(stderr, "Error inserting data: %s\n", sqlite3_errmsg(db));
            } else {
                printf("\nSign up successful!\n");
            }
        }
        goto user;
    }

    sqlite3_finalize(stmt); // Finalize the SQL statement
    sqlite3_close(db); // Close the database connection

    return 0;
}
