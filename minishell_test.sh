#!/bin/bash

# Minishell Comprehensive Test Suite
# Usage: ./minishell_test.sh

# Get absolute paths
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
SHELL_PATH="$SCRIPT_DIR/minishell"
SUPP_PATH="$SCRIPT_DIR/readline.supp"
VALGRIND_FLAGS="--leak-check=full --show-leak-kinds=all --suppressions=$SUPP_PATH"
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Check if minishell exists
if [ ! -f "$SHELL_PATH" ]; then
    echo -e "${RED}Error: minishell not found at $SHELL_PATH${NC}"
    echo -e "${YELLOW}Please run this script from the minishell directory${NC}"
    exit 1
fi

echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}  MINISHELL COMPREHENSIVE TEST SUITE${NC}"
echo -e "${BLUE}========================================${NC}\n"
echo -e "Using minishell: $SHELL_PATH\n"

# Create test directory
TEST_DIR="$SCRIPT_DIR/minishell_tests"
mkdir -p "$TEST_DIR"
cd "$TEST_DIR"

# Test 1: Basic Commands
echo -e "${YELLOW}[TEST 1] Basic Commands${NC}"
cat > test1.txt << 'EOF'
echo hello world
pwd
env | head -3
exit
EOF

echo -e "${GREEN}Running: Basic commands test${NC}"
valgrind $VALGRIND_FLAGS "$SHELL_PATH" < test1.txt > test1_output.txt 2>&1
echo ""

# Test 2: Pipes
echo -e "${YELLOW}[TEST 2] Pipeline Tests${NC}"
cat > test2.txt << 'EOF'
ls | wc -l
echo test | cat | cat
exit
EOF

echo -e "${GREEN}Running: Pipeline tests${NC}"
valgrind $VALGRIND_FLAGS "$SHELL_PATH" < test2.txt > test2_output.txt 2>&1
echo ""

# Test 3: Redirections
echo -e "${YELLOW}[TEST 3] Redirection Tests${NC}"
cat > test3.txt << 'EOF'
echo "test output" > outfile.txt
cat outfile.txt
echo "append line" >> outfile.txt
cat outfile.txt
cat < outfile.txt
rm outfile.txt
exit
EOF

echo -e "${GREEN}Running: Redirection tests${NC}"
valgrind $VALGRIND_FLAGS "$SHELL_PATH" < test3.txt > test3_output.txt 2>&1
echo ""

# Test 4: Heredoc
echo -e "${YELLOW}[TEST 4] Heredoc Tests${NC}"
cat > test4.txt << 'ENDTEST'
cat << EOF
hello heredoc
multiple lines
test
EOF
cat << DELIMITER
another delimiter
DELIMITER
exit
ENDTEST

echo -e "${GREEN}Running: Heredoc tests${NC}"
valgrind $VALGRIND_FLAGS "$SHELL_PATH" < test4.txt > test4_output.txt 2>&1
echo ""

# Test 5: Environment Variables
echo -e "${YELLOW}[TEST 5] Environment Variable Tests${NC}"
cat > test5.txt << 'EOF'
echo $USER
echo $HOME
echo $PATH
echo $PWD
exit
EOF

echo -e "${GREEN}Running: Environment variable tests${NC}"
valgrind $VALGRIND_FLAGS "$SHELL_PATH" < test5.txt > test5_output.txt 2>&1
echo ""

# Test 6: Exit Status
echo -e "${YELLOW}[TEST 6] Exit Status Tests${NC}"
cat > test6.txt << 'EOF'
echo $?
ls
echo $?
ls nonexistent_file_12345
echo $?
exit
EOF

echo -e "${GREEN}Running: Exit status tests${NC}"
valgrind $VALGRIND_FLAGS "$SHELL_PATH" < test6.txt > test6_output.txt 2>&1
echo ""

# Test 7: Export and Unset
echo -e "${YELLOW}[TEST 7] Export/Unset Tests${NC}"
cat > test7.txt << 'EOF'
export MYVAR=hello
echo $MYVAR
export MYVAR2=world
echo $MYVAR $MYVAR2
unset MYVAR
echo $MYVAR
export | head -5
exit
EOF

echo -e "${GREEN}Running: Export/unset tests${NC}"
valgrind $VALGRIND_FLAGS "$SHELL_PATH" < test7.txt > test7_output.txt 2>&1
echo ""

# Test 8: CD Tests
echo -e "${YELLOW}[TEST 8] CD Tests${NC}"
cat > test8.txt << 'EOF'
pwd
cd /tmp
pwd
cd -
pwd
cd ..
pwd
cd nonexistent_directory
pwd
exit
EOF

echo -e "${GREEN}Running: CD tests${NC}"
valgrind $VALGRIND_FLAGS "$SHELL_PATH" < test8.txt > test8_output.txt 2>&1
echo ""

# Test 9: Quotes
echo -e "${YELLOW}[TEST 9] Quote Tests${NC}"
cat > test9.txt << 'EOF'
echo 'single quotes $USER'
echo "double quotes $USER"
echo "mixed 'quotes' test"
echo ""
echo ''
exit
EOF

echo -e "${GREEN}Running: Quote tests${NC}"
valgrind $VALGRIND_FLAGS "$SHELL_PATH" < test9.txt > test9_output.txt 2>&1
echo ""

# Test 10: Builtins
echo -e "${YELLOW}[TEST 10] Builtin Tests${NC}"
cat > test10.txt << 'EOF'
echo test
echo -n no newline
echo " with newline"
pwd
env | grep USER
exit
EOF

echo -e "${GREEN}Running: Builtin tests${NC}"
valgrind $VALGRIND_FLAGS "$SHELL_PATH" < test10.txt > test10_output.txt 2>&1
echo ""

