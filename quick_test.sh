#!/bin/bash

# Quick Minishell Test
# Usage: ./quick_test.sh

# Get script directory
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
SHELL_PATH="$SCRIPT_DIR/minishell"
SUPP_PATH="$SCRIPT_DIR/readline.supp"

# Check if minishell exists
if [ ! -f "$SHELL_PATH" ]; then
    echo "Error: minishell not found at $SHELL_PATH"
    echo "Please run this script from the minishell directory"
    exit 1
fi

echo "=== MINISHELL QUICK TEST ==="
echo "Using: $SHELL_PATH"
echo ""

# Create a comprehensive test input
cat > test_input.txt << 'EOF'
echo === Basic Commands ===
echo hello world
pwd

echo === Environment Variables ===
echo $USER
echo $HOME
echo $?

echo === Pipes ===
ls | wc -l
echo test | cat | cat

echo === Redirections ===
echo "test" > tmpfile.txt
cat tmpfile.txt
cat < tmpfile.txt
rm tmpfile.txt

echo === Heredoc ===
cat << HEREDOC
line 1
line 2
HEREDOC

echo === Export/Unset ===
export MYVAR=hello
echo $MYVAR
unset MYVAR
echo $MYVAR

echo === Invalid Command ===
fake_command_12345

echo === Complex Pipeline ===
cat | cat | ls

echo === Exit Status ===
ls
echo $?
ls nonexistent
echo $?

echo === All Tests Done ===
exit
EOF

echo "Running tests with Valgrind..."
echo ""

valgrind --leak-check=full --show-leak-kinds=all --suppressions=readline.supp ./minishell < test_input.txt > quickoutput.txt 2>&1

echo ""
echo "Test completed. Check output above for memory leaks."
echo ""
echo "To cleanup: rm test_input.txt"
