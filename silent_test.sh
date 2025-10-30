#!/bin/bash

# Silent Minishell Test Suite - Clean output
# Usage: ./silent_test.sh

# Get absolute paths
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
SHELL_PATH="$SCRIPT_DIR/minishell"
SUPP_PATH="$SCRIPT_DIR/readline.supp"
VALGRIND_FLAGS="--leak-check=full --show-leak-kinds=all --suppressions=$SUPP_PATH -q"
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m'

# Check if minishell exists
if [ ! -f "$SHELL_PATH" ]; then
    echo -e "${RED}Error: minishell not found at $SHELL_PATH${NC}"
    echo -e "${YELLOW}Please run this script from the minishell directory${NC}"
    exit 1
fi

clear
echo -e "${BLUE}╔════════════════════════════════════════════════╗${NC}"
echo -e "${BLUE}║                                                ║${NC}"
echo -e "${BLUE}║     MINISHELL COMPREHENSIVE TEST SUITE         ║${NC}"
echo -e "${BLUE}║                                                ║${NC}"
echo -e "${BLUE}╚════════════════════════════════════════════════╝${NC}\n"

# Create test directory
TEST_DIR="$SCRIPT_DIR/minishell_tests"
rm -rf "$TEST_DIR"
mkdir -p "$TEST_DIR"
cd "$TEST_DIR"

TOTAL_TESTS=15
PASSED=0
FAILED=0

# Function to run a test
run_test() {
    local test_num=$1
    local test_name=$2
    local test_file="test${test_num}.txt"
    local output_file="test${test_num}_output.txt"
    
    echo -ne "${CYAN}[Test $test_num/$TOTAL_TESTS] ${test_name}${NC} ... "
    
    # Run test silently
    valgrind $VALGRIND_FLAGS "$SHELL_PATH" < "$test_file" > "$output_file" 2>&1
    
    # Check for leaks
    if grep -q "definitely lost: 0 bytes" "$output_file" && \
       grep -q "indirectly lost: 0 bytes" "$output_file" && \
       grep -q "possibly lost: 0 bytes" "$output_file" && \
       grep -q "still reachable: 0 bytes" "$output_file"; then
        echo -e "${GREEN}✓ PASS${NC}"
        PASSED=$((PASSED + 1))
        return 0
    else
        echo -e "${RED}✗ FAIL${NC}"
        FAILED=$((FAILED + 1))
        return 1
    fi
}

# Test 1: Basic Commands
cat > test1.txt << 'EOF'
echo hello world
pwd
exit
EOF
run_test 1 "Basic commands"

# Test 2: Pipes
cat > test2.txt << 'EOF'
ls | wc -l
echo test | cat | cat
exit
EOF
run_test 2 "Simple pipes"

# Test 3: Redirections
cat > test3.txt << 'EOF'
echo "test" > outfile.txt
cat outfile.txt
cat < outfile.txt
rm outfile.txt
exit
EOF
run_test 3 "Redirections"

# Test 4: Heredoc
cat > test4.txt << 'ENDTEST'
cat << EOF
hello heredoc
test
EOF
exit
ENDTEST
run_test 4 "Heredoc"

# Test 5: Environment Variables
cat > test5.txt << 'EOF'
echo $USER
echo $HOME
echo $PWD
exit
EOF
run_test 5 "Environment variables"

# Test 6: Exit Status
cat > test6.txt << 'EOF'
echo $?
ls
echo $?
exit
EOF
run_test 6 "Exit status"

# Test 7: Export
cat > test7.txt << 'EOF'
export MYVAR=hello
echo $MYVAR
unset MYVAR
exit
EOF
run_test 7 "Export/Unset"

# Test 8: CD
cat > test8.txt << 'EOF'
pwd
cd /tmp
pwd
cd -
pwd
exit
EOF
run_test 8 "CD command"

# Test 9: Quotes
cat > test9.txt << 'EOF'
echo 'single quotes'
echo "double quotes $USER"
exit
EOF
run_test 9 "Quote handling"

# Test 10: Builtins
cat > test10.txt << 'EOF'
echo test
echo -n no newline
echo
pwd
exit
EOF
run_test 10 "Builtins"

# Test 11: Complex Pipes
cat > test11.txt << 'EOF'
cat | cat | ls
echo test | cat | cat
exit
EOF
run_test 11 "Complex pipes"

# Test 12: Heredoc with Pipes
cat > test12.txt << 'ENDTEST'
cat << EOF | grep hello
hello world
test
EOF
exit
ENDTEST
run_test 12 "Heredoc + pipes"

# Test 13: Invalid Commands
cat > test13.txt << 'EOF'
fake_command_xyz
exit
EOF
run_test 13 "Invalid commands"

# Test 14: Edge Cases
cat > test14.txt << 'EOF'
echo     spaces
echo	tabs

exit
EOF
run_test 14 "Edge cases"

# Test 15: Mixed
cat > test15.txt << 'ENDTEST'
export VAR=test
echo $VAR | cat
cat << EOF > out.txt
line
EOF
cat out.txt
rm out.txt
exit
ENDTEST
run_test 15 "Mixed features"

# Summary
echo ""
echo -e "${BLUE}════════════════════════════════════════════════${NC}"
echo -e "${BLUE}                  RESULTS${NC}"
echo -e "${BLUE}════════════════════════════════════════════════${NC}"
echo ""
echo -e "  Total tests:    ${CYAN}$TOTAL_TESTS${NC}"
echo -e "  Passed:         ${GREEN}$PASSED${NC}"
echo -e "  Failed:         ${RED}$FAILED${NC}"
echo ""

if [ $FAILED -eq 0 ]; then
    echo -e "${GREEN}╔════════════════════════════════════════════════╗${NC}"
    echo -e "${GREEN}║                                                ║${NC}"
    echo -e "${GREEN}║          🎉  ALL TESTS PASSED!  🎉            ║${NC}"
    echo -e "${GREEN}║        NO MEMORY LEAKS DETECTED!              ║${NC}"
    echo -e "${GREEN}║                                                ║${NC}"
    echo -e "${GREEN}╚════════════════════════════════════════════════╝${NC}"
else
    echo -e "${RED}╔════════════════════════════════════════════════╗${NC}"
    echo -e "${RED}║                                                ║${NC}"
    echo -e "${RED}║        ⚠️  SOME TESTS FAILED  ⚠️              ║${NC}"
    echo -e "${RED}║                                                ║${NC}"
    echo -e "${RED}╚════════════════════════════════════════════════╝${NC}"
    echo ""
    echo -e "${YELLOW}Check output files in: $TEST_DIR${NC}"
fi

echo ""
echo -e "${CYAN}Detailed logs saved in:${NC} $TEST_DIR"
echo -e "${CYAN}To cleanup:${NC} rm -rf $TEST_DIR"
echo ""

cd "$SCRIPT_DIR"
