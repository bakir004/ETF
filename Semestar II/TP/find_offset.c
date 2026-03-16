#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// SECRET FUNCTION - This should never be called normally!
void secret_function() {
    printf("\n");
    printf("╔════════════════════════════════════════╗\n");
    printf("║  *** SECRET FUNCTION ACTIVATED! ***   ║\n");
    printf("║                                        ║\n");
    printf("║  SUCCESS! You found the correct offset!║\n");
    printf("╚════════════════════════════════════════╝\n");
    printf("\n");
    exit(0);
}

// Function that is vulnerable to buffer overflow
void vulnerable_function() {
    char buffer[16];  // Small buffer - only 16 bytes
    
    printf("Secret function address: %p\n", (void*)secret_function);
    printf("Buffer address: %p\n", (void*)buffer);
    
    // Calculate approximate offset (this is a guess, actual may vary)
    void* buffer_addr = (void*)buffer;
    void* approx_ret_addr = buffer_addr + 32;  // Common offset
    printf("Approximate return address location: %p\n", approx_ret_addr);
    printf("Difference: %ld bytes\n", (long)(approx_ret_addr - buffer_addr));
    
    printf("\nEnter your input: ");
    fflush(stdout);
    
    // VULNERABLE: gets() doesn't check buffer bounds
    gets(buffer);
    
    printf("\nBuffer contents (first 50 chars): %.50s\n", buffer);
    printf("Function should return normally...\n");
}

int main() {
    printf("=== Find Offset Tool ===\n\n");
    printf("This tool helps you find the exact offset needed.\n");
    printf("Try different payload lengths to find what triggers the secret function.\n\n");
    
    vulnerable_function();
    
    printf("\nNormal execution completed.\n");
    return 0;
}

