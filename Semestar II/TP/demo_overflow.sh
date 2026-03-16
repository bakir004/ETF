#!/bin/bash

echo "=== Buffer Overflow Demonstration ==="
echo ""
echo "Test 1: Normal input (safe - 5 characters)"
echo "-------------------------------------------"
echo "Hello" | ./buffer_overflow_demo 2>&1 | head -10
echo ""
echo ""
echo "Test 2: Buffer overflow (30 characters - will overflow)"
echo "--------------------------------------------------------"
echo "AAAAAAAAAAAAAAAAAAAAAAAAAAAA" | ./buffer_overflow_demo 2>&1 | head -10
echo ""
echo ""
echo "Note: The program may crash with segmentation fault, which demonstrates"
echo "the buffer overflow vulnerability. The overflow corrupts adjacent memory."