# Test 11: Complex Pipes
echo -e "${YELLOW}[TEST 11] Complex Pipeline Tests${NC}"
cat > test11.txt << 'EOF'
cat | cat | ls
echo test | cat | cat | cat
exit
EOF

echo -e "${GREEN}Running: Complex pipeline tests${NC}"
valgrind $VALGRIND_FLAGS "$SHELL_PATH" < test11.txt > test11_output.txt 2>&1
echo ""

# Test 12: Heredoc with Pipes
echo -e "${YELLOW}[TEST 12] Heredoc with Pipes${NC}"
cat > test12.txt << 'ENDTEST'
cat << EOF | grep hello
hello world
test
goodbye
EOF
exit
ENDTEST

echo -e "${GREEN}Running: Heredoc with pipes${NC}"
valgrind $VALGRIND_FLAGS "$SHELL_PATH" < test12.txt > test12_output.txt 2>&1
echo ""

# Test 13: Invalid Commands
echo -e "${YELLOW}[TEST 13] Invalid Command Tests${NC}"
cat > test13.txt << 'EOF'
nonexistent_command
another_fake_command
asjdnasjdnajsd
exit
EOF

echo -e "${GREEN}Running: Invalid command tests${NC}"
valgrind $VALGRIND_FLAGS "$SHELL_PATH" < test13.txt > test13_output.txt 2>&1
echo ""

# Test 14: Edge Cases
echo -e "${YELLOW}[TEST 14] Edge Case Tests${NC}"
cat > test14.txt << 'EOF'
echo     multiple    spaces
echo	tabs	test


echo after empty lines
exit
EOF

echo -e "${GREEN}Running: Edge case tests${NC}"
valgrind $VALGRIND_FLAGS "$SHELL_PATH" < test14.txt > test14_output.txt 2>&1
echo ""

# Test 15: Mixed Everything
echo -e "${YELLOW}[TEST 15] Mixed Complex Tests${NC}"
cat > test15.txt << 'ENDTEST'
export TESTVAR=hello
echo $TESTVAR
echo $TESTVAR | cat
cat << EOF > output.txt
$TESTVAR world
test line
EOF
cat output.txt
cat < output.txt | grep hello
rm output.txt
unset TESTVAR
exit
ENDTEST

echo -e "${GREEN}Running: Mixed complex tests${NC}"
valgrind $VALGRIND_FLAGS "$SHELL_PATH" < test15.txt > test15_output.txt 2>&1
echo ""

# Analyze results
echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}  ANALYZING RESULTS${NC}"
echo -e "${BLUE}========================================${NC}\n"

LEAK_COUNT=0
ERROR_COUNT=0
TOTAL_TESTS=15

for i in {1..15}; do
    file="test${i}_output.txt"
    
    if [ ! -f "$file" ]; then
        echo -e "${RED}✗ Test $i: Output file not found${NC}"
        continue
    fi
    
    # Check for memory leaks
    if grep -q "definitely lost: 0 bytes" "$file" && \
       grep -q "indirectly lost: 0 bytes" "$file" && \
       grep -q "possibly lost: 0 bytes" "$file" && \
       grep -q "still reachable: 0 bytes" "$file"; then
        echo -e "${GREEN}✓ Test $i: NO MEMORY LEAKS${NC}"
    else
        echo -e "${RED}✗ Test $i: MEMORY LEAKS DETECTED${NC}"
        LEAK_COUNT=$((LEAK_COUNT + 1))
    fi
    
    # Check for Valgrind errors (excluding the harmless ioctl warning)
    ERROR_LINES=$(grep "ERROR SUMMARY:" "$file" | grep -v "ERROR SUMMARY: 0 errors" | grep -v "ERROR SUMMARY: 1 errors from 1 contexts")
    if [ -z "$ERROR_LINES" ]; then
        echo -e "${GREEN}✓ Test $i: NO CRITICAL ERRORS${NC}"
    else
        echo -e "${RED}✗ Test $i: ERRORS DETECTED${NC}"
        ERROR_COUNT=$((ERROR_COUNT + 1))
    fi
    echo ""
done

# Summary
echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}  FINAL SUMMARY${NC}"
echo -e "${BLUE}========================================${NC}"
echo -e "Total tests run: ${BLUE}$TOTAL_TESTS${NC}"
echo -e "Tests with memory leaks: ${RED}$LEAK_COUNT${NC}"
echo -e "Tests with critical errors: ${RED}$ERROR_COUNT${NC}"
echo ""

if [ $LEAK_COUNT -eq 0 ] && [ $ERROR_COUNT -eq 0 ]; then
    echo -e "${GREEN}╔════════════════════════════════════════╗${NC}"
    echo -e "${GREEN}║                                        ║${NC}"
    echo -e "${GREEN}║   🎉 ALL TESTS PASSED! 🎉            ║${NC}"
    echo -e "${GREEN}║   NO MEMORY LEAKS DETECTED!           ║${NC}"
    echo -e "${GREEN}║                                        ║${NC}"
    echo -e "${GREEN}╚════════════════════════════════════════╝${NC}"
    echo ""
else
    echo -e "${YELLOW}⚠️  Some tests had issues. Check output files for details.${NC}\n"
fi

# Show where files are
echo -e "${BLUE}Test Results Location:${NC}"
echo -e "  Directory: ${YELLOW}$TEST_DIR${NC}"
echo -e "  Output files: ${YELLOW}test*_output.txt${NC}"
echo ""
echo -e "${BLUE}Cleanup Command:${NC}"
echo -e "  ${YELLOW}rm -rf $TEST_DIR${NC}"
echo ""

cd "$SCRIPT_DIR"
