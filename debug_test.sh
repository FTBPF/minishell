#!/bin/bash

# Debug Test - Check what's actually happening
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
SHELL_PATH="$SCRIPT_DIR/minishell"
SUPP_PATH="$SCRIPT_DIR/readline.supp"

echo "=== Diagnostic Test ==="
echo ""
echo "Shell path: $SHELL_PATH"
echo "Supp path: $SUPP_PATH"
echo ""

# Check files exist
if [ ! -f "$SHELL_PATH" ]; then
    echo "ERROR: minishell not found!"
    exit 1
fi

if [ ! -f "$SUPP_PATH" ]; then
    echo "ERROR: readline.supp not found!"
    exit 1
fi

echo "Files exist: OK"
echo ""

# Create simple test
TEST_DIR="$SCRIPT_DIR/debug_test_output"
mkdir -p "$TEST_DIR"
cd "$TEST_DIR"

echo "Creating test input..."
cat > test_simple.txt << 'EOF'
echo test
exit
EOF

echo "Running test with Valgrind..."
echo ""

valgrind --leak-check=full --show-leak-kinds=all --suppressions="$SUPP_PATH" "$SHELL_PATH" < test_simple.txt > output.txt 2>&1

echo "=== Test Output ==="
cat output.txt
echo ""
echo "=== End of Output ==="
echo ""

# Check for leak summary
if grep -q "LEAK SUMMARY" output.txt; then
    echo "✓ Found LEAK SUMMARY in output"
    echo ""
    echo "Leak details:"
    grep "definitely lost:" output.txt
    grep "indirectly lost:" output.txt
    grep "possibly lost:" output.txt
    grep "still reachable:" output.txt
else
    echo "✗ No LEAK SUMMARY found in output"
    echo "This means Valgrind didn't run properly or output is redirected wrong"
fi

echo ""
echo "Output saved to: $TEST_DIR/output.txt"
echo "To cleanup: rm -rf $TEST_DIR"
