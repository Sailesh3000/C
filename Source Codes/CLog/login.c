#include <stdio.h>
#include <string.h>

int main() {
    char pass[20];
    char saved_pass[20];
start:
    printf("Enter your Password: ");
    fgets(pass, sizeof(pass), stdin);
    pass[strcspn(pass, "\n")] = 0; // Remove trailing newline if present

    printf("\nYou Entered: %s\n", pass);

    FILE *ptr;
    ptr = fopen("password.txt", "r");

    if (ptr == NULL) {
        printf("Error opening the file.");
        return 1;
    }

    fscanf(ptr, "%19s", saved_pass);
    fclose(ptr);

    printf("Password: %s ", saved_pass);

    int l = strcmp(pass, saved_pass);
    if (l == 0) {
        printf("\n\n******LOGIN SUCCESSFUL******\n\n\n");
    } else {
        printf("\n\n******INCORRECT PASSWORD******\nPlease Try again...\n\n");
        goto start;
    }
    return 0;
}
