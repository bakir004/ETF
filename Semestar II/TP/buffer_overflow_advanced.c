#include <stdio.h>
#include <string.h>

// Global variable to protect
int authenticated = 0;
char password[32] = "correct_password_123";

// Vulnerable function that checks authentication
void login() {
    char username[16];  // Small buffer
    
    printf("=== Login System ===\n");
    printf("Authenticated flag address: %p\n", (void*)&authenticated);
    printf("Username buffer address: %p\n", (void*)username);
    printf("Password address: %p\n", (void*)password);
    printf("\nEnter username: ");
    
    // VULNERABLE: No bounds checking
    gets(username);
    
    printf("\nUsername: %s\n", username);
    printf("Authenticated: %d\n", authenticated);
    
    if (authenticated) {
        printf("\n*** ACCESS GRANTED ***\n");
        printf("Password: %s\n", password);
    } else {
        printf("\n*** ACCESS DENIED ***\n");
    }
}

int main() {
    printf("=== Advanced Buffer Overflow Demo ===\n\n");
    printf("Goal: Overflow the username buffer to modify the 'authenticated' variable\n");
    printf("The username buffer is 16 bytes.\n");
    printf("You need to overflow it to reach the 'authenticated' variable.\n\n");
    
    login();
    
    return 0;
}

